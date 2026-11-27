#!/bin/bash
# runc1.sh <dir> <runs>: route (c1)'s binaries built by c1.sh, each run under timeout -k 5 20, one line per run.
set -u
D=$1; N=$2
one() { # <label> <binary>, CO_N as set
  { timeout -k 5 20 "$D/$2" 2>&1; echo "exit $?"; } | head -c 8000 > "$D/last.txt"
  printf '%-28s %-9s %s\n' "$1" "$(tail -1 "$D/last.txt")" "$(grep -v '^exit' "$D/last.txt" | tr '\n' '|' | cut -c1-150)"
}
for b in malloc guarded bare thread static; do for lv in O0 O2; do for i in $(seq 1 $N); do one "$b-$lv #$i" "$b-$lv"; done; done; done
for lv in O0 O2; do for i in $(seq 1 $N); do one "malloc-san-$lv #$i" "malloc-san-$lv"; one "static-san-$lv #$i" "static-san-$lv"; done; done
echo "== canary, plain and sanitized, CO_N"
for spec in "O2 7000" "O2 9000" "O2 10000" "O2 14000" "O2 50000" "O0 1200" "O0 1500" "O0 2000" "O0 3000" "O0 30000"; do
  lv=${spec%% *}; n=${spec##* }
  for side in canary canary-san; do for i in $(seq 1 $N); do CO_N=$n one "$side-$lv n$n #$i" "$side-$lv"; done; done
done
echo "== malloc by CO_N"
for n in 1600 2000 3000 10000 20000 30000 50000; do CO_N=$n one "malloc-O0 n$n" malloc-O0; CO_N=$n one "malloc-san-O0 n$n" malloc-san-O0; done
for n in 9600 9800 10000 14000; do CO_N=$n one "malloc-O2 n$n" malloc-O2; CO_N=$n one "malloc-san-O2 n$n" malloc-san-O2; done
for n in 1 1300 9600; do CO_N=$n one "thread-O0 n$n" thread-O0; CO_N=$n one "static-O0 n$n" static-O0; CO_N=$n one "static-O2 n$n" static-O2; done
left=$(pgrep -f "$D/" || true); if [ -n "$left" ]; then echo "STILL RUNNING: $left"; fi
