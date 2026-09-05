// SPDX-FileCopyrightText: 2026 Robin Christ
// SPDX-License-Identifier: MPL-2.0
import { test, expect, type Download, type Page } from '@playwright/test'
import { pathToFileURL } from 'node:url'
import path from 'node:path'

const url = pathToFileURL(path.resolve('dist/index.html')).href
async function downloaded(download: Download) {
  const stream = await download.createReadStream()
  const chunks: Buffer[] = []
  for await (const chunk of stream!) chunks.push(Buffer.from(chunk))
  return Buffer.concat(chunks)
}
async function importBinary(page: Page, buffer: Buffer, standard = '8472') {
  await page.getByRole('button', { name: 'Import file', exact: true }).click()
  await page
    .getByLabel('Configuration file', { exact: true })
    .setInputFiles({ name: 'module.bin', mimeType: 'application/octet-stream', buffer })
  await page.getByLabel('Interpret 128-byte binary as').selectOption(standard)
  await page
    .getByRole('dialog')
    .getByRole('button', { name: 'Open configuration', exact: true })
    .click()
  await expect(
    page.getByRole('button', { name: 'Export configuration', exact: true }),
  ).toBeEnabled()
}

test.beforeEach(async ({ page }) => {
  // The distributed HTML must work directly from disk with all network access blocked.
  await page.route(/^https?:/, (route) => route.abort())
  await page.goto(url)
  await expect(page.getByText('WebAssembly core ready')).toBeVisible()
})

test('offline visual editing, invalid values, undo, JSON and byte views', async ({ page }) => {
  const pageErrors: string[] = []
  page.on('pageerror', (e) => pageErrors.push(e.message))
  await page.getByRole('button', { name: 'Explore SFP+ example' }).click()
  const vendor = page.getByRole('textbox', { name: 'Vendor Name', exact: true })
  await vendor.fill('LOCAL OPTICS')
  await expect(
    page.getByRole('button', { name: 'Export configuration', exact: true }),
  ).toBeEnabled()
  let event = page.waitForEvent('download')
  await page.getByRole('button', { name: 'Export configuration', exact: true }).click()
  await page.getByRole('button', { name: 'Download JSON', exact: true }).click()
  expect(JSON.parse((await downloaded(await event)).toString())['Vendor Name']).toBe('LOCAL OPTICS')
  await page.getByRole('button', { name: 'Close dialog' }).click()
  await vendor.fill('THIS VENDOR NAME IS TOO LONG')
  await expect(
    page.getByRole('button', { name: 'Export configuration', exact: true }),
  ).toBeDisabled()
  await expect(page.getByText('Fix input to export')).toBeVisible()
  await page.getByRole('button', { name: 'Undo last change' }).click()
  await expect(vendor).toHaveValue('LOCAL OPTICS')
  await page.getByRole('tab', { name: 'JSON', exact: true }).click()
  const source = page.getByRole('textbox', { name: 'JSON source', exact: true })
  await source.fill('{ broken')
  await expect(
    page.getByRole('button', { name: 'Export configuration', exact: true }),
  ).toBeDisabled()
  await page.getByRole('button', { name: 'Apply JSON' }).click()
  await expect(page.getByRole('alert')).toBeVisible()
  await page.getByRole('button', { name: 'Discard draft' }).click()
  await expect(source).toContainText('LOCAL OPTICS')
  await page.getByRole('tab', { name: 'Bytes', exact: true }).click()
  await expect(page.locator('.hex-table tbody tr')).toHaveCount(8)
  await expect(page.locator('.changed-byte').first()).toBeVisible()
  expect(pageErrors).toEqual([])
})

test('full QSFP binary roundtrip preserves lower page, mode switching, and edits', async ({
  page,
}) => {
  const original = Buffer.from(Array.from({ length: 256 }, (_, i) => (i * 43 + 19) & 255))
  await importBinary(page, original)
  await page.getByRole('button', { name: 'Export configuration', exact: true }).click()
  let event = page.waitForEvent('download')
  await page.getByRole('button', { name: 'Download full page · 256 bytes', exact: true }).click()
  expect(await downloaded(await event)).toEqual(original)
  await page.getByRole('button', { name: 'Close dialog' }).click()
  await page.locator('.module-details > summary').click()
  await page.getByRole('button', { name: 'Copper', exact: true }).click()
  await expect(
    page.getByRole('button', { name: 'Export configuration', exact: true }),
  ).toBeEnabled()
  await page.getByRole('button', { name: 'Link & optics', exact: false }).click()
  await expect(page.getByText('Device & Transmitter Properties', { exact: true })).toBeVisible()
  await page.getByRole('button', { name: 'Export configuration', exact: true }).click()
  event = page.waitForEvent('download')
  await page.getByRole('button', { name: 'Download full page · 256 bytes', exact: true }).click()
  expect(await downloaded(await event)).toEqual(original)
})

