# Panel 255, round two: one design for both platforms, built and run on each

Written 2026-11-22 after the critic's second reading (`docs/panel/255-reports/critic-reports.md`, 13
findings), and **repaired on the critic's reading of its first draft**
(`255-reports/critic-round2-brief.md`, 7 findings, its 8 repairs each marked *(r2-N)* below).

Round one's seats both approved a route C (*"C3"*), built apart on each platform. The critic re-ran both
from copies: they are two designs, not one; three programs get opposite verdicts; a coroutine whose
stack is an array on the thread's own stack (*shape A*) runs a Heroes function inside the signal under
**both**; each refuses a correct program the other follows; their calibrations fail differently and
silently (critic-reports findings 1 to 6 and 10). It also measured on Darwin that a rule nobody built,
*a swap made while a handler stands suspends that handler*, closes shape A and keeps every other row it
ran (finding 11, the probe `kC5`, Darwin only, never through the harness), and its reading of this
brief found shapes that rule does not yet answer (r2 finding 7). **A route that does not build is not
adopted**, so the sitting builds before it rules. Read, in this order: this file, `00-shared.md` § 6 to
§ 8 (what binds still binds), `critic-reports.md` findings 1 to 13, `critic-round2-brief.md` whole,
and your own seat's round-one report.

**Where the context findings go (r2-5).** Every context finding (shape A, `ctx2`, `x_unseen0` and
`t_unseen0`, the third stack's `m1`) is a Heroes function running inside a signal with no given-back
memory read: defect 2522's class, not 2679's (critic-reports finding 7). The coordinator drafts it as a
`blocking` defect of its own. **Defect 2679 does not wait for it unless it must**: your table keeps a
column for route C closing 2679 alone (below), so the synthesis can land 2679 first wherever that moves
no context verdict toward running inside a signal against today.

## The inputs, copied into `round2/` beside this file (the seats' folders are git-ignored; these are the record)

- `round2/ce-C3/`, `round2/ce-A/`, `round2/ce-Cfail/`: the compiler-engineer's Linux C3
  (`sigjump.c`, `siginstall.c`, `sigguard.c`), A and C-forced-to-fail (`sigjump.c` each; the forcing
  define is `HERO_JMP_FORCE_UNCALIBRATED`, `ce-C3/sigjump.c:315`).
- `round2/fp-C3/` (`rebind.c`, `sigguard.c`, the fragments `entries.c`, `jumps.c`) and `round2/fp-A/`
  (`rebind.c`): the ffi-pragmatist's Darwin C3 and A (r2-2: Darwin's A had no code among the inputs).
- `round2/kC5/`: the critic's probe, the Darwin C3 with five edits marked `critic probe kC5` and a
  `suspended` field in `sigguard.c` (r2-8). It keeps no record (`kC5/rebind.c:120`), has no forcing
  define, and calibrates nothing off arm64 (`kC5/rebind.c:376`, `:468`).
