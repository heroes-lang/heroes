#!/bin/bash
# calls.sh <label> <compiler-abs> <program.hero> [words...]: a cold build, a warm one, then the counted warm one, with
# clang wrapped to log each call whole; prints the count of calls, of those naming a member of the layer, of `-M`
# calls, and the seconds in clang; the log is kept as calls-<label>-<program>.log.
S=/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/260-compiler-engineer-scratch
l=$1; c=$2; p=$3; shift 3; pn=$(basename $p .hero)
d=$S/t/calls-$l-$pn; rm -rf $d; mkdir -p $d; cp $p $d/; cd $d
"$c" build $pn.hero "$@" -o bin > /dev/null 2>&1; "$c" build $pn.hero "$@" -o bin > /dev/null 2>&1
rm -f $S/wrap/log.txt; touch $S/wrap/log.txt
PATH=$S/wrap:$PATH "$c" build $pn.hero "$@" -o bin > /dev/null 2>&1; code=$?
echo "$l $pn exit=$code clang calls $(wc -l < $S/wrap/log.txt | tr -d ' '), naming platform/ $(grep -c 'platform/' $S/wrap/log.txt), with -M $(grep -cE ' -M( |$)' $S/wrap/log.txt), seconds in clang $(awk '{s+=$1} END {print s}' $S/wrap/log.txt), layer archives in build/ $(ls build 2>/dev/null | grep -c '^layer-.*\.a$')"
cp $S/wrap/log.txt $S/calls-$l-$pn.log
