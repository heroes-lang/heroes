#!/bin/bash
# buildall.sh: every program of p/ with the base and the (b'') compiler at the run form's four builds, into o/<prog>/.
set -u
S=/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/263-critic3-scratch
BASE_C=$S/base/heroes-next; BASE_R=$S/base/runtime
BPP_C=$S/../263-critic3/heroes-next; BPP_R=$S/../263-critic3/runtime
for prog in "$@"; do
  d=$S/p/$prog; o=$S/o/$prog; mkdir -p $o
  for h in $d/*.hero; do
    n=$(basename $h .hero)
    $S/bin/b4.sh $BASE_C $BASE_R $d $n $o/$n-base
    $S/bin/b4.sh $BPP_C $BPP_R $d $n $o/$n-bpp
  done
done
echo BUILT "$@"
