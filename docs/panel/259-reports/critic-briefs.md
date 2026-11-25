# Panel 259, the completeness critic: first reading (the briefs, before any seat)

Started 2026-11-25 06:40 by this Mac's clock (`date`). Written as I go. My copy is
`.claude/worktrees/259-critic` at `2cf85ea64` (`git rev-parse HEAD`); long output in
`.claude/worktrees/259-critic-scratch/`. Every finding carries its command and its result; what I read and did not
run says **read**. I give no verdict on any route.

## A. The framing facts, re-run

**F1. § 4.1's counts and lines hold.** `wc -l` of the nine files prints 270, 1148, 277, 165, 153, 214, 271, 448,
430, as the brief says. `grep -n 'hero_frame_is_below\|hero_callback_heal(' runtime/parts/*.c` prints frame.c:182
and 237, lend.c:440 and 472, sigguard.c:641 and 707, callback.c:213 and 257; `grep -n` puts `hero_frame_out_of_order`
at frame.c:152, `hero_frame_heal_below(own)` at 199, `hero_frame_give` at 224, `hero_lend_heal_inside` at lend.c:439
and its call by `hero_lend_push` at 526. One precision the seats need: the thunk's entry heals the regions through
`hero_lend_heal_below` (lend.c:450, which brackets and calls `hero_lend_heal_inside`), and the regions' give,
`hero_lend_heal_to` (lend.c:472), **returns silently** where an entry above is not below the giver, where the room's
give stops with *compiler bug* (frame.c:237): the two heals do not fail alike (read, `sed -n 425,535p lend.c`).

**F2. § 4.2's lines hold, and the Linux mechanism is one sentence short.** `stack_lane.c:51-55` is quoted exactly;
`HERO_LANE_CARVED 4` (callback.c:70), `HERO_SIG_STACKS 256` (sigentries.c:91), `HERO_REBIND_STACKS 256`
(rebind_entries.c:31), `hero_stacks_made` (callback.c:197), frame_aside.c:98, 99, 108, all by `grep -n`. The
Linux `dlsym` table (`siglookup.c:124-143`) holds twenty names and no `makecontext`; Darwin's
(`rebind_image.c:40`) holds `_makecontext`. **What the brief does not say**: on Linux the runtime defines
`makecontext` as a global symbol of the executable itself (`runtime/parts/sigjump_make.c:38-58`, `.globl
makecontext`), and `cr.h`'s `cr_unseen` asks `dlsym(RTLD_NEXT, ...)` on Linux and `RTLD_DEFAULT` on Darwin
(`shapes/cr.h`, its `#if defined(__APPLE__)`). `RTLD_NEXT` from the executable skips the executable's own
definitions by its definition, so the Linux door is also a question of **which handle** the program passes; the
shape beside it, `RTLD_DEFAULT` on Linux, is unlisted (G3 below).

**F3. `own.h`'s `#error` is line 49, not 54.** `grep -n error docs/panel/259-briefs/shapes/own.h` prints
`49:#error "own.h: AArch64 and x86-64 only"`. The brief's sentence (§ 4.7) is otherwise right: no `_WIN32` stop,
where the golden twin stops Windows at its line 8 (`sed -n 1,12p
tests/golden/run/fixedbugs-coroutines-switched-by-a-librarys-own-switch.h`).

**F4. § 4.4 holds, and its negative sentence survives a wider search.** 516 lines, 34,437 bytes, sha256
`c915f0f2...6372bee` (`wc -l`, `wc -c`, `shasum -a 256`); the grep prints line 428 alone; lines 331, 465-466, 473-474
and `1 the runtime` as said; `selfhost/emit/callback_thunk.hero:408` holds line 428's tail equal to
`HERO_SIGNAL_REMEDY` (`sigguard.c:110`), read. I searched the spec again for `large`, `threshold`, `KiB`, `MiB`,
`frame`, `context`, `setjmp`, `longjmp`, `jump`, `callback`, `across`, `keeps` (`grep -n -i`, one word each): the
hits are lines 11, 79, 200, 277, 280, 284, 326, 340, 376, 383, 427, 428, 488, 490, and none speaks of a value held
across a switch or a jump, nor of a threshold. The sentence stands as a measurement of those words.

**F5. The issues: the states hold; the list omits the family the § 5 Linux finding belongs to.** `gh issue view`
reads 2758 OPEN `kind:decision`, 2756 OPEN `class:adjacent`, 2745 OPEN `class:blocking`, 2702 and 2707 CLOSED, 2228
CLOSED, as said. I searched (`gh issue list --state all --search '<w> in:title,body'`, results in
`259-critic-scratch/issue-search.txt`) seventeen words beyond the clerk's five, twenty-two in all. What § 4.6 does not list and a
seat needs:

