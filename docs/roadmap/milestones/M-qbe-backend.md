# M-qbe-backend — the proof that the IR is not C in disguise


**Scheduled, no warrant**, and its warrant is stated here more honestly than
"a second backend" ever did. Two things were bought when panel 001 replaced QBE
with C emission, and one of them was never paid for: **as long as exactly one
backend exists, "the IR is target-agnostic" is an assertion no artifact tests**,
and the IR could be a C pre-processor wearing an abstraction's name without
anything in this repository noticing. QBE from the same IR (~500 lines) is what
turns that sentence into a measurement — and it restores the register-allocation
and instruction-selection lesson, which is the half of a compiler this project
deliberately handed to clang (DESIGN-LOG 2026-08-03, panel 001).

**A comparison read on 2026-10-25, and this row's first item.** Goose's
implementation notes open with what a second implementation must preserve and
what it need not (github.com/aardappel/goose, [goose/docs/implementation.md](https://github.com/aardappel/goose/blob/master/docs/implementation.md)): "the
same evaluation, mutation, results and required runtime failures ... It does
not require the same AST, specialization count, stack numbering, optimization
thresholds, generated C text, or diagnostic wording." The fixpoint here
compares emitted C byte for byte, which a second backend cannot share, so the
sitting that opens this row writes that contract first, before a line of the
backend.
