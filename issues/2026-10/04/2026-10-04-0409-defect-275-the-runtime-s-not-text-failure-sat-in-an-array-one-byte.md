---
kind: defect
area: golden
milestone: none
filed: 2026-10-04
commit: cf7d6efa3456c4657ccd6eab6b5c1f6287e62b03
github: none
---

- [x] **275 — the runtime's `not_text` failure sat in an array one byte short, so its message reads *these bytes are not valid UTF-* and C reads past it** | `b: [u8] = [99, 97, 102, 233]` and `b.validated_bytes()`: `.err` with `e.msg` *these bytes are not valid UTF-* and `len(e.msg)` 30, where the runtime's text is 31 letters; `.must()` on it panics *... not valid UTF-8*, `fprintf`'s `%s` reading the 31st letter and on to the first zero byte past the array (batch 8's round compiler, this Mac, 2026-10-04, `tests/golden/run/fixedbugs-275-a-failure-message-keeps-its-last-letter.hero`); the other seven static texts of the runtime measured right, `does_not_fit`'s 39 letters in 40, `missing_key`'s and *no such key* in 12, `true` and `false` | `runtime/parts/failure.c:104` (`hero_failure_not_text`: `char b[31]` for a 31-letter literal, which C fills with no terminator and no diagnostic) · `HERO_STR_STATIC` and `HERO_STR_LIT` (`runtime/heroes_runtime.h:186`), which size a text by its own literal · **class: blocking**

    **Origin:** panel 189's compiler-engineer, 2026-10-04, its Q6 (*a program sees the `validated_bytes` error message cut off*, `docs/panel/189-reports/compiler-engineer.md`); the cause found in the array's size and measured by the coordinator, with the seven texts beside it.

    **Class: blocking**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): a false message, a word cut, and a `str` without its terminator lent to C, which reads past the array it was given.

## The repair

Repaired at `cf7d6efa`. Every static text of the runtime is `HERO_STR_STATIC`, sized by `sizeof` of its own literal, and `HERO_STR_LIT`, so no number about a text is typed: `not_text`'s message keeps its 31 letters and its terminator. Case: `run/fixedbugs-275-a-failure-message-keeps-its-last-letter`, the three failures and a bool's two words with their lengths, its emission blessed.

**Closed 2026-10-04** after batch 8's platform legs ran its cases on the tree that closes (`7ec19cb7`, the code of the batch's closing commit): Linux arm64, Debian clang 22.1.8, the compiler's tests 1,158, all passed, and its 20 suites at 0 failed; the same image under clang 18, the compiler's tests 1,158, all passed, and its 20 suites at 0 failed; the Windows box, clang 23.1.1, the compiler's tests 1,158, all passed, and its 20 suites at 0 failed (the folder removed from the box, 20 of 20 green). The batch's gate on this Mac is the closing commit's body and the thirteen records closed with it.
