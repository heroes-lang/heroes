# Panel 256, the ffi-pragmatist

Written 2026-11-22 from 22:11, as it went. Copy:
`.claude/worktrees/p256-scratch/ffi-pragmatist/` (detached at `8aca3cc432a6007daaf416ecf3b1b8564472fa09`,
`git log -1 --format=%H`). Every probe lives under that copy's `ffi256/`; TMPDIR pointed at
`ffi256/tmp`. Contract read in the copy: `.claude/agents/ffi-pragmatist.md` and CLAUDE.md § Precedence
(the 2026-11-22 amendment present). Briefs read whole: `00-shared.md`, `ffi-pragmatist.md`,
`256-reports/critic-briefs.md`. Load at start: `uptime` 23.49 (a loaded machine: no duration below is a
duration of the code alone; latencies are reported as distributions under that load, said so).

## Status

- 22:11 compiler build started (seed, then stage 2), log `ffi256/build.log`; finished `heroes 0.6.0` /
  `sources 031aaabb358218fd`, exit 0 (the coordinator's).
- Q1 items 1, 2, 4b built and stressed on Darwin and in Linux arm64 (below).
- Q2 item 6 built and run (plain, ASan+UBSan, TSan); Q1 items 4c, 4 (AppKit, GLib), 3 (priced), 5, Q2
  item 7 (priced) after. Ended about 23:25; no process of my copy and no `p256` container left standing
  (`pgrep -fl`, `docker ps`). Untouched: the Win32 halves (box not started), item 3's Rebel server.

## Q1 items 1, 2 and 4b: three ways a mailbox joins a set, built and stressed

**The program**: `ffi256/q1/wait_routes.c` (one file, Darwin `kqueue` and Linux `epoll`), a model of
`runtime/parts/mail.c`'s queue at the frozen commit: the note allocated outside the lock, enqueued and
the condition signalled under it (`mail.c:415-435`), a receive taking the head under it
(`mail.c:493-530`). Three routes on it:

- **watch** (P1 item 1): the layer registers `(ring, ctx)` on the queue under the queue's lock; a send
  that takes the queue from empty to one value pins the watch (an atomic count) under the lock, drops the
  lock, then rings: `EVFILT_USER`'s `NOTE_TRIGGER` on Darwin, an `eventfd` write on Linux. The layer asks
  the queue's ready predicate before it blocks, so the entry is level. Unwatching waits for the pins to
  reach 0, so no ring reaches a set the layer has closed.
- **bell** (critic 12, `fs/watch`'s own shape, `hero_watch.c:606-646`): the queue owns a pipe (an
  `eventfd` on Linux as a variant), rung **under the queue's lock** when it goes from empty to one value,
  drained under the lock when a receive empties it; the set watches the read end as any stream. No call
  from the runtime into the layer at all: the runtime calls `write(2)` and `read(2)`.
- **waker** (P1 item 4, 2693's sketch): a user event in the set that the PROGRAM rings after its send;
  used right, forgotten (sender 0 never rings) and early (the ring before the send).

Loop styles: `drain`, `receive(within_ms: 0)` until empty at each wake; `one`, one value per wake.
A **lost wake** is `slept_past_a_value`: a 1 s wait that timed out while the queue held a value.
Each binary built three ways (`-std=c11 -Wall -Wextra -Werror`, plain `-O2`; `-fsanitize=address,undefined
-fno-sanitize-recover=all`; `-fsanitize=thread`), clang accepting all three with no warning: Apple clang
21.0.0 on this Mac, Debian clang 22.1.8 in `heroes-linux-arm64:gtk4-ui` (`uname -m` aarch64; container
`p256-ffi-matrix`, `--rm`, gone after: `docker ps` lists no `p256` name). Runner: `run_matrix.sh`
(Darwin), `run_linux.sh` (Linux); results `darwin.txt` (36 runs), `linux.txt` (42 runs), every run
under `timeout -s KILL 240`, output through `head -c 4000`.

**What they read** (`grep -c "exit=0"` / `"exit=1"`, and `grep -ic "sanitizer\|WARNING"` over both
files: **0**):

| route | senders x sends | loop | Darwin plain / ASan / TSan | Linux plain / ASan / TSan (pipe, eventfd) |
|---|---|---|---|---|
| watch | 4 x 100,000 and 16 x 100,000 | drain | every value taken, **0 lost wakes**, 1 wake | the same |
| watch | 4 x 100,000 | one | every value taken, 0 lost wakes | the same |
| bell | 4 and 16 x 100,000 | drain | every value taken, **0 lost wakes**, 1 to 2 rings in all | the same, pipe and eventfd |
| bell | 4 x 100,000 | one | every value taken, 0 lost wakes | the same |
| waker, right | 4 x 100,000 | drain | every value taken, 0 lost wakes, 400,000 rings | the same |
| waker, right | 4 x 100,000 | **one** | **320,799 / 365,859 / 358,716 left unseen**, 3 waits slept past a value | **392,574 / 389,533 / 373,860** |
| waker, forget / early | 4 x 100,000 | drain | every value taken (another sender's ring covers) | the same |
| waker, forget / early | 4 x 100,000 | one | 326,582 to 397,379 left unseen | 378,133 to 395,469 |

So under stress **the watch and the bell never lose a wake**, on either system, under either sanitizer,
and neither does a waker rung right by a loop that drains. **A waker loses values even when every send
rings**, as soon as the loop takes one value per wake: the system tells rings together (`EVFILT_USER`
and `eventfd` both coalesce), so 400,000 rings gave 7,426 to 79,201 wakes. The watch and the bell, read
one value per wake, wake 400,000 times: the entry stays ready while the queue holds a value.

**Item 2, the waker's two mistakes, one sender** (`GAP_US=500` between sends, 2,000 sends, `drain`,
`darwin_sparse.txt`): the **forgotten** ring left **all 2,000 values unseen** for the whole run, three
1 s waits slept past a value, plain and TSan; the **early** ring (before the send) woke the loop on an
empty queue 5 times in 2,000 (plain) and 15 (TSan), and each of those values sat until the next send's
ring: max 5.6 ms and 11.0 ms here, and **for ever for the last value of a burst** (an inference: no
later ring exists; not forced by a run). Neither mistake is a crash or a race; both are a value seen late
or never, which no instrument of this tree reports.

**The wake's latency** (one sender, 500 us gaps, so the queue is empty at each send and the number is a
wake's, not a queue's; under load ~23, no duration of the code alone): p50 **17 us** watch, **19 us**
bell, **14 us** waker (plain); 24 / 28 / 23 us under TSan; p99 1 to 4.4 ms, the load's. The flooded
runs' latency columns (4 x 100,000) measure time in a queue the loop is draining, not a wake, and the
16-sender TSan runs' `values_over_1s` are that queueing (`timeouts=0`, one wake), not lost wakes.

**One cost the watch pays and the bell does not, measured.** Darwin's `watch` read one value per wake in
**26.3 s** (ASan 21.3 s, TSan 13.5 s) against the bell's **0.19 s**. `ffi256/q1/kev0.c`: a zero-timeout
`kevent` that finds nothing costs **36 to 63 us** per call on this Mac (two runs: an untriggered
`EVFILT_USER` 44.0 / 38.0, an empty pipe 62.7 / 35.5), and **0.25 to 0.30 us** when an event is ready.
The watch's layer asks the predicate, finds the queue ready, and still polls the kernel with a zero
timeout for the set's other entries; the poll finds nothing because `EVFILT_USER` is edge-told
(`EV_CLEAR`), so it pays the slow path on every wait. The bell's pipe stays readable, so its poll always
finds an event. Linux's `epoll_wait(0)` showed no such cost (200 ms for the same run). A watch could
avoid it by triggering itself again while the predicate holds; that is more code in the layer the bell
does not need.


## Q2 item 6: a real `ptr` crossing a thread through a generic corrupts memory, silently

**The programs** (`ffi256/q2/real/`, checked and built by my stage 2): `cmem.hero` binds `calloc`,
`free`, `memset`, `strlen` and `usleep` at their headers' widths, with Heroes wrappers beside them (an
`extern` is not reachable across modules: `error[extern_across_modules]`, my first attempt);
`record Pointed` holds one `ptr`. `begin<S>(body, setup) -> (thread i64)` is `spawn(body, setup).must()`,
the coordinator's `p7` shape returning the thread instead of waiting.

