#!/usr/bin/env python3
# SPDX-FileCopyrightText: 2026 Robin Christ
# SPDX-License-Identifier: MPL-2.0
"""Check or update only table-backed enum arrays; preserve handwritten schema formatting."""
import argparse
import json
from pathlib import Path
import re
import subprocess


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("exporter", help="path to the schema_enums executable")
    parser.add_argument("--write", action="store_true")
    args = parser.parse_args()
    tables = json.loads(subprocess.check_output([args.exporter], text=True))
    directory = Path(__file__).resolve().parents[1] / "schemas"
    stale = []
    for filename, definitions in tables.items():
        path = directory / filename
        original = text = path.read_text()
        for name, values in definitions.items():
            schema = json.loads(text)
            branches = schema["definitions"][name]["anyOf"]
            enums = [branch["enum"] for branch in branches if "enum" in branch]
            if len(enums) != 1:
                raise ValueError(f"Expected exactly one enum in {filename}: {name}")
            if enums[0] == values:
                continue
            stale.append(f"{filename}: {name}")
            # Locate the named definition, then limit the edit to its enum array. JSON decoding
            # determines the end of the array, including strings containing brackets or escapes.
            start = re.search(r'^        ' + re.escape(json.dumps(name)) + r':\s*\{', text, re.M)
            if start is None:
                raise ValueError(f"Cannot locate definition {name}")
            _, length = json.JSONDecoder().raw_decode(text[start.end() - 1:])
            end = start.end() - 1 + length
            match = re.search(r'"enum"\s*:\s*', text[start.end():end])
            if match is None:
                raise ValueError(f"Cannot locate enum {name}")
            offset = start.end() + match.end()
            old, length = json.JSONDecoder().raw_decode(text[offset:])
            if old != enums[0]:
                raise ValueError(f"Ambiguous enum edit in {name}")
            replacement = '[\n' + ',\n'.join('                        ' + json.dumps(v, ensure_ascii=False) for v in values) + '\n                    ]'
            text = text[:offset] + replacement + text[offset + length:]
        if args.write and text != original:
            path.write_text(text)
    if stale:
        print("Updated" if args.write else "Stale schema enums (run with --write):")
        print("\n".join(stale))
    return int(bool(stale) and not args.write)


if __name__ == "__main__":
    raise SystemExit(main())
