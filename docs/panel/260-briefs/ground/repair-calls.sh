#!/bin/bash
# calls.sh <label> <compiler-abs>: a warm `print(1)` (a cold one first, then a warm one, then the counted one)
# with clang wrapped to log each call; prints the count of calls and of `-M` calls.
S=/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/panel260-repair-scratch
d=$S/t/calls-$1-${3:-print}; rm -rf $d; mkdir -p $d; cd $d
"$2" build $S/progs/${3:-print}.hero -o bin > /dev/null 2>&1; "$2" build $S/progs/${3:-print}.hero -o bin > /dev/null 2>&1
rm -f $S/wrap/log.txt
PATH=$S/wrap:$PATH "$2" build $S/progs/${3:-print}.hero -o bin > /dev/null 2>&1; code=$?
echo "$1 exit=$code clang calls $(wc -l < $S/wrap/log.txt | tr -d ' '), of them -M $(grep -c ' -M ' $S/wrap/log.txt), seconds in clang $(awk '{s+=$1} END {print s}' $S/wrap/log.txt)"
cp $S/wrap/log.txt $S/calls-$1-${3:-print}.log
