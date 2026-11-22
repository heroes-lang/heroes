# Panel 254, ffi-pragmatist's report

Started 2026-11-22 02:32 CET. Written as I go; the verdict sections at the top are filled in last, the
working log below them is appended as each run ends. Copy: `.claude/worktrees/p254-scratch/ffi-pragmatist/`
(detached at `89f155330`, `git status` clean at 02:32), my files under its untracked `p254/`,
`TMPDIR=<copy>/build/tmp`. Load at 02:32: `uptime` 15.30 11.15 8.54 on 8 cores, so no figure below is a
CPU-bound speed claim; the latency-bound runs carry their load and ratio beside them.

Read first: `00-shared.md`, `ffi-pragmatist.md`, `arms/spec-m.diff`, `254-reports/critic-briefs.md`,
`254-reports/spec-warden.md`.

## Verdict (the seat's structure)

- **verdict**: **veto on S11 as written (soundness); approve route B with S11 amended as below**, which
  lifts the veto. Every handle of the layer I could run crosses a thread soundly when it MOVES (one
  holder at a time); two threads in one C object at once corrupts memory (SQLite killed 20 of 20). S11's
  `@` empties ONE cell, and **a copy of the handle made before the send still reaches C on the sender**:
  measured, 8 of 10 killed, 2 silent races, with the runtime's check passing it (`p254/c/copy_then_send.c`).
  The repair is built and measured: the set records which thread holds each life and the check before C
  refuses any other (copy 10/10 stopped by name, a clean move 10/10 and 0 sanitizer reports, +3.6 ns per
  handle argument). Four further amendments are owed from the C side (S6, S7, the boundary clause, S12's
  link direction) and one gap recorded as owed (a C object tied to its thread).
- **section**: design.md §1.11 and §4.19 (everything comes from C, so the boundary must hold whatever
  crosses); §1.12 (robustness); Part 7.13 (`docs/design.md:4118-4189`, isolated heaps, copying at the
  boundaries, the queue a mailbox); `runtime/hero_net.h:157-164` and `:193-201` (the layer's own stop: a
  recv with bytes there still moves them); `runtime/hero_stream.h:255-259` (a stream's `close` on the
  thread that leaves it last). design.md does not cover a C object a library ties to one thread: **not
  covered, said so**.
