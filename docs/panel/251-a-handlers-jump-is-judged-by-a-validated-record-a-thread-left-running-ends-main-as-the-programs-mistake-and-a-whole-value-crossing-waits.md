# Panel 251: a jump out of a signal handler is judged by a validated record, a thread left running ends `main` as the program's mistake, and a value crossing whole waits

Sitting A of M-rebel-library, convened 2026-11-19 by its coordinator (the milestone page's item 33).
**The full panel**: the compiler-engineer, the ffi-pragmatist, the spec-warden, the historian
(advisory) and the llm-ergonomist (the blind seat, 50 graded paid sessions and one verdict session),
with the completeness critic reading the briefs before any seat and the reports after. The frozen
commit: `3a5ee269117a31780ef1f96d0305452630ac4d83` of `rebel-round`. Briefs:
`docs/panel/251-briefs/`; reports: `docs/panel/251-reports/` (`critic-briefs.md`, the five seats,
`critic-reports.md`).

**Status: provisional, ratified as proposed.** The author's word, given at about 13:10 on 2026-11-19,
before this file was written, meant as: *OK to the ratifications as proposed*, of sittings A, B and C.
So this sitting's resolutions are ratified in the words this file proposes them, by that advance word,
and its decision issue is opened and closed together (`.claude/rules/records.md` § The issues). It is
recorded as given: before the synthesis existed (CL-058).

## The proposal, verbatim

The three proposals as `docs/panel/251-briefs/00-shared.md:153-266` put them to the seats, copied by
script, unedited:

## The three proposals, put to the seats as the most robust and complete; none adopted

Every arm below is written from the frozen spec by `bw/tools/make_arms2.py` (each edit asserted to match
once) into `bw/arms2/`, its diff beside it, and counted by `./heroes-next measure bw/arms2/spec-<arm>.md`
(`bw/measure-arms2.txt`): the vendored maximum, a lower bound until the spec-warden's refreshes.

**Q1, a Heroes function a C handler reaches (defect 2522).** *Robust*: **route J, built and run on
every POSIX image, and 2522 closes only on no residue**. The runtime's definitions (Linux) or rewritten
slots (Darwin) of the setjmp family's save points, `setjmp`, `_setjmp`, `sigsetjmp`, `__sigsetjmp`, and
of the context family's, `getcontext`, `swapcontext`, record each buffer's caller frame and tail-call
the C library's own; a jump (`longjmp`, `_longjmp`, `siglongjmp`, `__longjmp_chk`, `setcontext`,
`swapcontext`) drops only the depth entries whose wrapper or trampoline frame lies deeper than its
target's; **a jump to a buffer the runtime holds no record of keeps every entry**, so a miss errs toward
R1's told refusal and never toward Heroes code running inside a signal; the C library's own pointers
are resolved before `main`, never by a `dlsym` inside a handler, and **the route's `sigaction` is
async-signal-safe in a forked child** (`run.c:1805`, and `hero_proc.c`'s child, § Question 1). Built into
D-rebind on Darwin (its `--sanitize` loop repaired first) and into B' on Linux arm64 and x86-64. **No
spec token** (the rule is spec § 13's *"never a result or a signal handler, nor called by one"*, line
403); R1's refusal words; route K stays **no** (the author's reading of 2026-11-15,
`docs/panel/249-*.md:551-559`). *Conservative*: route K answered yes, 2522 closing with J's residue told
where the rule stands (`bw/arms2/q1-k.diff`), *"... nor called by one, which aborts where the handler was
installed and left through the C library's own names and past them is the binding's word;"*, **+26**.

**Q2, the end of `main` with threads running (defect 2552).** *Robust*: **R, a thread nobody joins
aborts when `main` returns, saying how many, as the program's mistake**, merged into the lease sentence
(`bw/arms2/q2-r.diff`, **+5**; as its own sentence after T1 instead, `spec-q2-r-after-t1.md`, +23):

```
-cell, and a lease nobody ends, like a handle nobody consumes, aborts when
+cell, and a lease nobody ends, like a handle nobody consumes or a thread nobody joins, aborts when
 `main` returns, saying how many.
```

