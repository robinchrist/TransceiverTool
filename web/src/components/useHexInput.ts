// SPDX-FileCopyrightText: 2026 Robin Christ
// SPDX-License-Identifier: MPL-2.0
import { useEffect, useRef, useState } from 'react'
import { hex, parseHex, type BytePart } from '../lib/field-bytes'
import type { FieldByteContext } from './FieldBytes'

export function useHexInput(
  context: FieldByteContext | null,
  path: string,
  parts: BytePart[],
  value: string,
  wholeByte?: number,
  representation?: 'named' | 'raw',
) {
  const [text, setTextState] = useState<string | null>(null)
  const [error, setError] = useState('')
  const [revision, setRevision] = useState(0)
  const currentText = useRef(text)
  const inFlight = useRef(false)
  const completed = useRef<{ text: string; result: unknown } | null>(null)
  const mounted = useRef(true)
  const attempted = useRef<{ result: unknown; text: string; representation: unknown } | null>(null)
  const key = wholeByte === undefined ? path : `${path}/@byte-${wholeByte}`
  const partsKey = JSON.stringify(parts)
  const contextRef = useRef(context)
  contextRef.current = context
  useEffect(() => {
    mounted.current = true
    return () => {
      mounted.current = false
      contextRef.current?.pending(key, false)
    }
  }, [key])
  function setText(next: string) {
    currentText.current = next
    setTextState(next)
    setError('')
    context?.pending(key, true)
  }
  useEffect(() => {
    if (!context || text === null || !parts.length) return
    let normalized: string
    try {
      normalized = hex(parseHex(text, parts))
    } catch (error) {
      setError((error as Error).message)
      return
    }
    const clear = () => {
      if (mounted.current && currentText.current === text) {
        currentText.current = null
        setTextState(null)
        setError('')
        context.pending(key, false)
      }
    }
    if (context.locked || !context.result || inFlight.current) return
    // Keep the entered text until the new validated bytes are published. Clearing
    // on promise completion would briefly redisplay the previous byte value.
    if (
      completed.current?.text === text &&
      completed.current.result !== context.result &&
      normalized === value
    ) {
      clear()
      return
    }
    if (
      attempted.current?.result === context.result &&
      attempted.current.text === text &&
      attempted.current.representation === representation
    )
      return
    attempted.current = { result: context.result, text, representation }
    inFlight.current = true
    context
      .apply(path, text, wholeByte, representation)
      .then(() => {
        completed.current = { text, result: context.result }
      })
      .catch((error) => {
        if (mounted.current && currentText.current === text) setError(error.message)
      })
      .finally(() => {
        inFlight.current = false
        if (mounted.current) setRevision((value) => value + 1)
      })
  }, [text, context?.result, context?.locked, partsKey, value, wholeByte, representation, revision])
  return { text, error, setText }
}
