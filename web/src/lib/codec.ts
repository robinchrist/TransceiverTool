// SPDX-FileCopyrightText: 2026 Robin Christ
// SPDX-License-Identifier: MPL-2.0
import createModule from '../wasm/transceiver.mjs'
import type { Document, Standard } from './schema'

export interface CodecResult {
  ok: true
  document: Document
  bytes: number[]
  errors: string[]
  warnings: string[]
}
let modulePromise: ReturnType<typeof createModule> | undefined
export const ready = () => (modulePromise ??= createModule())
export async function convert(args: {
  operation: 'decode' | 'encode'
  standard: Standard
  fiber: boolean
  bytes?: number[]
  document?: Document
  lowerBytes?: number[]
}): Promise<CodecResult> {
  const module = await ready()
  const result = JSON.parse(module.request(JSON.stringify(args)))
  if (!result.ok) throw new Error(result.error)
  return result
}
export function standardOf(document: Document): Standard {
  if (document.Type === 'SFF-8472 Rev 12.4 Lower Page A0h') return '8472'
  if (document.Type === 'SFF-8636 Rev 2.11 Upper Page 00h') return '8636'
  throw new Error(
    'Unsupported JSON Type. Choose an SFF-8472 Lower A0h or SFF-8636 Upper 00h configuration.',
  )
}
export async function sample(standard: Standard) {
  const bytes = Array(standard === '8472' ? 128 : 256).fill(0)
  const offset = standard === '8472' ? 0 : 128
  const text = (at: number, value: string, width: number) =>
    [...value.padEnd(width, ' ')].forEach((c, i) => {
      bytes[at + i] = c.charCodeAt(0)
    })
  bytes[offset] = standard === '8472' ? 3 : 17
  if (standard === '8472') {
    bytes[1] = 4
    bytes[2] = 7
    bytes[3] = 0x10
    bytes[11] = 6
    bytes[12] = 103
    bytes[14] = 10
    bytes[60] = 5
    bytes[61] = 30
    bytes[94] = 8
    text(20, 'EXAMPLE OPTICS', 16)
    text(40, 'SFP-10G-LR', 16)
    text(56, 'A1', 4)
    text(68, 'DEMO00000001', 16)
    text(84, '260905', 8)
  } else {
    bytes[130] = 7
    bytes[131] = 0x80
    bytes[139] = 5
    bytes[140] = 255
    bytes[142] = 10
    bytes[192] = 3
    bytes[222] = 100
    bytes[186] = 0x66
    bytes[187] = 0x58
    text(148, 'EXAMPLE OPTICS', 16)
    text(168, 'QSFP28-100G-LR4', 16)
    text(184, 'A1', 2)
    text(196, 'DEMO00000002', 16)
    text(212, '260905', 8)
  }
  for (const [start, end] of [
    [offset, offset + 63],
    [offset + 64, offset + 95],
  ])
    bytes[end] = bytes.slice(start, end).reduce((a, b) => a + b, 0) & 255
  return convert({ operation: 'decode', standard, fiber: true, bytes })
}
export function download(name: string, data: string | Uint8Array, type: string) {
  const blob = new Blob([typeof data === 'string' ? data : new Uint8Array(data).buffer], { type })
  const url = URL.createObjectURL(blob)
  const link = document.createElement('a')
  link.href = url
  link.download = name
  link.click()
  setTimeout(() => URL.revokeObjectURL(url), 1000)
}