Beneath it, no spec token: the slot table writes state 2 at a thread's end (`spawn.c:222`'s comment
names it, no line writes it), **the gate asks the table first and reads no balance while a slot is
running or finished-unjoined**, its message names the thread and where it was started and never the
compiler, and so the premise at `alloc.c:79-80` becomes true again; the runtime's own threads (§ What
moved) are not counted; `exit(code:)` keeps ending every thread with the process. *Conservative*: **E,
in sitting 252's words** (its arm `pE`, the same bytes, sha256 `92c44fc0cb999390...`), appended after T1
(`bw/arms2/q2-e.diff`, **+27**): *"When `main` returns, the program ends, and a thread still running ends
with it; the program says how many it ended."*, the gate telling a running thread by count as the
program's, never *a compiler bug*. Both are read by the blind seat (task 1).

**Q3, threads that share nothing (decision 32).** *Robust*: **any value crosses, whole, as a copy, and
threads share nothing but a mailbox**: design.md Part 7 item 13's *"copying at the boundaries"* and
*"A message is any Heroes value"* (`docs/design.md:4085-4156`) made a ruling, **with a mailbox that is
not 7.13's**: 7.13's is *"one queue per thread; the sender copies into the receiver's heap"*
(`:4119-4123`), a queue a thread owns; the proposal's `(mailbox T)` is a value of its own that any
thread holding a copy sends to. **It is consistent with § 3 by amending § 3's address sentence**, the
one rule § 3 already gives a value two copies of which reach one thing: *"A `ptr` or a `cstr` is a
copied ADDRESS ... two copies reach one foreign thing, so a function taking one without `@` may still
change, or free, what C holds"* (spec lines 99-101) becomes *"A `ptr`, a `cstr` or a `(mailbox T)` is a
copied ADDRESS ... two copies reach one foreign thing, or one queue, ... what it reaches"*, so § 3's *"No
aliasing exists among the values this language owns"* (98) stays true of every value the language owns,
and a `send` without `@` changes the queue as a call without `@` already may change what a `ptr`
reaches. **And a mailbox is a handle with a life of its own** (§ 13's rules): `mailbox()` begins it, a
close ends it, a mailbox nobody closes aborts when `main` returns, a copy used after the close aborts.
`spawn` and a sized `spawn_sized` start a thread (the compiler's own command runs on a 268,435,456-byte
stack, `selfhost/main.hero:87`, `:96-97`, `:101`, joined at `:112`), so the proposal still expresses what
the compiler needs. **Two spellings, weighed by the seats** (finding 2 of the critic's reading, measured
below):

- **B, built-ins**: `spawn` · `spawn_sized` · `wait` · `mailbox` · `send` · `receive` · `close` join the
  `Built-ins:` sentence (`bw/arms2/spec-q3-b.md` **+162**; with Q2's words, `spec-proposal-b.md`
  **+168**, `bw/arms2/proposal-b.diff`);
- **M, a thread's and a mailbox's operations after a dot**: `spawn` · `spawn_sized` · `mailbox` join the
  sentence, `t.wait()`, `m.send(v)`, `m.receive(within_ms:)`, `m.close()` are written only after a dot
  and their names stay free for the program, § 9's *"There are no methods"* amended to *"There are no
  methods but a thread's and a mailbox's (section 11)"* (`spec-q3-m.md` **+185**; with Q2's words,
  `spec-proposal-m.md` **+191**, `bw/arms2/proposal-m.diff`).

