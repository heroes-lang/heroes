# Panel 259, the completeness critic: second reading (the reports, before the synthesis)

Started 2026-11-25 11:02 by this Mac's clock (`date`), a fresh agent. My copy is the detached worktree
`.claude/worktrees/259-critic` at `3ec775b66` (`git rev-parse HEAD`); long output in
`.claude/worktrees/259-critic-scratch/r2/`. Written as I go. Every finding carries its command and its result; what I
read and did not run says **read**. I give no verdict on any route.

Read before any run: `00-shared.md` (as repaired), `critic.md`, my first reading (`critic-briefs.md`), the five seats'
briefs, the four reports (`compiler-engineer.md`, `ffi-pragmatist.md` with `ffi-clauses.md`, `spec-warden.md` with
`spec-w*.txt`/`.diff`, `historian.md`), `blind/` (`answers.md`, `registration.md`, both tasks), and the programs the
seats kept.

## Status

- 11:03 my compilers building in my copy (seed, then stage 2); 11:12 the arm64 container `p259-critic-1` started
  (`aarch64`, *Debian GLIBC 2.41-12+deb13u3*, *Debian clang version 22.1.8*), building its own two from a `git archive
  3ec775b66` of `seed runtime selfhost spec vendor` and the root files.
- 11:15 this Mac's stage 2 built (`real 461.26`, `user 227.70`, the machine at load 97 to 116: indicative only);
  `./heroes-next --version` prints `sources 117362877b136f67`, the brief's key.
- Stopped by the usage limit after about 11:50; resumed 12:31 on the coordinator's word, nothing half-written. 12:33
  the arm64 container removed (`docker rm -f p259-critic-1`) and the x86-64 one, `p259-critic-2`
  (`heroes-linux:latest`, emulated), started for § J.

## A. The compiler-engineer's route, run against the other seats' shapes and mine (this Mac, at `3ec775b66`)

