// SPDX-FileCopyrightText: 2026 Robin Christ
// SPDX-License-Identifier: MPL-2.0
import { useContext, useId } from 'react'
import { ByteContext } from './FieldBytes'
import { useHexInput } from './useHexInput'
import { hex, type BytePart } from '../lib/field-bytes'

export function WholeByteControls({
  path,
  name,
  parts,
}: {
  path: string
  name: string
  parts: BytePart[]
}) {
  return (
    <div className="whole-byte-controls" role="group" aria-label={`${name} bytes`}>
      {parts.map(([offset]) => (
        <ByteControl key={offset} path={path} name={name} offset={offset} />
      ))}
    </div>
  )
}

function ByteControl({ path, name, offset }: { path: string; name: string; offset: number }) {
  const context = useContext(ByteContext)!
  const id = useId()
  const snapshot = context.displayResult ?? context.result
  const value = snapshot ? hex([snapshot.bytes[offset]]) : ''
  const binary = snapshot ? snapshot.bytes[offset].toString(2).padStart(8, '0') : ''
  const { text, error, setText } = useHexInput(context, path, [[offset, 255]], value, offset)
  return (
    <div
      className="whole-byte-row"
      title={`Byte ${offset} (0x${hex([offset])}) · bits 7–0 (whole byte)`}
    >
      <div className="whole-byte-value">
        <label className="byte-prefix" htmlFor={id}>
          Hex:
        </label>
        <input
          id={id}
          className="field-input"
          aria-label={`${name} byte ${offset} hex`}
          title={`Byte ${offset} (0x${hex([offset])}), bits 7–0. Edits update all flags sharing this byte.`}
          aria-invalid={!!error || undefined}
          aria-describedby={`${id}-binary${error ? ` ${id}-error` : ''}`}
          value={text ?? value}
          spellCheck={false}
          autoComplete="off"
          disabled={context.locked || !value}
          onChange={(event) => setText(event.target.value)}
        />
        <code id={`${id}-binary`} title={`Byte ${offset} (0x${hex([offset])}) · binary bits 7–0`}>
          Bin: {binary ? `0b${binary}` : 'unavailable'}
        </code>
        {context.invalid && value && <span className="field-hint">Last valid bytes</span>}
      </div>
      {error && (
        <p id={`${id}-error`} className="field-error" role="alert">
          {error}
        </p>
      )}
    </div>
  )
}