- **2755** OPEN `blocking`: *"A C signal handler that `siglongjmp`s out of a Heroes callback ends the program with
  a message accusing a Heroes signal handler that does not exist"*, and **2705** CLOSED: *"The words a refusal prints
  name causes the program does not have"*. § 5's Linux finding (`unseen_carved` told about a handler it does not
  have) is that class again, so it is not *"filed by nobody"* in kind: it is 2705's class, recurring, on a program
  with no handler at all. 2705's body names panel 257's `sched` (*"a green-thread scheduler that switches back"*), a
  shape of this sitting's kind already written.
- **2752**, **2753**, **2754** OPEN `blocking` (panel 258's family: the defect R1 repairs, a handle told falsely after
  a jump, a `pthread_exit`ed `spawn` thread). § 6 E names 2752; none is in § 4.6.
- **2811** OPEN `blocking` (`area:harness`): forty-two run cases bind POSIX headers Windows lacks. It bears on
  `own.h`'s missing Windows stop and on step 181b's `unbuildable_on_windows`.
- `fiber`, `carved`, `fcontext`, `jump_fcontext`, `switch_fiber`, `stack_lane`, `pthread_attr_setstack` and `Boost`
  in titles and bodies return **no issue about a switch** (Boost returns 2159 only, about panel 226). So *"nothing
  beyond the issues above names a switch the runtime cannot see"* holds for issue text, with one exception of
  substance outside the issues: **panel 257's evidence already holds real-library and green-thread shapes** the
  briefs do not point at (F6).

**F6. Panel 257's evidence holds the libraries § 6 and the ffi brief ask for, and the briefs point at one of
them.** `tar -tJf docs/panel/257-reports/evidence.tar.xz` lists, under `programs/ffi-pragmatist/`, `bctx*` (Boost,
named by the brief), and also `asyncjob.h` (OpenSSL's ASYNC fibers; its head says it is *"the one shipped library of
this Mac's 422 Homebrew dylibs that imports the context functions (libcrypto imports makecontext, swapcontext and
setcontext: nm -u)"*), `sched.c`/`sched.h` (a preemptive green-thread scheduler, `malloc` stacks, `swapcontext` from
the timer's handler), `gsrun`, `bmon`, `cxxguard`; and under `programs/critic/mine/`, `n6.h`, a `makecontext` task
on **a local array of a helper**, the carved-stack shape, run by panel 257's critic. Panel 257's census of the Mac's
dylibs is the start of the ffi-pragmatist's question 1 and is not cited.

**F7. My compilers are the clerk's.** In my copy, `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`
then `./heroes build selfhost/main.hero -o heroes-next` (exit 0, `259-critic-scratch/build.log`): `--version` prints
`sources 05186f2b38e77a97` and `sources 32437b681fe95f82`, as § 3 says. In `heroes-linux-arm64:latest` (container
`p259-critic-1`, from `git archive 2cf85ea64 seed runtime selfhost spec vendor` and the root files), the same two
builds exit 0 and print the same two keys (05:42 to 05:50 UTC).

**F8. § 4.5's budget holds.** `./heroes-next measure spec/heroes-spec.md`: claude-legacy 9,877, cl100k_base 10,054,
real 13,185, headroom 3,199, the floor's 60, 13,245 against the ceiling, as said.

## B. The carved shapes, three runs a cell

Each built in my copy with stage 2 (`./heroes-next build <s>.hero -o bin-<s>`, and `--sanitize`), run as
`timeout 30 ./bin-<s> > out 2>&1`; outputs in `259-critic-scratch/{mac,linux-arm,linux-x86}/runs/`.

**F9. This Mac (Darwin arm64), 06:47: the clerk's four cells, exactly.** `own_carved` plain exit 134, 3 of 3,
the three outputs equal by `cmp` (160 bytes: `body holds 11` twice, the *compiler bug* panic, `called from
0x10002f70f, in own_carved.run`); `--sanitize` exit 134, 3 of 3, `heap-use-after-free ... READ of size 8`, `#0 ... in
h_owncarved_body own_carved.hero:81`, `#2 ... in ow_host own.h:77` (the outputs differ only in addresses);
`unseen_carved` plain and `--sanitize` exit 0, 3 of 3, the four lines.

**F10. Linux arm64: the clerk's four cells, exactly.** `own_carved` plain exit 139, 3 of 3, nothing of the
program's printed (19 bytes, the shell's `Segmentation fault`); `--sanitize` exit 1, 3 of 3, `heap-use-after-free`,
`#0 ... in h_owncarved_body /w/runs/own_carved.hero:81:76`; `unseen_carved` plain exit 134, 3 of 3, 823 bytes,
`HERO_SIGNAL_REENTERED`'s words then `HERO_SIGNAL_REMEDY`, and `--sanitize` the same bytes (`cmp`).

**F11. § 5's Linux finding, TRACED: rule 6 judged coroutine 0's YIELD, a landing BELOW the jumping frame, and the
next thunk told it. The clerk's reading has the direction wrong.** A copy of the runtime with five `fprintf`s
(`259-critic-scratch/linux-arm/runtime-trace/`, built by `HEROES_RUNTIME=/w/runtime-trace`, script
`linux-arm/run-trace.sh`), in the arm64 image, prints (`linux-arm/runs/out-unseen_carved-trace.txt`):

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