- `c7r_nongeneric.hero` (the control, `spawn(look, Pointed(p: p))` with no generic): `check` **exit 1**,
  `error[thread_crossing]: ... reaches \`ptr\`, which never crosses`.
- `p7r_read_after_free.hero`: `main` callocs 64 bytes, fills 63 with `A`, starts `look` through
  `begin`, frees the block; the thread reads it 200 ms later. `check` **exit 0**, `build` exit 0.
  Plain: prints **`0`** (the thread read freed memory and answered wrong), exit 0. `--sanitize`
  (ASan+UBSan): **`heap-use-after-free ... READ of size 2 at ... thread T1`** in `strlen`, from
  `h_cmem_length cmem.hero:27`, freed by T0 at `p7r_read_after_free.hero:19`, exit 134.
- `p7w_write_after_free.hero`: the thread **writes** 63 bytes into the block 200 ms after `main` freed
  it; `main` meanwhile callocs a new 64-byte block and fills 10 bytes, then prints `strlen` of ITS OWN
  block after the wait. `check` exit 0, `build` exit 0. Plain: prints **`63`** where `10` is the answer:
  **the other thread's write landed in `main`'s fresh allocation**, exit 0, nothing told. `--sanitize`:
  **`heap-use-after-free ... WRITE of size 63 ... thread T1`** in `memset`, from `h_cmem_fill
  cmem.hero:24` (exit 141 is SIGPIPE from my `head -c 3000` cap on its report; the report is whole up to
  its shadow map). TSan: the emitted units compiled by hand (`clang -fsanitize=thread -I runtime
  build/tu-*/p7wwriteafterfree.c cmem.c library.c runtime/runtime.c`): prints **`63`**, exit 0, **no
  report**. Why TSan is blind here is an inference, unrun: the runtime's shared atomics may order the two
  threads in TSan's eyes. A TSan-green run is no witness for this class.

