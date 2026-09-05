// SPDX-FileCopyrightText: 2026 Robin Christ
// SPDX-License-Identifier: MPL-2.0
import {
  Children,
  isValidElement,
  useId,
  useLayoutEffect,
  useRef,
  useState,
  type ChangeEvent,
  type ReactNode,
  type SelectHTMLAttributes,
} from 'react'
import { createPortal } from 'react-dom'
import { ChevronDown, Check } from 'lucide-react'

type Props = SelectHTMLAttributes<HTMLSelectElement>
type Option = { value: string; label: string; disabled: boolean }
function plainText(node: ReactNode): string {
  return Children.toArray(node)
    .map((child) =>
      isValidElement<{ children?: ReactNode }>(child)
        ? plainText(child.props.children)
        : String(child),
    )
    .join('')
}

// A non-modal listbox: opening it never locks body scrolling or clips long labels
// to the trigger width. Native buttons retain fieldset/label/focus semantics.
export function Select({
  children,
  value,
  onChange,
  className = '',
  disabled,
  id,
  ...props
}: Props) {
  const generatedId = useId()
  const buttonId = id ?? generatedId
  const listId = `${buttonId}-options`
  const options: Option[] = Children.toArray(children)
    .filter(isValidElement)
    .map((child) => {
      const option = child.props as {
        value?: string | number
        children?: ReactNode
        disabled?: boolean
      }
      const label = plainText(option.children)
      return { value: String(option.value ?? label), label, disabled: !!option.disabled }
    })
  const selected = options.findIndex((option) => option.value === String(value))
  const [open, setOpen] = useState(false)
  const [active, setActive] = useState(0)
  const [position, setPosition] = useState({ left: 12, top: 12, minWidth: 100, maxHeight: 320 })
  const trigger = useRef<HTMLButtonElement>(null)
  const popup = useRef<HTMLDivElement>(null)
  const typed = useRef({ text: '', time: 0 })
  const restoreSelectionFocus = useRef(false)
  const optionKey = options.map((option) => option.label).join('\n')
  const close = () => setOpen(false)
  const show = () => {
    setActive(selected >= 0 ? selected : 0)
    setOpen(true)
  }
  function choose(index: number) {
    const option = options[index]
    if (!option || option.disabled) return
    restoreSelectionFocus.current = true
    onChange?.({
      target: { value: option.value },
      currentTarget: { value: option.value },
    } as ChangeEvent<HTMLSelectElement>)
    close()
    trigger.current?.focus({ preventScroll: true })
  }
  useLayoutEffect(() => {
    if (!open || !trigger.current) return
    function place() {
      const rect = trigger.current!.getBoundingClientRect()
      if (rect.bottom < 0 || rect.top > innerHeight) {
        close()
        return
      }
      const below = innerHeight - rect.bottom - 12
      const above = rect.top - 12
      const useBelow = below >= Math.min(240, above)
      const maxHeight = Math.max(80, Math.min(360, useBelow ? below : above))
      const width = popup.current?.getBoundingClientRect().width ?? rect.width
      const height = Math.min(popup.current?.scrollHeight ?? maxHeight, maxHeight)
      const next = {
        left: Math.max(12, Math.min(rect.left, innerWidth - width - 12)),
        top: useBelow ? rect.bottom + 4 : Math.max(12, rect.top - height - 4),
        minWidth: Math.min(rect.width, innerWidth - 24),
        maxHeight,
      }
      setPosition((previous) =>
        JSON.stringify(previous) === JSON.stringify(next) ? previous : next,
      )
    }
    function outside(event: PointerEvent) {
      if (
        !trigger.current?.contains(event.target as Node) &&
        !popup.current?.contains(event.target as Node)
      )
        close()
    }
    place()
    window.addEventListener('scroll', place, true)
    window.addEventListener('resize', place)
    document.addEventListener('pointerdown', outside)
    return () => {
      window.removeEventListener('scroll', place, true)
      window.removeEventListener('resize', place)
      document.removeEventListener('pointerdown', outside)
    }
  }, [open, optionKey])
  useLayoutEffect(() => {
    if (!open) return
    const option = popup.current?.querySelector<HTMLElement>(`[data-index="${active}"]`)
    if (option && popup.current) {
      if (option.offsetTop < popup.current.scrollTop) popup.current.scrollTop = option.offsetTop
      if (
        option.offsetTop + option.offsetHeight >
        popup.current.scrollTop + popup.current.clientHeight
      )
        popup.current.scrollTop =
          option.offsetTop + option.offsetHeight - popup.current.clientHeight
    }
  }, [open, active])
  useLayoutEffect(() => {
    if (disabled) {
      if (open) close()
      return
    }
    if (restoreSelectionFocus.current) {
      if (document.activeElement === document.body || document.activeElement === trigger.current)
        trigger.current?.focus({ preventScroll: true })
      restoreSelectionFocus.current = false
    }
  })
  return (
    <>
      <button
        {...(props as React.ButtonHTMLAttributes<HTMLButtonElement>)}
        id={buttonId}
        ref={trigger}
        type="button"
        role="combobox"
        aria-haspopup="listbox"
        aria-expanded={open}
        aria-controls={open ? listId : undefined}
        aria-activedescendant={open ? `${listId}-${active}` : undefined}
        className={`${className} select-trigger`}
        disabled={disabled}
        data-value={String(value ?? '')}
        onClick={() => (open ? close() : show())}
        onKeyDown={(event) => {
          if (event.key === 'Tab') {
            close()
            return
          }
          if (event.key === 'Escape') {
            if (open) event.stopPropagation()
            close()
            return
          }
          if (['Enter', ' '].includes(event.key)) {
            event.preventDefault()
            if (open) choose(active)
            else show()
            return
          }
          if (['ArrowDown', 'ArrowUp', 'Home', 'End'].includes(event.key)) {
            event.preventDefault()
            if (!open) {
              show()
              return
            }
            let next =
              event.key === 'Home'
                ? 0
                : event.key === 'End'
                  ? options.length - 1
                  : active + (event.key === 'ArrowDown' ? 1 : -1)
            next = Math.max(0, Math.min(options.length - 1, next))
            const step = event.key === 'ArrowUp' || event.key === 'End' ? -1 : 1
            while (options[next]?.disabled && next >= 0 && next < options.length) next += step
            if (options[next]) setActive(next)
          } else if (event.key.length === 1 && !event.ctrlKey && !event.metaKey) {
            event.preventDefault()
            const now = Date.now()
            typed.current = {
              text:
                (now - typed.current.time < 700 ? typed.current.text : '') +
                event.key.toLowerCase(),
              time: now,
            }
            const found = options.findIndex(
              (option) =>
                !option.disabled && option.label.toLowerCase().startsWith(typed.current.text),
            )
            if (found >= 0) {
              setOpen(true)
              setActive(found)
            }
          }
        }}
      >
        <span className="select-value">{options[selected]?.label ?? 'Select a value'}</span>
        <ChevronDown size={14} aria-hidden="true" />
      </button>
      {open &&
        createPortal(
          <div
            ref={popup}
            id={listId}
            className="select-popup"
            role="listbox"
            aria-label={`${props['aria-label'] ?? plainText(options[selected]?.label)} options`}
            style={position}
          >
            {options.map((option, index) => (
              <div
                id={`${listId}-${index}`}
                key={`${option.value}-${index}`}
                role="option"
                aria-selected={index === selected}
                aria-disabled={option.disabled || undefined}
                data-index={index}
                data-value={option.value}
                className={`select-option${index === active ? ' active' : ''}`}
                onPointerMove={() => {
                  if (!option.disabled) setActive(index)
                }}
                onMouseDown={(event) => event.preventDefault()}
                onClick={() => choose(index)}
              >
                <span>{option.label}</span>
                {index === selected && <Check size={14} aria-hidden="true" />}
              </div>
            ))}
          </div>,
          document.body,
        )}
    </>
  )
}
