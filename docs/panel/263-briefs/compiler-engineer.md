# Panel 263, the compiler-engineer's brief

**Repaired 2026-11-27 from about 11:00 by the coordinator's repair, on the critic's first reading, and re-based on
`f9f42fcc4`** (`00-shared.md`'s head). Read `00-shared.md` first, whole, then the critic's first reading
(`docs/panel/263-reports/critic-briefs.md`, beside these briefs under `.claude/worktrees/panel263/`). **Your agent file is
your copy's** (`.claude/agents/compiler-engineer.md`); design.md is reached by `grep -n`, never from memory (§1.12 is its
line 646, `grep -n "^### 1.12" docs/design.md`). Your veto on soundness stands; what you count is a cost to be written,
never a reason by itself (CLAUDE.md § Precedence, the author's amendment of 2026-11-22).

**Your copy**: `.claude/worktrees/263-compiler-engineer`, a detached worktree at **`f9f42fcc4`** (`00-shared.md` § 8,
with its never-touch list: `lane-sig-family-3`, `lane-win-paths`, `lane-win-paths-h`, `lane-unwinder`, `lane-land262`,
`lane-site-rebel`, `gate-push10`), scratch `.claude/worktrees/263-compiler-engineer-scratch/`, `TMPDIR` there. Build the
seed's compiler and stage 2 there (the critic's stage 2 at this commit read `sources 50e8403d5a885325`); a runtime you
edit is a copy in your scratch under `HEROES_RUNTIME`; **a compiler you edit is a copy of `selfhost/` in your scratch
built by your stage 2, never your copy's `selfhost/` edited**, so *before* stays the base's. Linux arm64: the container
`p263-compiler-engineer`, `--rm --pull never`, one at a time. **The Windows box** is up until about 12:50 for lane
unwinder: a run of minutes there is your choice (`00-shared.md` § 8). **No paid run, no credential printed, no GitHub
write, no file outside the repository's root, no `pkill`, no commit; every run under `timeout -k 5`.** Report as you go:
`docs/panel/263-reports/compiler-engineer.md` in your copy, programs under
`docs/panel/263-reports/programs/compiler-engineer/`, both copied to the same paths under `.claude/worktrees/panel263/`
when you finish or stop.

## The map (line counts by `wc -l` at `f9f42fcc4`; re-read every pointer before you cite it)

| what | where | lines |
|---|---|---|
| the entry guard's text, `entry` (`:140`, the string `:144`), the set `callbacks_of` (`:78`), `addresses_taken` (`:95`), `takes_a_callback` (`:121`), *"ONE computation with every consumer"* (`:61`) | `selfhost/emit/callback_guard.hero` | 207 |
| **a call-graph walk from a frontier the emitter already has** (the critic, § 5): `members` (`:137-216`), with `emit/worklist.hero` | `selfhost/emit/members.hero` | |
| the thunk: `definition` (`:98`), `written` (`:121`; its first line, the question, `:137`; `hero_callback_left` `:149-156`), `in_signal` (`:207-208`), its tests (`:413`, `:440`) | `selfhost/emit/callback_thunk.hero` | 472 |
| a function's prologue | `selfhost/emit/body.hero` | 402 |
| the stack guard, its thread-local bounds (`:467-468`), the window (`:517`), the witnesses (`:1018`), step 199i's arm (its note `:1061`, its functions `:1108-1143`, its call in the handler `:1150-1153`), the alternate stack (`:1324-1350`), the install (`:1377`) | `runtime/parts/stack.c` | 1546 |
| `hero_thread_is_home` (`:74`), the refusal (`:132-143`), the guard (`:145-148`) | `runtime/parts/thread.c` | 148 |
| the stop (`:146`, its `pause()` latch `:152-160`), **step 199j's stop `hero_signal_switch_begin` (`:600-622`)**, `hero_signal_in_switch` (`:671`), the thread's record `hero_signal_stack`, `_Thread_local` (`:519`), the thunk's question (`:912-990`), the walk bounded by a coroutine's range (`:955-970`) | `runtime/parts/sigguard.c` | 1011 |
| the ranges `makecontext` hands out (`:94-101`), their life (`:104-170`), `hero_sig_known_stack` (`:186`), `hero_sig_range_above` (`:325`) | `runtime/parts/sigentries.c` | 793 |
| Linux's `setcontext` (`:260`), `swapcontext` (`:289`), 199j's call into the stop (`:249`); the delivery's mark (`siginstall.c:169`) | `runtime/parts/sigjump_save.c` | 368 |
| Linux's `makecontext` wrapper (`:44`, `:120`); **reading memory where it is mapped, `process_vm_readv`** (`sigjump_record.c:116`, `:159`) | `runtime/parts/sigjump_make.c` | 135 |
| Darwin's ranges (`:31-34`), `hero_rebind_known_stack` (`:111`), `hero_rebind_range_above` (`:229`), `hero_rebind_relived` (`:497`, **its loop `:499`**), the trampoline (`:712`, the mark `:723`) | `runtime/parts/rebind_entries.c` | 770 |
| Darwin's swap (`:241`), 199j's call into the stop (`:154`), `makecontext` (`:403`, `:418`, `:433`); **`mach_vm_read_overwrite`** (`:78`, and `rebind_record.c:79`) | `runtime/parts/rebind_jump.c` | 447 |
| the count of callbacks entered and not left, `hero_callback_mine` (`:116`, a pointer, 48,944 bytes below a coroutine's `lo` in `00-shared.md` § 2.3) | `runtime/parts/callback.c` | 167 |
| guarded mappings the runtime already makes for lent buffers (`mmap` `:261`, `mprotect` `:281`, `:301`, `:311`) | `runtime/parts/lend.c` | 997 |
| the instrument of the refusal's arms (none of 199j's words) | `tests/harness/spec_refusals.hero` | 225 |

