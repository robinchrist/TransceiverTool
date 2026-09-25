// SPDX-FileCopyrightText: 2026 Robin Christ
// SPDX-License-Identifier: MPL-2.0
import { readFileSync } from 'node:fs'
import { describe, expect, it } from 'vitest'
import { formatBytes, textFormats, type TextFormat } from '../src/lib/export-formats'
import { readInput } from '../src/lib/import-formats'

const fixture = (name: string) => readFileSync(new URL(`fixtures/${name}`, import.meta.url), 'utf8')
// The bytes behind the tool-generated dumps in tests/fixtures.
const sample = Array<number>(128).fill(0)
sample.splice(0, 4, 0x03, 0x04, 0x07, 0x20)
sample.splice(20, 16, ...Buffer.from('ABCDEF 12345678 '))
sample.splice(40, 16, ...Buffer.from('CAFE 00 BEEF 99 '))
sample[63] = 0x5a
sample[127] = 0xa5
const full = Array.from({ length: 256 }, (_, i) => (i * 43 + 19) & 255)

describe('text export', () => {
  it('matches hexdump -C and xxd output byte for byte', () => {
    expect(formatBytes(sample, 'hexdump')).toBe(fixture('hexdump-C.txt'))
    expect(formatBytes(sample, 'xxd')).toBe(fixture('xxd.txt'))
  })
  it('writes the simple notations', () => {
    const bytes = [0x11, 0x07, 0xff]
    expect(formatBytes(bytes, 'hex')).toBe('11 07 ff\n')
    expect(formatBytes(bytes, 'hex-compact')).toBe('1107ff\n')
    expect(formatBytes(bytes, 'hex-list')).toBe('0x11, 0x07, 0xff\n')
    expect(formatBytes(bytes, 'c-array', 'page')).toBe(
      'const uint8_t page[3] = {\n    0x11, 0x07, 0xff,\n};\n',
    )
    expect(formatBytes(bytes, 'base64')).toBe('EQf/\n')
  })
  for (const format of Object.keys(textFormats) as TextFormat[])
    it(`imports ${format} output back to the same bytes`, () => {
      for (const bytes of [sample, full, Array(128).fill(0)])
        expect(readInput(formatBytes(bytes, format), 'auto', '8472')).toMatchObject({
          kind: 'bytes',
          bytes,
        })
    })
})
