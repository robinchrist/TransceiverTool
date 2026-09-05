// SPDX-FileCopyrightText: 2026 Robin Christ
// SPDX-License-Identifier: MPL-2.0
declare module '*.mjs' {
  export default function createModule(): Promise<{ request: (input: string) => string }>
}