## What you build and run (this Mac and the Linux arm64 image; `-O0` and `-O2`; three runs each unless a rate needs more)

**`--sanitize` cannot see these writes** (`00-shared.md` § 2.2, the critic's item 2): a sanitized run is a run of its own
layout, and its exit 0 says nothing of bytes below a stack. Every claim that a route leaves **0 bytes** below a stack is
made by the canary's count, or by a watchpoint, at both levels, both platforms, plain **and** sanitized.

1. **The defect on your stage 2, both platforms**: `reproducer/run.sh <your heroes-next> <your scratch>/rs 3 30` from
   your copy's root with `docs/panel/263-briefs/` copied in, `CO_SAY=1` once per build, and **`malloc.hero` sized by
   `CO_N`** as `00-shared.md` § 2.2 did (1,600 to 50,000 at `-O0`, 9,600 to 14,000 at `-O2`), the answers held to
   `thread.hero`'s. Then **the readings left**: (i) **which instruction writes `hero_thread_is_home`** first: lldb, a
   breakpoint at the first `hero_thread_guard`, the word's per-thread address by its TLV descriptor's thunk
   (`ground/lldb2-malloc-O2.txt` shows the expression), `watchpoint set expression -w write -s 1 -- <address>`, continue:
   the frame and the value; and the same for `hero_callback_mine`; (ii) **which signal the trampoline was called for in
   the hang**: the repair found the trampoline faulting in `hero_rebind_relived` on the overwritten record
   (`00-shared.md` § 2.4, `ground/f9f42fcc4/lldb-hang-*.txt`) and could not read the signal from an `-O2` build with no
   variables: build the reproducer at `-O0` against a runtime built `-O0 -g` in your scratch, or break at
   `hero_rebind_tramp`'s entry and read its third argument; (iii) **Linux**: what a 64 KiB `malloc` stack (from the heap,
   `00-shared.md` § 2.6) does, a copy of `co.h` in your scratch with `CO_STACK` changed.
2. **Route (c), built** (`00-shared.md` § 4): a stack check at each call of the functions a callback reaches, the stack
   pointer against a per-thread limit (`lo` of the stack the thread runs on, plus a reserve you size and justify), the
   limit kept by the switch wrappers and re-derived from the ranges on a miss. **(c1)**, the address-taken set where
   `hero_thread_guard` already stands; **(c2)**, every function reachable from a callback, built on
   `emit/members.hero:137-216`'s walk (no such set on `main`, `00-shared.md` § 3.3; say whether it is the set panel 258's
   R2 needs, and name one function each would get that the other would not); **(c3)**, every function of a program with a
   callback `extern`. For each: does the canary write **0 bytes** below the stack at every `CO_N` of `run.sh`, both levels,
   both platforms, plain and sanitized? Does `malloc.hero` at every `CO_N` of § 2.2 end told, by which words? `static.hero`
   (shape A)? Which of `00-shared.md` § 5's shapes does it close, and **which does it leave** (C's frames below the last
   check, shape D; a stack it never saw, item 11)? Where does the limit word live, and does it live in the memory the
   recursion writes (§ 2.3)?