- **experiment**: C against the real headers, compiled by clang and run, each 20 plain, 5 ASan+UBSan, 5
  TSan (a deliberate race seen by the same TSan as control): TLS accepted on one thread and served on
  another (`p254/c/tls_handover.c`, Network.framework's arm), socket stream, listener, UDP, panel 246's
  number (`net_handover.c`), SQLite moved and SQLite at once (`sqlite_handover.c`, `sqlite3.h`), a poll
  set moved and at once (`poll_handover.c`), the IPC lock (`ipclock_alive.c`), a TLS stream in a poll
  set (`tls_poll.c`), **a handle copied before a send** (`copy_then_send.c`) and its repair in a copy of
  the runtime (`p254/q11/rt/parts/alloc.c`, `copy_then_send_held.c`, `p254/q11/alive_bench.c`); route A's member `hero_mail.c` stressed (`p254/route-a/mail_stress.c`); and in
  Heroes, route A's gateway with TLS hand-over (`p254/gwa/`), a handle stranded in a mailbox
  (`p254/left/`), A0 (`p254/a0p/`), spelling B's collision (`p254/q2/exit_member.hero`), the boundary
  refusals (`p254/q2/arr_*.hero`), the stop's link (`p254/nm/`). **clang accepted every one; every
  run is in the log below with its count.**
- **argument** (≤120 words): A moved handle is sound: the set is process-wide and a stream's last
  `close` runs on whichever thread leaves it, measured for TLS, sockets, SQLite and poll sets. Sharing is
  not: this Mac's SQLite is `threadsafe=2`, two threads in one connection crash 20 of 20. S11's `@`
  empties one cell, but Heroes copies handles freely and only an END makes a copy detectable; a send
  ends nothing, so a copy kept on the sender reaches C beside the receiver (8 of 10 killed). The runtime,
  not the checker, must say which thread holds a life; built, it costs 3.6 ns per handle argument. On
  Darwin a TLS stream cannot `poll`, so threads are a TLS server's only route: the need is real.
- **prediction**: § Predictions below, P1 to P7, each with when it becomes checkable.
- **condition**: **the veto on S11 lifts** when the landed runtime refuses, before C, a handle argument
  on a thread that does not hold its life (`p254/q11/rt/parts/alloc.c`'s shape or an equal one, run on
  `p254/c/copy_then_send_held.c copy`: stopped by name, 10 of 10), with `wait` and `spawn(f, @x)` (if
  kept) moving the holder as `send` and `receive` do. I would **veto** again if the landed route let a
  handle, or a value holding one, reach two threads at once by any path, `@` or a copy (the SQLite
  program above is the compelling case), or let `(mailbox T)`
  or `(thread T)` stand in an `extern` signature, a callback's parameter or a group record's field, or
  put `wait`, `send`, `receive` or `close` in the built-in table (B's collision, measured). I would move
  to **object** if the landed S7 strands queued handles at the stop (the compiler-engineer's 24 of 32)
  or if the runtime came to call `hero_net.h` (every program would link it, and `ws2_32` on Windows).

**Principle 0, in one line**: I read the author's instruction of 2026-11-22 as the ruling on the need;
and the boundary measures the same need: on this Mac a TLS server cannot `poll` its connections
(`HERO_POLL_UNSUPPORTED`), so handing a connection to a thread is its only route.

## The rows, from the C side

| row | verdict | the exact text, where amended, and what stands behind it |
|---|---|---|
| S1 `spawn` | **amend** | *"... gives a `(thread T)?`, `T` being `f`'s answer, failing `limit` where the system or the runtime has no thread to give"*. Today the runtime's table of 256 is a `hero_panic` (`runtime/parts/spawn.c:522`, read): a server starting a thread per connection aborts where it could be told. With the compiler-engineer. |
| S2 `spawn_sized` | **adopt** | below 0 already panics at the door (`threads/hero_os.hero`'s *"below 0 panics"*); S1's widened `limit` reaches it by *"does the same"*. |
| S3 `t.wait()` | **adopt**, with the spec-warden's W-a (no null) | a thread is the runtime's table index, never a C pointer; it never reaches C (S9/S10 clause). |
| S4 `mailbox()` | **adopt**, with W-e | no C side. |
| S5 life | **adopt (a value)**, with W-f | the queue is the runtime's blocks: a cycle leaks without a word (the compiler-engineer built it), never corrupts; W-f refuses the self-holding types at compile time. A handle stranded in a dropped queue is told at `main`'s end by the process-wide set, measured on route A (`p254/left/`: *"1 C handle(s) never given back ... in hero_stream_number"*); the gate's words should name the mailbox (the compiler-engineer's ~10 lines). |
| S6 `send` | **amend** | append: *"a send that fails leaves what it was given as it was, `@` or not"*. The historian's objection 1, the compiler-engineer's build; § 13's `when` already says a failed call ends no life (*"every end ... happens only then"*). Without it a moved SQLite handle refused by a closed mailbox is lost and told as the program's. Unbounded: adopt (route A built both; neither corrupts: unbounded admits memory, a waiting bounded send admits a deadlock). |
| S7 `receive` | **amend** | replace *"or `stopped` once a stop is asked (section 13), even with values in it"* by **"or `stopped` once a stop is asked (section 13) and it is empty: a value already in it is received"**. Three measurements: the layer's own rule (`hero_net.h:162-164`, *"a `recv` with bytes already there ... still move them, so a stopping program can finish what it holds"*; only `accept` and `poll` stop under load); route A built so, a reply queued before the stop received after it, 20 + 5 + 5 runs; the compiler-engineer's gateway, stop-wins stranding 24 of 32 connections as handles nobody consumed. The spec-warden's W-i order then follows: closed and empty is `closed`. |
| S8 `close` | **adopt** | route A: a second `shut` does nothing, waiting receivers drain then answer `closed` (measured). |
| S9 what crosses | **adopt, amend** | add **"A `(thread T)` and a `(mailbox T)` never reach C: no `extern` signature, callback parameter or group record field holds one."** The spec states no sentence of what may cross to C (grep of § 13 for `cross`); the checker's `crosses_the_boundary` (`selfhost/check/ffi.hero:79-95`) owes the arm `return false`, as `[T]` gets `ffi_type`/`ffi_field_type` today (measured on three shapes). Function values: adopt (the compiler-engineer: a bare C function pointer, no closure). |
| S10 never crosses | **adopt**, with W-f's *"anything else is a compile error"* | a `ptr` is an address no life tracks, so crossing it is two holders by construction, the SQLite crash's shape; a `cstr` is a lease of `_Thread_local` regions (`lend.c:143-186`). |
| S11 a handle | **veto as written; adopt amended** | add to § 11 after *"... or as the answer `wait` gives"*: **"A copy of it kept from before then is not the receiving thread's: reaching C through it aborts, naming the call."** (+ the runtime's holder check, `p254/q11/rt/parts/alloc.c`, about 45 lines, +8 bytes per live handle, +3.6 ns per handle argument). Measured: a copy kept across a send reaches SQLite beside the receiver, 8 of 10 killed, 2 silent; with the check, 10 of 10 stopped before C, a clean move 10 of 10 and 0 TSan/ASan reports. Then W-j (through a mailbox only) or W-j' (`spawn(f, @x)` with the thread owning it): the C side is indifferent, provided `spawn` and `wait` move the holder too. Moves measured sound: TLS, socket stream, listener, UDP, a socket's number, SQLite, a poll set (20/20, 5/5, 5/5, 0 reports each); at once: SQLite crashes 20/20, a poll set refuses as a value. **Under B no number door is needed**: none of the 13 `acquires hero_stream_close` bindings changes (route A had to edit them). **Gap, owed, not a veto**: a C object its library ties to one thread has no binding word to say so (a `ptr` never crosses but tracks no life; a handle tracks a life and crosses with `@`); no memory fault from one measured in the tree (the IPC lock answers values). |
| S12 the stop | **amend** | with S7's drain; and **the runtime never calls `hero_net.h`**: `runtime.o` is linked whole into every program (measured: a `print` program holds `_hero_thread_spawn`), so a runtime `receive` asking `hero_net_stop_asked` would link `hero_net.o` (+49,920 bytes, measured) into every program, and `ws2_32` on Windows (read). The compiler-engineer's shape (the layer tells the runtime's queues from `hero_net_nudge_all`, +4,080 bytes and 25 symbols to every program, 0 `hero_net` symbols in `print(1)`, measured by that seat) keeps the dependency's direction, layer to runtime, which every member already has (the handle set, the allocator). I adopt it; route A's member (calling `hero_net_when_stopped`) is the other sound shape, costing a mailbox program `hero_net.o`. |
| S13 names | **adopt**, provided the four never enter the built-in table | spelling B's veto re-measured and **holding**: an `extern` member named like a built-in is refused `builtin_name_taken` (`p254/q2/exit_member.hero`, C's `exit`), and § 13 has no rename, so B makes POSIX `close(2)`, `send(2)` and `wait(2)` unbindable; under M `tests/golden/run/fixedbugs-396` builds and equals its `.expected`, and C's `close(2)` binds and runs (`close_member.hero`). B with names nothing declares (the spec-warden's `thread_wait` ...) is not reached by my veto. |
| S14 the doors | **adopt** the removal from the text; the doors stay in `hero_os.h` until every caller moves after the seed refresh (the spec-warden's sequence) | they are sound (one `i64` or one `str` copied, clang-checked against `hero_os.h`), so keeping them bindable meanwhile costs no robustness. |

## Route A, the conservative route, priced and built (Q3, Q4)

**Verdict on A: approve as the recorded conservative route; not adopted.** It repairs feature 2647 on
today's language, measured: the gateway shape with TLS connections accepted on the main thread and
served on W workers, an owner thread holding SQLite and a cache, right 20/20 plain, 5/5 `--sanitize`,
5/5 TSan, and **3.52 s on one worker against 0.32 s on sixteen** (16 clients, 10 requests, 20 ms each,
latency-bound). Its price:

- **C**: `runtime/platform/hero_mail.c` 525 lines and `runtime/hero_mail.h` 107 (`wc -l`), the Windows
  arm written and unrun; `hero_stream_number`/`hero_stream_of` +74 in `hero_stream.c`, +16 in
  `hero_stream.h`; `runtime.c` +2; tag **15, the last of four bits** (the next member needs a fifth bit,
  which moves every member's `HERO_HANDLE_LIFE_MOST`). 150 insertions, 5 deletions over the tracked
  files (`git diff --stat`), plus the two new files.
- **The runtime key moves once**: the first build after it `real 7.95 user 3.95 sys 1.52` against
  `real 2.50 user 1.38` warm (load about 50, a one-time cost, not a speed figure).
- **Rebel**: `io.hero` +34, `io/hero_stream.hero` +22/-1, `tls/hero_tls.hero` +2/-1 (`git diff --stat`);
  **every module whose bindings begin a stream that may cross owes `| hero_stream_number` in the mark and
  a declaration of `hero_stream_number` in its group**: 13 bindings in 6 files (`grep -rn`); a binding
  left unedited is stopped at run time by name (`p254/left/`, first run, measured). The mailbox face, 96
  lines (`p254/gwa/mailbox.hero`).
- **What a program pays**: text serialisation of every richer value; a mailbox door crosses as a
  one-shot number, so a reply mailbox is wired by hand (two numbers per mailbox in the gateway);
  a mailbox program links `hero_net.o` (+71,584 bytes with the member, measured).
- **Stress**: the member alone, 4 producers and 4 consumers on doors taken back from numbers, 0 to
  65,535 bytes, bounded and unbounded, shut, a door closed under a wait, the stop: 20/20, 5/5, 5/5,
  0 reports.

**Route A0** (Rebel's `ipc` channel, no new C): **cannot be served by a `poll` set on this Mac**
(`unsupported: ... this system cannot wait on what the stream stands on`), so the keeper serves one
channel to its end before the next unless each REQUEST takes a channel of its own; with that, the keeper
on its own thread: 20/20, 5/5, 5/5, 0 reports, `red: 402 green: 400 blue: 398 largest: 402`. **A TLS
connection cannot cross on A0 at all**, so A0 repairs the owner half of 2647 only.

## Where I disagree with the seats that reported, with my measurement

- **The compiler-engineer's S11 prototype** (`p254/q2/handles.c`: the send empties the cell to the dead
  value) and **the spec-warden's W-j** both stop the emptied cell and neither a copy kept from before
  the send: measured above, 8 of 10 killed. My repair is the runtime's holder check, built.

- **The spec-warden keeps S7's stop-wins in `w2`** (§ 2, *"I keep S7 as proposed"*): I disagree, on the
  layer's own rule, on route A's drain run, and on the compiler-engineer's 24 of 32 stranded. His
  priced drain text (-1 vendored on `w`) is the one I would adopt, with *"a value already in it is
  received"* said.
- **The spec-warden adopts S1 and S6 as written**: I amend both (S1's runtime table, S6's failed send),
  with the compiler-engineer and the historian.
- **The historian reads Dart's refusal of a socket** beside S11: Dart's reason (*"objects with native
  resources"*) does not bind this layer, whose tables are process-wide and whose sockets, TLS streams,
  listeners and UDP sockets I moved between threads 20/20 with 0 sanitizer reports. Node's
  `transferList` (`net.Socket`, `FileHandle`) is the precedent the measurement matches.
- **With the compiler-engineer on the stop's link**, against my own first recommendation of a layer
  member: his shape costs every program 4 KB and keeps `hero_net.o` out of a mailbox program; mine
  keeps 4 KB out of every program and puts `hero_net.o` into every mailbox program. Both are sound;
  his keeps the dependency's direction for every program, and I take it.

## Premises of the briefs found false, or wider than written

1. **`00-shared.md` § 5 question 8 and the critic's finding 27** (*"`hero_poll.h` takes a stream into a
   set"*, *"`runtime/hero_poll.h:132-185` takes sockets, streams and timers"*): **false for an `ipc`
   channel and for a TLS stream on this Mac**, each `HERO_POLL_UNSUPPORTED` (`p254/a0p/`, first run;
   `p254/c/tls_poll.c`). A stream made over a member's table has no descriptor to wait on.
2. **Q2.1's measure of B's veto as *"4 live bindings"* of `close`**: the veto is wider. B as spelled
   makes POSIX `close(2)`, `send(2)` and `wait(2)` unbindable for every future binding, since an extern
   member's name is C's and § 13 has no rename (grep, and `builtin_name_taken` on `exit`, measured).
3. **`runtime/hero_ipc.h:302-303`, *"only that thread releases it"***: false once the holder thread has
   ended: B, a new thread, released A's lock 3 of 3, `pthread_equal(A, B)` = 1 each time (Darwin reuses
   a dead thread's `pthread_t`; `hero_ipc.c:2897-2903`). Unfiled, owed to the coordinator as an
   `adjacent` defect with `p254/c/ipclock_alive.c`.
4. **`runtime/hero_poll.h:54-57`'s parenthesis** (*"a program of the language cannot ask it"*): becomes
   false under S11 as the brief says; measured that the guard beside it holds (`HERO_POLL_INVALID`, a
   value), so the parenthesis must go and nothing else.
5. **`00-shared.md` § 5's S11 row** (*"which empty it, its life going with it"*) and the critic's finding
   30 treat the `@` as what keeps a handle on one thread: false where a copy exists. Emptying a cell
   ends nothing the dead set can see, and a copy made before the send is live on the sender (measured,
   `p254/c/copy_then_send.c copy`). The compiler-engineer's `reuse` row measured the emptied cell only.
6. **The shared brief's route A** says nothing of the binding edits: every stream-acquiring binding owes
   a mark and a declaration (13 bindings, 6 files), and B owes none of them.

## Predictions, each with when it is checkable

- **P1**: on lane-mailbox's compiler, a TLS connection handed by `m.send(@conn)` to a worker and served
  there runs 20/20 plain and 5/5 `--sanitize` with **no edit to any of the 13 `acquires
  hero_stream_close` bindings**. Checkable when the lane's gateway rewrite builds.
- **P2**: `tests/golden/run/fixedbugs-396-posix-fills-its-buffers.hero` and the other three live `close`
  bindings build and run unchanged on lane-mailbox's compiler. Checkable at the lane's gate (`run`,
  `check`).
- **P3**: on the lane, a value holding a SQLite `Db` sent without `@`, or a `Db` copied and the copy
  sent, is refused at compile time (S11), and never reaches the 20-of-20 crash of
  `p254/c/sqlite_handover.c shared`. Checkable on the lane's `check` cases.
- **P4**: on the lane's compiler, a `print(1)` program has 0 `_hero_net_*` symbols (`nm`), whatever the
  mailbox's queue is. Checkable at the lane's first build.
- **P5**: `p254/c/tls_handover.c` on the Linux arm (OpenSSL by `dlopen`) passes 20/20 and 5/5 TSan.
  Checkable when Docker answers or on the CI's Linux leg.
- **P6**: a `(mailbox T)` in an `extern` signature is refused `ffi_type` at the binding's line on the
  lane's compiler, never `internal error` at exit 2. Checkable at the lane's `check` cases.
- **P7**: the Heroes program `db = sqlite.open(p).must()`, `keep = db`, `m.send(@db)`, a worker
  receiving and executing on it, `main` executing through `keep`, **builds on lane-mailbox's compiler**
  (the checker cannot see the copy) and, without a holder check in the runtime, **dies by a memory fault
  in at least 7 of 10 runs**; with the check, aborts 10 of 10 naming `keep`'s call. Checkable the day the
  lane's `send` builds.

## Unrun, owed

- The Linux (OpenSSL) and Windows (SChannel) arms of every TLS hand-over; route A's Windows arm (written,
  never compiled for Windows); the CI's legs or the box.
- A C object tied to its thread outside the layer (a COM apartment object, a GL context, an AppKit
  window): no binding of one in the tree, none run; the binding word it would need is a question for
  the sitting.
- Route B's gateway program is the compiler-engineer's; I built no B program (my C TLS hand-over is the
  emulation of B's hand-over at the C level: the handle's bits handed between threads, the set untouched).
- `hero_ipc`'s `pthread_t` reuse, filed by nobody here.
- The holder check through the emitter: `send`, `receive`, `spawn(f, @x)` and `wait` must call
  `hand_over` / `take_over` for every handle a value holds (the compiler-engineer's crossing walk already
  visits each, for the dead value); measured in C only. It covers every path to C because the emitter
  writes `hero_handle_alive_at` before every C call with a handle argument, the `consumes` call included
  (read in `heroes-next build p254/q11/copy_ends.hero --emit-c`, line 186). Its Windows arm needs no
  platform call (an atomic counter and a `_Thread_local`); unrun there.
- A handle inside a value a THREAD answers (`wait`) and a handle held by a thread that ends without
  sending it: the holder then names a thread that is gone; the check must treat `wait` as `take_over` by
  the waiter. Unrun.

## Working log

- 02:32 stage 1, 2, 3 build started in my copy (`p254/build.log`).
- 02:33 certificates: `p254/certs/{root,localhost_chain,localhost_key}.pem`, extracted by `python3 -I` from
  `packages/rebel/0.1.0/files/tls/internal/fixture.hero` (the loopback tests' own `ROOT`, `LOCALHOST_CHAIN`,
  `LOCALHOST_KEY`; `openssl x509` reads `CN=localhost`, valid to 2125). No network call.
- 02:34 **Q1.1, TLS handed to another thread, in C, against the layer's own headers**:
  `p254/c/tls_handover.c` (8 client threads, `hero_tls_connect` to `localhost` with the fixture's root and
  `HERO_TLS_CHECK_ALL`; one accepting thread, `hero_tls_accept`; the `hero_stream *` handed through a
  mutex queue to 4 worker threads, each calling `hero_tls_set_deadline`, `hero_tls_version`,
  `hero_tls_protocol`, 40 echo rounds of 16 bytes, `hero_stream_close` on the worker). Mode `gone`: the
  accepting thread is joined before any worker starts; mode `running`: it keeps accepting while workers
  serve. Linked with `runtime.c`'s object (it needs nothing of a program: `nm -u` names no `hero_` symbol)
  and the members `hero_tls hero_stream hero_stream_net hero_net hero_time hero_poll`, by
  `p254/c/build.sh <plain|asan|tsan>`. **Plain: `gone` 20 of 20, `running` 20 of 20** exit 0, each run
  `ok ... rounds=320 v12=0 v13=8 h2=8`. **ASan+UBSan: 5 of 5 and 5 of 5, 0 reports. TSan: 5 of 5 and 5 of 5,
  0 reports** (`grep -c -E 'WARNING: ThreadSanitizer|ERROR: AddressSanitizer|runtime error:'` over
  `p254/c/logs/tls-*`). The TSan build sees races here: `p254/c/control_race.c`, a deliberate unlocked
  counter, prints 1 `data race` warning under the same flags. So on Darwin's arm (Network.framework, one
  serial queue, a semaphore and a mutex per connection, `hero_tls.c:3857-3923`) a TLS stream's calls move
  between threads with no fault and no race the sanitizers read; what makes that true is the arm's
  design, not the language. Linux (OpenSSL by `dlopen`) and Windows (SChannel) arms: **unrun** (Docker not
  asked; the box not mine), owed to the CI legs.
- 02:37 **Q1.2, a plain socket stream, a listener, a UDP socket, and panel 246's number, each made on one
  thread and used and closed on another**: `p254/c/net_handover.c` (6 connections accepted on a thread
  joined before 6 workers serve them through `hero_stream_net_of`, `hero_stream_net_set_deadline` on the
  worker, 30 echo rounds each; a listener made on a joined thread, accepting 3 and closed on another; a
  UDP socket made on a joined thread, 30 `recv_from`/`send_to` rounds and closed on another;
  `hero_net_number` on one thread, `hero_net_of` and `hero_net_close` on another). **Plain 20 of 20, ASan
  5 of 5 with 0 reports, TSan 5 of 5 with 0 reports**, each run `ok stream=180 listener=3 udp=30 number=1`.
- 02:38 **Q1.3, SQLite**: `p254/c/sqlite_handover.c`, the system's `libsqlite3` (`-lsqlite3`, the SDK's
  `sqlite3.h`). **`sqlite 3.54.0 threadsafe=2`**: this Mac's library is built MULTI-THREAD, not serialized,
  so a connection is the program's to keep on one thread at a time and SQLite takes no lock on it.
  *Moved* (opened on a thread with Rebel's own flags, `READWRITE|CREATE|EXRESCODE`, `sql/sqlite.hero:107`,
  a statement prepared there, the thread joined; 400 steps of that statement on a second thread, 400 on
  a third, which finalizes and closes): **plain 20 of 20 `red=800`, ASan 5 of 5 and TSan 5 of 5, 0 reports.**
- 02:40 **Q1.6, the same connection used by two threads AT ONCE** (two threads each running 400
  `BEGIN; UPDATE; COMMIT` on one connection, what S11's `@` is to prevent; Rebel's flags, nothing added):
  **plain: 20 of 20 runs killed, exits `138` 4 times (SIGBUS) and `139` 16 times (SIGSEGV)**; ASan: 4 of 5
  `ERROR: AddressSanitizer: SEGV ... in sqlite3DbMallocRawNNTyped` (exit 134), the fifth hung until
  `timeout 120` (exit 124); TSan: 4 of 5 `ThreadSanitizer:DEADLYSIGNAL` (exit 134), the fifth hung (124),
  0 race reports, since TSan does not instrument the system's library. `pgrep` empty after. **So the `@` of
  S11 is load-bearing for memory safety, not tidiness: a SQLite handle reachable from two threads crashes
  the process on this Mac 20 runs of 20 with the binding Rebel ships today.** The `shared-nomutex` arm
  (`SQLITE_OPEN_NOMUTEX`) was not needed: the default already is that mode here.
- 02:44 **Q1.4, a poll set**: `p254/c/poll_handover.c`. *Moved*: a set made on a joined thread holding a
  listener, waited on by a second thread that accepts 4 connections, adds each, reads, removes and closes
  the set there. *At once*: one thread waits in `hero_poll_wait`, the other asks `add_socket`, `remove` and
  a `wait` of the same set (each answers -1 `HERO_POLL_INVALID`, nothing changed) and then closes it (the
  waiter answers -1 `HERO_POLL_NOT_OPEN`). **Plain 20 of 20 `ok moved=4 invalid=3 closed_wait=1`; ASan 5 of
  5 and TSan 5 of 5, 0 reports.** So the set's own guard holds whatever the language lets cross: two
  threads in one set are refused as a value, never a fault. What becomes false is one sentence, the
  header's parenthesis *"(a program of the language cannot ask it: a thread is handed an `int64_t`, and
  no number names a set)"* (`runtime/hero_poll.h:54-57`): under S11 a program CAN hand a set to another
  thread; the guard stays, the parenthesis must go.
- 02:42 compiler built: stage 1 `real 28.39`, stage 2 `real 287.29 user 176.67 sys 40.34`, stage 3 `real
  275.66 user 174.88 sys 43.85` (load 16 to 17, not a speed figure), `--version` `sources 6e891e681c14bca0`,
  equal to the coordinator's.
- 02:50 **Q1, the handle whose header ties it to a thread: `hero_ipc_lock`** (*"held by one thread of one
  process at a time, and only that thread releases it"*, `runtime/hero_ipc.h:302-303`; `release` answers
  `HERO_IPC_NOT_HELD` *"where the calling thread does not hold it"*, `:323-325`). `p254/c/ipclock_alive.c`:
  acquired on A, the handle handed to B. **A still running: B's release answers -1 `NOT_HELD` (kind 14),
  3 of 3**, as the header says. **A joined first: B's release answers 0, 3 of 3, and `pthread_equal(A, B)`
  is 1 each time**: Darwin gives the new thread the dead one's `pthread_t`, and the layer's holder test is
  `pthread_equal(slot->holder, pthread_self())` (`runtime/platform/hero_ipc.c:2897-2903`). A finding beside
  the sitting, not a soundness veto (no memory is touched: `flock` belongs to the open file description,
  so the release really releases), but it means a thread identity compared after its thread ended is
  not an identity, in today's tree too (`hero_ipc_held_here`, `:2907-2916`, is asked at every acquire).
  **Unfiled** (no GitHub write here); owed to the coordinator as an `adjacent` defect with this program.
- 02:58 **Q3, route A's member, written and built in my copy**: `runtime/hero_mail.h` (107 lines) and
  `runtime/platform/hero_mail.c` (525 lines, `wc -l`), a POSIX arm built and a Windows arm (SRW lock and
  condition variable, `SleepConditionVariableSRW`) **written and unrun**; tag **15**, the last of four bits,
  taken in `hero_handle_tag.h` (a member after it needs a fifth bit, which moves every member's
  `HERO_HANDLE_LIFE_MOST`); `hero_stream_number` / `hero_stream_of` beside `hero_stream.c`'s table (+74
  lines there, +16 in `hero_stream.h`, `git diff --stat`; 94 insertions and 2 deletions over four tracked files), the protocol of `hero_net_number`; `runtime.c`'s member block
  names the two new files. Doors (handles a program holds) and queues; a second door to a queue crosses as a
  number taken back once (`hero_mail_number` / `hero_mail_of`, a life in the handle set owed to
  `hero_mail_of`); a message is one layer block, taken on the sender's thread and given back on the
  receiver's; a bound per queue (0 none) with a send that waits; **the stop as the layer's recv rule**: a
  message there is still received after the stop, a call that would wait answers `STOPPED`.
  `clang -Wall -Wextra` on both members: silent.
- 02:55 **the runtime key moved once** (the four runtime files and the two new ones): a trivial
  `print("hi")` program, warm before, `real 2.65 to 3.50`, `user 1.39 to 1.48`; **the first build after
  the key moved `real 7.95 user 3.95 sys 1.52`** (the runtime's object and the layer's archive rebuilt once),
  then warm again `real 2.50 user 1.38` and `real 2.81 user 1.43`. Load 49.7 to 56.6 (`uptime`), so these
  are a one-time cost of about 2.5 s of CPU at that load, not a speed figure.
- 03:02 **Q3, the member stressed in C**: `p254/route-a/mail_stress.c` (4 producers and 4 consumers, each
  consumer on a door taken back from a number on its own thread; 500 messages each of 0 to 65,535 bytes
  checked by a checksum, unbounded and with a bound of 8; `shut` while receivers wait, which drain then
  answer `CLOSED`; a send after the shut `CLOSED`; a door closed under a waiting receive, which answers
  `NOT_OPEN`; `hero_net_ask_stop` under a receive on an empty queue, which answers `STOPPED`, and a reply
  queued before the stop still received, then `STOPPED`). **Plain 20 of 20, ASan 5 of 5 with 0 reports,
  TSan 5 of 5 with 0 reports**, each `ok many=2 ... closed_door=1 stopped_wait=1 drained_after_stop=1`.
- 03:08 **Q2.1, spelling B's veto re-measured.** `p254/q2/exit_member.hero` binds C's `exit` (a Heroes
  built-in's name) as an `extern "stdlib.h"` member: **`error[builtin_name_taken]: \`exit\` is a built-in of
  the language, so the name is taken everywhere — pick another name`**, at the member's own line, no
  binary. And § 13 has no renaming: an extern member's name is C's (`sed -n '/^## 13. FFI/,$p'` grepped for
  `name`, `rename`, ` as `: no such form). So under B, `close`, `send` and `wait` as built-ins make three
  POSIX functions unbindable, `close(2)`, `send(2)` (`sys/socket.h`) and `wait(2)` (`sys/wait.h`), not only
  the 4 live `close` bindings the brief counts. **The veto holds for B as panel 251 spelled it.** Under M the
  names stay free: `p254/q2/close_member.hero` (C's `close(2)` bound, called on -1) builds and prints `-1`,
  and **`tests/golden/run/fixedbugs-396-posix-fills-its-buffers.hero` builds, runs exit 0 and its output
  equals its `.expected`** (`diff` silent) on this compiler; M changes no name it uses, so it builds
  unchanged under M **provided the compiler never enters `wait`, `send`, `receive` or `close` in its
  built-in table** (the spec-warden's §4 says the same). **B with names nothing declares** (the
  spec-warden's `thread_wait`, `mail_send`, `mail_receive`, `mail_close`, 0 collisions on his count) is not
  reached by my veto: no C library I know binds those names (a question, not a search of every header).
- 03:10 **Q2.2, the two types at the boundary.** Today's checker refuses a refcounted type wherever C
  would see it: `p254/q2/arr_param.hero` `error[ffi_type]` (a parameter), `arr_callback.hero`
  `error[ffi_type]` (inside a callback's parameter), `arr_field.hero` `error[ffi_field_type]` (a group
  record's field). `selfhost/check/ffi.hero:79-95`, `crosses_the_boundary`, is an exhaustive `match` over
  the type's kinds with `return false` after it, so a new `Ty` case is a compile error in the compiler
  until someone writes its arm: the safe direction. The arm owed is `return false` for both (a queue's
  and a thread's layout are the runtime's, and no header can declare them), so `(mailbox T)` and
  `(thread T)` are refused in an `extern` signature, a callback's parameter and a group record's field,
  `ffi_type`/`ffi_field_type`, as `[T]` is. **The spec states no sentence of what may cross to C** (grep
  of § 13 for `cross`: only the thread doors' sentence the proposal deletes); the refusal's wording lives
  in the diagnostic. One clause is owed in § 3 or § 13, below in my S9/S10 text. `==` on a mailbox
  compares the queue's address; as a map key it is sound under S5 (a key is a copy, so its queue cannot
  be freed and its address reused while the key stands), unlike a handle's (§ 13: a map key is an error).
- 03:12 **Q2.3, where the stop lives, by `nm`** (`p254/nm/`, built by my stage 3): `plain.hero` (`print`
  only) 310,856 bytes, `_hero_net_listen` 0; `thread.hero` (the thread doors) 311,288, `_hero_net_listen`
  0; `stop.hero` (binds `hero_net_stop_asked` alone) **360,776, `_hero_net_listen` 1**: asking the stop takes
  the whole `hero_net.o`, +49,920 bytes; `mail.hero` (route A's `hero_mail_open`/`close`) **382,440,
  `_hero_net_listen` 1, `_hero_mail_open` 1**, +71,584. And **`runtime.o` is linked WHOLE into every
  program**: `plain` holds `_hero_thread_spawn`, `_hero_thread_spawn_str` and `_hero_handle_acquired_at`
  (`nm`). So **if route B's `receive` lives in the runtime and asks `hero_net_stop_asked`, every Heroes
  program links `hero_net.o`, and on Windows `ws2_32` with it** (`hero_net.c:149-151`, `#pragma
  comment(lib, "ws2_32")`, read, unrun there). Two C shapes avoid it, each a ruling the resolution must
  make: (a) the queue's C is a platform member the emitted C calls, taken only by a program that names a
  mailbox (route A's member is exactly that shape, so B's runtime half can be A's member); or (b) the
  stop's flag and its wake list move from `hero_net.c` into `parts/`, `hero_net.c` setting them. (a) is
  what I recommend: it keeps `runtime.o`'s link line and Windows' unchanged for every program that names
  no mailbox, measured above as 0 bytes of `hero_net.o` in `plain`.
- 03:05 **Q3/Q4, route A's Rebel face, in my copy's package**: `io/hero_stream.hero` +2 members
  (`hero_stream_number ... consumes`, `hero_stream_of ... acquires hero_stream_close | hero_stream_number`)
  and 2 wrappers (+14 lines); `io.hero` +35 lines (`to_number(@s)`, `of_number(n, name)`);
  `tls/hero_tls.hero`: `hero_tls_accept`'s mark gains `| hero_stream_number`, and **the group must
  declare `hero_stream_number` itself** (+1 line): without it the checker refused the mark,
  `error[unread_releaser]: hero_stream_number does not end the life of any Stream: no extern of this
  module declares it taking one back with consumes`. So **every module whose bindings begin a stream
  that may cross owes that pair of edits: 13 bindings in 6 files say `acquires hero_stream_close`**
  (`grep -rn`, io 2, ipc 2, net/hero_stream_net 1, process 1, serial 2, tls 5); I made the one the
  gateway needs. The mailbox face: `p254/gwa/mailbox.hero`, 95 lines (one group of 9 members and 4
  constants, `open`, `number`, `of_number`, `send`, `receive` with the fill form, `shut`, `close`).
- 03:05 **Q4, the gateway shape on route A, in Heroes, built**: `p254/gwa/main.hero` (+ `mailbox.hero`,
  `certs.hero` copied from the TLS fixture, `heroes.toml` naming my copy's Rebel). The OWNER thread
  (`threads.spawn_text`) opens SQLite in memory itself and keeps a `{str: i64}` cache; W workers; C client
  threads each holding one TLS connection (`tls.connect` with the fixture's root, ALPN `h2`), R requests,
  a pause before each; **the main thread `tls.accept`s every connection and hands it to the workers as a
  stream number through a mailbox** (`io.to_number`), each worker taking it back (`io.of_number`), asking
  `tls.set_deadline` and `tls.version_of` on its own thread, serving lines, asking the owner by the
  requests mailbox and waiting on its own reply mailbox; mailbox doors cross as numbers in the text each
  thread is spawned with. Built first time by my stage 3; `gwa 4 8 5 5`: **`workers=4 clients=8 rounds=5
  served=40 tls13_on_workers=8 wrong=0 requests=40 sqlite_sum=40 cache_agrees=true`, exit 0** (the exit
  gate passed: every handle given back, every number taken back). **Plain 20 of 20** that line exactly.
- 03:19 **Q4, route A's gateway under the sanitizers.** `--sanitize` (ASan+UBSan, the compiler's own
  flag): **5 of 5 right, 0 reports** (`grep -c -E 'ERROR: AddressSanitizer|runtime error:|panic:'`).
  **TSan by hand** (`p254/gwa/tsan/build.sh`: `heroes-next build --emit-c`, 86,057 lines, the compiler's
  `-std=gnu11 ... -fsigned-char ...` words with `-O1 -g -fsanitize=thread`, `runtime.c` and 16 layer
  members compiled the same way, `-lsqlite3` and the three frameworks): **5 of 5 right, exit 0, 0 `WARNING:
  ThreadSanitizer`**. The control of 02:35 (`p254/c/control_race.c`) shows this TSan reads races here.
- 03:20 **Q4, speed against one thread, latency-bound** (16 client threads, 10 requests each, a 20 ms
  pause before each; one TLS connection per client, accepted on the main thread and served on W
  workers; `/usr/bin/time -p`, three runs each; `uptime` 14.24 before, 13.13 after, so `real` far above
  `user + sys` is the 20 ms pauses, which is the point of the shape):

  | W workers | right | real (3 runs) | user + sys |
  |---|---|---|---|
  | 1 | 3 of 3, `served=160 ... sqlite_sum=160 cache_agrees=true` | 3.56, 3.52, 3.54 | 0.27 to 0.28 |
  | 4 | 3 of 3 | 0.96, 0.91, 0.94 | 0.17 to 0.21 |
  | 16 | 3 of 3 | 0.32, 0.31, 0.32 | 0.23 |

  One worker serves the connections one after another in about their sum (16 × 10 × 20 ms = 3.2 s),
  sixteen in about their maximum (10 × 20 ms = 0.2 s plus 16 handshakes): **11.1 times faster at W = 16**
  (3.52 / 0.32, best against best), with the owner's state right every time. This is feature 2647's
  repair, run, on route A, on today's language with no spec token.
- 03:22 **Q1.5, a handle left inside a mailbox when its queue goes** (`p254/left/main.hero`, route A): a
  pipe's reading end given up as a stream number, the number sent into a mailbox, the mailbox's last door
  closed, `main` returns. **First run: `panic: a C handle was given back to hero_stream_number, and the mark
  that began its life names hero_stream_close ...` exit 134, before C**: `hero_stream_pipe`'s mark did not
  name the new door, so the runtime's set stopped the crossing by name, the Rebel cost of 03:05 measured on
  a second binding. With `| hero_stream_number` added to `hero_stream_open`'s and `hero_stream_pipe`'s
  marks: **`main returns`, then `panic: 1 C handle(s) never given back — every life a door of the runtime
  began owes one release ... 1 taken at hero_stream.h:0:0, in hero_stream_number, a stream given up as a
  number no hero_stream_of took back`, exit 134.** So the process-wide handle set (one entry per address,
  `alloc.c:1155-1198`) already tells a handle stranded in a dropped queue at `main`'s end, naming the door
  that stranded it; a handle's life moving between threads needs nothing of the set, which never asks which
  thread holds a life (every table and the dead page are process-wide; the per-thread state,
  `hero_handle_eq_where`, is a call's place only).
- 03:27 **Route A0 priced** (`p254/a0p/main.hero`, the critic's worker copied, read only). **(1) A `poll`
  set cannot serve the channels on this Mac**: `poll.add_stream` of an accepted `ipc` channel fails
  `unsupported: adding a channel accepted on "a0p.sock" to a set: this system cannot wait on what the
  stream stands on`, exit 134 at `.must()`: a channel is `hero_stream_make` over the member's own table
  (`hero_ipc.h:43-47`, `hero_ipc.c:1153`), which `hero_poll.h` refuses (*"a stream that stands on neither a
  descriptor nor a socket of this layer"*). So the keeper serves one channel to its end before the next,
  which is feature 2647's complaint moved one level: the critic's 402/400/398 held because each worker
  waited its turn. **(2) The keeper on a thread of its own, with a channel PER REQUEST** (connect, one
  message, the reply, close; the keeper accepting in arrival order, so the four workers interleave):
  **plain 20 of 20, `--sanitize` 5 of 5 with 0 reports, TSan by hand 5 of 5 with 0 reports**, each
  `red: 402 green: 400 blue: 398 largest: 402`; 400 requests in `real 0.34` (one run, load about 13, not
  a speed claim: a connect and an accept per request). **(3) A TLS connection cannot cross on A0** (no
  stream door; a listener cannot cross either, so the keeper makes its own and workers retry `connect`
  until it exists, `connect_when_there`). So A0 repairs the owner half of 2647 at the price of a channel
  per request and text serialisation, and **cannot repair the hand-over half at all**: the gateway's
  accepting thread would have to proxy every decrypted byte to a worker over a second channel.
- 03:37 **A TLS stream in a `poll` set, on this Mac** (`p254/c/tls_poll.c`, one accepted TLS stream added
  with `hero_poll_add_stream`): **`-1`, kind 5, `HERO_POLL_UNSUPPORTED`**. Darwin's arm makes the stream
  over a table with no descriptor (`hero_tls.c:15-26`), which `hero_poll.h` refuses. So **on macOS a TLS
  server cannot serve its connections together with `poll` at all**: threads are its only route, and the
  hand-over of an accepted TLS connection to another thread is the whole of feature 2647 there, not half
  of it. (`00-shared.md` § 5 question 8 asks whether a mailbox must join a `poll` set; for a TLS server on
  Darwin the question does not arise, since its connections cannot join one either.) Linux and Windows
  arms run TLS over a socket stream, which a set takes: unrun here.
- 03:38 Read `254-reports/historian.md` (verdict and rows) and `254-reports/compiler-engineer.md` (Q1.1,
  Q1.3, Q2.4, Q1.5 to Q1.8, Q2.6, Q2.7). Agreements and disagreements are in § Rows and § Where I
  disagree, below.
- 03:30 **S11 attacked at the shape beside `m.send(@v)`: a handle COPIED before the send.** Today's
  language accepts a copy of a handle (`p254/q11/copy_ends.hero`: `keep = b`, `free(b)`, `free(keep)`
  builds), and the runtime stops the copy only because `free` ENDED the life: **`panic: a C handle reached
  the argument p of free at an address a call marked consumes or transfers ended ...: a copy made before
  the ending call`**, the dead set's work. **A send moves a life without ending it, so that defence never
  fires.** `p254/c/copy_then_send.c` emulates B's emitted C with the runtime's own calls (as the
  compiler-engineer's `handles.c` emits them: `hero_handle_acquired_at`, the cell overwritten with
  `hero_handle_dead()` on the send, `hero_handle_alive_at` before each C call), with SQLite opened with
  Rebel's flags: a copy made, the original sent, the receiver running 400 transactions, the sender running
  400 through the copy. **`cell` (the emptied cell, the compiler-engineer's `reuse`): 10 of 10 stopped,
  exit 134, `a dead C handle reached sqlite3_exec`. `copy`: the check passes and both threads reach
  SQLite: 8 of 10 killed (exits `139 138 139 0 139 139 139 0 139 139`), the other 2 racing in silence.**
  So **S11 as written, and as the compiler-engineer's prototype emits it, lets a value-semantics copy put
  one C object on two threads**, which is this Mac's SQLite crash. The checker cannot close it in general:
  the spec itself gives a copy's misuse to the runtime (*"through a copy ... is the runtime's to stop"*,
  the note under `handle_used_after_end`, the critic's finding 16).
- 03:33 **The repair, prototyped** (`p254/q11/rt/parts/alloc.c`, a copy of the runtime): each life in the
  set records the thread that holds it, by a runtime thread number from an atomic counter (never a
  `pthread_t`, given 02:50's reuse); `hero_handle_hand_over` (the send, for every handle the value holds)
  marks it in a mailbox, `hero_handle_take_over` (the receive) gives it to the receiver, and
  `hero_handle_alive_at`, the check every handle argument already passes, refuses a thread that does not
  hold the life. +1 field per entry, about 45 lines. `p254/c/copy_then_send_held.c`: **`copy` 10 of 10
  stopped before C, exit 134, `panic: a C handle reached sqlite3_exec on a thread that does not hold its
  life: it was sent to another thread, and this is a copy kept from before the send`; `cell` 10 of 10
  stopped as before; `moved` (the receiver alone uses it) 10 of 10 exit 0; TSan 5 of 5 and ASan+UBSan 5
  of 5 on `moved`, 0 reports**, and the TSan build stops `copy` by name too.
- 03:36 **Its price on the hot path** (`p254/q11/alive_bench.c`, 10 million `hero_handle_alive_at` on
  1,000 live handles, `-O2`, the two runtime objects alternated six times; CPU-bound, so reported with the
  load, `uptime` 19.78 then 19.78): base 6.8, 6.6, 9.4, 5.7, 7.2, 7.5 ns per check; with the holder
  9.3 to 11.1 ns; slower in 5 of 6 pairs, **about +3.6 ns per handle argument, best against best** (5.7
  and 9.3). One SQLite transaction above costs microseconds; robustness outranks it (CLAUDE.md
  § Precedence).
