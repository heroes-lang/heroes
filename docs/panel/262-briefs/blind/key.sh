#!/bin/sh
# Panel 262's blind key: every program of `programs/` built with the compiler
# named, plain and `--sanitize`, and run three times under `timeout 60`, its
# output and exit kept. Run from a copy's root (the compiler finds its runtime
# there): `sh docs/panel/262-briefs/blind/key.sh <compiler> <out>`.
# Prints one line per program and mode: exit, distinct outputs of three, the
# first two lines of the first run.
set -u
C=$1
OUT=$2
P=docs/panel/262-briefs/blind/programs
mkdir -p "$OUT"
for src in "$P"/h*.hero "$P"/n*.hero; do
    n=$(basename "$src" .hero)
    for mode in plain sanitize; do
        flag=
        [ $mode = sanitize ] && flag=--sanitize
        if ! "$C" build "$src" $flag -o "$OUT/bin-$n-$mode" > "$OUT/build-$n-$mode.txt" 2>&1; then
            echo "$n $mode: BUILD FAILED: $(tail -1 "$OUT/build-$n-$mode.txt")"
            continue
        fi
        for i in 1 2 3; do
            timeout 60 "$OUT/bin-$n-$mode" > "$OUT/out-$n-$mode-$i.txt" 2>&1
            echo "exit $?" >> "$OUT/out-$n-$mode-$i.txt"
        done
        distinct=$(for i in 1 2 3; do sed -E 's/==[0-9]+==/==PID==/; s/0x[0-9a-f]+/0xADDR/g; s/\([0-9]+\)$/(N)/' "$OUT/out-$n-$mode-$i.txt" | shasum | cut -c1-12; done | sort -u | wc -l | tr -d ' ')
        echo "$n $mode: $(tail -1 "$OUT/out-$n-$mode-1.txt"), $distinct distinct of 3: $(head -2 "$OUT/out-$n-$mode-1.txt" | cut -c1-150 | tr '\n' '|')"
    done
done