- `round2/shapes/critic/`: `xr.h` (shapes `a_1_0`, `a_2_0`, `a_1_1`, `a_2_1`, `b_0`, `b_1`, `c_0`,
  `c_1`, `d_0`, `d_1`, `e`), `xf.h` (`f`), `xc.h` and its twelve `ctx-m{1,2,3}-o{0,1}-s{0,1}` (the
  engineer's third stack ported to Darwin), `cojump.h` (`cj`), `nest.h` (`ns_0`, `ns_1`), `residue.h`
  and `residue0.h` (`t_partial`, `t_unseen`, `t_unseen0`), and the three 2522 headers `gcb`, `gcopy`
  and `grefill` build against. `round2/shapes/ce/`: `extra.h`, its `x_*` and `ctx-m*`, `xs.c`.
  `round2/shapes/fp/`: `jo_*`, `jumpout.h`, `jolib.c`, libpng's `pngdemo`, and `ctx2/`, `cojump/`,
  `nest/`, `res/` (whose `residue.h` is the one `t_partial` and `t_unseen` build against; the
  top-level `fp/residue.h` is an older header, r2-8), `ctx/`, `fake/`. `ijn`, `ijv`, `ijo`, `ijstale`
  are in `docs/panel/251-reports/evidence.tar.xz` (`00-shared.md` § 2). The runner scripts name the
  folders they were written in: adapt the paths.

## The expectations (r2-1 to r2-4)

`xr.h`, `xf.h` and `extra.h` state theirs in comments (`grep -c MUST`: 6, 1, 15); the other headers
state none, and their expectation is the one critic-reports findings 6 and 11 and the seats' reports
give. **Column *C4*:**

| MUST RUN | MUST BE REFUSED |
|---|---|
| `b_0`, `b_1`; `c_0`, `c_1`, `cj`; `d_0`, `d_1`, `e`, `gcb`; `ns_0`, `ns_1`; `f`; `ctx-m2-*`; `x_static`, `x_heap0`, `x_heap`, `x_tls`, `x_dlopen`, `x_dlclose`, `x_prot`, `x_realloc`, `x_refill_out`, `x_fake`; `jo_heap`, `jo_tls`, `jo_lib`, `jo_co`; `fake` (it tests anything on Darwin only under `ASAN_OPTIONS=detect_stack_use_after_return=1`, r2-8); libpng's `pngdemo` clean; the eight shapes of `repro/` and `ctx/` (exit 0, no sanitizer line) | `a_1_0`, `a_2_0`, `a_1_1`, `a_2_1`; `ctx-m1-*`, `ctx-m3-*`; `c2_1`, `c2_2`; `x_unseen`, `x_unseen0`, `x_partial`; `t_partial`, `t_unseen`, `t_unseen0`; `grefill`, `ijstale`; `ijn`, `ijv`, `ijo` |

**New shapes the rules make decisive (r2-3, r2-4, r2-7)**, each to be WRITTEN by both seats on their
platform from `critic-round2-brief.md` finding 7's description, and run:
- **A′** (MUST BE REFUSED, both orientations): shape A spelled `getcontext` + `setcontext`, the
  coroutine's `uc_link` the handler's context. Read against `kC5`, rule 6 below drops the handler's entry.
- **A by an unseen switch** (MUST BE REFUSED): shape A with the switch in hand-written assembly.
- **N** (MUST BE REFUSED): nested handlers suspended and resumed out of order.
- **F′, F″** (MUST RUN): `f` swapped into once before its helper returns; a coroutine's `mmap`ed stack
  unmapped and mapped again as the alternate stack, then `d_1`. **U** (Linux, if your rule 7 reads
  `uc_stack`): a swap target whose `uc_stack` is stale.
- **G, a handler swapped away from and never resumed**, its three forms (escape by `swapcontext` to a
  pre-signal context; the coroutine `siglongjmp`s out; the coroutine returns through `uc_link` to a
  pre-signal context). **Its verdict is the author's question**: each is correct by the reasoning that
  makes `b_*` MUST RUN, and a rule that suspends refuses it for good; report what C4 gives and why.

**Not yours to decide (r2-1)**: `gcopy` is byte-identical to the tree's copied-buffer case, whose
`.expected` pins a refusal under the author's ruling B. Report its verdict under each column; it and
the refill case's words (`grefill`, `ijstale`) are the two goldens a route C moves, and neither
`.expected` is rewritten by a seat.

**Column *C4 forced to fail* (r2-2)** is A's path: the MUST BE REFUSED column binds it unchanged, and
of the MUST RUN column it is held to `d_*`, `gcb`, `ns_*`, `b_*`, `c_*`, `f` and the 2679 shapes (`xr.h:179`:
the control a failed calibration must still follow); `e`, `x_heap`, `x_tls`, `x_dlopen`, `x_prot`,
`x_realloc`, `x_refill_out`, `x_fake` sanitized, `jo_heap`, `jo_tls`, `jo_lib`, `fake` and `gcopy` are
A's measured known costs, listed as such. Darwin x86-64 is that column by construction and is judged by
reading (no Rosetta here, `00-shared.md` § 7).

**Column *C, 2679 alone* (r2-5)**: the decode of rules 1 to 3 with the context handling of today's
runtime kept as far as the platform allows; on Darwin, where C replaces the entry list, the most
conservative build you can make that moves no context row toward running inside a signal against
today (A is a candidate). It tells the synthesis whether 2679 can land before the context defect.

## The design to build, "C4": one set of rules, each platform's mechanism

1. **The decode**: a jump of the `setjmp` family reads ONE word of its target and decodes the stack
   pointer by the process's key; a context jump reads the context's own stack-pointer field. On the
   decode's path no buffer a save filled is read at any other moment: no fingerprint, no pending pair.
2. **The calibration**, before `main`, takes both seats' strengths (critic-reports finding 10):
   callee-saved registers filled with patterns, the stack-pointer word, a second mangled word and the
   unmangled words verified under one key, at frames far apart on the thread's stack AND on a second,
   `mmap`ed stack, for every save name the platform has.
3. **Where it does not verify, A on both platforms** (critic-reports finding 3 and its must-settle item
   4): the record kept by the save points, its fingerprint read only of a buffer on the thread's stack,
   its current alternate stack, or a writable segment of an image loaded before `main`. **The runtime
   says which path is on**: an accessor a test can ask (a `runtime` suite check or a `run` golden), and
   a build define forcing the failure, run on both platforms. **Rules 4 to 7 bind the A path too**.
4. **Stacks** are the thread's own, the alternate stack registered now, a known context stack, or
   unknown; each entry records its stack's kind when it is pushed. **The classification's order is
   yours to choose and to justify by the shapes** (r2-7): own first lets shape A through by address
   (`fp-C3`), known first refuses `f` (`kC4`, finding 11); F′ and F″ test either.
5. **A swap made while a handler stands suspends it**: every standing entry is marked suspended until
   that swap returns in its own frame, and the suspension is cleared when its handler ends (`kC5`'s third
   part, `kC5/rebind.c:240`, r2-7). **Every prune obeys it**: at a jump, at a swap, at a trampoline's or
   wrapper's end (`kC5`'s `own->depth = at`, line 238, fails shape N), and in a callback's check by its
   own frame; and no prune compares two addresses on two different stacks (r2-3).