3. **Route (b) from the compiler's side**: the thunk switching to a stack the runtime owns. **The question's frames run
   on C's stack before any switch today** (`sigguard.c:912-990`, `callback_thunk.hero:137`; the critic's item 6): build
   the switch first and the question after it, and say what the question loses there (its walk reads frame records up
   C's stack), or keep the question first and measure how deep its frames reach below the thunk's on C's stack; and a
   nested entry, already on the runtime's stack. Prototype by hand in an emitted C first (`--emit-c`, edited in your
   scratch, built with clang against a runtime copy), then price it in `callback_thunk.hero`. Run: the canary,
   `malloc.hero` by `CO_N`, `static.hero`; **the signal guard's `run` cases on the (b) runtime**: `heroes-next run
   tests/harness/main.hero -- heroes-next run fixedbugs-26`, `fixedbugs-27` and `fixedbugs-28` (41, 95 and 39 cases by
   `ls tests/golden/run | grep -E "\.hero$" | grep -c -- <word>`, re-run at `f9f42fcc4`), **every red named with the rule
   of `00-shared.md` § 3.7 it breaks**; and **lane unwinder's walk across the switch**: since R2 is not on `main`, a
   minimal forced unwind of your own (a function built `-fexceptions` holding an `__attribute__((cleanup))` slot, called
   from the callback, and C calling `_Unwind_ForcedUnwind` from below it; panel 258's ffi-pragmatist's prototypes are the
   model, `docs/panel/258-reports/evidence.tar.xz`, `tar -xJf` into your scratch), counting the cleanups run **with and
   without CFI on the switch**, both platforms. Say what (b) costs R2 in words a lane can build. cgo as recalled is (b)
   and (c) together (`00-shared.md` § 4): the historian verifies it.
4. **Routes (e), (n), (l), (q), and (p), (k), (m)**: **(e)**, the probe at the thunk against the space left on the known
   range, and **(n)**, the same probe against a static bound of the callback's call tree (clang `-fstack-usage` and the
   walk of item 2; say which trees are bounded in the tree's own programs and examples); **(l)**, the runtime's
   per-thread words moved out of `_Thread_local` into a guarded mapping reached by the thread key, and **(q)**, the
   process-wide words below a static array too (`hero_stack_prev_segv`, `hero_signal_stopping`, `00-shared.md` § 2.5):
   each alone, does it change the false message, the hang, the exit 138, `static.hero`'s hang or 139, the canary? (l) and
   (q) are the honesty half (design.md §4.17): say whether they are owed beside whatever route is adopted. **(p)**, a
   hardware watchpoint, ruled in or out by name, with the run or the page that does it. **(k)** and **(m)**: one line
   each, read or run.
5. **The secondary question, step 199j's stop** (`00-shared.md` § 3.9; the critic's item 7): **its seven cases**, `heroes-next run
   tests/harness/main.hero -- heroes-next run fixedbugs-2871` (7 by `grep -c`), at both levels and under `--sanitize`, on
   this Mac and in Linux arm64; **name the four switch-family cases red in Linux arm64 on the step's base** (its commit
   body, *"Linux arm64 162 and 4, the four red on the base too"*; named nowhere the repair could find: run
   `fixedbugs-27` and `fixedbugs-28` in the image and name every red with its exit and first line); **one shape a correct
   program could be stopped by** (a handler switching to a context the interrupted switch already finished saving into,
   say), built and run; **and the stop under this defect**: it reads the thread record `own` the recursion overwrites
   (`rebind_entries.c:723`, `siginstall.c:169`), so a handler that switches contexts after `malloc.hero`'s recursion wrote
   the record: does the stop fire falsely, loop, or hold? Linux x86-64 and Windows by reading, or Windows by a run of
   minutes on the box.
6. **The costs, before and after, every route you built** (CLAUDE.md § Verification): per callback, panel 259's
   `cost_cb.hero` (20,000,000 calls) and libc's `qsort` comparing 1,000,000 `i64` through a callback; **per call**, a
   callback whose body makes 20,000,000 calls of a small function, and one recursing 10,000 deep repeated 1,000 times;
   **panel 104's own shape**, `down(n)` returning `1 + down(n - 1)` called from a callback at `-O2`: does the check, like
   104's counter, stop clang turning the recursion into a loop (read the `-O2` assembly before and after)? On this Mac
   `/usr/bin/time -l` instructions retired, best of 5 alternated, the first discarded; in Linux arm64 a clock the program
   reads (`clock_gettime`), alternated, the load named (`uptime` before and after). **And the byte identity**: the
   compiler's own `heroes-next build selfhost/main.hero --emit-c` and every `examples/` program with no callback
   `extern`, before and after each route, `cmp` with the `hero_program_sources` line taken out
   (`.claude/rules/verification.md` § The formatter's probe states that line's exception). **Never slow the compiler
   down**: a route that moves the compiler's own C is priced by `heroes build selfhost/main.hero` before and after,
   machine still or the reading discarded with its reason.
7. **The shapes nobody named** (CLAUDE.md § RUN IT): at least one of your own beside `00-shared.md` § 5's, with its run.

## What you judge

Per route of `00-shared.md` § 4 you built or read: `verdict`, `section`, `implementation_cost` (lines by file under
`selfhost/` and `runtime/`, and the per callback and per call numbers above), `needed_for_self_hosting`, `prediction`
(one falsifiable sentence, the landing that checks it), `condition`. **A route that leaves bytes written below a stack in
any shape it claims is a soundness veto**; say which program, platform, level and build, and by which count. Say whether
the route you support, and 199j's stop, need a sentence of the spec (`00-shared.md` § 7), and what a veto would compel.
