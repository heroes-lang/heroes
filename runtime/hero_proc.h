/* SPDX-FileCopyrightText: 2026 Giuseppe Arici
 * SPDX-License-Identifier: Apache-2.0
 *
 * With the Heroes runtime exception (LICENSE-RUNTIME-EXCEPTION), as
 * `heroes_runtime.h`.
 *
 * hero_proc.h — the platform layer's child processes: a process a program
 * holds as a handle, started, fed, read, waited for and stopped
 * (M-rebel-library; spec § 13).
 *
 * A HEADER OF THE PLATFORM LAYER, AND NOT OF THE RUNTIME, as `hero_net.h`
 * is: its functions are defined in `platform/hero_proc.c`, a member of the
 * archive on every program's link line, which the linker takes only into a
 * program that calls one; a group binds them by naming this header,
 * `extern "hero_proc.h"`, with no `link` word and no flag. The member calls
 * `hero_stream.h`'s, so a program that takes it takes the streams too.
 *
 * WHAT `hero_os.h` ALREADY HAS, AND WHAT THIS ADDS: `hero_run_go` runs a
 * program to its end and answers its exit code. Here the process is LIVE: the
 * call that starts it returns at once, and the program writes to it, reads
 * from it, asks whether it has ended, waits for it or stops it.
 *
 * WHY C OF THE PROJECT'S OWN, AND NOT A BINDING OF THE PLATFORM'S: POSIX
 * starts a process from an array of words and names it by a `pid_t`, which
 * the system hands to another process once this one is waited for; Windows
 * starts it from ONE line the child's C runtime cuts back into words, and
 * names it by a `HANDLE`. How a process ended is a status word read through
 * macros on one and an exit code on the other. So no platform type reaches
 * this header: a group sees `int64_t`, `uint64_t`, `void *`, `const char *`
 * and the two handles below.
 *
 * NO CONDITIONAL BUT THE GUARD (held by the net's `runtime` suite): every
 * platform's arm of the member is compiled against these prototypes.
 *
 * EVERY FAILURE IS A VALUE, NEVER A FAULT: a function handing out a handle
 * answers NULL, one answering a number -1, and `hero_proc_last_error()` then
 * names the kind. A process is a handle the layer issues and checks, never an
 * address it reads through, so one released already, NULL, or one this layer
 * never issued is `HERO_PROC_NOT_OPEN`, never a read of freed memory, never
 * another process that took its number, and never a signal sent to a process
 * the system gave the same id later. A handle of another member of the
 * layer, a stream's or a socket's, is one this layer never issued,
 * `HERO_PROC_NOT_OPEN` too, and never one of its slots: each table's handles
 * carry a tag of their own since 2026-11-16 (`platform/hero_handle_tag.h`).
 *
 * THE CHILD IS HANDED ITS THREE STREAMS AND NOTHING ELSE OF THE LAYER'S:
 * every descriptor the layer opens is close-on-exec and every `HANDLE` not
 * inheritable (`hero_stream.h`), and the child's three are copied into it by
 * this member's own act. On Windows, on Darwin and on Linux with glibc 2.34
 * or later it inherits nothing else at all; on another C library a
 * descriptor some C library of the program opened without close-on-exec is
 * inherited, as every child there inherits it.
 */

#ifndef HERO_PROC_H
#define HERO_PROC_H

#include <stdint.h>

#include "hero_stream.h"

/* A process this layer started and the program holds: a group declares it as
 * a handle, `record Proc tag hero_proc`, which `hero_proc_release` gives
 * back. */
typedef struct hero_proc hero_proc;

/* The kinds of failure, the same numbers on every platform, and where
 * `hero_stream.h` has the kind, its number; the platform's own number for the
 * last failure is `hero_proc_last_os_error()`. */
#define HERO_PROC_OK 0
/* An argument the layer refuses: no program, a list that does not end with a
 * zero byte, a variable that is not `NAME=value`, text that is not UTF-8
 * (Windows), a kind that is none of those below, or a pipe asked of a stream
 * that is none, or was taken already. */
#define HERO_PROC_INVALID 1
/* The process is none this layer holds: released already, or never one. Also
 * a stream handed to `hero_proc_start` that is no open stream. */
