---
kind: defect
area: check
milestone: none
filed: 2026-10-10
commit: 7e602cb82a0d231520d0d17f0805c48effeceb41
github: none
---

- [ ] **574 — `print({})` is offered *annotate the binding* where there is no binding** | defect 553's repair offers an empty map a map's annotation, `m: {str: i64} = {}`; for `print({})`, an empty map with nothing to type it and no binding, the fix still says *annotate the binding* | the empty-literal fix of `selfhost/check/` (defect 553's repair, `4156ee29`) · **class: adjacent**

    **Origin:** found by lane b18-close beside defect 465 (its final reply and notes, `.claude/worktrees/scratch-b15/b18-close/notes.txt`, ignored by git), filed by the coordinator at 04:29 on 2026-10-10.

    **Class: adjacent**, 2026-10-10 (`.claude/rules/verification.md` § Bounded discovery): a true message less exact than it could be.

    Repaired at `7e602cb8`, 2026-10-10 (lane b18-close), gated by its cases and the compiler's own tests; the net is owed at the batch's close. Where an empty literal stands its guess binds it first, *bind it first with its type, `m: {str: i64} = {}`, and use `m` here* (`contextless_errors.hero`); where it is the whole value of a binding with no annotation, `check/empty_binding.hero` has the guess annotate that binding under its own name, *annotate the binding: `counts: {str: i64} = {}`*. Case `full/fixedbugs-574-…` (5 shapes, red on the base); defect 553's full case moved on its four other shapes, corrected beneath; `check` 650, `full` 33, `permissive` 16, the compiler's 1,546 tests, 0 failed.
