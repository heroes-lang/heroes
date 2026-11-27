# Panel 263: no route stops a callback overrunning a stack C allocated without a seat's veto standing, so none is adopted and the candidates go to the author; and a signal handler's switch inside the switch its signal interrupted aborts, said in § 13 once the stop's gap is repaired

**Resolution adopted: provisional — author ratification pending.** On the main question **no route is adopted**: no
route the sitting built or read lifts every seat's veto, and that is the sitting's finding (§ The sitting's finding),
with every candidate set out for the author, measured, and one ranked first. On the secondary question, step 199j's
stop, **R1 adopts the spec-warden's `spec-w5`**, landing once the stop's gap is repaired. Full panel: the
compiler-engineer, the ffi-pragmatist, the spec-warden, the historian, the completeness critic twice (on the briefs,
`critic-briefs.md`, then on the reports, `critic-reports.md`); the llm-ergonomist's blind reading registered only, its
paid run the author's to authorise (§ The blind reading). **Convened by the author** on 2026-11-27 at about 10:01, the
answer *1*, meant as: *a sitting of the soundness lane now, before the tag* (`00-shared.md`, its third paragraph), on the
defect lane sig-family-2 drafted beside step 199i (`.claude/worktrees/rebel-scratch/coord/issues/d-lane-sig-family-2-1.md`,
`blocking`, unfiled); **widened by the coordinator at about 10:33** to four seats and the critic, and the secondary
question added at about 10:45. Briefs: `docs/panel/263-briefs/` (written at `a246032bf`, repaired from the critic's first
reading 10:58 to 11:15 and re-based on `f9f42fcc4`); reports: `docs/panel/263-reports/`, each seat's programs under
`programs/<seat>/`, the critic's second reading's under `programs/critic/second/`.

**The ground.** The four seats wrote on `f9f42fcc4` (the building seats' stage 2 `sources 50e8403d5a885325`, the
warden's the same). The critic's second reading ran on `main` at **`de57f9129`**, which held **step 214a** (lane
unwinder, panel 258's R2; its stage 2 `sources 269336480bd6aa5e`), and on that tree with 214a reversed by hand (`sources
1985db22174b7688`). **`main` is now `1b7b73a0b`** (`git rev-parse main`, 17:21; committed 17:13:23), and **214a was taken
back out of it at `e0328b090`** (17:03:24, *"panel 258's R2 unwind made memory faults, its repair is not whole"*): `git
diff --stat de57f9129 1b7b73a0b -- runtime selfhost seed spec tests/harness` reads 24 files, +16 −1,169, the mirror of
214a's own `git diff --stat fc19e694c^ fc19e694c` (+1,169 −16), and `runtime/parts/unwind.c` is gone (`ls`). **So the
critic's *"214a reversed"* column is today's `main` under those paths**, and its `main` column is a tree that no longer
stands; where the two differ below, the reversed one is the one that holds. From the seats' base, `git diff --stat
f9f42fcc4 1b7b73a0b -- runtime selfhost spec tests/harness seed`: 16 files, +374 −13 (steps 199p, 212g, 212h and their
merges). **The spec did not move**: `git show 1b7b73a0b:spec/heroes-spec.md | shasum -a 256` reads `736b534c…efd3`, the
base's. **The synthesis ran nothing heavy**: the seed's compiler built from `1b7b73a0b`'s seed in
`.claude/worktrees/panel263-scratch/synth/` (`git archive 1b7b73a0b` of `seed`, `runtime`, `selfhost`, `tests/harness`,
`.github`, `spec`, `vendor`, `ROADMAP.md`, `CLAUDE.md` and `docs/roadmap`; `clang -I runtime seed/heroes.c
runtime/runtime.c -o heroes`, `real 15.12 user 14.78`, load about 6, `heroes 0.6.0`, `sources 1e1eb23a1927d35b`), used for
`heroes measure` of one text (R1) and for `issues.hero draft` of the drafts. **Every program outcome below is a seat's or
the critic's, cited, never the synthesis's.** One command of the synthesis wrote a `git diff`'s output to `/tmp/x`,
outside the repository's root, against its brief; it was removed at once (`rm -f /tmp/x`, then `ls`: no such file). No duration below is judged: the seats ran at loads of 29 to 93 on 8 cores,
and every cost is instructions retired on this Mac or a program's own clock in the container, as each seat says.

## The proposal, as put to the seats

`00-shared.md` § 1: a C library runs a coroutine on a stack it took from `malloc` and handed to `makecontext`, nothing of
no access below it; a Heroes function C calls back on it recurses past the stack's lowest byte; nothing faults there, and
the recursion writes on through whatever lies below as far as memory stays mapped, this thread's thread-local block first
on this Mac (the runtime's own words among them), then the allocator's region. Spec § 9 says *"running out of stack
aborts"* (panel 232's R4) and design.md §1.12 says a program *"must not corrupt memory"*: both false here. **Which
mechanism stops the recursion before the first byte below the stack, on every stack a callback can run on, at what cost
per callback and per call, and with what to lane unwinder's R2 and the signal guard of steps 199a to 199o?** Routes (a)
to (q) (`00-shared.md` § 4), each a question; and, added at about 10:45, **does step 199j's stop (`hero_signal_switch_begin`,
`sigguard.c:600-622`, issue 2871) owe a sentence of the spec?**

## What the sitting found before judging, and beside it

- **The defect at its widest is a right answer at exit 0.** `malloc.hero` at `-O0` returns the answer `thread.hero`
  gives at every `CO_N` from 1,600 to 30,000, having written about 44 KiB to **5.24 MiB** below its stack (the repair's
  sweep, `ground/f9f42fcc4/sweep-mac.txt`, the critic's first reading); at `-O2` 9,600 the same. At the default `n` the
  end is a fact of the build and the run: a hang, the false *"ran on a thread this program did not start"*, exit 138 with
  nothing said, or `stack exhausted` after the writes (`00-shared.md` § 2.1). Linux arm64 tells `malloc.hero` only
  because glibc gave the 256 KiB block a mapping of its own with nothing below it in those runs; one more mapping lands
  directly below (the ffi-pragmatist, 3 of 3, its § 16), and a 64 KiB block from the heap is written through the heap's
  earlier chunks, 23,484 bytes at exit 0 (the compiler-engineer, § 9).
- **`--sanitize` is no instrument of these writes**: exit 0 and the right answer where the canary counts up to 232,195
  bytes below (`00-shared.md` § 2.2). Every *0 bytes* below is a count: the compiler-engineer's counting canary, which
  also counts at the runtime's `raise` and `abort` (its § 1.1; the clerk's canary could not count a run a route stops).
- **The defect is C's own before it is Heroes'.** OpenSSL's default ASYNC fibre is **32,768 bytes from `malloc` with
  writable memory below it** on libcrypto 3.5.9 and 4.1.0 on this Mac, and a heap chunk on 3.5.7 in the image, read from
  the run (the ffi-pragmatist, § 1; the historian verified the source at the `openssl-4.1.0` tag, its § 7.1). A job in C
  alone recursing 1,000 deep returns right at exit 0 having changed 33,343 bytes below the fibre (4.1.0); a Heroes job
  at `-O2` `CO_N` 20,000 returns right at exit 0 having changed **243,318** (§ 3); on Linux the same job a few KiB past
  dies **exit 135, nothing said** (§ 16). CPython 3.13 to 3.15, embedded on a 256 KiB `malloc`'d coroutine, dies 139 or
  138 with nothing said (§ 5).
- **A stack in the image's own data** (shape A) hangs this Mac and dies 139 on Linux, nothing said; below it lie the
  process-wide words, the handlers the guard chains to (`hero_stack_prev_bus`, `_segv`, 4,272 and 4,288 bytes below)
  and the stop's latch `hero_signal_stopping` (220,528 below) (`00-shared.md` § 2.5).
- **The false thread message is the guard reading its own return address** (the compiler-engineer, § 4 (i), lldb
  watchpoints): `hero_thread_is_home`, 49,111 bytes below `lo`, is first written by `hero_thread_guard`'s own prologue
  (`stp x29, x30`), its byte being byte 1 of the saved return address, `0x23`; the guard then compares it with 1 and
  refuses. **The hang is two signals in the runtime's trampoline**: the run-out's `SIGBUS` enters `hero_rebind_tramp`
  for `hero_stack_handler`, `hero_rebind_relived` (`rebind_entries.c:722`) faults on the overwritten record, and the
  `SIGSEGV` enters the trampoline nested (`-O0`, `lldb/tramp-static-O0.txt`); at `-O2` lldb's `continue` never returned.
  Both are drafted apart (§ Found beside).
- **Under this defect step 199j's stop neither fires nor holds**: with the record `hero_signal_stack` written and the
  guard's word not (`CO_N` 9,680 to 9,720 at `-O2`), a handler's switch hangs (124, 3 runs) or dies 138 (3 runs), nothing
  said (the compiler-engineer, § 5): **the stop's honesty rests on words this defect writes.**
- **The compiler hands C a function** (`hero_thread_spawn_sized`, `selfhost/main.hero:88`), so it opens the callback
  question, `callback_guard.hero:28-30`'s *"the compiler itself ... guards nobody"* is stale, and eleven examples declare
  `hero_thread_spawn` (the compiler-engineer, §§ 7, 10). **A switching route's spawn doors must not switch, and that is a
  spec fact** (the critic, § 2): the compiler with its four thunks switched refuses `check` of a one-line sum of 600
  terms, where it checks 13,000 unswitched, so § 9's *"A source nested up to 2,000 deep compiles on every platform"*
  (`:340-341`) is false of it; a spawned body 1,000,000 deep is refused on both platforms switched and runs exempt.
- **Panel 262's verdict says *"`so` by step 199i"*, and that is true of half of `so`** (the spec-warden, its first
  finding): 262's `so` was a 256 KiB stack *"from `malloc` or `mmap` with a guard page"*; 199i's four goldens are all on
  the guarded `mmap` header, and the `malloc` half is this defect, open. 262's R2(a) and the compiler-engineer's 262 veto
  name `so` (§ For the coordinator).
- **214a's walk and stop compare addresses by order** (`unwind.c:342-361`, `:398`, read by the critic at `de57f9129`)
  though its head says a frame is told by its own pointer (`:60-61`); across a switch where C's stack lies below the
  runtime's, its stop releases nothing (§ The candidates, (b')). 214a has since left `main`.
- **(i) is not a route, (p) and (d) are none, (j) is unavailable**: the page below a `malloc`'d block was never free on
  this Mac, 30 of 30 at every size from 16 KiB to 8 MiB, and readable on Linux 30 of 30 (the ffi-pragmatist, §§ 7, 16);
  lldb sets four 8-byte write watchpoints on this Mac and refuses the fifth (the compiler-engineer, § 8); no arrangement
  of `sigaltstack` raises a signal at an ordinary store (the ffi-pragmatist, § 9, read); both arm64 clangs emit no
  `__morestack` (`00-shared.md` § 3.8; GCC's page says x86 GNU/Linux only, the historian § 4.1).
- **Panel 104's ground for refusing a check per call has nothing to forbid today**: the base keeps `down` a call at
  `-O2` (`bl _h_costrec_down`, held by the overflow check after it and `-fno-optimize-sibling-calls`); what a check does
  instead is **double a small function's frame on both platforms** (16 to 32 bytes; the compiler-engineer, §§ 3, 11).
- **The precedents the historian verified** (each with its URL and day in `historian.md`): **cgo** runs the callback's Go
  code on a goroutine stack the runtime owns and a Go call into C on the system stack, `g0`; its thunk's frames stay on
  the C stack under a bound of their own, an assumed 32 KiB below `SP` (§§ 1.1, 1.6); caching a C thread's stack bounds
  broke on a C coroutine library in Go 1.21 (issue 62440), and refreshing them at every callback cost up to 96 µs a
  callback until CL 600296 (issue 68587). **OCaml 5** runs callbacks on a heap fibre checked at each function's entry,
  every external call on the system stack, and later callbacks on the current fibre (§ 6.7). **CPython 3.14** checks
  nothing off its known stack (§ 6.4); **HotSpot** places its guard zones inside an attached native thread's stack (§ 6.5).
  No precedent was found of (f), of (h), or of a runtime wrapping a foreign library's `free` (§ 10). **The historian's
  fetch tool cached the arXiv PDF of the OCaml paper outside the repository's root** (its report's head; it read the HTML
  instead): the author is owed that sentence (CLAUDE.md § Hard stops), and the file is the coordinator's to remove.
- **Step 199j's stop as it stands**: its seven cases pass at four builds each on both platforms (the compiler-engineer and
  the ffi-pragmatist at `f9f42fcc4`; the critic on this Mac at `de57f9129`); the four Linux switch-family cases its commit
  body calls red on its base **are not red in `fixedbugs-27` and `fixedbugs-28` at `f9f42fcc4`** (134 passed, 0 failed,
  the compiler-engineer, § 9), so they stay unnamed, and the ffi-pragmatist did not run them (its § 10).

## Verdicts

| judge | verdict | section | cost named | prediction | condition |
|---|---|---|---|---|---|
| compiler-engineer | **veto (c1)** on soundness (`helper.hero`: 57,921 bytes below at exit 0 on this Mac at `-O2`, 43,441 on Linux); **object** to (c2) (*"sound for Heroes frames; not the most robust route"*), (c3), (e), (n); **approve (b) with seven conditions**; (p), (k), (m) ruled out; (l) and (q) owed beside (c2), not beside (b); **199j's stop approved as it stands, a sentence of § 13 owed** | design.md §1.12, §4.17 | (c2): `selfhost` +93 −16, runtime +81, +17 instructions a callback (+10.1%), +5 to +20 a call (+41% to +161%); (b): runtime 98 lines, the thunk about 30, +284 a callback (+168%), 0 a call | (c1): a nonzero count on `helper.hero` at `-O2` n 10,000 on both platforms; (c2): `dshape.hero` at `-O0` n 4,000 above 40,000 bytes on both; (b): 0 bytes everywhere at its landing; 2871: 7 passed | (b)'s seven, each a run at the landing, any one red turning it to (c2) with (l) and (q) |
| ffi-pragmatist | **veto (a1) to (a4), (f), (g), (h)**, and **(b) as stated** (`csh.hero`: right today, `task stack overflow` 134 under (b)); **approve (c2) with (n), conditioned**; **(o) approved as a binding's practice**, not the runtime's route; (i) not a route; (d) none; **199j's stop: a sentence owed** | design.md §1.12 (*"any C library must be bindable"*), §1.11, §4.19 | (b): +4.4 to +8.2 ns a callback against today's 12; (g)'s probe 1.5 to 2.9 µs a callback | libco in shared-stack mode, its coroutine yielding inside a callback, right today and aborting or crashing under (b); under (c2) `csh.hero` right and `ofib.hero` `CO_N` 20,000 told with 0 bytes below | **(b) rebuilt with every C call from a callback made on the C stack it was entered on, run on `csh.hero` and the unbounded alternation**; (c2)'s three: a two-sided check, a reserve holding the deepest C between two entries (16 KiB here, 20 KiB on Linux), a stack no wrapper saw stated in § 9 |
| spec-warden | **approve (b) with `spec-w1`**; object to (a), (a4), (c), (e), (n), (h) alone (each needs w2, (h) w6), to (l), (q), (m) alone (only w3 is true), to (f) (w4); **veto any route that leaves a callback's stack unguarded with § 9 unmoved**; **no sentence for 199j's stop** | design.md §1.6, §1.2, §1.12; `spec-shape.md` § Where a rule lives | w1 **+10 real** (13,849 → 13,859, counted 2026-11-27), +6 vendored; every other text a lower bound, vendored only (w1b +20, w2 +25, w3 +33, w7 +34, w4 +39, w6 +48, w5 +52) | w1 lands at +10 ± 4 real; with (b), `malloc.hero` and the canary 0 bytes on both platforms | (b)'s thunk writing only a bounded frame before its switch; shape C broken by (b) turns its approval into an objection unless w7 lands or the route leaves such stacks alone; a correct program 199j's stop refuses makes w5 owed |
| historian (advisory) | **approve (b) with (c) together** as the route history took where a runtime is entered from C; object to (k) and (j) as the whole of any resolution | precedent | none | none | a runtime that stops such a recursion on a C library's `malloc`'d stack with no stack of its own and no protection in C's memory; HotSpot's or Rust's guard shown breaking on memory C later freed; a shipped precedent of (h) or (a4) |

The completeness critic gives no verdict. Its second reading built the route the coordinator asked of it, **(b')** (the
callback's frames on a runtime stack, every bound call it makes back on C's stack), ran it with (e) at the thunk, ran the
spawn doors, the unwinder across a switch and step 199j's stop on both platforms, and held the four reports against each
other (`critic-reports.md`, its ten items under *What the synthesis must settle*).

## The sitting's finding: no route lifts every veto

**Three ways exist to stop a write before the first byte below a stack C allocated with nothing of no access below it,
and each one is a seat's veto, measured.**

1. **A page of no access inside memory C owns**, (a1) to (a4): correct C faults in it, on both platforms, whenever C
   goes on owning its block after the coroutine ends (`pool`, `paint`, `carved`, `static` fault, `read` gets `EFAULT` on
   this Mac and a short read of 4,080 of 262,144 bytes on Linux, (a4) repairing only the three that pass through the
   allocator; `ra.c`, the ffi-pragmatist's § 2); in the runtime on Linux, 4 of the tree's correct `run` goldens go from
   exit 0 to 139 and 3 told ends go silent (`a1-count-linux.txt`); a page is 16 KiB here, half of OpenSSL's 32 KiB
   fibre, and a correct job using 15 to 26 KiB of it faults under (a2), 3 of 3 on both releases (§ 6). **The
   ffi-pragmatist's veto.**
2. **Every frame a callback makes moved off C's stack, its C calls included**, (b): 0 bytes below C's stack in every
   shape anyone ran; but **C that measures or copies its own stack from the stack pointer then runs on a stack that is
   not its block**: `csh.hero` (a shared-stack coroutine library modelled on libco's shared mode, and libtask's
   `needstack`) is right 3 of 3 at both levels today and ends `task stack overflow`, 134, 3 of 3 under (b), on both
   platforms (the ffi-pragmatist, §§ 5, 16; re-run by the critic, its § 1.1); in C alone the copy computes a 500 GB
   (this Mac) or 263 KB (Linux) stack and dies 139 or 138. **The ffi-pragmatist's veto**, on §1.12's *"any C library
   must be bindable"*.
3. **C's calls left on C's stack**, every other route: C's stack has no guard, so the frames on it can cross its bottom.
   **Shape D**, C's own recursion under a callback, writes 81,424 bytes below at exit 0 on the base, 81,432 under (c2),
   81,530 under (b') at `-O0` n 4,000 on this Mac (42,056 on Linux): the same writes whatever the route. **The
   alternation**, Heroes → C → Heroes …, is where the route itself counts: (b') puts the runtime's frames on C's stack
   at each level, **about 274 bytes a level against the base's 64** at `-O2`, and writes **258,243 bytes below at exit 0
   at `-O2` n 2,000 where the base writes 0**, both platforms (the critic, § 1.4 A). The only stop short of 1 or 2 is a
   **check with a reserve**, which refuses what fits in the reserve: (b')+(e) refuses the alternation at `-O2` n 1,000 to
   3,000, which the base runs right with 0 bytes below (§ 1.4 B); the check (c2) puts at each entry refuses a 16 KiB
   stack's last 4 KiB and a 256 KiB `-O0` recursion of 1,200 whose frames it grew (run under (c1)'s set, the same check)
   and, under (c2) itself, **a correct recursion 300,000 deep on `main`'s own stack**, by the frame's doubling (the
   compiler-engineer, §§ 1.2, 3.1). And nothing at all stops C's own recursion
   (shape D) but 1 or 2. **§ 9 then says either what is false (the spec-warden's veto) or that a program may write over
   memory it does not own, which design.md §1.12 refuses** (the warden's objection to w2 and w3).

So **each candidate's failure is the price of the horn it took, and every horn holds a seat's veto or the class a veto
names**: the critic's *"No route measured here is 0 bytes in all four of the canary, shape D, the alternation and `csh`
without refusing a program the base runs"* (its § 8), held to every run above. **No route is adopted.** The defect stays
what its draft says, `blocking`, and by `.claude/rules/verification.md` § Bounded discovery the milestone's tag waits for
it: this sitting answers the author's *before the tag* with the choice, not with a route.

**Each seat's veto against each candidate**, as cast and as its class reads on the runs (a veto cast by a seat is
**bold**; a class held to a run the seat did not see is marked so):

| candidate | ffi-pragmatist | compiler-engineer | spec-warden |
|---|---|---|---|
| (b) | **stands** (`csh.hero`) | approves, seven conditions owed | approved with w1; `csh` is the shape C its condition named, which turns its approval into an objection unless w7 lands |
| (b') | its ground gone (`csh` right); its own condition's second half, the alternation, **fails**, in its class (bytes below a stack in a binding the route claims) | its veto's class: bytes below a stack in a shape the route claims (the alternation, worse than the base) | its veto's class: a callback's stack unguarded with § 9 unmoved; its approval of (b) rested on C's frames on the runtime's stack, which (b') reverses |
| (b')+(e) | its class: a correct program refused (the alternation at n 1,000 to 3,000), a run the seat did not see | shape D left; a false message at D's n 6,000 (§4.17) | § 9 needs w2 for a stack no wrapper saw |
| (c2) | approves; its condition (2), a reserve of 16 KiB here and 20 KiB on Linux, on OpenSSL's 32 KiB fibre refuses a job using more than 12 to 16 KiB, the ground of its own (a2) veto (an inference from `cdepth.c`'s and `ofib`'s sizes, unrun as a (c2) run); and the 300,000-deep refusal, a run its C model could not see | objects: not the most robust (shape D, a stack no wrapper saw, the frame's doubling) | **veto with § 9 unmoved**; with w2 and a word for C called from a callback, an objection (§1.12) |

## The candidates, for the author (measured; *unrun* where it is)

Each row: what it closes, what it leaves, what it refuses, what it costs, and what it owes, with whether anybody ran it.
Costs are against the base the seat measured (`f9f42fcc4`'s, or the critic's `main` with 214a, whose per-callback base
was 9 instructions, 5.4%, above today's; § The ground).

**(b), every frame of a callback on a stack the runtime owns, its C calls included** (the compiler-engineer's prototype,
`rt-b/parts/cbstack.c`; the critic's re-build on `main`).
- *Closes*: the canary (kinds 4 and 6, 16 KiB to 1 MiB, `CO_N` to 10,000,000), `helper.hero`, the heap stack, shape A,
  **shape D** and **the alternation**, both platforms, both levels, plain and `--sanitize`: **0 bytes below, every
  counted run** (the compiler-engineer, §§ 2, 6, 9: 35 counted runs on Linux; the critic, §§ 1.2, 1.4; the
  ffi-pragmatist's `cbs.c`, § 4); a recursion that fits 8 MiB now **returns the right answer** where the base wrote
  (`-O2` n 9,000 to 100,000). A stack no wrapper saw is closed by construction (read: (b) needs to have seen no stack;
  unrun on a real one, boost.context or libco's own switch).
- *Leaves*: the thunk's question and its own frames on C's stack, at most 768 bytes (this Mac) and 512 (Linux), measured.
- *Refuses*: **C that measures or copies its own stack from the stack pointer inside a callback** (`csh.hero`, both
  platforms; the veto); a conservative collector scanning its thread's stack from a callback would scan the wrong range:
  **an inference, unrun**; a layout where the runtime's stack lies just below C's block would make a copy-stack
  library's extent plausible and its restore a write over what lies between: **an inference, unrun** (every measured
  run was loud: the library's own abort, or a crash).
- *Costs*: **+133 (the critic, `main`) to +284 (the compiler-engineer) instructions a callback**, +75% to +168%
  `cost_cb -O2`, +68% to +151% `qsort`; Linux +5.6 ns a callback (the critic's prototype), +194% to +206% `cost_cb`
  (the compiler-engineer's, its scan of 16 slots), +4.4 ns (the ffi-pragmatist's C model); **0 a call** (+0.06%); runtime 98 lines and a thunk of about 30, the x86-64 trampoline about 25 more (unwritten).
- *Holds*: one runtime stack per callback suspended or abandoned (100 held for 100 paused, 0 once resumed; 100 on a jump
  the hook is not told of, 0 when it is; the ffi-pragmatist, § 4); Linux's `vm.max_map_count` 262,144 in the image
  bounds them near 131,000 (the compiler-engineer). **`spec-w1` is owed** (+10 real, measured), and w7 if `csh`'s class
  is to be said (+34 vendored, a lower bound).
- *Owes* (the compiler-engineer's seven conditions; whether run): (1) **the spawn doors do not switch**: run by the critic
  (§ 2): switched, the compiler refuses a 600-term sum and § 9's 2,000 is false; exempt, it runs; (2) **CFI on every
  switch and R2's stop by the frame's identity**: run in `fu.c` (3 of 3 slots only with both, 0 of 3 by address where
  C's stack lies below the runtime's, both platforms); 214a as landed stopped by address and has left `main`, and lane
  unwinder's 214b and 214c (`git log lane-unwinder`, subjects only) are unread by the synthesis; (3) **`fixedbugs-26`,
  `-27`, `-28` (175 cases) on (b)'s runtime at four levels, both platforms: unrun by anyone** (it needs a compiler
  emitting (b)'s thunks); (4) a jump out releases its stack: run (the ffi-pragmatist's hook, 0 held); (5) **ASan's fibre
  annotations**: run: no false report unannotated but on a jump out (this Mac `CHECK failed`, 134), and annotated, a
  false `stack-buffer-overflow` on that jump (this Mac), so the jump hook owes the annotation's end; unwritten; (6) the
  bound on held stacks and its sentence: w1 priced, the count of suspended callbacks a correct program reaches unrun; (7)
  the per-callback cost brought under the prototype's: two prototypes, +133 and +284; a landing's unbuilt. **Windows**:
  a trampoline whose unwind data (`.pdata`/`.xdata`) describes the switch and the thread information block's stack
  limits moved at it, **recalled, not read** (the historian verifies `CreateFiber`'s stack and `PAGE_GUARD`, not
  `__chkstk` or SEH's unwind data; the critic, § 5).

**(b'), the callback's frames on a runtime stack, every bound call it makes back on C's stack** (the critic's,
`cbstack2.c`, `bprime.py`).
- *Closes*: **`csh.hero` right**, 3 of 3 each level, both platforms, with and without 214a (§ 1.1); the canary kinds 4
  and 6 and `helper.hero`, 0 bytes, both platforms (§ 1.2).
- *Leaves*: **shape D, the base's** (81,530 bytes at exit 0 here at `-O0` n 4,000, 81,522 with 214a reversed; 42,056 on
  Linux); **the alternation worse than the base**: this Mac `-O2` n 1,000 10,043 and n 2,000 **258,243 bytes below at
  exit 0** (base 0 and 0; with 214a reversed 10,043 and 258,242), `-O0` n 1,000 195,623 (base 61,920); Linux `-O2` n 1,000
  23,324 (base 0) and n 2,000 to 10,000 **139, nothing said**; its told ends the false handler messages of an overwritten
  record. About three quarters of its 274 bytes a level are the runtime's own frames (the thunk, the question,
  `hero_callback_run`, two switch records, the call's wrapper).
- *Refuses*: one runtime stack per nested level: the prototype's cap of 65,536 hit at the alternation's n 100,000 on
  Linux; **under w1's bound by mapped bytes, 128 levels of 8 MiB**, where the base runs the alternation to 3,000.
- *Costs*: +133 instructions a callback (+75.1%), and **+106 a C call made inside a callback (+71.5% `ccost -O2`), +40 for
  every bound call outside one** (the door alone +27.3%); Linux +6.9 ns a callback, +7.4 ns a C call, +2.8 ns the door.
- *Owes*: (b)'s (1), (2), (3), (5); and across a switch, with C's stack below the runtime's (a static array), **214a's
  unwind released nothing, 11,000 blocks live at exit on both platforms** (the critic, § 3; drafted, § Found beside).

**(b')+(e), the same with a probe at the thunk against a reserve on a known range** (the critic's `rt/main-bpe`).
- *Closes*: the alternation, **0 bytes, told** (*"stack exhausted: C called back a function of this program with less
  than its reserve left on C's stack"*), both platforms; `csh.hero` right.
- *Leaves*: shape D at n 4,000, 81,555 bytes at exit 0; at n 6,000 **a false message**, *"a null pointer was read
  through, at offset 0x4e6, called from `hero_callback_step`"*, after 112,298 and 114,639 bytes (this Mac), 90,048 (Linux).
- *Refuses*: **the alternation at `-O2` n 1,000 to 3,000, which the base runs right with 0 bytes below**, because (b')
  takes four times the base's C stack a level; what a landing's thunk can shrink to is unbuilt.

**(c2), a check of the stack pointer at each entry of every function a callback reaches, against a window kept per
thread** (the compiler-engineer's, built in the compiler: `heroes-c2`, `sources 3b93a6590f98b9d9`; `selfhost-c2.diff`).
- *Closes*: every shape whose writers are the program's own frames, on every stack the runtime saw made: `malloc`,
  `guarded`, `bare`, `thread`, **shape A**, the canary kinds 4 and 6 at 16 KiB to 1 MiB, `helper.hero`'s helper, by `CO_N`,
  both levels, plain and `--sanitize`, **both platforms: 0 bytes, told** (§§ 1.2, 1.5, 9); a 64 KiB heap stack on Linux
  (§ 9). Nothing C sees moves (the ffi-pragmatist's `cbs.c deepc`, a model).
- *Leaves*: **shape D, the base's** (81,432 bytes here, 41,957 on Linux); **every stack no wrapper saw** (the window
  goes wide there: the defect whole on boost.context's `fcontext`, libco's `coctx_swap`, hand assembly; read); the window
  word is a `_Thread_local`, in the block C's frames reach in shape D, so a later check may pass falsely (**an
  inference, unrun**); the alternation under (c2): **unrun** (its entries are checked, so told is the prediction).
- *Refuses*: **correct programs the base runs**: a recursion 300,000 to 500,000 deep on `main`'s own stack (this Mac,
  `-O2`: the check doubles `down`'s frame, 16 to 32 bytes; Linux's frame grows the same, read in the disassembly, its
  refusal unrun); a 16 KiB stack's last quarter (`-O2` n 500); `-O0` recursions whose frame the check grows (192 to 224
  bytes); on OpenSSL's fibre, the reserve condition (2) above (an inference).
- *Costs*: **+17 instructions a callback (+10.1% `cost_cb`, +9.0% `qsort`), +5 a call of a small function (+41%), +20 a
  call of panel 104's recursion (+161%)**; Linux +6.2% and +4.9% a callback, per call within the clock's noise; the
  parallel examples +3.2% (`nqueens`) and +13.2% (`mandelbrot`); **the compiler's own C moves** (7,782 functions checked,
  its binary +9.4%), and it is not slowed: `check selfhost/main.hero` 13% and the warm build 16% fewer instructions
  retired, a side effect of how clang lays out a large `-O0` frame (§§ 12, 13); 44 examples byte-identical but the
  `hero_program_sources` line, 12 programs moved (the compiler and the 11 that declare `hero_thread_spawn`).
- *Owes*: (l) and (q), the runtime's words out of the recursion's reach, for the honesty of what C's frames still write
  (read, not built: 225 `_Thread_local` words in 49 files); a § 9 sentence for C's frames and for a stack no wrapper saw
  (w2 with a word for C called from a callback: unwritten, uncounted; w2 alone +25 vendored); the switch wrappers' reset
  of a wide window (about 4 lines, unwritten).

**Routes no seat would adopt alone**: (c1) (vetoed), (c3) (nothing (c2) does not), (e) and (n) alone ((c)'s holes),
(f) (26 of the 55 `run` goldens that run today red, carved stacks among them; vetoed), (g) ((b) on (f)'s set, and its
probe 1.5 to 2.9 µs a callback, 0.6 to 1.3 on Linux; vetoed), (h) (`csh` and `needstack` break for every coroutine;
vetoed), (i), (j), (k), (m), (p), (d) (above), (l) and (q) alone (they change the words, never the writes).

**(o), the runtime's guarded allocator handed through a library's own door**, is **approved as a binding's practice,
not as the runtime's route** (the ffi-pragmatist): OpenSSL's `ASYNC_set_mem_functions` with 256 KiB `mmap`ed over a page
of no access turns the job's exit 0 with 243,318 bytes below into `panic: stack exhausted` at every size, every build,
both platforms (§§ 3, 16). No binding of Rebel's library calls `ASYNC_start_job` or `ASYNC_set_mem_functions` today
(`grep -rln` over `rebel/library`, empty), so it binds nothing yet; it is recorded for the binding that will.

**The route nobody built, and what would have to be true for it to lift every veto, (b'')**: the program's frames on a
guarded runtime stack (b), every C call on the C stack the callback was entered on (b'), **the switch made before the
thunk's question and a nested entry continuing on the runtime stack it came from**, as OCaml 5's verified fibre does, so
the runtime's frames on C's stack a level fall towards the base's, and (e) on a known range with a reserve that holds
those frames alone. **It is unbuilt**, and the critic names its hard case: `csh.hero`'s two coroutines share one C
block and their callbacks interleave on the runtime's side, so one runtime stack a thread corrupts the one suspended
below; a runtime stack a C context the wrappers saw, falling back to one a level on a switch they did not see, is the
shape that would have to hold. What it would leave: shape D, C's own frames on C's own stack, the writes C alone makes,
which § 9 would have to say are C's (the warden's own reading under (b): *"What C's own frames do on a stack of C's is
C's, as any C overrun is"*); what it would refuse: what fits in its reserve, the class (b')+(e) met, narrowed by as much
as its frames shrink. **Whether it lifts every veto is a question for its build**, not a premise.

### The route ranked first, and why

**(b) is ranked first, and it is not adopted: the ffi-pragmatist's veto stands.** The reason is rank 3 of CLAUDE.md
§ Precedence read on the runs: (b) is the only candidate measured at **0 bytes below every stack in every shape anyone
ran**, shape D and the alternation included, both platforms, both levels, plain and sanitized; it makes § 9's
*"running out of stack aborts"* true for every frame the program makes with no sentence telling a reader a program may
write over memory it does not own (w1 alone, +10 real, measured); its per-call cost is 0; and **every failure measured
under it is loud** (a library's own abort, or a crash), where (b') writes at exit 0 where the base did not and (c2)
leaves the defect whole on every stack no wrapper saw. **The strongest case against the ranking is history's**: both
verified precedents of a runtime entered from C, cgo and OCaml 5, run C's calls on the system stack, which is (b')'s
shape, and (b)'s veto is §1.12's own second clause, *"any C library must be bindable"*. **The recommendation is
therefore a build, not a ratification**: before any route is chosen, build (b'') and run it on the critic's four shapes
(the canary, shape D, the alternation, `csh.hero`) and on the 175 cases of (b)'s condition (3), on both platforms; if
it holds `csh` right and the alternation no worse than the base, it lifts the veto (b) cannot, and it ranks above (b);
if it does not, the author chooses between (b) with its veto standing and the conservative resolution below.
Second: (c2) with (l), (q) and its § 9 sentence (it breaks no program C sees and writes nothing the base did not, and
leaves the defect on every unseen stack). Last: (b') (silent writes the base does not make).

## The secondary question: step 199j's stop

**R1, the sentence** (`spec-w5.txt`, the spec-warden's), in § 13 after the handler sentence on `main` (`:438`), or after
panel 258's stop clause in panel 262's adopted text once lane land262 lands it:

> Where a signal handler of C's interrupted a `swapcontext` or `setcontext` before it loaded its target, a switch the
> handler makes naming either context of that one aborts the program before it switches, saying so, where this runtime
> saw both.

**Why a sentence, against the warden's *none*.** By the letter the warden is right: no sentence is falsified, and no
section lists every abort (`spec-shape.md` § Where a rule lives). **By a reader, the trigger, the moment and the words are
each predicted wrong without it** (the critic, its § 4 and item 6): the four refused 2871 cases end 134 inside the
handler's switch **before any callback**, printing no `answer ran for` and neither set of words § 13's handler sentence
names; and **`gt.hero` mode 0 has no callback at all** (the ffi-pragmatist's preemptive scheduler, a handler switching
threads by `swapcontext`): Heroes ends 134 in the stop's words 10 of 10 at each level on this Mac and at `-O2` on Linux,
where C alone dies 133, 138, 139 or hangs, and a reader who finds *"No function of the program is a signal handler"*
predicts it dies as C does. **The warden's own registered row says so** (*"≤ 2 name a switch inside a switch as the cause
(no sentence states it)"*). The compiler-engineer and the ffi-pragmatist each found a sentence owed. The warden reads
panel 258's stop, the shape R1 takes, as the contrast (it guards the runtime's state, this one C's own race, which C
alone dies of); the synthesis reads the reader's prediction as what decides, and without a sentence the reader predicts
this abort wrong, whoever's race it guards.
**It refuses no correct program measured**: the critic's mode 4 (each thread's first context made with `SIGALRM`
blocked, and unblocked once it runs) switches from the handler about 39,400 to 39,980 times in 2 s on this Mac and
about 39,900 on Linux, 0 checks failed, 10 of 10 at each level, Heroes and C alone; **the ffi-pragmatist's mode 3 never
preempted** (*"switched from the handler 0"*, every run), so its *"40 of 40"* is not that evidence. The windows the
compiler-engineer read lie inside the program's own call of `swapcontext` or `setcontext`, none of which is on Linux's
async-signal-safe list (the historian, § 8.2, verified for `signal-safety(7)`; POSIX 2008's own page unread).

**R2, the sentence lands once its gap is repaired.** *"aborts ... saying so"* is a promise, and **on Linux arm64 at
`-O0` one run of `gt.hero` mode 0 in ten ended `exit 135` (Bus error) with nothing said** (the critic, `linux2-runs.txt`,
on `de57f9129`, which held 214a: whether it happens on today's `main` is unrun, and where the signal landed in that run
is unread). So R1 lands only at a commit where that gap is repaired and `gt.hero` mode 0 re-runs in the stop's words at
each level on both arm64 platforms at a rate that would show one in ten (`d-p263-switch-stop-gap`, § Found beside). If
its repair is refused as a cost of what the runtime can see (a window inside libc's own switch, say: an inference), the
sentence lands with *"may abort"* for *"aborts"*, by a decision the author reads, as panel 262's R2 withdrew a clause.

**R3, its price and instrument.** **+52 and +52 vendored** (claude-legacy 10,374 to 10,426, cl100k_base 10,563 to
10,615), the warden's, **re-run by the synthesis on `main`'s spec** (`736b534c…`; `spec-w5.diff` applied to a copy in
`synth/m/`, `heroes measure` of both: the same four numbers), a lower bound; real unmeasured (the warden's one request
priced w1 alone, its answer here being *none*), re-counted at the landing, after land262's R1 on the same line 438. **Its
instrument** (the warden, item 4): none reads these words today (`spec_refusals.hero` reads `hero_signal_refuse`'s arms,
and the stop is no arm of it); the landing owes a check pairing the sentence's *"before it switches"* with the stop's
*"so the program stops here, before the switch"* (`sigguard.c:615`), as `spec_refusals.hero` pairs `CLAIMS` with the
arms, and the four refused 2871 goldens stand as its cases; then the `spec` suite, the net's own tests, the four pins
and the ledger's row, and **the site's build: `spec/heroes-spec.md` is in `deploy-site.yml`'s `paths:`, so the push that
carries R1 publishes the site and is asked of the author** (CLAUDE.md § Hard stops). **PARKED** is panel 262's R1's
first sentence's, not this sitting's (`00-shared.md` § 3.9).

## The resolution, item by item

- **R1 to R3**: step 199j's sentence, above.
- **R4, no route for the main question** (§ The sitting's finding); the defect `d-lane-sig-family-2-1` stays `blocking`,
  its draft to be filed by the coordinator as it stands, this sitting's file cited in its next comment.
- **R5, what any route owes, whichever the author chooses**: the counting canary's count, never `--sanitize`'s silence,
  as the golden's judge (`run` goldens `fixedbugs-<n>-*` per claimed shape, annotated in their sources, `malloc.hero`
  sized to return among them); the emitted C of a program with no callback `extern` byte-identical but its sources line
  (under (c2) the compiler and 11 examples move: measured); **the spawn doors exempt** under any switching route (the
  critic's run); CFI on the switch and R2's stop by identity under any switching route (`fu.c`); the 175 signal-guard
  cases on the route's runtime (unrun); both arm64 platforms at the gate and the CI's legs after the push; `stack.c`'s
  note of step 199i (`:1061` on) and `thread.c`'s guard rewritten to say what the route reaches; and design.md's
  Coroutines row (`:3642`) given the warden's dated line wherever a route that switches to a stack of the runtime's lands.
- **R6, the found-beside defects are drafted** (§ Found beside), four, each passing `issues.hero draft`.
- **R7, the blind reading is registered, nothing paid** (§ The blind reading).
- **R8, the record**: panel 262's verdict's *"`so` by step 199i"* is corrected underneath, with its date, by whoever
  next writes to that file (records are append-only; § For the coordinator); panel 104's refusal of a check per call is
  answered by the runs, not withdrawn: its *"counter forbids the recursion-to-loop transformation"* forbids nothing
  today, and what a check costs is the frame's doubling (the compiler-engineer).

## Not adopted

- **(b)**, ranked first: the ffi-pragmatist's veto stands (`csh.hero`).
- **(b')**: the alternation's writes at exit 0 where the base wrote none, both platforms; a runtime stack a nested level.
- **(b')+(e)**: correct alternations refused at n 1,000 to 3,000; a false message in shape D.
- **(c2)**, the conservative resolution below: it leaves the defect whole on every stack no wrapper saw, and refuses
  correct deep programs by the frame's doubling; it is the compromise CLAUDE.md § 4 forbids adopting in the robust
  route's place, and a veto's class stands on it besides.
- **(c1)**: vetoed by the compiler-engineer (`helper.hero`). **(c3)**: nothing (c2) does not do. **(a1) to (a4), (f),
  (g), (h)**: vetoed by the ffi-pragmatist. **(e), (n)** alone: (c)'s holes. **(i), (j), (k), (m), (p), (d)**: none
  exists or none closes anything. **(l), (q)** alone: they change the words, never the writes.
- **`spec-w1`** (+10 real): owed under (b) or (b'), and no route is adopted; **w1b, w2, w3, w4, w6, w7**: each the text of a
  route not adopted. **The warden's *"no sentence"* for 199j's stop**: R1 instead.
- **199j's sentence as a *may* now**: the promise is the more robust text, and its one falsifier is a defect to repair,
  as panel 262's R2 held its sentence for its falsifiers.

## Disagreements, stated plainly

1. **(b) or (c2)**: the compiler-engineer approves (b) and objects to (c2); the ffi-pragmatist vetoes (b) and approves
   (c2). **Checkable, and checked by the critic**: neither seat built (b'); (b') lifts the ffi-pragmatist's ground and
   gives back the compiler-engineer's (shape D, the alternation). Both seats are right about the route they refuse.
2. **On which stack C runs when a callback calls it** (the critic's question nobody put): (b) answers the runtime's and
   breaks C that reads its own stack; (b') answers C's and gives back C's overrun; (c2) leaves every frame on C's. The
   verified precedents answer C's (cgo, OCaml 5). The ranking goes against them on the runs and says so.
3. **199j's sentence**: the warden *none*, by the letter; the other two seats and the critic *owed*, by a reader. R1
   takes the reader's side, the warden's own registered row predicting the misreading.
4. **The ffi-pragmatist's (c2) *"Nothing C sees moves"*** is true of C and silent on Heroes programs: its model has no
   thread-local read, so it could not see the frame's doubling the compiler-engineer measured on the compiler's own
   emission. One side ran the compiler, the other a model.
5. **The ffi-pragmatist's *"40 of 40"* under the stop** tested no switch from a handler (mode 3 never preempts); the
   critic's mode 4 is the run that does.
6. **Costs**: (b) +284 (the compiler-engineer) against +133 (the critic) instructions a callback, two prototypes, two
   bases; neither is a landing's.
7. **The warden's w1 price** counts callbacks suspended; under (b') nested ones count too (128 levels at 8 MiB).

## What a veto would compel

**The compiler-engineer's veto of (c1)** compels a set wider than the address-taken one, which (c2) is. **The
ffi-pragmatist's vetoes** compel no protection inside C's memory, no refusal at the thunk by *readable below*, no swapped
stack at `makecontext`, and **no C call of a callback made on a stack that is not C's**; its condition names the build
that could lift its veto of (b), which is (b'), run, and failing on the alternation. **The spec-warden's veto** compels
§ 9 to move under any route but (b). **Together they compel what this synthesis does**: no route adopted, and the build
of (b'') put to the author as the one experiment that could reconcile them.

## The conservative resolution

**(c2) with (l) and (q), and a § 9 sentence saying that C's frames under a callback, and a callback on a stack this
runtime did not see C make, may write over what lies below that stack** (w2 widened: unwritten, uncounted; w2 alone +25
vendored): built in the compiler at +93 −16 lines, measured on both platforms, the compiler not slowed; it closes the
defect's own program, shape A and OpenSSL's default fibre on every stack the runtime saw made, breaks nothing C sees,
and refuses correct deep programs by the frame's doubling. **Robustness does not take it**: it leaves the defect whole
on every stack no wrapper saw and writes into the spec that a program may corrupt memory, which §1.12 refuses; it stands
here so the author can choose it.

## The critic's list of what must be settled, held line by line

From `critic-reports.md` § *What the synthesis must settle*, items 1 to 10:

1. **Which stack C runs on when a callback calls it**: settled as the finding's third horn and disagreement 2; (b') keeps
   `csh` right and gives back shape D and the alternation, (b) the reverse; no route adopted, (b'') the build owed.
2. **Whether (b') carries (e)**: it closes the alternation and refuses correct ones at n 1,000 to 3,000; not adopted;
   what a landing's thunk shrinks to is (b'')'s question.
3. **The spawn doors**: a condition of every switching route, R5, the critic's run its evidence; a spec fact (§ 9's 2,000).
4. **214a's walk and stop by address order**: drafted for lane unwinder (`d-p263-unwind-address-order`); **214a's own
   regression on `main`** (land262's `mac3-judge.txt`, read by the critic, run by nobody here) was the coordinator's,
   sent to lane unwinder, and 214a has since left `main` (`e0328b090`): not drafted again here.
5. **A runtime stack per nested level under (b')**: in the candidates; nesting on one stack is (b'')'s, unbuilt.
6. **199j's sentence**: R1, against the warden's reading, by the reader's prediction.
7. **The stop's honesty and its gaps**: the Linux `-O0` gap drafted and R2's condition; the loop-or-die under this defect
   drafted with the hang's cause (`d-p263-static-hang`) and the false message's (`d-p263-guard-return-address`), both
   (l)'s and (q)'s to answer whatever route lands; mode 4, not mode 3, is the correct scheduler's evidence.
8. **Costs on `main`**: the per-callback base moved +9 instructions with 214a and moved back without it; every percentage
   above is against its seat's base; a still machine never ran any of them.
9. **The precedents**: cgo and OCaml 5 verified, (b')'s shape; libtask's `needstack`, libaco, glibc's mask order,
   `__chkstk`, a fibre's guard page, Darwin's `thread_set_state` and the OOM killer **recalled**, and nothing here rests
   on them as facts (each is named as recalled where it appears).
10. **The spec**: no route moves § 9 or § 13 now; R1 moves § 13 for 199j's stop; the blind reading stays registered.

**And what this synthesis itself does not assert**: a real count of any text; a run of any route on `main` at
`1b7b73a0b` (every route run is a seat's at `f9f42fcc4` or the critic's at `de57f9129` and with 214a reversed); (b) on the
175 signal-guard cases; (b'') at all; the 199j gap on today's `main`; a run on Windows, Linux x86-64, an Intel Mac or musl;
OpenSSL 4.1.0's `_mmap` and `_mprotect` imports' origin (unverified by the historian, and the run shows the fibre
unguarded whatever they do).

## Found beside, for the coordinator

Four drafts in `.claude/worktrees/rebel-scratch/coord/issues/`, each `github: none`, each passing `heroes run
tests/harness/issues.hero -- draft <file>` with the synthesis's compiler (§ The ground), exit 0, its rendering in
`.claude/worktrees/panel263-scratch/synth/draft-*.txt`:

| draft | the defect | found by | class |
|---|---|---|---|
| `d-p263-switch-stop-gap` | step 199j's stop lets `gt.hero` mode 0 die `exit 135` with nothing said, one run in ten, Linux arm64 `-O0` (on `de57f9129`, with 214a; today's `main` unrun) | the critic (second reading, § 4) | `blocking` |
| `d-p263-guard-return-address` | the false thread message: `hero_thread_guard` tests a thread-local its own prologue's saved return address overwrote | the compiler-engineer (§ 4 (i)) | `blocking` |
| `d-p263-static-hang` | a callback overrunning a stack in the image's data hangs this Mac: `SIGBUS`, then `SIGSEGV` nested in the runtime's trampoline on the overwritten record; 139 on Linux | the compiler-engineer (§ 4 (ii)); the shape the critic's first reading | `blocking` |
| `d-p263-unwind-address-order` | 214a's unwind walks and stops by address order, so across a switch with C's stack below the runtime's it releases nothing (11,000 blocks live), for lane unwinder | the compiler-engineer (§ 2, `fu.c`) and the critic (§ 3, `uwx`) | `adjacent` (no program on `main` fails by it; the first item of any switching route's landing) |

The second and third are this sitting's measured causes of two outcomes the defect's own draft names or meets (the
false message, the hang); they are drafted apart because they stand under every route that leaves any writer below a
stack, C's own frames included, and their answers are (l) and (q), which no route here adopts; the coordinator may file
them as comments on the defect's issue instead, and the drafts say so. Beside them:

- **Panel 262's verdict** (`docs/panel/262-*.md:588-589`) reads *"`so` by step 199i (`94c2266a4`)"*: true of the guarded
  `mmap` half; the `malloc` half is this defect, open. 262's R2(a) and the compiler-engineer's 262 veto both name `so`, so
  whether R1 of panel 262 waits for this defect is the author's to read; the correction under the verdict is owed with its
  date (the warden, its first finding and item 5). Not the synthesis's to edit.
- **The historian's fetch tool saved a PDF outside the repository** (`historian.md`'s head): the author is owed it, the
  file the coordinator's to remove.
- **`callback_guard.hero:28-30`** (*"A program that binds no callback guards nobody — the compiler itself and all 45
  corpus programs"*) is stale: the compiler and 11 examples hand C a function (the compiler-engineer, § 10).
- **`parts/drop.c`'s *"`==` and `hash` still recurse"*** is stale for `==` on a 100,000-deep chain (the
  compiler-engineer, § 1.4; `hash` not run).
- **`frame.c`'s room words** (*"the room large values take off the stack is full, 1 GiB for the whole program"*, `:130`,
  `:138` at `main`) would name no stack under w1 (the warden, its third finding): owed by w1's landing, whenever a
  switching route brings it.
- **Line 438 holds panel 259's held-memory sentence, panel 257's handler sentence and, once land262 lands, panel 262's
  R1**: R1 here and any w1 later apply by anchor, never by a merge's hand resolution (the warden, its fourth finding).
- **The four Linux switch-family reds of 199j's commit body** stay unnamed: not red at `f9f42fcc4` in the 134 cases of
  `fixedbugs-27` and `-28` (the compiler-engineer); `fixedbugs-26`'s 41 and the commit's *"ten sets"* unrun.

## The blind reading, registered (nothing paid)

**A sentence of § 13 moves (R1), so the llm-ergonomist's blind reading is registered for the author to authorise.** It
would compare **`now`**, `main`'s spec at the landing (`736b534c…` today, or panel 262's R1 applied once land262 lands),
against **`adopted`**, `now` with R1, and, if the author wants R2's fallback read, **`rival`**, `now` with R1's *"may
abort"* for *"aborts"*; on the seven 2871 cases, `gt.hero` modes 0 and 4 (the critic's `gt.h`), and two controls of a
handler switching outside any switch; five readers an arm and a verdict session, panel 262's runner and labels. **Its
size, an inference from panel 262's measured run** (11.66 USD for 31 sessions, 0.376 a session): about 21 sessions and
8 USD for two arms, about 31 and 12 USD for three; nothing run, nothing paid. If the author chooses a route that moves
§ 9 or § 13 (w1, w7, or a § 9 sentence for C's frames), its text joins the arms and the warden's registered predictions
(`spec-warden.md` § Predictions registered before the blind run, standing whole, the critic's reading) are read for it.

## What is open and unrun

- **(b'')**, the build ranked as the next step; **(b) on the 175 signal-guard cases**; (b)'s Windows and x86-64 halves.
- **Any route on today's `main`** (`1b7b73a0b`): the critic's 214a-reversed column is its nearest run.
- **The 199j gap on today's `main`**, and where its signal lands.
- **The count of suspended callbacks a correct program reaches** (w1's bound, 1 GiB ÷ S).
- **The alternation under (c2)**; (c2)'s frame growth refusing on Linux; (c2) on OpenSSL's fibre with the ffi-pragmatist's
  reserve.
- **A real copy-stack library** (libco in shared-stack mode) and a conservative collector under (b): the seat's
  prediction and the synthesis's inference, unrun.
- **Windows, Linux x86-64, an Intel Mac, musl**: unrun by every seat and by the synthesis.

## Predictions to score

The seats' (the verdicts' table), each at the landing that checks it. **The synthesis's own, registered before any
build or blind session**:

- **(b'') built** as described: `csh.hero` right 3 of 3 at each level on both platforms, and the alternation's C stack a
  level at `-O2` within twice the base's 64 bytes; if either fails, (b) stays first and its veto stands.
- **(b) on the 175 cases** (condition 3): at least one red on its first run, a rule of steps 199a to 199o that asks
  which stack a frame is on (`00-shared.md` § 3.7) meeting the runtime's stack.
- **R1, under `adopted`**: at least 8 of 10 readers predict the four refused 2871 cases and `gt.hero` mode 0 *abort*
  naming a switch inside a switch; under `now`, at most 2 (the warden's own row); `gt.hero` mode 4 *runs* for at least 7
  of 10 under both.
- **R2**: the gap reproduces on today's `main` (one in ten or rarer at Linux `-O0`), and is repaired before R1 lands.

## Author's verdict

**Pending.** The decision's draft is `docs/panel/263-reports/decision-draft.md`, filed with this sitting as `panel 263`
(`issues.hero draft`: exit 0, run with the synthesis's compiler from this sitting's worktree, where its `## Prior art`
check finds the sitting's file; from the scratch, which git does not list, the same check refuses it as a file the tree
does not hold). Its `## Prior art` names the two precedents the ranking and (b'') rest on, cgo and OCaml 5, as the
historian verified them; nobody has re-read their pages since, so the session that files the decision re-reads them
first (`.claude/rules/records.md` § And the record says whose idea it was).

**The author's reading, 2026-11-27, at about 18:35**, in conversation (the answer *1a* to the coordinator's three
routes, meant as: *build (b'') and measure it, then the critic and the synthesis judge it*; the coordinator's
recommendation, and the synthesis's). **What it decides**: no route is adopted yet; (b''), C's calls on the stack C
entered the callback on and nested callbacks sharing one runtime stack, as OCaml 5 does, is built in a lane of its own
and run on the canary, shape D, the alternation, `csh.hero`, the static array and the signal guard's cases on both
arm64 platforms, with every condition the seats named (the spawn doors that must not switch, ASan's fibre
annotations, CFI on the switch and panel 258's R2 stopping by frame, a bound on held stacks); then the completeness
critic and the synthesis judge it against every seat's veto, and the author ratifies the route. The secondary
question's R1 (step 199j's sentence, `spec-w5`) stands provisional, as written. The decision issue opens with this
reading.

## Amendment of 2026-11-28: route (b'') as built and measured

**Resolution adopted: provisional — author ratification pending.** **On the main question no route is adopted, again,
and the reason is now a list of builds rather than a finding against every route.** Route (b''), built by lane bpp (steps
217a to 217g on `lane-bpp`, `c1abdf30a` to `2dd6d086f`, unmerged) on the author's *1a* of 2026-11-27, **lifts the
ffi-pragmatist's veto on its own ground as built** (`csh.hero` right on both arm64 platforms at both plain levels), and
**every other veto, and that one's class, only with conditions**: two the completeness critic prototyped and measured,
**P1** (the thunk's arguments and result in registers) and **P4** (the switch made before the decision), at the two plain
levels only; the rest unbuilt (§ Each seat's veto, held to (b'')). CLAUDE.md § 4 and § Precedence bind this amendment as
they bound the sitting: what lifts every seat's veto is adopted, or nothing is, and a veto lifted by a condition is lifted
only with that condition built. **(b'') does not lift every veto today, so it is not adopted.** **It is ranked first**,
above (b), carrying P1, P4, the § 9 sentence c1 and `spec-w1` (R10 to R12), and **R13 is the list a landing lane builds
from**; when that lane's runs stand, the critic and the synthesis read every veto again on them, and the route goes to
the author then. **The author's word of 2026-11-27 at about 23:50, given before this amendment existed and
meant as: *yes to the ratification*, which the coordinator recorded as advance consent applied only if this amendment
adopted a route lifting every seat's veto, is not applied**: nothing here is ratified by it. On the secondary question, **R1 now lands as *"may
abort"***, the sitting's own fallback in R2, on a run that shows a window no runtime can see (§ Step 199j's sentence).

Drafted 2026-11-28 from about 00:00 by the synthesis's drafter, from the coordinator's brief
(`.claude/worktrees/rebel-scratch/coord/panel263-amend-brief.md`), having read whole the sitting above, lane bpp's report
(`263-reports/bpp-build.md`, written by the coordinator from the lane's final message), the critic's third reading
(`263-reports/critic-bpp.md`), the four seats' reports and the critic's second reading (`critic-reports.md`). No
sub-agent, no paid run, no commit, push, merge or GitHub write, no credential printed, no download, nothing outside the
repository's root, no `pkill`; no report, brief or the sitting's file was edited.

### The ground

- **What this amendment ran, and nothing else.** The seed's compiler of **`main` at `64d962bd7`** (`git archive main seed
  runtime selfhost spec vendor tests/harness ROADMAP.md CLAUDE.md docs/measurements/010-spec-budget-ledger.md` into
  `.claude/worktrees/panel263-scratch/amend/tree/`; `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`, `real
  18.42 user 17.57`, `heroes 0.6.0`, **`sources 1e1eb23a1927d35b`**), then `heroes measure` of seven texts applied to
  `main`'s spec by `amend/apply.py`, each anchor asserted to match exactly once, offline, no request (`amend/measure-*.txt`).
  The copies `base`, `c1`, `c1-w1` and `c2` are byte-identical to the critic's (`shasum -a 256`: `736b534c…`,
  `33e93c72…`, `d3910f88…`, `3b66e374…` on both sides). Load 9 to 12 on 8 cores (`uptime`, 00:04 and 00:09); nothing is
  timed. **Every program outcome below is lane bpp's or the critic's, cited, never this amendment's.**
- **`main` moved under both.** Lane bpp branched at `8e818cf9c`; `main` is now `64d962bd7` and holds **panel 258's R2
  whole, steps 214a to 214d** (`eba5b297d`, merged at about 23:28 by the coordinator's log), and steps 199q and 199r;
  `lane-bpp` has merged none of it. `git merge-base --is-ancestor` reads 214d (`64e15c644`) on `main`, and step 199t
  (`551520f8c`, lane sig-family-3) not on it. **The spec did not move**: `main:spec/heroes-spec.md` is `736b534c…efd3`.
  The critic built (b'')'s stage 2 at `2dd6d086f` (**`sources 185600f90a609b3c`**) and the base's at `8e818cf9c`
  (**`692ac54673e38215`**), the same two in Linux arm64, and ran panel 258's R2 against (b'') on a tree merging `lane-bpp`
  and `lane-unwinder` in its scratch (`sources 3e2c574b2046b392`), whose 214d is the one `main` now holds.
- **What (b'') is, as built** (`runtime/parts/cbstack.c`'s head at `2dd6d086f`, read; its first words, *"A PROTOTYPE FOR
  A JUDGMENT, NOT A LANDING"*): a callback's Heroes frames on a runtime stack with 64 KiB of no access below it and, since
  217g, above it; every bound C call made from there through a door per extern, back on the stack C entered the callback
  on; a nested callback continuing on its caller's runtime stack where that stack has C's room left, *nested* told by
  frame records reaching an open door's switch record, never by an order of addresses; the spawn doors running in place;
  a reserve on C's known range of 1 KiB, 4 KiB under the sanitizer. **AArch64 alone switches**: *"every other target runs
  as before this route, the x86-64 and Windows switches unwritten"* (`#if defined(__aarch64__) && !defined(_WIN32)`). The
  same head names two shapes it does not reach, read here and run by nobody: *"a call through a Heroes function value
  whose target is a C function runs where its caller stands"*, and *"a callback suspended on one thread and resumed on
  another is unsupported"*.

### The sitting's predictions, scored on (b'')

- **The synthesis's own** (*"`csh.hero` right 3 of 3 at each level on both platforms, and the alternation's C stack a
  level at `-O2` within twice the base's 64 bytes; if either fails, (b) stays first and its veto stands"*): **held on
  both counts.** `csh.hero` prints `both: 10022002` 3 of 3 at `-O0` and `-O2` on both platforms, its sanitized ends the
  base's (lane bpp; the critic, §§ 1, 8); a level takes **80 bytes** at `-O2` (the thunk 32, the door's runner 16, C's
  `c_again_` 32, by `otool -tV`; the critic, § 2). **It was too weak a test**: within *"twice"*, (b'') refuses correct
  alternations the base runs (the band, below). So (b'') ranks above (b), and the band is what that rank still owes.
- **(b) on the signal guard's cases, *"at least one red on its first run"***: (b) was never run on them; read on (b'')
  instead, **4 red of 187 on each platform** (lane bpp: 187 distinct cases, not 175), **as many as predicted and not for
  the cause predicted**: no rule of steps 199a to 199o asking which stack a frame is on, but the room's count of
  callbacks not returned, which held runtime stacks now share (the critic, § 4).
- **R2's, *"the gap reproduces on today's `main` ... and is repaired before R1 lands"***: **reproduced** (step 199t's
  body: `gt.hero` mode 0 on `beecb6c30`, Linux arm64 `-O0`, 100 runs, 93 in the stop's words, 3 a null read inside the
  runtime, 3 `exit 135` and 1 `exit 139` with nothing said), **repaired where a runtime can see it** (99 of 100 after
  199t) **and not where none can** (§ Step 199j's sentence).

### Each seat's veto, held to (b'')

| seat, and the veto as cast | as built (217g) | with P1 and P4, the critic's prototypes | with a condition nobody has built | what stands |
|---|---|---|---|---|
| **ffi-pragmatist**: (b) as stated, on `csh.hero`; its class, a correct program refused; its condition, `csh.hero` and the unbounded alternation | **its ground lifted**: `csh.hero` right, both platforms, both plain levels; `hq`, `gt` unmoved; OpenSSL's default fibre better than the base (here 0 bytes, told past, where the base changed 276,369 at exit 0; in Linux right at n 200 and 1,200 where the base died 139 and 135 with nothing said) | **the band closed at `-O0` and `-O2`, both platforms**: the alternation's deepest right n **4,094 / 4,095** against the base's 4,093 / 4,094 at `-O2`, **1,259 / 1,364** against 908 / 961 at `-O0`; a single callback's smallest room **23 or under** against 103 / 23 or under (`-O2`, n 0) | P4 at the sanitized levels (**unrun on either platform**); P1 and P4 as a landing builds them | **its class, as built at every level** (3,262 against 4,093 at `-O2`; a callback with 1,047 bytes of a known range left refused where the base runs on 103), **and at the sanitized levels until P4 runs there** (2,014 / 2,302 against 3,270 / 3,272 at `--sanitize -O2`; 700 / 732 against 774 / 813 at `--sanitize -O0`) |
| **compiler-engineer**: (c1), on `helper.hero`; its class, bytes below a stack in a shape the route claims; its approval of (b) on seven conditions, *"any one red turns me to (c2)"* | (c1)'s veto untouched (no checked set in (b'')); **the class lifted for bytes in every count but one build** (`helper.hero`, the canary's kinds 4 and 6, the alternation at n 1,000 to 100,000, the reproducers: 0 bytes, told); conditions (1), (4), (5), (6) built; (3) 183 of 187 | P4 takes the reserve's refusal off C's stack (unrun under the sanitizer); (7) met in prototype: **+243** with the page size read once, **+237** with P1 too, +255 with P4, under (b)'s +284 | the refusal making no call on C's stack; **(2) in purpose: panel 258's R2 taught to cross (b'')'s stacks**, judged by `uwx`'s live count; (3)'s four: the room's words and the cases re-pinned; (7) in the route | **Linux arm64 `--sanitize -O0`: 368 bytes below** at every n of the alternation (the critic, § 5); **(2)**: 11,000 blocks live at exit where 214d alone leaves 0 (this Mac; Linux unrun); **(7)**: +310 |
| **spec-warden**: any route leaving a callback's stack unguarded with § 9 unmoved | **stands**: § 9 unmoved; shape D writes the base's bytes; on a stack no wrapper saw the entry checks no reserve, and the thunk with the decision puts 440 (`-O2`, warm) to 4,480 bytes (`--sanitize -O0`, first call) of the runtime's frames on C's block unchecked (`td`; the critic, §§ 1, 3) | **its letter answered by c1** (§ 9 moved); its condition on (b), *"the thunk ... write[s] on C's stack nothing but a bounded frame before the switch"*, met by P4 alone, which leaves the thunk's own frame and nothing of the runtime's | c1 written into § 9 and counted for real; **the x86-64 switches, Linux and Windows**, since § 9 names no platform; the door on a call through a function value; `frame.c`'s words naming the stacks (its third finding); the count of suspended callbacks a correct program reaches (its w1 condition) | **as built, everywhere**; with c1 and P4, on x86-64 and on a C call through a function value until those are built |
| historian (advisory) | no veto; (b'') is the shape it verified for OCaml 5: C on the system stack, later callbacks on the current fibre (its § 6.7) | | | |

**So every veto lifts, where it lifts, only with a condition, and at least one condition of each is unbuilt**: the band
and the 368 bytes at the sanitized levels, where P4 has never run; the warden's ground on x86-64 and wherever the
runtime's frames still stand before the switch. **No run here shows a veto that no named build could lift**, which the
sitting found of every route before (b''); that is what changed, and it is why the answer is a landing list.

**The ffi-pragmatist.** Its veto of (b) as stated named one program, `csh.hero`, and one condition, *"(b) rebuilt with
every C call from a callback made on the C stack it was entered on, run on `csh.hero` and the unbounded alternation"*.
(b'') is that build, and lldb counts it exercised: 2 entries through `hero_cb_switch` and 6 through
`hero_cb_door_switch` in one right run (the critic, § 1). The alternation writes nothing below C's stack at any n, both
platforms; **what stands is the seat's class**: as built, (b'') refuses, with 0 bytes and in words, correct programs the
base runs, in two shapes the critic measured: the alternation between 3,263 and 4,093 levels at `-O2` here, and **any
single callback entered with about a kilobyte or less of a known range left** (`CO_LEAVE` 103 to 1,047 here at `-O2`,
23 or under to 1,095 in Linux), which nobody had measured before. **The cause is the decision run on C's stack before
the switch, and the 1 KiB reserve that holds it**: the critic's `td.hero` reads (b'')'s entry 440 bytes below C's `sp`
at `-O2` warm, where the base's whole callback reaches 256 or less, the probe's floor. P1 brings a level to 64 bytes
(4,078 here); a smaller reserve cannot close the band while the decision stays on C's stack (256 bytes writes 40 below
at the refusal); **P4 moves the
decision off C's stack and closes both shapes at both plain levels on both platforms**, the reserve at 16 bytes. Under
the sanitizer the band is widest and P4 is unrun; the runner's frame there is 48 bytes because it brackets the C call
with the fibre annotations (the critic, § 2). Beside its class, four things the seat would want read, none a veto it
cast: **the held-stack bound** refuses a correct program the base runs (lane bpp's `held.hero`: the 7,282nd suspended
callback here, the 7,944th in Linux, where the base ran 9,000; 7,282 stacks of 144 KiB, a 64 KiB stack with one guard
and a page, fill 1 GiB, so the run predates 217g's second guard, by arithmetic), the price w1 states and the seat
accepted under w1 (its § 17); **a C function reached through a function value from a callback** runs on the runtime's
stack (`cbstack.c`'s head, read), so `csh.hero`'s class has a shape nobody ran;
**a conservative collector** scanning its thread's stack from a callback would not see the callback's Heroes frames, an
inference, unrun, as the sitting said of (b); and **a callback resumed on another thread** is unsupported, its end
unread.

**The compiler-engineer.** Its one veto is of (c1), which (b'') does not touch. Its class, held to (b''), is lifted for
bytes in every count but **one build: Linux arm64 `--sanitize -O0`, where the reserve's own stop writes 368 bytes below
the stack** at every n of the alternation (lane bpp; the critic reproduced it at `2dd6d086f`). The critic found the
writer with a hardware watchpoint: `hero_cb_reserve_refused` (`cbstack.c:535`), whose own head says it *"needs no call
that could take C's stack"*, asks `sigaltstack(NULL, &alt)`, which ASan intercepts in Linux, and the interceptor's callee
lands its frame record exactly at `lo − 1072`; Darwin does not intercept `sigaltstack`, and the same build here writes
nothing. Of its seven conditions on (b): (1) the spawn doors run in place (the critic's `check` of a 13,000-term sum and
a 14,000-term one: 0 and 134, as the base); (2) CFI on both switches is built (the unwinder crosses both to `main`, both
platforms), **and panel 258's R2 does not cross them**: with `lane-bpp` and `lane-unwinder` merged, `uwx.hero`'s 1,000
jumps over callbacks on the thread's own stack leave **11,000 blocks live at exit with (b'')'s switch and 0 with it
off**, because 214d's three one-stack checks (`rebind_jump.c:326-337`, `unwind.c:742-758`, `unwind.c:509-531`, read by
the critic at `eba5b297d`) answer *no unwind*, or stop, at a frame record outside the thread's own stack, which a
runtime stack's records are, by the critic's reading, and the run agrees; no fault, and 214a's golden cannot see it (the
critic, § 7.1). **That is a regression against `main` as it now stands**, met in its letter (CFI, a stop by
identity) and not in its purpose; (3) 183 of 187 green, the four being the room's count (below); (4) and (5) built,
the fibre annotations with 217e's unpoisoning on a jump; (6) the bound built, its words owed; (7) **+310 instructions a
callback as built, over (b)'s +284**, and +243 / +237 with the critic's two savings. **And shape D**, the run its
approval of (b) *"turns on"*: (b'') gives it back to (c2)'s count, the base's to the byte (81,424 here, 41,957 in Linux
at `-O0` n 4,000, exit 0; the critic, § 3). That is no veto; it was the seat's ground for calling (c2) *"not the most
robust route"*, and under c1 those writes are C's, as the warden read them under (b). Its false messages at larger n are
the base's (the lend-record message here at `-O0` n 6,000, both sides), and stay the drafts `d-p263-guard-return-address`
and `d-p263-static-hang`, with (l) and (q), whatever lands (the critic, § 3).

**The spec-warden.** Its veto as cast: *"any route that leaves a callback's stack unguarded while § 9 stays unmoved"*.
As built, § 9 is unmoved and shape D writes below at exit 0: it stands. **By its letter, c1 answers it**: § 9 moves.
**By its ground**, *"the sentence promises what the defect's own program falsifies"*, c1 is true only where three things
are built, and none is: **P4**, so that before the switch nothing of the runtime's stands on C's block (as built, on a
stack no wrapper saw, `known` false in `hero_cb_enter`, 440 to 4,480 bytes of the runtime's frames stand there unchecked:
bounded, which its condition on (b) asked, but the runtime's and not C's, and *"running out of stack aborts"* is false
for them); **the door on every call whose target is C**, a function value's included, since c1 says *"C that a callback
calls runs on the stack C called the callback on"*; and **the switch on every platform § 9 speaks for**, where (b'')
compiles to the base on Linux x86-64 and Windows. The seat has not sat again and has not read c1; its acceptance is its
own, and this amendment reads its letter and its ground, not its mind. Its own sentence under (b) is c1's content
(*"What C's own frames do on a stack of C's is C's, as any C overrun is: § 13 promises an abort of C's reach only past a
lend (`counted_by`)"*). Its w1 stands as priced (+6 vendored, **+10 real** counted on 2026-11-27), its condition on w1
unmet: *"the seats' count of suspended callbacks a correct program reaches below 1 GiB ÷ S, or w1b instead"* (`held.hero`
is the nearest run, above). Its third finding is now measured: the room's abort words name no stack while held stacks
are 43.8% of the room in one signal case (below). **CLAUDE.md § Precedence, as the author amended it on 2026-11-22**,
makes the warden's vetoes yield to the most robust route at the cost of the spec's size; this veto is on the spec's
truth, not its size, so it binds, and c1's price decides nothing.

### What this amendment adopts, and what it does not

- **R9, no route for the main question, again**: (b'') does not lift every veto as built, and its prototypes lift what
  they lift at the plain levels only. The defect `d-lane-sig-family-2-1` stays `blocking`, and by
  `.claude/rules/verification.md` § Bounded discovery the milestone's tag waits for it.
- **R10, (b'') ranked first**, with **P1**, **P4**, the § 9 sentence **c1** and **`spec-w1`**, **above (b)**, whose
  veto stands on `csh.hero`, and whose 0 bytes below C's stack (b'') matches in every counted shape but shape D, which c1
  assigns to C, and one build's 368 bytes, which R13's item 3 owes; then (c2) with (l), (q) and its sentence, the
  conservative resolution, as the sitting set it out; (b') last. **Recommended to the author: a landing lane, a build
  and not a ratification**, building R13 whole on `main` as it stands; at its gate the critic and
  the synthesis read every veto on its runs, and the route is put for ratification only if every one lifts there.
- **R11, the stack a callback is given, as built and now stated.** A runtime stack holds the room C's stack gave the
  callback: **the thread's own stack's size** where C called from the thread's own stack; **the size of the range
  `makecontext` was handed** where C called from one, **never under 64 KiB**; **8 MiB where no range is known**; with 64
  KiB of no access below and above it and its record's page. **Why**: no callback has less room than the base gave it and
  none more, so (b'') refuses no recursion the base ran within C's stack, and tells, with 0 bytes below, the ones the
  base ran by writing below it; the canary at `-O2` n 9,000 to 100,000, which (b) and (b') returned right on 8 MiB, is
  told, and OpenSSL's 32 KiB fibre gets 64 KiB (the critic, item 7); a stack of 8 MiB everywhere was refused at the
  127th callback left suspended (lane bpp). **Counted against the room**: a held stack and a live one alike, as built
  (w1's *"counted with the live ones"*), at the stack's usable size and its record's page; **the guards not**, this
  amendment's choice: they are address space with no memory behind them; counted, they refused the 7,282nd suspended
  callback here where the base ran 9,000 (`held.hero`, one guard a stack then); uncounted, 1 GiB holds about 13,107
  stacks of the floor's size here and 15,420 in Linux (arithmetic, unrun), each a few mappings (its layout unread), far
  under Linux's `vm.max_map_count` of 262,144 (arithmetic). The landing measures `held.hero` both ways, and each
  golden's header states the rule.
- **R12, the § 9 sentence the route carries: c1** (§ The § 9 sentence), and **c2 registered as the rival** for the
  blind reading.
- **R1 amended: *"may abort"***, R2 discharged by its own fallback, R3's price re-run (§ Step 199j's sentence).
- **R13, what the landing owes** (§ What the landing owes).
- **R4 to R8 stand**, R5's items folded into R13 where (b'') reaches them.

### The § 9 sentence

**c1**, the critic's, in § 9's last bullet (`spec/heroes-spec.md:340-341`), the warden's w2 family's place:

> A source nested up to 2,000 deep compiles on every platform, and none is refused for its depth; running out of stack
> aborts, and C that a callback calls runs on the stack C called the callback on, where running out is C's.

**Counted** (ran: the seed's compiler of `64d962bd7`, `sources 1e1eb23a1927d35b`, offline, each text applied to `main`'s
spec `736b534c…efd3`):

| text | claude-legacy | cl100k_base | the critic's (`heroes-next`, `185600f90a609b3c`) |
|---|---|---|---|
| the base | 10,374 | 10,563 | the same |
| **c1** | **10,397 (+23)** | **10,586 (+23)** | the same |
| **c1 with `spec-w1`** | **10,403 (+29)** | **10,592 (+29)** | the same |
| c2 | 10,411 (+37) | 10,600 (+37) | the same |
| R1 with *"may abort"* (below) | 10,426 (+52) | 10,615 (+52) | not counted there |
| **c1, `spec-w1` and R1 with *"may abort"*, the three texts a landing carries** | **10,455 (+81)** | **10,644 (+81)** | not counted there |

**Real tokens: unmeasured** (one request, a landing's `--refresh`, not this amendment's); the vendored counts are a lower
bound by the warden's own reading, w1's +6 vendored having been +10 real. The ceiling stands at 16,384 against a real
13,849 (the pin of 2026-11-26) and 13,859 with w1 (counted 2026-11-27); every count here is re-taken at the landing,
after panel 262's R1 on the same line 438 if it lands first.

**Whether it answers the warden's veto by its letter**: yes. The letter is *"any route that leaves a callback's stack
unguarded while § 9 stays unmoved"*, and c1 moves § 9. Whether it is true is the ground, and § Each seat's veto says
where it is not yet (the runtime's pre-switch frames, a function value's call of C, x86-64).

**Why c1 and not c2.** c2 (*"..., where C's own frames may run past it and write over what lies below, as C alone
would"*) tells a reader shape D's run more exactly, and it writes into the spec that the program may write over memory
it does not own: the sentence the sitting refused for every route (*"§ 9 then says either what is false ... or that a
program may write over memory it does not own, which design.md §1.12 refuses"*), and the one the warden called *"a
confession of §1.12"* in w3. c1 says of C what § 13 already says of it, that the runtime promises an abort of C's reach
only past a lend, and under c1 a reader of shape D predicts no told end and no promise, which is the run (81,424 bytes
below at exit 0, the base's to the byte, and the ffi-pragmatist's C-alone OpenSSL job the same in kind). Price is no part
of the reason. **c2 joins the blind reading as the rival arm**, so a reader's prediction can overturn the choice.

### Step 199j's sentence: R1 lands as *"may abort"*

**R2's fallback applies, on a run.** The sitting's R2: *"If its repair is refused as a cost of what the runtime can see
(a window inside libc's own switch, say: an inference), the sentence lands with "may abort" for "aborts", by a decision
the author reads."* Step 199t (`551520f8c`, lane sig-family-3, not on `main`) notes a switch as its wrapper's first act,
and took Linux arm64 from 92 to 99 of 100 runs in the stop's words at `-O0`, and from 46 to 50 of 50 at `-O2` and under
`--sanitize` (this Mac: 300 of 300 before and after). Its body names two windows no runtime reaches, and they bear on
R1 differently:

- **Inside R1's letter**: *"a delivery before the note, in the wrapper's first instructions and the call that finds the
  thread's record"*. The wrapper is the program's `swapcontext` as C sees it, so a handler's switch there is one R1
  promises to abort in words, and the runtime has noted nothing to stop it by. **After 199t, 1 run of 100 at Linux
  `-O0` still ended `exit 139` with nothing said**; where its signal landed is unread, so whether it is this window is a
  question, and the window itself is 199t's own reading of its code.
- **Outside R1's letter, inside a reader's prediction**: *"a delivery after the program chose the context it will switch
  to and before it calls `swapcontext` at all"*, in the program's own code. With that window in `gt.h` widened by a loop
  of 2,000 (lane sig-family-3's `gtw/gt.h`), Linux arm64 `-O0`, 20 runs each: **199t's runtime 20 `exit 139` with
  nothing said** (*"step 199s's 19 and 1, the prototype of this step 18 and 2"*, the body's words). No handler there
  interrupted a switch, so no *"aborts"* of R1's letter is falsified; but R1 was adopted on what a reader predicts of
  `gt.hero` mode 0 (§ The secondary question), and under *"aborts"* that reader predicts the stop's words every time,
  where the run dies silent as C alone does.

So *"aborts ... saying so"* is a promise one window inside its letter may break and no runtime can mend, and a reader
reads it wider than its letter: R2's fallback holds on both counts, the second measured. **The text R1 lands**:

> Where a signal handler of C's interrupted a `swapcontext` or `setcontext` before it loaded its target, a switch the
> handler makes naming either context of that one may abort the program before it switches, saying so, where this
> runtime saw both.

**Its price, re-run**: **+52 / +52 vendored** (10,426 / 10,615), the same as *"aborts"* in both tables, a lower bound,
real unmeasured. **It lands once 199t is on `main`**, on its own clock, owing nothing to the main question; R3's
instrument stands (a check pairing *"before it switches"* with `sigguard.c`'s *"so the program stops here, before the
switch"*, the four refused 2871 goldens its cases), and so does R3's note that the push carrying it publishes the site.
The warden's *"no sentence"* stays registered against it, as before.

### What the landing owes (R13)

Each item names what judges it; every one is run on Darwin arm64 and Linux arm64 at the run form's four builds unless it
says otherwise, and the vetoes are read again on these runs and no others.

**The route's two changes, as a landing builds them**

1. **P1 for every signature**: no record on C's stack; the arguments in the registers the C ABI gives them (eight
   integer, eight floating), a pointer to those C passed on its stack, the result back by its C type's own convention (a
   register pair, a floating register, the memory `x8` names). Judged by the alternation's level (64 bytes at `-O2`, the
   prototype's) and by the ffi-pragmatist's `cbs.c` `wide` signature (10 integers, 2 doubles, a 24-byte record each way).
2. **P4 as a landing builds it**: a decision stack per thread, read from the thread pointer without a call
   (`TPIDRRO_EL0` here, `TPIDR_EL0` in Linux), CFI on the stub's two switches, ASan's fibre annotations on them, a nested
   decision (a handler's callback while one is being decided) made in place, the reserve at 16 bytes. **Judged by the
   critic's `depth.sh` and `leave.sh` at all four builds on both platforms**, the alternation no shallower and a single
   callback's smallest room no larger than the base's at the same build: **the sanitized levels measured under P4 for
   the first time, which is the ffi-pragmatist's class's condition.** The runner's tail branch (P1c: 48 bytes a level,
   5,438 here, 5,437 in Linux) is not needed to close the band and is the landing's to take or leave, measured.
3. **The 368 bytes**: the reserve's refusal makes no call on C's stack before its switch (P4's refusal on the decision
   stack, or the alternate stack's bounds read once a thread); judged by the critic's watchpoint command
   (`out/lx-368-watch.txt`) and the alternation's count at Linux `--sanitize -O0`, n 1,000 to 100,000: 0 bytes.

**Panel 258's R2 across the switch**

4. **`main` merged first** (214a to 214d; the critic's three conflicts, `callback_thunk.hero`, `ctype.hero`,
   `guarded_names.hero`, resolved by the lane, never copied from the critic's scratch; and the switch wrappers'
   `rebind_jump.c` and `sigjump_save.c`, which 199s and 199t touch, if lane sig-family-3 lands first). **214d's three
   one-stack checks taught (b'')'s stacks**: crossing a door's switch record and an entry's by their identity
   (`HeroCbDoor.rec_at`, the entry's record), each frame bounded by the stack it stands on (`hero_cb_bounds_of`), never by
   an order of addresses, the walk's `sp > walk->target` among them; the thunk's mark where that walk reads it. **Judged
   by `uwx.hero`'s live count at exit, never by 214a's golden alone**: 0 on the thread's own stack with (b'')'s switch, as
   214d alone reads, both platforms (Linux unrun so far), and what it reads on a static array's and a `malloc`'d block's
   said in the case's header (11,000 under 214d alone, by its one-stack rule). Its price measured with the route's (214d's
   own +43 instructions a callback over no R2, by its commit body, read).

**The room, its words, its count**

5. **`frame.c`'s abort words name the stacks** wherever any is counted (as built they say *"the room large values take
   off the stack is full"* while stacks hold 0.8%, 0.1% and **43.8%** of the room in the four cases; `cbstack.c`'s own
   words name them only where a stack's own mapping overflows): the warden's third finding, a §4.17 false cause,
   `blocking` if it landed as built.
6. **The four signal cases re-pinned under w1's rule**, the rule written in each header (here: 2,047 → 2,031, 963 → 956,
   2,047 → 2,045, 2,046 → 1,149 at 217g with the guards counted; re-read under R11's rule), and **R11's rule stated in
   `cbstack.c` and measured**: `held.hero` with the guards counted and uncounted, both platforms, at the landing's stack
   layout; **the count of suspended callbacks a correct program reaches**, the warden's w1 condition, run or answered by
   w1b.

**The cost**

7. **Toward (b')'s +133 a callback, or the measured reason it cannot get there.** As built +310 at `-O2` here (Linux +277
   by lldb's count, +15.8 ns by its clock); the page size read once (`hero_cb_size_wanted` asks `sysconf` at every
   callback: +243), P1 (+237), P4 (+255); **by arithmetic, unrun**: one decision for the entry and the question, and one
   thread-local carried in the entry, about +130 to +150; the floor no prototype went under is `hero_cb_switch`'s 12
   instructions and P4's stub's 27. If the landing stays above +150, lldb's count by function (`lldbp/count.py`) says
   what stands. **Per C call**, +109 inside a callback and +9.7 outside every one, untouched by any prototype, measured
   again. Panel 104's recursion at 0.0%, held.
8. **The compiler is not slowed, measured**: it hands C a function (`hero_thread_spawn_sized`, the sitting's finding),
   so under `callback_door.hero` every bound call of its own goes through a door (read; its emission under (b'') unread):
   `tests/harness/latency.hero compare` of the compiler before and after (CLAUDE.md § Verification), and the eleven
   examples that declare `hero_thread_spawn` timed beside it.

**The spec and the records**

9. **c1 and `spec-w1` in the spec**, with R1's *"may abort"* if it has not landed first: each applied by its anchor, line
   438 holding panel 259's, panel 257's and, later, panel 262's R1; one `--refresh` for the real count of what lands (+81
   vendored for the three); the four pins and the ledger's row; the `spec` suite, the net's own tests; **c1 lands only
   with items 2, 12, 14 and 15**, since without them it is false for the runtime's frames before the switch, for a
   function value's call of C, and on x86-64; the site's build, and the push asked of the author
   (`spec/heroes-spec.md` is in `deploy-site.yml`'s `paths:`).
10. **The stale notes R5 named**, rewritten to say what the route reaches: `callback_guard.hero:28-30` (the compiler and
    eleven examples hand C a function), `stack.c`'s note of step 199i, `thread.c`'s guard; `cbstack.c`'s head, which
    calls itself a prototype; design.md's Coroutines row (`:3642`) given the warden's dated line.

**The goldens per shape**, `run` goldens `fixedbugs-<n>-*` annotated in their sources, judged by the counting canary's
count, never by `--sanitize`'s silence

11. **The canary**, kinds 4 (`malloc`) and 6 (a static array), the critic's sizes and levels: 0 bytes, told. **Shape
    D**, as c1 states it: the bytes below compared, inside the program, with those the same C recursion writes when C
    calls it directly, equal, and the program's answer right (its counts differ by platform: 81,424 here, 41,957 in
    Linux). **The alternation and the band**: right with 0 bytes at the base's deepest n at each build, told with 0
    bytes one past; a single callback entered with the base's smallest room, right. **`csh.hero`**: `both: 10022002` at
    the plain levels, the sanitized ends the base's. **The static array**, told (the base dies 139 or 135 in Linux with
    nothing said, and hangs here). **The defect's reproducers**, `malloc.hero` sized to return among them (R5), and
    `guarded`, `bare`, `thread`. **`uwx`** (item 4). **`held.hero`**, told in the stacks' words.
12. **The shapes beside them** (CLAUDE.md § RUN IT: a repair is attacked at the shapes next to the one that provoked it):
    **`csh.hero` with `needstack` and the yield reached through a function value**, right with the door applied there or
    refused in words; **a callback suspended on one thread and resumed on another**, right or told; each a golden.
13. **The 187 signal-guard cases** (or their number at the landing) at the four builds, both platforms, on the landing's
    runtime: 2871's seven among them, the four of item 6 re-pinned, none else moved. Lane bpp ran them at 217f here and
    with 217g's runtime copied mid-run in Linux: neither is a run of one tree.

**The platforms**

14. **Linux x86-64**: the SysV switch and door switch (the same three `DW_CFA` expressions over `rbp`; the
    compiler-engineer's estimate, about 25 lines), run in the emulated container or the CI's leg, every item above that
    is a run. (b'') compiles to the base there today, and so does the defect.
15. **Windows x86-64 owes the arm, not a qualifier.** The box answers `ssh` from this Mac again since about 00:00 (the
    coordinator's log: Tailscale on, the box's timer to about 04:00, the box given to lane ci-windows), so a Windows
    fact is runnable now and stays an inference until it is run. By reading, (b'') compiles to the base there, so the defect stands on
    Windows wherever C switches by hand onto a block with nothing of no access below (a fibre from `CreateFiber` gets a
    stack the system assigns, the historian's verified § 6.9; a guard below a fibre's stack: recalled). Windows owes: an
    x86-64 switch and door switch for PE/COFF whose unwind data (`.pdata`/`.xdata`) describes them, so the system's
    unwinder and a C jump over callbacks cross them; the thread information block's stack base and limit moved at each
    switch and back at each door, so `__chkstk`'s probes and the system's overflow handling judge the stack the frames
    stand on (recalled, not read: `__chkstk`, SEH's unwind data, whether the CRT's `longjmp` unwinds on x64; the
    ffi-pragmatist's § 14, the compiler-engineer's § 16); the runtime stack's guards made by `VirtualAlloc` or
    `VirtualProtect`, its run-out told by the runtime's guard there (how that guard finds a range: unread); a known range
    read from the thread information block where a fibre's bounds stand in it. **Run on the box**: a hand-switched heap
    block's canary (Windows has no `makecontext`: a model with its own switch, and a `CreateFiber` fibre beside it),
    `csh.hero`'s model on that switch, the alternation and its band, `uwx`'s jump; then the CI's Windows leg after the
    push. A § 9 qualifier instead would say a program may write over memory it does not own on one platform, which the
    sitting refused under §1.12 on every platform.
16. **The CI's four legs after the push**, and the census of emissions: a program that hands C no function emits the
    same C but its sources line (`callback_door.hero`'s head says so; the critic's `gt.hero` read it so), the compiler
    and the eleven spawn examples moving, named in the closing commit.

**What waits on the landing, and what does not**

17. **Panel 262's held sentence** (its R1, lane land262's step 215a, held at the coordinator's word of 15:47 for this
    sitting's repair): it lands after this route's `malloc` goldens are judged by the canary's count, which is 262's
    R2(a) for `so`'s `malloc` half; both texts on line 438 by anchor, never by a merge's hand; or R8's correction under
    262's verdict, read by the author.
18. **Panel 263's R1, *"may abort"***: after step 199t is on `main`, waiting for nothing else (§ Step 199j's sentence).
19. **Decision 2902's `## Prior art`**: cgo and OCaml 5 as the historian verified them, written when a route is
    ratified, after their pages are re-read (`.claude/rules/records.md` § And the record says whose idea it was); OCaml
    5 is (b'')'s own shape (C on the system stack, later callbacks on the current fibre).

### The blind reading, registered (nothing paid)

§ 9 and § 13 now both move in the texts a landing carries, so the llm-ergonomist's blind reading stays registered for
the author to authorise, with these arms: **`now`**, `main`'s spec at the landing; **`adopted`**, `now` with c1, w1 and
R1's *"may abort"*; **`rival`**, `adopted` with c2 for c1. Programs: the sitting's (the seven 2871 cases, `gt.hero`
modes 0 and 4, two controls), and shape D, the alternation, `csh.hero`, the canary and `held.hero` (w1's own row); five
readers an arm and a verdict session, panel 262's runner and labels. **Its size, an inference from panel 262's run**
(0.376 USD a session): about 31 sessions and 12 USD for three arms. The warden's registered predictions stand whole;
its row *"w1's own row"* reads the same under (b'').

### Predictions registered before the landing

- **P4 at the sanitized levels**: the alternation's deepest right n no shallower than the base's at `--sanitize -O2`
  and `--sanitize -O0` on both platforms (here 3,270 and 774; Linux 3,272 and 813), and a single callback's smallest
  room no larger than the base's. If either fails at any build, the ffi-pragmatist's class stands there and the route
  is not put for ratification.
- **The 368 bytes**: 0 at every n under P4 at Linux `--sanitize -O0`.
- **R2 across (b'')**: `uwx` reads 0 live blocks at exit on the thread's own stack with the switch, both platforms.
- **The cost**: under +160 instructions a callback at `-O2` here with the merged decision; if not, lldb's count names
  the function that holds the rest.
- **`held.hero` with the guards uncounted**: at least the base's 9,000 suspended callbacks held before the told abort,
  both platforms.
- **The blind reading, if run**: under `adopted`, at least 7 of 10 readers predict shape D runs on without a told end;
  under `rival`, at least 7 of 10 predict it may write below and run on; under `now`, at most 3 of 10 predict either.

### What is open and unrun

- **P4 at the sanitized levels**, and P1 and P4 beyond one integer argument, one decision stack and Darwin's and ELF's
  syntax; the 368 bytes under P4; R2 across (b'')'s stacks, and its Linux run at all.
- **Linux x86-64 and Windows**: unbuilt and unrun by everyone; an Intel Mac and musl, the same.
- **The four signal cases as one tree in Linux**; `gt.hero` under (b'') in Linux; the 187 cases after P1 and P4.
- **A real copy-stack library** (libco in shared-stack mode) and **a conservative collector** under (b''); **a C
  function reached through a function value**; **a callback resumed on another thread**.
- **The count of suspended callbacks a correct program reaches**; `held.hero` at 217g and with the guards uncounted.
- **The compiler's speed under (b'')**, the eleven spawn examples' cost, and the cost with the merged decision.
- **c1's, w1's and R1's real count**; the blind reading.

### For the coordinator

- **The advance consent is not applied**: no route is adopted; the author is owed the result in ten lines, and R1's
  *"may abort"* is the one text this amendment adopts for them to read.
- **Step 199t before R1**: R1 lands on `main` once lane sig-family-3's 199s and 199t are merged and judged.
- **The landing lane, if the author asks for it**, starts from `main` and takes `lane-bpp`'s seven commits by merge, not
  from the critic's merged scratch; the critic's prototypes are in `programs/critic/third/` (`alt-p1.c`, `alt-p4.c`,
  `cb-p4.c`, `rt-p-cbstack.diff`) as measurements to reproduce, never as code to copy.
- **`d-p263-unwind-address-order`** reads against `main` now: 214d stops by the frame's identity (*"Never `CFA >=
  target`"*, `unwind.c:643-651`, the critic's reading), while its walk still reads `sp > walk->target` as passed (the
  critic, § 7); the draft's stop half is answered, its walk half stands, and item 4 is where it is closed for (b'').
- **Nothing of this amendment ran a program**: its one build is the seed's compiler, and its one instrument
  `heroes measure`; the files are `.claude/worktrees/panel263-scratch/amend/` (`apply.py`, the seven `spec-*.md`,
  `measure-variants.txt`, `seed-build.txt`).
