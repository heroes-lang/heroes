---
kind: defect
area: records
milestone: none
filed: 2026-10-06
commit: 0a4b0340317d3011aab4e126b64d4da062e8851b
github: none
---

- [ ] **378 — no instrument reads a commit's message for a raw invisible character** | the body of `fe3f788e` holds one raw U+200B, the write tool having decoded a written `\u200b`, defect 356's hazard; the `unseen` suite that 356 added reads files, not commit messages (counted by the coordinator at 03:08 on 2026-10-06; found by lane b12-str192) | `tests/harness/suite_unseen.hero` · the commit guard, `.claude/hooks/guard_bash.py` · **class: improvement**

    **Origin:** lane b12-str192, 2026-10-06, found beside panel 192's landing (its report's *found beside*); filed by the coordinator at 03:08.

    **Class: improvement**, 2026-10-06 (`.claude/rules/verification.md` § Bounded discovery): hardening: a commit's body is outward-facing and append-only, and nothing reads it.

    Repaired at `0a4b0340`, 2026-10-07 (lane b14-hooks), gated by its cases, the hooks' own tests and the net's own tests; the net is owed at the batch's close. The `unseen` suite's list is data, `REFUSED` in `tests/harness/suite_unseen.hero`, and the commit guard reads it from there (`.claude/hooks/unseen.py`) to refuse a commit's, a merge's or a tag's message, `-m`, `-F` or a heredoc, holding such a character, by its code point, line and column; six cases in `.claude/hooks/test_hooks.py`, five red before, `fe3f788e`'s own message refused at line 17, column 28, the net's own tests 290 and 0, and `unseen` 3 and 0 at 23.92 billion instructions against 23.87.
