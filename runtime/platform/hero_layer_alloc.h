/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION), as
 * `heroes_runtime.h`.
 *
 * platform/hero_layer_alloc.h — the platform layer's memory, which
 * `parts/alloc.c` gives (M-rebel-library, 2026-11-16).
 *
 * A HEADER OF THE MEMBERS ALONE, as `hero_handle_tag.h` is: no Heroes group
 * names it, and no header a group names includes it. A member that keeps
 * state of its own past a call (a TLS session's records, a library's
 * objects) includes it after its own header. `runtime.c` names it before
 * `parts/alloc.c`, which defines its four functions, so the runtime's key
 * covers it and clang checks the definitions against it.
 *
 * WHY HERE AND NOT IN `heroes_runtime.h`: that header is the contract every
 * emitted unit includes, and its shape moving moves `HERO_RUNTIME_ABI`, which
 * every unit asserts; the layer's memory is no emitted unit's business.
 *
 * WHY THROUGH `parts/alloc.c` AT ALL: it is the single allocation point
 * (design.md §4.20; Part 7.13's per-thread heaps need one place to change),
 * and nothing under `runtime/` but that file calls the C library's
 * allocation family (the net's `runtime` suite, rule 1).
 *
 * WHAT THESE ARE, AND ARE NOT:
 *
 *   - PROCESS-WIDE, never a thread's heap: a block may be given back on
 *     another thread than the one that took it (a TLS stream is closed by the
 *     thread that leaves it last, and Network.framework releases a
 *     connection's state on its own queue).
 *   - A VALUE ON FAILURE: NULL where the system has no memory, never an
 *     abort, so a member answers its own `LIMIT` kind.
 *   - WEIGHED BY NO COUNTER OF THE EXIT GATE: Network.framework gives a
 *     connection's state back on its queue after the close has returned, so
 *     a count read at exit would accuse a program that closed everything.
 *     A leak here is judged by `leaks --atExit` on Darwin and LeakSanitizer
 *     on Linux (`.claude/rules/platforms.md`).
 *
 * NO CONDITIONAL BUT THE GUARD. */

#ifndef HERO_LAYER_ALLOC_H
#define HERO_LAYER_ALLOC_H

#include <stddef.h>

/* A block of `size` bytes, its contents undefined, or NULL. A size of 0 is
 * taken as 1, so a block is never NULL for being empty. */
void *hero_layer_alloc(size_t size);

/* A block of `count` elements of `size` bytes, every byte zero, or NULL,
 * also where `count` times `size` does not fit. */
void *hero_layer_alloc_zero(size_t count, size_t size);

/* `block` grown or shrunk to `size` bytes, its first bytes kept, perhaps at
 * another address; NULL where the system has no memory, and then `block` is
 * as it was and still the caller's. A NULL `block` is a new one. */
void *hero_layer_grow(void *block, size_t size);

/* Gives `block` back; NULL gives back nothing. Also the release of a block a
 * system function handed out with the C library's allocator and says the
 * caller frees with `free` (Apple's
 * `sec_protocol_metadata_copy_negotiated_protocol`). */
void hero_layer_free(void *block);

#endif