6. **`setcontext` and a jump that leaves the alternate stack** (`kC5`'s two shared edits): `kC5` reads a
   `setcontext` as a jump (`b_*` run) and a jump from the alternate stack to ANY other stack as leaving
   it. **A′ refutes the first as written** (a `setcontext` away from a handler may be resumed through
   `uc_link`, by reading): find the rule that runs `b_*` and refuses A′, or show that none can tell them
   apart at the moment of the callback and say which way the design errs (toward the told refusal) and
   which correct program pays.
7. **Known context stacks**: Darwin learns them in its `makecontext` slot; Linux has no `makecontext`
   wrapper (variadic, `runtime/parts/sigjump.c:62`): an assembly wrapper as Darwin's, or what else you
   measure. **glibc 2.41 arm64 does not read `uc_stack` at a swap** (r2 finding 5, measured: only
   `makecontext` does), so a field read there may be stale (shape U). Say what tells the runtime that a
   known range is still a stack, or what a stale range costs (F′, F″).
8. **The union gives the same verdict on both platforms**, each verdict the shape's expectation above.
   Where a shape exists on one platform only (glibc's `ucontext_t` size, Darwin's
   `pthread_get_stackaddr_np`), port it and say so.

What would refute the design, looked for and not only confirmed: a MUST row failed in either column; a
premise of rule 5 (a suspended handler resumed in the frame that suspended it); a correct program today
runs that C4 refuses (the author's, as ruling B was).

## The compiler-engineer: C4 on Linux (`sigjump.c`, `siginstall.c`, `sigguard.c`)

Your copy and your containers as in round one; your report this round is
`docs/panel/255-reports/compiler-engineer-r2.md`, written as you go. Start from `round2/ce-C3/` (read
`round2/kC5/` for rule 5's mechanism). On Linux arm64, stage 2 with each column's runtime: the eight 2679
shapes, the union and the new shapes, `ij*`, libpng in `gtk4-ui`, the case sets of `00-shared.md` § 4
and the `runtime` suite; on emulated x86-64 the 2679 shapes and the decisive rows (`x_unseen`,
`x_partial`, `ijstale`, shape A, A′, `b_1`, `c_0`, `f`).

## The ffi-pragmatist: C4 on Darwin (`rebind.c`, `sigguard.c`)

Your copy as in round one; your report this round is `docs/panel/255-reports/ffi-pragmatist-r2.md`,
written as you go. Start from `round2/kC5/` (and `round2/fp-A/` for the A path, which `kC5` lacks). On
this Mac, stage 2 with each column's runtime: the eight 2679 shapes, the union (the engineer's `x_*`
ported, as the critic ported `xc.h`) and the new shapes, `ij*`, libpng, the case sets and the `runtime`
suite (its census: name each static and the reason it owes).

## Both seats' report, the same table

One row per shape and per case set; the columns *today*, *C4*, *C4 forced to fail*, *C, 2679 alone*,
plain and `--sanitize`, so the critic can `diff` the two platforms row by row; then the diff's size per
file, what it removes, the known costs against today, your verdict (approve, object, veto with the
soundness reason), and **one falsifiable prediction a CI leg can see** (r2-6): the landing owes the
union's shapes as `run` goldens on both platforms (critic-reports finding 8's list and the new
shapes), so name the goldens and what each leg will read. Everything unrun, said. A paid run, a
credential, a GitHub write, a commit, a push: never; no wait over 3 minutes; the harness never piped
into `head` or `tail`; no seat reads the other's folder.
