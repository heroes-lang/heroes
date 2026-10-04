---
kind: decision
area: records
milestone: M-issue-files
filed: 2026-10-05
commit: self
github: none
---

# The author answers 1c 2a 3a 4a 5a 6b 7a: M-issue-files is pushed, its labels made, and three questions of the transcription settled

2026-10-05, written at 00:27 by the clock (`date`). The coordinator put seven
questions with a recommendation each, after M-issue-files landed on the trunk
at `4d6d68eb`; the author answered *"1c 2a 3a 4a 5a 6b 7a"*. Recorded as a
reading.

1. **The push, now, without the platform legs before it** (*1c*, against the
   recommended *1a*, which would have carried it with batch 11's push after
   that batch's Linux arm64 and Windows legs). The author's call, and an
   exception to CLAUDE.md § Verification's *Linux arm64 and Windows before the
   push*, written down as one rather than left to look like the rule: the
   change moves no line of `selfhost/`, `runtime/` or `seed/`, its gate on this
   Mac read the full net, the net's own tests and the compiler's own tests
   green, and the CI's matrix is its x86-64 leg after the push. Where the CI
   reads a shortened history, `records/cards` skips the question of the
   cards' commits and says so, by design.
2. **The folder stays the day** (*2a*): `issues/<year-month>/<day>/`, no counter
   and no bucket of a thousand.
3. **The 36 labels the issue files name are created with the push** (*3a*):
   `kind:` five, `class:` four and `area:` twenty-seven, so a defect a stranger
   files through `.github/ISSUE_TEMPLATE/defect.yml` takes `kind:defect` at
   once.
4. **The 83 GitHub milestones wait for the transcription** (*4a*): without
   issues they would be empty.
5. **The `learn` questions are not transcribed** (*5a*): no `/learn` rule ever
   ticks one, so they would stand open for ever on a public tracker, 436 of
   them against 77 open defects. They stay files with their cards
   (`.claude/rules/records.md` § The issues).
6. **The sittings are not GitHub Discussions** (*6b*, against the recommended
   *6a*, a milestone of their own): they stay files of `docs/panel/`, linked
   from the decision issues that ratify them.
7. **The verb that renders an issue file goes to a sitting when the author
   decides to transcribe, and not before** (*7a*); until then the rendering
   rule of § The issues is followed by hand.
