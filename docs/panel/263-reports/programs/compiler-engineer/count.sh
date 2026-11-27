#!/bin/bash
# count.sh <dir> <label>: the counting canaries in <dir>, by level, stack size and CO_N; one line a run.
set -u
D=$1; L=$2
one() { { timeout -k 5 20 "$D/$1" 2>&1; echo "exit $?"; } | head -c 8000 > "$D/last.txt"
  printf '%-6s %-16s KB%-5s n%-7s %-9s %s\n' "$L" "$1" "${CO_KB:-256}" "$CO_N" "$(tail -1 "$D/last.txt")" "$(grep -E '^co:|^panic|ERROR|ran on' "$D/last.txt" | head -2 | sed -E 's/ \(a stack check[^)]*\)//' | tr '\n' '|' | cut -c1-150)"; }
for p in canary4 canary6; do
  for lv in O0 O2; do
    if [ $lv = O0 ]; then ns="1000 1200 1500 2000 3000 30000"; else ns="7000 9000 10000 14000 50000"; fi
    for b in $p-$lv $p-san-$lv; do for n in $ns; do CO_N=$n one $b; done; done
  done
done
for kb in 16 64 1024; do
  for b in canary4-O2 canary4-san-O2 canary6-O2 canary4-O0; do
    for n in 300 500 1000 40000 1000000; do CO_KB=$kb CO_N=$n one $b; done
  done
done
