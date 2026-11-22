#!/bin/bash
# Panel 256, Q2's probes: every file checked, and each one check accepts built and run once, bounded.
# usage: bash run.sh <compiler>   (from a copy of this folder; writes build/ beside it)
H="$1"
for p in p1_defect p2_relay_only p3_spawn_generic p3b_spawn_generic_copy_kept c3_spawn_nongeneric p4_ptr_generic p5_ptr_written_generic \
         p5c_inferred_nongeneric_send p6b_unit_generic c6_unit_nongeneric p7_spawn_ptr_generic c7_spawn_ptr_nongeneric \
         p8_answer_ptr_generic p9_nongeneric_controls e1_partial_eq_generic e2_partial_key_generic \
         e3_partial_eq_nongeneric e4_partial_key_nongeneric; do
    out=$("$H" check "$p.hero" --brief 2>&1); c=$?
    printf '%-32s check %s  %s\n' "$p" "$c" "$(printf '%s' "$out" | head -1 | cut -c1-150)"
    if [ "$c" = 0 ]; then
        b=$("$H" build "$p.hero" -o "build/$p" 2>&1); bc=$?
        printf '%-32s build %s  %s\n' "" "$bc" "$(printf '%s' "$b" | grep -v '^wrote' | head -1 | cut -c1-150)"
        if [ "$bc" = 0 ]; then
            timeout -s KILL 20 "build/$p" > "build/$p.out" 2>&1; rc=$?
            r=$(head -c 600 "build/$p.out")
            printf '%-32s run   %s  %s\n' "" "$rc" "$(printf '%s' "$r" | tr '\n' ' ' | cut -c1-150)"
        fi
    fi
done
