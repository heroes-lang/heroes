---
kind: defect
area: golden
milestone: none
filed: 2026-10-04
commit: 155603c703c7f738afb1cddd88f7e63efb1e0497
github: none
---

- [ ] **271 — a fix that applies away from its message's line says neither in the message nor in `check --json` where it applies** | defect 178's run, three statements each ending with `,`: one `expected_end_of_line` at 13:10 carrying *fix (certain): delete the `,`* three times, none saying its line, and `check --json` gives each fix `title`, `replacement` and `certainty` and no place, the trunk's schema as well (batch 8's round compiler at `1eb854c3`, 2026-10-04, `tests/golden/check/fixedbugs-178-a-comma-ending-every-line-is-one-message-a-run.hero`); panel 187's instrument read the one pair whose second comma moved from its own message into the first's fixes (`tests/golden/run/fallible.hero`, two `arm-comma` mutants, lines 31 and 32) | `selfhost/diag_render.hero` (a fix's line) · `selfhost/cli/check_json.hero:37` to `:40` (a fix's fields; its schema is a tool surface, CLAUDE.md § 4) · **class: adjacent**

    **Origin:** the coordinator, 2026-10-04, reading panel 187's instrument at batch 8's gate.

    **Class: adjacent**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): a true message less exact than it could be: an author fixing by hand meets the next comma only after the first, and a tool reading `--json` cannot apply a fix it cannot place.

    Repaired at `1697cec1`, 2026-10-05, its message half: the rich form names a fix's place where it is not its message's line, gated by its case and the compiler's own tests; the net is owed at the batch's close. The JSON half stays open for a sitting: `check --json`'s fix carries no place, and its schema is a tool surface (CLAUDE.md § 4), so the item stays open.

    Repaired at `155603c7`, 2026-10-06 (lane cli12), its JSON half, gated by its cases and the compiler's own tests; the net is owed at the batch's close. Panel 193's resolution, provisional: every fix of `check --json` carries its place (`file`, `line`, `col`, `end_line`, `end_col`, `byte_start`, `byte_end`, `text`), a certain one its `first_round`, every answer `"schema": 2` and its `"kind"`, a clean one a document, and the files read with their SHA-256; the terms in design.md §4.17, the help, `llms.txt` and both errors pages held to the writer's schema. Writing only the `written` fixes and asking again equals `check --apply` on 399 of 399 roots and 143 of 143 made CRLF.
