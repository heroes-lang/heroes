# Panel 255, round two, compiler-engineer: C4 built and run on Linux

Written 2026-11-22 as I went, by the compiler-engineer seat. Read first, in the brief's order:
`255-briefs/round2.md` whole (156 lines), `00-shared.md` § 6 to § 8, `critic-reports.md` findings 1 to 13,
`critic-round2-brief.md` whole, my round-one report. Everything below ran in my copy,
`.claude/worktrees/p255-scratch/compiler-engineer/` (written `CE/`), a detached worktree at `a75179641` whose
tracked tree is clean (`git diff --stat HEAD -- runtime selfhost` prints nothing); round two's work is under
`CE/p255/r2/`. I read no other seat's folder; every input came from `255-briefs/round2/`.

## 1. C4 on Linux, as built

Four files replace the tree's under `runtime/parts/` (`CE/p255/r2/routes/C4/`); no line of the compiler
moves, no spec token. Sizes by `wc -l` and `diff | grep -c`:

| file | tree | C4 | diff |
|---|---|---|---|
| `sigjump.c` | 283 | 764 | +557 / -76 |
| `siginstall.c` | 362 | 699 | +381 / -44 |
| `sigguard.c` | 523 | 554 | +31 / -0 |
| `siglookup.c` | 197 | 198 | +1 (`dlsym("makecontext")` answered with the wrapper) |

Compiled clean on Linux arm64 under `clang -Wall -Wextra`, plain and `-fsanitize=address`
(container `ce255r2-chk`, no warning in the four files).

**Rule by rule, what the Linux build does** (the functions are in `siginstall.c` unless named):

1. **The decode.** A jump of the `setjmp` family reads one word of its target and decodes the stack pointer
   by the key (`sigjump.c` `hero_jmp_sp_of`); a context jump (`setcontext`, `swapcontext`) reads the
   context's own `uc_mcontext` field. No fingerprint, no pending pair on that path.
2. **The calibration** (`sigjump.c` `hero_jmp_calibrate`, before `main`): the probe fills every callee-saved
   register with a pattern; the stack pointer's word, the second mangled word (arm64 `x30`, x86-64 `rbp`
   and the return address) and the unmangled patterns are verified under ONE key, for `setjmp`, `_setjmp`
   and `__sigsetjmp`; `getcontext` AND `swapcontext`'s save half (the critic's r2 item 7) are verified by
   their field; each at two frames `0x9230` apart on the thread's own stack AND on a second 256 KiB stack
   `mmap`ed for it (reached by a 9-line assembly helper, `hero_jmp_on_stack`, with the calibration
   functions built without ASan instrumentation so no shadow is written on the borrowed stack). The key
   and the context field are verified and taken APART: the key is the C library's private, mangled layout;
   the field is its public one.
3. **Where the key does not verify, A**, the record and its fingerprint read only of provably live memory,
   as round one; **contexts stay read by their field** (public ABI). Two additions: A's pending buffers are
   read at every handler's begin (`hero_jmp_observe`, called by the wrapper), which closes round one's A
   residue `x_unseen0` (a refill inside the handler by an unseen save with no save between); and
   **`int hero_signal_jump_decoding(void)`** answers 1 (the key), 2 (contexts), 3 (both) or 0, for a test
   to ask; `HERO_JMP_FORCE_UNCALIBRATED` forces the key's failure. Rules 4 to 7 bind the A path unchanged.
