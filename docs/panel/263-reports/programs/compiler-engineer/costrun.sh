#!/bin/bash
# costrun.sh <dir> <rounds>: alternates base, c2, b of each cost program, one line per run: program level side
# instructions-retired ns answer. The first round is discarded by the reader.
set -u
D=$1; N=$2
for p in cost_cb cost_qsort cost_calls cost_rec; do for lv in O2 O0; do
  for i in $(seq 1 $N); do for side in base c2 b; do
    [ -x "$D/$p-$side-$lv" ] || continue
    out=$( { /usr/bin/time -l "$D/$p-$side-$lv" > "$D/ans.txt"; } 2>&1 )
    ins=$(echo "$out" | awk '/instructions retired/ {print $1}'); ns=$(echo "$out" | awk '/^ns / {print $2}')
    echo "$p $lv $side $i $ins $ns $(cat "$D/ans.txt")"
  done; done
done; done
