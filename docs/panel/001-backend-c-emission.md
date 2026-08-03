# Panel 001 — Backend: emit C11 via clang, replacing QBE (RETRO-RECORD)

Status: **decided by the author, 2026-08-03** (during planning, reaffirmed at
plan approval). This is a retro-record: the objections below are the real ones
raised in review, not staged dissent.

## Decision

The backend emits C11 from the three-address IR (one `goto`+label per basic
block, `#line` directives, `__builtin_*_overflow`), compiled by clang. QBE is
removed as primary and **scheduled** as a second backend post-fixpoint.

## Grounds (the admissible ones)

- §1.11 is the founding constraint and named the tension explicitly: "QBE
  makes the FFI harder than emitting C would."
- §4.19 names Nim's `importc` as "the design to steal from if the backend ever
  changes to C emission" and "the cheapest FFI mechanism that exists."
- With C emission, clang verifies FFI signatures **against the real header** —
  the project's thesis (plausible mistakes become compile errors) applied to
  the FFI boundary, which QBE structurally cannot give.
- `#line` restores source-level debugging, deleting a non-goal.
- Precedent: cfront, Nim, Vala, Chicken Scheme, Cython, early Haskell.

## Grounds ruled INADMISSIBLE

"clang -O2 is the full LLVM optimisation pipeline" — true, but Part 2 says
performance is not a goal, not a tiebreaker. The optimisation column is a
side effect, never the justification (compiler-engineering review, panel 000).

## Objections on record

- **Teaching value lost** (compiler-engineer): no contact with instruction
  selection/register allocation. Mitigation: the QBE backend is *scheduled*
  post-fixpoint from the same IR (~500 lines); `clang -S` / `-emit-llvm -S`
  remain a window; spike 02 carries the basic-block lesson.
- **Compile speed** (compiler-engineer): clang -O2 on generated C is slower
  than QBE; paid three times per fixpoint check. Accepted.
- **"IR agnosticism proven by construction" was an overclaim**: one backend
  proves nothing. Corrected — the claim is only redeemable by actually
  building the second backend, hence *scheduled*.

## Consequences applied

design.md §1.11 (tension resolved), Part 2 (debugger non-goal reworded), §3.1
(rewritten), §3.2 (QBE moved to alternatives), Part 7 item 14 (QBE scheduled),
Part 8 (+warts), Part 10 step 7, appendix (C-emission lineage promoted).
