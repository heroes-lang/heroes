/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION), as
 * `heroes_runtime.h`.
 *
 * hero_keyring.h — the platform layer's keyring: a secret kept by the
 * system's own store under a service and an account, stored, read, replaced
 * and removed, and the accounts a service holds listed (M-rebel-library,
 * 2026-11-18; spec § 13).
 *
 * A HEADER OF THE PLATFORM LAYER, AND NOT OF THE RUNTIME, as `hero_net.h` is:
 * its functions are defined in `platform/hero_keyring.c`, a member of the
 * archive on every program's link line, which the linker takes only into a
 * program that calls one; a group binds them by naming this header,
 * `extern "hero_keyring.h"`, with no `link` word and no flag.
 *
 * THE STORES. On macOS the login keychain, as generic passwords whose service
 * and account are the item's own attributes, through the Security framework's
 * item calls. On Windows the Credential Manager, as generic credentials of
 * the user's logon, persisted on this machine. On Linux the Secret Service
 * (GNOME Keyring, KWallet's bridge, KeePassXC), through libsecret, which the
 * member opens at its first call: where the library is not installed, no
 * session bus answers, or no service owns the bus's name (a container, a
 * server with no desktop), every call answers -1 with
 * `HERO_KEYRING_UNSUPPORTED`, and `hero_keyring_last_message` says which.
 *
 * WHAT A NAME IS. A service and an account are UTF-8 text, at most
 * `HERO_KEYRING_NAME_MOST` bytes each and never a zero byte, so they cross
 * as `const char *`; the service is never empty, the account may be. The
 * same two names reach the same secret on one machine, and two different
 * pairs never reach one secret: on Windows a pair is written into one target
 * name by an encoding that keeps every byte apart, its letters included,
 * since the Credential Manager compares target names without their case
 * (`hero_keyring.c` says how). On macOS and Linux the pair is the item's two
 * attributes, `service` and `account` on macOS, `service` and `username` on
 * Linux, so the items of other programs that use those attributes (Python's
 * keyring, Go's go-keyring) are the same items; on Windows the items are
 * this layer's own.
 *
 * WHAT A SECRET IS: any bytes, at most `HERO_KEYRING_SECRET_MOST`, the
 * Credential Manager's own bound, held on every platform so that a program
 * that stores a secret on one stores it on all. A secret crosses by the fill
 * form and no message of this layer ever holds one.
 *
 * EVERY FAILURE IS A VALUE, NEVER A FAULT: a call answering a count -1, and
 * `hero_keyring_last_error()` then names the kind, `hero_keyring_last_os_error()`
 * the platform's number, `hero_keyring_last_message` the system's own words.
 * No call hands out a handle: each is whole in itself. The system's stores
 * are safe to call from several threads, and on Linux the member's calls are
 * one at a time under a lock.
 *
 * NO CONDITIONAL BUT THE GUARD (held by the net's `runtime` suite): every
 * platform's arm of the member is compiled against these prototypes.
 */

#ifndef HERO_KEYRING_H
#define HERO_KEYRING_H

#include <stdint.h>

/* The kinds of failure, the same numbers on every platform; the platform's
 * own number for the last failure is `hero_keyring_last_os_error()`. */
#define HERO_KEYRING_OK 0
/* An argument the layer refuses: a name NULL, empty where it may not be,
 * longer than `HERO_KEYRING_NAME_MOST` or no UTF-8; a secret longer than
 * `HERO_KEYRING_SECRET_MOST`; a count with no bytes behind it. */
#define HERO_KEYRING_INVALID 1
/* No secret stands under that service and account. */
#define HERO_KEYRING_NOT_FOUND 2
/* The store refuses this to the program: the keychain or the collection is
 * locked and may not be opened here, the person refused, or the item belongs
 * to another program that has not shared it. */
#define HERO_KEYRING_DENIED 3
/* The system has no memory, or no room in its store, for the call. */
#define HERO_KEYRING_LIMIT 4
/* This machine has no keyring this layer reaches (THE STORES, above). */
#define HERO_KEYRING_UNSUPPORTED 5
/* The store refused for a reason none of the above names; the message
 * carries its words. */
#define HERO_KEYRING_OTHER 6

/* The most bytes a service or an account holds, its zero byte not counted. */
#define HERO_KEYRING_NAME_MOST 1024
/* The most bytes a secret holds, on every platform: the Credential Manager's
 * `CRED_MAX_CREDENTIAL_BLOB_SIZE`. */
#define HERO_KEYRING_SECRET_MOST 2560

/* The name of the store this machine offers, in English, written into
 * `buffer` as UTF-8 with no zero byte, and answers its length: "macOS
 * Keychain", "Windows Credential Manager" or "Secret Service". The store is
 * asked whether it answers (a search for a name nobody uses), so a machine
 * without one answers -1 here as every other call would: `UNSUPPORTED`,
 * `DENIED`. Where the name is longer than `count`, nothing is written and the
 * length is answered all the same. */
int64_t hero_keyring_system(void *buffer, uint64_t count);

/* Stores the `count` bytes of `secret` under `service` and `account`,
 * replacing the secret that stood there, and answers 0. -1: `INVALID`,
 * `DENIED`, `LIMIT`, `UNSUPPORTED`, `OTHER`. */
int64_t hero_keyring_store(const char *service, const char *account, const void *secret, uint64_t count);

/* Reads the secret under `service` and `account` into `buffer` and answers
 * its length in bytes. Where it is longer than `count`, nothing is written
 * and its length is answered all the same, so the caller asks again with
 * room. -1: `NOT_FOUND`, `INVALID`, `DENIED`, `UNSUPPORTED`, `OTHER`. */
int64_t hero_keyring_read(const char *service, const char *account, void *buffer, uint64_t count);

/* Removes the secret under `service` and `account` and answers 0. -1:
 * `NOT_FOUND`, `INVALID`, `DENIED`, `UNSUPPORTED`, `OTHER`. */
int64_t hero_keyring_remove(const char *service, const char *account);

/* Removes every secret under `service`, whatever its account, and answers
 * 0, also where none stood. -1: `INVALID`, `DENIED`, `UNSUPPORTED`, `OTHER`. */
int64_t hero_keyring_remove_all(const char *service);

/* Writes the accounts that hold a secret under `service` into `buffer`, each
 * one's UTF-8 followed by one zero byte, in the order the store gives them,
 * and answers how many bytes that takes; 0 where none does. Where it takes
 * more than `count`, nothing is written and the bytes are answered all the
 * same. An account the store holds that is no text this layer can name (a
 * zero byte in it, no UTF-8) is left out. -1: `INVALID`, `DENIED`,
 * `UNSUPPORTED`, `OTHER`. */
int64_t hero_keyring_accounts(const char *service, void *buffer, uint64_t count);

/* The kind of the last call's failure on this thread, `HERO_KEYRING_OK` after
 * a call that did not fail, and the platform's own number for it (0 where the
 * layer itself refused). */
int64_t hero_keyring_last_error(void);
int64_t hero_keyring_last_os_error(void);

/* The words of the last call's failure on this thread, the system's where it
 * gave some, written into `buffer` as UTF-8 with no zero byte, cut where a
 * character ends to fit `count`; answers the bytes written, 0 after a call
 * that did not fail. They never hold a secret. -1 with `INVALID` where
 * `buffer` is NULL or `count` is 0. */
int64_t hero_keyring_last_message(void *buffer, uint64_t count);

#endif
