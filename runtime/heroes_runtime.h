/* heroes_runtime.h — the C runtime interface (design.md §4.20).
 *
 * Generated C #includes this header, so clang type-checks every runtime call
 * against it. Keep declarations exact: this file IS the contract.
 *
 * M5a scope: panic (which flushes), hero_unreachable, and print's segment
 * printers for int and bool. The rest of §4.20 (str, array, map, refcount, COW,
 * join) arrives in M5b/M5c with the representation decided by
 * tools/spike/04-variant.c.
 */

#ifndef HEROES_RUNTIME_H
#define HEROES_RUNTIME_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* -- the stamp -------------------------------------------------------------
 * Every generated translation unit `_Static_assert`s this, because the runtime
 * is found by *searching* (design.md §3.1: $HEROES_RUNTIME, then runtime/ under
 * the working directory, then under an ancestor of the executable) and a
 * directory called `runtime/` is one of the commonest things in a source tree.
 * Measured (panel 020): a decoy runtime/ in the working directory compiled with
 * ZERO warnings under -Weverything and printed 0x1e where 30 was expected. With
 * the stamp the same C is `error: use of undeclared identifier
 * 'HERO_RUNTIME_ABI'`. Bump it whenever a declaration below changes shape. */
#define HERO_RUNTIME_ABI 1

/* -- failure ---------------------------------------------------------------
 * design.md §4.14: integer overflow aborts, division by zero aborts,
 * out-of-bounds aborts. Loud failure, never silence. */

_Noreturn void hero_panic(const char *msg);
_Noreturn void hero_panic_overflow(void);

/* Emitted at points the type system proves unreachable (an exhaustive match's
 * fallthrough, a function end after total returns). Reaching it is a compiler
 * bug, and UB is not an acceptable way to find out. */
_Noreturn void hero_unreachable(void);

/* -- printing --------------------------------------------------------------
 * `print` is a compiler form, not a function value (panel 006): the runtime
 * exposes one monomorphic printer per type and the emitter composes them.
 *
 * The contract, stated in the spec since M5a: NO separator between values, and
 * exactly ONE trailing newline. So a segment printer writes its own value and
 * nothing else — hero_print_int used to own the newline and no longer does —
 * while hero_print_end writes the line ending. `print()` with no arguments is
 * therefore one empty line. */

void hero_print_int(int64_t v);
void hero_print_bool(bool v);
void hero_print_end(void);

#endif /* HEROES_RUNTIME_H */
