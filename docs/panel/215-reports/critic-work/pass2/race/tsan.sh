#!/bin/bash
# Critic, panel 215 pass 2: race.hero's emitted C under ThreadSanitizer, -O0 and -O2.
set -u
mkdir -p /tmp/t && cd /tmp/t && cp -r /src/runtime . && cp /src/build/race/race.c /src/build/race/race.h .
F="-std=gnu11 -fno-strict-aliasing -fno-delete-null-pointer-checks -fsigned-char -D_GNU_SOURCE -g"
echo "leg $(uname -m), $(clang --version | head -1)"
for O in -O0 -O2; do
  clang $F $O -fsanitize=thread -I runtime -I . race.c runtime/runtime.c -o r$O -lm -lpthread 2> cc.txt || { echo "$O cc failed"; head -5 cc.txt; continue; }
  timeout 120 ./r$O > out.txt 2> tsan.txt; code=$?
  echo "$O: exit $code, printed '$(cat out.txt)', $(grep -c 'WARNING: ThreadSanitizer' tsan.txt) TSan warnings"
  grep -A6 "WARNING: ThreadSanitizer" tsan.txt | head -14
done
