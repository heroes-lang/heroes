#!/bin/sh
P=$(cd "$(dirname "$0")/../.." && pwd)
export TMPDIR="$P/tmp"
for route in C; do
  export HEROES_RUNTIME="$P/$route/rt"
  W="$P/q5/fake/work-$route"; rm -rf "$W"; mkdir -p "$W"; cp "$P"/q5/fake/fake.h "$P"/q5/fake/fk.hero "$W"/
  (cd "$W" && "$P/../heroes" build fk.hero --sanitize -o bin > b.log 2>&1) || { echo "$route BUILD-FAILED $(head -c 300 "$W/b.log")" >> "$P/logs/fake-C3.log"; continue; }
  for opt in default uar1; do
    if [ $opt = uar1 ]; then o="detect_stack_use_after_return=1"; else o=""; fi
    (cd "$W" && ASAN_OPTIONS="$o" timeout 20 ./bin > "r-$opt.log" 2>&1); code=$?
    echo "$route --sanitize ASAN_OPTIONS=$o exit=$code | $(grep -m1 -E '^!?panic|^held|ERROR: Address' "$W/r-$opt.log" | head -c 120)" >> "$P/logs/fake-C3.log"
  done
done
echo DONE >> "$P/logs/fake-C3.log"