So: every switch went through the runtime's `swapcontext` (`sigjump_save.c:191`, route C, `field=1`: the target
read from the context), and the one "range" every address fell in is the thread's whole measured stack (8 MiB),
because the `makecontext` that gave the array's quarters was libc's and named no range. The switch INTO coroutine 0
lands above (`below=0`); **coroutine 0's yield back to `body`** lands at `0x...e5ff4500`, below its own frame
`0x...e6034620` on that one range, so `hero_sig_reenter` (`sigentries.c:379-384`, rule 6, *"a jump that lands BELOW
the jumping frame on the range that frame stands on lands in frames this thread already unwound past"*) pushes an
entry `unsure = 2`; **the refusal is said at coroutine 1's first thunk entry**, by `hero_sig_inside`
(`sigentries.c:432-445`) through `hero_callback_asked` (`sigguard.c:644-668`). § 5 says *"the jump hook's own order
judges a landing above the thread's current frame"*: the landing judged is the one **below the frame making the
switch**. What it means for route A: the judgment is an order judgment on **a seen switch's two addresses**; the same
trace of `own_carved` prints three `TRACE thunk` lines and no `TRACE swapcontext` line before its SIGSEGV
(`out-own_carved-trace.txt`: `body` at `0xffffca136c70`, the two `run`s at `0xffffca176c70` and `0xffffca1b6c70`,
both above `body`), so **this judgment cannot reach `own_carved` on any platform**: it needs the switch seen. And on
Darwin `unseen_carved`'s `makecontext` pointer is the program's own (F12), so its range should be known and rule 6
  have two ranges to tell apart: an inference from the pointer, not traced on Darwin.

**F12. The Linux door is the handle, measured.** A probe of my own (`259-critic-scratch/probes/dlh.h`, `dlh.hero`)
compares four `dlsym` answers with the program's own symbol. This Mac prints `state 1111`: `RTLD_DEFAULT` and
`RTLD_NEXT` both answer the program's own `makecontext` and `swapcontext`. Linux arm64 prints `state 1011`:
**`RTLD_DEFAULT` answers the program's own `makecontext`** (the runtime's, which the executable exports: `nm -D bin-dlh`
prints `T makecontext`), `RTLD_NEXT` answers libc's, and both handles answer the program's own `swapcontext`. Asked
of libc's own handle (`dlopen("libc.so.6", RTLD_NOLOAD)`, `dlh2.h`), Linux prints `state 1`: libc's `makecontext`, the
program's `swapcontext`. So on Linux the unseen `makecontext` is the one asked by `RTLD_NEXT` or by the C library's
handle, never by `RTLD_DEFAULT`; § 4.2's *"the table has no makecontext row"* is true and not the whole reason, and
the dlsym-make lane's repair should be judged against all three handles.

## C. The blind programs, run

**F13. This Mac, 3 runs each (06:48): `answers.md` holds, row for row.** q1, q3 `order 0 1: 30 32`, `order 1 0: 30 32`,
exit 0; q2, q7 the four lines, exit 0; q4 `body holds 11` twice then the *compiler bug* panic, exit 134; q5 the
*stack exhausted* panic, exit 134; q6 `jumped inside a coroutine: 10 35021`, exit 0; each three outputs equal
(`cmp`).

