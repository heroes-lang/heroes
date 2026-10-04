- [ ] **227 — a source file holding a byte that is not UTF-8 is answered `cannot read` at exit 2, where the file was read and the author can be told which line holds the byte** | `function main()` over a comment `# caf` and the byte 0xE9, over `print(1)`: `check` exit 2, *error: cannot read `p.hero`*; the same with the byte inside a string literal (the trunk's compiler at `826ddc2f`, 2026-10-03, `<scratchpad>/repro-utf8/`) | the reading of a source file into a `str` · `.claude/rules/cli-surface.md` (*exit 1 the input has diagnostics, exit 2 the tool could not run*) · **class: blocking**

    **Origin:** panel 188's ffi-pragmatist, 2026-10-03, beside its sweep (its case `bad-utf8`); reproduced by the coordinator the same day.

    **Class: blocking**, 2026-10-03 (`.claude/rules/verification.md` § Bounded discovery): an exit 2 where the author can be told, and a false message (the file was read).

    Repaired at `5e4f2efb` and `649adb9b`, 2026-10-04, panel 189's resolution as provisional, gated by its cases and the compiler's own tests; the net is owed at the batch's close.
