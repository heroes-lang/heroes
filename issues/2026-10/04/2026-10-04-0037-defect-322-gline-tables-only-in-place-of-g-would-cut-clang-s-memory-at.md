---
kind: defect
area: cli
milestone: none
filed: 2026-10-04
commit: none
github: none
---

- [ ] **322 — `-gline-tables-only` in place of `-g` would cut clang's memory at `-O2` on a function of many returns by more than two fifths, a route nobody listed** | panel 190's completeness critic, 2026-10-04, by clang's own peak footprint at `-O2` with `flags()`'s sixteen words: on the 400-return shape the trunk's C needs 1,695,369,184 bytes under `-g` and 970,589,672 under `-gline-tables-only` (−42.7%), and route A-star's 1,985,595,312 and 913,409,056, below the trunk's; at 800 unrun; what it costs, read and not run: C-level variable inspection in lldb, which design.md Part 2 describes and does not promise, while line stepping, `-g`'s one reason in `flags.hero`, is kept | `selfhost/cli/flags.hero` (`flags()`, its `-g`) · `.claude/rules/generated-c.md` § Flags · design.md Part 2 (`:630` to `:632`) · panel 190's R11 · **class: improvement**

    **Origin:** panel 190's completeness critic, second pass, 2026-10-04 (`docs/panel/190-reports/completeness-critic.md` § 6, `<scratchpad>/190-critic/pass2/fpx.sh`).

    **Class: improvement**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): a cost on extreme shapes, nobody's program wrong for it; the flag list touches every program's debug information, a question for a sitting of its own (CLAUDE.md § 4, architecture).
