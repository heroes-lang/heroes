#!/bin/sh
# The critic, panel 261's second reading: the compiler-engineer's route
# (route-v4.diff applied to df92f9281) built from the seed inside a Linux
# image, then every kept case, my shapes and the ffi shapes under it.
set -u
OUT=/out
mkdir -p "$OUT" /work && cd /work && tar xf /src/route-leg.tar
{ uname -m; clang --version | head -1; date; } > "$OUT/machine.txt"
clang -I runtime seed/heroes.c runtime/runtime.c -o heroes > "$OUT/seed.log" 2>&1; echo "seed exit $?" >> "$OUT/seed.log"
./heroes build selfhost/main.hero -o heroes-route > "$OUT/stage2.log" 2>&1; echo "stage2 exit $?" >> "$OUT/stage2.log"
./heroes-route --version >> "$OUT/stage2.log" 2>&1
date >> "$OUT/machine.txt"
sh /src/cases.sh /work/heroes-route /work "$OUT/cases"
date >> "$OUT/machine.txt"
