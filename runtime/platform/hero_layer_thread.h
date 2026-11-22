/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION), as
 * `heroes_runtime.h`.
 *
 * platform/hero_layer_thread.h — the runtime's own number for the calling
 * thread, as the platform layer's members read it.
 *
 * A HEADER OF THE MEMBERS ALONE, as `hero_layer_alloc.h` is: no Heroes group
 * names it, and no header a group names includes it. `runtime.c` names it
 * before `parts/alloc.c`, which defines its one function, so the runtime's
 * key covers it and clang checks the definition against it.
 *
 * WHY A NUMBER AND NOT THE SYSTEM'S IDENTITY: a `pthread_t` names a thread
 * only while it runs, and Darwin gives a thread started after another ended
 * the same one at once; Windows reuses a thread's id too. A member that
 * remembers which thread did something (a lock's holder, the UI's owner)
 * would take a later thread for that one (defect 2673). The number is the
 * one the runtime keeps for its own handles and mailboxes (panel 254's R3),
 * so the layer and the runtime never count threads apart.
 *
 * WHY HERE AND NOT IN `heroes_runtime.h`: that header is the contract every
 * emitted unit includes, and its shape moving moves `HERO_RUNTIME_ABI`.
 *
 * NO CONDITIONAL BUT THE GUARD. */

#ifndef HERO_LAYER_THREAD_H
#define HERO_LAYER_THREAD_H

#include <stdint.h>

/* The calling thread's number: 1 for the first thread that asks, one more
 * for each thread after, never given to another thread of the process.
 * Never 0. Not to be asked inside a thread-local key's destructor, where
 * Darwin reads the thread's copy as 0 and a new number would be given. */
uint64_t hero_layer_thread(void);

#endif
