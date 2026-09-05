// SPDX-FileCopyrightText: 2026 Robin Christ
// SPDX-License-Identifier: MPL-2.0
import Ajv, { type ErrorObject } from 'ajv'
import sfp from '../../../schemas/SFF-8472_LowerA0h.json'
import qsfp from '../../../schemas/SFF-8636_Upper00h.json'

export type Standard = '8472' | '8636'
export type Value = string | number | boolean | null | Value[] | { [key: string]: Value }
export type Document = { [key: string]: Value }
export interface Schema {
  $ref?: string
  type?: string
  description?: string
  const?: Value
  enum?: Value[]
  anyOf?: Schema[]
  properties?: Record<string, Schema>
  required?: string[]
  definitions?: Record<string, Schema>
  minimum?: number
  maximum?: number
  multipleOf?: number
  maxLength?: number
  minLength?: number
  pattern?: string
}
export const schemas: Record<Standard, Schema> = {
  '8472': sfp as unknown as Schema,
  '8636': qsfp as unknown as Schema,
}
const ajv = new Ajv({ strict: false, allErrors: true, multipleOfPrecision: 8 })
const validators = { '8472': ajv.compile(sfp), '8636': ajv.compile(qsfp) }
export interface FieldIssue {
  path: string
  message: string
}
export const pointer = (key: string) => key.replace(/~/g, '~0').replace(/\//g, '~1')
export const unpointer = (key: string) => key.replace(/~1/g, '/').replace(/~0/g, '~')
export function selectedBranch(options: Schema[], value: Value | undefined, root: Schema) {
  const match = options.findIndex((option) => matches(option, value, root))
  if (match >= 0) return match
  // Keep an invalid value in its existing editor rather than jumping to another format.
  const shape = options.findIndex((option) => {
    const s = resolve(option, root)
    if (s.type === 'object') return value !== null && typeof value === 'object'
    if (s.enum) return s.enum.some((v) => typeof v === typeof value)
    return s.type === typeof value || (s.type === 'integer' && typeof value === 'number')
  })
  return Math.max(0, shape)
}
function activeSchema(schema: Schema, value: Value | undefined, root: Schema): Schema {
  const s = resolve(schema, root)
  if (s.anyOf) return activeSchema(s.anyOf[selectedBranch(s.anyOf, value, root)], value, root)
  const { definitions: _definitions, ...rest } = s
  delete (rest as Schema & { $id?: string }).$id
  if (!s.properties) return rest
  const object = value && typeof value === 'object' && !Array.isArray(value) ? value : {}
  return {
    ...rest,
    properties: Object.fromEntries(
      Object.entries(s.properties).map(([key, child]) => [
        key,
        activeSchema(child, object[key], root),
      ]),
    ),
  }
}
const messageValidators = new Map<string, ReturnType<typeof ajv.compile>>()
function friendlyError(e: ErrorObject): FieldIssue {
  const path =
    e.instancePath + (e.keyword === 'required' ? '/' + pointer(e.params.missingProperty) : '')
  const leaf = unpointer(path.split('/').at(-1) ?? '')
  let message = e.message ?? 'Check this value.'
  if (leaf === 'byteValue' && ['pattern', 'minLength', 'maxLength'].includes(e.keyword))
    message = 'Enter one hexadecimal byte, for example 0x03.'
  else if (e.keyword === 'enum') message = 'Choose one of the listed values.'
  else if (e.keyword === 'required') message = 'This field is required.'
  else if (e.keyword === 'pattern' && leaf === 'Vendor OUI')
    message = 'Enter three hexadecimal bytes, for example 00:11:22.'
  else if (e.keyword === 'pattern') message = 'Check the format shown below this field.'
  return { path, message }
}
export function schemaIssues(standard: Standard, document: Document): FieldIssue[] {
  const validate = validators[standard]
  if (validate(document)) return []
  // The original schema remains authoritative. Narrow only the displayed diagnostics
  // to the format currently being edited, removing irrelevant anyOf branch errors.
  const originalErrors = validate.errors ?? []
  const schema = activeSchema(schemas[standard], document, schemas[standard])
  const key = JSON.stringify(schema)
  let display = messageValidators.get(key)
  if (!display) {
    display = ajv.compile(schema)
    if (messageValidators.size >= 16) {
      const oldest = messageValidators.keys().next().value!
      ajv.removeSchema(messageValidators.get(oldest)!.schema)
      messageValidators.delete(oldest)
    }
    messageValidators.set(key, display)
  }
  display(document)
  const errors = display.errors?.length ? display.errors : originalErrors
  return [
    ...new Map(
      errors
        .filter((e) => e.keyword !== 'anyOf')
        .map((e) => {
          const issue = friendlyError(e)
          return [issue.path + issue.message, issue] as const
        }),
    ).values(),
  ]
}
export function schemaErrors(standard: Standard, document: Document) {
  return schemaIssues(standard, document).map((e) => `${e.path || '/'} ${e.message}`)
}
export function resolve(schema: Schema, root: Schema): Schema {
  if (!schema.$ref) return schema
  const name = schema.$ref.split('/').at(-1)!
  const found = root.definitions?.[name]
  if (!found) throw new Error(`Missing schema definition: ${name}`)
  return resolve(found, root)
}
export function matches(schema: Schema, value: Value | undefined, root: Schema): boolean {
  const s = resolve(schema, root)
  if (s.const !== undefined) return s.const === value
  if (s.enum) return s.enum.includes(value as Value)
  if (s.anyOf) return s.anyOf.some((x) => matches(x, value, root))
  if (s.type === 'object')
    return (
      !!value &&
      typeof value === 'object' &&
      !Array.isArray(value) &&
      (s.required ?? []).every((k) => k in value) &&
      Object.entries(s.properties ?? {}).every(
        ([k, sub]) => !(k in value) || matches(sub, value[k], root),
      )
    )
  if (s.type === 'integer') return typeof value === 'number' && Number.isInteger(value)
  if (s.type === 'number') return typeof value === 'number'
  return s.type === undefined || typeof value === s.type
}
export function defaultValue(schema: Schema, root: Schema): Value {
  const s = resolve(schema, root)
  if (s.const !== undefined) return s.const
  if (s.enum) return s.enum[0]
  if (s.anyOf) return defaultValue(s.anyOf[0], root)
  if (s.type === 'object')
    return Object.fromEntries(
      Object.entries(s.properties ?? {})
        .filter(([k]) => (s.required ?? []).includes(k))
        .map(([k, v]) => [k, defaultValue(v, root)]),
    )
  if (s.type === 'boolean') return false
  if (s.type === 'integer' || s.type === 'number') return s.minimum ?? 0
  if (s.pattern === '^0x[0-9a-fA-F]{2}$') return '0x00'
  return ''
}
export function branchLabel(schema: Schema, root: Schema): string {
  const s = resolve(schema, root)
  if (s.const !== undefined) return String(s.const)
  if (s.enum) return 'Named value'
  if (s.properties?.byteValue) return 'Raw byte'
  if (s.properties?.Type?.const) return String(s.properties.Type.const)
  if (s.type === 'object') return 'Structured value'
  if (s.type === 'integer' || s.type === 'number') return 'Numeric value'
  return 'Text'
}
export const groups = [
  {
    id: 'identity',
    label: 'Module identity',
    caption: 'Identification & vendor details',
    keys: [
      'Identifier',
      'Extended Identifier',
      'Connector Type',
      'Vendor Name',
      'Vendor OUI',
      'Vendor Part Number',
      'Vendor Rev',
      'Vendor Serial Number',
      'Date Code',
    ],
  },
  {
    id: 'link',
    label: 'Link & optics',
    caption: 'Rates, reach & optical properties',
    keys: [
      'Encoding',
      'Nominal Signaling Rate [MBaud] (Divisible by 100)',
      'Rate Identifier',
      'Link Length',
      'Wavelength Or Cable Specification Compliance',
      'Extended Signaling Rate',
      'Extended Rate Select Compliance',
      'Length (Standard SM Fiber) [km]',
      'Length (OM3 50 um) [m] (Divisible by 2)',
      'Length (OM2 50 um) [m]',
      'Copper or Fibre Properties',
      'Device & Transmitter Properties',
      'Maximum Case Temperature [degC]',
      'Extended Baud Rate [MBaud] (Divisible by 250)',
    ],
  },
  {
    id: 'compliance',
    label: 'Compliance',
    caption: 'Supported standards & capabilities',
    keys: [] as string[],
  },
  {
    id: 'diagnostics',
    label: 'Diagnostics & options',
    caption: 'Monitoring & optional features',
    keys: ['Diagnostic Monitoring Type', 'Enhanced Options', 'Options', 'Option Values'],
  },
  {
    id: 'raw',
    label: 'Checksums & raw data',
    caption: 'Integrity & vendor-specific bytes',
    keys: ['CC_BASE', 'CC_EXT', 'Vendor Specific'],
  },
]
export function groupKeys(group: string, standard: Standard) {
  const all = Object.keys(schemas[standard].properties ?? {}).filter((k) => k !== 'Type')
  const known = groups.filter((g) => g.id !== 'compliance').flatMap((g) => g.keys)
  return group === 'compliance'
    ? all.filter((k) => !known.includes(k))
    : all.filter((k) => groups.find((g) => g.id === group)?.keys.includes(k))
}
