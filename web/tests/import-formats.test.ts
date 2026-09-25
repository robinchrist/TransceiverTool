// SPDX-FileCopyrightText: 2026 Robin Christ
// SPDX-License-Identifier: MPL-2.0
import { readFileSync } from 'node:fs'
import { describe, expect, it } from 'vitest'
import { parseBase64, parseHex, parseMlxlink, readInput } from '../src/lib/import-formats'

const fixture = (name: string) => readFileSync(new URL(`fixtures/${name}`, import.meta.url), 'utf8')
// The bytes behind the tool-generated dumps in tests/fixtures. Its text fields put
// hex-looking words ("CAFE 00", "BEEF 99") into the dumps' ASCII columns.
const sample = Array<number>(128).fill(0)
sample.splice(0, 4, 0x03, 0x04, 0x07, 0x20)
sample.splice(20, 16, ...Buffer.from('ABCDEF 12345678 '))
sample.splice(40, 16, ...Buffer.from('CAFE 00 BEEF 99 '))
sample[63] = 0x5a
sample[127] = 0xa5
const hex = (bytes: number[], separator = ' ') =>
  bytes.map((b) => b.toString(16).padStart(2, '0')).join(separator)
const rows = (bytes: number[], line: (row: number[], offset: number) => string) =>
  Array.from({ length: bytes.length / 16 }, (_, i) =>
    line(bytes.slice(i * 16, i * 16 + 16), i * 16),
  ).join('\n')
const ascii = (row: number[]) =>
  row.map((b) => (b >= 32 && b < 127 ? String.fromCharCode(b) : '.')).join('')

