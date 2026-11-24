# Panel 258, the completeness critic's reading of the briefs (2026-11-24)

Read before any seat starts (the panel skill's step 3c). No verdict. Read whole: `00-shared.md`,
`compiler-engineer.md`, `ffi-pragmatist.md` and the six files of `leak/`, at `c4fc5b46a`.

## What I ran, and where

- My own worktree, `.claude/worktrees/258-critic` at `c4fc5b46a`. Stage 1 from the seed,
  `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`: `real 31.84 user 18.69 sys 0.61`
  (load average 43.73 when it started). Stage 2, `./heroes build selfhost/main.hero -o heroes-next`:
  `real 271.51 user 190.99 sys 47.03`. Stage 2 was needed (finding 1.26). No number below is a
  duration I lean on: what I read is exit codes, messages, resident sizes and `leaks` counts.
- Everything ran on this Mac (Darwin arm64). **Nothing ran on Linux or Windows; every sentence below
  about those two is unrun.** No paid run, no `claude -p`, no credential printed, no tracked file
  edited. The experiments are in the session's scratchpad
  (`/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/9d1670cb-db5d-4975-9d5b-1db104217e45/scratchpad/exp/`
  and `.../scratchpad/png/`). The scratchpad may be cleared, so the key reproducers are written out
  in § 5.

## 1. The framing facts, each checked by its command

Verdicts: **holds**, **false**, **imprecise** (true, but stated in a way a seat would misread),
**unverifiable** (no command here settles it).

| # | Brief's claim | Command | What it printed | Verdict |
|---|---|---|---|---|
| 1.1 | `callback.c` is 86 lines | `wc -l` | 86 | holds |
| 1.2 | `hero_callbacks_inside` is a `_Thread_local int64_t` **at line 61** (shared; the compiler-engineer's Q2 repeats `callback.c:61`) | `grep -n` | `58:static _Thread_local int64_t hero_callbacks_inside = 0;` and `61:static void hero_callback_in(void)` | **false**: line 58. The variable is also **`static`** to the runtime's unit, so route 3 of Q2 (a prologue test of it) first needs it exported. The brief does not ask whether that moves `HERO_RUNTIME_ABI` (32, `runtime/heroes_runtime.h:108`) |
| 1.3 | `hero_callback_left` at 63, `__lsan_is_turned_off` at the end | `grep -n`, `tail` | 63; 83-84, inside `#if defined(__linux__) && __has_feature(address_sanitizer)` | holds |
| 1.4 | "a program ending with a count above zero is **not audited at exit** for a missing decref" | `grep -n hero_callbacks_unfinished runtime/parts/*.c` | one reader outside `callback.c`: `alloc.c:363`, `if (blocks != 0 && hero_callbacks_unfinished() > 0) blocks = 0;` | **imprecise**: only the heap-block balance is excused. In the same `hero_runtime_check_leaks`, the scratch gate, the lease gate (`held`, `alloc.c:418-431`), the handle gate and `hero_lend_audit_given` read no count. Measured consequence: § 2, F2 |
| 1.5 | the jump hook "exists on Linux **and every non-Darwin POSIX system**" | `grep -n '^#if' sigjump_save.c`; `grep runs-on .github/workflows/*.yml` | line 25 as quoted. The file uses glibc's `struct __jmp_buf_tag` and `__longjmp_chk`. The CI legs are `ubuntu-*`, `macos-26` and `windows-latest`, with no BSD or musl leg | **imprecise**: that is how the `#if` reads, not something any platform has shown. Write "glibc Linux" |
| 1.6 | `longjmp`, `_longjmp`, `siglongjmp`, `__longjmp_chk` each call `hero_jmp_find()`, `hero_jmp_jump(env)`, then the real one (`sed -n 126,140p`) | `cat -n` | true at lines 130-153. The cited `sed` range shows only the first two | holds, wrong range |
| 1.7 | `hero_jmp_jump` "decodes the target's stack pointer (`hero_jmp_sp_of`) **before** the jump" | read `sigjump_save.c:43-59`, `sigjump.c:156` | decodes only where the key verified (`!hero_jmp_recording()`, `hero_jmp_decoding != 0`). Otherwise route A judges the jump by the save's record (`hero_jmp_by_record`), and it returns at once where `own == NULL` | **imprecise**: (c) needs the target in both branches, and the brief does not ask whether route A's record carries one |
| 1.8 | Darwin's route is `rebind_jump.c`, "route D-rebind" | read its doc and `rebind.c`'s | true. `rebind.c`'s doc also says the rewrite reaches "every loaded image **outside the shared cache**" | holds. A jump made by a dyld-shared-cache image is a door the briefs do not name (§ 2, D3) |
| 1.9 | "No file of `runtime/platform/*win*.c` holds `longjmp`" | `ls runtime/platform/*win*.c`; `grep -li longjmp runtime/parts/*.c` | the glob names three **UI** files of the platform layer (`hero_ui_print_win32.c`, `hero_ui_web_win32.c`, `hero_ui_win32.c`). The jump machinery is in `runtime/parts/`, where every file that interposes a jump (`sigjump*.c`, `sigentries.c`, `siglookup.c`) is `#if !defined(_WIN32) && !defined(__APPLE__)`, and `sigguard.c`'s Windows branch says "no probe and no walk (panel 247's R10)" | **the grep proves nothing**: it searches the wrong directory. The evidence is those guards |
| 1.10 | thunk at `main_1k.emitted.c` 212-218; "the runtime therefore **knows** the thunk's frame address and its return address" | `sed -n 210,219p`; read `sigguard.c:573-627`, `callback.c` | lines hold (212 signature, 213 entered, 215 left, 217 brace). Both addresses are passed in: `hero_callback_asked` uses them for its walk, and `hero_callback_in` stores nothing | **imprecise**: nothing keeps either address today. (c) has to add that |
| 1.11 | two definitions of `hero_callback_entered`, `sigguard.c` 573 and 636 | `grep -n` | 573, 636 | holds |
| 1.12 | line 189's slots, "its `own` slots `h6_own6`, `h10_own10`, `h11_own11`", released at "lines 199 to 203" | `grep -n` | the prologue declares `h3_xs0` (the `for` loop's array) and `h6_own6` to `h9_own9` (four strings) besides those three. Nine slots are released, at **200-208**; line 199 is `#line 200` | **imprecise**: a prototype that registers the named slots alone misses five owners |
| 1.13 | "a string of about 1 KB and an array of 50 numbers", "about 3.5 KB a jump" | `leaks --atExit -- ./main_1k.s2` (stage 2) | `7000 leaks for 3952000 total leaked bytes`: per jump, 2 blocks of 1,280 B (the 1,000-byte `repeat` and the concatenation), 2 of 640 B (`range(0, 50)` and `nums`), 2 of 32 B, 1 of 48 B | **imprecise**: **7 blocks and 3,952 bytes a jump**, two strings and two arrays. The resident size gives 3.50 KB from the high reading and 3.05 KB from the low one. `leaks` counts blocks; a resident size does not |
| 1.14 | "eight runs", "between 307,429,376 and 352,927,744" (shared); "the shared brief gives all eight readings" (compiler-engineer Q4) | `cat leak/time_*.txt` | **one** run each: 5,963,776 and **352,894,976**. 352,927,744 appears in no file | the eight readings are not in the evidence, and the compiler-engineer's sentence is **false** (the shared brief gives three values and a count). "macOS compresses resident pages, so the low readings are the machine's" is an inference, unrun. The file's own `peak memory footprint` (352,273,056) is the reading that counts compressed pages |
| 1.15 | "the files differ only by blank lines the formatter added" | diff of my stage-2 emission of the folder's `main_1k.hero` against `main_1k.emitted.c`, with `#line` and `HeroPlace` numbers masked | identical but for two joined blank lines and the `hero_program_sources` key. The brief's `#line`s name a source without the folder's blank lines (`for i` at 9 against 10, `ab_throw` at 11 against 13, `main` at 14 against 16) | holds, but no `#line` in `main_1k.emitted.c` names a line of the folder's `main_1k.hero` |
| 1.16 | `hero_thread_guard` at `thread.c:141`; the zero-initialisation rule of `generated-c.md` | `sed -n 135,150p`; read | both as stated | holds |
| 1.17 | interim cost: "six runs each, 0.67 to 0.69 s before and 0.83 to 0.85 s after, about +1.6 ns a callback (two thread-local stores)" | `gh issue view 2704 --json body,comments` | "old runtime 0.67 to 0.69 s **(one 0.85)**, new 0.83 to 0.85 s **(one 0.91)** … the two thread-local **accesses**" | **imprecise**: the brief drops the two outliers, and the old run's 0.85 falls inside the new range |
| 1.18 | "A frame record on every function entry has not been measured by anyone" | `grep -rli 'frame record\|shadow stack\|frame chain\|per-function record' docs/panel/*.md docs/measurements/*.md` | 5 sittings (156, 247, 248, 249, 253). In every hit "frame record" means the **hardware's** frame-pointer record, the pair the signal guard walks (`HERO_SIGNAL_WALK`), panel 249's route C (`-fno-omit-frame-pointer`), `stack.c` | holds **under those words**, which is a negative resting on my vocabulary. **The name collides**: in this tree "frame record" already means the hardware's. Rename (c)'s structure in the briefs (for example "slot record") |
| 1.19 | the same sentence, read as "nothing like it exists" | read `runtime/parts/frame.c` | a per-thread LIFO pushed in a prologue and popped at the one exit **already exists**: the large-value room (`hero_frame_take` and `hero_frame_give`, defect 2278, panel 233) | it is the shape (c) proposes, it is already exposed to the jump, and no brief names it. § 2, F1 |
| 1.20 | goldens `fixedbugs-2704-*`, three cases and two headers; "a handler that `siglongjmp`s out" | `ls`, `cat` | as stated. The handler case fires with `raise(SIGUSR1)` inside `wd_fire()`: a **synchronous** signal at a C call site | holds, but a real watchdog's timer is asynchronous, and that case ends otherwise today (§ 2, F6) |
| 1.21 | `libpng` 1.6.59; panel 257's `pngread` in `evidence.tar.xz` | `pkg-config --modversion libpng`; `tar -tJf` | 1.6.59; `pngread.hero`, `pngread.h`, `pngread_main.c` are present | holds. What `pngread` exercises is § 2, F7 |
| 1.22 | `pmset -g assertions` | run | Amphetamine, `PreventUserIdleSystemSleep 1` | holds |
| 1.23 | `heroes-linux-arm64:latest`; `docs/platforms/linux/LINUX-MACHINE.md` | `docker images`; `ls` | both present | holds |
| 1.24 | decision 2751; the milestone's section of the day | `gh issue view 2751`; `grep -n` | closed, `kind:decision`; `M-rebel-library.md:781` | holds |
| 1.25 | compiler-engineer: "never `crates/`, which is archived: nothing builds it" | `ls crates` | `No such file or directory` | **imprecise**: there is no `crates/` at `c4fc5b46a`. The Rust is at tag `m-bootstrap-archive` (CLAUDE.md § 5) |
| 1.26 | compiler-engineer: "the cheap route is the seed built with clang in a few seconds"; shared: stage 2 only "if you prototype in `selfhost/`" | stage 1's and stage 2's `--emit-c` of `main_1k.hero`, `grep -c 'hero_callback_left()'` | **stage 1: 0, stage 2: 1**. The seed predates defect 2704's thunk line, so a stage-1 program counts every callback entered and none left, and is never audited. Issue 2704's closing text says the same. The seed took 31.84 s here, not "a few seconds" | **false as a route**: every seat that reads the count, the audit or LeakSanitizer, or prototypes the hook's bookkeeping, needs **stage 2** (271.51 s here) |
| 1.27 | design.md §1.1, §1.7, §1.11, §1.12, §4.19, Part 5; spec § 13's sentence is panel 257's; `HERO_RUNTIME_ABI`; decision 2228's promise | `grep -n`; `git log -S 'saying the handler was not seen to end'`; `gh issue view 2228` | lines 176, 427, 484, 642, 2654, 3451; `598c0e562` (panel 257); `ABI 32`; `design.md:3794`: the stability of `hero_os.h`'s functions "promised with the rest at M-compatibility-promise" | holds |
| 1.28 | ffi: the box's lock `C:\w\box.lock` "with an owner file"; up "to about 06:29" | `git grep 'box\.lock'` | found only in `ffi-pragmatist.md` itself. No tracked document states the lock protocol. The timer was not asked | **unverifiable here**: cite where the protocol lives |
| 1.29 | ffi: "MSVC's `longjmp` unwinds through SEH and runs termination handlers" | `grep` of `seed/README.md:60-61`, `WINDOWS-MACHINE.md:411` | clang targets `x86_64-pc-windows-msvc`, so it is the UCRT's `longjmp` | **a claim for the seat** (unrun here). If it holds, shared point 2, "The jump runs no code", is **false on Windows**, and a Windows arm of (c) is a termination handler, not a hook |