**F14. Linux arm64, 3 runs each (06:52): six rows hold, and q4 SPLITS.** q1, q2, q3, q6, q7 print what the Mac
prints, exit 0; q5 the same panic, exit 134. **q4 exits 139, 3 of 3, printing nothing** (the shell's `Segmentation
fault` alone, 19 bytes). So under `now` the defect's own program is `WRONG then ABORTS` on the Mac and a silent crash
on Linux, and `answers.md`'s own rule (*"a program whose two platforms differ is graded by its own row, named, and is
out of D1 while it splits"*) takes q4 **out of D1 under `now`**, the arm D1's second half is about (*"under `now` at
most 2 of 5"*). And `task-t1.md` tells the reader the programs behave *"on a Mac and on Linux alike"*, which is false
of q4 today (registration findings, § E).

**F15. Design C of t2 runs on both platforms, and its key lives in a folder the sweep may take.** `answers.md`'s
design C is the clerk's variant header in `.claude/worktrees/panel259-scratch/blind-mmap/` (`diff` against the
registered `sw.h`: `mmap` of `SW_STACK + page`, the first page `PROT_NONE`; `q3m.hero` equal to `q3.hero`), not
registered and not hashed. Copied to my scratch and run 3 times: this Mac exit 0, Linux arm64 exit 0, both
`order 0 1: 30 32`, `order 1 0: 30 32`.

**F16. The lane's other shapes on Linux arm64 (one plain run each, 06:57): one row differs from the Mac's.**
`carved` 0 (`carved: 7`), `own_heap` 0, `two` 0, `three` 0 (six orders), `lend` 0 (`lent across switches: 3 104 205`),
`spawn` 0 (`thread: 30039 main: 5`), `migrate` 0, `quit` 0, `quit_fresh` 134 (*stack exhausted*), `jumpco` 0
(`10 35021`), `unseen` 0, as the Mac; **`ph` prints `state 10`** where the Mac prints `state 11` (F12's door).

## D. The arms

**F17. The three diffs apply and are now counted (vendored only).** `patch --dry-run -p1 < arms/arm-<x>.diff` in my
copy prints `patching file 'spec/heroes-spec.md'`, exit 0, three of three; each applied to a copy holds its `.txt`
once (`grep -c -F`). `./heroes-next measure <copy>`, **run from the repository's root** (run from inside the copy it
exits with `cannot read vendor/tokenizers/claude-legacy.json`, a trap for the warden):

| arm | bytes | claude-legacy | cl100k_base | vendored delta (max) |
|---|---|---|---|---|
| base | 34,437 | 9,877 | 10,054 | |
| `arm-p` | 34,575 | 9,909 | 10,086 | +32 |
| `arm-r` | 34,704 | 9,945 | 10,123 | +69 |
| `arm-t` | 34,723 | 9,954 | 10,131 | +77 |

A lower bound, vendored only; no `--refresh` was run.

**F18. Clauses the runs above already decide, for the seats to start from (facts, no ranking).** `arm-r`'s *"a stack
... given to `makecontext`, is followed"*: false on Linux today for a `makecontext` reached by `dlsym(RTLD_NEXT, ...)`
or libc's handle (F10 to F12; the dlsym-make lane may change it). `arm-r`'s scope *"switched to by code of its own,
not by `swapcontext` or `setcontext`"*: on Linux `unseen_carved` switches **by `swapcontext`** (the runtime's, F11)
onto a carved stack and aborts, so a program outside the clause's scope aborts today. `arm-r` and `arm-t` scope by
*"a callback C runs ... on a stack ..."*, which q7 (512 bytes, prints right on both platforms) and my `below` shape
(F21) both fall in. `arm-p`'s promise is false today (`own_carved`, `below`). `hero_stack_entered` exists nowhere
(`grep -rn hero_stack_entered runtime selfhost spec` in my copy prints nothing).

## E. The blind registration

**F19. What it measures, against what it says.**

1. **q4, D1's program, splits under `now`** (F14): Mac `WRONG then ABORTS`, Linux a silent SIGSEGV. By `answers.md`'s
   own rule it is out of D1 while it splits, so D1's *"under `now` at most 2 of 5"* has no graded row. The task's
   premise *"on a Mac and on Linux alike"* is false for q4 today, and a reader told so is told something untrue.
2. **q4's key is two verdict words.** The task offers four exclusive words; today's Mac key is `WRONG then ABORTS`. Which
   word is right (either, both, the first) is not registered, and D1 counts that row.
3. **`NOT PROMISED` against a sentence that says *may*.** `arm-r` and `arm-t` say a callback in the switch's class
   *"may abort"*; `answers.md` says `NOT PROMISED` *"is never right where the landed build runs or aborts the same
   way three times of three"*. A reader who applies `arm-r` exactly to q7 (a callback on a stack switched by the
   header's own code inside the thread's stack: in scope, F18) would answer *may abort*, `NOT PROMISED` (an inference;
   no reader was run), and be graded not right, while the build prints. D2 then falls by construction under any *may* text whose class holds q7, and D2
   failing *"withdraws the sentence"*. The same holds of q4 under a refusal route: the honest reading of *may abort*
   is not `ABORTS`. **The registration measures whether a reader predicts the build, not whether it reads the
   text**, and a hedged text loses by its hedge. A rule mapping *may abort* to the verdict words is owed before
   any session, or the arms' wording owes *aborts*.
4. **t2 points at a program its session does not hold.** `task-t2.md`: *"holds a record of 262,144 bytes across a
   yield, as q4's `run` does"*; `registration.md`: *"A t2 session's `programs/` holds `co.h` and `sw.h` alone."*
5. **The folder's files disagree with the task's words.** `task-t1.md` and `task-t2.md` say *"in the structure
   `brief.md` gives"* and name the spec `spec.md`; `llm-ergonomist.md` lists the folder as `spec.md`, `task.md`,
   `programs/` (no `brief.md`); panel 256's `make_sessions.py:35`, the runner to adapt, writes `brief.md`, `task.md`
   and **`heroes-spec.md`** (`grep -n brief docs/panel/256-briefs/blind/make_sessions.py`). The adaptation's
   `registration-runner.md` must settle the names before any session.
6. **Design C's key is an unregistered header** in a folder the sweep removes when idle (F15): copy it beside
   `answers.md` with its sha256.
7. **What holds**: every hash of `registration.md` matches its file (`shasum -a 256`, 12 of 12); the input sizes
   17,929 and 6,550 bytes (`cat | wc -c`) and panel 257's 6,485 (`prelude.h`, `p1.h` to `p7.h` of
   `docs/panel/257-briefs/blind/programs/`) and its 6.297764 USD (`llm-ergonomist-blind.md:12, 47`) are as said.

**F20. Leaks and the contract.** I read every registered file for a word that names the project, the runtime, a
defect, a wrapper or an arm: the `.hero` files carry no comment (`grep -c '#'` 0 each) and name only `co_`, `sw_`,
`jump_` functions and the records; the headers' comments describe what the C does (*"switched with the C library's
makecontext, swapcontext and setcontext"*, *"this header's own switch, written in assembly"*) and name nothing of
the project. The tasks name no arm. I found no leak. Nothing in the registration starts a paid run, writes outside
the root (the area is `.claude/worktrees/259-llm-ergonomist/`, its `HOME` and `TMPDIR` inside it) or prints a
credential; the one credential the briefs touch is the warden's `.env`, read only on an authorisation and printed
as `${#ANTHROPIC_API_KEY}`.

## F. The negative sentences and the claims about the option set

- *"The searched words name no value's room across a switch"* (§ 4.4): **holds** for twelve more words (F4).
- *"Nothing beyond the issues above names a switch the runtime cannot see"* (§ 4.6): **holds for issue titles and
  bodies** under twenty-two words (F5), **and is false of the record**: panel 257's evidence (F6) holds a
  green-thread scheduler, OpenSSL's ASYNC fibers, Boost.Context (panel 257's `bctx.h`: *"neither makecontext nor
  swapcontext is called and a runtime that rewrites those never sees a switch"*) and a carved `makecontext` stack.
- *"Whether the Linux images hold Boost is unsearched"* (§ 4.7): **searched, absent in both**. In
  `heroes-linux-arm64:latest` and `heroes-linux:latest`, `ls /usr/include/boost` and `ls
  /usr/lib/*/libboost_context*` find nothing, and `dpkg -l | grep -i boost` prints nothing. Installing it is a
  download, which the sitting forbids, so **every Boost cell is this Mac's alone**. And on this Mac Boost ships
  headers and libraries and **no assembly source**: `ls /opt/homebrew/opt/boost` is `INSTALL_RECEIPT.json README.md
  include lib sbom.spdx.json share`, and `find / -iname 'jump_x86_64_ms_pe*'` finds nothing, so the
  ffi-pragmatist's question 6 (Boost's Windows switch and the thread block) **cannot be read by that seat** (no web,
  no download): only the historian, who reads pages, can.
- **Route D's *"the runtime answering a call a library already makes for a sanitizer"*: no plain build makes that
  call.** Boost's default backend headers (`fiber_fcontext.hpp`, `continuation_fcontext.hpp`) hold no
  `__sanitizer` call (`grep -n -i sanitizer`); the calls are in the `ucontext` backends under `BOOST_USE_ASAN`
  (`fiber_ucontext.hpp:154-167`), which switch by `::swapcontext`, a switch the runtime already sees. The tree's
  own header calls them only `#if __has_feature(address_sanitizer)`
  (`fixedbugs-coroutines-switched-by-hand.h:20-26`). So that variant of D reaches `--sanitize` builds alone.
- § 4.2's *"on Linux the table `dlsym` is answered from has no `makecontext` row"*: true, and the door is the handle
  (F12).
- § 5's reading of the order: the Linux refusal is traced (F11), and the reading was wrong in its direction; the
  Mac's *"which of the thunk's entry or the first take gives it back first"* stays the compiler-engineer's, unrun by
  me. One fact for it: on Linux arm64 `own_carved`'s two coroutines' thunks enter at `0x...176c70` and `0x...1b6c70`,
  `body`'s at `0x...136c70` (F11), so the thunk entry is above `body`'s frame on that run.
- § 4.3's carried *"3 of 13 doors with no event"*: **a reading of panel 258's table, not its count.** Its Darwin and
  Linux tables (`docs/panel/258-reports/ffi-pragmatist.md:49-94`) mark *no* for doors 9, 10, 11, 12 and 13, and door
  8's *"abandonment is not an event"*; three of them leave the process running with frames abandoned (8, 12, 13),
  two end the thread with no event (9, 10), 11 is `exit`. Door 6, `__builtin_longjmp`, cannot be written on arm64
  and *"on x86-64 it can never be seen"* (its line 86), and panel 258 lists it **unrun** on x86-64 (its line 407).
  A route that heals only on a seen jump (B) loses doors 8, 12, 13, and door 6 on x86-64, a cell nobody has run.

## G. The routes nobody listed (§ 6 G, answered first by me), and the shapes beside

**F21. A shape the clerk did not list, run: two coroutines on a stack carved from the EXTERN CALL's own frame,
below its Heroes caller.** `259-critic-scratch/shapes-mine/below.h` (includes `own.h`; `ow_below(f)` holds the
array in its own frame, starts coroutines 0 and 1 on two quarters, resumes 1 then 0, answers `a * 100 + b`) and
`below.hero` (`run` as `own_carved`'s; `main` prints `ow_below(f: run)`, 3032 being right). No Heroes frame holds a
value across the call; only the coroutines can harm each other. Three runs a cell:

| platform | plain | `--sanitize` |
|---|---|---|
| this Mac | exit 134, 3 of 3: `panic: internal error: a large value's room was given back out of order (compiler bug)`, `called from 0x10002f57b, in below.run`; nothing else printed | exit 134, 3 of 3: `heap-use-after-free ... WRITE of size 262144`, `#1 ... in h_below_e5 below.hero:67`, `#2 ... in h_below_run below.hero:72` |
| Linux arm64 | exit 139, 3 of 3, nothing printed | exit 1, 3 of 3, `heap-use-after-free ... WRITE of size 262144` |

So the class is not *"the host's value is read wrong"*: the higher coroutine's thunk heals the lower coroutine's
live take, inside one open extern call. **It tests a route-C candidate the brief names**: *"the stack pointer of the
extern call that is still open"* separates `own_carved` (the coroutines' frames lie above `body`, the open call's
caller) and does not separate this one (every frame lies below `main`, inside the open call, as a nested callback's
would). It belongs in every seat's table beside `own_carved`.

**Routes outside A to F, each a question, none ranked:**

- **G1. Retire, do not reuse: a heal the runtime cannot prove leaves the room unreadable instead of handing it to
  the next take**, so a live owner's next read faults into a true message before any wrong value. The precedent is
  in this runtime: the lend regions already retire a region given back (`lend.c:573-599`, `hero_lend_retire`:
  regions over `HERO_LEND_KEEP`, 1 MiB, sealed; smaller ones summed and audited at the next lend; under the
  sanitizer a quarantine of 256, `lend.c:186-194`). The room (`frame.c`) has no such watch. What it would cost: the
  room's chunks are `malloc`'s, not pages; a retired entry is never reused, so R1's bound on repeated jumps (about
  1,260 for `room_order`, frame.c:35) returns unless a seen jump releases it (B). Not run.
