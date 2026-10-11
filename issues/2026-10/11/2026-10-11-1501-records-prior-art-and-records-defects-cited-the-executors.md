---
kind: task
area: harness
milestone: M-issue-files
filed: 2026-10-11
commit: self
github: none
---

- [x] **M-issue-files** | `records/prior-art` and `records/defects-cited`: every `## Prior art` section stands on a `decision` of a sitting the historian sat in, holds only `Taken from` and `Reached first by` lines each with a link, opens with its re-read day and is never empty; a closed `feature` or `task` holds `## What was done` with a hash or `self`; every `defect NNN` cited under `selfhost/`, `runtime/`, `seed/` and `tests/` resolves in the defects' register, `014` to two and a file of several by the numbers its first field names; and the write hook asks the first two of a touched issue file | `tests/harness/suite_records.hero` (`defect_register`, the shape of `records/citations`) · `tests/harness/cards.hero` · `.claude/hooks/fmt_check.py` and `.claude/hooks/test_hooks.py` · `.claude/rules/records.md` § The issues

    **Origin:** 2026-10-11, M-issue-files step 9, on the author's instructions of 2026-10-07 (`issues/2026-10/11/2026-10-11-1501-the-author-asks-that-the-issues-credit-who-got-there-first.md`). Measured that day on the trunk: 560 numbers cited, every one resolving, and nothing holding the 561st; `records` read 28 passed in 39.94 s real, 36.25 s user, on a still machine, the figure the new checks are timed against.

## What was done

`tests/harness/credit.hero` holds the two checks, called by `suite_records.hero` and narrowed to one path by the harness's third word (`-- <compiler> records <path>`), which `.claude/hooks/fmt_check.py` asks of an issue file as it is written and of a file under the cited roots that cites a defect; six cases in `.claude/hooks/test_hooks.py`, and the module's own tests. Measured on the day: the check resolved 7,457 citations and found no orphan, the whole `records` suite went from 28 checks to 30, and a narrowed question costs 7.4 s with the harness built warm. Landed in this file's own commit (`self`), M-issue-files step 10, 2026-10-11.
