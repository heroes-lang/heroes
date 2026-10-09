---
kind: defect
area: parse
milestone: none
filed: 2026-10-07
commit: 9107d98db56b5248d271ffb7502cdbbf18b5e568
github: none
---

- [x] **459 — an outer list never closed around an inner list the lexer closes elsewhere is told twice for one `]]` edit** | the never-closed outer `[` keeps its own report beside the binding's message naming the inner `[`, two messages for one edit, deliberate under defect 307's rule; merging them needs the outer opener's report carried to the binding's message across reports being cut (lane b14-parse, 313's six shapes on the base, none spurious) | `selfhost/parse/unclosed.hero`, `selfhost/closers.hero` · defects 313 and 307 · **class: adjacent**

    **Origin:** filed by the coordinator at 18:37 on 2026-10-07 from lane b14-parse's final report; the lane's measurement, not re-run by the coordinator.

    **Class: adjacent**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): a second message for one mistake, true each.

    Repaired at `9107d98d`, 2026-10-09 (lane b17-fix, batch 17), gated by its cases and the compiler's own tests; the net is owed at the batch's close. A binding's report below a list ended there names every opener the list stands in on its line, the ones the lexer named never closed too, with the closers the one edit writes (`]]`, `])]`, `]}`, `}]`), and carries their spans (`diag.Diagnostic.carries`), so `parse/unclosed.withdrawn` withdraws the lexer's report of each where that report is said; a closer of another kind where a separator goes keeps defect 307's rule. Cases `check/fixedbugs-459-*` (six shapes) and `full/fixedbugs-459-*`, both red on the base; `check/fixedbugs-313-*` moved deliberately from 18 reports to 13; `check` 639, `full` 29, `permissive` 16, the compiler's own tests 1,532, all passed; the whole compiler's `check` 72.60 G instructions to 72.77 G (+0.23%).

## The repair

Repaired at `9107d98d`, 2026-10-09 (lane b17-fix, batch 17), gated by its cases and the compiler's own tests; the net is owed at the batch's close. A binding's report below a list ended there names every opener the list stands in on its line, the ones the lexer named never closed too, with the closers the one edit writes (`]]`, `])]`, `]}`, `}]`), and carries their spans (`diag.Diagnostic.carries`), so `parse/unclosed.withdrawn` withdraws the lexer's report of each where that report is said; a closer of another kind where a separator goes keeps defect 307's rule. Cases `check/fixedbugs-459-*` (six shapes) and `full/fixedbugs-459-*`, both red on the base; `check/fixedbugs-313-*` moved deliberately from 18 reports to 13; `check` 639, `full` 29, `permissive` 16, the compiler's own tests 1,532, all passed; the whole compiler's `check` 72.60 G instructions to 72.77 G (+0.23%).

**Closed 2026-10-09** with batch 17 (lanes b17-fix, b17-emit and b17-check, landing panels 200 and 201 as ratified that evening, merged into one round tree made from the trunk), under the optimistic chain (`.claude/rules/verification.md` § The optimistic chain), its closing gate run on the round at `46b80b82`: the seed regenerated over two generations, the runtime's ABI moved to 30 by defect 470, 47,052,134 bytes, SHA-256 beginning `2b668ee89a3697cd`, its fixpoint by `cmp`, the committed seed of ABI 29 built against the runtime it was emitted for; the compiler's own tests 1,542, all passed; the net's own tests 326, all passed; the full net 7,306 passed over 29 suites, 0 failed, `run` in four shards of the harness's own (defect 537), 426 of 426 cases read, one skipped on this Mac (defect 437's case, `sys/prctl.h`); one floor told outgrown, `full`, raised in the closing commit as defect 536's rule asks. The census and panel 187's R2 run after the push, beside the CI.
