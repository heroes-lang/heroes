#!/bin/sh
# Panel 262's probes: two programs that install no signal handler (panel 259's
# `escape_carved`, `escape_heap`, the compiler-engineer's, and `mj_carved`, the
# critic's, copied from `docs/panel/259-reports/evidence.tar.xz`), each built
# plain and `--sanitize` and run three times, and the `run` goldens the shared
# brief cites, through the `run` form narrowed by its third word.
# From a copy's root: `sh docs/panel/262-briefs/ground/probes.sh <compiler> <out>`.
set -u
C=$1
OUT=$2
P=docs/panel/262-briefs/ground/probe
mkdir -p "$OUT"
for n in escape_carved escape_heap mj_carved; do
    for mode in plain sanitize; do
        flag=
        [ $mode = sanitize ] && flag=--sanitize
        if ! "$C" build "$P/$n.hero" $flag -o "$OUT/bin-$n-$mode" > "$OUT/build-$n-$mode.txt" 2>&1; then
            echo "$n $mode: build failed"
            continue
        fi
        for i in 1 2 3; do
            timeout 60 "$OUT/bin-$n-$mode" > "$OUT/out-$n-$mode-$i.txt" 2>&1
            echo "exit $?" >> "$OUT/out-$n-$mode-$i.txt"
        done
        echo "$n $mode: $(tail -1 "$OUT/out-$n-$mode-1.txt"), $(head -c 120 "$OUT/out-$n-$mode-1.txt" | head -1)"
    done
done
for s in fixedbugs-2707 fixedbugs-2706 fixedbugs-2848 fixedbugs-2705 \
    2494-a-heroes-function-a-c-handler-calls-on-a-thread \
    2702-a-handler-left-by-setcontext \
    2702-a-handler-on-the-alternate-stack-left-by-setcontext \
    2702-a-handler-swapped-away-from-and-never-resumed \
    2522-a-callback-after-a-siglongjmp-out-of-a-handler-on-its-own \
    2522-a-correct-inline-jump \
    2494-a-heroes-function-a-c-handler-calls-is-refused \
    2702-a-handler-jumps-into-a-coroutine-on-the-own-stack; do
    echo "### $s"
    "$C" run tests/harness/main.hero -- "$C" run "$s" 2>&1 | grep -E 'passed|failed|FAIL'
done
