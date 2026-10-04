- [ ] **247 — a file in UTF-16 without a byte order mark, holding ASCII only, is well-formed UTF-8 and is told in thirty messages, one for each NUL** | `function main()` over `    print(1)` in UTF-16 LE with no BOM: `check` exit 1 with 30 diagnostics, 29 `unexpected_character` *the control character U+0000* and one `expected_declaration` (batch 8's round compiler at `1eb854c3`, this Mac, 2026-10-04, `<scratchpad>/filings-b8/probe/u16.hero`; the trunk's, panel 189's ffi-pragmatist and compiler-engineer); the raw NULs in its excerpts are defect 244 | the lexer's `unexpected_character`, told once for each control character of one cause (`selfhost/lexer.hero`) · **class: adjacent**

    **Origin:** panel 189's ffi-pragmatist, 2026-10-04 00:17 (`docs/panel/189-reports/ffi-pragmatist.md`, *One shape beside*), not 227's cause, the file being UTF-8; filed apart by the coordinator.

    **Class: adjacent**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): one mistake, the file's encoding, told thirty times.

    Repaired at `cdc79563`, 2026-10-04, gated by its cases and the compiler's own tests; the net is owed at the batch's close.
