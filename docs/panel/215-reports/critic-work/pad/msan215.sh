#!/bin/bash
# Critic, panel 215 pass 1: panel 178's padding probe (adapted to @=) and its rest: zero variant under MemorySanitizer.
set -u
mkdir -p /tmp/m && cd /tmp/m
cp -r /src/runtime /src/seed . ; cp /src/build/pad/small.hero /src/build/pad/small_rz.hero /src/build/pad/small.h .
export HEROES_RUNTIME=/tmp/m/runtime
clang -O1 -I runtime seed/heroes.c runtime/runtime.c -o hc 2>/dev/null || { echo "compiler build failed"; exit 1; }
F="-std=gnu11 -g -fno-strict-aliasing -fno-delete-null-pointer-checks -fsigned-char -D_GNU_SOURCE"
echo "leg $(uname -m), $(clang --version | head -1)"
for prog in small.hero small_rz.hero; do
  b=${prog%.hero}
  ./hc build $prog --emit-c -o $b.c > $b.emit.txt 2>&1 || { echo "$prog: emit failed"; tail -5 $b.emit.txt; continue; }
  echo "$prog: memset lines in its C: $(grep -c memset $b.c)"
  for O in -O0 -O1 -O2 -O3; do
    clang $F $O -fsanitize=memory -fsanitize-recover=memory -I runtime -I . $b.c runtime/runtime.c -o $b$O -lm 2> $b$O.cc.txt || { echo "$prog $O: cc failed"; tail -3 $b$O.cc.txt; continue; }
    MSAN_OPTIONS=halt_on_error=0 timeout 60 ./$b$O > /dev/null 2> $b$O.msan.txt
    lines=$(grep -A3 "use-of-uninitialized-value" $b$O.msan.txt | grep -oE "$b\.hero:[0-9]+" | sort -t: -k2 -n | uniq | tr '\n' ' ')
    echo "$prog $O: $(grep -c 'WARNING: MemorySanitizer' $b$O.msan.txt) reports at: ${lines:-none}"
  done
done
