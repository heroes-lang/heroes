# Panel 262: a signal handler of C's ends, for its thread's callbacks, at its return or a followed jump back into the call its signal interrupted; a callback on that thread before then may abort; and the sentence lands once its falsifiers run

**Resolution adopted: provisional — author ratification pending.** Full panel: the compiler-engineer, the
ffi-pragmatist, the spec-warden, the historian, the completeness critic twice (on the briefs, `critic-briefs.md`, then
on the reports, `critic-reports.md`); the llm-ergonomist registered, its paid run authorised by the author on
2026-11-26 at about 21:45 with a ceiling of 25.00 USD (*ok a 25 dollari*, recorded at the head of `llm-ergonomist.md`,
`blind/registration.md` and `00-shared.md`), to be started by the coordinator after this synthesis. Convened on **the
author's instruction of 2026-11-26 at about 14:06**, the answer *3b* to the coordinator's recommendation, meant as:
*ratify panel 259, and hold a sitting on spec § 13's handler sentence before the milestone's tag, not after it* (the
coordinator had recommended after; `00-shared.md`, its fourth paragraph). Briefs: `docs/panel/262-briefs/` (written at
`aded91388`, repaired from the critic's first reading and moved to the base `87003cb42`); reports:
`docs/panel/262-reports/`, the seats' programs under `programs/`, the synthesis's own files under `synthesis/`.

**The ground this text is written for is `main` at `77eb38534`** (`git rev-parse main`, 2026-11-26 23:44 and again
2026-11-27 02:00: the same; 23:22:43, the merge of lane-kvd), not the base. `git log --oneline 87003cb42..main --
runtime spec` names four commits: `0df8f1a36` (step 203f, panel 261's spec text), `01706218e` (step 199e, defect
2853), `9f9a4f47a` (a merge into lane-sig-family) and **`8d8263f15` (step 199f, defect 2852, 21:38:42)**; `git log
c2b7574bf..77eb38534 -- runtime spec seed` and `bdef9ba06..77eb38534` print nothing, so **the second column every
builder and the critic ran (`c2b7574bf`, `0ce5b0cf3`, `deef27472`, `bdef9ba06`) is `main`'s runtime today**. The seed
did not move since the base (`git diff --stat 87003cb42 main -- seed`: empty). **The synthesis's runs** are this
Mac's, in `.claude/worktrees/panel262-scratch/synth/` (`TMPDIR` inside it): `git archive 77eb38534` of `seed`,
`runtime`, `selfhost`, `tests/harness`, `tests/golden/run`, `.github`, `spec`, `vendor`, `ROADMAP.md`, `CLAUDE.md`
and `docs/roadmap`, and **the seed's compiler built from that commit's seed**, `clang -I runtime seed/heroes.c
runtime/runtime.c -o heroes`, `heroes 0.6.0`, **`sources 1e1eb23a1927d35b`** (`build-seed.log`; `real 25.04 user
20.46`, at a load of 42 to 35, so not a duration). Every program it built links `77eb38534`'s runtime. The falsifiers
ran 2026-11-26 23:52 to 23:53 (load about 35 to 120, `uptime`); the arms, counts, instrument and drafts 2026-11-27
from 02:00, after a reboot of the machine (load 3 to 8). **No duration below is a measurement**, none being asked.
Outputs, text only, copied from the scratch into **`docs/panel/262-reports/synthesis/runs/`**: the runner `run.sh`
and `arms.py`, `run.log`, the refusals' first outputs (`out-*.txt`, `class-wdr-plain.txt`), `measure.txt`,
`instrument-adopted.txt`, `suite-spec-adopted.txt`, `key-main.txt`, `build-seed.log` and the three `draft-*.txt`; the
arms beside them in `synthesis/`.

## The proposal, as put to the seats

`00-shared.md` § 1, in three items. Spec § 13's sentence, *"A callback called after C left a handler of C's by a jump
no function of the C library makes (written in assembly, say), by a `swapcontext` away from it, by a jump or a
`setcontext` onto a stack once given to `makecontext`, or by a jump that lands within it, may abort, saying the handler
was not seen to end, or that C called it from inside one"*, is read at least four ways: as covering **every** switch C
makes (panel 259's blind `now`, W), as **aborts** for each exit it names (panel 257's, N), as an **exhaustive** list
promising every other exit (defect 2848's body, L), and as reaching a callback **on another thread** (the critic's C24,
T); and two more, the pronoun (I) and the remedy's own handlers. Decide what it says, so a reader of the spec alone
predicts, for a callback and a C library that switches stacks or jumps, when the program is promised to keep running,
when it may abort, and with which words: (a) split by kind, (b) the property the runtime tests, (c) the sentence plus
one clause, (d1) to (d9) the routes nobody listed, each built and measured.

## What the sitting found before judging, and beside it

- **Panel 257's first reason for *may* is gone; *may* stays for others.** Panel 257 chose *may* because an inline
  assembly exit ran or aborted on Linux by depth. At the base, in the Linux arm64 image, `p4s`, `p4d` at PAD 0 to
  65,536 and `p4x` all abort in STANDING's words, 3 of 3, plain and `--sanitize`, as on this Mac (the
  compiler-engineer, § 3). But **no exit of the five aborts on every program**: `p4j` (an assembly exit cleared by a
  later followed jump above it) runs, 6 of 6 on each column, both platforms (§ 14), and each exit's *runs* column holds
  correct programs (its § 4 table). What now splits an exit is the route, the stack the handler ran on, whether a
  coroutine is live, whether a later jump is followed, and the thread; a reader sees `SA_ONSTACK` and the thread, never
  the route.
- **The class *"a program with no signal handler told a handler's words"* has a fourth member, open on `main`.** The
  compiler-engineer's `fm_top` (`mj_carved` with no `makecontext`, each fibre started by the program's own assembly
  with `x29` at its stack's top) is refused **CALLED** under `--sanitize` on Darwin, base and `main`; plain and on
  Linux arm64 it runs. **Re-run by the synthesis on `77eb38534`**: `fmtop-san: CALLED, exit 134, 1 distinct of 3`
  (*"panic: fmtop.run was called from inside a signal handler: C called it while handling a signal"*), and `fm_zero`
  (`x29` 0) runs. The seat's reading of the cause (two dead spilled copies of the learned delivery address above the
  fibre's top, read by the walk as a frame record) is an inference from two words and the code, not a trace. The class
  so far: 2831 (closed), 2852 (repaired by step 199f, open until the gate), 2855 (repaired by 199d, open until the
  gate), and `fm_top`: four mechanisms in one day.
- **A full record of the frames of handlers left by a jump refuses a correct resume.** `wide` (nine handlers left by
  followed `siglongjmp`s, one on a static alternate stack, then a coroutine resumed at a save made before any handler
  ran): `WIDE_N=7` runs, `WIDE_N=8` **REENTERED**, false, both platforms, both builds, both runtimes (the
  compiler-engineer, §§ 3 and 14; the critic, § 5). **Re-run by the synthesis on `77eb38534`**: `wide7-plain: RUNS`,
  `wide8-plain: REENTERED, exit 134`, 3 of 3 each.
