#!/bin/bash
# usage (container): one.sh <program.hero> <level>   prints MSan's whole report
set -u
mkdir -p /tmp/m && cd /tmp/m && cp -r /src/runtime /src/seed . && cp /src/build/padg/*.hero /src/build/padg/*.h .
export HEROES_RUNTIME=/tmp/m/runtime
clang -O1 -I runtime seed/heroes.c runtime/runtime.c -o hc 2>/dev/null
b=${1%.hero}
./hc build $1 --emit-c -o $b.c > /dev/null 2>&1
clang -std=gnu11 -g -fno-strict-aliasing -fsigned-char -D_GNU_SOURCE $2 -fsanitize=memory -fsanitize-recover=memory -fsanitize-memory-track-origins -I runtime -I . $b.c runtime/runtime.c -o $b.bin -lm
MSAN_OPTIONS=halt_on_error=0 ./$b.bin > /dev/null 2> msan.txt
grep -E "WARNING|#[0-9] |Uninitialized value was" msan.txt | head -40
