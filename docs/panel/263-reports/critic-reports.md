# Panel 263, the completeness critic's second reading: the reports, before the synthesis

Written as it went, 2026-11-27 from about 16:26 to 17:45, a fresh agent, in the detached worktree
`.claude/worktrees/263-critic`, **moved to `main`'s tip** (`git -C .claude/worktrees/263-critic checkout --detach main`
at 16:27, `git status --short` before and after: the two untracked folders `263-briefs/` and `263-reports/` alone),
`main` = **`de57f9129`**, which holds step 214a. Scratch `.claude/worktrees/263-critic-scratch/`, `TMPDIR` there. Read,
whole and in this order: `263-briefs/critic.md`, `00-shared.md` as repaired, my first reading `critic-briefs.md`, the four
reports and the seats' programs under `programs/`, and land262's scratch (read only, nothing built or run there). No
sub-agent, no paid run, no commit, push, merge or GitHub write, no credential printed, no download, nothing outside the
repository's root, no `pkill`. **Ran** and **read** are kept apart; I give no verdict on any route.

**Compilers** (ran): `main`'s seed compiler (`clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`, `real 16.00`,
`sources 1e1eb23a1927d35b`); **`main`'s stage 2** (`real 226.99 user 167.53`, **`sources 269336480bd6aa5e`**,
`c2/stage2-main.txt`); **`main` with step 214a reversed** (`rv/`: `git archive de57f9129 runtime selfhost seed`, `patch -R`
of `git diff fc19e694c^ fc19e694c -- runtime selfhost`, one hunk of `sigjump_save.c` refused because 199p had touched it
and reversed by hand; `grep -rln "hero_unwind\|unwind_scope" runtime selfhost`: nothing; its stage 2 `real 225.24`,
**`sources 1985db22174b7688`**, `c2/stage2-rv.txt`); and my first reading's stage 2 at `f9f42fcc4` kept
(`c2/heroes-f9f-s2`, `sources 50e8403d5a885325`). Load 7 to 44 on 8 cores through the session (`uptime` at each step):
**no duration below is judged; costs are instructions retired on this Mac, and the programs' own clock in the
container** (its `/proc/loadavg` 1.18 at the end).

