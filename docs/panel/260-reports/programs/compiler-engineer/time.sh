#!/bin/bash
# time.sh <out> <rounds> <name> <label=compiler-abs=program-abs>... : `ground/repair-time.sh` with each compiler given
# its own spelling of the program (the base's compiler cannot read `layer`, and this route's refuses a layer
# header without it) and the level's words in $WORDS. `heroes build <program> $WORDS -o bin`, warm and cold, the
# compilers interleaved inside each round; round 1 is discarded by the reader. warm: one folder per compiler, built
# once before round 1 (prime), then once per round; cold: a fresh folder per run. Each line: name label mode round
# real user sys exit load.
S=/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/260-compiler-engineer-scratch
out=$1; rounds=$2; pn=$3; shift 3
tag=$(echo "$WORDS" | tr -d ' -'); tag=${tag:-O0}
echo "# start $pn $tag $(date '+%Y-%m-%d %H:%M:%S') $(uptime)" >> "$out"
one() { # label compiler program folder mode round
  cd "$4" || exit 2
  t=$( /usr/bin/time -p sh -c 'o=$1; e=$2; shift 2; exec "$@" > "$o" 2> "$e"' x "$4/out.txt" "$4/err.txt" "$2" build "$3" $WORDS -o "$4/bin" 2>&1 ); c=$?
  echo "$pn $tag $1 $5 $6 $(echo "$t" | awk '/^real/{r=$2} /^user/{u=$2} /^sys/{s=$2} END{print r, u, s}') exit=$c load=$(sysctl -n vm.loadavg | awk '{print $2}')" >> "$out"
}
for spec in "$@"; do l=${spec%%=*}; rest=${spec#*=}; c=${rest%%=*}; p=${rest#*=}; w=$S/t/$pn-$tag-$l-warm; rm -rf $w; mkdir -p $w; one "$l" "$c" "$p" $w prime 0; done
for r in $(seq 1 "$rounds"); do
  for spec in "$@"; do l=${spec%%=*}; rest=${spec#*=}; c=${rest%%=*}; p=${rest#*=}; one "$l" "$c" "$p" $S/t/$pn-$tag-$l-warm warm $r; done
  for spec in "$@"; do l=${spec%%=*}; rest=${spec#*=}; c=${rest%%=*}; p=${rest#*=}; w=$S/t/$pn-$tag-$l-cold; rm -rf $w; mkdir -p $w; one "$l" "$c" "$p" $w cold $r; done
done
echo "# done $pn $tag $(date '+%Y-%m-%d %H:%M:%S') $(uptime)" >> "$out"
