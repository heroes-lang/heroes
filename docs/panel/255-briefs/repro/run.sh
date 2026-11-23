#!/bin/sh
# Builds and runs each shape plain and under --sanitize with the compiler
# named by $1 (default ./heroes), from a copy of this folder; prints one line
# per run: shape, mode, exit code, and the first line naming the runtime.
H=${1:-./heroes}
OUT=${2:-out}
mkdir -p "$OUT"
for s in ${SHAPES:-heap mmap context stack handler_heap handler_mmap plain_heap plain_mmap}; do
  for mode in plain sanitize; do
    flag=""; [ "$mode" = sanitize ] && flag="--sanitize"
    if ! "$H" build "$s.hero" $flag -o "$OUT/$s-$mode" > "$OUT/$s-$mode.build" 2>&1; then
      echo "$s $mode BUILD-FAILED $(head -c 300 "$OUT/$s-$mode.build" | tr '\n' ' ')"; continue
    fi
    timeout 20 "$OUT/$s-$mode" > "$OUT/$s-$mode.log" 2>&1; code=$?
    first=$(grep -m1 -E 'ERROR: AddressSanitizer|SEGV|^#0|^#1 |hero_' "$OUT/$s-$mode.log" | head -c 160)
    printf '%s %s exit=%s %s | %s\n' "$s" "$mode" "$code" "$(head -1 "$OUT/$s-$mode.log" | head -c 60)" "$first"
  done
done
