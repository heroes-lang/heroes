/* Spike 1 — the hand-written target for the compiler's first program (M-scalars-run).
 *
 * Heroes source this C corresponds to (examples/gallery/00-first.hero):
 *
 *     function main()
 *         print((2 + 3) * 4)
 *
 * This file defines the SHAPE of what the emitter must produce, before the
 * emitter exists. In M-scalars-run we diff the generated C against this by eye.
 *
 * Shape rules (CLAUDE.md § "Generated-C rules"):
 *   - #line points every step at the .hero source, so clang errors and lldb
 *     land on the author's file, never on generated text.
 *   - Arithmetic aborts on overflow via __builtin_*_overflow — never C's UB.
 *   - All calls go through heroes_runtime.h, so clang type-checks them.
 *
 * TRY THIS (the property that justifies the whole backend):
 *   change hero_print_int(t1) to hero_print_int(&t1) and recompile —
 *   clang's error will point at examples/gallery/00-first.hero, not at this file.
 *   NOTE (panel 020): the LINE it names is not 2. `#line N` anchors the *next*
 *   line and C auto-increments, so a Heroes line lowering to K C lines drifts by
 *   K-1 — measured here as :6, which is blank. The real emitter re-anchors per C
 *   line for exactly this reason; this hand-written file keeps the drift on the
 *   record instead of hiding it.
 *
 * Build:  clang -std=c11 -Wall -Werror=return-type -Iruntime \
 *             tools/spike/01-first.c build/runtime.o -o build/spike01
 * Run:    build/spike01        → 20
 *         (checked against tools/spike/01-first.expected by a test)
 */

#include "heroes_runtime.h"

int main(void) {
#line 2 "examples/gallery/00-first.hero"
    int64_t t0;
    int64_t t1;
    if (__builtin_add_overflow((int64_t)2, (int64_t)3, &t0)) hero_panic_overflow();
    if (__builtin_mul_overflow(t0, (int64_t)4, &t1)) hero_panic_overflow();
    hero_print_int(t1);
    hero_print_end();
    return 0;
}
