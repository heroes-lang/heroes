# Panel 262, the compiler-engineer's brief

Read `00-shared.md` first, whole (its head: the repair and the move; § 1 the proposal, § 2 the questions, § 3 the
ground, § 4 the candidates, § 5 the shapes beside), then the critic's first reading
(`docs/panel/262-reports/critic-briefs.md`, C1 to C28), on which these briefs were repaired before any seat starts.
**Your agent file is the base's** (`.claude/agents/compiler-engineer.md` of your copy); design.md is reached by `grep
-n`, never from memory (it holds none of the guard's rules, and its rows on coroutines, `00-shared.md` § 3.6). Under the
author's amendment of 2026-11-22 the most robust route wins at any cost in size; **your veto on soundness stands**
(design.md §1.12: *"must not segfault and must not corrupt memory"*; panel 251's veto: no Heroes code inside a signal),
and what you count is cost to be written, never a reason by itself.

**Your copy**: `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/262-compiler-engineer`, a detached worktree at
the base **`87003cb42`** (the coordinator makes it). Build there: `clang -I runtime seed/heroes.c runtime/runtime.c -o
heroes`, then `./heroes build selfhost/main.hero -o heroes-next`; `./heroes-next --version` should read **`sources
6ce3f9e116d22e3a`**, the repair's stage 2 at the base (this Mac and the Linux arm64 image alike, the repair's); say if
it does not. A runtime you edit is a copy of `runtime/` inside your worktree, built with `HEROES_RUNTIME=<that copy>`;
never the trunk's `./heroes`, never another seat's, never the lane sig-family's. **`main`'s runtime as a second column**
where it moved after the base (`00-shared.md` § 6). **No paid run, no credential printed, `gh` reads only, nothing
outside the repository's root (`TMPDIR` inside your copy), no `pkill`, no commit; one container at a time, `--rm`, named
`p262-compiler-engineer`, no `docker pull`; the Linux arm64 image has no `/usr/bin/time`.** Write your report as you go
to `docs/panel/262-reports/compiler-engineer.md` in your copy, your programs under
`docs/panel/262-reports/programs/compiler-engineer/`.

## The map (line counts by `wc -l` at the base `87003cb42`, the repair; bold where the repair's two moves changed the clerk's; every pointer re-read before you cite it)

| what | where | lines |
|---|---|---|
| the guard: the four questions, `hero_callback_entered` (**659**), `hero_callback_asked` (**664**), the eight arms (`hero_signal_refuse`, **598 to 644**), `HERO_SIGNAL_REMEDY` (110 to 115), the walk (`HERO_SIGNAL_WALK`, 229), **panel 258's jump refusal** (`hero_signal_jump_checked`, 175 to 212), the per-thread entries (`_Thread_local HeroSignalStack`, **453**), the spans of a handler a jump left (step 199d, after 356), Windows (**714** on) | `runtime/parts/sigguard.c` (every line from 446 on **+22** since the clerk's base, step 199d) | **732** |
| a thread C made, `hero_thread_refuse` (**132 to 143**; 128 to 139 at the clerk's) | `runtime/parts/thread.c` | **148** |
| the handlers a thread is inside, rules 4 to 7 (**27 to 65**; rule 6, **39 to 47**, re-entry inside a span since step 199d), the life of a range (**104 to 131**), a coroutine the C library starts through `uc_link` (**132 to 147**, step 199b), route A's held ranges (**149 to 155**), the frames of a handler left by a jump, kept as spans (**432 to 445**, step 199d, defect 2855), `hero_sig_inside` (**657**) | `runtime/parts/sigentries.c` | **673** |
| the wrappers around every handler a program's C installs (Linux) | `runtime/parts/siginstall.c` | **332** |
| the C library's own save points and jumps, routes C and A (27 to 58; route A 45 to 53), what it does not reach (68 to 81), the simulation's switches (`HERO_JMP_NO_DECODE` 130 to 132, `HERO_JMP_MAKE_IN_C` 134 to 145), `hero_ctx_*` (94 to 205) | `runtime/parts/sigjump.c` | 207 |
| the key, verified; the stack pointer a jump installs | `runtime/parts/sigjump_decode.c` | 206 |
| route A's record; the context's word read wherever it is mapped (step 199c) | `runtime/parts/sigjump_record.c` | **321** |
| the save points and the jumps; `hero_jmp_set`; where the thread is seen running (step 199d) | `runtime/parts/sigjump_save.c` | **305** |
| `makecontext`'s wrapper, on every Linux architecture since step 199a | `runtime/parts/sigjump_make.c` | 129 |
| the calibration's assembly | `runtime/parts/sigjump_probe.c` | 163 |
| the C library's own functions found before `main` | `runtime/parts/siglookup.c` | 341 |
| the door that counts signals (`hero_signal_watch`, `hero_signal_take`) | `runtime/parts/sigdoor.c` | 361 |
| Darwin: the rewritten pointers, the trampoline, its rules (`hero_rebind_inside`, `rebind_entries.c:`**`630`**), its jumps (`hero_rebind_setcontext`, defect 2853), **Darwin x86-64 on route A by construction** (`rebind_decode.c:314-315`) | `runtime/parts/rebind.c`, `rebind_entries.c`, `rebind_jump.c`, `rebind_record.c`, `rebind_decode.c`, `rebind_image.c`, `rebind_install.c` | **145**, **647**, **360**, **281**, 320, **298**, 116 |
| the count of callbacks C called that have not returned (panel 259), every thread's since step 200a | `runtime/parts/callback.c` | **167** |
| **guard D**, a handler using the allocator while the code it interrupted was inside it (`hero_alloc_reentered`, 670 to 678) | `runtime/parts/alloc.c` | 1813 |
| **the runtime's own SIGSEGV and SIGBUS handler**, `SA_ONSTACK`, in every program (`hero_stack_guard_install`, 1269 to 1282; the alternate stack per thread, 1235; the fault off the thread's stack, `hero_stack_foreign`, 856 to 866; yielding to ASan, 112 to 117) | `runtime/parts/stack.c` (called from `os.c:703`) | 1438 |
| the thunk, the one door from C into Heroes code; the test of defect 2533 holding line 428's tail (400 to 413) | `selfhost/emit/callback_thunk.hero`, `callback_guard.hero` | 429, 207 |
| the instrument holding the sentence's two claims to the eight arms; its test against the real spec (213 to 222) | `tests/harness/spec_refusals.hero` (run as `spec/refusal-words`, `tests/harness/suite_spec.hero:1638-1648`) | 222 |

## What you build, in your copy, and run (this Mac plain and `--sanitize`; the Linux arm64 image plain and `--sanitize`; three runs each)

1. **The base's answer to each exit, as a table** (`00-shared.md` § 3.4 is this Mac's half, run by the clerk and again
   by the repair through the `run` form; **the repair ran the key and the probes on Linux arm64 at the base**,
   `ground/linux-arm64/`): each case of § 3.4 and each blind program (`blind/programs/`, `h1` to `h13` and `n1` to `n4`,
   by `blind/key.sh <compiler> <out>` from your copy's root once you copy `blind/` there) **on `main`'s runtime where it
   moved after the base** (the second column), both platforms. Name every cell where the two platforms, the two builds
   or the two runtimes differ.
2. **Question 4, the *may***: is the abort after an exit the runtime does not follow still **a stack-depth test on
   Linux** (panel 257's `p4s` runs, `p4d` at 5,120 bytes and deeper aborts: `docs/panel/257-*.md:56-62`, the
   programs in `docs/panel/257-reports/evidence.tar.xz`)? Run `p4s`, `p4d` and `p4x` at the base on both platforms.
   And **list, exit by exit of the sentence, which cases abort on every build and route and which run on some**, from
   the goldens (`fixedbugs-2702-*`, `2705`, `2706`, `2707`, `2745`, `2848`, `2850`, `2851`: **62, 1, 2, 4, 10, 6, 5 and
   3 `run` cases** by `ls tests/golden/run | grep -c 'fixedbugs-<n>.*\.hero$'`, the repair's census; 133, 2, 5, 9, 21,
   13, 11 and 7 files by the same `grep` without `\.hero$`: the clerk named the second command and gave the first's
   numbers, the critic's § 1 table) and from your own: the split inside the third exit (`2707`'s `siglongjmp` into a
   coroutine runs, `2705`'s is refused; `2702`'s `setcontext` to its coroutine runs **with the handler on the
   coroutine's own stack** and is refused from the alternate stack: the repair's correction, `00-shared.md` question 4)
   is a reading of names and `.expected` files: **say what, in a program a reader can see, decides it**, and **on which
   route** (the drafted `d-lane-sig-family-1` refuses `cs_0` on route A).
3. **Question 2, the property**: write the property the runtime tests in one sentence a reader could use, and **hold
   it against every refusal** you can produce: does each refused program have a signal handler **of its C** that
   began on the callback's thread? (Every plain-built program has the runtime's own, `stack.c:1269-1282`: say whether
   that matters to the property.) **The one that does not at the base**: `mj_carved` (CALLED under `--sanitize` on
   Darwin, defect 2852, open, unrepaired), `00-shared.md` § 3.4 and `ground/probe/`; **`escape_carved`**, refused in
   REENTERED's words at the clerk's base and at `c18a9a9a3` (defect 2855), **runs at the base** since step 199d made a
   landing re-enter a handler only inside the frames of a handler a jump left (`sigentries.c:432-445`), plain and
   `--sanitize`, this Mac and Linux arm64 (the repair's probes): **hold 199d's span rule against its shapes beside**
   (a coroutine on a carved stack leaving for good after a handler was left by a jump at the depth its host later runs
   is one of its three cases; what of a span the record widens when it is full, `sigentries.c:465`?). **Start from the class's three members** (the critic's C9:
   2831, closed by step 185a at `a4bdf4257`, an unseen `makecontext` through `dlsym`; 2852, the walk on Darwin's fiber
   frame pointer; 2855, rule 6 on a carved stack: three mechanisms, one day) and find a fourth if there is one (does
   any frame-record layout reach the walk plain? a thread C made is the thread's words, not the sentence's). **Ask the
   lane's state** (`git log --oneline 87003cb42..main -- runtime`) **before** you write a sentence resting on 2852:
   where the lane has landed G, build `main`'s runtime as your second column, **never inside its
   worktree**, and run the two again.
4. **Question 3, the promise, and its falsifiers** (repaired: the critic's C2, C3, C4, C7, C8, C24). Panel 257's `x1`
   promises a callback after a `siglongjmp`, `longjmp` or `_longjmp` out of a handler onto the thread's own stack; **both
   its conditions are met and it is false as written** (`00-shared.md` question 3). Run, for x1, x2 and every
   run-promise a warden text makes, **each falsifier**: `fixedbugs-2705` and
   `fixedbugs-2702-a-handler-jumps-into-a-coroutine-on-the-own-stack-which-calls-the-function-is-refused` (ONSTACK, the
   own-stack coroutine); a **watchdog** (`00-shared.md` § 5: a `SIGALRM` handler `siglongjmp`ing out of a Heroes loop
   that allocates, many runs, a rate, both platforms: panel 258's jump refusal, `sigguard.c:175-212`, stops it at the
   jump in words of its own); **route A** under `HERO_JMP_FORCE_UNCALIBRATED` (the values `sigjump.c`, `rebind.c` and
   199a's body name) and under the simulation 199a ran (`HERO_JMP_NO_DECODE`, `sigjump.c:130-132`, with
   `HERO_JMP_MAKE_IN_C`, `:134-145`; defect 2848's reproducer named `HERO_JMP_NO_MAKE`, which no file of
   `runtime/parts/` holds since 199a, `grep -rn`): 2706's two programs (HELD, ONSTACK, by 199a's body) and the drafted
   defect's `b_0` and `cs_0` (UNFOLLOWED, its reproducer: a copy of `runtime/` whose `runtime.c` opens `#define
   HERO_JMP_NO_DECODE 1` and `#define HERO_JMP_MAKE_IN_C 1`, in the Linux arm64 image); **the thread** (`blind/programs/h9`
   runs, `same.hero` in `docs/panel/262-reports/programs/critic/` aborts). **Which real systems stand on route A**:
   glibc's architectures other than aarch64 and x86-64, a glibc whose layout moved, **and Darwin x86-64 by
   construction** (`rebind_decode.c:314-315`; **no Intel Mac is on this disk**, so its cells are *unrun*, never
   inferred); name what the CI builds (`.github/workflows/`: `ubuntu-26.04`, `ubuntu-26.04-arm`, `macos-26`,
   `windows-latest`, the critic's reading) and what the runtime prints for `hero_signal_jump_route` there
   (`fixedbugs-2679-the-runtime-says-which-route-decides-a-jump`, `route 3`: passed on both CI Linux legs at
   `4a5e585a9` by the absence of its line among the failures, CI run `45654270318`, the critic's C3 re-read by the
   repair; on Linux arm64 at the base by the repair's container, `ground/linux-arm64/L-2679.txt`).
5. **Each candidate's promise, built**: for every text the warden writes (`docs/panel/262-reports/spec-w*.txt`), one
   program per clause that promises a run (it runs, three of three, both platforms, plain and `--sanitize`) and one per
   clause that promises an abort (it aborts, saying the claimed words), **and the falsifiers of item 4 against each
   run-promise**. **A clause with a falsifying program is not adopted**; say which program falsifies which clause, on
   which platform, route and thread. Do the same for panel 257's `x1` and `x2` (`257-reports/spec-x1.txt`,
   `spec-x2.txt`).
6. **Candidate (d3), a runtime change so the sentence can be simpler** (`00-shared.md` § 4): what would it take for
   *"a program whose C installs no signal handler is never told these words"* to be true, and is it the lane's E3 and G
   or more (a thread C made aside; 2831's class of unseen `makecontext`; the runtime's own handler)? And the other
   direction: a runtime that **always** refuses after an exit it does not follow, so *may* becomes *aborts*: which
   correct programs would it refuse that run today (§ 3.4's runs, the key's `h1`, `h3`, `h7`, `h9` to `h11`, `h13`)?
   **And (d8)**, abort at the exit the runtime sees rather than at the next callback, as panel 258 does for the
   allocator: which programs it refuses that never call a callback after (`h5`'s twin with no callback), counted. Price
   each, lines and the thunk's cost (`cost.sh` of panel 259, `docs/panel/259-reports/evidence.tar.xz`, at `-O2`,
   instructions retired, never a duration at this load), **without repairing the lane's defects in your copy**: the lane
   owns them.
7. **The instrument and the placement** (question 6): for each candidate text, the `spec` suite in your copy with the
   text applied (`heroes-next run tests/harness/main.hero -- heroes-next spec`), **the net's own tests**
   (`heroes-next test tests/harness/main.hero`, whose `spec_refusals.hero:213-222` reads the real spec), and what
   `spec_refusals.hero` must change first (`SENTENCE_OPENS`, `CLAIMS`, the first `". "`, `EXEMPT`); and whether a split
   sentence needs the instrument to read two. The compiler's test of defect 2533 (`callback_thunk.hero:400-413`): run
   `heroes-next test selfhost/main.hero` narrowed by its title, `every signal refusal ends with spec section 13's own
   sentence`, if the harness's filter reaches it, or whole. **Where a claim moves, count the goldens it moves by the
   form**: 90 `run` cases at the base; the `check` case `package-a-file-source-in-no-repository` holds *from inside*
   and is no refusal (`00-shared.md` question 13).
8. **The shapes beside** (`00-shared.md` § 5), **yours by the critic's C25 and C28**: the watchdog (item 4); **a handler
   installed by a raw system call that calls the function on Linux x86-64 with a restorer of its own** (guard D alone
   should stand: an inference, unrun; the x86-64 image, recorded, not graded); **a handler left by
   `_Unwind_ForcedUnwind` from C**; **§ 9's abort on a coroutine's stack** (a callback recursing past a `malloc` stack,
   and past an `mmap` stack with a guard page, both platforms: aborts, faults, or writes into the heap?); and one each
   that nobody named (CLAUDE.md § RUN IT: none, one, nested, the alternate stack, a thread, route A, Windows by reading
   `sigguard.c`'s branch).

## What you judge

`verdict` (approve, object, veto) per candidate of `00-shared.md` § 4 and per warden text, with `section` (design.md
§1.12, §4.17, §4.19), `cost` (lines of the runtime and the instrument, goldens whose words move, the thunk's cost if a
route changes it), `prediction` (one falsifiable sentence, with the landing that checks it) and `condition` (what would
change your mind). **A text that promises what a program you ran falsifies is a soundness veto**; say which program,
platform, route and thread. Name what a veto would compel.
