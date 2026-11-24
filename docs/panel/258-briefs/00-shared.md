# Panel 258, the shared brief: what a C jump over a callback's frames leaves behind

**Lane: soundness** (`compiler-engineer` and `ffi-pragmatist` only, no historian). The question changes no surface and no
spec token as the coordinator reads it; **each seat says in its report whether any resolution it supports needs a sentence of
`spec/heroes-spec.md`**, and a yes sends the sitting to the full panel, which this lane gives up. Convened 2026-11-24 by the
author's answer *2c* (`docs/roadmap/milestones/M-rebel-library.md`, the section of that day; decision issue 2751).

**This brief was rewritten the same night, after the completeness critic read the first one**
(`docs/panel/258-reports/critic-briefs.md`, which found false framing facts, five more leftovers than the brief named, and
doors and options it did not list). The first version's proposal was too narrow and its leak example (libpng) was wrong for
the binding it named; both are corrected below. A seat that finds a sentence here false says so first in its report.

Every number, path and count below was produced by a command named beside it, run on this Mac (macOS arm64) on 2026-11-24,
by the coordinator or by the critic and re-run by the coordinator. **No seat or critic starts a paid run, a `claude -p`
session, an API call or a cloud run; none prints a credential** (a seat that must know whether `gh` is authenticated runs
`gh auth status`, never `gh auth token`).

## The question

A C function can abandon the frames of a Heroes callback: `longjmp`, `siglongjmp`, `_longjmp`, `__longjmp_chk`, a `setcontext`,
a `pthread_exit` on a worker thread, a signal handler that jumps out, a coroutine never resumed. The jump runs no code, so
**whatever those frames held and whatever the runtime's per-thread bookkeeping counted for them is left as it was**. What does
the runtime owe a program that does this, and by which mechanism? The two ends the author set: the interim (a), which
stays until this sitting's resolution is built, and a resolution that stops the damage, (c) or another.

## What a jump leaves behind, measured (the first brief named only the first row)

Each row is a correct program (the C did what C says) built with the tree's **stage 2** compiler at `c4fc5b46a`, run on this
Mac; the programs and their C headers are in `docs/panel/258-briefs/jumps/` and `docs/panel/258-briefs/leak/`. Each is filed
as a defect (2752 to 2756) so that the sitting's resolution has something to close.

