#!/bin/bash
# Every line an emission moved, read by class: each failed label of the
# emission suite re-emitted by the route's compiler and diffed against its
# blessed file. Usage: moved.sh <suite output>
R=/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/scratch-panel-215/ce-build
cd $R
export TMPDIR=$R/build
mkdir -p build/moved
total_add=0; total_del=0; pad=0; line=0; close=0; other=0
for label in $(grep -oE 'FAIL[^ ]* emission/[^ :]+' "$1" | sed 's/.*emission\///' | sort -u); do
  group=${label%%/*}; stem=${label#*/}
  if [ "$group" = examples ]; then base=${stem%%/*}; entry=${stem#*/}; src=examples/$base/$entry.hero; stored=tests/emission/examples-$base-$entry.c
  else src=tests/golden/$group/$stem.hero; stored=tests/emission/$group-$stem.c; fi
  out=build/moved/$(echo $label | tr '/' '-').c
  build/heroes-g build $src --emit-c > $out 2> /dev/null
  diff $stored $out > $out.diff
  a=$(grep -c '^>' $out.diff); d=$(grep -c '^<' $out.diff)
  p=$(grep '^>' $out.diff | grep -c 'hero_pad_zero(')
  l=$(grep '^>' $out.diff | grep -cE '^> #line [0-9]+ "')
  c=$(grep '^>' $out.diff | grep -cE '^>     \}$')
  o=$(grep '^>' $out.diff | grep -vE 'hero_pad_zero\(|^> #line [0-9]+ "|^>     \}$' | grep -c .)
  echo "$label: +$a -$d (zeroing $p, #line $l, closing $c, other $o)"
  grep '^>' $out.diff | grep -vE 'hero_pad_zero\(|^> #line [0-9]+ "|^>     \}$' | head -5 | sed 's/^/    OTHER /'
  grep '^<' $out.diff | head -3 | sed 's/^/    REMOVED /'
  total_add=$((total_add+a)); total_del=$((total_del+d)); pad=$((pad+p)); line=$((line+l)); close=$((close+c)); other=$((other+o))
done
echo "TOTAL +$total_add -$total_del: zeroing $pad, #line $line, closing $close, other $other"
