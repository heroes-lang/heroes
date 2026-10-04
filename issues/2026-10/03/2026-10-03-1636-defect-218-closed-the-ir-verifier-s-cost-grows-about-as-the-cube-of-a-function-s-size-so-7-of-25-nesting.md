---
kind: defect
area: ir
milestone: none
filed: 2026-10-03
commit: 9bd02fcaeb9d66049caeda38afccc609bace4fce
github: none
---

# Defect 218 closed: the IR verifier's cost grows about as the cube of a function's size, so 7 of 25 nesting shapes at 2,000 deep do not build in 150 s on this Mac, 8 in the Linux container

- [x] **218 — the IR verifier's cost grows about as the cube of a function's size, so 7 of 25 nesting shapes at 2,000 deep do not build in 150 s on this Mac, 8 in the Linux container** | lane depth's 25 shapes (panel 184's thirteen and twelve beside them) at N = 2,000: `check` and `fmt` hold every one, `build` of seven does not finish in 150 s; the cost the lane traced to `released_on_return` in `selfhost/ir/phases.hero` and `dominators` in `selfhost/ir/values.hero` (`<scratchpad>/lane-depth/pass1-findings.md`) | those two functions · panel 184's R6 · **class: blocking** · **closed 2026-10-03**

    **Origin:** lane depth, 2026-10-03, measuring panel 184's R6 on this Mac and in the Linux arm64 container, reported to the coordinator; not yet run by the coordinator.

    **2026-10-03, lane irverify, the verifier reads a function in time near its size, every old reading kept in its tests as the oracle**: repaired at `6fd89a45`, gated by its own cases; the rest is owed at the round's gate. Measured on this Mac: the seven shapes at 2,000, six of them cut at 300 s on the trunk's compiler, build in 2.07 to 23.75 s of user time; what is left is the emitter's and the ownership pass's, apart.

    **Class: blocking**, 2026-10-03 (`.claude/rules/verification.md` § Bounded discovery): panel 184's R6, ratified, owes spec § 9 a sentence the lane drafted, *a source nested up to 2,000 deep compiles on every platform*, and it cannot be written true while these builds do not finish; a cost that stops a needed program from building at all is compiler need (CL-006).

    **Closed 2026-10-03** at the round of 2026-10-03's seventh gate, `da0ff6c9` (lane irverify, repaired at `6fd89a45`): the seed regenerated once, 39,956,525 bytes, SHA-256 beginning `3bc3aa8bd2b5ba65`, its fixpoint by `cmp`; the compiler's own tests 1,113 and the net's own 200, all passed; the full net, 26 suites, 5,049 passed and 0 failed; the census of `check --brief` over 1,910 tracked files and of `--emit-c` over 595, the trunk's compiler against the round's: no file moved, no byte of C and no message, so the verifier's verdicts are the trunk's on the whole corpus. The Linux fact its entry names, read in the Linux arm64 container with the round's compiler under Debian clang 22.1.8: all 25 of lane depth's shapes at 2,000 deep (`gen.py source 2000`), each built one after another under a 150 s cut, built and printed their outputs, `paren-open-2000` refused at exit 1 as a wrong program should be, the whole loop 1 minute 49 s by the launcher's clock (16:17:29 to 16:19:18, the compiler's own build from its seed included; each shape's time unrecorded, the image having no `bc`), where the entry read eight shapes past 150 s there. The round's Linux arm64 legs, its suites four at a time: under clang 22.1.8 and again under 18.1.8, the compiler's own tests 1,113 all passed and 20 suites at 0 failed, the two `run/fixedbugs-218-*` cases among `run`'s 258 (no `extern`, so never skipped). It closes at the round's gate (`.claude/rules/verification.md` § The batch), its Linux reading in hand.
