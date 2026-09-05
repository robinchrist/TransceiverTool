#!/usr/bin/env python3
# SPDX-FileCopyrightText: 2026 Robin Christ
# SPDX-License-Identifier: MPL-2.0
"""Generate top-level byte ownership; see web/docs/byte-layout.md.

C++ ToJSON assignments provide offsets. Explicit overrides describe shared bits
and exclude arguments that select interpretation but are not owned by a field.
This is intentionally a narrow extractor, not a general C++ parser.
"""
import argparse
import json
import re
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]

# Shared groups use disjoint masks in the native conversion helpers.
OVERRIDES = {
    "8472": {
        "10G Ethernet Compliance Codes": [[3, 0xF0]],
        "Infiniband Compliance Codes": [[3, 0x0F]],
        "ESCON Compliance Codes": [[4, 0xC0]],
        "SONET Compliance Codes": [[4, 0x3F], [5, 0xFF]],
        "Fibre Channel Link Length": [[7, 0xF8]],
        "Fibre Channel Technology": [[7, 0x07], [8, 0xF0]],
        "SFP+ Cable Technology": [[8, 0x0F]],
        # Byte 8 is read to select interpretation; it is not owned here.
        "Wavelength Or Cable Specification Compliance": [[60, 0xFF], [61, 0xFF]],
        # Nominal rate byte 12 is similarly only an interpretation dependency.
        "Extended Signaling Rate": [[66, 0xFF], [67, 0xFF]],
    },
    # These two assignments read a shared helper result (fibreChannelPair),
    # so their offsets as well as masks are explicit.
    "8636": {
        "Fibre Channel Link Length": [[135, 0xF8]],
        "Fibre Channel Transmitter Technology": [[135, 0x07], [136, 0xFF]],
    },
}


def extract(standard, page):
    source = (ROOT / f"src/Standards/SFF-{standard}_JSON.cpp").read_text()
    body = source.split(f"void SFF{standard}_{page}ToJSON")[1]
    body = body.split(f"void SFF{standard}_{page}FromJSON")[0]
    fields = {}
    assignments = re.findall(r'j\["([^"]+)"\]\s*=\s*([^;]+);', body)
    for name, expression in assignments:
        offsets = set()
        for start, end in re.findall(r"programming\.byte_(\d+)(?:_(\d+))?", expression):
            offsets.update(range(int(start), int(end or start) + 1))
        if offsets:
            if name in fields:
                raise SystemExit(f"{standard}: duplicate assignment for {name}")
            fields[name] = [[offset, 0xFF] for offset in sorted(offsets)]
    if not set(OVERRIDES[standard]).issubset(name for name, _ in assignments):
        raise SystemExit(f"{standard}: an override no longer matches a ToJSON assignment")
    fields.update(OVERRIDES[standard])

    # Fail closed if extraction stops covering the schema or EEPROM page.
    schema = json.loads((ROOT / f"schemas/SFF-{standard}_{page}.json").read_text())
    expected = set(schema["properties"]) - {"Type"}
    if set(fields) != expected:
        raise SystemExit(f"{standard}: schema/extraction mismatch: {set(fields) ^ expected}")
    owned = {}
    for name, parts in fields.items():
        for offset, mask in parts:
            if owned.get(offset, 0) & mask:
                raise SystemExit(f"{standard}: overlapping ownership at {offset}: {name}")
            owned[offset] = owned.get(offset, 0) | mask
    first = 0 if standard == "8472" else 128
    if owned != {offset: 0xFF for offset in range(first, first + 128)}:
        raise SystemExit(f"{standard}: incomplete EEPROM ownership")
    return fields


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--check", action="store_true", help="fail if metadata differs")
    args = parser.parse_args()
    result = {
        standard: extract(standard, page)
        for standard, page in [("8472", "LowerA0h"), ("8636", "Upper00h")]
    }
    output = ROOT / "web/src/lib/byte-layout.json"
    if args.check:
        if json.loads(output.read_text()) != result:
            raise SystemExit("byte-layout.json is stale; run npm run generate:byte-layout")
    else:
        output.write_text(json.dumps(result, indent=2) + "\n")


if __name__ == "__main__":
    main()
