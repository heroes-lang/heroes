---
kind: task
area: cli
milestone: M-issue-files
filed: 2026-10-04
commit: none
github: none
---

- [ ] **M-issue-files** | a `heroes` verb that renders an issue file as the issue GitHub takes, its title, labels, milestone, body and closing comment by `.claude/rules/records.md` § The issues' rendering rule, so a transcription is a command and not a reading | `.claude/rules/cli-surface.md` (the stopping rule) · `tests/harness/cards.hero` · `.github/ISSUE_TEMPLATE/`

    **Origin:** the plan of M-issue-files, 2026-10-04, which kept it out of the
    milestone's first steps: a new verb is a change to the tool's surface and so
    a sitting's question (CLAUDE.md § 4), and CLAUDE.md § 10's stopping rule asks
    first whether a flag of an existing verb serves. Until it exists, the
    rendering rule is followed by hand, in the order the names read.

    **2026-10-05, the author's *7a*** (`issues/2026-10/05/2026-10-05-0027-the-author-answers-1c-2a-3a-4a-5a-6b-7a.md`):
    the sitting is convened when the author decides to transcribe, not before.

    **2026-10-11, the author's question of 2026-10-07 on the defect numbers** (`issues/2026-10/11/2026-10-11-1501-the-author-asks-that-the-issues-credit-who-got-there-first.md`): the verb asks `field.defect:NNN` before it creates a defect's issue, and the title and the `Record:` permalink before any other's, so a run stopped between a creation and the commit filling `github:` leaves no double; and after the last file a second pass turns every `defect NNN` a body names into a link to that defect's issue (`.claude/rules/records.md` § The issues, CL-083).
