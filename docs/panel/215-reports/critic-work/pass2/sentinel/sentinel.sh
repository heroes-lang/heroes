#!/bin/bash
# usage: sentinel.sh <dir holding s_main.c and s_through.c>
cd "$1" || exit 1
echo "leg $(uname -s) $(uname -m), $(clang --version | head -1)"
for O in -O0 -O1 -O2 -O3; do
  clang -std=gnu11 $O s_main.c s_through.c -o s$O 2> cc.txt || { echo "$O cc failed"; head -3 cc.txt; continue; }
  echo "$O: $(./s$O | tr '\n' ' ')"
done