- **G2. A static tell: a word on the callback parameter, in the `extern` group, saying C may run it on another
  stack**, so the emitter takes every large value under that thunk aside (route E for those thunks alone, at
  compile time). D's twin without a runtime call; a new form (Principle 0, the warden's), and a binding that
  forgets it gets what D's forgetting gets. Not run.
- **G3. The door is a handle, and the handle is the program's** (F12): a repair that answers `RTLD_NEXT` and libc's
  handle for `makecontext` closes `unseen_carved`'s Linux door and leaves `dlvsym` and the shared cache; the
  dlsym-make lane's repair, not this sitting's, but every seat's table should hold the three handles.
- **G4. A hardware shadow stack as route C's identity** (x86 CET's shadow-stack pointer, AArch64 GCS's
  `GCSPR_EL0`), which a switch must change by instructions a counterfeit cannot reproduce: **unavailable here,
  measured**: the arm64 image's `/proc/cpuinfo` `Features` line holds no `gcs` (`grep -c gcs` prints 0), and this
  Mac (`Apple M3`) lists no `FEAT_GCS` in `sysctl hw.optional`. The x86-64 image's CET is unread. A route to name and
  set aside on a measurement, not on a guess.
- **G5. A cookie the take writes into the taking frame, read back before a heal**: by the layout read (not run), a live frame's cookie stays intact
  in `own_carved` and `below` (nothing ran below the live frame since); a dead frame's cookie may also be intact
  (nothing overwrote it yet), so it is fooled toward *alive*, which withholds a heal (2752's false *compiler bug*
  returns, never a wrong value). A route-C candidate the brief does not name; its failing direction is the safe one.
  Not run.
