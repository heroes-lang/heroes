#!/bin/bash
# costrun.sh <out file> <binary prefix pairs...>: each "<dir>/<prog>-<side>-<level>" named by its prefix list; six
# rounds alternated across the sides given, the first discarded, the best of five instructions retired printed.
set -u
O=$1; shift
for round in 1 2 3 4 5 6; do
  for b in "$@"; do
    n=$( { /usr/bin/time -l "$b" > /dev/null; } 2>&1 | awk '/instructions retired/ {print $1}')
    echo "$round $(basename $b) $n" >> "$O.raw"
  done
done
for b in "$@"; do
  best=$(awk -v n="$(basename $b)" '$2==n && $1>1 {print $3}' "$O.raw" | sort -n | head -1)
  echo "$(basename $b) $best" | tee -a "$O"
done
