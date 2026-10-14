#!/bin/sh
# Panel 215, the ffi-pragmatist's build of route C: what C cannot see, on glibc.
set -u
mkdir -p /w && cd /w
cp -r /src/runtime .
clang -I runtime /src/seed/heroes.c runtime/runtime.c -o heroes 2>&1 | tail -2
clang -I runtime /src/build/heroes-c.c runtime/runtime.c -o heroes-c 2>&1 | tail -2
cp -r /src/build/cannot c && cd c && rm -f bin
for f in zstream tq spin; do
  for k in heroes heroes-c; do
    out=$(../$k build $f.hero -o bin 2>&1); code=$?
    if [ $code = 0 ]; then res=$(timeout 30 ./bin 2>&1 | tr '\n' '|'); else res=$(printf '%s' "$out" | grep -E '^error' | head -2); fi
    echo "$f $k: build $code | $res"
  done
done
