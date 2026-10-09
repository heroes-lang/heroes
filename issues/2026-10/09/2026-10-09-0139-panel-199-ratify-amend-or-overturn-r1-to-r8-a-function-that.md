---
kind: decision
area: check
milestone: none
filed: 2026-10-09
commit: self
github: none
---

- [x] **panel 199** | ratify, amend or overturn R1 to R8 (`check` refuses, as `endless_recursion`, a function every path of whose body reaches a call of itself, a call before it counting as a way out only when its callee may end the program, R4, a thesis rule with no fix; recursion too deep aborts at every level, sibling calls kept as calls at `-O2`, so spec `:280` holds; clang's `-Winfinite-recursion` no longer passed on once the rule lands, closing defect 507; no spec sentence; the cases, the landing on its own trunk, the platforms; 457, 508 and 507 closed by them, mutual recursion and function values filed apart; (B), (C), (K), (D), R1, R2, R3, (H), (L) and (J) refused) | `docs/panel/199-a-function-that-can-only-call-itself-is-refused-and-recursion-too-deep-aborts-at-every-level.md`

    **Origin:** panel 199's synthesis, 2026-10-09 from 01:37, on the tree frozen at `56def9b4`, convened for defect 457.

    **Recommendation: ratify R1 to R8**, on what was built and run: R4 built and gated by the compiler-engineer (554 lines, `check` +0.97% on the compiler, the census moving only panel 173's two probes), refusing the four plausible endless functions R3 lets through and sparing the five correct programs R1 refuses; (G) measured to turn every `-O2` hang of the sitting into the abort `:280` promises, at +0.11% on `check`; the critic measured that (F) does not close 507 and that silencing clang's warning, the checker its witness, does. The spec-warden's objection on Principle 0 (no measured rate of the mistake) stands recorded.

    **The conservative alternative, the author's to choose instead**: no new refusal, (G) alone for 508, and 507 left open with clang's warning kept; or R3 in place of R4. **And the language's question**: unbounded recursion an abort (R2, recommended) or a loop (N, unbuilt).

    **Verdict, 2026-10-09:** **ratified**, R1 to R8, the author answering through the question widget between 01:39 and 04:07 by the clocks read before and after (*Ratifica R1-R8*), and to the language's question, *si ferma sempre*: unbounded recursion is an abort, R2, over the loop. Recorded as a reading (CLAUDE.md § 4).
