# Panel 246, spec-warden, Part B

Written 2026-11-12 from 04:54, as I go, in my own folder
`.claude/worktrees/p246-scratch/246-spec-warden/` (`git check-ignore -v`: `.gitignore:34:.claude/worktrees/`),
its `tree/` a `git archive ef50a7478` (`git -C .claude/worktrees/b33-round rev-parse ef50a7478
'ef50a7478^{tree}'`: `ef50a7478c241f765c0b308cb55282f95d64053f`, `084b36b7d5db6b342c281468fff120b2fe98349b`).
Part B only. Every number below sits beside the command that printed it. The vendored count only: no
`--refresh` was run, so every delta is a **lower bound** on the real one, and every verdict whose
outcome a real count could change is marked `provisional`.

## Where my brief is wrong, first

- **Question 6 says *"§ 2's 'There are no mutable globals'"***. The sentence is in spec **§ 4**: `grep -n 'no
  mutable globals' spec/heroes-spec.md` prints line 135, and the last `## ` heading at or above it is `## 4.
  Top-level declarations` (`awk`). design.md:4079 cites it by its old line, `spec:47`, and names no section;
  feature 1069's re-check of 2026-11-08 says *"§ 2 still says"* too. No verdict below rests on the number.
- **00-shared.md's rule that a program which may not end runs under `timeout 30`** does not bound a program
  that catches SIGTERM, which is the shape every B2 candidate asks a reader to write: my first q4 run outlived
  `timeout 20` by 58 s (below). `timeout -s KILL`, or a watchdog `kill -9`, is the bound for this shape;
  t3's grader needs it too.

## Setup, as run

- `uptime` at 04:54: load averages 7.07 4.40 3.38. No duration here is a measurement.
- Seed: `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`, exit 0 (`real 15.09`, `user 13.44`).
- Stage 2: `./heroes build selfhost/main.hero -o heroes-next`, exit 0 (`real 195.76`, `user 130.75`,
  `sys 26.40`: a waiting run under load 17 to 18, no duration). `./heroes-next --version`: `heroes 0.6.0`,
  `sources 796d1133542b1909`, **the base**.
- `python3 -I tools/drafts.py tree/spec/heroes-spec.md drafts` (my folder's `tools/drafts.py`, written from
  00-shared.md's sentences, never reading the coordinator's `price2/`): the base copy hashes
  `bcb60736643d7772` at 28,544 bytes, as the shared brief says.

## design.md §1.6, reached by grep in my tree today

- `grep -n '^### 1.6' docs/design.md`: 253. Line 255: *"must fit in 16384 tokens, measured by
  `claude-opus-5` through `POST /v1/messages/count_tokens`"*. `grep -n payment docs/design.md`: 282,
  301, 324, 328, 330, as the brief says. Line 324: *"the payment rule is unconditional. Every
  amendment owes a named removal or a registered prediction naming an instrument that exists today"*;
  330: a prediction pays only if it names (i) the instrument and (ii) the milestone that scores it,
  and the instrument exists the day it is registered.
- **The consolidation block's 1,500 no longer binds this sitting**:
  design.md:292 binds *"the rows from M-buildable-structs to M-compatibility-promise ... before the
  thesis harness has run"*; `ROADMAP.md` row 70 `M-thesis-harness | done 2026-10-24`, row 74
  `M-compatibility-promise | done 2026-10-30`, row 76 `M-core-packages | **OPEN**` (`grep -n -E
  'M-(buildable-structs|compatibility-promise|thesis-harness|package-manager|core-packages)' ROADMAP.md`).
  The last ledger row that states a running total is row 7856's *"251 of the consolidation block's 1,500 spent"*
  (`grep -o 'of the consolidation block[^|]*'`). So what binds Part B is the 16384 ceiling and the
  unconditional payment rule, nothing softer.

## The count before, run again in my tree

`./heroes-next measure spec/heroes-spec.md` from my tree's root, exit 0: claude-legacy **8267**, cl100k_base
**8417**, maximum **8417** (*"a lower bound, not the reader's tokeniser"*), spread 150, **real 11014**
(*"claude-opus-5, 2026-11-12 — the binding number"*, no `STALE`), *"Headroom: 5370 against the 16384
ceiling — but the FFI floor mortgages 60 of it (panel 030 R3), so what is measured against the ceiling is
11074"*. The brief's figures, every one. No different figure, so no finding here.

## Every draft priced, vendored only (each a lower bound on the real delta)

