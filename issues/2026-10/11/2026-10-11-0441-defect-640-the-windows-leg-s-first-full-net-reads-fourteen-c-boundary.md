---
kind: defect
area: emit
milestone: none
filed: 2026-10-11
commit: none
github: none
---

- [ ] **640 — the Windows leg's first full net reads fourteen C-boundary cases red** | `harness: 8020 passed, 20 failed` on Windows x86-64, fourteen of them the C boundary's: `unsupported/fixedbugs-555-*` (2: the program builds, `ffi_declared_two_ways` not told), `fixedbugs-556-*` (3: told without the header's line), `fixedbugs-562-a-header-only-inline-*` (a header-only C99 inline links there and prints 6), `fixedbugs-591-*` and `fixedbugs-592-*` (6: `labs(x: i64)` is `ffi_parameter_type` first, Windows' `long` being 32 bits), and `surface` rows *a header-only C99 inline is refused by run as by build* and *one C symbol carries two bindings, in two modules* (`twoarity`, exit 1 where 0 is owed); causes to be told apart on the Windows box: a case not portable, judged where its shape holds or rewritten portable, or a Windows defect of the reader | `selfhost/cli/` (`two_ways`, `defined_twice`, the link readers), the cases under `tests/golden/unsupported/` and `surface-fixtures/`; the Windows box · **class: blocking**

    **Origin:** filed by the coordinator at 04:41 on 2026-10-11 from the CI's Windows x86-64 leg of batch 20's push (`4fb8bd98`, run 38101444166, job 114358017019, its log through the API at `.claude/worktrees/scratch-b15/ci-4fb8/`, ignored by git), the first run of that leg to reach the net past the compiler's own tests (1,607 passed there), as `.claude/rules/verification.md` § The optimistic chain item 5 asks.

    **Class: blocking**, 2026-10-11 (`.claude/rules/verification.md` § Bounded discovery): a red CI.
