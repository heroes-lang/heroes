---
kind: defect
area: process
milestone: none
filed: 2026-10-06
commit: a7513e79d944adb1f1c2b377d1572f969c7aa994
github: none
---

- [ ] **384 — the layer-0 hook reads a compiler older than its tree as the file not parsing** | a file holding `"a\u{1b}b"`, panel 192's escape, written under a tree whose `./heroes` predates it: `.claude/hooks/fmt_check.py` exits 2 with *does not parse* and the old compiler's *this language has no escape for one by its code*, two false sentences, where the round's compiler formats the same file at exit 0 (run by the coordinator at 09:48 on 2026-10-06, the trunk's compiler at `0f48f9f9` standing in a scratch tree); lane cli12 met it on `tests/harness/suite_surface.hero` after merging lane str192 | `.claude/hooks/fmt_check.py:87` to `:118`, no age asked of the compiler before its verdict is read; `.claude/hooks/guard_bash.py`'s compiler-age check is the one layer 1 asks · defects 254 and 348, the same hook and guard choosing which compiler · **class: improvement**

    **Origin:** lane b12-cli12, 2026-10-06 (its final report, *the write hook's compiler is older than this lane*); reproduced and filed by the coordinator at 09:49.

    **Class: improvement**, 2026-10-06 (`.claude/rules/verification.md` § Bounded discovery): an instrument's notice, no program moves, as defects 254 and 348 were classed; one repair could answer the three, the tree a written file stands in and the age of that tree's compiler asked before its verdict.

    Repaired at `a7513e79`, 2026-10-07 (lane b14-hooks), gated by its cases, the hooks' own tests; the net is owed at the batch's close. Every refusal of `.claude/hooks/fmt_check.py` asks first whether the judging compiler was built before a source of its tree, the written file left out, and tells such a refusal as that age with the rebuild, never as *does not parse*; seven cases in `.claude/hooks/test_hooks.py`, four red before the repair, and 0f48f9f9's compiler in a scratch tree of today's sources, on a module holding `"a\u{1b}b"`, read *does not parse* on the base hook and its age on the repaired one.
