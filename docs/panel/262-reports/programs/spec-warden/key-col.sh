#!/bin/sh
# The spec-warden's second column: blind/key.sh's loop (same build, same three runs under
# `timeout 60`, same normalisation) over the programs named, with the compiler and
# HEROES_RUNTIME given by the caller. From the copy's root:
#   HEROES_RUNTIME=<dir> sh key-col.sh <compiler> <out> <program.hero>...
set -u
C=$1
OUT=$2
shift 2
mkdir -p "$OUT"
for src in "$@"; do
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
        echo "$n $mode: $(tail -1 "$OUT/out-$n-$mode-1.txt"), $distinct distinct of 3: $(grep -v '^==' "$OUT/out-$n-$mode-1.txt" | grep -v 'ignoring requested' | head -2 | cut -c1-150 | tr '\n' '|')"
    done
done
