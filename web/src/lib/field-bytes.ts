// SPDX-FileCopyrightText: 2026 Robin Christ
// SPDX-License-Identifier: MPL-2.0
import layout from './byte-layout.json'
import {
  resolve,
  selectedBranch,
  unpointer,
  type Schema,
  type Value,
  type Document,
  type Standard,
} from './schema'
import { convert, sample, type CodecResult } from './codec'
export type BytePart = [offset: number, mask: number]
export const hex = (bytes: number[]) =>
  bytes.map((b) => b.toString(16).padStart(2, '0').toUpperCase()).join(' ')
export function getAt(value: Value | undefined, path: string): Value | undefined {
  for (const key of path.split('/').slice(1).map(unpointer))
    value = value && typeof value === 'object' && !Array.isArray(value) ? value[key] : undefined
  return value
}
export function setAt(document: Document, path: string, value: Value): Document {
  const result = structuredClone(document)
  const keys = path.split('/').slice(1).map(unpointer)
  let parent = result
  for (const key of keys.slice(0, -1)) parent = parent[key] as Document
  parent[keys.at(-1)!] = value
  return result
}
// Byte ownership follows the native assemblers. Shared bytes have explicit masks;
// nested bit/byte labels come from the public schemas, with physical-property aliases below.
export function fieldLayout(standard: Standard, path: string): BytePart[] {
  const [top, ...children] = path.split('/').slice(1).map(unpointer)
  let parts = ((layout[standard] as Record<string, number[][]>)[top] ?? []).map(
    ([at, mask]) => [at, mask] as BytePart,
  )
  for (const name of children) {
    if (name === 'Type' || name === 'Vendor Name') return [] // JSON discriminator / informational OUI lookup
    const byte = name.match(/\(Bytes?\s+(\d+)(?:\s*-\s*(\d+))?/i)
    if (byte)
      parts = parts.filter(([at]) => at >= Number(byte[1]) && at <= Number(byte[2] ?? byte[1]))
    // Legacy JSON key says bit 1; parser/assembler and the member name use bit 2.
    // Preserve the public key while following physical ownership (see docs/byte-layout.md).
    if (
      standard === '8636' &&
      top === 'Fibre Channel Transmission Media' &&
      name === 'Multi-mode 50 um (M5) compliant (Bit 1)'
    )
      return [[137, 4]]
    const bit = name.match(/Bits?\s+(\d+)(?:\s*-\s*(\d+))?/i)
    if (bit) {
      const a = Number(bit[1]),
        b = Number(bit[2] ?? bit[1])
      const mask = ((1 << (Math.abs(a - b) + 1)) - 1) << Math.min(a, b)
      parts = parts.map(([at, owned]) => [at, owned & mask])
    }
    let positions: number[] | undefined
    if (standard === '8472' && top === 'Link Length') {
      const at = /12\.9|\(SMF\) \[km\]/.test(name)
        ? 14
        : /25\.78|\(SMF\) \[m\]/.test(name)
          ? 15
          : /OM2/.test(name)
            ? 16
            : /OM1/.test(name)
              ? 17
              : /Additional|OM3/.test(name)
                ? 19
                : /OM4|\(Copper\)/.test(name)
                  ? 18
                  : undefined
      if (at !== undefined) positions = [at]
      if (/Multiplier/.test(name)) parts = [[19, 192]]
      if (/Base Value/.test(name)) parts = [[19, 63]]
    }
    if (
      standard === '8472' &&
      top === 'Wavelength Or Cable Specification Compliance' &&
      /Wavelength/.test(name)
    )
      positions = [60, 61]
    if (standard === '8472' && top === 'Extended Signaling Rate') {
      if (/Upper|Extended Signaling Rate/.test(name)) positions = [66]
      else if (/Lower|Signaling rate margin/.test(name)) positions = [67]
    }
    if (standard === '8636' && top === 'Copper or Fibre Properties') {
      if (/25\.78|OM1/.test(name)) positions = [145]
      else if (/passive copper|OM4/.test(name)) positions = [146]
      else if (/Tolerance/.test(name)) positions = [188, 189]
      else if (/Wavelength/.test(name)) positions = [186, 187]
      else if (/2\.5 GHz/.test(name)) positions = [186]
      else if (/5\.0 GHz/.test(name)) positions = [187]
      else if (/7\.0 GHz/.test(name)) positions = [188]
      else if (/12\.9 GHz/.test(name)) positions = [189]
    }
    if (positions) parts = parts.filter(([at]) => positions.includes(at))
  }
  return parts.filter(([, mask]) => mask !== 0)
}
export function fieldHex(result: CodecResult, parts: BytePart[]) {
  return hex(parts.map(([at, mask]) => result.bytes[at] & mask))
}
export function parseHex(text: string, parts: BytePart[]) {
  const compact = text.trim().replace(/0x/gi, '').replace(/\s+/g, '')
  if (!/^[0-9a-f]+$/i.test(compact) || compact.length !== parts.length * 2)
    throw new Error(
      `Enter exactly ${parts.length} hexadecimal byte${parts.length === 1 ? '' : 's'}, for example ${hex(parts.map(() => 0))}.`,
    )
  const values = compact.match(/../g)!.map((v) => parseInt(v, 16))
  if (values.some((v, i) => (v & ~parts[i][1]) !== 0))
    throw new Error(
      `Only this field’s bits may be set. Allowed masks: ${hex(parts.map(([, m]) => m))}.`,
    )
  return values
}
function mergeChanged(original: Value, before: Value, after: Value): Value {
  if (JSON.stringify(before) === JSON.stringify(after)) return original
  if (
    before &&
    after &&
    original &&
    typeof before === 'object' &&
    typeof after === 'object' &&
    typeof original === 'object' &&
    !Array.isArray(before) &&
    !Array.isArray(after) &&
    !Array.isArray(original)
  ) {
    const next = { ...original }
    for (const key of Object.keys(before)) if (!(key in after)) delete next[key]
    for (const [key, value] of Object.entries(after))
      next[key] =
        key in before && key in original ? mergeChanged(original[key], before[key], value) : value
    return next
  }
  return after
}
export async function editHex(
  document: Document,
  result: CodecResult,
  standard: Standard,
  fiber: boolean,
  path: string,
  text: string,
  wholeByte?: number,
  representation?: 'named' | 'raw',
) {
  const owned = fieldLayout(standard, path)
  if (wholeByte !== undefined && !owned.some(([offset]) => offset === wholeByte))
    throw new Error('This byte is outside the selected field.')
  const parts: BytePart[] = wholeByte === undefined ? owned : [[wholeByte, 255]]
  const values = parseHex(text, parts)
  const bytes = [...result.bytes]
  parts.forEach(([at, mask], i) => {
    bytes[at] = (bytes[at] & ~mask) | values[i]
  })
  const before = await convert({ operation: 'decode', standard, fiber, bytes: result.bytes })
  const after = await convert({ operation: 'decode', standard, fiber, bytes })
  let next = mergeChanged(document, before.document, after.document) as Document
  if (representation) {
    if (parts.length !== 1 || parts[0][1] !== 255)
      throw new Error('A named byte must own one full byte.')
    next = setAt(
      next,
      path,
      representation === 'raw'
        ? { byteValue: `0x${hex([bytes[parts[0][0]]])}` }
        : getAt(after.document, path)!,
    )
  }
  for (const [name, at] of [
    ['CC_BASE', standard === '8472' ? 63 : 191],
    ['CC_EXT', standard === '8472' ? 95 : 223],
  ] as const)
    next[name] = parts.some(([offset]) => offset === at)
      ? { byteValue: `0x${hex([bytes[at]])}` }
      : document[name]
  // Native assembly verifies that the new JSON preserves every edited bit. Automatic
  // checksum bytes may differ by design; explicit checksum edits stay explicit.
  const check = await convert({
    operation: 'encode',
    standard,
    fiber,
    document: next,
    ...(standard === '8636' ? { lowerBytes: bytes.slice(0, 128) } : {}),
  })
  const automatic = new Set([
    ...(next.CC_BASE === 'auto' ? [standard === '8472' ? 63 : 191] : []),
    ...(next.CC_EXT === 'auto' ? [standard === '8472' ? 95 : 223] : []),
  ])
  if (bytes.some((b, i) => !automatic.has(i) && check.bytes[i] !== b))
    throw new Error(
      'This edit could not be represented without changing other bytes. The configuration was left unchanged.',
    )
  return next
}
export async function convertFieldFormat(
  document: Document,
  result: CodecResult,
  standard: Standard,
  fiber: boolean,
  path: string,
  schema: Schema,
  root: Schema,
): Promise<Value | undefined> {
  const s = resolve(schema, root),
    parts = fieldLayout(standard, path)
  if (!parts.length) return undefined
  const candidates: (Value | undefined)[] = [getAt(result.document, path)]
  if (path === '/Vendor OUI' && s.type === 'string')
    candidates.unshift(fieldHex(result, parts).replaceAll(' ', ':'))
  if (s.properties?.byteValue && parts.length === 1)
    candidates.unshift({ byteValue: `0x${fieldHex(result, parts)}` })
  if (s.properties?.Type?.const === 'Base64')
    candidates.unshift({
      Type: 'Base64',
      Value: btoa(String.fromCharCode(...parts.map(([at, mask]) => result.bytes[at] & mask))),
    })
  for (const candidate of candidates) {
    if (candidate === undefined || !matchesFormat(s, candidate, root)) continue
    try {
      const next = await convert({
        operation: 'encode',
        standard,
        fiber,
        document: setAt(document, path, candidate),
        ...(standard === '8636' ? { lowerBytes: result.bytes.slice(0, 128) } : {}),
      })
      if (next.bytes.every((b, i) => b === result.bytes[i])) return candidate
    } catch {
      /* An unrepresentable format leaves the current value intact. */
    }
  }
  return undefined
}
function matchesFormat(s: Schema, value: Value, root: Schema): boolean {
  if (s.anyOf)
    return matchesFormat(resolve(s.anyOf[selectedBranch(s.anyOf, value, root)], root), value, root)
  if (s.const !== undefined) return value === s.const
  if (s.enum) return s.enum.includes(value)
  if (s.type === 'object')
    return (
      !!value &&
      typeof value === 'object' &&
      !Array.isArray(value) &&
      (s.required ?? []).every((k) => k in value) &&
      Object.entries(s.properties ?? {}).every(
        ([k, c]) => !(k in value) || matchesFormat(resolve(c, root), value[k], root),
      )
    )
  return s.type === 'integer'
    ? typeof value === 'number' && Number.isInteger(value)
    : !s.type || typeof value === s.type
}

export function byteLocation(parts: BytePart[]): string {
  return parts
    .map(([offset, mask]) => {
      const bits = Array.from({ length: 8 }, (_, bit) => bit)
        .filter((bit) => mask & (1 << bit))
        .reverse()
      return (
        `Byte ${offset} (0x${hex([offset])})` +
        (mask === 255
          ? ' · bits 7–0 (whole byte)'
          : ` · ${bits.length === 1 ? 'bit' : 'bits'} ${bits.join(', ')}`)
      )
    })
    .join('; ')
}

const namedOptionsCache = new Map<string, Promise<Record<string, string>>>()
export function namedValueHexOptions(
  standard: Standard,
  path: string,
  names: string[],
): Promise<Record<string, string>> {
  const key = JSON.stringify([standard, path, names])
  let pending = namedOptionsCache.get(key)
  if (!pending) {
    pending = (async () => {
      const baseline = await sample(standard)
      const parts = fieldLayout(standard, path)
      const entries = await Promise.all(
        names.map(async (name) => {
          const encoded = await convert({
            operation: 'encode',
            standard,
            fiber: true,
            document: setAt(
              baseline.document,
              path,
              path === '/Vendor OUI' ? { 'Vendor Name': name } : name,
            ),
          })
          return [name, fieldHex(encoded, parts)] as const
        }),
      )
      return Object.fromEntries(entries)
    })()
    namedOptionsCache.set(key, pending)
    pending.catch(() => namedOptionsCache.delete(key))
  }
  return pending
}
export async function vendorOUIOptions(
  standard: Standard,
  names: string[],
): Promise<Record<string, string>> {
  const values = await namedValueHexOptions(standard, '/Vendor OUI', names)
  return Object.fromEntries(
    Object.entries(values).map(([name, bytes]) => [name, bytes.replaceAll(' ', ':')]),
  )
}
