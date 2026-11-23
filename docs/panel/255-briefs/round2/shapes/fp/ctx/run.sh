#!/bin/sh
P=$(cd "$(dirname "$0")/../.." && pwd)
export TMPDIR="$P/tmp"
for route in today T A C; do
  export HEROES_RUNTIME="$P/$route/rt"
  W="$P/q5/ctx/work-$route"; rm -rf "$W"; mkdir -p "$W"; cp "$P"/q5/ctx/ctx.h "$P"/q5/ctx/cx_heap.hero "$W"/
  for mode in plain sanitize; do
    flag=""; [ "$mode" = sanitize ] && flag="--sanitize"
    (cd "$W" && "$P/../heroes" build cx_heap.hero $flag -o "bin-$mode" > "b-$mode.log" 2>&1) || { echo "$route $mode BUILD-FAILED" >> "$P/logs/ctx.log"; continue; }
    (cd "$W" && timeout 20 "./bin-$mode" > "r-$mode.log" 2>&1); code=$?
    echo "$route $mode exit=$code | $(grep -m1 -E 'ERROR: AddressSanitizer' "$W/r-$mode.log" | head -c 100) | $(grep -m1 -E '^    #0' "$W/r-$mode.log" | head -c 90) | $(grep -E '^(context|coroutine)' "$W/r-$mode.log" | tr '\n' ';')" >> "$P/logs/ctx.log"
  done
done
echo DONE >> "$P/logs/ctx.log"
