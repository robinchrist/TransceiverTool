// SPDX-FileCopyrightText: 2026 Robin Christ
// SPDX-License-Identifier: MPL-2.0
import { expect, it } from 'vitest'
import { convert, sample } from '../src/lib/codec'
import {
  schemas,
  pointer,
  resolve,
  selectedBranch,
  type Schema,
  type Value,
} from '../src/lib/schema'
import { fieldLayout, fieldHex, editHex, convertFieldFormat, getAt } from '../src/lib/field-bytes'
for (const standard of ['8472', '8636'] as const) {
  for (const fiber of [true, false]) {
    async function fixture() {
      const original = await sample(standard)
      return convert({ operation: 'decode', standard, fiber, bytes: original.bytes })
    }
    it(`${standard} (${fiber ? 'fiber' : 'copper'}): every binary field is covered and raw edits preserve neighboring bits`, async () => {
      const result = await fixture()
      for (const name of Object.keys(schemas[standard].properties!).filter((k) => k !== 'Type')) {
        const path = '/' + pointer(name),
          parts = fieldLayout(standard, path)
        expect(parts.length, name).toBeGreaterThan(0)
        const text = parts
          .map(([at, mask]) => (result.bytes[at] ^ mask) & mask)
          .map((v) => v.toString(16).padStart(2, '0'))
          .join(' ')
        const next = await editHex(result.document, result, standard, fiber, path, text)
        const encoded = await convert({
          operation: 'encode',
          standard,
          fiber,
          document: next,
          ...(standard === '8636' ? { lowerBytes: result.bytes.slice(0, 128) } : {}),
        })
        for (const [at, mask] of parts)
          expect(encoded.bytes[at] & mask, `${name} at ${at}`).toBe(
            (result.bytes[at] ^ mask) & mask,
          )
      }
    })
    it(`${standard} (${fiber ? 'fiber' : 'copper'}): every nested editable field has a bounded layout`, async () => {
      const result = await fixture(),
        root = schemas[standard]
      async function walk(schema: Schema, value: Value | undefined, path: string) {
        const s = resolve(schema, root)
        if (s.anyOf) return walk(s.anyOf[selectedBranch(s.anyOf, value, root)], value, path)
        if (s.const !== undefined) return
        if (s.properties) {
          for (const [key, child] of Object.entries(s.properties))
            await walk(
              child,
              value && typeof value === 'object' && !Array.isArray(value) ? value[key] : undefined,
              path + '/' + pointer(key),
            )
          return
        }
        if (path.endsWith('/Vendor Name') && path.includes('/Vendor OUI/')) return
        const parts = fieldLayout(standard, path)
        expect(parts.length, path).toBeGreaterThan(0)
        if (typeof value === 'boolean' || typeof value === 'number') {
          const text = parts
            .map(([at, mask]) => ((result.bytes[at] ^ mask) & mask).toString(16).padStart(2, '0'))
            .join(' ')
          const next = await editHex(result.document, result, standard, fiber, path, text)
          expect(getAt(next, path), path).not.toEqual(value)
        }
      }
      for (const [key, s] of Object.entries(root.properties!))
        if (key !== 'Type') await walk(s, result.document[key], '/' + pointer(key))
    })
    it(`${standard} (${fiber ? 'fiber' : 'copper'}): named, raw and Base64 format changes preserve bytes`, async () => {
      const result = await fixture(),
        root = schemas[standard]
      for (const name of ['Identifier', 'Vendor Name', 'Vendor Specific']) {
        const s = resolve(root.properties![name], root)
        for (const branch of s.anyOf ?? []) {
          const converted = await convertFieldFormat(
            result.document,
            result,
            standard,
            fiber,
            '/' + pointer(name),
            branch,
            root,
          )
          if (
            resolve(branch, root).properties?.byteValue ||
            resolve(branch, root).properties?.Type?.const === 'Base64'
          )
            expect(converted, name).toBeDefined()
        }
      }
      expect(fieldHex(result, fieldLayout(standard, '/Identifier'))).toBe(
        standard === '8472' ? '03' : '11',
      )
    })
  }
}

it('whole-byte edits update every group sharing a byte and reject unrelated offsets', async () => {
  const result = await sample('8472')
  const next = await editHex(
    result.document,
    result,
    '8472',
    true,
    '/10G Ethernet Compliance Codes',
    '19',
    3,
  )
  expect(getAt(next, '/10G Ethernet Compliance Codes/10GBASE-LR compliant (Bit 5)')).toBe(false)
  expect(getAt(next, '/10G Ethernet Compliance Codes/10GBASE-SR compliant (Bit 4)')).toBe(true)
  expect(getAt(next, '/Infiniband Compliance Codes/1X SX compliant (Bit 3)')).toBe(true)
  expect(getAt(next, '/Infiniband Compliance Codes/1X Copper Passive compliant (Bit 0)')).toBe(true)
  const encoded = await convert({
    operation: 'encode',
    standard: '8472',
    fiber: true,
    document: next,
  })
  expect(encoded.bytes[3]).toBe(0x19)
  for (let offset = 0; offset < result.bytes.length; offset++) {
    if (![3, 63, 95].includes(offset)) expect(encoded.bytes[offset]).toBe(result.bytes[offset])
  }
  await expect(
    editHex(result.document, result, '8472', true, '/10G Ethernet Compliance Codes', 'FF', 20),
  ).rejects.toThrow('outside')
})

it('named-byte JSON representation is chosen independently from hex editing', async () => {
  const result = await sample('8472')
  for (const [hex, representation, expected] of [
    ['03', 'named', 'SFP or SFP+'],
    ['01', 'named', 'GBIC'],
    ['FF', 'named', { byteValue: '0xff' }],
    ['03', 'raw', { byteValue: '0x03' }],
  ] as const) {
    const next = await editHex(
      result.document,
      result,
      '8472',
      true,
      '/Identifier',
      hex,
      undefined,
      representation,
    )
    expect(next.Identifier).toEqual(expected)
  }
})

for (const standard of ['8472', '8636'] as const) {
  it(`${standard}: OUI format switches preserve bytes and never invent a vendor`, async () => {
    const result = await sample(standard)
    const root = schemas[standard]
    const branches = resolve(root.properties!['Vendor OUI'], root).anyOf!
    const vendorBranch = branches.find(
      (branch) => resolve(branch, root).properties?.['Vendor Name'],
    )!
    const textBranch = branches.find((branch) => resolve(branch, root).type === 'string')!
    expect(
      await convertFieldFormat(
        result.document,
        result,
        standard,
        true,
        '/Vendor OUI',
        vendorBranch,
        root,
      ),
    ).toBeUndefined()
    const document = { ...result.document, 'Vendor OUI': { 'Vendor Name': 'FS COM INC' } }
    const known = await convert({ operation: 'encode', standard, fiber: true, document })
    const text = await convertFieldFormat(
      document,
      known,
      standard,
      true,
      '/Vendor OUI',
      textBranch,
      root,
    )
    expect(text).toBe('64:9D:99')
    const textDocument = { ...document, 'Vendor OUI': text! }
    const encoded = await convert({
      operation: 'encode',
      standard,
      fiber: true,
      document: textDocument,
    })
    expect(encoded.bytes).toEqual(known.bytes)
    expect(
      await convertFieldFormat(
        textDocument,
        encoded,
        standard,
        true,
        '/Vendor OUI',
        vendorBranch,
        root,
      ),
    ).toEqual({ 'Vendor Name': 'FS COM INC' })
  })
}
