---
kind: defect
area: records
milestone: none
filed: 2026-10-06
commit: none
github: none
---

- [ ] **378 — no instrument reads a commit's message for a raw invisible character** | the body of `fe3f788e` holds one raw U+200B, the write tool having decoded a written `\u200b`, defect 356's hazard; the `unseen` suite that 356 added reads files, not commit messages (counted by the coordinator at 03:08 on 2026-10-06; found by lane b12-str192) | `tests/harness/suite_unseen.hero` · the commit guard, `.claude/hooks/guard_bash.py` · **class: improvement**

    **Origin:** lane b12-str192, 2026-10-06, found beside panel 192's landing (its report's *found beside*); filed by the coordinator at 03:08.

    **Class: improvement**, 2026-10-06 (`.claude/rules/verification.md` § Bounded discovery): hardening: a commit's body is outward-facing and append-only, and nothing reads it.