- The two § 6 G directions (a room owned by its C frame; a stack-copying library) stay assigned and unrun.

**Shapes beside the defect the clerk's list lacks** (CL-061): the `below` shape above (run); a carved stack on a
thread `spawn_sized` started, whose stack the runtime chose (the list has *"stacks carved from a `spawn` thread's
stack"*, not the sized one); a coroutine library's own switch **onto a stack inside the alternate signal stack's
memory** (`sigaltstack` carved from the thread's stack, which rules 4 to 7 read); the same shapes at `-O0` (the
draft ran Linux `--sanitize` at `-O0` and `-O2`, the clerk's cells name no level).

## H. What is missing, and which seat was handed a fact it cannot check

- **The ffi-pragmatist cannot answer its question 6 and part of its question 1.** Its tools are Read, Write, Bash
  (`.claude/agents/ffi-pragmatist.md`), no page reader; the sitting forbids a download; Boost's assembly is not on
  this Mac (F, third bullet); the Linux images hold no Boost. So *"Boost.Context's Windows switch ... read the source
  you can name"* and every library it *"cannot build here"* (libco, corosensei, greenlet) can only be read by the
  **historian**, whose brief asks the same Windows question (its question 2). One seat should own it, and the
  ffi-pragmatist's version should say *read from the historian's sources, or unread*.
- **The compiler-engineer is asked for `__builtin_longjmp` "where it compiles"** (its questions 4 and 5), and holds
  this Mac and Linux arm64, where clang refuses `__builtin_setjmp` (panel 258's ffi table, its line 58). It compiles
  on x86-64 alone, the ffi-pragmatist's image, and panel 258 left that door **unrun** there (its line 407). The run
  needs an owner on x86-64.
- **`shapes/cost/cost.sh` cannot run as copied**: it sets `S=.../lane-coroutine-room-scratch` and builds with
  `$S/heroes-base`, `HEROES_RUNTIME=$S/base-runtime` and `$S/../lane-coroutine-room/runtime` (`cat cost.sh`), another
  lane's folders, which § 2 says the sweep takes when idle (they stand today: `ls -d` finds all three). A seat must
  point `S`, the compiler and both runtimes at its own copy; the brief should say so.
- **`heroes-next measure <applied copy>` only works from the repository's root** (F17); the warden's brief writes
  it as if run anywhere.
- **The shared brief's freeze is stale by one lane.** At `8608fa9a2` (`git log --oneline -1 main`), `git diff
  --name-only 2cf85ea64 main -- spec runtime seed selfhost tests` adds, beyond § 2's seven runtime files,
  47 files of `selfhost/` (41 of `emit/`, 5 of `cli/`, 1 of `fetch/`), `selfhost/emit/large_value.hero` among them
  (one line, `NULL` to `((void *)0)`, `git diff`), and 59 of `tests/`, the emissions of the 2752 and 2753 cases among
  them; `callback_thunk.hero`, `frame.c`, `lend.c`, `callback.c`, `stack_lane.c` and `hero_os.h` did not move.
  `large_value.hero` is the compiler-engineer's question 9 file, and `heroes_runtime.h` (+86 and -2 on `main`) is where
  a route-D entry point would be declared; a prototype built at the base meets them at its merge. And the **dlsym-make lane** will move `unseen_carved`'s and
  `ph`'s Linux rows (F10, F16): the seats should name the base in every Linux cell.
- **Panel 257's real-library evidence is not pointed at** (F6): `asyncjob`, `sched`, `bctx_signal`, `n6`. The
  ffi-pragmatist is asked to bind Boost again from panel 257's tarball; the brief should also hand it `asyncjob.h`
  (a real library's `makecontext` switch) and panel 257's census of the Mac's 422 dylibs.
- **Defects 2755 and 2705** (F5) belong in § 4.6 and in the compiler-engineer's question 2: the § 5 Linux finding is
  their class (false words about a handler), and its filing should name them.
- **The x86-64 cells of the carved shapes and of the blind programs**: § I below.
- **Nobody is asked to run the blind programs on Linux x86-64**: the task says *"both 64-bit Arm"*, so that is in
  scope by design; I say it only so the key is not read as covering x86-64.

## I. Linux x86-64 (`heroes-linux:latest`, emulated)

**F22. The compilers.** In a container of `heroes-linux:latest` (`p259-critic-2`; `uname -m` prints `x86_64`, `ldd
--version` *Debian GLIBC 2.41-12+deb13u3*, clang 22.1.8, emulated on this Mac: docker warns the image's platform
`linux/amd64` does not match the host's), from the same `git archive`, the seed's compiler and stage 2 both exit 0
and print `sources 05186f2b38e77a97` and `sources 32437b681fe95f82` (05:42 to 06:14 UTC). One caveat for every cell
below: **emulated**, not the CI's native x86-64 leg.

**F23. `own.h`'s x86-64 arm and `unseen_carved`, three runs a cell (07:15 to 07:20), the cells nobody had run.**

| shape | plain | `--sanitize` |
|---|---|---|
| `own_carved` | **exit 139, 3 of 3**, nothing of the program's printed (19 bytes, the shell's `Segmentation fault`) | **exit 1, 3 of 3**: `heap-use-after-free ... READ of size 8`, `#0 ... in h_owncarved_body /w/runs/own_carved.hero:81:76`, `#2 ... in ow_host /w/runs/./own.h:77:17` |
| `unseen_carved` | **exit 134, 3 of 3**, 823 bytes, **equal by `cmp` to the arm64 output**: `HERO_SIGNAL_REENTERED`'s words, then the remedy | **exit 134, 3 of 3**, the same bytes |

So x86-64 is Linux arm64's row in both shapes: the defect is not an AArch64 artefact of `own.h`, and the false
handler words are glibc's door on both Linux architectures.

**F24. `below` on x86-64 (07:23 to 07:25)**: plain exit 139, 3 of 3, nothing printed; `--sanitize` exit 1, 3 of 3,
`heap-use-after-free ... WRITE of size 262144`, `#1 ... in h_below_e5 /w/mine/below.hero:67:309`, `#2 ... in
h_below_run /w/mine/below.hero:72:134`. The arm64 row again.

**F25. The seven blind programs on x86-64, three runs each (07:28 to 07:31)**: every output equal by `cmp` to its
arm64 output, three of three: q1, q2, q3, q6, q7 exit 0 with the Mac's lines, q5 exit 134 with the same panic, **q4
exit 139 with nothing printed**. The key's Linux row is one row for both architectures; the split is Darwin against
Linux.

## What the coordinator must repair before any seat starts

1. **§ 5's Linux reading, and § 6 A's datum, from the trace (F11).** The judgment that refuses `unseen_carved` on
   Linux is rule 6, `hero_sig_reenter` (`sigentries.c:379-384`), on coroutine 0's **yield**, a `swapcontext` the
   runtime saw landing **below** the frame making it on the one range it knows (the thread's 8 MiB), told at
   coroutine 1's first thunk entry. It needs the switch seen, so it **cannot reach `own_carved`** (whose trace shows
   no switch) on any platform. Replace *"a landing above the thread's current frame"*; name defects 2705 and 2755 as
   the class of its words (F5). § 5's tables gain the x86-64 row (F23): Linux arm64's cells, byte for byte for
   `unseen_carved`, so *"x86-64 is unrun"* goes.
2. **The blind registration, before anything else is drawn** (F14, F19): q4 splits under `now` (Mac `WRONG then
   ABORTS`, Linux arm64 and x86-64 alike a silent exit 139, F25), so D1's `now` half has no graded row by `answers.md`'s own rule, and
   `task-t1.md`'s *"on a Mac and on Linux alike"* is false of it; q4's key is two verdict words; a *may abort* text
   (`arm-r`, `arm-t`) is graded against `NOT PROMISED`'s rule so that D2 (q7 is in the arms' scope) and D1 fail by
   construction; `task-t2.md` points at q4, which a t2 session does not hold; `brief.md` and the spec's file name
   disagree between the tasks, `llm-ergonomist.md` and panel 256's runner; design C's key header is unregistered.
3. **Every seat's shape table gets `below`** (F21: two coroutines on a stack carved from the open extern call's own
   frame, no Heroes value held across it; Mac 134 *compiler bug*, Linux arm64 and x86-64 139, a use-after-free under
   `--sanitize` on all three, F24). It is the
   shape that separates route C's *"stack pointer of the extern call that is still open"* from a detector.
4. **§ 4.2's Linux door is the handle** (F12): `RTLD_DEFAULT` is answered with the runtime's `makecontext` (the
   executable exports it), `RTLD_NEXT` and libc's own handle are not; this Mac answers all. Say it, and say the
   dlsym-make lane will move `unseen_carved`'s and `ph`'s Linux rows, so every Linux cell names the base.
5. **Give each unrunnable question an owner** (H): the Boost Windows switch and the libraries nobody can build here
   to the historian alone (no source on this Mac, no Boost in either image, no page reader in the ffi seat);
   `__builtin_longjmp` (compiles on x86-64 only, panel 258's unrun door 6) to whoever holds x86-64; `cost.sh`'s
   hard-coded lane paths repointed at each seat's own copy; `measure` of an applied copy run from the root.
6. **Route D's sanitizer variant reaches `--sanitize` builds alone** (F, fourth bullet): Boost's default backend
   makes no sanitizer call, its `ucontext` backend makes it around a `swapcontext` the runtime already sees, and the
   tree's header makes it only under the sanitizer. State it in § 6 D so no seat builds on the plain-build case.
7. **§ 4.3's *"3 of 13 doors with no event"*** becomes the table's reading (F, sixth bullet): 8, 12, 13 abandon
   unseen with the process running on, 9 and 10 end the thread, door 6 on x86-64 unrun; route B is priced on those.
8. **§ 4.6 adds 2752, 2753, 2754, 2755, 2705 and 2811** (F5); **§ 4.7 and the ffi brief point at panel 257's
   `asyncjob`, `sched`, `bctx_signal` and `n6`** (F6); **§ 4.7's Boost-on-Linux question is answered: absent**.
9. **§ 2's freeze names `main` at `8608fa9a2`**: `selfhost/emit/large_value.hero` (question 9's file) and
   `runtime/heroes_runtime.h` moved there (H).
10. **The routes nobody listed** go into § 6 G as questions (G1 retire-not-reuse with the lend regions' precedent, G2
    a static tell on the callback parameter, G3 the handle, G4 a hardware shadow stack, measured absent here, G5 a
    cookie in the taking frame), and the shapes beside: `spawn_sized`, a `sigaltstack` carved from the thread's
    stack, the optimisation level of every cell.
11. **Small**: `own.h`'s `#error` is line 49, not 54 (F3); the lend regions' give returns silently where the room's
    stops (F1), which matters for what a seat expects a lend shape to print.

Finished 2026-11-25 07:32 by this Mac's clock (`date`). No container of mine is left (`docker ps -a | grep p259-critic`
prints nothing); every file I wrote is under `.claude/worktrees/259-critic-scratch/` and this report.
