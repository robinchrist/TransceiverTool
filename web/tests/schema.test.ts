// SPDX-FileCopyrightText: 2026 Robin Christ
// SPDX-License-Identifier: MPL-2.0
import { describe, expect, it } from 'vitest'
import {
  groups,
  groupKeys,
  schemas,
  defaultValue,
  matches,
  resolve,
  schemaIssues,
  type Document,
} from '../src/lib/schema'

describe('schema-driven field coverage', () => {
  for (const standard of ['8472', '8636'] as const) {
    it(`exposes every editable ${standard} property exactly once`, () => {
      const keys = groups.flatMap((g) => groupKeys(g.id, standard))
      expect(new Set(keys).size).toBe(keys.length)
      expect(keys.sort()).toEqual(
        Object.keys(schemas[standard].properties!)
          .filter((k) => k !== 'Type')
          .sort(),
      )
    })
    it(`resolves and initializes every alternative in ${standard}`, () => {
      const root = schemas[standard]
      for (const property of Object.values(root.definitions!)) {
        const s = resolve(property, root)
        for (const branch of s.anyOf ?? [s]) {
          const value = defaultValue(branch, root)
          expect(matches(branch, value, root)).toBe(true)
        }
      }
    })
  }
})

describe('actionable schema diagnostics', () => {
  it('keeps invalid raw bytes in their branch and removes unrelated named-value errors', () => {
    const document = defaultValue(schemas['8472'], schemas['8472']) as Document
    document.Identifier = { byteValue: 'banana' }
    const issues = schemaIssues('8472', document).filter((issue) =>
      issue.path.startsWith('/Identifier'),
    )
    expect(issues).toEqual([
      { path: '/Identifier/byteValue', message: 'Enter one hexadecimal byte, for example 0x03.' },
    ])
    document.Identifier = { byteValue: '0x03' }
    expect(
      schemaIssues('8472', document).filter((issue) => issue.path.startsWith('/Identifier')),
    ).toEqual([])
  })
  it('retains required-property errors from the authoritative schema', () => {
    const document = defaultValue(schemas['8472'], schemas['8472']) as Document
    delete document.Identifier
    expect(schemaIssues('8472', document)).toContainEqual({
      path: '/Identifier',
      message: 'This field is required.',
    })
  })
})
