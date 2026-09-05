// SPDX-FileCopyrightText: 2026 Robin Christ
// SPDX-License-Identifier: MPL-2.0
import { Select } from './ui/select'
import { createContext, useContext, useId, useState, type ReactNode } from 'react'
import { useHexInput } from './useHexInput'
import { fieldHex, fieldLayout, hex, byteLocation } from '../lib/field-bytes'
import type { CodecResult } from '../lib/codec'
import type { Schema, Standard, Value } from '../lib/schema'
export interface FieldByteContext {
  standard: Standard
  result?: CodecResult
  displayResult?: CodecResult
  invalid: boolean
  locked: boolean
  hasDrafts: boolean
  pending: (path: string, dirty: boolean) => void
  apply: (
    path: string,
    text: string,
    wholeByte?: number,
    representation?: 'named' | 'raw',
  ) => Promise<void>
  setNamed: (path: string, name: string, raw: boolean) => Promise<void>
  format: (path: string, schema: Schema) => Promise<Value | undefined>
}
export const ByteContext = createContext<FieldByteContext | null>(null)
export const FieldModeContext = createContext<{
  path: string
  raw: boolean
  setRaw: (raw: boolean) => void
  hexDisplay: ReactNode
} | null>(null)
export function FieldBytes({
  path,
  name,
  children,
  hasFormats,
}: {
  path: string
  name: string
  children: ReactNode
  hasFormats: boolean
}) {
  const context = useContext(ByteContext)
  const id = useId()
  const [raw, setRaw] = useState(name === 'Vendor Specific')
  const parts = context ? fieldLayout(context.standard, path) : []
  const snapshot = context?.displayResult ?? context?.result
  const value = snapshot && parts.length ? fieldHex(snapshot, parts) : ''
  const { text, error, setText } = useHexInput(context, path, parts, value)
  if (!context || !parts.length) return children
  const hexDisplay = (
    <div className={`field-byte-strip inline-hex${parts.length > 8 ? ' long-hex' : ''}`}>
      <code title={`${byteLocation(parts)}${!context.result ? '; last encoded bytes' : ''}`}>
        {context.invalid && value ? 'Hex (last valid)' : 'Hex'}: {value || 'unavailable'}
      </code>
    </div>
  )
  return (
    <FieldModeContext.Provider value={{ path, raw, setRaw, hexDisplay }}>
      <div className="byte-field">
        {!hasFormats && (
          <div className="union-heading">
            <span className="field-label">{name}</span>
            {hexDisplay}
            <Select
              className="variant-select"
              aria-label={`${name} value format`}
              value={raw ? 'hex' : 'structured'}
              disabled={text !== null || context.locked || context.hasDrafts}
              onChange={(event) => setRaw(event.target.value === 'hex')}
            >
              <option value="structured">Structured</option>
              <option value="hex">Hex bytes</option>
            </Select>
          </div>
        )}
        {(hasFormats || !raw) && children}
        {raw && (
          <div className="raw-field-editor">
            <textarea
              id={id}
              aria-label={`${name} hex bytes`}
              aria-invalid={!!error || undefined}
              aria-describedby={error ? `${id}-error` : `${id}-hint`}
              className="field-input hex-input"
              rows={parts.length > 8 ? 3 : 1}
              spellCheck={false}
              value={text ?? value}
              disabled={context.locked || !value}
              onChange={(event) => setText(event.target.value)}
            />
            <p id={`${id}-hint`} className="field-hint">
              {parts.length} byte{parts.length === 1 ? '' : 's'} · offset{' '}
              {parts.map(([at]) => at.toString(16).padStart(2, '0').toUpperCase()).join(', ')}
              {parts.some(([, mask]) => mask !== 255) && ` · mask ${hex(parts.map(([, m]) => m))}`}
            </p>
            {error && (
              <p id={`${id}-error`} className="field-error" role="alert">
                {error}
              </p>
            )}
          </div>
        )}
      </div>
    </FieldModeContext.Provider>
  )
}
