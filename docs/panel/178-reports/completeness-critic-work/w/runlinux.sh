#!/bin/bash
# Critic, panel 178: build a compiler from the given C inside the container, run each program.
# usage: runlinux.sh <compiler-C> <dir-under-/src/w> <prog.hero>...
set -u
mkdir -p /tmp/r && cd /tmp/r
cp -r /src/runtime . ; export HEROES_RUNTIME=/tmp/r/runtime
comp=$1; d=$2; shift 2
cp -r /src/w/$d/. .
clang -O1 -I runtime "$comp" runtime/runtime.c -o hc 2>/dev/null || { echo "compiler build failed"; exit 1; }
echo "leg $(uname -m)"
for p in "$@"; do echo "== $p"; ./hc run $p > o.txt 2>&1; e=$?; tail -12 o.txt; echo "EXIT=$e"; done
