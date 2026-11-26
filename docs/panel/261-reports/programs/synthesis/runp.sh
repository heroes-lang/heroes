#!/bin/bash
# runp.sh <compiler-tag> <file.hero>: `check` then `run` (under timeout 60, output bounded), each exit printed
here=$(cd "$(dirname "$0")" && pwd); c="$here/bin/heroes-$1"; f=$2
out=$("$c" check "$f" 2>&1); e=$?
printf '%s\n' "$out" | grep -E '^(error|warning)\[' | sort | uniq -c
echo "check exit $e"
if [ $e -eq 0 ]; then
  timeout 60 "$c" run "$f" 2>&1 | head -c 4000; echo "run exit ${PIPESTATUS[0]}"
fi
