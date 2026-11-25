#!/bin/bash
# A clang that logs each call, its seconds and its first and last words, then runs the real one.
s=$(perl -MTime::HiRes=time -e 'printf "%.3f", time')
/usr/bin/clang "$@"; c=$?
e=$(perl -MTime::HiRes=time -e 'printf "%.3f", time')
echo "$(echo "$e - $s" | bc) $c $(echo "$*" | cut -c1-50) ... $(echo "$*" | rev | cut -c1-60 | rev)" >> /Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/panel260-scratch/wrap/log.txt
exit $c