The shared text, B's spelling (§ 11, after `exit(code:)`): *"`spawn(f, x)` runs `f(x)` on a thread of
its own and gives a `(thread T)`, `T` being `f`'s answer, which `wait(t)` waits for and gives;
`spawn_sized(f, x, bytes: n)` gives the thread a stack of at least `n` bytes. `mailbox()` begins a
mailbox's life, which `close(m)` ends, as a handle's (section 13); `send(m, v)` adds to it and
`receive(m, within_ms:)` takes the oldest, failing `timeout`, or `stopped` once a stop is asked (section
13). Every value crosses whole, as a copy, so threads share nothing but a mailbox; a `ptr` or `cstr`
never crosses, and another handle only into `spawn` or a send, its life ending in the sender."*; § 3
gains the rows `(thread T)` (*"a handle (section 13) its wait consumes"*) and `(mailbox T)` (*"a handle
(section 13) whose copies reach one queue"*) and the `Prefix` alternative `"(" ( "thread" | "mailbox" )
Type ")"`; § 13 loses *"threads, "* from its first sentence and T1 whole; Q2's merged words read *"a
thread nobody waits for"*. **Not `join`**: § 11 already names `join(xs, sep)`, and § 9 allows no
overloading.

**What either spelling takes from programs, measured**: a built-in's name is refused everywhere, a
declaration's, an `extern` member's and a local's alike (`error[builtin_name_taken]`, the critic's runs,
carried); **the method-form built-ins of today take their names too** (`function lease`, `cstr`,
`validated`, `slice`, `must`, `default`, `is_err` each `error[builtin_name_taken]`, `heroes-next check`,
`bw/names/`), so **route M needs a kind of name the compiler does not have today**, one resolved by its
receiver's type alone. `grep -rn -E '^function <name>\(' --include='*.hero'` over `bw/tree`, the vendored
`heroes.packages/` copies left out: `spawn` 1, `spawn_sized` 1, `wait` 7, `send` 25, `receive` 12,
`mailbox` 1, `close` 77 declarations (one file each); **the critic's five together, 46 declarations in
39 files**; B's seven, **124 in 98 files**; **M's three, 3** (Rebel's `threads.hero:56` `spawn`, `:72`
`spawn_sized`, `mail/internal/address.hero:141` `mailbox`); and **30 `extern` members named `close`**,
C's `close(2)` bound under its own name, which B would refuse (`grep -rn -E '^ +function
(spawn|wait|send|receive|mailbox|close)\('`). Today all seven are free (`bw/names/p_free.hero`, binding
and declaring them, `check` exit 0). CLAUDE.md § 2 allows the renames before `1.0.0`; they are a cost.

**Whether the `hero_os.h` thread doors leave the program's surface is a question**: the proposal stops
naming them, and § 13 still lets a program bind what a header declares, so whether a program that binds
`hero_thread_spawn` is refused (a new refusal, a diagnostic class this sitting would rule) or left
unnamed is the compiler-engineer's and the spec-warden's to answer; the 38 door files (§ Question 2) are
what either moves.

*Conservative*: the rung as it stands, one `i64` or one `str` each way (decision 2498, panel 246), every
ledger refusal below standing. *Named and not proposed*: Part 7.13's *"first and probably only rung"*,
plain data parallelism, any value copied at `spawn` and `wait` with no mailbox; and **7.13's own
mailbox**, a queue per thread reached through its `(thread T)` and received inside the body, which needs
no aliasing value at all (the compiler-engineer prices it beside the proposal's).

**The three read together.** Q2's sentence reads true under today's doors (*"joins"*) and under Q3's
`wait` (*"waits for"*) alike; Q3's `(thread T)` is a handle, so the handle machinery would tell an
unjoined thread by itself **if the gate asked it first** (§ Question 2: Rebel's `threads` does exactly
this, and the gate's order defeats it).

## The verdicts

Each from its seat's report, the critic's re-runs beside it where it ran one.

| seat | Q1, the handler's jump (defect 2522) | Q2, the end of `main` (defect 2552) | Q3, values crossing (decision 32) |
|---|---|---|---|
| compiler-engineer | **veto of J as written**: J trusts a saved record the program can overwrite; `ijstale.c` runs the Heroes function inside the signal under B'+J 80 of 80 on both Linux images. **Approves J with a validated record** (`routes/bjv/`): the stale case refused 80 of 80, the eight in-jump shapes, `kmiss`, `flib`, the storms refused, the twelve depth programs run, 600 forked children; runtime only (`sigjump.c` 236 lines, `sigguard.c` +14, `runtime.c` +5), 0 compiler lines | **approve R**: runtime only, +5 spec tokens; eleven repro programs, 20 runs each, every unjoined thread told as the program's mistake; census: no tracked program moved (134 end-of-`main` checks, 0 threads left unjoined); E weaker (switches the leak check off, ends output mid-line 35 of 200) | **object**: one new type `(thread T)` leaves 288 `match` statements incomplete in 109 files, `(mailbox T)` a second; a shallow crossing passed the same 32-thread stress as a deep copy, reference counts being atomic; land R and the print lock now instead |
| ffi-pragmatist | **approve J on Darwin, object to closing 2522 on it**: four in-handler shapes refused 160 of 160, `flib` 80 of 80, storms 25 of 25, twelve depth programs 480 runs with no false refusal; the `--sanitize` loop repaired by leaving `__interpose` images unrewritten; residue: a jump through a buffer J never saw refuses a correct program with R1's words, 40 of 40 | **approve R as R2, object to E**: with a worker blocked writing stdout into a pipe nobody reads, R keeps its promise only if its gate flushes stdout under a try-lock (R2: 134 after 0.22 s); E cannot, C's own `exit` waiting on stdout's lock (hangs 15 s, then SIGPIPE); new defect: any `hero_panic` hangs while another thread is blocked writing stdout | **veto spelling B** (an `extern` member has one name for Heroes and C, so C's `close`, `send`, `wait` become unbindable but through a shim); **approve the shape under M**; the mailbox as index and generation held 600 runs (400 sanitized) on both platforms |
| spec-warden | **approve J at no spec token; object to K** (its residue clause tells a cost the author refused, and *"the binding's word"* names nothing a binding can declare) | **approve R merged into the lease sentence, +9 real tokens; object to E and to R as a sentence of its own**; payment: W1 | **object, provisional; a veto if W4 fails for both spellings** (Q3 is not compiler need); `spawn_sized` as written cannot fail; B takes `close`, 77 of B's 124 colliding declarations; M leaves § 9's UFCS sentence contradicting its own |
| historian (advisory) | J has a working ancestor, ThreadSanitizer's `setjmp` records; **objects to D-rebind's forward compatibility on Darwin taken as given** (fishhook's own log repairs it for iOS 13 and iOS 15, a user found iOS 14.5 moving the pointers it rewrites, and dyld-1378 maps read-only data with TPRO protection) | **approve R**; its nearest ancestor C++'s `~thread`; leak checkers read running threads' stacks as roots, which R's gate needs | **approve copying with a mailbox as a handle**, with three departures named; the proposal's mailbox is Go's channel, not Erlang's pid; nothing read settles B against M |
| llm-ergonomist (blind) | task 3: `now` 4 of 5, `q1-k` 5 of 5: **K changes no reader's prediction** | task 1: `now` 0 of 5 (5 of 5 predict a normal end), `q2-r` 5 of 5, `q2-e` 5 of 5: **R's and E's sentences both read as needed** | task 2: `now` 5, B 5, M 5: **neither spelling serves the thesis by the registered count**; tasks 2 and 4, B 10 against M 10: **the spelling does not decide**; task 4: the address sentence suffices, 5 of 5 each |

## The critic's second reading, what it changed

Fifteen findings, ten of them binding on this synthesis (`critic-reports.md` § Summary). Each was
settled by a command where one could settle it:

1. **J as written fails on Darwin too.** `ijstale.c` under the ffi-pragmatist's D-rebind+J runs the
   Heroes function inside the signal 40 of 40 (a control run showing J active). The compiler-engineer's
   validated record, carried by the critic into a copy of `rebind-j.c`, refuses it 40 of 40, refuses every
   must-refuse case, and runs the twelve depth programs 72 of 72. **Only validated J is adoptable.**
2. **"Approve R" was two runtimes.** With stdout a pipe nobody reads, the compiler-engineer's R hangs 3 of
   3 with no message, as today's runtime does; only the ffi-pragmatist's R2 ends, 134 in 0.23 to 0.47 s,
   its message said. E inherits the hang. **The `hero_panic` hang reproduces on the frozen runtime**, 3 of
   3, at `runtime/parts/panic.c:59`.
3. **The spec-warden's W1 to W8 were blind in fact.** The brief scored them only if registered before
   `run.sh` started; the seat was launched at 10:08:11, `run.sh` at 10:08:24, so the condition could not
   be met by the coordinator's own launch order; the seat wrote them at 10:14:05 and its transcript holds
   no tool call into the blind area. **R's sentence has no payment unless W1 is admitted** (design.md
   §1.6: every amendment owes a named removal or a registered prediction); **Q3 is unpaid either way,
   and the spec-warden's stated veto condition is met** (W4 false, B 0 and M 0 against `now`).
4. **A third leak reached all 51 blind sessions**: the main checkout's git status, rendered into each
   conversation with five commit subjects, as the sessions' transcripts show; `GIT_CEILING_DIRECTORIES`
   did not stop it and `claude --help` names no switch that does. **All 51 answered `context: no`**, so
   that answer is not evidence of a clean context in this set-up. The labels' cause is settled: the
   inherited standard input. No answer uses a label or a subject; the subjects name no thread, mailbox or
   end of `main`, and reached both of task 3's arms alike. No reading is voided, on that reading of the
   answers, recorded here as such.
5. **Q3's crossing departs from design.md Part 7.13**, both as built: the shallow crossing is the
   *"cross-heap refcount ... a trade to make consciously if the copies ever hurt"* 7.13 declines in those
   words, and the deep copy as built frees on one thread what another allocated, where 7.13 copies into
   the receiver's heap. **The `runtime` suite is red at the frozen commit, 8 passed and 2 failed**
   (`runtime/alloc`, `runtime/threads`), on the branch's own platform members: allocation outside
   `alloc.c`, and objects kept between calls that are neither `_Thread_local`, `_Atomic` nor in
   `SHARED_BY_DECISION`. A defect of the branch, owed before its gate.
6. **What nobody asked**: the two built mailboxes disagree on a send after the close (one aborts, one
   answers a status) and both give `receive` a third answer, *closed*, which the text lacks; queued
   messages at the close and a plain `spawn` failing are unsaid; on today's handle machinery, handles in a
   list each consumed once in a `for` end cleanly 5 of 5, consumed twice compile and abort with a true
   message 3 of 3.
7. **Defect 2522 closes on no platform**: by each seat's own residue list, two misses give a correct
   program a false message; `dyld_hw_tpro_pagers` is absent on this Mac, plain and hardened, and
   `macos-26` is owed; the escape is ten C library names, not three, and its golden census is owed.
8. **The shallow crossing holds where it was not checked** (`main`'s value intact after the workers
   write, an answer sharing `main`'s blocks: 20 of 20 plain, 5 of 5 under TSan, 0 reports), and the 288
   reproduces exactly: the compiler-engineer's objection rests on the type form's cost, not on the copy.
9. **The ffi-pragmatist's veto on B holds**: one built-in already takes a C function's name (`exit`),
   B would take three more (`wait`, `send`, `close`); the historian's Go-style shadowable spelling
   contradicts Heroes' shadow ban (`selfhost/resolve/state.hero:345-350`).
10. **Prior art**: TSan's `report_signal_unsafe` flag stands in the very tool the historian read, so the
   sentence *"R1 has no ancestor"* does not reach the decision's `## Prior art` as written.

And of the rest: `5c5e6f811`, committed to the branch during the sitting, moved nothing any seat
measured (B's 124 declarations, the 38 door files, the 22 Rebel callers, the 86-block census identical
at both commits); the blind grading holds cell by cell, the 25 runs of the `now` builds included;
**all five `now` programs of task 2 are silently wrong, 5 of 5, once a document holds their own
separator**; defect 2582 reproduces, 21 of 200 runs torn.

## Where the seats disagree, stated plainly

- **Q1, J.** The ffi-pragmatist approved J on Darwin; the compiler-engineer vetoed J as written, on a
  program the ffi-pragmatist's matrix did not hold. Settled by the critic's build: the veto holds on
  Darwin too, and the validated record carries. The historian's objection to taking D-rebind's forward
  compatibility on Darwin as given stands, unanswered by any run: `macos-26` is owed.
- **Q2, R against E.** Every compiling seat approves R and objects to E. The blind seat's readers default
  to a normal end, 5 of 5, which is E's ending class: **R surprises a reader of today's spec, and its
  sentence is what removes the surprise** (`q2-r` 5 of 5). And "R" was two runtimes; R2 is the one that
  keeps the promise.
- **Q3.** The ffi-pragmatist and the historian approve the shape; the compiler-engineer objects on cost;
  the spec-warden objects, a veto by its own condition, now met; the blind count shows no difference,
  with a ceiling the critic has since measured. B against M: vetoed against unpriced, the blind count 10
  against 10.

## The resolutions

### R1, Q1: validated J, and defect 2522 stays open on its residue

**Adopted: route J with a validated record**: the compiler-engineer's `bjv` into B' on Linux, and the
same validation carried into D-rebind on Darwin as the critic built it (`251-critic/work2/q1/`). Each
buffer is fingerprinted once the C library has filled it, a changed buffer read as having no record, so
a stale record errs toward R1's told refusal and never toward Heroes code running inside a signal. **No
spec token**: the rule is § 13's *"never a result or a signal handler, nor called by one"*. **Route K
stays no** (the blind seat: it changes no reader's prediction; the spec-warden objects to it).

**Defect 2522 does not close on it.** Its rows stay open, `blocking`, until each is repaired or pinned:
the jump through a buffer J never saw, refused with words false for a correct program (the
ffi-pragmatist's 40 of 40; it owes words of its own); the two misses that give a correct program a false
message (critic finding 12); the installer reached by `dlsym` and those inside the shared cache; the run
on `macos-26` that answers the historian's TPRO question; and the golden census of the escape's ten
names. **What conservative would have been**: route K, 2522 closing with J's residue told in the spec
(+26 vendored); refused on the blind count and the spec-warden's objection.

### R2, Q2: R, as R2, with its sentence; the print lock and the panic's flush with it

**Adopted: a thread nobody joins aborts when `main` returns, saying how many, as the program's
mistake**, with the ffi-pragmatist's runtime **R2**: the gate asks the slot table first, reads no
balance while a slot is running or finished and unjoined, and **flushes stdout only when it can take
stdout's lock**, so a worker blocked writing into a pipe nobody reads cannot hold the message back.
Spec § 13's lease sentence gains the words, merged (+9 real tokens, the spec-warden's count):

```
-cell, and a lease nobody ends, like a handle nobody consumes, aborts when
+cell, and a lease nobody ends, like a handle nobody consumes or a thread nobody joins, aborts when
 `main` returns, saying how many.
```

**Its payment, and the one departure this synthesis makes**: the spec-warden's W1, registered at
10:14:05, after `run.sh` started at 10:08:24, is **admitted as registered**, because the brief's
condition was made unmeetable by the coordinator's own launch order (the seat launched 13 seconds
before the runner) and the seat's transcript shows the predictions written blind; this is a departure
from the brief's letter, written down as one. W1 holds: `q2-r` 5 of 5, `now` 0. Had W1 not been
admitted, R's runtime half would land alone and its sentence wait for a reading registered first.

**With it, runtime only**: the print lock that ends defect 2582's torn lines (the compiler-engineer: 18
of 200 torn under base, 0 under the lock, about 209 instructions per `print`), and the same try-lock
flush in `hero_panic`, whose hang behind a blocked stdout writer is a new defect filed with this sitting.
`exit(code:)` keeps ending every thread with the process. **What conservative would have been**: E,
sitting 252's sentence, a running thread ended with a count; refused on measurement (it hangs where R2
ends, and it switches the leak check off for any program that leaves a thread).

### R3, Q3: a value crossing whole waits; the rung stands

**Not adopted.** The rung stays as decision 2498 and panel 246 left it, one `i64` or one `str` each way.
Four reasons, any one sufficient: **the spec-warden's veto**, its stated condition met (W4 false for
both spellings); **Principle 0 unpaid** (no registered reading shows the effect, and the compiler-engineer
finds no compiler need); **the cost** (288 `match` statements in 109 files, reproduced); and **design.md
Part 7.13**, which both built crossings depart from, a change CLAUDE.md § 4 sends to a sitting of its
own. Spelling B is vetoed (the ffi-pragmatist's, holding), so a reopened Q3 is spelled M or a spelling
nobody priced. **What robust would have been**: the proposal; a veto is a refusal and not a price
(CLAUDE.md § 4), so it is recorded and not adopted.

**What would reopen it, each named so it can be run**: a reading registered before it runs, task 2 with
a separator in a document, which the critic's post-run measurement (all five `now` programs silently
wrong, 5 of 5) says is where the effect lives; a `(thread T)` that leaves under 30 `match` statements
incomplete, or a program on the closure list that needs a record across a thread; a ruling on which of
7.13's isolated heaps a crossing keeps; and answers to what the text leaves unsaid (a send and a waiting
`receive` at the close, queued messages, a handle in a list, `spawn` failing).

