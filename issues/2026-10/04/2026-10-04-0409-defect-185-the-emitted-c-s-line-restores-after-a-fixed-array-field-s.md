---
kind: defect
area: golden
milestone: none
filed: 2026-10-02
commit: 1a4bb3442f31aed79e2f67aece9154fcc4916ee2
github: none
---

- [x] **185 — the emitted C's line restores after a fixed-array field's assertion name a line one too low per such field** | `heroes build tests/golden/run/ffi-a-char-field-becomes-text.hero --emit-c`: line 20 is `#line 19 "ffiacharfieldbecomestext.c"`, so line 21 is reported as 19, and every later restore is 2 short; over `tests/emission`, 261 of 36,781 restores in 19 files are 1 to 10 short, each file's shortfall equal to its number of two-assertion lines | `selfhost/emit/extern_field.hero:158`, `:161` (a `"\n             _Static_assert(` the printer does not count) · `.claude/rules/generated-c.md:27-29` · **class: adjacent**

    **Origin:** lane round1002b at its gate, 2026-10-02 (*not chased*); measured by the coordinator's file-queue agent on `62d65e48` (2026-10-02, `scratchpad/file-queue/line-restore/`) and re-read on `6bec7c8c` by the coordinator's re-verification agent (2026-10-02, `scratchpad/file-queue/reverify-6bec7c8c.txt`).

    **Class: adjacent**, 2026-10-02 (`.claude/rules/verification.md` § Bounded discovery): a place less exact than it could be in every clang note, sanitizer frame or debugger line past such a field; no value moves.

    **2026-10-03, lane b8-emit, each assertion of an array field is its own counted line, and the `lines` suite asks every restore of the generated file for its own line**: repaired at `1a4bb344`, gated by its cases and the compiler's own tests; the net is owed at the batch's close. All 261 short restores in 19 traces were the 40 continuation lines above them, none from another writer; a C-boundary file (`extern*`), so it closes after the push's platform legs.

## The repair

Repaired at `1a4bb344`. Each assertion of a fixed array field is its own counted line under the field's `#line`, so no restore below it names a line one too low; the `lines` suite asks every restore of the generated file for its own line. The census read 22 emitted C files move by their `#line` markers alone, the assertions otherwise byte for byte. Cases: `run/fixedbugs-185-*`, three.

**Closed 2026-10-04** after batch 8's platform legs ran its cases on the tree that closes (`7ec19cb7`, the code of the batch's closing commit): Linux arm64, Debian clang 22.1.8, the compiler's tests 1,158, all passed, and its 20 suites at 0 failed; the same image under clang 18, the compiler's tests 1,158, all passed, and its 20 suites at 0 failed; the Windows box, clang 23.1.1, the compiler's tests 1,158, all passed, and its 20 suites at 0 failed (the folder removed from the box, 20 of 20 green). The batch's gate on this Mac is the closing commit's body and the thirteen records closed with it.
