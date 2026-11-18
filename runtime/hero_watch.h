/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION), as
 * `heroes_runtime.h`.
 *
 * hero_watch.h — the platform layer's watches: a folder, a folder's whole
 * tree, or one file watched, and every change the system tells of it handed
 * to the program as a value, in order (M-rebel-library, 2026-11-18; spec
 * § 13). What Java's `WatchService`, .NET's `FileSystemWatcher`, Go's
 * `fsnotify` and Python's `watchdog` give a program, as one wait with a
 * deadline, and a bell a readiness set of `hero_poll.h` can wait on beside
 * sockets and timers.
 *
 * A HEADER OF THE PLATFORM LAYER, AND NOT OF THE RUNTIME, as `hero_poll.h`
 * is: its functions are defined in `platform/hero_watch.c`, a member of the
 * archive on every program's link line, which the linker takes only into a
 * program that calls one; a group binds them by naming this header,
 * `extern "hero_watch.h"`, with no `link` word and no flag. The member calls
 * `hero_net.h` (the stop, and the bell on Windows) and `hero_stream.h` (the
 * bell elsewhere), so a program that takes it takes those two; on macOS it
 * links CoreServices, on Windows `ws2_32`.
 *
 * NO CONDITIONAL BUT THE GUARD, AND NO PLATFORM TYPE: a group sees `int64_t`,
 * `uint64_t`, `void *`, `const char *`, the watch below, and the two handles
 * of the headers this one includes for their names.
 *
 * EVERY FAILURE IS A VALUE, NEVER A FAULT: a function handing out a watch
 * answers NULL, one answering a count -1, and `hero_watch_last_error()` then
 * names the kind. A watch is a handle the layer issues and checks, never an
 * address it reads through, so one closed already, NULL, or one this layer
 * never issued is `HERO_WATCH_NOT_OPEN`.
 *
 * WHAT IS WATCHED, AND HOW, ON EACH SYSTEM. A folder's own entries, the
 * entries of its whole tree (`HERO_WATCH_TREE`), or one file, which is
 * watched as its folder's entry of that name, so a file replaced by a rename
 * over it, as an editor saves, is still watched after. Linux: `inotify`, one
 * watch per folder, so a tree costs one of the user's watches per folder
 * (`/proc/sys/fs/inotify/max_user_watches`, `HERO_WATCH_LIMIT` past it); a
 * folder made in a watched tree is watched from when its creation is read,
 * and walked then, its entries told `HERO_WATCH_CREATED`. macOS: FSEvents
 * with file-level events, chosen over `kqueue` by measurement (2026-11-18,
 * `hero_watch.c` says what each answered): one stream per watch, which the
 * system keeps for the whole tree below the folder, so a watch of one
 * folder's own entries, or of one file, is told the tree's changes and keeps
 * its own. Windows: `ReadDirectoryChangesExW` (plain
 * `ReadDirectoryChangesW` where the system or the volume has not the
 * extended form), one directory handle per watch, which the system keeps for
 * the whole tree where a tree is asked. Each watch has one thread of the
 * layer's own on Linux and Windows, and one dispatch queue on macOS, which
 * read the system's changes into the watch's queue.
 *
 * WHAT AN EVENT IS: a kind below, whether the entry is a folder, its path,
 * and for a rename its path before. A path is the watched path AS THE
 * PROGRAM GAVE IT, joined with the entry's name below it by the system's
 * separator (a file watched is told by the very path given), so the program
 * can hand it on as it stands. Changes that come close together may be
 * told as one: a write told again before the program took the first is told
 * once, and on macOS the system itself joins the changes of one path that
 * come within about a second. An event the system told of what happened
 * just before the watch began may be told.
 *
 * THE QUEUE IS BOUNDED: a watch holds at most the number of events it was
 * opened with; past it, the events that do not fit are lost and ONE event
 * `HERO_WATCH_OVERFLOW` stands where they were, as it does where the system
 * itself lost them (Linux's queue, a macOS stream told to rescan, a Windows
 * buffer too small). After an overflow a program reads the tree again.
 *
 * THE STOP ENDS A WAIT (`hero_net.h`'s, as `hero_poll.h`'s waits end): once
 * `hero_net_ask_stop` is called, or a signal is caught after
 * `hero_net_catch_stop`, a wait answers -1 with `HERO_WATCH_STOPPED`, one
 * already waiting at once, and every later one before it waits.
 */

