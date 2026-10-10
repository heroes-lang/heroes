---
kind: decision
area: records
milestone: M-inferred-cell
filed: 2026-10-11
commit: self
github: none
---

# The tag `m-inferred-cell` waits for its seven blocking defects

2026-10-11, the author, in two answers in session heroes-lang-98. First, in
words meant as: *delete the public tag until they close*, on the question
panel 209's coordinator had put (waive the seven at this tag, or delete it
until they close). Then, told that the repository's ruleset *release and
milestone tags are immovable* (rules `deletion` and `non_fast_forward` over
`refs/tags/v*`, `m-*` and `site-v*`, no bypass actor) refused the deletion,
choosing *suspend the rule and delete* over keeping the tag with a dated
waiver and over deleting it by hand.

**What was done, by the clock read then.** At 00:45:22 the ruleset (id
22525143) was set `disabled`, the tag deleted from origin (`git push origin
:refs/tags/m-inferred-cell`, *deleted*), and at 00:45:26 the ruleset set
`active` again, read back identical to its state before but for its
`updated_at`; the tag was then deleted from this checkout. Its object,
`c779a8f8` on `d32c19a1` with its tagger line and message, is kept whole at
`.claude/worktrees/scratch-b15/m-inferred-cell.tag-object.txt` (ignored by
git), so it is placed again unchanged, the ruleset suspended the same way for
the push, once the seven are closed: 590, 591, 592, 602, 603, 604 and 605,
the open `blocking` defects filed on 2026-10-10 before the milestone closed,
which `records/tagged` read red at the tag. Anyone who fetched the tag before
00:45 keeps it.

**Why the rule was the author's to suspend**: a milestone is tagged only over
a clean list (CLAUDE.md § Verification, the author's instruction of
2026-09-08 as amended 2026-10-02), and the same author chose on 2026-10-10,
after 16:45 by the clock read before the question, not to tag M-issue-files
over its open blocking defects. The
ruleset makes a placed tag immovable; the author chose the list's rule over
the tag's.
