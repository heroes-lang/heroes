#!/bin/bash
# Builds each shape beside (x_*.hero, ctx-*.hero) plain and --sanitize with the
# compiler $1, runs each binary 3 times under timeout 20, prints one line per
# distinct outcome: name, mode, count, exit, stdout, first panic/ASan line.
H=${1:-./heroes}
clang -std=gnu11 -shared -fPIC -O2 xs.c -o libxs.so || echo "libxs.so BUILD FAILED"
for f in ${EXTRA:-x_*.hero ctx-*.hero}; do
  n=${f%.hero}
  for mode in plain sanitize; do
    flag=""; [ "$mode" = sanitize ] && flag="--sanitize"
    if ! "$H" build "$f" $flag -o "bin-$n-$mode" > "bin-$n-$mode.build" 2>&1; then
      echo "$n $mode BUILD-FAILED $(head -c 300 "bin-$n-$mode.build" | tr '\n' ' ')"; continue
    fi
    for i in 1 2 3; do
      timeout 20 "./bin-$n-$mode" > o.txt 2> e.txt; code=$?
      echo "exit=$code | $(tr '\n' '|' < o.txt | head -c 80) | $(grep -m1 -E 'panic|ERROR: AddressSanitizer|SEGV|runtime error' e.txt | cut -c1-150)"
    done | sort | uniq -c | sed "s/^ */$n $mode /"
  done
done