#ifndef HERO_WATCH_H
#define HERO_WATCH_H

#include <stdint.h>

#include "hero_net.h"
#include "hero_stream.h"

/* A watch this layer holds open: a group declares it as a handle,
 * `record Watch tag hero_watch`, which `hero_watch_close` gives back. */
typedef struct hero_watch hero_watch;

/* The kinds of failure, the same numbers on every platform; the platform's
 * own number for the last failure is `hero_watch_last_os_error()`. */
#define HERO_WATCH_OK 0
/* An argument the layer refuses: no path or an empty one, a flag or a size
 * outside what the function names, a limit below -1, a buffer too short to
 * hold an event's numbers. */
#define HERO_WATCH_INVALID 1
/* The watch is none this layer holds open: closed already, or never one. */
#define HERO_WATCH_NOT_OPEN 2
/* The path names nothing, or a folder on its way is not there. */
#define HERO_WATCH_NOT_FOUND 3
/* The system refused the program the path. */
#define HERO_WATCH_DENIED 4
/* A tree was asked of what is no folder, or a name on the path's way is a
 * file. */
#define HERO_WATCH_NOT_A_DIRECTORY 5
/* The process or the system has no watch, descriptor, thread, handle or
 * memory left: Linux's `max_user_watches` and `max_user_instances` among
 * them, or 1,024 watches open in the process. */
#define HERO_WATCH_LIMIT 6
/* The buffer handed to `hero_watch_next` is too short for the event's
 * paths: the numbers say how many bytes they take, and the event stays
 * first, for the next call. */
#define HERO_WATCH_SHORT 7
/* The watched folder is gone, its `HERO_WATCH_GONE` event was taken, and
 * nothing more will be told: the watch is still the program's to close. */
#define HERO_WATCH_ENDED 8
/* The stop was asked (THE STOP ENDS A WAIT, above). */
#define HERO_WATCH_STOPPED 9
/* This system has no such thing: the bell of the other kind (a stream on
 * Windows, a socket elsewhere), or a volume the system cannot watch. */
#define HERO_WATCH_UNSUPPORTED 10
#define HERO_WATCH_OTHER 11

/* The kinds of event, the first of an event's numbers. */
/* An entry came to be: made, or moved in from outside what is watched. */
#define HERO_WATCH_CREATED 1
/* An entry stopped being: removed, or moved out of what is watched. */
#define HERO_WATCH_REMOVED 2
/* A file's content was written. */
#define HERO_WATCH_CHANGED 3
/* An entry's facts changed and not its content: its permission, owner,
 * times or extended attributes. Windows tells these as
 * `HERO_WATCH_CHANGED`, which is all its system says. */
#define HERO_WATCH_ATTRIBUTES 4
/* An entry moved from one path to another, both inside what is watched: the
 * event's second path is where it stood before. */
#define HERO_WATCH_RENAMED 5
/* Events were lost here, by the system or for want of room in the queue:
 * the path is the watched path. */
#define HERO_WATCH_OVERFLOW 6
/* The watched folder (for a file, the folder holding it) was removed or
 * moved away, and nothing more will be told: the path is the watched path.
 * Told once, and last. */
#define HERO_WATCH_GONE 7

/* What `hero_watch_open` takes in `flags`: the whole tree below a folder,
 * where 0 watches its own entries alone. */
#define HERO_WATCH_TREE 1

/* The most events a watch holds before it overflows. */
#define HERO_WATCH_MOST 1048576
/* The numbers one event takes: its kind, 1 where the entry is a folder and
 * 0 where it is not, the bytes of its path, the bytes of its path before (0
 * but for `HERO_WATCH_RENAMED`). */
