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
