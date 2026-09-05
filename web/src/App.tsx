// SPDX-FileCopyrightText: 2026 Robin Christ
// SPDX-License-Identifier: MPL-2.0
import { useCallback, useEffect, useMemo, useRef, useState } from 'react'
import {
  Activity,
  ArrowDownToLine,
  ArrowRight,
  Braces,
  Cable,
  Check,
  CheckCheck,
  ChevronRight,
  CircleHelp,
  Cpu,
  FileCode2,
  FilePlus2,
  FileUp,
  Fingerprint,
  HardDrive,
  Layers3,
  Menu,
  Radio,
  Search,
  ShieldCheck,
  SlidersHorizontal,
  TriangleAlert,
  Undo2,
  X,
} from 'lucide-react'
import { Button } from './components/ui/button'
import { Dialog } from './components/ui/dialog'
import { SchemaField } from './components/SchemaField'
import { ModuleSummary } from './components/ModuleSummary'
import { ByteContext } from './components/FieldBytes'
import { editHex, convertFieldFormat, setAt, fieldHex, fieldLayout } from './lib/field-bytes'
import { cn } from './lib/utils'
import { convert, download, ready, sample, standardOf, type CodecResult } from './lib/codec'
import {
  groups,
  groupKeys,
  isCompositeField,
  schemaIssues,
  pointer,
  unpointer,
  schemas,
  type Document,
  type Standard,
  type Value,
} from './lib/schema'

interface Work {
  document: Document
  original: string
  standard: Standard
  fiber: boolean
  lowerBytes?: number[]
  name: string
  example: boolean
}
const icons = [Fingerprint, Radio, ShieldCheck, SlidersHorizontal, HardDrive]
const short = (v: Value | undefined, fallback = 'Not specified') =>
  typeof v === 'string' || typeof v === 'number' ? String(v) : fallback