**So the class corrupts memory**, in a plain build of a program `check` accepts, with no abort and no
message: design.md §1.12, *"A Heroes program must not segfault and must not corrupt memory ... a goal of
the language"*. **That is a soundness veto** on any resolution that leaves a `ptr` or a `cstr` crossing
a thread through a letter: `now`, and any Q2 rule that rules on the `@` of a handle but not on the
`ptr` half of spec § 11's same sentence (*"a `ptr` or a `cstr`, or a value holding one, never
crosses"*). `gc`, `gi` and `gr` each close it on their texts (judged per instance, or refused in the
body); whichever is adopted, the `ptr` half must be in the same rule, with `p7r`/`p7w` as its cases.
A run-time guard in the emitter's crossing walk (critic 14: `hero_cross_put_raw` is written per
instance, `selfhost/emit/crossing.hero:60,77`) would turn the corruption into an abort and is the right
**second** wall; as the only wall it trades a compile error the thesis owes for an abort.

## Q1 item 4c: the Windows stop bell, its socket logic run on POSIX

**Read at the frozen commit**: `hero_poll.c:369-397` binds a UDP socket to `127.0.0.1:0`, never
connects it; `:1294-1305` puts it in every set's array; the array arm (`:1477-1479`) sets `stopped` on
any readiness of the place whose entry is 0, and `:1504` fails the wait `stopped` on that flag alone
(`if (stopped || hero_net_stop_asked() != 0)`). The `kqueue` and `epoll` arms do the same on key 0 (read
at `:1395-1398`, `:1423-1426`), but their bell is a pipe only this process holds (`:400-423`, both
ends `FD_CLOEXEC`), so no other process can ring it.

