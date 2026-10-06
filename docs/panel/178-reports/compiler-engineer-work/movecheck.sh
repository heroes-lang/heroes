#!/bin/bash
# For every .hero under tests/golden and examples: `check` exit code and diagnostic codes, one line each.
# Usage: movecheck.sh <absolute compiler> <out>
c=$1; out=$2; : > "$out"
root=$PWD
export HEROES_RUNTIME=$root/runtime
find tests/golden examples -name '*.hero' | sort | while read -r f; do
  d=$(dirname "$f"); b=$(basename "$f")
  codes=$(cd "$d" && "$c" check "$b" 2>&1 | grep -oE '^(error|warning)\[[a-z_]+\]' | sort | tr '\n' ' ')
  rc=$(cd "$d" && "$c" check "$b" >/dev/null 2>&1; echo $?)
  echo "$f :: exit=$rc $codes" >> "$out"
done