#define HERO_PROC_NOT_OPEN 2
/* The program is not there: no file of that path, or of that name in any
 * folder the search reads. Told by the call that starts it, on every
 * platform, never by a child that ends quietly. */
#define HERO_PROC_NOT_FOUND 3
/* The system refused: the file is not this user's to run, it is a folder, or
 * the process is not this user's to stop. */
#define HERO_PROC_DENIED 5
/* The working folder is none the child can start in: not there, a file, or
 * not this user's to enter. The platform's number says which. */
#define HERO_PROC_NOT_A_DIRECTORY 6
/* The process or the machine has no process, descriptor, handle or memory
 * left; the layer holds as many processes as it can (4,096); or a list is
 * longer than the layer or the system takes. */
#define HERO_PROC_LIMIT 8
/* The platform, or the stream, has no such thing: a request to end on
 * Windows (`hero_proc_terminate`), or a stream that stands on no descriptor
 * handed to a child. */
#define HERO_PROC_UNSUPPORTED 11
#define HERO_PROC_OTHER 12
/* The process has not ended, where the call answers only once it has
 * (`hero_proc_cpu`). */
#define HERO_PROC_RUNNING 13
/* The file is there and the system does not run it as a program: a script
 * with no `#!` line, another machine's binary; on Windows also a batch file
 * (`.bat`, `.cmd`), which the layer refuses itself, since `cmd.exe` runs it
 * and reads its line by rules no quoting here can meet. */
#define HERO_PROC_NOT_A_PROGRAM 14
/* The process has ended, where the call needs one that has not: a group is
 * named by a process of it that runs or is stopped (`hero_proc_signal`,
 * `hero_proc_give_terminal`, a start joining a group). */
#define HERO_PROC_ENDED 15

/* What each of the child's input, output and error is, for
 * `hero_proc_start`. */
/* The parent's own: the child reads what this process reads, or writes where
 * it writes. Where this process was started without that stream, the child
 * has none either. */
#define HERO_PROC_STDIO_PARENT 0
/* Nothing: the null device, which reads as empty and takes what is written. */
#define HERO_PROC_STDIO_NOTHING 1
/* A new pipe, whose other end the program takes as a stream
 * (`hero_proc_pipe`). */
#define HERO_PROC_STDIO_PIPE 2
/* A stream the program holds, handed beside the kind: a file, a pipe's end,
 * one of its own three. The program keeps its stream, and closes it itself. */
#define HERO_PROC_STDIO_STREAM 3

/* What the child's environment is, for `hero_proc_start`. */
/* This process's own, as it stands when the child starts. */
#define HERO_PROC_ENV_PARENT 0
/* Exactly the list handed, and no variable of this process's. */
#define HERO_PROC_ENV_GIVEN 1

/* Which process group the child starts in, for `hero_proc_start` (feature
 * 2670): what a shell's job control stands on. */
/* This process's own, as every child before it: a Ctrl-C at the terminal
 * reaches the child and this process both. */
#define HERO_PROC_GROUP_PARENT 0
/* A group of its own, the child its leader and its number the child's own: a
 * job's first program, which the terminal can be handed to and a signal can
 * reach alone. */
#define HERO_PROC_GROUP_NEW 1
/* The group `leader` stands in, read when the child starts: a job's later
 * programs join its first. `leader` is a process this layer started and the
 * program holds, which has not ended and does not stand in this process's
 * own group. */
#define HERO_PROC_GROUP_JOIN 2

/* Which of the child's three streams, for `hero_proc_pipe`. */
#define HERO_PROC_INPUT 0
#define HERO_PROC_OUTPUT 1
#define HERO_PROC_ERROR 2

