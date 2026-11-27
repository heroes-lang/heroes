# Panel 262, the shared brief: what spec § 13's handler sentence promises a callback, and when it may abort

**Amended 2026-11-26, about 21:45, by the author's word** (*ok a 25 dollari*, in conversation): the paid run **is authorised, with a ceiling of 25.00 USD for the whole run**, never more. The ceiling of 25.50 USD proposed below for the 31 sessions is **lowered to 25.00 USD** (the runner stops before a session whose cap plus margin would pass it; the 21-session plan, 17.50 USD at the caps, stays within it). The lines below that say the run is not authorised, or that the ceiling is proposed, are history and stay; this paragraph governs. **Still owed before any session**: the labels' reliability, two independent readers on panel 259's wrong answers (the repair measured kappa 0.02 on W and 0.07 on H against its own keyword rule: `blind/labels-259/`), and the synthesis.

**A full panel** (`.claude/skills/panel/SKILL.md` § Two lanes): `compiler-engineer`, `ffi-pragmatist`, `spec-warden`,
`historian`, and `llm-ergonomist` **registered, its paid run NOT authorised** (the author authorised 20.00 USD for
panels 259, 260 and 261 only; **the run waits for the author's yes, which the coordinator asks for**:
`llm-ergonomist.md`), and the completeness critic, who read these briefs FIRST, before any seat starts (§ 3c). The
change is a spec sentence and what a reader predicts from it (CLAUDE.md § 4: `spec/`, a diagnostic's words), and the
finding that convenes the sitting is a reader's, so the soundness lane would lose the half the sitting is for.

**Repaired before any seat starts**, 2026-11-26 from 15:46, from the critic's first reading
(`docs/panel/262-reports/critic-briefs.md`, 28 findings C1 to C28, and its *What the coordinator must repair before any
seat starts*, nine items), and **moved onto a new base** (below): every number, line pointer and probe the move or the
repair could change was re-run by the repair, at the new base, with compilers it built from the seed in
`.claude/worktrees/panel262-repair-scratch/` (§ 7). A sentence the repair changed says so where the change matters;
the critic's findings are credited to the critic by their number. **The critic's six repairs, as the repair's brief
ordered them**: (1) x1 and every run-promise are no longer presented as waiting on conditions (§ 2 question 3, § 3.5);
(2) panel 258's refusal of a jump out of the runtime's allocator stands in question 9 and § 5; (3) the blind
registration labels every wrong verdict, fixes D2's set, adds five labels and was tried on panel 259's answers
(`blind/registration.md`); (4) the ground is re-anchored on `main` (below, § 6); (5) what the landing owes outside the
spec is named (question 13); (6) the critic's framing facts and unlisted routes and shapes are handed on (§ 2, § 4, § 5
and the seats' briefs).

**The author's instruction, 2026-11-26 at about 14:06** (the answer *3b* to the coordinator's recommendation, recorded in
panel 259's synthesis, `docs/panel/259-*.md:315-323`, and in `.claude/worktrees/rebel-scratch/coord/agents-live.md:419`,
meant as: *ratify panel 259, and hold a sitting on spec § 13's handler sentence before the milestone's tag, not after
it*; the coordinator had recommended after). Under CLAUDE.md § Precedence as the author amended it on 2026-11-22 (*"the
most robust solution now always wins, a new form included and at the cost of the spec's size"*), **the budget and
Principle 0 are costs to be measured and written, never a veto**; a seat's soundness veto stands as ever.

**The base is `main` at `87003cb42`** (`87003cb42946dc0f99dbab36582693c4afb3777a`, 2026-11-26 20:44:29 +0100, the merge
of lane-seed-regen's step 207b; `git log -1 --format='%H %ci %s' main`, the repair, 20:45), unless the coordinator's
prompt names another. **The clerk wrote these briefs at `aded91388`** (`aded91388584da5378f073c516662a095f48b7cd`,
2026-11-26 14:11:41 +0100, the merge of lane-land261). **The repair moved them twice**, with `git -C
.claude/worktrees/panel262 checkout --detach` (the briefs are untracked and travelled with it; `git status --short`
after each: the two folders `262-briefs/` and `262-reports/`, nothing else): **first onto `c18a9a9a3`**
(`c18a9a9a3cd777bca7701797b90ca5c36333107a`, 15:48:15, the repair's first pass, 15:46 to 16:31; its runs are kept
under `ground/` and `blind/` with `c18a9a9a3` in their names), **then onto `main`'s tip** at the coordinator's word after
the usage limit had stopped every agent (*"Main moved a lot ... move the base onto its tip once, at the end, and refresh
what the move changes"*), 20:44 on. **What the two moves changed** (`git log --oneline` and `git diff --stat` of each
path, the repair):

- **the spec, design.md, `selfhost/emit/callback_thunk.hero`, `callback_guard.hero`, `tests/harness/spec_refusals.hero`
  and `suite_spec.hero`: nothing** (`git diff --stat aded91388 87003cb42 --` of each, empty), so every spec line, every
  design.md line and every pointer into the instrument and the thunk cited below stands where the clerk read it;
- **the runtime**, from `aded91388` to `c18a9a9a3` (13 files, +412 −117): **step 199b** (`3e02951e0`, merged at
  `612776983`, **defect 2850**: a coroutine the C library starts through another's `uc_link` is entered with it),
  **step 199c** (`590ab2503`, merged at `3e2c5d8e8`, **defect 2851**: the context a swap switched to is read wherever
  it is mapped), **step 200a** (`2978e6aed`, merged at `51b9be1ca`, **defect 2854**, lane room-mixedown: the room's
  abort counts every thread's callbacks that have not returned; `callback.c` 111 lines to 167, `thread.c` 144 to 148,
  `hero_thread_refuse` from 128-139 to **132-143**), step 198d (`cd001bf18`, `replace.c`); **and from `c18a9a9a3` to
  `87003cb42`** (8 files, +271 −43, `git diff --numstat`): **step 199d** (`77fade16c`, merged at `c54db505f`,
  **defect 2855**: *"a landing re-enters a handler only inside the frames of a handler a jump left, so a coroutine
  leaving for good to its host runs"*), in `sigentries.c` (+102 −17; 588 lines to 673), `rebind_entries.c` (+99 −14;
  562 to 647), **`sigguard.c` (+22, the thread's record of spans: every `sigguard.c` pointer from line 446 on moved by
  +22**, those before it did not), `rebind_jump.c` (+21 −8), `sigjump_save.c` (+15), `rebind_image.c` (+9 −4),
  `rebind_record.c` (+2), `siginstall.c` (+1);
- **the seed refreshed** (step 207a, `43df12cbf`, merged at `ee2aef6d9`, *"so the compiler it makes reads the harness's
  `layer`"*): the seed's compiler is a new build (`sources 1e1eb23a1927d35b`, below);
- **`selfhost/`**: panel 261's landing, steps 203b to 203e (lane land261: a mark in an extern callback's type refused,
  an extern taking a callback reaching C through its thunk when called through a value, panel 261's R3 to R8), and
  step 200b (defect 2856, the platform layer's declarations), in `check/`, `cli/`, `emit/` and `resolve/`, **none in
  the thunk's two files** (above); **the spec text of panel 261 is not landed at the base** (`git log -1 87003cb42 -- spec`: step 197d; it landed after,
  below);
- **the goldens**: `fixedbugs-2850-*` (5 `run` cases), `fixedbugs-2851-*` (3), **`fixedbugs-2855-*` (3)**, step 200a's
  room cases, panel 261's `function-type-marks-*` and defect 2856's cases;
- `ee2aef6d9` to `87003cb42`: Rebel's files and `.claude/rules/verification.md` alone (`git diff --stat ee2aef6d9
  87003cb42 -- spec runtime selfhost seed tests docs/design.md .github site/src`: empty), so **the repair's trees,
  archived at `ee2aef6d9`, hold every compiler, runtime, spec and case byte of the base**.

**The critic read the briefs at `aded91388` and found `main` had moved** (its C1: step 200a changing the door every
callback crosses, the room's count, and moving `hero_thread_refuse`); the moves answer it. **`main` will move again
while the sitting sits**: the lane sig-family had **2852 and 2853** left at the base (§ 3.5), each a change to the
runtime this sitting describes, and 2853 has landed since (below). **A later merge changes the ground**: a seat asks `git log --oneline 87003cb42..main -- runtime
spec` before it writes a sentence that rests on the runtime, and where it is not empty **runs its decisive rows on
`main`'s runtime as a second column** beside the base's (`git archive main runtime | tar -x -C <a copy>`, then
`HEROES_RUNTIME=<that copy>`), as panel 257 learned (*"a sitting that tests a sentence against a route a lane is landing
owes the lane's tree as a second column from the first brief"*, the critic's C1, quoting `docs/panel/257-*.md:216`).

**And `main` moved again while the repair refreshed the ground, materially: read this before any spec line.** At
21:18 `main` was `0bde00d84` (2026-11-26 21:18:22 +0100; `git log --oneline 87003cb42..main`: 15 commits; `git diff
--stat 87003cb42 0bde00d84 -- spec runtime selfhost seed tests docs/design.md .github site/src README.md`: 22 files).
The coordinator asked for one move, at the end, and the repair made it; it did not chase this one, and **names it
here so no seat reads the base's spec as `main`'s**:

- **panel 261's spec text has landed**, step 203f (`0df8f1a36`, merged at `1456f8d92`, 20:51): `spec/heroes-spec.md`
  is now **526 lines, 36,120 bytes, sha256 `9ff7d2a1...2fc3`** (`wc -l -c`, `shasum -a 256` of `git show
  0bde00d84:spec/heroes-spec.md`, kept as `ground/spec-at-0bde00d84.md`); **the handler sentence stands byte for byte**
  (`grep -c -F` of it whole: 1), **on line 438** (`grep -n 'left a handler'`), the callback's line before it now
  reading *"... its parameters follow the same rule, with neither mark, and `()` is `void`"*; § 9's *"running out of
  stack aborts"* on 341, the platform layer on 411, `spawn`'s thread on 393, *"fails the call where the kernel
  writes"* on 435, the handle's *"a million other lives"* on 497 (each `grep -n`): **every spec line this brief cites
  past line 330 is +10 there**. **The landed text is not panel 261's adopted diff**: `ground/spec-261.md` (the clerk's
  `patch` of `synthesis-adopted.diff`) differs from it at lines 438-439 (`diff`), the landing having worded the
  callback's marks otherwise. `tests/harness/suite_spec.hero` there pins `SPEC_TOKENS` 10507 and `REAL_TOKENS`
  **13776** (`git show 0bde00d84:tests/harness/suite_spec.hero`, read, **not measured by the repair**); design.md moved
  by 47 lines (+37 −10), its coroutine rows now at 3611, 3642 and 4184 to 4190 (`grep -n`);
- **step 199e** (`01706218e`, 21:14, **defect 2853**: *"a rewrite of the C library's that does not return clears
  AddressSanitizer's shadow first, so a correct fibre program runs under --sanitize"*), `rebind_jump.c` +27 −1 and
  `sigjump_save.c` +6, two `fixedbugs-2853-*` cases; its issue open until the gate (`gh issue view 2853`). **2852
  (G) is the lane's last** guard defect.

**So**: the warden's diffs are written against the spec `main` holds when it writes them (the landed one, unless the
coordinator's prompt names the base's), and checked to apply to both; a seat whose worktree the coordinator makes at
`0bde00d84` or later reads the numbers above as its own and re-runs what this brief ran at `87003cb42` (§ 6's second
column); the blind registration's rule for a landed 261 applies (`blind/registration.md` § The arms: every arm the
landed spec with the candidate re-applied, re-hashed before any session).

**The repair's compilers** are in `.claude/worktrees/panel262-repair-scratch/tree2/`, `git archive ee2aef6d9`
extracted whole: the seed (`clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`, **`sources
1e1eb23a1927d35b`**; `05186f2b38e77a97` at `aded91388` and `c18a9a9a3`, the clerk's, the critic's and the repair's
first pass) and stage 2 (`./heroes build selfhost/main.hero -o heroes-next`, **`sources 6ce3f9e116d22e3a`**; `7205f5b93a1fc8f1`
at `aded91388`, `24c248d27fa1f712` at `c18a9a9a3`), 20:44 to 20:52 (`/usr/bin/time -p`: seed `real 59.66 user 20.50`, stage 2 `real 453.68 user 170.70 sys
34.42`; `ground/build-repair.log`, and `build-repair-c18a9a9a3.log` for the first pass). **A seat's own stage 2 at
`87003cb42` should read `sources 6ce3f9e116d22e3a`; say if it does not.** The machine read a load of 42 to 159 through the
repair's first pass (`uptime`, 15:47 to 16:31), 7 to 91 through its second (20:44 to 21:18), and 6 to 112 through the clerk's, so
**no duration in this brief is a measurement** (CLAUDE.md § Verification: the ratio).

**Every number, path, line and count below was produced by the command named beside it**: by the repair at
`87003cb42` from 20:44 where the moves or the critic's reading could change it, by the repair's first pass at
`c18a9a9a3` where the second move could not (each such place says so), by the clerk at `aded91388` from 14:13 where
nothing the moves touched could (each such place says *the clerk's*), or is marked *carried from <file>* where nobody
re-ran it, or *unrun*. Scripts and outputs: `ground/` (§ 7).

**No seat or critic starts a paid run, a `claude -p` session, an API call or a cloud run its brief does not name; none
prints a credential** (`gh auth status` masks it, `gh auth token` never); `gh` reads only; no file outside the
repository's root (**no `/tmp`: set `TMPDIR` to a folder inside your copy**, the lesson of panel 257's three seats that
wrote there, `docs/panel/257-*.md`, *How the sitting went*); no `pkill`; no commit, push or merge. Each seat works in
its own detached worktree, `.claude/worktrees/262-<seat>` (made by the coordinator at the base), builds its own compiler
there from the seed and stage 2 (§ 6), and **never reads, builds or runs inside another seat's directory, the lane
sig-family's worktree or its scratch** (`.claude/worktrees/lane-sig-family`, `lane-sig-family-scratch`): the lane is
repairing the runtime this sitting describes, today (§ 3.5).

## 1. The proposal, in ten lines

1. Spec § 13's *"A callback called after C left a handler of C's by [five exits] ... may abort, saying the handler was
   not seen to end, or that C called it from inside one"* is read at least **four** ways: as covering **every** switch
   C makes (panel 259's blind `now`: 3 of 5 t1, 4 of 5 t2), as **aborts** for each exit it names (panel 257's: 10 of
   10 `adopted` and `rival`), as an **exhaustive** list promising every other exit (defect 2848's body), and **as
   reaching a callback on another thread** than the handler's (the critic's C24, unseen by any run: `now` has no *"on
   its thread"*); and two more no run has shown, the pronoun and the remedy's own handlers (question 1).
2. Decide what it says, so that a reader of the spec alone predicts, for a callback and a C library that switches
   stacks or jumps, **when the program is promised to keep running, when it may abort, and with which words**:
   (a) split, a *signal handler* of C's apart from a *coroutine switch*; (b) the property the runtime tests, not C's
   mechanisms; (c) the sentence plus one clause; (d) the routes nobody listed, (d1) to (d9). Each built and measured,
   none assumed.
3. Ground: `main` at `87003cb42` (panels 259's and 260's landings, panel 261's compiler steps, steps 199a to 199d,
   200a, the refreshed seed) and the guard defects of § 3.5, two of them still under repair.

## 2. The questions the seats measure, never assume

Each is a question; a seat that answers one says by which command.

1. **The readings, and which a candidate leaves.** Panel 257's blind run: *"10 of 10 readers of `adopted` and `rival`
   took *"may abort"* as *aborts*"* and none wrote RUNS on its p4 or p7 (`docs/panel/257-*.md:281-283`). Panel 259's:
   *"the handler sentence's *"may abort"* covers any frame C leaves by a `swapcontext`, an assembly jump or a
   `setcontext`, whatever a handler is"*, the reading of `t1-h-1`, `t1-h-2`, `t1-h-4` and `t1-u-5`; the two `now` t1
   readers who printed read *handler* as a signal handler (`259-reports/llm-ergonomist-blind.md:157-161`); t2's four
   (`:178-185`); the verdict session: *"It does not fix the larger ambiguity in the same paragraph, the 'handler of C's'
   sentence"* (`:237-240`). Defect 2848's Origin (`gh issue view 2848`), written by lane-ctx-after, a writer of the
   runtime and no blind reader: *"spec § 13 lets a callback abort after a jump or a `setcontext` onto a stack once given
   to `makecontext`, and this one is not, so the refusal is a change of what the spec promises"*. **The first two are
   opposite errors from one word**: one widens *may* to every switch, the other narrows it to *will*. **Added by the
   repair, from the critic's C24 and from 259's archive**:
   - **the thread** (T): the runtime's entries are per thread (`HeroSignalStack` is `_Thread_local`,
     `sigguard.c:453` at the base, `:431` at the critic's), so a callback on one thread after C left a handler by
     assembly on another **runs**: the critic's `programs/critic/other.hero` on this Mac and Linux arm64 at
     `aded91388`, and the repair's `h9` at `c18a9a9a3` and at the base, both platforms (`blind/key-mac-repair/out-h9-*`:
     `left on the thread: 1`, `answer ran for 7`, `result 70`, exit 0, 3 of 3 plain and `--sanitize`), where
     `same.hero`, both on one thread, aborts in STANDING's words (the critic's; the repair's container at the base,
     `ground/linux-arm64/L-out-same-*`). `now` says
     *"A callback called after C left a handler"*, with no *"on its thread"*; x1 has one;
   - **the pronoun** (I): in *"by a jump or a `setcontext` onto a stack once given to `makecontext`, or by a jump that
     lands within it"*, the nearest antecedent of *it* is *a stack*. **Measured in 259's archive by the repair**: 2 of
     5 `now` t1 readers name that choice and both read *it* as the stack (`t1-h-1` choice point 4, `t1-h-4` choice
     point 2; `blind/registration.md` § The labels, tried);
   - **the remedy's own handlers**: the sentence after says to count signals with `hero_signal_watch`, whose handler is
     the runtime's C; a reader may take it for *"a handler of C's"*. It runs (the critic's `watched.hero`, this Mac and
     Linux arm64; the repair's `h13`, this Mac and Linux arm64 at the base: `watch 0`, `answer ran for 7`, `result 70`,
     `counted 1`, exit 0, 3 of 3 each build);
   - **a platform split** (P): *may* came from one (panel 257, `:56-62`, `:106-109`).

   What does each candidate leave a reader, and is there a text every run would read right? The blind registration
   measures it (`llm-ergonomist.md`, `blind/registration.md`: D1, D2, D5 and the labels).
2. **The property the runtime tests** (§ 3.3). Every refusal of the guard's fourth question is a callback called while
   the thread holds an entry for *"a handler of C's that the runtime saw begin and has not seen end"*
   (`runtime/parts/sigguard.c:49-51`), and three more questions find a delivery's own frame beneath the callback. **Is
   every such refusal one where a signal handler of C's began on the callback's thread? At the base, no**: a program
   that installs no handler is refused in the sentence's words under `--sanitize` on Darwin (§ 3.4's `mj_carved`,
   defect 2852, open and unrepaired), by defect; a second, `escape_carved` (defect 2855), was refused at the clerk's
   base and at `c18a9a9a3` and **runs at the base** (step 199d); **the class has three members in one day** (the critic's C9: defect 2831,
   closed, *"a `makecontext` reached through `dlsym` is glibc's, so ... a correct program is refused in the words of a
   signal handler it does not have"*, repaired by step 185a at `a4bdf4257`, then 2852 and 2855; `gh issue view 2831`:
   closed 2026-11-26T12:37:48Z), each by another mechanism. **And every plain-built program already has a signal
   handler before C does anything** (the critic's C12): `runtime/parts/stack.c:1269-1282` (`hero_stack_guard_install`)
   installs `hero_stack_handler` for SIGSEGV and SIGBUS with `SA_SIGINFO | SA_ONSTACK` once per process, called from
   `hero_args_set` (`os.c:703`), on an alternate stack it registers per thread (`stack.c:1235`, `sigaltstack`), and
   chains to the handler it found (`hero_stack_prev_segv`, `:786`, `:804`); under `--sanitize` the guard yields to ASan
   (`HERO_STACK_GUARD_YIELDS_TO_ASAN`, `:112-117`). So *"a program whose C installs no signal handler"* is a class of
   the program's C, never of the process. Can a sentence name the property without the mechanisms, and is it the same
   on Linux (wrappers, `siginstall.c`), Darwin (rebound pointers, `rebind*.c`) and Windows (*"no probe and no walk"*,
   `sigguard.c`'s Windows branch, line 714 on at the base)? And the residues the guard says it does not see (`sigguard.c:61-74`,
   `sigjump.c:68-81`).
3. **What is promised to keep running. Rewritten by the repair** (the critic's C2, C3, C4, C8; repair 1). The spec
   promises nothing positive about a callback after a handler: panel 257's R2 withheld its positive clause `x1`
   (`257-reports/spec-x1.txt`, quoted in § 3.2), *"It lands in a later amendment when the runtime lane's Darwin repair
   of the `fp3` class lands (a defect drafted apart, R4), and `ubuntu-26.04` has read the accessor"*
   (`docs/panel/257-*.md:143-151`). **Both conditions are met, and x1 is still false as written**:
   - **the first**: defect 2706 (`fp3`, `n6`) closed 2026-11-23 at `d223a7917` (`gh issue view 2706`), its two goldens
     pass at the base (§ 3.4);
   - **the second**: CI run `45654270318` (the push of `4a5e585a9`, 2026-11-25, `failure` for other reasons), its
     Linux x86-64 and Linux arm64 legs on `ubuntu-26.04` and `ubuntu-26.04-arm` (`Ubuntu clang version 21.1.8`), name
     in their `run` shards' failures and skips no line of `fixedbugs-2679-the-runtime-says-which-route-decides-a-jump`,
     whose `.expected` is `route 3`, and print its emission's *"nothing blessed"* in the `emission` form (the critic's
     C3; **re-read by the repair**, `gh run view 45654270318 --job <id> --log` for jobs `139346835548`, `...603`,
     `...562`, `...834`, `...543`, `...6292`: six `FAIL run/` lines, none of 2679's route, one `SKIP run/` of
     `fixedbugs-2679-libpng-...`). So the route case passed on both CI Linux legs, **an inference from its absence**
     (the log prints failures and skips by name, passes by count); the glibc on that image is *carried* as 2.43 from
     panel 257. No CI run on a later commit has completed its Linux `run` shards (`gh run list --workflow ci.yml`, the
     repair, 15:55: `45809762298` at `a60dfb77f` cancelled with `run 3/3` cancelled on both Linux legs; `45826172756`
     at `1e56b5da5` queued). Steps 199a to 199c moved no calibration file (`sigjump_decode.c`, `sigjump_probe.c`,
     `rebind_decode.c`: `git diff --stat aded91388 87003cb42 --` of the three, empty, and the critic's reading of 199a);
   - **x1 is false at the base, on the platforms the CI builds, by programs this brief runs**: `fixedbugs-2705` (a
     handler's `siglongjmp` into a live coroutine on the thread's own stack: refused in ONSTACK's words) and
     `2702-a-handler-jumps-into-a-coroutine-on-the-own-stack-which-calls-the-function-is-refused` (the same words); a
     coroutine carved from the thread's own stack is *"the thread's own stack"* by address, and x1's reader cannot tell
     it is excluded (the critic's C4);
   - **any asynchronous handler that `siglongjmp`s out of code inside the runtime's allocator stops the program at the
     jump** (panel 258's R1, `sigguard.c:175-212`, question 9), whatever the target;
   - **on route A** (glibc's architectures but aarch64 and x86-64, a glibc whose layout moved, a build forced to it,
     **and Darwin x86-64**, the critic's C2: `runtime/parts/rebind_decode.c:314-315`, *"Darwin x86-64: no decode is
     read here (no machine to calibrate it on), so both families stay on route A, by construction"*), **2706's two
     programs are refused** (step 199a's body, `git show -s 65da4688c`: *"in HELD's and ONSTACK's true words, route A's
     ratified cost (panel 259's R3)"*, by simulation) **and `fixedbugs-2702-a-handler-left-by-setcontext-to-the-runner-runs`
     (`b_0`) and `...-to-its-coroutine-runs` (`cs_0`), correct programs, exit 134 in UNFOLLOWED's words** (the drafted
     defect `d-lane-sig-family-1`, § 3.5, measured by the lane under the simulation on Linux arm64). **No CI leg runs
     route A**: `ci.yml` and `rebel.yml` name `ubuntu-26.04`, `ubuntu-26.04-arm`, `macos-26` and `windows-latest`
     (the critic's reading; an Intel Mac is a machine a reader calls *a Mac*, route A by construction, **unrun** on such
     a machine);
   - **x1 says *"on its thread"*; `now` does not** (question 1's T).

   So: **on which platforms, routes and threads does a callback after a `siglongjmp`, `longjmp` or `_longjmp` out of a
   handler run, and what does a run-promise promise at each falsifier above?** The spec names no system, Linux, Darwin,
   Windows or macOS, anywhere, and *platform* twice, § 9 line 330's *"compiles on every platform"* and § 13 line 401's
   *"platform layer"* (`grep -n -i -E 'platform|linux|darwin|windows|macos' spec/heroes-spec.md`, the clerk's; the spec
   did not move). **And with no handler at all**: is a callback in a coroutine promised to run? Today the tree's `run`
   goldens say yes but by the room (§ 3.4), defect 2852 says no under `--sanitize` on Darwin, and defect 2855 said no until
   step 199d.
4. **The word *may*.** It is the spec's **only** *may abort* by the phrase (`grep -n -o -i '[^.]*may abort[^.]*'
   spec/heroes-spec.md`: one sentence, line 428); every other abort the spec names is stated as *aborts*, and § 6
   defines it (*"An abort ends the program at once, saying why"*). **But not the only abort a reader cannot predict from
   the program** (the critic's C6, re-read by the repair): § 9's *"running out of stack aborts"* (line 331, no depth
   given); lines 424-425's *"C reaching past them aborts, or fails the call where the kernel writes"*; line 487's handle
   given back, *"aborts, ... unless C has since handed its address out again or a million other lives have ended"* (a
   stated exception: *unless*, not *may*); and the room's bound (line 428, a count the run decides). These are the spec's
   own precedents for wording a conditional abort. Panel 257 chose *may* on two measurements: the abort after an exit
   the runtime does not follow was *"a stack-depth test on Linux"* and not on Darwin, *"so ... "may abort" is the only
   true word on both"* (`docs/panel/257-*.md:56-62`), and Linux ran its `p7` (`swapcontext` away) where Darwin refused
   it *"until 2702"* (`:106-109`). **Both may have moved**: defect 2702 closed 2026-11-23 and `sigentries.c` now carries
   *"Darwin's rules ... the same seven rules, the same shapes judged the same way, so that one golden holds on both
   platforms"* (`runtime/parts/sigentries.c:9-15`, unchanged by the move); whether the depth test on Linux survives is
   **unrun** by the clerk and the repair. And *may* still covers a measured split **inside one exit**: a `siglongjmp`
   out of a handler on an alternate stack **into a coroutine** runs
   (`fixedbugs-2707-a-siglongjmp-out-of-a-handler-on-an-alternate-stack-into-a-coroutine-mapped-after-it-runs`), where a
   `siglongjmp` into a live coroutine from a handler on the thread's own stack is refused (`fixedbugs-2705-...`); and a
   `setcontext` out of a handler to its coroutine **runs with the handler on the coroutine's own stack** and is refused
   with it on the alternate stack (`fixedbugs-2702-a-handler-left-by-setcontext-to-its-coroutine-runs`,
   `fixedbugs-2702-a-handler-on-the-alternate-stack-left-by-setcontext-to-its-coroutine-is-refused`; § 3.4 runs them).
   **Corrected by the repair**: the clerk wrote *"with the handler on the thread's own stack"*; the header says *"the
   handler (on the coroutine's stack where `onstack` is 0, on the alternate stack where 1)"*
   (`fixedbugs-2702-a-handler-left-by-setcontext.h`, its first comment): the coroutine raises, and a handler with no
   `SA_ONSTACK` runs on the stack that raised. **And that split is route C's alone** (the critic's C8): on route A the
   same `cs_0` is refused in UNFOLLOWED's words (`d-lane-sig-family-1`). Which exits abort on every build and route
   today, which only sometimes, and by what a reader can see in their program?
5. **The word *handler*.** Line 427: *"never a result or a signal handler, nor called by one"*; line 428: *"a handler of
   C's"*; the remedy that follows: *"No function of the program is a signal handler"*. Readers took *handler* as any
   frame of C's (`t1-u-5`'s choice point 2: *"I took it to mean a callback frame: a function of the program that C
   called"*, `259-reports/llm-ergonomist-blind.md:159-160`) or as a signal handler (`:161`); in 259's archive every one
   of the 30 answers the repair labelled H also reads the *may* as covering the switch (W), so the two are one finding
   there (`blind/registration.md` § The labels, tried). Does *signal handler* everywhere settle it, and what does it
   cost the instrument (question 6)? And the runtime's own handlers (question 2, C12; question 1, the remedy's): is a
   text about *"a signal handler of the program's C"* or *"of the process"*?
6. **The words a refusal prints.** The sentence's two claims are held to the runtime's eight arms by
   `tests/harness/spec_refusals.hero` (222 lines, unchanged by the move): it finds the sentence by **its opening
   words**, `SENTENCE_OPENS = "A callback called after C left a handler of C's"` (`:39-40`), ends it at the first
   `". "` (`:68`), and asks each claim, `CLAIMS = ["seen to end", "from inside"]` (`:32-36`), of the sentence and of
   every arm but `DELIVERED` (`:43-44`); it runs as `spec/refusal-words` (`tests/harness/suite_spec.hero:1638-1648`),
   green at the base (§ 3.7), and **the net's own tests run its test against the real spec and the real
   `sigguard.c`** (`spec_refusals.hero:213-222`, *"the runtime's refusals as the tree holds them keep a claim each"*;
   the critic's repair 5). So **a candidate that re-opens, splits or re-words the sentence moves that instrument**,
   which `.claude/rules/spec-shape.md` § How a change to the document is made moves first (*"instruments first"*). The
   tail of line 428, from *"No function of the program is a signal handler"*, is held byte for byte to
   `HERO_SIGNAL_REMEDY` by the compiler's own test of defect 2533 (`selfhost/emit/callback_thunk.hero:400-413`), so
   every candidate stands **before** that sentence. **Three arms' words stand in no golden** (§ 3.3's table): CALLED's,
   HELD's and UNFOLLOWED's current words. **The thread refusal** (`runtime/parts/thread.c:132-143` at the base, 128-139
   at the clerk's, *"ran on a thread this program did not start"*) carries neither claim and no sentence of the spec
   names it (question 8). **And two more refusals of a handler's doing carry neither claim** (the critic's C7, re-read
   by the repair): **panel 258's jump refusal** (`sigguard.c:175-212`, `hero_signal_jump_checked`: *"panic: a signal
   handler jumped out of the code it interrupted while that code was inside this runtime's allocator ... so the program
   stops here, before the jump"*), no arm of `hero_signal_refuse`, so `spec_refusals.hero` never reads it, **in no
   golden** (`grep -rlF 'jumped out of the code it interrupted' tests/`: nothing) and in no spec sentence; and **guard
   D** (`runtime/parts/alloc.c:670-678`, `hero_alloc_reentered`: *"a signal handler used this runtime's allocator while
   the code it interrupted was inside it ... a Heroes function given to C as a signal handler, or called from one"*,
   ending in `HERO_SIGNAL_REMEDY`) and its twin for the record of lent buffers (`runtime/parts/lend.c:216-225`,
   `hero_lend_reentered`), **one golden each**
   (`fixedbugs-2522-a-handler-installed-by-a-raw-system-call-that-allocates-inside-the-allocator-is-stopped-by-name`
   and `...-that-lends-inside-the-lend-tracking-is-stopped-by-name`; `ground/census.txt` counts the first's words, 1,
   and `grep -rlF "a signal handler used this runtime" tests/golden` finds the two).
7. **The lane's state, asked before any sentence about the defects** (§ 3.5). A sentence written today is true of the
   base; the sitting says which defect's landing each clause waits for, and what a reader is told meanwhile. **The
   ground moves under the sitting** (the head of this brief): ask again before writing.
8. **The shapes beside** (CLAUDE.md § RUN IT; CL-061): **none** (a callback on a thread C made, no handler: refused in
   its own words, `fixedbugs-2494-a-heroes-function-a-c-handler-calls-on-a-thread-c-made-is-refused`, § 3.4; `grep -n -i
   'thread C\|C made\|thread of its own' spec/heroes-spec.md` finds only line 383, `spawn`'s), **one** coroutine, two,
   **nested** handlers (`fixedbugs-2745-*`, 10 `run` cases), the **alternate stack** (`2702` and `2707` pairs),
   **route A** (199a's simulation; Darwin x86-64), **`--sanitize`** (2852, 2853), **Windows** (no probe, no walk),
   **another thread** (question 1's T), and **a coroutine library that makes its stacks through a signal handler** (the
   "signal stack trick" of GNU Pth, as recalled: **unverified**, the historian's to source, the ffi-pragmatist's to
   build in four variants, `ffi-pragmatist.md`): every coroutine of such a library starts inside a handler's frames,
   so what are its callbacks told? unrun. **And the shapes the critic's C25 adds** (§ 5): a watchdog, a handler
   installed by a raw system call on Linux x86-64, `_Unwind_ForcedUnwind`, POSIX's own callbacks on threads the C
   library makes, systems the runtime was never built on.
9. **The aborts beside it. Widened by the repair** (the critic's C7; repair 2): a coroutine or a handler program can
   meet, beside the handler sentence's own: **the room's** (panel 259's R2, step 192c, the sentence immediately before
   the handler sentence: its words *"saying how many callbacks have not returned"*); **§ 9's** *"running out of stack
   aborts"* (line 331); **the thread's** (question 8, no sentence); **DELIVERED**, line 427's *"never ... a signal
   handler, nor called by one"*; **panel 258's jump refusal**, which stops a program **at a handler's `siglongjmp`** out
   of code inside the runtime's allocator or its record of lent buffers, in words of its own, **before** the jump
   (`sigguard.c:175-212`, question 6), the shape a **watchdog** meets (a `SIGALRM` handler of C's that `siglongjmp`s out
   of whatever it interrupted: the outcome is where the signal lands, which no reader sees); and **guard D**, the words a
   handler installed by a raw system call meets (question 6). **Every candidate that promises a run after a `siglongjmp`
   (x1, (b)'s *"known to have ended"*, (d1)) says what it promises there**: scoped to a signal that interrupted C and
   not the runtime, or saying that a jump out of the runtime's own code stops the program. **And § 9's abort on a C
   coroutine's stack may be none** (the critic's C28): the runtime's stack guard is a fault on a guard page (`stack.c`,
   `hero_stack_foreign`, `:856-866`), and a coroutine stack from `malloc` (every `co.h`, `sw.h`, `n*` and `h8` stack)
   has none; whether a callback recursing past its end aborts or writes into the heap is **unrun** (`ls
   tests/golden/run | grep -i -E 'overflow|exhaust|deep'` crossed with *coroutine*, *context*, *stack*: none, the
   critic's), and 259's `t2-h-2` gave it as its reason (`blind/registration.md`'s label S). Does an order or a word
   tell a reader which abort is which, and which words each prints?
10. **The spec's real cost** (§ 3.7), where each sentence lives (`.claude/rules/spec-shape.md` § Where a rule lives),
    and **panel 261's text on the same two lines**, adopted and not landed (§ 3.1).
11. **The record**: panel 257's R2 promised `x1` *"lands in a later amendment when"* its conditions hold. **The
    conditions hold and x1 is false as written** (question 3): what would make a positive clause true, where, and is
    that amendment this sitting's (§ 3.6)?
12. **The routes nobody listed** (§ 4 (d1) to (d9)).
13. **What the landing owes. Widened by the repair** (the critic's C5, C11, C27; repair 5): the instrument and the net's
    own tests (`spec_refusals.hero:213-222` reads the real spec); the compiler's test of 2533; **the goldens' words
    where a claim moves: 91 `run` cases** (`grep -rlE 'seen to end|from inside' tests/golden/run --include='*.expected'
    | wc -l`, the repair at `87003cb42`, `ground/census.txt`; 90 at `c18a9a9a3`; **84 at the clerk's base**, where the
    clerk wrote 85), **and one `check` case
    that is no refusal and must not be rewritten**, `check/package-a-file-source-in-no-repository.case/main.expected`,
    whose *from inside* is *"read only from inside the program's own repository"* (a `grep -rl` over `tests/golden`
    counts it); the runtime's eight arms (`sigguard.c:598-644` at the base, 576-622 at the clerk's) where a claim moves; the four pins of the `spec` suite
    and the ledger row (`docs/measurements/010-spec-budget-ledger.md`, whose lines 259 and 260 are the rows of panels 257 and 259);
    panel 261's landing on the same lines; and **the site**: `.github/workflows/deploy-site.yml`'s `paths:` hold
    `spec/heroes-spec.md`, `tests/harness/suite_spec.hero`, `selfhost/**`, `runtime/**` and `docs/panel/**`
    (`sed -n 66,85p`, the repair), `site/src/lib/markdown.ts:1-10` serves *"the one markdown document this site serves
    verbatim: the specification"*, byte for byte, asserted at build time, and `site/src/lib/claims.ts:895-915`
    (`checkSpecLinesThreshold`, `:903`) holds the README's *"fewer than 600 lines"* (`README.md:12`) to the spec's line
    count: **the landing owes the site's build, and its push publishes the site** (CLAUDE.md § Hard stops: asked for,
    with the count of site commits), as does any lane's `runtime/**` repair. **Nothing in `editors/`**; in `rebel/`,
    nothing quotes the handler sentence, and `rebel/library/0.1.0/files/os.ledger:454` (*"No function of the program is
    a signal handler (spec § 13)"*) is Rebel's one reader-facing page about signals, which a text on *handler* must not
    contradict (§ 3.8).

## 3. The ground

### 3.1 The spec at the base (516 lines, 35,246 bytes, `wc -l -c spec/heroes-spec.md`; sha256 `3df14254998f68eb4a95ffe6f4c52b6772e2b0298a88ed82eee4b436fd12d93f`, `shasum -a 256`; the repair at `c18a9a9a3` and at `87003cb42`, the same bytes as the clerk's at `aded91388`)

The same bytes as panel 261's base: the spec's last change is step 197d (`770648d1b`, 2026-11-26 09:39:27; `git log -3
--format='%h %ci %s' -- spec/heroes-spec.md`, the repair, which also names 192c, `168d3d6c7`, and 126a, `598c0e562`, the
step that wrote the sentence). **The sentence** (`grep -n 'left a handler' spec/heroes-spec.md`: line 428), whole, 347
bytes (`ground/parts/handler-sentence.txt`, `wc -c`, the clerk's):

> A callback called after C left a handler of C's by a jump no function of the C library makes (written in assembly,
> say), by a `swapcontext` away from it, by a jump or a `setcontext` onto a stack once given to `makecontext`, or by a
> jump that lands within it, may abort, saying the handler was not seen to end, or that C called it from inside one.

**What stands beside it on line 427 and 428** (`sed -n 427,428p`): *"A callback is a **parameter**, never a result or a
signal handler, nor called by one; its parameters follow the same rule and `()` is `void`: `atexit(f: (function() ->
()))  -> i32`."*; then **the room's sentence** (*"Where C leaves a callback's frames without their returning (by a jump,
an exception or a coroutine it never resumes, say), ... past which it aborts, saying how many callbacks have not
returned."*, panel 259's R2); then **the handler sentence**; then *"No function of the program is a signal handler:
after `hero_net_catch_stop() -> i64` ..."*, the remedy, to the line's end. Vendored counts (`heroes-next measure` of
each piece cut into `ground/parts/`, the clerk's and the critic's, the same; **a lower bound, not the reader's
tokeniser**): the handler sentence **88** on both tables; the room's sentence 90 (claude-legacy) and 91 (cl100k_base);
lines 427 and 428 together 371 and 373.

**Panel 261's adopted text rewrites lines 427 and 428** (`docs/panel/261-reports/synthesis-adopted.diff`: line 427 gains
*"an `@` one the place C hands it and a `lent` one that place through a pointer to const ... aborts, naming the
callback"*) **and is not landed at the base** (`git log -1 87003cb42 -- spec`, the repair: 197d); **it landed after
it**, in other words, by step 203f (the head of this brief). Applied to a copy by `patch`
(`ground/spec-261.md`, 527 lines, sha256 `e9b0c819...758c`, the clerk's), **it keeps the handler sentence byte for
byte**, at line 439 (`grep -c` of the whole sentence: 1). So a candidate names the sentence by its words, never by its
line, and its diff is written against the spec `main` holds and checked to apply to the base's (`spec-warden.md`
item 8).

### 3.2 How the sentence came, and how it has been read (each read whole by the clerk at the lines named; the critic re-read each and found it as stated)

- **Panel 257** (sat 2026-11-23, ratified by the author's reading that day at about 17:00; issue 2703, closed): R1, the
  sentence, *"which says what may abort and promises nothing about what runs"* (`docs/panel/257-*.md:113-142`); its
  *may* rests on the two measurements of question 4 (`:56-62`, `:106-109`). **R2, the positive clause withheld**
  (`:143-151`), recorded as `x1` (`257-reports/spec-x1.txt`): *"A handler of C's is over where it returns, or where
  `longjmp`, `siglongjmp`, `_longjmp` or a `setcontext` to a context `getcontext` took carries the thread out of it onto
  the thread's own stack, and a callback then runs; after a handler left any other way (...) a callback called later on
  its thread may abort, saying the handler was not seen to end, or that C called it from inside one."* (+142 vendored,
  carried from `:148`); and `x2` (`spec-x2.txt`), the same promise in a shorter order. **x1 is false at the base as
  written** (question 3). The warden's `w26` (`spec-w7.txt`), which promised a run and named a depth, was not adopted,
  measured false on Darwin by `n6` and `fp3`. Step 126a (`598c0e562`, 2026-11-23 22:05) landed R1 with the runtime's
  words re-worded and the instrument `spec_refusals.hero`.
- **Panel 257's blind run** (2026-11-23, 31 sessions, 6.297764 USD; `docs/panel/257-*.md:244-331`): 15 of 15 abort
  sentences under `adopted` carry its words; **10 of 10 `adopted` and `rival` readers took *may abort* as *aborts***;
  its p7 on Linux's row the one cell readers got wrong (the landed Linux build then ran it); 12 of 15 blamed the
  library's C, 9 of them because `swapcontext` is not async-signal-safe (`:329-331`).
- **Panel 258** (decision 2758, **open**: ratified in its landed parts): **R1, the refusal of a jump out of a handler
  that interrupted the runtime's allocator** (`sigguard.c:175-212`, defect 2755, *"no spec sentence"*: the critic's C7),
  question 9.
- **Panel 259** (2026-11-25, ratified 2026-11-26): its R2, the room's sentence, placed *"before the handler sentence"*
  (`docs/panel/259-*.md:128-136`), line 428's tail from *No function of the program* kept byte-equal (`:141-142`). **Its
  blind run** (`259-reports/llm-ergonomist-blind.md`, 31 sessions, 10.393155 USD): the finding of question 1, and *"a
  question for a sitting on § 13's handler sentence, not a change of this resolution"* (synthesis `:303-306`). Its
  archived answers are what the repair tried this sitting's labels on (`blind/registration.md`).
- **Defect 2848's Origin** (question 1), and **2848's own title**, *"... and refusing them refuses a jump spec § 13
  promises"* (`gh issue view 2848 --json title`).

### 3.3 The property the runtime tests (read at the base `87003cb42`; every pointer by `grep -n` or `sed -n`, the repair where the move changed a line, the clerk's where `git diff` shows the file unmoved)

**Where it is asked**: the first line of every thunk, the one door from C into Heroes code
(`selfhost/emit/callback_thunk.hero`, 429 lines; `selfhost/emit/callback_guard.hero`, 207; `wc -l`, unmoved), calls
`hero_callback_entered` (`runtime/parts/sigguard.c:659` at the base, `:637` at the clerk's), which asks
`hero_callback_asked` (`:664`) and then counts the
callback in (`parts/callback.c`, **167 lines at the base, 111 at the clerk's: step 200a**, defect 2854, counts every
thread's callbacks for the room's abort, the critic's C1; it changed the count the room's abort reads, **not the guard's
questions**: `sigguard.c` unmoved). **In this order** (`sed -n 664,712p`):

1. **The exact test**: the thunk's return address is a delivery's (`_sigtramp`, `__kernel_rt_sigreturn`, `__restore_rt`,
   learned once): **DELIVERED**, *"ran as a signal handler"* (line 427's case, exempt from the instrument).
2. **The thread key**: a thread the runtime never claimed (C's own): **`hero_thread_refuse`** (`parts/thread.c:132-143`
   at the base), *"ran on a thread this program did not start: C called it back from a thread of its own, where this
   runtime's values are not safe"*, **whatever C did with signals**.
3. **The fourth question** (*"asked where a route stands"*, panel 251's R1, `sigguard.c:49-60`): *"is this thread inside
   a handler of C's that the runtime saw begin and has not seen end?"* Linux: the wrappers `parts/siginstall.c` runs
   around every handler a program's C installs, judged by `parts/sigentries.c`'s `hero_sig_inside` (**`:657` at the
   base**, `:472` at the clerk's); Darwin: the rewritten `sigaction`/`signal` pointers and the kernel's trampoline,
   `parts/rebind_entries.c`'s `hero_rebind_inside` (**`:630`**, `:453` at the clerk's). Its answers: **UNFOLLOWED,
   SWITCHED, REENTERED, ONSTACK, HELD, STANDING** (`sigguard.c:685-690`).
4. **ONSTACK by the kernel** (the thunk's frame off its thread's stack while `sigaltstack` says `SS_ONSTACK`) and **the
   walk** (`HERO_SIGNAL_WALK` 8 frame records, `:229`, `:701-710`): **CALLED**.

**Windows**: *"no probe and no walk (panel 247's R10)"*; `hero_callback_entered` counts and returns (`sigguard.c`, the
`#else` branch at `:714`, its comment `:716`).

**Beside the thunk's questions, two stops a handler's C meets elsewhere** (question 6): panel 258's jump refusal at the
jump hooks (`sigguard.c:175-212`, asked by `parts/sigjump_save.c` on Linux and `parts/rebind_jump.c` on Darwin after a
jump has left every handler the thread stood in), and guard D in the allocator (`alloc.c:670-678`).

**And the runtime's own handler** (question 2, the critic's C12): `stack.c:1269-1282` installs SIGSEGV and SIGBUS
handlers with `SA_ONSTACK` in every program before its first line, on an alternate stack per thread (`:1235`); rule 4's
*"the alternate stack registered now"* is the runtime's on every thread until C registers its own.

**How a handler ends, for the runtime** (`parts/sigentries.c:27-65` at the base, rules 4 to 7 and route A, 27-61 at
the clerk's; `parts/sigjump.c:27-58`, routes C and A, unmoved): by its return, or by a jump of the `setjmp` family whose
**target** (read at the jump: route C decodes the target's stack pointer from the buffer; route A trusts a validated
record) lies above the handler on the same range, or by a `setcontext` that is a jump; a `swapcontext` made while a
handler stands **suspends** it until the swap returns; a `setcontext` onto a coroutine's stack **holds** every standing
entry; a jump that lands on the alternate stack from outside it, **or, since step 199d, inside the frames of a handler
a jump left that the thread has not run in since** (defect 2855; until then *"below the jumping frame on its own
range"*), **re-enters** a handler's frames and stands as an entry of its own (rule 6, `:39-47`); and where contexts fall
to route A every range `makecontext` named is **held** from then on (rule 7, `:48-55`). **The life of a range**
(`sigentries.c:104-131` at the base, 103-126 at the clerk's): a range holds a coroutine's frames between a switch into
it and the end of its coroutine, and **since step 199c** (defect 2851) the context switched to is read wherever it is
mapped as a second witness; **a coroutine the C library starts through another's `uc_link`** (`:132-147`, **new since
step 199b**, defect 2850) is entered with its chain; **route A's held ranges** (`:149-155`, 127-133 at the clerk's);
**the frames of a handler left by a jump** (`:432-445`, **new since step 199d**): kept as a span until the thread is
seen running there again, a landing inside a span being that handler re-entered, *"shape R, a handler left by a jump and
jumped back into, from its own stack or another (panel 255's `rr_2`, a residue until this defect)"*. **`sigjump.c`'s
module doc still names `rr_2` as a residue** (`sigjump.c:78`, unmoved by step 199d), where `sigentries.c:439` says it
was one *until this defect*: a module doc behind its rule, read by the repair, not filed. **The residue the move
names** (`sigjump.c:75-77` at the base, re-worded by 199c): *"a coroutine left by such a switch that saves into another
context than the one it was switched to, leaving that one as it was, which is judged ended"*, filed apart as the
drafted defect `d-lane-sig-family-2` (§ 3.5).

**Which route stands where** (`sigjump.c:45-53`, *"ROUTE A, WHERE THE KEY IS NOT VERIFIED (glibc's four other
architectures, a glibc whose layout moved, a build forced to it by `HERO_JMP_FORCE_UNCALIBRATED`)"*): route C where the
key is verified; route A elsewhere; **and on Darwin x86-64, by construction** (`rebind_decode.c:314-315`, the critic's
C2), a platform no CI leg runs. Step 199a's body: *"the placement (`elsewhere`, `jump_away`) is gone, every
architecture now taking the path aarch64 takes forced onto route A"*. The simulation's switches are
`HERO_JMP_NO_DECODE` (`sigjump.c:130-132`) and `HERO_JMP_MAKE_IN_C` (`:134-145`; the clerk's *"130-141"* named one
range for the two). **What the guard says it does not see**: `sigguard.c:61-74` (a handler installed by a raw system
call or by a `sigaction` found by `dlvsym`, a frameless handler, a handler more than 8 records below, Windows) and
`sigjump.c:68-81` (a save found by walking a symbol table by hand, an inline `__builtin_longjmp`, assembly, a C++
exception or `_Unwind_*`, a coroutine left by such a switch saving into another context, and panel 255's residue
`rr_2`).

**The eight arms and their words** (`sigguard.c:598-644`; the count of `.expected` files holding each arm's own words,
`ground/census.sh`, `grep -rlF` over `tests/golden`, **the repair at `87003cb42`**, the clerk's count beside it where it
moved):

| arm | its first words after the function's name | which claim (`spec_refusals.hero`) | goldens |
|---|---|---|---|
| DELIVERED | *ran as a signal handler: the system called it inside a signal* | exempt | 3 |
| STANDING | *was called while a signal handler of C's that this runtime saw begin had not been seen to end: either C called it from inside that handler, or C left the handler in a way this runtime does not follow (...)* | both | 26 |
| SWITCHED | *... had switched to another context (...) and had not been seen to switch back or to end* | seen to end | 41 |
| REENTERED | *was called after C jumped into stack frames the thread had already left (...): the handler is held to be running again, and the function to be called from inside it* | from inside | **6** (5 at the clerk's: step 199d's `rr_2` case, refused) |
| ONSTACK | *was called after C, inside a signal handler, jumped (...) onto a stack a context switch had entered and this runtime has not seen end* | seen to end | **18** (12 at the clerk's: the six refused cases of steps 199b and 199c) |
| HELD | *... onto a stack `makecontext` was given, which this runtime holds may still be a coroutine's* | seen to end | **0** |
| UNFOLLOWED | *... jumped to a buffer this runtime holds no record of as it now stands (...)* | seen to end | **0** |
| CALLED (the default) | *was called from inside a signal handler: C called it while handling a signal* | from inside | **0** |

And beside them, no claim: the thread's words (2 goldens), panel 258's jump refusal (0), guard D (2, one per bracket).
**Every `.expected` holding either claim**: *seen to end* 85, *from inside* 33, either **92**: **91 `run` cases**
(STANDING 26 + SWITCHED 41 + REENTERED 6 + ONSTACK 18 = 91) **and the `check` case of question 13, which is no
refusal** (the clerk's 79, 32 and 85 counted it: 84 `run` cases then, the critic's C5; 85, 32 and 91 at `c18a9a9a3`,
`ground/census-c18a9a9a3.txt`). Every arm ends with
`HERO_SIGNAL_REMEDY` (`sigguard.c:110-115`), spec § 13's remedy byte for byte. On this Mac a callback a C handler calls
is told in STANDING's words, not CALLED's
(`fixedbugs-2494-a-heroes-function-a-c-handler-calls-is-refused.expected`, `head -c 330`, the clerk's): the fourth
question answers first.

**What touched the guard since panel 257** (`git show --numstat --format= <hash> -- runtime/parts/sig*
runtime/parts/rebind* runtime/parts/callback.c`; the critic's C1 and § 1 table corrected the clerk's): step 120d
(`d223a7917`, defect 2706: `sigentries.c` and `rebind_entries.c` **+56 −7** each, `rebind_jump.c` **+11 −1**,
`sigjump_save.c` **+10**; the clerk's *"+63"* were the changed lines); panel 259's landing, step 192a (`3f9fe858d`:
`sigentries.c` and `rebind_entries.c` −24 each, `sigguard.c` 8, `siglookup.c` 32, `callback.c` **+9 −177, net −168**
(the clerk's *"−186 net"* was the changed lines), `frame.c`, `lend.c`, `lend_aside.c`), 192b (`callback.c`,
`frame.c`), 192c the spec, 192d cases only; **panel 260's landing, steps 197a to 197h, touched no guard file and the
spec at line 401 alone**; step 199a (`65da4688c`, defect 2848): eight guard files, +154 −118; **and since the clerk's
base** step 200a (`callback.c`, the room's count), step 199b and step 199c (above).

### 3.4 What runs and what is refused today, run (the repair's stage 2 at `87003cb42`, this Mac, Darwin arm64, and its container on Linux arm64; the first pass at `c18a9a9a3` and the clerk's at `aded91388` beside it)

**Through the `run` form narrowed by its third word** (`heroes-next run tests/harness/main.hero -- heroes-next run
<substring>`, from the scratch tree's root; each case built plain and `--sanitize` and held to its `.expected`;
`ground/run-cited-repair.txt`, the repair's at the base):

| case (substring) | what C does | `.expected` | the form |
|---|---|---|---|
| `2522-a-callback-after-a-siglongjmp-out-of-a-handler-on-its-own` | a handler left by `siglongjmp` onto the thread's own stack, then a callback | runs (2) | 2 passed, 0 failed |
| `2522-a-correct-inline-jump` | a handler left by an inline jump no C library function makes | STANDING | 1 passed, 0 failed |
| `2702-a-handler-swapped-away-from-and-never-resumed` | `swapcontext` away from a handler, never resumed (6 forms) | SWITCHED | 6 passed, 0 failed |
| `2702-a-handler-left-by-setcontext` | `setcontext` out of a handler **on the coroutine's own stack** to its coroutine, and to the runner | runs (2) | 2 passed, 0 failed |
| `2702-a-handler-on-the-alternate-stack-left-by-setcontext` | the same with the handler on the alternate stack | SWITCHED | 1 passed, 0 failed |
| `fixedbugs-2705` | a handler's `siglongjmp` into a live coroutine on the own stack | ONSTACK | 1 passed, 0 failed |
| `fixedbugs-2706` | a `siglongjmp` out of a handler onto frames reusing memory once given to `makecontext` | runs (2) | 2 passed, 0 failed |
| `fixedbugs-2707` | a handler on an alternate stack left by `siglongjmp` **into a coroutine** (runs, 2) or by `setcontext` into one (refused, 2) | 2 run, 2 refused | 4 passed, 0 failed |
| `fixedbugs-2848` | 199a's six: `setcontext` into made contexts (refused, 4), twelve arguments (runs), `getcontext`/`setcontext` as a jump on the own stack then a jump out of a handler (runs) | 4 refused, 2 run | 6 passed, 0 failed |
| `2494-a-heroes-function-a-c-handler-calls-on-a-thread` | a C handler on a thread C made calls the function | the thread's words | 1 passed, 0 failed |
| `2494-a-heroes-function-a-c-handler-calls-is-refused` | a C handler calls the function | STANDING on this Mac | 1 passed, 0 failed |
| `2702-a-handler-jumps-into-a-coroutine-on-the-own-stack` | a handler on the own stack jumps into a coroutine | refused (ONSTACK) | 1 passed, 0 failed |

**Every one of the twelve narrowed runs read `0 failed` at the base, on both platforms** (`grep -c '0 failed'`: 24, a
suite line and a harness line each, every count line `diff`-equal to the clerk's `ground/run-cited.txt`): this Mac,
`ground/run-cited-repair.txt`, 20:54 to 21:15; **Linux arm64**, the repair's container (`ground/linux-arm64/L-probes.txt`),
the same 24. The first pass read the same on this Mac at `c18a9a9a3` (`run-cited-repair-c18a9a9a3.txt`) and in its
container (`linux-arm64-c18a9a9a3/`), and the critic at `aded91388` on both (its § 4). **Beside them, at the base**:
step 199d's three cases (`fixedbugs-2855`: a coroutine on a carved stack leaving for good by a `longjmp` to its host
runs, and so after a handler was left by a jump; panel 255's `rr_2`, a handler left by a jump and jumped back into from
a coroutine on the heap, refused in REENTERED's words) read `3 passed, 0 failed` on this Mac (`ground/2855-repair.txt`)
and in the container (`linux-arm64/L-2855.txt`); the route accessor's case `fixedbugs-2679-the-runtime-says-which-route`
`1 passed, 0 failed` in the container (`L-2679.txt`, route 3 under Debian's glibc 2.41).

**Two programs that install no signal handler, refused in the sentence's words at the clerk's base, and one of them
still at this one** (`ground/probes.sh`, built from panel 259's evidence, `docs/panel/259-reports/evidence.tar.xz`;
`grep -nE 'sigaction|signal\(|raise\(|kill\(|SIG[A-Z]'` of their headers and sources: nothing, the clerk's), each
three runs of three, identical: **the repair at `87003cb42`**, this Mac (`ground/probe/out-*-repair-*.txt`) and Linux
arm64 (`ground/linux-arm64/L-probes/`); the first pass at `c18a9a9a3` read the clerk's lines and exits, all 18 outputs
equal after `key.sh`'s normalisation (`ground/probe/out-*-c18a9a9a3-*.txt`, `diff`):

| program | plain | `--sanitize` |
|---|---|---|
| `escape_carved` (a coroutine of a header's own assembly switch, on a stack carved from the host's frame, leaves for good by `longjmp` to its host; then a callback) | **at the base: `escaped: -1 then 42`, exit 0**, both platforms (step 199d, defect 2855); at `aded91388` and `c18a9a9a3`: exit 134, REENTERED: *"escapecarved.twice was called after C jumped into stack frames the thread had already left (a handler left by a jump and jumped back into, ...)"* | the same, both platforms |
| `escape_heap` (the same on `malloc`'s stacks) | `escaped: -1 then 42`, `carved: 42`, exit 0 | the same lines, exit 0, ASan's *"ignoring requested `__asan_handle_no_return`"* warning before them |
| `mj_carved` (two fibers made by `makecontext` on quarters of an array in a C frame, entered by `setcontext`, then `_longjmp`) | its four right lines, exit 0, both platforms | **this Mac: exit 134, CALLED**: *"mjcarved.run was called from inside a signal handler: C called it while handling a signal"* (defect 2852, open, unrepaired at the base); **Linux arm64: its lines, exit 0** (the critic's at `aded91388`, the repair's at `c18a9a9a3` and at the base) |

**The blind programs, the key** (`blind/registration.md` § The key: the repair at the base, this Mac and Linux arm64,
every cell three runs of three, every Linux cell this Mac's; at `c18a9a9a3` **the clerk's 72 outputs reproduced byte for
byte** after `key.sh`'s normalisation, and at the base every cell the same but `n3`'s): of t1's
programs, `h1` (`siglongjmp` out), `h3` (the handler returns), `h7` (a `siglongjmp` onto memory a returned `makecontext`
task once used), **`h9` (a handler left by assembly on another thread), `h10` (2707's run), `h11` (2702's run) and
`h13` (a signal the program counts)** run; `h2` (an assembly jump out), `h4` (the handler calls the function) and `h6` (a
jump within the handler) abort in STANDING's words, `h5` (`swapcontext` away) in SWITCHED's, `h8` (a `siglongjmp` into a
live `makecontext` coroutine) in ONSTACK's, **and `h12` (a `siglongjmp` into a live coroutine of an assembly switch,
none of `now`'s five exits) in STANDING's**. Of t2's four with **no handler**: `n1` (a `makecontext` coroutine) and `n2`
(an assembly coroutine leaving by `longjmp` on heap stacks) **run**, **and so does `n3`** (the same on carved stacks;
REENTERED's words at `aded91388` and `c18a9a9a3`, defect 2855, repaired by step 199d); `n4` (a callback on a thread C
made) aborts in the thread's words.

**The `run` goldens whose name holds `coroutine`** (`ground/census.sh`, the repair at `87003cb42`: 92, 89 at
`c18a9a9a3`; each read by
whether a header it names, **followed through every `#include "..."`**, installs a handler, or its `.hero` calls
`hero_signal_watch` or `hero_net_catch_stop`: the critic's `census2.sh` rule, which the repair's census now runs):
**33 install none: 31 run, 2 abort, both by the room's bound** (the runtime's words, *"the room large values take off the
stack is full, 1 GiB for the whole program, while 2047 callbacks C called have not returned"* and *"... 2046 ..."*;
**the spec's sentence says only *"saying how many callbacks have not returned"***: the clerk called the runtime's words
*"panel 259's sentence"*, the critic's item 9), **none in the handler sentence's words**; 59 install one: 16 run, 43
refused (14 and 42 at `c18a9a9a3`, 12 and 36 at the clerk's base). And the converse, asked of **every** `run` golden
holding a claim (91): **all 91 install a handler** in their header chain (`census.txt`'s last line, *"installing none: 0"*). So in the tree's goldens
the handler sentence's aborts are reached only by programs with a handler; the open defect above (2852, under
`--sanitize` on Darwin) is the exception, and is no golden.

### 3.5 The guard's defects, the drafted ones and the lane (`gh issue view <n>` and `git log`, the repair, 15:48 to 16:00 and again 20:47)

| defect | what happens (its title, shortened) | class | where | the lane's cluster | **at the base `87003cb42`** |
|---|---|---|---|---|---|
| **2831** (the critic's C9) | on Linux a `makecontext` reached through `dlsym` is glibc's, so a correct program with no handler is refused in a signal handler's words | blocking | `siglookup.c`, `rebind_image.c` (`git show --stat a4bdf4257`) | (before the lane) | **closed** 2026-11-26T12:37:48Z, repaired by step 185a (`a4bdf4257`) |
| **2848** | where `makecontext` is not wrapped, a coroutine started by `setcontext` and a handler's `setcontext` into a made context run the function inside the signal, *"and refusing them refuses a jump spec § 13 promises"* | blocking | `sigjump_save.c`, `sigjump_record.c` | S | **repaired by step 199a** (`65da4688c`, merged at `30af0a33a`); open until the batch's gate |
| **2850** | a coroutine the C library starts through another's `uc_link` is never seen entered, so a handler's jump into it, on a carved stack, runs the function inside the signal | blocking | `sigentries.c`, `rebind_entries.c` | E (E1) | **repaired by step 199b** (`3e02951e0`, merged at `612776983`); open until the gate |
| **2851** | a heap context yielding by a switch the runtime never sees is judged ended, so a handler's jump into it runs the function inside the signal | blocking | `sigentries.c`, `rebind_entries.c`, `sigjump_record.c`, `rebind_jump.c` | E (E2) | **repaired by step 199c** (`590ab2503`, merged at `3e2c5d8e8`); open until the gate |
| **2852** | Darwin's `makecontext` keeps `getcontext`'s frame pointer, the walk climbs into gone frames and, **under `--sanitize`, refuses a correct program in a signal handler's words** (no handler) | blocking | `sigguard.c` | G | **open, not repaired** (§ 3.4's `mj_carved`) |
| **2853** | on Darwin under `--sanitize` the runtime's own `setcontext` frame stays poisoned, and ASan reports a correct program | blocking | `rebind_jump.c` | J | **open, not repaired** |
| **2855** | a coroutine on a carved stack leaving for good by `longjmp` to its host **is refused at the next callback in the words of a signal handler the program does not have** | blocking | `sigentries.c` (rule 6), Darwin's `hero_rebind_reenter` | E (E3) | **repaired by step 199d** (`77fade16c`, merged at `c54db505f`); open until the gate (§ 3.4's `escape_carved` and the key's `n3` run) |
| **`d-lane-sig-family-1`** (drafted, unfiled; the critic's C8) | **on route A** a handler's `setcontext` to a context kept on the heap is never trusted: `fixedbugs-2702-a-handler-left-by-setcontext-to-the-runner-runs` (`b_0`) and `...-to-its-coroutine-runs` (`cs_0`), **correct programs, exit 134 in UNFOLLOWED's words**, and ten rows that must be refused are refused in those words where route C says SWITCHED or REENTERED | blocking (its draft) | `sigjump_record.c`, `sigguard.c`, `rebind_record.c`, `rebind_jump.c` | filed apart by 199a's body (*"13 rows keep route A's own UNFOLLOWED"*, recounted by the lane the same day as 12, `coord/issues/d-lane-ctx-after-1.md:118`) | **unfiled** (`.claude/worktrees/rebel-scratch/coord/issues/d-lane-sig-family-1.md`, 14:11, `github: none`; no issue by its title, `gh issue list --search`, the repair). Its draft: *"Route A stands on glibc's riscv64, loongarch64 and 32-bit arm, on Darwin x86-64 and in a forced build; no CI leg runs it"* |
| **`d-lane-sig-family-2`** (drafted, unfiled; found by the repair) | a coroutine yielding by a switch the runtime never sees **into another context than the one it was switched to** is judged ended, so a handler's jump into it on a carved stack **runs the function inside the signal** (Linux arm64, plain, before and after 2851's repair; refused under `--sanitize` in ONSTACK's words; this Mac unrun) | blocking (its draft) | `sigentries.c`, `rebind_entries.c` (the life of a range) | beside 2851 | **unfiled** (`coord/issues/d-lane-sig-family-2.md`, 15:46, `github: none`) |

**What each means for a sentence**: 2852 is **a program with no handler told the sentence's words** (§ 3.4): until it
lands, a text promising such a program a run is false on this Mac under `--sanitize`, and a reader who reads the
sentence as covering every switch predicts the build; **2831 and 2855 are the same class, repaired** (three members,
three mechanisms, one day: the base rate behind (a) and (d3)). `d-lane-sig-family-2` (and 2850 and 2851 until today) are **a `may abort` that
does not abort**: the function runs inside the signal (panel 251's veto), so a text promising an abort for a jump into
a coroutine is false there until it lands. `d-lane-sig-family-1` makes **route A refuse correct programs** a run-promise
would promise. 2853 is no refusal of the runtime's; 2848's, 2850's, 2851's and 2855's repairs are in.

**The lane, asked** (`git log --oneline main..lane-sig-family`, the repair at 15:48 and at 20:47: nothing; the lane's
head at 20:47 is `ee2aef6d9`, an ancestor of `main`: `git merge-base --is-ancestor`): its brief
(`.claude/worktrees/rebel-scratch/coord/sig-family-brief.md`, 13:02, the coordinator's) orders **S, then E, then G,
then J**; S became step 199a, E1 step 199b, E2 step 199c, **E3 step 199d** (`77fade16c`, 16:18); **G (2852) and J (2853)
are not landed** (`git log --oneline aded91388..87003cb42 -- runtime`: steps 198d, 199b, 199c, 199d and 200a and their
merges, none of 2852 or 2853; `gh issue view`: both open). Its design notes at 13:18, read by the clerk (`progress.md:5`, before the rule that keeps every seat out
of the lane's scratch; *carried* by the repair, which read nothing of the lane's): *"E3 escape = re-entry only into
frames of a handler left by a jump; G = clear Darwin fiber fp at makecontext; J = __asan_handle_no_return before
setcontext"*. **E3, landed as 199d, made rule 6's re-entry need a handler left by a jump; G would make the walk's**:
the two that decide whether *"no program without a signal handler of C's is ever refused by the fourth question or
the walk"* is true, the first now in. A seat asks the lane's state again (`git log --oneline 87003cb42..main -- runtime`)
before writing a sentence that rests on them, runs the second column the head of this brief names, and never reads
inside the lane's worktree.

Beside them, open and not the guard's: **2854** (blocking, the room's count across threads: **repaired by step 200a**,
`2978e6aed`, merged at `51b9be1ca`, open until the gate; the critic's C1) and 2849, 2861 (adjacent, the runtime not
compiling on glibc's ppc64le, i386 and gcc older than 15; `gh issue view`, open).

### 3.6 The record: who weighed this before (`grep -l -i` over `docs/panel/*.md`: *seen to end* 255, 257, 259; *handler of C* 257; *may abort* 257, 259; *swapcontext* 251, 255, 257, 259; *inside a signal* 251, 253, 255; *signal handler* 076, 156, 172, 173, 247, 248, 249, 251, 253, 254, 255, 257, 258, 259, 261; `gh issue list --state all --search` *handler sentence*, *may abort*, *seen to end*, *swapcontext callback*, *coroutine signal handler callback*, *not promised handler*: the clerk's; the entries marked *(the critic)* added by the critic's first reading and re-read by the repair)

- **Panels 246 and 247** (the guard: *no Heroes code runs inside a signal handler*, R4; R1, R2, R9, R10; defect 2494),
  as `sigguard.c:10-11` cites them.
- **Panel 249** (a jump within a handler told from a jump out; its file holds *"from inside one"*, as does panel 172's critic report, `grep -l`).
- **Panel 251** (decision 2590, ratified 2026-11-19): R1, the fourth question, a jump judged by a validated record.
- **Panel 255** (decision 2701): R1 (a jump reads only its own target, route C), R3 (Darwin's rules on Linux), R4 (a
  context switched inside a handler, the author's ruling of SWITCHED).
- **Panel 257** (decision 2703): the sentence (R1), the positive clause withheld (R2), § 3.2.
- **Panel 258** (decision 2758, **open**: ratified in its landed parts, its healing half superseded by 259, the author's
  *4a* of 14:07, `agents-live.md:419`): **R1, the jump refusal** (`sigguard.c:175-212`; the critic's C7).
- **Panel 259** (decision 2812): the room's sentence (R2), HELD stands (R3); its blind run's finding (question 1).
- **Defects** 2494, 2522, 2533 (the remedy's copy), 2679, 2702, 2705 (*"The words a refusal prints name causes the
  program does not have"*), 2706, 2707, 2745, 2755 (the jump refusal's), **2831** *(the critic)*, and those of § 3.5.
- **design.md** names neither `swapcontext`, `makecontext`, *seen to end* nor *handler of C's* (`grep -n -i`: nothing,
  the clerk's and the critic's; design.md did not move), and its *signal* lines (2762, 2767, 3156, 3626, 3645) are about
  `raise` as a binding and leases, none about the guard. **But it speaks on coroutines** *(the critic, C10)*:
  `docs/design.md:3628` (Part 6, the refusals: *"| Coroutines | require stack switching (`ucontext` or stack copying) —
  a step change in complexity |"*), `:3597` (*"`Coroutines` ... are refused on **cost alone**"*) and `:4170-4176`
  (*"Threads, not green threads, not coroutines ... Coroutines are already refused permanently in Part 6"*). Coroutines
  are refused **in the language**; a sentence promising what a **C** coroutine library is told stands beside them, and
  the spec already says *"a coroutine it never resumes"* (the room's sentence). So **the guard's rules live in the
  runtime's module docs and the sittings' records**, and whether a text owes design.md a line is the warden's question.
- **The spec's own conditional aborts** *(the critic, C6)*: question 4.

### 3.7 The spec's count, at the base (the repair's stage 2 at `87003cb42`, `sources 6ce3f9e116d22e3a`, `heroes-next measure spec/heroes-spec.md` from the scratch tree's root, no `--refresh`, 20:52; `ground/measure-spec-repair.txt`)

claude-legacy **10,067**, cl100k_base **10,245**, **real 13,439** (`claude-opus-5`, the pin of 2026-11-26), *"Headroom:
2945 against the 16384 ceiling — but the FFI floor mortgages 60 of it (panel 030 R3), so what is measured against the
ceiling is 13499"*: **byte-equal to the clerk's at `aded91388`** (`diff`, trailing blanks and the repair's `exit` line
aside; `ground/measure-spec.txt`), the spec not having moved. **The seed's compiler no longer prints `STALE`**: at
`aded91388` and `c18a9a9a3` it did (its pin `4dced6767460e590` against the file's `24de425e6c28e69b`, the clerk's and
the critic's), and the seed refreshed by step 207a reads the same counts and `real 13439` (`./heroes measure
spec/heroes-spec.md`, the repair, 20:52, `ground/measure-seed-repair.txt`); `selfhost/measure/pinned.hero:55` and `:64`
hold `13439` and `"24de425e6c28e69b"` (the clerk's and the critic's; the file did not move). **The `spec` suite at the
base**: **`spec: 36 passed, 0 failed`** (`heroes-next run tests/harness/main.hero -- heroes-next spec`, the repair,
21:16, `ground/suite-spec-repair.txt`; the same at `c18a9a9a3` and at the clerk's `aded91388`, `ground/suite-spec*.txt`),
`spec/refusal-words` among them. **A candidate's real count needs `--refresh`**, one request to the count-tokens endpoint:
`spec-warden.md` says when.

### 3.8 Who quotes the sentence (the clerk's `grep -rlF` of *left a handler*, *seen to end*, *from inside one*, *handler of C*; **corrected by the critic's C11**, and **re-run by the repair** as `git grep -l -F <words> -- . ':!tests/golden' ':!docs/panel'`, every tracked file, with *may abort* added: at `c18a9a9a3` and again at `87003cb42`, the same lists)

| words | files (the repair's `git grep`) |
|---|---|
| *left a handler* | `spec/heroes-spec.md`, `tests/harness/spec_refusals.hero`, `docs/measurements/010-spec-budget-ledger.md` |
| *seen to end* | the same three, `runtime/parts/sigguard.c`, and `rebel/library/0.1.0/files/process.hero` and `process.ledger` (*"the child was not seen to end"*, a process's, unrelated) |
| *from inside one* | the spec, the instrument, the ledger |
| *handler of C* | the spec, the instrument, the ledger, `runtime/parts/sigguard.c`, `thread.c`, `siglookup.c`, `runtime/runtime.c`, `runtime/heroes_runtime.h` |
| *may abort* | the spec, the instrument, the ledger, and two unrelated comments (`runtime/runtime.c:158`, *"everything below may abort"*; `selfhost/ir/in_place.hero:117`, *"an operator may abort"*) |

**The clerk's *"Nothing in `rebel/`, `site/`, `editors/` or `docs/` outside `docs/panel/`"* was false by its own command**
(the critic): the ledger's rows for panels 257 and 259 (`docs/measurements/010-spec-budget-ledger.md:259-260`, a file
the landing writes anyway) and Rebel's `process` files, which are unrelated. Wider words (the critic's, re-run by the
repair): `rebel/library/0.1.0/files/os.ledger:454` quotes the remedy (*"No function of the program is a signal handler
(spec § 13)"*; `git grep -n -F` over `rebel`: that one line), Rebel's one reader-facing page about signals;
`term.ledger:137` speaks of *"the layer's `atexit` and signal handlers"* (the critic's); `hero_net_catch_stop` is
called by `examples/server/net.hero`, `rebel/library/0.1.0/files/net/hero_net.hero` and `os/hero_stop.hero` (`git grep
-l`). **So: nothing quotes the handler sentence outside `spec/`, the runtime, the instrument and the ledger**; and the
site serves the spec verbatim (question 13). Inside `docs/panel/`, the diffs of panels 259 and 261 that touch line 428
(each carries the whole line, the clerk's).

## 4. The candidates, and what each must meet

Each is built and measured by the seats, none assumed; **the texts are the warden's** (`spec-warden.md`); the
compiler-engineer builds what a text promises as a program and runs it; the ffi-pragmatist runs real C under it; the
blind seat measures what a reader predicts from it.

- **(a) Two sentences, by kind**: what a **signal handler** of C's does to a callback (the fourth question, the walk,
  the exact test), apart from what a **coroutine switch** with no handler does (today: nothing of the guard's; by
  defect, 2852 at the base, and 2855 until step 199d), each the promise it is. It must say what it promises a
  coroutine library with no handler, and be true of 2852 at the moment it lands, or wait for the lane's G; and say what *"no handler"* means
  for a process whose runtime installs its own (question 2).
- **(b) The property**: the sentence states what the runtime tests (*a callback called while a signal handler of C's
  that began on its thread is not known to have ended, or from inside one*) rather than C's mechanisms, so that a
  mechanism not listed is neither read as promised away (259's reading) nor as promised (2848's). It must say what
  *known to have ended* is for a reader: which exits end a handler for every build (a return; `longjmp`, `siglongjmp`,
  `_longjmp` above it on the same stack, per `sigentries.c`'s rules) **and on which route** (route A's
  `d-lane-sig-family-1`), and what happens at panel 258's jump refusal.
- **(c) The sentence as it stands, plus one clause**: e.g. *"a switch with no handler of C's running is none of
  these"*, or *signal* before *handler*, or *"on its thread"*. **`x1`'s positive clause may be offered only as held to
  its falsifiers** (question 3: 2705, the own-stack coroutine, the jump refusal, route A, the thread): the critic's C4.
  It must keep `SENTENCE_OPENS` or move it first.
- **(d) The routes nobody listed**: name one, and say what would have to be true for it to exist. Some the clerk and
  the critic did not build or price, **each a question**: (d1) **no list at all**: the remedy's place says what a
  callback is promised (*it runs unless a signal handler of C's is running on its thread, or C left one in a way the
  runtime does not follow*) and the words are the runtime's; (d2) **a promise per platform** (`x1` with its route-A
  scope), **which must say what it promises an Intel Mac** (route A by construction) **and musl or FreeBSD** (systems
  the runtime was never built on, § 5); (d3) **a change to the runtime so the sentence can be simpler** (e.g. after E3
  and G, *a program whose C installs no signal handler is never told these words*, which needs the thread rule said
  apart, or a stricter runtime that refuses every exit it does not follow, so *may* becomes *aborts*); (d4) **a told
  end** (a `hero_os.h` call by which C says a handler is over): panel 259 refused a told switch as *"a `hero_os.h`
  promise for ever"* that bought nothing under E (`docs/panel/259-*.md:200-205`), so its precedent cuts against; (d5)
  **the words alone**, no sentence, as before panel 257 (its *"What conservative would have been"*, R2's note and
  `:180-184`). **Added from the critic's C26**: (d6) **merge, not append**: one sentence on how C leaving a callback's
  frames or a handler costs the program, joining the room's sentence (which already says *"by a jump, an exception or
  a coroutine it never resumes"*) and the handler sentence (`.claude/rules/spec-shape.md`'s *"merging beats
  appending"*); true if the two classes can be said once each inside one sentence; (d7) **line 427's home**: extend
  *"never a result or a signal handler, nor called by one"* with its consequence (*"... nor called while one is running
  on its thread, which C's leaving by a way the runtime does not follow leaves running for good: such a call aborts,
  saying ..."*) and drop the list; true if the instrument reads the claim there and line 427's `atexit` example
  survives; (d8) **abort at the exit, not at the next callback**, as panel 258 already does for a jump out of the
  allocator: for the exits the runtime **sees** (a `swapcontext` away, a `setcontext` onto a coroutine's stack, a jump
  into a live coroutine), stop the program there, so the sentence says *aborts* with no *may* and no *later*; true only
  where the exit is seen, and it refuses programs that never call a callback after (`h5`'s twin with no callback): a
  widening of refusals the warden prices and the engineer counts; (d9) **a told route**: the spec says *may abort where
  this runtime cannot follow C's exits, which `hero_signal_jump_route()` and `heroes doctor` name on a machine*; the
  accessor exists (`fixedbugs-2679-the-runtime-says-which-route-decides-a-jump`, `route 3`); true if `doctor` prints
  it (**unread**) and the spec may name a machine-dependent fact through a command, which § 13's *"platform layer"*
  sentence does not yet do.

**Every candidate must meet**: each row of § 3.4 on every platform the text claims, and the falsifiers of question 3;
the guard's defects at the state the landing will meet them in (§ 3.5), the drafted two included; the instrument and
the net's own tests (question 6); the placement before the remedy; panel 261's text on the same lines; the blind
readings (a text that fixes one misreading by inviting another is not robust: D1, D2 and D5 of the registration); the
shapes of question 8 and § 5; and what the landing owes outside the spec (question 13).

## 5. The shapes beside, which no row of § 3.4 holds (each unbuilt by the clerk and the repair; each a seat's)

- **A coroutine library with no handler**, on every stack kind: `malloc`, `mmap` with a guard page, carved from the
  host's frame (2855's), copied in and out (greenlet's and Tencent libco's designs, panel 259's `cpo_*`), Boost.Context
  1.92 (on this Mac: `/opt/homebrew/Cellar/boost/1.92.0_1`, `boost/version.hpp` `"1_92"`), OpenSSL 3.5.9's `ASYNC`
  (`/opt/homebrew/Cellar/openssl@3/3.5.9`; its libcrypto imports `__longjmp __setjmp _getcontext _makecontext
  _setcontext`, `nm -u`; `/opt/homebrew/lib/libcrypto.dylib` links to `openssl@4/4.1.0`, the critic's C21).
- **A handler that switches** to a coroutine and back, never back, onto the alternate stack, nested in another handler.
- **The signal stack trick** (question 8), **in four variants** (the critic's C20): the handler **returns** after its
  save or **leaves by a jump**; the library **restores** the previous alternate stack after the creation or **leaves
  its own**. Every plain-built thread already has the runtime's alternate stack (§ 3.3), which a library's
  `sigaltstack` replaces: what § 9's *"running out of stack aborts"* then does is unrun.
- **A watchdog** (question 9, the critic's C25): a `SIGALRM` handler of C's that `siglongjmp`s out of whatever it
  interrupted, the interrupted code a Heroes loop that allocates: a rate, not a verdict, many runs, both platforms
  (the compiler-engineer's); a real library on it, libcurl's synchronous resolver timeout (`alarm` and `siglongjmp`
  around `getaddrinfo`, as the critic recalls it, **unverified**; curl 8.14.1 with its headers is in the Linux arm64
  image, the critic's § 4, and none under `/opt/homebrew/Cellar`), the ffi-pragmatist's. *"POSIX leaves everything after
  such a jump undefined"* (`sigguard.c:184-185`, panel 258's reading), so it is no *correct C* in the sense the blind
  tasks use.
- **A handler installed by a raw system call that calls the function, on Linux x86-64 with a restorer of its own**
  (the critic's C25): the delivery's address learned is glibc's `__restore_rt`, the restorer the program's, so neither
  the exact test nor the walk should find it and guard D alone stands (**an inference** from `sigguard.c:29-37` and
  `:61-66`, unrun; 2522's raw-system-call `run` cases are allocator-and-lend shapes). A text that says *aborts* for a
  call from inside a handler is false there. The compiler-engineer's, the x86-64 image (recorded, not graded).
- **A handler left by `_Unwind_ForcedUnwind` from C** (the residue *"a C++ exception or `_Unwind_*`"*, STANDING's
  words); on Darwin `_Unwind_*` lives in libSystem, so *"a jump no function of the C library makes"* is itself
  ambiguous there. The compiler-engineer's.
- **POSIX's own callbacks on threads the C library makes**: `timer_create` with `SIGEV_THREAD`, `mq_notify`, glibc's
  `getaddrinfo_a`: a binding author who never wrote `pthread_create` meets the thread's refusal. The ffi-pragmatist's.
- **A callback on a thread C made** (no handler; the thread's words), on a thread `hero_thread_spawn` made, and **a
  handler left on one thread with the callback on another** (question 1's T, run: `h9`).
- **§ 9's abort on a coroutine's stack** (question 9, the critic's C28): a callback recursing past a `malloc` stack and
  past an `mmap` stack with a guard page, both platforms. The compiler-engineer's.
- **Systems a reader calls Linux that the runtime was never built on**: musl (no `makecontext` in libc; `libucontext`),
  FreeBSD (*"every POSIX system but Darwin"*, `sigjump.c:14`): what a run-promise or *may* means there is unrun; (d2)
  must say. The warden's, as a question.
- **Route A** (`HERO_JMP_FORCE_UNCALIBRATED`, the simulation of 199a's body, **Darwin x86-64**), **`--sanitize`**,
  **Linux arm64** (`heroes-linux-arm64:latest`, image id `338a1044ac90`, `docker image ls`, the repair), **x86-64**
  (`heroes-linux:latest`, `6f78598a4eba`, emulated), **Windows** (the box, `ssh win`, which the author turned on at
  14:06 by `agents-live.md:419`; no seat is asked to use it without the coordinator's word).

## 6. What each seat runs, and where

- **Its copy**: `.claude/worktrees/262-<seat>` (made by the coordinator, detached at the base `87003cb42`). **Build
  there**: `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`, then `./heroes build selfhost/main.hero -o
  heroes-next`, and judge with `heroes-next` (`sources 6ce3f9e116d22e3a`; `.claude/rules/verification.md` § The compiler
  that judges is a build artifact; the seed at the base reads the spec's pin, § 3.7). A runtime a seat edits is built as
  `HEROES_RUNTIME=<its copy of runtime/>`, never in place.
- **The second column** (the head of this brief; the critic's C1): where `git log --oneline 87003cb42..main -- runtime`
  is not empty when a seat runs a decisive row, the row runs on `main`'s runtime too, `HEROES_RUNTIME` pointing at a
  `git archive main runtime` copy inside the seat's own folder, and the report names `main`'s hash.
- **A route that does not build is not adopted** (§ 3c): a clause that promises a run is run; a clause that promises an
  abort is run to its abort; on this Mac and in the Linux arm64 image, plain and `--sanitize`, three runs each.
- **A program that may not end** is built with `heroes build` and its binary run under `timeout`, its output bounded
  (`.claude/rules/verification.md` § A run that may not end); **one container at a time**, `--rm`, named
  `p262-<seat>`, removed when idle, **no `docker pull`**. **The Linux arm64 image has no `/usr/bin/time`** (the
  critic's § 0; `ls -l /usr/bin/time` there, the repair: *No such file or directory*, `ground/linux-arm64/L-libs.txt`): a script run there times nothing.
- **Reports** go to `docs/panel/262-reports/<seat>.md` in the seat's copy, written as it goes, and are copied to the
  trunk's same path by the coordinator; programs a seat keeps go to `docs/panel/262-reports/programs/<seat>/`.

## 7. The ground (`ls ground/`)

Every file the clerk wrote stands under its own name; **the repair's at the base carry `repair` in their names, its
first pass's at `c18a9a9a3` carry `c18a9a9a3`**. `build.log` (the clerk's seed and stage 2 at `aded91388`),
`build-repair-c18a9a9a3.log` and **`build-repair.log`** (the repair's two); `measure-spec.txt` (the clerk's),
**`measure-spec-repair.txt`** and **`measure-seed-repair.txt`** (§ 3.7: stage 2's and the refreshed seed's at the base);
`suite-spec.txt` (the clerk's `spec` suite), `suite-spec-repair-c18a9a9a3.txt` and **`suite-spec-repair.txt`**;
`parts/` (the sentence, the room's sentence and lines 427 to 428, cut out and measured, § 3.1, the clerk's);
`spec-261.md` (the spec with panel 261's adopted diff applied, § 3.1, the clerk's); `census.sh` (amended by the repair,
its head says how) and **`census.txt`** (its output at the base), `census-c18a9a9a3.txt` and `census-aded91388.txt` (the
same script over the first pass's base and the clerk's, so the three compare line by line); `probes.sh` (the probes and
the narrowed `run` cases, `sh ground/probes.sh <compiler> <out>` from a copy's root) with `run-cited.txt` (the clerk's
run), `run-cited-repair-c18a9a9a3.txt` and **`run-cited-repair.txt`**, and **`2855-repair.txt`** (step 199d's three
cases at the base); `probe/` (panel 259's `escape_carved.hero`, `escape_heap.hero`, `escape.h`, `own.h`,
`mj_carved.hero`, `mj.h`, and every run's output, `out-<program>-<mode>-<n>.txt` the clerk's,
`out-<program>-<mode>-c18a9a9a3-<n>.txt` and **`out-<program>-<mode>-repair-<n>.txt`** the repair's; `escape_heap.hero`
is 259's bytes but one blank line inside its group, taken out by `heroes-next fmt --in-place` so a commit's hook takes
it); **`linux-arm64/`** (the repair's container at the base: its environment, its builds' keys, the key, the probes and
the narrowed `run` cases, the route case, step 199d's cases, the critic's `same.hero`, and `L-libs.txt`, what the image
holds of the ffi-pragmatist's libraries) and `linux-arm64-c18a9a9a3/` (the first pass's, with the critic's `watched.hero`
and `same.hero`). **`blind/`**: the registration (`registration.md`), the two tasks, the session brief, `programs/` (35
files), `key.sh`, `key-new.sh`, the key on this Mac (`key-mac/`, the clerk's 72 at `aded91388`;
`key-mac-repair-c18a9a9a3/`, the first pass's 102; **`key-mac-repair/`**, the repair's 102 at the base), `labels-259/`
(the labels tried on panel 259's archived answers), every input's hash (`inputs.sha256`, `arms.sha256`).
