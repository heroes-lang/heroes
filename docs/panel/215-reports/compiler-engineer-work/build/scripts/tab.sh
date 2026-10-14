#!/bin/bash
# One row per program: reports at -O0/-O1/-O2/-O3/-O2 -fno-inline, today | route, and any output that differs from .expected.
f=$1
for prog in $(grep -oE '^(today|g) [a-z0-9_.-]+\.hero: hero_pad_zero lines' "$f" | awk '{print $2}' | tr -d ':' | awk '!s[$0]++'); do
  row=""
  for comp in today g; do
    cells=""
    for O in "-O0" "-O1" "-O2" "-O3" "-O2 -fno-inline"; do
      n=$(grep -F "$comp $prog $O: " "$f" | grep -oE '[0-9]+ reports' | head -1 | cut -d' ' -f1)
      cells="$cells${n:-?}/"
    done
    pads=$(grep -F "$comp $prog: hero_pad_zero lines" "$f" | grep -oE '[0-9]+$')
    row="$row $comp ${cells%/} (zeroing lines $pads) |"
  done
  diffs=$(grep -F "$prog" "$f" | grep -c 'expected:DIFFERS')
  printed=$(grep -E "^(today|g) $prog " "$f" | grep -oE 'printed: .*' | sed 's/ expected:.*//' | sort -u | tr '\n' '#')
  echo "$prog |$row differs-from-expected $diffs | outputs: $printed"
done
grep -E 'dv.hero|main.hero' "$f"
