#!/bin/sh
# The spec-warden's Linux arm64 column: the base's seed and stage 2, programs built against main's runtime (deef27472).
set -u
export TMPDIR=/w/.tmp
mkdir -p $TMPDIR
cd /w
{ uname -a; head -3 /etc/os-release; ldd --version | head -1; clang --version | head -1; } > W-env.txt 2>&1
clang -I runtime seed/heroes.c runtime/runtime.c -o heroes > W-build-seed.txt 2>&1; echo "seed exit $?" >> W-build-seed.txt
./heroes --version >> W-build-seed.txt 2>&1
./heroes build selfhost/main.hero -o heroes-next > W-build-stage2.txt 2>&1; echo "stage2 exit $?" >> W-build-stage2.txt
./heroes-next --version >> W-build-stage2.txt 2>&1
P=progs
HEROES_RUNTIME=/w/main-rt/runtime sh key-col.sh ./heroes-next W-main $P/mj_carved.hero $P/escape_carved.hero $P/escape_heap.hero $P/n1.hero $P/n2.hero $P/n3.hero $P/h13.hero $P/h1.hero $P/h3.hero $P/h7.hero $P/h9.hero $P/h10.hero $P/h11.hero $P/h12.hero $P/h8.hero $P/n4.hero > W-main.txt 2>&1
echo ALL-DONE > W-done.txt