**Built**: `ffi256/q1c/bell_udp.c`, the same calls (Winsock copies the BSD socket calls; Winsock
itself is **unrun**, the box not started by this sitting). A stranger process (`fork`, its own socket)
sends one datagram to the bell's port. Darwin and Linux arm64 (container `p256-ffi-bell`, `--rm`, gone):

| bell | stranger's datagram makes it readable | the self-ring by `sendto(addr)` | by `send()` |
|---|---|---|---|
| as Windows makes it (bound, unconnected) | **1** on Darwin, **1** on Linux | readable | n/a |
| connected to its own address | **0** on both (dropped by the system) | Darwin **-1, errno 56 "Socket is already connected"**; Linux accepts | readable on both |

So the critic's finding 13 holds for the socket logic on two systems: **any process on the machine that
sends one datagram to that port makes every later `poll.wait` fail `stopped`** on Windows' arm, if
Winsock behaves as BSD sockets do (unrun). **What a repair needs**, each measured here: (1) the bell
connected to its own address, which drops a stranger's datagram; (2) the ring then made with `send()`,
since `hero_poll_hear_stop`'s `sendto` with an address is refused on a connected socket by Darwin and
may be by Winsock (unrun); (3) defence in depth, `:1504` asking `hero_net_stop_asked()` instead of
trusting the bell's readiness, draining a bell that rang with no stop asked. Whether Windows'
`SO_REUSEADDR` lets another process bind the same port and so pass the connected filter is a question,
unrun (`SO_EXCLUSIVEADDRUSE` is its documented answer; not read this session). **Every route that
builds on this mechanism inherits the hole**: P1 item 3's Windows `add_signals`, 2693's waker on
Windows, and `fs/watch`'s own bell (whose comment, `watch.hero:307-309`, admits the stray datagram and
is harmless there because `wait(0)` re-asks the queue). The bell route of item 4b, built on a pipe on
POSIX, does not inherit it; on Windows it would need the same three repairs.

## Q1 item 3: the stop scoped to a server, priced from the code (the server itself unbuilt)

**Unbuilt, said plainly**: the smallest Rebel server of the brief (a counted SIGTERM through
`poll.add_signals`, a closing exchange under a deadline, a second loop serving on) was not written in
this time box. What the code settles:

