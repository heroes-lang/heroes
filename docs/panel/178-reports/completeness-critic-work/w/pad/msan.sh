#!/bin/bash
# Critic, panel 178: the Heroes pipeline's own C under MemorySanitizer.
# usage (in a container): msan.sh <compiler-C-file> <program.hero> ...
set -u
mkdir -p /tmp/m && cd /tmp/m
cp -r /src/runtime /src/seed . ; cp /src/w/pad/*.hero /src/w/pad/*.h .
export HEROES_RUNTIME=/tmp/m/runtime
comp=$1; shift
if [ ! -x ./hc ]; then clang -O1 -I runtime "$comp" runtime/runtime.c -o hc 2>/dev/null || { echo "compiler build failed"; exit 1; }; fi
F="-std=gnu11 -g -fno-strict-aliasing -fno-delete-null-pointer-checks -fsigned-char -D_GNU_SOURCE"
echo "leg $(uname -m), $(clang --version | head -1)"
for prog in "$@"; do
  b=${prog%.hero}
  ./hc build $prog --emit-c -o $b.c > $b.emit.txt 2>&1 || { echo "$prog: emit failed"; tail -5 $b.emit.txt; continue; }
  for O in -O0 -O1 -O2 -O3; do
    clang $F $O -fsanitize=memory -fsanitize-recover=memory -I runtime -I . $b.c runtime/runtime.c -o $b$O -lm 2> $b$O.cc.txt || { echo "$prog $O: cc failed"; tail -3 $b$O.cc.txt; continue; }
    MSAN_OPTIONS=halt_on_error=0 ./$b$O > /dev/null 2> $b$O.msan.txt
    lines=$(grep -A3 "use-of-uninitialized-value" $b$O.msan.txt | grep -oE "$b\.hero:[0-9]+" | sort -t: -k2 -n | uniq | tr '\n' ' ')
    echo "$prog $O: $(grep -c 'WARNING: MemorySanitizer' $b$O.msan.txt) reports at: ${lines:-none}"
  done
done
