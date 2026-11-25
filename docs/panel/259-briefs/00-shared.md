# Panel 259, the shared brief: what may the runtime assume about a stack switch it cannot see?

Written 2026-11-25 by the sitting's clerk, the first command at 06:12 and the last before 06:40 by this Mac's
clock (`date`), at the first base `2cf85ea64`. **Repaired** from 07:35 by the repair of the briefs, from the
completeness critic's first reading (`docs/panel/259-reports/critic-briefs.md`, F1 to F25, its last section's items 1
to 9), and **re-frozen at the base of § 2**: every line number, count and quotation below was re-run at that base by
the repair, and every run cell names the base it was run on. Every number, path and count was produced by a command
run while this brief was written, and the command is named beside it; a fact taken from a document without running it
says **carried** and from where. Read this file whole, then your own brief, then the critic's first reading. **A brief
is the one document nobody was assigned to check**: the critic read it before any seat starts
(`.claude/skills/panel/SKILL.md` § 3c), and every seat re-runs what bears on its verdict and says first which fact
here was wrong.

## 1. The proposal, in ten lines

1. Panel 258's R1 (provisional, issue 2758) heals the large-value room (`frame.c`) and the lend regions (`lend.c`) by
   frame ADDRESS: a take, a lend or a thunk's entry gives back every entry of a lower frame on the thread's own stack.
