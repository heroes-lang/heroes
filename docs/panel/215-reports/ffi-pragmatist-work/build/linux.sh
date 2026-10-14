#!/bin/sh
# Panel 215, the ffi-pragmatist's build of route C: every shape under today's
# compiler and the route's, inside the container, in the container's own /w.
set -u
mkdir -p /w && cd /w
cp -r /src/runtime .
clang -I runtime /src/seed/heroes.c runtime/runtime.c -o heroes 2>&1 | tail -2
clang -I runtime /src/build/heroes-c.c runtime/runtime.c -o heroes-c 2>&1 | tail -2
./heroes --version; ./heroes-c --version; clang --version | head -1
cp -r /src/build/shapes shapes; cp -r /src/build/kept kept
mkdir -p g ct; cp /src/tests/golden/run/fixedbugs-413-* g/; cp /src/examples/ctime/* ct/
one() {
  for c in heroes heroes-c; do
    out=$(./$c build "$1" -o bin 2>&1); code=$?
    if [ $code = 0 ]; then res=$(timeout 60 ./bin 2>&1 | head -c 200 | tr '\n' '|'); else
      res=$(printf '%s\n' "$out" | grep -oE '^error\[[a-z_]+\]' | sort | uniq -c | tr -s ' ' | tr '\n' ' '); fi
    echo "$1 $c: build $code | $res"
  done
}
for f in shapes/mx.hero shapes/copy.hero shapes/ctor.hero shapes/arr.hero shapes/holder.hero shapes/b.hero shapes/race.hero shapes/lend.hero kept/r*/b.hero; do one "$f"; done
for f in g/*.hero ct/main.hero; do
  e=${f%.hero}.expected
  for c in heroes heroes-c; do
    ./$c run "$f" > out-$c.txt 2>&1; code=$?
    if cmp -s out-$c.txt "$e"; then m=same; else m=DIFF; fi
    echo "$f $c: exit $code vs .expected: $m $(grep -oE '^error\[[a-z_]+\]' out-$c.txt | sort | uniq -c | tr '\n' ' ')"
  done
  cmp -s out-heroes.txt out-heroes-c.txt && echo "  today and route identical"
done
./heroes-c build shapes/mx.hero -o bin 2>&1 | head -3
