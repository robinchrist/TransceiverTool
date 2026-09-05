// SPDX-FileCopyrightText: 2026 Robin Christ
// SPDX-License-Identifier: MPL-2.0
import type { Document, Value, Standard } from '../lib/schema'
function leaves(value: Value | undefined, prefix = ''): [string, Value][] {
  if (value && typeof value === 'object' && !Array.isArray(value))
    return Object.entries(value).flatMap(([key, child]) =>
      leaves(child, prefix ? `${prefix} / ${key}` : key),
    )
  return value === undefined ? [] : [[prefix, value]]
}
export function ModuleSummary({
  document,
  standard,
  fiber,
}: {
  document?: Document
  standard: Standard
  fiber: boolean
}) {
  const fields = document ? leaves(document) : []
  const compliance = fields
    .filter(
      ([key, value]) =>
        /Compliance Codes/.test(key) &&
        (value === true ||
          (typeof value === 'string' && /BASE|LR4|SR4|ER4|PSM4|CWDM/i.test(value))),
    )
    .map(([key, value]) =>
      typeof value === 'string'
        ? value
        : key
            .split(' / ')
            .at(-1)!
            .replace(/\s*\([^)]*Bit[^)]*\)/gi, '')
            .replace(/ compliant$/i, ''),
    )
  const wavelengths = fields
    .filter(([key, value]) => /Wavelength \[nm\]/.test(key) && typeof value === 'number')
    .map(([, value]) => `${value} nm`)
  let power = 'Not declared in this page'
  if (standard === '8636') {
    const extended = fields.find(([key]) => key.endsWith('Extended Power Class (Bit 1-0)'))?.[1]
    const base = fields.find(
      ([key]) => key.endsWith('Power Class (Bit 7-6)') && !key.includes('Extended Power'),
    )?.[1]
    power = fields.some(([key, value]) => /Power Class 8 implemented/.test(key) && value === true)
      ? 'Class 8 · limit outside this page'
      : typeof extended === 'string' && /Class [567] /.test(extended)
        ? extended
        : typeof base === 'string'
          ? base
          : power
  } else {
    const level = fields.some(([key, value]) => /Level 4\) requirement/.test(key) && value === true)
      ? 4
      : fields.some(([key, value]) => /Level 3 or Level 4/.test(key) && value === true)
        ? 3
        : fields.some(([key, value]) => /Power Level 2 requirement/.test(key) && value === true)
          ? 2
          : 1
    power = `Power level ${level}${level === 4 ? ' · maximum outside this page' : ''}`
  }
  const reach = fields
    .filter(
      ([key, value]) => /Length.*\[(?:km|m)\]/.test(key) && typeof value === 'number' && value > 0,
    )
    .map(
      ([key, value]) =>
        `${key
          .split(' / ')
          .at(-1)!
          .replace(/Length |\(Divisible by \d+\)/g, '')
          .trim()}: ${value}`,
    )
  return (
    <dl className="technical-summary">
      <div>
        <dt>Transceiver type / compliance</dt>
        <dd>
          {document
            ? [...new Set(compliance)].join(' · ') || 'Not declared'
            : 'Unavailable until input is valid'}
        </dd>
      </div>
      <div>
        <dt>Wavelength</dt>
        <dd>{!fiber ? 'Copper interpretation' : wavelengths.join(' · ') || 'Not declared'}</dd>
      </div>
      <div>
        <dt>Declared power</dt>
        <dd>{document ? power : '—'}</dd>
      </div>
      <div>
        <dt>Reach</dt>
        <dd>{reach.join(' · ') || 'Not declared'}</dd>
      </div>
      <div>
        <dt>Connector</dt>
        <dd>
          {typeof document?.['Connector Type'] === 'string'
            ? document['Connector Type']
            : 'Not decoded'}
        </dd>
      </div>
    </dl>
  )
}
