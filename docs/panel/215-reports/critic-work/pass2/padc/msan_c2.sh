#!/bin/bash
# Critic, panel 215 pass 2: each program under MemorySanitizer with today's
# compiler (the seed) and the compiler-engineer's prototype (build/g2c), at
# -O0..-O3 and -O2 -fno-inline. In a container, the copy mounted at /src.
# usage: msan_c2.sh <program.hero> ...
set -u
mkdir -p /tmp/m && cd /tmp/m
cp -r /src/runtime /src/seed . ; cp /src/build/g2c/heroes.c ./g2.c; cp /src/build/padc/*.hero /src/build/padc/*.h .
export HEROES_RUNTIME=/tmp/m/runtime
clang -O1 -I runtime seed/heroes.c runtime/runtime.c -o hc_today 2>/dev/null || { echo "today build failed"; exit 1; }
clang -O1 -I runtime g2.c runtime/runtime.c -o hc_g 2>/dev/null || { echo "prototype build failed"; exit 1; }
F="-std=gnu11 -D_USE_MATH_DEFINES -Wall -Werror=return-type -Werror=uninitialized -fno-strict-aliasing -fno-delete-null-pointer-checks -fsigned-char -fno-optimize-sibling-calls -Wno-infinite-recursion -D_GNU_SOURCE -g"
echo "leg $(uname -m), $(clang --version | head -1)"
for comp in today g; do
for prog in "$@"; do
  b=${prog%.hero}_$comp
  ./hc_$comp build $prog --emit-c -o $b.c > $b.emit.txt 2>&1 || { echo "$comp $prog: emit failed"; tail -5 $b.emit.txt; continue; }
  echo "$comp $prog: hero_pad_zero lines $(grep -c 'hero_pad_zero(' $b.c)"
  for O in -O0 -O1 -O2 -O3 "-O2 -fno-inline"; do
    tag=$(echo "$O" | tr -d ' ')
    clang $F $O -fsanitize=memory -fsanitize-recover=memory -I runtime -I . $b.c runtime/runtime.c -o $b$tag -lm 2> $b$tag.cc.txt || { echo "$comp $prog $O: cc failed"; tail -3 $b$tag.cc.txt; continue; }
    MSAN_OPTIONS=halt_on_error=0 timeout 60 ./$b$tag > $b$tag.out 2> $b$tag.msan.txt
    code=$?
    lines=$(grep -A4 "use-of-uninitialized-value" $b$tag.msan.txt | grep -oE "[a-z_.0-9-]+\.hero:[0-9]+" | sort | uniq -c | tr -s ' ' | tr '\n' ';')
    echo "$comp $prog $O: exit $code, $(grep -c 'WARNING: MemorySanitizer' $b$tag.msan.txt) reports at:${lines:- none}; printed: $(tail -n1 $b$tag.out)"
  done
done
done
