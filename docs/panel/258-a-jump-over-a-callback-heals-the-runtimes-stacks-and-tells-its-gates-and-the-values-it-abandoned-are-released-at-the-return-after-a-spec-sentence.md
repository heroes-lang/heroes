# Panel 258: a C jump over a callback heals the runtime's stacks and tells its gates, and the values it abandoned are released at the return, after a spec sentence

**Resolution adopted: provisional — author ratification pending.** Soundness lane (the compiler-engineer and the ffi-pragmatist,
no historian). Convened 2026-11-24 by the author's answer *2c* to `/decide` (decision issue 2751). Briefs:
`docs/panel/258-briefs/` (first version at `c4fc5b46a`, rewritten at `a23ea157a` after the critic); reports:
`docs/panel/258-reports/`.

## The proposal, as put to the seats

A C function can abandon the frames of a Heroes callback (`longjmp`, `siglongjmp`, `_longjmp`, `__longjmp_chk`, `setcontext`, a
`pthread_exit` on a worker, a signal handler that jumps out, a coroutine never resumed). The jump runs no code, so whatever those
frames held and whatever the runtime's per-thread bookkeeping counted for them stays as it was. The interim (a), defect 2704's
repair, turns the exit audit of the heap blocks off for such a program. **(c), to be judged**: the compiler gives the functions
that can run inside a callback an *extent record* of their counted slots on a thread-local chain; the jump hook walks it from the
jumping frame to the target and releases. The completeness critic then widened the question to what a jump leaves behind in
seven rows (refcounted values, the large-value room, handles, a `spawn` thread's answer, a signal handler's jump, a lent buffer's
region, leases and scratch) and added options (a'), (d) to (h).

## What the sitting found before judging (the critic, then the seats)

- The interim is not "the leak whole": five correct programs end today with a false message or leak outside every gate. The
  coordinator re-ran each: defects **2752** (a large value, 134 and *compiler bug*, 3 of 3), **2753** (a C handle, 134, 3 of 3),
  **2754** (`pthread_exit` in a `spawn` body, 134 and a null read, 3 of 3), **2755** (a `SIGALRM` handler that jumps out, 8 of 8),
  **2756** (a lent buffer, 334,741,504 bytes resident against 2,998,272). All are `blocking` but 2756.
- The first brief's example, *a server decoding corrupt images through libpng loses 3.5 KB each*, was **false for panel 257's
  `pngread`**: 4,000 corrupt images abandoned no Heroes frame (exit 0, 3.49 MB). No program or package of the tree jumps over Heroes
  frames today; the benefit of any resolution is to bindings that do not exist yet.

## Verdicts

| judge | verdict | section | cost named | prediction | condition |
|---|---|---|---|---|---|
| compiler-engineer | **veto of (c)** as specified; **approve with conditions** a two-layer resolution: the runtime's stacks and gates (layer 1), then the values by (d), a forced unwind scoped to a program that hands a function to a foreign extern | design.md §1.12 ("where two admissible forms disagree and one of them can be made to crash, the other wins"), CLAUDE.md § Precedence rank 3 | layer 1 about 200 lines of runtime and 15 of emitter; (d) about 200 of emitter and 140 of runtime; per call, Darwin arm64: **(c) +30.6 instructions (its thread-local test +27.0), (d) −0.4**; (d) costs clang +7.5% at `-O0` and +27.4% at `-O2` on the largest unit, so its scope stays narrow | the commit that lands (d) leaves the compiler's `--emit-c` byte-identical bar the `hero_program_sources` line; `latency compare --clock all` shows no loss past the floor; `leaks --atExit` reads 0 on `leak/main_1k.hero` and exactly 4,000 (exit 0) on his `d6b` | (1) the veto falls if (c) proves every record it releases is alive; (2) the approval of (d) falls if a forced unwind runs a cleanup in a dead frame, or if the box shows UCRT's `longjmp` runs no termination handler; (3) if the author wants no release, the floor is layer 1 alone |
| ffi-pragmatist | **objects to (c) and to (d) as a release *at* the jump; veto on (g)**; supports (a') now and **collect at the jump, release at the return** (forced unwinding as the collector, every owned value moved onto a per-thread list off the thread key, released when the extern call holding the catch returns); handles never consumed by the runtime | design.md §1.11 and §1.12; spec § 13 lines 466-475 (a lend lives for its call; handles, leases) | a release at the hook frees what C still reads at the catch site: a lent message freed there makes a plain build report an empty message with exit 0 and ASan report a heap-use-after-free (`pngmsg_lent`, real libpng 1.6.59), where the interim reports it right, 3 of 3; (g) breaks every binding shape measured at its first jump | `pngmsg_lent.hero` (real libpng 1.6.59, this Mac) prints `the binding reports: the program refused stream 0 at byte 4096` and exits 0, 3 of 3, under today's runtime, under (a') and under a release at (h)'s point (the return of the extern call); under a release at `hero_jmp_jump`, or (d)'s cleanups releasing during the unwind, it prints a different or empty message in at least 1 of 3 plain runs and `--sanitize` reports a heap-use-after-free 3 of 3; falsified by a clean pass under a release at the jump | **spec: yes** for the release rule (what ends a lend and a lease of a frame a C jump left), which sends the sitting to the full panel; (a') as he supports it needs none |

## The resolution adopted (provisional)

**R1, now: the runtime's stacks heal and the gates tell the truth; no hook, no instruction on a call; no spec sentence** (both
seats' floor; the compiler-engineer's layer 1 and the ffi-pragmatist's (a') in its spec-free form):
the large-value room and the lend regions remember the frame that took each entry and give back the entries of deeper frames when
a deeper frame asks (prototype run: the room row goes from exit 134 to 0, 3 of 3; the 21 room goldens that build alone read the same);
the handle, lease and scratch gates keep their aborts, the ones spec § 13 line 473 promises, and say the true cause (a callback's
frames never finished, abandoned by a C jump or a thread's end); `wait` on a thread C ended aborts naming that, never a null read
(Darwin's `pthread_exit` runs no C cleanup, run, so this needs the runtime's own repair whatever else is chosen); a jump out of a
signal handler that interrupted this runtime's allocator stops the program saying so, since POSIX leaves what follows undefined.
**It closes 2752, 2753, 2754, 2755 and 2756's false messages and the lend leak**, and it is the conservative floor of the sitting.

**R2, provisional, pending a spec sentence and the full panel: the values.** (d) scoped, the compiler-engineer's mechanism with the
ffi-pragmatist's timing: where a program declares a foreign extern with a function-typed parameter, each counted slot of a function
reachable from the function handed over is declared `__attribute__((cleanup))`, the unit is built `-fexceptions`, and the hook, where
no signal handler stands, runs `_Unwind_ForcedUnwind` to the save point's frame; **the unwind collects the owned values onto a
per-thread list and the list is released when the extern call that holds the catch returns into Heroes code**, so a value C lent
and reads at the catch (a message, a buffer) outlives the jump. Where the program declares no foreign extern, nothing changes: that
is the compiler's case and every shipped program's. Where unwind information is missing it degrades to today's leak. The release
timing is **read, not run** (the ffi-pragmatist's prototypes run the unwind and count the released slots, 2,000 of 2,000 on Darwin
and on Linux arm64, 0 where the callback was built without `-fexceptions`; the deferred list was not built).

**Not adopted:** (c) as specified (D6, run: a jump the runtime never saw, in assembly here, leaves records in reused stack, and the
next seen jump releases through them: **exit 134 or a silent SIGBUS at 0x4141414141414141, 3 of 3; an address check at the push does not
save it**), where (d) leaks exactly the unseen half and releases the seen; (g), the told refusal (veto: it breaks every measured
binding); (b), audit by subtraction; (a') as the brief wrote it, which still aborts the room row and whose *forgive* branch contradicts
spec § 13 line 473. (e) is R1's healing for the room and lend rows and reaches no value (a slot is on the stack); (h) alone is unsound
unless it copies at the hook, which is (c)'s walk (the compiler-engineer's rows, read, not run); R2 takes (h)'s point for the release only.

## Disagreements, stated plainly

1. **When the values are released.** The compiler-engineer releases in the cleanups during the forced unwind, at the jump; the
   ffi-pragmatist measured with real libpng that this frees a lent value C reads after landing. **The synthesis takes the later
   release: it is the one form that cannot corrupt a correct binding**, and records the earlier as what the compiler-engineer
   supports. The compiler-engineer's own argument of rank 3 decides it: where two forms disagree and one can be made to give a wrong
   answer in silence, the other wins.
2. **Whether a spec sentence is owed.** The compiler-engineer reads a lend as ending with its call, so none for what he supports and
   one only for (a')'s forgiving branch; the ffi-pragmatist finds neither line 466 nor 470 says what ends a lend or a lease of a frame
   a C jump left. **R1 needs none, by both; R2 does**, by the stricter reading, and a spec-warden may differ on the wording for `wait`
   on a thread C ended (R1 reads the spec's silence as an abort naming it).
3. **What (a') is.** The compiler-engineer's (a') is the brief's, which leaves two programs wrong; the ffi-pragmatist's is the
   spec-free one that also tells the truth and repairs the room. R1 is the second.

## What a veto would compel

The compiler-engineer's veto of (c) compels a written answer to D6 (which check makes the walk release only live records after an
unseen jump, including `d6b`, where both jumps happen inside one C call), and *every jump is seen* is false here (assembly), on Linux
(`sigjump.c:66-74`) and on Windows (no hook). The ffi-pragmatist's veto of (g) compels that a jump over a callback stays legal for a
binding. Neither blocks R1 or the (d)-collector.

## The conservative resolution

R1 alone: the stacks heal, the gates tell the truth, the values stay leaked with the heap-block audit forgiven as the interim
does. It is what the author may choose instead of R2.

## What is open and unrun

Nothing ran on Windows (UCRT's `longjmp` and whether it runs termination handlers: the box's question); the compiler-engineer ran on
Darwin only, the ffi-pragmatist on Darwin and Linux arm64 (glibc 2.41, a stand-in library for libpng's error model there, the
container having no libpng); the deferred release list of R2, the lend regions' healing (read, not prototyped), D1 to D4 and D8 of the
critic's doors (read), and (d)'s cost over a whole program and on the compiler's own build are unmeasured. The sitting had no historian,
so no precedent (Rust's `C-unwind`, cgo, OCaml's callbacks) is claimed.

## Predictions to score

The compiler-engineer's and the ffi-pragmatist's (table above), at the landing of R1 and R2 respectively.

## The author's verdict

**Pending.** Decision issue: opened with this sitting as `panel 258`.
