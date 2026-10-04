---
kind: defect
area: golden
milestone: none
filed: 2026-10-04
commit: none
github: none
---

- [ ] **271 — a fix that applies away from its message's line says neither in the message nor in `check --json` where it applies** | defect 178's run, three statements each ending with `,`: one `expected_end_of_line` at 13:10 carrying *fix (certain): delete the `,`* three times, none saying its line, and `check --json` gives each fix `title`, `replacement` and `certainty` and no place, the trunk's schema as well (batch 8's round compiler at `1eb854c3`, 2026-10-04, `tests/golden/check/fixedbugs-178-a-comma-ending-every-line-is-one-message-a-run.hero`); panel 187's instrument read the one pair whose second comma moved from its own message into the first's fixes (`tests/golden/run/fallible.hero`, two `arm-comma` mutants, lines 31 and 32) | `selfhost/diag_render.hero` (a fix's line) · `selfhost/cli/check_json.hero:37` to `:40` (a fix's fields; its schema is a tool surface, CLAUDE.md § 4) · **class: adjacent**

    **Origin:** the coordinator, 2026-10-04, reading panel 187's instrument at batch 8's gate.

    **Class: adjacent**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): a true message less exact than it could be: an author fixing by hand meets the next comma only after the first, and a tool reading `--json` cannot apply a fix it cannot place.
