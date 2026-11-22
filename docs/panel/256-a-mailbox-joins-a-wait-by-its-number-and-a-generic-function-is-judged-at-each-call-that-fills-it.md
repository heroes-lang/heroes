# Panel 256: a mailbox joins a wait by its number, and a generic function is judged at each call that fills it

Convened 2026-11-22 by the coordinator of M-rebel-library, relaying the author: **the full panel** (both
questions touch spec § 11 and the checker), the compiler-engineer, the ffi-pragmatist, the spec-warden, the
historian (advisory) and the llm-ergonomist (the blind seat, **prepared and owed**, § The blind seat), the
completeness critic reading the briefs before any seat and the reports after. **The frozen commit**:
`8aca3cc432a6007daaf416ecf3b1b8564472fa09` of `rebel-round`, in the lane `lane-p256`; every seat in a detached
worktree of it under `.claude/worktrees/p256-scratch/<seat>/`, each building its compiler from the seed and
two stages (`sources 031aaabb358218fd`). Briefs: `docs/panel/256-briefs/`; reports: `docs/panel/256-reports/`
(`critic-briefs.md`, `compiler-engineer.md`, `ffi-pragmatist.md`, `spec-warden.md` with `spec-warden/`,
`historian.md`, `critic-reports.md`, `resolution/`, `evidence.md`).

**Status: provisional — author ratification pending.** The resolution is the most robust and complete the
seats' builds and measurements support (CLAUDE.md § 4, § Precedence as amended at `691531908`); it is
provisional on the author **and on the blind seat**, whose paid run is prepared and not run
(`256-briefs/blind/RUN.md`), its consequence for this resolution registered before it.

## The questions, as put

- **Q1, everything a program waits on, in one wait**: feature 2693 (a `(mailbox T)` cannot join Rebel's
  `poll` set; chat and notes ring UDP doorbells and loopback pairs to wake their loops), feature 2660 (a
  server cannot stop gracefully past its accept loop: the stop is process-wide and permanent), and the `ui`
  package's TH01 (a thread's message cannot wake `ui.wait`). What form lets one wait take sockets, streams, a
  counted signal, a mailbox and the user interface's events, and a stop scoped to a server?
- **Q2, a send inside a generic function**: defect 2695 (`blocking`): a generic body is checked once, and the
  walk that asks whether a type holds a handle answers nothing for a type parameter, so spec § 11's crossing
  rule is decided for no instance. What rule makes it true for every instance, at what cost?

## The proposal, verbatim (`256-briefs/00-shared.md` § 3.3 and § 4.5)

