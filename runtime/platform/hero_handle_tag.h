/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION), as
 * `heroes_runtime.h`.
 *
 * platform/hero_handle_tag.h — the one list of the tags that keep the handles
 * of the platform layer's tables apart (M-rebel-library, 2026-11-16).
 *
 * A HEADER OF THE MEMBERS ALONE: no Heroes group names it, and no header a
 * group names includes it. Each member that issues handles includes it after
 * its own header and puts its table's tag in the bits below.
 *
 * WHY A TAG. A member's handle is no address but a slot and a life in one
 * pointer's bits, `(life << INDEX_BITS) | (index + 1)`, and each member counts
 * its slots and lives alone. So, until this header, slot 3 at its first life
 * was `0x20004` in the table of sockets and in the table of streams alike. The
 * runtime's set of ended handles (`parts/alloc.c`, route T) remembers an
 * address a `consumes` or `transfers` call ended, with no type beside it, until
 * C hands that address out again; a socket handed into a stream by `transfers`
 * left `0x20004` remembered, and a stream that held `0x20004` from before was
 * then refused at its next read as a copy kept past its end (measured
 * 2026-11-16: two accepted connections closed, then a read of the first
 * client's stream, `panic: a C handle reached the argument s of
 * hero_stream_read at an address ... ended`, exit 134; the reproducer is
 * `hero_stream_net.hero`'s test of two cycles).
 *
 * THE BITS. 62 is `HERO_NET_NUMBERED`'s, a socket's number taken back. 58 to 61
 * hold the tag, so no handle of one table is ever a handle of another, at any
 * slot and any life. A member's life therefore stays below bit 58:
 * `HERO_HANDLE_LIFE_MOST(index_bits)` is the last life a slot of a member with
 * that many index bits is given. With 17 index bits it is 2^41 - 1: a slot
 * given a new life a hundred thousand times a second, every second, reaches
 * it after 254 days, and a slot at it is never issued again, so the bound
 * costs a slot and never a wrong handle.
 *
 * NO TAG IS 0, so a handle of no tag is no member's. A new member that issues
 * handles takes the next free number here, and nowhere else.
 *
 * NO CONDITIONAL BUT THE GUARD. */

#ifndef HERO_HANDLE_TAG_H
#define HERO_HANDLE_TAG_H

#include <stdint.h>

#define HERO_HANDLE_TAG_SHIFT 58
#define HERO_HANDLE_TAG_BITS 4
#define HERO_HANDLE_TAG_MASK (((UINT64_C(1) << HERO_HANDLE_TAG_BITS) - 1) << HERO_HANDLE_TAG_SHIFT)

/* The last life a slot is given by a member whose index takes `index_bits`
 * bits: the life ends below the tag. */
#define HERO_HANDLE_LIFE_MOST(index_bits) ((UINT64_C(1) << (HERO_HANDLE_TAG_SHIFT - (index_bits))) - 1)

/* A tag in its place in a handle's bits. */
#define HERO_HANDLE_TAG(tag) ((uint64_t)(tag) << HERO_HANDLE_TAG_SHIFT)

/* One number per table that issues handles; 0 is no table's. */
#define HERO_HANDLE_TAG_STREAM 1
#define HERO_HANDLE_TAG_NET 2
#define HERO_HANDLE_TAG_POLL 3
#define HERO_HANDLE_TAG_PROC 4
#define HERO_HANDLE_TAG_TLS 5
#define HERO_HANDLE_TAG_TERM 6
/* `hero_ipc.c`'s four kinds of handle (2026-11-17), one tag each, so a
 * listener's handle is never memory's at any slot and life. */
#define HERO_HANDLE_TAG_IPC_LISTENER 7
#define HERO_HANDLE_TAG_IPC_MEMORY 8
#define HERO_HANDLE_TAG_IPC_SEMAPHORE 9
#define HERO_HANDLE_TAG_IPC_LOCK 10
/* `hero_crypto.c`'s keys (2026-11-17). */
#define HERO_HANDLE_TAG_CRYPTO 11
/* `hero_disk.c`'s files mapped in memory (2026-11-17, the small additions:
 * until then that member issued no handle). */
#define HERO_HANDLE_TAG_DISK_MAP 12
/* 13 to 15 are free; the next member takes 13. */

#endif
