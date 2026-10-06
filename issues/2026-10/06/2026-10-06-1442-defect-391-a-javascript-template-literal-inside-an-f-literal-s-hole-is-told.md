---
kind: defect
area: compiler
milestone: none
filed: 2026-10-06
commit: none
github: none
---

- [ ] **391 — a JavaScript template literal inside an f-literal's hole is told one message per character** | panel 187's R2 mutant `1980e403a92d6cb3` (`interp-js`) writes `` m[`ключ${x}`] `` inside a hole of `print(f"...")` in `tests/golden/run/fixedbugs-an-interpolated-string-holds-characters-above-ascii.hero`: the trunk's compiler at `0f48f9f9` told one message, `hole_not_renderable`, about the hole's type; batch 12's round at `43e6be50` tells seven `unexpected_character`, the opening backtick, each of the four Cyrillic letters, the `$` and the closing backtick, under `check` and under `check --permissive` alike (R2's differential at batch 12's gate, the round arm run by the coordinator from 13:54:11, the differential written at 14:26:36 on 2026-10-06, `FINDINGS: 2`, the two arms of this one mutant) | the lexer's reading of a hole's text, `selfhost/lex_interp.hero`, and its `unexpected_character` told per character where a run is one mistake · panel 187 R2 · **class: adjacent**

    **Origin:** panel 187's R2 instrument, rebuilt after the restart of 2026-10-06, its round arm at batch 12's gate; the mutant's lines read by the coordinator at 14:42.

    **Class: adjacent**, 2026-10-06 (`.claude/rules/verification.md` § Bounded discovery): seven true messages for one mistake, a second message for one mistake; the trunk's one message named the hole's type rather than the backticks. Into batch 13 under the author's instruction of 2026-10-05.
