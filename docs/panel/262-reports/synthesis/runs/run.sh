#!/bin/sh
# Panel 262, the synthesis: the seconds-long falsifiers of question 1 and 3, on main's tip
# (77eb38534, its runtime in ./runtime), with the seed's compiler built from that tip's seed.
# From synth/main: sh ../run.sh
set -u
S=<repository>/.claude/worktrees/panel262-scratch/synth
P=$S/progs
O=$S/out
export TMPDIR=$S/tmp
mkdir -p "$O"
C=./heroes
norm() { sed -E 's/==[0-9]+==/==PID==/; s/0x[0-9a-f]+/0xADDR/g; s/\([0-9]+\)$/(N)/' "$1" | shasum | cut -c1-12; }
cls() {
    f=$1
    if grep -q 'stack frames the thread had already left' "$f"; then echo REENTERED
    elif grep -q 'had not been seen to end' "$f"; then echo STANDING
    elif grep -q 'had switched to another context' "$f"; then echo SWITCHED
    elif grep -q 'onto a stack a context switch had entered' "$f"; then echo ONSTACK
    elif grep -q 'called from inside a signal handler' "$f"; then echo CALLED
    elif grep -q 'jumped out of the code it interrupted' "$f"; then echo JUMP-REFUSED
    elif grep -q 'used this runtime' "$f"; then echo GUARD-D
    elif grep -q 'ERROR: AddressSanitizer' "$f"; then echo ASAN
    elif [ "$(tail -1 "$f")" = "exit 0" ]; then echo RUNS
    else echo "OTHER($(tail -1 "$f"))"; fi
}
build() { # name src flag
    b="$O/bin-$1"
    [ -x "$b" ] && return 0
    "$C" build "$2" $3 -o "$b" > "$O/build-$1.txt" 2>&1 || { echo "$1: BUILD FAILED: $(tail -3 "$O/build-$1.txt" | tr '\n' '|')"; return 1; }
}
runn() { # tag binary env...
    tag=$1; b=$2; shift 2
    for i in 1 2 3; do
        o="$O/out-$tag-$i.txt"
        env "$@" timeout 60 "$b" > "$o" 2>&1
        echo "exit $?" >> "$o"
    done
    d=$(for i in 1 2 3; do norm "$O/out-$tag-$i.txt"; done | sort -u | wc -l | tr -d ' ')
    echo "$tag: $(cls "$O/out-$tag-1.txt"), $(tail -1 "$O/out-$tag-1.txt"), $d distinct of 3"
}
date
build wide-plain "$P/wide.hero" "" && { runn wide7-plain "$O/bin-wide-plain" WIDE_N=7; runn wide8-plain "$O/bin-wide-plain" WIDE_N=8; }
build ck-plain "$P/ck.hero" "" && {
    runn ck-on0-how0-to0 "$O/bin-ck-plain" CK_ON=0 CK_HOW=0 CK_TO=0
    runn ck-on0-how1-to0 "$O/bin-ck-plain" CK_ON=0 CK_HOW=1 CK_TO=0
    runn ck-on0-how0-to1 "$O/bin-ck-plain" CK_ON=0 CK_HOW=0 CK_TO=1
    runn ck-on1-how0-to0 "$O/bin-ck-plain" CK_ON=1 CK_HOW=0 CK_TO=0
    runn ck-on1-how1-to1 "$O/bin-ck-plain" CK_ON=1 CK_HOW=1 CK_TO=1
}
for g in fixedbugs-2702-a-handler-on-the-alternate-stack-left-by-setcontext-to-its-coroutine-is-refused \
         fixedbugs-2707-a-setcontext-out-of-a-handler-on-an-alternate-stack-into-a-coroutine-mapped-after-it-is-refused \
         fixedbugs-2707-a-setcontext-out-of-a-handler-on-an-alternate-stack-into-a-coroutine-mapped-before-it-is-refused \
         fixedbugs-2702-a-handler-left-by-setcontext-to-its-coroutine-runs; do
    build "$g" "tests/golden/run/$g.hero" "" && { runn "$g" "$O/bin-$g"; echo "  .expected's class: $(f=tests/golden/run/$g.expected; cp "$f" "$O/exp-$g.txt"; echo "exit 0" >> "$O/exp-$g.txt"; cls "$O/exp-$g.txt")"; }
done
date
build fmtop-san "$P/fm_top.hero" --sanitize && runn fmtop-san "$O/bin-fmtop-san"
build fmzero-san "$P/fm_zero.hero" --sanitize && runn fmzero-san "$O/bin-fmzero-san"
date
build wdr-plain "$P/wdr.hero" "" && {
    i=0; : > "$O/class-wdr-plain.txt"
    while [ $i -lt 34 ]; do
        us=$(echo "300 700 1100 1500 2000 2600 3300 4100 5000 6200 7600 9000 11000 13500 16000 20000 25000" | tr ' ' '\n' | sed -n "$((i % 17 + 1))p")
        o="$O/out-wdr-plain-$i.txt"
        WD_US=$us timeout 60 "$O/bin-wdr-plain" > "$o" 2>&1
        echo "exit $?" >> "$o"
        echo "$us $(cls "$o")" >> "$O/class-wdr-plain.txt"
        i=$((i + 1))
    done
    echo "wdr plain, 34 runs: $(cut -d' ' -f2 "$O/class-wdr-plain.txt" | sort | uniq -c | tr '\n' ';')"
}
date
