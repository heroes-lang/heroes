#!/bin/bash
# Build routes.c under the emitter's own flags at -O0 and -O2 and run it.
# Usage: build_routes.sh <srcdir> <outdir>
set -u
SRC=$1; OUT=$2; mkdir -p "$OUT"
FLAGS=$(cat "$SRC/flags.txt")
for O in -O0 -O2; do
  echo "=== $(uname -s) $(uname -m) $O"
  clang $FLAGS $O "$SRC/routes.c" -o "$OUT/routes" -lpthread 2>&1 | grep -E 'error|warning'
  "$OUT/routes"; echo "EXIT=$?"
done
