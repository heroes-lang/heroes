---
kind: defect
area: process
milestone: none
filed: 2026-10-03
commit: 5e4f2efbedfc9474fafd4bb1158bf5e59dac8364
github: none
---

- [x] **227 — a source file holding a byte that is not UTF-8 is answered `cannot read` at exit 2, where the file was read and the author can be told which line holds the byte** | `function main()` over a comment `# caf` and the byte 0xE9, over `print(1)`: `check` exit 2, *error: cannot read `p.hero`*; the same with the byte inside a string literal (the trunk's compiler at `826ddc2f`, 2026-10-03, `<scratchpad>/repro-utf8/`) | the reading of a source file into a `str` · `.claude/rules/cli-surface.md` (*exit 1 the input has diagnostics, exit 2 the tool could not run*) · **class: blocking**

    **Origin:** panel 188's ffi-pragmatist, 2026-10-03, beside its sweep (its case `bad-utf8`); reproduced by the coordinator the same day.

    **Class: blocking**, 2026-10-03 (`.claude/rules/verification.md` § Bounded discovery): an exit 2 where the author can be told, and a false message (the file was read).

    Repaired at `5e4f2efb` and `649adb9b`, 2026-10-04, panel 189's resolution as provisional, gated by its cases and the compiler's own tests; the net is owed at the batch's close.

## The repair

Repaired at `5e4f2efb`, with `649adb9b`, on panel 189's resolution R1 to R7, ratified by the author's reading of 2026-10-04 (*2a*, `f6a3122e`). A source file that is not UTF-8 is told `not_text` at exit 1 by every verb that compiles or prints a program, one diagnostic per line holding such a byte, at most eight with the rest counted, each byte named with its place, and the file is never lexed and never handed to a writer; a `use`d module the same at its own file; `mutate` keeps exit 2 and its corpus message, `measure` counts lines exactly, and the runtime is keyed file by file. Its cases are five `check/fixedbugs-227-*`, `run/fixedbugs-227-the-shown-read-names-every-byte` with its blessed emission, and `surface-fixtures/notext227`.

**Closed 2026-10-04** after batch 9's platform legs ran its cases on the tree that closes (`38d6c6b1`, the code of the batch's closing commit): Linux arm64, Debian clang 22.1.8 and the same image under clang 18.1.8, the compiler's tests 1,190, all passed, and its 21 suites at 0 failed under each; the Windows box, clang 23.1.1 on Windows Server 2025 at code page 1252, the compiler's tests 1,190, all passed, and its 21 suites at 0 failed, `annotations` read again on `0470afd5`'s file after the floor's revision (its first read, 678 passed and 1 failed, was the floor counting the asked marks alone, six stepped aside there for headers the box lacks). The batch's gate on this Mac is the closing commit's body and the twenty-two records closed with it.
