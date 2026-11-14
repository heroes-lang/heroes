# 248: the C library's installers wait for a census, the layer's waits end on one poller, a child gets its folder, and a program asks its own stop and ends a quiet connection

**Status** a full sitting, convened 2026-11-14 by the coordinator of M-core-packages under CLAUDE.md
§ 4 without asking again (the milestone's gate was asked at panel 242, `248-briefs/00-shared.md:38-49`),
on five questions, each a mechanism of the runtime or of its platform layer and the face it shows
(`00-shared.md:12-15`): the C library's handler installers and the runtime (defect 2522, panel 247's R3
residue), the platform layer's waits (defect 2502), a child's working folder (defect 2490), a stop the
program asks itself, and a deadline on a connection that sends nothing. **First drafted for the
soundness lane and escalated to the full panel on the completeness critic's item 23**
(`248-reports/critic-briefs.md:601-605`, `00-shared.md:77-107`): three of the five have a sentence of
the specification or a diagnostic class behind them. **The paid runs, approved by the author at 10:30**,
a ceiling of 25 USD in all (`00-shared.md:17-23`): the blind seat's forty sessions, **12.9748 USD**
(`248-reports/critic-reports.md:224-227`), and the spec-warden's `heroes measure --refresh` calls, 10
invocations and 30 requests (`248-reports/spec-warden.md:122-123`, `:167-168`). The resolution is
`provisional — author ratification pending` (§ Author's verdict).
**Seats** the full panel: compiler-engineer, ffi-pragmatist, spec-warden, historian (advisory),
llm-ergonomist (the blind seat, forty sessions, ten a cell). The completeness critic read the briefs
before any seat (29 items, `critic-briefs.md:498-619`; the briefs repaired on them, the first versions
kept as `248-briefs/*.before-critic.md`), the three briefs added after (15 items, `critic-briefs.md:791-853`,
the texts before kept as `*.before-second-pass.md`), and the reports after (20 items,
`critic-reports.md:248-345`).
**Base** `df7612ad3dc62dcc6d90bccc650adf6b9736078f` (tree `f72f48da250e...`), batch 35's round closed,
holding panel 247's landing; stage 2 prints `sources 4c9a6a9800095eef` on this Mac and in Linux arm64
(`00-shared.md:109-133`). Each seat built its own in `.claude/worktrees/p248-scratch/248-<seat>/`.
**Platforms** Darwin arm64 (this Mac), Linux arm64 (the Docker image), and **Windows on the box, which
was off when the briefs were written and which the author switched on at 10:31**
(`00-shared.md:256-258`; `248-reports/compiler-engineer.md:289`; `248-reports/ffi-pragmatist.md:14-15`):
both soundness seats and the critic ran their Windows rows there, clang 23.1.1 with its MSVC target.
**Briefs** `docs/panel/248-briefs/`, **reports** `docs/panel/248-reports/`.

## The proposal

Verbatim in `248-briefs/00-shared.md`, § Question 1 to § Question 5 (lines 276 to 837); in short:

- **Q1, defect 2522**: a library of the program built `-O2 -fomit-frame-pointer` installs its own
  handler and calls the program's callback from it; the callback passes R1's three questions and D.
  On Darwin the process dies by SIGKILL in silence in the locale lock, and a C program alone dies the
  same way (`00-shared.md:298-325`); in Linux arm64 the Heroes function runs inside the signal. Panel
  247's R4 asked a soundness sitting's ruling before the runtime defines names of the C library
  (`docs/panel/247-*.md:197-211`). The routes: **I** (R4's corrected form, a wrapper and a per-thread
  depth), **I-dylib**, **I-onstack**, **B** (signals blocked on claimed threads), **T** (a stack scan),
  **W** (`--wrap`), **R** (`objcopy`), **U** (unwind tables), **M** (the mask, a question), **K** (the
  residue kept, which R3 forbids) (`00-shared.md:446-518`).
- **Q2, defect 2502**: every wait of the layer polls in slices of 50 ms. The routes (a) a wake
  descriptor per waiting thread (lane b35-runtime's), (b) one shared wake, (c) one per socket, (d) the
  status quo, (e) **one poller for the process**, (f) a thread-directed signal; on Windows
  `WSAEventSelect`, a loopback socket pair `WSAPoll` can watch (the coordinator's addition), or the slices
  (`00-shared.md:623-642`).
- **Q3, defect 2490**: `hero_run_go` gives a child no folder, so the net starts four cases through
  `/usr/bin/env -C` and skips them on Windows; the shapes `chdir` in the child, **`fchdir` on a folder
  the parent opened** (the critic's), `posix_spawn_file_actions_addchdir`, or a `heroes -C` flag
  (`00-shared.md:719-740`).
- **Q4**: a door such as `hero_net_ask_stop()`, and what a Ctrl-C after it means (`00-shared.md:783-789`).
- **Q5**: a deadline per call, per socket (Go's `SetDeadline`), in `net/http` alone, or a join that does
  not wait, in two shapes (`00-shared.md:823-837`).
- **The spec's drafts** (`spec-warden.md:39-49`, `:163-168`): **q4m**, line 404 merged, *"Ctrl-C,
  SIGTERM and `hero_net_ask_stop() -> i64` only make ..."*; **q4s**, the first of the three only makes it
  answer 1 *"and a signal after it ends the program"*; **q4t**, **q4a** (a thread clause); **q5j**,
  **q5n**, **q5o** (line 377 and a join); **q5d** (a deadline); **q3a** (`hero_run_dir`).

## The verdicts

| seat | verdict | section | cost | prediction | condition |
|---|---|---|---|---|---|
| compiler-engineer | Q1 **approve I-onstack** (built), **object** to R4's wrapper-and-depth, B, T, W, R, I-dylib; Q2 **approve route (g)** (built), **object** to (a); Q3 **approve the door** (built), **object** to a `heroes -C` flag; Q4 **approve `hero_net_ask_stop()`** (built, sharing the count) and `core` 0.4.0's face; Q5 **approve a deadline per socket** (built), **object** to a join that does not wait and to a limit in `net/http` alone; **no veto** (`compiler-engineer.md:553-690`) | design.md §1.12, §4.20, §1.7, Part 5 | `siginstall.c` 250 lines, `hero_net.c` +214, `run.c` +91, headers +42; `selfhost/` 0, emitted C 0 (`:559-560`, `:569-572`) | R3's two cases refused on every CI leg, `qsortcost` within 0.01 %, exactly the two D cases of 2494 move (`:582-585`); `waiters-checked 100 wait 10` under 80 million instructions (`:617-618`); fetch 2479 and the two 2344 cases run on Windows (`:641-642`); t4 compiles at least as often with no result (`:662-663`); the narrow shape answered within the deadline plus 1 s (`:684-685`) | Q1 a correct program refused, **a library whose handler is measured to need the interrupted stack**, or ASan's report lost (`:586-589`) |
| ffi-pragmatist | Q1 **approve I-mask** (built), **veto I-onstack**, **veto B**, **veto T**, **object** to U, W, R; Q2 **approve one poller** (built as probes on three systems), **object** to a descriptor per waiting thread and to a thread signal; Q3 **approve the door in the `fchdir` shape**; Q4 **approve the door kept apart from the signals' count**, **object** to sharing it; Q5 **approve a per-socket deadline at the layer's wait**, **object** to leaning on `SO_RCVTIMEO` (`ffi-pragmatist.md:691-842`) | §1.11, §4.19, §1.12 for the vetoes | I-mask 1,346 instructions a callback on Darwin (`:381-385`); a link change on Darwin, a dylib beside the program (`:740-743`) | bdwgc intact under I-mask, faulting under I-onstack, already run (`:717-720`); a Homebrew-shaped dylib refused 20 of 20 (`:720-722`); the differential test fails on a C library it was not written against (`:722-725`); the poller within 2 of the base's count, under 1 % of the slices' instructions (`:764-766`) | Q1: I-mask withdrawn **if a library a binding plausibly links needs the marker signal itself, "blocked by it outside a handler through a raw system call, or waited for"**, or the `longjmp` family dropped (`:726-731`) |
| spec-warden | **approve q4m** conditional on the door keeping the program's ask apart, else **q4s**; **object** to q4t, q4a, q4z (if the door lands); **veto** the sentences q5j, q5d, q3a (Principle 0), the doors at 0 tokens; **veto** q5n and q5o as replacements of the waiting join; **approve** no sentence for Q1 and Q2 (`spec-warden.md:226-418`) | design.md §1.6, §1.0, §4.17; CLAUDE.md § 12 | real counts, measured: q4m **+18** (11285 to 11303), q4s +33, q4t +22, q4a +28, q5n +23, q5o +29, q5j +30, q3a +33, q5d +56 (`:127-137`, `:163-168`); **no budget veto possible** (`:139-144`) | a q4m spec arm, 3 sessions, against t4-q: at least 1 of 3 under x signals itself or names *"only"*, 0 of 3 under q4m (`:246-255`) | q4m false if the door shares the count (`:256-258`) |
| historian | **approve (advisory)** each question's precedented route; **object (advisory)** to route I from the executable alone on Darwin and to `hero_thread_join` itself answering "not yet" (`historian.md:65-82`) | precedent | | | dyld honouring an executable's own `__interpose`; a runtime defining every installer its C library exports; a thread library changing its join (`:269-287`) |
| llm-ergonomist | an experiment, graded by its registered rule (§ The blind seat) | design.md Part 11 | 12.9748 USD | the decision rule, registered before any session (`248-briefs/llm-ergonomist.md:314-321`) | |

## What each seat measured, in short

- **compiler-engineer** (`compiler-engineer.md`): route **I-onstack** built as `runtime/parts/siginstall.c`,
  the runtime defining the installers each C library exports and adding `SA_ONSTACK`, so R3's handler
  runs on the alternate stack every claimed thread already holds and R1's existing second question
  refuses it (`:45-76`). R3's two cases **refused 20 of 20 each on Darwin and in Linux arm64**, before
  `localeconv`, the reports SIGABRT and none SIGKILL (`:78-92`, `:197-204`, `:536-538`), under
  `--sanitize` too (`:152-160`); a Darwin dylib **not refused, 5 of 5 `answer ran for 30`** (`:150-151`);
  a stale `SS_ONSTACK` on Darwin after `_longjmp` (`:138-145`). A route nobody listed for Q2, **the
  close's own shutdown as the wake**, measured on three systems (`:208-223`), built as route **(g)**:
  100 waiters for 10 s 72.8 to 73.2 million instructions against 504.6 to 506.0 million (`:242-249`), a
  listener's close 32 µs against 27,390 µs (`:254-258`), a server N + 8 descriptors (`:259-268`). The
  folder door built and run on the box (`:272-304`), the net's four cases moved and green (`:319-342`).
  The stop door and the deadline built in `core` 0.4.0 (`:404-489`).
- **ffi-pragmatist** (`ffi-pragmatist.md`): dyld **ignores an executable's `__interpose`** and honours a
  dependent dylib's (`:50-64`); one interposed name, `__sigaction`, sees every Darwin installer (`:77-84`);
  a differential test of every installer against its C library, 1,284 and 1,128 rows, 0 mismatches
  (`:113-128`). **Found after §§ 1.3 to 1.8: forcing `SA_ONSTACK` makes a collector's suspend handler
  record a stack pointer outside its thread's stack** (`:319-347`), and the real **bdwgc 8.2.8 in Linux
  arm64: 5 of 5 intact at the base, 5 of 5 `Segmentation fault` under I-onstack, 5 of 5 intact under
  I-mask** (`:424-439`). Route **I-mask**, which none of the briefs listed: a marker signal (SIGURG)
  added to every handler's mask, the thunk asking the thread's mask (`:349-363`), R3's cases refused
  20 of 20 on Darwin and in Linux arm64 (`:373-379`, `:446-450`); a false refusal after glibc's plain
  `longjmp`, repaired by defining the `longjmp` family (`:399-407`, `:441-444`). B, T and U run and
  failing (`:181-204`). Route I by name **cannot link on Windows** (`:270-275`), and Windows has no
  asynchronous delivery onto a claimed thread (`:284-290`). Q2's five wakes measured on three systems
  (`:459-526`); Q3 49,000 and 71,500 children of a threaded parent with no hang (`:553-582`, `:782-783`);
  Q4 both designs on three systems (`:610-653`); Q5 `SO_RCVTIMEO` reaching no wait of the layer's and the
  deadline at the wait (`:655-689`).
- **spec-warden** (`spec-warden.md`): every draft priced, real counts by `--refresh` on the coordinator's
  direction (`:109-137`); a reader's route today, its own SIGTERM through `kill` and `getpid`, works on
  POSIX at one correction round (`:73-86`); a second SIGTERM ends the program, exit 143, so line 404's
  *"only"* was never whole under one reading (`:172-187`); **one quiet connection first stopped the
  example server after 255 requests, in silence**, on Darwin and Linux arm64 (`:96-101`, `:152-153`).
- **historian** (`historian.md`): runtimes define the C library's installers in short lists (OpenJDK,
  ART, compiler-rt), none complete (`:88-121`); **ART forces `SA_ONSTACK` and Go turns it on** in
  handlers it finds (`:98-104`, `:117-121`); one poller per process with one wake is the norm (Go, libuv,
  mio) (`:128-140`); libevent's loopback pair *"does not work when localhost is down"* (`:143-146`);
  **Go's deadline is absolute and reaches pending calls** (`:54-58`, `:197-200`); Node added header and
  request deadlines as security fixes (`:206-213`); a non-waiting join always stands beside an unchanged
  join (`:218-222`).
- **The critic's pass over the reports** (`critic-reports.md`): the veto's measurement **re-run and
  holding** (`:16-31`); **I-mask breaks a program that blocks and waits for SIGURG**, misses a handler
  that unblocks first, and clears an inherited SIGURG block by a side effect (`:38-78`); **Go uses
  SIGURG for preemption and throws on a handler without `SA_ONSTACK`** (`:51-61`); the ffi-pragmatist's
  base close latency is not the base's (`:117-129`); the harness gives clang `;` on Windows and clang
  reads the case's header (`:142-159`); on Windows a program has no way to ask its own stop through any
  door of the base (`:178-185`); the deadline has three meanings in the sitting's files (`:189-206`).

## The blind seat, graded by its registered rule

**The forty** (`248-briefs/llm-ergonomist.md`, registered before any session): tasks **t4**, a server
that ends itself on `GET /quit`, and **t5**, a server that closes a connection that sends nothing, each
under today's card (arm **x**) and a candidate face (arm **q**, `ask_stop() -> ()?` and `stop_asked() ->
bool`; arm **d**, `set_deadline(c: Conn, ms: i64) -> ()?`, *"a `read` or `write` on `c` that waits more
than `ms` milliseconds fails `timed_out`"*), ten sessions a cell, each capped at 0.40 USD
(`llm-ergonomist.md:23-45`, `:69-70`). **The rule**: a face helps if its arm gives at least three more
`right` than x over the ten sessions of its task, both graded under the setup that has the door
(`:314-321`). **The spellings graded are the cards' candidates**, written before any seat built a door
(the coordinator's note of about 10:45, `:411-413`).

**What ran**: 40 sessions, 37 `success` and 3 `error_max_budget_usd` with an empty result
(`248-reports/llm-ergonomist.md:10-13`); **12.9748 USD in all, the largest 0.5323**, past the 0.40 cap
and the 0.496 the brief assumed as a session's worst (`critic-reports.md:224-227`,
`llm-ergonomist.md:225-229`); every session under the base's spec, so **the spec-warden's q4m arm did
not run and its prediction stays unscored** (`critic-reports.md:226-228`, `:337-340`). The grading
setups were pinned before any answer was graded, arm q's door given the card's `-> ()?` and arm d's the
card's limit counted from each call, as the thinnest changes over the compiler-engineer's build
(`248-reports/llm-ergonomist.md:33-97`); each reference read 5 of 5 right (`:106-110`).

**The grading** (`248-reports/llm-ergonomist.md`, finished at about 12:37; read as **about the cards'
faces**, not about the faces R4 and R5 adopt, R6). Every answer that built was built three times and run
twice, every refused one gave the same error on its three builds, and three verdicts first read under a
load of 62 to 175 read the same again at about 20 (`:121-155`):

| cell | right | refused | stopped | silent | hung | right of those that answered |
|---|---|---|---|---|---|---|
| t4-x | **0** | 9 (2 no answer) | 1 | 0 | 0 | 0 of 8 |
| t4-q | **6** | 4 | 0 | 0 | 0 | 6 of 10 |
| t5-x | **0** | 5 (1 no answer) | 0 | 5 | 0 | 0 of 9 |
| t5-d | **9** | 1 | 0 | 0 | 0 | 9 of 10 |

(`:204-212`). **By the registered rule both faces help: t4, q 6 against x 0, +6; t5, d 9 against x 0,
+9**, the threshold being +3; each holds with the three budget-ended sessions left out (6 of 10 against
0 of 8; 9 of 10 against 0 of 9), and the ten d answers read the same nine right and the same refusal
under the seat's own absolute deadline, so the t5 verdict does not depend on which meaning was built
(`:234-245`). **The wrong ones, by cause** (`:214-232`): a block arm written with no `=>`
(`expected_arm_arrow`), 8 of the 37 answers and in every arm; C's `raise` bound to signal its own
process, refused as a reserved word with the one-line shim in the message, 7 of the 8 t4-x answers, none
using `kill` and `getpid`; one t4-x answer packing `net.number(...) * 65536 + port` into one `i64` and
aborting on the overflow, a connection's number measured at 4611686018427518979, about 2^62, which `net`'s
card does not mention; one t5-x answer guessing a C door through a module the package does not have; and
five t5-x answers saying the card offers no limit, leaving the quiet connections open past 10 s.
**Beside the rule, not counted by it** (`:246-268`): with only the compiler's own printed fixes applied
once, q and d each read 10 of 10, t4-x 6 of 10 and t5-x 0 of 10, gaps of +4 and +10: the t4 gap at
first try is mostly `raise`, the t5 gap the face alone. **Context** (`:270-280`): 37 of 37 that answered
said `no`; one names the working directory's `heroes-lang`, recorded and not voided as the brief says of
the path; none names `CLAUDE.md`, a setting or a hook, so none is voided. **Not graded** (`:295-308`):
the three budget-ended sessions' answers (none exists), a t5 reference under today's card (none is
known), any platform but Darwin, and the write half of the card's limit, built and unrun.

## The resolution adopted

`provisional — author ratification pending`. Robust over conservative wherever the two differ, the
conservative form recorded in § Conservative alternatives.

**R1, question 1 (defect 2522): no route is adopted; 2522 stays open and `blocking`.**

- **I-onstack is not adopted.** The ffi-pragmatist's veto stands on a measurement the critic re-ran:
  bdwgc 8.2.8 in Linux arm64, **5 of 5 intact at the base and under I-mask, 5 of 5 `Segmentation
  fault` (exit 139) under I-onstack** (`ffi-pragmatist.md:432-436`; `critic-reports.md:23-27`, item 1
  at `:250-255`), the collector's suspend handler storing its own frame, `GC_approx_sp()`, as the
  thread's stack pointer (`ffi-pragmatist.md:325-333`). **It meets the compiler-engineer's own
  withdrawal condition**, written before he saw it, *"a library whose handler is measured to need the
  interrupted stack"* (`compiler-engineer.md:586-589`), which his report never weighs
  (`critic-reports.md:29-31`). And it leaves the **Darwin two-level dylib residue, 5 of 5 `answer ran for
  30`, exit 0, not refused** (`compiler-engineer.md:150-151`), of the class panel 247's R3 calls *"Never a
  known cost"* (`docs/panel/247-*.md:191-195`; the critic's item 7, `critic-reports.md:280-284`). **The
  veto's program is Linux's**: bdwgc's Darwin path suspends a thread by `thread_suspend` and reads its
  registers by `thread_get_state`, no signal handler, so on Darwin I-onstack is not shown to break a
  collector, by the critic's reading, unrun (item 2, `critic-reports.md:32-36`, `:256-259`). **Against
  the veto, recorded**: two production runtimes want what I-onstack does, ART (*"~SA_ONSTACK: always
  silently enable this"*, `historian.md:98-104`) and Go (*"If the non-Go code installs any signal
  handlers, it must use the SA_ONSTACK flag with sigaction"*, `historian.md:117-121`; *"non-Go code set up
  signal handler without SA_ONSTACK flag"*, thrown at `signal_unix.go:1133`, the critic's item 4,
  `critic-reports.md:51-61`).
- **I-mask is not adopted.** It meets its own author's withdrawal condition (`ffi-pragmatist.md:726-729`):
  **a correct C program that blocks SIGURG and waits for it in `sigtimedwait` loses it** under the route,
  *"nothing in 2 s"*, its block dropped by the route's mask calls (the critic's item 3,
  `critic-reports.md:40-50`). Beside it: a handler that unblocks before calling back passes the route, a
  miss with the walk and D beneath (item 5, `:62-65`); an inherited SIGURG block is cleared by a side
  effect of the probe's restore, which the route's correctness then leans on (item 6, `:66-78`); **on
  Darwin every program that hands C a function would need a dylib beside it**, `heroes build` giving a
  program no `LC_RPATH` (*"no LC_RPATH's found"*, exit 134), dyld refusing it under the hardened runtime
  (*"different Team IDs"*, exit 134), resting on an `__interpose` that `man dyld` does not document
  (`ffi-pragmatist.md:59`, `:70-75`, `:155-158`; the critic's item 8, `critic-reports.md:93-98`); and
  **Go preempts goroutines by SIGURG** (`const sigPreempt = _SIGURG`, `signal_unix.go:74`, item 4).
- **The follow-up, owed before 2522 can close**: a sitting of its own, convened when this one's
  ratification is read, on **the question the critic says this sitting should have asked** (item 20,
  `critic-reports.md:230-246`, `:341-345`): what each route changes in what a program's own C observes
  (its handler's stack, its thread's mask), and which libraries a Heroes program plausibly binds depend
  on it (collectors reading their own frame, Go's `c-archive`, ART, SIGURG's users), and who owns
  SIGURG; that census made first, each candidate run against its first entries. **The candidates, named
  and unbuilt**: a marker the runtime owns that no library uses (a real-time signal on Linux; Darwin's
  SDK declares no `SIGRTMIN`, by this synthesis's `grep -n -E 'SIGRTMIN|SIGRTMAX'` of its `signal.h` and
  `sys/signal.h`, exit 1); a wrapper handler with a per-thread depth, the table both building seats set
  aside (`compiler-engineer.md:493-500`, `ffi-pragmatist.md:91-98`); and frame records kept in every unit
  the compiler builds (`-fno-omit-frame-pointer`). For the last, the coordinator's comment on 2522 of
  2026-11-14 records **three of defect 2494's own cases red at `-O2` on the CI's Linux x86-64 leg** (run
  43571761187, job 132377462166: `run` 803 and 3), passing at `-O0` there and at every level on Darwin and
  Linux arm64, and gives their cause, clang omitting frame pointers at `-O2` on x86-64, as **an inference,
  unrun for want of an x86-64 machine**; so that frame records in every unit would close those three is an
  inference too, and a library built `-fomit-frame-pointer` stays its residue.
- **What stands meanwhile**: panel 247's R1 and R2 (the thunk's three questions and the allocator's
  guard), D beneath them (`docs/panel/247-*.md:153-189`); 2522 blocks M-core-packages' tag as before
  (`00-shared.md:342-343`).

**R2, question 2 (defect 2502): one poller for the process.** The ffi-pragmatist's route
(`ffi-pragmatist.md:748-763`): `kqueue` on Darwin, `epoll` on Linux, and on Windows a `WSAPoll` thread
woken by a loopback socket pair, since `WSAPoll` handed an event answers **-1, `WSAENOTSOCK`**
(`:507-508`), so no wait slices on any platform. Measured as probes on three systems, not yet as the
layer: 100 waiters on Darwin **48,959,302 / 82,345,853 / 52,524,281 instructions at 1 / 5 / 10 s against
the slices' 123,212,576 / 227,234,879 / 397,831,358** (`:466`, `:469`); in Linux arm64 0.016 s of CPU
against the slices' 0.143 s at 5 s (`:497-501`); on the box **1,732,250 cycles against 395,168,250**
(`:515-526`); **a close answered in 24 to 27 µs on all three systems** (`:469`, `:500`, `:524`, `:756`);
**a server holding 249 connections under a soft limit of 256 against the base's 250, a pipe per waiting
thread 83, a descriptor per waiting thread 125**, on Darwin and Linux alike (`:485-491`, `:501-503`;
agreed by both seats, `critic-reports.md:131-132`). **It closes 2502 on Windows too, where route (g)
keeps the slices** (`compiler-engineer.md:392-394`; the critic's item 10, `critic-reports.md:296-300`).
**The landing owes**: the poller built in `runtime/platform/hero_net.c`, with **its own close path built
and measured, Linux x86-64 among the legs**, since both routes rest on a shutdown waking a waiting
connected socket (the poller probe's wake is `shutdown(w[i].sock, SHUT_RDWR)`, `wake.c:103`, `:106`),
measured on Darwin and Linux arm64 and not on Linux x86-64, and a real poller's close path is unbuilt
(item 10, `critic-reports.md:106-115`); **libevent's caveat answered**, a loopback pair *"does not work
when localhost is down"* (`historian.md:143-146`), by falling back to slices where the pair cannot be
made; and **the ffi-pragmatist's base latency corrected** (item 9, `critic-reports.md:117-129`,
`:290-295`): his slice shape never shuts the socket down, while at the base a waiting `recv` ends 133 to
139 µs after the close and an `accept` 27,464 to 29,021 µs, so the gain is idle CPU and a Darwin
listener's close, not a connected socket's. `hero_net.h`'s two *"within about 50 ms"* become *"at once"*,
a header promise told (`ffi-pragmatist.md:771-772`; CLAUDE.md § 2). Conservative, recorded below:
route (g).

**R3, question 3 (defect 2490): the door `hero_run_dir`, in the `fchdir` shape.** Both building seats
approve it (`compiler-engineer.md:625-628`, `ffi-pragmatist.md:776-780`): the parent opens the folder
(`O_DIRECTORY | O_CLOEXEC`) before the fork, the child opens its three streams and then `fchdir`s, the
last call before `exec`, `fchdir` being on POSIX's async-signal-safe list (`00-shared.md:686-688`); on
Windows the folder is `CreateProcessW`'s `lpCurrentDirectory`. **Every relative path of a launch, the
program's included, is read in the parent's folder**: what `CreateProcessW` does, measured on the box by
both seats (a relative folder resolved in the parent's, a missing one and a file both refused with 267,
a program named bare found in the parent's folder, `compiler-engineer.md:289-304`,
`ffi-pragmatist.md:589-603`), POSIX made to match, where `execvp("./x")` after a `chdir` runs the child
folder's `x` (`ffi-pragmatist.md:568-570`). **A folder that cannot be opened is refused in the parent
before any child exists**: `HERO_OS_NOT_FOUND` with `hero_run_why` the errno, the vocabulary a failed
start already has (`compiler-engineer.md:276-280`), ENOENT, EACCES or ENOTDIR on POSIX, measured on both
legs (`ffi-pragmatist.md:563-566`, `:572-573`); Node's single ENOENT for a missing folder and a missing
command (`historian.md:177-179`) is the precedent recorded against telling them alike. **The net's four
cases use the door instead of `/usr/bin/env -C`**, green on this Mac (`compiler-engineer.md:319-342`);
**whether the two `fixedbugs-2344` folder cases pass on Windows is owed to the CI's leg**: the critic's
item 11 struck both seats' contrary predictions, the harness giving clang `;` for `:` there and clang
reading the case's header first (`critic-reports.md:142-159`, `:301-307`). **No spec sentence**: the
spec-warden's veto of q3a stands (real +33, Principle 0's burden unmet, the door at 0 tokens in panel 242's
R6 shape, `spec-warden.md:378-393`) until a need is measured; the ffi-pragmatist's *"Needs a spec
sentence"* is recorded (`ffi-pragmatist.md:797-799`; the critic's item 17, `critic-reports.md:331-333`).

**R4, question 4: `hero_net_ask_stop()`, kept apart from the signals' count.** A program asks its own
stop from any thread, with no signal; every wait answers `stopped`, on all three systems
(`ffi-pragmatist.md:623-636`), ThreadSanitizer 0 warnings in 8 runs over the two POSIX legs
(`:626-632`, `:806-808`). **Kept apart**: the first Ctrl-C after an asked stop does not end the process
and the second does, measured on all three systems (`:638-653`). **The compiler-engineer's built door
shares the count**, the asked stop counting as the first request, so a first Ctrl-C then ends the
process, exit 130 (`compiler-engineer.md:410-414`, `:419-422`); **the landing separates it** (the critic's
item 12, `critic-reports.md:163-170`, `:308-312`). **On Windows today a program has no way to ask its own
stop through any door of the base**, run on the box, `raise(SIGTERM)` exit 3 unarmed and armed (the
critic's item 15, `critic-reports.md:178-185`), so the door is the only route there. **Spec line 404
takes the spec-warden's q4m** (+18 real tokens, 11285 to 11303, `spec-warden.md:130`, `:235`), the
sentence true of the kept-apart design (`:189-196`, `:256-258`). **The face**: `net.ask_stop()`
returning nothing, since the door cannot fail (it answers 0 whatever stood), and `net.stop_asked() ->
bool`, in a `core` 0.4.0 (`compiler-engineer.md:436-444`). The blind seat's card spelled `ask_stop() ->
()?` (`248-briefs/llm-ergonomist.md:44`); its result is read as about that face (R6).

**R5, question 5: a deadline per socket, at the layer's wait.** **One meaning, Go's**: an absolute time
per socket, applying to future and pending waits (the historian's precedent, *"The deadline applies to
all future and pending I/O"*, `historian.md:54-58`, `:197-200`), kept in the socket's table entry so it
crosses to a connection's thread with the socket's number (the ffi-pragmatist's shape,
`ffi-pragmatist.md:824-826`, `:832-834`), set from now by **`net.set_deadline(c: Conn, ms: i64) -> ()?`**,
0 taking it away (`compiler-engineer.md:463-471`). **Measured**: at the wait, quiet peers **timed out 20
of 20** on Darwin, Linux arm64 and the box, and a byte sent 50 ms before the deadline **delivered 20 of
20** on each (`ffi-pragmatist.md:668-678`); the compiler-engineer's door freed the example server under
one quiet connection and under eight, the new client answered after 2.4 to 2.5 s
(`compiler-engineer.md:473-478`). **Windows clocks it with `QueryPerformanceCounter`**: under
`GetTickCount64` the worst lateness measured was 16.83 ms, which Microsoft's page puts to the system
timer's 10 to 16 ms resolution (`ffi-pragmatist.md:677-682`). `SO_RCVTIMEO` reaches neither a
non-blocking `recv` nor `poll`/`WSAPoll`, on the three systems (`:657-666`), so the deadline is the
layer's own wait bound. **`net/http`'s `serve_conn` takes a header deadline** on Node's precedent
(CVE-2018-12122, `historian.md:206-213`), unbuilt (`compiler-engineer.md:487-489`). **No spec sentence**:
the spec-warden's vetoes of q5d, q5j and q5n stand (`spec-warden.md:312-376`); a card change. **A join
that does not wait is not adopted**: in q5n's shape it makes line 377 false and takes the waiting join
the compiler exits through (`spec-warden.md:328-342`), and in either shape it leaves the quiet
connection's thread and socket held, one of 256 slots each (`compiler-engineer.md:483-487`).

**R6, the blind seat.** Forty sessions ran, 12.9748 USD, the largest 0.5323, above the assumed 0.496
(`critic-reports.md:224-227`), all under the base spec with the candidate cards; graded against the
cards' faces (`ask_stop() -> ()?`; the deadline as a limit counted from each call that waits,
`248-reports/llm-ergonomist.md:33-97`), which differ from what R4 and R5 adopt; **its result, when
graded, is read as about the cards, and its decision rule as registered**. The spec-warden's own arm
(q4m sessions) did not run and stays unscored (`critic-reports.md:337-340`).

**Refused**: route I-onstack (vetoed by the ffi-pragmatist), B and T (vetoed by the ffi-pragmatist,
objected to by the compiler-engineer), U, W, R and I-dylib (objected to); I-mask (its own condition
met); K (R3's own words); a descriptor per waiting thread and a thread-directed signal for Q2 (objected
to); a `heroes -C` flag for Q3 (objected to); the door sharing the count (objected to); a join that does
not wait and a limit in `net/http` alone (objected to); the sentences q3a, q5d, q5j, q5n, q5o, q4t,
q4a (vetoed or objected to by the spec-warden).

## What a veto compels

- **The ffi-pragmatist's veto of I-onstack** stands on bdwgc's fault, re-run by the critic: I-onstack is
  not adopted. Its Darwin half is the critic's reading, unrun (R1); it is the follow-up's to weigh
  against Go's and ART's want of the flag.
- **The ffi-pragmatist's vetoes of B and T**: B stops `raise` delivering, an `alarm` interrupting a
  `read`, and SIGPROF sampling the running thread, on both legs (`ffi-pragmatist.md:183-190`, `:254-256`);
  T refuses a correct program at `-O0` and `-O2` on both legs (`:191-196`, `:256-257`). Neither is adopted.
- **The spec-warden's vetoes** of the sentences q3a, q5d and q5j (Principle 0) and of q5n and q5o as
  replacements of the waiting join (the compiler's need, `selfhost/main.hero:112`, `spec-warden.md:219-224`):
  none enters.
- **The compiler-engineer cast none** (`compiler-engineer.md:560-561`).

## Disagreements, not smoothed

- **Which stack a C handler runs on, or which mask it holds.** The compiler-engineer approves I-onstack
  and objects to the wrapper form; the ffi-pragmatist vetoes I-onstack and approves I-mask; the
  historian approves I-onstack as *"the one route here two production runtimes already do"*
  (`historian.md:72`); the critic measured each route breaking the first library looked for, a collector
  under one and a SIGURG waiter under the other, and neither soundness seat citing Go or ART
  (`critic-reports.md:58-61`). R1 adopts neither and asks the census first.
- **Darwin.** I-onstack in the executable leaves a two-level dylib unrefused; I-mask needs a dylib
  beside every program that hands C a function (`critic-reports.md:80-98`). Both seats agree Windows
  needs nothing (`compiler-engineer.md:526-530`, `ffi-pragmatist.md:285-290`), that the two D cases of
  2494 move under any route I (`compiler-engineer.md:114-123`, `ffi-pragmatist.md:409-422`), and that
  `ffi_defined_elsewhere` widens on Linux, in the escape's words (`compiler-engineer.md:170-177`,
  `ffi-pragmatist.md:301-311`); the historian reads compiler-rt's co-existence protocol against that
  widening (`historian.md:75`).
- **Q2's route.** The compiler-engineer's (g) costs N + 8 descriptors and keeps Windows' slices; the
  poller costs one thread and a hand-off a wait (`ffi-pragmatist.md:767-769`) and closes Windows too. R2
  takes the poller.
- **Q3's program path.** The compiler-engineer keeps `execvp` in the child with a program path holding a
  `/` made whole in the parent, and recommends moving the `PATH` search to the parent as a defect of its
  own (`compiler-engineer.md:276-286`, `:313-317`); the ffi-pragmatist has the child `execve` a path the
  parent resolved, taking `execvp` out of the child in the door itself (`ffi-pragmatist.md:776-790`). R3
  adopts the order both promise; who resolves the bare name is the landing's to settle on that promise.
- **Q3's order against the precedents.** Go evaluates a relative program against `Dir`, POSIX's spawn
  action against the last `chdir`, Python on POSIX against `cwd`, all the child's folder; Python on
  Windows does not let `cwd` override the search, and Rust calls the case *"platform specific and
  unstable"* (`historian.md:155-176`). R3's one meaning, the parent's folder on every system, is Windows'
  measured order.
- **Q3's sentence**: the ffi-pragmatist (*"Needs a spec sentence"*) against the spec-warden's veto (R3).
- **Q4's design**: the built door shares the count, the ffi-pragmatist objects to it, and the
  spec-warden's sentence follows the design (q4s if shared, q4m if apart, `spec-warden.md:410-416`). R4
  takes apart.
- **Q5's meaning**: the card the sessions read, a limit on each wait; the compiler-engineer's door, an
  absolute time from the call that a pending call does not see; the ffi-pragmatist's, an absolute time
  in the socket's entry (the critic's item 14, `critic-reports.md:189-206`). R5 takes Go's.

## Conservative alternatives, recorded

- **Route (g) for Q2** (the compiler-engineer's, built in the layer, about 130 lines of `hero_net.c`,
  `compiler-engineer.md:225-236`, `:602-609`): no slice on POSIX, a closed connection woken by its own
  shutdown, a listener by a pipe of its own, the stop's pipe made with the first socket; Windows keeps its
  slices, so 2502 stays open there.

## Prior art, per resolution

The historian's candidate lines (`248-reports/historian.md` § Candidate prior-art lines, `:344-408`) for
the routes this resolution adopts, each page re-read by this synthesis's session on 2026-11-14 and
written from the page's own words; Q1's routes, adopted by none, carry none.

- R2: `- **Reached first by Go**: [runtime/netpoll.go](https://raw.githubusercontent.com/golang/go/master/src/runtime/netpoll.go): an "Integrated network poller (platform-independent part)", which a particular implementation (epoll/kqueue/port/AIX/Windows) wakes with netpollBreak, "Wake up the network poller, assumed to be blocked in netpoll."; each descriptor holds the goroutine waiting to read or write and a read and a write deadline, each with its timer, "io notification or timeout/close changes the state to pdReady or pdNil respectively and unparks the goroutine".`
- R2: `- **Taken from libevent**: [evutil.c](https://raw.githubusercontent.com/libevent/libevent/master/evutil.c): a socket pair built from a listener on the loopback address and a socket connected to it, "originally from Tor", of which the code says "This socketpair does not work when localhost is down"; on Windows, socket(AF_UNIX) is tried first, falling back to AF_INET.`
- R3: `- **Reached first by POSIX**: [posix_spawn_file_actions_addchdir](https://pubs.opengroup.org/onlinepubs/9799919799/functions/posix_spawn_file_actions_addchdir.html): the function "shall add a chdir action to the object referenced by file_actions", and its fchdir twin an fchdir action; "First released in Issue 8. Derived from Solaris posix_spawn_file_actions_addchdir_np."`
- R3: `- **Reached first by glibc**: [posix: New function posix_spawn_file_actions_addchdir_np](https://sourceware.org/legacy-ml/libc-alpha/2018-10/msg00579.html): "The posix_spawn_file_actions_addchdir_np function has been added, enabling posix_spawn and posix_spawnp to run the new process in a different directory", under GLIBC_2.29.`
- R3: `- **Reached first by Rust**: [Command::current_dir](https://doc.rust-lang.org/std/process/struct.Command.html): "If the program path is relative (e.g., "./script.sh"), it's ambiguous whether it should be interpreted relative to the parent's working directory or relative to current_dir. The behavior in this case is platform specific and unstable, and it's recommended to use canonicalize to get an absolute program path instead."`
- R4: `- **Reached first by Go**: [issue 39919](https://redirect.github.com/golang/go/issues/39919): "Shutdown() is synchronous."; a handler that called it was found "still waiting to Shutdown()", which "obviously cannot since its the active connection that its awaiting"; with the call moved, "That looks right. We should close this."`
- R5: `- **Taken from Go**: [net.Conn](https://raw.githubusercontent.com/golang/go/master/src/net/net.go): "A deadline is an absolute time after which I/O operations fail instead of blocking. The deadline applies to all future and pending I/O, not just the immediately following call to Read or Write."; "An idle timeout can be implemented by repeatedly extending the deadline after successful Read or Write calls."`
- R5: `- **Taken from Node.js**: [November 2018 security releases](https://nodejs.org/en/blog/vulnerability/november-2018-security-releases): "An attacker can cause a Denial of Service (DoS) by sending headers very slowly keeping HTTP or HTTPS connections and associated resources alive for a long period of time." (CVE-2018-12122); "A timeout of 40 seconds now applies to servers receiving HTTP headers. This value can be adjusted with server.headersTimeout."`
- R5: `- **Reached first by Go**: [Go 1.8 release notes](https://go.dev/doc/go1.8): "The Server adds configuration options ReadHeaderTimeout and IdleTimeout and documents WriteTimeout."`
- R5: `- **Reached first by Python**: [socket](https://docs.python.org/3/library/socket.html): "At the operating system level, sockets in timeout mode are internally set in non-blocking mode."`

## Found beside the questions, not filed by this synthesis

- **Darwin keeps a thread's alternate-stack mapping after it ends**: `sigaltstack` with `SS_DISABLE` and a
  zeroed `ss_size` answers -1, errno 12, and leaves the stack standing, which is how
  `hero_stack_guard_leave` disables it (`compiler-engineer.md:125-131`); one measurement and a reading,
  the count over many spawns unrun, *"a question for a defect, not this sitting's"*.
- **The child's `getrlimit`, `setrlimit` and `execvp` after a threaded `fork`** are off POSIX's list
  (`00-shared.md:686-697`); no hang in 2,000 forks a leg by the compiler-engineer (`:306-317`,
  `:550-551`) and 49,000 and 71,500 children by the ffi-pragmatist (`ffi-pragmatist.md:553-582`), a
  sample; the compiler-engineer recommends the move as its own defect (`compiler-engineer.md:313-317`).
- **A relative path a parent hands its child in a variable is read in the child's folder**: the door's
  case failed under Guard Malloc until the harness gave `MALLOC_LOG_FILE` whole
  (`compiler-engineer.md:325-331`); the door's comment and the card owe the sentence.
- **The brief contradicted itself on the refresh calls**, approving them at `00-shared.md:17-27` and
  forbidding them at `:188-190` (the critic's item 16, `critic-reports.md:326-330`).
- **The blind seat's first runner stopped after its first session ended**, found by the critic running it
  dry with a local stub in place of `claude` (`critic-briefs.md:732-746`, item 1 of its second pass at
  `:793-798`), repaired before any session, a session writing `run.json.part` and renaming it when it ends
  (`248-briefs/llm-ergonomist.md:186-188`).

## What the landing owes beside the resolutions

- R2's poller in `hero_net.c` with its close path, measured on every CI leg, the fall-back to slices
  where the Windows pair or a descriptor cannot be made, `hero_net.h`'s promises rewritten.
- R3's door in `run.c` and `hero_os.h`, the net's four cases moved, the door's own `run` case, the card's
  sentence on a relative path in a variable; the two 2344 cases and fetch 2479 judged by the CI's Windows leg.
- R4's door kept apart from `hero_net_stop_asks`, q4m in spec § 13's line 404 with
  `heroes measure spec/heroes-spec.md --refresh` on the landed text (`spec-warden.md:428-429`), `core`
  0.4.0 with `ask_stop()` and `stop_asked() -> bool`.
- R5's deadline with Go's meaning, pending waits included, which no seat built (the compiler-engineer's
  door leaves a pending call its old deadline, `compiler-engineer.md:465-466`), `QueryPerformanceCounter`
  on Windows, `set_deadline` on `net`'s card in `core` 0.4.0, `serve_conn`'s header deadline.
- R1's follow-up sitting convened when this ratification is read; 2522 open and `blocking`.

## Left open, as questions

- The census R1 asks for, and every candidate route against it.
- What I-mask's dropped blocks do to a Go `c-archive`'s own SIGURG (`critic-reports.md:55-57`).
- Whether a connected socket's shutdown wakes a waiting `poll` on Linux x86-64 (`compiler-engineer.md:619-621`).
- Whether `WSAPoll` has a bound of its own past the 101 descriptors one call took (`ffi-pragmatist.md:525-526`).
- The compiler's own split of `C_INCLUDE_PATH` at `;` on Windows (`critic-reports.md:155-159`).
- Whether a runtime's strong `sigaction` in front of ASan's weak interceptor loses ASan's reports in Linux
  (`compiler-engineer.md:588-589`, `ffi-pragmatist.md:241-244`), should any route I return.

## Predictions to score

- compiler-engineer: Q2 `waiters-checked 100 wait 10` under 80 million instructions and 108 descriptors at
  N = 100 under route (g), unscored with (g) not adopted; Q3 the two 2344 cases and fetch 2479 on the CI's
  Windows leg; Q4 t4 compiling at least as often with no result, read against the blind seat's grading;
  Q5 the narrow shape answered within the deadline plus 1 s on every CI leg (`compiler-engineer.md:617-618`,
  `:641-642`, `:662-663`, `:684-685`).
- ffi-pragmatist: (1) on bdwgc, **held** (`ffi-pragmatist.md:717-720`, re-run by the critic); (2) and (3)
  with I-mask not adopted, unscored; Q2 the poller within 2 of the base's count and under 1 % of the
  slices' instructions (`:764-766`), at the landing; Q4 a single Ctrl-C during the example server's own
  stop not ending it (`:814-815`); Q5 a server under 256 quiet connections keeps accepting, each timed out
  within 20 ms of its deadline on POSIX (`:835-837`).
- spec-warden: the q4m arm **unscored**, its sessions never run (R6); q4m's real count at the landing,
  11303 alone on this base (`spec-warden.md:254-255`).
- The blind seat's rule, **scored at its grading: both faces help**, t4 +6 and t5 +9 (§ The blind seat).
  The compiler-engineer's Q4 prediction (t4 compiling at least as often with no result) stays
  **unscored**: the graded arm q carried the card's `-> ()?`, and no arm carried the face with no result.

## Process notes

- **The escalation**: the briefs were drafted for the soundness lane and escalated to the full panel on
  the critic's first-pass item 23 before any seat ran (`critic-briefs.md:601-605`, `00-shared.md:79-101`).
- **The Windows box** was off when the briefs were written, `ssh ... win` answering *"Could not resolve
  hostname"* at 08:51 (`00-shared.md:256-258`), and on from 10:31, switched on by the author
  (`compiler-engineer.md:289`, `ffi-pragmatist.md:14-15`); by the coordinator's account it answered once
  Tailscale was stopped on the Mac, a fact no report records.
- **Three agents stopped at about 12:00** on the old account's weekly limit and were resumed on a new
  account, by the coordinator's account; no report records it.
- **The blind seat's runner** stopped after its first session in its first form, which the critic found
  with a stub and which was repaired before any session (§ Found beside the questions). The first
  session ran under `--restricted` with `Read` alone and put its report in its answer, so every report
  was read from `run.json`'s `result` (`248-briefs/llm-ergonomist.md:415-417`).
- **The paid runs**: the blind seat's 12.9748 USD of the author's 25 (`critic-reports.md:224-227`); the
  spec-warden's 10 `--refresh` invocations, 30 requests, made on the coordinator's direction of 10:30,
  the seat saying it did not see the author's approval itself (`spec-warden.md:111-113`, `:122-123`,
  `:167-168`), whose cost in USD no report states.
- **The machine** stood at load averages of 224 to 333 through the sitting (`compiler-engineer.md:13-15`,
  `ffi-pragmatist.md:12-13`, `critic-reports.md:8-9`), so every count is instructions, cycles or events,
  and every latency indicative.

## Author's verdict

Pending: the author's reading of R1 to R6, asked by the decision issue drafted beside this sitting and
not yet filed. A yes settles the six resolutions as adopted: 2522 open and `blocking` with its follow-up
sitting owed, the poller, the folder door, the stop door kept apart with q4m, and Go's deadline. It does
not settle the follow-up's route, the conservative alternative, which stays the author's to choose, or
the predictions, on their own clocks.

Filed 2026-11-14 at about 20:35, the records suite's verdict check asking it at the batch's gate: the author's reading
is asked by decision 2527 (https://github.com/heroes-lang/heroes/issues/2527).

**Ratified, R1 to R6, 2026-11-14**: the author's answer at about 21:47, `1a 2a 3a`, to the
coordinator's `/decide` batch, which put the six resolutions with their measurements and the
conservative alternative of R2 (route (g), a pipe per listener, Windows unchanged) as *1b*, and the
two choices lane b36-net made in R5's landing as questions 2 and 3; given in the coordinator's
conversation, recorded as a reading (`.claude/rules/records.md` § And the record says whose idea
it was). **What the yes settles**: R1 to R6 as adopted, so the landing pushed at 215df7fde stands
(the poller, the folder door, the asked stop kept apart with spec line 404's q4m, Go's deadline);
the conservative alternative not taken; a deadline passed sparing a call that need not wait, the
deadline bounding the layer's wait (*2a*, where Go fails even such a call); `serve_conn`'s header
deadline an internal 60 s (*3a*, not a parameter of the face); and defect 2522 open and `blocking`,
its follow-up sitting convened now, as R1 says. **What it does not**: the follow-up's route; the
predictions, on their own clocks; the Windows rows the box left unread, owed to the CI's leg.

## Added 2026-11-14 at about 12:40, by the coordinator: the routes R2, R4 and R5 are built before this synthesis is final

The drafting found, and the coordinator accepts, that no seat built R2 (the poller), R4 (the stop kept apart from the signals' count) or R5 (Go's deadline, applying to pending waits) as the layer's code: each stands on the seats' probes and the historian's precedents, and `00-shared.md:182-184` says a route that does not build is reported as not built, never adopted (the /panel skill's step 3c). So this synthesis is not committed or filed until lane b36-net of batch 36, opened at 12:30 on these three routes, has built them in `runtime/platform/hero_net.c` and run the cases this sitting names (idle CPU and close latency against the base, the descriptor count under `ulimit -n 256`, the stop kept apart under a second signal, a deadline over a pending wait, on this Mac, in Linux arm64 and on the Windows box); its results are written under this heading, and a route that does not build as resolved is reported here as not built and its question reopened. R3 is built (the compiler-engineer's door, `run.c` +91), and lane b36-dir lands it.

**The results, written 2026-11-14 at about 17:40 by the coordinator from the lanes' reports and
the Windows box's logs: R2, R3, R4 and R5 are built as resolved, and no question is reopened.**
Lane b36-net (branch `b36-net`, merged into batch 36's round at `21fed5db7`) and lane b36-dir
(branch `b36-dir`, merged at `d44558acf`) built them in `runtime/platform/hero_net.c` and
`runtime/parts/run.c`; every number below is the lane's, from the commands its report names, or
the box's, read from its logs.

- **R2, one poller** (`e2ee7084468b745dfbe2957e8141fd6b6ae30720`, defect 2502). Each waiting call
  sleeps on a condition variable of its own; one layer thread waits on every waiting socket, kqueue
  on Darwin, epoll on Linux, and on Windows `WSAPoll` over the sockets and the reading end of a
  loopback pair whose accepted end is checked against the connecting socket. A close and the stop
  wake the calls directly, never through a `shutdown` (critic items 9 and 10). Where the poller
  cannot be made the calls wait in slices as before; on Windows a `connect` still waits in slices,
  `WSAPoll` before Windows 2004 not reporting a refused connection. Measured, base against the
  poller: 100 receives waiting 1, 5 and 10 s on Darwin, 122.1, 300.5 and 512.8 M instructions
  against 87.3, 87.5 and 88.6 M, involuntary switches at 10 s 24,700 against 1,013; process CPU in
  Linux arm64 0.037, 0.137 and 0.177 s against 0.018, 0.021 and 0.020 s; a close's median latency
  for a receive on Darwin 8 to 10 µs against 14 to 23 µs, for an accept 37,845 µs against 7 µs on
  Darwin and 44 against 28 µs in Linux arm64; connections held under a soft limit of 256, 250
  against 249 on Darwin and Linux; emulated Linux x86-64 every step passed, a close 42 to 43 µs.
  Cases `run/fixedbugs-2502-a-close-wakes-a-waiting-call-at-once` and
  `-one-poller-tells-many-waiting-calls`, 2 passed, and the platform layer's cases, on this Mac and
  on the Windows box (at `6a6e50dcd`: 2 passed, and the layer's 12 passed, its two POSIX-only cases
  skipped by name), where the `WSAPoll` arm was compiled and run for the first time. **Not read from the
  box**: its idle-CPU and close-latency probe, which compiled against both layers and waited for a
  still machine; the box was last read at about 14:08, with the probe still waiting, and was offline
  when it was next asked at 17:23 (Tailscale: last seen about two hours before), so whether the probe
  ran is unknown and its numbers are not in this record.
- **R3, the door `hero_run_dir`** (`822c0048f24b55ee7c3aae6053e3e324ea0c175d`, defect 2490). Built
  in the `fchdir` shape on POSIX (the folder opened with `O_SEARCH` or `O_PATH` before the fork, so a
  folder that may be entered and not listed is entered) and as `lpCurrentDirectory` on Windows, the
  folder checked before any stream is opened; every relative path of a launch read in the parent's
  folder, a relative or empty `PATH` entry included; a folder that cannot be entered answers the new
  status `HERO_OS_NO_FOLDER` (7). On the box the door's case matched its `.expected` line for line,
  measuring what was an inference until then: `CreateProcessW` reads a relative `PATH` entry in the
  parent's folder. The three `directory` cases and the fetch case start in their folder through the
  door instead of `/usr/bin/env -C`, and **the two 2344 folder cases now run and pass on Windows**
  (`unsupported` narrowed to `fixedbugs-2344` 9 passed, `an-empty-cpath` Darwin's alone), which
  settles critic item 11; the fetch case's one red there, the harness reading only `/` in the
  compiler's spelling of a resolved folder, is answered at `d20c97262`, whose rerun on the box is
  owed to the CI's Windows leg.
- **R4, `hero_net_ask_stop()` kept apart** (`dc33ddee15ed89f87c7207dcdf4edd83085f950d`). The door
  sets a flag of its own and the handlers count signals alone, so the first Ctrl-C after an asked
  stop does not end the process and the second does, exit 130; it returns nothing and takes the
  table's lock, so the header says never from a handler. Cases: twelve waiting calls each answering
  `stopped` (median under 10 ms, a first interrupt survived) and the second interrupt's end, POSIX's
  alone, skipped on Windows by name. ThreadSanitizer 0 warnings in 3 runs on Darwin.
- **R5, the deadline** (`6a6e50dcde0f20fa262176a44552cf35364c1a05`). An absolute time kept in the
  slot, crossing with the socket's number, applying to later calls and to calls already waiting,
  which are woken to read it again; 0 takes it away; past it a wait answers `HERO_NET_TIMED_OUT`. The
  clock is the layer's: `CLOCK_UPTIME_RAW` on Darwin, as `hero_time.h` uses (the ffi-pragmatist's
  `CLOCK_MONOTONIC` counts sleep there), `CLOCK_MONOTONIC` on Linux with condition variables made on
  it, `QueryPerformanceCounter` on Windows. The case's six shapes pass: a quiet receive and a quiet
  accept, a byte before the deadline delivered, a deadline set while a call waits, 0, a call that
  need not wait spared with the deadline kept across the number, and the refusals. ThreadSanitizer on
  Darwin (3 runs) and in Linux arm64 (2 runs), clean.
- **Core 0.4.0** carries the faces: `net.ask_stop()`, `net.stop_asked() -> bool`,
  `net.set_deadline(c: Conn, ms: i64) -> ()?`, and `net/http`'s `serve_conn` a header deadline of
  60 s (Node's and nginx's default; a quiet connection that times out ends quietly, Go's way, and a
  half-sent head is answered 408); its card's line on `number` answers defect 2526. Published, every
  program moved to it and 0.3.0 removed by the author's decision 2b; `net.md` at panel 234's ceiling
  of 1,024 tokens after its first sentences were shortened.

**Two choices the lane made and the author may turn.** A deadline passed spares a call that need
not wait (bytes already there, a connection already queued): the deadline is the bound on the
layer's wait, as R5 adopts it, where Go fails even such a call. And the header deadline is an
internal constant of 60 s, since a face's card carries no constant that is not a `str`.

**Predictions, scored at the landing.** The ffi-pragmatist's Q2, the poller within 2 of the base's
count: **held** (249 against 250). Its *under 1 % of the slices' instructions*: held if read as the
waits' own cost, the growth from 1 s to 10 s (1.3 M against the base's 390.7 M, 0.33 %), and not
held if read as the process's total (87.5 against 300.5 M at 5 s, 29 %), which includes the hundred
threads' own start; both readings are given. Its Q4 is held in the layer's case (a first interrupt
after the asked stop survived); the example server was not run under a Ctrl-C. Its Q5 (256 quiet
connections, each timed out within 20 ms) stays **unscored**: the case's six shapes pass and the
256-connection run was not made. The compiler-engineer's Q3 and Q5 are the CI's, after the push.
