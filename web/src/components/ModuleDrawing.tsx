// SPDX-FileCopyrightText: 2026 Robin Christ
// SPDX-License-Identifier: MPL-2.0
export function ModuleDrawing() {
  return (
    <svg viewBox="0 0 280 144" aria-hidden="true" className="module-drawing">
      <defs>
        <linearGradient id="shell" x1="0" y1="0" x2="1" y2="1">
          <stop stopColor="#edf2f6" />
          <stop offset="1" stopColor="#a1b1c3" />
        </linearGradient>
        <linearGradient id="top" x1="0" x2="0" y2="1">
          <stop stopColor="#fcfdfe" />
          <stop offset="1" stopColor="#d3dfe9" />
        </linearGradient>
      </defs>
      <ellipse cx="153" cy="121" rx="96" ry="10" fill="#1c304a" opacity=".08" />
      <path d="M42 83 158 29 248 65 127 124Z" fill="url(#shell)" stroke="#8294a8" />
      <path d="m42 65 116-52 90 35-121 57Z" fill="url(#top)" stroke="#a1b1c3" />
      <path d="M42 65v18l85 41v-19Z" fill="#8e9fae" />
      <path d="m127 105 121-57v17l-121 59Z" fill="#b1c0ce" />
      <path d="m57 67 90-41 53 22-91 43Z" fill="#fafcfe" stroke="#c5d1db" />
      <path d="m76 62 65-29m-58 33 65-29m-58 33 65-29" stroke="#8498ad" strokeWidth="2" />
      <path d="m100 75 37-17m-30 22 44-20" stroke="#bdd0e2" strokeWidth="3" />
      <path d="m44 67 37 17v23L44 89Z" fill="#254875" />
      <path d="m50 75 12 6v13l-12-6Zm16 7 11 5v13l-11-6Z" fill="#11253d" />
      <path
        d="m42 86-10 6 43 22 8-7"
        fill="none"
        stroke="#3f88d8"
        strokeWidth="5"
        strokeLinejoin="round"
      />
      <path d="m196 81 8-4m6-3 8-4m6-3 8-4" stroke="#dfb968" strokeWidth="4" />
      <circle cx="185" cy="34" r="2" fill="#93a7b9" />
    </svg>
  )
}