`./heroes-next measure ../drafts/<d>.md` from my tree's root, each printing *"No ceiling judges ... a count
and not a verdict"*. The first nine rows reproduce the brief's table **to the token**; the last five are
mine.

| draft | claude-legacy | cl100k_base | maximum | delta (max) |
|---|---|---|---|---|
| base | 8267 | 8417 | 8417 | |
| T1 appended | 8340 | 8490 | 8490 | +73 |
| T2 appended | 8282 | 8432 | 8432 | +15 |
| S1 | 8335 | 8487 | 8487 | +70 |
| S2 | 8330 | 8482 | 8482 | +65 |
| S3 (§ 11) | 8325 | 8475 | 8475 | +58 |
| T1 + S1 | 8408 | 8560 | 8560 | +143 |
| T1 + S3 | 8398 | 8548 | 8548 | +131 |
| R2 (panel 114's sentence) | 8298 | 8448 | 8448 | +31 |
| **T1 without *"and nothing else crosses"*** | 8335 | 8485 | 8485 | **+68**: the clause is **+5** |
| **M1, T1 merged into F3's sentence** | 8325 | 8475 | 8475 | **+58**: **15 below** T1 appended |
| **M2, T2 merged into F3's sentence** | 8279 | 8429 | 8429 | **+12**: 3 below T2 appended |
| **R2 with its own named removal** (§ 10's quadratic sentence out) | 8267 | 8417 | 8417 | **+0** |
| § 10's quadratic sentence out alone | 8236 | 8386 | 8386 | **−31**, R2's own price of 2026-09-06, to the token |

M1's text: F3's *"threads, processes and files from the language's `hero_os.h`, "* becomes *"threads, processes
and files from the language's `hero_os.h`, whose `hero_thread_spawn(body: (function(i64) -> i64), arg: i64)
-> i64` starts a thread and `hero_thread_join(handle: i64) -> i64` waits for its answer, nothing else
crossing; "*. M2's: *"threads (one `i64` crossing each way and nothing else), processes and files from the
language's `hero_os.h`, "*. The quadratic sentence still stands, `spec/heroes-spec.md:330` (`grep -n quadratic`).

**What these numbers do not say.** None is the binding instrument. For scale only, not a measurement of any
draft: 11014 / 8417 = 1.3085 at the base, and row 8417 landed +41 real on +31 vendored (1.32). Scaled so, the
largest bundle, T1 + S1, is on the order of +190 real against **5310 free** after the FFI floor (16384 − 11074,
the instrument's own line). **No draft and no bundle of this sitting comes within an order of magnitude of
the ceiling**: the budget cannot veto anything here, and a verdict of mine that rests on a vendored delta
rests on the ranking between drafts, which spec-shape.md says the vendored scale may get wrong (*"a sitting
that picks the cheaper of two drafts on the vendored number has ranked them on a scale that does not judge
them"*). Where two drafts differ by fewer than ten vendored tokens (T2 against M2, S2 against S3) I rank
neither on price.

## Question 1: what a reader of the base's spec writes for a thread, and what the compiler answers

Nine programs in my folder's `probes/`, each `heroes-next check main.hero` then `heroes-next build main.hero
-o prog` (exit codes captured one by one), the two that build run as binaries under `timeout 30`, output
through `head -c 4096`, `pgrep -f /246-spec-warden/` empty afterwards.

| probe | what the reader guessed | `check` | `build` | what the reader is told |
|---|---|---|---|---|
| q1h-right | `hero_thread_spawn(body: (function(n: i64) -> i64), arg: i64) -> i64`, as `hero_os.h` declares it | 0 | 0 | runs: `42`, exit 0 |
| q1e-builtin | no `extern`, `spawn(work, 21)` | 1 | 1 | `error[unknown_name]: nothing named spawn is in scope`, note: *"if `spawn` is a function of C, an `extern` group in this file binds it ... `function spawn(…) -> …`"* |
| q1a-name | `extern "hero_os.h"`, a guessed `hero_thread_create` | **0** | 1 | `error[ffi_unknown_name]: hero_os.h declares no hero_thread_create`, note: *"check the spelling, or name the header that does declare"* |
| q1b-sig | the right name, a pthread-shaped body `(function(p: ptr) -> ptr)`, `arg: ptr` | **0** | 1 | `error[ffi_callback_type]: ... clang read the header, and it says long long (*)(long long)` |
| q1d-str | body `(function(s: str) -> i64)`, `arg: str` | **0** | 1 | the same `ffi_callback_type`, naming `long long (*)(long long)` |
| q1f-f64 | body `(function(x: f64) -> f64)` (R5's *scalars*) | **0** | 1 | the same |
| q1g-bool | body `(function(x: bool) -> bool)` | **0** | 1 | the same |
| q6-handle | body `(function(s: Socket) -> i64)`, `record Socket tag hero_net_socket` | **0** | 1 | the same (00-shared.md's `handle.hero`, reproduced) |
| q1c-pthread | `extern "pthread.h"`, `record Thread tag _opaque_pthread_t`, `pthread_create(@t, attr: nullptr, start: worker, arg: nullptr)` | 0 | **0** | runs and stops: `panic: main.worker ran on a thread this program did not start — C called it back from a thread of its own, and this runtime's values are not safe there yet (design.md Part 7.13)`, **exit 134**. Panel 114's R4, run today: holds. |

**What this measures, and what it does not.** Two kinds of mistake, and the compiler treats them differently:

- **The type half corrects itself in one round.** Every wrong body (pthread-shaped, `str`, `f64`, `bool`, a
  handle) is refused at `build`, exit 1, by a message that quotes the header's own `long long (*)(long
  long)`: the fix is written in the message (design.md §4.17's promise, kept). Each costs one round trip,
  and `check` passes all five, so an editor's loop sees nothing until a build.
- **The name half does not.** `spawn` is told to bind *a C function named `spawn`*; `hero_thread_create` is
  told to *check the spelling*; neither message names `hero_thread_spawn`, and none lists what `hero_os.h`
  declares. And `pthread_create`, the name a model knows best, builds at exit 0 and stops at **run time**
  with a message that cites *design.md Part 7.13*, a file the reader does not have, and never says where
  the right door is. **No path from a wrong name to the right one runs through a diagnostic**: it runs
  through opening `hero_os.h`, which is the other file §4.17 says a fix must not need. How many rounds a
  model spends there is unmeasured here: it is the blind seat's arm x against arm y.

So **T2 buys the half the compiler already corrects in one round, and leaves the half it does not**: it says
what crosses and names no function, so a T2 reader still has to guess `hero_thread_spawn`. T1 carries the
name. That is the measured core of my B1 position below. (A finding beside it, outside this sitting's
spec question and not filed by me, no GitHub write: the run-time panic of a Heroes body on a C thread
cites design.md and names no remedy; by `.claude/rules/verification.md` § Bounded discovery it reads as
*"a true message less exact than it could be"*, `adjacent`. The coordinator may file it.)

## A finding for B2, run: a server that asks between connections does not stop until a client connects

The shape S1, S2 and S3 each tell a reader to write (*"a loop asks between its steps, finishes what it holds
and returns from `main`"*), built at the base with the program's own header, feature 1069's route, handler
and loop **in one module** (so this is not the two-module trap). `probes/q4-accept/`: `stopflag.h` (the
three lines of 1069's re-check), `signal` bound with a Heroes handler for SIGINT and SIGTERM, the layer's
binding copied from `tests/golden/run/the-platform-layer-echoes-over-loopback-in-one-thread.hero` (the
marks `acquires hero_net_close` and `consumes`), and the loop `while stop_asked() == 0` around
`hero_net_accept(l)` and `hero_net_close(c)`. `build`, exit 0.

- **The clean run**: the binary started directly (`./prog > out.fifo &`, its PID from `$!`, a `head -c
  65536` reading the fifo, a watchdog `kill -9` at 30 s): `kill -INT` at 2 s; **at 5 s it was still
  running**, blocked in `hero_net_accept`; one loopback connection (`python3 -I -c` `create_connection`)
  at 5 s; then `stopped after 1 connections`, **exit 0**. `pgrep -f q4-accept/prog`: nothing left.
- **The first run, kept because it says the same thing louder**, and because its error is mine: my
  `pgrep -f` pattern matched my own `bash -c` script, so its SIGINT went to the script (PID 35485, read
  back with `ps` and `lsof -d cwd`: my folder), never to the program and never to another seat's
  process. The program (35489) then took **`timeout 20`'s SIGTERM at 20 s, which its handler caught**,
  and was still running at 78 s (`ps -o etime`: `01:18`); one connection then ended it, `stopped after 2
  connections` (one at about 7 s, one at 78 s). **A program that catches SIGTERM outlives `timeout`**:
  a grader of t3 arms, and any later probe of this shape, needs `timeout -s KILL` or a watchdog, not a
  bare `timeout` (the shared brief's rule says `timeout 30`; for this shape that bound does not hold).
- **Why, read and not run on S1 to S3** (they are unbuilt): `runtime/platform/hero_net.c:509-512` asks
  `accept` again while it fails with `EINTR`, and BSD `signal` restarts it in the kernel anyway. A stop
  flag owned by the runtime (S1, S2, S3) changes who sets the flag, not where the loop is waiting. So
  **every candidate sentence would be false for the program it exists for**, a server with no client, on
  this Mac today, until route (b) or (c) of 00-shared.md makes the wait itself end on a stop. The blind
  seat's t3 is a loop of `sleep(1)` steps, the shape where the sentence is true, so its arms p, q and r
  cannot see this.

## Question 2: T1 appended after F3 against T1 merged into it

Priced above: **T1 appended +73, M1 merged +58** (vendored, lower bounds). `grep -n 'Anything beyond'` over
the two drafts shows the reader's line. M1 is one sentence of **74 words** (`wc -w`) whose rule has no
subject of its own: *"..., whose `hero_thread_spawn(...)` starts a thread and `hero_thread_join(...)` waits
for its answer, nothing else crossing; and sockets ..."*. *"nothing else crossing"* is an absolute
construction that a reader can attach to `hero_os.h` (nothing else comes through that header) as easily as
to a thread, and the signature sits mid-list between *files* and *sockets*. T1 appended is a sentence whose
subject is the thread and whose colon hands over the two signatures: one rule, one sentence. **I read T1
appended as the one rule and M1 as two half-rules**. That is my reading, not a measurement. What is
measured is that the blind seat's arm y carries **T1 appended** (`llm-ergonomist.md`'s table, arm y), so it
is the only text of the two any reader will have been tested on. spec-shape.md's *"Merging beats appending"*
is a price rule (panel 122), and CLAUDE.md § Precedence's instruction of 2026-09-28 says of the spec's own
tokens *"always the most robust and solid route, at their cost too"*. So **if a rung lands, land the
measured text, T1 appended**, and do not save 15 vendored tokens by landing a wording no reader read.

## Question 3: which rung the spec would promise, and what a 1.x could no longer change

Panel 232's paragraph, read at `docs/panel/232-*.md:272-274` in my tree: *"the functions `hero_os.h` declares
at 1.0.0, their names, their types and what each result hands its caller, which a 1.x release may add to but
never removes, narrows or changes"*. `hero_os.h` declares at the base (`sed -n 410,460p`)
`hero_thread_spawn`, `hero_thread_spawn_sized`, `hero_thread_join` and `hero_thread_limit`, the body typed
`int64_t (*)(int64_t)`. So:

- **T1's two signatures add no promise.** The i64 rung of `hero_thread_spawn` is **already** promised at
  1.0.0 by panel 232, spec sentence or none: no 1.x may change that function's type to admit a `str`, and a
  wider rung (R5's *"scalars and `str`"*) could only come as a **new** function, which 232 allows. Writing the
  signatures down makes the existing promise visible to the one reader it binds.
- **The clause *"and nothing else crosses"* (T1) or the whole of T2 is the new promise**, +5 vendored in T1
  (T1 against T1 without it, 8490 against 8485). It speaks of the language, not of one function, so a 1.x
  that adds a wider door must also retract a sentence of the spec. Under panel 232 that is lawful (a
  widening refuses no 1.0 program, and the spec may change in a minor release, `.claude/rules/records.md`
  § Release tags), so it binds less than it reads; what it costs is that **it settles R5 against
  journal 034 without a ruling**: R5, ratified *"in full"* (`docs/panel/113-*.md:276`, `:410`), admits
  *"the scalars and `str`"*; my q1f and q1g show `f64` and `bool` bodies refused today (by clang reading the
  header, `ffi_callback_type`), q1d `str` the same. The spec saying *"nothing else"* turns the code's
  narrowing, for which the critic found no ruling, into the spec's. **That is a decision for the author,
  named as such**, not a wording choice: the sitting should put R5 against the code to the author and let
  the clause follow the answer, not precede it.
- **S1 and S2 add two doors to `hero_os.h`**, frozen by panel 232 at 1.0.0 with their names, types and
  results; S2 also freezes a side effect on a query (its first call arms the catch), so no 1.x could make
  `hero_stop_asked` a pure question. **S3 adds a built-in**: a word reserved everywhere at 1.0.0
  (`selfhost/inventory.hero`'s head: *"Every name here is reserved everywhere"*), measured below to refuse
  any `extern` member of that name.
- **Before 1.0.0 none of this binds** (CLAUDE.md § 2, the author's rule of 2026-10-29): each is a promise
  taken at the tag, not today.
- **And `runtime/hero_os.h:430-434` is false today, reproduced**: *"WHAT MAY CROSS IS DECIDED BY THE
  CHECKER ... a `[T]` or a `{K: V}` ... is `error[ffi_type]`"*; my q1d, q1f, q1g and q6 pass `check` at exit
  0 and are refused by clang at `build`. `selfhost/check/ffi.hero:79-94` answers yes for `.int_ty`,
  `.float_ty`, `.bool`, `.str`, `.ptr`, `.cstr` and a group's record. A comment in the header panel 232
  promises is a false sentence beside the promise; it costs no spec token, and it is the comment's to
  correct whatever this sitting rules.

## Question 4: B2 under Principle 0

- **The compiler does not need a stop.** It binds what it uses from `hero_os.h` directly
  (`selfhost/main.hero:87` binds `hero_thread_spawn_sized`; no stop is bound anywhere: `grep -rn 'hero_stop\|stop_asked'` over `selfhost/` and `runtime/`'s
  `.hero`, `.h` and `.c` files counts **0** lines). So only the thesis branch can admit
  S1, S2 or S3.
- **No measured thesis effect exists today**, and the one this sitting will measure is on the wrong shape:
  the blind seat's t3 is a `sleep(1)` loop, while my q4-accept probe shows the shape a server has, a step
  that blocks in the layer's `accept`, keeping the program alive 3 s after SIGINT, and (first run) 58 s
  after SIGTERM, until a client connects. **Every one of S1, S2, S3 says a loop that asks between its steps
  *"finishes what it holds and returns from `main`"***, and for a server with no client that sentence is
  false on this Mac at the base, by measurement for the program's own flag and by reading
  (`hero_net.c:509-512`) for a runtime flag. A false sentence in the one document the reader is told to
  trust is a §1.2 loss whatever it saves: the program it produces compiles, runs, and ignores Ctrl-C.
- **Do routes (a), (b), (c) need a spec sentence?** (a), signals blocked and asked by `sigpending`, has the
  same blind spot as S1 (the wait does not end). (b) and (c) end the wait, and **both live in the platform
  layer**, whose header the spec already names (F3, line 377) and whose functions panel 232 does not promise
  (it promises `hero_os.h`'s). A stop the layer owns is documented where `hero_net.h`'s other functions are,
  in that header and the `net` package's card (panel 234's R3 card), at **zero spec tokens**, exactly as
  `hero_net_accept` itself is. A spec sentence is owed only by S3 (a built-in is the language) or by a
  `hero_os.h` door (S1, S2), and only once the door ends a blocked wait.
- **What S3 would owe, measured in part.** `selfhost/inventory.hero` holds 42 entries (`grep -c
  'Builtin(name:'`), its `NAMES` order being identity (its own head), and a built-in's name refuses an
  `extern` member of that name: `probes/q4-reserved/`, `function keys() -> i32` bound from a header,
  **`error[builtin_name_taken]: keys is a built-in of the language, so the name is taken everywhere`**, exit
  1. So S3 would **refuse feature 1069's own re-checked program and every program that ships a
  `stop_asked`**, my q4 and q5 included, a 0.x break that CLAUDE.md § 2 says is told and not undone. In
  the `spec` suite S3 sits in § 11's *"Files and the process, also provided"* sentence, which the `offered`
  check does not read (it reads the `Built-ins:` sentence only, spec-shape.md); the `named` check would
  want `stop_asked` in a code span, which S3 gives. That reading of the suite is mine and unrun.

## Question 5: panel 114's R2, ratified and never landed

- **Its price is no obstacle**: R2 with its own named removal, § 10's quadratic sentence, is **+0** vendored
  (8417 to 8417), the removal alone −31 to the token of R2's 2026-09-06 price. The quadratic sentence still
  stands at line 330.
- **Its gate is**, and it is a ratified one: R3, ratified in the same *"in full"*: *"Three instruments are
  owed, and R2 does not land without them"* (`docs/panel/114-*.md`, R3). Read today: the third (a broken
  shipped header told as the author's, not `internal error`) **exists**, `selfhost/cli/header_refused.hero`,
  defect 360, 2026-10-05. The second (the compiler reporting which `extern` declarations it did not verify
  on this machine, the dead `#ifdef` arm) **I did not find**: searched `selfhost/` for `dead arm`, `other
  arm`, `not checked on this machine`, `#ifdef` (only test fixtures and unrelated readers answered). The
  first (a shipped header and a missing one told apart in the harness) I did not trace. **Both are questions,
  not absences.** And panel 234's R6, ratified 2026-11-06, says the ffi-pragmatist's *"veto of panel 114 R3
  binds the shipped header"* (`docs/panel/234-the*.md`, R6), so the gate was read as standing five days ago.
- **The compiler already says it, at zero spec tokens.** `raise` bound from `signal.h` is
  `error[reserved_word]` whose message tells the reader to *"call it in a header of the program's own that
  includes "signal.h", `static inline int c_raise(int x) ...`"* (my q5 first build). So a reader meets R2's
  content where it is needed, in a diagnostic, without the spec sentence.
- **The trap, reproduced in my tree.** `probes/q5-one/` (handler and loop in one module):
  `drained after 3 requests`, exit 0. `probes/q5-two/` (the handler in `handler.hero`, each module binding the
  same shipped `stopflag.h`): **`drained after 1000 requests`**, exit 0, the cap the only end. R2's sentence
  is about `#ifdef`, not about state, but it is the sentence that tells a reader a header of their own is a
  route, and the first thing a reader builds with that route for a stop is this flag. **R2 should not land
  in this sitting**, and not as part of B2.

## Question 6: B3, a connection is a handle

- Measured: q6-handle, a `Socket` body, `check` 0 and `build` 1, `ffi_callback_type` (00-shared.md's
  `handle.hero`, reproduced). So under T1, *"one connection per thread"* (feature 1073, feature 893) is
  unwritable, and the milestone's own row says the server is *"writable, one thread"*
  (`docs/roadmap/milestones/M-core-packages.md:35`).
- **The spec owes no sentence for it.** If T1 lands, its *"nothing else crosses"* already says a handle does
  not cross; if no rung lands, the spec says nothing of threads' contents and a reader meets
  `ffi_callback_type`, which names the header's type. A sentence about sockets and threads in particular
  would be a second home for T1's rule (spec-shape.md: every rule has exactly one home).
- **Copy-on-write owes no spec token.** Under the i64 rung nothing a program hands a thread is a refcounted
  value; what two bodies may share is a module `constant`, which is static (`heroes_runtime.h:196`:
  *"refcount < 0 means static, never freed"*), so `cow.c:44`'s `refcount == 1` is false for it and it is
  copied. That is my reading, not a run (no sanitizer here either). design.md:4079 keeps § 4's *"There are
  no mutable globals"* as the spec-side twin, true of the program: it stays as written, and nothing in B3
  moves it.
- **A numbered handle** (the layer giving a socket a number) would make a connection cross as an `i64` with
  no spec token at all, which is exactly why it is dangerous: it reopens what panel 234's R3 says the
  wrapping is for, *"never another socket that took its number"*, and no spec sentence would announce it.
  That route belongs to the soundness seats; I only note that the budget would never see it.

## Question 7: the payment, and the predictions, registered 2026-11-12 at 05:06 CET

Registered **before the blind seat's results exist**: `ls docs/panel/246-reports/` at 05:06:19 CET (`date`)
listed `compiler-engineer.md`, `critic-briefs.md`, `ffi-pragmatist.md`, `historian.md` and this file, and no
`llm-ergonomist.md`; I have read no session of it. My drafts hash as the blind arms do (`shasum -a 256`, first
16 digits): base `bcb60736643d7772` = x, T1 `41ad6690571bd27b` = y, T2 `a5f822c1d0f79b03` = z, S1
`c1cf6e539b036b7d` = p, S2 `ea0c4dc22560d2d7` = q, S3 `f4e3f600482a50fb` = r. So a prediction naming an arm
names that text. Every instrument named exists today.

- **W1 (B1, where readers fail).** In this sitting's blind run as `llm-ergonomist.md` registers it (graded
  with stage 2 at `sources 796d1133542b1909`), **arm x's four t1 and t2 answers hold at most one `right`,
  and every one of the others fails on the name, never on the body's type alone**: `unknown_name`,
  `ffi_unknown_name`, a binding of `pthread_create` stopped at run time, or no thread at all, and no answer
  whose only refusal is `ffi_callback_type`. Falsified by two `right` under x, or by one x answer refused
  by `ffi_callback_type` alone. Scored in this sitting.
