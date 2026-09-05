// SPDX-FileCopyrightText: 2026 Robin Christ
// SPDX-License-Identifier: MPL-2.0
import test from 'node:test'
import assert from 'node:assert/strict'
import { readFileSync } from 'node:fs'
import Ajv from 'ajv'
import createModule from '../src/wasm/transceiver.mjs'

const module = await createModule()
const request = (args) => JSON.parse(module.request(JSON.stringify(args)))
const ajv = new Ajv({ strict: false, allErrors: true, multipleOfPrecision: 8 })
for (const [standard, filename] of [
  ['8472', 'SFF-8472_LowerA0h'],
  ['8636', 'SFF-8636_Upper00h'],
]) {
  const schema = JSON.parse(
    readFileSync(new URL(`../../schemas/${filename}.json`, import.meta.url)),
  )
  const validate = ajv.compile(schema)
  test(`${standard}: every uniform byte pattern survives WASM and satisfies the editor schema`, () => {
    for (let value = 0; value < 256; value++) {
      const bytes = Array(standard === '8472' ? 128 : 256).fill(value)
      for (const fiber of [false, true]) {
        const decoded = request({ operation: 'decode', standard, fiber, bytes })
        assert.equal(decoded.ok, true, decoded.error)
        assert.equal(validate(decoded.document), true, JSON.stringify(validate.errors))
        assert.deepEqual(
          Object.keys(decoded.document).filter((k) => !(k in schema.properties)),
          ['GeneratedBy'],
        )
        const encoded = request({
          operation: 'encode',
          standard,
          fiber,
          document: decoded.document,
          ...(standard === '8636' ? { lowerBytes: bytes.slice(0, 128) } : {}),
        })
        assert.equal(encoded.ok, true, encoded.error)
        assert.deepEqual(encoded.bytes, bytes)
      }
    }
  })
  test(`${standard}: mixed bytes, interpretation switching, and raw checksum preservation`, () => {
    let seed = 0x86368472
    for (let sample = 0; sample < 100; sample++) {
      const bytes = Array.from({ length: standard === '8472' ? 128 : 256 }, () => {
        seed = (Math.imul(seed, 1664525) + 1013904223) | 0
        return (seed >>> 16) & 255
      })
      let decoded = request({ operation: 'decode', standard, fiber: true, bytes })
      const encoded = request({
        operation: 'encode',
        standard,
        fiber: true,
        document: decoded.document,
        ...(standard === '8636' ? { lowerBytes: bytes.slice(0, 128) } : {}),
      })
      assert.deepEqual(encoded.bytes, bytes)
      decoded = request({ operation: 'decode', standard, fiber: false, bytes: encoded.bytes })
      assert.equal(validate(decoded.document), true, JSON.stringify(validate.errors))
      const again = request({
        operation: 'encode',
        standard,
        fiber: false,
        document: decoded.document,
        ...(standard === '8636' ? { lowerBytes: bytes.slice(0, 128) } : {}),
      })
      assert.deepEqual(again.bytes, bytes)
    }
  })
}
test('bridge rejects malformed and oversized requests without poisoning subsequent calls', () => {
  assert.equal(JSON.parse(module.request('{')).ok, false)
  assert.equal(JSON.parse(module.request(' '.repeat(1024 * 1024 + 1))).ok, false)
  for (const args of [
    { operation: 'decode', standard: 'other', bytes: [] },
    { operation: 'other', standard: '8472' },
    { operation: 'decode', standard: '8472', bytes: [1] },
    { operation: 'decode', standard: '8472', bytes: Array(128).fill(256) },
    { operation: 'decode', standard: '8472', bytes: Array(128).fill(-1) },
    { operation: 'encode', standard: '8636', lowerBytes: [], document: {} },
  ])
    assert.equal(request(args).ok, false)
  assert.equal(
    request({ operation: 'decode', standard: '8472', bytes: Array(128).fill(0) }).ok,
    true,
  )
})
