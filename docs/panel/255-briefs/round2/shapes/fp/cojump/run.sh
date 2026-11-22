#!/bin/sh
P=$(cd "$(dirname "$0")/../.." && pwd)
export TMPDIR="$P/tmp"
for route in today T A C1 C; do
  export HEROES_RUNTIME="$P/$route/rt"
  W="$P/q5/cojump/work-$route"; rm -rf "$W"; mkdir -p "$W"; cp "$P"/q5/cojump/cojump.h "$P"/q5/cojump/*.hero "$W"/
  for s in cj; do for mode in plain sanitize; do
    flag=""; [ "$mode" = sanitize ] && flag="--sanitize"
    (cd "$W" && "$P/../heroes" build $s.hero $flag -o "bin-$s-$mode" > "b-$s-$mode.log" 2>&1) || { echo "$route $s $mode BUILD-FAILED $(head -c 200 "$W/b-$s-$mode.log")" >> "$P/logs/cojump.log"; continue; }
    (cd "$W" && timeout 20 "./bin-$s-$mode" > "r-$s-$mode.log" 2>&1); code=$?
    echo "$route $s $mode exit=$code | $(grep -m1 -E '^!?panic|answer ran|ERROR: Address' "$W/r-$s-$mode.log" | head -c 120) | $(grep -E '^coroutine' "$W/r-$s-$mode.log")" >> "$P/logs/cojump.log"
  done; done
done
echo DONE >> "$P/logs/cojump.log"