## 2. Doors and routes the briefs do not list

### Measured on this Mac, stage 2 (and stage 1 where both are said)

**F1. The large-value room is a second stack the jump unbalances, and it aborts a correct program
with a false *compiler bug*. 3 of 3 under stage 1 and 3 of 3 under stage 2.** A value of a program's
own type past 4,096 bytes lives in `parts/frame.c`'s per-thread room. It is taken in the prologue and
given back at the one exit, and "a give that is not of the newest take is the compiler's error"
(`frame.c:26-28`, the panic at `:121`). A callback holding such values (`body` takes 832 KB a call: 10 × 32 KB and
2 × 256 KB, counted in its emitted prologue) jumped over 8 times while `main` holds a 32 KB value
exits **134** with `panic: internal error: a large value's room was given back out of order
(compiler bug)`, called from `room_order.main`. With **one** jump it exits 0 (`room_one`, 3 of 3): the
next give back inside the same chunk pops the abandoned room as a side effect, so the room heals itself
until a jump leaves a fresh chunk on top. Arithmetic, **unrun**: with no large value in `main`,
`hero_frame_held` grows 832 KB a jump against `HERO_FRAME_ROOM`, 1 GiB, so about 1,260 jumps would
end in `panic: stack exhausted … the room large values take off the stack is full`. Defect 2704's
interim does not reach this path, so **(a) is not "the leak whole": it is a false abort.**

