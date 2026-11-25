#!/bin/bash
# own-headers.sh <tree>: the tracked headers outside runtime/ that include a header of a C member of the
# platform layer, and the tracked run goldens whose groups name none of the layer's headers and one of those.
cd "$1" || exit 2
L=$(ls runtime/platform/*.c | xargs -n1 basename | sed 's/\.c$//' | tr '\n' '|' | sed 's/|$//')
echo "## tracked headers outside runtime/ including a layer header"
git ls-files -z '*.h' | grep -zv '^runtime/' | xargs -0 grep -lE "#[[:space:]]*include[[:space:]]*[<\"]($L)\.h[>\"]"
H=$(git ls-files -z '*.h' | grep -zv '^runtime/' | xargs -0 grep -lE "#[[:space:]]*include[[:space:]]*[<\"]($L)\.h[>\"]" | xargs -n1 basename | tr '\n' '|' | sed 's/|$//')
echo "## tracked run goldens binding the layer only through one of them"
for p in $(git ls-files 'tests/golden/run/*.hero'); do
  g=$(grep -oE '^extern "[^"]+"' "$p" | tr -d '"' | awk '{print $2}')
  own=$(echo "$g" | grep -cE "^($H)$"); lay=$(echo "$g" | grep -cE "^($L)\.h$")
  [ "$own" -gt 0 ] && [ "$lay" -eq 0 ] && echo "$p"
done