- **The tree's own goldens refuse, by design, three exits a reader would call *"a `setcontext` back to a call the
  signal interrupted"*** (the critic's K9): `fixedbugs-2702-a-handler-on-the-alternate-stack-left-by-setcontext-to-its-coroutine-is-refused`
  and `fixedbugs-2707-a-setcontext-out-of-a-handler-on-an-alternate-stack-into-a-coroutine-mapped-{after,before}-it-is-refused`,
  under rule 6 (*"A `setcontext` whose target lies on a coroutine's stack holds every standing entry until its handler
  ends"*). **Re-run by the synthesis on `77eb38534`**, each program built and run directly (not through the harness):
  SWITCHED, exit 134, 3 of 3 each, the class its `.expected` holds; `fixedbugs-2702-a-handler-left-by-setcontext-to-its-coroutine-runs`
  runs, as its `.expected`. And on route A the two `setcontext` exits that run on route C, `b_0` and `cs_0`, are
  UNFOLLOWED (the compiler-engineer, forced on both platforms and simulated on Linux arm64).
- **A followed exit to the runner is told three causes the program does not have** (the critic's `ck`, K8): a handler
  on a coroutine's stack left by `siglongjmp` or a `setcontext` to the runner's save, then the runner calls back:
  STANDING, *"either C called it from inside that handler, or C left the handler in a way this runtime does not follow
  (an inline `__builtin_longjmp`, a C++ exception, a jump inside a system library)"*. **Re-run by the synthesis on
  `77eb38534`**: both cells STANDING, exit 134, 3 of 3; back into the coroutine runs (`70`); from an alternate stack to
  the runner runs; from an alternate stack by `setcontext` back into the coroutine SWITCHED (2702's golden's shape).
- **Guard D's words reach a program that gives C no handler of its own** (the critic's `wdr`, K10): a watchdog whose
  handler a raw system call installed is never stopped at the jump, and where the signal landed in the allocator it is
  stopped at the next allocation saying *"a Heroes function given to C as a signal handler"*. **Re-run by the synthesis
  on `77eb38534`**, 34 runs, `WD_US` over the compiler-engineer's 17 delays: **7 GUARD-D, 27 RUNS**
  (`out/class-wdr-plain.txt`). These are the words defect 2755 was filed for (*"ends the program with a message accusing
  a Heroes signal handler that does not exist"*), which panel 258's stop replaced only where the runtime saw the
  handler begin (2755's closing, `gh issue view 2755`, names *"a jump the hooks never see"* as what stays).
- **The watchdog the runtime does see is stopped at its jump, at about one run in five** (the compiler-engineer, § 6):
  28 of 136 runs on this Mac and 25 of 136 on Linux arm64 in panel 258's words, the rest `after the jump: 75`, spread
  over 13 and 14 of 17 delays: where the signal lands, which no reader sees. Its `setcontext` twin (the critic's `wdc`)
  stops at the exit the same way.
- **§ 9's *"running out of stack aborts"* is false on a C coroutine's stack** (the critic's C28, the compiler-engineer's
  `so`): a callback recursing past a 256 KiB `makecontext` stack from `malloc` or `mmap` with a guard page dies by the
  fault with nothing said. **Re-run by the synthesis on `77eb38534`**, plain: `thread` *"panic: stack exhausted in
  so.deep"*, exit 134; `malloc` and `mmap` **exit 138** (`Bus error`), 3 of 3 each. Every text is silent on it.
- **The real libraries on the signal stack trick run; three hand variants poison a thread for good** (the
  ffi-pragmatist, § 4, from the historian's sources: Engelschall's paper, GNU Pth 2.0.1's `pth_mctx.c`, libcoro's
  `coro.c`, QEMU's `coroutine-sigaltstack.c`, each read and quoted): the handler **returns** and the library **restores**
  the alternate stack (RR) in all four, and RR runs on both platforms, both runtimes, both deliveries; RL, JR and JL
  (a jump back into a returned handler's frames, or a handler left by a jump) refuse every later callback on the
  thread in REENTERED's words, on any stack.
- **The thread's abort is the one a binding author meets most with no signal in sight, and no sentence states it**:
  libuv's work callback, `dispatch_async_f`, `timer_create`, `mq_notify` and `getaddrinfo_a` with `SIGEV_THREAD`, 30
  of 30 in the thread's words (the ffi-pragmatist, §§ 2.1 to 2.3); `thread.c` has no platform branch (read), Windows
  unrun.
- **Two more refusals the sentence's list cannot place**: `uw` (a handler left by libunwind's `unw_resume`, in
  libSystem on Darwin, so *"a jump no function of the C library makes"* does not name it) is STANDING (the
  compiler-engineer); and `gtc` (a `SIGALRM` scheduler running only C, then a callback) segfaults inside the runtime's
  `hero_rebind_swapcontext` about once in 160 runs under Darwin's `--sanitize` (the ffi-pragmatist, § 2.2), a fault of
  the runtime's, not a reading.
- **The blind key holds on `main`'s tip, this Mac**: `sh blind/key.sh` with the synthesis's compiler over `h1` to
  `h13` and `n1` to `n4` (`key-main.txt`, 02:10 to 02:11): 34 cells, each one distinct output of three, every verdict
  the repair's at the base (`h1`, `h3`, `h7`, `h9`, `h10`, `h11`, `h13`, `n1`, `n2`, `n3` run; `h2`, `h4`, `h6`,
  `h12` STANDING; `h5` SWITCHED; `h8` ONSTACK; `n4` the thread's words), `n1 --sanitize` running (the critic's C18).
  Linux arm64 on `main` is the warden's (32 cells, `deef27472`, the same runtime); the run's day still owes both.
- **The briefs called 2852 open after it landed** (the critic's K4): step 199f is on `main` since 21:38; `mj_carved
  --sanitize` runs there (the spec-warden, § 2; the ffi-pragmatist's `sw` and `lj` carved, 30 of 30, § 2.2). Every
  builder found it and ran `main` as its second column.
- **The historian's line on Go is the page's own** (the critic's K3, first half, answered): re-read by the synthesis
  (`https://pkg.go.dev/os/signal`, 2026-11-27, WebFetch): the page says that where the non-Go code changes no signal
  handlers or masks, the behaviour is that of a pure Go program, so the phrase the historian quoted at its line 505
  is on the page. **The second half stands corrected too**: the same page names `SIGCANCEL` and `SIGSETXID`, used only
  on Linux by glibc, whose existing handlers the Go runtime keeps with `SA_ONSTACK` turned on (paraphrased, not
  quoted). Neither precedent is one the resolution rests on but the first.

## Verdicts

| judge | verdict | section | cost named | prediction | condition |
|---|---|---|---|---|---|
| compiler-engineer | **veto**, on soundness, of every text that promises (i) a run after `longjmp`, `siglongjmp`, `_longjmp` or `setcontext` out of a handler onto the thread's own stack, (ii) that a program whose C installs no handler is never told these words, (iii) *aborts* for any of the five exits; **object** to (d8), (d9), (d4); **approve** (c) with a clause that promises nothing positive, keeping *may* | design.md §1.12, §4.17, §4.19 | a text moves no runtime line; a new opening moves `SENTENCE_OPENS`; new claim words move `CLAIMS`, six arms and 91 `run` goldens; (d3) unknown; (d8) three sites on each of two platforms and at least 6 correct programs refused | at the landing's commit `wd.hero` stops at the jump in at least 3 of 34 runs of each build, and `fm_top --sanitize` is CALLED 3 of 3 unless a repair of the walk lands first | a text whose every run-promise is scoped to what holds on every route, to a signal that interrupted C and not the runtime, and to fewer than nine handlers left by jumps, run to a run on both platforms; **or repairs of `fm_top`, `wide` and `so` landed and re-run, after which (a) and (d3) may be judged again** |
| ffi-pragmatist | **object** to `now`; **veto** (d8) and (d3)'s stricter half (any *aborts* for a `swapcontext` away, or for a jump into a returned handler's frames); **approve, conditioned**, (a), (c), (d7), (d3)'s first half; **object** to (b) unless it says a jump back into a handler's frames holds it running again, and to (d1), (d2), (d4), (d5), (d9); no warden text read | design.md §1.12 (*any C library must be bindable*), §1.11, §4.19 | none at the ABI; (d4) a `hero_os.h` entry | at the landing, `bx` and `aj` carved and heap under Darwin `--sanitize` run 3 of 3, and libuv's work callback and `timer_create`'s `SIGEV_THREAD` callback abort in the thread's words on every CI leg that builds them | landed with or after 199f; *on its thread*; the re-entry kept in some words; *no handler* the program's C and its libraries, never the process; **the thread's abort said as an *aborts*** |
| spec-warden | **approve `spec-w2` (b), provisional**; **veto `spec-w7` (d2) and `spec-w9` (d8)** on soundness; object to `spec-w8` and `spec-w6`; w1, w3, w4, w5 sound and weaker | design.md §1.6, §1.2; CLAUDE.md § 2 and § Precedence as amended 2026-11-22; the veto §1.12 and CLAUDE.md § 12 | w2 **+182 and +182** vendored on `main` (a lower bound); real unmeasured, about +240 by the base's ratio, an inference | under `adopted` = w2: D1 at least 4 of 5 `RUNS` on each of `h1`, `h3`, `h13`, `n1` to `n3`; D2 0 or 1 `ABORTS` each; D5 `h12` 0 or 1 `RUNS`; real delta +210 to +270 | w2's clause (v) waits for a no-handler refusal on a graded platform; w1 if it passes D2 while w2 fails D1 on `h1`; (iv)'s *"at the jump"* goes if a watchdog runs past it |
| historian (advisory) | **approve** a text of (b)'s shape, bounded by a property, with (c)'s *"on its thread"*, its permitted outcome naming its two outcomes or said as the runtime's permission; **against** re-offering x1 keyed to mechanisms | precedent | none | none | a document that promised a run after a handler left by `longjmp` or a switch, keyed to mechanisms, and kept it a decade; a standard whose lowercase *may* readers got right |

The completeness critic gives no verdict. Its second reading graded panel 259's 48 wrong answers as the second reader
the repair could not give, held every warden text to every program a seat ran (its § 4 table), re-ran the two decisive
falsifiers on both platforms and both runtimes, and built three shapes of its own (`ck`, `wdc`, `wdr`).

## The resolution adopted (provisional)

**R1, the sentence** (`synthesis/spec-adopted.txt`, sha256 `20ae64da...6e6e`), in the place of the handler sentence,
before the remedy, on the same line:

> A signal handler of C's ends, for the callbacks of its thread, when it returns, or when C jumps out of it by
> `longjmp`, `siglongjmp` or `_longjmp` back into a call the signal interrupted, unless this runtime cannot tell that
> the jump left it (on a system whose C library it cannot read, say), and C jumping back into its frames starts it
> again; a callback called on that thread before it ends, from inside it or after C left it any other way (by a jump no
> function of the C library makes, a `setcontext`, a `swapcontext`, or a jump into a coroutine the signal did not
> interrupt, say), may abort, saying the handler was not seen to end, or that C called it from inside one. Where C
> jumps out of a signal handler whose signal interrupted this runtime's allocator, or its record of what is lent to C,
> the program aborts: at the jump, saying so, where this runtime saw the handler begin and the jump, and else where the
> program next uses that code; on a thread where no signal handler of C's has run (counting a signal with
> `hero_signal_watch` runs none), no coroutine switch or jump of C's makes a callback abort in these words; and a
> callback C calls on a thread of its own aborts, saying it ran on a thread this program did not start.

**It is the spec-warden's `spec-w2` with five changes, each a finding of the reports**:

1. **`setcontext` leaves the end-clause for the *may* list** (the critic's K9). w2 ends a handler at *"a `setcontext`
   to a context `getcontext` took, back to a call the signal interrupted"*; the tree refuses three such programs by
   design (rule 6, the goldens above, re-run on `main`), route A refuses `b_0` and `cs_0`, and w2 kept them true only
   through *"where this runtime can tell"*. With `setcontext` in the list, every `setcontext` exit is a *may*, which
   each of those runs and each refusal satisfies, and `h11` and `cs_0`, which run on route C, are D2's to watch (a
   *may* read as *aborts*).
2. **The jump-back undoes an end** (the critic's § 6 item 1; the ffi-pragmatist's condition on (b)): *"and C jumping
   back into its frames starts it again"*. Under w2 a reader of RL (a handler that returned, then a jump back into its
   frames) predicts every later callback runs, and every one is refused in REENTERED's words. The real libraries are
   RR and run; the clause makes the hand variants' refusal a *may* too.
3. **The runtime's limit is a stated exception with an example** (the critic's K7). w2's *"where this runtime can
   tell that the jump left it"* is kept as *"unless this runtime cannot tell that the jump left it (on a system whose
   C library it cannot read, say)"*: the spec's own precedent for a promise with a stated exception is line 497's
   *"unless C has since handed its address out again or a million other lives have ended"*. It holds route A (glibc's
   other architectures, a moved glibc, Darwin x86-64, a forced build: HELD's and UNFOLLOWED's own words say *"whether
   the jump left the handler is not known"*) and a jump C reaches past the C library's names (`sigjump.c:68-81`'s
   residues). The registration classes it (R3 below): a promise, with an exception no graded platform's key meets.
4. **Panel 258's stop says its words only where they are true** (the critic's K10). w2's *"at the jump or where it
   next uses that code, saying so"* promised words guard D does not say. The adopted clause promises the abort in both
   places and the words only at the jump, *"where this runtime saw the handler begin and the jump"*: the stop is
   asked only there (`sigguard.c:198-212`, by the hooks, *"only if (standing && !hero_sig_standing(own))"*, the critic's
   § 6 item 4), and guard D, which exists on Windows too (`alloc.c`'s comment: *"for a handler's C whose frames keep no
   record, ... and Windows"*; the file's only `#if !defined(_WIN32)` near it, lines 661 to 663, holds
   `hero_alloc_interrupted` alone, read), stops the rest at the next use. Its words there are the defect `d-p262-wdr` (§ Found beside).
