// SPDX-FileCopyrightText: 2026 Robin Christ
// SPDX-License-Identifier: MPL-2.0
import type { Document, Standard } from './schema'

// Turns imported files and pasted text into either a TransceiverTool JSON document or
// programming bytes for the C++ decoder. Only byte extraction happens here; all binary
// interpretation stays in the WASM core.

export type ImportFormat = 'auto' | 'binary' | 'json' | 'mlxlink' | 'hex' | 'base64'
export const formatLabels: Record<ImportFormat, string> = {
  auto: 'Detect automatically',
  binary: 'Binary EEPROM dump',
  json: 'TransceiverTool JSON',
  mlxlink: 'mlxlink / mstlink JSON',
  hex: 'Hex bytes',
  base64: 'Base64',
}

export interface Programming {
  standard: Standard
  /** 128 bytes (SFP A0h or QSFP upper page 00h) or 256 bytes (full QSFP page 00h). */
  bytes: number[]
  lowerBytes?: number[]
}
export type ImportResult =
  | { kind: 'document'; document: Document }
  | ({ kind: 'bytes'; format: Exclude<ImportFormat, 'auto' | 'json'> } & Programming)

export function readInput(
  input: Uint8Array | string,
  format: ImportFormat,
  selected: Standard,
  name = '',
): ImportResult {
  if (typeof input !== 'string') {
    if (input.length === 0) throw new Error('The file is empty.')
    if (format === 'binary') return bytesResult('binary', [...input], selected)
    const text = decodeText(input)
    if (text === undefined) {
      if (format === 'auto' && !name.toLowerCase().endsWith('.json'))
        return bytesResult('binary', [...input], selected)
      throw new Error(`The file is not text. Choose “${formatLabels.binary}” to read it as bytes.`)
    }
    if (format === 'auto' && !name.toLowerCase().endsWith('.json')) {
      try {
        return detectText(text, selected, false)
      } catch (e) {
        // A binary dump can consist of printable bytes only; fall back to it by length.
        if ((input.length === 128 || input.length === 256) && !/^\s*[{[]/.test(text))
          return bytesResult('binary', [...input], selected)
        throw e
      }
    }
    input = text
  }
  if (!input.trim()) throw new Error('No data found.')
  switch (format) {
    case 'binary':
      throw new Error('Pasted text cannot be read as a binary dump. Choose a text format.')
    case 'json':
      return { kind: 'document', document: parseJsonObject(input) as Document }
    case 'mlxlink':
      return { kind: 'bytes', format, ...parseMlxlink(parseJsonObject(input), selected) }
    case 'hex':
      return bytesResult('hex', parseHex(input), selected)
    case 'base64':
      return bytesResult('base64', parseBase64(input), selected)
  }
  return detectText(input, selected, name.toLowerCase().endsWith('.json'))
}

function detectText(text: string, selected: Standard, jsonName: boolean): ImportResult {
  const trimmed = text.trim()
  if (jsonName || /^[{[]\s*("|$)/.test(trimmed)) return fromJson(parseJsonObject(trimmed), selected)
  if (/^[{[]/.test(trimmed)) {
    let value: unknown
    try {
      value = JSON.parse(trimmed)
    } catch {
      // Not JSON: likely a C or Python array of hex bytes.
    }
    if (value !== undefined) return fromJson(jsonObject(value), selected)
  }
  const attempt = (parse: () => number[]) => {
    try {
      return parse()
    } catch (e) {
      return e as Error
    }
  }
  const hex = attempt(() => parseHex(text))
  if (Array.isArray(hex)) return bytesResult('hex', hex, selected)
  const base64 = attempt(() => parseBase64(text))
  // Most plain words are valid Base64, so only a plausible length counts as a match.
  if (Array.isArray(base64) && (base64.length === 128 || base64.length === 256))
    return bytesResult('base64', base64, selected)
  const asBase64 = Array.isArray(base64)
    ? ` As Base64: ${base64.length} bytes instead of 128 or 256.`
    : ''
  throw new Error(
    `The data was not recognized as hex bytes, Base64, or JSON. As hex bytes: ${hex.message}${asBase64}`,
  )
}

function fromJson(value: Record<string, unknown>, selected: Standard): ImportResult {
  if (!('Type' in value) && hasMlxlinkBytes(value))
    return { kind: 'bytes', format: 'mlxlink', ...parseMlxlink(value, selected) }
  return { kind: 'document', document: value as Document }
}

function parseJsonObject(text: string) {
  return jsonObject(JSON.parse(text.trim()))
}
function jsonObject(value: unknown): Record<string, unknown> {
  if (typeof value !== 'object' || value === null || Array.isArray(value))
    throw new Error(
      'Expected a JSON object: a TransceiverTool configuration or mlxlink / mstlink output. For a list of hex values, choose Hex bytes.',
    )
  return value as Record<string, unknown>
}

function bytesResult(
  format: Exclude<ImportFormat, 'auto' | 'json'>,
  bytes: number[],
  selected: Standard,
): ImportResult {
  if (bytes.length === 256)
    return { kind: 'bytes', format, standard: '8636', bytes, lowerBytes: bytes.slice(0, 128) }
  if (bytes.length === 128) return { kind: 'bytes', format, standard: selected, bytes }
  throw new Error(
    `Expected exactly 128 bytes (SFP or QSFP upper page) or 256 bytes (QSFP full page), but found ${bytes.length}.`,
  )
}

/** Decodes a file as text, or returns undefined when it contains binary data. */
export function decodeText(data: Uint8Array): string | undefined {
  let text: string
  try {
    // PowerShell 5 redirects write UTF-16LE with a byte order mark.
    text =
      data[0] === 0xff && data[1] === 0xfe
        ? new TextDecoder('utf-16le', { fatal: true }).decode(data)
        : new TextDecoder('utf-8', { fatal: true }).decode(data)
  } catch {
    return undefined
  }
  return /[\x00-\x08\x0b\x0c\x0e-\x1f\x7f]/.test(text) ? undefined : text
}

export function parseBase64(text: string): number[] {
  const data = text
    .replace(/^\s*data:[^,]*,/, '')
    .replace(/^-----(BEGIN|END) [^-\n]*-----$/gm, '')
    .replace(/\s+/g, '')
    .replace(/-/g, '+')
    .replace(/_/g, '/')
  if (!data) throw new Error('No Base64 data found.')
  if (!/^[A-Za-z0-9+/]+={0,2}$/.test(data))
    throw new Error('Base64 may contain only A–Z, a–z, 0–9, + and / (or - and _), and = padding.')
  const body = data.replace(/=+$/, '')
  if (body.length % 4 === 1 || (body.length !== data.length && data.length % 4 !== 0))
    throw new Error('The Base64 data is truncated or incorrectly padded.')
  const binary = atob(body + '='.repeat((4 - (body.length % 4)) % 4))
  return Array.from(binary, (c) => c.charCodeAt(0))
}

interface HexRow {
  line: number
  label?: string
  star?: boolean
  tokens: string[]
}
// Column header rows of i2cdump and PowerShell Format-Hex.
const columnHeaders = new Set([
  '0 1 2 3 4 5 6 7 8 9 a b c d e f',
  '0 1 2 3 4 5 6 7 8 9 a b c d e f 0123456789abcdef',
  '00 01 02 03 04 05 06 07 08 09 0a 0b 0c 0d 0e 0f',
])
const hexGroup = /^(?:0x[0-9a-f]{1,2}|(?:[0-9a-f]{2})+)$/i

function tokenBytes(token: string): number[] | undefined {
  if (!hexGroup.test(token)) return undefined
  const digits = token.replace(/^0x/i, '')
  if (digits.length <= 2) return [parseInt(digits, 16)]
  return Array.from({ length: digits.length / 2 }, (_, i) =>
    parseInt(digits.slice(i * 2, i * 2 + 2), 16),
  )
}

/**
 * Reads hex bytes in common notations: separated or contiguous digits, 0x prefixes,
 * C/JSON arrays, \x escapes, colon- or dash-separated bytes, and dumps with offset
 * columns (hexdump -C, xxd, od -tx1, i2cdump, ethtool -m hex, PowerShell Format-Hex).
 * Offset columns must be contiguous; `*` lines repeat the previous line up to the
 * next offset, as hexdump and od print them.
 */
export function parseHex(text: string): number[] {
  let source = text.replace(/\r\n?/g, '\n').replace(/\/\*[\s\S]*?\*\//g, ' ')
  // Array literals: `{0x11, 0x07}`, `uint8_t eeprom[128] = { ... };`, `[0x11, 0x07]`.
  const array = /^\s*(?:[\w\s[\]*]*=\s*)?([{[])/.exec(source)
  if (array) {
    const close = source.lastIndexOf(array[1] === '{' ? '}' : ']')
    if (close < 0) throw new Error(`The array has no closing “${array[1] === '{' ? '}' : ']'}”.`)
    source = source.slice(array[0].length, close)
  }
  if (/\\x[0-9a-f]{2}/i.test(source))
    source = source
      .replace(/\bb(?=["'])/g, '')
      .replace(/["']/g, ' ')
      .replace(/\\x/gi, ' 0x')

  const rows: HexRow[] = []
  source.split('\n').forEach((raw, index) => {
    const line = raw
      .replace(/\s\|.*$/, '') // hexdump -C ASCII column
      .replace(/(#|\/\/).*$/, '')
      .trim()
    if (
      !line ||
      /^[-=\s]+$/.test(line) ||
      /^(offset\b|label:)/i.test(line) ||
      columnHeaders.has(line.toLowerCase().split(/\s+/).join(' '))
    )
      return
    if (line === '*') {
      rows.push({ line: index + 1, star: true, tokens: [] })
      return
    }
    const labelled = /^(?:0x)?([0-9a-f]+):(?:\s+(.*))?$/i.exec(line)
    if (labelled)
      rows.push({ line: index + 1, label: labelled[1], tokens: split(labelled[2] ?? '') })
    else if (split(line).length) rows.push({ line: index + 1, tokens: split(line) })
  })
  if (!rows.some((row) => row.tokens.length)) throw new Error('No hex bytes found.')

  if (rows.some((row) => row.label !== undefined)) {
    const unlabelled = rows.find((row) => !row.star && row.label === undefined)
    if (unlabelled)
      throw new Error(`Line ${unlabelled.line} has no offset, unlike the other lines.`)
    return fromOffsets(rows)
  }
  // Offsets without a colon (hexdump -C, od -tx1, Format-Hex): a longer first column
  // followed by single bytes. A lone final offset marks the end of the dump.
  const data = rows.filter((row) => !row.star)
  const width = data[0].tokens[0].length
  if (
    width >= 4 &&
    width <= 16 &&
    data.every(
      (row, i) =>
        /^[0-9a-f]+$/i.test(row.tokens[0]) &&
        row.tokens[0].length === width &&
        (row.tokens.length > 1 || i === data.length - 1),
    )
  ) {
    const next = data.map((row) => row.tokens[1]).filter((token) => token !== undefined)
    if (width === 7 && next.every((token) => /^[0-9a-f]{4}$/i.test(token)))
      throw new Error(
        'This looks like 16-bit word output from hexdump or od, which swaps byte order. Use hexdump -C, xxd, or od -An -tx1.',
      )
    if (next.every((token) => /^[0-9a-f]{2}$/i.test(token)))
      return fromOffsets(
        rows.map((row) =>
          row.star ? row : { ...row, label: row.tokens[0], tokens: row.tokens.slice(1) },
        ),
      )
  }
  const star = rows.find((row) => row.star)
  if (star) throw new Error(`Line ${star.line}: “*” repeats lines only in dumps with offsets.`)
  return rows.flatMap((row) =>
    row.tokens.flatMap((token) => {
      const bytes = tokenBytes(token)
      if (!bytes) throw new Error(notHex(row.line, token))
      return bytes
    }),
  )
}

function notHex(line: number, token: string) {
  const hint = /^0x[0-9a-f]{3,}$/i.test(token)
    ? ' (0x values must be single bytes)'
    : /^[0-9a-f]+$/i.test(token)
      ? ' (odd number of digits)'
      : ''
  return `Line ${line}: “${token}” is not a hex byte${hint}.`
}

function split(text: string) {
  return text
    .split(/[\s,;]+/)
    .flatMap((token) =>
      /^[0-9a-f]{2}([:-][0-9a-f]{2})+$/i.test(token) ? token.split(/[:-]/) : [token],
    )
    .filter(Boolean)
}

function fromOffsets(rows: HexRow[]): number[] {
  // Dumps may append an ASCII column. Row widths follow from the offsets, so read each
  // line's leading hex groups and keep only as many bytes as the offsets allow.
  const lines = rows.map((row) => {
    const bytes: number[] = []
    let stop: string | undefined
    for (const token of row.tokens) {
      const value = tokenBytes(token)
      if (!value) {
        stop = token
        break
      }
      bytes.push(...value)
    }
    return { ...row, bytes, stop }
  })
  let failure = ''
  const radixes = lines.every((row) => row.star || /^[0-7]+$/.test(row.label!)) ? [16, 8] : [16]
  for (const radix of radixes) {
    const result = expand(lines, radix)
    if (Array.isArray(result)) return result
    failure ||= result
  }
  throw new Error(failure)
}

function expand(
  lines: (HexRow & { bytes: number[]; stop?: string })[],
  radix: number,
): number[] | string {
  const offsets = lines.map((row) => (row.star ? NaN : parseInt(row.label!, radix)))
  const counts = new Map<number, number>()
  lines.forEach((row, i) => {
    const next = lines[i + 1]
    if (!row.star && next && !next.star) {
      const delta = offsets[i + 1] - offsets[i]
      counts.set(delta, (counts.get(delta) ?? 0) + 1)
    }
  })
  const width = [...counts].sort((a, b) => b[1] - a[1])[0]?.[0]
  const base = offsets.find((offset) => !Number.isNaN(offset))!
  const out: number[] = []
  let previous: number[] | undefined
  let repeat = false
  for (const [i, row] of lines.entries()) {
    if (row.star) {
      if (!previous || repeat) return `Line ${row.line}: “*” must follow a line of bytes.`
      repeat = true
      continue
    }
    const offset = offsets[i] - base
    if (repeat) {
      const gap = offset - out.length
      if (gap <= 0 || gap % previous!.length)
        return `Line ${row.line}: offset ${row.label} does not continue the repeated line.`
      for (let n = 0; n < gap / previous!.length; n++) out.push(...previous!)
      repeat = false
    }
    if (offset !== out.length)
      return `Line ${row.line}: offset ${row.label} does not follow the previous line.`
    const next = lines[i + 1]
    if (next && !next.star && offsets[i + 1] <= offsets[i])
      return `Line ${next.line}: offset ${next.label} does not follow the previous line.`
    const expected = next && !next.star ? offsets[i + 1] - offsets[i] : (width ?? row.bytes.length)
    if (row.bytes.length < expected && (row.stop || (next && !next.star)))
      return row.stop && row.bytes.length < (width ?? expected)
        ? notHex(row.line, row.stop)
        : `Line ${row.line}: expected ${expected} bytes before the next offset, found ${row.bytes.length}.`
    const bytes = row.bytes.slice(0, expected)
    out.push(...bytes)
    if (bytes.length) previous = bytes
  }
  if (repeat) return 'The dump ends with “*”; include the final offset line.'
  return out
}

const byteKey = /^page\[(\d+)\]\.byte\[(\d+)\]$/i
const qsfpIdentifiers = new Set([0x0c, 0x0d, 0x11])
const sfpIdentifiers = new Set([0x03])

function hasMlxlinkBytes(value: unknown): boolean {
  if (typeof value !== 'object' || value === null) return false
  return Object.entries(value).some(([key, child]) => byteKey.test(key) || hasMlxlinkBytes(child))
}

/**
 * Reads `mlxlink --cable --read ... --json` (or mstlink) output. Keys like
 * `page[0].Byte[130]` address EEPROM offsets within a page, so the module's identifier
 * decides whether offsets 0–127 hold an SFP A0h page or a QSFP lower page.
 */
export function parseMlxlink(value: Record<string, unknown>, selected: Standard): Programming {
  const status = value.status as { code?: unknown; message?: unknown } | undefined
  if (typeof status?.code === 'number' && status.code !== 0)
    throw new Error(`mlxlink reported an error: ${String(status.message ?? `code ${status.code}`)}`)
  const pages = new Map<number, Map<number, number>>()
  const visit = (node: unknown) => {
    if (typeof node !== 'object' || node === null) return
    for (const [key, child] of Object.entries(node)) {
      const match = byteKey.exec(key)
      if (!match) {
        visit(child)
        continue
      }
      const [page, start] = [Number(match[1]), Number(match[2])]
      const values =
        typeof child === 'object' && child !== null && 'values' in child
          ? (child as { values: unknown }).values
          : child
      const list = Array.isArray(values) ? values : [values]
      const bytes = pages.get(page) ?? new Map<number, number>()
      pages.set(page, bytes)
      list.forEach((item, i) => {
        const byte =
          typeof item === 'number'
            ? item
            : typeof item === 'string' && /^0x[0-9a-f]{1,2}$/i.test(item.trim())
              ? parseInt(item.trim(), 16)
              : NaN
        if (!Number.isInteger(byte) || byte < 0 || byte > 255)
          throw new Error(`${key}: ${JSON.stringify(item)} is not a byte value such as "0x1f".`)
        const offset = start + i
        if (offset > 255) throw new Error(`${key}: offset ${offset} is outside the 256-byte page.`)
        if (bytes.has(offset) && bytes.get(offset) !== byte)
          throw new Error(`page[${page}].Byte[${offset}] has two different values.`)
        bytes.set(offset, byte)
      })
    }
  }
  visit(value)
  if (!pages.size) throw new Error('The JSON contains no page[…].Byte[…] cable read output.')
  const number = pages.size === 1 ? [...pages.keys()][0] : [0, 0xa0].find((p) => pages.has(p))
  if (number === undefined)
    throw new Error(
      `The output contains pages ${[...pages.keys()].join(', ')}. Include page 0 in the read.`,
    )
  const page = pages.get(number)!
  const range = (start: number) => Array.from({ length: 128 }, (_, i) => page.get(start + i))
  const missing = (bytes: (number | undefined)[], start: number) => {
    const absent = bytes.flatMap((b, i) => (b === undefined ? [start + i] : []))
    return absent.length > 8
      ? `${absent.slice(0, 8).join(', ')} and ${absent.length - 8} more`
      : absent.join(', ')
  }
  const identifier = page.get(0)
  const qsfp =
    identifier === undefined ||
    qsfpIdentifiers.has(identifier) ||
    (!sfpIdentifiers.has(identifier) && selected === '8636')
  if (qsfp) {
    const upper = range(128)
    if (upper.every((b) => b === undefined) && range(0).some((b) => b !== undefined))
      throw new Error(
        `The output contains only bytes 0–127 of page ${number}, the QSFP lower page. The editable configuration is upper page 00h: read page 0, offsets 128–255, as well.`,
      )
    if (upper.includes(undefined))
      throw new Error(`Page ${number} is missing upper page bytes ${missing(upper, 128)}.`)
    const lower = range(0)
    return lower.includes(undefined)
      ? { standard: '8636', bytes: upper as number[] }
      : {
          standard: '8636',
          bytes: [...lower, ...upper] as number[],
          lowerBytes: lower as number[],
        }
  }
  const bytes = range(0)
  if (bytes.includes(undefined))
    throw new Error(`Page ${number} is missing SFP A0h bytes ${missing(bytes, 0)}.`)
  return { standard: '8472', bytes: bytes as number[] }
}
