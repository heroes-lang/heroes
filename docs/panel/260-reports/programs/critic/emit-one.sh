#!/bin/bash
# emit-one.sh <file>: the route's --emit-c of one program from its own folder; prints the file and, where the
# layer's class is told, its first line.
f=$1; d=$(dirname "$f"); b=$(basename "$f")
R=/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/260-critic-scratch/r2/route
err=$(cd "$R/$d" && "$R/heroes-route" build "$b" --emit-c 2>&1 >/dev/null); c=$?
l=$(printf '%s\n' "$err" | grep -m1 -E 'error\[' | cut -c1-200)
echo "$c | $f | $l"