export default function App() {
  const [work, setWork] = useState<Work | null>(null)
  const workRef = useRef(work)
  workRef.current = work
  const [hexDrafts, setHexDrafts] = useState<Set<string>>(new Set())
  const pendingHex = useCallback(
    (path: string, dirty: boolean) =>
      setHexDrafts((previous) => {
        if (previous.has(path) === dirty) return previous
        const next = new Set(previous)
        if (dirty) next.add(path)
        else next.delete(path)
        return next
      }),
    [],
  )
  const [history, setHistory] = useState<Work[]>([])
  const [group, setGroup] = useState('identity')
  const [view, setView] = useState('editor')
  const [search, setSearch] = useState('')
  const [focusPath, setFocusPath] = useState<string | null>(null)
  const [importOpen, setImportOpen] = useState(false)
  const [exportOpen, setExportOpen] = useState(false)
  const [helpOpen, setHelpOpen] = useState(false)
  const [newOpen, setNewOpen] = useState(false)
  const [sidebarOpen, setSidebarOpen] = useState(false)
  const [importStandard, setImportStandard] = useState<Standard>('8472')
  const [importFiber, setImportFiber] = useState(true)
  const [file, setFile] = useState<File | null>(null)
  const [busy, setBusy] = useState(false)
  const [engine, setEngine] = useState<'loading' | 'ready' | 'failed'>('loading')
  const [message, setMessage] = useState('')
  const [notice, setNotice] = useState('')
  const [validation, setValidation] = useState<{
    key: string
    result?: CodecResult
    error?: string
  }>({ key: '' })
  // A display snapshot survives pending/failed conversion. Only `result`, keyed to
  // the current document, may drive edits, interpretation changes, or export.
  const [byteSnapshot, setByteSnapshot] = useState<{
    standard: Standard
    fiber: boolean
    result: CodecResult
  } | null>(null)
  const [jsonDraft, setJsonDraft] = useState('')
  const [rawDirty, setRawDirty] = useState(false)
  const [pendingAction, setPendingAction] = useState<(() => void) | null>(null)
  const fileInput = useRef<HTMLInputElement>(null)
  const docText = work ? JSON.stringify(work.document, null, 2) : ''
  const key = work
    ? JSON.stringify([work.document, work.standard, work.fiber, work.lowerBytes])
    : ''
  const result = validation.key === key ? validation.result : undefined
  const error = validation.key === key ? validation.error : undefined
  const displayResult =
    result ??
    (byteSnapshot?.standard === work?.standard && byteSnapshot?.fiber === work?.fiber
      ? byteSnapshot?.result
      : undefined)
  const dirty =
    !!work && (JSON.stringify(work.document) !== work.original || rawDirty || hexDrafts.size > 0)
  const issues = useMemo(() => (work ? schemaIssues(work.standard, work.document) : []), [work])
  const errors = issues.map((issue) => `${issue.path || '/'} ${issue.message}`)
  const canExport = !!result && !rawDirty && hexDrafts.size === 0 && errors.length === 0
  const changes = work
    ? Object.keys(work.document).filter(
        (k) => JSON.stringify(work.document[k]) !== JSON.stringify(JSON.parse(work.original)[k]),
      ).length
    : 0

  useEffect(() => {
    ready()
      .then(() => setEngine('ready'))
      .catch((e) => {
        setEngine('failed')
        setMessage(String(e))
      })
  }, [])
  useEffect(() => {
    if (!work) return
    let active = true
    const timer = setTimeout(() => {
      convert({
        operation: 'encode',
        document: work.document,
        standard: work.standard,
        fiber: work.fiber,
        lowerBytes: work.lowerBytes,
      })
        .then((value) => {
          if (active) {
            setValidation({ key, result: value })
            setByteSnapshot({ standard: work.standard, fiber: work.fiber, result: value })
          }
        })
        .catch((e) => {
          if (active) setValidation({ key, error: String(e.message) })
        })
    }, 120)
    return () => {
      active = false
      clearTimeout(timer)
    }
  }, [key, work])
  useEffect(() => {
    if (!dirty) return
    const leave = (event: BeforeUnloadEvent) => {
      event.preventDefault()
      event.returnValue = ''
    }
    window.addEventListener('beforeunload', leave)
    return () => window.removeEventListener('beforeunload', leave)
  }, [dirty])
  useEffect(() => {
    if (!rawDirty) setJsonDraft(docText)
  }, [docText, rawDirty])
  useEffect(() => {
    if (!notice) return
    const id = setTimeout(() => setNotice(''), 4000)
    return () => clearTimeout(id)
  }, [notice])

  useEffect(() => {
    if (!focusPath) return
    const control = Array.from(document.querySelectorAll<HTMLElement>('[data-field-path]')).find(
      (element) => element.dataset.fieldPath === focusPath,
    )
    if (control) {
      control.focus()
      control.scrollIntoView({ block: 'center', behavior: 'smooth' })
    }
    setFocusPath(null)
  }, [focusPath, view, search, group])
  useEffect(() => {
    if (!sidebarOpen || !window.matchMedia('(max-width: 760px)').matches) return
    const navigation = document.getElementById('workspace-navigation')!
    const controls = () =>
      Array.from(navigation.querySelectorAll<HTMLButtonElement>('button:not(:disabled)'))
    controls()[0]?.focus()
    const onKey = (event: KeyboardEvent) => {
      if (!window.matchMedia('(max-width: 760px)').matches) return
      if (event.key === 'Escape') {
        event.preventDefault()
        setSidebarOpen(false)
      }
      if (event.key === 'Tab') {
        const buttons = controls()
        const first = buttons[0],
          last = buttons.at(-1)
        if (event.shiftKey && document.activeElement === first) {
          event.preventDefault()
          last?.focus()
        } else if (!event.shiftKey && document.activeElement === last) {
          event.preventDefault()
          first?.focus()
        }
      }
    }
    navigation.addEventListener('keydown', onKey)
    return () => {
      navigation.removeEventListener('keydown', onKey)
      document.querySelector<HTMLButtonElement>('[aria-controls="workspace-navigation"]')?.focus()
    }
  }, [sidebarOpen])
  useEffect(() => {
    if (view !== 'editor' || search || !work) return
    let frame = 0
    const trackSection = () => {
      frame = 0
      const sections = groups
        .map(({ id }) => document.getElementById(`section-${id}`))
        .filter((section): section is HTMLElement => !!section)
      let active: HTMLElement | undefined = sections[0]
      for (const section of sections) {
        if (section.getBoundingClientRect().top <= 140) active = section
      }
      if (window.scrollY + window.innerHeight >= document.documentElement.scrollHeight - 2)
        active = sections.at(-1)
      if (active) setGroup(active.id.replace('section-', ''))
    }
    const schedule = () => {
      if (!frame) frame = requestAnimationFrame(trackSection)
    }
    window.addEventListener('scroll', schedule, { passive: true })
    window.addEventListener('resize', schedule)
    const observer = new ResizeObserver(schedule)
    const panel = document.querySelector('.app-main')
    if (panel) observer.observe(panel)
    schedule()
    return () => {
      cancelAnimationFrame(frame)
      window.removeEventListener('scroll', schedule)
      window.removeEventListener('resize', schedule)
      observer.disconnect()
    }
  }, [view, search, work?.standard])
  function jumpToIssue(path: string) {
    const key = unpointer(path.split('/')[1] ?? '')
    const section = work && groups.find((g) => groupKeys(g.id, work.standard).includes(key))
    setGroup(section ? section.id : 'identity')
    setSearch('')
    setView('editor')
    setFocusPath(path)
  }
  function update(next: Work) {
    if (work) setHistory((h) => [...h.slice(-49), work])
    setWork(next)
  }
  function replace(action: () => void) {
    if (hexDrafts.size) {
      setMessage('Complete or correct the hex input before replacing this configuration.')
      return
    }
    if (dirty) setPendingAction(() => action)
    else action()
  }
  function load(
    document: Document,
    standard: Standard,
    name: string,
    fiber: boolean,
    example: boolean,
    lowerBytes?: number[],
  ) {
    setWork({
      document,
      original: JSON.stringify(document),
      standard,
      name,
      fiber,
      example,
      lowerBytes,
    })
    setByteSnapshot(null)
    setValidation({ key: '' })
    setHistory([])
    setGroup('identity')
    setView('editor')
    setSearch('')
    setRawDirty(false)
    setMessage('')
    window.scrollTo(0, 0)
  }
  async function create(standard: Standard, blank = false) {
    setBusy(true)
    setMessage('')
    try {
      const data = blank
        ? await convert({
            operation: 'decode',
            standard,
            fiber: true,
            bytes: Array(standard === '8472' ? 128 : 256).fill(0),
          })
        : await sample(standard)
      load(
        data.document,
        standard,
        blank
          ? 'untitled.json'
          : standard === '8472'
            ? 'example-sfp-10g-lr.json'
            : 'example-qsfp28-lr4.json',
        true,
        !blank,
      )
      setNewOpen(false)
    } catch (e) {
      setMessage((e as Error).message)
    } finally {
      setBusy(false)
    }
  }
  async function importFile() {
    if (!file) return
    setBusy(true)
    setMessage('')
    try {
      if (file.size > 1024 * 1024) throw new Error('Choose a configuration smaller than 1 MiB.')
      const buffer = await file.arrayBuffer()
      if (
        file.name.toLowerCase().endsWith('.json') ||
        new TextDecoder().decode(buffer.slice(0, 64)).trimStart().startsWith('{')
      ) {
        const document = JSON.parse(new TextDecoder().decode(buffer)) as Document
        const standard = standardOf(document)
        await convert({ operation: 'encode', standard, document, fiber: importFiber })
        load(document, standard, file.name, importFiber, false)
      } else {
        const bytes = [...new Uint8Array(buffer)]
        const standard = bytes.length === 256 ? '8636' : importStandard
        if (bytes.length !== 128 && bytes.length !== 256)
          throw new Error(
            'Binary input must be exactly 128 bytes (SFP or QSFP upper page) or 256 bytes (QSFP full page).',
          )
        const data = await convert({ operation: 'decode', standard, bytes, fiber: importFiber })
        load(
          data.document,
          standard,
          file.name,
          importFiber,
          false,
          bytes.length === 256 ? bytes.slice(0, 128) : undefined,
        )
      }
      setImportOpen(false)
      setFile(null)
      setNotice('Configuration opened. Edits will not overwrite the original file.')
    } catch (e) {
      setMessage(
        `Could not open this file. ${(e as Error).message} Choose a supported binary or TransceiverTool JSON file.`,
      )
    } finally {
      setBusy(false)
    }
  }
  async function changeMode(fiber: boolean) {
    if (!work || !result || rawDirty) return
    try {
      const data = await convert({
        operation: 'decode',
        standard: work.standard,
        fiber,
        bytes: result.bytes,
      })
      update({ ...work, fiber, document: data.document })
    } catch (e) {
      setMessage((e as Error).message)
    }
  }
  async function applyJson() {
    if (!work) return
    try {
      const document = JSON.parse(jsonDraft) as Document
      const standard = standardOf(document)
      await convert({ operation: 'encode', standard, document, fiber: work.fiber })
      update({
        ...work,
        standard,
        document,
        lowerBytes: standard === work.standard ? work.lowerBytes : undefined,
      })
      setRawDirty(false)
      setMessage('')
      setNotice('JSON changes applied to the editor.')
    } catch (e) {
      setMessage(
        `JSON changes were not applied. ${(e as Error).message} Correct the draft and apply it again, or discard it to return to the current configuration.`,
      )
    }
  }
  function exportFile(format: 'json' | 'upper' | 'full') {
    if (!work || !canExport || !result) return
    const base = work.name.replace(/\.[^.]+$/, '')
    if (format === 'json') download(`${base}.json`, docText + '\n', 'application/json')
    else
      download(
        `${base}${format === 'upper' && work.standard === '8636' ? '-upper00h' : ''}.bin`,
        new Uint8Array(
          format === 'upper' && work.standard === '8636' ? result.bytes.slice(128) : result.bytes,
        ),
        'application/octet-stream',
      )
    setNotice(`${format === 'json' ? 'JSON' : 'Binary'} download started.`)
  }
  const visibleKeys = work
    ? search
      ? Object.keys(schemas[work.standard].properties ?? {}).filter(
          (k) =>
            k !== 'Type' &&
            (k.toLowerCase().includes(search.toLowerCase()) ||
              String(JSON.stringify(work.document[k]) ?? '')
                .toLowerCase()
                .includes(search.toLowerCase())),
        )
      : Object.keys(schemas[work.standard].properties ?? {}).filter((key) => key !== 'Type')
    : []
  const checks = result ? result.errors.length + result.warnings.length : 0

  return (
    <div className="app-shell">
      {sidebarOpen && (
        <button
          className="sidebar-scrim"
          aria-label="Close navigation"
          onClick={() => setSidebarOpen(false)}
        />
      )}
      <aside
        id="workspace-navigation"
        aria-label="Workspace navigation"
        className={cn('sidebar', sidebarOpen && 'is-open')}
      >
        <div className="brand">
          <span className="brand-symbol">
            <Cable size={24} />
          </span>
          <div>
            transceiver<span>STUDIO</span>
          </div>
        </div>
        <div className="workspace-label">LOCAL WORKSPACE</div>
        <button
          className="workspace-file"
          onClick={() => {
            if (work) setView('editor')
            else setImportOpen(true)
            setSidebarOpen(false)
          }}
        >
          <FileCode2 size={18} />
          <span>
            {work ? work.name : 'No configuration open'}
            <small>
              {work
                ? `SFF-${work.standard} · ${work.example ? 'Example configuration' : 'Local file'}`
                : 'Import a file to get started'}
            </small>
          </span>
          <span
            className="unsaved-dot"
            aria-hidden="true"
            style={{ visibility: dirty ? 'visible' : 'hidden' }}
          />
        </button>
        {work && (
          <>
            <div className="nav-heading">Edit configuration</div>
            <nav aria-label="Configuration sections">
              {groups.map((g, i) => {
                const Icon = icons[i]
                return (
                  <button
                    key={g.id}
                    data-section={g.id}
                    aria-current={group === g.id && view === 'editor' ? 'page' : undefined}
                    disabled={!work || (hexDrafts.size > 0 && !!search)}
                    className={cn('nav-item', group === g.id && view === 'editor' && 'active')}
                    onClick={() => {
                      setGroup(g.id)
                      requestAnimationFrame(() =>
                        document
                          .getElementById(`section-${g.id}`)
                          ?.scrollIntoView({ block: 'start' }),
                      )
                      setView('editor')
                      setSearch('')
                      setSidebarOpen(false)
                    }}
                  >
                    <Icon size={17} />
                    <span>{g.label}</span>
                    {work && <small>{groupKeys(g.id, work.standard).length}</small>}
                    <ChevronRight className="nav-chevron" size={15} aria-hidden="true" />
                  </button>
                )
              })}
            </nav>
          </>
        )}
        <div className="sidebar-bottom">
          <button className="help-link" onClick={() => setHelpOpen(true)}>
            <CircleHelp size={16} />
            Workflow & supported formats
            <ArrowRight size={14} />
          </button>
        </div>
      </aside>
      <ByteContext.Provider
        value={{
          standard: work?.standard ?? '8472',
          result,
          displayResult,
          invalid: !!error || errors.length > 0,
          locked: rawDirty,
          hasDrafts: hexDrafts.size > 0,
          pending: pendingHex,
          apply: async (path, text, wholeByte, representation) => {
            if (!work || !result) throw new Error('Resolve input errors before editing bytes.')
            const document = await editHex(
              work.document,
              result,
              work.standard,
              work.fiber,
              path,
              text,
              wholeByte,
              representation,
            )
            if (workRef.current !== work)
              throw new Error(
                'The configuration changed. Review the current bytes and apply again.',
              )
            update({ ...work, document })
          },
          setNamed: async (path, name, raw) => {
            if (!work || !result)
              throw new Error('Wait for a valid configuration before changing the byte.')
            let document = setAt(work.document, path, name)
            if (raw) {
              const encoded = await convert({
                operation: 'encode',
                standard: work.standard,
                fiber: work.fiber,
                document,
                lowerBytes: work.lowerBytes,
              })
              document = setAt(document, path, {
                byteValue: `0x${fieldHex(encoded, fieldLayout(work.standard, path))}`,
              })
            }
            if (workRef.current !== work) throw new Error('The configuration changed. Try again.')
            update({ ...work, document })
          },
          format: async (path, schema) => {
            if (!work || !result) return undefined
            const converted = await convertFieldFormat(
              work.document,
              result,
              work.standard,
              work.fiber,
              path,
              schema,
              schemas[work.standard],
            )
            if (workRef.current !== work)
              throw new Error('The configuration changed. Try the format change again.')
            return converted
          },
        }}
      >
        <div className="app-main">
          <header className="topbar">
            <div className="breadcrumb">
              <Button
                variant="ghost"
                size="icon"
                className="mobile-menu"
                aria-label="Open navigation"
                aria-expanded={sidebarOpen}
                aria-controls="workspace-navigation"
                onClick={() => setSidebarOpen(true)}
              >
                <Menu />
              </Button>
              <strong>{work ? 'Configuration editor' : 'Getting started'}</strong>
            </div>
            <div className="topbar-actions">
              <span className={cn('engine-status', engine !== 'ready' && 'loading')}>
                <span />
                {engine === 'ready'
                  ? 'WebAssembly core ready'
                  : engine === 'failed'
                    ? 'WebAssembly core unavailable'
                    : 'Loading WebAssembly core…'}
              </span>
              <Button
                variant="outline"
                size="sm"
                disabled={engine !== 'ready' || busy}
                onClick={() => replace(() => setNewOpen(true))}
              >
                <FilePlus2 />
                New
              </Button>
              <Button
                variant="outline"
                size="sm"
                onClick={() =>
                  replace(() => {
                    setImportOpen(true)
                    setMessage('')
                  })
                }
                disabled={engine !== 'ready' || busy}
              >
                <FileUp />
                Import file
              </Button>
            </div>
          </header>
          <main id="main" className="page-content">
            {message && !importOpen && (
              <div className="error-banner" role="alert">
                <TriangleAlert size={17} />
                <span>{message}</span>
                <button aria-label="Dismiss error" onClick={() => setMessage('')}>
                  <X size={16} />
                </button>
              </div>
            )}
            {!work ? (
              <div className="welcome">
                <h1>Edit a transceiver configuration</h1>
                <p>Open a binary or JSON file, edit its fields, and download your configuration.</p>
                <div
                  className="welcome-import"
                  onDragOver={(e) => e.preventDefault()}
                  onDrop={(e) => {
                    e.preventDefault()
                    setFile(e.dataTransfer.files[0] ?? null)
                    setImportOpen(true)
                  }}
                >
                  <div className="upload-symbol">
                    <FileUp size={26} />
                  </div>
                  <p>Drop a binary or JSON file, or browse your device.</p>
                  <Button onClick={() => setImportOpen(true)} disabled={engine !== 'ready'}>
                    <FileUp />
                    Open configuration
                  </Button>
                  <span className="supported">SFF-8472 · SFF-8636 · .bin · .json</span>
                </div>
                <div className="sample-row">
                  <div>
                    <p>No file handy? Try an example.</p>
                  </div>
                  <Button
                    variant="outline"
                    onClick={() => create('8472')}
                    disabled={engine !== 'ready' || busy}
                  >
                    Explore SFP+ example
                    <ArrowRight />
                  </Button>
                  <Button
                    variant="outline"
                    onClick={() => create('8636')}
                    disabled={engine !== 'ready' || busy}
                  >
                    Explore QSFP+ example
                    <ArrowRight />
                  </Button>
                </div>
              </div>
            ) : (
              <>
                <div className="page-heading">
                  <div>
                    <h1>
                      {[
                        short(work.document['Vendor Name'], ''),
                        short(work.document['Vendor Part Number'], 'Untitled configuration'),
                      ]
                        .filter(Boolean)
                        .join(' ')}
                    </h1>
                    <div className="file-meta">
                      <FileCode2 size={14} />
                      {work.name}
                      <span className="meta-separator">/</span>
                      <span className={cn('document-status', dirty && 'dirty')}>
                        {dirty
                          ? `${changes} field${changes === 1 ? '' : 's'} modified${rawDirty ? ' · JSON draft' : ''}`
                          : work.example
                            ? 'Example configuration'
                            : 'Original configuration'}
                      </span>
                    </div>
                  </div>
                  <div className="heading-actions">
                    <Button
                      variant="outline"
                      disabled={history.length === 0 || rawDirty || hexDrafts.size > 0}
                      onClick={() => {
                        setWork(history.at(-1)!)
                        setHistory(history.slice(0, -1))
                      }}
                      aria-label="Undo last change"
                    >
                      <Undo2 />
                      Undo
                    </Button>
                    <Button disabled={!canExport} onClick={() => setExportOpen(true)}>
                      <ArrowDownToLine />
                      Export configuration
                    </Button>
                  </div>
                </div>
                <details className="module-details">
                  <summary>
                    <span>
                      <strong>{short(work.document['Vendor Name'], 'Unknown manufacturer')}</strong>
                      {' · '}
                      {short(work.document['Vendor Part Number'], 'Unnamed module')}
                      {' · '}
                      {moduleFormFactor(result?.document ?? work.document)} (SFF-{work.standard})
                    </span>
                    <span>Module details & interpretation</span>
                  </summary>
                  <section className="module-summary" aria-label="Module overview">
                    <ModuleSummary
                      document={displayResult?.document}
                      standard={work.standard}
                      fiber={work.fiber}
                    />
                    <div className="interpretation">
                      <span className="interpretation-label">Interpretation</span>
                      <div className="segmented">
                        <button
                          aria-pressed={work.fiber}
                          className={cn(work.fiber && 'selected')}
                          disabled={!result || rawDirty || hexDrafts.size > 0}
                          onClick={() => changeMode(true)}
                        >
                          <Radio size={14} />
                          Fiber
                        </button>
                        <button
                          aria-pressed={!work.fiber}
                          className={cn(!work.fiber && 'selected')}
                          disabled={!result || rawDirty || hexDrafts.size > 0}
                          onClick={() => changeMode(false)}
                        >
                          <Cable size={14} />
                          Copper
                        </button>
                      </div>
                      <p>Interpret the same bytes as optical or copper fields.</p>
                    </div>
                  </section>
                </details>
                {hexDrafts.size > 0 && (
                  <div className="draft-banner" role="status">
                    Complete or correct the hex input before exporting or replacing this
                    configuration.
                  </div>
                )}
                {(issues.length > 0 || error) && (
                  <div className="issue-summary" role="region" aria-label="Input problems">
                    <strong role="status">
                      {issues.length
                        ? `${issues.length} input issue${issues.length === 1 ? '' : 's'}`
                        : 'Binary conversion failed'}{' '}
                      — fix these values to export
                    </strong>
                    {issues.length ? (
                      <div>
                        {issues.map((issue, i) => (
                          <button key={i} onClick={() => jumpToIssue(issue.path)}>
                            {issue.path.split('/').slice(1).map(unpointer).join(' › ')}:{' '}
                            {issue.message}
                          </button>
                        ))}
                      </div>
                    ) : (
                      <p>{error}</p>
                    )}
                  </div>
                )}
                <details className="validation-details">
                  <summary>
                    <ShieldCheck size={16} aria-hidden="true" />
                    <span>
                      {error || errors.length
                        ? 'Fix input to export'
                        : rawDirty
                          ? 'Apply or discard the JSON draft to export'
                          : result
                            ? 'Ready to export'
                            : 'Checking configuration…'}
                    </span>
                    {result && checks > 0 && <span>{checks} standard advisories</span>}
                    <span className="details-action">Validation details</span>
                  </summary>
                  <div className="validation-body">
                    <div className="check-row">
                      <span>JSON schema</span>
                      <span className={errors.length ? 'text-amber-700' : 'text-emerald-700'}>
                        {errors.length ? `${errors.length} issues` : 'Valid'}
                        {!errors.length && <Check size={13} />}
                      </span>
                    </div>
                    <div className="check-row">
                      <span>Binary conversion</span>
                      <span className={error ? 'text-red-600' : 'text-emerald-700'}>
                        {error ? 'Failed' : result ? 'Valid' : 'Checking…'}
                        {result && <Check size={13} />}
                      </span>
                    </div>
                    <div className="check-row">
                      <span>Standard checks</span>
                      <span>{result ? (checks ? `${checks} advisories` : 'Passed') : '—'}</span>
                    </div>
                    {result && checks > 0 && (
                      <details className="advisories">
                        <summary>
                          <TriangleAlert size={14} />
                          Review {checks} standard advisories
                        </summary>
                        <p>
                          These checks report standard-compliance issues. Export remains available
                          so you can preserve the current bytes. A successful export does not
                          establish device compatibility.
                        </p>
                        {result.errors.map((e, i) => (
                          <p key={`e${i}`}>
                            <strong>Error</strong> {e}
                          </p>
                        ))}
                        {result.warnings.map((e, i) => (
                          <p key={`w${i}`}>
                            <strong>Warning</strong> {e}
                          </p>
                        ))}
                      </details>
                    )}
                  </div>
                </details>
                <div className="editor-layout">
                  <section className="editor-main">
                    <div className="editor-toolbar">
                      <div className="view-tabs" role="tablist" aria-label="Configuration view">
                        {[
                          ['editor', 'Visual editor', SlidersHorizontal],
                          ['json', 'JSON', Braces],
                          ['bytes', 'Bytes', Layers3],
                        ].map(([id, label, Icon]) => {
                          const TabIcon = Icon as typeof Braces
                          return (
                            <button
                              key={String(id)}
                              role="tab"
                              id={`view-tab-${id}`}
                              aria-controls="configuration-panel"
                              tabIndex={view === id ? 0 : -1}
                              disabled={hexDrafts.size > 0 && view !== id}
                              onKeyDown={(event) => {
                                if (hexDrafts.size > 0) return
                                const tabs = ['editor', 'json', 'bytes']
                                const index = tabs.indexOf(view)
                                const next =
                                  event.key === 'ArrowRight'
                                    ? (index + 1) % tabs.length
                                    : event.key === 'ArrowLeft'
                                      ? (index + tabs.length - 1) % tabs.length
                                      : event.key === 'Home'
                                        ? 0
                                        : event.key === 'End'
                                          ? tabs.length - 1
                                          : -1
                                if (next < 0) return
                                event.preventDefault()
                                setView(tabs[next])
                                document.getElementById(`view-tab-${tabs[next]}`)?.focus()
                              }}
                              aria-selected={view === id}
                              className={cn(view === id && 'active')}
                              onClick={() => setView(String(id))}
                            >
                              <TabIcon size={15} />
                              {String(label)}
                            </button>
                          )
                        })}
                      </div>
                      {view === 'editor' && (
                        <div className="search-box">
                          <Search size={15} />
                          <input
                            aria-label="Search configuration fields"
                            placeholder="Search fields or values…"
                            disabled={hexDrafts.size > 0}
                            value={search}
                            onChange={(e) => setSearch(e.target.value)}
                          />
                          {search && (
                            <button aria-label="Clear search" onClick={() => setSearch('')}>
                              <X size={13} />
                            </button>
                          )}
                        </div>
                      )}
                    </div>
                    <div
                      id="configuration-panel"
                      role="tabpanel"
                      aria-labelledby={`view-tab-${view}`}
                      tabIndex={0}
                    >
                      {rawDirty && view !== 'json' && (
                        <div className="draft-banner">
                          Your JSON draft has not been applied. Apply or discard it to resume visual
                          editing.
                          <button onClick={() => setView('json')}>
                            Review draft <ArrowRight size={14} />
                          </button>
                        </div>
                      )}
                      {view === 'editor' && (
                        <>
                          {(search
                            ? [
                                {
                                  id: 'search',
                                  label: 'Search results',
                                  caption: `${visibleKeys.length} matching fields`,
                                  keys: visibleKeys,
                                },
                              ]
                            : groups.map((section) => ({
                                ...section,
                                keys: groupKeys(section.id, work.standard),
                              }))
                          ).map((section) => (
                            <section
                              id={`section-${section.id}`}
                              className="configuration-section"
                              key={section.id}
                              aria-labelledby={`heading-${section.id}`}
                            >
                              <div className="section-heading">
                                <div>
                                  <h2 id={`heading-${section.id}`}>{section.label}</h2>
                                  <p>{section.caption}</p>
                                </div>
                              </div>
                              <fieldset className="fields-grid" disabled={rawDirty}>
                                {section.keys.map((name) => (
                                  <div
                                    className={cn(
                                      'field-card',
                                      isCompositeField(
                                        schemas[work.standard].properties![name],
                                        schemas[work.standard],
                                      ) && 'wide-field',
                                    )}
                                    key={name}
                                  >
                                    <SchemaField
                                      path={`/${pointer(name)}`}
                                      issues={issues}
                                      name={name}
                                      schema={schemas[work.standard].properties![name]}
                                      root={schemas[work.standard]}
                                      value={work.document[name]}
                                      onChange={(value) =>
                                        update({
                                          ...work,
                                          document: { ...work.document, [name]: value },
                                        })
                                      }
                                    />
                                  </div>
                                ))}
                              </fieldset>
                            </section>
                          ))}
                          {visibleKeys.length === 0 && (
                            <div className="empty-search">
                              <Search />
                              <h3>No fields match your search</h3>
                              <p>
                                Search field names or current values, for example “vendor” or
                                “checksum”.
                              </p>
                            </div>
                          )}
                        </>
                      )}
                      {view === 'json' && (
                        <div className="source-panel">
                          <div className="source-heading">
                            <div>
                              <h2>JSON source</h2>
                              <p>
                                Edit the configuration below, then select Apply JSON to update the
                                visual editor.
                              </p>
                            </div>
                            <Button size="sm" disabled={!rawDirty} onClick={applyJson}>
                              <Check />
                              Apply JSON
                            </Button>
                          </div>
                          <textarea
                            aria-label="JSON source"
                            className="json-editor"
                            spellCheck={false}
                            value={jsonDraft}
                            onChange={(e) => {
                              setJsonDraft(e.target.value)
                              setRawDirty(e.target.value !== docText)
                            }}
                          />
                          <div className="source-footer">
                            <span>{jsonDraft.split('\n').length} lines · UTF-8</span>
                            {rawDirty ? (
                              <button
                                onClick={() => {
                                  setRawDirty(false)
                                  setJsonDraft(docText)
                                  setMessage('')
                                }}
                              >
                                Discard draft
                              </button>
                            ) : (
                              <span>In sync with visual editor</span>
                            )}
                          </div>
                        </div>
                      )}
                      {view === 'bytes' && (
                        <div className="source-panel">
                          <div className="source-heading">
                            <div>
                              <h2>Binary inspector</h2>
                              <p>
                                Binary output for the current configuration. Changes from the
                                original are highlighted.
                              </p>
                            </div>
                            <span className="standard-badge">
                              {work.standard === '8472' ? 'A0h' : 'Page 00h'}
                            </span>
                          </div>
                          {result ? (
                            <ByteView result={result} work={work} />
                          ) : (
                            <div className="empty-search">
                              A binary preview is available after conversion succeeds. Resolve any
                              input errors shown above.
                            </div>
                          )}
                          <div className="source-footer">
                            {work.standard === '8636'
                              ? work.lowerBytes
                                ? 'Lower page: preserved from imported file · Upper page: encoded from editor'
                                : 'Upper 128 bytes only · Lower page was not supplied'
                              : '128 bytes · Lower page A0h'}
                          </div>
                        </div>
                      )}
                    </div>
                  </section>
                </div>
              </>
            )}
          </main>
        </div>
      </ByteContext.Provider>
      <Dialog
        open={importOpen}
        onOpenChange={setImportOpen}
        title="Open a configuration"
        description="Open a binary EEPROM dump or a TransceiverTool JSON file. A successful import replaces the configuration in this tab; the original file is unchanged."
      >
        <div
          className="dialog-upload"
          onDragOver={(e) => e.preventDefault()}
          onDrop={(e) => {
            e.preventDefault()
            setFile(e.dataTransfer.files[0] ?? null)
          }}
        >
          <FileUp size={26} />
          <strong>{file?.name ?? 'Choose a file or drop it here'}</strong>
          <span>
            {file ? `${file.size.toLocaleString()} bytes` : '.bin, .json, or a raw EEPROM dump'}
          </span>
          <Button variant="outline" size="sm" onClick={() => fileInput.current?.click()}>
            Browse files
          </Button>
          <input
            ref={fileInput}
            type="file"
            aria-label="Configuration file"
            className="sr-only"
            onChange={(e) => setFile(e.target.files?.[0] ?? null)}
          />
        </div>
        <label className="field-label" htmlFor="import-standard">
          Interpret 128-byte binary as
        </label>
        <select
          id="import-standard"
          className="field-input mt-2"
          value={importStandard}
          onChange={(e) => setImportStandard(e.target.value as Standard)}
        >
          <option value="8472">SFP · SFF-8472 A0h</option>
          <option value="8636">QSFP · SFF-8636 00h</option>
        </select>
        <p className="field-description mt-2">
          For 128-byte dumps, choose SFP Lower A0h or QSFP Upper 00h. JSON uses its Type field; a
          256-byte dump is read as a full QSFP page.
        </p>
        <label className="field-label mt-5 block" htmlFor="import-mode">
          Shared byte interpretation
        </label>
        <select
          id="import-mode"
          className="field-input mt-2"
          value={importFiber ? 'fiber' : 'copper'}
          onChange={(e) => setImportFiber(e.target.value === 'fiber')}
        >
          <option value="fiber">Fiber / optical</option>
          <option value="copper">Copper / direct attach</option>
        </select>
        {message && (
          <p className="text-sm text-red-600 mt-4" role="alert">
            {message}
          </p>
        )}
        <Button
          className="w-full mt-6"
          onClick={importFile}
          disabled={!file || busy || engine !== 'ready'}
        >
          {busy ? 'Reading configuration…' : 'Open configuration'}
          <ArrowRight />
        </Button>
      </Dialog>
      <Dialog
        open={newOpen}
        onOpenChange={setNewOpen}
        title="Create a configuration"
        description="Choose an example or start with all bytes set to zero. Examples illustrate the fields; they are not device-qualified presets."
      >
        {(['8472', '8636'] as Standard[]).map((s) => (
          <div className="new-option" key={s}>
            <div>
              <strong>{s === '8472' ? 'SFP / SFP+' : 'QSFP / QSFP28'}</strong>
              <p>SFF-{s} · 128 editable bytes</p>
            </div>
            <Button variant="outline" size="sm" disabled={busy} onClick={() => create(s, true)}>
              Blank
            </Button>
            <Button size="sm" disabled={busy} onClick={() => create(s)}>
              Example
            </Button>
          </div>
        ))}
      </Dialog>
      <Dialog
        open={exportOpen}
        onOpenChange={setExportOpen}
        title="Export configuration"
        description="Download the editable JSON configuration or encoded binary bytes. Binary output uses your current checksum settings."
      >
        <div className="export-options">
          <Button variant="outline" onClick={() => exportFile('json')} disabled={!canExport}>
            <Braces />
            Download JSON
            <ArrowDownToLine />
          </Button>
          {work?.standard === '8636' && (
            <p className="field-description">
              JSON contains the editable upper page only. Keep the original binary if you need the
              lower page.
            </p>
          )}
          <Button variant="outline" onClick={() => exportFile('upper')} disabled={!canExport}>
            <Cpu />
            Download {work?.standard === '8636' ? 'upper page' : 'binary'} · 128 bytes
            <ArrowDownToLine />
          </Button>
          {work?.standard === '8636' && (
            <>
              <Button
                variant="outline"
                disabled={!canExport || !work.lowerBytes}
                onClick={() => exportFile('full')}
              >
                <Layers3 />
                Download full page · 256 bytes
                <ArrowDownToLine />
              </Button>
              <p className="field-description">
                {work.lowerBytes
                  ? 'The full-page download combines the unchanged lower 128 bytes from your import with the edited upper 128 bytes.'
                  : 'To download all 256 bytes, open the original 256-byte QSFP dump. A JSON file contains only the upper page.'}
              </p>
            </>
          )}
        </div>
      </Dialog>
      <Dialog
        open={helpOpen}
        onOpenChange={setHelpOpen}
        title="Editing and export guide"
        description="Transceiver Studio runs TransceiverTool entirely in your browser."
      >
        <ol className="help-steps">
          <li>
            <strong>01 · Open</strong>
            <p>
              Import SFF-8472 Lower A0h (128 bytes), SFF-8636 Upper 00h (128 bytes), a full QSFP
              page (256 bytes), or a TransceiverTool JSON file. Choose the standard explicitly for
              128-byte dumps.
            </p>
          </li>
          <li>
            <strong>02 · Configure</strong>
            <p>
              Edit named values, raw bytes, flags, and nested properties. Review replacements when
              changing a field’s format. Fiber/copper switching reinterprets the same encoded bytes.
            </p>
          </li>
          <li>
            <strong>03 · Inspect & export</strong>
            <p>
              Review format errors and standard advisories. Use “auto” for calculated checksums or a
              raw byte to preserve an explicit value. Export JSON or binary. Keep the original full
              QSFP dump if you need its lower page.
            </p>
          </li>
        </ol>
        <p className="help-footnote">
          No account, uploads, or server are used. Changes live in this tab; download them before
          closing it. Unknown JSON properties survive visual edits, but only modeled fields affect
          the binary.
        </p>
      </Dialog>
      <Dialog
        open={!!pendingAction}
        onOpenChange={(open) => {
          if (!open) setPendingAction(null)
        }}
        title="Replace this configuration?"
        description="Replacing this configuration discards its edits, including any unapplied JSON draft. To keep your changes, apply any JSON draft and download a copy first."
      >
        <div className="flex justify-end gap-3 mt-6">
          <Button variant="outline" onClick={() => setPendingAction(null)}>
            Keep editing
          </Button>
          <Button
            onClick={() => {
              const action = pendingAction
              setPendingAction(null)
              action?.()
            }}
          >
            Choose replacement
          </Button>
        </div>
      </Dialog>
      {notice && (
        <div className="toast" role="status">
          <CheckCheck size={18} />
          {notice}
        </div>
      )}
    </div>
  )
}

