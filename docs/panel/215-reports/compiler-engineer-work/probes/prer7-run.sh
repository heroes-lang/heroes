#!/bin/bash
# The compiler of 40c055a33^ (before panel 196's R7), panel 178's original probe, x86-64 MSan.
set -u
mkdir -p /tmp/m && cd /tmp/m && cp -r /src/build/prer7/src/runtime /src/build/prer7/src/seed . && cp /src/build/prer7/pad/* .
export HEROES_RUNTIME=/tmp/m/runtime
clang -O1 -I runtime seed/heroes.c runtime/runtime.c -o hc 2>/dev/null || { echo "compiler build failed"; exit 1; }
./hc build small.hero --emit-c -o small.c > emit.txt 2>&1 || { echo "emit failed"; cat emit.txt | head -5; exit 1; }
grep -c hero_lend_local small.c
F="-std=gnu11 -g -fno-strict-aliasing -fno-delete-null-pointer-checks -fsigned-char -D_GNU_SOURCE"
for O in -O0 -O1 -O2 -O3; do
  clang $F $O -fsanitize=memory -fsanitize-recover=memory -I runtime -I . small.c runtime/runtime.c -o s$O -lm 2>/dev/null || { echo "$O cc failed"; continue; }
  MSAN_OPTIONS=halt_on_error=0 ./s$O > /dev/null 2> m$O.txt
  echo "$O: $(grep -c 'WARNING: MemorySanitizer' m$O.txt) reports at: $(grep -A3 use-of-uninit m$O.txt | grep -oE 'small\.hero:[0-9]+' | sort | uniq -c | tr -s ' ' | tr '\n' ';')"
done