## The prior art, per resolution

Re-read on 2026-11-19 by the session that files this sitting, each page's own words checked against the
line (a reader with no stake in the resolution, the page quoted back); two of the historian's lines were
narrowed to what their pages state.

- **R1**: ThreadSanitizer's `setjmp` records (`tsan_interceptors_posix.cpp`, `tsan_rtl_amd64.S`), its
  `report_signal_unsafe` flag (`tsan_flags.inc`), and Go's issue 35814. These go into the decision issue.
- **R2**: C++'s `std::thread::~thread`, LeakSanitizer's design document and Memcheck's manual. These go
  into the decision issue.
- **R3**: none; it adopts nothing.

## Predictions to score

| seat | prediction | checkable at |
|---|---|---|
| compiler-engineer | with `bjv` landed, `ijstale.c` as a run golden is refused on both CI Linux legs; the CI's `run:` line goes from 3 failed to exactly those five goldens until B' fixes their wording | the push that lands R1 |
| compiler-engineer | the `run` count is unchanged with R at M-rebel-library's close | M-rebel-library's close |
| compiler-engineer | a landing of `(thread T)` as a type touches at least 100 files of `selfhost/` | a reopened Q3 |
| ffi-pragmatist | `miss.c` HOW 2 is refused 20 of 20 at `-O2` on `macos-26` | the CI's `macos-26` leg after R1 lands |
| ffi-pragmatist | `bw_unwaited_done` is refused under R2 on the CI's Linux x86-64 leg | the push that lands R2 |
| ffi-pragmatist | the `close` member of `fixedbugs-396-posix-fills-its-buffers` builds unchanged under M and cannot under B | a reopened Q3 |
| spec-warden | W1 to W8, admitted as registered (R2 above); scored: W1, W2, W3, W5, W6, W7, W8 hold; **W4 false** (B 0, M 0) | scored here |

