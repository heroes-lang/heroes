# Panel 255, round two: the ffi-pragmatist builds C4 on Darwin

Written 2026-11-22 as I went, by the ffi-pragmatist seat. Every run below is in my copy,
`.claude/worktrees/p255-scratch/ffi-pragmatist/` (written `F/` below), a detached worktree at
`a75179641`, `TMPDIR=F/p255/tmp`, this Mac (macOS 27.2.1, Apple clang 21.0.0), unless a line says
otherwise. Round two's work is under `F/p255/r2/`. Read before building: `255-briefs/round2.md` whole,
`00-shared.md` § 6 to § 8, `critic-reports.md` findings 1 to 13, `critic-round2-brief.md` whole, my
round-one report; the inputs under `255-briefs/round2/` (kC5, fp-A, fp-C3, the shapes). I read nothing
of the compiler-engineer's folder or its round-two report. Another seat builds on this Mac and in
Docker at the same time: every time here is indicative. A claim without its command says *unrun*.

## Status

Started 21:41 (`uptime`, load 3.70). Sections are appended as each part is built and run.

## What I built: C4 on Darwin, `F/p255/r2/C4/rebind.c` and `sigguard.c`

Started from `round2/kC5/` (rule 5's mechanism) and `round2/fp-A/` (the A path, which kC5 lacks), as the
brief says. Assembled from parts kept beside it (`F/p255/r2/C4/part-*.c`, `cat` in order into
`rebind.c`): kC5's installs, `dlsym` and image rewrite unchanged; the rest new. Size (`wc -l`, `git diff
--no-index --numstat`): `rebind.c` today 727 lines, C4 1,369, **+853 −211** against today (+887 −265
against kC5); `sigguard.c` today 523, C4 556, **+34 −1** (an entry record and four fields). Per part:
entries and trampoline 328, A's records 217, decode and calibration 305, jumps and save points 207,
table 56.

**How each rule is carried, and the choices the brief leaves to the seat:**

1. **The decode** (kC5's, unchanged in substance): a `setjmp`-family jump reads word 12 and decodes
   `xpacd(word ^ k)`; a context jump reads byte 0x148, `xpacd(word)`, the bytes `__setcontext` reads
   next (`_setcontext` passes `uc + 0x40` to `__setcontext`, which loads its SP at `+0x108`: Q1's
   disassembly, `F/p255/q1/platform-arm64e.dis` lines 7922 to 7944 and 5061 to 5079). **A robustness
   repair against kC5 and my C3**: `setcontext` refuses a context whose size word (byte 0x28) is 0 and
   returns -1 (the same listing, `ldr x8, [x0, #0x28]; cbz`), and `swapcontext` refuses a NULL `from`
   or `to`; kC5 and C3 read `to + 0x148` before the call, so a `setcontext(NULL)` inside a handler
   faulted in the runtime where the C library answers `EINVAL` (by reading; not run as a program), and
   a refused jump was judged as taken. C4 judges nothing for a jump the C library will refuse.
2. **The calibration takes both seats' strengths** (critic-reports finding 10): a naked stub saves the
   caller's callee-saved registers, fills x19..x28, x29 and d8..d15 with 19 patterns, stores the stack
   pointer and its own return address, and calls the save; every word the save writes is then checked:
   for `setjmp`, `_setjmp`, `sigsetjmp(,0)`, `sigsetjmp(,1)` words 0 to 9 (the patterns), word 10
   `xpacd(w ^ k)` = the x29 pattern, word 11 `xpaci(w ^ k)` = the return address, word 12 = the stack
   pointer, d8..d15 at 0x70; for `getcontext` the same fields of the machine context and its size word;
   for **the save half of `swapcontext`** (the brief's rule 2 did not name it, critic-round2-brief
   finding 6) x22..x28 and d8..d15 (libsystem's `swapcontext` keeps x19..x21 for itself, its listing at
   7759) and a stack pointer within the 4 KiB below the caller's, checked on a second stack made by
   `makecontext` and resumed. Each at a frame, 16 frames of 64 KiB deeper, and on a 256 KiB `mmap`ed
   stack. **The two families are verified apart and fall apart**: a jump-buffer layout that fails sends
   only the `setjmp` family to A. Measured (`F/p255/r2/w-try/route.hero`, an `extern` header declaring
   the accessor, built against each runtime, plain and `--sanitize`): C4 `route 3 / route 3`;
   forced 1 `route 2 / route 2`; forced 3 `route 0 / route 0`. **The negative controls**, each a copy of
   C4 with one offset wrong: word 11 for the stack pointer → `route 2`; context SP at 0x140 → `route 1`;
   the frame-pointer word 9 → `route 2`; d8 at 0x78 → `route 2`. Each wrong layout refuses its own family
   and only it. ASan on this Mac interposes `longjmp`, `_longjmp`, `siglongjmp`, `sigaltstack`,
   `signal`, `sigaction` and no save and no context function (`dyld_info -fixups` of
   `libclang_rt.asan_osx_dynamic.dylib`, its `__interpose` binds), so the calibration reaches the same
   saves plain and sanitized, and read the same route.
3. **A where a family does not verify**: fp-A's record (fingerprint read only of a buffer on this
   thread's stack, its alternate stack while on it, or a writable segment of an image loaded before
   `main`), its save points rewritten **only for the family that failed** (the table carries a family
   per save point and the calibration runs before the first image is rewritten). **The accessor**:
   `int hero_signal_jump_route(void)`, bit 1 the `setjmp` family decodes, bit 2 contexts; a `run`
   golden asks it through an `extern` header (built and run above). **The define**:
   `HERO_REBIND_FORCE_UNCALIBRATED` 1, 2 or 3. **Rules 4 to 7 bind the A path**: the same entries, the
   same suspension and hold; a trusted record drops the entries pushed after its save that nothing pins
   (the serial of the innermost entry at the save is what the record keeps), never by address.
4. **Stacks: the order, chosen and justified by the shapes.** A TARGET is classified known range
   first, then own, then alternate, else unknown; an ENTRY is never classified: it records the
   alternate stack it was pushed on, and is left by a target only where it lies inside the target's
   range, at or below it. So no prune compares two addresses on two different ranges, and a stale
   range is only ever asked about a target. The order is forced by four shapes: `f` and F′ (a stale
   range under the handler's frame, the target outside it) need the entry judged on the thread's own
   stack; F″ (the alternate stack mapped over a stale range) needs the alternate stack the entry was
   pushed on; shape A's `siglongjmp` spelling (A″ below: a target inside a live coroutine array carved
   from the thread's own stack) needs the target judged on the coroutine. The price is F‴ (below): a
   target inside a STALE range carved from the thread's own stack, the handler below it, refused.
5. **A swap suspends; per entry, never by index.** Each entry carries a count of the swaps made while
   it stood that have not returned in their own frames; the swap wrapper (not a tail call) marks every
   standing entry, keeps their serials in its own frame, and releases only those when it returns (or
   fails). Nothing drops a counted entry but its own handler's end. **The trampoline neither prunes at
   its push nor truncates at its end**: it drops its own entry, by serial, and every entry pushed after
   it that nothing pins (kC5's `own->depth = at` is what fails N). **The thunk does not prune by its own
   frame** (kC5's line 254 is what lets A by an unseen switch through: a stack switched in assembly
   inside the thread's own bounds passes for the same stack). Entries are removed by one store
   (`dead`), never shifted, so a nested signal never sees one half moved; a full record counts a
   handler as `lost`, which refuses until that handler ends.
6. **`setcontext`**: a target inside a known range (a coroutine's stack) drops the entries on that
   range above it and HOLDS every other standing entry until its handler ends; any other target is a
   jump (b_* run). A jump that leaves the alternate stack an entry was pushed on, for any other stack,
   leaves that entry (kC5's second edit, kept). **And a jump or swap landing on the alternate stack from
   outside it** (only a delivery puts frames there) re-enters a handler left earlier: it stands again as
   an unsure entry. That is what refuses A″ with a save (below), which no rule of the brief reached.
7. **Known stacks** in `makecontext`'s slot (kC5's table, its spin flag made an `_Atomic int` so the
   census reads it). Nothing tells the runtime a range has ended (no `munmap` is seen, and a range
   carved from the thread's own stack ends when its frame returns); a stale range costs only where it
   holds a TARGET: F‴, and a `setcontext` escape whose target lies in one (held, refused, told). U is
   Linux's (rule 7 here reads `uc_stack` at `makecontext`, where libsystem reads and zeroes the same
   range: its listing at 7807, `ldr x0, [x19, #0x8]`, `ldr x1, [x19, #0x10]`, `bl ___bzero`); not
   written.

**Corrected 22:12, by a run, the sentence of item 1 above about `setcontext(NULL)`**: it is false.
libsystem's own `_setcontext` reads byte 0x28 of its argument with no NULL check and faults (today and
C4 both stop at `a null pointer was read through, at offset 0x28`, the C library's read). The refusals the
C library really makes are a context whose size word is 0 (`setcontext` answers -1) and a NULL `from` or
`to` given to `swapcontext` (`cbz x0`/`cbz x1`, `EINVAL`). The shape, `F/p255/r2/sh/sn.h` (a handler asks
for both, checks the -1, then leaves by the plainest correct jump, the program called 64 KiB deeper:
MUST RUN), run 3 times each, plain / `--sanitize`, from `F/p255/r2/w-sn-<runtime>/`:

| `sn` | today | my round-one C3 (`round2/fp-C3`) | kC5 | **C4** |
|---|---|---|---|---|
| refused `setcontext` and `swapcontext` in a handler, then a correct jump out | 0, `answer ran for 72` / the same | **134, `null pointer read through at offset 0x148` / ASan SEGV at 0x148** | **the same as C3** | 0, `answer ran for 72` / the same |

So C3 and kC5 turn a correct program into a fault inside the runtime (design.md §1.12's first clause),
reading `to + 0x148` before `swapcontext` refuses `to`; C4 asks what the C library asks first. A shape
for the union on both platforms (glibc's `swapcontext` with a NULL target: unrun here).

## The new shapes, written here (`F/p255/r2/sh/r2new.h`, `rr.h`, `sn.h`; each MUST in its comment)

From critic-round2-brief finding 7's descriptions, on Darwin:
- **A′** `ap_<how>_<o>`: the handler `getcontext`s its own context, `setcontext`s into a coroutine whose
  `uc_link` is that context; how 1 the coroutine calls the program, how 2 the coroutine returns, the C
  library resumes the handler through `uc_link` (inside libsystem's `__ctx_done`, no slot sees it) and
  the handler calls. o 0: the coroutine's stack an array in a live frame of the thread's own stack, the
  handler on the thread's own; o 1: a heap stack, the handler on the alternate stack. MUST BE REFUSED.
- **A by an unseen switch** `au_<how>`: shape A's array coroutine reached by an assembly switch
  (`au_switch`, callee-saved registers pushed, stack pointer swapped), no context function called.
  MUST BE REFUSED.
- **N** `nn`: H1 swaps to K1; K1 raises; H2 runs on K1 and swaps to K2; K2 swaps to H1, which returns;
  the runner swaps into K2, which calls the program while H2 never returned. MUST BE REFUSED.
- **F′** `fp1` (f with one swap into its context before the helper returns), **F″** `fp2` (a
  coroutine's `mmap`ed stack run once, unmapped, mapped again `MAP_FIXED` at the same address as the
  alternate stack, then d_1). MUST RUN.
- **G** `g1_0`, `g1_1` (the handler escapes by `swapcontext` to a context the runner took before the
  signal, own / alternate stack), `g2` (the handler swaps to a coroutine, which `siglongjmp`s to the
  runner's pre-signal buffer), `g3` (the coroutine returns through `uc_link` to the runner's pre-signal
  context); the program called 64 KiB deeper. The author's question.
- **The seat's own, beside them** (CL-061): **F‴** `fp3`, a stale range carved from the thread's own
  stack that holds the jump's TARGET, the handler's frame 320 KiB below it (a correct program, MUST
  RUN); **A″** `as_1` (shape A spelled with `siglongjmp`: the handler on the alternate stack saves its
  own context, jumps INTO a heap coroutine through a buffer the coroutine saved, the coroutine resumes
  the handler's context, the handler calls: MUST BE REFUSED) and `as_0` (the handler on the thread's
  own stack saves nothing and jumps into an array coroutine, which calls and returns through `uc_link`:
  correct by the reasoning that makes c_* MUST RUN); **R** `rr_0`, `rr_1` (a handler saves, by
  `sigsetjmp` or `getcontext`, jumps out to the runner on its own stack, and the runner at once jumps
  back into the handler's save; the handler calls: MUST BE REFUSED; the jump back is undefined in C for
  `rr_0`, C11 7.13.2.1, and works on this Mac); **`sn`** (above).
- **U** is Linux's and not written (item 7 above).

## Today's Darwin, measured on the union: three more programs run inside a signal

`F/p255/r2/logs/shapes-today.log` (today's runtime, `rt/today` = HEAD's `runtime/`, `md5`
`4d39ad4a...` for `rebind.c` both places; 3 runs per cell, plain and `--sanitize`): **`nn` exit 0,
`answer ran for 31`**, plain and sanitized: today's Darwin runs N's coroutine call inside H2's
signal; **`rr_0` and `rr_1` print `answer ran for 80` / `81`** (then a silent exit 134 as the handler
returns through clobbered frames): the resumed handler's call runs (today's `hero_rebind_jump` returns at
once where no entry stands, HEAD `runtime/parts/rebind.c:427`, so the jump back into the handler's save
restores nothing); and `t_unseen0` and the Darwin port of `x_unseen0` run (`answer ran for 30`,
`answer ran for 5`), as the critic measured for `t_unseen0`. kC5 runs `rr_0` and `rr_1` too (same
logs' method, `F/p255/r2/w-rr-kC5`). Each is defect 2522's class (no ASan line; a Heroes function inside
a signal), for the coordinator's context defect.

## Two repairs made to C4 while running it (every table below is the final file)

- **22:14, R closed.** C4 as first built ran `rr_0` and `rr_1` inside the resumed handler, as today and
  kC5 do (`answer ran for 80` / `81`, 3 of 3, plain and `--sanitize`, `F/p255/r2/w-rr-{today,C4,kC5}`):
  the decode path sees no save, so a handler left by a jump out on its own stack and jumped back into
  stood as left. The repair is rule 6's last sentence widened to its twin on one range: **a jump that
  lands BELOW the jumping frame on the range that frame stands on lands in frames already unwound**,
  and stands as an entry of its own, unsure (every correct jump lands above the frame that makes it, on
  its own range or another). So the target is now decoded at every jump, inside a handler or not (one
  word of a buffer the C library reads next; the round-one A's *no read outside a handler* was about the
  pending buffer, which C4's decode path never reads). And route A's re-entry by record no longer skips a
  record whose bytes could not be fingerprinted (`rr_1` ran under the forced column with that skip).
  After: `rr_0`, `rr_1` refused U under C4 and under forced 3, 3 of 3 both modes; `sn`, `d_0`, `ns_0`,
  `ns_1`, `c_0`, `c_1`, `cj`, `b_0`, `b_1`, `gcb`, `jo_co`, `ctx-m2-o1-s1`, `f`, `fp1` unchanged (run),
  `c2_1` refused S (`F/p255/r2/logs/shapes-C4.log` of 22:15, overwritten by the final run).
- **22:20, a false word repaired.** The final union's first rows read `a_2_1` refused in U words (*"holds
  no record of"*) where its handler is standing, suspended: the coroutine's swap back into it landed on
  the alternate stack from outside, and the re-entry rule pushed an unsure entry beside a handler that
  still stood. A landing at or below an entry still standing on the target's range now pushes nothing
  (that entry refuses, and nothing pushed there could outlive it: anything that drops the standing
  entry by address drops one below it on its range, and its trampoline's end drops what was pushed after
  it). After: `a_2_1` and `ctx-m3-o0-s1` refused S, `rr_0` refused U, 3 of 3 both modes. Final
  `rebind.c` 1,386 lines, `md5` `ebc1e335a6fedf81a1c03df843e9ad09`.

Both runs of the gate and the union started before these repairs were stopped and restarted on the
final file (`kill`, the tree's `runtime/parts` restored from `HEAD`, `git status --short` printing only
`?? p255/`).
- **22:23, the cost of decoding every jump, measured and paid down.** `F/p255/r2/bench/lj.hero` (two
  million `setjmp`/`longjmp` pairs outside every handler, a C library's error path), built `-O2` against
  each runtime, `/usr/bin/time -p`, three runs each alternating, this Mac at load above 40 (indicative
  only, `real` read against `user + sys`). C4 after R's repair: user 1.54 to 1.57 s, sys 0.77 to 0.80
  against today's user 1.21 to 1.24, sys 0.63 (the re-entry check asked `sigaltstack`, a system call,
  at every jump, and scanned the 256 known ranges). Two changes that move no verdict: the known ranges
  are not scanned until a `makecontext` has ever named one, and the alternate stack is asked only for a
  landing below or on a thread a handler once ran on it. After: **C4 user 0.48 s, sys 0.45; today user
  0.99 to 1.02, sys 0.45 to 0.47**: C4's error path costs half today's user time (today records each
  save and fingerprints the pending buffer at every jump; C4 reads one word). Final `rebind.c` 1,393
  lines, `md5` `9f9899485bb8804fe85fa75b8315427b`; the decisive shapes re-run on it (`a_2_1` S, `rr_0`,
  `rr_1`, `as_1` U, `ap_2_1` S refused; `c_1`, `b_1`, `ns_1`, `cj`, `fp2` run; 3 of 3, both modes), and
  the union and the gate restarted on it at 22:24.

## Column *C, 2679 alone* on Darwin: A, built and run on the union

On Darwin C replaces the entry list (today's entries are a list threaded through the trampolines'
frames, and a decode that drops an entry by address cannot keep a list it would read through), so the
most conservative build that leaves today's context handling as it is is **A**: today's `rebind.c` with
round one's four changes (`round2/fp-A/rebind.c`, `md5` `f872835c...`, the same file as round one's
`F/p255/A/rebind.c`), `rt/A2679` = HEAD's `runtime/` with that file. Every row where it differs from
today, from `F/p255/r2/logs/shapes-{today,A2679}.log` (3 runs a cell, plain / `--sanitize`;
`F/p255/r2/table.py` prints the cells):

| shape | today | A |
|---|---|---|
| 2679's eight and Darwin's context shape (`heap`, `mmap`, `handler_*`, `plain_*`, `cx_heap`) | ASan use-after-free, 139 or ASan SEGV | clean, both modes |
| `x_dlclose` (a dlclosed library's buffer, then a save) | **139 / ASan SEGV** | runs |
| `x_prot` (a page made `PROT_NONE`, then a save) | **138 / ASan BUS** | 134 U (refused) |
| `x_realloc` | 134 U / ASan use-after-free | 134 U |
| `pngdemo` (libpng 1.6.59) | ASan use-after-free on the second decode | clean |
| `b_0`, `b_1`, `e`, `x_heap0`, `x_heap`, `x_tls`, `x_dlopen`, `jo_heap`, `jo_tls`, `jo_lib`, `g1_0`, `g1_1`, `cs_0`; `fk` under the ASan option | run | **134 U: refused**, in words false for each |

**It moves no context row toward running inside a signal against today**: on every context row (`a_*`,
`ctx-m*`, `c2_*`, `ap_*`, `au_*`, `nn`, `as_*`, `g*`, `rr_*`, `cs_*`, `t_*`, `x_unseen*`) it gives
today's verdict or refuses where today runs. **And it keeps every residue today has**: `nn`, `rr_0`,
`rr_1`, `t_unseen0` and `x_unseen0` run inside the signal under A exactly as under today. So on Darwin
2679 CAN land before the context defect, as A, at the price of thirteen correct programs refused (the
twelve above and `cs_0`), two of them (`b_*`) in the MUST RUN column of C4 forced to fail. Whether it
should is answered below by C4's own column against today.
- **22:32, the forced column's two unseen refills closed.** With the `setjmp` family on route A
  (`HERO_REBIND_FORCE_UNCALIBRATED 1`), `t_unseen0` and the port of `x_unseen0` ran inside the signal
  (`answer ran for 30`, `answer ran for 5`, 3 of 3, both modes), as under today and under A: route A
  fingerprints the pending buffer at the next save or jump, so a handler that refills it by a save no
  slot sees and jumps to it has the refill fingerprinted as the save's own bytes. The repair: **the
  trampoline settles the pending buffer before it calls the handler** (A's own settle, so read only
  where the buffer provably lives). After, forced 1: `t_unseen0` and `x_unseen0` refused U, `d_0`,
  `ns_1`, `gcb` run, 3 of 3 both modes. On the decode path nothing is ever pending (no save point is
  rewritten and `swapcontext` notes only when contexts are on A), so the line returns at its first
  test there. Final `rebind.c` 1,403 lines, `md5` `6b4fcb0b7f6604962e9dd336082bdad7`; the forced
  runtimes `3e8a9a34...` (1) and `0b0ffcf2...` (3), each the final file plus its one define. **The C4
  column's union and gate below ran on `9f989948...`, which lacks only this line**; the decisive rows
  are re-run on the final file at the end (below).

## A residue C4's decode path keeps, measured: R across ranges (`rr_2`)

The shape beside R (CL-061): the jump back into the left handler's save made from a coroutine on a
heap stack (`sh/rr.h` spell 2: the handler `getcontext`s, `siglongjmp`s out to the runner on its own
stack; the runner swaps into a heap coroutine that `setcontext`s to the handler's save; the handler,
running again, calls the program). Built and run 3 times each, plain / `--sanitize`, in
`F/p255/r2/w-rr2-<runtime>/`:

| `rr_2` | today | A | **C4** (final file) | C4 forced 3 |
|---|---|---|---|---|
| R across ranges (MUST BE REFUSED) | `answer ran for 82`, then 134 / the same | the same | **`answer ran for 82`**, then 134 / **`answer ran for 82`, exit 0** | 134 U / 134 U |

So the decode path, which sees no save, follows the handler's jump out (an upward jump on its own
range: it is left) and cannot tell the later landing in its dead frames from a coroutine's legitimate
resumption of the thread's own stack, because the landing comes from another range. Today and A run it
too (today's jump returns at once where no entry stands; A's record of the save is untrusted on the
heap). C4's route A refuses it (the record of the save names an entry that no longer stands). **This is
defect 2522's class, shared with today, and C4 as built does not close it.** What would, by design and
unbuilt: a per-thread *bottom* for each range the thread is not running on, recorded where the thread
leaves it (a swap's `from`, a jump's own frame, a delivery's interrupted stack pointer read from the
signal context the kernel hands the trampoline) and cleared where it lands on it; a landing below a
range's recorded bottom is a landing in dead frames, whichever range it comes from. The jump back is
undefined in C for a `jmp_buf` (C11 7.13.2.1); for a context I found no rule either way.

## kC5 against C4 on the new shapes (what C4's departures from kC5 buy, measured)

`rt/kC5` = HEAD's `runtime/` with `round2/kC5/rebind.c` and `sigguard.c` (`md5` `759ce0b9...`,
`dc99660f...`), run on the new shapes by the same runner (`F/p255/r2/logs/shapes-kC5.log`, 3 runs a
cell, plain / `--sanitize`); C4 from `shapes-C4pre.log` (the file before the trampoline's settle,
inert on this path; the final file's union is the next table):

| shape (MUST) | today | kC5 | C4 |
|---|---|---|---|
| `ap_1_0`, `ap_2_0` A′, own-stack orientation (REFUSED) | 134 U | **runs: `answer ran for 11`, `12`**, both modes | 134 S |
| `ap_1_1`, `ap_2_1` A′, alternate-stack orientation (REFUSED) | 134 U | 134 K (the thunk's on-stack test) | 134 S |
| `au_1` A by an unseen switch, the coroutine calls (REFUSED) | 134 S | **runs: `answer ran for 21`** | 134 S |
| `au_2` the same, the handler calls (REFUSED) | 134 S | 134 S | 134 S |
| `nn` N (REFUSED) | **runs, 31** | **runs, 31** | 134 S |
| `rr_0`, `rr_1` R (REFUSED) | **runs, then 134** | **runs, then 134** | 134 U |
| `sn` refused context jumps, then out (RUN) | runs | **134, null pointer read at 0x148 / ASan SEGV** | runs |
| `fp1`, `fp2` F′, F″ (RUN) | run | run | run |
| `fp3` F‴ (correct) | runs | runs | **134 S: a cost** |
| `as_0` A″ without a save (correct) | runs | runs | **134 S: a cost** |
| `as_1` A″ with a save (REFUSED) | 134 K | 134 K | 134 U |
| `g1_0`, `g1_1`, `g2` G (the author's) | run | 134 S | 134 S |
| `g3` G through `uc_link` | 134 U | 134 S | 134 S |
| `b_1`, `c_1` | run | run | run |

So kC5's three departures that C4 does not take are each measured to run a Heroes function inside a
signal: reading `setcontext` as a jump whatever its target (A′, own-stack orientation, the brief's
reading confirmed), the thunk's prune by its own frame (A by an unseen switch), and the trampoline's
`depth = at` with a suspension kept as an index (N). And kC5 faults on `sn`.

## Rule 6, answered: what tells b_* from A′, and which way C4 errs

**The rule that runs b_* and refuses A′**, at the moment of the `setcontext`, by its target alone: a
target inside a range `makecontext` handed out lies on a coroutine (A′: the coroutine may come back to
the handler through `uc_link`, inside libsystem's `__ctx_done`, where no slot sees it), so every
standing entry off that range is HELD until its handler ends; any other target is a jump (b_*: the
runner's own frame on the thread's own stack, above the handler, or off the alternate stack). Measured
above: `b_0`, `b_1` run; `ap_1_0`, `ap_2_0`, `ap_1_1`, `ap_2_1` refused S, plain and `--sanitize`.

**What no rule on the decode path can see**: whether the handler saved a context of its own before it
left. No save is rewritten there, so `getcontext(&hc)` inside a handler is invisible, and the
difference between A′ (the coroutine will resume `hc`) and a handler that leaves for good into a
coroutine (the coroutine never returns to it) exists only in that save. **C4 errs toward the told
refusal on the targets it can classify**: a handler that leaves for good by `setcontext` into a
coroutine from another stack is held (`cs_1`, the handler on the alternate stack: refused, and refused
by today too, by the thunk's on-stack test, since libsystem's `setcontext` never calls
`__sigunaltstack` and the kernel's flag stays set: today 134 K, measured); a handler on the thread's own
stack that jumps into a coroutine carved from that stack is never judged left, its range being
another's (`as_0`: correct by c_*'s reasoning, today runs it, **C4 refuses it**). **It errs toward
running in one shape, measured**: `rr_2`, a resumption of a left handler's frames from another range
(above), shared with today and A. The correct programs that pay are `as_0` and `cs_1` (and `cs_0`
where the coroutine's own stack carries the handler: see the final table).

## G, the author's question: what C4 gives and why

Every form of G is refused under C4, for good (`g1_0`, `g1_1`, `g2`, `g3`: 134 S, both modes, 3 of 3):
a swap made while a handler stands suspends it (rule 5), and a handler swapped away from and never
resumed stays suspended; nothing but that handler's end releases it, so every later callback of the
thread is refused, in S words that are true of a suspended handler and false of an abandoned one.
**Today runs `g1_0`, `g1_1` and `g2`** (and refuses `g3`, U): three correct programs, by b_*'s
reasoning, that C4 refuses and today runs, a cost for the author as ruling B was. kC5 refuses all four
too (S). What a less conservative C4 would give, unbuilt: a swap whose target lies on no coroutine read
as b_*'s jump (G1 would run), its later resumption of the handler caught where it comes from the same
range below the jumper or into the alternate stack from outside (the re-entry rules), and missed where
it comes from a coroutine on another range (`rr_2`'s class). Robust keeps the suspension; conservative
and robust agree here, and the author decides whether G1 and G2 are worth that residue. G2 and G3 (the
handler swaps INTO a coroutine first) cannot be told from shape A at the swap, so no rule I can build
runs them without running shape A's `a_1_*`.

## The table: every shape, every column, this Mac (final files)

Columns in the brief's order: **today** (`rt/today`, HEAD's runtime); **C4** (`rt/C4`, `rebind.c`
`6b4fcb0b...`); **C4 forced to fail** (`rt/C4f3`, `HERO_REBIND_FORCE_UNCALIBRATED 3`, both families on
route A, A's path as the brief defines it, and Darwin x86-64's path by construction); **C4 forced, the
`setjmp` family only** (`rt/C4f1`, define 1: the realistic failure, a libsystem that changes its
munge, contexts still decoded); **C, 2679 alone** (`rt/A2679`, A). Each cell *plain / `--sanitize`*,
3 runs each, every distinct outcome listed (`F/p255/r2/run.sh`, `all.sh`, logs
`F/p255/r2/logs/shapes-<runtime>.log`, cells by `F/p255/r2/table.py`, which wrote this table as
`logs/table-final.md`): **R*n*** the program ran, printing `answer ran for n`; **0:…** exit 0 and the
program's last line; **134S** refused, *"had not been seen to end"*; **134U** refused, *"holds no record
of"*; **134K** refused by the thunk's walk or on-stack test; **134+ran** the program ran inside the
signal, then the process aborted; **A:uaf / A:segv / A:bus** an AddressSanitizer report; 139, 138 a
SEGV, a BUS. `fk` sanitized runs under `ASAN_OPTIONS=detect_stack_use_after_return=1`.

| shape | today | **C4** | C4 forced to fail (both) | C4 forced (setjmp family) | C, 2679 alone (A) |
|---|---|---|---|---|---|
| `heap` | 0:heap: 0 / A:uaf | 0:heap: 0 / 0:heap: 0 | 0:heap: 0 / 0:heap: 0 | 0:heap: 0 / 0:heap: 0 | 0:heap: 0 / 0:heap: 0 |
| `mmap` | 139 / A:segv | 0:mmap: 0 / 0:mmap: 0 | 0:mmap: 0 / 0:mmap: 0 | 0:mmap: 0 / 0:mmap: 0 | 0:mmap: 0 / 0:mmap: 0 |
| `context` | 0:context: -2 / 0:context: -2 | 0:context: -2 / 0:context: -2 | 0:context: -2 / 0:context: -2 | 0:context: -2 / 0:context: -2 | 0:context: -2 / 0:context: -2 |
| `stack` | 0:stack: 0 / 0:stack: 0 | 0:stack: 0 / 0:stack: 0 | 0:stack: 0 / 0:stack: 0 | 0:stack: 0 / 0:stack: 0 | 0:stack: 0 / 0:stack: 0 |
| `handler_heap` | 0:handler_heap: 1 / A:uaf | 0:handler_heap: 1 / 0:handler_heap: 1 | 0:handler_heap: 1 / 0:handler_heap: 1 | 0:handler_heap: 1 / 0:handler_heap: 1 | 0:handler_heap: 1 / 0:handler_heap: 1 |
| `handler_mmap` | 139 / A:segv | 0:handler_mmap: 1 / 0:handler_mmap: 1 | 0:handler_mmap: 1 / 0:handler_mmap: 1 | 0:handler_mmap: 1 / 0:handler_mmap: 1 | 0:handler_mmap: 1 / 0:handler_mmap: 1 |
| `plain_heap` | 0:plain_heap: 1 / A:uaf | 0:plain_heap: 1 / 0:plain_heap: 1 | 0:plain_heap: 1 / 0:plain_heap: 1 | 0:plain_heap: 1 / 0:plain_heap: 1 | 0:plain_heap: 1 / 0:plain_heap: 1 |
| `plain_mmap` | 139 / A:segv | 0:plain_mmap: 1 / 0:plain_mmap: 1 | 0:plain_mmap: 1 / 0:plain_mmap: 1 | 0:plain_mmap: 1 / 0:plain_mmap: 1 | 0:plain_mmap: 1 / 0:plain_mmap: 1 |
| `cx_heap` | 0:coroutine freed: 0 / A:uaf | 0:coroutine freed: 0 / 0:coroutine freed: 0 | 0:coroutine freed: 0 / 0:coroutine freed: 0 | 0:coroutine freed: 0 / 0:coroutine freed: 0 | 0:coroutine freed: 0 / 0:coroutine freed: 0 |
| `a_1_0` | 134U / 134U | 134S / 134S | 134U / 134U | 134S / 134S | 134U / 134U |
| `a_2_0` | 134U / 134U | 134S / 134S | 134U / 134U | 134S / 134S | 134U / 134U |
| `a_1_1` | 134U / 134U | 134S / 134S | 134U / 134U | 134S / 134S | 134U / 134U |
| `a_2_1` | 134U / 134U | 134S / 134S | 134U / 134U | 134S / 134S | 134U / 134U |
| `b_0` | R3 / R3 | R3 / R3 | 134U / 134U | R3 / R3 | 134U / 134U |
| `b_1` | R3 / R3 | R3 / R3 | 134U / 134U | R3 / R3 | 134U / 134U |
| `c_0` | R9 / R9 | R9 / R9 | R9 / R9 | R9 / R9 | R9 / R9 |
| `c_1` | R9 / R9 | R9 / R9 | R9 / R9 | R9 / R9 | R9 / R9 |
| `cj` | R9 / R9 | R9 / R9 | R9 / R9 | R9 / R9 | R9 / R9 |
| `d_0` | R4 / R4 | R4 / R4 | R4 / R4 | R4 / R4 | R4 / R4 |
| `d_1` | R4 / R4 | R4 / R4 | R4 / R4 | R4 / R4 | R4 / R4 |
| `e` | R5 / R5 | R5 / R5 | 134U / 134U | 134U / 134U | 134U / 134U |
| `f` | R6 / R6 | R6 / R6 | R6 / R6 | R6 / R6 | R6 / R6 |
| `gcb` | 0:shallow 6 / 0:shallow 6 | 0:shallow 6 / 0:shallow 6 | 0:shallow 6 / 0:shallow 6 | 0:shallow 6 / 0:shallow 6 | 0:shallow 6 / 0:shallow 6 |
| `gcopy` | 134U / 134U | 0:called 8 / 0:called 8 | 134U / 134U | 134U / 134U | 134U / 134U |
| `grefill` | 134U / 134U | 134S / 134S | 134U / 134U | 134U / 134U | 134U / 134U |
| `ns_0` | R7 / R7 | R7 / R7 | R7 / R7 | R7 / R7 | R7 / R7 |
| `ns_1` | R7 / R7 | R7 / R7 | R7 / R7 | R7 / R7 | R7 / R7 |
| `c2_1` | 134U / 134U | 134S / 134S | 134U / 134U | 134S / 134S | 134U / 134U |
| `c2_2` | 134U / 134U | 134S / 134S | 134U / 134U | 134S / 134S | 134U / 134U |
| `t_partial` | 134U / 134U | 134S / 134S | 134U / 134U | 134U / 134U | 134U / 134U |
| `t_unseen` | 134U / 134U | 134S / 134S | 134U / 134U | 134U / 134U | 134U / 134U |
| `t_unseen0` | R30 / R30 | 134S / 134S | 134U / 134U | 134U / 134U | R30 / R30 |
| `ctx-m1-o0-s0` | 134U / 134U | 134S / 134S | 134U / 134U | 134S / 134S | 134U / 134U |
| `ctx-m1-o0-s1` | 134U / 134U | 134S / 134S | 134U / 134U | 134S / 134S | 134U / 134U |
| `ctx-m1-o1-s0` | 134U / 134U | 134S / 134S | 134U / 134U | 134S / 134S | 134U / 134U |
| `ctx-m1-o1-s1` | 134U / 134U | 134S / 134S | 134U / 134U | 134S / 134S | 134U / 134U |
| `ctx-m2-o0-s0` | R1 / R1 | R1 / R1 | R1 / R1 | R1 / R1 | R1 / R1 |
| `ctx-m2-o0-s1` | R1 / R1 | R1 / R1 | R1 / R1 | R1 / R1 | R1 / R1 |
| `ctx-m2-o1-s0` | R1 / R1 | R1 / R1 | R1 / R1 | R1 / R1 | R1 / R1 |
| `ctx-m2-o1-s1` | R1 / R1 | R1 / R1 | R1 / R1 | R1 / R1 | R1 / R1 |
| `ctx-m3-o0-s0` | 134U / 134U | 134S / 134S | 134U / 134U | 134S / 134S | 134U / 134U |
| `ctx-m3-o0-s1` | 134U / 134U | 134S / 134S | 134U / 134U | 134S / 134S | 134U / 134U |
| `ctx-m3-o1-s0` | 134U / 134U | 134S / 134S | 134U / 134U | 134S / 134S | 134U / 134U |
| `ctx-m3-o1-s1` | 134U / 134U | 134S / 134S | 134U / 134U | 134S / 134S | 134U / 134U |
| `x_static` | R1 / R1 | R1 / R1 | R1 / R1 | R1 / R1 | R1 / R1 |
| `x_heap0` | R1 / R1 | R1 / R1 | 134U / 134U | 134U / 134U | 134U / 134U |
| `x_heap` | R1 / R1 | R1 / R1 | 134U / 134U | 134U / 134U | 134U / 134U |
| `x_tls` | R1 / R1 | R1 / R1 | 134U / 134U | 134U / 134U | 134U / 134U |
| `x_dlopen` | R1 / R1 | R1 / R1 | 134U / 134U | 134U / 134U | 134U / 134U |
| `x_dlclose` | 139 / A:segv | R1 / R1 | R1 / R1 | R1 / R1 | R1 / R1 |
| `x_prot` | 138 / A:bus | R1 / R1 | 134U / 134U | 134U / 134U | 134U / 134U |
| `x_realloc` | 134U / A:uaf | R1 / R1 | 134U / 134U | 134U / 134U | 134U / 134U |
| `x_refill_out` | 134U / 134U | R1 / R1 | 134U / 134U | 134U / 134U | 134U / 134U |
| `x_fake` | R1 / R1 | R1 / R1 | R1 / R1 | R1 / R1 | R1 / R1 |
| `x_unseen` | 134U / 134U | 134S / 134S | 134U / 134U | 134U / 134U | 134U / 134U |
| `x_unseen0` | R5 / R5 | 134S / 134S | 134U / 134U | 134U / 134U | R5 / R5 |
| `x_partial` | 134U / 134U | 134S / 134S | 134U / 134U | 134U / 134U | 134U / 134U |
| `jo_heap` | R1 / R1 | R1 / R1 | 134U / 134U | 134U / 134U | 134U / 134U |
| `jo_tls` | R2 / R2 | R2 / R2 | 134U / 134U | 134U / 134U | 134U / 134U |
| `jo_lib` | R3 / R3 | R3 / R3 | 134U / 134U | 134U / 134U | 134U / 134U |
| `jo_co` | R4 / R4 | R4 / R4 | R4 / R4 | R4 / R4 | R4 / R4 |
| `fk` | R5 / R5 | R5 / R5 | R5 / 134U | R5 / 134U | R5 / 134U |
| `pngdemo` | 0:truncated again -3 / A:uaf | 0:truncated again -3 / 0:truncated again -3 | 0:truncated again -3 / 0:truncated again -3 | 0:truncated again -3 / 0:truncated again -3 | 0:truncated again -3 / 0:truncated again -3 |
| `ijn` | 134S / 134S | 134S / 134S | 134S / 134S | 134S / 134S | 134S / 134S |
| `ijstale` | 134U / 134U | 134S / 134S | 134U / 134U | 134U / 134U | 134U / 134U |
| `ijo` | 134S / 134S | 134S / 134S | 134S / 134S | 134S / 134S | 134S / 134S |
| `ijv1` | 134S / 134S | 134S / 134S | 134S / 134S | 134S / 134S | 134S / 134S |
| `ijv2` | 134S / 134S | 134S / 134S | 134S / 134S | 134S / 134S | 134S / 134S |
| `ijv3` | 134S / 134S | 134S / 134S | 134S / 134S | 134S / 134S | 134S / 134S |
| `ap_1_0` | 134U / 134U | 134S / 134S | 134U / 134U | 134S / 134S | 134U / 134U |
| `ap_2_0` | 134U / 134U | 134S / 134S | 134U / 134U | 134S / 134S | 134U / 134U |
| `ap_1_1` | 134U / 134U | 134S / 134S | 134U / 134U | 134S / 134S | 134U / 134U |
| `ap_2_1` | 134U / 134U | 134S / 134S | 134U / 134U | 134S / 134S | 134U / 134U |
| `au_1` | 134S / 134S | 134S / 134S | 134S / 134S | 134S / 134S | 134S / 134S |
| `au_2` | 134S / 134S | 134S / 134S | 134S / 134S | 134S / 134S | 134S / 134S |
| `nn` | R31 / R31 | 134S / 134S | 134U / 134U | 134S / 134S | R31 / R31 |
| `fp1` | R41 / R41 | R41 / R41 | R41 / R41 | R41 / R41 | R41 / R41 |
| `fp2` | R42 / R42 | R42 / R42 | R42 / R42 | R42 / R42 | R42 / R42 |
| `fp3` | R43 / R43 | 134S / 134S | R43 / R43 | R43 / R43 | R43 / R43 |
| `as_1` | 134K / 134K | 134U / 134U | 134U / 134U | 134U / 134U | 134K / 134K |
| `as_0` | R51 / R51 | 134S / 134S | R51 / R51 | R51 / R51 | R51 / R51 |
| `g1_0` | R61 / R61 | 134S / 134S | 134U / 134U | 134S / 134S | 134U / 134U |
| `g1_1` | R61 / R61 | 134S / 134S | 134U / 134U | 134S / 134S | 134U / 134U |
| `g2` | R62 / R62 | 134S / 134S | 134U / 134U | 134S / 134S | R62 / R62 |
| `g3` | 134U / 134U | 134S / 134S | 134U / 134U | 134S / 134S | 134U / 134U |
| `rr_0` | 134+ran / 134+ran | 134U / 134U | 134U / 134U | 134U / 134U | 134+ran / 134+ran |
| `rr_1` | 134+ran / 134+ran | 134U / 134U | 134U / 134U | 134U / 134U | 134+ran / 134+ran |
| `sn` | R72 / R72 | R72 / R72 | R72 / R72 | R72 / R72 | R72 / R72 |
| `cs_0` | R90 / R90 | R90 / R90 | 134U / 134U | R90 / R90 | 134U / 134U |
| `cs_1` | 134K / 134K | 134S / 134S | 134U / 134U | 134S / 134S | 134U / 134U |

**Read against the brief's expectations (every cell above, both modes):**
- **C4, MUST RUN: all run.** `b_0`, `b_1`, `c_0`, `c_1`, `cj`, `d_0`, `d_1`, `e`, `gcb`, `ns_0`, `ns_1`,
  `f`, the four `ctx-m2`, the ten `x_*` (`x_static`, `x_heap0`, `x_heap`, `x_tls`, `x_dlopen`,
  `x_dlclose`, `x_prot`, `x_realloc`, `x_refill_out`, `x_fake`), `jo_heap`, `jo_tls`, `jo_lib`,
  `jo_co`, `fk` under the ASan option, `pngdemo` clean, the eight `repro/` shapes and `cx_heap` exit 0
  with no sanitizer line; and the new MUST RUN: `fp1` (F′), `fp2` (F″), `sn`.
- **C4, MUST BE REFUSED: all refused.** The four `a_*`, the eight `ctx-m1`/`ctx-m3`, `c2_1`, `c2_2`,
  `x_unseen`, `x_unseen0`, `x_partial`, `t_partial`, `t_unseen`, `t_unseen0`, `grefill`, `ijstale`,
  `ijn`, `ijv1` to `ijv3`, `ijo`; and the new: the four `ap_*` (A′), `au_1`, `au_2` (A by an unseen
  switch), `nn` (N), `rr_0`, `rr_1` (R), `as_1`. Every refusal in S words but `as_1`, `rr_*` (U: a
  landing in frames already left).
- **C4 against today, toward refusal of a correct program**: `fp3` (F‴), `as_0`, `g1_0`, `g1_1`,
  `g2` (G, the author's question). **Toward running a correct program**: `gcopy` (ruling B's known cost
  gone, the author's to read), `x_realloc`, `x_refill_out`, `x_prot`, `x_dlclose` (the last two crash
  today). **Toward refusing what today runs inside a signal**: `nn`, `rr_0`, `rr_1`, `t_unseen0`,
  `x_unseen0`. **No row moves toward running inside a signal.** Words: every U refusal of today on a
  context or a refill becomes S where the handler is in fact still running.
- **C4 forced to fail (both families on A)**: of its MUST RUN column (`d_*`, `gcb`, `ns_*`, `b_*`,
  `c_*`, `f`, the 2679 shapes) **`b_0` and `b_1` fail**, 134 U both modes: on Darwin route A cannot
  trust a context on the heap (its fingerprint may be read only where the buffer provably lives), so a
  `setcontext` out of a handler through one is refused. The MUST BE REFUSED column holds whole (after
  the trampoline's settle). A's listed costs are measured as listed (`e`, `x_heap0`, `x_heap`,
  `x_tls`, `x_dlopen`, `x_prot`, `x_realloc`, `x_refill_out`, `jo_heap`, `jo_tls`, `jo_lib`, `fk`
  sanitized, `gcopy`), plus `g*` and `cs_0`; `x_fake` sanitized runs on Darwin. **With the setjmp family
  alone forced** (contexts still decoded, each family verified apart) **the whole forced column holds**,
  `b_*` included. Darwin x86-64, A for both by construction, is the column that fails `b_*`.
- **C, 2679 alone (A)**: closes 2679's shapes, `x_dlclose`'s and `x_prot`'s crashes and libpng's
  report; moves no context row toward running inside a signal; keeps today's five residues and refuses
  thirteen correct programs (its section above).

## The goldens a landing owes, on both platforms, and what each CI leg will read

No case in the tree switches a context (critic-reports finding 8), so every row above that a C4
landing decides is invisible to every leg until it is a `run` golden. Owed, one `.hero` and one
`.expected` each, the same bytes on `macos-26`, `ubuntu-26.04-arm` and `ubuntu-26.04` (the harness
keeps one `.expected` per case, so a word that differs by platform is a red leg, which is rule 8 made
executable):
1. **the route**: a case whose header declares the accessor and prints whether every family decodes
   (`hero_signal_jump_route() == 3` on Darwin; the Linux accessor's spelling is the engineer's, and one
   name for both is the synthesis's); a second case built with the forcing define is not expressible
   as a `run` golden (the define is the runtime's), so the forced path's guard is a `runtime` suite
   check that builds the runtime with it: **owed, unbuilt**.
2. **the union, from `F/p255/r2/sh/`**: `a_1_0` `a_2_0` `a_1_1` `a_2_1` (refused S), `b_0` `b_1` (run),
   `c_0` `c_1` `cj` (run), `d_0` `d_1` `e` `f` (run), `ns_0` `ns_1` (run), `c2_1` `c2_2` (refused S),
   the twelve `ctx-m*` (m1, m3 refused S; m2 run), `t_partial` `t_unseen` `t_unseen0` (refused S), the
   thirteen `x_*` as ported (ten run, three refused S), `jo_heap` `jo_tls` `jo_lib` `jo_co` (run);
3. **the new**: `ap_1_0` `ap_2_0` `ap_1_1` `ap_2_1` (refused S), `au_1` `au_2` (refused S; the switch
   is arm64 assembly, so the `ubuntu-26.04` leg needs its x86-64 twin: **owed, unwritten**), `nn`
   (refused S), `fp1` `fp2` (run), `rr_0` `rr_1` (refused U), `as_1` (refused U), `sn` (run); and,
   pinned as the author rules, `fp3` `as_0` `g1_0` `g1_1` `g2` `g3` `cs_0` `cs_1` `rr_2`;
4. **unpinnable as `run` goldens as the harness stands**: `fk` (needs `ASAN_OPTIONS` set by the
   harness), `pngdemo` (needs libpng on the leg).

**The prediction, one, falsifiable by a CI leg**: on `macos-26`, after the push that lands C4 on Darwin
with goldens 1 to 3, the route case prints `route 3` plain and under `--sanitize` (both families
calibrate on that libsystem: word 12 `pacdb(sp) ^ k`, the context's `+0x148`, the patterns and the
second mangled words), and every golden of 2 and 3 blessed on this Mac passes there unchanged, in
particular **`nn`, `ap_1_0`, `au_1` exit 134 in S words, `rr_0` in U words, and `b_1`, `c_1`, `fp2`,
`sn`, `gcb` exit 0 with their lines**. It is falsified by any other route there (a calibration that
fails on `macos-26`; then `e`, `x_heap`, `jo_heap` would go red with it) or by any of those goldens
reading otherwise. Checkable at the lane that lands C4 (this Mac), then at the `macos-26` leg.

## The harness, per column, this Mac

`F/p255/r2/gate.sh`: a column's `rebind.c` and `sigguard.c` copied into the copy's `runtime/parts`,
stage 2 built by the copy's compiler into `./heroes-gate` (C4: `EXIT 0 WALL 269s`, `heroes 0.6.0`,
`sources 589f1e6261e49c42`), then `./heroes-gate run tests/harness/main.hero -- ./heroes-gate run
<set>` and `-- ./heroes-gate runtime`, every count line read whole from
`F/p255/r2/logs/gate-<column>-<set>.log`; the tree's two files restored from `HEAD` after (`git status
--short`: `?? p255/` only). Today's and A's columns are round one's (`F/p255/logs/today-*.log`,
`A-*.log`; A's `rebind.c` is the same file, `md5` `f872835c...`, so C, 2679 alone's column is
measured, not carried). Every run beside the other seat's work (load 30 to 100): no time is claimed.
**C4's gate ran on `rebind.c` `9f989948...`**, the file before the trampoline's settle, which is
inert on the decode path (the final union read every shared row the same).

| set | today | **C4** | C4 forced to fail (both) | C, 2679 alone (A) |
|---|---|---|---|---|
| `fixedbugs-2571` | 4 / 0 | **4 passed, 0 failed** | 4 / 0 | 4 / 0 |
| `fixedbugs-2558` | 5 / 0 | **5 / 0** | 5 / 0 | 5 / 0 |
| `fixedbugs-2522` | 25 / 0 | **23 passed, 2 failed**: the two goldens the brief names | **25 / 0** | 25 / 0 |
| `fixedbugs-a-c-handler-that-recovers-leaves-no-line` | 1 / 0 | **1 / 0** | 1 / 0 | 1 / 0 |
| `fixedbugs-2494` | 15 / 0 | **15 / 0** | 15 / 0 | 15 / 0 |
| `fixedbugs-2257` | 3 / 0 | **3 / 0** | 3 / 0 | 3 / 0 |
| `fixedbugs-2583` | 6 / 0 | **6 / 0** | 6 / 0 | 6 / 0 |
| `fixedbugs-2496` | 1 / 0 | **1 / 0** | 1 / 0 | 1 / 0 |
| `fixedbugs-2503` | 1 / 0 | **1 / 0** | 1 / 0 | 1 / 0 |
| `an-ignored-signal-stays-ignored` | 1 / 0 | **1 / 0** | 1 / 0 | 1 / 0 |
| `the-platform-layer-stop` | 3 / 0 | **3 / 0** | 3 / 0 | 3 / 0 |
| `runtime` | 10 / 0, 2 floors outgrown | **10 passed, 0 failed**, 2 floors outgrown | 10 / 0, 2 floors outgrown | 9 / 1 (the census) |

**C4's two 2522 moves** (`gate-C4-fixedbugs-2522.log`): `...-a-buffer-a-handler-refills-by-memcpy-and-jumps-to-is-refused`
still refused, its words S (*"had not been seen to end"*, true: the handler still runs) where its
`.expected` pins U; `...-a-correct-jump-out-of-a-handler-through-a-copied-buffer-is-refused-in-words-that-say-so`
**runs**, `called 8`, exit 0, against an `.expected` that pins the refusal under the author's ruling
B. Neither `.expected` was touched; both are the author's to read. The twelve `a-callback-after-*`
cases pass, alternate and own stack, every jump spelling, deep-first and shallow-first.

**The census, each static named with its reason** (`diff` of the `static … ;` rows, HEAD against C4;
the suite's count of objects that survive between calls, today 348, C4 357):
- **gone with the list** (the entries now live in the claimed thread's record, `parts/sigguard.c`):
  `hero_rebind_key`, `hero_rebind_keyed`, `hero_rebind_unsure_key`, `hero_rebind_unsure_keyed`;
- **new, each `_Atomic`, so the spelling is the decision and no `SHARED_BY_DECISION` entry is owed**:
  the known stacks, `hero_rebind_stack_seq`, `_lo`, `_hi` (256 each), `hero_rebind_stack_next`,
  `hero_rebind_stack_writing` (the spin word that replaced kC5's `atomic_flag`, which the census does
  not read as atomic), `hero_rebind_stack_any`: process-wide on purpose, a context made on one thread
  may be resumed on another, written under the spin word and read slot by slot under a sequence word;
  the decode, `hero_rebind_decodes`, `hero_rebind_key_k`, `hero_rebind_cal_bad`,
  `hero_rebind_cal_swap_sp`, `hero_rebind_cal_from`: written before `main`, read after; route A's
  regions, `hero_rebind_region_lo`, `_hi`, `hero_rebind_regions` (round one's A owed them a reason;
  spelled `_Atomic` now, they owe none);
- **kept, `SHARED_BY_DECISION`'s `hero_rebind_records`**: still named, so the suite stays green, but
  its words (*"the trampoline entry live at its save"*) now describe the serial of the innermost entry
  standing at the save, and only for a family on route A: **the entry's text owes that rewording**
  (`tests/harness/suite_runtime.hero:383`), unwritten here;
- **not read by the census at all** (it reads `static` objects only, round one's note): the
  non-static `_Atomic` `hero_rebind_cal_sp`, `hero_rebind_cal_lr` (written by the calibration stub's
  `str`) and `hero_rebind_real_makecontext` (read by the wrapper's `ldr`).

## Rule 5's premise, refuted by a run, and the repair (C4r, a variant built beside C4)

The brief's refuter, looked for: **a suspended handler resumed elsewhere than the frame that suspended
it.** `F/p255/r2/sh/sp.h` (`sp_0`): the handler saves a buffer of its own, swaps to a heap coroutine,
the coroutine resumes the handler by `siglongjmp` to that buffer (not through the swap), and the
handler, running again, leaves for good by the plainest jump out; the runner calls the program 64 KiB
deeper. Correct: MUST RUN. 3 runs, plain / `--sanitize`:

| `sp_0` | today | kC5 | **C4** | C4 forced (both) | C4 forced (setjmp family) | A |
|---|---|---|---|---|---|---|
| resumed by a jump, then left | runs, `answer ran for 95` | **134 S** | **134 S** | 134 U | 134 S | runs |

Under C4 the swap's suspension is released only by the swap's own return, which never comes; nothing
drops a suspended entry, so the handler that was resumed and then left stands for ever: a correct
program today runs, refused, for every later callback of the thread. **The repair, built as C4r**
(`F/p255/r2/C4r/`, `rebind.c` 1,438 lines; `rt/C4r`): a landing at or below a standing entry on the
target's range is that handler running again, so it releases the entry's suspension and hold; each
entry carries a generation that this bumps, and a swap records the generation with its mark, so a swap
that returns late releases nothing a later swap holds (a CAS that never lets the count below 0); on route
A, a trusted record naming a standing entry releases it the same way. Measured on the decisive set
(`F/p255/r2/logs/shapes-C4r.log`, 3 runs, both modes): `sp_0` **runs** (`answer ran for 95`); `nn`,
`a_1_0`, `a_2_1`, `c2_1`, `c2_2`, `ctx-m3-o1-s1`, `ap_1_0`, `ap_2_1`, `g1_0`, `g2` refused S and
`rr_0` U, as under C4; `ctx-m2-o0-s0`, `b_1`, `c_1`, `ns_1`, `gcb`, `cj` run. C4r compiles clean for
arm64 and x86-64 with the runtime's flags (`-O2`, 0 warnings). Its whole union follows below; **its
harness gate is unrun** (the forced column's gate holds the copy until late).

**C4r's whole union** (`F/p255/r2/logs/shapes-C4r.log`, every shape of the table above plus `sp_0` and
`rr_2`, 3 runs, both modes), diffed cell by cell against C4's by `table.py`: **identical on every row
but `sp_0`** (C4 134 S, C4r runs), and `rr_2`, where both run the program inside the resumed handler
(the residue above; only the exit after it differs, the handler returning through clobbered frames).
`rebind.c` against HEAD: C4 **+887 −211**, C4r **+922 −211**; `sigguard.c` C4 **+34 −1**, C4r
**+35 −1** (`git diff --no-index --numstat` against `git show HEAD:...`).

## What C4 removes, what it costs against today (Darwin, measured)

**Removes**, on the decode path (both families verified, `route 3` on this Mac): every save point's
rewrite and record, and with them the fingerprint read of a buffer the program may have given back
(2679: the eight shapes, `cx_heap`, libpng's second decode, `x_dlclose`'s 139 and `x_prot`'s 138 all
clean, both modes); today's list of entries threaded through the trampolines' frames and read through
`me.prev`; today's early return at a jump made where no entry stands (HEAD `rebind.c:427`), which is
what lets R run. **Adds**: the decode and its calibration, entries kept as addresses in the claimed
thread's record with serials, per-entry suspension, the hold, the re-entry rule, the known stacks.

**Correct programs today runs that C4 refuses** (the author's to read, as ruling B was): `g1_0`, `g1_1`,
`g2` (G: refused for good, by rule 5 as the brief states it); `fp3` (F‴: a stale range carved from the
thread's own stack holding the target, the price of judging a target on a coroutine first); `as_0` (a
handler on the thread's own stack jumping into a coroutine carved from it, never judged left because
its range is another's); `sp_0` (C4 only; **C4r runs it**). `cs_1` is refused by both (today by the
on-stack test). **Correct programs today refuses or crashes that C4 runs**: `gcopy` (ruling B's cost,
the author's), `x_realloc`, `x_refill_out`, `x_prot`, `x_dlclose`. **Programs today runs inside a
signal that C4 refuses**: `nn`, `rr_0`, `rr_1`, `t_unseen0`, `x_unseen0`. **Residue C4 keeps, shared
with today**: `rr_2`. **Words**: U becomes S on every context and refill row where the handler is in
fact still running; the two goldens that move are the brief's (`grefill` words, `gcopy` verdict).

**Where the calibration fails** (any libsystem that moves a word, and Darwin x86-64 by construction):
the family that failed falls to A alone (`route 2` or `route 1`, measured by the negative controls), and
A's known costs follow it (`e`, `x_heap0`, `x_heap`, `x_tls`, `x_dlopen`, `x_prot`, `x_realloc`,
`x_refill_out`, `jo_heap`, `jo_tls`, `jo_lib`, `fk` sanitized, `gcopy`), plus `b_*` where contexts
fail too: the one split from the brief's forced column, measured.

**The FFI seat's own reading.** No route changes what a binding writes: the libpng binding is round
one's 25-line shim, verified by clang against Homebrew's `png.h` in every build of every column (the
build exits 0 under all five), and C4 adds no name a program defines and changes no rewritten name a
program calls (the table rewrites the same jumps and `makecontext`, and the saves only for a family
on A). The header check did its job on the seat's own C this round: `cs.h` was refused at build time
(`MAP_ANON` undeclared under `_XOPEN_SOURCE` without `_DARWIN_C_SOURCE`) before anything ran.

**The forced column's gate** (`rt/C4f3`, `rebind.c` `0b0ffcf2...`, stage 2 `EXIT 0 WALL 157s`, logs
`F/p255/r2/logs/gate-C4f3-*.log`), filled into the table above: every set passes, **2522 at 25 / 0**
(route A keeps today's verdicts and words on the golden tree, the copied buffer refused and the refill
in U words), `runtime` 10 / 0. The gate of the `setjmp`-family-only column (C4f1) was not run: the gate
went to C4r instead (below).

## Unrun, said so

- **`macos-26`'s libsystem** (the prediction's leg; after a push), Darwin x86-64 (no Rosetta here:
  C4 and C4r **compile** clean for it with the runtime's flags at `-O0` and `-O2`, 0 warnings, and are
  route A for both families there by construction; nothing ran), an arm64e process (no Heroes program is
  one).
- **Linux, every row**: the engineer's. My new headers (`r2new.h`, `rr.h`, `sn.h`, `cs.h`, `sp.h`) are
  written portable (`_GNU_SOURCE` off Darwin) and syntax-checked on Darwin only; `au_switch` is arm64
  assembly, so `ubuntu-26.04` needs its x86-64 twin, unwritten. Rule 8's same-verdict test across the
  platforms is the critic's `diff` of the two tables.
- **C4f1's gate** (the `setjmp`-family-only forced column): its union is above, its harness is unrun.
  **C4r's gate**: running at the time of writing (below, if it ends in this round).
- `--sanitize=thread` under any column; a stress run of the known-stack table, the entries or the
  suspension counts under many threads; the per-range *bottom* that would close `rr_2`; the less
  conservative G (a swap to a non-coroutine read as a jump); the rewording of `hero_rebind_records`'s
  `SHARED_BY_DECISION` entry; a `runtime` suite check built with the forcing define; `fk` and
  `pngdemo` as `run` goldens.
- **Timing beyond the one error-path benchmark** (indicative, load above 40): not owed, not taken.

## The new shapes on Linux arm64 under TODAY's runtime (for rule 8; C4 on Linux is the engineer's)

`F/p255/r2/linux-today.sh` in `heroes-linux-arm64:latest` (`--network none`, my copy mounted read-only,
HEAD's `rebind.c` and `sigguard.c` put back inside the container because the Mac's gate held the copy's
`runtime/parts`; stage 1 from the seed, `sources 05186f2b38e77a97`, as round one's Linux run: a compiler
that builds programs, not the harness's), each shape plain / `--sanitize`, 3 runs, log
`F/p255/r2/logs/linux-today.log` (a first run's log was lost with its `--rm` container; this is the
second, whole):

| shape | Linux arm64, today | Darwin, today | Darwin, C4 |
|---|---|---|---|
| `ap_1_0`, `ap_2_0`, `ap_1_1` A′ | **run inside the signal** (`11`, `12`, `11`) | 134 U | 134 S |
| `ap_2_1` | 134 K | 134 U | 134 S |
| `au_1` A by an unseen switch | **runs** (`21`) | 134 S | 134 S |
| `au_2` | 134 S | 134 S | 134 S |
| `nn` N | **runs** (`31`) | **runs** | 134 S |
| `rr_0`, `rr_1`, `rr_2` R | **run, then 139 / ASan SEGV** | **run, then 134** | 134 U, 134 U, **runs** |
| `g1_0`, `g1_1`, `g2`, `g3` G | run, run, run, **run** | run, run, run, 134 U | 134 S each |
| `as_0`, `fp3`, `cs_0`, `sp_0` (correct) | run | run | 134 S (`cs_0` runs; `sp_0` runs under C4r) |
| `as_1`, `cs_1` | 134 K; `cs_1` runs | 134 K, 134 K | 134 U, 134 S |
| `fp1`, `fp2` | run | run | run |
| `sn` | **134, a null function pointer called / ASan stack-overflow**: glibc's `setcontext` jumps through a zeroed context where libsystem answers -1, so `sn` is a Darwin-only program as written; its port keeps only `swapcontext(from, NULL)` | runs | runs |

So today's Linux runs a Heroes function inside a signal on eight of the new shapes (three A′, `au_1`,
`nn`, `g3` as a resumption it cannot see, three R), defect 2522's class, for the context defect; and the
new shapes are now the rows on which the engineer's C4 and this one must agree (rule 8).

*Corrected 23:22, the sentence above*: `g3` does not belong in that list. G is the author's question
(correct by b_*'s reasoning, its handler swapped away from and never resumed); that today's Linux runs
`g3` is the verdict today's Darwin refuses (U) and C4 refuses (S). The eight shapes on which today's
Linux runs a Heroes function inside a signal are the three A′, `au_1`, `nn` and the three R.

## C4r's gate (ended before 23:46; logs `F/p255/r2/logs/gate-C4r-*.log`)

Stage 2 with C4r's runtime `EXIT 0 WALL 258s`; every set as under C4: 2571 4/0, 2558 5/0, **2522 23
passed, 2 failed** (the same two goldens: the refill case in S words, the copied buffer running),
the recovers case 1/0, 2494 15/0, 2257 3/0, 2583 6/0, 2496 1/0, 2503 1/0, the ignored signal 1/0,
the platform-layer stop 3/0, **`runtime` 10 passed, 0 failed** (2 floors outgrown, as today). After
the gate the copy's `runtime/parts` was restored from `HEAD` (`git status --short`: `?? p255/` only).
No container of this seat remains (`docker ps -a --filter name=ffi255`: none).

## Verdicts (Darwin)

- **C4, as C4r builds it: approve.** Every MUST row of the brief's C4 column holds, plain and
  `--sanitize`, on the eight 2679 shapes, the union, the new shapes and libpng; the harness moves only the
  two goldens the brief names; the census needs no new entry. It refuses five programs today runs inside
  a signal (`nn`, `rr_0`, `rr_1`, `t_unseen0`, `x_unseen0`) and moves none the other way; kC5's three
  departures it does not take are each measured to run a Heroes function inside a signal. Its costs are
  told refusals for the author: G (`g1_0`, `g1_1`, `g2`), `fp3`, `as_0`; under a failed calibration,
  A's listed costs and, where contexts fail too (Darwin x86-64), `b_*`. Plain C4 is the same verdict
  less `sp_0`, a correct program it refuses for good (rule 5's premise, refuted by that run).
- **C, 2679 alone (A): object.** Sound (it closes 2679's shapes and libpng's report and moves no context
  row toward running inside a signal), but it refuses thirteen correct programs today runs, in U words
  false for each, `b_0` and `b_1` among them, and keeps today's five residues. C4 itself meets this
  column's test on Darwin, so Darwin needs no separate 2679 landing.

## The seat's form

- **verdict**: C4 (as C4r) approve; C, 2679 alone (A) object.
- **section**: design.md §1.12 (*"must not segfault and must not corrupt memory"*; *"any C library must
  be bindable"*), §1.11, §4.19; `00-shared.md` § 6's veto list. design.md has no rule on a verdict
  that differs by platform: said in round one, unchanged.
- **experiment**: libpng 1.6.59 through round one's 25-line shim, clang-verified against Homebrew's
  `png.h` in every build under all five runtimes (exit 0 each): today `--sanitize` heap-use-after-free on
  the second decode, every other column clean. Plus the calibration and its four negative controls, the
  union, the new shapes, the error-path benchmark and the gates above.
- **argument**: C4 reads one word of the jump's target, which the C library reads next, decoded by a key
  verified before `main` on patterns at two stacks, each family falling to A alone. On this Mac it meets
  every MUST row, closes five programs today runs inside a signal, runs five correct ones today refuses
  or crashes on, and costs half today's user time on a C library's error path. Its refusals are told:
  G, `fp3`, `as_0`, A's costs where a calibration fails. C4r frees a handler resumed by a jump, moving no
  other cell. A as 2679-alone is sound but refuses thirteen correct programs and keeps today's residues.
- **prediction**: on `macos-26`, after the push landing C4 with the union as `run` goldens, the route
  case prints `route 3` plain and under `--sanitize`, and `nn`, `ap_1_0`, `au_1` (134 S), `rr_0` (134
  U), `b_1`, `c_1`, `fp2`, `sn`, `gcb` (exit 0) pass with this Mac's `.expected` unchanged.
- **condition**: approve becomes object if `macos-26`'s calibration does not verify, if the critic's
  `diff` shows a union row the Linux C4 and this one cannot agree on, or if the author rules G, `fp3` or
  `as_0` must run; veto if any MUST BE REFUSED row is shown to run under C4r on Darwin beyond `rr_2`, the
  residue shared with today that the per-range bottom would close.