/* Starts `program` and answers the handle of the process, which runs beside
 * this one from then on; or NULL, and then no process was started.
 *
 * THE PROGRAM: a path where it holds a separator (`/`, or `\` on Windows),
 * read in this process's folder where it is relative; else a name, looked
 * for in the folders of THIS process's `PATH`, in their order, an empty
 * entry being this process's folder (`/usr/bin:/bin` where `PATH` is unset).
 * On Windows the search is the system's own: this program's folder, this
 * process's folder and the system's folders, then `PATH`, and a name with no
 * extension is tried as `.exe`. One that is not there is
 * `HERO_PROC_NOT_FOUND`. The child's first word is `program` as written.
 *
 * THE ARGUMENTS, WHICH NEED NO QUOTING: `arguments` is `arguments_count`
 * bytes holding each argument followed by one zero byte, in order, so an
 * argument is any bytes but a zero, an empty one included, and no shell
 * reads them. A count of 0 is no arguments; any other list must end with a
 * zero byte. On POSIX each reaches the child as its bytes. On Windows a
 * child is handed one line, which the layer writes by the rule
 * `CommandLineToArgvW` and the C runtime undo, so a child that reads its
 * line that way gets each argument back whole; the arguments must be UTF-8.
 * At most 4,096 arguments.
 *
 * THE ENVIRONMENT: `HERO_PROC_ENV_PARENT`, and `environment` is not read; or
 * `HERO_PROC_ENV_GIVEN`, and it is `environment_count` bytes holding each
 * variable as `NAME=value` followed by one zero byte: a `=` after the first
 * byte, the value any bytes but a zero, an empty one included. A count of 0
 * is a child with no variable. The search for the program reads this
 * process's `PATH` either way. At most 4,096 variables.
 *
 * THE FOLDER: the child's working folder, or "" for this process's own. This
 * process's folder never changes. One the child cannot start in is
 * `HERO_PROC_NOT_A_DIRECTORY`.
 *
 * THE THREE STREAMS: each kind one of `HERO_PROC_STDIO_*`, and the stream
 * beside it read only for `HERO_PROC_STDIO_STREAM`. A stream that stands on
 * no descriptor (`hero_stream_make`'s) is `HERO_PROC_UNSUPPORTED`.
 *
 * SIGNALS (POSIX): the child starts with the calling thread's signal mask,
 * with every signal this process handles at its default and those it ignores
 * still ignored, but SIGPIPE, which it starts with at its default whatever
 * this process does with it.
 *
 * THE GROUP (POSIX; feature 2670): `group_kind` one of `HERO_PROC_GROUP_*`,
 * and `leader` NULL for all but `HERO_PROC_GROUP_JOIN`, where it names the
 * process whose group the child joins. The group is set in the child before
 * it becomes the program (a spawn's attribute, or `setpgid` after the fork),
 * so it stands when this call returns. With `HERO_PROC_GROUP_PARENT` the child
 * stays in this process's group, so a Ctrl-C at the terminal reaches both.
 * A `leader` that has ended is `HERO_PROC_ENDED`, one in this process's own
 * group or any other mismatch of the two `HERO_PROC_INVALID`, and on Windows,
 * where a console program has no such group, every kind but the parent's is
 * `HERO_PROC_UNSUPPORTED`. The leader's group is read under the table's lock
 * while nothing has taken the leader's status, so its number is its group's
 * (`hero_proc_signal` says why); a leader waited for by another thread while
 * this call runs is the caller's race. */
hero_proc *hero_proc_start(const char *program,
                           const void *arguments, uint64_t arguments_count,
                           int64_t environment_kind,
                           const void *environment, uint64_t environment_count,
                           const char *folder,
                           int64_t input_kind, hero_stream *input,
                           int64_t output_kind, hero_stream *output,
                           int64_t error_kind, hero_stream *error,
                           int64_t group_kind, hero_proc *leader);

/* The program's end of the pipe that is the child's `which`, one of
 * `HERO_PROC_INPUT`, `HERO_PROC_OUTPUT` and `HERO_PROC_ERROR`: a stream to
 * write the child's input to, or to read its output or error from. TAKEN
 * ONCE: from this call the stream is the program's, given back by its own
 * `hero_stream_close`, and closing the one written to is how the child reads
 * the end of its input. NULL with `HERO_PROC_INVALID` where that stream was
 * not started as a pipe, or its end was taken already. An end never taken is
 * closed by `hero_proc_release`. */
hero_stream *hero_proc_pipe(hero_proc *p, int64_t which);

/* Waits for the process to end and says how, in two numbers: `*code` its
 * exit code and `*by_signal` 0 where it ended itself; `*code` -1 and
 * `*by_signal` the signal's number where a signal ended it (POSIX). Answers
 * 0, or -1 with both left at -1 and 0. ASKED AGAIN IT ANSWERS THE SAME, at
 * once: the answer is kept with the handle until it is released.
 *
 * On Windows there are no signals: `*by_signal` is always 0 and `*code` the
 * exit code, 0 to 4,294,967,295, which for a process `hero_proc_kill` ended
 * is 1 and for one that crashed the exception's code, above 3,221,225,471.
 *
 * It fails `HERO_PROC_OTHER` where the system no longer knows the child:
 * something else in the process waited for it first (POSIX's `ECHILD`). */
