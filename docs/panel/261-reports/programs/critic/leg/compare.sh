#!/bin/sh
# The critic: each case's platform-free verdict under two runs, side by side.
#   sh compare.sh <dir A> <dir B>
A=$1; B=$2; S=$(dirname "$0")/summ.sh
same=0; diff=0
for f in "$A"/*.out; do
  n=$(basename "$f")
  case "$n" in ._*) continue ;; esac
  if [ ! -f "$B/$n" ]; then echo "MISSING $n"; diff=$((diff+1)); continue; fi
  a=$(sh "$S" "$f" | grep -v '^  the generated\|^  $\|error.* generated\|^  In file\|^  [0-9]* | ')
  b=$(sh "$S" "$B/$n" | grep -v '^  the generated\|^  $\|error.* generated\|^  In file\|^  [0-9]* | ')
  if [ "$a" = "$b" ]; then same=$((same+1)); else diff=$((diff+1)); echo "DIFF $n"; echo "$a" | head -6 | sed 's/^/  A: /'; echo "$b" | head -6 | sed 's/^/  B: /'; fi
done
echo "same $same differ $diff"
