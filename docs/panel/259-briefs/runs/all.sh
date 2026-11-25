#!/bin/sh
# The repair's Linux run at the new base 3ec775b66: compilers, the carved shapes and `below` (plain and --sanitize, three runs a
# cell), the lane's other shapes (one plain run each), the blind programs and design C (three runs each), the dlsym probes, and
# on arm64 the critic's trace. Every output under /w; one summary line per run.
date -u
cd /w/tree
clang -I runtime seed/heroes.c runtime/runtime.c -o heroes > /w/build-seed.txt 2>&1; echo "seed exit $?"
./heroes build selfhost/main.hero -o heroes-next > /w/build-stage2.txt 2>&1; echo "stage2 exit $?"
./heroes --version; ./heroes-next --version; uname -m; ldd --version 2>&1 | head -1; clang --version | head -1
date -u
C=/w/tree/heroes-next
cd /w/runs
for s in own_carved unseen_carved; do
  $C build $s.hero -o bin-$s > build-$s.txt 2>&1; echo "build $s plain $?"
  $C build $s.hero --sanitize -o bin-$s-san > build-$s-san.txt 2>&1; echo "build $s san $?"
done
for b in own_carved own_carved-san unseen_carved unseen_carved-san; do
  for r in 1 2 3; do timeout 120 ./bin-$b > out-$b-$r.txt 2>&1; echo "run $b $r exit $? bytes $(wc -c < out-$b-$r.txt)"; done
done
cd /w/mine
$C build below.hero -o bin-below > build-below.txt 2>&1; echo "build below plain $?"
$C build below.hero --sanitize -o bin-below-san > build-below-san.txt 2>&1; echo "build below san $?"
for b in below below-san; do
  for r in 1 2 3; do timeout 120 ./bin-$b > out-$b-$r.txt 2>&1; echo "run $b $r exit $? bytes $(wc -c < out-$b-$r.txt)"; done
done
date -u
cd /w/blind
for q in q1 q2 q3 q4 q5 q6 q7; do $C build $q.hero -o bin-$q > build-$q.txt 2>&1 || echo "BUILD FAIL $q"; done
for q in q1 q2 q3 q4 q5 q6 q7; do
  for r in 1 2 3; do timeout 120 ./bin-$q > out-$q-$r.txt 2>&1; echo "$q $r exit $? bytes $(wc -c < out-$q-$r.txt)"; done
done
cd /w/designc
$C build q3m.hero -o bin-q3m > build-q3m.txt 2>&1 || echo "BUILD FAIL q3m"
for r in 1 2 3; do timeout 120 ./bin-q3m > out-q3m-$r.txt 2>&1; echo "q3m $r exit $? bytes $(wc -c < out-q3m-$r.txt)"; done
cd /w/probes
for p in dlh dlh2; do $C build $p.hero -o bin-$p > build-$p.txt 2>&1 || echo "BUILD FAIL $p"; timeout 60 ./bin-$p > out-$p.txt 2>&1; echo "$p exit $?: $(cat out-$p.txt | tr '\n' '|')"; done
nm -D bin-dlh 2>/dev/null | grep -E ' (makecontext|swapcontext)$'
date -u
cd /w/runs
for s in carved own_heap two three lend spawn migrate quit quit_fresh jumpco unseen ph; do
  $C build $s.hero -o bin-$s > build-$s.txt 2>&1 || { echo "$s BUILD FAIL"; continue; }
  timeout 120 ./bin-$s > out-$s.txt 2>&1; e=$?
  echo "$s exit $e: $(head -c 200 out-$s.txt | tr '\n' '|')"
done
if [ -d /w/runtime-trace ]; then
  for s in unseen_carved own_carved; do
    HEROES_RUNTIME=/w/runtime-trace $C build $s.hero -o bin-$s-trace > build-$s-trace.txt 2>&1; echo "build $s trace $?"
    timeout 60 ./bin-$s-trace > out-$s-trace.txt 2>&1; echo "run $s trace exit $?"
  done
fi
date -u
echo ALL-DONE
