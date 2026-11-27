#!/bin/sh
# The repair's key of the programs it added or changed (h2.h's first line, h9 to h12): the clerk's key.sh's loop, over these alone.
set -u
C=$1
OUT=$2
P=$3
mkdir -p "$OUT"
for n in h2 h9 h10 h11 h12 h13; do
    src=$P/$n.hero
    for mode in plain sanitize; do
        flag=
        [ $mode = sanitize ] && flag=--sanitize
        if ! "$C" build "$src" $flag -o "$OUT/bin-$n-$mode" > "$OUT/build-$n-$mode.txt" 2>&1; then
            echo "$n $mode: BUILD FAILED: $(tail -3 "$OUT/build-$n-$mode.txt" | tr '\n' ' ')"
            continue
        fi
        for i in 1 2 3; do
            timeout 60 "$OUT/bin-$n-$mode" > "$OUT/out-$n-$mode-$i.txt" 2>&1
            echo "exit $?" >> "$OUT/out-$n-$mode-$i.txt"
        done
        distinct=$(for i in 1 2 3; do sed -E 's/==[0-9]+==/==PID==/; s/0x[0-9a-f]+/0xADDR/g; s/\([0-9]+\)$/(N)/' "$OUT/out-$n-$mode-$i.txt" | shasum | cut -c1-12; done | sort -u | wc -l | tr -d ' ')
        echo "$n $mode: $(tail -1 "$OUT/out-$n-$mode-1.txt"), $distinct distinct of 3: $(grep -v WARNING "$OUT/out-$n-$mode-1.txt" | grep -v 'False positive' | head -3 | cut -c1-170 | tr '\n' '|')"
    done
done
