/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION), as
 * `heroes_runtime.h`.
 *
 * hero_stream.h — the platform layer's streams: the one handle for everything
 * a program reads and writes (M-rebel-library; spec § 13).
 *
 * A HEADER OF THE PLATFORM LAYER, AND NOT OF THE RUNTIME, as `hero_net.h`
 * is: its functions are defined in `platform/hero_stream.c`, a member of the
 * archive on every program's link line, which the linker takes only into a
 * program that calls one; a group binds them by naming this header,
 * `extern "hero_stream.h"`, with no `link` word and no flag, and no unit
 * reads it unless one of its groups names it.
 *
 * ONE HANDLE, AND A TABLE OF OPERATIONS BEHIND IT. Heroes has no interfaces,
 * so what Unix does with the file descriptor this layer does with one handle:
 * a file, a pipe, the process's own input and outputs, a terminal, and,
 * through another member of the layer, a socket, are each a `hero_stream`,
 * read and written by the same two calls. A stream over something another
 * member holds is made from that member's table of three functions
 * (`hero_stream_make`, below), so this member names no other: a program that
 * only reads files links no socket.
 *
 * WHY C OF THE PROJECT'S OWN, AND NOT A BINDING OF THE PLATFORM'S: what a
 * stream stands on is an `int` on POSIX and a `HANDLE` on Windows; opening a
 * file is the variadic `open` and its flags on one and `CreateFileW`'s seven
 * arguments on the other; and a write to a pipe nobody reads raises a signal
 * that ends the program on POSIX, where on Windows it is an error's number.
 * So no platform type reaches this header: a group sees `int64_t`,
 * `uint64_t`, `void *`, `const char *` and the handle below.
 *
 * NO CONDITIONAL BUT THE GUARD (held by the net's `runtime` suite): every
 * platform's arm of the member is compiled against these prototypes.
 *
 * EVERY FAILURE IS A VALUE, NEVER A FAULT: a function handing out a stream
 * answers NULL, one answering a count or a position -1, and
 * `hero_stream_last_error()` then names the kind. A stream is a handle the
 * layer issues and checks, never an address it reads through, so one closed
 * already, NULL, or one this layer never issued is `HERO_STREAM_NOT_OPEN`,
 * never a read of freed memory and never another stream that took its
 * number. A handle of another member of the layer, a socket's or a
 * process's, is one this layer never issued, `HERO_STREAM_NOT_OPEN` too,
 * and never one of its slots: each table's handles carry a tag of their own
 * since 2026-11-16 (`platform/hero_handle_tag.h`). A write to a pipe or a
 * socket whose reader is gone is a value too, `HERO_STREAM_CLOSED_PEER`: no
 * signal reaches the program for it.
 *
 * BYTES AS THEY ARE: nothing is translated on any platform, no line ending
 * and no encoding, a terminal's bytes included. A path is UTF-8.
 *
 * NOTHING THE LAYER OPENS IS HANDED TO A CHILD PROCESS BY DEFAULT: every
 * descriptor is close-on-exec and every `HANDLE` not inheritable. A member
 * that starts a process passes a stream on by its own act
 * (`hero_stream_hold`, below).
 */

#ifndef HERO_STREAM_H
#define HERO_STREAM_H

#include <stdint.h>

/* A stream this layer holds open: a group declares it as a handle,
 * `record Stream tag hero_stream`, which `hero_stream_close` gives back. */
typedef struct hero_stream hero_stream;

/* The kinds of failure, the same numbers on every platform; the platform's
 * own number for the last failure is `hero_stream_last_os_error()`. */
#define HERO_STREAM_OK 0
/* An argument the layer refuses: no path, an access, a permission, a
 * standard stream or a place to seek from that is none of those below, a
 * count with no bytes behind it, a read of no bytes, or a close of one of
 * the process's own three streams. */
#define HERO_STREAM_INVALID 1
/* The stream is none this layer holds open: closed already, or never one. */
#define HERO_STREAM_NOT_OPEN 2
/* The path names nothing, or a folder on its way is not there. */
#define HERO_STREAM_NOT_FOUND 3
/* `HERO_STREAM_ACCESS_CREATE` found the file already there. */
#define HERO_STREAM_EXISTS 4
/* The system refused the access: the file's permissions, a read of a stream
 * opened to write, a write of one opened to read. */
#define HERO_STREAM_DENIED 5
/* A name on the path's way is a file, where a folder must stand. */
#define HERO_STREAM_NOT_A_DIRECTORY 6
/* The path names a folder, which is no stream of bytes. */
#define HERO_STREAM_IS_A_DIRECTORY 7
/* The process or the machine has no descriptor, handle, room or memory
 * left, or the path is longer than the system takes. */
#define HERO_STREAM_LIMIT 8
/* The stream has no positions: a pipe, a terminal or a socket was asked to
 * seek, or for its size, or to be cut. */
#define HERO_STREAM_NOT_SEEKABLE 9
/* A write to a pipe or a socket whose reader is gone. A value, never a
 * signal that ends the program. */
#define HERO_STREAM_CLOSED_PEER 10
/* The stream, or the platform, has no such operation: a sync of what is no
 * file, a read or a write the stream's table of operations does not hold, a
 * descriptor of a stream that stands on none. */
#define HERO_STREAM_UNSUPPORTED 11
#define HERO_STREAM_OTHER 12
/* 13 to 15 are left unused: the sibling headers give those numbers other
 * meanings, and a reader comparing two of them should not meet one number
 * meaning two things. */
/* A deadline the stream's source keeps passed before the call could end: a
 * socket's, set by `hero_net_set_deadline` (`hero_stream_net.h`). */
#define HERO_STREAM_TIMED_OUT 16
/* The program asked to stop (`hero_net_ask_stop`, or Ctrl-C after
 * `hero_net_catch_stop`) while the call waited. */
#define HERO_STREAM_STOPPED 17

/* The kinds of stream `hero_stream_kind` answers. */
/* A file of the file system, with positions and a size. */
#define HERO_STREAM_KIND_FILE 1
/* A pipe, or a named pipe. */
#define HERO_STREAM_KIND_PIPE 2
/* A terminal, or a Windows console. */
#define HERO_STREAM_KIND_TERMINAL 3
#define HERO_STREAM_KIND_SOCKET 4
/* Anything else the system reads and writes: a device, the null device. */
#define HERO_STREAM_KIND_OTHER 5

/* The accesses `hero_stream_open` takes, one of them. */
/* To read; the file must be there. */
#define HERO_STREAM_ACCESS_READ 1
/* To write from the start: the file is created where absent and emptied
 * where there. */
#define HERO_STREAM_ACCESS_WRITE 2
/* To write a file this call creates: where one is there already the call
 * fails `HERO_STREAM_EXISTS` and touches nothing, also where the name is a
 * link to nothing. */
#define HERO_STREAM_ACCESS_CREATE 3
/* To write at the end: the file is created where absent, and every write
 * goes after the last byte the file holds at that moment, wherever a seek
 * left the position. */
#define HERO_STREAM_ACCESS_APPEND 4
/* To read and to write; the file must be there and is not emptied. */
#define HERO_STREAM_ACCESS_READ_WRITE 5

/* The process's own three streams, for `hero_stream_std`. */
#define HERO_STREAM_STD_IN 0
#define HERO_STREAM_STD_OUT 1
#define HERO_STREAM_STD_ERR 2

/* Where `hero_stream_seek` counts its offset from. */
#define HERO_STREAM_FROM_START 0
#define HERO_STREAM_FROM_CURRENT 1
#define HERO_STREAM_FROM_END 2

/* The file at `path`, opened for `access`, one of `HERO_STREAM_ACCESS_*`.
 * `perm` is the permission of a file the call creates, the nine bits of
 * POSIX, 0 to 0777 (`0644` reads for all and writes for its owner), less
 * what the process's mask takes away; a file already there keeps its own. It
 * must be 0 to 0777 whatever the access. Windows has no such bits: there a
 * file created takes its folder's access and `perm` changes nothing. A path
 * that names a folder fails `HERO_STREAM_IS_A_DIRECTORY`, except to
 * `HERO_STREAM_ACCESS_CREATE`, which finds it there and fails
 * `HERO_STREAM_EXISTS`, on every platform. Other programs may
 * open, write, rename and remove the file while the stream is open, on every
 * platform. */
hero_stream *hero_stream_open(const char *path, int64_t access, int64_t perm);

/* One of the process's own three streams, `which` one of `HERO_STREAM_STD_*`.
 * BORROWED: the same handle at every call, never given back and never closed
 * by the program, so a group binds it with no `acquires`. NULL with
 * `HERO_STREAM_NOT_OPEN` where the process was started without that stream. */
hero_stream *hero_stream_std(int64_t which);

/* A pipe: what is written to `*writer` is read from `*reader`, in order.
 * Answers 0 with both set, or -1 with both NULL. A read of the pipe answers 0
 * once every writer is closed and its bytes are read; a write to it once
 * every reader is closed fails `HERO_STREAM_CLOSED_PEER`. Each end is given
 * back by its own `hero_stream_close`. */
int64_t hero_stream_pipe(hero_stream **reader, hero_stream **writer);

/* Reads up to `count` bytes into `buffer`, waiting for one, and answers how
 * many, which may be fewer than `count`; 0 at the end: the end of a file, a
 * pipe whose writers are all closed, a peer that closed its side. So a read
 * of no bytes, `count` 0, is refused `HERO_STREAM_INVALID`, since its 0
 * would read as the end. */
int64_t hero_stream_read(hero_stream *s, void *buffer, uint64_t count);

/* Writes up to `count` bytes of `data` and answers how many were written,
 * which may be fewer: the caller writes the rest. A write of no bytes
 * answers 0. To a pipe or a socket whose reader is gone it fails
 * `HERO_STREAM_CLOSED_PEER`. */
int64_t hero_stream_write(hero_stream *s, const void *data, uint64_t count);

/* Moves the position the next read or write begins at to `offset` bytes
 * from `from`, one of `HERO_STREAM_FROM_*`, and answers the new position,
 * counted from the start; with offset 0 from `HERO_STREAM_FROM_CURRENT` it
 * reads the position and moves nothing. A position past the end is allowed
 * and one before the start refused `HERO_STREAM_INVALID`. A pipe, a terminal
 * or a socket fails `HERO_STREAM_NOT_SEEKABLE`. */
int64_t hero_stream_seek(hero_stream *s, int64_t offset, int64_t from);

/* The file's size in bytes, as the system holds it now, or -1;
 * `HERO_STREAM_NOT_SEEKABLE` for what is no file. */
int64_t hero_stream_size(hero_stream *s);

/* Asks the system to put everything written to the file on the storage
 * itself, and answers 0 once it says it has, or -1. What is no file fails
 * `HERO_STREAM_UNSUPPORTED`: its bytes went to their reader when they were
 * written. A stream not opened to write is `HERO_STREAM_DENIED` on every
 * platform, since Windows gives such a stream no right to ask. */
int64_t hero_stream_sync(hero_stream *s);

/* Makes the file `size` bytes long, cutting what is past it or adding bytes
 * to reach it, and answers 0, or -1. The position does not move. Bytes
 * added read as zero on POSIX; Microsoft's page for Windows leaves them
 * undefined. A negative size is `HERO_STREAM_INVALID`, what is no file
 * `HERO_STREAM_NOT_SEEKABLE`, a stream not opened to write
 * `HERO_STREAM_DENIED`, and one opened by `HERO_STREAM_ACCESS_APPEND`
 * `HERO_STREAM_DENIED` on every platform, since Windows gives such a stream
 * no right to cut. */
int64_t hero_stream_truncate(hero_stream *s, int64_t size);

/* What the stream is, one of `HERO_STREAM_KIND_*`, or -1. */
int64_t hero_stream_kind(hero_stream *s);

/* 1 where the stream is a terminal, 0 where it is not, or -1. */
int64_t hero_stream_is_terminal(hero_stream *s);

/* Closes the stream and gives its handle back, whatever is answered: 0, or
 * -1 where the stream was not open or the system reported a failure while
 * closing it. No later call reaches the stream. A call another thread is
 * still in on it runs to its end and answers what the system answers, and
 * what the stream stands on is released when the last such call returns.
 * One of the process's own three streams is refused `HERO_STREAM_INVALID`:
 * nothing is closed and its handle stays. */
int64_t hero_stream_close(hero_stream *s);

/* The kind of the last call's failure on this thread, `HERO_STREAM_OK` after
 * a call that did not fail, and the platform's own number for it (0 where
 * the layer itself refused). */
int64_t hero_stream_last_error(void);
int64_t hero_stream_last_os_error(void);

/* -- FOR THE OTHER MEMBERS OF THE LAYER, AND BOUND BY NO HEROES GROUP ------
 *
 * What stands below is called from C of the layer alone. No group declares
 * it: a `void *` and a table of functions are nothing a Heroes program can
 * be trusted to hand over. */

/* The table of operations a member hands out a stream over. Each function
 * is given the `payload` the stream was made with. `read` and `write`
 * answer as `hero_stream_read` and `hero_stream_write` do, a count or -1,
 * and `close` 0 or -1; one that answers -1 first names the failure with
 * `hero_stream_set_error`, and one that does not is told
 * `HERO_STREAM_OTHER`. A function may be NULL: a stream with no `read` or no
 * `write` fails that call `HERO_STREAM_UNSUPPORTED`, and one with no `close`
 * has nothing to release. `read` is never called with a count of 0, and no
 * function with a count above 2^30.
 *
 * `close` is called once, on the thread that leaves the stream last: at
 * `hero_stream_close`, or, where another thread is still inside `read` or
 * `write`, when the last such call returns. So it never runs beside a
 * `read` or a `write` of the same stream, which may themselves run beside
 * each other on two threads. */
typedef struct hero_stream_ops {
    int64_t (*read)(void *payload, void *buffer, uint64_t count);
    int64_t (*write)(void *payload, const void *data, uint64_t count);
    int64_t (*close)(void *payload);
} hero_stream_ops;

/* A stream over `payload`, read, written and closed through `ops`, which
 * must stay where it is for as long as the stream is open (a `static const`
 * table), and answering `kind`, one of `HERO_STREAM_KIND_*`, to
 * `hero_stream_kind`. Its handle is given back by `hero_stream_close`, which
 * calls `ops->close`. NULL with `HERO_STREAM_INVALID` where `ops` is NULL or
 * `kind` is none, and with `HERO_STREAM_LIMIT` where the layer holds as many
 * streams as it can: the caller then still owns `payload`, and `close` was
 * not called. It has no position, no size and no descriptor: `seek`, `size`
 * and `truncate` fail `HERO_STREAM_NOT_SEEKABLE`, `sync`, `descriptor` and
 * `hold` `HERO_STREAM_UNSUPPORTED`. */
hero_stream *hero_stream_make(void *payload, const hero_stream_ops *ops, int64_t kind);

/* Sets this layer's last error on the calling thread, `kind` one of the
 * kinds above and `os_error` the platform's own number or 0: how a function
 * of a table says why it answered -1, and how a member that takes a stream
 * says what it refused. */
void hero_stream_set_error(int64_t kind, int64_t os_error);

/* The platform's own descriptor of the stream as a number: the `int` of a
 * POSIX file, pipe or standard stream, or the `HANDLE` of a Windows one, its
 * bits unchanged; or -1, `HERO_STREAM_UNSUPPORTED` for a stream that stands
 * on none (one made by `hero_stream_make`) and `HERO_STREAM_NOT_OPEN` for
 * what is no open stream. THE NUMBER ALONE, HELD BY NOTHING: a close on
 * another thread may hand it to another file before the caller uses it, so a
 * member that passes it to the system uses `hero_stream_hold`. */
int64_t hero_stream_descriptor(hero_stream *s);

/* What a member that starts a process needs to pass a stream to a child as
 * its input or an output: the stream's descriptor, as
 * `hero_stream_descriptor` answers it, HELD OPEN until `hero_stream_release`
 * is called on the same handle, whatever closes the stream meanwhile; or -1,
 * nothing held. Each hold is released once.
 *
 * The descriptor is close-on-exec, and the `HANDLE` not inheritable, as
 * everything the layer opens: the member duplicates it into the child by its
 * own act (`dup2` after the fork, or a spawn's file action, neither of which
 * copies the close-on-exec mark; `DuplicateHandle` asking an inheritable
 * copy, which the member closes after the launch). One of the process's own
 * three streams is the exception: it was not opened by the layer, and its
 * descriptor or `HANDLE` is held as the process received it. */
int64_t hero_stream_hold(hero_stream *s);

/* Ends one `hero_stream_hold` of `s`, which may have been closed meanwhile:
 * where it was and this was its last user, what it stands on is released
 * here. Answers 0, or -1 with `HERO_STREAM_NOT_OPEN` where `s` is no stream
 * with a hold on it. */
int64_t hero_stream_release(hero_stream *s);

/* What a member that made a stream over a payload of its own needs to reach
 * that payload again (M-rebel-library, round 3, 2026-11-16: the socket under
 * a stream `hero_stream_net.h` made, whose deadline, options and shutdown a
 * program asks for after the socket became the stream): the payload into
 * `*payload`, HELD as `hero_stream_hold` holds a descriptor, so whatever
 * closes the stream meanwhile, its table's `close` is not called until
 * `hero_stream_release` is, once, on the same handle. Answers 0, or -1 with
 * nothing held: `HERO_STREAM_NOT_OPEN` where `s` is no open stream,
 * `HERO_STREAM_INVALID` where `ops` or `payload` is NULL, or where `s`
 * stands on a descriptor or was made with another table than `ops`, so a
 * member reaches no payload but one of its own. */
int64_t hero_stream_hold_payload(hero_stream *s, const hero_stream_ops *ops, void **payload);

#endif
