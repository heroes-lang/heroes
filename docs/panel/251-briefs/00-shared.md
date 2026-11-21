# Panel 251, sitting A of M-rebel-library: threads and signals. The brief every seat reads first

Convened by the coordinator of M-rebel-library on 2026-11-19, as the milestone page's item 33
(`docs/roadmap/milestones/M-rebel-library.md:257-276`, read at the frozen commit): *"**A, threads and
signals**: 251 (a Heroes function a C handler reaches, defect 2522), 252 (the end of `main` and the
threads still running, defect 2552), and threads that share nothing (decision 32); their briefs are A's
input."* **Nothing sat is reopened**: sittings 251 and 252 of 2026-11-15 wrote briefs and a critic's
first reading and no seat's report, their seats stopped by the usage limit (item 33's own words), and
none of their files was committed (`git ls-files docs/panel | grep -c -E '^docs/panel/25[12]'` prints 0
at the frozen commit). Their briefs are this sitting's **input, never its source of facts**: every number
below was run again at the frozen commit, and § What moved says what changed.

**The full panel** (`.claude/skills/panel/SKILL.md:25-34`): question 1 may widen a diagnostic class
(below, the escape), question 2 puts a sentence into spec § 13 and changes a runtime message's class,
question 3 changes what crosses into a thread, a surface and a spec section. **The seats**: the
compiler-engineer, the ffi-pragmatist, the spec-warden, the historian (advisory) and the llm-ergonomist
(the blind seat: 50 graded sessions and one verdict session the coordinator runs by
`run.sh` from `.claude/worktrees/rebel-scratch/251-llm-ergonomist/`, `llm-ergonomist.md`). **The
completeness critic reads these briefs before any seat starts** and the reports after
(`SKILL.md:269-273`); its first reading is `docs/panel/251-reports/critic-briefs.md`, and § Repairs after
the critic's first reading says what each of its findings changed. **The paid-run bound: 10 USD for the
sitting** (the author's bound, given 2026-11-19, as the coordinator relays it; the author's words are not
in the tree): **the blind seat's sessions, each capped at 0.40 USD, the runner stopping before 9.50 in
all; and the spec-warden's `heroes measure --refresh` calls, at most eight, counted at 0 USD inside the
bound** (the coordinator's decision of 2026-11-19 on the critic's finding 6: `count_tokens` is free by
Anthropic's page as panel 252's brief read it, carried, `p252/docs/panel/252-briefs/00-shared.md:333-339`).
**No other paid run, by anyone**: no `claude -p`, no other API call, no cloud run; one a seat finds worth
running goes in its report with its size, for the coordinator (`SKILL.md:288-295`).

**Where this brief is wrong, say so first.** Written by the coordinator's brief-writer on 2026-11-19,
finished at the time at its foot (`date`); its first clock reading was `uptime`'s 09:08. Every number, path and count below carries the command
that produced it, run while writing; every output is under
`/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/rebel-scratch/251-briefwriter/` (called `bw/`
below), read-only to every seat. A figure from an earlier sitting, a report or an issue says *carried*
with its file and line, and is yours to re-run before you lean on it. Paths are from the repository's
root, `/Users/joseph/Temp/heroes/heroes-lang`, and a seat reads the frozen tree through the worktree
`.claude/worktrees/rebel-round` or its own copy.

## The frozen commit

