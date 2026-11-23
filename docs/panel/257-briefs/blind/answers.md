# The answers, never given to a reader

**The t2 words are shortened on purpose** (the critic's B6, 337 of 809 characters): the parenthetical of causes is the cue the arms' own lists repeat, and the last sentence of the real message is an instruction to the program; both are out, equally in every arm, so D3 reads a message no build prints exactly. A run that wants the real words quotes them whole and registers that change.

Each program is a file of `programs/`. **Today** is the frozen `runtime/` of `828567a30` on this Mac
(Darwin arm64), `plain`, built by the lane's stage 2 and run once under `timeout 20` with its output into
files (the coordinator, 2026-11-23, `blind/programs/`; the run is `coord/blindrun/` of the sitting's
scratch). **Adopted** is what panel 255's ratified rules say (R1 to R4) and what the landed build must be
graded against: **the grading is the landed build's own run, on each platform**, and a program whose verdict
differs between the two platforms is graded by its own row and named.

| program | what C does | today, Mac plain | adopted (the ratified rule) | spec claim it tests |
|---|---|---|---|---|
| p1 | `siglongjmp` out, a static buffer | RUNS (exit 0, `result 70`) | RUNS | C1 |
| p2 | `siglongjmp` out, a buffer in the heap | RUNS | RUNS | C2 |
| p3 | `siglongjmp` out through a whole copy of the saved buffer | ABORTS, words *"jumped to a buffer this runtime holds no record of"* (exit 134) | **RUNS** (R2: the copied buffer runs) | C3 |
| p4 | a jump no C library function makes (`__builtin_longjmp` on x86-64, hand-written assembly on arm64: the golden's code with its comments taken out) | ABORTS, S words (exit 134) | ABORTS, S words | C4 |
| p5 | the handler returns, then the program calls | RUNS | RUNS | the control: a handler that ended |
| p6 | the handler calls the program's function itself | ABORTS, S words (exit 134) | ABORTS | line 427's rule, *nor called by one* |
| p7 | the handler `swapcontext`s to where the program was before the signal, never resumed (G) | **RUNS** (exit 0) | **ABORTS** (R4: G refused on both platforms; Linux only after defect 2702) | C7 |

**p8, p9 and p10 are the critic's, for the seats, not in t1** (p8 a jump that lands within the handler: ABORTS S, on all four cells the critic ran; p9 a jump to the first of 40 saved buffers: RUNS, but ABORTS in U on Linux today; p10 the handler refills a buffer and leaves by it: ABORTS in U today, RUNS on both adopted builds).

**t2 is graded by the landed build's run of three real programs that do not exist yet** (the ffi-pragmatist's item 3c), so the key below is the ratified rule's prediction and **UNRUN**: **A** RUNS (the callback is called after a `siglongjmp` out of the handler);
**B** ABORTS (G); **C** RUNS (the handler returns). The design a reader should ask for: **A or C**, B being
the one that cannot be followed. **Whose mistake for B**: *neither*: the library's C is correct, the
language cannot follow it. A reader who says *the library's C is wrong* has read the abort as an accusation
of C, which the sentence under test must not teach (the golden's name: *refused in words never false*).
