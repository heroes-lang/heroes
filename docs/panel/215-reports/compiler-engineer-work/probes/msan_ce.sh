#!/bin/bash
# Compiler-engineer, panel 215: every probe under MemorySanitizer, at -O0..-O3
# and at -O1..-O3 with -fno-inline (a real call, as between two modules).
# usage (in a container, the copy mounted read-only at /src):
#   msan_ce.sh <dir under /src/build> <program.hero> ...
set -u
src=/src/build/$1; shift
mkdir -p /tmp/m && cd /tmp/m
cp -r /src/runtime /src/seed . ; [ -n "${COMP:-}" ] && cp /src/build/g2c/heroes.c ./g2.c; cp $src/*.hero $src/*.h . ; cp $src/*.expected . 2>/dev/null
export HEROES_RUNTIME=/tmp/m/runtime
[ -x ./hc ] || clang -O1 -I runtime ${COMP:-seed/heroes.c} runtime/runtime.c -o hc 2>/dev/null || { echo "compiler build failed"; exit 1; }
F="-std=gnu11 -D_USE_MATH_DEFINES -Wall -Werror=return-type -Werror=uninitialized -fno-strict-aliasing -fno-delete-null-pointer-checks -fsigned-char -fno-optimize-sibling-calls -Wno-infinite-recursion -D_GNU_SOURCE -g"
echo "leg $(uname -m), $(clang --version | head -1)"
for prog in "$@"; do
  b=${prog%.hero}
  ./hc build $prog --emit-c -o $b.c > $b.emit.txt 2>&1 || { echo "$prog: emit failed"; tail -5 $b.emit.txt; continue; }
  for O in -O0 -O1 -O2 -O3 "-O1 -fno-inline" "-O2 -fno-inline" "-O3 -fno-inline"; do
    tag=$(echo "$O" | tr -d ' ')
    clang $F $O -fsanitize=memory -fsanitize-recover=memory -I runtime -I . $b.c runtime/runtime.c -o $b$tag -lm 2> $b$tag.cc.txt || { echo "$prog $O: cc failed"; tail -3 $b$tag.cc.txt; continue; }
    MSAN_OPTIONS=halt_on_error=0 timeout 60 ./$b$tag > $b$tag.out 2> $b$tag.msan.txt
    code=$?
    lines=$(grep -A3 "use-of-uninitialized-value" $b$tag.msan.txt | grep -oE "[a-z_.-]+\.(hero|h):[0-9]+" | sort | uniq -c | tr -s ' ' | tr '\n' ';')
    said=$(tr -c '[:print:]\n' '?' < $b$tag.out | tail -n 1 | tr -cd '0-9 .\n-' | tr -s ' ')
    exp=""; [ -f $b.expected ] && { cmp -s $b$tag.out $b.expected && exp=" expected:same" || exp=" expected:DIFFERS"; }
    echo "$prog $O: exit $code$exp, $(grep -c 'WARNING: MemorySanitizer' $b$tag.msan.txt) reports at:${lines:- none}; printed:${said}"
  done
done