## How the sitting went, for the next

- **The tree was not frozen.** The coordinator committed `5c5e6f811` (Rebel packages only) to the branch
  during the sitting; every seat worked in its own `git archive` of the frozen commit, and the critic's
  re-count shows nothing a seat measured moved. The rule was broken all the same, and is kept from
  sitting 252 on.
- **The blind seat leaked three ways** (labels by standard input, the folder's path, the main checkout's
  git status), and `context: no` did not see the third. **Owed before sitting 252's runner spends**:
  `claude -p ... < /dev/null`, and a way to keep the git status out inside the repository's root; one
  route nobody listed is a nested, empty git repository in the blind area, which the CLI would read
  instead of the main checkout's: **unrun**, and to be tried with a key that makes no billed call before
  any paid session.
- **The seats ran long**: the compiler-engineer about three hours, asked by the coordinator at 13:02 to
  close, its unrun emulated x86-64 rows written as owed to the CI's Linux x86-64 leg.

## What this sitting files and owes

- **Defects filed with this sitting**: the `hero_panic` hang behind a blocked stdout writer
  (`runtime/parts/panic.c:59`), `blocking`; the `runtime` suite red on the branch's platform members,
  `blocking`; the blind runner's leaks, `blocking` for the sittings that follow.
- **Comments on open defects**: 2522 (R1 adopted, its rows open), 2552 (R2 adopted), 2582 (reproduced,
  21 of 200; the print lock adopted).
- **The implementation**, one lane before M-rebel-library's gate: R2 with its sentence in spec § 13 in
  one change (the spec and the runtime land together, so neither says what the other does not), the print
  lock, the panic's flush, validated J on Linux and Darwin, the goldens J and B' move, and the
  `runtime` suite green.

## Author's verdict

**Ratified, 2026-11-19**, R1 to R3 as proposed, the W1 departure with them: the author's word given at
about 13:10, before this synthesis was written, meant as *OK to the ratifications as proposed*, of
sittings A, B and C of M-rebel-library, recorded as given. Its decision is
https://github.com/heroes-lang/heroes/issues/2590, opened and closed that day with this file's commit,
its `## Prior art` the six lines re-read that day. Filed with it: defects 2587 (a panic behind a blocked
stdout writer), 2588 (the `runtime` suite red on the branch's platform members) and 2589 (the blind
runner's leaks); commented: 2522, 2552 and 2582.