- **W2 (B1, T1 against T2).** **Arm y gives at least two more `right` than arm x over t1 and t2**, the
  sitting's own rule for admitting a rung, **and arm z at most one more than x**. Falsified by y under x + 2,
  or by z at x + 2 or more (then my objection to T2 is wrong and T2, or M2, is owed). Scored in this sitting.
- **W3 (T1's real price, for its landing).** T1 appended, the text of arm y, lands at **+80 to +110 real** on
  `heroes measure spec/heroes-spec.md --refresh`. Above +110, the landing commit also lands the named
  removal below, in the same commit. Scored by the landing's `--refresh`, the one paid call any landing makes.
- **W4 (B2's condition).** Under any of S1, S2 or S3 built as 00-shared.md writes them, with
  `hero_net.c:509-512` unchanged, the program of `probes/q4-accept/`, rewritten to that candidate's spelling
  in place of `stopflag.h` and `signal`, **is still running 3 s after SIGINT with no client** on this Mac and
  on Linux arm64; it ends within 2 s of the signal only once the wait itself ends on a stop (route (b) or
  (c)). Instrument: `heroes build`, the binary, `kill -INT`, a `kill -0` at 5 s, a watchdog `kill -9` (the
  recipe above, which exists today). Scored by the compiler-engineer's S1 and S2 runtimes in this sitting if
  they are built, else at the server step of M-core-packages.

**The named removal available, and who may spend it.** § 10's *"`+` on `str` copies both sides — a
concatenation loop is quadratic; `join` and `repeat` build in one pass."* (line 330), **−31** vendored,
measured alone above. Panel 114's R2 named it as its own removal, and the author ratified R2 *"in full"*
on 2026-09-06 with that ground: *"its whole content is performance advice and §13 says performance is never
a justification"*. R2 has not landed, so the removal is unspent. **It pays T2 or M2 outright** (+15 and +12,
both below 31) and about **two fifths of T1** (+73). If W1 or W2 fails after T1 lands, it is the removal
branch's payment (panel 046's R2), as row 8325 paid row 8335. It can be spent once: if the sitting lands T1
with it, R2 loses its ratified removal, and the synthesis must say so.

