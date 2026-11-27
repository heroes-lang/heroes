# Panel 262, the completeness critic: second reading, the reports before the synthesis

Started 2026-11-26 23:14, a fresh agent, in the detached copy `.claude/worktrees/262-critic` at the base `87003cb42`
(`git -C 262-critic log -1`), scratch `.claude/worktrees/262-critic-scratch/`, `TMPDIR` the copy's `tmp/`. The machine
read `load averages: 113.79 129.95 153.64` at 23:14 (`uptime`), so **no duration below is a measurement**. Written as
it goes; a section marked *in progress* was not finished. Findings are numbered **K<n>** (C1 to C28 are the first
reading's); the last section orders what the synthesis must settle.

## 0. What was read and run, and on what

- Read, whole: `262-briefs/critic.md`, `00-shared.md` (as repaired), `blind/registration.md`, my first reading
  `critic-briefs.md`, the reports `compiler-engineer.md`, `ffi-pragmatist.md`, `spec-warden.md` and the texts
  `spec-w1..w9.txt`, `historian.md`.
- Compilers built here at the base: the seed `sources 1e1eb23a1927d35b` (`./heroes --version`, the brief's), stage 2
  **`sources 6ce3f9e116d22e3a`**, the brief's key (`262-critic-scratch/r2-build-stage2.log`; `real` 16:46 against
  `user` 189, a load of 150: no duration). **The second column**: `git archive main runtime` at `bdef9ba06`
  (`262-critic/rt-main/`, `HEROES_RUNTIME`).
- **The warden's counts reproduce** with this stage 2, no `--refresh`: the base's spec 10,067, 10,245, real 13,439;
  `main`'s 10,318 and 10,507; `applied/main-w2.md` (`shasum` `b1e76202bfc13a7c`, the warden's) 10,500 and 10,689,
  **+182 and +182**; `main-w1.md` 10,381 and 10,571; and `main-w2.md` differs from `git show main:spec/heroes-spec.md`
  by the one sentence alone (`diff`: one line out, two in, the text found whole once by `grep -c -F`).
- **Linux arm64**: one container `p262-critic` (`heroes-linux-arm64:latest` `338a1044ac90`, `--rm --pull never
  --network none`) over `git archive 87003cb42` with `main`'s runtime and the programs added
  (`262-critic-scratch/linux2/`, its `L.sh`).
- **Not run, and why**: the x86-64 image (an emulated stage 2 at a load of 150); the Windows box (no coordinator's
  word); any `--refresh` (paid); a web fetch (the brief forbids downloads); the net's own tests with w2 applied (913
  tests at this load: the warden's stopped at 6 in 16 minutes).

## 1. The labels of panel 259's 48 wrong answers, graded apart (the second reader the repair could not give)

**How.** `docs/panel/259-reports/llm-ergonomist-blind-answers.tar.xz` unpacked in `262-critic-scratch/a259/` (never the
tree). For each of the 48 units of `blind/labels-259/units.txt` I read its session's `result.md`: the answer block for
that program or design and the `choice_points` whole, 13 sessions. Rule applied: the registration's as repaired (*"the
choice point the reader names as deciding that program's verdict, or its answer block's own reason ... every label its
reason supports"*), with one reading of my own made explicit before I read grader A: **a label is given where the
unit's answer block, or the shared block its verdict cites, offers the consideration as a ground for the wrong
verdict; a consideration the reader says changes nothing is not a ground.** My file,
`262-critic-scratch/labels/graderC.tsv`, was written and hashed **before I opened `graderA.tsv` or `agree.txt`**
(`shasum -a 256` at 23:19:43: `23228bf968c913bb...a5b7`, `labels/graderC.registered`). Then `python3 -I
labels-259/agree.py graderA.tsv graderC.tsv` (`labels/agree-A-C.txt`):

| label | grader A | me | both | agreement | kappa |
|---|---|---|---|---|---|
| W | 42 | 43 | 42 | 0.98 | **0.90** |
| H | 30 | 40 | 30 | 0.79 | **0.50** |
| O | 4 | 3 | 3 | 0.98 | **0.85** |
| I | 2 | 2 | 2 | 1.00 | **1.00** |
| S | 7 | 8 | 4 | 0.85 | **0.45** |
| N, L, T, P, R | 0 | 0 | 0 | 1.00 | none (no instance on either side) |

Same set of labels on **32 of 48** units; at least one label shared on **47 of 48**. Against the repair's rule-grader
(kappa 0.02 on W, 0.07 on H) two readers agree far better, as its § The labels, tried predicted.

**Where we differ, 16 units, two causes, each a rule the registration leaves open:**

- **H, 10 units, all mine and not grader A's** (`t1-h-4` q1 to q4, q6, q7; `t1-h-1` q1, q2, q5, q6). **K1.** The
  registration's trial says *"the other 12 W units are two sessions (`t1-h-1`, `t1-h-4`) that apply the clause to
  switches without saying what *handler* means"*. **`t1-h-4` says it**, in the block every one of its verdicts cites
  (`s/t1-h-4/result.md:9`): *"A callback may abort if C called it after C left a callback ("handler") in one of these
  ways"*. `t1-h-1` names what was left without the word: *"after `co_start` left its own frame by `swapcontext`"* (q1,
  a frame of C's), *"Clause (b) applies to every call of `before`, and its frame is abandoned"* (q5), *"Clause (b)
  applies to `holder` and `jumper`"* (q6), both the program's callbacks. So H turns on whether a gloss in brackets, or
  a frame named as the thing left, counts as *"handler read as ..."*. Grader A read it as not; I read it as so.