5. ***"back into"* for *"back to"*, and *"before it ends"* for *"before then"***: the call the signal interrupted is
   the one whose frames the jump lands in, so a jump from a handler on a coroutine's stack to the runner's save (`ck`'s
   refused cells) is no end, and its refusal is a *may*; *"before it ends"* reads with the re-entry of change 2.

**What R1 does to each reading** (the classification in R3 is the same table, by program): **W**, the *may* is now the
property of a signal handler of C's on the callback's thread, and the no-handler clause names the class 259's readers
widened it to; **N**, *may* stays, and every exit the text puts in it is one a program ran past (each exit's *runs*
column); **L**, *"any other way ... say"* makes the list examples, and `h12`, `ck`, `uw` fall in it by the property,
not by a name; **T**, *"for the callbacks of its thread"*, *"on that thread"*; **I**, no pronoun has *a stack* before
it; **H**, *signal handler* throughout, *"of C's"* the program's C and its libraries, `hero_signal_watch`'s handler
named as none; **the thread**, an *aborts* in the thread's own words; **panel 258's stop**, an *aborts* in the place
it happens.

**Its truth, held to every program a seat or the critic ran** (the run's ground is each seat's; the synthesis's own
re-runs on `77eb38534` are marked *s*):

| program | runs on | R1 says | true? |
|---|---|---|---|
| `h1`, 2522's `siglongjmp` onto the own stack, `p4j`, `wd` where the signal lands in C | both platforms, both routes (2522: route A too, the compiler-engineer) | ended: no abort in these words | true |
| `h3` (returns) | both | ended | true |
| `h7`, 2706's two | route C runs; route A HELD and ONSTACK | ended, unless the runtime cannot tell | true (route A: the exception) |
| `h10`, 2707's two `siglongjmp` | both routes run | ended | true |
| `h11`, `cs_0`, `b_0`; 2702's and 2707's three `setcontext` refused; `ck` alternate-stack `setcontext` into the coroutine *s* | some run, some refused, route A UNFOLLOWED | a `setcontext`: may | true |
| `h2`, `p4s`, `p4d`, `p4x`, `same`, `uw`, 2522 inline | STANDING | another way: may | true |
| `h4` | STANDING | from inside it: may | true |
| `h5`, 2702's swapped away; `gt`; `gtc` (runs) | SWITCHED or runs | a `swapcontext`: may | true |
| `h6`; 2745's nested | STANDING or runs | from inside it, or its frames jumped back into: may | true |
| `h8`, `h12`, 2705, the own-stack coroutine | ONSTACK, STANDING | a jump into a coroutine the signal did not interrupt: may | true |
| `ck`, coroutine-stack handler to the runner *s* | STANDING (false words) | not back into the interrupted call: may | true (the words are a defect) |
| signal stack trick RR; RL, JR, JL | RR runs; the rest REENTERED for good | RR ended by its return; RL started again; JR, JL left another way | true |
| `h9`, `other` | runs, both platforms | its thread: no handler of C's ran there | true |
| `h13`, `watched` | runs | `hero_signal_watch` runs none | true |
| `n1`, `n2`, `n3`, `mj_carved`, `escape_carved`, `escape_heap`, `bx`, `aj`, `sw`, `lj`, `cpo`, `bfc` | run on `main`, both platforms, both builds | no handler: no abort in these words | true |
| **`fm_top --sanitize`** *s* | **Darwin: CALLED**, base and `main` | no handler: no abort in these words | **FALSE on `main`** |
| **`wide` with `WIDE_N=8`** *s* | **REENTERED**, both platforms, builds, runtimes | ended nine times: no abort in these words | **FALSE on `main`** |
| `wd`, signal in the allocator | panel 258's stop, both platforms | aborts at the jump, saying so | true |
| `wdr` *s*, signal in the allocator | guard D at the next allocation | aborts where the program next uses that code; no words promised | true (the words are a defect) |
| `n4`, libuv's pool, `dispatch_async_f`, `SIGEV_THREAD` ×3 | the thread's words, 30 of 30 | aborts, saying it ran on a thread this program did not start | true on both arm64 platforms; Windows by the code, unrun |
| `so` | Bus error or SIGSEGV, nothing said | (R1 is silent; § 9's *"aborts"* is the false sentence) | n/a |

**R2, the landing waits for its falsifiers to run, and the sentence lands whole.** R1 is false on `main` today in two
cells, each a runtime defect drafted and unfiled: `fm_top --sanitize` on Darwin (clause *no handler*,
`d-lane-sig-family-3`) and `wide` 8 on both platforms (the end-clause, `d-lane-sig-family-4`). So **R1 lands only at a
commit where** (a) both are repaired and, with `so` (`d-lane-sig-family-5`, the compiler-engineer's condition names
it), re-run by the gate's own command to a run on this Mac and in the Linux arm64 image, plain and `--sanitize`, each
as a `run` golden; (b) every program of R1's table above runs or is refused as its row says, under that commit's
stage 2, on both arm64 platforms (the key, the probes, `ck`, `wdr`, `wd`, the critic's and the seats' programs that
are on disk); (c) `n4` and the ffi-pragmatist's `pt timer` abort in the thread's words on the Windows box, the
coordinator's word for `ssh win`, or the thread clause's Windows half is written as an inference in the landing's
commit and its run owed. **Until then the spec holds the sentence as it stands.** No part lands alone: a sentence with
the promises taken out is the conservative resolution's text (below), which no arm of the blind run reads, and both
blocking repairs are owed before the milestone's tag anyway (§ Bounded discovery: a tag over zero open `blocking`
defects). **If a repair of either is refused as a cost of the runtime's design** (the compiler-engineer prices
`fm_top`'s as unknown: *"a walk that never reads a word no frame record put there ... no bound can give"*), the clause
it falsifies is withdrawn by a decision the author reads, and the sentence lands without it: the no-handler clause
for `fm_top`, the end by a jump for `wide`.

