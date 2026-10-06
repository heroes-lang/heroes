#!/bin/bash
# Inside a Linux container: build the seed carrying the 091 prototype, then run
# each named program from a writable copy, printing its exit code.
set -u
mkdir -p /tmp/w && cd /tmp/w
cp -r /src/runtime . && cp -r /src/p/s . && cp -r /src/p/r . 2>/dev/null
if [ ! -x /tmp/w/heroes ]; then
  s0=$(date +%s); clang -I runtime /src/p/seed-next.c runtime/runtime.c -o heroes 2>&1 | tail -3; echo "seed build $(( $(date +%s) - s0 )) s"
fi
uname -m
for prog in "$@"; do
  echo "== $prog"
  ./heroes run "$prog" > out.txt 2>&1; e=$?
  tail -14 out.txt; echo "EXIT=$e"
done
