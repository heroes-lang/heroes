#!/bin/sh
# Panel 262's repair, inside heroes-linux-arm64 at /w: seed, stage 2, the blind key (h1 to h12, n1 to n4),
# the probes and the narrowed run cases, the route accessor's case, and the critic's watched program.
set -u
export TMPDIR=/w/.tmp
cd /w
{ uname -a; head -3 /etc/os-release; ldd --version | head -1; clang --version | head -1; } > L-env.txt 2>&1
clang -I runtime seed/heroes.c runtime/runtime.c -o heroes > L-build-seed.txt 2>&1; echo "seed exit $?" >> L-build-seed.txt
./heroes --version >> L-build-seed.txt 2>&1
./heroes build selfhost/main.hero -o heroes-next > L-build-stage2.txt 2>&1; echo "stage2 exit $?" >> L-build-stage2.txt
./heroes-next --version >> L-build-stage2.txt 2>&1
sh docs/panel/262-briefs/blind/key.sh ./heroes-next L-key > L-key.txt 2>&1
sh docs/panel/262-briefs/ground/probes.sh ./heroes-next L-probes > L-probes.txt 2>&1
./heroes-next run tests/harness/main.hero -- ./heroes-next run fixedbugs-2679-the-runtime-says-which-route > L-2679.txt 2>&1
for p in watched same; do for mode in plain sanitize; do flag=; [ $mode = sanitize ] && flag=--sanitize;
  ./heroes-next build critic/$p.hero $flag -o L-bin-$p-$mode > L-build-$p-$mode.txt 2>&1;
  for i in 1 2 3; do timeout 60 ./L-bin-$p-$mode > L-out-$p-$mode-$i.txt 2>&1; echo "exit $?" >> L-out-$p-$mode-$i.txt; done; done; done
echo ALL-DONE > L-done.txt
