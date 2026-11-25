#!/bin/sh
# builds unseen_carved and own_carved against the traced runtime copy and runs each once
cd /w/runs
for s in unseen_carved own_carved; do
  HEROES_RUNTIME=/w/runtime-trace /w/tree/heroes-next build $s.hero -o bin-$s-trace > build-$s-trace.txt 2>&1; echo "build $s trace $?"
  timeout 30 ./bin-$s-trace > out-$s-trace.txt 2>&1; echo "run $s trace exit $?"
done
