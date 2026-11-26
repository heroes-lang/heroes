#!/bin/sh
# second reading: base compilers at df92f9281 in my copy, then the route from route-v4.diff in a copy of the base
set -u
W=/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees
cd $W/261-critic || exit 1
echo "base seed $(date +%T)"; /usr/bin/time -p clang -I runtime seed/heroes.c runtime/runtime.c -o heroes || exit 1
echo "base stage2 $(date +%T)"; /usr/bin/time -p ./heroes build selfhost/main.hero -o heroes-next || exit 1
./heroes-next --version
R=$W/261-critic-scratch/r2/route
rm -rf $R; mkdir -p $R
git archive df92f9281 | tar -x -C $R || exit 1
cd $R || exit 1
git apply --whitespace=nowarn $W/panel261/docs/panel/261-reports/programs/compiler-engineer/route-v4.diff || { echo APPLY FAILED; exit 1; }
echo "route seed $(date +%T)"; /usr/bin/time -p clang -I runtime seed/heroes.c runtime/runtime.c -o heroes || exit 1
echo "route stage2 $(date +%T)"; /usr/bin/time -p ./heroes build selfhost/main.hero -o heroes-route || exit 1
./heroes-route --version
echo "done $(date +%T)"
