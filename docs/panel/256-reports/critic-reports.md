# Panel 256, the completeness critic, second reading: the reports before the synthesis

Written 2026-11-22 from 23:30, as it went. Read again: `00-shared.md` and the four seat briefs as repaired
(each *(critic N)* found in place), then `compiler-engineer.md`, `ffi-pragmatist.md`, `spec-warden.md` with
`spec-warden/`'s diffs and hashes, `historian.md`, and last the blind seat's state. Every command ran in my
copy, `.claude/worktrees/p256-scratch/critic/` (frozen commit `8aca3cc43`, its `heroes-next` `sources
031aaabb358218fd`), on COPIES of seats' files under `zcritic/r2/`; no seat's folder was built or run in, no
brief or report edited. No verdict on any route. The coordinator's provisional reading (Q2 `gi` by
`spec-wi.md`'s sentence with the 12-line emitter guard, § 9's widening queued, conservative `gr`; Q1 the
mailbox a set entry by the ffi-pragmatist's bell named to C by `m.number()`, Windows owed, `ui.add_set` and
`ui.add_mailbox`, 2660 by the server's mailbox and a counted SIGTERM) is what the findings attack.

## What was re-run and agreed (no finding)

- **The compiler-engineer's `gi` build closes the shapes beside the probes.** `heroes-gi2` copied
  (`shasum -a 256` equal to `compiler-engineer/heroes-gi2`'s, `--version` `sources 0e8d15027f0726af`) into
  `zcritic/r2/`, with `kinds.hero`; six shapes of mine, `check` by the frozen `heroes-next` and by the copy:
  `[Pointed]` handed to a generic `spawn` (`i2`), `Pointed?` (`i3`), a `cstr` leased from a `str` (`i5`), a
  `(thread i64)` sent bare through `relay<T>` (`i4`), a generic function **value** passed as an argument to
  another generic (`i7`), a generic called by UFCS (`i8`): **frozen exit 0 on all six; gi2 exit 1 on all six**,
  each with the right rule at the call and the body's line (`thread_crossing` *"`S` the type `[Pointed]` ...
  reaches `ptr`"*, *"`Pointed?`"*, *"`cstr`"*; `marker_mismatch` *"`(thread i64)`, which holds a thread"*, and
  twice *"`Boxed`, which holds a C handle"*). A mailbox made with no written type and no generic (`i1`) is
  `mailbox_needs_type` under both, so the written-type hole is reached only through a generic.
- **The ffi-pragmatist's veto condition is met by the compiler-engineer's build, which neither seat ran
  together.** FFI's `p7r_read_after_free.hero`, `p7w_write_after_free.hero`, `c7r_nongeneric.hero` and
  `cmem.hero` copied to `zcritic/r2/ffi-real/`: frozen `check` 0, 0, 1; gi2 `check` **1, 1, 1**, the two
  generic ones *"this call gives `S` the type `Pointed`, which reaches `ptr`, and `begin` makes a value of it
  cross between threads at p7r_read_after_free.hero:7"* (and `:8` for `p7w`).
- The spec-warden's counts are internally consistent (its table's deltas against 12,809; six refreshes, the
  brief's ceiling) and its predictions stand under the registered heading, timed 22:14 before its measures.

## Findings

### A. Question 1: the bell, as adopted