**R3, the classification of every blind program under each arm, written into the registration before any session**
(`blind/registration.md` § What the synthesis added). The critic's K7 settled: **R1's end-clause is a promise, its
exception a stated one**; a reader's `NOT PROMISED` on `h1`, `h7` or `h10` is wrong under `adopted` and labelled P
where it names the exception, so D1 reads whether the exception is weighed as a promise's limit or as a *may*. Under
`rival` (the warden's w1), `h7`, `h10` and `h11` stay inside its *may*, as under `now`.

**R4, the labels' rule, fixed before any session** (the critic's § 1, K1, K2, K6): H wherever the answer names the
thing C left as a callback, a frame of C's or any frame, a gloss in brackets included; S only where the unit's own
answer block offers § 9's stack as a ground for that program's verdict, never from a list the reader applies to every
program alike; L's second clause bounded to a key that aborts by the runtime's design, a defect's key graded O; two
fresh readers label the run's own wrong answers apart, seeing no arm label and no key, **run by the coordinator after
the paid run, not by the synthesis**. The text is in the registration.

**R5, what the landing owes** (the critic's item 9, the warden's item 10, the brief's question 13), for a landing lane:

- the instrument first: `spec_refusals.hero`'s `SENTENCE_OPENS` set to *"A signal handler of C's ends, for the
  callbacks of its thread"* and its test built on the constant (`docs/panel/262-reports/instrument-w2.diff`, which
  applies to `main`'s file unchanged: `patch` in the synthesis's scratch); **run by the synthesis with R1 applied**:
  the instrument's own tests, `14 tests, all passed` (`instrument-adopted.txt`); the `spec` suite in the scratch's
  partial tree, `14 passed, 14 failed`, **`spec/refusal-words` not among the fourteen failures it names** (the suite
  names failures and counts passes, so its pass is read from that absence); `budget`, `spendable` and `real` failed
  as a landing's pins do (*"the spec measures 10710 and the record says 10507, a delta of 203"*, the dash of the
  suite's own words left out), and the other eleven for files and citations the partial tree does not hold
  (`CLAUDE.md`, the ledger, design.md, `editors/`): **the suite on a whole tree is unrun**;
- **the real price with `--refresh`**, one paid request, at the landing: unrun here; **R1 is +203 and +203 vendored**
  (`heroes measure` of the landed spec, 10,318 and 10,507, and of `spec-adopted.md`, 10,521 and 10,710; the text alone
  291 and 291), a lower bound; the four pins and the ledger row (`docs/measurements/010-spec-budget-ledger.md`) from
  that one count;
