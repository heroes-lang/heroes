#!/bin/sh
# Panel 215, the ffi-pragmatist's build of route C: the --emit-c census on glibc,
# today's compiler, the route's, and the route without its classify gate.
set -u
mkdir -p /w && cd /w
cp -r /src/runtime .
clang -I runtime /src/seed/heroes.c runtime/runtime.c -o heroes 2>&1 | tail -2
clang -I runtime /src/build/heroes-c.c runtime/runtime.c -o heroes-c 2>&1 | tail -2
clang -I runtime /src/build/heroes-u.c runtime/runtime.c -o heroes-u 2>&1 | tail -2
for who in heroes heroes-c heroes-u; do
  mkdir -p /w/e-$who && cd /w/e-$who
  sed 's#^#/src/#' /src/build/census-files.txt | grep -v '^/src/selfhost/main.hero$' | xargs -P 8 -n 1 /src/build/census.sh /w/$who emit > /w/emit-$who.txt
  sed 's#^/src/##' /w/emit-$who.txt | sort > /out/emit-linux-$who.s
  echo "$who done $(wc -l < /w/emit-$who.txt)"
  cd /w
done