**F2. The handle gate accuses the program for a release the jump skipped. 3 of 3, stage 1 and stage
2.** A callback that opens a handle (`slot_open … acquires slot_close`), calls C that `longjmp`s, and
would then have called `slot_close(a)` exits **134** with `panic: 1 C handle(s) never given back …
this program is missing that many`, `taken at handlejump.hero:10:9, in handlejump.body`. This is the
same class as defect 2704, on the gate that repair did not touch (1.4). It also bears on (c): a
handle is released by an explicit consuming call, not at the end of a scope, so "release what those
frames held" means **running the binding's C consumer** inside the hook. The proposal does not say
whether it does that, forgives the gate, or neither.

**F3. A lent buffer's region is never given back, outside every gate.** A callback calling
`lj_fill(@md: [u8] counted_by 4096 lent)`, whose C `longjmp`s out of the lending call, 20,000 times:
exit 0, maximum resident **334,725,120** bytes (twice, stage 1 and stage 2), about 16.7 KB a jump.
The same program whose C returns instead reads **2,981,888**. The regions are `mmap`ed by
`parts/lend.c` and lie outside `alloc.c` "on purpose" (`lend.c` doc), so no gate and no LeakSanitizer
ever sees them. `hero_lend_depth` is left one higher per jump.

**F4. Read, not run.** The lease gate (`.lease()` owes `end_lease`, `alloc.c:418-431`) and the
scratch gate (`hero_run_*`, `hero_dir_*`) read no count either, so a callback holding a lease or a
directory listing when C jumps should end as F2 does. **Unrun.**

