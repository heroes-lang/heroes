# Panel 255: a jump out of a handler reads only its own target, so no buffer a program gave back is read, and a context switched inside a handler is a defect of its own

**Sat** 2026-11-22 to 2026-11-23. **Lane**: soundness (compiler-engineer, ffi-pragmatist), in two
rounds, with the completeness critic on the briefs before each round and on the reports after it.
**Tree**: `lane-p255` at `a75179641` (`rebel-round`'s head that day). **Status: provisional — author
ratification pending.** **Defect**: 2679 (`blocking`, `area:runtime`, M-rebel-library), on panel 251's
R1. Briefs `255-briefs/` (round one: `00-shared.md`, the seats', `critic.md`, `repro/`; round two:
`round2.md` and its inputs `round2/`), reports `255-reports/`, evidence `255-reports/evidence.tar.xz`.

## The question, and why the soundness lane

Panel 251's R1 (ratified 2026-11-19) validates each save point's record by a fingerprint of the buffer's
bytes *"once the C library has filled it"*. The save reaches the C library by a tail call, so the
runtime first reads the filled buffer at the thread's NEXT save point or jump, and by then the program
may have given the buffer's memory back: freed it (libpng's `png_struct`, the issue's origin through
GTK), unmapped it, or closed the library that held it. Measured at this tree before any seat
(`255-briefs/repro/`, eight shapes, `mac.log`, `linux-arm64.log`, `mac7.log`, `linux7.log`): a correct
C program that unmaps a page it once saved into **segfaults inside the runtime, exit 139, on this Mac
and on Linux arm64, with no sanitizer**; a freed heap buffer is a heap-use-after-free under
`--sanitize` on both; and on Darwin every `longjmp`, a handler running or not, is such a read
(`rebind.c:425`). Lane-core's analysis (`255-briefs/2679-question.md`, verbatim) put three routes: A
(read only memory the runtime can prove lives), B (no read; a generation count), C (record-free on
Linux: the jump decodes its target's stack pointer), and recommended C on Linux, A on Darwin.

**The lane.** The convener's test was the full panel if the spec states the mechanism R1 rules on.
`grep -n -i -E 'setjmp|longjmp|jmp_buf|getcontext|swapcontext|signal handler|handler|fingerprint|save point' spec/heroes-spec.md`
prints lines 427 and 428 only, the RULE (*"A callback is a **parameter**, never a result or a signal
handler, nor called by one"*; *"No function of the program is a signal handler"*), and no word of the
mechanism; R1 itself says *"No spec token"*. So the soundness lane sat. **What it gave up**: no blind
reader was asked whether a program that leaves a handler by a jump is expected to run its callback (both
seats answered by reading lines 427 and 428 instead), no spec-warden priced a sentence (R4 below asks
whether one is owed), and no historian sourced precedent, so the decision issue carries no
`## Prior art`.

## The proposal, verbatim

`255-briefs/2679-question.md` § Recommendation, lane-core's, as put to the seats:

> **C on Linux, with A on Darwin until a seat builds Darwin's decode**: C is the one route that reads no
> memory the program may have given back AND refuses no correct program today's record follows; its one
> premise is checked before `main` and fails toward the told refusal. A is the robust floor where C cannot
> run. What conservative would be: A everywhere, its new known costs (heap, thread-local and late-image
> buffers jumped out of a handler) put to the author as ruling B's were. B is recorded and not recommended.

## The critic's first reading, and what it changed before any seat

`255-reports/critic-briefs.md`, 13 findings, every repair made before a seat started (`00-shared.md`
names each *(critic N)*). What changed the sitting: **a route nobody listed**, T, a token written into a
span of the buffer no save writes (measured spans on both C libraries), and two more (the composition by
calibration, F in assembly); **shapes 7 and 8**, a give-back then a jump outside every handler, which
faults on Darwin alone (re-run by the coordinator, `repro/mac7.log`, `repro/linux7.log`); lane-core's
output found (`rebel-scratch/logs/lane-core-2679.log`) and shown to hold no transformation, and a wrong
slot passing at frames 0x20 apart; musl shown to be no platform any route reaches (the runtime does not
compile there); the CI's legs read (`ubuntu-26.04`, `ubuntu-26.04-arm`, `macos-26`); the platform-split
and spec-sentence questions put to every seat; the ASan fake-stack shape corrected; the baseline counts
of both platforms measured.

## Round one: the verdicts

| seat | route | verdict | section | cost | prediction | condition |
|---|---|---|---|---|---|---|
| compiler-engineer (Linux) | C, then C3 (C with the third stack told apart, A beneath) | **approve** | §1.12; §1.1, §1.7 | runtime only, compiler 0, spec 0; `sigjump.c` 283 to 561 | at the lane that lands C, `ubuntu-26.04-arm` and `ubuntu-26.04` read `fixedbugs-2522` 25 passed, 0 failed with exactly two expectations moved | the key does not verify on the runner, or a correct program C refuses that today runs |
| compiler-engineer | A alone | object (sound; C's fallback only) | §1.12; spec 427, 428 | `sigjump.c` about 340 | | C failing on a CI leg |
| compiler-engineer | T | **veto** | §1.12 | `sigjump.c` 283 to 226 | | `x_unseen` refused under a T |
| compiler-engineer | B veto (by reading, its mechanism run under T); D object; E veto; F in assembly object | | | | | |
| ffi-pragmatist (Darwin) | C3 | **approve** | §1.12, §1.11, §4.19 | `rebind.c` 727 to 737, `sigguard.c` +6 | on `macos-26`, the calibration verifies and `fixedbugs-2522` gives this Mac's C verdicts | |
| ffi-pragmatist | A | object | §1.12 *"any C library must be bindable"* | `rebind.c` 783 | | |
| ffi-pragmatist | T | **veto** | §1.12 | `rebind.c` 697 | | |
| ffi-pragmatist | C1 (Linux's rules carried whole) | veto, superseded by C3 | §1.12 | | | |
| ffi-pragmatist | D, E | object, not built | | | | |

Round one's measured core, re-run by the critic from copies (critic-reports findings 4 and 13): **the
decode closes 2679** on both platforms (the eight shapes 16 of 16 exit 0 clean; libpng clean under
`--sanitize` where today reports a use-after-free on the second PNG, on both); **T runs a Heroes
function inside the signal** on a buffer refilled by a save the runtime never sees (both platforms) and
on a partial copy (x86-64, Darwin); and under every C **the copied-buffer case runs** (`called 8`) while
**the refill case stays refused**, in the true *"had not been seen to end"* words. The Darwin decode,
carried from panel 251 as impossible, was measured to hold: word 12 is `pacdb(sp, 0xcbed)` XORed with
the thread's munge token, decoded by one per-process key and `xpacd` (ffi-pragmatist round one, Q1).
glibc's slot was measured on arm64 (word 13, `xor`) and x86-64 (word 6, `ror 17` then `xor`), and the
probe binary agreed on Ubuntu 26.04's glibc 2.43, the CI's (compiler-engineer round one, Q2).

## The critic's second reading, and what it changed: round two

`255-reports/critic-reports.md`, 13 findings. **The two C3s were two designs, not one**: a coroutine
whose stack is an array on the thread's own stack (*shape A*) ran a Heroes function inside the signal
under **both** (Linux three orientations of four, as today; Darwin two of four, where today refuses
all four); three programs got opposite verdicts; each refused a correct program the other followed;
their calibrations failed differently and silently. And every context finding was **defect 2522's class,
not 2679's**: a Heroes function inside a signal, no given-back memory read (clean under `--sanitize`),
held by no issue and no ruling. Its probe `kC5` (*a swap made while a handler stands suspends it*)
closed shape A on Darwin. A route that does not build is not adopted, so **round two** was convened
(`255-briefs/round2.md`): one design, *C4*, built on each platform by its seat; its brief was read first
by a critic (`critic-round2-brief.md`, 7 findings, 8 repairs: `gcopy` taken out of the seats' hands as
the author's ruling B, the forced column given its own expectations, new decisive shapes A′, N, F′, F″,
U, G and A by an unseen switch, the context defect's filing and a 2679-alone column).

## Round two: the verdicts

| seat | route | verdict | measured | prediction | condition |
|---|---|---|---|---|---|
| compiler-engineer (Linux) | C4 | approve | arm64: 27 MUST BE REFUSED refused, 34 MUST RUN run, both modes; the case sets move only `gcopy` and the refill words; `runtime` suite green; `sigjump.c` 283 to 764, `siginstall.c` 362 to 699 | at the landing, the accessor golden prints `decoding: 3` on both Ubuntu legs; `fixedbugs-2522` 25 passed, 0 failed with the two `.expected` moved | the accessor printing 2 or 0, or a case pinned refused exiting 0 |
| compiler-engineer | **C, 2679 alone** | **approve, as the landing that may come first** | closes 2679's shapes and libpng on arm64; moves no row toward running inside a signal; refuses `c2_1`, `x_unseen0` (today runs them inside the signal); costs `g_3_0`; `sigjump.c` 283 to 671 | | |
| ffi-pragmatist (Darwin) | **C4, as C4r** | **approve** | every MUST row of the C4 column, both modes, union, new shapes, libpng 1.6.59; the harness moves only the two goldens; census needs no entry; refuses five programs today runs inside a signal (`nn`, `rr_0`, `rr_1`, `t_unseen0`, `x_unseen0`) and moves none the other way | on `macos-26`, the route case prints `route 3` plain and `--sanitize`; `nn`, `ap_1_0`, `au_1` refused S, `rr_0` refused U, `b_1`, `c_1`, `fp2`, `sn`, `gcb` exit 0 | object if `macos-26` does not verify, if a union row cannot be agreed with Linux, or if the author rules G, `fp3` or `as_0` must run |
| ffi-pragmatist | C, 2679 alone (A) | object | sound, but refuses thirteen correct programs today runs (`b_0`, `b_1` among them) and keeps today's five residues | | |

## The critic's third reading, and what it changed

`255-reports/critic-r2-reports.md`, 14 findings, both seats' builds re-run from copies on both
platforms with shapes of its own (`gr`, `cv`, `ax`, `cu`, `ub`):

1. **Rule 8 (one set of verdicts) is not met.** The two C4s split on 22 rows; on 15 the shape MUST BE
   REFUSED and the **Linux C4 runs it inside the signal** while the Darwin C4 refuses it (`rr_0`,
   `rr_1`, `gr_0_0`, `gr_1_0`, `ax_0_0_1`, `ax_0_2_1`, nine `cv` rows with a save the runtime never
   sees); none goes the other way (findings 2, 3, 9).
2. **The Linux C4 moves one MUST BE REFUSED row toward running against today**: `ax_0_2_1`, refused
   today, runs under C4 and its forced column (finding 4). This overturns the Linux C4's approval on
   the engineer's own veto ground.
3. **G's real cost**: on Linux, G's own-stack forms run only through the *overwrite proof*, and that
   rule runs `gr` (G resumed after it) inside the signal (finding 5); without it, every G and every `gr`
   is refused and nothing else moves (finding 13). So the author's choice is *G refused on both* or *G
   runs and `gr` with it*, not G by stack.
4. **C, 2679 alone moves no row toward running inside a signal on either platform** (finding 11, every
   shape of the reading); **the Darwin C4 and C4r move none either**; the Linux C4 and its forced column
   are the only columns that do.
5. Residues shared with today: `rr_2` under both C4s; Darwin's realistic failed calibration (the
   `setjmp` family alone on A) runs all four own-stack A″ (`ax_0_*`) inside the signal, as today
   (finding 12), so rule 3 does not hold on Darwin's A path for A″.
6. The two forced columns measure different things (finding 8); Darwin's context stack-pointer word is
   a public field, so Darwin x86-64 contexts could be decoded (unbuilt).

## Where the seats disagree, stated plainly

- **The Linux C4 against the Darwin C4** (third reading, 1 and 2): the engineer approved his C4; the
  critic measured it running fifteen MUST BE REFUSED rows inside the signal that the Darwin C4 refuses,
  one of them refused today. The sitting goes with the measurement: the Linux C4 is not adopted.
- **The 2679-alone landing on Darwin**: lane-core recommended A on Darwin; the ffi-pragmatist objects
  (A refuses thirteen correct programs, `b_*` among them, keeps today's residues), measured, and shows
  that C4r passes the 2679-alone test (critic, third reading 4).
- **A platform split**: the engineer's rule was *a verdict may differ by platform only between runs and
  a told refusal, never toward Heroes code inside a signal*; the ffi-pragmatist's was *no split*, since a
  binding's author cannot test a platform they do not have. Under R1 below the 2679 shapes, libpng and
  every case set agree on both platforms; the context rows split as they split today, Darwin refusing
  where today's Linux runs a Heroes function inside a signal, until R3 lands on Linux.
- **G**: the engineer's C4 ran its own-stack forms by the overwrite proof; the ffi-pragmatist's refused
  all; the critic measured the proof unsound (`gr`). Not a disagreement left open: the proof goes.

## The resolutions (provisional — author ratification pending)

### R1. Defect 2679 closes by route C, the decoded target, on both platforms

**A jump of the `setjmp` family reads ONE word of its target and decodes the stack pointer by the
process's key; a context jump reads the context's own stack-pointer field; on that path no buffer a
save filled is read at any other moment** (no fingerprint, no pending pair): the target lives, since the
C library reads it next. The rest of R1:
- **The calibration** before `main` takes both seats' strengths: callee-saved registers filled with
  patterns, the stack-pointer word, a second mangled word and the unmangled words verified under one
  key, at frames far apart on the thread's stack and on a second, `mmap`ed stack, for every save name
  the platform has (glibc arm64 word 13 `xor`, x86-64 word 6 `ror 17` then `xor`; Darwin arm64 word 12,
  `pacdb` then the munge token).
- **Where it does not verify, route A** (a fingerprint read only of a buffer on the thread's stack, its
  current alternate stack, or a writable segment of an image loaded before `main`): glibc's four other
  architectures, a future C library, Darwin x86-64. **The runtime says which path is on**, by one
  accessor with one name on both platforms (the seats built `decoding: N` on Linux and
  `hero_signal_jump_route()` on Darwin; the landing takes one) asked by a `run` golden, and a forcing
  define run on both platforms in the lane.
- **How it lands, per platform, on what was measured**:
  - **Linux: *C, 2679 alone***, the engineer's `CE/p255/r2/routes/C2679/sigjump.c` (283 to 671 lines; a
    landing splits it past §11's ~300): the decode with today's context handling. Measured to close
    2679's shapes and libpng on arm64, to move no row toward running inside a signal against today
    (engineer round two § 3.4; critic third reading 11), to refuse two today runs inside the signal
    (`c2_1`, `x_unseen0`), and to cost one program, `g_3_0` (G, R4). x86-64 rows of this build: unrun,
    owed by the lane (round one's C ran the shapes and the decisive rows there).
  - **Darwin: C4r**, the ffi-pragmatist's `F/p255/r2/C4r/`: Darwin's C must replace today's list of
    entries threaded through the trampolines' frames, which would itself be read after its frame died,
    so Darwin has no 2679-alone build short of A; C4r is measured to meet every MUST row of the C4 column,
    to move no row toward running inside a signal (critic third reading 4), and to refuse nine programs
    today runs inside one (the seat's five, and the four own-stack A″ the critic found today's Darwin
    running, third reading 7). Its costs against today are R4's.
- **The 2679 shapes, libpng and every case set read the same on both platforms**; the context rows
  differ as they differ today (R3).

### R2. Ruling B's known cost is lifted by the decode: the copied buffer runs, the refill stays refused

Under R1, `fixedbugs-2522-a-correct-jump-out-of-a-handler-through-a-copied-buffer-is-refused-in-words-that-say-so`
runs (`called 8`), on both platforms and every C built, and
`fixedbugs-2522-a-buffer-a-handler-refills-by-memcpy-and-jumps-to-is-refused` stays refused, its words
moving from *"no record of"* to the true *"had not been seen to end"*. Two `.expected` move and the first
case's name stops being true; the author's ruling B of 2026-11-20 accepted that refusal as a known cost,
so this is theirs to read. **Recommendation: accept**: the decode reads the buffer the C library is about
to act on, so the copy is followed for what it is, and no seat or critic found a shape the change makes
unsound.

### R3. A context switched inside a handler is a defect of its own, `blocking`, and its design is Darwin's C4r carried to Linux

Every context finding is a Heroes function running inside a signal with no given-back memory read:
defect 2522's class, named in `sigjump.c:59` to 66 as its rows, and held by no issue, row or ruling
(critic second reading 7). Measured under **today's runtime**: on Linux arm64, shape A in three
orientations of four, A′ in three, A by an unseen switch, `ctx2` both ways, the third stack's `m1` in
three orientations, `n`/`nn`, `x_unseen0`, two own-stack A″ (`ax_0_0_*`) and the R shapes (`rr_*`, then
a fault or ASan's report in the program's own undefined jump back); on Darwin, `nn`, `rr_0`, `rr_1`,
`t_unseen0`, `x_unseen0` and all four own-stack A″ (`ax_0_*`, critic third reading 7); `rr_2` on both. **Filed apart** (its draft is the coordinator's, unfiled). **Its design, provisional**:
the rules C4r carries on Darwin, carried to Linux: per-entry suspension at a swap made while a handler
stands, released only where the runtime sees the handler run again; a `setcontext` into a `makecontext`
range holding the entries off it; a jump that lands below the frame making it read as re-entering a
handler; no prune by a trampoline's, wrapper's or callback's own frame; no overwrite proof; known
context stacks learned where they are made (Linux by an assembly `makecontext` wrapper, glibc reading
`uc_stack` nowhere else). **Not adopted on Linux as built**: the Linux C4 runs fifteen MUST BE REFUSED
rows Darwin refuses and one today refuses (R1's evidence; critic third reading 1 and 2). It lands when
the Linux build meets the union on both platforms: one header per shape and one `.expected` for both
(the seats' names collide: `ap_*`, `au_*` and G's forms are different programs in the two reports),
the union as `run` goldens. Residues left by design and unbuilt, each shared with today: `rr_2` (the
ffi-pragmatist's per-range *bottom*, recorded where the thread leaves a range), a save the runtime never
sees (`apu_*`, `cv`: answer `dlvsym` as the author's ruling C answers `dlsym`, the engineer's), and
Darwin's `setjmp`-family-forced A″.

### R4. The correct programs R1 and R3 refuse that today runs: told, and the author's to read

On Darwin, C4r refuses `g1_0`, `g1_1`, `g2`, `g2_1` (G, a handler swapped away from and never resumed),
`fp3` and `as_0` (and `cs_1`, which today refuses too); on Linux, *C, 2679 alone* refuses `g_3_0`; R3's Linux build would refuse every G form and
`ub_*` (a callback above a handler left by an inline jump, which today's Darwin already refuses), and,
where contexts fall to A, `b_*`. **Recommendation: G refused on both platforms**, since *G runs* means
`gr` runs inside the signal (critic third reading 3), and each refusal told in words true of it.
**Whether the spec owes a sentence**: by both seats' reading of lines 427 and 428, a refusal of a
program that has left its handler owes the reader words; this lane has no seat to write or price them
(the author's amendment of 2026-11-22 lifts the budget's reason not to), so the decision asks the
author: a sentence through a sitting with the spec-warden, or the refusal told in the runtime's words
alone as ruling B's was.

### R5. What the landing owes

The eight 2679 shapes and libpng (where `pkg-config` answers) as `run` goldens on both platforms; the
accessor's golden; the forcing define run in the lane on both platforms; the two `.expected` of R2;
the `runtime` suite's census reasons (Darwin: `hero_rebind_records`'s `SHARED_BY_DECISION` entry reworded,
measured unwritten by the ffi-pragmatist); `sigjump.c` and `rebind.c` split by concern past §11's ~300;
x86-64 runs of Linux's build (emulated here, the `ubuntu-26.04` leg after the push). R3's union, when
it lands, as named there.

## What conservative would have been

**A on both platforms** for 2679, no decode premise: sound by every run, closes 2679's shapes and
libpng, and refuses correct programs today follows, a correct jump out of a handler through a heap,
thread-local or late-loaded buffer (both platforms), `x_prot`, `x_realloc`, `x_refill_out`, `x_fake`
under `--sanitize`, and on Darwin `b_*` and nine more; it keeps today's context residues and owes the
spec a sentence. Both seats object to it as a route and approve it only as the decode's floor; it is
recorded so the author can choose it.

## The prior art, per resolution

None: the historian did not sit (soundness lane). ThreadSanitizer's `setjmp` records, which panel
251's prior art names, are lane-core's word in the question and were not re-read by a historian here.

## Predictions to score

| seat | prediction | checkable at |
|---|---|---|
| compiler-engineer, round one | `ubuntu-26.04-arm` and `ubuntu-26.04` read `run (only fixedbugs-2522): 25 passed, 0 failed` with exactly two expectations moved (refill words; copied buffer runs, `called 8`), every other set unchanged, the eight 2679 shapes as `run` goldens exit 0 at `-O0`, `-O2` and both sanitized; falsified by the refill case reading the *"no record"* words | the push that carries the landing |
| compiler-engineer, round two | the accessor golden prints `decoding: 3` on both Ubuntu legs; every new 2679 and context golden passes in all four builds; falsified by 2 or 0, or a case pinned refused exiting 0 | the push that carries the landing |
| ffi-pragmatist, round one | on `macos-26`, the calibration verifies and `fixedbugs-2522` gives this Mac's C verdicts (the twelve `a-callback-after-*` unchanged, the copied buffer runs, the refill refused in S words, none in unsure words) | the push that carries the landing |
| ffi-pragmatist, round two | on `macos-26`, the route case prints `route 3` plain and `--sanitize`; `nn`, `ap_1_0`, `au_1` refused S, `rr_0` refused U, `b_1`, `c_1`, `fp2`, `sn`, `gcb` exit 0, with this Mac's `.expected` unchanged | the push that carries C4r with the union as goldens |

## What this sitting files and owes

- **A decision issue**, `panel 255`, drafted unfiled at
  `.claude/worktrees/rebel-scratch/coord/issues/p255-decision.md`: R1 to R5 for the author's reading,
  R2's ruling B and R4's G and spec sentence named as the author's.
- **A defect**, the context class of R3, `blocking`, drafted unfiled at
  `.claude/worktrees/rebel-scratch/coord/issues/p255-defect-context.md`, with today's measured shapes
  on both platforms.
- **Defect 2679** stays open until the landing of R1 is gated; its implementation is a lane after the
  sitting, never this one.

## How the sitting went, for the next

- **Lane-core's scratch folder was gone** when the briefs were written; its transcript survived in
  `rebel-scratch/logs/lane-core-2679.log`, which the first critic found by `-iname '*2679*'` and the
  coordinator's `find` had not (it searched for the question's file names, `probe_mangle*`,
  `ijstale.c`, `linux-arm64.log`, and `ijstale.c` was inside panel 251's evidence archive). The coordinator wrote and ran the reproducers again
  (`255-briefs/repro/`), so every number of the shared brief is this sitting's.
- **The `fixedbugs-2494` filter counts 15 cases**, not the brief's 14: the brief counted `.hero` files
  with `ls | grep -c`, the harness's own count line read 15 (both seats).
- **Two API limits** stopped the round-one compiler-engineer (15:54, resumed 17:25), the round-two
  compiler-engineer (about 21:20, resumed 21:42) and the round-two ffi-pragmatist; each was resumed from
  its own report file with its context.
- **The second critic reading was first given to the first critic's finished agent**, against the
  author's rule of that day (CLAUDE.md § 3: a new task goes to a new agent, never to a finished agent's
  long context); the coordinator stopped it before it wrote anything and a fresh agent took the reading.
- **Each seat held one platform and could not read the other's folder**, the right rule for independence
  and the reason the two C3s and then the two C4s diverged unseen until a critic ran both: three of the
  sitting's four decisive findings were the critic's. A sitting that asks one design on two platforms
  owes a cross-run step inside the seats' own round.
- Every container the sitting started was removed; the seats' copies hold only their untracked `p255/`.

## Author's verdict

*Pending.*
