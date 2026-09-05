# Field byte ownership

`src/lib/byte-layout.json` is generated metadata for the graphical editor's hex
controls. It maps a top-level JSON property to `[absolute byte offset, bit mask]`
pairs. Offsets are decimal EEPROM offsets: SFP starts at 0, QSFP upper page at 128. A mask of 255 means the entire byte; for example the SFP 10G Ethernet group
owns `[3, 240]`, while Infiniband owns `[3, 15]`.

## Source and regeneration

Run from `web/` (Python 3 required):

```sh
npm run generate:byte-layout
npm run check:byte-layout
npm test
```

The reproducible generator is `scripts/generate-byte-layout.py`. It extracts
`programming.byte_X` and `programming.byte_X_Y` references from the top-level
`SFF8472_LowerA0hToJSON` and `SFF8636_Upper00hToJSON` assignments in
`src/Standards/SFF-8472_JSON.cpp` and `src/Standards/SFF-8636_JSON.cpp` at the
repository root. The checked-in JSON was produced using that extraction.

Some details **are maintained explicitly** in the generator: disjoint masks for
compliance groups sharing a byte, and exclusion of interpretation dependencies.
The two QSFP Fibre Channel groups are assigned from `fibreChannelPair`, rather
than direct programming members, so their offsets are explicit too.
SFP wavelength/cable specification owns bytes 60–61, although its converter also
reads cable technology byte 8. Extended signaling rate owns bytes 66–67, although
it also reads nominal rate byte 12. These dependencies must not become writable
through another field's hex input. Review overrides against the C++ helper
implementations whenever those mappings change. This narrow extractor is not a
C++ parser and cannot infer the semantics of arbitrary helper code.

Generation rejects missing/extra schema fields, overlapping ownership, and
incomplete 128-byte page coverage. Both `npm test` and `npm run build` check that
the committed metadata matches the generated result. Do not edit the JSON by
hand; update the source/overrides and regenerate it.

## Nested fields and conversion

`src/lib/field-bytes.ts` narrows top-level ownership using nested schema property
names with `(Byte …)` / `(Bit …)` annotations. Its documented aliases handle
nested fields without such annotations (length, wavelength, attenuation, and
extended rate). These aliases are handwritten ownership metadata and must be
reviewed alongside schema and native codec changes. One legacy QSFP key,
`Multi-mode 50 um (M5) compliant (Bit 1)`, actually maps to byte 137 bit 2 in
both the C++ parser and assembler (`Multi_mode_50_um_M5_bit_2`). An explicit
nested override preserves that public JSON key while editing the correct bit. JSON-only discriminators
and the informational OUI vendor name have no independent EEPROM bytes.

Individual bit fields expose toggles or named values, with no separate hex input.
Their top-level group summarizes each affected physical byte in hexadecimal and
binary (bits 7 through 0). The byte input edits all eight bits, including bits
belonging to other groups sharing that offset. Those groups and their toggles
update together. `editHex` verifies that a requested whole-byte offset belongs
to the group before accepting it; unrelated offsets are rejected.

Ordinary non-bit fields retain their format dropdown and current hex display.
The lower-level masked edit helper remains available and tested for ownership
checks, but the UI does not present a masked hex value as an individual bit's
representation. Applying an edit decodes with the native WebAssembly codec,
merges changed values into the user's JSON, then encodes again to verify byte
preservation outside the edit. Only automatic checksums may change elsewhere;
explicit checksums remain explicit.

The metadata does not implement numeric/string encoding or validation; those
remain in C++. Format switching likewise accepts a candidate representation
only when native encoding reproduces every original byte. Unrepresentable
switches require an explicit replacement choice.

`tests/field-bytes.test.ts` exercises field coverage and shared-bit edits through
the real WASM codec. Existing codec tests exercise uniform and deterministic
mixed byte patterns in both fiber and copper modes. They complement the native
round-trip fuzzing infrastructure; regeneration checks alone cannot prove a
handwritten mask or nested alias semantically correct.
