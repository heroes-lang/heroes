#!/bin/bash
# time.sh <out> <rounds> <program> <label=compiler-abs>... : ground/time.sh with the program an argument:
# `heroes build <program> -o bin`, warm and cold, the compilers interleaved inside each round; round 1 is
# discarded by the reader. warm: one folder per compiler and program, built once before round 1 (prime), then
# once per round; cold: a fresh folder per run. Each line: label mode round real user sys exit load.
S=/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/panel260-repair-scratch
out=$1; rounds=$2; P=$3; shift 3
pn=$(basename $P .hero)
echo "# start $pn $(date '+%Y-%m-%d %H:%M:%S') $(uptime)" >> "$out"
one() { # label compiler folder mode round
  cd "$3" || exit 2
  t=$( /usr/bin/time -p sh -c 'o=$1; e=$2; shift 2; exec "$@" > "$o" 2> "$e"' x "$3/out.txt" "$3/err.txt" "$2" build "$P" -o "$3/bin" 2>&1 ); c=$?
  echo "$pn $1 $4 $5 $(echo "$t" | awk '/^real/{r=$2} /^user/{u=$2} /^sys/{s=$2} END{print r, u, s}') exit=$c load=$(sysctl -n vm.loadavg | awk '{print $2}')" >> "$out"
}
for pair in "$@"; do l=${pair%%=*}; c=${pair#*=}; rm -rf $S/t/$pn-$l-warm; mkdir -p $S/t/$pn-$l-warm; one "$l" "$c" $S/t/$pn-$l-warm prime 0; done
for r in $(seq 1 "$rounds"); do
  for pair in "$@"; do l=${pair%%=*}; c=${pair#*=}; one "$l" "$c" $S/t/$pn-$l-warm warm $r; done
  for pair in "$@"; do l=${pair%%=*}; c=${pair#*=}; rm -rf $S/t/$pn-$l-cold; mkdir -p $S/t/$pn-$l-cold; one "$l" "$c" $S/t/$pn-$l-cold cold $r; done
done
echo "# done $pn $(date '+%Y-%m-%d %H:%M:%S') $(uptime)" >> "$out"
