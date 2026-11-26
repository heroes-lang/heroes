#!/bin/sh
# The critic: the base's stage 2 in a Linux image, then reach's own
# `reach_web.hero` (no mode anywhere), to ask whether `ffi_unpaired_release`
# on libm's `nextafterf` is the base's.
set -u
OUT=/out
mkdir -p /work && cd /work && tar xf /src/base-leg.tar
{ uname -m; clang --version | head -1; date; } > "$OUT/machine.txt"
clang -I runtime seed/heroes.c runtime/runtime.c -o heroes > "$OUT/seed.log" 2>&1; echo "seed exit $?" >> "$OUT/seed.log"
./heroes build selfhost/main.hero -o heroes-next > "$OUT/stage2.log" 2>&1; echo "stage2 exit $?" >> "$OUT/stage2.log"
./heroes-next --version >> "$OUT/stage2.log" 2>&1
cd rebel/examples/reach
{ /work/heroes-next build reach_web.hero -o rw.bin 2>&1; echo "build exit $?"; } | head -c 6000 > "$OUT/reach_web.out"
date >> "$OUT/machine.txt"