**Linux arm64**: one container at a time, `docker run --rm --pull never --name c263b -v <scratch>:/s
heroes-linux-arm64:latest`, twice (`bp/linux.sh`, `bp/linux2.sh`), each removed when it ended. **What it built is the
Mac stage 2's emission at `main`**, compiled in the image by the image's clang (`bp/hb2.sh`, the compiler's own words of
`selfhost/cli/flags.hero:135-155`, `-fexceptions` where the emission carries 214a's mark): `grep -c
"__APPLE__\|__linux__\|_WIN32"` of the emissions reads 0, so this is a shortcut, said; **stage 2 was not built in the
image**, so no `heroes` command and no golden ran there.

Programs, scripts and every run's file: `programs/critic/second/` beside this report (the list at its end); a path
below written `out/<name>`, `lx/runs.txt`, `lx/cost.txt`, `lx2/runs.txt` or `c2/run-2871-main.txt` is my scratch's, and
its file there is `<name>`, `linux-runs.txt`, `linux-cost.txt`, `linux2-runs.txt` and `run-2871-main.txt`; the Mac's
`csh` runs are `csh-main-mac.txt`.

## 0. `main` asked again (ran, 16:27): what moved under the seats' sentences

`git log --oneline f9f42fcc4..main -- runtime selfhost spec tests/harness seed`: **214a** (`fc19e694c`, lane unwinder,
panel 258's R2), **199p** (`e7626166a`, the signal family's case padding and the switch wrappers' note cleared in ASan's
shadow), **212g/212h** (paths), and their merges; `git diff --stat f9f42fcc4 main` over those paths: 38 files, +1,543
−29, among them `runtime/parts/unwind.c` (new, 499 lines), `callback.c`, `frame.c`, `alloc.c`, `heroes_runtime.h`,
`sigguard.c`, `rebind_jump.c`, `sigjump_save.c`, `selfhost/emit/unwind_scope.hero` (new, 371). **No `spec/`**: the spec
is `736b534c852e39af` at `main` (`shasum -a 256`). Every seat wrote on `f9f42fcc4`; the second column each owes:

- **The per-callback *before* moved.** 214a's thunk now declares a cleanup (`hero_unwound … hero_unwind_handed()`,
  the emission's thunk line) in every program that hands a C library's group a function: `cost_cb -O2`, base, **main
  3,543,611,452 instructions against 3,362,837,219 with 214a reversed** (best of five of six alternated,
  `cost-main-vs-214a-reversed.txt`), **+9 a callback, +5.4%**; the compiler-engineer's base, 3,370,579,731, is the reversed one's. Every
  per-callback percentage the seats wrote is against `f9f42fcc4`'s base.
- **214a's walk and stop compare addresses by order**, read at `runtime/parts/unwind.c:342-361` (*"`else if (sp >
  walk->target) { walk->passed = true; return _URC_NORMAL_STOP; }`"*) and `:398` (*"`(uintptr_t)_Unwind_GetCFA(ctx) >=
  me->target`"*), though its head says *"A frame is told by its own pointer, never by an order of addresses, which a
  switch of stacks can falsify"* (`:60-61`), which is true of its release (`hero_unwind_names`, `:253`) and not of its
  walk or stop. That is the model the compiler-engineer measured at 0 of 3 across a switch (§ 3 below runs it on `main`).
- **214a is under suspicion for new memory faults** (read, land262's scratch, never run there): `mac3-judge.txt`
  (16:18, its compiler `sources 7e45455736c4ec15`, at `de57f9129` by its `main-moved3.txt`) reads 5 cells of 256 not as
  promised: `n2` and `escape_heap` under `--sanitize` **ASan `SEGV on unknown address 0xfffffffffffffff8` in
  `libunwind::UnwindCursor::step`**, 3 of 3 each (`mac3/out-n2---sanitize-1.txt`); `wdr rate34` plain **`EXIT134:6
  EXIT139:5`** of 34, six of them *"panic: an array released after its last reference — a compiler bug"* and five
  `Segmentation fault`; `so malloc` plain hung 3 of 3. The same cells in `mac2-judge.txt` (at `f9f42fcc4`) read `OK`. The
  coordinator's note of 16:19 infers 214a and has sent it to lane unwinder; **I ran none of them** (§ 3 says what I ran
  of 214a instead).
- **`frame.c`'s room words are unchanged at `main`** (`:130`, `:138`, *"the room large values take off the stack is
  full, 1 GiB for the whole program"*), 214a's eight lines there being the unwind's: the warden's item 3 (the words name
  no stack) stands on `main`.
- **Step 199j's seven cases at `main`, this Mac** (ran: `./heroes-next run tests/harness/main.hero -- ./heroes-next run
  fixedbugs-2871`, `c2/run-2871-main.txt`): **`7 passed, 0 failed`**, `real 135.47`. **Linux arm64 at `main`: unrun**
  (it needs stage 2 in the image; the seats' 7 and 7 there are `f9f42fcc4`'s).

## 1. Route (b'), built and run on both platforms (the coordinator's item 1)

**What I built** (ran; `bp/cbstack2.c`, 260 lines with its notes; `bp/bprime.py`; `bp/mkrt.sh`): the compiler-engineer's
switch `hero_on_stack` (AArch64, Mach-O and ELF, its three CFI expressions) used both ways. The thunk asks its question
on C's stack as today, then `hero_callback_run` runs the function on a runtime stack (8 MiB `mmap`ed over a `PROT_NONE`
page) taken from a per-thread free list, one per callback entered from C and not returned, the thread's guard bounds
naming it while the function runs; **every call the program's code makes to a bound C function goes through
`hero_c_call`**, which, where the caller stands on such a stack, makes the call on the stack C entered the callback on,
below the thunk's own frames (64 bytes under `hero_callback_run`'s `sp`), the thread's guard bounds naming C's stack
again while C runs. A segment is found by the stack pointer and removed by identity (a coroutine C switches inside a
callback returns out of order). A jump of the setjmp family releases the segments it abandons (`hero_cb_jumped`, hooked
after `hero_rebind_jump` and `hero_jmp_jump` in a runtime copy). The emitter's side is written onto each emission by
`bprime.py`: mode `b` switches the thunks only (the compiler-engineer's `b.sh`, widened to 214a's thunk line), **mode
`bp` also routes every bound call through a wrapper per function**, mode `cc` the routing alone. Built against copies of
`main`'s runtime (`rt/main-bp`) and of the reversed one (`rt/rv-bp`); the emissions by each one's stage 2.

### 1.1 The ffi-pragmatist's veto: `csh.hero` (ran, both platforms; `out/csh-main/`, `lx/runs.txt`)

| `csh.hero`, the seat's own program | this Mac, `main` | this Mac, 214a reversed | Linux arm64, `main` |
|---|---|---|---|
| base | **`both: 10022002`**, exit 0, 3 of 3 each level | the same | the same |
| (b), thunks switched | **`task stack overflow`, 134**, 3 of 3 each level | the same | the same |
| **(b')** | **`both: 10022002`, exit 0, 3 of 3 each level** | **the same** | **the same** |

So the seat's own refusal, reproduced at the emission's level by (b), **is gone under (b')**: `csh_needstack` and
`csh_yield` run on the library's block, and two coroutines' callbacks interleave on two runtime stacks. Under
`--sanitize` base and (b') alike stop in **ASan's `stack-buffer-underflow` inside `csh_run`'s `memcpy`** (the model
copies `[sp − 1024, bp)` of a live stack, `CSH_SAY`-free run, both levels): the model's own, as the seat said of its 1 KiB
margin, and no route's.

### 1.2 The defect's own shapes (ran, both platforms; `out/canary-main.txt`, `lx/runs.txt` lines 147-176)

The counting canary of kinds 4 (`malloc`) and 6 (shape A, a static array) and the compiler-engineer's `helper.hero`
(a recursion in a helper whose address is never taken), `CO_N` 1,500 and 2,000 at `-O0`, 9,000, 10,000 and 100,000 at
`-O2`, plain and `--sanitize`; `helper` at 10,000 and 30,000: **(b') writes 0 bytes below C's stack in every counted run
on both platforms**, as (b); past 8 MiB `panic: stack exhausted in <m>.deep`, 134 (`-O0` 100,000), or ASan's
`stack-overflow`, told and uncounted; at `-O2` 100,000 the right answer (969147), where the base hangs (this Mac) or is
told after writing 196,544 bytes (Linux).

### 1.3 Shape D, the compiler-engineer's deciding run (ran; `out/dshape-main.txt`, `out/rv-mac.txt`, `lx/runs.txt`)

`dshape.hero` (theirs): the function C is handed calls `c_walk`, C recursing `n` deep with 64-byte locals.

| bytes below C's 256 KiB `malloc` stack, `-O0` | n 2,500 | n 3,000 | n 4,000 | n 6,000 |
|---|---|---|---|---|
| this Mac, base | 7,798, exit 0 | 32,449, exit 0 | **81,431, exit 0** | 114,639, exit 0 |
| this Mac, (b) | 0 | 0 | 0 | 0 |
| **this Mac, (b')** | 8,058, exit 0 | 32,548, exit 0 | **81,530, exit 0** | 77,218, then `stack exhausted in c_deep`, 134 |
| this Mac, (b') with 214a reversed | 8,050 | — | **81,522, exit 0** | exit 138, nothing said |
| Linux arm64, base / **(b')** | 0 / 0 | 8,987 / 9,070 | **41,972 / 42,056, exit 0** | 68,225 told / hang, 124 |

At `-O2` n 100,000 (b') is told after writing 139,200 bytes (this Mac) or dies 139 with nothing said (Linux), where (b)
returns the right answer. **So under (b') shape D is the base's**, give or take the 100 bytes of the thunk's record: the
recursion is `c_deep`'s, a C function of the program's own header, on C's own block, the same writes it makes when C
calls it; the spec-warden's report calls such writes *"C's"* (its § 1, *"What C's own frames do on a stack of C's is
C's"*). Whether that settles the compiler-engineer's deciding run is the synthesis's; the run says only that (b')'s count
is (c2)'s (81,432) to within 100 bytes, not (b)'s 0.

### 1.4 Three shapes of my own

**A. The alternation**, Heroes → C → Heroes … `n` deep (`d/alt.hero`, `c_again` calling the callback one level
shallower, 64 bytes of C locals), on the counting canary's 256 KiB `malloc` stack (ran; `out/alt-main.txt`,
`out/rv-mac.txt`, `lx/runs.txt`):

| bytes below C's stack | base `-O0` | **(b') `-O0`** | base `-O2` | **(b') `-O2`** | (b) |
|---|---|---|---|---|---|
| this Mac, n 1,000 | 61,920, exit 0 | **195,623, exit 0** | 0 | **10,043, exit 0** | 0, both levels |
| this Mac, n 2,000 | 199,669, then *"`alt.step` was called after C, inside a signal handler, jumped …"*, 134 | hang, 124 | 0 | **258,243, exit 0, the answer right** | 0 |
| this Mac, n 3,000 to 100,000 | the same false message, 134 | *"`alt.step` was called while a signal handler of C's that this runtime saw begin had n[ot ended]"*, 134, after 225,671 to 226,278 | 0 at 3,000; hang at 10,000 | the same false message, or 137 | 0 to 10,000; `stack exhausted`, 0 bytes, at 100,000 |
| Linux arm64, n 1,000 | 43,339, exit 0 | **201,037, exit 0** | 0 | **23,324, exit 0** | 0 |
| Linux arm64, n 2,000 to 10,000 | `stack exhausted`, told, after 181,764 | **139, nothing said** | 0 to 3,000 | **139, nothing said** | 0 |

With 214a reversed the (b') column reads the same to within 1 byte at `-O2` (n 1,000: 10,043; n 2,000: 258,242).
**Per level of the alternation, the C stack each takes** (ran: `CO_KB=32`, a 32 KiB stack, the stack plus the lowest
byte written divided by `n`, `out/bpe-and-perlevel-mac.txt`): **base `-O2` about 64 bytes** (n 1,000: lowest 31,408
below), **(b') `-O2` about 274** (n 200: lowest 22,008); `-O0` base about 342 (n 100: 1,416), (b') about 486 (n 100:
15,816). The level's C frame is the same under
both; **the rest of (b')'s 274, about three quarters, is the runtime's own frames on C's stack** (the thunk, the
question, `hero_callback_run`'s frame, two switch records, the call's wrapper). Which frame first crossed the stack's
bottom is unread (a watchpoint would say). **So the alternation, which (b) closes and the base runs at `-O2` with 0
bytes up to n 3,000, writes below C's block at exit 0 under (b'), four times sooner, on both platforms, with the
runtime's frames among the writers, and its told ends are the false handler messages of an overwritten record.** The
ffi-pragmatist's condition foresaw it (*"in it the C frames of an alternation pile up on C's unguarded block again"*).

**B. (b') with route (e) at the thunk** (ran; `rt/main-bpe`, `HERO_BP_ENTRY_CHECK`: where the stack C entered on is a
range a wrapped `makecontext` was handed or the thread's own, the entry is refused inside its lowest reserve, the
compiler-engineer's 16 KiB, a quarter under 64 KiB; `out/bpe-and-perlevel-mac.txt`, `lx/runs.txt`):

| | this Mac | Linux arm64 |
|---|---|---|
| alternation, n 1,000 to 100,000, both levels | **0 bytes, `panic: stack exhausted: C called back a function of this program with less than its reserve left on C's stack`**, 134 | the same |
| alternation, n 100 | right, 0 | right, 0 |
| `csh.hero` | right, both levels | right, both levels |
| shape D `-O0` n 4,000 / 6,000 | 81,555 at exit 0 / **`panic: a null pointer was read through, at offset 0x4e6, called from hero_callback_step`** after 112,298 and 114,639 (two runs) | 42,056 / the same false null-pointer message, `dshape.enter`, after 90,048 |

**So (e) closes the alternation and refuses n 1,000 to 3,000 at `-O2`, which the base runs right with 0 bytes below**
(Mac and Linux, the table of A): a correct program refused, because (b') puts four times the base's runtime frames on
C's stack per level. And shape D's C recursion, past C's block, is told with a false message on both platforms.

**C. One runtime stack per nested level** (ran, Linux, the alternation at n 100,000 under (b')): *"panic: more
callbacks entered from C and not returned on this thread than this prototype holds"*, my prototype's cap of 65,536
stacks, after 233,645 bytes below C's block. Under (b'), every Heroes → C → Heroes level enters from C's stack and takes a
whole runtime stack (8 MiB reserved, two mappings); under the compiler-engineer's (b) a nested entry runs where it is.
**The spec-warden's w1 prices held stacks at *"1 GiB ÷ S callbacks suspended at once"*: under (b') it is also 1 GiB ÷ S
callbacks nested at once, 128 at 8 MiB**, which the base's alternation passes at n 129 and runs to 3,000. Continuing a
nested level on its caller's runtime stack (as OCaml 5 is verified to, § 5) is unbuilt: `csh.hero` is the shape that
makes it hard, two coroutines' callbacks interleaving below one another.

### 1.5 What (b') costs (ran; `out/cost-mac.txt`, `lx/cost.txt`)

This Mac, instructions retired (`/usr/bin/time -l`), six rounds alternated base / (b) / (b') (/ door alone), the first
discarded, the best of five, every answer equal; `main`'s base:

| program | level | base | (b) | **(b')** | door alone (`cc`) |
|---|---|---|---|---|---|
| `cost_cb`, 20,000,000 callbacks (panel 259's) | `-O2` | 3,545,536,778 | +75.0% (+133 a callback) | **+75.1% (+133)** | |
| | `-O0` | 9,117,421,848 | +47.0% | +47.0% | |
| `cost_qsort`, 1,000,000 `i64` | `-O2` | 4,126,422,698 | +68.2% | +68.1% | |
| | `-O0` | 10,345,679,110 | +44.0% | +44.1% | |
| **`ccost`: 20,000,000 calls of a trivial C function inside one callback** (`d/ccost.hero`) | `-O2` | 2,958,546,577 | 0.0% | **+71.5% (+106 a C call)** | +27.3% (+40) |
| | `-O0` | 7,752,670,686 | 0.0% | +38.3% | +10.7% |

Linux arm64, the program's clock (`cost.h`'s `ns`; `ccost` by `date +%s%N`), the same rounds: `cost_cb -O2` 10.2 ns a
callback, (b) +5.6 ns, **(b') +6.9 ns**; `cost_qsort -O2` (b) +33.9%, (b') +36.3%; **`ccost -O2` (b') +7.4 ns a C call
(+58.2%)**, the door alone +2.8 ns. My per-callback +133 against the compiler-engineer's +284 is the prototype's (a free
list and one thread-local record against a scan of 16): neither is a landing's. **(b') adds a per-C-call price (b) does
not have, and a C call outside every callback still pays the door's check (+40 here).**

### 1.6 Does (b') lift every veto? (the coordinator's question; a reading of the runs above, no verdict)

- **The ffi-pragmatist's veto of (b) as stated: its ground, `csh.hero`, is gone** under (b'), both levels, both
  platforms (§ 1.1). Its own condition asked the alternation too: there (b') writes below C's block at exit 0 (§ 1.4 A),
  and with (e) refuses a correct program the base runs (§ 1.4 B), which is the seat's own veto class.
- **The spec-warden's veto** (*"any route that leaves a callback's stack unguarded with § 9 unmoved"*) **reads on the
  alternation under (b')**: C's stack is unguarded and the frames crossing it are the runtime's as well as C's. (e)
  answers it on stacks the runtime knows, not on a stack no wrapper saw, where § 9 would need the warden's w2 or w3.
  Its approval of (b) rested on *"every C frame a callback calls then runs on a stack the runtime made"*, which (b')
  reverses by construction.
- **The compiler-engineer's approval of (b)** turned on shape D (*"This is the run my verdict turns on"*): under (b')
  shape D reads 81,530 at exit 0, (c2)'s count (§ 1.3); its conditions (1) and (2) are §§ 2 and 3 below.

So **(b') lifts one veto's ground and moves the class it vetoed elsewhere**; no candidate I built lifts every veto.

## 2. The spawn doors must not switch (the coordinator's item 2; the compiler-engineer's condition, read from code by it, run here)

**A program** (ran, both platforms; `sp/spawn.hero`, `out/spawn-mac.txt`, `lx/runs.txt`): `hero_thread_spawn_sized(body:
work, …, bytes: 268435456)`, the compiler's own shape, `work` recursing `n` deep:

| | n 100,000 | n 1,000,000 | n 3,000,000 |
|---|---|---|---|
| base, both platforms | 0 / 0 (`-O0` / `-O2`) | 0 / 0 | `stack exhausted` / 0 |
| (b) and (b'), the door's thunk switched, both platforms | **`stack exhausted`, 134** / 0 | **134 / 134** | 134 / 134 |
| (b') with the door's thunk exempt (`BP_EXEMPT=h_0cb_spawn_work`) | 0 / 0 | 0 / 0 | `stack exhausted` / 0 |

**The compiler itself** (ran, this Mac): `main`'s stage 2's `--emit-c` of `selfhost/main.hero` (41,849,793 bytes, four
thunks: `main_command`, `clilink_build_beside`, `mutatecutworkers_work`, `mutateworkers_work`), built by `hb2.sh` at
`-O0` as it is (`heroes-base`) and with its four thunks switched (`heroes-b`), then `check` of a one-line sum of `n`
terms, the compiler-engineer's shape (`cc/sum<n>.hero`, `out/nest-mac.txt`):

| `check sum<n>.hero` | 400 | 600 | 1,000 | 2,000 | 13,000 | 14,000 |
|---|---|---|---|---|---|---|
| `heroes-base` | 0 | 0 | 0 | 0 | 0 | `stack exhausted in walk.expr`, 134 |
| **`heroes-b`, its doors switched** | 0 | **134** | **134** | **134** | 134 | 134 |

**So the condition is a spec fact, not a preference**: a compiler built under (b) or (b') whose own doors switch refuses
a source nested 600 deep, and **spec § 9's *"A source nested up to 2,000 deep compiles on every platform"* (`:340-341`)
is false of it** (Linux: the program's rows ran, the compiler's did not).

## 3. The unwinder across a switch, against 214a on `main` (the coordinator's item 3)

**Read**: § 0, 214a's walk stops at the first frame standing above the target by address and its stop jumps at the first
`CFA >= target`. **Ran** (both platforms; `uw/uwx.hero` with `uw/uwx.h`: 214a's golden
`unwind-a-c-jump-over-callbacks-…` with its rounds run inside a callback on the thread's own stack, a static array or a
`malloc`'d block, and `hero_runtime_live()` printed after the exit audit, which reads 0 only where every value the 2,000
jumps left was collected and released; `out/uw-mac.txt`, `lx2/runs.txt`):

| live blocks at exit, 1,000 rounds | thread's own stack | **static array (below every `mmap`)** | `malloc`'d block |
|---|---|---|---|
| 214a reversed, base and (b'), both levels, this Mac | 11,000 | 11,000 | 11,000 |
| `main`, base, both levels, both platforms | 0 | 0 | 0 |
| **`main`, (b'), both levels, both platforms** | **0** | **11,000** | **0** |

Every run exit 0 and *"C jumped over the callbacks 1000 times"*; the arrays (`UW_SAY`): this Mac `0x10496329c` (static),
`0x757d01c000` (`malloc`); Linux `0xaaaa…` (static), `0xffff…` (`malloc`). **So with my switch's CFI, 214a's unwind crosses
(b')'s two switches where C's stack lies above the runtime's, and releases nothing where it lies below**, the jump then made
as before 214a, its values left held and the audit forgiving: no fault in these runs, a leak. The compiler-engineer's
condition (2), *"R2's stop by the frame's identity"*, is unmet by 214a as landed. **And 214a's own regression on `main`**
(§ 0, n2/escape_heap/wdr) is lane unwinder's, unrun by any seat; any landing of (b) or (b') meets it there.

## 4. Step 199j's sentence: the claim a reader would get wrong, by a program (the coordinator's item 4)

**The only sentence of the spec on an abort a handler of C's causes** is § 13 `:438`: *"A callback called after C left a
handler of C's by … a `swapcontext` away from it, by a jump or a `setcontext` onto a stack once given to `makecontext`
… may abort, saying the handler was not seen to end, or that C called it from inside one."* It ties the abort to **a
callback called after** and to **two sets of words**. Run:

- **`fixedbugs-2871-a-handler-saving-into-the-context-an-interrupted-swapcontext-loads-is-refused`** (and the three other
  refused cases; ran at `main`, 7 passed): the program prints **no `answer ran for`**: it ends 134 inside the handler's
  switch, before any callback, saying *"a signal handler called `swapcontext` naming a context that the code the signal
  interrupted was itself switching from or to"* (its `.expected`). A reader of `:438` predicts either the callback's
  line or the sentence's words; neither comes.
- **`gt.hero` mode 0** (the ffi-pragmatist's, **no callback at all**; ran at `main`, `out/gt-mac.txt`, `lx2/runs.txt`):
  Heroes **134 with the stop's words, 10 of 10 at each level on this Mac and at `-O2` on Linux, 9 of 10 at `-O0` on
  Linux and once `exit 135`, nothing said**; C alone 133 or 138 (this Mac, `gt-c0 0 2` and `gt-c2 0 2`), 139 or a hang (Linux). The spec says *"No
  function of the program is a signal handler"* and *"A callback is a parameter, never … called by one"*: a reader finds
  no sentence under which this program aborts, and predicts it dies as C does. **By the letter the warden is right that
  no sentence is falsified** (*"no section lists every abort"*); **by a reader, the abort's trigger, its moment and its
  words are each predicted wrong**, and the warden's own registered row says so (*"≤ 2 [of 10] name a switch inside a
  switch as the cause (no sentence states it)"*). Which of the two governs is the synthesis's.
- **The stop does not cover every window**: Linux `-O0`, one run of ten ends `exit 135` (SIGBUS) with nothing said
  (above), where nine say the stop's words.
- **The correct discipline, run** (ran; my mode 4 in a copy of `gt.h`: the threads' first contexts made with `SIGALRM`
  blocked, as the seat's mode 3, **and each thread unblocking it once it runs**): **about 39,400 to 39,980 (this Mac)
  and 39,900 (Linux) switches from the handler in 2 s, 0 checks failed, 10 of 10 at each level, Heroes and C alone, both
  platforms.** **The seat's mode 3 never preempts**: *"signals 1, switched from the handler 0"* (this Mac: of the 40
  mode-3 runs in `gt-mac.txt`, 34 read 1 signal and 6 read 2, every one 0 switches from the handler; Linux the same), because a thread's mask keeps `SIGALRM` blocked for good, so its *"the correct
  discipline runs 40 of 40 under the stop"* tested no switch from a handler. Mode 4 is the run that shows the stop lets a
  correct preemptive scheduler through.

## 5. Every precedent the seats lean on: verified by the historian, or recalled (read)

| precedent, as a seat leans on it | seat | the historian's report |
|---|---|---|
| cgo runs the callback's Go code on a stack the runtime owns | historian, critic | **verified** (§ 1.1, `cgocall.go` lines 51-52) |
| cgo runs a C call on the system stack (`g0`), *"the shape cgo is recalled to take for C calls"* | ffi-pragmatist | **verified** that C runs on `g0` via `asmcgocall` (§ 1.1); **that `g0` on a C thread is the C caller's own stack: unverified as one sentence** (§ 1, end) |
| cgo's thunk frames on C's stack under their own bound (32 KiB below `SP`) | historian | **verified** (§ 1.6, lines 276-300) |
| OCaml 5: callbacks on a fibre, **C calls on the system stack**, later callbacks continuing on the current fibre | historian | **verified**, quoted (§ 6.7): the shape of **(b')**, not of the compiler-engineer's (b), and with nesting on one fibre, not a stack a level (§ 1.4 C) |
| GHC's `foreign export` on a new Haskell thread | historian | verified (§ 6.8); which stack its C calls use: the page is silent |
| Tencent libco's shared stack copies the live stack out and in | ffi-pragmatist, warden | **verified** (§ 7.4); the name `save_stack_buffer`: not on the page as quoted |
| **libtask's `needstack`** (half of `csh.h`'s model) | ffi-pragmatist | **not in the historian's report: recalled** |
| greenlet's copy-stack | warden | verified (§ 7.4) |
| **libaco** as a copy-stack library | warden | **not in the report: recalled** |
| POSIX leaves a handler's `swapcontext` undefined; none of the four async-signal-safe | compiler-engineer, warden (*"recalled … a question"*) | **verified** for Linux's `signal-safety(7)` and `getcontext(3)` (§ 8.2); POSIX 2004's own pages say nothing of async-signal-safety (§ 8.1); 2008's page unread |
| glibc's `swapcontext` installs the target's mask before its registers | ffi-pragmatist, 2871's commit | **not in the report: recalled** (the mode-1 race is consistent with it, not a page) |
| `perf_event_open` breakpoints of 1 to 8 bytes | compiler-engineer | verified (§ 7.3) |
| **Darwin: no user API for a watchpoint but a debugger's `thread_set_state`** | compiler-engineer, stated as a fact | **unverified either way** (§ 7.3) |
| ASan's fibre annotations | ffi-pragmatist, compiler-engineer | verified (§ 6.10), and the header says nothing of overflow detection |
| **Windows: `__chkstk` probes against the TIB's `StackLimit`; a fibre's stack has a guard page and ends in `STATUS_STACK_OVERFLOW`; (b) needs `.pdata`/`.xdata`** | warden, ffi-pragmatist, compiler-engineer | **recalled**: § 6.9 verifies `CreateFiber` assigns a stack and the thread stack's `PAGE_GUARD` mechanics, not a fibre's guard, `__chkstk` or SEH's unwind data |
| Linux's OOM killer before a told abort under w1b | warden (*"as recalled, a question"*) | not in the report |
| OpenSSL 4.1.0's default fibre, 32 KiB from `OPENSSL_malloc`, no guard | ffi-pragmatist (ran) | verified at the tag (§ 7.1), and ran by the seat on the binary |

The historian marks each verified line with its URL and at most one short quotation; I re-read no page (no web read in
this reading), so **whether any verified line says more than its page is unchecked by me**.

## 6. The seats against each other

1. **(b) or (c2)**: the compiler-engineer approves (b) and objects to (c2); the ffi-pragmatist vetoes (b) and approves
   (c2). Checkable, and checked: § 1. Neither built (b'); the historian's two verified precedents of a runtime entered
   from C (cgo, OCaml 5) are (b')'s shape.
2. **199j's sentence**: the warden *none*, the other two *owed*. Checkable by a reader's prediction, which is § 4 and
   the warden's own registered row; by the letter, nothing is falsified.
3. **The ffi-pragmatist's (c2) *"Nothing C sees moves"*** is true of C and silent on Heroes programs: the
   compiler-engineer measured (c2) refusing a correct program 300,000 deep on `main`'s own stack (its § 3.1) and moving
   the compiler's own C (7,782 functions); the ffi-pragmatist's (c2) model (`cbs.c deepc`) is C with no thread-local
   read, so it could not see the frame growth. One side ran the compiler's emission, the other a model.
4. **The ffi-pragmatist's 40 of 40** for the correct discipline tested no handler switch (§ 4); mode 4 tests it.
5. **The warden's w1 price** counts suspended callbacks; under (b') nested ones count too (§ 1.4 C). Its text is
   unmoved by it (*"a stack this runtime gave them"*); its cost is.
6. **Costs**: +284 (compiler-engineer) against +133 (mine) a callback for the same route: two prototypes; both against
   `f9f42fcc4`'s or `main`'s base (§ 0).

**The warden's texts held to the programs**: w1 *"a buffer they lent to C and a stack this runtime gave them stay held
until the program ends"* reads true of (b) and (b') in every run here (a jump's stacks are released in my prototype, so
*"until the program ends"* is the bound's ceiling, not a promise of holding). **w1's application and its count, re-run
offline** (ran: `spec-w1.diff` applied to `main`'s spec in my scratch): `shasum -a 256` **`f372f420b838493f`**, the
warden's; `./heroes-next measure`: **claude-legacy 10,380, cl100k_base 10,569**, the warden's. Its one `--refresh` and
its restore (`736b534c…` before and after) are in its copy and scratch, which my brief forbids me to read: the restore is
the warden's word; `main`'s spec reads `736b534c852e39af`.

## 7. The repair of my first reading, item by item (read, spot-checked by command)

Items 1 to 14 of `critic-briefs.md` are each answered in `00-shared.md` as its head says (§ 2.2's sweep and the
sanitizer, § 2.5, § 2.6, § 3.2's corrected negative, route (b)'s row, § 3.9, § 2.3's table and the 225 in 49 files, the
re-base on `f9f42fcc4`, § 2.7's headroom and § 3.4's ratification, the OpenSSL caveat, `timeout -k 5`, the five routes,
cgo as (b) and (c) and the box). **Numbers the repair wrote, held to its files**: § 2.2's sweep table is
`ground/f9f42fcc4/sweep-mac.txt` line for line (1,600 to 30,000 exit 0 and equal, 50,000 exit 138; `-O2` 9,600 exit 0,
9,800 and 10,000 exit 124); § 2.3's *"60 of 60"* is `where-60.txt`'s two lines of 30; § 2.6's *"directly above"* is
`adj-mac.txt` (second's start = first's end at 64 KiB to 8 MiB and the `mmap`). The ffi-pragmatist's 16 KiB row *"128
KiB apart"* against the brief's *"4 MiB"* is a layout of the run, as the seat says.

## 8. The question the sitting should have asked, and what moves the spec

**The question nobody put to the seats: on which stack does the C a callback calls run?** The two building seats split
on exactly that, each without naming it: (b) answers *the runtime's*, which breaks C that reads its own stack (`csh`);
(b') answers *C's*, which brings back the C stack's overrun in every shape whose recursion passes through C (shape D, the
alternation) and adds a stack per nested level; (c2) leaves every frame on C's stack and checks the Heroes ones. **No
route measured here is 0 bytes in all four of the canary, shape D, the alternation and `csh` without refusing a program
the base runs**: (b) fails `csh`; (b') fails the alternation, or with (e) refuses it; (c2), by the compiler-engineer's
runs, fails shape D and refuses a correct 300,000-deep program. The synthesis chooses which failure the spec states.

**Does a sentence move** (`00-shared.md` § 7): (b') holds runtime stacks as (b) does, so w1 (or w1b) is owed, now for
nested callbacks as well as suspended ones; and (b') leaves § 9 false on the alternation unless (e) lands, and false on a
stack no wrapper saw even then (w2's qualifier, the warden's text); the doors' exemption is owed whichever switching
route lands, or § 9's 2,000 is false of the compiler (§ 2). 199j's stop moves nothing by the letter and every reader's
prediction by the warden's own row (§ 4). **Either way a sentence of § 13 moves (w1), so the llm-ergonomist's blind reading
stays registered for the author to authorise.** **The warden's `## Predictions registered before the blind run` stands
whole as written**; its row *"w1's own row: 10,000 callbacks left suspended"* would read the same under (b'), and none of
its rows names a nested callback, which (b') makes the bound's commoner case.

## What the synthesis must settle

1. **Which stack C runs on when a callback calls it**, the question under the (b)/(b')/(c2) split: (b') keeps `csh.hero`
   right (3 of 3, both levels, both platforms) and gives back shape D (81,530 bytes at exit 0, this Mac; 42,056, Linux)
   and the alternation (this Mac `-O2` n 2,000: **258,243 bytes below at exit 0, base 0**; Linux `-O2` n 1,000: 23,324,
   base 0). Settled by `bp/build.sh <stage 2> <dir> <program> rt/main rt/main-bp out/<x>` then `bp/r1.sh
   out/<x>/<program>-<mode>-<level> CO_N=<n>` for `csh`, `dshape`, `alt` (`out/*-main.txt`, `lx/runs.txt`).
2. **Whether (b') carries route (e) at the thunk**: it closes the alternation (0 bytes, told, both platforms) and refuses
   n 1,000 to 3,000 at `-O2`, which the base runs right, because (b') takes about 274 bytes of C's stack a level against
   the base's 64. Settled by `bp/mkrt.sh` with `HERO_BP_ENTRY_CHECK` (`rt/main-bpe`) and `CO_KB=32 CO_N=<n> bp/r1.sh
   out/d-main/alt-<mode>-O2` (`out/bpe-and-perlevel-mac.txt`); what a landing's thunk can shrink to is unbuilt.
3. **The runtime's own spawn doors must not switch**, a spec fact: switched, the compiler refuses a sum of 600 terms and
   § 9's *"nested up to 2,000 deep compiles"* is false of it (`heroes-b check cc/sum600.hero`, 134; `heroes-base` 0 to
   13,000); `sp/spawn.hero` 1,000,000 deep refused at both levels on both platforms, exempt runs. Settled by
   `out/nest-mac.txt` and `out/spawn-mac.txt`, `lx/runs.txt`.
4. **214a's walk and stop by address order** (`unwind.c:347-351`, `:398`) against any route that switches: under (b')
   a jump over callbacks whose C stack lies below the runtime's (a static array) releases nothing, **11,000 blocks live
   at exit on both platforms**, where the base releases all. And **214a's own regression on `main`** (land262's
   `mac3-judge.txt`: `n2`, `escape_heap` sanitized SEGV in libunwind; `wdr` 6 false *"compiler bug"* and 5 SIGSEGV of
   34), lane unwinder's to repair before any route lands on it. Settled by `UW_KIND=<0|1|2> out/uw-main/uwx-<mode>-<level>`
   (`out/uw-mac.txt`, `lx2/runs.txt`) and lane unwinder's own runs.
5. **A runtime stack per nested callback under (b')** (Linux, alternation n 100,000: 65,536 stacks then refused): w1's
   bound counted by mapped bytes stops nesting at 128 levels of 8 MiB, where the base's alternation runs to 3,000;
   whether nested levels continue on one runtime stack, as OCaml 5's verified fibre does, without breaking `csh`'s
   interleaving, is unbuilt. Settled by a build of that variant run on `csh.hero` and `alt.hero`.
6. **199j's sentence**: no sentence is falsified by the letter; the trigger, moment and words are predicted wrong by a
   reader (the refused 2871 cases abort before their callback; `gt.hero` mode 0, with no callback, aborts 134 in the
   stop's words where C alone dies 133/138/139), and the warden's own registered row predicts it. Settled by
   `./heroes-next run tests/harness/main.hero -- ./heroes-next run fixedbugs-2871` (7 passed at `main`, this Mac) and
   `GT_MODE=0 gt-main-O<l>` (`out/gt-mac.txt`, `lx2/runs.txt`); the blind run, if authorised.
7. **The stop's honesty under this defect and its gaps**: Linux `-O0` `gt.hero` mode 0, one of ten `exit 135` with nothing
   said; and the compiler-engineer's loop-or-die of the stop on an overwritten record (its § 5). A correct preemptive
   scheduler (mode 4: about 39,400 to 39,980 handler switches in 2 s, 10 of 10 at each level, both platforms) runs; **the
   ffi-pragmatist's mode 3 never preempted**, so its 40 of 40 is not evidence. Settled by `GT_MODE=4` (`gt/gt.h`).
8. **Costs on `main`**: (b) and (b') +133 instructions a callback (+75% `cost_cb -O2`), Linux +5.6 and +6.9 ns; **(b')
   +106 instructions a C call made inside a callback (+71.5%), Linux +7.4 ns, and +40 for every bound call outside one**;
   and 214a moved every seat's *before* by +9 instructions a callback. Settled by `bp/costrun2.sh 6 out/cost-main:cost_cb
   out/cost-main:cost_qsort out/d-main:ccost` on a still machine, which this session never had.
9. **The precedents the routes rest on**: the historian's verified cgo and OCaml 5 are (b')'s shape (C on the system
   stack), not (b)'s; OCaml 5 nests on one fibre; libtask's `needstack`, libaco, glibc's mask order, `__chkstk`, a fibre's
   guard page, Darwin's `thread_set_state` and the OOM killer are recalled. Settled by the historian's report § 1, § 6.7,
   § 7.4 and a page for each recalled one.
10. **The spec**: w1 is owed under (b) and (b'), its price now counting nesting; § 9 needs (e) or w2 under (b'); the
    doors' exemption keeps § 9's 2,000 true of the compiler; so a sentence of § 13 moves and the blind reading stays
    registered for the author. Settled by `./heroes-next measure` of the adopted text offline, then the warden's one
    `--refresh` if the text changes from w1 (`f372f420b838493f`, 10,380 / 10,569 vendored, re-run here).

## Files beside this report (`programs/critic/second/`)

`cbstack2.c` (route (b')), `mkrt.sh`, `bprime.py`, `hb2.sh`, `build.sh`, `r1.sh`, `costrun2.sh`, `linux.sh`, `linux2.sh`;
programs `alt.hero`, `ccost.hero`, `co.h` (the compiler-engineer's counting header with `c_again` and `c_add`),
`spawn.hero`, `sp.h`, `uwx.hero`, `uwx.h`, `gt.h` (the ffi-pragmatist's with mode 4); runs `alt-main.txt`,
`dshape-main.txt`, `canary-main.txt`, `rv-mac.txt`, `uw-mac.txt`, `spawn-mac.txt`, `nest-mac.txt`, `cost-mac.txt`,
`bpe-and-perlevel-mac.txt`, `gt-mac.txt`, `run-2871-main.txt`, `linux-runs.txt`, `linux-cost.txt`, `linux2-runs.txt`.