| leftover | program | what happens today | issue |
|---|---|---|---|
| refcounted values of the abandoned frames (strings, arrays) | `leak/main_1k.hero`, `main_100k.hero` | `leaks --atExit` counts **7 blocks and 3,952 bytes a jump** (two strings of about 1 KB and two 50-element arrays; 7,000 leaks and 3,952,000 bytes at 1,000 jumps); maximum resident 5,963,776 and 6,012,928 bytes at 1,000 jumps, **between 307,429,376 and 352,927,744 at 100,000** (the eight readings are in `leak/readings.md`, six of them 352.9 MB; macOS compresses resident pages); exit 0. Defect 2704's interim switches the exit audit of the heap blocks off for such a program, so nothing is said | none (the interim) |
| the large-value room, a per-thread LIFO in `runtime/parts/frame.c` | `jumps/room_order.hero` | a callback holding values past 4 KiB, jumped over 8 times while `main` holds one: prints its result, then exit 134 `a large value's room was given back out of order (compiler bug)`, 3 of 3; with one jump exit 0 | 2752 |
| the handle gate (`acquires`/`consumes` obligations) | `jumps/handlejump.hero` | prints `result 7`, then exit 134 `1 C handle(s) never given back`, 3 of 3 | 2753 |
| a `spawn` thread's answer | `jumps/pexit.hero` | the worker calls C's `pthread_exit`; `t.wait()` exit 134 `a null pointer was read through, at offset 0x38`, 3 of 3. The thread's body runs with **no thunk and no count** (emitted C) | 2754 |
| a signal handler's jump out of a callback | `jumps/alarm.hero` | a `SIGALRM` handler `siglongjmp`s out; 8 processes of 8 here (25 of 25 for the critic) exit 134 `a signal handler used this runtime's allocator ...` naming a Heroes handler that does not exist (`alloc.c` near line 617 leaves `hero_alloc_inside` set) | 2755 |
| a lent buffer's region (`runtime/parts/lend.c`, `mmap`ed outside `alloc.c` on purpose) | `jumps/lendjump.hero`, control `jumps/lendok.hero` | 20,000 jumps: maximum resident **334,741,504** bytes against **2,998,272** when C returns; no gate sees it | 2756 |
| the lease gate and the scratch gates (`.lease()` owes `end_lease`, `hero_run_*`, `hero_dir_*`) | none | **read in `alloc.c:418-431`, not run**: they read no callback count, so they should end as the handle gate does | none |

**So the interim (a) is not "the leak whole".** The count of callbacks entered and not left is read by one place outside
`callback.c`, `alloc.c:363` (`grep -n hero_callbacks_unfinished runtime/parts/*.c`), the heap-block check. Every other gate
and stack the jump unbalances still speaks. The conservative baseline is therefore (a'), *every gate that reads a balance
forgives a program with an unfinished callback, or tells the truth*, not (a).

## The proposals on the table (the first brief had (a), (b), (c); the critic added the rest)

- **(a) the interim as built**: the heap-block audit off. Leaves the rows above.
- **(a') the interim widened**: every balance gate forgives or tells the truth about a program with an unfinished callback.
  No release, so every leak stays; it removes the false messages. Cheap.
- **(b)** audit by subtraction: **not a candidate** (it leaves the leak and needs the knowledge that would release the blocks).
- **(c) release at the jump**: the compiler gives the functions that can run inside a callback an *extent record* (the
  critic: "frame record" already names the hardware frame-pointer record the signal guard walks in this tree), the
  addresses and kinds of their refcounted slots, on a thread-local chain from the thunk; the runtime's hook
  (`hero_jmp_jump`) walks it from the jumping frame to the target and releases. **The critic's point: (c) must unwind every
  runtime stack the table above names, not only slots**: the count, the room, the lend depth, the handle and lease
  obligations; otherwise turning the audit back on still aborts rows 2 and 3.
- **(d) table-driven forced unwinding**: `__attribute__((cleanup))` on every owned slot, `-fexceptions`, and
  `_Unwind_ForcedUnwind` from the hook with a stop at the target, glibc's own route for `pthread_exit` and cancellation. No
  instruction on entry; would cover `pthread_exit` and an Objective-C exception. Needs unwind information in every C frame
  between; Windows would be `__try/__finally` (a claim, unrun). `frame.c:135-143` describes a `__cleanup__` route that
  `grep -rn '__cleanup__\|taken_to_exit\|give_at' selfhost/` does not find in the compiler: a stale comment or a removed route.
- **(e) lazy detection by stack pointer, no hook**: at an extern call's return or a thunk's entry, a thunk entry whose frame
  lies deeper than the current stack pointer belongs to an abandoned extent; the runtime's stacks are unwound to that
  entry's saved markers (room top, lend depth, count). Works for unseen jumps and on Windows, repairs the room and lend rows
  without reading a slot; values stay leaked unless their pointers live off the stack.
- **(f) hybrid**: release in the hook where no signal handler stands (`hero_sig_standing(own)` false), defer where one does.
- **(g) a told refusal at the hook**: stop the program, naming the jump and the callback (the loud direction,
  `.claude/rules/module-shape.md`); breaks a binding that reports a stream error by `png_error` from a callback. **Prior art
  (Rust's `C-unwind`, cgo's rule against C jumping over Go frames, OCaml's callbacks) is unverified here and this lane has
  no historian: a seat may name a precedent only as a question.**
- **(h) a release point after the landing**: the return of the extern call into Heroes code, never inside a handler.

## Hazards the seats must settle, from the critic's reading (each read, not all run)

1. **Where a release may run.** `hero_jmp_jump` runs before the real jump with the abandoned frames still on the stack and may
   run inside a signal handler (the second golden of `tests/golden/run/fixedbugs-2704-*`); `free`, the allocator's locks and a
   refcount drop that reaches `free` are not async-signal-safe; the stack memory is reused by the code that lands.
2. **An asynchronous jump** (the `SIGALRM` row) abandons frames at any instruction, inside the allocator or between a slot's
   store and its old value's release: not "at a C call site". Whether (c) covers it at all is a ruling nobody has asked for.
3. **(c) after a jump it did not see** (the critic's D6): records left by an unseen jump point into stack memory the landing
   code reuses; the next seen jump walks them and releases what the reused memory now holds, and a hand-written stack switch
   interleaves two stacks on one chain. **In both (c) turns a leak into memory corruption, which the interim cannot do**
   (rank 3 of CLAUDE.md § Precedence). Under which events does (c) release a value that is still live?
4. **A release that runs C inside the library that is jumping** (D7): libpng keeps its `jmp_buf` in the `png_struct`; a hook
   that releases a png handle held by an abandoned frame frees the buffer `longjmp` reads next. A handle is released by an
   explicit consuming call, so "release what the frames held" means running a binding's C consumer in the hook: does it, or
   does the gate forgive?
5. **Thread-local state in a handler or a key destructor** (D8): the handler-side record is reached through the thread key,
   never a thread-local access (`runtime/parts/sigentries.c:60-63`); on Darwin arm64 a `_Thread_local` read inside a key
   destructor reads 0 (`alloc.c:1668-1672`, panel 111). A chain drained there must hang off the key.
6. **A coroutine never resumed is no event** (D5): no jump to hook, and C may free its stack, records included.
7. **Doors the first brief did not list**: a handler that returns with an edited register file (libsigsegv's shape,
   `parts/stack.c:868-878`); an Objective-C or Swift exception through the AppKit layer
   (`runtime/platform/hero_ui_appkit*.swift`); a jump made by an image inside Darwin's dyld shared cache (`rebind.c` rewrites
   only outside it); what `runtime/parts/sigjump.c:66-74` already lists as not reached on Linux (`dlvsym`,
   `__builtin_longjmp`, assembly, C++ and `_Unwind_*`, the residue `rr_2`).

## The libpng example was wrong for the binding it named, and the first brief said otherwise

The first brief said *a server that decodes untrusted images through `libpng` loses that on every corrupt image*. The critic
ran panel 257's `pngread` (read function calls the program's callback first and `png_error`s on a short read after it returned;
a CRC is checked after the read function returns): 2,000 truncated and 2,000 bad-CRC images, callback holding about 100 KB a
call, all 4,000 returned -3, exit 0, maximum resident 3,489,792 bytes (4,000 abandoned 100 KB frames would be about 400 MB).
**No Heroes frame is abandoned in that shape.** Only a program's own `pr_fail()` from inside the callback does, and
`git grep -l -E 'longjmp|siglongjmp|png_error|png_jmpbuf|setjmp|swapcontext|setcontext|pthread_exit|pthread_cancel|lua_error'
-- examples examples-rebel packages runtime/platform` finds one line, a ledger naming Python's `decimal.setcontext`: **no
program or package in the tree jumps over Heroes frames today** (Rebel's PNG is written in Heroes). So the benefit of any
resolution is to bindings and programs that do not exist yet, which is a fact the sitting weighs, not hides. The real
question for the ffi-pragmatist: which binding shapes put a Heroes frame above a library's jump (a read callback reporting its
own stream's failure by `png_error`, a progressive reader's row callback calling back into libpng, Lua's `lua_error` over a
Heroes function, a watchdog)?

## What the coordinator read, each with its command (corrected)

- `runtime/parts/callback.c` is 86 lines (`wc -l`); **`hero_callbacks_inside` is a `static _Thread_local int64_t` at line
  58** (not 61; line 61 is `hero_callback_in`), so a prologue test of it needs an export nobody has priced;
  `hero_callback_left` is at 63; `__lsan_is_turned_off` at 83-84, inside `#if defined(__linux__)` and the ASan feature test.
- **The jump hook exists in `runtime/parts/sigjump_save.c` under `#if !defined(_WIN32) && !defined(__APPLE__)` (line 25),
  which is glibc Linux in practice** (the file uses glibc's `struct __jmp_buf_tag` and `__longjmp_chk`; the CI's legs are
  Ubuntu, macOS, Windows). `longjmp`, `_longjmp`, `siglongjmp` and `__longjmp_chk` call `hero_jmp_find()`, then
  `hero_jmp_jump(env)`, then the real function (lines 130 to 153). **`hero_jmp_jump` decodes the target's stack pointer
  (`hero_jmp_sp_of`) only where the key verified; otherwise route A's record judges the jump**
  (`sigjump_save.c:43-59`, `sigjump.c:156`). Darwin's route is `runtime/parts/rebind_jump.c`. **Windows**: the three Windows
  files of the platform layer hold no `longjmp`, and the evidence is the `#if` guards of `runtime/parts/`: a question about
  Windows, not a premise.
- The thunk's first line is `hero_callback_entered("<name>", __builtin_return_address(0), __builtin_frame_address(0))` and
  its line after the call is `hero_callback_left()` (`leak/main_1k.emitted.c`, 212 to 218). `hero_callback_entered` has two
  definitions in `runtime/parts/sigguard.c`, lines 573 and 636.
- **A frame's owned values** are C locals declared and zeroed in the prologue and released before the return: in
  `leak/main_1k.emitted.c` line 189 the callback's prologue declares `h1_word`, `h2_nums`, `h3_xs0` (the `for` loop's array)
  and `h6_own6` to `h9_own9` (four strings) and `h10_own10`, `h11_own11`: nine slots, released at lines 200 to 208. The
  prologue calls `hero_thread_guard("main1k.body")` (`runtime/parts/thread.c:141`) and registers none of the slots.
  **A per-thread LIFO pushed in a prologue and popped at the one exit already exists for another purpose**: the large-value
  room (`hero_frame_take`, `hero_frame_give`, `runtime/parts/frame.c`, defect 2278). Five sittings use the words *frame
  record* or *shadow stack* in other senses (156, 247, 248, 249, 253): nothing of (c)'s kind has been measured.
- **Stage 1 (the seed's compiler) emits no `hero_callback_left()`** (0 occurrences against 1 from stage 2, the critic's
  `grep -c`): its programs are never audited, so **a seat measures with a stage 2 compiler** (`./heroes build
  selfhost/main.hero -o heroes-next`; the critic's took `real 271.51` under load 43; the seed itself took 31.84 s, not "a few
  seconds").
- The interim's cost, from defect 2704's closing comment (issue 2704): 100 million calls of a trivial callback at `-O2`, six
  runs each, 0.67 to 0.69 s before **(one 0.85)** and 0.83 to 0.85 s after **(one 0.91)**, about +1.6 ns a callback. A record
  on every function entry has not been measured by anyone.
- There is no `crates/` in the tree (`ls crates`); the Rust bootstrap is at the tag `m-bootstrap-archive` (CLAUDE.md § 5).
- Goldens that exist: `tests/golden/run/fixedbugs-2704-*` (three) with `fixedbugs-2704-longjmp.h` and `fixedbugs-2704-handler.h`;
  `libpng` 1.6.59 (`pkg-config --modversion libpng`); panel 257's `pngread` in `docs/panel/257-reports/evidence.tar.xz`.
- This Mac holds itself awake (`pmset -g assertions`: Amphetamine) and is loaded by about ten other agents: a timing whose
  ratio of `real` to `user` plus `sys` is far above 1 is discarded with the reason written down; **report `leaks` counts and
  exit codes rather than resident sizes** where either answers the question.

## How a seat works

- **Your own copy, one seat's**: `git worktree add --detach .claude/worktrees/258-<seat> <the base commit the coordinator's
  prompt names>` from `/Users/joseph/Temp/heroes/heroes-lang`; build your own compiler there from the seed
  (`clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`, **about half a minute under load**) **and then stage 2**
  (`./heroes build selfhost/main.hero -o heroes-next`, **several minutes under load**), which every measurement needs;
  never use the trunk's compiler or another seat's, never read, build or run inside another seat's folder. Linux arm64 (glibc)
  is the container `heroes-linux-arm64:latest` (`docs/platforms/linux/LINUX-MACHINE.md`); remove any container you start.
- **Write your report as you go** to `docs/panel/258-reports/<your-seat>.md` of your own copy and copy it to the same path of
  `/Users/joseph/Temp/heroes/heroes-lang` when you finish or stop, so a killed seat leaves what it had. **Never commit and
  never edit the trunk's tracked files**: the coordinator commits the sitting.
- **The Windows box** (`ssh win`): one run at a time under its lock, a directory made with `mkdir C:\w\box.lock` and an owner
  file named for you, released with `rm -r` only if it names you; it is armed to about 06:29 this morning. **This protocol is
  written in no tracked document**: it is the convention the night's lanes keep (their briefs are untracked scratch of the
  session). The box answers a build only through the desktop session.
- **Contract**: your verdict (approve, approve with a condition, object, veto), the section it rests on, the cost it names,
  **one falsifiable prediction** with the measurement at which it is checkable, and every place this brief was false or
  silent. Say what you ran and what you read, apart. A seat's veto is a refusal, never a price.
- Never `pkill` by name; stop a process you started by its PID.