**F5. A thread `spawn` started, ended by C's `pthread_exit`, makes `wait` read through a null. 3 of
3, stage 2.** `spawn`'s thread runs `h_0run_…`, which calls the body **directly, with no thunk and no
count** (emitted C, `hero_answer = hero_box->f(hero_arg)`). The body calls an extern that calls
`pthread_exit(NULL)`, so the answer is never put, and `t.wait()` exits **134** with `panic: a null
pointer was read through, at offset 0x38, called from h_0take_1b9a87 … This runtime raised neither`.
Two consequences: (c)'s chain, which "the thunk starts", does not cover a thread body; and a null
read, caught here, is a rank-3 matter today, under the interim. Glibc's `pthread_exit` unwinds by
force: **unrun on Linux.**

**F6. An asynchronous watchdog ends the program today, with words naming a cause that is not there.
25 of 25 processes, stage 2.** A C handler for `SIGALRM` (`setitimer`, 500 µs) that `siglongjmp`s out
of a callback which builds strings, up to 300 jumps a process. Every process exits **134** with
`panic: a signal handler used this runtime's allocator while the code it interrupted was inside it …
a Heroes function given to C as a signal handler, or called from one`. No Heroes function ran in a
handler. The jump left `alloc.c:617`'s thread-local bracket `hero_alloc_inside` set, and the landing
code's next allocation tripped it. So the brief's hazard 7 is narrower than the world: an
asynchronous jump abandons frames at **any instruction**, inside the runtime's allocator, between a
slot's store and its old value's release, or inside `hero_array_push_owned`. That is not "at a C call
site", which is all the compiler-engineer's Q1 asks. Under POSIX a jump out of a handler that
interrupted `malloc` is undefined afterwards anyway, so whether (c) covers asynchronous jumps at all is
a ruling nobody has asked for.