## Verdicts

### B1, T1 (feature 1073)

- `verdict`: **object**, `provisional` (vendored count only; and the blind run is pending)
- `section`: design.md §1.6 (payment, 255 and 324 to 330); §1.0 / CLAUDE.md § 2 (Principle 0); §1.2
- `spec_token_delta`: **estimated as a lower bound**, vendored maximum 8417 to **8490 (+73)**, legacy
  8267 to 8340; real unmeasured (W3 predicts +80 to +110). M1, merged: +58. The clause alone: +5.
- `removal`: § 10's quadratic sentence, −31, ratified as removable with R2; the rest paid by W1 and W2
- `needed_for_self_hosting`: **no** (`selfhost/main.hero:87` binds `hero_os.h` directly; spec unread)
- `argument`: Measured: wrong body types correct themselves in one round (`ffi_callback_type` quotes the
  header); a wrong *name* never does: `spawn`, `hero_thread_create` and `pthread_create` meet messages that
  never name `hero_thread_spawn`, the last stopping at run time citing design.md. So the signatures earn
  their tokens under §1.2, if the blind run agrees. What I object to is *"and nothing else crosses"*: it
  writes journal 034's i64-only into the spec over panel 113's ratified R5 (*"scalars and `str`"*), and
  nobody has ruled between them. The signatures are already promised by panel 232; the clause is the
  only new promise.
