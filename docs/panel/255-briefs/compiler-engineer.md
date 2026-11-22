# Panel 255, compiler-engineer

Read `00-shared.md` first, whole (repaired after the critic's first reading; read
`docs/panel/255-reports/critic-briefs.md` findings 1 to 10 too), then `2679-question.md`, panel 251's § R1
(`docs/panel/251-a-handlers-jump-is-judged-by-a-validated-record-a-thread-left-running-ends-main-as-the-programs-mistake-and-a-whole-value-crossing-waits.md`,
`grep -n '^### R1'`), and your own report of that sitting, `docs/panel/251-reports/compiler-engineer.md`
lines 585 to 592, where you preferred a record-free J (*"the target's frame read from the buffer itself,
glibc's saved stack pointer demangled by its pointer guard"*) *"if a seat builds it and it refuses the
stale and `memcpy`-within shapes without refusing the `memcpy`-out one"*. You judge the ceiling
(design.md §1.1, §1.7, Part 5): what each route costs in `runtime/`, and **whether it is sound**, measured.
Your veto is a refusal on soundness; a cost is reported.

**Your copy**: `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/p255-scratch/compiler-engineer/`,
a detached worktree at `a75179641`. Build there as `00-shared.md` § 7 says; your work goes under an
untracked `p255/` folder in it; keep each route's runtime as files there (`p255/C/sigjump.c`, ...) and copy
them into `runtime/parts/` before a build, restoring the tree's own with `git show HEAD:<path>` when done.
**Your report**:
`/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/lane-p255/docs/panel/255-reports/compiler-engineer.md`,
written as you go. **Your platform is Linux** (`sigjump.c`, glibc): arm64 in `heroes-linux-arm64:latest`
and its `gtk4-ui` twin, x86-64 in the emulated `heroes-linux:latest`. The ffi-pragmatist holds Darwin
(`rebind.c`); you never read its folder. Never the Rust bootstrap.

Work in this order; what you cannot reach is said *unrun* in your report.

## Q1. Linux arm64's baseline, today's runtime

In the container, stage 2 built there: the eight shapes (`repro/run.sh`, all eight by default), and through
the harness `fixedbugs-2522`, `fixedbugs-2558`, `fixedbugs-2571`, `fixedbugs-a-c-handler-that-recovers-leaves-no-line`,
the handler filters `00-shared.md` § 4 lists, and the `runtime` suite (`-- ./heroes runtime`), each count
line whole from a file. This is the baseline every moved verdict on Linux is read against.

## Q2. The premise C rests on, re-run (lane-core's output holds no transformation)

A probe that saves with `setjmp`, `_setjmp` and `sigsetjmp(,0)` and `(,1)` at known stack pointers in
frames **far apart** (say how far; a wrong slot agreed at 0x20, `00-shared.md` § 2), at `-O0` and `-O2`:
which slot holds the stack pointer and by what transformation, per architecture (arm64 native, x86-64
emulated); one key per process, the same on a second thread and after `fork`, and for `siglongjmp`'s and
`__longjmp_chk`'s buffers; whether any other slot can pass a calibration that verifies a KNOWN slot at two
far frames. Run the same probe binary in `ubuntu:26.04` where `docker pull` answers (the CI's glibc),
else unrun.

## Q3. Build route C on Linux

A calibration before `main` (a constructor, never a handler, no lock inside a handler) verifying the known
slot per architecture; a jump that reads ONE word of its target, decodes it and drops what
`hero_sig_prune` says that address left; context jumps reading `uc_mcontext`'s stack pointer. Then, with
the diff's line counts: whether the save points stay defined on Linux at all and what removing them moves
(`ffi_defined_elsewhere`, `selfhost/cli/supplied*.hero`; `siglookup.c:134` to 145; the `runtime` suite's
census); what becomes of the `jump_*` fields and `HERO_SIG_JUMPS`; the header comment's residue list line
by line under C. **What the calibration's failure gives** (glibc's four other architectures, a future
glibc): the question's *keep every entry* (the told refusal of every jump out of a handler), or A, or T
(`00-shared.md` § 5, the composition): say which is the most robust, by a case run with the calibration
forced to fail. **Two stacks**: panel 251's ffi-pragmatist chose Darwin's entry over the frame because
*"a record of the entry stays right across two stacks ... where comparing addresses would not"*
(`251-reports/ffi-pragmatist.md:72-77`), and `hero_sig_on_own` lumps the alternate stack with any third
one (`00-shared.md` § 4): a case on a third stack (a `makecontext` context swapped into from inside a
handler on the alternate stack and back), both orders of the two stacks' addresses, under today's runtime
and C, telling a refusal for the `swapcontext` pre-fill cause from one for the cause tested.

## Q4. Build route T on Linux

The save point writes an identifier into a span glibc never writes (`00-shared.md` § 5 gives the critic's
measured spans on arm64; measure x86-64's), verified before `main` as C's slot is, and a jump reads that
span of its target only. Settle (a) to (f) of § 5 by runs where a run can settle them: glibc's jumps
reading or checking the span (`__longjmp_chk`); a partial copy; a stale identifier (a generation per
thread); contexts; a buffer filled by a save the runtime never sees; and say, as a soundness seat, whether
the runtime writing into memory the program handed the C library is sound, with the reason.

## Q5. Build route A on Linux, the conservative arm

A fingerprint read only of a pending buffer inside memory the runtime can prove lives (the thread's stack,
its alternate stack, the writable segments of images loaded before `main`, `dl_iterate_phdr` in a
constructor). The sanitizer shape that moves (critic 8): a buffer held by a frame that does NOT save,
which a callee saves into, sits OFF the thread's stack under `--sanitize` with ASan's default options on
Linux (the critic measured `0xfbff8d9f0020`); run a correct jump out of a handler through such a buffer
under A, plain and `--sanitize`.

## Q6. The same inputs under every route you built (today, C, T, A)

On Linux arm64: Q1's whole list; `ijstale.c`, `ijv.c` (its nested SIGUSR2 shape above all), `ijo.c` and
`ijn.c` from `docs/panel/251-reports/evidence.tar.xz`; and the shapes beside (CL-061), at least: a correct
jump out of a handler through a buffer on the heap, in `_Thread_local` storage, and in a library
`dlopen`ed after `main`; a `static` buffer of a library `dlclose`d before the next save; a page made
`PROT_NONE` after the save; a struct holding a buffer moved by `realloc`; a buffer refilled by `memcpy`
outside a handler and jumped to inside one. Name every verdict that moves against Q1's baseline and its
direction; the refill case must stay refused, the copied-buffer case is reported as it lands. **libpng**,
the issue's origin, in `heroes-linux-arm64:gtk4-ui` (1.6.48, the critic's `pkg-config`): a Heroes program
decoding a PNG through libpng and one that takes its error path (a truncated file), under each route.
Under emulated x86-64: the probe and the eight shapes under C and T; the cases under the route you argue
for in the background where it ends in time, else unrun.

## Q7. Routes B, D, E, F in assembly, and any you find

Judge each from the code and say what would have to be true for it to be more robust than C or T; build
one only where you believe it could be. Lane-core's case against B is lane-core's: check it.

## Your report

A table per route and platform: the shapes, the case sets with their count lines, the extra shapes and
libpng, exit and verdict; the diff's size; what it removes; your answers to `00-shared.md` § 6's two
questions (a verdict that depends on the platform; a sentence owed to the spec); your verdict per route
and platform (approve, object, veto, with the soundness reason for a veto); **one falsifiable
prediction** with the moment it becomes checkable (the lane that lands the route, the CI's
`ubuntu-26.04` and `ubuntu-26.04-arm` legs after the push); and everything unrun, said so.
