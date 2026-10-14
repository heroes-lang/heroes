#!/bin/sh
# Panel 215, the ffi-pragmatist's build of route C, on the Windows box.
set -u
cd /c/w/p215-ffib/tree
clang -I runtime seed/heroes.c runtime/runtime.c -Wl,/STACK:67108864 -o heroes.exe 2>&1 | tail -2
clang -I runtime heroes-c.c runtime/runtime.c -Wl,/STACK:67108864 -o heroes-c.exe 2>&1 | tail -2
./heroes.exe --version; ./heroes-c.exe --version; clang --version | head -1
cd win
for f in wcs.hero wsrw.hero wmtx.hero wholder.hero wkept.hero; do
  for c in heroes heroes-c; do
    out=$(../$c.exe build "$f" -o bin.exe 2>&1); code=$?
    if [ $code = 0 ]; then res=$(timeout 60 ./bin.exe 2>&1 | head -c 200 | tr '\r\n' ' |'); else
      res=$(printf '%s\n' "$out" | grep -oE '^error\[[a-z_]+\]' | sort | uniq -c | tr -s ' ' | tr '\n' ' '); fi
    echo "$f $c: build $code | $res"
  done
done
for f in wcs.hero wsrw.hero wmtx.hero wholder.hero; do ../heroes-c.exe build "$f" -o bin.exe 2>&1 | grep -E '^error|^  at'; done