2. Step 181a (`f767e0d4e`) sends a frame on another stack aside, judged there only where the runtime can name the
   stack (a `makecontext` range, Windows' thread block); a frame inside the thread's measured stack and outside every
   range it knows is **the thread's own, to every question it asks**.
3. A C library's own switch onto memory carved from the thread's stack makes that order lie: a coroutine's frames lie
   above a live function's, the heal gives its room away; `own_carved.hero` prints 11 for 7 then exits 134 on this
   Mac, and dies by SIGSEGV with nothing printed in the Linux arm64 image (at this base) and in the x86-64 one (the
   critic's, at `2cf85ea64`, emulated; § 5).
4. **The question**: what may the runtime assume about a switch it cannot see, and what is a program told or promised
   when C makes one? The routes (§ 6) are questions; a spec sentence may be owed (drafts `arms/`, counts the warden's).
5. **An item before the sitting** (the coordinator, 08:10): lane d2745's step 179a (`b0b2a975a`, 2026-11-25 05:40, in
   both bases, `git merge-base --is-ancestor`) added an arm to `hero_signal_refuse` (`runtime/parts/sigguard.c:576`),
   **`HERO_SIGNAL_HELD`** (`:504`; its words at `:607-612`, chosen at `:669` from `hero_sig_inside`'s
   `HERO_SIG_HELD`): *"... was called after C, inside a signal handler, jumped (`longjmp`, `siglongjmp` or `_longjmp`)
   onto a stack `makecontext` was given, which this runtime holds may still be a coroutine's, since on this system it
   cannot read where a context switch lands or see a coroutine end, so whether the jump left the handler is not
   known: a handler not seen to end is held to be running ..."*. Where contexts fall to route A, every stack
   `makecontext` is given is held a coroutine's, and a held stack **no switch was seen to enter** is refused in these
   words (its comment, `:557-564`). The same step moved the pin of `tests/harness/spec_refusals.hero` whose test is
   titled *"... and the arms are the ones the sitting named"* (`:213`; the sitting is panel 257, the file's head at
   `:1-2`), its comment adding *"and HELD, added by defect 2745"* (`:210-212`) and its kinds now `DELIVERED STANDING
   SWITCHED REENTERED ONSTACK HELD UNFOLLOWED default` (`:221`). The file counts **eight arms** (`:46`, *"has eight
   arms"*): seven `how == HERO_SIGNAL_...` arms and the default (`grep -n 'how == HERO_SIGNAL'
   runtime/parts/sigguard.c`); the coordinator's message called HELD the ninth. **It is a refusal about a stack switch
   the runtime did not see enter, so this sitting ratifies or refuses that arm and its words** (`git show b0b2a975a --
   runtime/parts/sigguard.c tests/harness/spec_refusals.hero`).

## 2. The tree, the sitting and the seats

- **The base**: `3ec775b664c9cf0d6b8bae8c94f407f8a934e3a2`, `main`'s head at 07:35:42 (`date`), when the repair re-froze
  the clerk's worktree by `git -C .claude/worktrees/panel259 checkout --detach main` (`git rev-parse HEAD` then):
  *"M-rebel-library: merge lane-layer-speed, what the platform layer's members reach is asked of one clang run, not one
  run a member"* (`git log --oneline -1`). Step 181a is in it (`git merge-base --is-ancestor f767e0d4e HEAD` succeeded).
  The worktree's only change is two untracked folders (`git status --short` prints `?? docs/panel/259-briefs/` and `??
  docs/panel/259-reports/`).
- **The first base, and what moved since it**: the clerk wrote these briefs, and the critic read them first, at
  `2cf85ea64`; ten commits lie between (`git log --oneline 2cf85ea64..3ec775b66 | wc -l`). `git diff --name-only
  2cf85ea64 3ec775b66 -- spec runtime seed selfhost tests` lists 115 files: 7 of `runtime/` (`heroes_guard_open.h` and
  `heroes_guard_close.h` +31 each, `heroes_reserved_open.h` and `heroes_reserved_close.h` +4 each, `heroes_runtime.h`
  +86 −2, `parts/failure.c` +3 −3, `parts/mail.c` +6 −6, `git diff --numstat`), 49 of `selfhost/` (41 of `emit/`, 7 of
  `cli/`, 1 of `fetch/`), 59 of `tests/` (40 of `emission/`, 19 of `golden/`), none of `spec/` or `seed/`; beyond those
  trees, `.claude/rules/generated-c.md` and `editors/lldb/heroes_lldb.py`. **Of the files these briefs cite, two moved**
  (`git diff --quiet 2cf85ea64 3ec775b66 -- <file>`, over each): `runtime/heroes_runtime.h`, where a route-D entry
  point would be declared, and `selfhost/emit/large_value.hero`, the compiler-engineer's question 9 file (one line,
  `out.push("NULL")` became `out.push("((void *)0)")`, `git diff`); and `.claude/rules/generated-c.md`, which the
  compiler-engineer's brief names. Every `runtime/parts/` file of § 4, `runtime/hero_os.h`,
  `selfhost/emit/callback_thunk.hero`, `selfhost/main.hero`, `seed/heroes.c`, the spec, `docs/design.md`,
  `.claude/rules/spec-shape.md` and the two golden coroutine headers of § 4.7 did not, so their line numbers are the
  same at both bases; each was re-run at this one.
- **Frozen**: from the moment the briefs go out until the synthesis, nothing under `spec/`, `runtime/`, `selfhost/`
  or `tests/` moves in any seat's tree. `main` moves under other lanes. **The lane `dlsym-make` is repairing now the
  doors of § 4.2** (the `RTLD_NEXT` and C-library-handle answers for `makecontext`; the coordinator's brief to the
  repair, 07:40; its worktree is `.claude/worktrees/lane-dlsym-make`, whose head read `8608fa9a2` at 07:45, `git log
  --oneline -1`): it will move `unseen_carved`'s and `ph`'s Linux rows. **So every Linux cell, in a brief or a report,
  names the base it was run on**: *at 3ec775b66*, *at 2cf85ea64*, or a lane's hash.
- **The lane is the full panel** (`.claude/skills/panel/SKILL.md` § Two lanes: *"a question that looks internal but
  has a sentence in the spec behind it is a full panel"*): `compiler-engineer`, `ffi-pragmatist`, `spec-warden`,
  `historian`, the completeness critic (first on these briefs, done; then on the reports), and the `llm-ergonomist`,
  **registered and NOT RUN** (`llm-ergonomist.md`, `blind/`): no paid run is authorised for this sitting, so the
  resolution is provisional on that reading and says so.
- **This folder** (`ls -R docs/panel/259-briefs`): this file; `compiler-engineer.md`, `ffi-pragmatist.md`,
  `spec-warden.md`, `historian.md`, `llm-ergonomist.md`, `critic.md`; `arms/` (three draft sentences, each `.txt` and
  `.diff`); `blind/` (two tasks, seven programs and three headers under `programs/`, `answers.md`, `registration.md`,
  and `design-c/`, the key's header for t2's design C, never a reader's); `shapes/` (the lane's 21 reproducers, the
  critic's `below.h` and `below.hero`, and `cost/`, the lane's three cost programs with `cost.h` and `cost.sh`,
  repointed at a seat's own copy); `critic/` (what the critic's first reading cites, copied from
  `.claude/worktrees/259-critic-scratch/`, which is swept when idle: `trace/`, the critic's five `fprintf`s into the
  runtime as a diff (`runtime-trace.diff`, made by `diff -ru` of its traced copy against its tree), its script and its
  two outputs; `probes/`, the `dlsym` probes `dlh` and `dlh2`; `issue-search.txt`, its searches of the issues); `runs/`,
  the repair's outputs at this base, one file a cell, text only.
- **Each seat's copy**: `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/259-<seat>`, a detached worktree of
  the base `3ec775b66` made by the coordinator, never outside the repository. **Build your own compiler there from the
  seed, then stage 2** (§ 3). Never use the trunk's `./heroes`, the clerk's, the repair's or another seat's, and never
  read, build or run inside another seat's directory. This folder is untracked and lives in the coordinator's worktree
  `.claude/worktrees/panel259` alone: read it there, and copy into your own copy what you build from it.
- **Your report**: `docs/panel/259-reports/<seat>.md` of your own copy, **written as you go**, copied to the same
  path of the tree the coordinator's prompt names when you finish or stop, so a killed seat leaves what it had. It
  ends with the verdict structure of your agent file (`.claude/agents/<seat>.md` of your copy), then one line per
  numbered question of your brief: the answer, and the one measurement that would change it. **Say what you ran and
  what you read, apart.**

## 3. Rules for every seat

- **The compilers**, built by the repair at this base in `.claude/worktrees/panel259-repair-scratch/tree/` (a `git
  archive` of `seed runtime selfhost spec vendor` and the root files): `clang -I runtime seed/heroes.c
  runtime/runtime.c -o heroes` read `real 27.59`, `user 21.85`, `sys 0.80`, and `./heroes build selfhost/main.hero -o
  heroes-next` read `real 448.47`, `user 247.09`, `sys 62.35` (`/usr/bin/time -p`, ending by 07:44, `uptime` read load
  averages 124.43 to 134.46 at 07:44 and 07:45: `real` far above `user` plus `sys`, a run that waited): indicative,
  never a number to quote. **The seed is older than the tree**: `./heroes --version` prints `sources
  05186f2b38e77a97`, `./heroes-next --version` prints `sources 117362877b136f67` (at `2cf85ea64` stage 2 printed
  `sources 32437b681fe95f82`: the sources moved, § 2). **Measure with stage 2**: panel 258 recorded that stage 1 emits
  no `hero_callback_left()` (carried, `docs/panel/258-briefs/00-shared.md`).
- **The machine**: shared with other lanes; a duration is indicative and enters no verdict unless `real` is close
  to `user` plus `sys`, and the clock rules of CLAUDE.md § Verification hold. It holds itself awake: `pmset -g
  assertions` read `PreventUserIdleSystemSleep 1` at 07:45, held by Amphetamine (`Amphetamine (Single-Use - System)`).
- **A program that may not end** is built with `heroes build` and its BINARY run under `timeout`, both streams
  bounded (`2>&1 | head -c 100000 > o.txt`, the exit from `PIPESTATUS`), never `timeout heroes run`; after a timeout
  look for the binary (`pgrep -f <its name>`) and stop it by `kill <pid>`, never `pkill`
  (`.claude/rules/verification.md` § A run that may not end).
- **Linux** is two images here (`docker image inspect --format '{{.Os}}/{{.Architecture}}'`):
  `heroes-linux-arm64:latest` (arm64, the CI's `ubuntu-26.04-arm` architecture) and `heroes-linux:latest`
  (`linux/amd64`, emulated on this Mac: run it **without** `--platform linux/amd64`, which made `docker run` look for
  the image in a registry, refused, exit 125, where the plain `docker run --pull never` finds it; the repair's runs,
  07:43). A Linux compiler is built inside the container from the seed and stage 2; a Mac binary does not run there.
  Name every container `p259-<seat>-<n>` and `docker rm -f` it when done; **no `docker pull`, no download**. Neither
  image holds Boost (§ 4.7). **Windows** is not this sitting's: the box is paid by the hour
  (`.claude/rules/platforms.md` § The Windows box); a seat that finds a Windows run worth its cost says so in its
  report, with the run, and the coordinator decides.
- **No paid run**: no `claude -p` session, no API call, no cloud run, no download. The spec-warden's count requests
  are its brief's question and run only if the coordinator's prompt authorises them.
- **No credential printed**: not `gh auth token`, not `.env`'s content; `gh auth status` masks.
- **Nothing outward**: no commit, push, merge or GitHub write (`gh` reads only), no file outside the repository's
  root, no `git add -A`, `.` or `-u`, no `git stash`. The coordinator commits the sitting by pathspec.
- **Every number in your report is a command's, with the command beside it**; a negative sentence (*the runtime
  cannot*, *no library*) is run or goes out as a question naming what was searched.

## 4. The ground, each fact by its command

### 4.1 Panel 258's R1: the order that heals

`wc -l` at the base: `runtime/parts/frame.c` 270, `lend.c` 1,148, `callback.c` 277, `stack_lane.c` 165,
`frame_aside.c` 153, `lend_aside.c` 214, `sigjump_record.c` 271, `sigentries.c` 448, `rebind_entries.c` 430.
Every line number below is `grep -n` of the signature, the heading or the line quoted, at the base.

- **The test of order** is `hero_frame_is_below(deeper, frame)` (`callback.c:213`): true where `deeper < frame`,
  `deeper` at or above the thread's measured low end and `frame` below its high end. **An address, and nothing
  else.**
- **Who asks it** (`grep -n 'hero_frame_is_below\|hero_callback_heal(' runtime/parts/*.c` prints `frame.c:182` and
  `:237`, `lend.c:440` and `:472`, `callback.c:213` and `:257`, `sigguard.c:641` and `:707`):
  - the room's heal, `hero_frame_heal_below` (`frame.c:181`, its test at `:182`), called by every take
    (`frame.c:199`);
  - the room's give, `hero_frame_give` (`frame.c:224`), which gives back the takes above a give where every one is of
    a frame below the giver (its test at `:237`), **else stops** with `hero_frame_out_of_order` (`frame.c:152`,
    *"internal error: a large value's room was given back out of order (compiler bug)"*);
  - the regions' heal, `hero_lend_heal_inside` (`lend.c:439`, its test at `:440`), called by every lend's push
    (`hero_lend_push`, `lend.c:524`, the call at `:526`) and, through `hero_lend_heal_below` (`lend.c:449`, which
    brackets it and calls it at `:452`), by the thunk's entry;
  - the regions' give, `hero_lend_heal_to` (`lend.c:462`, its test at `:472`), called at a buffer's and a local's
    give (`lend.c:662`, `:970`), which **returns silently** where an entry above is not below the giver (`if
    (!hero_frame_is_below(...)) return;`, `sed -n 462,480p`): **the two gives do not fail alike**, the room's stops
    with *compiler bug* and the regions' says nothing, which bears on what a seat expects a lend shape to print
    (critic, F1);
  - the thunk's entry, `hero_callback_heal` (`callback.c:257`), called by both definitions of
    `hero_callback_entered` (`sigguard.c:638` and `:703`, the calls at `:641` and `:707`), which gives back **every
    entry at or below the thunk's own frame** (`callback.c:259`, `above = frame + 1`; the room at `:262` and the
    regions at `:263`, `sed -n 257,263p`).
- **What R1 rests on**, in the room's own head (`frame.c:30`, *BUT A C JUMP LEAVES TAKES NOBODY GIVES BACK*; `:41`):
  *"the giver is the deepest frame alive"*. On one stack that is true; the defect is the one place it is not.
- **Its standing**: `gh issue view 2758 --repo heroes-lang/heroes` reads OPEN, `kind:decision`, *"Panel 258: A C
  jump over a callback heals the runtime's stacks and tells its gates now ..."*; its *Recommendation* (body line 17):
  *"ratify R1 and build it, which the tag waits for"*. R1 is built on `main` and unratified.

### 4.2 Step 181a: which stack a frame is on

- `git show --stat f767e0d4e`: 36 files, `stack_lane.c` +165, `frame_aside.c` +153, `lend_aside.c` +214. The lane's
  report is `.claude/worktrees/rebel-scratch/coord/coroutine-room-report.md` (49 lines, `wc -l`).
- **The test** (`hero_stack_lane`, `stack_lane.c:135`, inlined into each take; the slow half `hero_stack_lane_asked`,
  `stack_lane.c:88`): POSIX, the thread's measured range less every range `makecontext` was given that overlaps it;
  Windows, the thread block's stack (`stack_lane.c:89-100`). A thread keeps at most `HERO_LANE_CARVED` 4 carved
  ranges of its own (`callback.c:70`) and asks the process's table past that; the process keeps `HERO_SIG_STACKS` 256
  ranges on Linux (`sigentries.c:91`) and `HERO_REBIND_STACKS` 256 on Darwin (`rebind_entries.c:31`).
- **Off the thread's own stack** a take is a block of its own (`hero_frame_aside_take`, `frame_aside.c:108`), healed
  among one tag's blocks (`hero_frame_aside_heal`, `frame_aside.c:98`), and **a block of a stack nobody can name, tag
  0, is never judged gone** (`frame_aside.c:99`, `if (stack == 0) return;`). That is, as the clerk reads it (not
  traced), why the library's own switch onto the heap's stacks runs (§ 5, `own_heap`).
- **What it says it does not see** (`stack_lane.c:51` to 55, whole): *"a switch onto a stack carved from the
  thread's own that no `makecontext` this runtime wraps named (a library's own switch, a call of the C library's
  `makecontext` through a pointer), or one whose range a later 256 others pushed out of the table, is the thread's
  own stack to every question here, and R1's healing may give back a live entry there."*
- **The pointer `dlsym` gives: on Linux the door is the HANDLE** (critic, F2 and F12; re-run by the repair at this
  base, `runs/mac/out-dlh.txt`, `runs/linux-arm64/out-dlh.txt` and `out-dlh2.txt`):
  - **Darwin** answers `dlsym` for every name its table rewrites, `_makecontext` among them
    (`runtime/parts/rebind_image.c:40`, and its comment at `:84-96`, *"`dlsym` ANSWERED FOR THE NAMES ABOVE"*: *"every
    name the table rewrites is answered this file's function ... wherever the C library would have answered one at
    all"*). On this Mac the critic's probe `critic/probes/dlh.hero` (four answers, each compared with the program's own
    symbol: thousands `RTLD_DEFAULT` `makecontext`, hundreds `RTLD_NEXT` `makecontext`, tens `RTLD_DEFAULT`
    `swapcontext`, units `RTLD_NEXT` `swapcontext`, 1 where the answer is the program's own) prints `state 1111` (the
    repair, at this base, `runs/mac/out-dlh.txt`; the critic's the same at `2cf85ea64`); the C library's own handle is
    not probed on Darwin (`dlh2` opens `libc.so.6`, a Linux name). `ph.hero` prints `state 11` (`ph.h`'s
    `cr_seen_state`: `dlsym` was asked, 10, plus the pointer equals the program's own `makecontext`, 1), and
    `unseen_carved.hero` runs right (§ 5).
  - **Linux**: the table `dlsym` is answered from, `runtime/parts/siglookup.c:123-143` (`table[] = {` at 123, twenty
    rows, `{"sigaction", ...}` at 124 to `{"swapcontext", ...}` at 143), answers `getcontext`, `setcontext`,
    `swapcontext` and the jump and signal names, and `grep -n makecontext runtime/parts/siglookup.c` prints nothing.
    **But the runtime defines `makecontext` as a global symbol of the executable itself**
    (`runtime/parts/sigjump_make.c:38-55` for AArch64 and `:57-77` for x86-64, `.globl makecontext`), so the answer
    depends on the handle the program passes: `dlsym(RTLD_DEFAULT, "makecontext")` is answered with **the runtime's**
    (the executable exports it: `nm -D bin-dlh` prints `T makecontext`, and `T swapcontext`, the repair's arm64 run at
    this base), and `dlsym(RTLD_NEXT, "makecontext")` from the executable, which skips the executable's own definitions,
    and the C library's own handle (`dlopen("libc.so.6", RTLD_NOW | RTLD_NOLOAD)`, `critic/probes/dlh2.hero`) answer
    **the C library's**; both handles answer the program's own `swapcontext`. In the arm64 image `dlh` prints `state
    1011` and `dlh2` `state 1` (the repair, at 3ec775b66; the critic read `state 1011` and `state 1` at 2cf85ea64). **In
    the x86-64 image the probes are unrun** by the critic and by the repair: that cell is the ffi-pragmatist's, who
    holds the image. `shapes/cr.h`'s `cr_unseen` asks `RTLD_NEXT` on Linux and `RTLD_DEFAULT` on Darwin (`cr.h:35-39`,
    its `#if defined(__APPLE__)`), so on Linux it gets the C library's `makecontext`, which names no range, and the
    runtime's `swapcontext`, which sees the switch (§ 5's trace).
  - So **the unseen `makecontext` on Linux is the one asked by `RTLD_NEXT` or by the C library's handle, never by
    `RTLD_DEFAULT`**; the first base's *"the table has no `makecontext` row"* is true and not the whole reason, and
    every seat's table of doors holds **the three handles**. The lane's report says *"calling `makecontext` through
    `dlsym` is not an unseen switch on either platform (the runtime answers `dlsym` for that name too)"* (carried):
    **false on Linux for two of the three handles**. The `dlsym-make` lane is repairing those two doors now (§ 2), so
    `unseen_carved`'s and `ph`'s Linux rows name their base.
  - Whether a pointer reached another way is unseen (`dlvsym`, which `sigjump.c:69-70` says is not answered; a pointer
    taken inside an image of Darwin's dyld shared cache, which `rebind.c:109-110` says is not rewritten) is **a
    question, unrun**.

### 4.3 What the runtime already lists as unseen

- **Linux**, `runtime/parts/sigjump.c:68-77` (*WHAT IT DOES NOT REACH*): a save reached by `dlvsym`; *"a handler left
  by a switch or a jump this file never sees (an inline `__builtin_longjmp`, hand-written assembly, a C++ exception
  or `_Unwind_*`)"*; and panel 255's residue `rr_2`.
- **Darwin**, `runtime/parts/rebind.c:109-118`: an installer or a jump reached inside the shared cache, a raw
  `__sigaction`, an interposer's own installs, a C++ exception out of a handler, and two save-and-resume shapes (the
  second panel 255's `rr_2`).
- **Windows**: no jump hook (panel 258's sitting, *"What a veto would compel"*: *"every jump is seen is false here
  (assembly), on Linux (`sigjump.c:66-74`) and on Windows (no hook)"*, carried); a fiber's switch changes the thread
  block, which `stack_lane.c:89-100` reads; an own assembly switch does not change it (read, **unrun**: there is no
  Windows machine in this sitting).
- **Panel 258's door table, read** (`docs/panel/258-reports/ffi-pragmatist.md`, Darwin's table at lines 51 to 67, Linux
  arm64's at 81 to 96; the repair read both, **a reading of the table, not a count**): the doors the hook does not see
  are 8 (*"the abandonment is not an event"*: a coroutine swapped away and never resumed, its two swaps seen), 9
  (`pthread_exit` in a callback), 10 (`pthread_cancel` of itself), 11 (`exit(0)`, no jump), 12 (a `SIGUSR1` handler
  returning with its register file set to a `getcontext` point) and 13 (a C++ exception through the frames). **Three
  abandon frames unseen with the process running on** (8, 12, 13: exit 0, `sum 21`), **two end the thread with no
  event** (9 and 10: on the main thread the process exits 0 printing nothing; in a `spawn` thread, `wait` reads through
  a null pointer, exit 134, defect 2754), and **11 is `exit`**. **Door 6**, `__builtin_longjmp`, *"cannot be written"*
  on arm64 (its line 58: clang refuses `__builtin_setjmp` for `arm64-apple-macos` and `aarch64-linux-gnu`; run again by
  the repair, 07:44: Apple clang 21.0.0 prints *"`__builtin_setjmp` is not supported for the current target"* for this
  Mac's default target and for `--target=aarch64-linux-gnu`, and writes an object for `--target=x86_64-linux-gnu`), and
  *"on x86-64 it can never be seen"* (its line 86, an `objdump` reading); panel 258 lists it **unrun** on x86-64 (its
  line 407). The defect draft's *"3 of 13 doors with no event"* is a reading of this table, not its count. **Route B (§
  6) is priced on these**: healing only on a seen jump loses 8, 12 and 13, and door 6 on x86-64, a cell nobody has run.

### 4.4 What the specification says today

- The file: 516 lines, 34,437 bytes (`wc -l`, `wc -c`), sha256
  `c915f0f2d47f35c6b3162d198c27bedfbb3fe2b962e05185a878619356372bee` (`shasum -a 256`), the same bytes at both bases (§
  2).
- `grep -n -i -E 'coroutine|makecontext|swapcontext|fiber|room|large value|4,096|4096' spec/heroes-spec.md` prints
  **line 428 alone**, whose sentence is panel 257's: *"A callback called after C left a handler of C's by a jump no
  function of the C library makes (written in assembly, say), by a `swapcontext` away from it, by a jump or a
  `setcontext` onto a stack once given to `makecontext`, or by a jump that lands within it, may abort, saying the
  handler was not seen to end, or that C called it from inside one."* So the document speaks of switches the
  runtime does not follow **out of a handler**, and the searched words name **no value's room across a switch,
  no coroutine and no threshold of a large value** (the 4,096 bytes are the emitter's, `frame.c:13-15`). The critic
  searched twelve more words (`large`, `threshold`, `KiB`, `MiB`, `frame`, `context`, `setjmp`, `longjmp`, `jump`,
  `callback`, `across`, `keeps`, `grep -n -i`, one word each); the repair ran them again at this base: the hits are
  lines 11, 79, 200, 277, 280, 284, 326, 340, 376, 383, 427, 428, 488 and 490, and the repair read each: none speaks
  of a value held across a switch or a jump, nor of a threshold.
- Line 331: *"running out of stack aborts"*; lines 465 to 466: *"A lend lives for its call and no longer"*; lines 473
  to 474: *"a lease nobody ends, like a handle nobody consumes or a thread nobody waits for, aborts when `main`
  returns"* (`grep -n`, `sed -n 464,475p`). `grep -o -i 'this runtime\|the runtime' spec/heroes-spec.md | sort | uniq
  -c` prints `1 the runtime`.
- **Where a sentence can go**: the compiler's own test *"every signal refusal ends with spec section 13's own
  sentence, byte for byte (defect 2533)"* (`selfhost/emit/callback_thunk.hero:408`) holds the text from `No function
  of the program is a signal handler` to the end of line 428 equal to `HERO_SIGNAL_REMEDY`
  (`runtime/parts/sigguard.c:110`). A sentence placed after that phrase on line 428 would be printed at the end of
  every signal refusal; the drafts in `arms/` go **before** it, as panel 257's did. Elsewhere (§ 9 beside line 331,
  § 13 beside lines 465 to 466) is the warden's to price.
- **What design.md says of coroutines** (`grep -n -i coroutine docs/design.md`): Part 6's row *"Coroutines | require
  stack switching (`ucontext` or stack copying) — a step change in complexity"* (line 3624), refused *"on cost
  alone"* (line 3593); Part 7, *"Threads, not green threads, not coroutines ... do not build a scheduler"* (lines
  4166 to 4174). The language has no coroutine; **every one here is a C library's, and a Heroes function runs on its
  stack only as a callback**. §1.12 (its heading at line 642, the sentence at 649 to 651): *"A Heroes program must
  not segfault and must not corrupt memory"*, and the boundary is *complete*: *"any C library must be bindable"*.

### 4.5 The budget, before

`./heroes-next measure spec/heroes-spec.md` (the repair's stage 2 at this base, from its tree's root, 07:46):
claude-legacy **9,877**, cl100k_base **10,054** (the vendored maximum), **real 13,185** (`claude-opus-5`, the pin of
2026-11-23, *"the binding number"*), headroom **3,199** against the **16,384** ceiling, the FFI floor mortgaging 60
(13,245 measured against the ceiling); the clerk's stage 2 read the same at `2cf85ea64`. No `--refresh` was run. The
rule that prices a change: the most robust solution wins, a new form and the spec's size included, and the ceiling is
raised to fit (CLAUDE.md § Precedence, amended by the author 2026-11-22): **a cost measured and written, never a
veto**.

### 4.6 The issues (`gh issue view` and `gh issue list --search`, read only; states read again by the repair at 07:39)

- **2758** OPEN, panel 258's ratification (above). **2756** OPEN, `class:adjacent`, the lend region a jump leaves.
  **2745** OPEN, `class:blocking`, *"With the setjmp family forced onto route A, panel 255's four own-stack A''
  programs run the Heroes function inside the signal on Linux ..."*. **2702** and **2707** CLOSED (a context switched
  inside a handler; a jump between two stacks the program made, refused on Linux by which stack lies lower).
- **Panel 258's family**, OPEN, `class:blocking`: **2752** (*"A C jump over a Heroes callback that holds values past 4
  KiB leaves the large-value room out of order, and the program ends at exit with a false 'compiler bug' abort"*:
  the defect R1 repairs), **2753** (a C handle told falsely *"never given back"* after a jump), **2754** (a `spawn`
  thread that C ends with `pthread_exit`: `wait` reads through a null pointer). None was in this list at the first
  base; § 6 E names 2752.
- **The class of § 5's Linux words**: **2755** OPEN `blocking` (*"A C signal handler that `siglongjmp`s out of a Heroes
  callback ends the program with a message accusing a Heroes signal handler that does not exist"*) and **2705** CLOSED
  `blocking` (*"The words a refusal prints name causes the program does not have, and one program is told K on Linux
  and U on Darwin"*). § 5's Linux finding, `unseen_carved` told about a handler it does not have, is 2705's class
  recurring, on a program with no handler at all; 2705's body names panel 257's `sched` (*"a green-thread scheduler
  that switches back"*, `gh issue view 2705 --json body`), a shape of this sitting's kind already written.
- **2811** OPEN `blocking` (`area:harness`): *"Forty-two run cases added since the CI's last Windows leg bind POSIX
  headers the Windows target lacks ..."*; it bears on `own.h`'s missing Windows stop (§ 4.7) and on step 181b's
  `unbuildable_on_windows`.
- **The two defects this sitting answers are drafts, unfiled, with no number** (`ls -la
  .claude/worktrees/rebel-scratch/coord/issues/` lists both at 07:39):
  `.claude/worktrees/rebel-scratch/coord/issues/d-lane-coroutine-room-1.md` (**systemic**: *"A coroutine library's
  own switch onto a stack carved from the thread's own lets panel 258's healing hand a live value's room to
  another"*) and `d-lane-coroutine-room-2.md` (**adjacent**: a coroutine left for good on a fresh stack each time
  keeps its large values, and after enough of them the room's bound says *stack exhausted*). Read both whole: the
  first names three repairs as *"a ruling no rule reaches"*. Its filing should name 2705 and 2755 as the class of its
  Linux words.
- **Two drafts lane d2745 found beside defect 2745's repair**, unfiled, with no number, now in lane ctx-after's hands
  (the coordinator, 08:10; `.claude/worktrees/lane-ctx-after` stands, `ls -d`), both written 04:13 (`ls -la`), both
  `class: blocking` (`grep -o 'class: [a-z]*'`): `.claude/worktrees/rebel-scratch/coord/issues/d-lane-d2745-1.md`
  (*"A `swapcontext` reads the context it switched to after the switch has returned, so a correct program whose
  coroutine unmaps its own context before ending through `uc_link` dies of SIGSEGV inside the runtime"*) and
  `d-lane-d2745-2.md` (*"On glibc's architectures other than aarch64 and x86-64 no `makecontext` wrapper learns a
  coroutine's stack, so a coroutine on an array of the thread's own stack is judged as the thread's own frames and a
  handler's jump into it lets the program's function run inside the signal"*). The second is this sitting's carved
  shape on the architectures where `sigjump_make.c` defines no `makecontext` (its guard at `:25`, AArch64 and x86-64
  only; § 4.2).
- **Searched**: the clerk's five words, `coroutine`, `stack switch`, `makecontext`, `unseen switch`, `fiber` (`gh issue
  list --state all --search '<word> in:title,body' --limit 15`), and the critic's seventeen more, twenty-two in all
  (`critic/issue-search.txt`, written by 06:43, its `ls -la` time): `fiber`, `carved`, `fcontext`, `jump_fcontext`,
  `switch_fiber`, `stack_lane`, `pthread_attr_setstack` and `Boost` in titles and bodies return **no issue about a
  switch** (`Boost` returns 2159 alone, about panel 226). So *"nothing in the issues names a switch the runtime cannot
  see onto the thread's own stack"* holds **for issue titles and bodies under those words**, and is **false of the
  record**: panel 257's evidence holds a green-thread scheduler, OpenSSL's ASYNC fibers, Boost.Context and a carved
  `makecontext` stack (§ 4.7).

### 4.7 What is at hand on this Mac, and in the images

- **A real library's own switch**: Boost 1.92.0_1 (`brew list --versions boost`), with `libboost_context`,
  `libboost_coroutine` and `libboost_fiber`, each `.a` and `.dylib` (`ls /opt/homebrew/opt/boost/lib`), and
  `include/boost/context/detail/fcontext.hpp` present (`ls`). **Boost ships here no assembly source**: `ls
  /opt/homebrew/opt/boost` prints `INSTALL_RECEIPT.json README.md include lib sbom.spdx.json share`, and `find
  /opt/homebrew -iname 'jump_*'` prints nothing (the critic's wider `find / -iname 'jump_x86_64_ms_pe*'` found nothing
  either, carried). So `make_fcontext` and `jump_fcontext` are read here only as the libraries' symbols, never as
  source.
- **Boost on Linux: absent in both images** (the repair, 07:43, `docker run --rm --pull never` of each): `ls -d
  /usr/include/boost` and `ls /usr/lib/*/libboost_context*` find nothing, `dpkg -l | grep -i -c boost` prints 0, and
  `find / -xdev -iname '*boost*'` finds only CMake's `FindBoost` module and libstdc++'s `boost_concept_check.h`.
  Installing it is a download, which the sitting forbids: **every Boost cell is this Mac's alone**.
- **Panel 257's real-library evidence** (`tar -tJf docs/panel/257-reports/evidence.tar.xz`, extract into an empty
  folder of your own copy): its ffi-pragmatist's Boost binding (`programs/ffi-pragmatist/bctx.h`, `bctx.cpp`,
  `bctx_plain.hero`, `bctx_signal.hero`; `bctx.h`'s head: *"fibers whose switch is hand-written assembly
  (jump_fcontext), so neither makecontext nor swapcontext is called"*); **`asyncjob.h`** (OpenSSL's ASYNC fibers; its
  head: *"the one shipped library of this Mac's 422 Homebrew dylibs that imports the context functions (libcrypto
  imports makecontext, swapcontext and setcontext: nm -u)"*); **`sched.c` and `sched.h`** (*"a preemptive green-thread
  scheduler"*, switching *"FROM THE TIMER'S SIGNAL HANDLER (swapcontext to the next thread)"*); and, under
  `programs/critic/mine/`, **`n6.h`**, a `makecontext` task *"on a stack that is a LOCAL ARRAY of the helper"*, the
  carved-stack shape, run by panel 257's critic (each head read by the repair, extracted into its scratch). **Panel
  257's census of this Mac's 422 dylibs** is `docs/panel/257-reports/ffi-pragmatist.md:563-572` (`nm -u` of every
  `.dylib` under `/opt/homebrew/opt/*/lib`).
- **The tree already tells a sanitizer of a switch**: `tests/golden/run/fixedbugs-coroutines-switched-by-hand.h` says
  so in its comment (line 4, *"as a coroutine library's must be"*), declares `__sanitizer_start_switch_fiber` and
  `__sanitizer_finish_switch_fiber` **only under** `#if __has_feature(address_sanitizer)` (lines 20 to 26), and calls
  them at lines 45 to 49. **Boost tells it only in its `ucontext` backends**: `grep -c -i sanitizer` prints 0 for
  `boost/context/fiber_fcontext.hpp` and `continuation_fcontext.hpp` (the backends `fiber.hpp` and `continuation.hpp`
  include unless `BOOST_USE_UCONTEXT` or `BOOST_USE_WINFIB` is defined, their lines 7 to 12) and 7 for
  `fiber_ucontext.hpp`, whose calls stand under `#if defined(BOOST_USE_ASAN)` around a `::swapcontext` (lines 154 to
  167), a switch the runtime already sees (§ 6 D).
- The golden twin of `own.h`, `fixedbugs-coroutines-switched-by-a-librarys-own-switch.h`, stops Windows with `#error`
  (its lines 7 and 8, `sed -n 1,12p`); **the shapes' `own.h` does not**: `grep -n error shapes/own.h` prints only its
  line 49, *"own.h: AArch64 and x86-64 only"* (the first base's brief said 54).
- **Claude Code** 2.1.323 (`claude --version`, 07:45), panel 257's blind run's version (carried, its report).

## 5. The reproducers, run

The lane's shapes are copied into `docs/panel/259-briefs/shapes/` (21 source files, no binary; `cmp` equal to
`.claude/worktrees/rebel-scratch/panel259/shapes/` and to `.claude/worktrees/lane-coroutine-room-scratch/shapes/`, 21 of
21 each, the clerk's check, run again by the repair at 08:17), with the critic's `below.h` and `below.hero` beside them
(`cmp` equal to `.claude/worktrees/259-critic-scratch/shapes-mine/`). **The repair ran every Mac and arm64 cell below at
this base**: in `.claude/worktrees/panel259-repair-scratch/`, each shape built with stage 2 (`heroes-next build
<shape>.hero -o bin-<shape>`, and `--sanitize -o bin-<shape>-san`) and each binary run as `timeout 120 ./bin-<shape> >
out-<shape>-<r>.txt 2>&1`, the exit `$?`; on this Mac (Darwin arm64, from 06:45 UTC), and in a container of the arm64
image (`p259-repair-1`, run with `--rm`; `uname -m` printed `aarch64`, `ldd --version` *Debian GLIBC 2.41-12+deb13u3*,
`clang --version` *Debian clang version 22.1.8*) from a `git archive 3ec775b66` of `seed runtime selfhost spec vendor`
and the root files, the seed's compiler and stage 2 built inside it (06:37 to 06:54 UTC, both exit 0, `sources
05186f2b38e77a97` and `sources 117362877b136f67`, the Mac's two keys). **The x86-64 cells are the critic's, at
`2cf85ea64`, emulated** (`critic-briefs.md` § I, F22 to F25: container `p259-critic-2`, *Debian GLIBC 2.41-12+deb13u3*,
clang 22.1.8, both compilers built inside it); the repair's x86-64 run was stopped unfinished at the coordinator's word
(07:55, the machine overloaded), so **their re-run at this base is the seats' who hold that image**. The script is
`runs/all.sh`, the arm64 container's (`runs/linux-arm64/all.log`); this Mac ran its steps with this Mac's paths and
without the compiler build (`runs/mac/all.log`). One output of each cell is in `runs/mac/` and `runs/linux-arm64/`. The
clerk's and the critic's cells at `2cf85ea64` read the same, cell for cell, where a row says so.

**The two carved shapes and `below`, three runs a cell:**

| shape | what C does |
|---|---|
| `own_carved.hero` + `own.h` | two coroutines on quarters of `ow_host`'s 1 MiB array (`char stacks[OW_N * OW_STACK]`, `own.h:75`), switched by the header's assembly; `body` holds `e5(7)` across both |
| `unseen_carved.hero` + `cr.h` | the same on `cr_host`'s array (`cr.h:69`), but switched by `makecontext` and `swapcontext` reached through `dlsym` (`cr_unseen`, § 4.2: `RTLD_NEXT` on Linux, `RTLD_DEFAULT` on Darwin) |
| `below.hero` + `below.h` (the critic's, F21) | two coroutines on quarters of an array in **the extern call's own frame** (`ow_below`, `below.h:6`), below its Heroes caller, switched by `own.h`'s assembly; the call starts both, resumes 1 then 0 and answers `a * 100 + b`, 3032 being right; **no Heroes frame holds a value across the call**, only the coroutines can harm each other |

| shape | platform, base | plain | `--sanitize` |
|---|---|---|---|
| `own_carved` | this Mac, at `3ec775b66` (the clerk's at `2cf85ea64` the same) | **exit 134, 3 of 3**, 160 bytes, equal by `cmp`: `body holds 11`, `body holds 11`, `panic: internal error: a large value's room was given back out of order (compiler bug)`, `called from 0x10002f70f, in own_carved.run` | **exit 134, 3 of 3** (3,393 bytes each, differing in addresses): `AddressSanitizer: heap-use-after-free ... READ of size 8`, `#0 ... in h_owncarved_body own_carved.hero:81`, `#2 ... in ow_host own.h:77` |
| `own_carved` | Linux arm64, at `3ec775b66` (the clerk's and the critic's at `2cf85ea64` the same) | **exit 139, 3 of 3**: the file holds `Segmentation fault` alone (19 bytes), nothing of the program's (`grep -c 'body holds'` 0) | **exit 1, 3 of 3**: `heap-use-after-free ... READ of size 8`, `#0 ... in h_owncarved_body /w/runs/own_carved.hero:81:76`, `#2 ... in ow_host /w/runs/./own.h:77:17` |
| `own_carved` | Linux x86-64, **at `2cf85ea64`, the critic's, emulated** (F23) | exit 139, 3 of 3, 19 bytes, nothing of the program's | exit 1, 3 of 3: `heap-use-after-free ... READ of size 8`, `#0 ... in h_owncarved_body /w/runs/own_carved.hero:81:76`, `#2 ... in ow_host /w/runs/./own.h:77:17` |
| `unseen_carved` | this Mac, at `3ec775b66` (the clerk's the same) | **exit 0, 3 of 3**, 66 bytes: `body holds 7`, `body holds 7`, `body holds 7 coroutines 30 32`, `carved: 7` | **exit 0, 3 of 3**, the same bytes (`cmp`) |
| `unseen_carved` | Linux arm64, at `3ec775b66` (**the `dlsym-make` lane will move this row**) | **exit 134, 3 of 3**, 823 bytes, equal by `cmp` to each other and to the critic's at `2cf85ea64`: `panic: unseencarved.run was called after C jumped into stack frames the thread had already left (a handler left by a jump and jumped back into, or frames only a signal's delivery puts on the alternate stack): the handler is held to be running again, and the function to be called from inside it, where neither this runtime's memory nor the C library's is safe to use.`, then spec line 428's tail (`HERO_SIGNAL_REMEDY`), then `Aborted` | **exit 134, 3 of 3**, the same bytes (`cmp`) |
| `unseen_carved` | Linux x86-64, **at `2cf85ea64`, the critic's, emulated** (F23) | exit 134, 3 of 3, 823 bytes, equal by `cmp` to the arm64 output | exit 134, 3 of 3, the same bytes |
| `below` | this Mac, at `3ec775b66` (the critic's at `2cf85ea64` the same, F21) | **exit 134, 3 of 3**, 127 bytes: `panic: internal error: a large value's room was given back out of order (compiler bug)`, `called from 0x10002f57b, in below.run`; nothing else printed | **exit 134, 3 of 3** (3,245 bytes each, differing in addresses): `heap-use-after-free ... WRITE of size 262144`, `#1 ... in h_below_e5 below.hero:67`, `#2 ... in h_below_run below.hero:72` |
| `below` | Linux arm64, at `3ec775b66` (the critic's the same, F21) | **exit 139, 3 of 3**, `Segmentation fault` alone (19 bytes) | **exit 1, 3 of 3**: `heap-use-after-free ... WRITE of size 262144`, `#1 ... in h_below_e5 /w/mine/below.hero:67:316`, `#2 ... in h_below_run /w/mine/below.hero:72:143` |
| `below` | Linux x86-64, **at `2cf85ea64`, the critic's, emulated** (F24) | exit 139, 3 of 3, nothing printed | exit 1, 3 of 3: `heap-use-after-free ... WRITE of size 262144`, `#1 ... in h_below_e5 /w/mine/below.hero:67:309`, `#2 ... in h_below_run /w/mine/below.hero:72:134` |

**Who freed the room, under `--sanitize`** (the reports' *freed by* stacks, read by the repair at this base): for
`own_carved` and for `below`, on this Mac and in the arm64 image alike, the room that `body` read (in `own_carved`) or
that the lower coroutine wrote (in `below`) was freed by **the thunk's entry**: `hero_callback_entered`
(`sigguard.c:641`) → `hero_callback_heal` (`callback.c:262`) → `hero_frame_heal_below` (`frame.c:183`) →
`hero_frame_pop` (`frame.c:176`) → `hero_frame_give_raw` (`frame.c:168`), called from the coroutine's thunk
(`h_0cb_owncarved_run`, `h_0cb_below_run`) under `ow_entry` and `ow_boot` (`own.h:52`, `:26`); it had been taken by
`hero_frame_take_raw` (`frame.c:139`). That is the sanitizer's build, where every chunk of the room is a `malloc` of its
own; the plain build's order is still a seat's to trace (§ 5's last paragraph).

`own_carved.hero:81` is `print("body holds ", mine.a.a.a.a.a.x)`, the first read of `body`'s record (`sed -n 81p`);
`below.hero:67` is `e5`'s `return E5(...)` and `:72` `run`'s `w = e5(k + 20)` (`sed -n`). **Darwin's AddressSanitizer
reports the use-after-free too**; the draft names it only on Linux.

**So `below` is not *"the host's value is read wrong"***: the higher coroutine's thunk heals the lower coroutine's live
take, inside one open extern call (critic, F21). It **separates a route-C candidate** (§ 6 C, *"the stack pointer of
the extern call that is still open"*): in `own_carved` the coroutines' frames lie above `body`, the open call's caller,
and in `below` every frame lies below `main`, inside the open call, as a nested callback's would. It belongs in every
seat's table beside `own_carved`.

**The Linux refusal of `unseen_carved`, TRACED** (critic, F11, at `2cf85ea64`; **re-run by the repair at `3ec775b66`**
in the arm64 image, `runs/all.sh`'s last step, with the critic's diff applied to this base's runtime by `patch -p1`: the
same ten lines in the same order, the same `field`, `known`, `back`, `below` and `unsure` values, other addresses,
`runs/linux-arm64/out-unseen_carved-trace.txt`; `own_carved`'s again three `TRACE thunk` lines and no `TRACE
swapcontext` line before its SIGSEGV, `body` at `0xffffc7b0c530` and the two `run`s at `0xffffc7b4c530` and
`0xffffc7b8c530`, `runs/linux-arm64/out-own_carved-trace.txt`). A copy of the runtime with five `fprintf`s
(`critic/trace/runtime-trace.diff`, against `sigentries.c`, `sigguard.c`, `sigjump_record.c` and `sigjump_save.c`, none
of which moved, § 2), handed to a build by `HEROES_RUNTIME=` (`critic/trace/run-trace.sh`), prints in the arm64 image
(`critic/trace/out-unseen_carved-trace.txt`):

```
TRACE thunk unseencarved.body frame=0xffffe5ff4850
TRACE swapcontext field=1 t=0xffffe6034880 wrapper-frame=0xffffe5ff4620 known=1 back=0
TRACE rule6 reenter t=0xffffe6034880 here=0xffffe5ff44f0 range=[0xffffe58f5000,0xffffe60f5000) below=0
TRACE thunk unseencarved.run frame=0xffffe6034850
TRACE swapcontext field=1 t=0xffffe5ff4500 wrapper-frame=0xffffe6034750 known=1 back=0
TRACE rule6 reenter t=0xffffe5ff4500 here=0xffffe6034620 range=[0xffffe58f5000,0xffffe60f5000) below=1
TRACE push unsure=2 t=0xffffe5ff4500 here=0xffffe6034620 below=1 into_alt=0
TRACE swapcontext field=1 t=0xffffe6074880 wrapper-frame=0xffffe5ff4620 known=1 back=0
TRACE rule6 reenter t=0xffffe6074880 here=0xffffe5ff44f0 range=[0xffffe58f5000,0xffffe60f5000) below=0
TRACE thunk unseencarved.run frame=0xffffe6074850
panic: unseencarved.run was called after C jumped into stack frames the thread had already left ...
```

**What it says**: every switch went through **the runtime's** `swapcontext` (`sigjump_save.c:191`, its route C,
`field=1`: the target read from the context), and the one range every address fell in is the thread's whole measured
stack (8 MiB), because the `makecontext` that gave the array's quarters was the C library's and named no range (§ 4.2).
The switch INTO coroutine 0 lands above (`below=0`); **coroutine 0's yield back to `body`** lands at `0x...e5ff4500`,
below its own frame `0x...e6034620` on that one range, so `hero_sig_reenter` (`sigentries.c:379-384`; **rule 6**, its
comment at `:367-376`: *"a jump that lands BELOW the jumping frame on the range that frame stands on lands in frames
this thread already unwound past"*) pushes an entry `unsure = 2`; and **the refusal is said at coroutine 1's first
thunk entry**, by `hero_sig_inside` (`sigentries.c:432-446`, `unsure == 2` read as `HERO_SIG_REENTERED`) through
`hero_callback_asked` (`sigguard.c:644`), in `HERO_SIGNAL_REENTERED`'s words (`sigguard.c:596-600`). **The landing
judged is the one below the frame making the switch**, not *"a landing above the thread's current frame"*, which the
first base's brief said. **It needs the switch seen, so it cannot reach `own_carved` on any platform**: the same trace
of `own_carved` (`critic/trace/out-own_carved-trace.txt`) prints three `TRACE thunk` lines, `body` at `0xffffca136c70`
and the two `run`s at `0xffffca176c70` and `0xffffca1b6c70`, both above `body`, and **no** `TRACE swapcontext` line
before its SIGSEGV. On Darwin `unseen_carved`'s `makecontext` is the program's own (§ 4.2), so its range should be
known and rule 6 have two ranges to tell apart: **an inference from the pointer, not traced on Darwin**.

**So on Linux `unseen_carved` is refused, in words about a signal handler the program does not have**: `grep -n -i
-E 'signal|sigaction|sigaltstack|raise' shapes/cr.h shapes/unseen_carved.hero` prints nothing. Those words are the
class of **2705** (closed) and **2755** (open), § 4.6: a correct program refused with a false message, `blocking` by
`.claude/rules/verification.md` § Bounded discovery. **Two findings, filed by nobody (neither the clerk nor the repair
files)**: the lane's *"not an unseen switch on either platform"* is false on Linux for two handles (§ 4.2); and the
false words. It is also route A's datum (§ 6 A): **an order judgment of the runtime already refuses this class on
Linux**, before any wrong value, in the wrong words, **and only where the switch is seen**.

**The rest, one plain run each** (the same commands), this Mac and the arm64 image at `3ec775b66` alike but where
said: `carved` (the same shape through `makecontext` the runtime wraps) 0, `body holds 7` and `carved: 7`; `own_heap`
(the library's own switch onto `malloc`'s stacks) 0, `order 0 1: 30 32`; `two` 0; `three` 0, six orders; `lend` 0,
`lent across switches: 3 104 205`; `spawn` 0, `thread: 30039 main: 5`; `migrate` 0, `migrated: 30 32 3`; `quit` 0;
**`quit_fresh` 134**, `panic: stack exhausted in quit_fresh.e5: the room large values take off the stack is full, 1
GiB for the whole program` (draft 2's message); `jumpco` 0, `jumped inside a coroutine: 10 35021`; `unseen` 0; **`ph`
0, `state 11` on this Mac and `state 10` in the arm64 image** (`dlsym` was asked, and on Linux `RTLD_NEXT` gave the
C library's `makecontext`, not the program's: § 4.2's door; **the `dlsym-make` lane will move this row**). The
clerk's Mac row and the critic's arm64 row (F16) at `2cf85ea64` read the same. The x86-64 image's rest is unrun.

**How the order lies in `own_carved`, as the clerk reads the code (read, not traced by a debugger)**: `ow_host`'s frame
holds the array; `body`'s frame lies below it and takes its 256 KB room; a coroutine's thunk is then entered **above**
`body`'s frame, inside the array, and `hero_callback_heal` gives back every entry at or below it, `body`'s included
(`callback.c:257-263`); the coroutine's own takes then reuse that room. Which of the thunk's entry or the first take
gives it back first is a seat's to trace in the plain build; under `--sanitize` the reports name the thunk's entry
(above, *Who freed the room*). One fact for it (critic, F11, Linux arm64 at `2cf85ea64`): the two
coroutines' thunks enter at `0x...176c70` and `0x...1b6c70`, `body`'s at `0x...136c70`, so the thunk's entry is above
`body`'s frame on that run.

## 6. The routes, as questions

**Every route below is a question the seats answer by running it, never a premise.** The ones the clerk was asked to
list come first.

- **A. A refusal that tells the truth where an unseen switch makes the order lie, if the runtime can detect it.**
  The events in hand are a thunk entered above a live entry's frame, a take above one, and a give that finds entries
  above it. Each is what a C jump over a callback leaves (R1's case, where healing is right) **and** what a switch
  onto a carved stack leaves (this case, where it corrupts). **Is there any fact the runtime can read, at that
  moment, that tells the two apart?** If one exists, what does the program get and in which words (and before any
  wrong value is printed, which `own_carved` does today)? If none exists, say what was tried. **The one judgment that
  already fires** (§ 5, traced) is rule 6 on Linux: an order judgment on **a seen switch's two addresses**, the
  landing below the frame making it, told at the next thunk's entry in a signal handler's words (2705's and 2755's
  class). It **cannot reach `own_carved` or `below`**, whose switches the runtime does not see at all: what fact could?
- **B. Healing only on a jump the runtime saw.** An entry would be given back only where a jump the hook decoded
  (`sigjump_save.c`, `rebind_jump.c`) crossed its frame, over the interval between the jumping frame and the target,
  never *"everything below"*. **What does each unseen door then get** (§ 4.3's table: 8, 12 and 13 abandon frames with
  the process running on, 9 and 10 end the thread, door 6 on x86-64, `dlvsym`, the dyld shared cache, Windows): a leak,
  an out-of-order abort with words that are false today, or a told refusal? Which correct program that runs on `main`
  today stops running?
- **C. A stack identity the runtime cannot be fooled about.** The address alone is fooled. **Is there a fact of the
  running frame that no switch can counterfeit**, and what fools each candidate? The ones in sight: the chain of frame
  records from the asking frame to the thread's outermost frame (`own.h`'s first frame writes `x29 = 0`, `ow_prime`,
  `own.h:28-31`, and `rbp = 0` on x86-64, `:42-45`; what glibc's and Darwin's `makecontext` write; whether a library
  links its first frame to the resumer's on purpose, for backtraces; C frames that keep no frame record, x86-64
  `-fomit-frame-pointer`); the stack pointer of the extern call that is still open (a record on every extern call: a
  store and a load, the draft's second repair), **which `below` (§ 5) does not separate**, every frame of it lying
  inside the open call; a token a told switch changes (route D). Its cost on the path where nothing heals, and on the
  path where something would.
- **D. A switch the binding tells.** The sanitizers' model: AddressSanitizer is told of a fiber by
  `__sanitizer_start_switch_fiber`, and the tree's own golden header already tells it (§ 4.7). **Would a runtime entry
  point a binding calls at each switch (`hero_os.h`, *"a stability promise, decision 2228"* in panel 258's ffi brief,
  carried; `gh issue view 2228` reads CLOSED, *"May a program bind `hero_os.h`, the runtime's own header, which the
  specification never names?"*), or the runtime answering a call a library already makes for a sanitizer, make the order
  true?** **The second variant reaches `--sanitize` builds alone** (§ 4.7, run by `grep`): Boost's default backends make
  no sanitizer call, its `ucontext` backends make it under `BOOST_USE_ASAN` around a `::swapcontext` the runtime already
  sees, and the tree's header makes it only under `__has_feature(address_sanitizer)`; **no seat builds on a plain build
  answering it**. What does a binding that does not tell get? A function a binding calls is a new form: does it owe a
  spec sentence (`arms/arm-t` is the clerk's draft of one)?
- **E. No healing by address at all.** Stop judging a take gone by its frame's address (the draft's first repair):
  every take of a callback-reachable frame a block of its own, given back by its give in any order (the lane
  measured an aside take at **+550 instructions at 32 KB and +1,900 at 256 KB** over a reused block, carried from
  `f767e0d4e`'s body, `git show -s --format=%B f767e0d4e`). **Then what repairs defects 2752 and 2756 for the jumps R1
  heals today**, and what does a leaked block do to the room's gibibyte (draft 2's *stack exhausted*)?
- **F. The specification alone.** §1.12 forbids corrupting memory, so a sentence that leaves a value's bytes
  unspecified after a switch is not on the table. **Which sentence does each of A to E owe**, and does any route
  owe none (R1 owed none)? `arms/arm-r` (a told refusal), `arm-t` (a told switch) and `arm-p` (a promise) are the
  clerk's drafts, each clause a claim to be tested and repaired, none ranked; the critic's F18 marks clauses the runs
  above already decide.
- **G. The route nobody listed.** **What would have to be true for a route outside A to F to exist?** Two
  directions nobody has run, said as questions: a large value whose room is owned by its C frame and not by a
  per-thread order at all (the room exists because an 8 MiB stack cannot hold a value megabytes deep,
  `frame.c:16-21`); and whether the answer differs for a library that **copies** stacks in and out of one region
  (design.md's row names *"stack copying"*), where two coroutines' frames hold **the same addresses** in turn. The
  critic's first reading answered this question first (its § G, G1 to G5): read it.

**The shapes beside the defect (CL-061)**, none run by the clerk but the last but one; `own.h`'s x86-64 arm and
`below` are run now (§ 5): a lend held across an own switch onto a carved stack (`lend.c`'s heal asks the same order:
is the region C writes into then another frame's?); a thunk entered on a carved stack while a frame below has a lend
open; the same with the stacks carved from a `spawn` thread's stack; a C `longjmp` inside a coroutine on a carved stack
(`jumpco`'s shape with `own.h`); a signal delivered while a coroutine runs on a carved stack (panel 255's rules 4 to 7,
`sigentries.c`, also judge stacks by the ranges they know); a value **under** the 4,096-byte threshold held the same
way (`blind/programs/q7.hero`, the clerk's: exit 0 on every platform, `blind/answers.md`); a library that copies
stacks.

## 7. What the coordinator does with the reports

The synthesis adopts **the most robust and complete resolution**, never the cheapest and never a compromise
(CLAUDE.md § 4), records what the conservative one would have been, and marks it `provisional — author ratification
pending`. A spec sentence is a diff file in `docs/panel/259-reports/`, **never applied to `spec/`** in this sitting;
its real count is the warden's. The decision issue is drafted, not filed, by the coordinator. The sitting's files,
the spec and the runtime are on `deploy-site.yml`'s `paths:` (`'docs/panel/**'` at line 77, `'spec/heroes-spec.md'`
at 73, `'runtime/**'` at 83, `grep -n`): the sitting is committed by pathspec and **not pushed** without the
author's yes (CLAUDE.md § Hard stops).