**P1**: (1) a mailbox is a source a wait takes, named to C by `m.number()` (`arms/spec-q1.diff`), Rebel's
`poll.add_mailbox(set, m)` and `ui.add_mailbox(@app, m)` told ready while the queue holds a value, is closed or
answers `stopped`, rung by the send itself; (2) a set joins the user interface's wait, `ui.add_set(@app,
set)`; (3) the stop scoped to a server is that server's own mailbox in its set, graceful shutdown counting
SIGTERM (`os.count_signal`, `poll.add_signals`) instead of catching the stop; (4) recorded as conservative,
the waker (`poll.add_waker`, `poll.wake`). **P2** (`arms/spec-gc.diff`): the § 11 rules judged at each call on
the types it gives the letters, and a letter's value always sent `m.send(@v)`; beside it `gi` (judged at each
call, the `@` included) and `gr` (refused in the body).

## The critic's first reading, and what it changed before any seat

29 findings (`critic-briefs.md`), each answered in the briefs as *(critic N)* before a seat started. Those
that changed the sitting: the main checkout stands on `main`, which lacks `691531908` and the frozen
`spec-warden.md`, so the frozen files were named as binding (1); the Windows box is not started by this
sitting (2); **the frozen § 11, read as written, already refuses t2's programs 3 and 5 and accepts 2: `gi`'s
reading** (16), put to every seat; `e2` (a `partial` map key through a generic) is panel 155's ruled guard,
not a new hole (4); panel 155's measured obstacle to a call-site route (no callee in the instantiations table,
150 of its 284 lines) handed to the compiler-engineer (9); the routes nobody listed: **a mailbox handing out a
descriptor, as `fs/watch` ships**, a forwarder per mailbox, the one wait in the runtime, a process stop with a
grace, **a run-time guard in the emitter's per-instance crossing walk** (11, 12, 14); the Windows stop bell
taking a stranger's datagram (13); a second clock on a Rebel program (17); the blind seat's rows widened, a
refusal graded by its reason, the short cell and chance at five registered, one run built after the
synthesis, reference programs owed (19 to 27).

## The verdicts

| seat | verdict | the measurement it rests on |
|---|---|---|
| compiler-engineer | **object** to P2 as worded (`gc`) and to `gr`; **adopt `gi`** with a 12-line emitter guard behind it; no veto | `gi` **built** (`check/instances.hero` 446 lines, +19 in six checker files, nothing in lowering, emitter or runtime): every probe refused at `check`, at the call, the body's line and the chain named (*"reached through `outer` -> `mid` -> `inner`"*); t2's programs 3 and 5 turn from run-time aborts into compile errors; **140 programs (98 golden thread cases, 42 Rebel `main.hero`) move 0**; `gc`'s `@` empties nothing at `i64` (`7` read after the send) and aborts at `Boxed`; `gr` refuses all four live generics and makes `poll.add_mailbox<T>` unwritable; Q1's number and watch built in a copy of `mail.c` (+115, +4 header), a C test rung and woke `poll()`; clocks discarded (load 29 to 114) |
| ffi-pragmatist | **veto** on any Q2 route that leaves a `ptr` crossing open; object to the watch in favour of the bell; approve `ui.add_set` on `kqueue` and `epoll` | **a `ptr` through a generic corrupts memory**: `p7w` prints `63` where `10` is right, the other thread's write landing in `main`'s next allocation, exit 0; ASan `heap-use-after-free WRITE of size 63 ... thread T1`; TSan silent; watch and bell **0 lost wakes** at 4 and 16 senders × 100,000, Darwin and Linux arm64, plain, ASan+UBSan, TSan; **the waker loses 320,799 to 397,379 of 400,000 values** with a loop taking one per wake, every send ringing; Darwin's zero-timeout `kevent` costs 36 to 63 µs, so the watch read one value per wake in 26.3 s, the bell in 0.19 s; AppKit wakes on a `kqueue` descriptor through `CFFileDescriptor` (206 to 228 ms after a 200 ms ring), GLib 2.84.4 on an `epoll` descriptor through `g_unix_fd_add` (203 to 209 ms); the Windows stop bell's socket logic takes a stranger's datagram on Darwin and Linux, dropped once connected |
| spec-warden | **object** to `gc` and `gr`; **approve `gi` repaired** (and Q1's `m.number()` repaired); no veto | six real counts (frozen 12,809): `q1` 12,875 (+66), `gc` 12,875 (+66), `gi` 12,915 (+106), `gr` 12,898 (+89), its route `spec-wadopt.md` 13,083 (+274), the proposal repaired 13,074; its four count predictions inside their ranges; **T0**: the spec states half the send's `@` rule (the checker refuses `m.send(@v)` of a value holding no handle, no sentence says so); `gi` is what § 11 already says (T4); `gc` one writing, two meanings by place (T2); `gr`'s advice leads a handle back to an abort (T5); `q1`'s *"has gone"* repaired to *"never another"* (T6) |
| historian (advisory) | Q1: approve P1.1's principle (the wait's truth a predicate on the queue, the send the ring); Q2: favours `gr` or a written bound; **object to `gi` without a signature that shows the obligation** | pages read 2026-11-22: mio's `Waker` (`EVFILT_USER`, `eventfd`); libuv coalesces `uv_async_send`; tokio's `Notify` stores a permit; Erlang's `{active, true}` *"provides no flow control"*; Stroustrup P0557r1: requirements *"hidden"* in the body, the error *"long after the point of call"*; Ada's contract model; Rust's auto traits from fields; Go 1.20's `comparable`, *"given up static type safety for a run-time check"* |
| llm-ergonomist (blind) | **owed** | prepared: 46 sessions drawn, a free preflight clean, 0 USD spent; `blind/RUN.md` |

## The critic's second reading, and what it changed

15 findings (`critic-reports.md`). **Agreed by re-run**: the compiler-engineer's `gi2` refuses six more shapes
the frozen compiler accepts (a `ptr` in a list or an optional, a `cstr`, a thread sent bare, a generic value
passed to a generic, a UFCS call) and **refuses the ffi-pragmatist's `p7r` and `p7w` at `check`**, the veto's
own condition, which no seat had run across seats. **What it changed**:

1. **The bell was adopted unbuilt** (1): the ffi-pragmatist's model never closes, never stops, and its one
   queue never dies; the watch is the route built in a copy of `mail.c` with close and a gone queue tested.
2. **On Windows the bell needs Winsock in the runtime** (2): `grep` of `runtime/runtime.c` and
   `runtime/parts/*.c` for Winsock prints nothing; the precedent named, `sigdoor.c`'s `hero_signal_fd`, answers
   `-1` on Windows (`sigdoor.c:310`), which is why `add_signals` is `unsupported` there.
3. **The bell's life contradicts the number's sentence** (3): pinned by the set, C holds a count; not pinned,
   the set keeps a descriptor the system may give the program's next `open`.
4. **The watch's stop ring runs under the layer's lock** (4, read): a ring that takes any lock of `hero_poll`'s
   can deadlock.
5. **`gi` accepts a generic no type can satisfy and blames its caller** (8, `i9`, `i10`).
6. **`spec-wi.md`'s *"these rules"* points back at one sentence** (9): repaired.
7. **The blind run read no chain and had no consequence** (13): program 7 added, the consequence registered.
8. **2660's half has no server built, and Windows counts no SIGTERM** (7; `hero_net.c:1444-1457`).
9. **The adopted text has no real count** (10); **`gi` is a prototype whose gates are unrun** (11); **the
   historian's objection has no built answer, and Nim's generics, CLAUDE.md § 6's model, were read by nobody**
   (12); **§ 7 still says `==` works on any two values of one type** (14).

## Where the seats disagree, stated plainly

- **Q2, `gi` against a refusal or a bound.** The compiler-engineer, the spec-warden and the ffi-pragmatist
  (*"judge each instance, the `ptr` half included"*) take `gi`; the historian's precedent (C++'s concepts, Ada's
  contract model, Pony's constraints) favours telling the rule against a declared letter, and objects to `gi`
  without a shown obligation. **Settled by measurement for robustness, not for locality**: `gi` and `gr` both
  close every shape, `gi` refusing nothing the tree needs and `gr` refusing four live generics and Q1's own
  face; a written bound is a core form by Part 5's test (the compiler-engineer's veto, not exercised because
  nobody proposed it as the resolution). **The locality half is the blind run's program 7** and the shown
  obligation the lane owes (R4).
- **Q1, the watch against the bell.** The ffi-pragmatist measured the bell faster on Darwin and R6's words kept;
  the compiler-engineer built the watch and showed the bell puts Winsock into the runtime on Windows. **Settled
  by the critic's checkable half**: the runtime links no Winsock, the bell has no Windows path that is not the
  watch, and its lifetime is unbuilt; the watch is adopted with the ffi-pragmatist's measured cost as a
  condition (R8).
- **2660.** No seat built a server; the ffi-pragmatist priced a stop token (an argument on every waiting call
  of five packages, a slot per token) and showed a call blocked outside any set is reached by no server
  mailbox. Settled as a direction, not a form (R10).

## The resolutions

### R1. A send, a spawn and a thread's answer inside a generic function are judged at each call that fills its letters (`gi`)

**Adopted, provisional — author ratification pending.** Every rule of spec § 11 on what a value holds (a
handle crosses only by `m.send(@v)` or as `wait`'s answer; a `ptr` or a `cstr` never crosses; a mailbox
carries no `()`; no queue holds itself; a written or inferred `(mailbox T)` and `(thread T)`) is judged at
each call of a generic function, on the types that call gives its letters, **the send's `@` included**: a
send of a value whose type names a letter serves, written `m.send(@v)`, only the types that hold a handle or a
thread, and written `m.send(v)` only those that hold neither. **Told at that call**, the body's file and line
named, and **the chain of generics named** where the need was carried (*"reached through `outer` -> `inner`"*),
one message per call, rule and letter. **Defect 2695 is the compiler's bug** (CLAUDE.md § 12, the spec-warden's
T4, the compiler-engineer's § 2.2): the frozen text already refuses programs 3 and 5; this adds where the error
is told and the `@` rule's missing half. The mechanism is the compiler-engineer's built one: a need per
declaration and letter, carried through generic calls, a finite set (polymorphic recursion terminates, `n3`),
checked at each recorded instantiation, the callee recorded where the checker already holds it (panel 155's
obstacle answered without its 150-line map). **It closes the probes' crossings** (`p2`, `p3`, `p5`, `p5c`,
`p6b`'s compiler panic, `p7`, `p8`, and the ffi-pragmatist's `p7r` and `p7w`, the critic's re-run).

**The spec text** (`256-reports/resolution/spec-r.diff`, made by `resolution/make_resolution.py` from the
spec-warden's `spec-wq.md` and `spec-wi.md`, its *"these rules"* repaired; sha256 `6c905ad57c68d63c...`;
**9,967 vendored**, +206 on 9,761; **its real count is unmeasured**, an inference near 13,060 from the
warden's 13,083 for a text that also held a § 9 sentence; the landing's `--refresh` measures it): § 11 gains,
after *"... a callback's parameter or a group's record."*:

> A send is written `m.send(@v)` exactly where `v` holds a handle or a thread, and `m.send(v)` everywhere
> else. In a generic function every rule of this paragraph on what a value holds is judged at each call, on
> the types that call gives its type parameters, a send's `@` included: a send of a value whose type names a
> type parameter serves, written `m.send(@v)`, only the types that hold a handle or a thread, and written
> `m.send(v)` only those that hold neither, and a call giving it a type it does not serve is a compile error,
> told at that call.

### R2. A generic no type can satisfy is refused at its declaration

**Adopted** (the critic's second reading, finding 8): where a declaration's needs contradict each other
(`both<T>` sending `m.send(@v)` and `m.send(w)` of one letter), no call can serve it, and it is refused in its
body, as the frozen compiler refuses it today; decidable on the needs set `gi` already makes. **Unbuilt**: the
lane builds it with its case (`i9`) before it is held to R1.

### R3. The emitter's crossing walk refuses a `ptr` or a `cstr` at run time, behind R1

**Adopted, the compiler-engineer's built guard** (`emit/crossing_bodies.hero`, +12 lines): a crossing function
for a type reaching a `ptr` or a `cstr` aborts before a byte is copied, *"a value reaching a ptr or a cstr
crosses between threads (spec section 11)"*. Measured with R1 switched off: `p5`, `p5c`, `p7`, `p8`, `n4` run
**134**. It is the wall panel 155's veto condition asks to stand behind any give-up, and the ffi-pragmatist's
*"second wall"*; **the ffi-pragmatist's soundness veto is lifted by R1 and R3 together**, its condition (*"when
`check` refuses `p7r` and `p7w`, with an abort in the emitter's crossing walk as a second wall"*) met by the
critic's re-run and the guard's build.

### R4. The obligation is shown where a reader looks: `heroes doc` prints a generic's crossing needs

**Adopted as owed, unbuilt** (the historian's objection, the compiler-engineer's answer, the critic's 12): a
generic function's inferred needs (*"sends its `T` by copy: `T` holds no handle"*) are printed by `heroes doc`
beside its signature, so the obligation a call must meet is readable without the body. **What would reopen
R1 toward a written bound or `gr`**: the blind run's program 7 read wrong by two or more of five `gi` readers
(registered, `llm-ergonomist.md`), or a program whose need, carried three calls deep, is told at a call its
author cannot connect to the body's line. **Not read by anyone in this sitting**: Nim's generics, Haskell,
OCaml, D, Zig (the historian's list).

### R5. Queued, not adopted: the same rule for § 13's `partial` and § 7's `==`

`spec-w9.md`'s § 9 sentence (*"A rule on what a type holds (sections 11 and 13) is judged at each call"*) would
close `e2` (a `partial` map key through a generic, check 0, build 0, run 134) and **reopen panel 155's ruling**
(R2 closed `==` on a letter alone; `e2`'s abort a designed guard, issue 599). **Unbuilt**: R1's machinery
carries a map-key need as it carries the others, nobody built it, and panel 155 measured that a body rule on a
letter as a map key deletes two goldens. **And § 7 contradicts the compiler** (the warden's T7, the critic's
14): `==` on a bare letter is refused (`generic_equality`) and no sentence says so. Both go to the lane as
questions with their cases, and to a sitting if the lane's build moves a golden.

### R6. A mailbox's number: `m.number()`

**Adopted, provisional** (P1 item 1's language half, the spec-warden's `spec-wq.md`): a fifth operation after a
dot, *"`m.number()` gives an `i64`, the same for every copy, naming its queue to C for a package that waits on it
beside other things: it holds no copy of the mailbox and reaches no value, and once the mailbox's last copy is
dropped it names no queue, never another"*; the UFCS sentence of § 9 gains `number`. The runtime's table holds
a queue by slot and life (the compiler-engineer's build: a new queue in a reused slot gets a new number, the
old reads `-1`); **spec § 11's boundary row holds word for word**: a mailbox never reaches C, an `i64` does,
and C reaches no value with it. No function of the tree named `number` takes a mailbox first (critic 29, 55
read). About 10 compiler lines, priced and unbuilt.

### R7. A mailbox is an entry of a set, told ready while it holds a value, is closed or answers `stopped`

**Adopted, provisional**: Rebel's `poll.add_mailbox<T>(set: poll.Set, m: (mailbox T)) -> i64?` (writable under
R1, checked and run by the compiler-engineer as `q9`), level-triggered on the queue's own state, **rung by the
send, the close and the stop**, so no ring is written by the program, none forgotten, none put before its value.
Measured on a model of `mail.c`: **0 lost wakes** at 4 and 16 senders × 100,000 on Darwin and Linux arm64,
plain and under both sanitizers, for the watch and for the bell alike; **the program-rung waker loses up to
397,379 of 400,000 values** to a loop that takes one per wake, and a forgotten ring all of them.

### R8. Its mechanism: the layer's watch, rung through a pointer, the ring taking no lock

**Adopted, the compiler-engineer's built route, with the ffi-pragmatist's measured cost as its condition.**
The layer registers a watch on the queue by its number (`hero_mail_watch`, `hero_mail_ready`, +115 lines of
`mail.c`, built and rung in a C test); the send rings it after the queue's lock is dropped, the close likewise,
the stop under the list's lock. **Three rules the landing writes down**: (a) **a ring takes no lock at all**:
an `EVFILT_USER` trigger, an `eventfd` write, a connected loopback `send()` (the critic's finding 4: the stop's
ring runs under the layer's lock); (b) **R6 of panel 254 is read by its measurement**: the runtime names no
layer symbol and a `print(1)` program links no layer code; the words *"the runtime never calls the platform
layer"* are amended to *"names no symbol of"* (the critic's first reading, 28); (c) **the set asks the queue's
predicate before it blocks**, and on Darwin answers a ready mailbox without paying the zero-timeout `kevent`
(36 to 63 µs per empty call, measured), **held to the ffi-pragmatist's harness: one value per wake within
twice the bell's 0.19 s**. **Should (c) fail**, the bell is taken on POSIX (a pipe or an `eventfd` the queue
owns, close-on-exec, the set holding a duplicate so a gone queue tells hangup and never a reused number) and the
watch on Windows, and the record says which ran. **Windows**: the set's ring is a loopback socket with the
three repairs of R12; written, unrun, owed to the box or the CI's Windows leg.

### R9. The user interface's wait takes a set and a mailbox (TH01)

**Adopted**: `ui.add_set(@app, set)` and `ui.add_mailbox(@app, m)`, `ui.wait` answering an event of the entry's
key. **Measured**: AppKit's `nextEventMatchingMask:untilDate:` wakes on a `kqueue` descriptor through
`CFFileDescriptor`, and GTK 4's `g_main_context_iteration` on an `epoll` descriptor through `g_unix_fd_add`.
**Owed**: `ui.add_mailbox` run (the watch rung into the arm's own `kqueue` or `eventfd`); the level source in
AppKit's one-shot callback, which must not spin while the mailbox holds a value (the critic's 6); Win32's half
(`MsgWaitForMultipleObjectsEx` given a handle). TH01's own sketch, `hero_ui_wake(app_number)`, is the waker's
shape, and its premise that an `App` cannot cross a thread is panel 254's stale rule (`ui.hero:36-38`).

### R10. 2660: no new form; a server stops itself through its own mailbox

**Adopted as the direction, not closed**: a server's loop holds its own control mailbox in its set (R7), and
closing or sending to it stops that server and no other wait; graceful shutdown counts SIGTERM
(`os.count_signal(.terminate)`, `poll.add_signals`) rather than catching the stop, so handlers' outbound calls
are never failed `stopped`; the process stop stays as it is. **What it leaves, measured or read**: a call
blocked outside every set (a worker's `recv`) is reached by no server mailbox and ends by its own deadline
(`net.set_deadline`), by its peer or by the process stop, R3 of panel 254 forbidding another thread to close its
socket; **Windows counts SIGINT alone** (`hero_net_count_arm`), so a counted console close, shutdown or a
service's stop is a feature owed there; and no server in the tree calls `poll.add_signals`. **Feature 2660
stays open** until a Rebel server shows its three cases (the WebSocket closing exchange, a handler's outbound
call in its grace, one server stopped beside another) on Darwin and Linux. **Not adopted**: the stop token
(2660's own sketch), priced by the ffi-pragmatist at an argument on every waiting call of `net`, `tls`, `ipc`,
`process` and `fs/watch` and a slot per token; **unlisted and unpriced**, a stop asked of one thread's waits
(the layer's waiters carrying the runtime's thread number, which R3 of panel 254 already records), a question
for a later sitting.

### R11. 2688 and 2675: each library's, with a mailbox now able to carry them

Bytes a library holds above a drained socket are no wait's to see; **once R7 lands, a library may keep its parsed
messages in a mailbox it adds to the caller's set**, the ffi-pragmatist's inference, unbuilt. Both defects stay
with their lanes.

### R12. The Windows stop bell takes a stranger's datagram: a defect, drafted

`hero_poll.c:369-397` binds the Windows bell unconnected; any readiness of it fails every later `poll.wait`
`stopped` (`:1477-1479`, `:1504`). Its socket logic run on Darwin and Linux: a datagram from another process
makes it readable; connected to its own address, dropped (and the ring must then be `send()`, Darwin refusing
`sendto` with an address, errno 56). **The repair**: connect it, ring with `send()`, ask `hero_net_stop_asked()`
rather than trust the bell. Winsock unrun. Drafted for the coordinator (§ What this sitting files and owes).

### What conservative would have been, and what robust alternatives were not adopted

- **Q2, `gr`**: refuse in the body any send, spawn or answer of a letter's value and any `(mailbox T)` naming a
  letter (panel 155's shape, about 60 lines, sound by construction): it refuses the four live generics (certs'
  `start<S>`, iothub's `start_thread<S>`, two goldens) and makes `poll.add_mailbox<T>` unwritable, so Q1's face
  could not be written in Rebel. **`gc`**: one `@` for every letter's send; its `@` empties nothing at `i64`
  (`7` read after, measured) and aborts at `Boxed`, one line with two meanings. **A written bound** (`T: Send`):
  a core form by Part 5's test, nothing it refuses that R1 accepts.
- **Q1, the waker** (library only, no spec token): measured to lose values with every send ringing, all of
  them with a forgotten ring, and to wake on an empty queue before its value with an early ring.
- **The bell** as the mechanism (R8's fallback); **the forwarder per mailbox**, **the pump**, **the one wait in
  the runtime**, **a mailbox handed to C under a mark**: priced or read by the compiler-engineer (§ 5) and
  refused on the ceiling, the boundary or the forwarder's thread per mailbox; **an event-based Windows set**
  (`WSAEventSelect`, the critic's second reading, 2), unpriced, the likeliest Windows answer for R8, R9 and
  `add_signals` together.

## The prior art, per resolution

Re-read 2026-11-22 by this session (each page's own words checked; P0557r1 read on its page 3 from the file
the fetch saved; the Go `Server.Shutdown` page could not be read whole and is not cited):

- **R7, R8**: Rust's mio `Waker` (*"Waker allows cross-thread waking of Poll"*, `EVFILT_USER` on kqueue,
  `eventfd` on Linux, *"Only a single `Waker` can be active per Poll"*); libuv's `uv_async_send` (*"libuv will
  coalesce calls to uv_async_send(), that is, not every call to it will yield an execution of the callback"*);
  tokio's `Notify` (*"If `notify_one()` is called **before** `notified().await`, then the next call to
  `notified().await` will complete immediately, consuming the permit"*).
- **R9**: GLib's `g_main_context_wakeup` (*"Wake up `context` if it's currently blocking in
  g_main_context_iteration(), causing it to stop blocking"*; its example changes state, then wakes).
- **R1, R4**: Stroustrup, P0557r1, page 3 (*"The requirements of sort on its argument type are implicit
  ("hidden") in its function body"*; *"The error message for sort(d) will appear only when the template is
  instantiated, and that may be long after the point of call"*); Rust's auto traits (*"Structs, enums, unions,
  and tuples implement the trait if all of their fields do"*).
- **R3**: Go's *All your comparable types* (*"We have given up static type safety for a run-time check"*).
- **R2, R5, R6, R10, R11, R12**: none.

## Predictions to score

| seat | prediction | checkable at |
|---|---|---|
| spec-warden | S1 to S21 on the blind counts (registered 22:14), S26 to S28 (22:23); S6 to S10 and S17 to S19 scored on programs 1 to 6 as registered | the blind run |
| spec-warden | S22 to S25, the arms' real deltas | **scored here: four hits** (`q1` +66 in +55 to +80; `gc` +66 in +55 to +80; `gi` +106 in +95 to +130; `gr` +89 in +70 to +100) |
| coordinator | C1 to C18 (`blind/registration.md`, sha256 `fe6e3cb39fe73024...`), program 7 and the resolution's arm among them | the blind run |
| compiler-engineer | the census of `check` over the tracked tree at the landing's gate moves **zero** programs outside defect 2695's cases; `check/instances.hero` with its split **under 500 lines** at that commit | the lane's gate |
| ffi-pragmatist | on Windows, one datagram from another process to the stop bell's port makes the next `poll.wait` fail `stopped`, and every later one | the box or the CI's Windows leg |
| historian | precedent favours the obligation told against a declared letter; changed by a language that judges per instance with no declared requirement and whose designers call its messages adequate | a later reading (Nim, Zig, D) |

## The blind seat

**Prepared, not run, owed.** 46 sessions (t1 the wake, arms `w` and `m`; t2 seven programs, arms `now`, `gc`,
`gi`, `gr`; t3 a generic `deliver`, arms `gc`, `gi`, `gr`; one verdict, `now` against the resolution's `gi`),
drawn into `.claude/worktrees/p256-scratch/256-llm-ergonomist/`; the free preflight at 23:02:41: cost 0, no
file under `/private/tmp/claude-501/`, the scan clean (`prompt-alone=yes git-status=reader, no commit
claude-md-or-memory=no`). **The command and its folder are `256-briefs/blind/RUN.md`**, for the coordinator of
M-rebel-library: `bash /Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/p256-scratch/256-llm-ergonomist/run.sh`,
each session capped at 1.00 USD (the verdict 1.50), the run stopping before 19.50 USD. **What it does to this
resolution, registered**: a row read wrong by two or more of five `gi` readers reopens that row's text; D2
failing for `gi` where it holds for `gr` reopens Q2 toward `gr`; `lost(w) = 0` with `right(w) >= right(m)`
leaves the case against the waker on the stress runs alone.

## What this sitting files and owes

Drafted in `.claude/worktrees/rebel-scratch/coord/issues/`, **unfiled** (this sitting writes nothing to
GitHub): **the decision**, `p256-decision.md`; **a defect**, the Windows stop bell (R12),
`p256-defect-windows-stop-bell.md`; **a comment for defect 2695**, the class widened to the shapes the probes
and the critic found (`p256-comment-2695.md`).

## What the implementation lane builds (held to R1 to R10)

1. **The spec**: `resolution/spec-r.diff`, refreshed at landing, its commit body saying what paid (+206
   vendored, the rule of `691531908`).
2. **The checker**: `check/instances.hero` split under the 300-line ceiling, the prototype's switches removed;
   R2's refusal with `i9`; one message per call, rule and letter; a `check` case per probe of
   `256-briefs/probes/q2/` and the critic's `i2` to `i10` and the ffi-pragmatist's `p7r`, `p7w`, each annotated
   in its source.
3. **The emitter**: R3's 12 lines, with a `run` case aborting.
4. **`heroes doc`**: R4's printed needs.
5. **The runtime and the layer**: R6's table and R8's watch, its three rules; the stress of the ffi-pragmatist's
   `wait_routes.c` re-run on the real `mail.c` on Darwin and Linux; R8 (c)'s bound.
6. **Rebel**: `poll.add_mailbox`, `ui.add_set`, `ui.add_mailbox`; chat's doorbell and notes' rings replaced;
   `poll.ledger`'s `poll/wake` and `ui.ledger`'s `ui/mailbox` answered; `ui.hero:36-38` and `poll.ledger:145`'s
   stale premises rewritten.
7. **The gates the prototype left unrun**: the compiler's own tests, the golden forms, the census of `check`
   over the tracked tree (the 555 generics of `packages/` among them), and `latency.hero compare` on a still
   machine against the frozen compiler on `check selfhost/main.hero` and on a Rebel program.

## How the sitting went, for the next

- **The critic's two readings changed the resolution both times**: the first put the frozen text's own answer
  (`gi`) in front of every seat; the second unadopted a mechanism nobody had built in the runtime.
- **The machine was loaded** (load 27 to 114, other lanes); every clock was discarded by its own ratio, and the
  latency question is owed to a still machine.
- **A coordinator's error, caught before any spend**: the blind registration and brief carried three clock
  times their writer had not read (23:20, 23:25, 23:35); `date` read 23:02. They were removed and the
  registration re-hashed (`run.log`, 23:03), its predictions unchanged.
- **The contract the seats loaded was `main`'s** (critic 1): the briefs named the frozen files as binding;
  whether a subagent loads the session's checkout's agent files is an inference, unrun by a launch.

## Author's verdict

Pending.
