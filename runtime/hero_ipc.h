/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION), as
 * `heroes_runtime.h`.
 *
 * hero_ipc.h — the platform layer's communication between the processes of
 * one machine (M-rebel-library, 2026-11-17; the author's request of
 * 2026-11-16): a local channel by name, memory shared by name, and a lock
 * and a semaphore between processes, named and waited on with a bound.
 *
 * A HEADER OF THE PLATFORM LAYER, AND NOT OF THE RUNTIME, as `hero_net.h`
 * is: its functions are defined in `platform/hero_ipc.c`, a member of the
 * archive on every program's link line, which the linker takes only into a
 * program that calls one; a group binds them by naming this header,
 * `extern "hero_ipc.h"`, with no `link` word and no flag. On Windows the
 * member links `ws2_32` for its sockets.
 *
 * WHY C OF THE PROJECT'S OWN: a local socket's address is `sockaddr_un` with
 * a path on POSIX and on Windows 10 1803 and later, a byte of zero before a
 * name on Linux alone, and on Windows a named pipe besides; shared memory is
 * `shm_open` and `mmap` on POSIX and a section and a view on Windows; a
 * semaphore that waits with a bound is `sem_timedwait` on Linux and nothing
 * at all on macOS. So no platform type reaches this header: a group sees
 * `int64_t`, `uint64_t`, `void *`, `const char *` and the handles below.
 *
 * NO CONDITIONAL BUT THE GUARD: every platform's arm of the member is
 * compiled against these prototypes.
 *
 * EVERY FAILURE IS A VALUE, NEVER A FAULT: a function handing out a handle
 * or a stream answers NULL, one answering a count -1, and
 * `hero_ipc_last_error()` then names the kind. A handle is one the layer
 * issues and checks, a slot and a life in one pointer's bits with this
 * member's tags (`platform/hero_handle_tag.h`), never an address it reads
 * through: one closed already, NULL, one of another member, or one this
 * layer never issued is `HERO_IPC_NOT_OPEN`, never a read of freed memory.
 *
 * NO POINTER INTO SHARED MEMORY EVER LEAVES THIS MEMBER. Heroes values are
 * copies, so memory shared by name is read and written by copies whose every
 * offset and length is checked against its size before any byte moves: one
 * past it is `HERO_IPC_INVALID` and never reaches the mapping.
 *
 * A CHANNEL IS A STREAM (`hero_stream.h`): what `hero_ipc_accept` and
 * `hero_ipc_connect` hand out is a `hero_stream`, made by
 * `hero_stream_make` over this member's table, so a program reads, writes
 * and closes it with `hero_stream_read`, `hero_stream_write` and
 * `hero_stream_close`; its failures are then the stream's kinds.
 *
 * EVERY WAIT IS BOUNDED, AND ENDS AT THE STOP: a call that waits takes
 * `within_ms`, the milliseconds it may wait, 0 to try once without waiting
 * and `HERO_IPC_FOREVER` for no bound; a channel's reads and writes end by
 * the deadline `hero_ipc_set_deadline` gives. And every wait of this member
 * asks, at least every 50 ms, whether the program was asked to stop
 * (`hero_net.h`'s `hero_net_catch_stop` and `hero_net_ask_stop`), and
 * answers `HERO_IPC_STOPPED` if so, and whether another thread closed the
 * handle it waits on, `HERO_IPC_NOT_OPEN` if so.
 *
 * NOTHING THIS MEMBER OPENS IS HANDED TO A CHILD PROCESS: every descriptor
 * is close-on-exec, and every `HANDLE` not inheritable.
 */

#ifndef HERO_IPC_H
#define HERO_IPC_H

#include <stdint.h>

#include "hero_stream.h"

/* A listening channel: a group declares it as a handle,
 * `record Listener tag hero_ipc_listener`, which `hero_ipc_listener_close`
 * gives back. */
typedef struct hero_ipc_listener hero_ipc_listener;
/* Memory shared by name, mapped into this process, which
 * `hero_ipc_memory_close` gives back. */
typedef struct hero_ipc_memory hero_ipc_memory;
/* A semaphore shared by name, which `hero_ipc_semaphore_close` gives back. */
typedef struct hero_ipc_semaphore hero_ipc_semaphore;
/* A lock shared by name, which `hero_ipc_lock_close` gives back. */
typedef struct hero_ipc_lock hero_ipc_lock;

/* The kinds of failure, the same numbers on every platform, and the same
 * numbers as `hero_stream.h`'s kinds of the same names; the platform's own
 * number for the last failure is `hero_ipc_last_os_error()`. */
#define HERO_IPC_OK 0
/* An argument the layer refuses: a name outside the rules below, a path
 * longer than the system's address holds, a size or an offset outside the
 * memory, a count of no bytes, a `within_ms` below 0 that is not
 * `HERO_IPC_FOREVER`, a lock acquired again by the thread that holds it. */
#define HERO_IPC_INVALID 1
/* The handle is none this layer holds open: closed already, or never one,
 * or closed by another thread while the call waited. */
#define HERO_IPC_NOT_OPEN 2
/* The name names nothing: no listener at a path, no memory, semaphore or
 * lock of that name, or memory its maker has not sized yet. */
#define HERO_IPC_NOT_FOUND 3
/* Something has that name already: a file at a listener's path (a socket
 * file a process that died left among them, which the listen never
 * removes), a listener on that pipe's name, memory or a semaphore of that
 * name; on Windows one name names one object of any kind. */
#define HERO_IPC_EXISTS 4
/* The system refuses the access: the name's permissions are another's. */
#define HERO_IPC_DENIED 5
/* The process or the machine has no descriptor, handle, memory or room left,
 * or the layer's table is full. */
#define HERO_IPC_LIMIT 8
/* A write to a channel whose other end is gone, or the other end reset it. */
#define HERO_IPC_CLOSED_PEER 10
/* This platform has no such thing: an abstract name off Linux, a named pipe
 * off Windows, a process's user on Windows. */
#define HERO_IPC_UNSUPPORTED 11
#define HERO_IPC_OTHER 12
/* A socket file is at the path and nothing listens on it: the file a process
 * that died left, or one whose listener is not yet listening. */
#define HERO_IPC_REFUSED 13
/* The lock is not held by the calling thread, so it cannot release it. */
#define HERO_IPC_NOT_HELD 14
/* The bound a wait was given passed first. */
#define HERO_IPC_TIMED_OUT 16
/* The program asked to stop while the call waited (`hero_net.h`). */
#define HERO_IPC_STOPPED 17

/* What `within_ms` takes for a wait with no bound. */
#define HERO_IPC_FOREVER (-1)

/* Where a channel's name lives, `space` below. */
/* A path of the file system, a socket file made there: on Darwin, Linux, and
 * Windows 10 1803 and later. At most 103 bytes on Darwin and 107 on Linux
 * and Windows, no byte of zero. */
#define HERO_IPC_SPACE_PATH 1
/* A name in Linux's abstract space, which no file holds and which ends with
 * its last user. `HERO_IPC_UNSUPPORTED` elsewhere. */
#define HERO_IPC_SPACE_ABSTRACT 2
/* A Windows named pipe, `\\.\pipe\` and the name, which the system ends
 * with its last user. `HERO_IPC_UNSUPPORTED` elsewhere. */
#define HERO_IPC_SPACE_PIPE 3

/* THE NAMES OF MEMORY, SEMAPHORES AND LOCKS, and of abstract and pipe
 * channels: 1 to 29 bytes of ASCII letters, digits, `.`, `_` and `-`, the
 * first no `.` (29 is macOS's: a semaphore's name there is at most 30 bytes
 * with the `/` POSIX puts before it, measured 2026-11-17). On POSIX memory
 * is `shm_open`'s `/<name>` and a semaphore `sem_open`'s `/<name>`, so a
 * program in another language finds them by the same name; a lock is the
 * file `/tmp/heroes-lock-<name>`. On Windows each is the object `<name>` in
 * the session's space. */
#define HERO_IPC_NAME_MOST 29

/* -- channels -------------------------------------------------------------- */

/* A listener on `name` in `space`, one of `HERO_IPC_SPACE_*`. For a path,
 * the socket file is made with the permission `perm`, the nine bits of POSIX
 * (0600 lets only its owner connect), set before the first connection can
 * be made and taken away by no mask; Windows has no such bits and ignores
 * it, and so do the other two spaces. It must be 0 to 0777. A file already
 * at the path fails `HERO_IPC_EXISTS` and stays, a socket file a process that
 * died left among them, as Go's `net.Listen` does. */
hero_ipc_listener *hero_ipc_listen(const char *name, int64_t space, int64_t perm);

/* The next connection on `l`, as a stream of kind
 * `HERO_STREAM_KIND_SOCKET` (`HERO_STREAM_KIND_PIPE` for a pipe), which
 * `hero_stream_close` gives back; waiting at most `within_ms` for one, and
 * failing `HERO_IPC_TIMED_OUT` past it. */
hero_stream *hero_ipc_accept(hero_ipc_listener *l, int64_t within_ms);

/* A connection to the listener on `name` in `space`, as `hero_ipc_accept`'s
 * stream; waiting at most `within_ms` while the listener has no room for
 * another. `HERO_IPC_NOT_FOUND` where nothing has the name,
 * `HERO_IPC_REFUSED` where a socket file has it and nothing listens. */
hero_stream *hero_ipc_connect(const char *name, int64_t space, int64_t within_ms);

/* Closes the listener and gives its handle back, whatever is answered: 0,
 * or -1. A path's socket file is removed where it is still the one the
 * listen made, as Go's does. An accept another thread waits in ends within
 * about 50 ms, failing `HERO_IPC_NOT_OPEN`. */
int64_t hero_ipc_listener_close(hero_ipc_listener *l);

/* Makes every later and waiting read and write of the channel `s` end by
 * `ms` milliseconds from now, failing the stream's `HERO_STREAM_TIMED_OUT`;
 * 0 takes the deadline away. 0, or -1: `HERO_IPC_INVALID` where `ms` is
 * below 0 or `s` is no channel of this member, `HERO_IPC_NOT_OPEN` where it
 * is no open stream. */
int64_t hero_ipc_set_deadline(hero_stream *s, int64_t ms);

/* Closes the writing side of the channel `s`, so its peer reads its end
 * once the bytes written are read, while `s` still reads. 0, or -1;
 * `HERO_IPC_UNSUPPORTED` for a named pipe, which has no half. */
int64_t hero_ipc_shutdown_write(hero_stream *s);

/* The process at the other end of the channel `s`, as the system tells it
 * at the connection: its id into `*pid` and its user's id into `*uid`
 * (-1 on Windows, which has none of that number). 0, or -1. */
int64_t hero_ipc_peer(hero_stream *s, int64_t *pid, int64_t *uid);

/* -- shared memory ----------------------------------------------------------- */

/* New memory named `name`, at least `size` bytes, every byte zero, mapped
 * into this process; `perm` the nine bits of POSIX, less the process's
 * mask, ignored on Windows. `HERO_IPC_EXISTS` where the name is taken, and
 * `HERO_IPC_INVALID` for a size of 0. Its size is what the system holds,
 * which is `size` on Linux and `size` rounded up to the page on macOS (16
 * KB; both measured 2026-11-17) and on Windows (its page; unrun). On Linux
 * its pages are reserved at once, so memory the machine has no room for
 * fails here, `HERO_IPC_LIMIT`, and never at a write (measured: 200 MB on a
 * `/dev/shm` of 64 MB refused `ENOSPC`). */
hero_ipc_memory *hero_ipc_memory_create(const char *name, int64_t size, int64_t perm);

/* The memory named `name`, made by any process, mapped into this one. */
hero_ipc_memory *hero_ipc_memory_open(const char *name);

/* The memory's size in bytes, or -1. */
int64_t hero_ipc_memory_size(hero_ipc_memory *m);

/* Copies `count` bytes from `offset` into `buffer` and answers `count`, or
 * -1; `HERO_IPC_INVALID` where the bytes do not all stand inside the
 * memory, before any is read. A count of 0 answers 0. */
int64_t hero_ipc_memory_read(hero_ipc_memory *m, int64_t offset, void *buffer, uint64_t count);

/* Copies the `count` bytes of `data` to `offset` and answers `count`, or -1,
 * `HERO_IPC_INVALID` as for a read, before any is written. */
int64_t hero_ipc_memory_write(hero_ipc_memory *m, int64_t offset, const void *data, uint64_t count);

/* THE ATOMIC WORDS: an `int64_t` at `offset`, a multiple of 8 inside the
 * memory, read and written whole by one instruction that every process
 * mapping the memory sees whole, in sequentially consistent order, with no
 * lock. `HERO_IPC_INVALID` for an offset that is no multiple of 8 or not
 * inside. */
/* The word, into `*value`. 0, or -1. */
int64_t hero_ipc_memory_load(hero_ipc_memory *m, int64_t offset, int64_t *value);
/* The word made `value`. 0, or -1. */
int64_t hero_ipc_memory_store(hero_ipc_memory *m, int64_t offset, int64_t value);
/* The word plus `delta`, the sum into `*value`, wrapping at 64 bits as two's
 * complement, since no process can be stopped from the sum the others make.
 * 0, or -1. */
int64_t hero_ipc_memory_add(hero_ipc_memory *m, int64_t offset, int64_t delta, int64_t *value);
/* The word made `desired` where it is `expected`, the word found into
 * `*found` either way, so the swap was made where `*found == expected`.
 * 0, or -1. */
int64_t hero_ipc_memory_swap(hero_ipc_memory *m, int64_t offset, int64_t expected, int64_t desired, int64_t *found);

/* Unmaps the memory and gives the handle back, whatever is answered: 0, or
 * -1. The memory stays for every other process that has it, and on POSIX
 * for every later open, until `hero_ipc_memory_remove`. A copy another
 * thread is in runs to its end first. */
int64_t hero_ipc_memory_close(hero_ipc_memory *m);

/* Takes the name `name` away from its memory, so no later open finds it and
 * a create may take it again; who has it mapped keeps it until they close
 * it. On Windows the system takes a name away with its last handle, so
 * this answers 0 where memory of that name is open and changes nothing.
 * `HERO_IPC_NOT_FOUND` where none is. */
int64_t hero_ipc_memory_remove(const char *name);

/* -- semaphores -------------------------------------------------------------- */

/* A new semaphore named `name`, counting `count`, 0 to 32,767 (macOS's
 * most), `perm` as for memory. `HERO_IPC_EXISTS` where the name is taken. */
hero_ipc_semaphore *hero_ipc_semaphore_create(const char *name, int64_t count, int64_t perm);

/* The semaphore named `name`, made by any process. */
hero_ipc_semaphore *hero_ipc_semaphore_open(const char *name);

/* Takes one from the count, waiting at most `within_ms` while it is 0. 0,
 * or -1: `HERO_IPC_TIMED_OUT` past the bound. ON MACOS, which has no
 * `sem_timedwait`, the wait tries again in steps that grow from 0.1 ms to
 * 2 ms, so it sees a count given back within about 3 ms, and costs under
 * 1% of a core while it waits (measured 2026-11-17; the member's opening
 * comment). Linux waits in `sem_clockwait`, Windows on the object. */
int64_t hero_ipc_semaphore_wait(hero_ipc_semaphore *s, int64_t within_ms);

/* Adds one to the count, waking one waiter. 0, or -1: `HERO_IPC_LIMIT` past
 * the platform's most where it tells one: Linux's 2,147,483,647
 * (`EOVERFLOW`, measured 2026-11-17), Windows' the same by its reference
 * (unrun); macOS takes a post past its 32,767 and says nothing (measured). */
int64_t hero_ipc_semaphore_post(hero_ipc_semaphore *s);

/* Gives the handle back, whatever is answered: 0, or -1. The semaphore
 * stays as `hero_ipc_memory_close` says memory stays. */
int64_t hero_ipc_semaphore_close(hero_ipc_semaphore *s);

/* Takes the name `name` away from its semaphore, as
 * `hero_ipc_memory_remove` does for memory, Windows' rule the same. */
int64_t hero_ipc_semaphore_remove(const char *name);

/* -- locks ------------------------------------------------------------------- */

/* The lock named `name`, made where no process has it yet, `perm` as for
 * memory. A lock is held by one thread of one process at a time, and only
 * that thread releases it.
 *
 * ON POSIX it is the file `/tmp/heroes-lock-<name>` locked whole by
 * `flock`, which the system releases when the process holding it ends, how
 * ever it ends; on Windows a mutex, which the system gives the next waiter
 * when the thread holding it ends, marked abandoned. The file is refused
 * `HERO_IPC_DENIED` where it is no regular file or has another name besides
 * (a link someone made to make this write elsewhere). */
hero_ipc_lock *hero_ipc_lock_open(const char *name, int64_t perm);

/* Takes the lock, waiting at most `within_ms` while another holds it.
 * Answers 0, or 1 where the holder before ended without releasing it (its
 * process killed, or on Windows its thread ended), so what it guarded may
 * be half done; or -1: `HERO_IPC_TIMED_OUT` past the bound,
 * `HERO_IPC_INVALID` where the calling thread holds it already, through this
 * handle or another (it would wait for itself). ON POSIX the wait tries
 * again in steps that grow from 0.1 ms to 2 ms, `flock` taking no bound; on
 * Windows it waits on the mutex. */
int64_t hero_ipc_lock_acquire(hero_ipc_lock *l, int64_t within_ms);

/* Releases the lock. 0, or -1: `HERO_IPC_NOT_HELD` where the calling thread
 * does not hold it. */
int64_t hero_ipc_lock_release(hero_ipc_lock *l);

/* Gives the handle back, whatever is answered: 0, or -1, releasing the lock
 * first where the calling thread holds it. Closed by another thread while
 * held, it is released on POSIX and the next holder is told it abandoned;
 * on Windows the mutex stays its holder's until that thread releases it or
 * ends. */
int64_t hero_ipc_lock_close(hero_ipc_lock *l);

/* Takes the name `name` away: on POSIX the lock's file is removed, so a
 * process that opens it later has another lock than a process that has it
 * open: remove a lock only once no process uses it. On Windows as
 * `hero_ipc_memory_remove`. `HERO_IPC_NOT_FOUND` where there is none. */
int64_t hero_ipc_lock_remove(const char *name);

/* The kind of the last call's failure on this thread, `HERO_IPC_OK` after
 * a call that did not fail, and the platform's own number for it (0 where
 * the layer itself refused). */
int64_t hero_ipc_last_error(void);
int64_t hero_ipc_last_os_error(void);

#endif