- **the net's own tests whole** with R1 and the instrument applied (the warden stopped them at 6 of 913; unrun);
- **a `run` golden of panel 258's stop** (0 today, `grep -rlF 'jumped out of the code it interrupted' tests/`): the
  timer cannot be aimed, so the case is a raise from inside the bracket's window if the runtime offers one, or the
  landing says why there is none;
- `run` goldens for `fm_top`, `wide` 8 and `so` with their repairs (R2), and for `n4`'s shape as a library makes it
  (`pt timer`), and the ffi-pragmatist's `bx` and `aj` where the CI builds them;
- **the compiler's test of 2533** (`callback_thunk.hero:400-413`): R1 stands before the remedy, which keeps its bytes;
- **the 91 `run` goldens holding a claim move by no word** (no arm's words change), and the `check` case
  `package-a-file-source-in-no-repository` is no refusal and is not rewritten;
- `sigguard.c`'s module comments that quote the sentence stay true (the warden, item 9); `sigjump.c:78`'s naming of
  `rr_2` as a residue, behind step 199d's rule, is corrected in the same lane (the repair's reading, unfiled);
- **the site's build**, `npm run build` in `site/`: the site serves the spec verbatim and holds its line count to
  *"fewer than 600 lines"* (526, unmoved by R1, `wc -l`); **the push publishes the site**, asked for with its count of
  site commits (CLAUDE.md § Hard stops);
- **no tool that re-prints a program moves**: R1 is a sentence, no form (`.claude/rules/diagnostics-and-goldens.md`'s
  walk is owed only by a new surface form).

**R6, the defects the sitting found are filed before any text lands on them** (§ Found beside): eight drafts, two of
them the synthesis's.

**R7, the record.** Panel 257's R1 sentence is **replaced**, its two claims kept word for word; panel 257's R2 (x1
withheld until two conditions) is **answered**: both conditions held and x1 was false as written (2705, the own-stack
coroutine, panel 258's stop, route A, the thread), and R1's end-clause is x1's exits held to every one of those
falsifiers and narrowed by `setcontext`. The eight arms' words move by none. design.md owes no line (its coroutine
rows refuse coroutines in the language; the guard's rules live in the runtime's module docs; the warden, item 9).
Rebel's `os.ledger:454` quotes the remedy, which R1 leaves byte for byte.

## Not adopted

