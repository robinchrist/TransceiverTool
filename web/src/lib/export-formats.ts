// SPDX-FileCopyrightText: 2026 Robin Christ
// SPDX-License-Identifier: MPL-2.0

// Text representations of exported programming bytes. Every format can be imported
// again; offsets in dumps count from the start of the exported bytes, as when the tool
// runs on the downloaded binary.

export type TextFormat =
  | 'hex'
  | 'hex-compact'
  | 'hex-list'
  | 'c-array'
  | 'hexdump'
  | 'xxd'
  | 'base64'
export const textFormats: Record<TextFormat, { label: string; file: string }> = {
  hex: { label: 'Hex bytes · 11 07 02', file: 'hex.txt' },
  'hex-compact': { label: 'Hex string · 110702', file: 'hex.txt' },
  'hex-list': { label: '0x list · 0x11, 0x07', file: 'txt' },
  'c-array': { label: 'C array', file: 'h' },
  hexdump: { label: 'hexdump -C', file: 'hexdump.txt' },
  xxd: { label: 'xxd', file: 'xxd.txt' },
  base64: { label: 'Base64', file: 'b64' },
}

const hex = (byte: number) => byte.toString(16).padStart(2, '0')
const offset = (at: number) => at.toString(16).padStart(8, '0')
const ascii = (row: number[]) =>
  row.map((b) => (b >= 0x20 && b <= 0x7e ? String.fromCharCode(b) : '.')).join('')
const rows = (bytes: number[]) =>
  Array.from({ length: Math.ceil(bytes.length / 16) }, (_, i) => bytes.slice(i * 16, i * 16 + 16))

/** `name` is the C array identifier. */
export function formatBytes(bytes: number[], format: TextFormat, name = 'eeprom'): string {
  switch (format) {
    case 'hex':
      return (
        rows(bytes)
          .map((row) => row.map(hex).join(' '))
          .join('\n') + '\n'
      )
    case 'hex-compact':
      return bytes.map(hex).join('') + '\n'
    case 'hex-list':
      return (
        rows(bytes)
          .map((row) => row.map((b) => `0x${hex(b)}`).join(', '))
          .join(',\n') + '\n'
      )
    case 'c-array':
      return (
        `const uint8_t ${name}[${bytes.length}] = {\n` +
        rows(bytes)
          .map((row) => `    ${row.map((b) => `0x${hex(b)},`).join(' ')}`)
          .join('\n') +
        '\n};\n'
      )
    case 'hexdump': {
      // Matches `hexdump -C`, which prints `*` in place of repeated lines.
      const lines: string[] = []
      let previous = ''
      rows(bytes).forEach((row, i) => {
        const data = row.map(hex).join(' ')
        if (data === previous && row.length === 16) {
          if (lines.at(-1) !== '*') lines.push('*')
          return
        }
        previous = data
        const halves = `${row.slice(0, 8).map(hex).join(' ')}  ${row.slice(8).map(hex).join(' ')}`
        lines.push(`${offset(i * 16)}  ${halves.padEnd(48)}  |${ascii(row)}|`)
      })
      return [...lines, offset(bytes.length)].join('\n') + '\n'
    }
    case 'xxd':
      return (
        rows(bytes)
          .map((row, i) => {
            const groups = (
              row
                .map(hex)
                .join('')
                .match(/.{1,4}/g) ?? []
            ).join(' ')
            return `${offset(i * 16)}: ${groups.padEnd(39)}  ${ascii(row)}`
          })
          .join('\n') + '\n'
      )
    case 'base64':
      return btoa(String.fromCharCode(...bytes)) + '\n'
  }
}
