// SPDX-FileCopyrightText: 2026 Robin Christ
// SPDX-License-Identifier: MPL-2.0
import { Select } from './ui/select'
import { useContext, useEffect, useId, useState } from 'react'
import { ByteContext } from './FieldBytes'
import { useHexInput } from './useHexInput'
import {
  fieldHex,
  fieldLayout,
  getAt,
  byteLocation,
  namedValueHexOptions,
} from '../lib/field-bytes'
import type { FieldIssue, Value } from '../lib/schema'

export function NamedByteField({
  name,
  path,
  value,
  names,
  issues,
}: {
  name: string
  path: string
  value: Value | undefined
  names: string[]
  issues: FieldIssue[]
}) {
  const context = useContext(ByteContext)!
  const id = useId()
  const [optionHex, setOptionHex] = useState<Record<string, string>>({})
  useEffect(() => {
    let active = true
    namedValueHexOptions(context.standard, path, names)
      .then((options) => {
        if (active) setOptionHex(options)
      })
      .catch(() => {
        /* Names remain usable if the codec is unavailable. */
      })
    return () => {
      active = false
    }
  }, [context.standard, path, names])
  const isRaw = !!value && typeof value === 'object' && 'byteValue' in value
  const [mode, setMode] = useState(isRaw ? 'hex' : 'named')
  const [writeRaw, setWriteRaw] = useState(isRaw)
  const [busy, setBusy] = useState(false)
  const [error, setError] = useState('')
  const snapshot = context.displayResult ?? context.result
  const bytes = snapshot ? fieldHex(snapshot, fieldLayout(context.standard, path)) : ''
  const decoded = snapshot ? getAt(snapshot.document, path) : undefined
  const named = typeof decoded === 'string' && names.includes(decoded) ? decoded : undefined
  const fieldErrors = issues
    .filter((issue) => issue.path === path || issue.path.startsWith(path + '/'))
    .map((issue) => issue.message)
  const currentName = typeof value === 'string' && names.includes(value) ? value : named
  const locked = busy || context.locked || !context.result
  const {
    text,
    error: hexError,
    setText,
  } = useHexInput(
    context,
    path,
    fieldLayout(context.standard, path),
    bytes,
    undefined,
    writeRaw ? 'raw' : 'named',
  )
  useEffect(() => {
    if (context.result && named) setWriteRaw(isRaw)
  }, [context.result, named, isRaw])
  async function perform(action: () => Promise<void>) {
    setBusy(true)
    setError('')
    try {
      await action()
    } catch (e) {
      setError((e as Error).message)
    } finally {
      setBusy(false)
    }
  }
  return (
    <div className="byte-field named-byte-field">
      <div className="union-heading">
        <label className="field-label" htmlFor={id}>
          {name}
        </label>
        <div className="field-byte-strip inline-hex">
          <code
            title={`${byteLocation(fieldLayout(context.standard, path))}${!context.result ? '; last encoded bytes' : ''}`}
          >
            {context.invalid && bytes ? 'Hex (last valid)' : 'Hex'}: {bytes || 'unavailable'}
          </code>
        </div>
        <Select
          aria-label={`${name} value format`}
          className="variant-select"
          value={mode}
          disabled={busy || context.locked || context.hasDrafts}
          onChange={(event) => setMode(event.target.value)}
        >
          <option value="named">Named value</option>
          <option value="hex">Hex bytes</option>
        </Select>
      </div>
      {mode === 'hex' ? (
        <>
          <input
            id={id}
            className="field-input hex-input"
            aria-label={`${name} hex bytes`}
            data-field-path={path}
            spellCheck={false}
            value={text ?? bytes}
            disabled={context.locked || busy || !bytes}
            aria-invalid={!!error || !!hexError || fieldErrors.length > 0 || undefined}
            aria-describedby={`${id}-meaning${error || hexError ? ` ${id}-error` : ''}`}
            onChange={(event) => {
              setError('')
              setText(event.target.value)
            }}
          />
          <p id={`${id}-meaning`} className="field-hint">
            {named ?? 'No corresponding named value'}
          </p>
        </>
      ) : (
        <Select
          id={id}
          aria-label={name}
          className="field-input"
          data-field-path={path}
          value={currentName ?? ''}
          disabled={locked || context.hasDrafts}
          onChange={(event) => {
            const selected = event.target.value
            void perform(() => context.setNamed(path, selected, writeRaw))
          }}
        >
          {!currentName && (
            <option value="" disabled>
              No corresponding named value{bytes ? ` (0x${bytes})` : ''}
            </option>
          )}
          {names.map((option) => (
            <option key={option} value={option}>
              {option}
              {optionHex[option] ? ` (Hex: ${optionHex[option]})` : ''}
            </option>
          ))}
        </Select>
      )}
      <label className="raw-byte-preference">
        <input
          type="checkbox"
          aria-label={`${name} write as raw byte`}
          checked={writeRaw}
          disabled={locked || context.hasDrafts}
          onChange={(event) => {
            const raw = event.target.checked
            void perform(async () => {
              await context.apply(path, bytes, undefined, raw ? 'raw' : 'named')
              setWriteRaw(raw)
            })
          }}
        />
        Write as raw byte
      </label>
      {(error || hexError || fieldErrors.length > 0) && (
        <p id={`${id}-error`} className="field-error" role="alert">
          {error || hexError || fieldErrors.join(' ')}
        </p>
      )}
    </div>
  )
}