4. **Stacks**: the alternate stack registered now FIRST (the kernel's word, `sigaltstack(NULL)`), then a
   context stack `makecontext` named on this thread, then the thread's own, then unknown. **Known before
   own**, so an array of the thread's own stack used as a coroutine's stack (shape A) is never compared by
   address with a handler below it. Each entry records its stack (kind, bounds, slot and the slot's
   sequence) when pushed; an entry whose known slot was rewritten or proved stale since is judged by where
   its frame lies now (`hero_sig_entry_where`).
5. **A swap made while a handler stands suspends it** (`hero_sig_swap_leaves`, called by the
   `swapcontext` wrapper, which is not a tail call): every standing entry is marked suspended, and the
   lowest frame of each handler on the swap's own stack is noted. **The suspension is per entry** (not a
   watermark), lifted when the swap comes back in its own frame for the handlers whose frames hold that
   frame (`hero_sig_swap_back`), when a seen jump or swap lands inside a suspended handler's frames, or
   when the handler ends. **No prune drops a suspended entry**, with ONE exception that is a proof, not a
   prune (`hero_sig_overwritten`): the thread seen on the same stack BELOW the lowest frame a swap
   suspended a handler at, the handler not resumable (below), means whatever runs there grew through the
   handler's frames, which can then never be resumed. **A handler's end** removes its own entry and every
   entry pushed after it on its own stack below its frame, and keeps the rest (`kC5`'s `own->depth = at`,
   which drops a nested handler suspended on another stack, is gone: shape N). **No frame prunes by its
   address alone**: the wrapper's push and the callback's check drop nothing by their frame (C3's did), and
   the callback's check refuses while ANY entry stands. Reason, rule 5's own text and the shape it forces:
   *A by an unseen switch* (a coroutine on an own-stack array reached by hand-written assembly) is
   indistinguishable by address from a thread that left the handler by a jump the runtime never sees, so
   any frame prune runs the first inside the signal; only seen transfers drop entries now.
6. **`setcontext` and the alternate stack.** A seen jump (`hero_sig_jumped`): entries on the same stack and
   deeper than the target are left; across stacks (`hero_sig_cross_left`) an entry is left when the thread
   leaves its stack for one whose frames cannot be the handler's: off the alternate stack for any other
   stack (kC5's), off a coroutine's stack for the thread's own or another coroutine's, off the thread's own
   stack for a coroutine's; a range carved from the thread's own stack counts as another stack only where
   the other address lies above it. **The rule that runs `b_*` and refuses A′**: a `getcontext` made while
   a handler stands, at a frame that may lie inside it, marks it **resumable** (`hero_sig_context_saved`):
   that context may resume it by a way no wrapper sees (a coroutine's return through `uc_link`, which is
   glibc's own internal `setcontext`, unseen: `llvm-readelf -r libc.so.6` relocates no context function,
   the critic's r2 finding 5). A jump that would leave a resumable handler across stacks HOLDS it suspended
   until it ends instead. `b_*` saves no context inside its handler, so its `setcontext` leaves it; A′ does
   (`getcontext(&hc)`), so it is held. Which correct program pays: one that saves a context inside its
   handler and then escapes by `setcontext` or `longjmp` to another stack, never resuming it: refused,
   told, for good.
7. **Known context stacks on Linux**: an assembly `makecontext` wrapper (arm64 and x86-64; `sigjump.c`),
   the argument registers kept, the range noted, a branch to glibc's own; on glibc's four other
   architectures none (no stack known: a told refusal where a coroutine's frames matter). The range is read
   at `makecontext`, where glibc reads `uc_stack` and nowhere else (the critic's disassembly), so it is
   never stale WHEN noted; **shape U is therefore not decisive here** (nothing reads `uc_stack` at a swap)
   and I did not build it. **What tells the runtime a known range is no longer a stack**: a later
   `makecontext` overlapping it; the alternate stack registered now overlapping it (F″); and, for a range
   carved from the thread's own stack, the thread seen on its own stack outside every known range ABOVE the
   range's top (a jump's target, a handler's frame, a callback's frame, a swap's frame): its holder frame
   returned (F, F′). The table is per thread (written only by its thread, a sequence per slot, odd while
   written, so a handler interrupting a write passes over it: no lock, async-signal-safe); a coroutine
   made on one thread and run on another is of unknown extent there.
8. **Same verdicts on both platforms**: Linux's half is below; Darwin's is the ffi-pragmatist's, which I did
   not read. The rule set above is written so the critic can diff it against Darwin's.

*C4 forced to fail* is the same four files with `#define HERO_JMP_FORCE_UNCALIBRATED 1`
(`CE/p255/r2/routes/C4fail/`). *C, 2679 alone* is round one's C (`sigjump.c` only) with rules 2 and 3 applied
(the same calibration, byte-identical by `diff` of the section, the accessor, contexts by their field) and
today's `siginstall.c` and `sigguard.c` (`CE/p255/r2/routes/C2679/sigjump.c`, 283 -> 671, +460 / -72).

## 2. The shapes

**Written this round** (`CE/p255/r2/shapes/xn.h`, beside the critic's `xr.h`, whose helpers it uses):
`ap_{1,2}_{0,1}` A′ (`getcontext` + `setcontext`, `uc_link` the handler's context; orient 0 an own-stack
array and a handler on the own stack, orient 1 a heap stack and a handler on the alternate stack);
`au_{1,2}_{0,1}` A by an unseen switch (a 17-line assembly switch per architecture, the coroutine on an
own-stack array); `n` (H1 swaps to K1, H2 on K1 swaps to K2, K2 resumes H1, which returns, then the runner
resumes K2, which calls the program); `fp` F′ (`f` with one swap into the context before the helper
returns); `fpp` F″ (a coroutine's `mmap`ed stack unmapped, mapped again at the same address with
`MAP_FIXED` and registered as the alternate stack, then `d_1`); `g_{1,2,3}_{0,1}` G's three forms
(swap to the runner's pre-signal context; swap to a coroutine that `siglongjmp`s to the runner's buffer;
swap to a coroutine that returns through `uc_link` to the runner's context), each then calling the program
64 KiB deeper, as `b_*` does; `acc` the accessor.

**Ported to Linux, said** (r2 rule 8): FP's `ctx2.h`, `jumpout.h` (`jo_co`) and `ctx.h` allocate a
`ucontext_t` with `calloc(1, 1024)`, Darwin's size; glibc's is larger, so each became
`calloc(1, sizeof(ucontext_t))` (the critic did the same for `ctx2`). `jo_lib` opens `./libjo.so`, built
from FP's `jolib.c`. Darwin's `t_partial`, `t_unseen`, `t_unseen0` read Darwin's pointer guard
(`TPIDRRO_EL0`, `[x16, #0x38]`) and Darwin's `jmp_buf` offsets, so on Linux their twins are my `x_partial`,
`x_unseen`, `x_unseen0` (the same shapes on glibc's layout), and the `t_*` themselves were not run.
FP's `pngdemo` and my round-one `png.hero` run in `heroes-linux-arm64:gtk4-ui`.

## 3. The legs

Linux arm64, `heroes-linux-arm64:latest` (`aarch64`, `Debian GLIBC 2.41-12+deb13u3`, `Debian clang version
22.1.8`), one container per column, my copy mounted read-only, `CE/p255/r2/leg2.sh` inside: the tree copied, the
column's files over `runtime/parts/`, stage 1 from the seed, **stage 2** (`./heroes build selfhost/main.hero`,
then `mv`), then the accessor, the eight 2679 shapes, the union and the new shapes (`CE/p255/r2/shapes/run-r2.sh`:
each shape built plain and `--sanitize`, run 3 times under `timeout 20`, one line per distinct outcome), the `ij`
cases (5 runs each, `-O0` and `-O2`), the case sets of `00-shared.md` § 4 (four at a time) and the `runtime`
suite. Containers `ce255r2-{today,C4,C4fail,C2679}`, all four started together beside an emulated x86-64 leg and a
libpng leg (load 3 to 8 on this Mac's 8 cores: no timing below is a resolution's ground). Files
`CE/p255/r2/out/<column>/`.

| stage 2 (arm64) | today | C4 | C4 forced | C, 2679 alone |
|---|---|---|---|---|
| built in | 243 s | 244 s | 244 s | 244 s |
| `heroes --version` | `sources 61416c0c33ecc5a9` (the tree's key) | `5ae02d7665de0286` | `04f8e46926c15f58` | `1647aec447ad7e35` |
| **the accessor**, `acc.hero`, plain / `--sanitize` | **`ffi_missing_link`**: today's runtime has no way to say its path | **`decoding: 3` / `3`** | **`decoding: 2` / `2`** (the `setjmp` family on A, contexts by their field) | `decoding: 3` / `3` |

The calibration verified under `--sanitize` as plain, so the second, borrowed stack works under
AddressSanitizer. Stage 1, linked with its column's runtime, built stage 2, and stage 2, linked with it too, built
every program below: the constructor's calibration ran at every start of both compilers.

### 3.1 Defect 2679's eight shapes (`p255/repro/`, the brief's, `diff -rq` differing only in its logs)

| arm64, 1 run each as the brief's runner does | today | C4 | C4 forced | C, 2679 alone |
|---|---|---|---|---|
| `heap`, `context`, `handler_heap` | 0 plain; **ASan heap-use-after-free** sanitized | 0 / 0 | 0 / 0 | 0 / 0 |
| `mmap`, `handler_mmap` | **139 SIGSEGV** plain; **ASan SEGV** sanitized | 0 / 0 | 0 / 0 | 0 / 0 |
| `stack`, `plain_heap`, `plain_mmap` | 0 / 0 | 0 / 0 | 0 / 0 | 0 / 0 |

**16 of 16 exit 0 with no sanitizer line in each of the three new columns**; today 2 SIGSEGV plain and 5 sanitizer
reports, as the coordinator's table.

### 3.2 Two defects of my first build, found by the union and repaired (the first build's logs: `CE/p255/r2/out-first/`)

The first C4 build ran the union's first 120 rows before I stopped it. **Four MUST BE REFUSED shapes ran the
program inside the signal** under it, plain and `--sanitize`, 3 of 3: `a_2_0` (`answer ran for 2`), `c2_1`
(`answer ran for 1`), `ap_1_0` (`answer ran for 11`) and `ap_2_0` (`answer ran for 12`); `a_2_1` fell to the
walk (K). Both causes are mine, read in my code against those lines:

- **the lowest frame a swap suspended a handler at** was the `swapcontext` wrapper's frame ADDRESS
  (`__builtin_frame_address(0)`), and the context glibc saves there resumes at the wrapper's stack POINTER,
  below that address. So the coroutine's swap back into the handler (`a_2_0`, `c2_1`) read as a target below
  the handler's lowest frame, and the overwrite proof dropped a handler about to run again. Repaired: the
  wrapper reads its own stack pointer (one instruction, `hero_jmp_stack_pointer`), and the proof applies only
  to a handler a swap suspended from inside its own frames (`entry_low` 0 otherwise);
- **a fresh `makecontext` context's stack pointer is the very top of its range** on glibc arm64, so `[lo, hi)`
  classified A′'s coroutine (an own-stack array) as the thread's own stack above the handler, and its
  `setcontext` as leaving the handler. Repaired: an address is classified by the byte below it, where the
  next push lands.

What it says about the design, not only my build: **every one of C4's rules turns on an exact address
comparison, and an off-by-one-frame reading of them runs a program inside the signal, silently, with no
sanitizer line** (both defects are clean under `--sanitize`). Only a union of shapes that switch contexts,
run under both modes, saw them; the tree holds no such case (critic-reports finding 8). The landing owes those
shapes as goldens before anything else.

A second thing the first build showed, not a defect: `f`, and my `fp` and `fpp`, exit 1 under `--sanitize` in
every decode column (C4, forced, 2679 alone) and 0 under today, the program's every line printed first.
`docker exec ce255r2-C4 ... ./bin-f-sanitize` read `ERROR: LeakSanitizer: detected memory leaks`, **4560 bytes
allocated at `xf.h:25`**, the shape's own `calloc` of a `ucontext_t`, never freed, its pointer lost when the
helper returns. Today's save-point record keeps the pointer (`jump_buf`), so LeakSanitizer finds it reachable;
the decode keeps no record. These rows RUN; their goldens must free what they allocate.

### 3.3 libpng 1.6.48, the issue's origin (`heroes-linux-arm64:gtk4-ui`, container `ce255r2-png`)

One stage 2 from the tree (`sources 61416c0c33ecc5a9`), each column's runtime a copy of `runtime/` with its
files, named by `HEROES_RUNTIME`; round one's `png.hero` (`-O0`, `-O2`) and FP's `pngdemo` (`-O0`), plain and
`--sanitize`, 3 runs each; `CE/p255/r2/out/png/png.log`. The C4 and forced columns of this first run were
C4's FIRST build; **re-run on the repaired build** (`ce255r2-png2`, `CE/p255/r2/out/png2/png.log`): every row of
both columns the same as below, exit 0, the whole output, no sanitizer line.

| | today | C4 (first build) | C4 forced (first build) | C, 2679 alone |
|---|---|---|---|---|
| `png`, `-O0` and `-O2`, plain | `0 68 3072 -1 3072`, exit 0 | the same | the same | the same |
| `png`, `--sanitize` | **exit 1, heap-use-after-free, `#0 hero_jmp_fingerprint sigjump.c:138`**, after `0 68` | **exit 0, the whole output** | exit 0 | exit 0 |
| `pngdemo` plain | `good 64048`, `truncated -3`, twice, exit 0 | the same | the same | the same |
| `pngdemo` `--sanitize` | **exit 1, the same heap-use-after-free**, after `good 64048` | **exit 0, clean** | exit 0 | exit 0 |

### 3.4 "C, 2679 alone" against today, the union on Linux arm64 (complete)

Both legs ran the whole union (68 programs, `union done (68 shapes)` in each `leg.log`, plain and `--sanitize`, 3 runs each; `CE/p255/r2/out/{today,C2679}/union.log`,
summarised by `CE/p255/r2/tools/cells.py`). **Every MUST BE REFUSED row today runs inside the signal, C, 2679 alone
runs the same way or refuses; no row moves the other way.** Today runs inside the signal, plain and
`--sanitize`, 3 of 3: `a_1_0`, `a_1_1`, `a_2_0` (shape A, three orientations of four, the critic's finding 5
reproduced), `ap_1_0`, `ap_1_1`, `ap_2_0` (A′: three of four, the heap orientation on the alternate stack
included), `au_1_0`, `au_1_1` (A by an unseen switch, how 1), `c2_1`, `c2_2` (`ctx2`), `ctx-m1` in three
orientations of four, `n` (`answer ran for 15`), and `x_unseen0`. Under C, 2679 alone the same rows run but
**`c2_1` and `x_unseen0`, refused** (S words), as round one's C refused them.

Rows that differ between the two, every one:

| row | today | C, 2679 alone | direction |
|---|---|---|---|
| `c2_1`, `x_unseen0` (MUST BE REFUSED) | runs inside the signal | refused, S | toward refusal, sound |
| `x_dlclose`, `x_prot` (MUST RUN) | 139 plain, ASan sanitized (2679) | run | 2679 closed |
| `x_realloc` (MUST RUN) | refused U plain, ASan heap-use-after-free sanitized | runs | 2679 closed |
| `x_refill_out` (MUST RUN) | refused U | runs | a correct program followed |
| `cx_heap`, `jo_co` `--sanitize` (MUST RUN) | exit 1, ASan (the context shape of 2679) | run, clean | 2679 closed |
| `gcopy` (ruling B) | refused U | **runs, `called 8`** | the author's question |
| `grefill`, `x_partial`, `x_unseen`, `ctx-m3` (MUST BE REFUSED) | refused U | refused S (`ctx-m3-o0-s1` K) | words only |
| **`g_3_0`** (G form 3, own stack: the author's question) | **runs** | **refused, S** | **a new refusal against today** |
| `f`, `fp`, `fpp` `--sanitize` | exit 0 | exit 1 after the whole output: LeakSanitizer, the shapes' own `calloc` (§ 3.2) | none: the record hid a leak |

`ap_1_0` and `ap_2_0` `--sanitize` exit 1 in both columns AFTER running inside the signal: `docker exec
ce255r2-today ... ./bin-ap_1_0-sanitize` printed `answer ran for 11`, then `ERROR: LeakSanitizer`, 262,144
bytes at `xn.h:39`, my own shape's unused heap stack (orient 0 allocates it and never frees it). Mine to repair
before it is a golden. Why `g_3_0` moves: today's R1 follows the coroutine's swap through a record whose
fingerprint was taken after `makecontext`, and prunes the handler by that record's frame; under C the swap is
decoded to the coroutine's heap stack, which proves nothing left on today's two-stack rule, and the coroutine's
return through `uc_link` is glibc's own `setcontext`, unseen, so the handler stands. **So on Linux "C, 2679
alone" moves no context row toward running inside a signal against today**, closes 2679's shapes and libpng,
refuses two programs today runs inside the signal, and costs one: `g_3_0`, a program whose verdict is the
author's question either way.

### 3.5 The union and the new shapes, every column (Linux arm64, the repaired C4; complete)

68 programs, each built plain and `--sanitize` and run 3 times (`union done (68 shapes)` in every `leg.log`),
generated from the four `union.log` by `CE/p255/r2/tools/cells.py` and `table.py` (the table below is its output,
`CE/p255/r2/tools/union-table.md`). A cell is `plain / sanitize` where they differ; `runs` is exit 0 with the
program's lines; S, U, K the refusal's words (*not seen to end*, *no record*, *called it while handling a
signal*); `(n)` a count of runs other than 3. In today's and 2679 alone's legs the runner did not yet name
LeakSanitizer, so their `1` after a full run is the same leak (read by `docker exec`, § 3.2 and § 3.4).

| shape | expected | today | C4 | C4 forced | C, 2679 alone |
|---|---|---|---|---|---|
| `a_1_0` | MUST BE REFUSED | runs | 134 S | 134 S | runs |
| `a_1_1` | MUST BE REFUSED | runs | 134 S | 134 S | runs |
| `a_2_0` | MUST BE REFUSED | runs | 134 S | 134 S | runs |
| `a_2_1` | MUST BE REFUSED | 134 K | 134 S | 134 S | 134 K |
| `ap_1_0` | MUST BE REFUSED | runs / 1 | 134 S | 134 S | runs / 1 |
| `ap_1_1` | MUST BE REFUSED | runs | 134 S | 134 S | runs |
| `ap_2_0` | MUST BE REFUSED | runs / 1 | 134 S | 134 S | runs / runs (2), 1 (1) |
| `ap_2_1` | MUST BE REFUSED | 134 K | 134 S | 134 S | 134 K |
| `au_1_0` | MUST BE REFUSED | runs | 134 S | 134 S | runs |
| `au_1_1` | MUST BE REFUSED | runs | 134 S | 134 S | runs |
| `au_2_0` | MUST BE REFUSED | 134 S | 134 S | 134 S | 134 S |
| `au_2_1` | MUST BE REFUSED | 134 S | 134 S | 134 S | 134 S |
| `b_0` | MUST RUN | runs | runs | runs | runs |
| `b_1` | MUST RUN | runs | runs | runs | runs |
| `c2_1` | MUST BE REFUSED | runs | 134 S | 134 S | 134 S |
| `c2_2` | MUST BE REFUSED | runs | 134 S | 134 S | runs |
| `c_0` | MUST RUN | runs | runs | runs | runs |
| `c_1` | MUST RUN | runs | runs | runs | runs |
| `cj` | MUST RUN | runs | runs | runs | runs |
| `ctx-m1-o0-s0` | MUST BE REFUSED | runs | 134 S | 134 S | runs |
| `ctx-m1-o0-s1` | MUST BE REFUSED | runs | 134 S | 134 S | runs |
| `ctx-m1-o1-s0` | MUST BE REFUSED | runs | 134 S | 134 S | runs |
| `ctx-m1-o1-s1` | MUST BE REFUSED | 134 S | 134 S | 134 S | 134 S |
| `ctx-m2-o0-s0` | MUST RUN | runs | runs | runs | runs |
| `ctx-m2-o0-s1` | MUST RUN | runs | runs | runs | runs |
| `ctx-m2-o1-s0` | MUST RUN | runs | runs | runs | runs |
| `ctx-m2-o1-s1` | MUST RUN | runs | runs | runs | runs |
| `ctx-m3-o0-s0` | MUST BE REFUSED | 134 U | 134 S | 134 S | 134 S |
| `ctx-m3-o0-s1` | MUST BE REFUSED | 134 U | 134 S | 134 S | 134 K |
| `ctx-m3-o1-s0` | MUST BE REFUSED | 134 U | 134 S | 134 S | 134 S |
| `ctx-m3-o1-s1` | MUST BE REFUSED | 134 U | 134 S | 134 S | 134 S |
| `cx_heap` | MUST RUN | runs / 1 ASan (1), 1 ASan (1), 1 ASan (1) | runs | runs | runs |
| `d_0` | MUST RUN | runs | runs | runs | runs |
| `d_1` | MUST RUN | runs | runs | runs | runs |
| `e` | MUST RUN (forced: A's known cost) | runs | runs | 134 U | runs |
| `f` | MUST RUN | runs | runs / 1 LSan (1), 1 LSan (1), 1 LSan (1) | runs / 1 LSan (1), 1 LSan (1), 1 LSan (1) | runs / 1 |
| `fk` | MUST RUN (forced: A's known cost) | runs; UAR runs | runs; UAR runs | runs / 134 U; UAR 134 U | runs; UAR runs |
| `fp` | MUST RUN | runs | runs / 1 LSan (1), 1 LSan (1), 1 LSan (1) | runs / 1 LSan (1), 1 LSan (1), 1 LSan (1) | runs / 1 |
| `fpp` | MUST RUN | runs | runs / 1 LSan (1), 1 LSan (1), 1 LSan (1) | runs / 1 LSan (1), 1 LSan (1), 1 LSan (1) | runs / 1 |
| `g_1_0` | author's question | runs | runs | runs | runs |
| `g_1_1` | author's question | runs | 134 S | 134 S | runs |
| `g_2_0` | author's question | runs | runs | runs | runs |
| `g_2_1` | author's question | runs | 134 S | 134 S | runs |
| `g_3_0` | author's question | runs | runs | runs | 134 S |
| `g_3_1` | author's question | runs | 134 S | 134 S | runs |
| `gcb` | MUST RUN | runs | runs | runs | runs |
| `gcopy` | author's question (forced: A's known cost) | 134 U | runs | 134 U | runs |
| `grefill` | MUST BE REFUSED | 134 U | 134 S | 134 U | 134 S |
| `jo_co` | MUST RUN | runs / 1 ASan (1), 1 ASan (1), 1 ASan (1) | runs | runs | runs |
| `jo_heap` | MUST RUN (forced: A's known cost) | runs | runs | 134 U | runs |
| `jo_lib` | MUST RUN (forced: A's known cost) | runs | runs | 134 U | runs |
| `jo_tls` | MUST RUN (forced: A's known cost) | runs | runs | 134 U | runs |
| `n` | MUST BE REFUSED | runs | 134 S | 134 S | runs |
| `ns_0` | MUST RUN | runs | runs | runs | runs |
| `ns_1` | MUST RUN | runs | runs | runs | runs |
| `x_dlclose` | MUST RUN | 139 / 1 ASan (1), 1 ASan (1), 1 ASan (1) | runs | runs | runs |
| `x_dlopen` | MUST RUN (forced: A's known cost) | runs | runs | 134 U | runs |
| `x_fake` | MUST RUN (forced: A's known cost) | runs | runs | runs / 134 U | runs |
| `x_heap` | MUST RUN (forced: A's known cost) | runs | runs | 134 U | runs |
| `x_heap0` | MUST RUN | runs | runs | 134 U | runs |
| `x_partial` | MUST BE REFUSED | 134 U | 134 S | 134 U | 134 S |
| `x_prot` | MUST RUN (forced: A's known cost) | 139 / 1 ASan (1), 1 ASan (1), 1 ASan (1) | runs | 134 U | runs |
| `x_realloc` | MUST RUN (forced: A's known cost) | 134 U / 1 ASan (1), 1 ASan (1), 1 ASan (1) | runs | 134 U | runs |
| `x_refill_out` | MUST RUN (forced: A's known cost) | 134 U | runs | 134 U | runs |
| `x_static` | MUST RUN | runs | runs | runs | runs |
| `x_tls` | MUST RUN (forced: A's known cost) | runs | runs | 134 U | runs |
| `x_unseen` | MUST BE REFUSED | 134 U | 134 S | 134 U | 134 S |
| `x_unseen0` | MUST BE REFUSED | runs | 134 S | 134 U | 134 S |

**What the table says, column by column:**

- **C4 meets every MUST row of the brief's table on Linux arm64**, plain and `--sanitize`, 3 of 3 (counted
  from the table by `grep` and `awk`): the **27** MUST BE REFUSED programs refused, all in S words (shape A,
  A′, A by an unseen switch, N, `ctx2`, the third stack's m1 and m3, `x_unseen`, `x_unseen0`, `x_partial`,
  `grefill`), and the **34** MUST RUN programs run. Today runs **15 of those 27 inside the signal** (`a_1_0`,
  `a_1_1`, `a_2_0`, `ap_1_0`, `ap_1_1`, `ap_2_0`, `au_1_0`, `au_1_1`, `c2_1`, `c2_2`, three `ctx-m1`, `n`,
  `x_unseen0`; `a_2_1` and `ap_2_1` fall to the walk, K).
- **C4 forced to fail** meets the MUST BE REFUSED column unchanged (`x_unseen0` refused, U words: the
  handler-begin read, § 1 rule 3) and the held MUST RUN set (`d_*`, `gcb`, `ns_*`, `b_*`, `c_*`, `f` and the
  2679 shapes run). Its refusals of MUST RUN programs are the brief's list of A's costs (`e`, `x_heap`, `x_tls`,
  `x_dlopen`, `x_prot`, `x_realloc`, `x_refill_out`, `x_fake` sanitized, `jo_heap`, `jo_tls`, `jo_lib`, `fk`
  sanitized and with `detect_stack_use_after_return`, `gcopy`) **and one more, `x_heap0`**, a heap buffer with no
  save between, which round one's A ran: the handler-begin read drops a record it cannot prove lives where
  round one followed it unread, which is the reading that ran `x_unseen0`'s heap twin inside the signal. Kept,
  and said: a refusal against a run inside the signal.
- **C, 2679 alone**: § 3.4.
- **G**: C4 runs the three forms with the handler on the thread's own stack (`g_1_0`, `g_2_0`, `g_3_0`: the
  overwrite proof, the runner's callback 64 KiB below the frame the swap suspended the handler at) and refuses
  the three with the handler on the alternate stack, for good. Today and 2679 alone run all six but `g_3_0`
  under 2679 alone.

### 3.6 C4's own residue, measured: A′ with a `getcontext` the runtime never sees

Rule 6's mark rests on SEEING the `getcontext` inside the handler. `apu_{1,2}_{0,1}` (`CE/p255/r2/shapes/xn2.h`)
is A′ with the handler's context saved by glibc's own `getcontext` reached by `dlvsym(RTLD_NEXT, "getcontext",
"GLIBC_2.17")`, as a library bound to the C library directly would (`x_unseen`'s technique). Built and run
inside the live C4 and C4-forced containers (`docker exec ce255r2-<column> ... run-r2.sh /w/heroes apu_*`, the
column's own stage 2; `CE/p255/r2/out/<column>/apu.log`), 3 runs each:

| | C4 | C4 forced |
|---|---|---|
| `apu_1_0`, `apu_2_0` (own-stack array, handler on the own stack) | **runs inside the signal**, `answer ran for 21` / `22`, plain and `--sanitize` | the same |
| `apu_1_1` (heap stack, handler on the alternate stack; the coroutine calls) | **runs inside the signal**, plain and `--sanitize` | the same |
| `apu_2_1` (the handler calls) | refused, K (the alternate-stack check) | the same |

**So C4 on Linux runs a Heroes function inside a signal handler where a handler saves its context by a
`getcontext` this runtime never sees and leaves by a `setcontext` it does see.** Today runs it the same way,
measured: today's stopped container committed to a temporary image (`docker commit ce255r2-today`, its stage 2
and runtime in `/w`), the four programs run there (`CE/p255/r2/out/today/apu.log`): `apu_1_0`, `apu_2_0`,
`apu_1_1` run inside the signal, plain and `--sanitize`, 3 of 3, `apu_2_1` refused K. **Not new against today,
measured; not closed either.** Its class is
2522's (a save the runtime never sees, `x_unseen0`'s), and it is the reason I do not call C4 complete.
What would close it, both unbuilt:
- **`dlvsym` answered as `dlsym` is** (the author's ruling C of 2026-11-20 extended to its sibling,
  `parts/siglookup.c`'s table): every save and jump reached through `dlvsym` becomes the runtime's, which closes
  this spelling and `x_unseen`'s alike; what remains unseen is hand-written assembly and glibc's internal calls.
  Recommended.
- **every `setcontext` out of a handler into a stack other than the thread's own HOLDS it**, resumable or not
  (only a `setcontext` to the thread's own stack, `b_*`, leaves it): closes the `setcontext` spelling at the
  price of a handler that escapes by `setcontext` into a coroutine's saved context, refused for good; and it
  leaves the `siglongjmp` spelling (the coroutine entered by `siglongjmp`, returning through `uc_link` into the
  unseen context) open, since holding that one refuses `c_1`, a MUST RUN row. Not recommended alone.

### 3.7 The `ij` cases, the case sets of `00-shared.md` § 4, the `runtime` suite (Linux arm64, every column)

`ij`: `ijn`, `ijv` 1 / 1 fortified / 2 / 3 and `ijo`, at `-O0` and `-O2`, 5 runs each: **refused 5 of 5 in
every column**, S words. `ijstale` (MUST BE REFUSED): today U, **C4 S**, C4 forced U, 2679 alone S.

Each set by `./heroes run tests/harness/main.hero -- ./heroes run <set>`, the column's stage 2, four at a time;
the count lines read whole from `CE/p255/r2/out/<column>/h-<set>.log`:

| set | today | **C4** | C4 forced | C, 2679 alone |
|---|---|---|---|---|
| `fixedbugs-2522` | 25 / 0 | **23 / 2** | 25 / 0 | 23 / 2 |
| `fixedbugs-2558`, `-2571`, recovers, `-2494`, `-2257`, `-2583`, `-2496`, `-2503`, ignored | 5, 4, 1, 15, 3, 6, 1, 1, 1 passed, 0 failed | the same | the same | the same |
| `the-platform-layer-stop` | 3 / 0 | 3 / 0 | 3 / 0 | 2 / 1, then **3 / 0 twice** alone |
| `runtime` suite | 10 / 0, census 348 | **10 / 0, census 354** | 10 / 0, 354 | 10 / 0, 354 |

- **The two 2522 failures under C4 and 2679 alone are exactly the two goldens the brief names**:
  `a-buffer-a-handler-refills-by-memcpy-and-jumps-to-is-refused` still refused, in S words where it pins U;
  `a-correct-jump-out-of-a-handler-through-a-copied-buffer-is-refused-in-words-that-say-so` runs, `called 8`
  (ruling B's known cost lifted: the author's question). Neither `.expected` was touched.
- **No case of the tree relied on the frame prunes C4 removes**: every other set reads as today, the inline
  `__builtin_longjmp` case among them (it pins the STANDING words C4 gives).
- 2679 alone's `the-platform-layer-stop` read `2 passed, 1 failed` once: `the-platform-layer-stops-every-waiting-
  call-when-the-program-asks` under `--sanitize` printed `-7`, its own bound on the median time from the stop's
  ask to the twelve calls' return (`took[6] >= 10000` µs, `the-platform-layer-ask-stop.h`), at load 20 to 34.
  Re-run alone twice from that leg's stage 2 (`docker commit` of the stopped container), load 7 to 11: `3
  passed, 0 failed` both times (`CE/p255/r2/out/C2679/pls-rerun.log`). A timing bound under load, not the route.
- The census reads 354 objects under every new column against today's 348; every new static is `_Atomic`
  (the census refuses plain ones), and the record's new fields live in the thread's `HeroSignalStack`.

## 4. What the rules answer on Linux, and what was looked for to refute them

**Rule 4, the order: known before own, justified by the shapes.** Own first lets shape A through by address
(FP's C3 on Darwin, the critic's finding 1); known first refused `f` in the critic's `kC4` because a stale range
was trusted for ever. C4 takes known first AND says when a range carved from the thread's own stack is stale (the
thread seen on its own stack above it), and judges an entry pushed on a range found stale by where its frame
lies now. Measured: shape A refused in all four orientations, and `f`, F′ (`fp`, the range a learn-at-swap table
holds) and F″ (`fpp`, the range mapped again as the alternate stack) run, plain and `--sanitize` (§ 3.5).

**Rule 5, suspension, and its premise looked for.** The premise *a suspended handler is resumed in the frame that
suspended it* is FALSE, and two shapes refute it: A′ resumes the handler at its own `getcontext` through
`uc_link`, and N resumes H1 while H2 stays suspended on another stack. C4 therefore does not rest on it: the
suspension is per entry, lifted only where the runtime SEES the handler run again (its swap comes back in the
swap's own frame, or a seen jump or swap lands inside its frames), and a handler that may be resumed unseen
(a context saved inside it) stays suspended until it ends. Measured: `ap_*` refused (four of four), `n` refused
(today runs it, `answer ran for 15`). `kC5`'s `own->depth = at` is gone: a handler's end removes its own entry
and those nested in its frames, and keeps an entry suspended on another stack (`n`). Every prune obeys the
suspension; the one exception is the overwrite proof, which is a proof that the handler's frames were written
over, not a prune by address (G below).

**Rule 5's frame prunes, removed.** The wrapper's push and the callback's check dropped entries by their own
frame's address. A by an unseen switch (`au_*`) is a coroutine on an own-stack array reached by hand-written
assembly; by address it is a thread that left the handler by a jump nobody saw, and today runs `au_1_0` and
`au_1_1` inside the signal through exactly that prune. C4 drops entries only at transfers it SEES (a decoded
jump, a swap, a handler's end): `au_*` refused, four of four, and **no case of the tree moved** (§ 3.7: the
inline `__builtin_longjmp` case already pins the STANDING words for a jump nobody sees). Its cost, by reading:
a handler left by a jump the runtime cannot see (a C++ exception, `_Unwind_*`, an inline `__builtin_longjmp`)
now refuses every later callback on the thread, where today ran a callback made ABOVE the dead handler's frame.

**Rule 6, `setcontext`: the rule that runs `b_*` and refuses A′ exists, for a SEEN save.** A `getcontext` made
while a handler stands, at a frame that may lie inside it, marks it resumable; a jump that would leave a
resumable handler for another stack holds it instead. `b_*` save nothing inside their handler and run; `ap_*`
are refused. It errs toward the told refusal for a handler that saves a context and then escapes across stacks
for good (unbuilt). **It does not reach a save the runtime never sees** (§ 3.6, measured: `apu_*` three of four
inside the signal, as today).

**Rule 7.** Linux learns context stacks in an assembly `makecontext` wrapper (arm64 and x86-64), because glibc
reads `uc_stack` there and nowhere else. Its proof that it works: `c_0` and `cj` run under C4 (today's
two-stack rule runs them by an address comparison that also runs shape A; round one's C3, which knew no context
stack, refused them, the critic's finding 5). U is not decisive and was not built (§ 1, rule 7).

**G, the author's question, and what C4 gives.** A handler swapped away from and never resumed. Where its frames
are on the thread's own stack, the runner's chain grows down through them (the callback 64 KiB below the frame
the swap suspended the handler at, nothing live below a suspended handler's lowest frame, no context saved
inside it): they are overwritten and it can never be resumed, so C4 drops it and runs `g_1_0`, `g_2_0`, `g_3_0`.
Where they are on the alternate stack nothing overwrites them, and the context the swap saved could still
resume the handler correctly: C4 cannot tell that program from shape A's, and refuses `g_1_1`, `g_2_1`, `g_3_1`
for good, told. Today runs all six. **So the verdict of one program depends on which stack its handler ran on**;
the author rules whether both should run (no rule I can build tells the alternate-stack G from shape A at the
callback), or both be refused (drop the overwrite proof: `g_*_0` refused too, and nothing else of the union
moves, by reading of where the proof fires: only in a suspended handler's stack, below its suspended frame).

**What would refute the design, looked for:**
- *a MUST row failed in either column*: none on Linux arm64 under the repaired C4 (27 refused, 34 run) or its
  forced column (the MUST BE REFUSED column whole, the held MUST RUN set whole); x86-64 below;
- *a premise of rule 5*: refuted as worded (above), and not used;
- *a correct program today runs that C4 refuses*: `g_1_1`, `g_2_1`, `g_3_1` (the author's question);
  `gcopy` moves the other way (runs; ruling B's cost lifted); under the forced column `x_heap0`, beyond the
  brief's list; and, by reading, a callback made above a handler left by an unseen exit;
- **a MUST BE REFUSED row run inside the signal**: none of the union; `apu_*` (§ 3.6) is C4's measured residue,
  run by today the same way.
- *fragility*: my first build ran four MUST BE REFUSED shapes inside the signal with no sanitizer line (§ 3.2);
  the union caught it in its first rows. Every rule here is an exact comparison of addresses on one stack.

**What Darwin must carry for rule 8 to hold** (by reading `round2/kC5/rebind.c`, the only Darwin code I may
read; FP's C4 I have not read, so each line is a question for the critic's diff, not a claim about it): a
`getcontext` the rebind sees (`kC5`'s table rewrites `_setcontext`, `_swapcontext`, `_makecontext`, the jumps
and the installers, and no `_getcontext`, so A′'s mark has nothing to set it there); no prune by the
trampoline's or the thunk's own frame (`round2/kC5/rebind.c` lines 226 and 254 by `grep -n 'hero_rebind_prune(own'`, which run `au_*_0` by reading);
per-entry suspension and a handler's end that keeps an entry suspended on another stack (`kC5`'s watermark and
`own->depth = at`, which the critic says fail N); known before own with the staleness rules; a stack pointer
judged by the byte below it; and the overwrite proof, or its absence on both platforms. Each is one of the
Linux rows above, which the Darwin table must read the same way.

## 5. The CI's other glibc and the other architecture

### 5.1 Linux x86-64, emulated (`heroes-linux:latest`, Debian GLIBC 2.41), the repaired C4

Container `ce255r2-x86-C4`, stage 2 built in it (1,766 s under emulation, `sources 65cec87e689b9022`, the arm64
C4's key, so the same tree), `CE/p255/r2/out/x86-C4/`:

| x86-64, C4 | plain / `--sanitize`, 3 runs (the `ij` row 5) |
|---|---|
| the accessor | `decoding: 3` / `decoding: 3` |
| the eight 2679 shapes | **16 of 16 exit 0**, no sanitizer line |
| `x_unseen`, `x_partial`, `a_1_0`, `a_2_0`, `a_1_1`, `a_2_1`, `ap_1_0`, `ap_2_0`, `ap_1_1`, `ap_2_1`, `c2_1` (MUST BE REFUSED) | **refused, S**, every one, both modes |
| `b_1`, `c_0`, `f` (MUST RUN) | run (`f` `--sanitize`: the same LeakSanitizer report of its own `calloc`) |
| `ijstale`, `-O0` and `-O2` | refused, S, 5 of 5 |

Every decisive row of the brief reads on x86-64 as on arm64. **Unrun on x86-64**: the forced column and 2679
alone, the rest of the union, the case sets (round one measured one case of three minutes under emulation),
libpng (no `gtk` x86-64 image here).

### 5.2 Ubuntu 26.04, glibc 2.43, arm64: the CI leg's C library

C4's stage 2 built in the Debian image (`ce255r2-ubbuild`, `CE/p255/r2/ubuntu-bins.sh`), 46 programs plain and
`--sanitize` (the accessor and the decisive union rows), then run in `ubuntu:26.04` (`ldd`: `Ubuntu GLIBC
2.43-2ubuntu2.4`, `aarch64`; `CE/p255/r2/ubuntu-run.sh`, `CE/p255/r2/out/ub-C4/ubuntu.log`), 3 runs each:
**every row reads as on Debian's 2.41**: the accessor `decoding: 3` both modes (the calibration, second stack
and `swapcontext`'s save half included, verifies against 2.43); shape A, A′ and A by an unseen switch, four
orientations each, `n`, `c2_*`, `ctx-m1`, `ctx-m3`, `x_unseen`, `x_unseen0`, `x_partial`, `grefill` refused S;
`b_*`, `c_*`, `cj`, `ctx-m2`, `d_*`, `e`, `x_heap`, `x_static`, `f`, `fp`, `fpp`, `ns_*`, `gcb` run; `gcopy` runs;
`g_*_0` run and `g_*_1` refused. **Unrun**: Ubuntu 26.04 x86-64 (round one ran its key's layout there by the
probe, not this calibration).

## 6. Cost, what it removes, the known costs

**Where the lines land** (`wc -l`, `diff | grep -c`; § 1): runtime only, **compiler 0 lines, spec 0 tokens
written** (the spec sentence § 6 of `00-shared.md` asks about is owed and unpriced, below). `sigjump.c` 283 ->
764 (+557 / -76): the probe stubs 112 lines, the calibration with its second stack and `swapcontext` probe 150,
the `swapcontext` and `makecontext` wrappers 81, A's record kept. `siginstall.c` 362 -> 699 (+381 / -44): C4's
rules are 425 lines (113 of them comment-only). `sigguard.c` +31 (the record's fields: 8 entries now carry
bounds, a lowest frame, an id, a sequence, a kind, a slot, two flags; and 16 known stacks per claimed thread),
`siglookup.c` +1. Both large files pass `.claude/rules/module-shape.md`'s ~300-line threshold: a landing splits
`sigjump.c` (the names, the calibration, A's record) and `siginstall.c` (the installers, the context rules).
*C, 2679 alone* is `sigjump.c` alone, 283 -> 671 (+460 / -72).

**What C4 removes**: every read of a buffer a save filled at any moment but its own jump (the fingerprint path,
2679), except under A; the frame prunes of the wrapper and the callback; C3's two-class prune (an address on a
context stack compared with one on the alternate stack); `kC5`'s suspension watermark and `own->depth = at`.

**Its known costs against today, every one measured unless said**: `g_1_1`, `g_2_1`, `g_3_1` refused (the
author's question); `gcopy` runs (ruling B's refusal lifted: the author's question) and `grefill` keeps its
refusal in other words (the two goldens moved, neither rewritten by me); under the forced column `x_heap0`
refused beyond the brief's list (§ 3.5); by reading, unrun: a callback made above a handler left by a jump the
runtime never sees (a C++ exception, `_Unwind_*`, an inline `__builtin_longjmp`), and a handler that saves a
context and then escapes across stacks for good. **What it does not close**: `apu_*` (§ 3.6, as today); the
rest of 2522's unseen-save class; a coroutine made on one thread and run on another (its stack unknown there).

## 7. Verdicts (Linux; Darwin is the ffi-pragmatist's, whose folder I did not read)

### C4 on Linux: **approve**

- `verdict`: approve, Linux half; no veto (it adds no core construct, moves no program toward running inside a
  signal, and the compiler is untouched).
- `section`: design.md §1.12 (`docs/design.md:648`, *"A Heroes program must not segfault and must not corrupt
  memory"*, and the boundary *complete*), for the soundness; §1.1 (`:180`) and §1.7 (`:432`) for the ceiling,
  which the runtime's growth does not reach (no language construct, no compiler line).
- `implementation_cost`: § 6, runtime only: `sigjump.c` +557 / -76, `siginstall.c` +381 / -44, `sigguard.c`
  +31, `siglookup.c` +1; compiler 0 lines.
- `needed_for_self_hosting`: **no**.
- `argument`: On Linux arm64 C4 meets every MUST row of the brief (27 refused, 34 run), plain and sanitized,
  where today runs 15 of the 27 inside the signal; 2679's shapes and libpng are clean; no case of the tree
  moves but the two named goldens; Ubuntu's glibc 2.43 and x86-64 read the same on the rows run. Its forced
  column keeps every refusal. Against it, measured: my own first build ran four refused shapes inside the
  signal with no sanitizer line, every rule being an exact address comparison; `apu_*` stays as today; G splits
  by stack. So it lands only with the union as goldens.
- `prediction`: below.
- `condition`: the Darwin table reading any union row otherwise (rule 8), or a context case of the landing
  running inside the signal on a CI leg, turns this to object; the author ruling G one way decides the
  overwrite proof's fate on both platforms.

### "C, 2679 alone" on Linux: **approve, as the landing that may come first**

- `argument`: It closes 2679's eight shapes and libpng (arm64; its x86-64 rows unrun this round), and on the
  whole union and `apu_*` it moves **no** row toward running inside a signal against today; it refuses two rows
  today runs inside (`c2_1`, `x_unseen0`) and one more correct-or-not program (`g_3_0`, the author's question);
  its sets read as C4's (2522 23 / 2, the same two goldens) once its timing flake is re-run. It leaves the 13
  context rows today runs inside running inside, so the context defect stays `blocking` and C4 (or its
  successor) owes its landing. **So on Linux 2679 can land before the context defect**, measured.
- `implementation_cost`: `sigjump.c` only, 283 -> 671 (+460 / -72); compiler 0 lines.

## 8. The goldens a landing owes, and the prediction

`run` goldens, on BOTH platforms (the harness keeps one `.expected` per case, so a row whose verdict differs by
platform cannot be pinned until rule 8 holds), each written leak-free (`f`, `fp`, `fpp` and my `xn.h` leak
today, § 3.2, § 3.4):
- **2679's own** (`fixedbugs-2679-...`): the eight shapes, exit 0 with their lines; libpng where `pkg-config`
  answers; **the accessor**, `decoding: 3` (Linux arm64 and x86-64, Darwin arm64; Darwin x86-64 has no leg,
  Windows has no signals and skips);
- **the context defect's** (the coordinator's filing): refused in STANDING words, shape A ×4, A′ ×4, A by an
  unseen switch ×4, N, `ctx2` ×2, the third stack's m1 ×4 and m3 ×4, `x_unseen`, `x_unseen0`, `x_partial`;
  running, `b` ×2, `c` ×2, `cj`, `d` ×2, `e`, `f`, F′, F″, `ns` ×2, m2 ×4, the `x_*` that must run, `jo_*`,
  `fk`, `cx_heap`; G ×6 once the author rules; `apu_*` once its closure lands (it cannot be pinned running);
- **the forced column** has no instrument: the harness cannot build the runtime with
  `HERO_JMP_FORCE_UNCALIBRATED`, so a CI job or a harness word is owed (a tool surface: the panel's), unbuilt.

**Prediction.** At the lane that lands C4 (or "C, 2679 alone" first), on the CI's `ubuntu-26.04-arm` and
`ubuntu-26.04` legs, the accessor golden prints `decoding: 3` and every new `run` golden of 2679's set and of
the context set passes in all four builds of each case, while `fixedbugs-2522` reads 25 passed, 0 failed with
exactly two `.expected` moved (the refill case's words, the copied-buffer case running). **Falsified** on either
leg by the accessor printing `2` or `0` (the key or the field not verified against the runner's glibc 2.43 on
its CPU, which arm64 here did not show and x86-64 has not been asked), or by any context case pinned as refused
exiting 0. Checkable on those two legs' `run` lines after the push that carries the landing.

## 9. Everything unrun, said

- Darwin, every column (the ffi-pragmatist's); whether its table reads my rows the same (the critic's diff).
- x86-64: the forced column, 2679 alone, the union beyond the brief's decisive rows, the case sets, libpng;
  Ubuntu 26.04 x86-64 under this calibration; a CPU with a shadow stack or GCS enabled.
- glibc's four other architectures (no machine): there C4 learns no context stack (no `makecontext` wrapper)
  and the key is A by construction; their verdicts are by reading.
- U (not decisive for a rule that reads `uc_stack` only at `makecontext`); G's forms with a shallow callback;
  a handler that saves a context and then escapes across stacks; a C++ exception or `_Unwind_*` out of a
  handler with a later callback above it; a coroutine made on one thread and run on another; the two closures
  of `apu_*` (§ 3.6), both unbuilt.
- The `runtime` suite's census asked of the new fields by name: it reads 354 and passes; I did not list each.
- Every timing: the durations above are wall-clock of legs run four to six at once at load 3 to 34, beside
  other seats' work; none is a resolution's ground.

Containers: every one I started is removed (`docker ps -a --filter name=ce255r2` printed none after the last
run); two temporary images (`docker commit` of the stopped today and 2679-alone legs) removed by `docker rmi`.
