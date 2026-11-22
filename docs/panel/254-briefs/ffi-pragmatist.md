# Panel 254, ffi-pragmatist

Read `00-shared.md` first, whole (repaired after the completeness critic's first reading,
`docs/panel/254-reports/critic-briefs.md`; its findings 10, 27, 30, 35 and 37 are in this brief). You judge the founding constraint (design.md §1.11, §4.19): everything
comes from C, so what route B does at the C boundary, **written and compiled**, and whether it is sound
there. Your veto is a refusal on soundness or ABI breakage with the program that compels it; a cost is
reported, not vetoed (`00-shared.md` § 2).

**Your copy**: `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/p254-scratch/ffi-pragmatist/`, a
detached worktree at `89f155330`. Build its compiler as `00-shared.md` § 7 says, `export TMPDIR` inside
it, and work under an untracked `p254/` folder there; a Rebel program there names
`packages/rebel/0.1.0/files` of your copy in its `heroes.toml`, as `examples-rebel/gateway/heroes.toml`
does with `path = "../../packages/rebel/0.1.0/files"`. **Your report**:
`/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/lane-p254/docs/panel/254-reports/ffi-pragmatist.md`,
written as you go.

## Q1. A handle handed to another thread whole (S11), measured handle by handle

The proposal lets a handle, or a value holding one, cross only through `spawn`, `wait` or `m.send(@v)`,
its life going with it. Whatever the language spells, **the C must hold when a handle's calls move from
one thread to another**. For each, a C program and, where you can, a Heroes one, run on this Mac, 20
runs plain and 5 under ASan + UBSan and 5 under TSan (`00-shared.md` § 6), accepted on one thread, used
and closed on another, the first thread gone or still running:

1. **A TLS connection** (`runtime/platform/hero_tls.c`, 5,629 lines by `wc -l`): on Darwin its arm is
   Network.framework's (*"its arm (THE APPLE ARM) waits on that queue's blocks with a semaphore"*,
   lines 15-26); a `tls.accept` connection (`packages/rebel/0.1.0/files/tls.hero:379`, a `net.Conn`
   whose `stream` is an `io.Stream`, `net.hero:143-149`) read and written on a second thread after the
   hand-over, with its deadline, `tls.version_of` and `tls.protocol_of` asked there. A certificate for
   the test: what `packages/rebel/0.1.0/files/tls` and its tests already use (find it; do not make a
   network call). The Linux arm (OpenSSL by `dlopen`) is unrun unless Docker answers.
2. **A plain socket stream** (`hero_stream_net.c`), **a listener** and **a UDP socket**
   (`hero_net.c`'s table takes any socket by `hero_net_find`, line 520; Rebel exposes only
   `accept_number`).
3. **A SQLite connection**: `sqlite3_threadsafe()` on this Mac's library, and a connection used by one
   thread after another (moved, never at once), as the owner thread of § 8 would hand it at start.
4. **A poll set**, whose header says *"ONE CALL AT A TIME ON A SET"* and *"a program of the language
   cannot ask it: a thread is handed an `int64_t`, and no number names a set"*
   (`runtime/hero_poll.h`, its opening comment): that premise becomes false under S11; say what holds
   and what must change.