- **`spec-w2` as written** (+182 vendored, the warden's approval): R1 is it with five changes, each because a program
  breaks w2 or a reader cannot place it: its `setcontext` end (three goldens and `b_0`, `cs_0`), its silence on a jump
  back into a returned handler (RL), its unclassed *"where this runtime can tell"* (K7), its *"saying so"* at the next
  use (`wdr`), and *"back to"*, which does not say `ck`'s jump to the runner is no end. Its two promises are R1's and
  wait as R1 does (R2).
- **`spec-w1`** (+63, the rival, run as the blind run's `rival`): its list keeps the mechanisms, so a reader of an
  unlisted exit (`ck` to the runner, `uw`'s `unw_resume`) reads silence; `h7`, `h10` and `h11` run inside its *may*, the
  narrowing D2 measures; it states neither the thread's abort nor panel 258's stop, both of which the ffi-pragmatist
  and the critic ask said; and its second sentence, R1's no-handler clause, is false on `main` by `fm_top` as R1's is.
- **`spec-w3`** (+9): `now` with *"a signal handler on its thread"* and *"within the handler"*: true today everywhere,
  and the conservative resolution below; it leaves W with no positive clause, L (`h12` silent and aborting) and N.
- **`spec-w4`** (−20) and **`spec-w5`** (+20): the *may* scoped by *"by its return, or by a jump of the C library's back
  to a call the signal interrupted that this runtime can tell left it"* implies the same promise w2 makes, falsified by
  `wide` 8 and RL with no list to tell a reader why (the critic's § 4 table); w5 makes one sentence of 197 tokens with
  two unrelated aborts (the warden).
- **`spec-w6`**, line 437's home (−29): turns a description of what the runtime does into the language's rule, `h2`'s
  and `h5`'s correct C becoming a call the line forbids, and gives *"the same rule"* two antecedents (the warden).
- **`spec-w7`**, a promise per platform: **vetoed by the warden**, false as written (it leaves out panel 258's stop,
  promises route C on every glibc where a moved glibc is route A, and names systems the spec has never named).
- **`spec-w8`**, the words alone: reverses panel 257's R1, which the author asked for, and retires the instrument's spec
  side (`spec/refusal-words` red, the warden's § 5).
- **`spec-w9`**, abort at the exit: **vetoed by the warden and by the ffi-pragmatist**: false today on every platform
  (the abort is the callback's, `h5`, `h8`), and the runtime change it needs refuses `gtc`, 2702's resumed handler,
  `cs_0`, `h11`, 2707's two and `h10`, correct programs that run.
- **x1** (panel 257's R2): superseded by R1 (R7).
- **(d3)'s stricter runtime**, *may* made *aborts*: **vetoed by the ffi-pragmatist**; at least 12 `run` goldens and 5
  programs that run would be refused (the compiler-engineer, § 8).
- **(d3)'s first half as a sentence alone**, *a program whose C installs no signal handler is never told these words*:
  R1 holds it as its no-handler clause, under R2.
- **(d4), a told end**: no program measured needs it, and a `hero_os.h` promise for ever (panel 259's precedent,
  `docs/panel/259-*.md:200-205`).
- **(d9), a told route**: `heroes doctor` prints no route (run by the compiler-engineer and the ffi-pragmatist) and the
  accessor has no public declaration; a tool surface change is a sitting of its own (`.claude/rules/cli-surface.md`).
  It is what would make R1's exception evaluable by a reader on their own machine.

## Disagreements, stated plainly

1. **Whether w2's run-promise is a promise.** The compiler-engineer vetoes every text that promises a run after a jump
   out of a handler and every no-handler promise; the warden approves w2, which makes both. **Settled by the command**:
   the synthesis ran `WIDE_N=8` and `fm_top --sanitize` on `main` at `77eb38534` (REENTERED and CALLED, 3 of 3 each), so
   **the command settles it on the compiler-engineer's side for today**: read as a promise, w2 (and R1) are false on
   `main` in two cells; read as scoped away, w2 promises nothing a reader can use (K7). **R1 is a promise** (its
   exception stated and classed), and **R2 lifts the veto by its own condition**, *"repairs of `fm_top`, `wide` and `so`
   landed and re-run"*: the sentence lands only where every falsifier runs. The warden's approval of w2's shape stands;
   its text does not land as written.
2. **`setcontext`.** x1, x2 and w2 end a handler at a `setcontext` to a context `getcontext` took; R1 puts every
   `setcontext` in the *may*. Robust: three goldens refuse it by design and route A refuses two correct ones; the price
   is that `h11` and `cs_0`, which run on route C, are told *may*, which D2 reads (a *may* is true of a program that
   runs; *aborts* would not be). Conservative here would be w2's wording with *"where this runtime can tell"* bearing
   the three goldens; the synthesis does not take it, since a reader sees `SA_ONSTACK` and the target and no text tells
   them what those decide.
3. **The jump back into a returned handler.** The ffi-pragmatist objects to (b) without it; the warden's w2 names the
   jump only as a way of leaving. R1 says it. No sourced library does it (all four RR); RL, JR and JL are hand variants.
4. **The thread's abort on Windows.** The ffi-pragmatist asks an *aborts*; it is true on both arm64 platforms by every
   run and on Windows by the code alone (`thread.c` has no `#if`, `hero_thread_guard` at every handed function's
   entry). R1 says *aborts* everywhere; **its Windows half is an inference** until R2(c)'s run.
5. **K3, the historian's Go line.** The critic read line 505's quotation as the fetch tool's summary re-presented as
   the page's words; the synthesis re-read the page, and the words are the page's. The critic's caution (re-read
   before citing) was right and is what was done; its doubt about the words is answered. Its second point,
   `SIGCANCEL` unsourced, is answered by the same page.
6. **The labels.** The repair's grader A and the critic (a second reader) agree on W, O and I and not on H (0.50) or
   S (0.45), from two open points the registration left; R4 fixes both, the critic's way for H (any words, a gloss
   included) and the registration's own principle for S (the deciding reason, the unit's own block).
7. **The spec's size.** R1 costs +203 vendored against w2's +182 and w1's +63. Robustness (rank 3) and the author's
   amendment of 2026-11-22 admit it; the warden holds no veto against R1 and none was cast on a cost.

## What a veto would compel

**The compiler-engineer's veto compels R2**: no run-promise lands while `wide` 8 refuses a correct resume, and no
no-handler promise while `fm_top` is CALLED; it lifts by the seat's own second condition, the three repairs landed and
re-run (R2(a)), and its first condition's other parts are R1's words (route A by the stated exception, panel 258's
stop by its own clause, the thread by *"for the callbacks of its thread"*). Its (iii), *aborts* for any of the five
exits, is met: every exit of the old list is in R1's *may*. **The ffi-pragmatist's veto** on (d8) and (d3)'s stricter
half is met by adopting neither; its conditions on (b) are each in R1 (the re-entry, *on its thread*, the thread's
abort as *aborts*, panel 258's stop scoped, the no-handler clause landing after 199f and `fm_top`'s repair). **The
spec-warden's vetoes** fall on w7 and w9, neither adopted. **No veto stands against R1 as R2 lands it.**

## The conservative resolution

**`spec-w3` and the thread's abort, landing now**: the sentence as it stands with *"a signal handler on its thread"*
and *"within the handler"* (+9 vendored, true everywhere at the base and on `main`, the instrument's opening kept), and
*"a callback C calls on a thread of its own aborts, saying it ran on a thread this program did not start"* beside it;
no run promised, no no-handler clause, no word on panel 258's stop. It fixes T, H and I and states the abort binding
authors meet most; it leaves W (259's finding: the *may* read over switches with no handler), L (`h12` silent and
aborting) and N, and it waits for nothing. **Robustness takes R1 under R2 instead**: the reading the sitting was
convened on is W, and only a positive clause addresses it; the defects that make that clause false today are
`blocking` and owed before the tag in any case.

## The critic's list of what must not be asserted, held line by line

From `critic-reports.md` § *What the synthesis must settle*, items 1 to 10, and K1 to K10:

1. **w2's run-promise** (K7, K9, § 4): settled as a promise with a stated exception (R1, R3); held false today by the
   synthesis's own runs of `wide` 8 and `fm_top --sanitize` on `main` and the three `setcontext` goldens (SWITCHED, as
   their `.expected`), the last taken out of the promise by R1's change 1. The harness form over the 2707 goldens (the
   critic's `heroes-next run tests/harness/main.hero -- heroes-next run fixedbugs-2707-a-setcontext`) is **unrun**: the
   synthesis built and ran each program directly and read its class against its `.expected`.
2. **The defects** (K8): eight drafts, R6; whether each still happens on `main`'s tip in § Found beside.
3. **Clause 4's *"saying so"*** (K10): **false**, by `wdr` on `main` (7 of 34, guard D's words); R1 does not promise the
   words there.
4. **The ground at the landing** (K4): the four commits named at the head; 199f in, 2852's `mj_carved` running on
   `main`; the class open on `main` by `fm_top`.
5. **The `now` arm's hash** (K5): repaired, `arms.sha256` now names the landed spec, `9ff7d2a1...2fc3` (`git show
   main:spec/heroes-spec.md | shasum -a 256`, the synthesis, 23:44), and the two arms beside it, their texts, diffs,
   hashes and counts in `blind/registration.md` § What the synthesis added.
6. **The labels' rule** (K1, K2, K6): R4; the readers are the coordinator's, after the paid run.
7. **The thread's abort**: said, as *aborts*; **Windows unrun**, an inference from `thread.c` (R2(c)).
8. **The historian's two precedents** (K3): re-read (2026-11-27); the decision's `## Prior art` cites only the two
   pages the synthesis re-read, Go's `os/signal` and POSIX's `longjmp()`, paraphrased.
9. **What the landing owes**: R5, every line a landing lane's.
10. **Platforms nobody ran**: an Intel Mac (route A by construction, `rebind_decode.c:314-315`), glibc's other
    architectures, musl, FreeBSD, Windows, Linux x86-64 (the image not built at the sitting's load). **R1 promises a run
    after a jump only through its exception there**: on route A, by R1's own words, a jump the runtime cannot follow
    leaves the handler running and a callback *may* abort; the forced and simulated route A on both arm64 platforms (the
    compiler-engineer's 18 cases) is the nearest evidence, an inference for every real such machine. Its no-handler
    clause and its thread clause are **inferred** on every system but Darwin arm64 and Linux arm64 (Windows: no probe
    and no walk, so nothing there is refused in these words, read in `sigguard.c`'s `#else`; the rest, unrun).

And what this synthesis itself does not assert: **a real count** (every count is vendored, a lower bound); **the `spec`
suite on a whole tree** with R1 (run on a partial tree, its relevant check green); **the net's own tests** with R1;
**any run on Linux arm64 by the synthesis** (its Linux cells are the seats' and the critic's, on the same runtime);
**a repair of any defect**; **a blind-run outcome**.

## Found beside, for the coordinator

The eight drafts in `.claude/worktrees/rebel-scratch/coord/issues/`, every one `github: none` at 02:05
(`grep '^github:'`), each `blocking`:

| draft | the defect | found by | on `main` `77eb38534` |
|---|---|---|---|
| `d-lane-sig-family-1` | route A: a handler's `setcontext` to a heap context never trusted; `b_0`, `cs_0` refused in UNFOLLOWED's words, ten more rows in them | lane sig-family (the critic's C8) | **unrun by the synthesis** (route A needs the simulation's container or a forced build); the compiler-engineer ran it forced on both platforms at `c2b7574bf`, `main`'s runtime |
| `d-lane-sig-family-2` | a coroutine yielding into another context than the one switched to judged ended; a handler's jump into it runs the function inside the signal (Linux arm64, plain) | the repair | **unrun** (Linux arm64; no seat re-ran it on `main`) |
| `d-lane-sig-family-3` | `fm_top`: CALLED under Darwin `--sanitize`, no handler | the compiler-engineer | **still happens**, 3 of 3 |
| `d-lane-sig-family-4` | `wide` 8: REENTERED after nine followed exits | the compiler-engineer | **still happens**, 3 of 3 (`WIDE_N=7` runs) |
| `d-lane-sig-family-5` | `so`: a callback past a C coroutine's stack dies with nothing said | the compiler-engineer (the critic's C28) | **still happens**: `malloc` and `mmap` exit 138, 3 of 3 each; the thread's own stack `stack exhausted` |
| `d-lane-sig-family-6` | `gtc`: a SEGV inside `hero_rebind_swapcontext` about once in 160 runs under Darwin `--sanitize` | the ffi-pragmatist | **unrun** (a rate of hundreds of runs, not seconds) |
| **`d-p262-ck`** (the synthesis's) | a handler on a coroutine's stack, left by a followed exit to the runner, told STANDING's three false causes | the critic (K8) | **still happens**, 3 of 3 each cell; `issues.hero draft`: exit 0 |
| **`d-p262-wdr`** (the synthesis's) | a raw-installed watchdog stopped at the next allocation in guard D's words | the critic (K10) | **still happens**, 7 of 34; `issues.hero draft`: exit 0 (its title shortened once, the first form 246 characters, which the program refused) |

**None of the six is wrong** as far as the reports read: each item's measurement is its seat's, each class `blocking`
by § Bounded discovery (a correct program refused, a false message, a fault with nothing said). Two notes: `-3`, `-4`
and `-5` name their programs *"in that seat's tree"*, and the programs are also in the sitting's own folder,
`docs/panel/262-reports/programs/compiler-engineer/`, which the issue will cite once the sitting is committed; `-6`'s
class rests on a fault inside the runtime whatever the C's strictness, as its Origin says. `d-p262-wdr`'s Origin
says the coordinator may read it as defect 2755's residue (2755's closing names a jump the hooks never see; here the
hooks see the jump and never saw the handler begin).

Beside them:

- **`spec-w2.diff` in the reports folder is w2's first wording** (*"stops the program, at the jump or at the next
  allocation"*), while `spec-w2.txt` is its final (`grep -c`: 1 and 0); the warden's priced file is the final: the
  synthesis applied `spec-w2.txt` to the landed spec and got `b1e76202bfc13a7c...`, the warden's `main-w2.md`, at
  10,500 and 10,689 (+182). The diff is the coordinator's to regenerate or annotate before the commit; reports are
  not the synthesis's to edit.
- **The seed's compiler pins the base's spec**: `heroes measure spec/heroes-spec.md` with it on `main`'s spec prints
  *"STALE: the recorded count is for 24de425e6c28e69b and this file is 056888b8540b9126"* (`measure.txt`), the seed
  predating step 203f; stage 2 carries `main`'s pin. Not a defect (the seed is refreshed when the two-stage build goes
  red or a release closes), said so nobody reads it as one.
- **The historian's fetch tool cached two PDFs outside the repository** (its report's head: ISO/IEC Directives Part 2
  and the Oberon report, *"read as page images the fetch tool cached outside the repository"*): the author is owed
  that sentence (CLAUDE.md § Hard stops), as panel 261's synthesis said of the same tool. The synthesis's two
  WebFetches read HTML pages.
- **`sigjump.c:78` still names `rr_2` as a residue**, where step 199d's rule (`sigentries.c:439`) says it was one until
  defect 2855 (the repair's reading): a module comment behind its rule, owed with the landing (R5).
- **`00-shared.md` holds no sentence giving the old hash as the `now` arm** (`grep -n '3df14254'`: only § 3.1's heading,
  which describes the base's spec truly), so the synthesis edited nothing there; `llm-ergonomist.md:29` says *"`now`
  (the spec at `87003cb42` ... sha256 `3df14254...d93f`"* and goes on to the landed spec in the same sentence, and is
  not the synthesis's to edit: the registration governs (its head).

## What is open and unrun

- **The landing itself**, all of R2 and R5: the three repairs and their goldens, the falsifiers at the landing's
  commit on both arm64 platforms, the real count, the net's own tests whole, the `spec` suite on a whole tree, panel
  258's golden, the site's build and its publication.
- **Windows**: the thread's abort and every cell of R1 (no probe, no walk: a reading); `n4` and `pt timer` on the box
  wait for the coordinator's word.
- **Route A on a real machine** (an Intel Mac, glibc's riscv64, loongarch64, 32-bit arm), **musl**, **FreeBSD**, **Linux
  x86-64**: unrun by every seat and by the synthesis.
- **`d-lane-sig-family-1`, `-2` and `-6` on `main`'s tip**: unrun by the synthesis (above).
- **The blind run**: registered, its arms and classification written; its sessions, its scoring and the two readers of
  its labels are the coordinator's.
- **The cause of `fm_top`'s words** (an inference from two words and the code), **of `gtc`'s SEGV** (located by `atos`,
  not traced), and **what `so`'s recursion wrote below a `malloc` stack** before the fault.

## Predictions to score

The seats' (the verdicts' table) at the landing in M-rebel-library: the compiler-engineer's `wd` at least 3 of 34
stopped per build, and `fm_top --sanitize` CALLED until its repair; the ffi-pragmatist's `bx` and `aj` running under
Darwin `--sanitize` and `uvq work` and `pt timer` in the thread's words on every leg that builds them; the warden's,
**read for `rival`** (its diff is `spec-w1.diff` byte for byte: the hunks' lines compared, equal) and **unpredicted for
`adopted`**, whose diff is no warden text's (the registration's own rule); w2's predictions are read against nothing,
w2 being no arm. **The synthesis's own**, registered here before any session:

- under **`adopted`**: D1 holds on `h3`, `h13`, `n1`, `n2` and `n3` (at least 4 of 5 `RUNS` each) and **`h1` is the one
  at risk** (3 or 4 of 5 `RUNS`, the stated exception drawing P); D2 holds (0 or 1 `ABORTS` on each of `h7`, `h9`,
  `h10`, and 0 to 2 on `h11`, the one in a *may*); D3 holds with the words right; D5 holds (0 or 1 `RUNS` on `h12`);
  `n4` `ABORTS` in the thread's words 4 or 5 of 5;
- under **`rival`**: D2 fails on at least one of `h7`, `h10`, `h11` (2 or more `ABORTS`), and D1 holds on `n1` to `n3`
  through its second sentence;
- **R1's real delta** on `--refresh` at the landing: **+250 to +290** (the vendored +203 by the base's real-to-cl100k
  ratio, 13,439 / 10,245 = 1.31, gives +266: an inference, not a price);
- **R2 is met before the milestone's tag**, or a decision withdraws a clause: scored by the landing's commit holding R1
  or by that decision issue.

**What would withdraw R1 for the conservative resolution**: D1 failing on two or more of `h3`, `h13`, `n1`, `n2`, `n3`
under `adopted` (the positive clause does not cure W, so its cost buys nothing), or a repair of `fm_top` or `wide`
refused as a design cost with the author choosing the conservative text over a sentence without that clause.

## The blind reading, run 2026-11-27

Added 2026-11-27, after this sitting's commit (`542b8b732`): the registered reading ran on the author's authorisation
(*ok a 25 dollars*), its report `262-reports/llm-ergonomist-blind.md`, the sessions' answers
`262-reports/llm-ergonomist-blind-answers.tar.xz` (each session's `result.md` and `run.json`, never a key or a spec),
the two readers' labels `262-reports/blind-readers/`, and the runner as adapted, every change a `diff -u` against panel
261's, `262-briefs/blind/registration-runner.md`, written before any session. **The registration was amended before any
session by the coordinator** (its last section): `main` had moved the spec at `750d2540d` (step 208b, one line of § 13's
first paragraph; the handler sentence untouched), so **`now` is `main`'s spec pinned at `9b88d3e84`**
(`736b534c...efd3`) and `adopted` and `rival` are the synthesis's two diffs applied to it (`d66dfd30...7b17`,
`ecfb9931...4d7c`); and step 208a (`b626d4c05`) refuses `extern "hero_os.h" layer`, so the registered `h13.hero` no
longer built, and **it lost the word `layer` and nothing else**, the certain fix the compiler names; `claude --version`
read `2.1.326`, where the registration carried `2.1.323`. A process choice inside the author's authorisation, recorded
for the author to turn. **The key, re-run under `9b88d3e84`'s stage 2 (`sources 7c7c978a4dfeddf3`) on this Mac and in
Linux arm64, did not move**: 102 of 102 Mac outputs equal the registered key, every Linux first run equal to the Mac's,
no program split between the platforms or between the plain and `--sanitize` builds; `h13` runs. **31 sessions, all
counted, 11.66 USD of the 25.00** (11.657696 by the 31 sessions' own JSON; the most expensive the verdict, 1.33), each
`success`, `is_error` false, `completed`, none voided; every one offered `Read` alone, as the registration assumed, so
each report is its `result.md`. The run (03:40 to 04:30) went on while the session that started it was paused by the
account's five-hour limit; nothing was cut or started twice.

**By the registration's own rules the resolution stands as adopted.** `adopted`'s fifteen readers are wrong on **none**
of their 85 answers: **D1 holds** (`h1`, `h3`, `h13`, `n1`, `n2`, `n3`: 5 of 5 `RUNS` each, 0 `NOT PROMISED`), so the
text no longer reads as covering switches it does not name; **D2 holds** (`h7`, `h9`, `h10`, `h11`: 0 `ABORTS` each), so
no *may* is read as *aborts*; **D3 holds** (`h2`, `h5`, `h6`, `h8`: 5 of 5 `NOT PROMISED` each, text-right), the four
read as left open; **D5 holds** (`h12`: 0 `RUNS`, 5 `NOT PROMISED`), so the list no longer reads as closed; **D4**:
`h4` 5 of 5 `NOT PROMISED`, text-right under *"from inside it"*, and `n4` 5 of 5 `ABORTS` in the thread's words, with
nothing printed before. **`now`, reported**, is wrong on 21 of 85: `n2` and `n3` 5 of 5 `NOT PROMISED` each and `n1` 1
(the widening 259 found, at its most), `h12` 5 of 5 `RUNS` (the list read as exhaustive) and `n4` 5 of 5 `RUNS`; its
`h4` 5 of 5 `ABORTS` with the words. **`rival` (`spec-w1`) also holds D1, D2, D3 and D5** (5 of 5 on the D1 set, 0
`ABORTS` on the D2 set, 5 right on each of the D3 set, 0 `RUNS` on `h12`) and is wrong on 6: **`n4` 5 of 5 `RUNS`**
(D4: its text says nothing of a thread C made, the key aborting) and `h4` once (`NOT PROMISED`, where `rival` has no
text-right). Every `ABORTS` of the run, 18, names the key's cause. Neither D1 nor D2 fails for `adopted`, so the rule
that would move the resolution to `rival` does not fire.

**The predictions.** The spec-warden's `spec-w1` column, read for `rival`, predicted D2 failing (60 %, 1 to 2 `ABORTS`
on each of `h7`, `h10`, `h11`); **`rival` passed D2 on all four, 0 `ABORTS` of 20**, the result the warden registered
as falsifying its own recommendation of w2 over w1 (*"w1 passing D2 on all four (then the list's may does not invite
N, and w1, 119 vendored tokens cheaper, is the better text)"*). Its `now` column held D2's, D3's and `h4`'s ranges and
missed `n2`, `n3` (0 `RUNS` each against 1 to 5), `h12` (5 `RUNS` against 1 to 4), `n4` (0 `ABORTS` against 1 to 3)
and most label counts (H 11, L 10, and no N, T, I, R or S); its spend, 14.5 USD (12 to 18), was above the measured
11.66. `adopted` was unpredicted by the warden. **This sitting's own**, registered above: under `adopted` D1, D2, D3,
D5 and `n4` 4 or 5 of 5 `ABORTS` all held, and **`h1` was not at risk** (5 of 5 `RUNS`, no reader citing the stated
exception: no P anywhere); under `rival` D1 on `n1` to `n3` held and **D2 did not fail**, as predicted it would.

**The `context` question**: 31 of 31 answers say no instruction, rule, contract, memory or document outside their folder
reached them; seven name the working directory's path, a path and not a rule; the scan flagged the verdict session
once, every hit a reader's own *"No CLAUDE.md ... reached me"* read back from a report. **The verdict session approves**
the change to `adopted` and says a model predicts these programs better from it than from `rival` (`rival`'s clause
*"onto a stack once given to `makecontext` or into a coroutine the signal did not interrupt"* read two ways, its silence
on `n4`, its split on `h4` and `h6`); it told the reports' versions apart by their quotations, so the labels' blinding
did not hold for it, and the scorer read the key before grading.

**The labels**: two fresh readers, started by the coordinator after the run with packets that named no arm, labelled the
27 wrong units apart and **agree on every one** (the registered `agree.py`: same set 27 of 27; W 11, L 15, H 11 and O 1
for each, kappa 1.00 on each; N, T, I, P, R and S given by neither, so their agreement stays unmeasured). Mapped back to
the arms: **`now` t1 `h12` L 5; `now` t2 `n1`, `n2`, `n3` W and H 11, `n4` L 5; `rival` t1 `h4` O 1** (a prohibition
read as having no promised run-time result, which no label names); **`rival` t2 `n4` L 5; `adopted` none.** So `now`'s
wrong answers are the two misreadings the sitting was convened on, the widening (W with H, on every coroutine program
with no signal handler) and the closed list (L), and `rival`'s are the silence on a thread C made (L) and one `h4`.

**What it means for R1** (provisional, the author's to ratify): the run gives the registered reasons to keep R1 as
adopted and none to move or amend it; `adopted` is the one text whose readers predicted every one of the 17 programs
as the key or the text allows, and the one that names `n4`'s abort (5 of 5 against `rival`'s 0 of 5). Beside the
rules, the run also measured that **`rival` cures the widening and the closed list as well**, at +64 vendored tokens
against R1's +203 (`cl100k_base`, the registration's § What the synthesis added, counted over `77eb38534`'s spec; over
the pinned spec, unrun), and the warden's own falsifier of w2 over w1 is met;
`rival`'s price is `n4` and one `h4`. Whether that turns the choice between the two texts is the author's, with R2's
landing and its falsifiers still owed as before. **Unrun**: the x86-64 legs and route A (no key here says anything of
them), the S reading (no seat has run a callback recursing past a `malloc` stack), and the labels N, T, I, P, R and S,
which no wrong answer of this run carried.

## Author's verdict

**Pending.** The decision's draft is `docs/panel/262-reports/decision-draft.md`, filed with this sitting as
`panel 262`.

**Ratified by the author's reading, 2026-11-27, at about 08:25**, in conversation (the answer *1a* to the
coordinator's recommendation, meant as: *ratify the adopted text*). **What it ratifies**: R1, the spec-warden's
`spec-w2` with the synthesis's five changes (`262-reports/synthesis/spec-adopted.txt`), over the rival `spec-w1` and
the conservative sentence, on the blind reading's result (none of the adopted text's 85 answers wrong; the rival's five
wrong on `n4`); and R2 to R7 as written. **R2's three repairs stand on `main`**: `fm_top` by step 199g (`d783018e7`),
`wide` by step 199h (`a0088cee2`), `so` by step 199i (`94c2266a4`); R1 lands once every program of its table re-runs on
both arm64 platforms, in the landing lane, with what R5 says the landing owes. Decision issue 2901 closes with this
verdict.
