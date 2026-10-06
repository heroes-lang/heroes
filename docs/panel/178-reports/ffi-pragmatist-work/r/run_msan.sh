#!/bin/bash
for O in -O0 -O1 -O2 -O3; do
  echo "=== $(uname -m) MSan $O"
  clang -std=gnu11 -fsanitize=memory $O -g /src/r/padmsan.c -o /tmp/pm 2>&1 | grep -E 'error|warning'
  MSAN_OPTIONS=exitcode=77:halt_on_error=1 /tmp/pm 2>/dev/null
done
