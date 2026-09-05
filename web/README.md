# Transceiver Studio

A local-first, schema-driven editor for TransceiverTool configurations. React, TypeScript,
Tailwind CSS, and shadcn-compatible Radix components provide the interface. The actual
`TransceiverToolCore` C++ library performs binary conversion and standard validation through
WebAssembly. There is no backend or JavaScript reimplementation of the binary format.

## Build

Install Conan 2, CMake 3.27 or newer, and Node.js 22.12+ (Node 24 is used in CI).
From this directory:

```sh
# Once, if you do not already have a native Conan build profile:
conan profile detect

npm ci
npm run build:wasm
npm run build
```

`build:wasm` installs the root `conanfile.py` using the checked-in
`TransceiverTool-wasm` host profile and `conan.lock`. Conan supplies **both the Emscripten SDK
and all C++ dependencies**. No separate SDK installation or dependency download URLs are used.
The profile pins the full compiler version because Emscripten patch releases are not ABI-compatible.
The native `default` profile is the build profile; it is not overwritten.

The WASM target links the same `TransceiverToolCore` CMake target as the CLI. Output goes to
`src/wasm/transceiver.mjs`; build files stay in `build-conan-wasm/`. Both are ignored by Git.
Rerun `build:wasm` after C++ changes. When intentionally updating dependencies, regenerate
the lockfile from the repository root:

```sh
conan lock create . -pr:b default -pr:h ./conan-profiles/profiles/TransceiverTool-wasm --lockfile-out web/conan.lock
```

**The distributable is `dist/index.html`.** JavaScript, CSS, schemas, and the WASM binary are
embedded in that one file. Open it directly from disk, copy it to another machine, or serve it
from any static host. No runtime network requests, external fonts, analytics, or server are needed.

## Develop and test

```sh
npm run dev
npm test
npm run build
npx playwright install chromium
npm run test:e2e
npm run format:check
```

Set `CHROMIUM_PATH` to use an existing Chrome/Chromium executable for browser tests.
The E2E suite opens the production HTML via `file://` with HTTP(S) blocked. It exercises visual
edits, invalid input, undo, JSON drafts, binary inspection, mobile navigation, binary downloads,
explicit QSFP upper-page import, and full-page byte preservation. Codec tests run every uniform
byte pattern plus deterministic mixed patterns through the real WASM module in both modes.
The schema coverage test ensures every modeled top-level field has an editor section.

Native round-trip fuzzers and schema tests remain available through the existing build. The
browser integration also adds native regression coverage for whole-number wavelength values:
JavaScript serializes `1310.0` as `1310`, and both are valid JSON numbers.

## Supported workflow

- Import SFF-8472 Lower A0h (128 bytes), SFF-8636 Upper 00h (128 bytes), a full QSFP page
  (256 bytes), or TransceiverTool JSON. For a 128-byte binary, explicitly select the standard;
  file length alone cannot distinguish them. Oversized and unsupported inputs are rejected.
- Edit scalar fields, enums, flags, nested objects, and alternative representations directly
  from the repository's schemas. Representation changes automatically preserve the current bytes when possible; otherwise
  the editor asks before replacing the value. Undo restores the previous value. Unknown JSON properties survive ordinary visual edits.
- Switch fiber/copper interpretation by encoding and decoding the same bytes. This canonicalizes
  the modeled JSON through the C++ converter; JSON-only annotations are not part of the binary.
- Inspect raw JSON or hex bytes. JSON drafts require Apply before exporting or editing other
  fields. Invalid input blocks export, and validation results are tied to the exact current document.
- Review schema/conversion errors separately from standard-compliance advisories. Reserved values
  and incorrect explicit checksums can be preserved. `auto` recalculates a checksum; raw byte
  values preserve it as entered.
- Download JSON, a 128-byte programming, or a full 256-byte QSFP page when imported lower-page
  bytes are available. The lower half is preserved verbatim. **JSON contains only the modeled
  upper page**, so JSON-only imports cannot produce a full-page download.

Changes are held in the tab, with up to 50 undo states. Download before closing it. A replacement
confirmation and a browser unload warning guard modified documents. Examples are clearly labeled
and are illustrative configurations, not device-qualified presets. No hardware is programmed by
the app.

The shipped schemas remain handwritten and are bundled directly from `../schemas/`. Field
grouping is presentation metadata in `src/lib/schema.ts`; binary interpretation stays in C++.

All configuration sections remain visible; sidebar links scroll to each section and
follow the section currently in view. Binary-backed fields show their current hex
value. Bit fields use toggles or named values; their groups summarize whole bytes
as hex and binary, with hex inputs that update all flags sharing each byte. Vendor Specific starts in hex mode. Complete valid hex input applies automatically;
partial or invalid input must be completed or corrected before switching views or exporting. The module summary
shows declared transceiver characteristics, not live measurements.

See [Field byte ownership](docs/byte-layout.md) for the generated byte map, its C++
sources, handwritten mask exceptions, regeneration commands, and validation.

Named-byte fields offer Named value and Hex bytes editing modes. Hex mode shows
the corresponding name when one exists. JSON stores that name by default and
falls back to a raw byte for unknown values; Write as raw byte explicitly selects
raw output. Short byte values appear beside field titles, long sequences directly
below them. Field tooltips identify byte offsets and bit positions.