#define HERO_WATCH_EVENT_NUMBERS 4

/* A new watch of `path`, a folder or a file, UTF-8, which must be there:
 * `flags` 0 or `HERO_WATCH_TREE` (a folder alone), `most` the events it holds
 * before it overflows, 1 to `HERO_WATCH_MOST`. Every change made after the
 * call returns is told. NULL: `HERO_WATCH_INVALID` for another flag or size
 * or an empty path, `HERO_WATCH_NOT_FOUND`, `HERO_WATCH_DENIED`,
 * `HERO_WATCH_NOT_A_DIRECTORY` for a tree of a file, `HERO_WATCH_LIMIT`,
 * `HERO_WATCH_UNSUPPORTED` for a volume the system will not watch. */
hero_watch *hero_watch_open(const char *path, int64_t flags, int64_t most);

/* WAITS until an event is queued, `limit_ns` nanoseconds pass (0 asks
 * without waiting, -1 waits with no limit), or the stop is asked; then takes
 * the first event, writes its `HERO_WATCH_EVENT_NUMBERS` numbers into
 * `numbers` and its path, then its path before, into `text`, and answers 1;
 * 0 where the limit passed with none. -1: `HERO_WATCH_INVALID` for a limit
 * below -1, a `numbers_count` below 4 or a NULL buffer with a count;
 * `HERO_WATCH_SHORT` where `text_count` cannot hold the paths, the numbers
 * then written and the event left first; `HERO_WATCH_ENDED` once the
 * `HERO_WATCH_GONE` event was taken and the queue is empty;
 * `HERO_WATCH_NOT_OPEN`, also where the watch is closed while the call
 * waits; `HERO_WATCH_STOPPED`. */
int64_t hero_watch_next(hero_watch *w, int64_t limit_ns, int64_t *numbers, uint64_t numbers_count, void *text, uint64_t text_count);

/* How many events wait in the watch's queue, an overflow among them, or -1
 * with `HERO_WATCH_NOT_OPEN`. */
int64_t hero_watch_count(hero_watch *w);

/* THE BELL, for `hero_poll.h`: a stream that is ready to read while an event
 * waits in the queue and not while it is empty, which a readiness set waits
 * on as it waits on a pipe. BORROWED: the watch's own, the same at every call
 * and closed by `hero_watch_close`; the program never reads, writes or closes
 * it, and a set that holds it keeps its descriptor open until it leaves the
 * set. Darwin and Linux only, a pipe: NULL with `HERO_WATCH_UNSUPPORTED` on
 * Windows, whose readiness waits on sockets alone, and `HERO_WATCH_NOT_OPEN`,
 * or `HERO_WATCH_LIMIT` where no pipe can be made. */
hero_stream *hero_watch_bell_stream(hero_watch *w);

/* The bell as `hero_watch_bell_stream`'s, a datagram socket of this layer
 * bound to the loopback, which a datagram reaches while an event waits:
 * Windows only, NULL with `HERO_WATCH_UNSUPPORTED` elsewhere, where the pipe
 * is the bell. BORROWED as that one is. A datagram a stranger sends it makes
 * it ready while no event waits, so a wait on the watch after a set told it
 * ready may find none. */
hero_net_socket *hero_watch_bell_socket(hero_watch *w);

/* Closes the watch and gives its handle back: the system's watch ended, the
 * layer's thread joined, the queue and the bell given back. Answers 0, or -1
 * with `HERO_WATCH_NOT_OPEN`. A wait another thread is in on it answers
 * `HERO_WATCH_NOT_OPEN`. */
int64_t hero_watch_close(hero_watch *w);

/* The kind of the last call's failure on this thread, `HERO_WATCH_OK` after
 * a call that did not fail, and the platform's own number for it (0 where
 * the layer itself refused). */
int64_t hero_watch_last_error(void);
int64_t hero_watch_last_os_error(void);

#endif