describe('hex bytes', () => {
  it.each([
    ['hexdump -C with repeated lines', fixture('hexdump-C.txt')],
    ['xxd with hex-looking ASCII', fixture('xxd.txt')],
    ['od -tx1 with octal offsets', fixture('od-tx1.txt')],
    ['spaced bytes', hex(sample)],
    ['contiguous digits', hex(sample, '')],
    ['xxd -p lines', hex(sample, '').replace(/(.{60})/g, '$1\n')],
    ['colon-separated bytes', hex(sample, ':')],
    ['dash-separated uppercase bytes', hex(sample, '-').toUpperCase()],
    ['0x list', sample.map((b) => `0x${b.toString(16)}`).join(', ')],
    [
      'C array declaration',
      `/* A0h */\nstatic const uint8_t eeprom[128] = {\n  ${rows(sample, (r) => r.map((b) => `0x${hex([b])},`).join(' '))}\n};`,
    ],
    ['JSON-style array of hex strings', `[${sample.map((b) => `0x${hex([b])}`).join(',')}]`],
    ['escaped string', `"${sample.map((b) => `\\x${hex([b])}`).join('')}"`],
    [
      'i2cdump',
      '     0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f    0123456789abcdef\n' +
        rows(sample, (r, o) => `${hex([o])}: ${hex(r)}    ${ascii(r)}`),
    ],
    [
      'ethtool -m hex',
      'Offset\t\tValues\n------\t\t------\n' +
        rows(sample, (r, o) => `0x${o.toString(16).padStart(4, '0')}:\t\t${hex(r)} `),
    ],
    [
      'PowerShell Format-Hex',
      '   Label: C:\\eeprom.bin\n\n          Offset Bytes                                           Ascii\n' +
        '                 00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F\n' +
        '          ------ ----------------------------------------------- -----------------\n' +
        rows(
          sample,
          (r, o) => `${o.toString(16).padStart(16, '0')} ${hex(r).toUpperCase()} ${ascii(r)}`,
        ),
    ],
    ['commented lines with CRLF', `# module dump\r\n${rows(sample, (r) => `${hex(r)} // row`)}`],
  ])('reads %s', (_, text) => {
    expect(parseHex(text)).toEqual(sample)
  })

  it('reports the line of an invalid byte', () => {
    expect(() => parseHex('11 07 02\n00 0g 00')).toThrow('Line 2: “0g” is not a hex byte.')
    expect(() => parseHex('11 070')).toThrow('“070” is not a hex byte (odd number of digits)')
    expect(() => parseHex('0x1107')).toThrow('(0x values must be single bytes)')
    const typo = fixture('xxd.txt').replace('4142 4344', '4142 43G4')
    expect(() => parseHex(typo)).toThrow('Line 2: “43G4” is not a hex byte.')
  })
  it('rejects offsets that skip or repeat bytes', () => {
    const gap = fixture('xxd.txt').replace('00000050:', '00000058:')
    expect(() => parseHex(gap)).toThrow('Line 5: expected 24 bytes')
    expect(() => parseHex('0000: 11 07\n0000: 02 00')).toThrow('does not follow the previous line')
  })
  it('rejects repeat markers it cannot expand', () => {
    expect(() => parseHex(fixture('od-An-tx1.txt'))).toThrow('only in dumps with offsets')
    const truncated = fixture('hexdump-C.txt').split('\n').slice(0, 6).join('\n')
    expect(() => parseHex(truncated)).toThrow('include the final offset line')
  })
  it('rejects byte-swapped word dumps', () => {
    expect(() => parseHex(fixture('hexdump-words.txt'))).toThrow('swaps byte order')
  })
  it('rejects lines without offsets inside a dump', () => {
    expect(() => parseHex('00: 11 07\n02 00')).toThrow('Line 2 has no offset')
  })
})

describe('Base64', () => {
  const encoded = Buffer.from(sample).toString('base64')
  it.each([
    ['padded', encoded],
    ['wrapped', encoded.replace(/(.{76})/g, '$1\n')],
    ['URL-safe without padding', Buffer.from(sample).toString('base64url')],
    ['data URI', `data:application/octet-stream;base64,${encoded}`],
    [
      'certutil -encode',
      `-----BEGIN CERTIFICATE-----\r\n${encoded.replace(/(.{64})/g, '$1\r\n')}\r\n-----END CERTIFICATE-----\r\n`,
    ],
  ])('reads %s data', (_, text) => {
    expect(parseBase64(text)).toEqual(sample)
  })
  it('rejects invalid characters and truncation', () => {
    expect(() => parseBase64('AAA*')).toThrow('Base64 may contain only')
    expect(() => parseBase64('AAAAA')).toThrow('truncated')
    expect(() => parseBase64('AAA==')).toThrow('truncated')
  })
})

function mlxlink(pages: Record<number, Record<number, number>>, code = 0) {
  const output: Record<string, { values: string[] }> = {}
  for (const [page, bytes] of Object.entries(pages))
    for (const [offset, value] of Object.entries(bytes))
      output[`page[${page}].Byte[${offset}]`] = { values: [`0x${hex([value])}`] }
  return {
    result: { output: { 'Cable Read Output': output, 'Operational Info': { State: 'Active' } } },
    status: { code, message: code ? 'Cable is not connected' : 'success' },
  }
}
const at = (start: number, bytes: number[]) =>
  Object.fromEntries(bytes.map((b, i) => [start + i, b]))
const qsfpLower = Array.from({ length: 128 }, (_, i) => (i === 0 ? 0x11 : i))
const qsfpUpper = Array.from({ length: 128 }, (_, i) => (i === 0 ? 0x11 : 255 - i))

describe('mlxlink / mstlink JSON', () => {
  it('explains that a QSFP lower page alone cannot be edited', () => {
    const value = JSON.parse(fixture('mlxlink-qsfp28-lower-page.json'))
    expect(() => parseMlxlink(value, '8472')).toThrow(
      'contains only bytes 0–127 of page 160, the QSFP lower page',
    )
  })
  it('reads a real mstlink read of page 0, offsets 128–255', () => {
    const result = parseMlxlink(JSON.parse(fixture('mlxlink-qsfp28-upper-page.json')), '8472')
    expect(result.standard).toBe('8636')
    expect(result.lowerBytes).toBeUndefined()
    expect(result.bytes.length).toBe(128)
    expect(Buffer.from(result.bytes.slice(20, 36)).toString()).toBe('INNOLIGHT       ')
  })
  it('reads a full QSFP page 0 and keeps the lower page', () => {
    const value = mlxlink({ 0: at(0, [...qsfpLower, ...qsfpUpper]), 3: at(128, qsfpUpper) })
    expect(parseMlxlink(value, '8472')).toEqual({
      standard: '8636',
      bytes: [...qsfpLower, ...qsfpUpper],
      lowerBytes: qsfpLower,
    })
  })
  it('reads a QSFP upper page by its offsets', () => {
    expect(parseMlxlink(mlxlink({ 0: at(128, qsfpUpper) }), '8472')).toEqual({
      standard: '8636',
      bytes: qsfpUpper,
    })
  })
  it('reads SFP A0h and ignores the vendor area above it', () => {
    const value = mlxlink({ 160: at(0, [...sample, ...qsfpUpper]), 162: at(0, qsfpLower) })
    expect(parseMlxlink(value, '8636')).toEqual({ standard: '8472', bytes: sample })
  })
  it('uses the selected standard for unknown identifiers', () => {
    const other = [0x18, ...sample.slice(1)]
    expect(parseMlxlink(mlxlink({ 0: at(0, other) }), '8472').standard).toBe('8472')
    expect(() => parseMlxlink(mlxlink({ 0: at(0, other) }), '8636')).toThrow('QSFP lower page')
  })
  it('accepts several consecutive values per key', () => {
    const output = Object.fromEntries(
      Array.from({ length: 32 }, (_, i) => [
        `page[0].Byte[${i * 4}]`,
        { values: sample.slice(i * 4, i * 4 + 4).map((b) => `0x${hex([b])}`) },
      ]),
    )
    expect(parseMlxlink({ 'Cable Read Output': output }, '8472').bytes).toEqual(sample)
  })
  it('reports missing, conflicting, and malformed bytes', () => {
    const partial = mlxlink({ 0: at(128, qsfpUpper.slice(0, 100)) })
    expect(() => parseMlxlink(partial, '8472')).toThrow(
      'missing upper page bytes 228, 229, 230, 231, 232, 233, 234, 235 and 20 more',
    )
    const conflicting = {
      a: { 'page[0].Byte[5]': { values: ['0x01'] } },
      b: { 'page[0].Byte[4]': { values: ['0x00', '0x02'] } },
    }
    expect(() => parseMlxlink(conflicting, '8472')).toThrow('Byte[5] has two different values')
    expect(() => parseMlxlink({ 'page[0].Byte[0]': { values: ['17'] } }, '8472')).toThrow(
      'is not a byte value',
    )
    expect(() =>
      parseMlxlink(mlxlink({ 1: at(128, qsfpUpper), 2: at(128, qsfpUpper) }), '8472'),
    ).toThrow('contains pages 1, 2')
  })
  it('reports errors from mlxlink', () => {
    expect(() => parseMlxlink(mlxlink({ 0: at(0, sample) }, 1), '8472')).toThrow(
      'mlxlink reported an error: Cable is not connected',
    )
  })
})

describe('input detection', () => {
  const file = (text: string) => new TextEncoder().encode(text)
  it('reads binary files and length-checks them', () => {
    expect(readInput(new Uint8Array(sample), 'auto', '8636')).toEqual({
      kind: 'bytes',
      format: 'binary',
      standard: '8636',
      bytes: sample,
    })
    const full = readInput(new Uint8Array(256), 'auto', '8472')
    expect(full).toMatchObject({ standard: '8636', lowerBytes: Array(128).fill(0) })
    const printable = new Uint8Array(128).fill(0x23)
    expect(readInput(printable, 'auto', '8636')).toMatchObject({ format: 'binary' })
    expect(() => readInput(new Uint8Array(512), 'auto', '8472')).toThrow('but found 512')
    expect(() => readInput(new Uint8Array(0), 'auto', '8472')).toThrow('The file is empty.')
  })
  it('detects text formats in files and pasted text', () => {
    expect(readInput(file(fixture('xxd.txt')), 'auto', '8472')).toMatchObject({ format: 'hex' })
    expect(readInput(Buffer.from(sample).toString('base64'), 'auto', '8472')).toMatchObject({
      format: 'base64',
      bytes: sample,
    })
    expect(readInput(fixture('hexdump-C.txt'), 'auto', '8472')).toMatchObject({ format: 'hex' })
    expect(readInput(`{ ${hex(sample, ', ')} }`, 'auto', '8472')).toMatchObject({ format: 'hex' })
    const mlx = JSON.stringify(mlxlink({ 0: at(128, qsfpUpper) }))
    expect(readInput(file(mlx), 'auto', '8472', 'dump.json')).toMatchObject({
      format: 'mlxlink',
      standard: '8636',
    })
    const document = { Type: 'SFF-8472 Rev 12.4 Lower Page A0h' }
    expect(readInput(JSON.stringify(document), 'auto', '8472')).toEqual({
      kind: 'document',
      document,
    })
  })
  it('decodes UTF-16 text written by PowerShell', () => {
    const text = `\ufeff${hex(sample)}\r\n`
    const utf16 = new Uint8Array([
      0xff,
      0xfe,
      ...new Uint8Array(Buffer.from(text.slice(1), 'utf16le')),
    ])
    expect(readInput(utf16, 'hex', '8472')).toMatchObject({ bytes: sample })
  })
  it('honors an explicit format', () => {
    const text = hex(sample, '')
    expect(readInput(file(text), 'binary', '8472')).toMatchObject({ format: 'binary' })
    expect(() => readInput(new Uint8Array(sample), 'hex', '8472')).toThrow('The file is not text.')
    expect(() => readInput(text, 'base64', '8472')).toThrow('but found 192')
    expect(() => readInput(text, 'mlxlink', '8472')).toThrow()
  })
  it('explains unrecognized text and JSON', () => {
    expect(() => readInput('hello world', 'auto', '8472')).toThrow(
      'not recognized as hex bytes, Base64, or JSON. As hex bytes: Line 1: “hello” is not a hex byte. As Base64: 7 bytes instead of 128 or 256.',
    )
    expect(() => readInput('11 07 02', 'auto', '8472')).toThrow('but found 3')
    expect(() => readInput('[17, 70, 20]', 'auto', '8472')).toThrow('Expected a JSON object')
    expect(() => readInput('{ "Type": ', 'auto', '8472')).toThrow(SyntaxError)
  })
})
