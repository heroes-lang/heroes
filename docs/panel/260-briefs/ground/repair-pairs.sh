#!/bin/bash
# pairs.sh <out> <pairs> <program> <labelA=compilerA> <labelB=compilerB>: warm builds only, A and B alternated, the
# order inside a pair alternating too (AB, BA, AB ...), each compiler in its own warm folder primed twice first; the
# first pair is discarded by the reader. Each line: program pair label real user sys exit load.
S=/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/panel260-repair-scratch
out=$1; n=$2; P=$3; A=$4; B=$5; pn=$(basename $P .hero)
echo "# start $pn ${A%%=*} ${B%%=*} $(date '+%Y-%m-%d %H:%M:%S') $(uptime)" >> "$out"
one() { # label compiler pair
  d=$S/t/pairs-$pn-$1; cd "$d" || exit 2
  t=$( /usr/bin/time -p sh -c 'o=$1; e=$2; shift 2; exec "$@" > "$o" 2> "$e"' x "$d/out.txt" "$d/err.txt" "$2" build "$P" -o "$d/bin" 2>&1 ); c=$?
  echo "$pn $3 $1 $(echo "$t" | awk '/^real/{r=$2} /^user/{u=$2} /^sys/{s=$2} END{print r, u, s}') exit=$c load=$(sysctl -n vm.loadavg | awk '{print $2}')" >> "$out"
}
for x in "$A" "$B"; do l=${x%%=*}; rm -rf $S/t/pairs-$pn-$l; mkdir -p $S/t/pairs-$pn-$l; one "$l" "${x#*=}" prime; one "$l" "${x#*=}" prime; done
for i in $(seq 1 "$n"); do
  if [ $((i % 2)) -eq 1 ]; then first=$A; second=$B; else first=$B; second=$A; fi
  one "${first%%=*}" "${first#*=}" $i; one "${second%%=*}" "${second#*=}" $i
done
echo "# done $pn $(date '+%Y-%m-%d %H:%M:%S') $(uptime)" >> "$out"