**`3a5ee269117a31780ef1f96d0305452630ac4d83`** (`git -C .claude/worktrees/rebel-round log -1 --format='%h %ad %s' --date=iso`:
*3a5ee2691 2026-11-19 08:49:25 +0100 M-rebel-library step 52: S3*), on branch `rebel-round`. **It is not `main`**: `origin/main` is `0becec5b6`
(*"Batch 39 round closed ..."*, 2026-11-15 23:30:55 +0100), the branch forked from `f6c49d84c`
(`git merge-base origin/main HEAD`), and `origin/main` holds three commits the branch does not
(`git log --oneline HEAD..origin/main`: `0becec5b6`, `4faab013f`, `3c04bfd2b`, the emissions blessed,
three citations, batch 39's close). The worktree's `git status --short` printed 28 lines when the
writer finished, every one a file of `packages/rebel/` or `examples-rebel/` (the coordinator's next
step, uncommitted): **you judge the commit, never the working tree**, so your copy is made by
`git archive`, which reads the commit alone.

**The two stages at the frozen commit**, built by the writer in `bw/tree/` (a `git archive` of
`3a5ee2691`, 20,022 files by `find bw/tree -type f | wc -l`, the archive's own count by `git archive
... | tar -t | grep -v '/$' | wc -l`): the seed by `clang -I runtime seed/heroes.c runtime/runtime.c -o
heroes`, exit 0, `real 12.08` (`/usr/bin/time -p`, load 4.05), **`sources 5aae00294f7cd6fa`**, the
seed's key unchanged since 2026-11-15; stage 2 by `./heroes build selfhost/main.hero -o heroes-next`,
exit 0, `real 151.04`, `user 113.32`, `sys 29.24` (load 4 to 6, other sessions building), **`sources
23c7d2a626b29fd5`** (`./heroes-next --version`, `bw/stage2.log`). **A seat whose stage 2 prints another
`sources` line is not on the frozen commit and says so first.** Apple clang 21.0.0 (clang-2100.4.1.1),
macOS 27.2, Darwin 27.2.0 (`clang --version`, `sw_vers -productVersion`, `uname -r`).

## What moved since the 2026-11-15 briefs (their base `cc83b4d91`)

`git log --oneline cc83b4d91..HEAD | wc -l`: 68 commits. `git diff --stat cc83b4d91 HEAD -- runtime/`:
**38 files, 39,270 insertions, 71 deletions**, of which **30 files are new** (`git diff --name-status ...
| awk '$1=="A"'`): the platform layer's fourteen new members and their headers (`hero_crypto`,
`hero_disk`, `hero_ipc`, `hero_keyring`, `hero_odbc`, `hero_poll`, `hero_proc`, `hero_serial`,
`hero_stream`, `hero_stream_net`, `hero_sys`, `hero_term`, `hero_tls`, `hero_watch`) and
`platform/hero_handle_tag.h`, `platform/hero_layer_alloc.h`. `selfhost/`: 7 files, 289 insertions, 14
deletions, all under `selfhost/fetch/` and `selfhost/module/`. **`spec/heroes-spec.md` and
`seed/heroes.c` did not move** (`git diff --quiet cc83b4d91 HEAD -- <path>`, exit 0): the spec is 492
lines, 29,407 bytes, sha256 `1d532d514831aa70...` (`wc -l -c`, `shasum -a 256`).

**Unchanged since `cc83b4d91`, so every line the old briefs cite in them stands** (`git diff --quiet
cc83b4d91 HEAD -- <file>`, then `wc -l`): `runtime/parts/sigguard.c` 371, `sigdoor.c` 137, `thread.c` 144,
`spawn.c` 664, `spawn_str.c` 107, `lend.c` 985, `frame.c` 157, `run.c` 2,559, `panic.c` 159, `str.c` 707,
`runtime/hero_os.h` 555, `runtime/hero_compiler.h` 109; `selfhost/emit/callback_thunk.hero` 375,
`c_main.hero` 214, `printing.hero` 98, `selfhost/check/acquiring.hero` 325, `moved.hero` 145, `ffi.hero`
412, `selfhost/cli/flags.hero` 584, `link_flags.hero` 215, `supplied.hero` 357, `link.hero` 855,
`selfhost/main.hero` 329; `tests/harness/suite_run.hero` 878, `suite_runtime.hero` 1,102,
`freed_bytes.hero` 183.

**Moved, and the old briefs' lines in them with it**:

- **`runtime/parts/alloc.c` 1,548 to 1,585**: `hero_layer_alloc` and three siblings inserted at 619-655,
  the platform layer's memory, *"deliberately no pair of the exit gate's ... a count read at exit would
  accuse a correct program"* (its comment, 619-628). Lines before 619 stand: `hero_live_blocks` 85, the
  premise *"read once, at exit, after every thread has been joined"* 79-80, the gate
  `hero_runtime_check_leaks` 306, its block message 311-312, the scratch message 356, the lease message
  371. After it, +37: `hero_alloc_block` 673, `hero_release_block` 679, the thread-exit destructors
  `FlsAlloc` 1524 and `pthread_key_create` 1535 (`grep -n`).
- **`runtime/parts/stack.c` 1,377 to 1,379**: a comment at 534-536 (defect 2571), every later line +2:
  `hero_stack_call_prior` 743, `hero_stack_foreign` 796, `hero_stack_prior_recovered` 823, the guard's own
  install `sigaction(SIGSEGV, &sa, &hero_stack_prev_segv)` 1220, `sigaltstack` 1175, the comment naming
  `siglongjmp` 810 (`grep -n`).
- **`runtime/parts/os.c` 1,019 to 1,140**: defect 2571's lease handler now calls the handler found
  before it and goes on where that handler edited the register file (`hero_lease_call_prior` at 392,
  `hero_lease_prior_recovered` at 437), defect 2584's sender line; `hero_exit` at 1126, its `exit` at
  1131. **Defects 2571 and 2584 read `OPEN`** (`gh issue view <n> --json state`), their repairs merged at
  `504b59c77` and `981338f52`.
- **`runtime/runtime.c` 321 to 355**: `#include "platform/hero_layer_alloc.h"` at 183, so `parts/alloc.c`
  moved from 181 to 187; `stack.c` 154, `sigguard.c` 161, `sigdoor.c` 166, `thread.c` 171, `spawn.c` 177,
  `spawn_str.c` 197 (`grep -n '#include "parts/'`).
- **`runtime/platform/hero_net.c` 4,427 lines** (+2,178 -25): the poller's detached thread is
  `pthread_attr_setdetachstate` 1267 and `pthread_create` 1269, Windows' `CreateThread` 1102 and its
  `CloseHandle` 1105, the console handler 1401.
- **The runtime's own signal lines, by 249's two commands** (`docs/panel/249-briefs/00-shared.md:520-525`,
  re-run with `git grep` at both commits): **33 install lines, 27 at `cc83b4d91`**, six new lines in
  `platform/hero_proc.c` (1628, 1630) and `platform/hero_term.c` (247, 258, 272, 278), **three of them
  installs** (`hero_proc.c:1630`, `hero_term.c:258`, `:278`) **and three reads**
  (`sigaction(sig, NULL, &...)` at `hero_proc.c:1628`, `hero_term.c:247`, `:272`; `sed -n`, the critic's
  finding 1); **27 mask lines, 18
  at `cc83b4d91`**, the new in `hero_proc.c`, `hero_watch.c` and `hero_stream.c`.
- **Threads the runtime starts that are not in the spawn table** (`grep -n -E
  'pthread_create|CreateThread|dispatch_queue_create|PTHREAD_CREATE_DETACHED' runtime/parts/*.c
  runtime/platform/*.c`): `hero_net.c`'s poller (detached, 1267-1269; Windows 1102-1105), `run.c`'s launch
  watcher (detached, 1469-1470), **new**: `hero_proc.c` 684-694 (detached), `hero_watch.c` 1258 (a
  joinable `pthread_create`), 1736 (a dispatch queue, Darwin) and 2161 (`CreateThread`), and
  `hero_tls.c`'s two dispatch queues, 3812 and 5082 (Network.framework, Darwin).
- **The runtime now registers a process exit handler**: `runtime/platform/hero_term.c:170`,
  `atexit(hero_term_at_exit)` (`grep -rn -E 'atexit\(|at_quick_exit|__attribute__\(\(\s*destructor'
  runtime/`, comment lines dropped: that one line), new since `cc83b4d91`, for a program that links the
  terminal member; sitting 252's *"the runtime registers no exit handler"* is false of the frozen commit.
  `grep -rn -E 'flockfile|funlockfile|_lock_file|_unlock_file' runtime/` still prints nothing.
- **The setjmp family in the two trees** (the 2026-11-15 brief's command,
  `/usr/bin/grep -rn -E '\b(_?setjmp|sigsetjmp|__sigsetjmp|_?longjmp|siglongjmp)\b' selfhost/ runtime/`):
  **six lines, not five** (sitting 251's critic's correction, re-run): four header names
  (`selfhost/cli/order_search.hero:49`, `shadowed_library.hero:68`, `:134`, `own_names.hero:335`) and two
  comments (`runtime/parts/stack.c:810`, `os.c:451`); no call of the family in either tree.
- **Goldens**: `tests/golden/run/` holds **811** `.hero` cases (806 on 2026-11-15; `ls | grep -c -E
  '\.hero$'`); `fixedbugs-2558-*` 5, `fixedbugs-2494-*` 14 `.hero` and the folder case
  `fixedbugs-2494-a-signal-one-module-watches-another-module-takes.case`.
- **The seats' evidence of 249 stands**: `.claude/worktrees/p251-scratch/ev249/` holds 3,232 files, 28 MB
  (`find -type f | wc -l`, `du -sh`), and ten of its case files re-hash to the 2026-11-15 brief's values
  (`shasum -a 256`, first sixteen: `ij.hero` `24374aa277a83647`, `lib/ij.c` `a44017d7e4a35cb8`, `ijv.c`
  `7e944fa4ab57cb76`, `depth.h` `b95fce1e97b154d6`, `kmiss.h` `48ad7d61bca44e05`, `kmiss.hero`
  `46b981a697c4022f`, `rebind.c` `0281ee1c166636cb` 86 lines, `linux/run.sh` `272a4b05b40b250d`,
  `asanprobe/q.c` `9332433233c2ecdd`, `qsortcost.hero` `8548129a957d3ca5`; nine `dp-*.hero` under
  `249-compiler-engineer/depth/` and three under `amd64/bpship/`).
- **The route overlays had to be ported again**: their `runtime/runtime.c` is a whole file of
  `cc83b4d91`, and laid over the frozen runtime it **drops `#include "platform/hero_layer_alloc.h"`** in
  silence (it still passes `-fsyntax-only`, `bw/routes/check/stale/syntax.txt`, empty). The writer
  re-ported each (`diff -u` of the overlay's `runtime.c` against `git show cc83b4d91:runtime/runtime.c`,
  applied by `/usr/bin/patch --verbose` to the frozen `runtime.c`: one hunk each, `succeeded at 159`, 158
  for A, no fuzz, `bw/routes/<route>-patch.log`). **Use `bw/routes/<route>/`, never
  `p251-scratch/briefs/inputs/routes/`**: `bprime`, `bsecond`, `drebind`, `a-sigrtmax`, `c-word`
  (`bw/routes/routes-manifest.txt`, `shasum -a 256` and `wc -l`; `sigguard.c`, `siginstall.c`,
  `rebind.c`, `hero_compiler.h` and `flags.hero` byte-equal to 2026-11-15's, their base files unchanged).
  Each, laid over the frozen `runtime/`, passes `clang -fsyntax-only` with `flags()`'s nineteen words
  (`selfhost/cli/flags.hero:133-153`) at `-O0` and `-O2` with no diagnostic, and so does the frozen
  runtime itself (`bw/routes/check/<route>/syntax-O0.txt` and `-O2`, empty). **Built into a program and
  run by nobody at the frozen commit.**

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

## Question 1: what is asked, and its record

**Defect 2522** (`gh issue view 2522 --json state,labels,milestone,comments`, `bw/gh/issue2522.json`):
`OPEN`, `kind:defect`, `class:blocking`, `area:runtime`, milestone `M-core-packages`, three comments
dated 2026-11-14T10:52:00Z, 2026-11-14T20:31:50Z and 2026-11-15T08:29:48Z, none since. **Decision 2563**
(panel 249) `CLOSED 2026-11-15T12:22:56Z`, *"Settled by fd37b4bfd..."*. **What binds**: 249's R3, the
order (`docs/panel/249-*.md:253-282`): route J built first by a seat with Bash, then each candidate's
owed runs, then question 3 *"(how a residue is told, and where the specification says it) with the
spec-warden and the blind seat, after the author's reading of route K"*; the author's reading (`:551-559`):
R1 to R4 ratified, **route K answered *no, for now***, 247's R3 standing (*"Never a known cost"*), the blind
seat bounded at 10 USD. **This sitting is R3's items 1 to 3 at once**, the full panel seating what 249's
lane lacked (the historian for D-rebind's forward compatibility and Ubuntu 26.04's glibc, sitting
251's critic's item 1; the spec-warden and the blind seat for question 3).

**The CI at the newest `main`**: run 43769808533 at `0becec5b6`, `completed failure` (`gh run list
--branch main --workflow ci.yml --limit 4`); its Linux x86-64 job 132982964177 reads **`run: 830
passed, 3 failed`** (log line 4116), the three defect 2494 cases **at `-O2`** (lines 4319, 4330, 4341:
`...-atexit-function-a-c-handler-reaches-through-exit-is-refused`, `...-a-c-handler-calls-is-refused`,
`...-reaches-through-fork-is-refused`), the shape 2522's first comment names (`gh api
--allow-escape-sequences repos/heroes-lang/heroes/actions/jobs/132982964177/logs`, 477,159 bytes,
`bw/ci/`). Its Windows x86-64 job and the Windows net's part 2 read `failure` too (not read further: not
this sitting's). **The same case on this Mac at the frozen commit**, the writer's stage 2, its header
renamed `signal_lib.h` (`bw/t3/`): `installed: 0`, then R1's refusal on stderr, **exit 134, 5 of 5 at
`-O0` and 5 of 5 at `-O2`**.

**The guard at the frozen commit**, every line the 2026-11-15 brief named (`sigguard.c` unchanged):
`HERO_SIGNAL_WALK 8` at 163, `hero_signal_refuse` 305, `hero_callback_entered` 323, `if (plain == 0 ||
own == NULL) return;` 333; the thunk's first line built at `callback_thunk.hero:147`; spec § 13 opens at
376, its sentence *"No function of the program is a signal handler"* on 404.

**The runtime's own calls a route meets** (sitting 251's critic, items 11 and 5, carried and re-read):
the child a launch forks calls `sigaction(SIGXFSZ, &ignore, NULL)` between `fork` and `exec`
(`run.c:1805`, defect 2531, closed), so a route's `sigaction` runs in a forked child and must be
async-signal-safe there (no lock, no allocation, no first `dlsym`); the stack guard's own install
(`stack.c:1220`) and the prior it saves (called at `stack.c:743-746`), defect 2571's prior-handler call
in the lease handler (`os.c:392`, `:437`), and the three new installs and three reads of `hero_proc.c`
and `hero_term.c` all go through whatever the route defines (Linux) or rewrites (Darwin). **And a second
forked child** (the critic's finding 1, re-read): `hero_proc_child` (`platform/hero_proc.c:1603`) reads and
may reset the disposition of every signal in a loop, `sigaction(sig, NULL, &now)` at 1628 and
`sigaction(sig, &plain, NULL)` at 1630, between `fork()` (1671) and `execve` (1633): up to twice
`NSIG - 1` calls of the route's `sigaction` in a forked child. That child is taken only where
`hero_proc_spawn_enters()` (1438) answers false and a folder is named (1777-1779), which by the critic's
reading of 1438-1449 is Darwin before 10.15 or a C library neither Darwin's nor glibc 2.29 or later: no
image here takes it by itself, so a building seat reaches it in code (a build with
`hero_proc_spawn_enters` forced false, a second thread holding a lock the route takes at the fork). **Where J's pointers to the C
library's own save points and jumps are resolved** (B' takes `sigaction` in a constructor,
`bw/routes/bprime/runtime/parts/siginstall.c:55`, `:63-65`; A calls `dlsym(RTLD_NEXT, ...)` at every
jump, `bw/routes/a-sigrtmax/runtime/parts/siginstall.c:113`, `:119`, `:126`, inside a handler) is a
question each building seat answers in code.

**The escape is this sitting's to judge**: J's Linux definitions of `setjmp`, `_setjmp` and
`__sigsetjmp` are three more C library names the runtime would define, which panel 239's
`ffi_defined_elsewhere` (`selfhost/cli/supplied_words.hero`, `grep -rn -l`) would then refuse a linked
library for defining: **a widened diagnostic class, unrun**.

**The cases**, all in `.claude/worktrees/p251-scratch/ev249/` (read-only; copy into your folder): the
2026-11-15 brief's table stands, re-hashed above (`p251/docs/panel/251-briefs/00-shared.md:316-327`,
uncommitted, readable in the worktree `.claude/worktrees/p251`): `injump`, the three shapes beside it
(`ijv.c -DHOW=1,2,3`), the fortified Linux shapes (`critic/reports/linux/run.sh`), the twelve depth
programs (must exit 0), `kmiss` (must be refused), 247's `flib` and `fliblocale` static and through a
shared library, D-rebind's own programs. **The goldens a route through the family must keep**:
`fixedbugs-2558-*` (5), `fixedbugs-a-c-handler-that-recovers-leaves-no-line`, `fixedbugs-2494-*` (15).

**J's holes, each to be named and run where a seat can** (the 2026-11-15 list, and sitting 251's critic
added the `dlsym`ed save point): a buffer copied by `memcpy`; a `setjmp` the C library makes inside itself
(two direct calls to `_setjmp` in each Linux image's `libc.so.6`, carried, `p251-scratch/briefs/linux/
probe4-*.log`); the inline `__builtin_setjmp` pair on x86-64 (cross-compiled by Apple clang only,
carried); the context family; a target and entries on different stacks; a C++ exception or `_Unwind_*`
out of a handler; a save point or `sigaction` reached by `dlsym`; ASan's weak interceptors of the jumps
under `--sanitize` (read in Linux arm64's archive alone, carried); `musttail` measured to stay a branch
under `flags()`, **compiled and never run** (carried).

## Question 2: what is asked, and its record

**Defect 2552** (`bw/gh/issue2552.json`): `OPEN`, `kind:defect`, `class:blocking`, `area:runtime`,
milestone `M-core-packages`, **no comment**; its class line: *"What is owed is a ruling before a repair:
what the end of `main` owes a thread still running (join it, refuse to end, or end it and say so), or how
the leak check tells a running thread's blocks from a missing decref."* **Defect 2582** (two threads'
`print`s tear a line) `OPEN`, `class:adjacent`, filed by sitting 252's writer; sitting 252's critic read
it `blocking` by `.claude/rules/verification.md` § Bounded discovery and its repair the runtime's (one
lock of the stream around each `print`, carried, `p252/docs/panel/252-reports/critic-briefs.md:196-213`).

**Measured at the frozen commit** (the writer's stage 2, `bw/repro/`, each binary under `timeout -s KILL
60`, its output through `head -c 300`, `pgrep -fl 251-briefwriter` empty after every batch):

| program (copied from `p252-scratch`, header saying what it asks) | runs | outcome |
|---|---|---|
| `unjoined.hero` (worker holds a `str`, `main` returns after 0.2 s) | 20 | **20** exit 134, *"panic: 2 heap blocks still live at exit (a missing decref) — this is a compiler bug"* |
| `unjoined_str.hero` (the `str` door) | 20 | **20** exit 134, the same line |
| `unjoined_noheap.hero` (worker holds numbers only) | 20 | **20** exit 0, nothing on stderr: the thread ended in silence |
| `cr-late.hero` (sitting 252's critic: the worker builds its `str` after 0.5 s) | 20 | **20** exit 0, nothing on stderr |
| `cr-lease.hero` (the worker holds a lease it would end) | 10 | **10** exit 134, *"panic: 1 lease(s) never ended — every `.lease()` owes one `end_lease`, and this program is missing that many"*: a false message of the program |
| `finished_unjoined.hero` | 5 | 5 exit 0 |
| `exit_running.hero` (ends by `exit(code: 0)`) | 5 | 5 exit 0: `exit` skips the gate (`hero_os.h:145-151`) |
| `t1.hero` (the blind task of sitting 252) | 10 | **10** exit 134, the block line |
| `t3.hero` (a worker that never ends) | 5 | 5 exit 0 at once |
| `t2-ref.hero` (joins; both threads print) | 100 | 74 `main done/sum 15/`, 26 `sum 15/main done/`, **0 torn** this morning (load 5.89; sitting 252's critic read 27 of 200 torn at load 4.5 to 5.7) |
| `t2-trap.hero` (no join) | 30 | 19 exit 0, **10 exit 134** with *"1 heap blocks ... compiler bug"*, **1 torn** (`sum main done15//`) |

So **the gate's verdict is what the running thread holds at the instant `main` returns**, both ways
deterministic (`unjoined` against `cr-late`), and Q-gate is every balance the gate reads, leases and C
handles too, not the block count alone.

**And the library already tried the typed handle, measured**: Rebel's `threads` makes a started thread a
record holding a life (`packages/rebel/0.1.0/files/threads.hero:1-20`, *"A thread never waited for stops
the program when `main` returns, saying how many"*). Two programs written by the writer in its copy's
`examples-rebel/` (`bw/tree/examples-rebel/bw_unwaited.hero`, `bw_unwaited_done.hero`, the catalogue's
`path` entry): a **finished** thread never waited for, 10 of 10 exit 134, *"panic: 1 C handle(s) never
given back ... 1 taken at <rebel>/threads/internal/lives.hero:18:12, in lives.begin"* (the place inside
the package, as its face says); a **running** thread holding a `str`, **10 of 10 exit 134 with *"2 heap
blocks still live at exit (a missing decref) — this is a compiler bug"***: the gate reads the blocks
before the handles, so the package's rule never speaks for the case 2552 is about.

**The record** (re-read): spec § 13's T1 (line 377); the lease sentence *"a lease nobody ends, like a
handle nobody consumes, aborts when `main` returns, saying how many"* (449-450); § 6's abort (183); §
11's `exit(code:)` (362, the one status the spec names, 0 to 255); the stop (404). design.md §1.12 at
621, Part 7 item 13 at 4085-4156. **Rulings near it, found by sitting 252's critic and re-read**: panel
248 did not adopt a join that does not wait (`docs/panel/248-*.md:306-309`); panel 246's historian read
Java's *"live non-daemon threads"* rule (`246-reports/historian.md:422`); panel 246's blind reader assumed
*"abort when `main` returns"* for a forgotten join (`246-reports/llm-ergonomist.md:1766`) and raised
joining handles kept in an array (`:2110`); panel 248's wrote *"how running threads behave when `main`
returns is undocumented"* (`248-reports/llm-ergonomist.md:834`); panel 111's two holes of a per-thread
check (`docs/panel/111-*.md:182-188`). **Panel 232's promise** covers *"the functions `hero_os.h`
declares at 1.0.0"* (`docs/panel/232-*.md:272-274`), so before `1.0.0` CLAUDE.md § 2 (line 136) rules
and the doors' types may move; `hero_os.h:481-482`'s *"whose type does not move (panel 232's
promise)"* reads it as binding now: a reading for the spec-warden to confirm.

**Programs a ruling moves** (`find bw/tree -name '*.hero' | xargs grep -l -E
'hero_thread_spawn(_str|_sized)?\('`, `bw/spawners.txt`): **38 files** (37 on 2026-11-15): 11 under
`examples/`, 4 under `selfhost/` (`main.hero:101` runs every command on a sized thread), 21 under
`tests/golden/`, 1 under `tests/harness/`, **1 new under `packages/rebel/`** (`threads/hero_os.hero`, the
door); and **22 files call Rebel's `threads.spawn`, `spawn_text`, `spawn_sized`, `run_all` or
`run_all_text`** (`bw/rebel-spawners.txt`: 21 under `packages/`, 1 under `examples-rebel/`). Which of
them leaves a thread unjoined on some path is unrun.

## Question 3: what Rebel could not write, each witness by its ledger block

**Decision 32** is the milestone page's item 32 (`M-rebel-library.md:241-256`), whose last sentence is
the question: *"a server serving connections in parallel rebuilds its router in each thread and shares no
connection, cache or session (`mail/smtp` refused a pool for it; `web` and `http` said it), the limit a
web application with traffic meets first, which only the language can decide."* **No GitHub issue was
found for it**: `gh issue list --state all --search` for *"share nothing"*, *"shares nothing"*,
*"threads that share"*, *"one value per thread"*, *"sitting A"*, *"threads and signals"*, *"panel 251"*,
*"panel 252"* found none on point (`bw/gh/`); the issues that rule what crosses are **decision 2498**
(panel 246, `CLOSED 2026-11-13`: the rung widened to one `str`, copied) and **feature 1073**
(`CLOSED 2026-11-13`, *"only an `i64` crosses into a thread"*). *"Rebuilds its router"* is the page's own
word: `grep -rn -i -E 'rebuil|router.{0,60}thread|thread.{0,60}router'` over Rebel's faces and ledgers
finds no ledger saying it.

**The census** (`bw/tools/ledger_threads2.py`, run with `python3 -I` over `bw/tree/packages/rebel/0.1.0/
files`: every block of the 144 ledgers, `find -name '*.ledger' | wc -l`, whose `heroes` or `note` line
names a thread, sharing, one `i64` or one `str`, or spec § 13; **86 blocks, 38 refused, 9 pending, 39
kept**, `bw/ledger-threads2-head.txt`; and a second grep of the ruling lines for *background*, *beside
the program*, *timer*, which added `nats` and `websocket`). Each witness by ledger, lines, capability,
and its ruling's words:

*A value of any type cannot cross* (one `i64` or one `str`):

| ledger:lines | capability | the ruling |
|---|---|---|
| `bench.ledger:150-154` | bench/parallel, refused | *"a thread of the language runs a function of one `i64` or one `str` (spec § 13), so a work of any type cannot be run on many threads"* |
| `compress/lz4.ledger:243-249` | lz4/concurrency, refused | *"a block cannot reach another thread but as text written and read back, which costs what the thread would save"* |
| `s3.ledger:201-205` | objects/get-parallel, pending | *"a range read on each thread needs a thread to carry a client and a range, which the language's threads (an i64 or a str each way) do only through text"* |
| `tokenizer.ledger:190-195` | tok/batch, refused | *"tiktoken's batches spread the work over threads, which are `threads`' and carry one `str` each way"* |
| `sql/sqlite.ledger:223-227` | sqlite/interrupt, refused | *"a thread receives an `i64` or a `str` and nothing else crosses (spec § 13), so no other thread holds the connection to interrupt it"* |
| `sql/odbc.ledger:326-331` | odbc/cancel, refused | the same words, *"no other thread holds the statement to cancel it"* |
| `redis.ledger:299-304` | redis/subscribe-channel, refused | *"the language moves a function and one value between threads (spec § 13)"* |
| `threads.ledger:71-75`, `:77-81` | thread/cancel, thread/identity, refused | *"nothing crosses into a running thread, so nothing can ask it to stop"*; *"a body is given one value and nothing else"* |

*Nothing is shared*: a pool, a client, a registry, a set, a lock:

| ledger:lines | capability | the ruling |
|---|---|---|
| `mail/smtp.ledger:231-235` | pool, refused | *"a pool needs threads sharing handles, which Heroes keeps apart"* (item 32's own witness) |
| `mongodb.ledger:88-92`, `:82-86` | mongodb/pool, mongodb/monitor, refused | *"threads share nothing in this language, so a pool has no one to share with"*; *"a monitor is a thread sharing the client"* |
| `redis.ledger:362-367`, `:376-381` | redis/pool, redis/auto-pipeline, refused | *"a pool is the program's"*; *"gathering needs it shared between threads, which the language does not share"* |
| `ldap.ledger:521-528` | ldap/pool, refused | *"a connection is one place one thread owns"* |
| `jobs.ledger:271-276`, `:229-234` | jobs/concurrency refused, jobs/rate-limit pending | *"Heroes' threads share nothing and the package starts none"*; *"a limit shared by workers needs a counter kept in the store"* |
| `metrics.ledger:336-340` | otlp/periodic, refused | *"a reader on a timer runs on another thread, which shares no registry with the program"* |
| `poll.ledger:100-104` | poll/one-shot, refused | *"it exists for several threads that share one set, which a set does not allow"* |
| `threads.ledger:89-93` | thread/shared-state, refused | *"threads share nothing in Heroes, so no value is held by two threads and nothing needs a lock"* |
| `log.ledger:88-93` | log/level-shared, refused | *"needs a mutable global, or state shared between copies of a value"* |

*No thread beside the program, no channel between threads*:

| ledger:lines | capability | the ruling |
|---|---|---|
| `kafka.ledger:119-123` | kafka/linger, refused | *"a write in the background needs a thread the program does not own, and its failure would come back to no call"* |
| `nats.ledger:218-222` | nats/retry-first-connect, refused | *"needs a loop running beside the program"* |
| `http2.ledger:310-314`, `websocket.ledger:246-250` | keepalive, refused | *"no thread or timer runs here"*; *"the language runs no code behind the program's back"* |
| `term.ledger:292-296`, `:241-246` | term/steady-tick refused, term/line-written-beside pending | *"it needs a thread of the library's drawing beside the program's"* |
| `poll.ledger:159-168`, `:140-145` | poll/callback-timer refused, poll/wake pending | *"no function of the program runs on a thread the library makes"*; *"a set crosses to no thread, a thread being handed one `i64`"* |
| `jobs.ledger:313-318`, `:278-283` | jobs/events, jobs/timeout, refused | *"keeps no channel to another thread"*; *"nothing can stop it from outside"* |
| `web.ledger:459-463`, `:465-469` | middleware/throttle, middleware/timeout, refused | *"one connection is served at a time on the thread that serves"*; *"spec § 13 joins a thread, never cancels one"* |
| `serial.ledger:232-238`, `:280-286` | serial/data-events refused, serial/cancel pending | *"Heroes has no closure and no event loop"*; *"a close on another thread does not end a wait already begun"* |
| `metrics.ledger:295-300` | statsd/batching, pending | *"several in one, without a thread of their own, are not written yet"* |
| `net.ledger:537-541` | tcp/nonblocking, refused | *"a program that waits on several at once waits on each in a thread"* |

*Question 2's, in the same ledger*: `threads.ledger:83-87`, thread/detach refused, *"every thread is
waited for, which is how one forgotten is told; a thread left running past `main` would be cut off with
its work half done"*; `threads.ledger:64-69`, thread/is-finished pending, *"it needs a door of the runtime
that asks a thread's state"*. *Kept rows that state the workaround*: `http.ledger` server/concurrent
(*"a thread is handed one number (spec § 13)"*), `net.ledger:523-528` tcp/thread, `metrics.ledger:194-199`
(*"`start_http_server` in a thread would serve a copy the program no longer changes, the language sharing
no value between threads"*), `tracing.ledger:173-178`, `jobs.ledger:142-148`. **Not the thread model's,
and they would stand under any of the three routes**: `os.ledger:29-46` env/set, remove, clear, and
`config.ledger:373-378` dotenv/load (*"in C a write beside another thread's read is undefined"*),
`i18n.ledger:127-135` (C's `setlocale`), and the rows the grep caught on another word (`markdown`
dialects, `sql/build` orm, `sqlite/sql-functions`, `io` position/at, `os` user/change).

**`http.ledger`'s line numbers move in the uncommitted working tree** (459-464 and 515-520 at the frozen
commit, 466-471 and 522-527 beside it, `diff` of the two runs of the census); every other row reads the
same in both.

## Where each seat works, and the rules every seat obeys

**Your folder is `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/rebel-scratch/251-<seat>/`**
(the `<scratchpad>` of `SKILL.md:225`, inside the repository's root because CLAUDE.md § Hard stops writes
no file outside it; ignored by git, `.gitignore`'s `.claude/worktrees/`). **Make it yourself, from the
frozen commit, and build nothing in the tree**:

```
F=/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/rebel-scratch/251-<seat>
mkdir -p $F/tmp && cd $F && export TMPDIR=$F/tmp
git -C /Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/rebel-round archive 3a5ee269117a31780ef1f96d0305452630ac4d83 | tar -x -C $F
find $F -type f -not -path "$F/tmp/*" | wc -l         # 20,022 at 3a5ee2691; say what yours reads
printf 'gitdir: /nonexistent-251-<seat>\n' > .git    # so the hooks judge your files in your copy (.claude/hooks/trees.py)
clang -I runtime seed/heroes.c runtime/runtime.c -o heroes          # 12.08 s for the writer
./heroes build selfhost/main.hero -o heroes-next                    # 151 s for the writer, 238 s for the critic
./heroes-next --version                                             # sources 23c7d2a626b29fd5
```

The `.git` file makes 8 of the compiler's own tests fail in a copy (carried, `docs/panel/249-*.md:413-419`);
**no seat runs `heroes test`** (the coordinator's verification pass holds the test slots this morning).
Rules, each from `SKILL.md` § 3 and 3c or CLAUDE.md:

- **Your folder and your report, nothing else.** Your report is
  `.claude/worktrees/rebel-round/docs/panel/251-reports/<seat>.md`, **written as you go**, in your
  definition's output structure (`.claude/agents/<seat>.md`); make the folder if it is absent. Never the
  tree's other files, another seat's folder, a lane, `bw/` (read-only), `p251`, `p252`, their scratch
  folders (read-only where a brief names a file in them), or the blind seat's folder.
- **No file outside the repository's root**: no `/tmp`, nothing under the home directory. Go only with
  `HOME`, `GOPATH`, `GOCACHE`, `GOMODCACHE`, `GOTMPDIR` and `TMPDIR` inside your folder, exported before
  every `go` command, `go version` included (the 2026-11-15 writer broke this once, carried); `lldb`
  likewise.
- **No paid run but the spec-warden's named refreshes and the coordinator's `run.sh`** (above). **Never print a credential**: `gh auth status` at most, never `gh auth
  token`. No GitHub write, no commit, no push, no tag; `gh issue view`, `gh run view` and a `gh api` GET
  are reads.
- **A program that may not end** is built with `heroes build` or clang and its **binary** run under
  `timeout -s KILL <seconds>`, never `timeout heroes run`; its output to `/dev/null` or through `head -c
  65536` into a file; `pgrep -fl <your folder>` read after, anything left stopped
  (`.claude/rules/verification.md` § A run that may not end). Bounds: a case 20 s, a program 60 s, a
  stage 2 1800 s here and in Linux arm64 and 3000 s in the emulated image, a storm `storm.py`'s own.
- **Docker**: `heroes-linux-arm64:latest` (`338a1044ac90`, 2026-09-17, native) and `heroes-linux:latest`
  (`6f78598a4eba`, 2026-09-09, amd64, **emulated, not the CI's leg**) (`docker images`); fed the tree by
  `git archive` on stdin, only a folder of yours mounted, one container of yours at a time, named
  `<prefix>-251-<n>`, under an outer `timeout -s KILL`, then `docker ps --filter name=<prefix>-251-<n>`
  read and `docker stop -t 1` where it prints one. Prefixes: `ce`, `fp`, `sw`; every binary you build
  carries yours.
- **Costs in instructions** (`/usr/bin/time -l`, *instructions retired*), best of N stated, the first
  discarded, never beside another heavy command of yours; neither Linux image has a counter (carried,
  `p251-scratch/briefs/linux/probe4-*.log`).
- **The route you would adopt, you build in your folder and run on the cases you name, before the
  synthesis** (`SKILL.md:274-277`); a route that does not build is reported as not built.
- **Every number with its command; every negative sentence run, or posed as a question**; *only*,
  *never*, *provably* with what was searched and where (CL-077, `/panel` step 2). A claim about another
  language from memory is a question for the historian.
- **The machine is held awake by you for a long run**: `pmset -g assertions` read
  `PreventUserIdleSystemSleep 1` at 09:08, held by powerd's *"Prevent sleep while display is on"* and
  coreaudiod's sound output, both of which end with the display or the sound (the critic's finding 10), so
  a run past a minute takes `caffeinate -i -w <pid>` beside it, which costs nothing
  (`.claude/rules/verification.md` § A long run holds the machine awake).

## The platforms

**Darwin arm64**, this Mac (above). **The CI's Darwin leg is `macos-26`**
(`.github/workflows/ci.yml:368`, `:1434`; `:373` is the full matrix, the Linux legs on `ubuntu-26.04` and
`ubuntu-26.04-arm` among it), after a push. **Linux arm64** and **emulated x86-64**,
the two images. **Windows**: not run by any seat; the box is the author's paid machine and no brief of
this sitting names a run on it; a Windows fact a seat needs is named **owed** in its report.

## What the two critics of 2026-11-15 raised, and where this brief answers it

Both first readings stopped before their numbered lists (sitting 251's having worked its items 11, 5
and 3 of 20, `p251/docs/panel/251-reports/critic-briefs.md`, 91 lines; sitting 252's its items 1 to 5 of
13, `p252/docs/panel/252-reports/critic-briefs.md`, 220 lines; `wc -l`), so every point below is from their
working notes, and the items they never reached are listed after them.

**Sitting 251's critic**: (1) the family's grep printed six lines, not five: re-run, six (§ What
moved). (2) defect 2531's `sigaction` in a forked child (`run.c:1805`) and (3) defect 2558's install and
prior call (`stack.c:1220`, `:743-746`) go through any route: in § Question 1, widened to the new
installs and reads, `hero_proc.c`'s forked child and `os.c:392`, and asked of both building seats. (4) `port.sh` fails by a relative path,
makes five overlays, leaves `.orig` files: the overlays are ported again into `bw/routes/` by absolute
paths, `.orig` removed. (5) no hunk moved by place, the moves semantic: the frozen `runtime.c` moved, so
the port's one hunk is named per route and the stale overlay's silent drop measured. (6) where J's
pointers are resolved (`dlsym` is not async-signal-safe): the proposal says before `main`, and both seats
answer it in code. (7) B''s source hash `bf6ed0abe5a25f72` against the rebuilt `495a6338a3e21eef`,
unsettleable from `ev249/`: it stays carried; this sitting builds B' anew from `bw/routes/bprime/`,
whose bytes are named, and 249's B' rows stay carried. **Never reached** (its instruction's items 1, 2,
4, 6 to 10 and 12 to 20; item 4's `rebind.c` is 86 lines, re-run above): the lane, now the full
panel with the historian for D-rebind's forward compatibility and Ubuntu 26.04 (`historian.md`); the division of work, 249's R3's as before; the emulated stage 2 (a seat
runs its own); glibc's own save points, `musttail` never run, the inline pair cross-compiled only, ASan's
interceptors (J's holes, § Question 1); the CI (re-read at the newest `main`); `macos-26` (the
historian's and the ffi-pragmatist's); Windows (owed, no seat); the `.git` file (kept, and no seat runs
`heroes test`); the run counts (20 of 20 kept); the 2026-11-15 writer's two Go telemetry files outside the
root (`go@go1.27.3-go1.27.3-darwin-arm64-2026-11-15.v1.count` still stands, 13:53 that day; `upload.token`
was born again 2026-11-18 19:05:21, `stat -f %SB`, and the folder holds 23 files whose names carry
2026-11-15 to 2026-11-19, `ls | grep -c '2026-11-1[5-9]'`: Go has run with `HOME` unmoved since, by
whom is unread; left for the author); the table of 249's measurements, carried and **not re-sampled by
this writer**.

**Sitting 252's critic**: (1) *"20 of 20"* holds at shorter sleeps, the two blocks are `to_str`'s and the
concatenation's, and `cr-late` is exit 0 20 of 20: `unjoined` and `cr-late` re-run at the frozen commit,
the same (§ Question 2's table). (2) the negative sentences: *"no line writes 2"* holds (`spawn.c`
unchanged); thread-exit destructors exist (`alloc.c:1524`, `:1535`) and now a process exit handler too
(`hero_term.c:170`); *"no `flockfile`"* holds; *"nothing detaches"* is false of the runtime, widened to
the new members; the spec names `exit(code:)`'s status (362) and no stream. (3) the ruling search
widened: panel 248's non-waiting join, 246's historian on Java, two blind readers, in § Question 2's
record and the historian's brief. (4) the premise at `alloc.c:79-80` falsified and every balance the
gate reads leaning on it: `cr-lease` re-run, 10 of 10 a false lease message; the proposal's gate reads
no balance while a thread runs. (5) the torn line: re-run, 0 of 100 this morning and 1 of 30 on the trap
(27 of 200 for the critic), defect 2582 still `OPEN`, `class:adjacent`, its class the coordinator's to
answer; the blind seat's task 2 prints from `main` alone after every join, so no multiset grading.
**Never reached** (its items 6 to 13): the census (38 door files and 22 Rebel callers, the instrumented
census the compiler-engineer's); the routes (E, J, R, C, M0, D, a typed handle, G1 to G3: the proposal is
R with G1 beneath, E its conservative, the typed handle measured through Rebel's `threads`, D refused by
Rebel's own ledger); the blind seat (a new design, `llm-ergonomist.md`); the bound (since the repairs, 51 sessions at 0.40 USD each and a
runner that stops before 9.50, `llm-ergonomist.md`); the bounds of the runs (kept); Windows (owed); panel 232 against CLAUDE.md § 2 (§ Question 2's
record, the spec-warden's to confirm) and `SKILL.md` against the hard stops (settled by the coordinator: the blind area inside
the root, with panel 252's guards, `llm-ergonomist.md`); the 2026-11-15 writer's slip into the scratchpad (and this writer's own writes outside the
root, below).

**This writer's writes outside the repository's root**: the blind seat's first folder, made where the
coordinator's first brief named it, in the session scratchpad; **the coordinator moved it inside the
root** (`.claude/worktrees/rebel-scratch/251-llm-ergonomist/`, its first five files now under
`superseded-0931/` there), so no file of this sitting stands outside the root.

## Repairs after the critic's first reading

The critic's first reading (`docs/panel/251-reports/critic-briefs.md`, 321 lines, finished 09:51) found
eleven things and answered one question; every number the writer had re-run held. The coordinator
decided what the critic left to it (2026-11-19, items (1) to (6) of its message, relayed). Each finding
and its repair, in the critic's order:

1. **Three of "the six new installs" are reads, and `hero_proc.c`'s child calls `sigaction` up to twice
   `NSIG - 1` times between `fork` and `execve`** (the coordinator's (5)). Repaired: § What moved and §
   Question 1 say three installs and three reads, by line, and name `hero_proc_child` as a second
   forked-child path beside `run.c:1805`, reached in code since no image here takes it;
   `compiler-engineer.md` and `ffi-pragmatist.md` put it in their Q1 scope. Re-read with `sed -n` at the
   frozen commit.
2. **The proposed built-ins take names programs use: 46 declarations in 39 files** (the coordinator's
   (3)). Repaired: § The three proposals gives the count per name (re-run: `spawn` 1, `spawn_sized` 1,
   `wait` 7, `send` 25, `receive` 12, `mailbox` 1, `close` 77; B's seven 124 in 98 files; 30 `extern`
   members named `close`), the diagnostic, and **route M**, the operations after a dot, beside B, both
   written as arms and counted; the writer measured that **today's method-form built-ins take their
   names too** (`lease`, `cstr`, `validated`, `slice`, `must`, `default`, `is_err`), so M needs a new kind
   of name; the compiler-engineer prices the renames and M's name kind, the spec-warden judges the rule
   the spec does not state, the blind seat's task 2 records a declared `spawn`, `wait`, `send`,
   `receive`, `mailbox` or `close`.
3. **`(thread T)` and `(mailbox T)` do not parse today and would parse with no new keyword** (an answer,
   no brief faulted). Re-run: `function take(t: (thread i64))` is `error[expected_function_type]`
   (`bw/names/p_thread.hero`); the place is `selfhost/parse/type.hero`'s `unit_or_function` (193), from
   `prefix` (73). The question leaves § Questions; the compiler-engineer builds the arm there and says
   what `grammar`, `surface` and the formatter's probe owe.
4. **The blind folder is inside the repository, with none of the guards that case needs** (the
   coordinator's (1)). Repaired: `llm-ergonomist.md` rewritten for the folder where it stands, under the
   repository's `CLAUDE.md` and git tree, run by `run.sh` with panel 252's guards: `env -i` with `PATH`,
   `HOME`, `LANG`, `GIT_CEILING_DIRECTORIES` set to each session folder's parent (with it, `git rev-parse
   --show-toplevel` from a session folder prints *"not a git repository"*, run) and the key from `.env`,
   its length alone printed; `--bare --restricted --safe-mode --strict-mcp-config --model
   claude-opus-5-5`; the tools as `SKILL.md` names them.
5. **One session holding both arms measures less and spends a fraction of the bound** (the coordinator's
   (1)). Repaired: panel 252's shape, **one folder per task, arm and replicate**, each holding its
   specification as `heroes-spec.md` and no diff, label-stripped, the labels drawn at random before any
   folder was made and kept in `key.txt` beside `s/`, never inside; 10 cells, 5 replicates, 50 graded
   sessions, plus **one verdict session** holding the frozen spec, the proposal's B and their diff, never
   graded; each capped at 0.40 USD, the runner stopping before 9.50 USD in all and skipping a folder
   that holds `run.json`; the counted decision rules registered in `llm-ergonomist.md` before any run.
   `run.sh` is written and **not run** (`bash -n` clean).
6. **The spec-warden was told not to refresh** (the coordinator's (2)). Repaired: `spec-warden.md` names
   the refreshes, at most eight, one per arm, each applied to the seat's copy, refreshed and reverted, the
   key from `.env` with its length alone printed, each refresh's time and printed count logged; the
   counts are then the real ones, not lower bounds.
7. **The proposal's mailbox is not Part 7.13's, and spec-B contradicted its own § 3 and § 9's `@` rule**
   (the coordinator's (4) and (6)). Repaired: § The three proposals no longer calls Q3 7.13's shape; it
   states the departure (7.13's queue a thread owns, the proposal's a value of its own); the mailbox is a
   **handle with a life** (`mailbox()` begins it, a close ends it, § 13's handle rules), and **§ 3's
   address sentence is amended** to class it with `ptr` and `cstr`, so *"No aliasing exists among the
   values this language owns"* stays true and a `send` without `@` changes the queue as a call without
   `@` may change what a `ptr` reaches; 7.13's own mailbox is named for the compiler-engineer to price;
   the historian is asked how Erlang addresses a mailbox (a pid) against Go's channels; the blind seat's
   task 4 predicts `m2 = m`, a send through `m2`, a receive through `m`.
8. **"The thread doors leave the program's surface" was unrun, and the compiler's sized thread had no
   answer** (the coordinator's (6)). Repaired: the proposal adds `spawn_sized(f, x, bytes: n)`, naming
   `selfhost/main.hero:87`, `:96-97`, `:101`, `:112` (re-read with `sed -n`: `STACK_BYTES` 268,435,456,
   the join inside `hero_exit`); whether a program's binding of the old doors is refused or left
   unnamed is written as a question for the compiler-engineer and the spec-warden.
9. **Q2's conservative E had no words** (the coordinator's (6)). Repaired: E is sitting 252's `pE`, the
   same bytes (sha256 `92c44fc0...`), **+27**, an arm the spec-warden refreshes and the blind seat reads in
   task 1.
10. **Smaller facts**: `ci.yml:373` is the matrix's Linux line, corrected (§ The platforms); the D-rebind
   overlay's `rebind.c` is the 249 source less its line 10, so its lines stand one lower (22, 39, 48, 67,
   75 by `grep -n`), said in `ffi-pragmatist.md`, which names the overlay as J's start; the machine is
   held by display and sound assertions alone, so a long run takes `caffeinate -i -w <pid>` (§ Where each
   seat works); stage 2 read 151 s for the writer and 237.88 s for the critic (§ Where each seat works);
   **the sitting reuses the number 251**, the 2026-11-15 sitting's `docs/panel/251-briefs/` and
   `251-reports/` standing uncommitted in the worktree `p251`: a line the synthesis owes.
11. **The blind grading could not see two things** (the coordinator's (6)). Repaired in
   `llm-ergonomist.md` § The grading: task 1 accepts any integer for the handle in `main done, worker <n>
   not joined`; task 2 under the proposal is graded on the compiler-engineer's built slice wherever the
   answer stays inside it (`spawn` and `wait` of one record holding a `str` and a `[str]`), by spec words
   elsewhere; and every task 2 answer records a declared name the arm takes.
12. **Defect 2582 under Q3, the question the sitting did not ask** (the coordinator's (6)). Repaired:
   § Questions asks it, and `compiler-engineer.md` (a runtime lock that keeps a `print` whole, priced and
   built under Q3's workers) and `spec-warden.md` (whether Q3 owes a sentence on a line written whole)
   each carry it, with sitting 252's critic's 27 of 200 (carried) and this writer's 0 of 100 and 1 of 30.


## Questions no one has run, posed as questions

- Does route J refuse `injump`, its three shapes, their fortified Linux variants and `kmiss`, and refuse
  none of the twelve depth programs, the 2558 goldens and `recovers-leaves-no-line`, on each image?
- Which way does J err on a buffer it has no record of, and how often does a correct program meet one?
- Does any of the 38 door programs or the 22 Rebel callers leave a thread unjoined on some path before
  `main` returns, the compiler's own `selfhost/main.hero` first?
- Is every counted block born and freed on one thread today?
- Under R, what does an abort at the end of `main` do to a worker inside the allocator, `printf` or a
  lock (sitting 252's critic measured E cutting a worker's `print` in two, 2 of 20, carried)?
- Under Q3, what does a deep copy of a `[T]` or a record cost per byte at `spawn` and `wait`, and what
  does a mailbox cost per `send`, in instructions?
- Does a `[(thread T)]` waited for in a `for` meet spec § 13's *"handing C a copy made before ...
  aborts"*? (The type form itself parses with no new keyword: § Repairs, finding 3.)
- Does Q3's `wait` consume a handle the compiler can track on every path, as `consumes` does?
- Is a program that binds `hero_thread_spawn` under Q3 refused, or the door left unnamed?
- What does route M's name kind, a name resolved by its receiver's type alone, cost the resolver, and
  does it break UFCS's *"`x.f(y)` is sugar for `f(x, y)`"* for the four operations?
- What does 7.13's own mailbox, a queue per thread reached through its `(thread T)`, cost beside the
  proposal's?
- **Defect 2582 under Q3**: once threads are the language's own and a worker that prints is the common
  shape, does Q3 owe a runtime lock that keeps each `print` whole, or a sentence? (The blind tasks print
  from `main` alone, so no instrument of this sitting sees it.)
- What do Go, Rust, Erlang and Swift do at each of the three questions (the historian's brief)?
- Whether every blind session, run from inside the repository with panel 252's guards, answers
  `context: no`.

Written by the coordinator's brief-writer, its first clock reading 09:08 (`uptime`) and its last 09:31
CET on 2026-11-19 (`date`); repaired after the critic's first reading until 10:05 (`date`). No seat had
started.

**Added 2026-11-21.** The folders under `.claude/worktrees/` this file names were never part of
the repository, git ignoring that directory, and were removed that day by the author's
instruction, meant as: *delete every worktree that is not active; where one is cited, find a
solution*. What a seat or the coordinator kept under `rebel-scratch/` for this sitting is read
from `docs/panel/251-reports/evidence.tar.xz`, its paths relative to `rebel-scratch/`; `evidence.md`
beside it says what was kept and what was left out. `p251-scratch/ev249/` is read from
`docs/panel/249-reports/evidence.tar.xz`, whose 3,232 members are the 3,232 files named above. The routes under
`p251-scratch/briefs/inputs/routes/` stand in the 251 archive under `251-briefwriter/routes/`. `p251-scratch/briefs/linux/probe4-*.log`, named here as carried, is in no archive and is gone. `p252-scratch` is in no archive and is gone; what a seat copied from it into its own folder
stands in that sitting's archive with the folder.
