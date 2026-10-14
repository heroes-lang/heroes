#!/bin/bash
# ffi-pragmatist, panel 215, Q2: build and run each copy probe with a compiler made from the seed here.
set -u
mkdir -p /tmp/m && cd /tmp/m && { [ -d runtime ] || cp -r /src/runtime /src/seed . ; }
[ -x /cache/hc ] || clang -O1 -I runtime seed/heroes.c runtime/runtime.c -o /cache/hc 2>/dev/null
export HEROES_RUNTIME=/tmp/m/runtime
rm -rf w && cp -r /src/build/q2 w && cd w && rm -rf build
echo "leg $(uname -m), $(clang --version | head -1)"
for p in "$@"; do
  echo "== $p"; /cache/hc build $p.hero -o $p > b.txt 2>&1; grep -E "^(error|wrote)" b.txt | head -3
  [ -x $p ] && { timeout 20 ./$p; echo "exit $?"; }
done
