// SPDX-FileCopyrightText: 2026 Robin Christ
// SPDX-License-Identifier: MPL-2.0
import { test, expect, type Download, type Page, type Locator } from '@playwright/test'
import { pathToFileURL } from 'node:url'
import path from 'node:path'

const url = pathToFileURL(path.resolve('dist/index.html')).href
async function menuOptions(control: Locator) {
  if ((await control.getAttribute('aria-expanded')) !== 'true') await control.click()
  const id = await control.getAttribute('aria-controls')
  return control
    .page()
    .locator(`[id=${JSON.stringify(id)}]`)
    .getByRole('option')
}
async function choose(control: Locator, option: string | { label?: string; value?: string }) {
  if ((await control.evaluate((el) => el.tagName)) === 'SELECT') {
    await control.selectOption(option)
    return
  }
  const options = await menuOptions(control)
  if (typeof option === 'object' && option.label !== undefined)
    await options.getByText(option.label, { exact: true }).click()
  else {
    const value = typeof option === 'string' ? option : option.value!
    await control
      .page()
      .getByRole('option')
      .and(control.page().locator(`[data-value=${JSON.stringify(value)}]`))
      .click()
  }
}

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
  await choose(page.getByLabel('Interpret 128-byte binary as'), standard)
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

test('module summary leads with the extended compliance code and folds Infiniband rates', async ({
  page,
}) => {
  const upper = Buffer.alloc(128)
  upper[0] = 0x11 // QSFP28
  upper[3] = 0x80 // 10/40G/100G Ethernet Compliance Codes: Extended
  upper[36] = 0x38 // Extended Module Codes: Infiniband HDR, EDR, FDR
  upper[64] = 0x06 // Extended Specification Compliance Codes: 100G CWDM4
  await importBinary(page, upper, '8636')
  await expect(page.locator('.technical-summary')).toContainText(
    '100G CWDM4 · Infiniband HDR (200G) / EDR (100G) / FDR (56G)',
  )
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
  await expect(
    page.getByRole('heading', { name: 'Example Mfg. PN-1234-SFP', exact: true }),
  ).toBeVisible()
  await page.getByRole('button', { name: 'Open navigation' }).click()
  await page.getByRole('button', { name: 'Checksums & raw data', exact: false }).click()
  await expect(
    page.getByRole('heading', { name: 'Checksums & raw data', exact: true }),
  ).toBeVisible()
  expect(await page.evaluate(() => document.documentElement.scrollWidth <= window.innerWidth)).toBe(
    true,
  )
})

