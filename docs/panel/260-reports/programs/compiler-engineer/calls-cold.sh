#!/bin/bash
# calls-cold.sh <label> <compiler-abs> <program.hero> [words...]: one cold build in a fresh folder with clang wrapped,
# each call's seconds and line kept; prints the count, the seconds in clang, and the seconds of the call compiling the
# runtime's `runtime.c`, the program's units and the `-M`/`-E` questions, apart.
S=/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/260-compiler-engineer-scratch
l=$1; c=$2; p=$3; shift 3; pn=$(basename $p .hero)
d=$S/t/cold-$l-$pn; rm -rf $d; mkdir -p $d; cp $p $d/; cd $d
rm -f $S/wrap/log.txt; touch $S/wrap/log.txt
PATH=$S/wrap:$PATH "$c" build $pn.hero "$@" -o bin > /dev/null 2>&1; code=$?
f=$S/wrap/log.txt
echo "$l $pn cold exit=$code calls $(wc -l < $f | tr -d ' '), seconds $(awk '{s+=$1} END {printf "%.2f", s}' $f): runtime.c $(grep 'runtime\.c' $f | grep -v -- '-###' | awk '{s+=$1} END {printf "%.2f", s}'), units $(grep -E ' -c ' $f | grep -v 'runtime\.c' | awk '{s+=$1} END {printf "%.2f", s}'), -M or -E $(grep -E ' -M( |$)| -E ' $f | awk '{s+=$1} END {printf "%.2f", s}'), platform/ $(grep -c 'platform/' $f)"
cp $f $S/cold-$l-$pn.log