**F7. `libpng`'s own error on a corrupt image abandons no Heroes frame in `pngread`'s shape.**
`pngread.h`'s C read function calls the program's callback **first**, then `fread`, and `png_error`s
on a short read after the callback has returned. A CRC is checked by libpng after the read function
returns. Run: 2,000 truncated (`trunc.png`) and 2,000 bad-CRC decodes (`good.png` with one byte of
IDAT flipped), with a callback holding about 100 KB a call. All 4,000 returned -3 (libpng jumped).
The program exits **0** under stage 2's live audit, maximum resident **3,489,792** bytes, footprint
2,458,032. 4,000 abandoned 100 KB frames would be about 400 MB. In this shape only the program's
own `pr_fail()` from inside the callback abandons a frame. So shared point 4, "a server that decodes
untrusted images through `libpng` loses that on every corrupt image", is **false for this binding**,
and the ffi-pragmatist's Q2 (a loop of corrupt images through `pngread`) will measure no leak. The
real question is which binding shapes put a Heroes frame above a library's jump: a read callback that
reports its own stream's failure by `png_error`, a progressive reader's row callback that calls back
into libpng. **And none in the tree today**: `git grep -l -E
'longjmp|siglongjmp|png_error|png_jmpbuf|setjmp|swapcontext|setcontext|pthread_exit|pthread_cancel|lua_error'
-- examples examples-rebel packages runtime/platform` finds one hit, a ledger line naming Python's
`decimal.setcontext`. Rebel's PNG is written in Heroes (`packages/rebel/0.1.0/files/image/png.hero`).

### Doors, read and not run

- **D1. A handler that returns with an edited register file**, the way every recovery is written
  according to `parts/stack.c:868-878`, abandons frames with no `longjmp` and no `setcontext`
  (libsigsegv's shape). Unseen by every hook.
- **D2. An Objective-C or Swift exception through the platform layer's AppKit UI**
  (`runtime/platform/hero_ui_appkit*.swift`), an unwinder rather than a jump, caught by AppKit's run
  loop. The briefs name C++ only.
- **D3. A jump made by an image inside Darwin's dyld shared cache** (`rebind.c` rewrites only outside
  it).
- **D4. `sigjump.c:66-74` lists what Linux's route does not reach**: a save reached by `dlvsym`,
  `__builtin_longjmp`, assembly, C++ and `_Unwind_*`, and the residue `rr_2`. Shared point 8 names
  four and says "none of it checked". That list is in the tree.
- **D5. A coroutine never resumed is no event at all.** There is no jump to hook, and C may free its
  stack, records included, before any "safe point". Shared point 1 counts it among the jumps; (c)
  cannot reach it.
