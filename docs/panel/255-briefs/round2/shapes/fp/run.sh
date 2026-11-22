#!/bin/sh
# p255/q5/run.sh <route>: the libpng program and the four Q5 shapes, built
# against p255/<route>/rt, plain and --sanitize, plus the heap shape under
# ASAN_OPTIONS=detect_stack_use_after_return=1; into p255/logs/<route>-q5.log.
P=$(cd "$(dirname "$0")/.." && pwd)
H="$P/../heroes"
route=$1
export TMPDIR="$P/tmp"
export HEROES_RUNTIME="$P/$route/rt"
W="$P/q5/work-$route"
rm -rf "$W"; mkdir -p "$W"
cp "$P"/q5/*.hero "$P"/q5/*.h "$P"/q5/*.png "$P"/q5/libjo.dylib "$W"/
cd "$W" || exit 2
out="$P/logs/$route-q5.log"
: > "$out"
for s in pngdemo jo_heap jo_tls jo_lib jo_co t_partial t_unseen; do
  for mode in plain sanitize; do
    flag=""; [ "$mode" = sanitize ] && flag="--sanitize"
    if ! "$H" build "$s.hero" $flag -o "bin-$s-$mode" > "$s-$mode.build" 2>&1; then
      echo "$s $mode BUILD-FAILED $(head -c 300 "$s-$mode.build" | tr '\n' ' ')" >> "$out"; continue
    fi
    timeout 20 "./bin-$s-$mode" > "$s-$mode.log" 2>&1; code=$?
    last=$(grep -v '^libpng error' "$s-$mode.log" | grep -m1 -E 'ERROR: AddressSanitizer|^!?panic|^(heap|tls|lib|co|good|truncated|seen)' | head -c 140)
    all=$(grep -E '^(good|truncated|heap|tls|lib|co|answer|seen)' "$s-$mode.log" | tr '\n' ';' | head -c 200)
    printf '%s %s exit=%s | %s | %s\n' "$s" "$mode" "$code" "$last" "$all" >> "$out"
  done
done
echo DONE >> "$out"
