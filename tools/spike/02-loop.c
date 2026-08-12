/* Spike 2 — basic blocks and jumps, by hand.
 *
 * ============================================================================
 * This spike carried the author's first uncued prediction: draw the
 * control-flow graph before reading the code, then compare. Done at M-day-zero;
 * the divergence and its lesson live in docs/journal/000-setup.md §3 and
 * docs/glossary/000-basic-block.md.
 * ============================================================================
 *
 * Heroes source:
 *
 *     function main()
 *         i: int @ 0
 *         total: int @ 0
 *         for i < 5
 *             total @ total + i
 *             i @ i + 1
 *         print(total)                # → 10
 *
 * The lowering (M-ir-lowering) reduces all control flow to labels and conditional jumps:
 * one goto+label per basic block, no while/for reconstruction. This file is
 * that exact shape, hand-written.
 *
 * PROOF THAT goto COSTS NOTHING:
 *     clang -S -O2 -Iruntime tools/spike/02-loop.c -o -
 * The optimiser reconstructs the loop by itself (look for the compact
 * compare-and-branch cycle in the assembly; at -O2 it constant-folds the
 * whole loop to 10 — try -O0 to see the real branches).
 *
 * Build:  clang -std=c11 -Wall -Werror=return-type -Iruntime \
 *             tools/spike/02-loop.c build/runtime.o -o build/spike02
 * Run:    build/spike02        → 10
 */

#include "heroes_runtime.h"

int main(void) {
    /* All locals hoisted to the prologue: goto may not jump over
     * declarations in C, so the emitter declares everything up front. */
    int64_t i;
    int64_t total;

    /* FINDING for the emitter: the entry label is never a jump target, and
     * clang warns on unused labels. So the emitter always enters through an
     * explicit goto — uniform shape, no special case for block zero. */
    goto bb0;

#line 2 "spike02.hero"
bb0: /* entry */
    i = 0;
    total = 0;
    goto bb1;

bb1: /* loop condition: for i < 5 */
    if (i < 5) {
        goto bb2;
    } else {
        goto bb3;
    }

bb2: /* loop body */
    if (__builtin_add_overflow(total, i, &total)) hero_panic_overflow();
    if (__builtin_add_overflow(i, (int64_t)1, &i)) hero_panic_overflow();
    goto bb1;

bb3: /* after the loop */
    hero_print_int(total);
    hero_print_end();
    return 0;
}
