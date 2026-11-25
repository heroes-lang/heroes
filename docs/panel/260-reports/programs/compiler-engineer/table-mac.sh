#!/bin/bash
# table-mac.sh: this Mac's timing table, `time.sh` per program and level, 4 rounds (round 1 discarded by the reader):
# print with old, the base and the route; clock, audio and ui with the base (as written) and the route (declared).
# Each compiler beside its own runtime; old read only (`rebel-scratch/perf/old`).
S=/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/260-compiler-engineer-scratch
P=$(dirname "$0")
OLD=/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/rebel-scratch/perf/old/heroes
out=$S/time-mac.txt
for w in "" "-O2" "--sanitize"; do
  WORDS="$w" bash $P/time.sh $out 4 print old=$OLD=$S/progs/print.hero base=$S/base-o2/heroes=$S/progs/print.hero route=$S/mine-o2/heroes=$S/progs/print.hero
  for p in clock audio ui; do
    WORDS="$w" bash $P/time.sh $out 4 $p base=$S/base-o2/heroes=$S/progs/$p.hero route=$S/mine-o2/heroes=$S/progs-declared/$p.hero
  done
done
echo "# table done $(date '+%H:%M:%S')" >> $out
