/* Spike 3 — the FFI premise, proven in three lines.
 *
 * design.md §4.19's acceptance ladder, step 2: call libm. Under the old QBE
 * backend this meant transcribing signatures by hand with nothing checking
 * them against reality. With C emission it is an #include — and clang
 * VERIFIES the call against the real header, which is the project's thesis
 * ("plausible mistakes become compile errors") applied to the FFI.
 *
 * The Heroes surface for this (M-ffi-ladder, importc-style):
 *
 *     extern function sqrt(x: f64) -> f64        # header "math.h"
 *
 * TRY THIS: change sqrt(2.0) to sqrt("2") — clang rejects it. That error
 * was a runtime disaster under QBE.
 *
 * Build:  clang -std=c11 -Wall -Iruntime \
 *             tools/spike/03-ffi.c build/runtime.o -o build/spike03
 * Run:    build/spike03        → 1 (sqrt(2) is ~1.41421, truncated: 1)
 */

#include "heroes_runtime.h"

#include <math.h> /* the entire FFI mechanism */

int main(void) {
    double r = sqrt(2.0);
    /* No implicit conversions in Heroes (§4.3): the truncation to int is an
     * explicit to_i64(x) at the surface, so the C spells the cast out too. */
    hero_print_int((int64_t)r);
    hero_print_end();
    return 0;
}
