#!/bin/bash
# A clang that logs each call whole, its seconds and its exit, then runs the real one (`ground/clang-wrapper.sh`
# with the whole line kept, so a `-M` in the middle of a long line is seen).
s=$(perl -MTime::HiRes=time -e 'printf "%.3f", time')
/usr/bin/clang "$@"; c=$?
e=$(perl -MTime::HiRes=time -e 'printf "%.3f", time')
echo "$(echo "$e - $s" | bc) $c $*" >> /Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/260-compiler-engineer-scratch/wrap/log.txt
exit $c
