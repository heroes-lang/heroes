#!/bin/bash
# shapes.sh <label> <compiler-abs> : `heroes build` of every shape, in a copy of shapes/ of the
# label's own (one shared build/ there), each line: shape exit first-stderr-line.
S=/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/panel260-scratch
l=$1; c=$2; d=$S/t/shapes-$l; rm -rf $d; mkdir -p $S/t; cp -R $S/shapes $d; cp $S/progs/*.hero $d/
out=$S/shapes-$l.txt; echo "# $l $c $(date '+%H:%M:%S') load $(sysctl -n vm.loadavg)" > $out
cd $d
for p in ${ONLY:-print.hero clock.hero audio.hero ui.hero usemod/main.hero ownhdr/main.hero owndecl/main.hero rebel/reach_hero_time.hero rebel/reach_hero_audio.hero rebel/reach_hero_net.hero $(ls $S/shapes/*.hero | xargs -n1 basename)}; do
  [ -f "$p" ] || continue
  n=$(echo "$p" | sed 's|/|-|g; s|\.hero$||')
  ( cd $(dirname $p) && "$c" build $(basename $p) -o $d/bin-$n > $d/$n.out 2> $d/$n.err ); code=$?
  echo "$p exit=$code $(grep -m1 -E 'error|undefined|Undefined' $d/$n.err | cut -c1-200)" >> $out
done
echo "# done $(date '+%H:%M:%S')" >> $out