- **Windows has no counted SIGTERM at all**, before `add_signals` is reached: `hero_net_count_arm`
  (`hero_net.c:1447-1457`) arms SIGINT alone through `SetConsoleCtrlHandler` and answers
  `HERO_NET_UNSUPPORTED` for any other signal (*"SIGTERM, which nothing sends a console program"*).
  The Windows events a graceful server would count are `CTRL_BREAK`, `CTRL_CLOSE`, `CTRL_SHUTDOWN` and a
  service's stop control, none counted today. So P1 item 3's *"Windows' `add_signals` made to work by
  the stop's own loopback mechanism"* needs (a) a counted close/shutdown event, (b) a second loopback
  socket per process (the stop's own must keep meaning *stopped*), rung by the console thread and
  drained by `take_signals`, with item 4c's three repairs; about the size of `hero_poll_bell_made`
  (`:369-397`, 29 lines) plus the ring and the drain, unrun.
- **A call blocked outside any set** (a worker's `recv` on a socket no set holds): under P1 item 3's
  scoped mailbox, nothing in the mailbox reaches it. What ends it today is its own deadline
  (`net.set_deadline`, `net.hero:417`), the peer, or the process stop (`hero_net_nudge_all` wakes every
  waiting call, `hero_net.c:897`). R3 forbids the obvious fix: another thread cannot close the worker's
  socket, since a handle reached on a thread that does not hold it aborts. So under the scoped route a
  worker that may block must carry a deadline, or the server must ask the process stop; under a stop
  token (2660), the token must reach the layer's per-call wait (a condition per waiting call,
  `hero_net.c:44-69`), which means every waiting call of `net`, `tls`, `ipc`, `process` and `fs/watch`
  takes it: an argument on each, and per token a slot (life, flag, list of waiting calls) the token's
  ask walks, as `nudge_all` walks every call today. Priced by reading; unbuilt.

## Q1 item 4: the user interface's wait taking a set, two of three systems run

- **AppKit** (`ffi256/q1ui/appkit_kq.m`, `clang -fobjc-arc -Wall -Werror -framework Cocoa`, accepted;
  headless, activation policy *prohibited*): a `kqueue` holding one `EVFILT_USER`, wrapped in a
  `CFFileDescriptor` whose run loop source is on the main loop's common modes; a thread triggers it at
  200 ms; the callback posts an application-defined `NSEvent`. `nextEventMatchingMask:untilDate:3s
  inMode:NSDefaultRunLoopMode` (the call `hero_ui_appkit.swift:1280` makes) **returned the posted event
  at 206.4, 210.6 and 228.0 ms** in three runs, the callback 6 to 22 ms after the ring. A dispatch
  source was not tried.
- **GTK 4's loop** (`ffi256/q1ui/glib_epoll.c`, in `heroes-linux-arm64:gtk4-ui`, GLib 2.84.4, `clang
  -Wall -Werror`, accepted; container `p256-ffi-glib`, gone): an `epoll` holding an `eventfd`, given to
  `g_unix_fd_add`; a thread writes the `eventfd` at 200 ms. `g_main_context_iteration(NULL, TRUE)` (the
  call `hero_ui_gtk.c:2277` makes) **returned at 206.8, 203.4 and 209.3 ms**, the callback told 1 event.
- **Win32**: unrun, the box not started. From the code: `hero_ui_win32.c:2436` waits with
  `MsgWaitForMultipleObjectsEx(0, NULL, ...)`, so it already takes handles and is given none; a set on
  Windows is a `WSAPoll` array with no handle (`hero_poll.c:62-96`), so P1 item 2's Windows half needs a
  handle a set can signal, `WSAEventSelect` on the set's sockets into one event (sockets only, which is
  what Windows' set holds) or the bell route's loopback socket with item 4c's repairs. Priced, unrun.

So **a set's own descriptor joins two of the three user-interface loops with no thread and no shim**,
the fd being pollable (`kqueue` and `epoll` are). One cost seen in the AppKit run: the callback must
re-enable itself (`CFFileDescriptorEnableCallBacks` is one-shot), a line the layer owns.

## Q1 item 5: bytes a library holds above the socket (2688, 2675)

From what was built, not a run of either defect: the bell and the watch both keep one invariant, *the
entry is ready while the queue holds a value*, **under the lock of whoever owns the queue**. A library
that parses whole messages out of a socket and keeps them (websocket's inbox, nats' unparsed reply)
owns such a queue, and no wait can see inside it: the socket is drained, so a level wait on the socket
is quiet while a message waits above it. So it is **each library's question**, but it need not be
answered by each library's own C: **if a mailbox becomes a set entry (P1 item 1, by the bell route),
a library written in Heroes can keep its parsed messages in a mailbox it adds to the caller's set**, and
2688's and 2675's shape becomes the mailbox's, with no new mechanism. An inference, unbuilt; the
falsifier is a websocket inbox moved into a mailbox and a drained-socket loop that still sees its
message.

## Q2 item 7: `gc`'s `@` on a letter's value that holds no handle

Not compiled: `m.send(@v)` at a type that holds no handle is refused today (`p1_defect`,
`marker_mismatch`, the coordinator's run), and `gc` exists in no compiler of this sitting. What the
emitted C does today, read at `mail.c:415-435`: `hero_mail_send(m, put, sent, &v, where)`, `sent`
being NULL *"for a type that holds none"*. So for an `i64` or a `str` instance there is no C for
`@` to change: the copy is written whole either way, and "emptied" can only be a checker fact (a read
after the send refused), unless `gc` asks the lowering to write something into the cell (a zeroed
`i64`, an empty `str`), which would be a run-time meaning the checker cannot show. **What I would need**:
the compiler-engineer's `gc` build and `--emit-c` of the golden `relay<A>` at `A = i64`, diffed against
today's `m.send(v)` emission; if the diff is empty, `gc`'s `@` is a spelling with no C behind it.

## Verdict

- `verdict`: **veto** on any Q2 resolution that leaves a `ptr` or `cstr` crossing a thread through a
  letter (`now`, or a rule ruling on the handle's `@` alone); **object** to P1 item 1's watch called
  through a pointer, in favour of the bell (descriptor) route; **object** to the waker (P1 item 4) as
  more than a record of the conservative route; **approve** P1 item 2 on `kqueue` and `epoll`.
- `section`: design.md §1.12 (*"A Heroes program must not segfault and must not corrupt memory"*) for
  the veto; §1.11 and §4.19 for the routes (the layer is every package's door; a mechanism a binding can
  use without C is worth more than one it must call correctly).
- `experiment`: `ffi256/q2/real/p7w_write_after_free.hero` (calloc, free, memset, strlen bound at their
  headers' widths; clang accepted, `check` 0, `build` 0): plain prints `63` for `main`'s own 10-byte
  block, ASan `heap-use-after-free WRITE of size 63 ... thread T1`, TSan silent; the non-generic control
  refused `thread_crossing`. `ffi256/q1/wait_routes.c` (clang accepted `-Werror` plain, ASan+UBSan, TSan,
  Darwin and Linux arm64): watch and bell 0 lost wakes in 4 and 16 x 100,000 sends; the waker strands
  320,799 to 397,379 of 400,000 values under a one-per-wake loop even rung right; Darwin's watch pays
  36 to 63 us per empty zero-timeout `kevent`, the bell never. `bell_udp.c`: a stranger's datagram rings
  an unconnected loopback bell on Darwin and Linux, a connected one drops it, and Darwin then refuses
  `sendto` with an address (errno 56).
- `argument`: The class Q2 leaves open is not an abort: a generic hands a second thread a pointer `main`
  frees, and the plain build writes into `main`'s next allocation with exit 0. That is §1.12's one
  absolute, and every arm must carry the `ptr` half of spec § 11's sentence with these two programs as
  cases. For Q1, the bell is the watch's equal on every safety and latency measure taken, keeps R6 by its
  words (the runtime calls `write(2)`, never the layer), is `fs/watch`'s shipped shape, and needs no pin
  protocol. Its cost is a descriptor per watched mailbox, whose life the set's entry must pin.
- `prediction`: **On Windows, one UDP datagram sent by another process to the stop bell's port makes the
  next `poll.wait` of a Rebel program fail `stopped`, and every later one** (hero_poll.c at 8aca3cc43).
  It is falsified by a run on the box where the wait returns normally.
- `condition`: The veto lifts when the adopted text and compiler refuse `p7r` and `p7w` at `check`, with a
  run-time abort in the emitter's crossing walk as a second wall. My objection to the watch turns if a
  program needs more watched mailboxes than descriptors (none in the tree: 148 `(mailbox ` lines in 49
  files, critic 8), or if the Windows bell cannot be repaired (item 4c's three repairs, unrun on Winsock).

## One line per question

- **Q1** (one wait): adopt the **bell route** (the mailbox owns a pipe, an `eventfd` on Linux, rung and
  drained under its own lock, the set's entry pinning the queue; on Windows a loopback socket connected to
  itself and rung with `send()`), plus `ui.add_set` on `kqueue` and `epoll`. **The measurement that would
  change my mind**: a stress run where the bell loses a wake, or a Windows run where a connected loopback
  bell still takes a stranger's datagram.
- **Q2** (a send inside a generic): any rule that judges the **instance**, the `ptr` half included,
  with `p7r` and `p7w` refused at `check` and the emitter's crossing walk aborting as a second wall.
  **The measurement that would change my mind**: none can lift the veto on leaving it open; between
  `gc`, `gi` and `gr`, a `gc` emission at `i64` that differs from `m.send(v)` would tell me `@` means
  something there, and I have not seen one.
