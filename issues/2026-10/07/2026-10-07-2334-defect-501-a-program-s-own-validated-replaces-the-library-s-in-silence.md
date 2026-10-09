---
kind: defect
area: resolve
milestone: none
filed: 2026-10-07
commit: 9c72d0b5ff64a7d2ce26aceeeb161b9a48c08a91
github: none
---

- [x] **501 — a program's own `validated` replaces the library's in silence** | panel 162's text says `validated` moves into the table of built-ins; it is still on `resolve/builtin_names.hero`'s exception list, so a program's own `validated` silently replaces the library's in its module; reserving it needs `emit/bytes_text.hero`'s own `validated` renamed (lane b14-resolve) | `selfhost/resolve/builtin_names.hero`, `selfhost/emit/bytes_text.hero` · defect 455 · panel 162 · **class: improvement**

    **Origin:** filed by the coordinator at 23:34 on 2026-10-07 from lane b14-resolve's final report; the lane's measurement, not re-run by the coordinator.

    **Class: improvement**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): a rule the compiler enforces missing one name a sitting named.

    Repaired at `9c72d0b5`, 2026-10-09 (lane b15-emit), gated by its cases and the compiler's own tests; the net is owed at the batch's close. The compiler's own function of that name is `bytes_text.emit`, the exception and the test of its premise are gone, and a program's `validated` is refused `builtin_name_taken` at a top-level function, a constant, a parameter, a type parameter, a local, a loop variable and a match binding (`tests/golden/check/fixedbugs-501-a-program-s-own-validated-is-refused-everywhere`), where on the base the function was told nothing and its call printed the program's own text at exit 0. A field and a variant case of the name stay legal. `check` over the 3,047 tracked programs moved this case alone.

## The repair

Repaired at `9c72d0b5`, 2026-10-09 (lane b15-emit), gated by its cases and the compiler's own tests; the net is owed at the batch's close. The compiler's own function of that name is `bytes_text.emit`, the exception and the test of its premise are gone, and a program's `validated` is refused `builtin_name_taken` at a top-level function, a constant, a parameter, a type parameter, a local, a loop variable and a match binding (`tests/golden/check/fixedbugs-501-a-program-s-own-validated-is-refused-everywhere`), where on the base the function was told nothing and its call printed the program's own text at exit 0. A field and a variant case of the name stay legal. `check` over the 3,047 tracked programs moved this case alone.

**Closed 2026-10-09** with batch 16 (lanes b16-emit, b16-runtime, b16-land199, b15-parse, b16-tools, b16-compiler, b16-land198 and b16-misc, merged into one round tree made from the trunk), its closing gate run on the round at `164e699a`: the seed regenerated over two generations, the runtime's ABI staying at 29, 46,097,621 bytes, SHA-256 beginning `d3bf3451eae3f3e8`, its fixpoint by `cmp`; the compiler's own tests 1,531, all passed; the net's own tests 322, all passed; the full net 7,237 passed over 29 suites, 0 failed, `run` taken one harness process per case and its one red the skip ratio of a one-case run (defect 437's case, which binds `sys/prctl.h`, skipped on this Mac as in every whole run). It is the first batch under the optimistic chain (author instruction 2026-10-09, `.claude/rules/verification.md` § The optimistic chain): the census and panel 187's R2 run after the push beside the CI, no platform leg ran before it, and a defect at the C boundary closes here, a CI leg red on its case filing a new `blocking` defect naming it. A trial of the same gate on the round at `2dbbf7e6`, before lane b16-misc's four repairs, read the full net 7,226 and 0, the census's every move attributed and R2's one finding read as defect 457's rule speaking.