int64_t hero_proc_wait(hero_proc *p, int64_t *code, int64_t *by_signal);

/* 1 where the process has ended, 0 where it still runs, without waiting; or
 * -1. Once it answers 1, `hero_proc_wait` answers at once. A stopped process
 * still runs here. */
int64_t hero_proc_ended(hero_proc *p);

/* WHERE THE PROCESS STANDS, A STOP TOLD (feature 2670): 0 it runs, 1 it has
 * ended (`hero_proc_wait` then answers at once), 2 a signal stopped it,
 * `*stop_signal` that signal's number (0 for the other answers); or -1.
 *
 * `waits` 0 never waits, and sees a stop, a going on and an end: a process seen
 * stopped answers 2 until it is seen to go on or to end. `waits` 1 waits until
 * the process ends or stops, so it never answers 0; a stop it was seen in
 * before this call is none of the two, and the call waits past it for the
 * next, so a caller sends SIGCONT first to wait for a stopped process again.
 * The wait is `waitid` with `WNOWAIT` outside the table's lock, as
 * `hero_proc_wait`'s, and what it saw is taken under it.
 *
 * `HERO_PROC_INVALID` for a `waits` that is neither, and `HERO_PROC_OTHER`
 * where something else in the process waited for it first. On Windows no
 * process of this layer's stops: `waits` 0 answers 0 or 1, and `waits` 1
 * waits for the end. */
int64_t hero_proc_state(hero_proc *p, int64_t waits, int64_t *stop_signal);

/* A WAIT A COUNTED SIGNAL ENDS (feature 2690): `hero_proc_state` with
 * `waits` 1, ended also where a delivery of a signal the runtime's door
 * counts (`hero_os.h`) is not taken, or `limit_ns` passes. Answers 1 it has
 * ended, 2 a signal stopped it in this call, `*stop_signal` that signal; 3 a
 * counted signal came, and at once while one stays untaken, so the caller
 * takes it (`hero_signal_take`) before it waits again; 0 `limit_ns` passed;
 * or -1 as `hero_proc_state` fails, and `HERO_PROC_INVALID` for a `limit_ns`
 * below -1. With 3 and 0 `*stop_signal` is the signal it was last seen
 * stopped by, 0 where it runs. `limit_ns` -1 waits as long as it takes, 0
 * looks once and never waits.
 *
 * A stop the process was seen in before the call is waited past, as
 * `hero_proc_state`'s is; a stop and an end are put before a signal that
 * came with them. POSIX: the wait polls the door's descriptor beside a pipe
 * one helper thread writes when the process ends or stops, so nothing is
 * asked at intervals; where the system gives no pipe or no thread it looks
 * again every 50 ms, and still ends at once on a signal. Windows: no process
 * stops, and the wait is on the process and the door's event together, which
 * the console's handler sets for Ctrl-C and Ctrl-Break, the one signal
 * counted there. */
int64_t hero_proc_heed(hero_proc *p, int64_t limit_ns, int64_t *stop_signal);

/* Asks the process to end: SIGTERM, which a program may catch to finish its
 * work, or ignore. Answers 0 once it is asked, also where the process has
 * ended already, when nothing is sent; or -1.
 *
 * ON WINDOWS THERE IS NO SUCH REQUEST, and the door does not pretend one:
 * it answers -1 with `HERO_PROC_UNSUPPORTED` and the process is not touched
 * (0 where it has ended already). The requests Windows has reach a console's
 * whole group or a window, never one process a program names. A caller that
 * must stop it there calls `hero_proc_kill`. */
int64_t hero_proc_terminate(hero_proc *p);

/* Ends the process at once: SIGKILL, or `TerminateProcess` with the exit
 * code 1, neither of which the process can catch, so it finishes nothing.
 * Answers 0 once the system took it, also where the process has ended
 * already; or -1. The process has ended when `hero_proc_wait` returns, which
 * may be a moment later. Processes it started itself are not ended. */
int64_t hero_proc_kill(hero_proc *p);

