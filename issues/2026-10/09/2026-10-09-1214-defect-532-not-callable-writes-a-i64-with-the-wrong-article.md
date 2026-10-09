---
kind: defect
area: check
milestone: none
filed: 2026-10-09
commit: 4186d8ad7f3628b2b68fb3dfcef7692a869eac71
github: none
---

- [ ] **532 — `not_callable` writes `a i64` with the wrong article** | the message reads *this is a `i64`, and a `i64` cannot be called* (lane b16-compiler) | `not_callable`'s message, `selfhost/check/` · **class: adjacent**

    **Origin:** filed by the coordinator at 12:14 on 2026-10-09 from lane b16-compiler's final report (its notes `.claude/worktrees/scratch-b15/compiler/notes.txt`, ignored by git); the lane's measurement, not re-run by the coordinator.

    **Class: adjacent**, 2026-10-09 (`.claude/rules/verification.md` § Bounded discovery): a true message less exact than it could be.

    Repaired at `4186d8ad` and `86bcb31a`, 2026-10-09 (lane b16-misc), gated by its cases and the compiler's own tests; the net is owed at the batch's close, and since the second commit is in `selfhost/emit/ffi*`, the push's platform legs too. The class is every message that wrote `a` before a type, a record, a handle or a C member, found by grep: eleven checker sites in eight modules and the FFI emitter's in four, which on the base also read *a `SA` built here*, *comparing a `AL`*, *a `Inner`*, *has a `x`*. A record's name is the program's and no spelling says how it is said, so each sentence is shaped to need no article (*this value's type is `i64`*, *its payload is itself fallible, `i64?`*, *a handle of type `Image`*, *comparing `AL` values*, *cannot be declared `Pt`*, *has a member `x`*) rather than given one chosen by its first letter. What the grep still finds is right: the declaring keywords (`function`, `constant`, `record`, `variant`), `widths`' article for an integer chosen by how it is said, and a quote character. Two new cases, a `check` one of ten messages and a `full` one for a note, red on the base; ten `check` and thirty `unsupported` expectations moved, every line read.
