#!/bin/bash
set -u
mkdir -p /tmp/b && cd /tmp/b && cp -r /src/seed /src/runtime /src/selfhost . && mkdir -p p && cp /src/w/padh/pad.hero /src/w/padh/pad_today.hero /src/w/padh/padprobe.h p/
export HEROES_RUNTIME=/tmp/b/runtime
clang -O2 -I runtime seed/heroes.c runtime/runtime.c -o heroes-seed
./heroes-seed build selfhost/main.hero -o heroes-r1 > /dev/null 2>&1
cd p && uname -m
for O in -O0 -O2; do ../heroes-r1 build pad.hero $O -o pad$O > /dev/null 2>&1; ../heroes-seed build pad_today.hero $O -o today$O > /dev/null 2>&1; echo "$O: rest: zero -> $(./pad$O) nonzero of 4; today's partial construction -> $(./today$O) nonzero of 4"; done
