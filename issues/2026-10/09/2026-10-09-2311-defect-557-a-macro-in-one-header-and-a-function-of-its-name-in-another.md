---
kind: defect
area: emit
milestone: none
filed: 2026-10-09
commit: cac4fc0b607b583f04653138d5bf6e86a8710600
github: none
---

- [ ] **557 — a macro in one header and a function of its name in another are told that one header does not compile** | the critic's `macro`: `a.h` `#define twice(x)`, `b.h` `static inline int twice`; `test` exits 1 with the old false message *`b.h` ... does not compile: expected identifier or '('*, and with the `use` lines swapped the program passes every tool: a false message, and a verdict that depends on `use` order | `selfhost/cli/headers_together.hero` · defects 538 and 550 · panel 202 · **class: blocking**

    **Origin:** filed by the coordinator at 23:11 on 2026-10-09 from panels 202 and 203's completeness critic, first pass (its report committed with the sitting, its cases under `.claude/worktrees/scratch-b15/critic-202-203/p202/`, ignored by git); the critic's measurement, not re-run by the coordinator.

    **Class: blocking**, 2026-10-09 (`.claude/rules/verification.md` § Bounded discovery): a false message, the shape beside defect 550.

    Repaired at `cac4fc0b607b583f04653138d5bf6e86a8710600`, 2026-10-10, gated by its cases and the compiler's own tests; the net is owed at the batch's close. A header clang refused in a unit is compiled alone on the failure path, and where it compiles, after each header its unit read before it (`cli/header_alone.hero`): the first it does not compile after is named, *`b.h`, the header this group names, compiles alone and not after `a.h`*, as 538's class, `ffi_header_refused`; the critic's `macro` in `test` is told so, and the other order passes, a one-unit `test`'s verdict until defect 560 makes it per module. Case `unsupported/fixedbugs-557-*` (one module, red on the base); unsupported 203 and 0, the compiler's own tests 1,548 passed.
