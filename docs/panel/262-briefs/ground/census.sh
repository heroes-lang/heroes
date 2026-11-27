#!/bin/sh
# Panel 262's census of the goldens that pin the handler sentence's words, and of
# the `run` goldens named `coroutine`, read by what their headers do. Run from a
# copy's root: `sh docs/panel/262-briefs/ground/census.sh`. Reads files only.
# The clerk's at `aded91388`; amended by the repair (2026-11-26, after the critic's
# first reading): the claims' files split by form, so the `check` case whose
# *from inside* is no refusal is named (the critic's C5); the families 2850,
# 2851 and 2855 added, and every family counted as `.hero` files AND as files, both named
# (the critic's § 1 table); and the no-handler reading follows every `#include
# "..."` a named header makes and counts `hero_signal_watch` and
# `hero_net_catch_stop` as installing (the critic's `census2.sh`, its rule).
set -u
cd tests/golden || exit 2

echo "## .expected files holding each claim (grep -rl over tests/golden)"
printf '%-58s %s\n' "seen to end" "$(grep -rl 'seen to end' . --include='*.expected' | wc -l | tr -d ' ')"
printf '%-58s %s\n' "from inside" "$(grep -rl 'from inside' . --include='*.expected' | wc -l | tr -d ' ')"
printf '%-58s %s\n' "either" "$(grep -rlE 'seen to end|from inside' . --include='*.expected' | wc -l | tr -d ' ')"
printf '%-58s %s\n' "either, under run/" "$(grep -rlE 'seen to end|from inside' run --include='*.expected' | wc -l | tr -d ' ')"
echo "either, outside run/ (each named):"
grep -rlE 'seen to end|from inside' . --include='*.expected' | grep -v '^\./run/' | sed 's/^/  /'

echo "## .expected files holding each arm's own words (grep -rlF)"
for w in \
    'ran as a signal handler' \
    'this runtime saw begin had not been seen' \
    'had switched to another context' \
    'jumped into stack frames the thread had already left' \
    'onto a stack a context switch had entered' \
    'which this runtime holds may still be a coroutine' \
    'holds no record of as it now stands' \
    'was called from inside a signal handler: C called it' \
    'ran on a thread this program did not start' \
    'jumped out of the code it interrupted' \
    "a signal handler used this runtime's allocator"; do
    printf '%-58s %s\n' "$w" "$(grep -rlF "$w" . --include='*.expected' | wc -l | tr -d ' ')"
done

echo "## run goldens per family: .hero files, then every file (ls tests/golden/run | grep -c)"
for p in 2494 2522 2679 2702 2705 2706 2707 2745 2848 2850 2851 2855; do
    printf '%-10s %4s .hero  %4s files\n' "$p" "$(ls run | grep -c "fixedbugs-$p.*\.hero$")" "$(ls run | grep -c "fixedbugs-$p")"
done

echo "## run goldens whose name holds 'coroutine', by whether a header they name installs a handler"
echo "## (every header followed through #include \"...\"; hero_signal_watch or hero_net_catch_stop in the .hero counts)"
cd run || exit 2
deps() { _d=$(dirname "$1"); echo "$1"; for _i in $(grep -oE '#include "[^"]+"' "$1" 2>/dev/null | sed 's/#include "//; s/"$//'); do [ -f "$_d/$_i" ] && deps "$_d/$_i"; done; }
INST='sigaction|(^|[^_a-z])signal *\(|sigset *\(|bsd_signal|sigvec|raise *\(|pthread_kill|(^|[^_a-z])kill *\('
nh_run=0; nh_panic=0; h_run=0; h_panic=0
for f in $(ls | grep coroutine | grep '\.hero$'); do
    b=${f%.hero}
    handler=0
    for hh in $(grep -oE 'extern "[^"]+"' "$f" | sed 's/extern "//; s/"$//'); do
        [ -f "$hh" ] || continue
        for x in $(deps "$hh"); do grep -qE "$INST" "$x" && handler=1; done
    done
    grep -qE 'hero_signal_watch|hero_net_catch_stop' "$f" && handler=1
    panic=0
    grep -q 'panic' "$b.expected" && panic=1
    if [ $handler = 0 ]; then
        if [ $panic = 1 ]; then nh_panic=$((nh_panic + 1)); echo "  no handler, aborts: $b"; else nh_run=$((nh_run + 1)); fi
    else
        if [ $panic = 1 ]; then h_panic=$((h_panic + 1)); else h_run=$((h_run + 1)); fi
    fi
done
echo "no handler: run $nh_run, abort $nh_panic; a handler: run $h_run, abort $h_panic"

echo "## every run golden holding a claim: does a header it names (deep) install a handler?"
none=0
for e in $(grep -rlE 'seen to end|from inside' . --include='*.expected'); do
    h=${e%.expected}.hero
    [ -f "$h" ] || { echo "  no .hero beside $e"; continue; }
    inst=0
    for hh in $(grep -oE 'extern "[^"]+"' "$h" | sed 's/extern "//; s/"$//'); do
        [ -f "$hh" ] || continue
        for x in $(deps "$hh"); do grep -qE "$INST" "$x" && inst=1; done
    done
    [ $inst = 0 ] && { none=$((none + 1)); echo "  installs none: $e"; }
done
echo "claim goldens under run/ installing none: $none"