- **D6. (c)'s chain after a jump it did not see.** Records left by an unseen jump point into stack
  memory the landing code reuses. A later frame's push links onto them, and the next **seen** jump
  walks them and releases what the reused memory now holds. A stack switch by hand-written assembly
  interleaves two stacks' records on one thread-local chain. In both cases **(c) turns a leak into
  memory corruption**, which the interim cannot do. Shared point 8 ("the interim rule stays") assumes
  an unseen jump costs only a leak.
- **D7. A release that runs C inside the library that is jumping.** libpng keeps its `jmp_buf` inside
  the `png_struct`. A hook that releases a png handle held by an abandoned frame (F2's consumer)
  frees the buffer `longjmp` reads next.
- **D8. The runtime's own rules on thread-local state.** The handler-side record is reached "through
  the thread key … never a thread-local access (which may allocate inside a signal on a thread nothing
  claimed)" (`sigentries.c:60-62`, the accessor at `:63`). On Darwin arm64 a `_Thread_local` read inside a key destructor
  reads 0 (`alloc.c:1668-1672`, panel 111). A "thread-local chain" drained in a handler or at "the
  thread's end", two of Q3's safe points, must hang off the key.

### Routes and options beyond (a), (b) and (c)

- **(d) Table-driven forced unwinding**: `__attribute__((cleanup))` on every owned slot, `-fexceptions`,
  and `_Unwind_ForcedUnwind` called from the hook with a stop at the target, which is glibc's own
  route for `pthread_exit` and cancellation. It adds no instruction on entry, and it would also cover
  D2 and glibc's `pthread_exit`. Its Windows arm is `__try/__finally`, if 1.29 holds. It needs unwind
  information in every C frame in between. **Unrun.** Note: `frame.c:135-143` describes
  `hero_frame_give_at` "for `__attribute__((__cleanup__))`" through `large_value.hero`'s
  `taken_to_exit`, but `grep -rn '__cleanup__\|taken_to_exit\|give_at' selfhost/` finds only the
  guarded-names list and `hero_lend_give_at`. A stale comment or a removed route, a question.
- **(e) Detect lazily by stack pointer, with no hook.** At an extern call's return or a thunk's entry,
  any thunk entry whose frame lies deeper than the current stack pointer on the same stack belongs to
  an abandoned extent. The runtime's stacks are then unwound to that entry's saved markers: room top,
  lend depth, count. This works for unseen jumps and on Windows, and repairs F1 and F3 without reading
  a slot; values stay leaked unless their pointers live off the stack.
- **(f) A hybrid**: release in the hook where `hero_sig_standing(own)` is false (no handler stands,
  libpng's synchronous case), and defer only where one does. Q3 poses the choice as either/or.
- **(g) A told refusal at the hook**: stop the program, naming the jump and the callback. This is the
  loud direction (`.claude/rules/module-shape.md`), and it breaks a binding that reports a stream
  error by `png_error` from a callback. Its prior art (Rust's `C-unwind` and "plain old frames",
  cgo's rule against C jumping over Go frames, OCaml's callbacks) is **unverified here**, and the
  soundness lane has no historian to source it.
- **(h) A release point**: the return of the extern call into Heroes code, the earliest instruction
  after a landing, never inside a handler. It is not in Q3's list.
- **Which functions need a record, a fourth route for Q2**: callback-reachable **and** live across a
  call into C (an extern or a function value). A frame that calls no C cannot be on the stack at a
  synchronous jump. An asynchronous one widens this to every callback-reachable frame (F6).

## 3. The question the sitting should have asked

**What does a frame a C jump abandons hold besides its refcounted slots, and which of the runtime's
per-thread stacks and exit gates does the jump leave unbalanced?** Measured above: the large-value
room (F1, false *compiler bug*), the handle set (F2, false accusation of the program), the lend
regions (F3, outside every gate), leases and scratch (F4, read), a `spawn` thread's answer (F5, a
null read), and the allocator's bracket (F6, a false cause). The consequences:

- **The conservative resolution the briefs name, (a), is not a safe baseline.** It is a leak only
  for heap blocks, and several correct programs abort under it today. The coordinator should file
  these, not this critic: F1, F2, F5 and F6's words look `blocking` by `verification.md` § Bounded
  discovery; F3 is a leak no instrument sees.
- **(c) must be specified as an unwinding of every runtime stack**: count, room, lend depth, handle
  and lease obligations. Otherwise turning "the audit on again" still aborts F1 and F2.

Two narrower questions also go unasked:

- **Under which events does (c) release a value that is still live** (D6, D7)? This is where (c) can
  cost rank 3 and the interim cannot.
- **Is an asynchronous jump in (c)'s scope at all** (F6)? If it is, Q1's "at every C call site" is
  the wrong frame.

## 4. What I did not do

I did not prototype (c), time anything, run Linux, Windows, the pngread loop at 100,000, or any door
in § 2's read-only list. I did not file anything. My worktree holds two compilers and `build/`; I
leave its removal to the sweep's rules or the coordinator (it holds no commit).

## 5. Reproducers (stage 2, `heroes-next build <file> -o <bin>`, the header beside the program)

`handlejump.h` / `handlejump.hero` (F2):

```c
#include <setjmp.h>
#include <stdint.h>
#include <stdlib.h>
typedef struct Slot Slot;
struct Slot { int64_t n; };
static inline Slot *slot_open(int64_t n) { Slot *s = malloc(sizeof *s); s->n = n; return s; }
static inline void slot_close(Slot *s) { free(s); }
static inline int64_t slot_value(Slot *s) { return s->n; }
typedef int64_t (*hj_cb_t)(int64_t);
static jmp_buf hj_env;
static inline void hj_throw(void) { longjmp(hj_env, 1); }
static inline int64_t hj_try(hj_cb_t cb) { if (setjmp(hj_env) == 0) { cb(1); return -1; } return 7; }
```

```
extern "handlejump.h"
    record Slot tag Slot
    function slot_open(n: i64) -> Slot acquires slot_close
    function slot_close(s: Slot consumes)
    function slot_value(s: Slot) -> i64
    function hj_try(cb: (function(i64) -> i64)) -> i64
    function hj_throw()

function body(n: i64) -> i64
    a = slot_open(n: n)
    v = slot_value(s: a)
    hj_throw()
    slot_close(a)
    return v

function main()
    print("result ", hj_try(cb: body))
```

`room_order.hero` (F1): `leak/fixedbugs-2704-longjmp.h`'s `ab_try` and `ab_throw`; records `E0`
(`x: i64`) to `E5`, each `E<k>` eight fields `a` to `h` of `E<k-1>`, as in
`tests/golden/run/fixedbugs-2278-a-two-mib-record-built-in-one-function-runs-at-every-level.hero`;
`body(n)` builds `v0 = E0(x: n)` up to `v5`, calls `ab_throw()`, returns `v5.a.a.a.a.a.x`; `main`
builds `w0` up to `w4`, then `for _ in range(from: 0, to: 8)` adds `ab_try(cb: body)`, and prints
the sum and `w4.a.a.a.a.x`. With `to: 1` it exits 0.

`pexit.h` is `static inline void px_exit(void) { pthread_exit(NULL); }`, and `pexit.hero` (F5) is:

```
extern "pexit.h"
    function px_exit()

function worker(n: i64) -> i64
    word = f"{n}-held" + "0123456789".repeat(100)
    px_exit()
    return word.len()

function main()
    t = spawn(worker, 7).must()
    print("answer ", t.wait())
```

`lendjump` (F3): a C `lj_fill(unsigned char *md)` that writes `md[0]` and `longjmp`s, bound
`function lj_fill(@md: [u8] counted_by 4096 lent) -> i32`, called from a callback holding
`md: [u8] @= []`, 20,000 times through a `setjmp` like `ab_try`; the control returns 0 instead.
`alarm` (F6): a `SIGALRM` handler `siglongjmp`ing out of a callback that loops `while true` building
a string and pushing onto an array, `setitimer` at 500 µs, 300 times a process. `corrupt.hero` (F7):
panel 257's `pngread.h` unchanged, a callback holding `f"{count}" + "0123456789".repeat(10000)`, 2,000
decodes each of `trunc.png` and a bad-CRC copy of `good.png`.