**1. The bell is built only as a model: no close, no stop, one queue that never dies.** `ffi256/q1/
wait_routes.c` (398 lines, read in place, nothing run there): `grep -n closed` prints lines 18, 69, 125, 192,
the field read and **never assigned**; no close function exists (`grep -c close`: 5, every one a comment, the
field or `epoll`'s flag); `hero_mail_hear_stop` has no counterpart; one static `Mail mail` made at start and
never freed. So the readiness the adopted sentence promises, *"told ready while it holds a value, is closed
or answers `stopped`"*, was stressed for its first third alone. The compiler-engineer's watch was built in a
copy of the real `mail.c` (+115 lines) and its C test covers close (*"ready once closed: 1, rings 2"*) and a
queue gone (*"-1"*), with one send and no stop. **Neither seat built the bell in `runtime/parts/mail.c`.**
`.claude/skills/panel/SKILL.md` § 3c (a route nobody built is not adopted) is the rule the synthesis meets
here.

**2. On Windows the bell cannot be a bell without network code in the runtime, and the precedent it is
named after is the one that left Windows unsupported.** `grep -rln -i 'winsock\|ws2_32\|WSAStartup\|
ws2tcpip' runtime/`: four headers and `runtime/platform/` only; `grep` of the same over `runtime/runtime.c
runtime/parts/*.c`: **nothing**. A Windows set is a `WSAPoll` array taking sockets alone (`poll.hero:286`,
and `:476`, *"add_stream: a pipe's reading end, or unsupported on Windows"*), so a mailbox's descriptor there
must be a socket. The shape the coordinator names, `runtime/parts/sigdoor.c`'s `hero_signal_fd`, answers
**`-1` on Windows** (`sigdoor.c:310`) and makes a Win32 event instead (`hero_signal_door_event`, `:315-320`),
which a `WSAPoll` set cannot take: that is why `poll.add_signals` is `unsupported` there. `fs/watch`'s bell
is a socket on Windows because `hero_watch.c` lives in `runtime/platform/`, the layer. So the seats
contradict each other and the checkable half is settled: the compiler-engineer (§ 5: *"puts Winsock calls into
`parts/mail.c`, and R6's measured reason ... is the one this would break"*) is right that no runtime part links
Winsock today; the ffi-pragmatist's *"on Windows a loopback socket connected to itself and rung with
`send()`"* places that socket in the runtime. *Windows owed* is then one of three things, none built: (a)
Winsock in `parts/mail.c`, R6's measured reason given up; (b) the layer makes the socket and the runtime rings
it through a pointer, which is the watch, on Windows alone, with finding 4's lock; (c) **unlisted: Windows'
set rebuilt over events** (`WSAEventSelect` into one event per set, waited with `WaitForMultipleObjects`),
which would take a mailbox's event, `sigdoor`'s event (closing `add_signals`' `unsupported`) and give
`ui.add_set` its Windows half through `MsgWaitForMultipleObjectsEx`; its limit of handles per wait and its
price are **unread and unrun** in this sitting.

**3. The bell's life contradicts the number's sentence.** The ffi-pragmatist: *"Its cost is a descriptor per
watched mailbox, whose life the set's entry must pin"* (verdict). `spec-wq.md`'s sentence: *"it holds no copy
of the mailbox ... once the mailbox's last copy is dropped it names no queue, never another"*; the
compiler-engineer refused *"a mailbox handed to C under a mark ... a counted reference it must give back"*. If
the set's entry pins the queue, C holds a count and the number names a queue after its last copy is dropped
(the sentence false, the refused route taken). If it does not, the runtime closes the bell when the last copy
drops while the set still holds the descriptor, and the next `open`/`socket` of the program may receive that
number: the array arm then polls another file under the mailbox's key, and the layer's `remove` acts on a
descriptor it no longer owns. `fs/watch` escapes it because a `Watch` is a handle the program closes and its
stream is added `borrowed: true` (`watch.hero:314-322`); a mailbox dies implicitly with its last copy, no act
written. **Nobody built or ran an entry that outlives its queue**, under either route; the watch answers it in
the compiler-engineer's test by the number reading `-1`, the bell has no test.

**4. The watch's stop ring runs under the layer's lock.** `hero_net.c:893-904`: `hero_net_nudge_all` is
*"Under the lock"* and calls `hero_mail_hear_stop()` last; the compiler-engineer rings watches *"in
`hero_mail_hear_stop` under the list's lock alone"*, so that ring runs under the layer's lock and the list's
together, and a ring that takes any lock of `hero_poll`'s which `hero_poll` ever holds while calling into
`hero_net` deadlocks. His constraint (*"a ring may take no lock a `hero_mail_watch` caller holds"*) is
narrower than this; his C test asked no stop. Read, not run. It weighs for the bell on POSIX and binds route
2(b) on Windows.

**5. Two smaller things the bell's model left out.** The Darwin pipe is made `O_NONBLOCK` and not
`FD_CLOEXEC` (`wait_routes.c:326`), where `sigdoor.c:124` sets `FD_CLOEXEC`: a runtime bell must, or every
child `process` starts inherits each watched mailbox's pipe. And two descriptors per watched mailbox against
this Mac's per-process default: `launchctl limit maxfiles` prints **`256 unlimited`** (my shell's `ulimit -n`
1048576 is raised); a program launched from the Dock starts at 256. Neither is a defect of a design; both
are a landing's.

**6. `ui.add_mailbox` is unrun; what ran is `ui.add_set`.** The ffi-pragmatist put a `kqueue` descriptor in a
`CFFileDescriptor` (AppKit) and an `epoll` descriptor in `g_unix_fd_add` (GLib), three runs each. A
mailbox's own pipe or `eventfd` handed to the user interface's loop was not run, and the one cost seen
(*"`CFFileDescriptorEnableCallBacks` is one-shot"*) meets a level source: a callback that re-enables while
the mailbox still holds a value fires again at once, so who drains, and when, decides whether TH01's loop
spins. Unrun.

**7. The 2660 half is a route whose program nobody wrote, and on Windows a mechanism that does not exist.**
The ffi-pragmatist: *"Unbuilt, said plainly: the smallest Rebel server of the brief ... was not written"*.
`hero_net.c:1444-1457` (read): `hero_net_count_arm` arms SIGINT alone and answers `HERO_NET_UNSUPPORTED` for
SIGTERM (*"which nothing sends a console program"*). With finding 3 of my first reading (no server in the
tree calls `poll.add_signals`), the adopted 2660 half is a mechanism used by no program, unbuilt as a
server, and absent on one of three platforms. And a call blocked outside every set (a worker's `recv`) is
reached by no server mailbox (the ffi-pragmatist's item 3): the second of 2660's three cases (*a grace for
work in flight whose handlers call a database*) then rests on each worker carrying a deadline, which no
seat measured.

### B. Question 2: `gi`, as adopted

**8. `gi` accepts in the body a generic no type can satisfy, and blames its caller.** `zcritic/r2/
i9_unsatisfiable_uncalled.hero`: `both<T>(m: (mailbox T), @v: T, w: T)` sends `m.send(@v)` and `m.send(w)`, so
it needs `T` to hold a handle and to hold none. Frozen `check`: **1**, `marker_mismatch` in the body. gi2
`check`: **0**. Called at `Boxed` (`i10`): gi2 says *"this call gives `T` the type `Boxed`, which holds a C
handle, and `both` sends a value of it without `@` at ...:5"*, the caller's mistake, where every type fails.
`spec-wi.md`'s sentence (*"a call giving it a type it does not serve is a compile error, told at that call"*)
makes the uncalled one legal. Rebel's `packages` declare 555 generics (my first reading, finding 17); a
package's author running `heroes check` on it hears nothing of such a body. A conflict between two needs of
one declaration is decidable on the needs set the build already makes; unbuilt.

**9. `spec-wi.md`'s *"these rules"* has the antecedent defect the spec-warden found in `gi`'s own text.**
The diff: R-base's sentence (*"A send is written `m.send(@v)` exactly where `v` holds a handle or a thread,
and `m.send(v)` everywhere else."*) then *"In a generic function these rules are judged at each call ..."*.
The coordinator reads *"the `ptr` half included"*; read by position, *"these rules"* may be R-base's send rule
alone, the sentence before it (the spec-warden's T3 against *"the other kind"*). The paragraph's `ptr`,
`cstr`, unit-mailbox and holds-itself rules are reached only if the reader takes *"these"* as the whole
section. Naming them (*"every rule of this section on what a type holds"*) is a few tokens.

**10. The adopted text has no real count.** Measured by `--refresh`: `gi` 12,915, `spec-wadopt.md` 13,083,
`spec-wcq.md` 13,074 (`spec-warden.md` § 2). `spec-wi.md` (*"about +172, an inference"*), `spec-wq.md`
(*"about +84"*) and their union, which is what the coordinator adopts (not `wadopt`, which adds § 9's sentence),
are **unmeasured**; the warden spent its six. My first reading's finding 25 again, one step later. And the
warden's S26 is registered for `wadopt`, not for `wq` + `wi`.

**11. The build behind the adoption is a prototype, and its every gate is unrun.** By the compiler-engineer's
own words: `instances.hero` 446 lines (326 code, *"past the 300 ceiling: a landing splits it"*), a switch read
from the environment (`P256_ROUTE`, `P256_GUARD`), *"Unrun: the compiler's own tests, the golden forms and the
census of `check` with the rule"*, the clocks *"nothing admissible"* (four runs, each `real` above `user +
sys`, load 29 to 114). Its census is 140 programs, 98 golden and 42 `examples-rebel/*/main.hero`; the test
blocks of `packages/` (555 generics), `examples/` and the rest of `tests/` were not checked. Under §
Precedence speed is rank 5, so the clock is a cost to report, not a gate; **who owes it, on what still machine,
and with which instrument** (`latency.hero compare`, the engineer's own falsifier) is unsaid.

**12. The historian objects to `gi` without a shown obligation; nobody built the answer, and nobody read
the languages that would falsify the objection.** `historian.md`'s verdict: *"object (advisory) to `gi` as
the adopted reading if it is adopted without a signature that shows the obligation"* (Stroustrup's P0557r1,
Ada's contract model). The compiler-engineer's answer is *"`heroes doc` printing the inferred need, not a
form"* (§ 4.4), **unbuilt and unpriced**; § 4.5 of the shared brief listed *"the obligation inferred and
shown in the signature by the formatter or `heroes doc`"* and no seat took it. The historian's own condition
names where its falsifier would be and says it did not look: *"Haskell, OCaml, D's templates, Zig's
`comptime`, Nim's generics"*. Nim is CLAUDE.md § 6's named model for this language's surface. What Nim's
generics check, and where, is a question this sitting left unread.

**13. The blind run cannot see what the historian and panel 155 object to, and its rules do not say what
it would undo.** t2's six programs hold **one generic each** (`grep -c -E '^function [a-z_0-9]+<'` on
`blind/t2-programs/prog*.hero`: 1 each), so no session reads a need carried through a chain (the
engineer's `n1`, `n6`), which is panel 155's llm-ergonomist condition and the historian's P0557r1 shape. And
D2 and D3 (`llm-ergonomist.md:131-146`) say *"read as written"*, nothing of a consequence: the synthesis adopts
`gi` before the run, the blind seat holds a veto on non-local constructs, and `gi` is the route whose
acceptance turns on a body's hidden use. Which result of the run moves the adoption to `gr` (or to a shown
obligation) is registered nowhere.

**14. The `==` rule the resolution leaves the spec contradicting.** The spec-warden's T7: panel 155's R2,
`==` on a bare letter refused in the body (`generic_equality`), has no sentence, and § 7 reads *"`==` is
structural equality on any two values of one type"*. Not repaired by any text; queued with `spec-w9.md`, it
would be the second rule on a letter the spec does not state.

### C. The blind seat

**15. There is nothing to read yet, and the brief says so.** `ls p256-scratch/`: `compiler-engineer coord
critic ffi-pragmatist spec-warden`, **no `256-llm-ergonomist/`**; `ls -la 256-briefs/blind/`: the same ten
entries as at my first reading, the task files dated 21:41, no `run.sh`, `RUN.md`, `registration.md`,
`tools/` or `reference/`. `llm-ergonomist.md:10-15`: *"The area is built after the synthesis, and the run is
one run"*. So the runner, the registration, the voiding tools and the reference programs (critic 26's
repair, `:157-161`) are unread by anyone but their future writer; no preflight, scan or `run.log` exists.

## What the synthesis must settle

1. **Whether the bell is adopted while unbuilt in `mail.c`** (finding 1): its close, its stop and a queue
   that dies are untested by anyone; the watch is the route built in the real runtime. SKILL.md § 3c.
2. **The bell's life against `m.number()`'s sentence** (finding 3): the set's entry pins the queue (C holds
   a count, `spec-wq.md`'s *"holds no copy ... names no queue"* false) or it does not (a closed descriptor the
   set still polls). One of the two must be built and run, and the sentence follow it.
3. **What "Windows owed" is** (finding 2): Winsock in `parts/mail.c`, the watch on Windows alone, or the
   unlisted event-based Windows set; the cited `sigdoor.c` precedent answers `-1` there today.
4. **Whether a generic whose needs conflict is refused at its declaration** (finding 8), and the text
   saying so; today gi2 accepts it uncalled and blames every caller.
5. **What the blind run's result does to an adoption made before it** (finding 13), and whether a task with
   a chained need is added, since none of t2 has one.
6. **`spec-wi.md`'s *"these rules"*** named so the `ptr` half is unmistakable (finding 9), and **a real
   count of the adopted `wq` + `wi` text** with its own registered prediction (finding 10).
7. **The shown obligation** (`heroes doc` or the formatter) built or refused with a reason, since the
   historian's objection rests on its absence; and **Nim's generics read** before the precedent is called
   one-sided (finding 12).
8. **The 2660 half recorded as unbuilt**, its Windows absence (SIGTERM not countable) and the worker
   outside every set named (finding 7), rather than adopted.
9. **Who owes gi's clock, the compiler's own tests, the net and a census wider than 140 programs**
   (finding 11), and whether the guard ships behind an environment switch or unconditionally.
10. **The watch's stop ring under the layer's lock** (finding 4) if route 2(b) keeps it on Windows; and the
    bell's `FD_CLOEXEC` and descriptor budget (finding 5), `ui.add_mailbox`'s level source in a one-shot
    callback (finding 6), and § 7's `==` sentence (finding 14), each named in the resolution as owed.