**What I ran it with.** `rt-ebo6` is a copy of my tree's `runtime/` with the compiler-engineer's
`programs/compiler-engineer/runtime-rt-ebo6.diff` applied by `patch -p1` from inside the copy (its README's recipe);
`diff -r` against the seat's own `259-compiler-engineer-scratch/rt-ebo6/` prints nothing, and the diff equals its
scratch's `rt-ebo6.diff` by `cmp`. `rt-e` (route E alone) the same way from `runtime-rt-e.diff`. Every cell below is
built by my stage 2 with `HEROES_RUNTIME=<copy>` (or none, *base*), run three times, `timeout 120`, output bounded to
100,000 bytes (`259-critic-scratch/r2/cells.sh`, one line a cell; logs `mac-matrix.log`, `mac-first.log`,
`mac-jumps.log`, `mac-blind-sw.log`; outputs under `mac/<runtime>/`). **The ffi-pragmatist ran no route-E runtime**
(its report: *"a route-E runtime (no seat handed me one)"*), so these are the first cells of its shapes under the
route the compiler-engineer would adopt.

**A1. Three correct programs that route E alone runs right are corrupted under `rt-ebo6`; two of them (`aj_carved`,
`mj_carved`) run right on `main` today.**

| program | what C does | base (R1) | `rt-e` (E alone) | `rt-ebo6` (E+B+owner, the adopted route) |
|---|---|---|---|---|
| `aj_carved` (the ffi-pragmatist's: **OpenSSL 3.5.9's ASYNC**, Homebrew's libcrypto, fibers given quarters of a helper's array through the library's own `ASYNC_set_mem_functions`; libcrypto switches by `makecontext`, `setcontext` and `_setjmp`/`_longjmp`) | its jobs pause and are resumed by the C library's `_longjmp` UP into a fiber carved above the resuming frames | **exit 0, 3 of 3**: `body holds 7` three times, `jobs carved: 7` (mine, and the ffi-pragmatist's) | **exit 0, 3 of 3**, the same bytes | **exit 134, 3 of 3, equal by `cmp`**: `body holds 7 paused 1 1`, **`body holds 21`**, **`body holds 20 jobs 1030 1032`**, then `panic: internal error: a large value's room was given back out of order (compiler bug)`, `called from ... in aj_carved.body`; `--sanitize` exit 134, 3 of 3, `heap-use-after-free` |
| `cpu_heap` (the ffi-pragmatist's: one `malloc`'d region shared by two coroutines, copied out and in, every coroutine made by `makecontext` and switched by `swapcontext`, the runtime seeing every switch) | `makecontext` for coroutine 1 is called on the region while coroutine 0 is suspended, its image saved to the heap | exit 134, 3 of 3, not equal (two panics, as the seat found) | **exit 0, 3 of 3**: `body holds 7` three times, `copied by ucontext, heap: 7` | **exit 134, 3 of 3, equal**: `body holds 7` twice, then `panic: a null pointer was read through, at offset 0x14, called from hero_frame_aside_unlink` and *"a handle or `ptr` holding `nullptr` handed to a C function ... This runtime raised neither"*, false words; `--sanitize` voided by ASan's own `stack-buffer-underflow`, as the seat found at the base |

**Traced** (a copy `rt-ebo6-trace`, three `fprintf`s: each aside take, each entry route B's jump hook gives back, each
entry its `makecontext` event gives back; `rt-ebo6-trace.diff`, 19 lines before the third, built the same way):

- `aj_carved`: `TRACE heal-jump gives frame=0x16d99e260` twice, the two blocks `body`'s first takes had
  (`TRACE take frame=0x16d99e260 block=0x7bfd420030` and `...464030`), **while `body` is live**; the fibers' frames
  stand at `0x16d9de240`, 0x3ffe0 above, inside `aj_host`'s array. So **the jump hook (`hero_heal_jump`, B's first
  event) took a `_longjmp` UP into a fiber carved from the thread's stack as abandoning the live frames below it**:
  its bound is `here >= own->lo && t < own->hi` (the thread's whole measured range, the diff's line 222), and it asks
  `hero_lane_known` only where the jumper is outside that range, so a carved fiber is *the thread's own* to it, the
  premise the seat's veto names in R1 (`stack_lane.c:51-55`). The owner's proof does not help: a live frame's image
  holds its own pointers.
- `cpu_heap`: `TRACE heal-range lo=0x75114a8000 hi=0x7511528000 gives frame=0x7511527fb0` four times, coroutine 0's
  four takes in its suspended `run`, at coroutine 1's `makecontext` on the shared region; coroutine 1's `run`, at the
  same frame address, then takes the same four blocks (`TRACE take frame=0x7511527fb0 block=0x7511528030` ...). So
  **B's second event (`hero_heal_range`, *"`makecontext` names a range as a stack that starts over"*) is false for a
  copying library over `makecontext`**: coroutine 0's pointer is still a word of the region (copied out, never
  cleared), which is the owner's proof passing on a live entry.

**A reproducer of the first with no OpenSSL** (mine, `mj/mj.h` and `mj_carved.hero`: OpenSSL's
`async_fibre_swapcontext` written out, fibers made by `makecontext` on quarters of `mj_host`'s array, entered first by
`setcontext` and then by `_longjmp`, every switch saved by `_setjmp`, as libcrypto 3.5.9 imports them here: `nm -u
libcrypto.3.dylib` prints `__longjmp __setjmp _getcontext _makecontext _setcontext`, the ffi-pragmatist's correction of
the brief, re-run): base **exit 0, 3 of 3**, `body holds 7` three times; `rt-e` **exit 0, 3 of 3**, the same; **`rt-ebo6`
exit 134, 3 of 3, `body holds 7`, `body holds 21`, `body holds 20 fibres 30 32`, then *compiler bug***. It needs no
library, so it can stand as a golden case of whatever route lands.

Both are the seat's own `condition` (*"a program in which the owner's proof gives a live entry back"*), met on this
Mac and on Linux arm64 (§ B, traced there too); the seat's sentence *"Every carved, copying, heap and lend shape runs right under it"* and its § 4 *"B with the
owner's proof is sound under every switch I could write"* do not hold of the other seat's shapes. **None of the three
failures is E's**: `rt-e` runs all three right. What `rt-e` alone costs is the seat's own table (the two jump goldens and `jumpco`
*stack exhausted*).

**A2. Everything else the ffi-pragmatist ran corrupting at the base runs right under `rt-ebo6`, three of three,
plain and `--sanitize`** (each output read whole): Boost.Context 1.92 `bfc_host`, `bfc_below`, `bfc_heap`,
`bfc_host_01` (coroutine 0 ended first), and told, `bfc_host_told`, `bfc_below_told`, `bfc_heap_told`; the copying
designs `cpo_host` (greenlet's), `cpo_host_01`, `cpo_host_told`, `cpo_heap` (Tencent libco's), `cpo_heap_01`,
`cpo_heap_told`; `aj_heap` (OpenSSL on `malloc`'s fibers). So **the ffi-pragmatist's measured objection to a range
tell (`cpo_heap_told`, `cpo_host_told` crash) is a fact of R1's lanes, not of a tell**: under E the told and the
untold run alike, and its *"no runtime built here makes that clause true ... untested until a route-E runtime
exists"* is now tested: true for these thirteen, false for `cpu_heap` and `aj_carved` under `rt-ebo6`, true for both
under `rt-e`.

**A3. B's price is wider than *"an abandonment the runtime does not see"*: a seen jump from deep below the callback
leaks.** My shapes (`mine/` of the scratch; `dj.h` and `deep_jump.hero`: a callback holding an `E5` calls C, which
recurses and then jumps back above the callback with the C library's `longjmp`, a jump every hook sees; 5,000 times):

| program | the span the jump abandons | base | `rt-ebo6` |
|---|---|---|---|
| `deep_jump` | 1,200 C frames of 128 bytes of locals | exit 0, 3 of 3, `sum 35000` | **exit 134, 3 of 3**: `panic: stack exhausted in deep_jump.e5: the room large values take off the stack is full, 1 GiB for the whole program` |
| `deep300` | 300 frames of 128 bytes | (not run) | exit 0, 3 of 3, right |
| `deep600s` | 600 frames of 16 bytes (more than 512 frames, under 64 KiB) | (not run) | **exit 134, 3 of 3**, the same panic |
| `deep200b` | 200 frames of 512 bytes (fewer than 512 frames, over 64 KiB) | (not run) | **exit 134, 3 of 3**, the same panic |

So **both of the hook's bounds bite**, `HERO_HEAL_NOTED 512` frames and `HERO_HEAL_WORDS 8192` words (the diff's lines
29 and 46): a jump abandoning more than 512 frame records or 64 KiB of stack between the hook and the callback leaves
the callback's values held, and a loop of it aborts where it runs today. A recursive-descent parser's error escape is
this shape. **The seat's price list (§ 4: `asm_jump`, door 6, doors 8, 12, 13, Windows) omits it, and its draft
sentence (*"a jump or a switch this runtime does not see ... stays held"*) is false of it**: the jump is seen.
`qsort_jump` (`qj.h`: the callback called from inside the C library's own `qsort` through the comparator, jumping out
of `qsort`) runs right here under both, 3 of 3 (Darwin keeps frame records in its C library); its Linux cells are § B.

**A4. My other shapes beside the defect** (CL-061), three runs a cell, plain and `--sanitize`:

| shape | what C does | base | `rt-ebo6` |
|---|---|---|---|
| `below` (mine, first reading) | two coroutines carved from the extern call's own frame | 134 ×3, *compiler bug*; `--sanitize` 134, heap-use-after-free | **0 ×3**, `below: 3032`; `--sanitize` 0 ×3 |
| `own_carved` | the lane's | (the repair's: 134, `body holds 11`) | **0 ×3**, plain and `--sanitize` |
| `one` (mine: **one** coroutine on `ow_host`'s array) | one own switch, no second coroutine | **134 ×3: `body holds 10`** (7 is right), `body holds 20 coroutine 30`, then *compiler bug*; `--sanitize` heap-use-after-free | **0 ×3**: `body holds 7` twice, `coroutine 30`, plain and `--sanitize` |
| `nested` (mine, `nest.h`: inside a carved coroutine, two more carved from an array in the extern call's frame on the coroutine's stack) | coroutines in a coroutine | 134 ×3, *compiler bug* `in nested.inner`, nothing printed before; `--sanitize` heap-use-after-free | **0 ×3**: `body holds 7` ×3, `coroutines 10050 1011052` (right), plain and `--sanitize` |
| `own_spawned` (the warden's: `own_carved` on a thread `spawn_sized` started) | the carved shape off the main thread | 134 ×3, `body holds 11`, *compiler bug* (the warden's cell, again) | **0 ×3**, `carved on a spawned thread: 7`, plain and `--sanitize` |
| `copy_stale`, `copy_stale2` (mine, after the seat's `copy_jump`: coroutine 1 jumps before its own take, or coroutine 0 yields from a callee holding its value's pointer, so a stale copy of coroutine 0's pointer may lie in the abandoned span) | an attack on the owner's proof by a stale word | 134 ×3, *compiler bug* | **0 ×3, right**, plain and `--sanitize`: not fooled at these two layouts. The emitter takes every room in the prologue (`--emit-c` of `copy_stale`: four `hero_frame_take` before the first branch), so a jumper's own slots always overwrite the stale ones; whether some other layout leaves one is a question, unrun |

`one` says a single coroutine is enough for a wrong value at the base: the defect does not need two.

**A5. The seat's cross-checks, re-run** (plain, 3 of 3): `escape_carved` under `rt-ebo6` exit 134 in
`HERO_SIGNAL_REENTERED`'s words about a handler the program does not have, **on this Mac** (the seat's finding,
held); `asm_jump` exit 134 *stack exhausted* (the price, held); `copy_jump`, `lend_carved`, `coval_carved` right.

**A6. Door 13 under the route itself** (the ffi-pragmatist ran it against its no-heal copy, not against `rt-ebo6`):
`cx13` (`cx258.cpp` built `clang++ -std=c++17 -O2 -dynamiclib`, `DOOR_N=5000`), base exit 0, 3 of 3, `throws 35000
last 5`; **`rt-ebo6` exit 134, 3 of 3**, `panic: stack exhausted in cx13.e5: the room large values take off the
stack is full, 1 GiB for the whole program`.

**A7. The cause is countable at every such abort.** A copy `rt-ebo6-gate` (one `fprintf` in
`hero_frame_exhausted` printing `hero_callbacks_unfinished()`, `rt-ebo6-gate.diff`, 11 lines), one run each, this
Mac: `asm_jump` **1,261** callbacks entered and never left at the abort, `deep_jump` 2,046, `cx13` (`DOOR_N=5000`)
2,046, the blind `q5` 2,047. So the count the compiler-engineer's one-line change would read is above zero at every
abort of the price, and the clause is buildable. **And the seat's *"aborts after about 3,700 of its 5,000 jumps"*
(§ 4 and § 10 of its report, no command beside it) is not what I measure**: 1,261 callbacks were abandoned when
`asm_jump` aborted, which `frame.c`'s own head predicts (*"about 1,260 jumps of that program's 832 KB"*), so the abort
comes near the 1,262nd jump. The clause it would add, `HERO_CALLBACKS_LEFT` (`callback.c:133-135`, *"C jumped over
their frames (`longjmp`, `siglongjmp`, a signal handler's jump) or ended their thread"*), names causes `q5` and
`quit_fresh` (a coroutine left for good by `setcontext`) and `cx13` (a C++ exception) do not have: **read, not built**;
the words of that clause would need their own repair before they join the abort.

**A8. The compiler, linked with the route's runtime, compiles itself to the same C.** `HEROES_RUNTIME=rt-ebo6
./heroes-next build selfhost/main.hero -o heroes-ebo6` exit 0 (`--version`: `sources 52f86cc441b4cc08`, the runtime
being part of the key); `heroes-ebo6 build selfhost/main.hero --emit-c` and my stage 2's, both exit 0, both
40,230,893 bytes, **`cmp` prints nothing** (`selfbuild-ebo6.log`, 11:28 to 11:48). The compiler's own lends (it takes no large value: `grep -c
'hero_frame_take(' seed/heroes.c` prints 1, a string its emitter writes) run under E and B without a fault. This is the compiler running on `rt-ebo6`, which the compiler-engineer's
§ 9b did not measure: its two emissions were by one compiler built on the base runtime, with `HEROES_RUNTIME` naming
the runtime of the program emitted, which `--emit-c` never links. Not timed (load 90 and more).

## B. Linux arm64 (my container `p259-critic-1`, compilers built inside it, every cell at `3ec775b66`)

The two compilers built inside the container from the archive (10:10 to 10:24 UTC, exit 0, `sources
05186f2b38e77a97` and `sources 117362877b136f67`, the Mac's keys). Runtimes the same copies (`rt-ebo6`, `rt-e`,
`rt-ebo6-trace`, applied on this Mac to the same `runtime/` bytes, `diff -r` against the archive's `runtime/` empty).
Three runs a cell (`lx-matrix.log`, `lx-mj.log`; outputs under `lx/out/`).

| program | base | `rt-e` | `rt-ebo6` |
|---|---|---|---|
| `aj_carved` (OpenSSL 3.5.7 ASYNC, Debian's `libssl3t64`, which on arm64 imports `_longjmp`, `_setjmp`, `getcontext`, `makecontext`, `setcontext`, the ffi-pragmatist's `nm -D`) | **0 ×3**, right | **0 ×3**, right | **139 ×3**: `body holds 7 paused 1 1`, then SIGSEGV, nothing more; `--sanitize` **1 ×3, `heap-use-after-free`** (base `--sanitize` 0 ×3) |
| `mj_carved` (mine, no library) | **0 ×3**, right | **0 ×3**, right | **139 ×3**, `body holds 7`, then SIGSEGV |
| `cpu_heap` | 139 ×3 | **0 ×3**, right | **134 ×3**, the false null-pointer panic in `hero_frame_aside_unlink`; `--sanitize` 1 ×3, `heap-use-after-free` |
| `deep_jump`, `deep600s`, `deep200b` | 0 ×3 each, `sum 35000` | (not run) | **134 ×3 each**, *stack exhausted ... 1 GiB* |
| `deep300` | 0 ×3 | (not run) | 0 ×3 |
| `qsort_jump` (glibc's `qsort`) | 0 ×3 | (not run) | 0 ×3: the climb crosses glibc's `qsort` on arm64 |
| `cx13`, `DOOR_N=5000` | 0 ×3, `throws 35000` | (not run) | **134 ×3**, *stack exhausted in cx13.e5 ... 1 GiB* |
| `own_carved`, `below`, `one`, `nested`, `own_spawned` | 139 ×3 each (`nested` 134 ×3, *compiler bug*); `--sanitize` of `below`, `one`, `nested` 1 ×3, `heap-use-after-free` | (not run) | **0 ×3 each, right, plain and `--sanitize`** |
| `cpo_host`, `cpo_host_01`, `cpo_host_told`, `cpo_heap_told` | 139 ×3 each | (not run) | **0 ×3 each, right** |
| `cpo_heap`, `cpo_heap_01`, `aj_heap` | 0 ×3 each | (not run) | 0 ×3 each |

**Traced on arm64** (`rt-ebo6-trace`): `aj_carved` prints `TRACE heal-jump gives frame=0xffffe01e0d50` twice, the
blocks of `body`'s first two takes (`TRACE take frame=0xffffe01e0d50 ...`), then dies; `cpu_heap` prints `TRACE
heal-range lo=0xffffa8ecc010 hi=0xffffa8f4c010 gives frame=0xffffa8f4bfc0` four times, coroutine 0's takes, which
coroutine 1's `run` at the same frame then takes again (`TRACE take frame=0xffffa8f4bfc0 block=0xaaaac368ce90`). **Both
mechanisms of § A1, on the second platform.** On Linux arm64 `rt-ebo6` turns a correct OpenSSL binding that runs today
into a segmentation fault, which §1.12 names first.

**The blind programs on arm64** (plain, 3 of 3): base q1, q2, q3, q6, q7 `PRINTS`, q5 `ABORTS`, q4 139 with nothing
printed (the key, again), **`q4e4` 134, `body holds 11` twice, *compiler bug* (the Mac's verdict, § F1)**, `q3e4`
`PRINTS`; under `rt-ebo6` q1 to q3, q6, q7 `PRINTS`, q5 `ABORTS`, **q4 and `q4e4` `PRINTS` `body holds 7` ×3 and
`carved: 7`** (the Mac's lines), design C (`q3m`) `PRINTS`. So under `rt-ebo6` q4 would not split.

## C. The two vetoes: what is measured, and what each was measured against

**The compiler-engineer's veto of R1's heal by address.** Its premise is measured, on three platforms and by a real
library: at the base a value is handed to another frame under an own switch onto memory of the thread's stack
(`own_carved`, `coval_carved`, `lend_carved`, the seat's; `one`, `nested`, `below`, mine; `own_spawned`, the warden's;
`bfc_host`, Boost.Context 1.92, the ffi-pragmatist's), this Mac printing a wrong value or the false *compiler bug*,
Linux arm64 and x86-64 dying by SIGSEGV with nothing said (the seats' cells at `3ec775b66`, and mine, § B), and the
ffi-pragmatist's no-heal copy runs its eight corrupting cells right on all three. **What the veto offers as its built
answer is not what is measured sound**: `rt-ebo6` hands a live value to another frame in `aj_carved`, `mj_carved` and
`cpu_heap` (§ A1, § B), correct programs, the first a shipped library's documented interface, the first two right at
the base. The defect is B's two events, not E (`rt-e` runs all three right). So the synthesis holds three measured facts
at once: R1's heal corrupts; E alone corrupts nothing I or the seats ran, and leaks the two jump goldens (*stack
exhausted*, the seat's E column); E with B as built corrupts three programs E alone runs right. **No seat has built a
route that is right on all of them and keeps 2752's and 2756's repair.**

**The ffi-pragmatist's veto of a range tell (`arm-t` as drafted).** Measured, on three platforms, **against R1's
lanes**: `p259_stack_entered` calls the runtime's own `hero_*_known_stack`, the range a `makecontext` names, and under
R1 a named range puts two copied coroutines under one tag, whose thunk then heals the other's live take
(`cpo_heap_told`, `cpo_host_told`). Under `rt-ebo6` the same told programs run right, plain and `--sanitize`, 3 of 3
(§ A2), since E judges no entry by its range. So the veto's measured ground is *"a range tell over a heal that judges
by range"*; over E a tell is harmless and, as the compiler-engineer says, buys nothing a program needs. **The veto's
other half, of any heal by address for a coroutine nobody named, stands measured** (its no-heal table, and `rt-e`
here). Its *"no runtime built here makes that clause true"* is now tested (§ A2).

**The identity-tell finding.** The ffi-pragmatist's identity runtime (R1 kept, a thread-local naming the coroutine)
makes every told shape right and leaves the untold wrong (its cells); its context-identity copy (the runtime reading
the context `swapcontext` switches into) makes `cpu_heap` right with no tell, on this Mac and Linux arm64. Against
`rt-ebo6`: the untold forgetting bindings the finding is about (`bfc_host_01`, `cpo_host_01`, `cpo_host`) run right
(§ A2), so *"a binding that forgets gets today's wrong value under every tell route"* is true of tell routes over R1
and false of E; but **`cpu_heap`, the shape on which the identity is the runtime's own to read, is exactly where
`rt-ebo6` fails**, through `makecontext` called on a range a suspended coroutine still occupies. The two findings meet
there: the identity a `swapcontext` carries is the fact `hero_heal_range`'s *"a range named again starts over"* lacks.
No seat ran an E runtime that keys B's `makecontext` event by the context: **unbuilt, unrun**.

## D. Is a spec sentence owed? The disagreement, and which side is checkable

**The two positions.** The spec-warden: *"A route that makes the value keep on every platform ... owes no
sentence"*, § 3's *"Every value behaves as an independent copy"* already promising q4 its value; *"B and E alone change
only the runtime, but for the room's bound a leak can reach (q5's stack exhausted, draft 2's, adjacent)"*. The
compiler-engineer: one sentence is owed, *"about the price, not the switch"*, its draft *"What a function held when C
left its frame by a jump or a switch this runtime does not see (code of its own, a C++ exception, any jump on
Windows) stays held until the program ends, and a program that holds 1 GiB of such values aborts, saying so."*
**The warden never priced the draft**: its report ends *"Finished 2026-11-25 09:26"* (its last line, and `ls -l` reads
09:26), the compiler-engineer's file reads 10:43, its § 9b cites runs of 09:58 to 10:23, and `grep -n -i 'GiB\|price'
spec-warden.md` finds no `GiB` and two *price*s about other things (its lines 200 and 224): the leak's abort appears in
it only as the clause quoted above; no seat counted the compiler-engineer's draft.

**What I measured.** The draft, applied by a script asserting one match (after line 474's *"`main` returns, saying how
many."*, and again after line 331's *"running out of stack aborts."*), `./heroes-next measure <copy>` from my copy's
root: 34,684 bytes, claude-legacy 9,937, cl100k_base **10,115**, a vendored delta of **+61** at either place (base
10,054). **A lower bound, vendored only**; no `--refresh` (none authorised).

**The checkable part, settled by runs.** A route that keeps values and does not see every abandonment turns correct
programs that run on `main` into aborts: `asm_jump` (the compiler-engineer's, this Mac and Linux arm64), `cx13` at
5,000 C++ throws (§ A6, this Mac, under `rt-ebo6` itself; the ffi-pragmatist's no-heal copy on this Mac and arm64),
`bl6_5000`, 5,000 `__builtin_longjmp`s on x86-64 (the ffi-pragmatist's no-heal copy), and **seen** jumps from more
than 512 frames or 64 KiB below the callback (`deep_jump`, `deep600s`, `deep200b`, § A3). So the warden's *"what a
reader predicts from the spec is then what the program does, and nothing is lost"* is false of those programs under
`rt-ebo6`: no sentence of the spec today tells a reader that a loop of jumps over a callback ends (`grep -n -i -E
'GiB|gibibyte'` prints nothing; line 331's *"running out of stack aborts"* closes a bullet about a source's nesting
depth). **And the project's own rule classes them**: `.claude/rules/verification.md` § Bounded discovery lists *"a
correct program refused"* as `blocking`. Without a sentence that makes the abort the program's promised end, each of
those programs is a `blocking` defect of the route as soon as it lands; with one, it is the spec's price. **That is
the choice, and it is the synthesis's**; neither seat's report states it in those terms.

**The compiler-engineer's draft, clause by clause, against the rows**: *"a jump or a switch this runtime does not
see"*: **false as the class's edge**, `deep_jump` is seen and held (§ A3); *"stays held until the program ends"*:
false where `makecontext` names the range again (B's second event gives back, by design) and **false where B gives
back a live value** (§ A1), which no sentence can make right; *"aborts, saying so"*: **false today**, the words are
*"stack exhausted in <function>: the room large values take off the stack is full, 1 GiB"* and name no jump or
switch (§ E); *"any jump on Windows"*: **untested**, no box (read: Windows has no jump hook, § 4.3 of the shared
brief). The warden's `w1` (*"A switch of stacks C makes changes no value a function holds, wherever the stacks
lie"*): **false under `rt-ebo6`** on this Mac (`aj_carved`'s fiber switch by `_longjmp` changes `body`'s value to 21,
`cpu_heap`'s `makecontext` and `swapcontext` hand coroutine 0's value to coroutine 1, § A1), **true under `rt-e`** for
every shape run (and silent on its leaks, which are aborts, not changed values).

## E. The *"stack exhausted"* words both seats report

Both report the words; they disagree on what they are. The compiler-engineer: *"Words true of the room, silent on
the cause"*, and the abort *"should gain the gate's clause"*. The ffi-pragmatist: *"words that name a depth the
program never reached ... 2705's class again"*. The lane's draft 2 (`d-lane-coroutine-room-2.md`, read) classes the
same words at the base `adjacent`, *"a true message less exact than it could be"*. **What is checkable**: the words
are `frame.c:100-103`'s, and the function's own comment says they open as the stack guard's on purpose *"since the
stack itself may be nearly empty"* (`frame.c:85-94`, read); the clause after the colon is true (the room is full: the
count at the abort, § A7); **the cause is knowable at the abort** (1,261 to 2,047 callbacks entered and never left,
§ A7), so *"less exact than it could be"* is measured, not argued; whether *"stack exhausted in cx13.e5"* is *false*
(a recursion that never happened) or *less exact* is a reading of the words against § Bounded discovery's two lists,
and no command settles it. **Under `rt-ebo6` the words reach programs that run right today** (`asm_jump`, `cx13`,
`deep_jump`, door 6), where at the base they reached door 8 alone (`quit_fresh`, `q5`): the class is the synthesis's,
and the route moves it from a rare abort to the price's every abort. The clause the compiler-engineer would add names
causes two of those doors do not have (§ A7).

## F. The blind registration, as repaired

**It measures what it says on the two graded platforms where it now names them**, with four findings:

1. **The warden's `E4` repair removes q4's split, measured.** `q4e4` (the warden's: q4 at 32 KiB records, `diff`
   shows the `e5(` to `e4(` lines alone), at the base, 3 of 3 each: **this Mac exit 134, `body holds 11` twice then
   *compiler bug*; Linux arm64 exit 134, the same two lines and the same panic** (`lx/out/base/out-q4e4-plain-1.txt`,
   at `3ec775b66`), where q4 at `E5` is `WRONG then ABORTS` on the Mac and a silent SIGSEGV on Linux (the key, and my
   arm64 cell again). `q3e4` exit 0 on both, `order 0 1: 30 32`, `order 1 0: 30 32`. So under `now` q4 at `E4` has a
   key on both graded platforms, `WRONG`, and D1's `now` half can be graded instead of read from the adopted build's
   verdict; the warden registered the `E4` change for its stack confound, and it also repairs the split. Its x86-64
   cell is unrun.
2. **Under `rt-ebo6` the adopted key would be** (this Mac, 3 of 3 each, plain): q1, q2, q3 `PRINTS` as today, **q4
   `PRINTS` `body holds 7` three times and `carved: 7`** (and `q4e4` the same), q5 `ABORTS` (*stack exhausted*, as
   today), q6, q7 `PRINTS`, design C (`designc/q3m.hero` with the registered `design-c/sw.h`) `PRINTS`; Linux arm64 in
   § B. **But `rt-ebo6` is not a resolution that holds** (§ A1), so these are a prototype's cells, never a key to
   register.
3. **`registration.md` still says the ceiling is 15.00 USD and *"The author's to change; none given"***, while
   `llm-ergonomist.md` records the author's authorisation of 09:25, *a ceiling of 20.00 USD for the whole run*: the
   runner reads the registration, so the two files disagree on the one number that stops the run (`grep -n '15.00\|20.00'`
   of both).
4. **The warden's predictions read the rules as registered**: D1's `now` half counted as *"that same verdict, the
   adopted build's"*, `NOT PROMISED` in neither half, D2w as the rise in `NOT PROMISED`, D3 on design A at 4 of 5 (its
   table, read against `llm-ergonomist.md` § The decision rules). Its D1 rows for `arm-p` and `w1` (*fails, under
   `now` 3 already give `PRINTS`*) assume q4's adopted verdict is `PRINTS`, which is `rt-ebo6`'s Mac cell and no
   landed build's.

## G. The texts against the measured rows

**The warden would adopt no sentence** under a value-keeping route, `w1` the rival; under a refusal route, `w2`.

- **No sentence** rests on the route keeping every value: true of `rt-e` for every shape anyone ran (its price, the two
  jump goldens' *stack exhausted*, is § D's); **false of `rt-ebo6`** (§ A1, this Mac; Linux in § B).
- **`w1`**, *"A switch of stacks C makes changes no value a function holds, wherever the stacks lie"*: as no sentence;
  it names no platform, so it claims all three, and Windows is unrun by anyone.
- **`w2`** (the told refusal, route A's): **untested on every platform**, since no seat built route A's refusal (the
  compiler-engineer built its probe, `rt-probe`, which prints and refuses nothing). Read against the rows: its class
  *"a stack C carved from the thread's own ... switched to by code of its own, not by `swapcontext` onto a stack
  given to `makecontext`"* leaves `cpu_heap` (heap, `makecontext` and `swapcontext`), which corrupts at the base on
  all three platforms (the ffi-pragmatist's cells), outside it, and a reader cannot place `aj_carved` (a stack given
  to `makecontext`, carved, entered by `setcontext` and then by the C library's `_longjmp`, which is neither
  `swapcontext` nor plainly *code of its own*); and its threshold, *"a value of more than 4,096 bytes"*, omits the
  emitter's other room-dweller, *"holding a group's `partial` record by value"* (`selfhost/emit/large_value.hero:14-17`,
  read). On Linux at `3ec775b66` `unseen_carved` (a `swapcontext` onto a stack `makecontext` was given through
  `RTLD_NEXT`) is outside `w2`'s class and aborts (the warden says so: *"knowingly false on Linux"*).
- **The compiler-engineer's price sentence**: § D, clause by clause.

## H. What nobody measured (each searched in the four reports and their programs; my runs where I made them)

1. **The whole `run` form under any route.** The compiler-engineer ran `run coroutine`, the 51 cases its brief named
   (rt-ebo3, 50 and 1; the 51st under rt-ebo5); `ls tests/golden/run/*.hero | wc -l` prints **1,044**, and E moves
   every take and every lend of every program (`grep -l -E 'e5\(|E5|@=|lent|@ '` matches 451 of them, a crude
   floor). Its 51 took 27 minutes here; the whole form under the route is owed before any adoption, unrun.
2. **Route E's lock under an asynchronous jump.** Under E every take, give and lend of every program takes one
   process-wide `pthread_mutex_t` (`stack_lane.c:161-164`) and calls `malloc`; at the base the thread's room takes no
   lock. A C signal handler that jumps out while a thread holds it (2755's shape, refused today in false words)
   would leave it held and the next take would wait for ever, a hang where today there is a refusal: **a question,
   unrun** (timing-dependent; nobody wrote the shape).
3. **B inside a signal handler and on the alternate stack**: `hero_heal_jump` is written to run there (*"reads
   only"*); no seat ran a jump out of a handler over a callback under `rt-ebo6`. Unrun.
4. **Route A's refusal**: built by nobody (the compiler-engineer built the probe that finds the fact, `rt-probe`, and
   no runtime that refuses), so `w2` and every *"aborts, saying the switch was not seen"* clause have no cell.
5. **`HELD`** (step 179a's arm): ratified by the compiler-engineer and the warden **by reading**; the ffi-pragmatist
   had no shape that jumps inside a handler onto a `makecontext` stack; the historian did not search. The only runs
   are the lane's own, carried in `b0b2a975a`'s body (148 run goldens forced onto route A on both systems; *"Cost
   under 2 and 3: the two fixedbugs-2706 programs now refused"*). No seat re-ran them.
6. **Windows**: every cell of every route (no box authorised). The compiler-engineer's *"every jump on Windows"* in its
   price, the historian's Boost-at-`develop` thread-block reading, and `w1`'s unscoped claim all rest on it.
7. **Real token counts**: every text's count is vendored only (`arm-p` +32, `arm-r` +69, `arm-t` +77, `w1` +18, `w2`
   +78, `w3` +111, the compiler-engineer's price sentence +61, mine); no `--refresh` was authorised.
8. **A per-thread aside** (the compiler-engineer's *"removes the `malloc` and the scan"*): unbuilt, so E's cost is
   the prototype's, +958 instructions a lend and about +1,940 a 32 KB take at `-O2` (its `cost.sh`, not re-run by me:
   the machine was at load 90 to 116 all morning).
9. **B keyed by the context switched into** (§ C): the fact `cpu_heap` needs, unbuilt with E.
10. **A shipped library of `cpu_heap`'s design**: a shared stack over `makecontext`/`swapcontext`, copied out on yield.
    I know of one only from memory (a small C coroutine library on GitHub by the author of skynet, *cloudwu/coroutine*,
    whose resume makes a context on one shared stack after saving the suspended one): **unread here, a question for
    the historian**, not a premise. The ffi-pragmatist's `cpu_heap` is its own design.
11. **Door 12 with a large value** (a handler returning to a `getcontext` point): nobody, on any platform.
12. **`deep_jump`'s shape with a lend** (B's bounds apply to the lend asides by the same arrays, `hero_lend_aside_release_frames`): read, unrun.

## I. A repair of the briefs that is itself wrong, and the route nobody listed

**A carried fact the repair spread, and it is the one that matters most.** `00-shared.md` § 4.7 and the
ffi-pragmatist's brief quote panel 257's `asyncjob.h` head, *"libcrypto imports makecontext, swapcontext and
setcontext: nm -u"*; my first reading's F6 quoted it too, unrun, and the repair copied it. `nm -u
/opt/homebrew/opt/openssl@3/lib/libcrypto.3.dylib | grep -E 'context|jmp'` prints `__longjmp __setjmp _getcontext
_makecontext _setcontext` (OpenSSL 3.5.9, `brew list --versions`): **no `swapcontext`**; the ffi-pragmatist found it
and said so first. Its consequence is § A1: OpenSSL's fibers on this Mac and on Debian arm64 switch by the C library's
`_longjmp`, a jump the hook sees and route B reads as abandoning frames. A brief that said *"swapcontext"* pointed every
seat away from the jump.

**What would have to be true for a route that is sound and reclaims seen jumps to exist?** Every built heal judges
*which frames a switch or a jump abandoned* by an order of addresses on one range (R1 at the thunk, B at the jump
over the thread's whole measured range, B's `makecontext` event over a named range), and each is falsified by a
program that puts a live frame where the order says *gone* (`own_carved`; `aj_carved`, `mj_carved`; `cpu_heap`). The
facts nobody has built on: the identity of the coroutine that runs (the ffi-pragmatist's context-identity copy, for
`swapcontext` and `setcontext` alone), and **the save point's own identity**: route A already records which buffer a
`setjmp` saved (`sigjump_record.c`), so a jump to buffer J abandons exactly the callbacks entered after `setjmp(J)` *on
the same coroutine*, a fact of the program's order of events rather than of addresses. Whether it survives a library
that switches by `_longjmp` between coroutines (OpenSSL), where a save and its jump belong to different coroutines, is
**a question, unrun**; I name it so the synthesis does not read *"no fact exists"* into a search nobody made.

## J. Linux x86-64 (emulated, `heroes-linux:latest`, container `p259-critic-2`, at `3ec775b66`)

The compiler-engineer's route on the platform no seat ran it on (my brief's ask). Both compilers built inside the
container from the archive (11:32 to 12:24 UTC, exit 0, `sources 05186f2b38e77a97` and `sources 117362877b136f67`);
*Debian GLIBC 2.41-12+deb13u3*, *Debian clang version 22.1.8*. **Emulated**, not the CI's native leg. Plain builds,
three runs a cell (`x86-matrix.log`, `lx86/x86-matrix.sh`; outputs under `lx86/out/`):

| program | base | `rt-e` | `rt-ebo6` |
|---|---|---|---|
| `mj_carved` (no library, `_longjmp` up into carved fibers) | **0 ×3**, right | **0 ×3**, right | **139 ×3**, `body holds 7`, then SIGSEGV |
| `aj_carved` (Debian's libcrypto, which on x86-64 switches by `swapcontext`, the ffi-pragmatist's `nm -D`) | 0 ×3 | 0 ×3 | **0 ×3, right**: B's jump hook is not on the `swapcontext` path, so the OpenSSL failure is the `_longjmp` builds' (this Mac, Debian arm64) |
| `cpu_heap` | 139 ×3 | **0 ×3**, right | **134 ×3**, the false null-pointer panic |
| `deep_jump` | 0 ×3 | (not run) | **134 ×3**, *stack exhausted* |
| `deep300`, `qsort_jump` (glibc's `qsort` between the jump and its target) | 0 ×3 each | `qsort_jump` **134 ×3**, *stack exhausted in qsort_jump.e5*: E alone leaks every loop of seen jumps, B's reason to exist | 0 ×3 each: B's climb crossed glibc's `qsort` here too |
| `bl6_5000` (door 6, 5,000 `__builtin_longjmp`s over 256 KiB) | 0 ×3, `jumps 35000` | (not run) | **134 ×3**, `panic: stack exhausted in bl6_5000.e5 ... 1 GiB`: **the price on x86-64 under the route itself** (the compiler-engineer's cell, *"unrun by me"*; the ffi-pragmatist's was its no-heal copy) |
| `asm_jump` | 0 ×3 | (not run) | **134 ×3**, *stack exhausted* |
| `own_carved`, `below`, `one` | 139 ×3 each, nothing printed | (not run) | **0 ×3 each, right** |
| `q4` (the blind) | 139 ×3, nothing printed | (not run) | **0 ×3**, `body holds 7` ×3, `carved: 7` |
| `room_order`, `lendjump`, `jumpco` (panel 258's and the lane's) | 0 ×3 each | (not run) | 0 ×3 each, the same lines |
| the jump golden and the left-for-good golden (`fixedbugs-a-c-longjmp-over-a-callback-inside-a-coroutine-gives-its-room-back`, `fixedbugs-a-coroutine-left-for-good-holding-a-large-value-has-its-room-given-back-on-its-stack`) | 0 ×3, `golden=equal` both (`cmp` with `.expected`) | (not run) | 0 ×3, **`golden=equal` both** |
| `aj_heap` | 0 ×3 | (not run) | 0 ×3 |

So on x86-64 the route keeps every value of the carved shapes, repairs the two goldens B was built for, and pays
door 6, `asm_jump` and the deep seen jump as on the other two; **and it corrupts `mj_carved` and `cpu_heap` there
too**, so § A1's two mechanisms hold on all three platforms (OpenSSL's own binding only where its build switches by
`_longjmp`). B's frame-record climb was the question I brought to this image (§ H's `-fomit-frame-pointer`): it
reached its target through glibc's `qsort` (3 of 3), and I have no x86-64 cell where it does not.

## What the synthesis must settle, most important first

1. **The route the compiler-engineer built is not sound, and no built route is both sound and repairs 2752 and
   2756.** `rt-ebo6` hands a live value to another frame in three correct programs that E alone runs right: `aj_carved`
   (OpenSSL's ASYNC through its own `ASYNC_set_mem_functions`; right at the base) and `mj_carved` (its switch with no
   library; right at the base), by B's jump hook reading a `_longjmp` up into a fiber carved from the thread's stack
   as abandoning the live frames below, WRONG then a false abort on this Mac, SIGSEGV on Linux arm64 and x86-64
   (`mj_carved`; OpenSSL's own build on x86-64 switches by `swapcontext` and runs); and `cpu_heap`
   (a copying library over `makecontext` and `swapcontext`), by B's `makecontext` event treating a named range as a
   stack that starts over while a suspended coroutine's value is still in it, a false null-pointer panic on all three
   platforms (§ A1, § B, § J; traced on this Mac and arm64). The synthesis must choose, and say it chose, among **E alone** (right on every shape
   any seat or I ran; the two jump goldens and every loop of abandonment abort at the room's gibibyte), **a B proved
   by some fact other than an order of addresses** (unbuilt; § I names the candidates as questions), or R1 (vetoed,
   its corruption measured on three platforms). *"Built"* is not *"adopted"* (`/panel` § 3c): the route a veto
   compels has to be re-built and re-run on `aj_carved`, `mj_carved`, `cpu_heap` and everything in § A and § B.
2. **The price, and whether the spec owes it a sentence.** Under any route that keeps values without seeing every
   abandonment, correct programs that run today abort: `asm_jump`, `cx13` at 5,000 throws, door 6 at 5,000 on
   x86-64 (under `rt-ebo6` itself, § J), seen jumps from more than 512 frames or 64 KiB below the callback (§ A3, § A6, § B), and by reading every
   jump on Windows. `verification.md` § Bounded discovery makes *"a correct program refused"* `blocking`; so the
   choice is a sentence that makes that abort the program's promised end (the compiler-engineer's, +61 vendored, a
   lower bound, false as drafted in three clauses, § D) or no sentence and a `blocking` defect per such program the
   day the route lands (the warden's *"nothing is lost"* is false of them). Neither seat states the choice so.
3. **The abort's words.** *"stack exhausted in <function>: the room large values take off the stack is full, 1
   GiB"*: `adjacent` (draft 2, and the compiler-engineer's *"true of the room"*) or a false message, 2705's class (the
   ffi-pragmatist). Measured: the cause is countable at every such abort (1,261 to 2,047 callbacks never returned,
   § A7); the clause the compiler-engineer would add names causes `q5` and `cx13` do not have. The route moves these
   words from one door's abort to the price's every abort.
4. **The ffi-pragmatist's veto of a range tell was measured against R1's lanes, not against E** (§ C): under
   `rt-ebo6` every told shape runs right. Its veto of a heal by address for an unnamed coroutine stands measured.
5. **The blind registration**: the warden's `E4` change removes q4's split at the base (both graded platforms
   `WRONG`, § F1, § B), which lets D1's `now` half be graded; `registration.md` still says 15.00 USD where the author
   authorised 20.00; no key under `adopted` may be drawn from `rt-ebo6`.
6. **`HELD`** is ratified by two seats' reading and no seat's run (§ H 5); the synthesis records it so.
7. **Owed before any adoption, unmeasured by anyone**: the whole `run` form (1,044 cases) under the route; Windows;
   real counts; E's single mutex under an asynchronous jump; route A's refusal as a runtime.

## What the synthesis must not assert

- that E with B and the owner's proof is sound under every switch, or that *"every carved, copying, heap and lend
  shape runs right under it"* (`aj_carved`, `mj_carved`, `cpu_heap`, three platforms);
- that B's price is only *"an abandonment the runtime does not see"* (`deep_jump` and its two siblings are seen);
- that `asm_jump` aborts *"after about 3,700 of its 5,000 jumps"* (1,261 callbacks abandoned at the abort, § A7);
- that the range tell's crash was measured against a route that does not judge by range;
- that a value-keeping route owes no sentence *because a reader loses nothing*;
- that libcrypto on this Mac switches by `swapcontext` (it imports `_longjmp` and `_setjmp`);
- that the compiler-engineer's § 9b shows the compiler running on the route (§ A8 does, on this Mac);
- that a seat ran `HELD`, or that any count of any text is real rather than vendored.
- that the compiler-engineer's programs are beside the reports: they are in its own copy only
  (`.claude/worktrees/259-compiler-engineer/docs/panel/259-reports/programs/compiler-engineer/`, 53 files, `ls | wc -l`), and
  `docs/panel/259-reports/programs/` of this tree holds `ffi-pragmatist/` and my `critic/` alone; they must be copied
  before the sitting is committed, or the cells above cite files the record lacks.

My sources and scripts are in `docs/panel/259-reports/programs/critic/` (24 files: `one`, `nested` with `nest.h`,
`mj_carved` with `mj.h`, `qsort_jump` with `qj.h`, `deep_jump` and its three siblings with their headers,
`copy_stale`, `copy_stale2`, `cells.sh` and the three matrix scripts, `rt-ebo6-trace.diff`, `rt-ebo6-gate.diff`, and
`ce-price.txt`, the compiler-engineer's draft as I counted it); every output under
`.claude/worktrees/259-critic-scratch/r2/`. Both my containers are removed (`docker ps -a | grep -c p259-critic`
prints 0). Finished 2026-11-25 13:57 by this Mac's clock (`date`).