- **S, 7 units** (A alone: `t2-h-1` A, B, C; me alone: `t2-h-4` B, C, `t2-h-5` C; and `t1-h-1` q5, where A gave O and I
  gave W, H, S). `t2-h-1`'s § 9 doubt sits in the shared *"Why"* list beside the reader's own *"the clause covers A, B
  and C in the same way"*; `t2-h-4`'s is a bullet under B's and C's own verdicts (*"The spec also doesn't say how
  "running out of stack aborts" (§9) is checked on a stack the runtime didn't allocate"*, `:18`). Grader A labelled by
  the session's reasoning, I by the unit's block. `t1-h-1` q5's reason is two things, *"Clause (b) applies ... If
  records live on the stack, the program runs out of stack, and §9 promises that "running out of stack aborts""*: W
  and S by the rule's letter; grader A's O follows the registration's note that it *"rests on where records live"*.

**What it says for the paid run.** W, O and I are reliable between two readers on this data; **H and S are not
(kappa 0.50 and 0.45), and the cause is the rule's wording, not the readers**: the registration should say, before any
session, (i) that H is given wherever the answer block or a choice point names the thing C left as a callback or a
frame of C's, in any words, a gloss in brackets included, and (ii) that S is given only where the unit's own block
offers § 9 as a ground, never from a shared list the reader says applies to every design alike (or the reverse; either
rule, fixed). **N, L, T, P and R have no instance in 259's archive on either side, so their reliability is unmeasured
by any trial**, and they are the labels this run exists to read (L is D5's, T is `h9`'s, N is D2's); two readers
apart on the run's own wrong answers is then the only measurement there will be, and D4 already reports both graders'
labels apart (`registration.md:312`), which it should keep. **And one definitional fault, K2**: L's second clause,
*"also a `RUNS` that reads silence as a promise"*, by its letter labels `t1-h-3` q4 and `t1-h-5` q4 (each: *"The
specification has no rule against Heroes frames on a C coroutine stack"*, a `RUNS` on silence), whose `RUNS` was wrong
only because the key was a compiler defect's; both graders gave O by the registration's note 5. A rule that labels a
reading right on every correct runtime is a label of the key, not of the text: L's second clause should be bounded to
a program the key aborts **by the runtime's design**, and a defect's key graded O, as written for `n3`.

## 2. The historian: does every `verified` precedent quote its page? (read, not re-fetched)

Each of the report's 38 `verified`/`unverified` marks (`grep -c`) stands beside a URL and a quotation, and the report
says where a sentence is the fetch tool's summary rather than the page's words (eight places, `grep -n "tool's
\(reading\|summary\|keyword\)\|as the tool summari"`: lines 36, 60, 154, 220, 230, 236, 247, 284, 318, 455). **Two
places break that discipline. K3.**

- **Line 505**, under *Which precedent each candidate rests on*, (b): *"Go's promise keyed to *"does not change any
  signal handlers or masks"*"*, in quotation marks, where § 5 (line 230) says of the same sentence *"as the tool
  summarised a sentence too long to quote: if the non-Go code changes no handlers or masks, the behaviour is the same
  as for a pure Go program"*. A summary is re-presented as the page's words, and it is one of the three precedents the
  historian gives for (b), the shape w2 takes. The page's own sentence is **unread by me** (I fetched nothing: the
  brief forbids downloads); the synthesis quotes it from `pkg.go.dev/os/signal` before a `## Prior art` line cites it
  (`.claude/rules/records.md` § And the record says whose idea it was: *"a claim the page does not carry is cut"*).
- **Line 510**, (d2): *"Go's Linux-only `SIGCANCEL` case"* appears nowhere else in the report (`grep -n -i sigcancel`:
  that line alone), with no URL and no quotation: an unsourced precedent beside sourced ones.

The rest (the `_longjmp` table row, 516's scope, golang 62440's description, Perl Coro's default, CVE-2023-28320's
versions) are marked as the tool's reading in their own sentences, which is the honest form.

## 3. The repair of the briefs, against my first reading's nine items and C1 to C28

Read in `panel262/docs/panel/262-briefs/` (the repaired copy; my copy's `262-briefs/` is the first reading's, `diff
-rq`). **Every one of the nine items was applied**, and the numbers I re-ran are the commands':

| item (first reading) | applied where | a number re-run now |
|---|---|---|
| 1 x1 not waiting on conditions; route A incl. Darwin x86-64; the seventh defect; q4 per route (C2, C3, C4, C8) | q3 rewritten (*"Both conditions are met, and x1 is still false as written"*), five falsifiers listed; § 3.5's `d-lane-sig-family-1` row; q4's *"that split is route C's alone"* | the compiler-engineer forced route A on both platforms: `b_0`, `cs_0` UNFOLLOWED, 2706's two HELD and ONSTACK (its § 3), as the repair wrote |
| 2 panel 258's stop in q9 and § 5 (C7) | q6, q9, § 5's watchdog | `grep -rlF 'jumped out of the code it interrupted' tests/`: 0 here too |
| 3 the registration (C13, C14, C15, C24) | every wrong verdict labelled, several per answer, T I P R S, D2 fixed to `h7 h9 h10 h11`, `h9`, `h12` and D5 added, the trial on 259 | `python3 -I agree.py graderA.tsv graderB.tsv`: the registration's table, kappa 0.02 W, 0.07 H, every row; `shasum -a 256 graderB.py` = `graderB.registered` |
| 4 re-anchor on `main` (C1) | the base moved to `87003cb42`, the second column ordered | `git log --oneline 87003cb42..main -- runtime spec`, 23:15: 199e, 199f, 203f and merges (K4 below) |
| 5 the landing outside the spec (C27, C5) | q13: the site, 91 `run` refusal goldens and the `check` outlier, the net's own tests | `grep -rlE 'seen to end\|from inside' tests/golden/run --include='*.expected' \| wc -l`: **91**; the only other file is `check/package-a-file-source-in-no-repository.case/main.expected` |
| 6 the facts not handed (C9 to C12, C28) | 2831 in § 3.5; the runtime's handler in q2 and § 3.3; design.md's rows in § 3.6; q4's conditional aborts; § 3.8; C28 in q9 and § 5 | none needed |
| 7 the registration's smaller faults (C16 to C19) | the room's sentence on `n2`, `n3`; `h6` against `h4`; `n3`'s condition; the plain key; `prelude.h`; `h2.h`'s first line; `inputs.sha256` | `shasum -a 256 -c inputs.sha256` in `blind/`: **40 OK**, none failing; `shasum -a 256 -c blind/arms.sha256` from the copy's root: OK |
| 8 the seats' instructions (C20, C21, C23, C25, C26) | four variants; the image's libraries; JNI, Lua, Python; the shapes; (d6) to (d9) | the ffi-pragmatist built all four and the historian sourced Pth, libcoro, QEMU (all RR) |
| 9 small corrections | `sigjump.c` 130-132 and 134-145, 192a's `+9 −177`, the room's words as the runtime's, 2046 | read |

**Three repairs are right as far as they go and leave a fault the run will meet. K4, K5, K6.**

- **K4. The ground moved again: the briefs' remedy for it reached every builder, and the briefs themselves stop
  before it.** `main` is `bdef9ba06`
  (23:15); since the base: step **199e** (2853, `01706218e`), **199f** (**2852, G**, `8d8263f15`, merged at
  `c2b7574bf` 21:38) and 203f. `00-shared.md` names 199e and 203f (written at 21:18) but **not 199f**, and § 3.4, §
  3.5 and the registration still say *"2852 ... open and unrepaired"*, *"`mj_carved` ... CALLED"*, *"`n1 --sanitize`
  is re-run on Darwin on the run's day"*. The three builders each found 199f and ran a second column (the
  compiler-engineer at `c2b7574bf`, the ffi-pragmatist at `0ce5b0cf3`, the warden at `deef27472`); `git log
  c2b7574bf..main -- runtime spec` is empty, so **their three columns are one runtime and `main`'s today**
  (`diff -rq` of my `git archive main runtime` against the base names the same five files they name). What the
  synthesis inherits: 2852 is in (its issue open until the gate), `mj_carved --sanitize` runs on `main`, **and the
  no-handler class is not closed on `main`** (the compiler-engineer's `fm_top`, re-run below).
- **K5. The registration's arm `now` still hashes the base's spec.** `blind/arms.sha256` names
  `spec/heroes-spec.md` = `3df14254...` (the base's, 516 lines); the registration says *"unless the coordinator names
  the base's spec for the run, `now` is the landed spec, its hash written here before any session"*, and the warden
  registered its predictions against `main`'s spec (`9ff7d2a1...2fc3`). Nothing yet writes the landed hash into
  `arms.sha256`; a runner started from the file as it stands would hand the readers a spec `main` no longer holds.
- **K6. The trial of the labels asked for "two readers apart" and got a rule; my reading in § 1 is the second
  reader, and it shows the rule's open points (H, S) and the labels no trial can measure (N, L, T, P, R).**

## 4. The seats disagree on `spec-w2`: what each text promises, against the other seats' programs

**Nobody held the warden's texts to the builders' programs**: the compiler-engineer (*"none stands in ... at 22:32"*,
its § 7) and the ffi-pragmatist (*"not readable to me while I write"*, its § 6) judged the candidates as § 4
describes them, and the warden ran the blind key alone (its § 2). This table is that missing join, each row a program
a seat **ran** (both platforms, both runtimes unless said; its outcome the seat's, the starred ones re-run by me in §
5), each cell what the text tells that program's author: **run** (promised, by the text's words or by the
end-clause a reader draws from them), **may**, **aborts**, **silent**, and **FALSE** where the run breaks a promise.

| program (seat) | ran | `now` | w1 | **w2** | w3 | w4 | w5 | w6 | w7 | w9 |
|---|---|---|---|---|---|---|---|---|---|---|
| `wd`, `siglongjmp` out of a SIGALRM landing anywhere (CE) | stop at the jump in 53 of 272, else runs | silent | silent | run, and (iv) *aborts at the jump*: **true** | silent | the stop unsaid: **FALSE** by the warden's own reading of w7 | the same | the same | run: **FALSE** (the warden's § 3) | silent |
| `wide` 8, nine handlers left by followed `siglongjmp`s back to their raise, then a coroutine resumed (CE)* | REENTERED, false of it | silent | silent | run (ended, *"where this runtime can tell"*: it tells): **FALSE** | silent | run: **FALSE** | run: **FALSE** | run: **FALSE** | run: **FALSE** | silent |
| `fm_top --sanitize`, no handler, an assembly fibre with `x29` at its top (CE)* | Darwin CALLED, base and `main`; Linux runs | silent | S2: **FALSE** on Darwin | (v): **FALSE** on Darwin | silent | silent | silent | silent | silent | silent |
| `sw`, `lj` carved `--sanitize`, no handler (FFI) | Darwin: ASan's report (`sw`) and CALLED (`lj`) at the base; run on `main` | silent | S2 at the base: **FALSE** (`lj`); `main` true | (v) the same | silent | silent | silent | silent | silent | silent |
| `p4j`, assembly exit then a later followed jump above it (CE) | runs | may | may | may | may | may | may | may | may | may |
| `b_0` on route A (CE, forced and simulated) | UNFOLLOWED | silent | silent | run *"where this runtime can tell"*: scoped, true | silent | scoped | scoped | scoped | run on *"Linux with glibc on 64-bit Arm"* where the glibc moved: **FALSE** | silent |
| 2705, `h8`, the own-stack coroutine (key) | ONSTACK | may | may | may (*"a coroutine the signal did not interrupt"*) | may | may | may | may | may | aborts at the exit: **FALSE** (the abort is the callback's) |
| `gtc`, SIGALRM `swapcontext` scheduler, C's bodies, callback after (FFI) | runs (Darwin `--sanitize`: 4 of 646 SEGV or silent 134) | may | may | may | may | may | may | may | may | **aborts: FALSE** |
| signal stack trick RR, Pth's, libcoro's, QEMU's design (FFI) | runs | may | may | may | may | run: true | run: true | run: true | run: true | silent |
| signal stack trick RL, JR, JL, hand variants (FFI)* | REENTERED, every later callback on the thread | may | may | may (*"into the handler's own frames"*) | may | RL: run, **FALSE** | RL: **FALSE** | RL: **FALSE** | RL: run (ended by its return), **FALSE** | silent |
| libuv's pool, `dispatch_async_f`, `SIGEV_THREAD` ×3, `n4` (FFI, key) | the thread's words | silent | silent | **aborts: true** | silent | silent | silent | silent | silent | silent |
| `h9`, assembly exit on one thread, callback on another (key) | runs | may | S2: run, true | (v): run, true | silent | silent | silent | silent | silent | may |
| `cu_alarm`, `siglongjmp` out of C's `read` (FFI) | runs | silent | silent | run: true | silent | run: true | run: true | run: true | run: true | silent |
| `ck` handler on a coroutine's stack, `siglongjmp` or `setcontext` to the runner (mine, § 5)* | STANDING, words naming causes it has none of | silent | silent | run if the runner's pending call is *"a call the signal interrupted"*, else *may*: **unreadable** | silent | the same | the same | the same | as w2: unreadable | silent |
| `setcontext` from an alternate-stack handler back into the coroutine it interrupted (2702, 2707 ×2 goldens, `ck`)* | SWITCHED | may (exit 4) | may | run by the letter, kept true only by *"where this runtime can tell"* | may | the same as w2 | the same | the same | run: **FALSE** | *aborts there*: **FALSE** (the abort is the callback's, and `h11`'s `setcontext` runs) |
| `wdr`, the watchdog installed by a raw system call (mine, § 5)* | guard D at the next use, 2 to 8 of 34, false words | silent | silent | (iv) *"saying so"*: **FALSE** (guard D's words) | silent | silent on the stop | the same | the same | run: **FALSE** | silent |
| `so`, a callback recursing past a C coroutine's stack (CE) | SEGV or Bus error, nothing said | (§ 9 *"aborts"*: **FALSE**, every arm, untouched by every text) | | | | | | | | |

**Reading it.** **w1, w3 and `now` promise nothing a run breaks but w1's S2 on Darwin `--sanitize`**; every text
that draws an end from *"its return, or a jump ... back to a call the signal interrupted"* (w2, w4, w5, w6, w7) is
falsified by `wide` 8 on both platforms and both runtimes, and every one of them but w2 by the watchdog too (w2's (iv)
is the clause that saves it there); w4, w5, w6 and w7, which end a handler at its return, are falsified by RL (a
hand variant, no sourced library's) where w2 and w3 keep the re-entry inside their *may*. **So the compiler-engineer's
veto reaches w2 by two programs and the warden's text survives the rest of them**: the watchdog (its (iv)), 2705 (its
*"did not interrupt"*), route A (its *"where this runtime can tell"*), the thread (its last clause). The two that
break it are **runtime defects, not readings**: `wide` 8 tells a correct program a cause it does not have, and
`fm_top` tells a program with no handler that one is running. **Which side a command checks**: both, the same command,
`WIDE_N=8 <wide's binary>` and `<fm_top's binary built --sanitize>` on Darwin, at the landing's commit: while either
refuses, w2 promises what the runtime does not do; once both are repaired, the veto's two remaining grounds are
scoped by w2's own words (§ 5 re-runs both). **My own shapes (§ 5) add two more against w2 and narrow its rescue**: `wdr` falsifies clause 4's *"saying so"* (K10), and clause 2's promise survives the three refused `setcontext` goldens and `ck`'s two STANDING cells only through *"where this runtime can tell"* (K9), a condition that absorbs any refusal whatever and that no reader can evaluate (K7): read as a promise, w2 is false by five more programs; read as scoped, it promises nothing a program can rely on and the warden's D1 and D2 predictions grade a reading the text does not fix.

**K7. w2's run-promise rests on a condition no program shows, and the registration does not yet say how it is
classed.** *"where this runtime can tell that the jump left it"*: the landed spec names *the runtime* once, `spawn`'s
line 393 (`git show main:spec/heroes-spec.md | grep -c -i runtime`: 1), and nothing a reader can read tells them
where it can tell (the compiler-engineer: *"the route is invisible in any program"*). The warden says a reader who
reads it as a platform split (P) *"reads route A right, so under w2 P is not a misreading"*, and in the same report
registers P 1 to 4 among w2's **wrong** answers and its own falsifier as *"w2 failing D1 on `h1` (2 or more `NOT
PROMISED` on it, read as P)"*. Both cannot hold: before any session the synthesis writes w2's classification of
`h1`, `h7`, `h10`, `h11` (promised, or a *may* scoped by the runtime), since D1 and D2 grade by it
(`registration.md` § The grades: *"text-right: the reader's `NOT PROMISED` for a program the arm's classification
puts inside a *may*"*).

## 6. w2 clause by clause against the runtime, at the base `87003cb42` and at `main` `bdef9ba06`

`main`'s runtime is `git archive main runtime` into `262-critic/rt-main/` (`rt-main/HASH`); `diff -rq` against the
base names five files, `rebind_entries.c`, `rebind_jump.c`, `sigentries.c`, `sigguard.c` (199f's walk bound, +23 −2),
`sigjump_save.c` (199e's shadow), and **`thread.c`, `alloc.c`, `lend.c` and `sigguard.c:175-212` are byte-equal in
both** (`diff`). Pointers below are `main`'s.

1. **"A signal handler of C's ends, for the callbacks of its thread, when it returns"**: rule 5's *"a wrapper's end
   drops its own entry"* (`sigentries.c:33-38`; Darwin's trampoline twin), the record per thread (`HeroSignalStack`,
   `_Thread_local`). True, both columns, **but for a handler whose frames C jumps back into after it returned**:
   rule 6 re-enters it (`sigentries.c:39-47`), and where the jump lands on the alternate stack the library left
   registered (the trick's L variants) every later callback on the thread is refused for good (the ffi-pragmatist's
   C-only rows). w2 names *"a jump ... into the handler's own frames"* only as a way of **leaving** a handler (*"after
   C left it any other way (by ... or into the handler's own frames, say)"*), never as undoing an end: a reader of
   clause 1 predicts RL's later callbacks run. The real libraries (RR) run, so this is a hand variant's, but the
   sentence's grammar is the gap. *(The ffi-pragmatist's condition on (b), *"unless it says a jump back into a
   handler's frames holds it running again"*, is half met.)*
2. **"or when C jumps out of it by `longjmp`, `siglongjmp`, `_longjmp` or a `setcontext` to a context `getcontext`
   took, back to a call the signal interrupted, where this runtime can tell that the jump left it"**: rules 4 and 6
   on route C's decoded target (`sigjump_decode.c`) or route A's record; the hooks `sigjump_save.c` (Linux; its
   `setcontext` at `:211`, `hero_jmp_set` `:93-128`) and `rebind_jump.c` (Darwin). The three `_longjmp` 2522 cases
   run, so all four names are followed. *"where this runtime can tell"* is HELD's, ONSTACK's and UNFOLLOWED's own
   words, *"so whether the jump left the handler is not known"* (`sigguard.c:630`, `:636`, `:641-642`): true by
   construction. **False, both columns and both platforms, where the runtime can tell and then forgets**: `wide` 8
   (§ 5). And *"back to a call the signal interrupted"* is unread on a coroutine: the call a handler that interrupted
   a coroutine jumps **back to the runner** for is the runner's pending `swapcontext`; my `ck` asks it (§ 5).
3. **The *may* list** (*"from inside the handler or after C left it any other way (by a jump no function of the C
   library makes, a `swapcontext`, or a jump into a coroutine the signal did not interrupt or into the handler's own
   frames, say)"*): STANDING, SWITCHED, ONSTACK/HELD, REENTERED, CALLED: a *may*, true on every route; Windows
   refuses none (`sigguard.c`'s `#else`, `hero_callback_entered` counts and returns), which a *may* allows.
4. **"Where a handler jumps out of code the signal interrupted inside this runtime's allocator, or its record of what
   is lent to C, the program aborts, at the jump or where it next uses that code, saying so"**:
   `hero_signal_jump_checked` (`sigguard.c:198-212`), asked by the jump hooks (`sigjump_save.c:86`, `:108`, `:129`;
   `rebind_jump.c:150`, `:171`, `:192`) **only `if (standing && !hero_sig_standing(own))`**: the thread stood in a
   handler **the runtime saw begin** and the jump left every one. *"At the jump"*: true for a wrapped or rebound
   handler (the compiler-engineer's 53 of 272, both platforms). *"Where it next uses that code, saying so"*: the next
   use is guard D (`alloc.c` `hero_alloc_reentered`, `lend.c` `hero_lend_reentered`), whose words are *"a signal
   handler used this runtime's allocator while the code it interrupted was inside it ... a Heroes function given to C
   as a signal handler, or called from one"*: **not** *"so"* (that a handler jumped out), and false of a program that
   gave C no function as a handler; panel 258's own comment says so (*"stopped the program saying *a Heroes function
   given to C as a signal handler*, of a program that has none"*, `sigguard.c:180-183`). Reached where the jump's
   handler was never seen begin (a raw system call, `dlvsym`, `sigguard.c:61-74`) or the jump is one the hooks never
   see; my `wdr` asks the first (§ 5). POSIX only (`#if !defined(_WIN32)`, `:194`).
5. **"on a thread where no signal handler of C's has run (counting a signal with `hero_signal_watch` runs none), no
   coroutine switch or jump of C's makes a callback abort in these words"**: the walk (`sigguard.c:715-730`) is the
   question that reaches a thread with no entry; 199f bounds it at a `makecontext` range's top only
   (`hero_sig_coroutine_top`, `hero_rebind_coroutine_top`). **False at the base** (`mj_carved`, `lj` carved,
   `--sanitize`, Darwin: CALLED) **and at `main`** (`fm_top --sanitize`, Darwin: CALLED, § 5); true on Linux arm64
   for every program any seat ran.
6. **"and a callback C calls on a thread of its own aborts, saying it ran on a thread this program did not start"**:
   `hero_thread_guard` at every handed function's entry (`thread.c:145-148`, emitted by
   `selfhost/emit/callback_guard.hero:144`), `hero_thread_is_home` set only by a claim (`thread.c:74-91`: `main` and
   `hero_thread_spawn`'s workers), no `#if` in `thread.c`, so Windows too (unrun); its words `thread.c:136`, as w2
   quotes them. True, both columns. The thunk asks DELIVERED before the thread (`sigguard.c:673-679`), so a function
   the system calls **as** a handler on C's thread says DELIVERED's words, which line 437 already covers.

**w2 read against the blind key, by me, program by program** (the key as the repair, the compiler-engineer and the
warden ran it, both platforms, both runtimes): `h1` (the handler's `siglongjmp` to the `sigsetjmp` its `raise` sits
under), `h3`, `h7`, `h10`, `h11`: ended, promised a run, **run**; `h2`, `h4`, `h5`, `h6`, `h8`, `h12`: in the *may*
(assembly; from inside; a `swapcontext`; the handler's own frames; *"a coroutine the signal did not interrupt"*,
twice), **abort**; `h9`, `h13`, `n1`, `n2`, `n3`: clause 5's thread with no handler of C's, promised, **run**; `n4`:
clause 6, *aborts*, **aborts in the thread's words**. **No blind program reads wrong under w2**, for 259's reader (no
switch without a handler is in a *may*), 257's (the four D2 programs are promised, not *may*), or the list's (`h12`
is named). The two programs that break w2 (`wide` 8, `fm_top`) are not in the key, and the run cannot see them.
Under **w1** (the warden's rival), `h7`, `h10`, `h11` stay inside *"a jump or a `setcontext` onto a stack once given
to `makecontext`"*, a *may* on three programs that run: the N the warden predicts w1 fails D2 on.

## 5. What I ran: the two decisive falsifiers again, and three shapes of my own (both platforms, both runtimes)

My stage 2 (`6ce3f9e116d22e3a` on both), the base's runtime and `main`'s (`bdef9ba06`), plain and `--sanitize`,
three runs per cell (`262-critic-scratch/r2/run.sh`, classifying each run by its arm's words; outputs
`r2/mac/cells.txt` and `linux2/out/cells.txt`). **Every cell is one verdict of three**, and **every cell but
`fm_top --sanitize` is the same on this Mac and on Linux arm64** (`diff` of the two `cells.txt` without `fm_top`'s
lines and the shells' `Abort` lines: empty).

**The compiler-engineer's two, re-run (they break w2's clauses 2 and 5):**

| program | this Mac base / `main` | Linux arm64 base / `main` |
|---|---|---|
| `wide` `WIDE_N=7` | runs (`result (102 is right): 102`), both builds | the same |
| `wide` `WIDE_N=8` | **REENTERED**, both builds, both columns | **REENTERED**, the same |
| `fm_top`, plain | runs | runs |
| `fm_top`, `--sanitize` | **CALLED / CALLED** | runs / runs |
| the ffi-pragmatist's trick, `P262_SST` RR and RL, C-only and not | RR runs, RL REENTERED, every cell | the same |

So the compiler-engineer's `wide` and `fm_top` are what that seat said, on my compiler, and **`main` (199e and 199f
in) does not move either**. Drafted, unfiled: `rebel-scratch/coord/issues/d-lane-sig-family-3.md` (`fm_top`), `-4`
(`wide`), `-5` (`so`), `-6` (`gtc`), all *"github: none"* at 23:13, as `-1` and `-2` still are.

**Mine, each new to the tree** (`docs/panel/262-reports/programs/critic/`, canonical and checked by the seed):

1. **`ck`: a handler that interrupted a coroutine, left by w2's own exits** (`ck.h`: a `makecontext` coroutine on a
   heap stack raises SIGUSR1; the handler on the coroutine's stack (`CK_ON=0`) or an alternate stack (`1`) leaves by
   `siglongjmp` (`CK_HOW=0`) or `setcontext` to a context `getcontext` took (`1`), back into the coroutine at a save
   made just before the raise (`CK_TO=1`) or to the runner's save made before it switched in (`0`); then the function,
   twelve frames down). Every cell the same on both platforms, both columns, both builds:

   | handler on | exit | back into the coroutine (`TO=1`, *"a call the signal interrupted"*) | to the runner (`TO=0`) |
   |---|---|---|---|
   | the coroutine's stack | `siglongjmp` | runs (new) | **STANDING** (new) |
   | the coroutine's stack | `setcontext` | runs (= 2702's `cs_0`, `h11`) | **STANDING** (new) |
   | an alternate stack | `siglongjmp` | runs (= 2707, `h10`) | runs (new) |
   | an alternate stack | `setcontext` | **SWITCHED** (= 2702's alternate-stack case and 2707's two `setcontext` goldens, all three *"is-refused"*) | runs (new) |

   **K8.** The two new STANDING cells are told *"C left the handler in a way this runtime does not follow (an inline
   `__builtin_longjmp`, a C++ exception, a jump inside a system library)"* (`r2/mac/c/out-ck-CK_ON-0_CK_HOW-0_CK_TO-0-
   base-plain-1.txt`): **three causes the program has none of**, for a `siglongjmp` and a `setcontext` the runtime's
   hooks do follow, which is defect 2705's class (*"The words a refusal prints name causes the program does not
   have"*), the one ONSTACK's arm was made to end. The program leaves a handler for good to its runner, as 2855's
   golden coroutine leaves for good by `longjmp` to its host (*"runs"*); with the handler on an alternate stack the
   runtime lets it run. A `blocking` defect by § Bounded discovery (*"a false message"*, and by my reading *"a correct
   program refused"*), **unfiled; the coordinator's.**
   **K9. And it is w2's clause 2 the cells test.** By w2's letter a `setcontext` to a context `getcontext` took *"back
   to a call the signal interrupted"* ends the handler; **the tree's own goldens refuse three such programs**
   (`fixedbugs-2702-a-handler-on-the-alternate-stack-left-by-setcontext-to-its-coroutine-is-refused`, `fixedbugs-2707-
   a-setcontext-out-of-a-handler-on-an-alternate-stack-into-a-coroutine-mapped-{after,before}-it-is-refused`, whose
   header calls them *"a CORRECT jump out of a signal handler"*), by design (rule 6: *"A `setcontext` whose target
   lies on a coroutine's stack holds every standing entry until its handler ends"*), and my `ck` refuses a
   `siglongjmp` and a `setcontext` to the runner. w2 keeps all five true **only through *"where this runtime can
   tell that the jump left it"***, and what decides them is visible in the program (`SA_ONSTACK`, whether the target
   lies on a coroutine's stack) and named in no text. The warden's § 3 cites `h1`, `h7`, `h10`, `h11` for clause 2 and
   none of the three refused goldens.
2. **`wdc`: the watchdog leaving by `setcontext`** (`wdc.h`, the compiler-engineer's `wd.h` with the other exit w2
   names). 34 runs per cell, `WD_US` cycling over the same 17 delays (`r2/rate.sh`): this Mac base plain **9 stopped at
   the exit**, 25 run; `main` plain 3 and 31; `main --sanitize` 6 and 28; Linux arm64 base plain 4 and 30, `main` plain
   1 and 33, `main --sanitize` 6 and 28. Every stop in panel 258's words, nothing else. **w2's clause 4 *"at the jump"* holds for `setcontext`**
   (`sigjump_save.c`'s `hero_jmp_set` asks `hero_signal_jump_checked`, as read in § 6).
3. **`wdr`: the watchdog whose handler a raw system call installed** (`wdr.h`, the same with `raw_install.h`, a copy of
   2522's golden header). **Never stopped at the jump**; **guard D at the next use** in 6 of 34 (this Mac base plain),
   3 of 34 (`main` plain), 7 of 34 (`main --sanitize`), 2 of 34 (Linux arm64 base and `main`, plain), 8 of 34 (Linux
   arm64 `main --sanitize`), every other run
   `after the jump: 75`. Guard D's words (`r2/mac/r/out-wdr-main-plain-15.txt`): *"panic: a signal handler used this
   runtime's allocator while the code it interrupted was inside it, which would corrupt memory: a Heroes function given
   to C as a signal handler, or called from one, does so whenever it builds or drops a value"*. **K10.** w2's clause 4
   says the program aborts *"at the jump or where it next uses that code, saying so"*; at the next use it says a
   handler **used** the allocator and names a Heroes function given to C as a handler, of a program that gave C none:
   the very false message panel 258's stop exists to replace (`sigguard.c:180-183`). Clause 4's *"saying so"* is false
   on this branch, on both platforms, both columns. The handler is one `sigguard.c:61-74` says the guard does not see,
   so it is a residue the runtime names, not a new defect; the text has to say it or not promise the words.

## What the synthesis must settle

Most important first. Each names the finding it rests on and the command that settled it or would; none is a verdict
on a candidate.

1. **Whether w2's run-promise is a promise.** Read as one, it is false today on both arm64 platforms and both
   runtimes by `wide` 8 (REENTERED), on Darwin `--sanitize` by `fm_top` (CALLED, clause 5), and by the letter of
   clause 2 on three refused goldens and two `ck` cells; read as scoped by *"where this runtime can tell"*, it
   promises nothing a reader can evaluate and the compiler-engineer's veto does not reach it, but then `h1`, `h7`,
   `h10`, `h11` are not *promised* and D1 and D2 must grade them as a *may* (K7, K9, § 4). Settle one reading, write
   it into the registration's classification of every arm, and if it is the promise, wait for or withhold. Commands:
   `WIDE_N=8 ./bin-wide`, `fm_top --sanitize` on Darwin, `CK_ON=0 CK_TO=0 CK_HOW=0|1 ./bin-ck`, and `heroes-next run
   tests/harness/main.hero -- heroes-next run fixedbugs-2707-a-setcontext` (2 refused) at the landing's commit, both
   arm64 platforms (`262-critic-scratch/r2/run.sh`).
2. **File the defects the sitting found before any text lands on them**: `fm_top` (`d-lane-sig-family-3`), `wide`
   (`-4`), `so` (`-5`, § 9's *"running out of stack aborts"* false on a C coroutine's stack, every text silent),
   `gtc`'s Darwin `--sanitize` SEGV (`-6`), the two drafts still unfiled since 14:11 and 15:46 (`-1` route A, `-2`),
   **and my `ck` (K8)**: a handler on a coroutine's stack left by a followed `siglongjmp` or `setcontext` to its
   runner, refused in STANDING's words that name three causes the program does not have, both platforms, both
   runtimes, both builds. Command: `grep '^github:' .claude/worktrees/rebel-scratch/coord/issues/d-lane-sig-family-*`
   (six `none`, read at about 23:30), then `issues.hero draft` and `file`, the coordinator's.
3. **w2's clause 4, *"saying so"*** (K10): a watchdog whose handler the runtime never saw begin (a raw system call)
   is stopped at its next allocation in guard D's words, which blame a Heroes function given to C as a handler; 2 to
   8 runs of 34 on both platforms. Say the words are guard D's there, or do not promise them. Command: `N=34 sh
   262-critic-scratch/r2/rate.sh <stage 2> <out> main <rt> plain .../critic/wdr.hero`.
4. **The ground at the landing** (K4): 199f (2852) is in `main` since 21:38 and the briefs still call 2852 open and
   `mj_carved` refused; every seat's second column was that runtime. Command: `git log --oneline 87003cb42..main --
   runtime spec` (four commits at 23:15), and the key and the decisive rows again under the landing's runtime.
5. **The arm `now` and its hash** (K5): `blind/arms.sha256` names the base's spec (`3df14254...`); the registration's
   own rule makes `now` the landed spec (`9ff7d2a1...2fc3`), which the warden's predictions read. Write it, and every
   arm's, before any session. Command: `git show main:spec/heroes-spec.md | shasum -a 256`.
6. **The labels' rule, before any session** (§ 1, K1, K6): two readers agree on W (kappa 0.90), O (0.85), I (1.00);
   **not on H (0.50) or S (0.45)**, from two open points in the rule (a gloss in brackets; a § 9 doubt in a shared
   list); and **N, L, T, P, R have no instance in 259's archive**, so the labels this run exists to read are unmeasured
   for agreement until two readers label the run's own answers apart. Command: `python3 -I
   blind/labels-259/agree.py graderA.tsv 262-critic-scratch/labels/graderC.tsv`, then the same on the run's labels.
   And L's second clause labels a correct `RUNS` wrong against a defect's key (K2): bound it.
7. **The thread's abort**: the ffi-pragmatist objects to every text that leaves it unsaid (libuv's pool,
   `dispatch_async_f`, three POSIX notifications, 30 of 30); w2 alone says it, and it is true on both arm64
   platforms by every run and on Windows by the code alone (`thread.c` has no `#if`; unrun on the box). If the
   synthesis adopts a text without w2's other clauses, this clause is separable. Command: `n4` and `pt timer` on the
   box (`ssh win`), the coordinator's word.