test('128-byte QSFP input uses explicit standard and cannot invent a lower page', async ({
  page,
}) => {
  const original = Buffer.alloc(128, 0x23)
  await importBinary(page, original, '8636')
  await page.getByRole('button', { name: 'Export configuration', exact: true }).click()
  await expect(
    page.getByRole('button', { name: 'Download full page · 256 bytes', exact: true }),
  ).toBeDisabled()
  const event = page.waitForEvent('download')
  await page.getByRole('button', { name: 'Download upper page · 128 bytes', exact: true }).click()
  expect(await downloaded(await event)).toEqual(original)
})

test('bad import preserves the current configuration and mobile navigation works', async ({
  page,
}) => {
  await page.setViewportSize({ width: 390, height: 844 })
  await page.getByRole('button', { name: 'Explore SFP+ example' }).click()
  await page.getByRole('button', { name: 'Import file', exact: true }).click()
  await page.getByLabel('Configuration file', { exact: true }).setInputFiles({
    name: 'bad.bin',
    mimeType: 'application/octet-stream',
    buffer: Buffer.alloc(17),
  })
  await page
    .getByRole('dialog')
    .getByRole('button', { name: 'Open configuration', exact: true })
    .click()
  await expect(page.getByRole('dialog').getByRole('alert')).toContainText('exactly 128 bytes')
  await page.getByRole('button', { name: 'Close dialog' }).click()
  await expect(page.getByRole('heading', { name: 'SFP-10G-LR', exact: true })).toBeVisible()
  await page.getByRole('button', { name: 'Open navigation' }).click()
  await page.getByRole('button', { name: 'Checksums & raw data', exact: false }).click()
  await expect(
    page.getByRole('heading', { name: 'Checksums & raw data', exact: true }),
  ).toBeVisible()
  expect(await page.evaluate(() => document.documentElement.scrollWidth <= window.innerWidth)).toBe(
    true,
  )
})

test('mobile representation replacement is deliberate and errors lead back to fields', async ({
  page,
}) => {
  await page.setViewportSize({ width: 390, height: 844 })
  await page.getByRole('button', { name: 'Explore SFP+ example' }).click()
  const representation = page.getByLabel('Identifier value format', { exact: true })
  await representation.selectOption({ label: 'Raw byte' })
  const dialog = page.getByRole('dialog')
  await expect(dialog).toContainText('SFP or SFP+')
  await expect(dialog).toContainText('0x00')
  await dialog.getByRole('button', { name: 'Keep current value' }).click()
  await expect(page.getByLabel('Identifier', { exact: true }).locator('option:checked')).toHaveText(
    'SFP or SFP+',
  )
  await representation.selectOption({ label: 'Raw byte' })
  await dialog.getByRole('button', { name: 'Replace value', exact: true }).click()
  const raw = page.getByRole('textbox', { name: 'byteValue', exact: true }).first()
  await expect(raw).toHaveValue('0x00')
  await raw.fill('banana')
  await expect(raw).toHaveAttribute('aria-invalid', 'true')
  await expect(raw).toHaveAccessibleDescription('Enter one hexadecimal byte, for example 0x03.')
  await expect(page.getByRole('region', { name: 'Input problems' })).not.toContainText(
    'listed values',
  )
  await page.getByRole('button', { name: 'Open navigation' }).click()
  const link = page.getByRole('button', { name: 'Link & optics' })
  await link.click()
  await expect(page.locator('#workspace-navigation button[aria-current="page"]')).toContainText(
    'Link & optics',
  )
  await page.getByRole('region', { name: 'Input problems' }).getByRole('button').click()
  await expect(raw).toBeFocused()
  await page.getByRole('button', { name: 'Undo last change' }).click()
  await expect(raw).toHaveValue('0x00')
  await page.getByRole('button', { name: 'Undo last change' }).click()
  await expect(page.getByLabel('Identifier', { exact: true }).locator('option:checked')).toHaveText(
    'SFP or SFP+',
  )
  await expect(
    page.getByRole('button', { name: 'Export configuration', exact: true }),
  ).toBeEnabled()
})

test('keyboard tabs and mobile navigation keep focus predictable', async ({ page }) => {
  await page.getByRole('button', { name: 'Explore SFP+ example' }).click()
  await page.getByRole('tab', { name: 'Visual editor' }).focus()
  await page.keyboard.press('ArrowRight')
  await expect(page.getByRole('tab', { name: 'JSON', exact: true })).toBeFocused()
  await expect(page.getByRole('tabpanel')).toHaveAccessibleName('JSON')
  await page.keyboard.press('End')
  await expect(page.getByRole('tab', { name: 'Bytes' })).toBeFocused()
  await page.keyboard.press('Home')
  await expect(page.getByRole('tab', { name: 'Visual editor' })).toBeFocused()
  await page.setViewportSize({ width: 390, height: 844 })
  const opener = page.getByRole('button', { name: 'Open navigation' })
  await opener.click()
  await expect(page.locator('#workspace-navigation button').first()).toBeFocused()
  await page.keyboard.press('Shift+Tab')
  await expect(page.getByRole('button', { name: 'Workflow & supported formats' })).toBeFocused()
  await page.keyboard.press('Escape')
  await expect(opener).toBeFocused()
  await expect(opener).toHaveAttribute('aria-expanded', 'false')
})