5. **The runtime's side**: the handle set is one entry per address, process-wide under one lock
   (`runtime/parts/alloc.c:1155-1198`); a handle's life moving between threads; a handle left inside a
   mailbox when its last copy goes (what is told at `main`'s end); a handle inside a value sent without
   `@` (what the checker must refuse, S11).

6. **`spawn(f, @x)`** (S11 as repaired, the critic's finding 30: the first text let `spawn(f, x)` leave
   the spawner a live copy): measure what two threads in one SQLite connection and in one poll set do
   at once, so the record says what the `@` prevents.

Say which handles may cross, which may not, and what a binding must say for each (a C library whose
objects are tied to a thread, a terminal or an ODBC connection among those to ask).

## Q2. The C boundary of route B

1. **Spelling B's veto, re-measured**: panel 251's ffi-pragmatist vetoed built-ins `wait`, `send`,
   `close` because an `extern` member has one name for Heroes and C. Today **30 `extern` members are
   named `close`** and 0 `send`, `wait`, `receive` (`00-shared.md` § 5 item 9), **of which 26 are copies
   inside `docs/panel/178-reports/` and 4 are live** (3 under `tests/golden/check/`, 1 under
   `tests/golden/run/`). Does the veto still hold, and does spelling M leave every one of the 30 bindable? Build
   `tests/golden/run/fixedbugs-396-posix-fills-its-buffers.hero` (its `close` member, the program panel
   251's ffi-pragmatist predicted *builds unchanged under M and cannot under B*) and any other of the 30
   under the proposal's names, as far as your copy can.
2. **`(thread T)` and `(mailbox T)` at the boundary**: in an `extern` signature, a callback's parameter,
   a group's record field; `==` on a mailbox (§ 7 compares a `ptr` *"as an address"*; the address
   sentence now names a mailbox); a mailbox as a map key; what clang sees for each.
3. **Where the stop lives** (`00-shared.md` § 5 item 7): what a program that calls `receive` links,
   on this Mac by `nm`, and on Windows by reading (`hero_net.c` links `ws2_32` by `#pragma comment`).
4. **The queue's memory and 7.13**: through `parts/alloc.c`, the single point (`runtime/platform/
   hero_layer_alloc.h:21-24`), and which thread allocates and frees each block.

## Q3. The conservative route A, priced (`00-shared.md` § 5, last paragraph)

Enough that the synthesis can record it: a platform member `runtime/platform/hero_mail.c` and header
`runtime/hero_mail.h` (a queue of bytes with a kind, a handle by slot and life and tag 15, a number for
another thread, a send, a receive bounded in time answering `timeout`, `closed`, `stopped`, a close),
and `hero_stream_number` / `hero_stream_of` beside `hero_stream.c`'s table; Rebel's face in
`threads.hero` over it. **Its C lines** (`wc -l`), **its Rebel lines**, its TSan and ASan stress
(the compiler-engineer's Q2.3 shapes, in bytes), what it does to `hero_handle_tag.h` (the last tag), and
what it moves in the runtime's key (`selfhost/cli/runtime_key.hero:55-63`: every file `runtime.c`
names; a new member moves the key, so every build's runtime object is rebuilt once: measure that once).

**And route A0** (`00-shared.md` § 5, *Routes beside the proposal*): Rebel's `ipc` channel by name
between threads of one process, which the critic ran on task t3's shape (`p254-scratch/critic/p254/a0/`,
5 of 5 plain, 3 of 3 sanitized; do not run inside that folder, copy what you need into yours): price its
gateway shape beside A's, with a `poll` set serving the channels together, the keeper on a thread of its
own, and TSan, none of which the critic ran. **The Windows arm of A** (`00-shared.md` § 5 question 13):
written, its run owed. **The stop's link by `nm`** is asked of the compiler-engineer too (its Q2.4), on
purpose, as a cross-check.

## Q4. The gateway-shaped program, route A, and the TLS hand-over in both (`00-shared.md` § 8)

Route A's program is yours: built in Heroes over your Q3 member, correctness under the sanitizers and
speed against one thread as § 8 says, with TLS connections accepted on one thread and served on N. Route
B's program is the compiler-engineer's; **the TLS hand-over is yours in both** (Q1.1), and you give the
compiler-engineer nothing: each seat builds its own.

## Q5. The rows

For each row S1 to S14, from the C side: adopt, amend (the exact spec text), or refuse, with what you
ran. **Whether you read the author's instruction of 2026-11-22 as the ruling on the need (Principle
0)**, in one line. Your verdict on B, and on A as the conservative route.

## Your output

`.claude/agents/ffi-pragmatist.md`'s structure, and `00-shared.md` § 10: the C each binding needs,
compiled or shown why it cannot be; every program named by its path in your copy so the critic can
re-run it.
