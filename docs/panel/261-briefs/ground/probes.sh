#!/bin/sh
# Panel 261's probes, re-run by the repair (2026-11-26) at the base df92f9281. Run from a copy's root with the
# compiler to judge as the first argument and this folder's probe/ as the second; writes <probe>.repair.out beside
# each probe: what `check` prints and its exit, then, where `check` exits 0, what `run` prints and its exit.
# A `.hero.txt` probe does not parse by design (a commit's hook refuses a `.hero` that does not parse): it is copied
# to a `.hero` in the output folder (the third argument) and checked there. x1 is also built by the control arm,
# `build --permissive`, its binary run where the build exits 0. q2b is emitted (`--emit-c`) to q2b.c.
# usage: sh probes.sh ./heroes-next <path of ground/probe> <an output folder outside the tree's tracked files>
H=$1; P=$2; O=$3
[ -x "$H" ] && [ -d "$P" ] && [ -n "$O" ] || { echo "usage: sh probes.sh <compiler> <probe dir> <out dir>"; exit 2; }
mkdir -p "$O"
for t in "$P"/*.hero.txt; do cp "$t" "$O/$(basename "$t" .txt)"; done
for f in "$P"/*.hero "$O"/*.hero; do
    [ -f "$f" ] || continue
    b=$(basename "$f" .hero)
    out="$O/$b.repair.out"
    "$H" check "$f" > "$out" 2>&1
    e=$?
    echo "check exit $e" >> "$out"
    if [ "$e" = 0 ]; then
        "$H" run "$f" >> "$out" 2>&1
        echo "run exit $?" >> "$out"
    fi
done
"$H" check --permissive "$P/x1.hero" > "$O/x1.permissive.out" 2>&1
echo "check --permissive exit $?" >> "$O/x1.permissive.out"
"$H" build --permissive "$P/x1.hero" -o "$O/x1.bin" >> "$O/x1.permissive.out" 2>&1
e=$?
echo "build --permissive exit $e" >> "$O/x1.permissive.out"
if [ "$e" = 0 ]; then "$O/x1.bin" >> "$O/x1.permissive.out" 2>&1; echo "run exit $?" >> "$O/x1.permissive.out"; fi
"$H" build "$P/q2b.hero" --emit-c > "$O/q2b.c" 2>&1
echo "q2b --emit-c exit $?"
