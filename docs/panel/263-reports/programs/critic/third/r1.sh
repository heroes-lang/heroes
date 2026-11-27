#!/bin/bash
# r1.sh <binary> [VAR=value ...]: one run under timeout -k 5 20, output cut at 64 KiB; prints the exit, the counting
# canary's lines, the answer and the first panic or ASan line, on one line (lane bpp's r1.sh, the same words).
b=$1; shift
out=$(env "$@" timeout -k 5 20 "$b" 2>&1 | head -c 65536; echo "EXIT=${PIPESTATUS[0]}")
ex=$(printf '%s\n' "$out" | sed -n 's/^EXIT=//p' | tail -1)
cnt=$(printf '%s\n' "$out" | grep -oE "co: [a-z ]+: [0-9]+ bytes of the [0-9]+ below the stack were written, the lowest [0-9]+" | sed -E 's/co: ([a-z ]+): ([0-9]+) bytes of the [0-9]+ below the stack were written, the lowest ([0-9]+)/\1 \2 (lowest \3)/' | tr '\n' ';')
ans=$(printf '%s\n' "$out" | grep -E "^(depth reached|total|both|C jumped|bytes|sorted|answer|checks failed|door)" | head -2 | tr '\n' ' ')
pan=$(printf '%s\n' "$out" | grep -E "panic:|ERROR: AddressSanitizer|task stack overflow|Segmentation|Bus error|td:" | head -1 | cut -c1-200)
echo "$(basename $b) $* | exit $ex | $cnt | $ans | $pan"
