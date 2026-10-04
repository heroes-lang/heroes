- [ ] **297 — `suite_lines.hero`'s `unquoted` undoes only two escapes, so a `#line` name holding a line end would be misread** | `tests/harness/suite_lines.hero:242` reads a `#line` name back undoing `\\` and `\"` alone; since defect 240's repair the emitter also writes `\n`, `\r` and `\?` in such a name, and before it this reader misread every name above ASCII; no tracked path holds those bytes today (lane b9-emit, 2026-10-04, a reading) | `tests/harness/suite_lines.hero:242` · defect 240's spelling, `selfhost/emit/c_text.hero` · **class: improvement**

    **Origin:** lane b9-emit, 2026-10-04 (its reply's *found beside*).

    **Class: improvement**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): an instrument that would misread a name no tracked path holds; no program moves.
