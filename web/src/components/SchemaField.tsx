// SPDX-FileCopyrightText: 2026 Robin Christ
// SPDX-License-Identifier: MPL-2.0
import { useId, useState } from 'react'
import { Plus, X } from 'lucide-react'
import { Switch } from './ui/switch'
import { Button } from './ui/button'
import { Dialog } from './ui/dialog'
import {
  branchLabel,
  defaultValue,
  selectedBranch,
  pointer,
  type FieldIssue,
  resolve,
  type Schema,
  type Value,
  type Document,
} from '../lib/schema'

interface Props {
  name: string
  schema: Schema
  root: Schema
  value: Value | undefined
  onChange: (value: Value) => void
  path?: string
  issues?: FieldIssue[]
  depth?: number
}
export function SchemaField({
  name,
  schema,
  root,
  value,
  onChange,
  depth = 0,
  path = '',
  issues = [],
}: Props) {
  const id = useId()
  const [pendingBranch, setPendingBranch] = useState<number | null>(null)
  const s = resolve(schema, root)
  const fieldIssues = issues.filter((issue) => issue.path === path)
  const errorId = `${id}-error`
  const accessibility = {
    'aria-invalid': fieldIssues.length > 0 || undefined,
    'aria-describedby': fieldIssues.length ? errorId : undefined,
    'data-field-path': path,
  }
  const errorText = fieldIssues.length > 0 && (
    <p id={errorId} className="field-error">
      {fieldIssues.map((e) => e.message).join(' ')}
    </p>
  )
  if (s.anyOf) {
    const branch = selectedBranch(s.anyOf, value, root)
    const replacement = pendingBranch === null ? null : defaultValue(s.anyOf[pendingBranch], root)
    return (
      <div className="union-field">
        <div className="union-heading">
          <span className="field-label">{name}</span>
          <select
            aria-label={`${name} value format`}
            title="Change value format. Review the replacement before applying it."
            className="variant-select"
            value={branch}
            onChange={(e) => setPendingBranch(Number(e.target.value))}
          >
            {s.anyOf.map((option, i) => (
              <option key={i} value={i}>
                {branchLabel(option, root)}
              </option>
            ))}
          </select>
        </div>
        {errorText}
        {s.description && <p className="field-description">{s.description}</p>}
        <SchemaField
          name={name}
          schema={s.anyOf[branch]}
          root={root}
          value={value}
          onChange={onChange}
          path={path}
          issues={issues}
          depth={depth + 1}
        />
        <Dialog
          open={pendingBranch !== null}
          onOpenChange={(open) => {
            if (!open) setPendingBranch(null)
          }}
          title={`Replace ${name}?`}
          description="This replaces the current value with the default shown below, which may change the EEPROM bytes. It does not convert the existing value. Undo can restore it."
        >
          <div className="replacement-preview">
            <strong>Current value</strong>
            <pre>{JSON.stringify(value, null, 2)}</pre>
            <strong>Replacement value</strong>
            <pre>{JSON.stringify(replacement, null, 2)}</pre>
          </div>
          <div className="flex justify-end gap-3 mt-6">
            <Button variant="outline" onClick={() => setPendingBranch(null)}>
              Keep current value
            </Button>
            <Button
              onClick={() => {
                onChange(replacement)
                setPendingBranch(null)
              }}
            >
              Replace value
            </Button>
          </div>
        </Dialog>
      </div>
    )
  }
  if (s.const !== undefined)
    return (
      <div className="constant-value">
        <span className="status-dot" />
        {String(s.const)}
      </div>
    )
  if (s.type === 'object' || s.properties) {
    const object =
      value && typeof value === 'object' && !Array.isArray(value) ? (value as Document) : {}
    return (
      <div className={depth ? 'nested-fields' : 'object-fields'}>
        {depth === 0 && <div className="field-label mb-3">{name}</div>}
        {Object.entries(s.properties ?? {}).map(([key, child]) => {
          const required = s.required?.includes(key)
          const present = key in object
          return (
            <div key={key} className="object-property">
              {!required && !present ? (
                <Button
                  variant="ghost"
                  size="sm"
                  onClick={() => onChange({ ...object, [key]: defaultValue(child, root) })}
                >
                  <Plus />
                  Add {key}
                </Button>
              ) : (
                <>
                  <SchemaField
                    name={key}
                    schema={child}
                    root={root}
                    value={object[key]}
                    onChange={(next) => onChange({ ...object, [key]: next })}
                    path={`${path}/${pointer(key)}`}
                    issues={issues}
                    depth={0}
                  />
                  {!required && (
                    <button
                      className="remove-field"
                      aria-label={`Remove ${key}`}
                      onClick={() => {
                        const next = { ...object }
                        delete next[key]
                        onChange(next)
                      }}
                    >
                      <X size={12} />
                    </button>
                  )}
                </>
              )}
            </div>
          )
        })}
      </div>
    )
  }
  if (s.type === 'boolean')
    return (
      <div className="boolean-field">
        <label htmlFor={id}>{name}</label>
        <Switch {...accessibility} id={id} checked={value === true} onCheckedChange={onChange} />
      </div>
    )
  const label = depth === 0 && (
    <label className="field-label" htmlFor={id}>
      {name}
    </label>
  )
  if (s.enum)
    return (
      <div className="scalar-field">
        {label}
        <select
          {...accessibility}
          id={id}
          aria-label={name}
          className="field-input"
          value={s.enum.findIndex((v) => v === value)}
          onChange={(e) => onChange(s.enum![Number(e.target.value)])}
        >
          {!s.enum.includes(value as Value) && (
            <option value={-1} disabled>
              Select a value
            </option>
          )}
          {s.enum.map((option, i) => (
            <option value={i} key={i}>
              {String(option)}
            </option>
          ))}
        </select>
        {String(value ?? '').length > 35 && <p className="selected-value">{String(value)}</p>}
        {errorText}
      </div>
    )
  const numeric = s.type === 'integer' || s.type === 'number'
  return (
    <div className="scalar-field">
      {label}
      <input
        {...accessibility}
        id={id}
        aria-label={name}
        className="field-input"
        type={numeric ? 'number' : 'text'}
        value={typeof value === 'string' || typeof value === 'number' ? value : ''}
        step={s.multipleOf ?? (s.type === 'integer' ? 1 : 'any')}
        min={s.minimum}
        max={s.maximum}
        spellCheck={false}
        onChange={(e) =>
          onChange(
            numeric ? (e.target.value === '' ? null : Number(e.target.value)) : e.target.value,
          )
        }
      />
      {errorText}
      {s.description && <p className="field-description">{s.description}</p>}
      {s.maxLength !== undefined && (
        <span className="field-hint">
          {String(value ?? '').length} / {s.maxLength} characters
        </span>
      )}
      {numeric && (s.minimum !== undefined || s.maximum !== undefined) && (
        <span className="field-hint">
          {s.minimum ?? '…'} – {s.maximum ?? '…'}
          {s.multipleOf ? ` · increments of ${s.multipleOf}` : ''}
        </span>
      )}
      {s.pattern && (
        <span className="field-hint">
          {name === 'byteValue'
            ? 'One byte in hexadecimal, e.g. 0x0A'
            : name === 'Vendor OUI'
              ? 'Three hexadecimal bytes separated by colons'
              : 'Use text matching this field’s format'}
        </span>
      )}
    </div>
  )
}