- `prediction`: W1, W2 and W3 above
- `condition`: approve when W2 holds **and** the decision issue names the clause a narrowing of R5 that the
  author rules on; land arm y's text verbatim, not M1

### B1, T2

- `verdict`: **object**, `provisional`
- `section`: design.md §1.2; §1.6
- `spec_token_delta`: lower bound, 8417 to **8432 (+15)**; M2 merged 8429 (+12)
- `removal`: § 10's quadratic sentence (−31) would pay it outright, so price is not the objection
- `needed_for_self_hosting`: no
- `argument`: T2 tells the reader what crosses and names no function. My nine probes show that this is the
  half the compiler already teaches in one round trip: every wrong body is refused with the header's
  `long long (*)(long long)` in the message. The half no message teaches, the function's name, T2 leaves
  out, so a T2 reader still writes `spawn` or `pthread_create` and meets the same dead ends as today.
  Fifteen tokens that buy no round trip are a §1.2 loss, however cheap.
- `prediction`: W2's second half, arm z at most one `right` more than arm x
- `condition`: arm z at x + 2 or more in this sitting's blind run

### B2, S1, S2, S3 (feature 1069)

- `verdict`: **veto**. Not provisional: no count enters it.
- `section`: CLAUDE.md § 2 and design.md §1.0 (Principle 0's burden unmet); §1.2; design.md §1.12 and
  CLAUDE.md § Precedence rank 3 for the false-stop shape; spec-shape.md for S3's reservation
- `spec_token_delta`: lower bounds, S1 +70, S2 +65, S3 +58 (8487, 8482, 8475); with T1 +143 and +131
- `removal`: nothing named, and that is a problem; a stop the platform layer owns needs no spec token
- `needed_for_self_hosting`: **no** (0 lines name a stop in `selfhost/` or `runtime/`)
- `argument`: The compiler needs no stop, and no thesis effect is measured. The one this sitting measures
  (t3) is a `sleep` loop, where the sentence happens to be true. The shape the feature exists for is not:
  my q4-accept server, asking between connections as all three sentences instruct, was still in `accept`
  3 s after SIGINT and 58 s after SIGTERM, ending only when a client connected
  (`hero_net.c:509-512` retries on `EINTR`). Each candidate promises *"finishes what it holds and returns
  from `main`"*, which is false for that server. S3 also reserves `stop_asked` everywhere, measured to
  refuse the very programs that ship one.
- `prediction`: W4
- `condition`: W4 falsified (the wait ends on a stop), on Darwin and Linux arm64, then a sentence describing
  the built behaviour, priced, and only if the stop is a `hero_os.h` door or a built-in; a stop inside the
  layer is documented in `hero_net.h` and the `net` card at zero spec tokens

### Panel 114's R2 (question 5)

- `verdict`: **object** (no veto: it was ratified, and its own R3 already holds it)
- `section`: design.md §1.6 (payment met: net +0); the ratified R3 gate; §1.12
- `spec_token_delta`: lower bound, +31 alone (8448); **+0 with its named removal** (8417 to 8417)
- `removal`: § 10's quadratic sentence, −31, its own and ratified
- `needed_for_self_hosting`: no
- `argument`: The price is settled at zero; the gate is not. R3 (*"R2 does not land without them"*) owes
  three instruments. One exists (defect 360's `header_refused.hero`); the dead-arm report I could not find,
  and panel 234's R6 read the gate as standing on 2026-11-06. The compiler already teaches R2's content
  where it is needed (the `raise` refusal names *"a header of the program's own"*), and the route's first use
  for a stop is a measured trap: two modules, `drained after 1000 requests`, exit 0.
- `prediction`: none registered; this is a gate, not a bet
- `condition`: R3's first and second instruments shown by command, and the two-module flag told by the
  compiler or ruled out by a sitting

### B3 (features 925 and 893, task 1072)

- `verdict`: **approve** the absence of any spec sentence for it
- `section`: spec-shape.md (one home per rule); design.md Part 7 item 13 (design.md:4079)
- `spec_token_delta`: **0**
- `removal`: none owed
- `needed_for_self_hosting`: no
- `argument`: A connection is a handle (panel 234's R3); a handle body is refused at `build` (q6-handle). So
  the milestone's *"writable, one thread"* server needs no rung, and T1's clause, if it lands, already says
  a handle does not cross: a socket sentence would be a second home. Copy-on-write's window is not reached
  by anything the i64 rung carries, by my reading (constants are static, refcount below zero; unrun under
  a sanitizer). § 4's *"There are no mutable globals"* stays as written. A numbered handle would cost no
  spec token, and that is the danger: no sentence would announce what it reopens.
- `prediction`: M-core-packages' server step lands with **0** spec tokens about threads beyond what B1
  lands, scored by `docs/measurements/010-spec-budget-ledger.md`'s rows at the milestone's close
- `condition`: a server that must cross a connection to a worker; then the rung that moves a handle is a
  new sitting, priced against T1's clause

## Paid runs

None run. One I would name, and leave to the author: a `--refresh` of T1 appended on the real path, then
reverted, would turn W3 into a measurement before landing. It is one `count_tokens` call. Row 7854 calls
such a call the author's *"free count"*, against a `claude -p` run, and records one refused for the
account's credit balance; whether it bills, I did not measure, and 00-shared.md counts it a paid call.
It needs `.env` and the author's yes (`.claude/rules/records.md` § Working in lanes), so it is not mine
to run.
