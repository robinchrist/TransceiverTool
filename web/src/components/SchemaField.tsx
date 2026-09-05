// SPDX-FileCopyrightText: 2026 Robin Christ
// SPDX-License-Identifier: MPL-2.0
import { Select } from './ui/select'
import { useContext, useEffect, useId, useState, type ReactNode } from 'react'
import { Plus, X } from 'lucide-react'
import { Switch } from './ui/switch'
import { Button } from './ui/button'
import { Dialog } from './ui/dialog'
import { ByteContext, FieldBytes, FieldModeContext } from './FieldBytes'
import { NamedByteField } from './NamedByteField'
import { WholeByteControls } from './WholeByteControls'
import { fieldLayout, byteLocation, vendorOUIOptions } from '../lib/field-bytes'
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
  byteSummary?: ReactNode
}
export function SchemaField(props: Props) {
  const context = useContext(ByteContext)
  const parts = context ? fieldLayout(context.standard, props.path ?? '') : []
  return (
    <div className="field-location" title={parts.length ? byteLocation(parts) : undefined}>
      <FieldEditor {...props} />
    </div>
  )
}
function FieldEditor(props: Props) {
  const byteContext = useContext(ByteContext)
  const content = <ValueEditor {...props} />
  const schema = resolve(props.schema, props.root)
  const path = props.path ?? ''
  const parts = byteContext ? fieldLayout(byteContext.standard, path) : []
  const topLevel = path.split('/').length === 2
  if (!props.depth && parts.length) {
    const alternatives = schema.anyOf?.map((branch) => resolve(branch, props.root))
    const named = alternatives?.find((branch) =>
      branch.enum?.every((value) => typeof value === 'string'),
    )
    if (
      alternatives?.length === 2 &&
      named?.enum &&
      alternatives.some((branch) => branch.properties?.byteValue) &&
      parts.length === 1 &&
      parts[0][1] === 255
    )
      return (
        <NamedByteField
          name={props.name}
          path={path}
          value={props.value}
          names={named.enum as string[]}
          issues={props.issues ?? []}
        />
      )
    // A bit has no independent byte representation. Keep its ordinary toggle or
    // enum; the containing top-level group exposes the physical bytes together.
    if (
      !topLevel &&
      (schema.type === 'boolean' ||
        /\bBits?\s+\d/i.test(props.name) ||
        parts.some(([, mask]) => mask !== 255))
    )
      return content
    if (topLevel && hasBits(schema, props.root))
      return (
        <div className="byte-field byte-group">
          <ValueEditor
            {...props}
            byteSummary={<WholeByteControls path={path} name={props.name} parts={parts} />}
          />
        </div>
      )
  }
  return props.depth ? (
    content
  ) : (
    <FieldBytes
      path={props.path ?? ''}
      name={props.name}
      hasFormats={!!resolve(props.schema, props.root).anyOf}
    >
      {content}
    </FieldBytes>
  )
}
function ValueEditor({
  name,
  schema,
  root,
  value,
  onChange,
  depth = 0,
  path = '',
  issues = [],
  byteSummary,
}: Props) {
  const id = useId()
  const byteContext = useContext(ByteContext)
  const fieldMode = useContext(FieldModeContext)
  const mode = depth === 0 && fieldMode?.path === path ? fieldMode : null
  const [converting, setConverting] = useState(false)
  const [formatError, setFormatError] = useState('')
  const [pendingBranch, setPendingBranch] = useState<number | null>(null)
  const s = resolve(schema, root)
  const [vendorOUIs, setVendorOUIs] = useState<Record<string, string>>({})
  useEffect(() => {
    if (path !== '/Vendor OUI/Vendor Name' || !s.enum || !byteContext) return
    let active = true
    vendorOUIOptions(byteContext.standard, s.enum as string[])
      .then((options) => {
        if (active) setVendorOUIs(options)
      })
      .catch(() => {
        /* Keep the existing vendor names if the codec is unavailable. */
      })
    return () => {
      active = false
    }
  }, [path, s.enum, byteContext?.standard])
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
          {byteSummary ?? mode?.hexDisplay}
          <Select
            aria-label={`${name} value format`}
            title={`${byteContext ? byteLocation(fieldLayout(byteContext.standard, path)) + '. ' : ''}Change value format while preserving bytes when possible.`}
            className="variant-select"
            value={mode?.raw ? 'hex' : branch}
            disabled={
              converting ||
              byteContext?.locked ||
              byteContext?.hasDrafts ||
              (!!byteContext && !byteContext.result)
            }
            onChange={async (e) => {
              if (e.target.value === 'hex') {
                setFormatError('')
                mode?.setRaw(true)
                return
              }
              const next = Number(e.target.value)
              setConverting(true)
              setFormatError('')
              try {
                const converted = await byteContext?.format(path, s.anyOf![next])
                if (converted !== undefined) {
                  onChange(converted)
                  mode?.setRaw(false)
                } else if (path === '/Vendor OUI') {
                  setFormatError(
                    'No vendor in the built-in OUI list matches these bytes. The OUI has been kept unchanged.',
                  )
                } else setPendingBranch(next)
              } catch (error) {
                setFormatError((error as Error).message)
              } finally {
                setConverting(false)
              }
            }}
          >
            {mode && <option value="hex">Hex bytes</option>}
            {s.anyOf.map((option, i) => (
              <option key={i} value={i}>
                {path === '/Vendor OUI'
                  ? resolve(option, root).properties?.['Vendor Name']
                    ? 'Vendor name'
                    : 'OUI'
                  : branchLabel(option, root)}
              </option>
            ))}
          </Select>
        </div>
        {errorText}
        {formatError && (
          <p className="field-error" role="alert">
            {formatError}
          </p>
        )}
        {s.description && <p className="field-description">{s.description}</p>}
        {!mode?.raw && (
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
        )}
        <Dialog
          open={pendingBranch !== null}
          onOpenChange={(open) => {
            if (!open) setPendingBranch(null)
          }}
          title={`Replace ${name}?`}
          description="The current bytes cannot be represented in the selected format. You can keep the current value or replace it with the default shown below. Replacing it may change the EEPROM bytes; Undo can restore it."
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
                mode?.setRaw(false)
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
  if (s.properties?.Type?.const === 'Base64') {
    const object = value && typeof value === 'object' && !Array.isArray(value) ? value : {}
    const base64Issues = issues.filter(
      (issue) => issue.path === path || issue.path === `${path}/Value`,
    )
    return (
      <div className="scalar-field">
        <textarea
          className="field-input base64-input"
          aria-label={`${name} Base64`}
          data-field-path={`${path}/Value`}
          spellCheck={false}
          rows={3}
          value={typeof object.Value === 'string' ? object.Value : ''}
          aria-invalid={base64Issues.length > 0 || undefined}
          aria-describedby={base64Issues.length ? errorId : undefined}
          onChange={(event) => onChange({ ...object, Type: 'Base64', Value: event.target.value })}
        />
        {base64Issues.length > 0 && (
          <p id={errorId} className="field-error">
            {base64Issues.map((issue) => issue.message).join(' ')}
          </p>
        )}
      </div>
    )
  }
  if (s.type === 'object' || s.properties) {
    const object =
      value && typeof value === 'object' && !Array.isArray(value) ? (value as Document) : {}
    return (
      <div className={depth ? 'nested-fields' : 'object-fields'}>
        {depth === 0 && !mode && (
          <div className="byte-group-heading">
            <span className="field-label">{name}</span>
            {byteSummary}
          </div>
        )}
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
        {!mode && <label htmlFor={id}>{name}</label>}
        <Switch
          {...accessibility}
          aria-label={name}
          id={id}
          checked={value === true}
          onCheckedChange={onChange}
        />
      </div>
    )
  const label = depth === 0 && !mode && (
    <label className="field-label" htmlFor={id}>
      {name}
    </label>
  )
  if (s.enum)
    return (
      <div className="scalar-field">
        {label}
        <Select
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
              {vendorOUIs[String(option)] ? ` (OUI: ${vendorOUIs[String(option)]})` : ''}
            </option>
          ))}
        </Select>
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

function hasBits(schema: Schema, root: Schema): boolean {
  const resolved = resolve(schema, root)
  return (
    resolved.type === 'boolean' ||
    (resolved.anyOf?.some((branch) => hasBits(branch, root)) ?? false) ||
    Object.entries(resolved.properties ?? {}).some(
      ([name, child]) => /\bBits?\s+\d/i.test(name) || hasBits(child, root),
    )
  )
}
