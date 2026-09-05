# TransceiverTool

<!-- impeccable:product-schema 1 -->

## Platform

web

This record guides the browser interface in `web/` within the existing C++
TransceiverTool project. The native CLI remains part of the same project.

## Users

Network engineers and technicians inspecting and modifying transceiver EEPROM
configurations. Their primary job is to understand a programming, edit its fields,
and obtain a usable JSON configuration or binary programming file.

## Product Purpose

Make transceiver configurations understandable and graphically editable while
preserving the information needed to convert them back to binary. Success means
users can read a configuration, make intentional changes, inspect validation
results, and download the intended output without manually interpreting every byte.

## Operating Context

- Users work with local EEPROM dumps and TransceiverTool JSON files.
- The browser interface supports SFF-8472 Lower A0h and SFF-8636 Upper 00h,
  including full 256-byte QSFP input where the lower half is preserved.
- The current app is a self-contained SPA: its built HTML embeds the interface,
  schemas, and WebAssembly module and works directly from disk without a backend.
- Import, graphical editing, JSON editing, byte inspection, validation, undo, and
  JSON/binary downloads form the existing workflow.
- The app edits files; it does not directly program hardware.

## Capabilities and Constraints

### Explicit user requirements

- Keep JSON schemas human-readable and C++ code straightforward.
- Drive graphical configuration from the JSON schemas.
- Support reading configurations and downloading binary and JSON outputs.
- Prefer a self-contained SPA and reuse the C++ project through WebAssembly.
- Manage C++ dependencies and the WebAssembly build through Conan. Do not introduce
  a separate dependency-download mechanism or hardcoded dependency download URLs.
- Consider shadcn and Tailwind CSS; the existing implementation uses React,
  TypeScript, Tailwind, and shadcn-compatible Radix components.

### Existing implementation facts to preserve

- The browser module links the same `TransceiverToolCore` target as the CLI.
  Conan manages the Emscripten SDK and C++ dependencies with a checked-in lockfile.
- A 128-byte dump is ambiguous: users explicitly choose SFP or QSFP upper-page
  interpretation. A 256-byte dump is a full QSFP page.
- Full-page QSFP export preserves the imported lower 128 bytes verbatim. JSON
  represents only the modeled upper page and cannot supply the missing lower half.
- Fiber/copper mode reinterprets shared bytes through the existing conversion code.
- Named values and raw-byte alternatives support readable configurations and
  preservation of reserved or unknown values.
- Automatic checksums and explicit raw checksums have different semantics. An
  intentionally incorrect explicit checksum can be preserved.
- Schema/conversion failures are distinct from standard-compliance advisories.
  Export must correspond to the current document, never stale conversion results.
- Edits live in the tab; users download files to retain them. The current app has
  no account system, backend storage, or direct hardware connection.

## Evidence on Hand

- `schemas/`: handwritten public JSON contracts, with selected enum arrays kept
  synchronized with the C++ assigned-value tables.
- `src/Standards/` and `include/TransceiverTool/Standards/`: the parser, assembler,
  JSON conversion, checksum, and standard-validation implementations.
- `tests/` and `fuzz/`: native contract and round-trip checks and libFuzzer targets.
- `web/tests/`: WASM round-trip, schema-field coverage, and offline browser tests.
- `web/README.md`: current browser workflows, build instructions, and limits.
- In-app examples are illustrative configurations, not device-qualified presets.
  Test results do not establish compatibility with every transceiver or host device.

## Product Principles

1. Make changes understandable and intentional; retain access to the underlying
   JSON and bytes when users need precision.
2. Preserve binary information and distinguish representation changes from changes
   to the programming itself.
3. Keep conversion behavior shared with C++ and dependencies governed by Conan.
4. Keep the browser workflow self-contained and local, with explicit file exports.
5. Explain format errors and standard advisories without conflating them or
   presenting illustrative examples as hardware-qualified configurations.

## Open Decisions

- The browser's working name is Transceiver Studio; no separate branding commitment
  has been confirmed.
- Desktop versus mobile usage priority has not been specified. The existing app
  supports responsive browser layouts.
- No product-specific accessibility standard, additional hardware formats, direct
  device integration, or persistent-storage roadmap has been confirmed.
