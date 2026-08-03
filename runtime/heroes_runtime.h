/* heroes_runtime.h — the C runtime interface (design.md §4.20).
 *
 * Generated C #includes this header, so clang type-checks every runtime call
 * against it. Keep declarations exact: this file IS the contract.
 *
 * M0 scope: panic + integer printing, enough for the spikes and M5a.
 * The rest of §4.20 (str, array, map, refcount, COW, join) arrives in M5b/M5c
 * with the representation decided by tools/spike/04-variant.c.
 */

#ifndef HEROES_RUNTIME_H
#define HEROES_RUNTIME_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

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
 * print's variadic mixed-type contract is an open design question (panel 006);
 * until it is decided the runtime exposes monomorphic entry points and the
 * emitter composes them. */

void hero_print_int(int64_t v);

#endif /* HEROES_RUNTIME_H */
