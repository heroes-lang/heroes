#!/bin/bash
# costbuild.sh <stage2> <c2 compiler or -> <out>: base, c2 and b builds of the four cost programs, -O2 and -O0.
set -u
H=$1; C2=$2; O=$3
S=$(cd "$(dirname "$0")" && pwd)
mkdir -p "$O"; cd "$S/cost"
for p in cost_cb cost_qsort cost_calls cost_rec; do
  for lv in O2 O0; do
    "$H" build $p.hero -$lv -o "$O/$p-base-$lv" > "$O/b-$p-base-$lv.txt" 2>&1 || echo "base $p $lv failed"
    if [ "$C2" != "-" ]; then HEROES_RUNTIME=$S/rt-c "$C2" build $p.hero -$lv -o "$O/$p-c2-$lv" > "$O/b-$p-c2-$lv.txt" 2>&1 || echo "c2 $p $lv failed"; fi
  done
  bash "$S/b.sh" "$H" "$S/cost" $p "$S/rt-b" "$O/bsrc" > /dev/null || echo "b emit $p failed"
  for lv in O2 O0; do cp "$O/bsrc/$p-$lv" "$O/$p-b-$lv" 2>/dev/null || echo "b $p $lv missing"; done
done
