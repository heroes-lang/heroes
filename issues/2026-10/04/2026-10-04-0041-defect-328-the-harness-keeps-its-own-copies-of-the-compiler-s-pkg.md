---
kind: defect
area: harness
milestone: none
filed: 2026-10-04
commit: 6e01041809acc7e37972293f71f6180eaa3da806
github: none
---

- [ ] **328 — the harness keeps its own copies of the compiler's pkg-config spellings, and three of them now lag the compiler** | `absence.notes_of` joins pkg-config's notes in the compiler's old order, trim then split, which defect 285 changed to split then trim; `suite_golden.hero:702` and `absence.hero:849` plant defect 249's old bare sentence; and defect 292's new headline, *is installed, and pkg-config cannot give its flags*, is no spelling the harness reads, so a case whose package is installed and broken would read red rather than be stepped aside by name (no leg is known to hold one) | `tests/harness/absence.hero` (`notes_of`, `:849`) · `tests/harness/suite_golden.hero:702` · defects 249, 285 and 292 · **class: improvement**

    **Origin:** lane b10-cli, 2026-10-04, reproduced on its compiler at `86b29733` (its final reply's *Found beside*); its proposed `surface` rows for 290, 295 and 248 are part of this item's remedy.

    **Class: improvement**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): hardening of the net: no case reads wrong today.

    Repaired at `6e010418`, 2026-10-07 (lane b14-harness-a), gated by its cases and the net's own tests; the net is owed at the batch's close. `absence.hero` holds every spelling the net reads a missing library by, and a test reads all 17 out of the compiler's files that write them; `notes_of` reads `\r\n` before it trims, writes a control character by its code and words nothing said as the compiler does, the bare `pkg-config` sentence is planted as the compiler writes it since defect 249, and defect 292's headline is read as an absence whose witness asks the machine both halves. Measured on the base's module over the compiler's own words under a stand-in `pkg-config`: the installed headline 0 absences, the `\r\n` and tab answers refused, the silent one rebuilt `pkg-config said:`; after, the net's own tests 294, all passed, `unsupported` 188 and `annotations` 846, each 0 failed. The `surface` rows lane b10-cli proposed for 290, 295 and 248 are not in the repository; this lane wrote its own for the coordinator, `suite_surface.hero` being lane b14-harness-b's.
