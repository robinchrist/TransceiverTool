#!/bin/sh
# SPDX-FileCopyrightText: 2024-2026 Robin Christ
# SPDX-License-Identifier: MPL-2.0
#
# Shared release smoke test: refuse to publish a binary that cannot do a byte-exact roundtrip.
# Exercises every binary type, both the file and the stdin input path (which is implemented
# differently on Windows), and the full decode -> encode path a user would actually run.

set -eu
BIN=$1
"$BIN" --version

tmp=$(mktemp -d)
trap 'rm -rf "$tmp"' EXIT

i=0
while [ "$i" -lt 25 ]; do
    head -c 128 /dev/urandom > "$tmp/a.bin"
    head -c 256 /dev/urandom > "$tmp/b.bin"

    "$BIN" roundtrip-test --binary-type 8472-128b --from-file "$tmp/a.bin" > /dev/null
    "$BIN" roundtrip-test --binary-type 8636-128b --from-file "$tmp/a.bin" > /dev/null
    "$BIN" roundtrip-test --binary-type 8636-256b --from-file "$tmp/b.bin" > /dev/null
    "$BIN" roundtrip-test --binary-type 8472-128b < "$tmp/a.bin" > /dev/null

    i=$((i + 1))
done

# decode -> encode has to reproduce the input exactly, in both fiber and copper interpretation
for mode in --fiber-mode --copper-mode; do
    "$BIN" decode --binary-type 8472-128b "$mode" --from-file "$tmp/a.bin" --json-to-file "$tmp/a.json"
    "$BIN" encode --binary-type 8472-128b --from-file "$tmp/a.json" --to-file "$tmp/a.out"
    cmp "$tmp/a.bin" "$tmp/a.out"

    "$BIN" decode --binary-type 8636-256b "$mode" --from-file "$tmp/b.bin" --json-to-file "$tmp/b.json"
    "$BIN" encode --binary-type 8636-256b --from-file "$tmp/b.json" --to-file "$tmp/b.out"
    # Only the upper page is modelled by SFF8636_Upper00h; the lower half is not written back
    cmp -i 128 "$tmp/b.bin" "$tmp/b.out"
done

# Random data is full of legitimate validation errors, so this only asserts that validating a
# JSON file does not crash; the output is noise in a release log.
"$BIN" validate-json --from-file "$tmp/a.json" > /dev/null 2>&1

echo "smoke test passed"