test('mobile named-byte hex drafts validate and recover without changing the configuration', async ({
  page,
}) => {
  await page.setViewportSize({ width: 390, height: 844 })
  await page.getByRole('button', { name: 'Explore SFP+ example' }).click()
  const exportButton = page.getByRole('button', { name: 'Export configuration', exact: true })
  await expect(exportButton).toBeEnabled()
  const representation = page.getByLabel('Identifier value format', { exact: true })
  await expect(await menuOptions(representation)).toHaveText(['Named value', 'Hex bytes'])
  await choose(representation, { label: 'Hex bytes' })
  const raw = page.getByRole('textbox', { name: 'Identifier hex bytes', exact: true })
  await expect(raw).toHaveValue('03')
  await expect(raw).toHaveAccessibleDescription('SFP or SFP+')
  await raw.fill('banana')

  await expect(raw).toHaveAttribute('aria-invalid', 'true')
  await expect(raw).toHaveAccessibleDescription(/Enter exactly 1 hexadecimal byte/)
  await expect(exportButton).toBeDisabled()
  await raw.fill('03')
  await expect(raw).toHaveValue('03')
  await expect(exportButton).toBeEnabled()
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

test('continuous sections, compact specifications, and vendor hex editing', async ({ page }) => {
  await page.getByRole('button', { name: 'Explore SFP+ example' }).click()
  await expect(page.locator('.configuration-section')).toHaveCount(5)
  await expect(
    page.getByRole('textbox', { name: 'Vendor Specific hex bytes', exact: true }),
  ).toBeVisible()
  await expect(page.getByRole('heading', { level: 1 })).toHaveText('Example Mfg. PN-1234-SFP')
  await expect(page.locator('.module-details > summary')).toContainText(
    'Example Mfg. · PN-1234-SFP · SFP/SFP+ (SFF-8472)',
  )
  await expect(page.locator('.technical-summary')).toContainText('1310 nm')
  await expect(page.locator('.module-drawing')).toHaveCount(0)
  const vendor = page.getByRole('textbox', { name: 'Vendor Specific hex bytes', exact: true })
  await vendor.fill('A')
  await expect(
    page.getByRole('button', { name: 'Export configuration', exact: true }),
  ).toBeDisabled()
  await expect(page.getByRole('tab', { name: 'JSON', exact: true })).toBeDisabled()
  await vendor.fill('AA ' + Array(31).fill('00').join(' '))

  await expect(vendor).toHaveValue('AA ' + Array(31).fill('00').join(' '))
  await page.getByRole('button', { name: 'Export configuration', exact: true }).click()
  const event = page.waitForEvent('download')
  await page.getByRole('button', { name: 'Download binary · 128 bytes', exact: true }).click()
  const bytes = await downloaded(await event)
  expect(bytes[96]).toBe(0xaa)
  expect(bytes[0]).toBe(3)
})

test('sidebar follows scrolling without filtering or snapping back', async ({ page }) => {
  await page.getByRole('button', { name: 'Explore SFP+ example' }).click()
  await expect(page.locator('.configuration-section')).toHaveCount(5)
  const sections = page.locator('.configuration-section')
  const ids = await sections.evaluateAll((elements) => elements.map((el) => el.id))
  for (const id of [...ids.slice(1), ids[0]]) {
    await page.locator('#' + id).evaluate((el) => el.scrollIntoView({ block: 'start' }))
    const expected = id.replace('section-', '')
    await expect
      .poll(() =>
        page
          .locator('#workspace-navigation button[aria-current="page"]')
          .getAttribute('data-section'),
      )
      .toBe(expected)
    await expect(sections).toHaveCount(5)
  }
})

test('hex remains populated while editing and format selection includes hex', async ({ page }) => {
  await page.getByRole('button', { name: 'Explore SFP+ example' }).click()
  const exportButton = page.getByRole('button', { name: 'Export configuration', exact: true })
  await expect(exportButton).toBeEnabled()
  const before = await page.locator('.workspace-file').boundingBox()
  const vendorBytes = page.getByRole('textbox', { name: 'Vendor Specific hex bytes', exact: true })
  const originalBytes = await vendorBytes.inputValue()
  await page.evaluate(() => {
    const observations: string[] = []
    const observer = new MutationObserver(() => {
      for (const code of document.querySelectorAll('.field-byte-strip code')) {
        if (code.textContent?.includes('unavailable')) observations.push(code.textContent)
      }
      const vendor = document.querySelector<HTMLTextAreaElement>(
        '[aria-label="Vendor Specific hex bytes"]',
      )
      if (!vendor?.value) observations.push('Vendor hex cleared')
    })
    observer.observe(document.querySelector('.app-main')!, {
      childList: true,
      characterData: true,
      subtree: true,
      attributes: true,
    })
    Object.assign(window, { hexObservations: observations, hexObserver: observer })
  })
  const vendorName = page.getByRole('textbox', { name: 'Vendor Name', exact: true })
  await vendorName.fill('UPDATED MFG')
  await expect(exportButton).toBeEnabled()
  expect((await page.locator('.workspace-file').boundingBox())?.height).toBe(before?.height)
  await expect(vendorBytes).toHaveValue(originalBytes)
  const vendorField = vendorName.locator('xpath=ancestor::div[contains(@class,"byte-field")][1]')
  await expect(vendorField.locator('.field-byte-strip code')).toContainText('55 50 44 41 54 45 44')
  await vendorName.fill('THIS VENDOR NAME IS TOO LONG')
  await expect(exportButton).toBeDisabled()
  await expect(vendorField.locator('.field-byte-strip code')).toContainText('Hex (last valid)')
  await expect(vendorBytes).toHaveValue(originalBytes)
  await vendorName.fill('Example Mfg.')
  await expect(exportButton).toBeEnabled()
  expect(
    await page.evaluate(() => {
      const state = window as unknown as {
        hexObserver: MutationObserver
        hexObservations: string[]
      }
      state.hexObserver.disconnect()
      return state.hexObservations
    }),
  ).toEqual([])
  const identifierFormat = page.getByLabel('Identifier value format', { exact: true })
  await choose(identifierFormat, { label: 'Hex bytes' })
  const identifierBytes = page.getByRole('textbox', { name: 'Identifier hex bytes', exact: true })
  await expect(identifierBytes).toHaveValue('03')
  await choose(identifierFormat, { label: 'Named value' })
  await expect(page.getByLabel('Identifier', { exact: true })).toHaveText('SFP or SFP+ (Hex: 03)')
  const vendorFormat = page.getByLabel('Vendor Specific value format', { exact: true })
  await choose(vendorFormat, { label: 'Base64' })
  await expect(vendorBytes).toHaveCount(0)
  await expect(exportButton).toBeEnabled()
  await choose(vendorFormat, { label: 'Hex bytes' })
  await expect(vendorBytes).toHaveValue(originalBytes)
  await expect(page.getByRole('button', { name: 'Edit hex', exact: true })).toHaveCount(0)
})

test('whole-byte hex and binary controls update shared toggles in both directions', async ({
  page,
}) => {
  await page.getByRole('button', { name: 'Explore SFP+ example' }).click()
  const exportButton = page.getByRole('button', { name: 'Export configuration', exact: true })
  await expect(exportButton).toBeEnabled()
  await page.getByRole('button', { name: 'Compliance', exact: false }).click()
  const ethernet = page.getByRole('group', {
    name: '10G Ethernet Compliance Codes bytes',
    exact: true,
  })
  const infiniband = page.getByRole('group', {
    name: 'Infiniband Compliance Codes bytes',
    exact: true,
  })
  const byte = ethernet.getByRole('textbox', { name: '10G Ethernet Compliance Codes byte 3 hex' })
  await expect(byte).toHaveValue('20')
  await expect(ethernet.locator('code')).toHaveText('Bin: 0b00100000')
  const lr = page.getByRole('switch', { name: '10GBASE-LR compliant (Bit 5)', exact: true })
  const sr = page.getByRole('switch', { name: '10GBASE-SR compliant (Bit 4)', exact: true })
  await expect(lr).toBeChecked()
  await expect(
    lr.locator('xpath=ancestor::div[contains(@class,"field-location")][1]'),
  ).toHaveAttribute('title', 'Byte 3 (0x03) · bit 5')
  await expect(
    page.getByLabel('10GBASE-LR compliant (Bit 5) value format', { exact: true }),
  ).toHaveCount(0)
  await byte.fill('1')
  await expect(exportButton).toBeDisabled()
  await byte.fill('19')

  await expect(exportButton).toBeEnabled()
  await expect(lr).not.toBeChecked()
  await expect(sr).toBeChecked()
  await expect(
    page.getByRole('switch', { name: '1X SX compliant (Bit 3)', exact: true }),
  ).toBeChecked()
  await expect(infiniband.getByRole('textbox')).toHaveValue('19')
  await expect(ethernet.locator('code')).toHaveText('Bin: 0b00011001')
  await lr.click()
  await expect(byte).toHaveValue('39')
  await expect(infiniband.getByRole('textbox')).toHaveValue('39')
  await expect(ethernet.locator('code')).toHaveText('Bin: 0b00111001')
})

test('named bytes default to JSON names, fall back for unknown values and allow explicit raw output', async ({
  page,
}) => {
  await page.getByRole('button', { name: 'Explore SFP+ example' }).click()
  const exportButton = page.getByRole('button', { name: 'Export configuration', exact: true })
  await expect(exportButton).toBeEnabled()
  async function identifierJSON() {
    await exportButton.click()
    const download = page.waitForEvent('download')
    await page.getByRole('button', { name: 'Download JSON', exact: true }).click()
    const document = JSON.parse((await downloaded(await download)).toString())
    await page.getByRole('button', { name: 'Close dialog' }).click()
    return document.Identifier
  }
  await choose(page.getByLabel('Identifier value format', { exact: true }), 'hex')
  const field = page.getByRole('textbox', { name: 'Identifier hex bytes', exact: true })
  async function apply(value: string) {
    await field.fill(value)

    await expect(exportButton).toBeEnabled()
  }
  await apply('01')
  await expect(field).toHaveAccessibleDescription('GBIC')
  expect(await identifierJSON()).toBe('GBIC')
  await apply('FF')
  await expect(field).toHaveAccessibleDescription('No corresponding named value')
  expect(await identifierJSON()).toEqual({ byteValue: '0xff' })
  await apply('03')
  expect(await identifierJSON()).toBe('SFP or SFP+')
  const raw = page.getByRole('checkbox', { name: 'Identifier write as raw byte', exact: true })
  await raw.check()
  await expect(exportButton).toBeEnabled()
  expect(await identifierJSON()).toEqual({ byteValue: '0x03' })
  await choose(page.getByLabel('Identifier value format', { exact: true }), 'named')
  await choose(page.getByLabel('Identifier', { exact: true }), { value: 'GBIC' })
  await expect(exportButton).toBeEnabled()
  expect(await identifierJSON()).toEqual({ byteValue: '0x01' })
  await raw.uncheck()
  await expect(exportButton).toBeEnabled()
  expect(await identifierJSON()).toBe('GBIC')
  await expect(
    page
      .getByLabel('Identifier', { exact: true })
      .locator('xpath=ancestor::div[contains(@class,"field-location")][1]'),
  ).toHaveAttribute('title', 'Byte 0 (0x00) · bits 7–0 (whole byte)')
})

test('named-byte output preference does not resize either dropdown', async ({ page }) => {
  await page.getByRole('button', { name: 'Explore SFP+ example' }).click()
  const exportButton = page.getByRole('button', { name: 'Export configuration', exact: true })
  await expect(exportButton).toBeEnabled()
  const value = page.getByLabel('Identifier', { exact: true })
  const format = page.getByLabel('Identifier value format', { exact: true })
  const valueWidth = (await value.boundingBox())!.width
  const formatWidth = (await format.boundingBox())!.width
  const toggle = page.getByRole('checkbox', { name: 'Identifier write as raw byte', exact: true })
  for (const checked of [true, false]) {
    await toggle.setChecked(checked)
    await expect(exportButton).toBeEnabled()
    expect((await value.boundingBox())!.width).toBe(valueWidth)
    expect((await format.boundingBox())!.width).toBe(formatWidth)
  }
})

test('OUI format changes preserve unknown and known vendor identifiers', async ({ page }) => {
  await page.getByRole('button', { name: 'Explore SFP+ example' }).click()
  const exportButton = page.getByRole('button', { name: 'Export configuration', exact: true })
  await expect(exportButton).toBeEnabled()
  const format = page.getByLabel('Vendor OUI value format', { exact: true })
  const oui = page.getByRole('textbox', { name: 'Vendor OUI', exact: true })
  await expect(oui).toHaveValue('00:00:00')
  await choose(format, { label: 'Vendor name' })
  await expect(page.getByRole('dialog')).toHaveCount(0)
  await expect(oui).toHaveValue('00:00:00')
  await expect(page.getByRole('alert')).toContainText('No vendor in the built-in OUI list matches')
  await expect(format).toHaveText('OUI')
  await oui.fill('64:9d:99')
  await expect(exportButton).toBeEnabled()
  await choose(format, { label: 'Vendor name' })
  await expect(page.getByRole('dialog')).toHaveCount(0)
  const group = format.locator('xpath=ancestor::div[contains(@class,"byte-field")][1]')
  await expect(group.getByRole('combobox', { name: 'Vendor Name', exact: true })).toHaveText(
    'FS COM INC (OUI: 64:9D:99)',
  )
  for (const option of await (
    await menuOptions(group.getByRole('combobox', { name: 'Vendor Name', exact: true }))
  ).allTextContents()) {
    expect(option).toMatch(/ \(OUI: [0-9A-F]{2}:[0-9A-F]{2}:[0-9A-F]{2}\)$/)
  }
  await choose(format, { label: 'OUI' })
  await expect(oui).toHaveValue('64:9D:99')
  await expect(exportButton).toBeEnabled()
})

test('named-byte dropdown options include codec-derived hexadecimal values', async ({ page }) => {
  await page.getByRole('button', { name: 'Explore QSFP+ example' }).click()
  await expect(
    page.getByRole('button', { name: 'Export configuration', exact: true }),
  ).toBeEnabled()
  const identifier = page.getByLabel('Identifier', { exact: true })
  await expect(
    (await menuOptions(identifier)).filter({ hasText: /^GBIC \(Hex: 01\)$/ }),
  ).toHaveText('GBIC (Hex: 01)')
  const compliance = page.getByLabel('Extended Specification Compliance Codes', { exact: true })
  await expect
    .poll(async () =>
      (await (await menuOptions(compliance)).allTextContents()).every((text) =>
        / \(Hex: [0-9A-F]{2}\)$/.test(text),
      ),
    )
    .toBe(true)
})

test('dropdowns show long labels and allow page scrolling while open', async ({ page }) => {
  await page.getByRole('button', { name: 'Explore QSFP+ example' }).click()
  const field = page.getByLabel('Extended Specification Compliance Codes', { exact: true })
  await expect(field).toBeEnabled()
  const options = await menuOptions(field)
  const popup = page.getByRole('listbox')
  const box = (await popup.boundingBox())!
  expect(box.x).toBeGreaterThanOrEqual(0)
  expect(box.x + box.width).toBeLessThanOrEqual(page.viewportSize()!.width)
  expect(await popup.evaluate((el) => el.scrollWidth <= el.clientWidth)).toBe(true)
  expect(await options.count()).toBeGreaterThan(10)
  const before = await page.evaluate(() => scrollY)
  await page.mouse.move(page.viewportSize()!.width - 5, 100)
  await page.mouse.wheel(0, 120)
  await expect.poll(() => page.evaluate(() => scrollY)).toBeGreaterThan(before)
  await expect(popup).toBeVisible()
  await page.keyboard.press('Escape')
  await expect(popup).toHaveCount(0)
})

test('Base64 uses one text editor and converts edited bytes immediately', async ({ page }) => {
  await page.getByRole('button', { name: 'Explore SFP+ example' }).click()
  const exportButton = page.getByRole('button', { name: 'Export configuration', exact: true })
  await expect(exportButton).toBeEnabled()
  const format = page.getByLabel('Vendor Specific value format', { exact: true })
  await choose(format, { label: 'Base64' })
  const text = page.getByRole('textbox', { name: 'Vendor Specific Base64', exact: true })
  await expect(text).toBeVisible()
  const field = text.locator('xpath=ancestor::div[contains(@class,"byte-field")][1]')
  await expect(field.locator('.constant-value')).toHaveCount(0)
  await expect(field.locator('.field-byte-strip')).toHaveCount(1)
  await expect(field.getByRole('combobox')).toHaveCount(1)
  await text.fill(Buffer.from(Array(32).fill(0xaa)).toString('base64'))
  await expect(exportButton).toBeEnabled()
  await choose(format, { label: 'Hex bytes' })
  await expect(
    page.getByRole('textbox', { name: 'Vendor Specific hex bytes', exact: true }),
  ).toHaveValue(Array(32).fill('AA').join(' '))
  await expect(
    page.getByRole('button', { name: /Apply (hex|byte)|Discard (hex|byte)/ }),
  ).toHaveCount(0)
})

test('rapid byte corrections keep the latest input and custom menus support keyboard selection', async ({
  page,
}) => {
  await page.getByRole('button', { name: 'Explore SFP+ example' }).click()
  const exportButton = page.getByRole('button', { name: 'Export configuration', exact: true })
  await expect(exportButton).toBeEnabled()
  const byte = page.getByRole('textbox', {
    name: '10G Ethernet Compliance Codes byte 3 hex',
    exact: true,
  })
  await byte.fill('19')
  await byte.fill('20')
  await expect(exportButton).toBeEnabled()
  await expect(byte).toHaveValue('20')
  await expect(
    page.getByRole('switch', { name: '10GBASE-LR compliant (Bit 5)', exact: true }),
  ).toBeChecked()
  await expect(
    page.getByRole('switch', { name: '10GBASE-SR compliant (Bit 4)', exact: true }),
  ).not.toBeChecked()
  const identifier = page.getByLabel('Identifier', { exact: true })
  await identifier.focus()
  await page.keyboard.press('ArrowDown')
  await expect(page.getByRole('listbox')).toBeVisible()
  await page.keyboard.press('Home')
  await page.keyboard.press('ArrowDown')
  await page.keyboard.press('Enter')
  await expect(identifier).toHaveText('GBIC (Hex: 01)')
  await expect(page.getByRole('listbox')).toHaveCount(0)
  await expect(identifier).toBeFocused()
})
