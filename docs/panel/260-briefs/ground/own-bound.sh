#!/bin/bash
# own-bound.sh <tree>: for each run golden own-headers.sh lists, the functions its groups bind and how many of
# them are named `hero_*` (a name a layer header could declare).
cd "$1" || exit 2
for p in $(sed -n '/^## tracked run goldens/,$p' /Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/panel260-scratch/own-headers.txt | grep '\.hero$'); do
  b=$(grep -A40 '^extern' "$p" | grep -oE '^    function [a-z_0-9]+' | awk '{print $2}' | sort -u)
  echo "$(basename "$p" .hero): binds $(echo "$b" | grep -c .) ($(echo $b | tr '\n' ' ')), of them hero_* $(echo "$b" | grep -cE '^hero_')"
done
