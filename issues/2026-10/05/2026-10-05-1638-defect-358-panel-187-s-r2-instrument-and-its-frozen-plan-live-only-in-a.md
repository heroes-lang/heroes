---
kind: defect
area: process
milestone: none
filed: 2026-10-05
commit: self
github: none
---

- [x] **358 — panel 187's R2 instrument and its frozen plan live only in a session's scratchpad, which macOS empties** | the instrument every round touching a recovery's files owes, `<scratchpad>/instrument/tool/recovery.py` (`docs/panel/184-briefs/00-shared.md:94`), was gone on 2026-10-05 with the rest of `/private/tmp`'s old files, and batch 11's coordinator rebuilt it (`replay.py`, `validate.py`) from batch 10's recorded results and the snapshot `c85bccb8`, matched by the plan's own sha1s; the plan itself, `plan-singles.jsonl` and `plan-pairs.jsonl`, is in the same scratchpad and no tracked file holds it, so a second loss would leave no frozen corpus to replay | `docs/panel/187-a-recovery-is-done-when-every-row-is-repaired-filed-or-pinned-and-the-instrument-compares-two-compilers.md` § R2 · `.claude/rules/verification.md` § Bounded discovery (*the counts §4.17 names are read at each round*) · CLAUDE.md § 10 (*never a script*) · **class: adjacent**

    **Origin:** batch 11's coordinator, 2026-10-05, at the round's gate: the R2 run owed was the first since the tool's source went missing.

    **Class: adjacent**, 2026-10-05 (`.claude/rules/verification.md` § Bounded discovery): found beside the work; nothing told was wrong, since the rebuilt tool, against batch 10's recorded run of the same compiler, gives the same class to each of 13,594 singles in both arms, the same flags and sites, and the same told status to 15,839 of 15,842 pairs. The rule asks for an instrument the repository does not hold, a process's premise about the world (`.claude/rules/module-shape.md`), and a gate that cannot replay the frozen plan cannot read R2.

## The repair

Not repaired: a duplicate of defect 210 (`issues/2026-10/03/2026-10-03-0000-defect-210-the-recovery-instrument-lives-in-the-scratchpad-as-a-python.md`), which holds the same instrument and the home panel 187's R2 ruled for it on 2026-10-03, the port to `heroes mutate`'s recovery arm with its plan pinned in the tree; until that port lands the scratchpad tool is the gate's, and a reboot that empties it is the gate's loss, as the loss of 2026-10-05 was. The coordinator filed this item at 16:38 without the search CLAUDE.md § RUN IT asks before a negative claim, *grep `issues/` and `docs/panel/`*, which finds 210 by its own words; found after 22:46 while briefing panel 193 on the same question.

**Closed 2026-10-05** as a duplicate of 210, which stays open as an `improvement`, outside the batches of that night by the author's instruction that only improvements stay out.
