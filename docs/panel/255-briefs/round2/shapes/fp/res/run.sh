#!/bin/sh
P=$(cd "$(dirname "$0")/../.." && pwd)
export TMPDIR="$P/tmp"
for route in today T A C; do
  export HEROES_RUNTIME="$P/$route/rt"
  W="$P/q5/res/work-$route"; rm -rf "$W"; mkdir -p "$W"; cp "$P"/q5/res/residue.h "$P"/q5/res/*.hero "$W"/
  for s in t_partial t_unseen; do
  for mode in plain sanitize; do
    flag=""; [ "$mode" = sanitize ] && flag="--sanitize"
    (cd "$W" && "$P/../heroes" build $s.hero $flag -o "bin-$s-$mode" > "b-$s-$mode.log" 2>&1) || { echo "$route $s $mode BUILD-FAILED" >> "$P/logs/res.log"; continue; }
    (cd "$W" && timeout 20 "./bin-$s-$mode" > "r-$s-$mode.log" 2>&1); code=$?
    echo "$route $s $mode exit=$code | $(grep -m1 -E '^!?panic|answer ran|ERROR: Address' "$W/r-$s-$mode.log" | head -c 110) | $(grep -E '^seen' "$W/r-$s-$mode.log")" >> "$P/logs/res.log"
  done; done
done
echo DONE >> "$P/logs/res.log"