8. **The historian's two unquoted precedents** (K3): line 505 quotes Go's `os/signal` in words § 5 calls the fetch
   tool's summary, and line 510's *"Go's Linux-only `SIGCANCEL` case"* has no source. Re-read the page before a
   `## Prior art` line rests on either. Command: the page, `https://pkg.go.dev/os/signal`, read and quoted.
9. **What the landing still owes, unrun by every seat**: the real price (`heroes measure --refresh`, one paid
   request; w2's +182 and +182 vendored reproduce here), the net's own tests whole with the text and its instrument
   applied (the warden stopped them at 6 of 913), a golden of panel 258's stop if a clause names it (0 today), the
   site's build and its publication. Commands: `./heroes-next test tests/harness/main.hero` over the applied tree;
   `npm run build` in `site/`.
10. **Systems no one ran**: an Intel Mac (route A by construction, `rebind_decode.c:314-315`), glibc's other
    architectures, musl, FreeBSD, Windows, Linux x86-64 (the emulated image, not built at this load). Every text that
    names an end or a platform (w2, w4 to w7) is unrun there; the forced route A on both arm64 platforms is the
    nearest evidence and an inference for the rest.

Finished 23:38. No container is left (`docker ps -a | grep p262-critic`: none). The programs of § 5 are in this
copy's `programs/critic/` and copied beside the report in the sitting's folder.