/* A SIGNAL TO THE PROCESS, OR TO ITS GROUP (POSIX; feature 2670): `sig`
 * this platform's number for it (`hero_os.h`'s `hero_signal_number`), 1 or
 * above and below the platform's count; `group` 0 sends it to the process
 * alone, 1 to every process of the group it stands in, read from the system
 * at this call. Answers 0 once the system took it; a process seen to have
 * ended is sent nothing, 0 with `group` 0, as `hero_proc_terminate`, and -1
 * with `HERO_PROC_ENDED` with `group` 1, whose group it no longer names.
 *
 * NEVER A GROUP NUMBER THE SYSTEM MAY HAVE GIVEN AWAY: the group is read and
 * signalled under the table's lock, while nothing has taken the process's
 * status, so the process is a member of that group and its number names it
 * (POSIX gives no new process a number while a group of that number lives).
 * A process that moves itself to another group at that very moment is the
 * one case outside it.
 *
 * -1 with `HERO_PROC_INVALID` for a `sig` or a `group` out of range, and for
 * a group that is this process's own, which a signal to it would reach too.
 * On Windows `HERO_PROC_UNSUPPORTED`, the process untouched: a console's
 * events reach its whole group, never one process a program names. */
int64_t hero_proc_signal(hero_proc *p, int64_t sig, int64_t group);

/* THE TERMINAL'S FOREGROUND (POSIX; feature 2670): `terminal` a stream on
 * this process's controlling terminal, a shell's input. The group in front
 * is the one Ctrl-C, Ctrl-Z and the terminal's reads reach.
 *
 * `hero_proc_give_terminal` puts the group `p` stands in in front, read as
 * `hero_proc_signal` reads it; `hero_proc_take_terminal` puts this process's
 * own group in front again. Each holds SIGTTOU on the calling thread across
 * the call, so a process that is not in front sets it rather than being
 * stopped (POSIX's `tcsetpgrp`). `hero_proc_holds_terminal` answers 1 where
 * this process's group is in front, 0 where another is.
 *
 * -1 with `HERO_PROC_UNSUPPORTED` where the stream is no terminal, is not this
 * process's controlling one, or stands on no descriptor; `HERO_PROC_ENDED` and
 * `HERO_PROC_INVALID` as `hero_proc_signal` answers them; the stream's own
 * kind where it is no open stream. On Windows `HERO_PROC_UNSUPPORTED`: a
 * console has no group in front for a program to set. */
int64_t hero_proc_give_terminal(hero_stream *terminal, hero_proc *p);
int64_t hero_proc_take_terminal(hero_stream *terminal);
int64_t hero_proc_holds_terminal(hero_stream *terminal);

/* The system's own number for the process, its pid, or -1. A number to show
 * or to log: the system gives it to another process once this one has ended
 * and been waited for, so the handle, never the number, names the process. */
int64_t hero_proc_id(hero_proc *p);

/* The CPU time the process used, once it has ended: `*user_ns` in its own
 * code and `*system_ns` in the system's on its behalf, in nanoseconds.
 * Answers 0, or -1 with both left at -1: `HERO_PROC_RUNNING` where it has
 * not ended. On POSIX the time of the children it waited for is counted
 * with it; on Windows its own alone. */
int64_t hero_proc_cpu(hero_proc *p, int64_t *user_ns, int64_t *system_ns);

/* Gives the handle back, whatever is answered: 0, or -1 with
 * `HERO_PROC_NOT_OPEN` where it named no process. The ends of its pipes the
 * program never took are closed.
 *
 * A PROCESS THAT STILL RUNS KEEPS RUNNING, AND LEAVES NO ZOMBIE: on POSIX a
 * thread of the layer's own, one per such process, waits for it and takes its
 * status when it ends (where the system gives no thread, the layer looks for
 * it at its next `hero_proc_start` or `hero_proc_release`); on Windows the
 * process's object goes with its last handle. A call another thread is still
 * in on the handle answers `HERO_PROC_NOT_OPEN` when it returns. */
int64_t hero_proc_release(hero_proc *p);

/* The kind of the last call's failure on this thread, `HERO_PROC_OK` after a
 * call that did not fail, and the platform's own number for it (0 where the
 * layer itself refused). */
int64_t hero_proc_last_error(void);
int64_t hero_proc_last_os_error(void);

#endif