function ByteView({ result, work }: { result: CodecResult; work: Work }) {
  const [original, setOriginal] = useState<number[]>([])
  useEffect(() => {
    let active = true
    convert({
      operation: 'encode',
      standard: work.standard,
      fiber: work.fiber,
      document: JSON.parse(work.original),
      lowerBytes: work.lowerBytes,
    })
      .then((r) => {
        if (active) setOriginal(r.bytes)
      })
      .catch(() => {})
    return () => {
      active = false
    }
  }, [work.original, work.standard, work.fiber, work.lowerBytes])
  const offset = work.standard === '8636' && !work.lowerBytes ? 128 : 0
  return (
    <div className="hex-scroll">
      <p className="byte-change-summary">
        {original.length
          ? `${result.bytes.slice(offset).filter((b, i) => b !== original[offset + i]).length} bytes changed from the original configuration`
          : 'Comparing with original configuration…'}
      </p>
      <table className="hex-table">
        <thead>
          <tr>
            <th>OFFSET</th>
            <th>HEX DATA</th>
            <th>ASCII</th>
          </tr>
        </thead>
        <tbody>
          {Array.from({ length: (result.bytes.length - offset) / 16 }, (_, row) => {
            const start = offset + row * 16
            const chunk = result.bytes.slice(start, start + 16)
            return (
              <tr key={start}>
                <th>{start.toString(16).padStart(4, '0').toUpperCase()}</th>
                <td>
                  {chunk.map((b, i) => (
                    <span
                      key={i}
                      className={cn(original.length && original[start + i] !== b && 'changed-byte')}
                    >
                      {b.toString(16).padStart(2, '0').toUpperCase()}
                    </span>
                  ))}
                </td>
                <td className="ascii">
                  {chunk.map((b) => (b >= 32 && b <= 126 ? String.fromCharCode(b) : '·')).join('')}
                </td>
              </tr>
            )
          })}
        </tbody>
      </table>
    </div>
  )
}

function moduleFormFactor(document: Document): string {
  const identifier = document.Identifier
  if (typeof identifier !== 'string') return 'Unknown form factor'
  if (identifier === 'SFP or SFP+') return 'SFP/SFP+'
  if (identifier.startsWith('QSFP28')) return 'QSFP28'
  if (identifier.startsWith('QSFP+')) return 'QSFP+'
  if (identifier.startsWith('QSFP (')) return 'QSFP'
  return identifier
}
