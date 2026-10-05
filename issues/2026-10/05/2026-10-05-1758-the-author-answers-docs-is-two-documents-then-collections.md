---
kind: decision
area: records
milestone: M-issue-files
filed: 2026-10-05
commit: self
github: none
---

# The author answers: the top of `docs/` is two documents, then collections

2026-10-05, written at 17:58 by the clock (`date`). The author asked the
coordinator to reason about the folders of `docs/`, meant as: *I would like to
make some things uniform: `design` holds a single file, `design.md`, and I do
not see whether it belongs there; check whether what is inside `ref` makes
sense there or should move; I see a `roadmap` folder with many files in it*.
The coordinator measured each folder, what it holds and what reads it, and put
four questions with a recommendation each; the author took the four
recommendations. Recorded as a reading; the rule is `.claude/rules/records.md`
§ The top of `docs/`.

**What was measured**, on `de053027`: `docs/design/` held `design.md` alone,
the folder having existed for `DESIGN-LOG.md`, deleted that morning; 7 living
lines named its full path, while 3,330 mentions name it `design.md`, which the
redirect already resolves. `docs/ref/` held three unrelated things: the README's pictures
(`assets/`, 7 files, three pictures read by nine tags of the README), the Linux and
Windows machines (`environment/`, 5 files, cited by the CI, `heroes doctor`'s
comment and the platform rules) and the glossary (`glossary/`, 4 files,
written by `/learn`). `docs/roadmap/` held six files beside the milestone
pages, four opening with a second-level title because each was a section cut
from the ROADMAP, and `decisions.md` holding three things: two decisions about
the table, and the books' rules, two of which `docs/records/book/README.md`
already stated.

1. **`design.md` stands at `docs/design.md`**, beside `docs/ROADMAP.md`.
2. **`docs/ref/` is divided by subject**: `docs/assets/`, `docs/platforms/`
   (the name `.claude/rules/platforms.md` already used), and
   `docs/learn/glossary/`, beside the questions `/learn` also writes.
3. **`docs/roadmap/` is made uniform and the ROADMAP stays where the site reads
   it**: the two decisions about the table joined `shape.md`, which says why
   the table looks the way it does; the books' third rule joined their README,
   where the other two already stood; `decisions.md` is deleted; every file
   opens with one title.
4. **`docs/measurements/`, `docs/panel/` and `docs/records/` stay where they
   are**: moving them would touch 2,254 citations and the code that reads
   `docs/panel/` and `docs/measurements/`, and clarify nothing for a reader.

**What the change does not move**: the files' contents, which moved by renames
alone (17 at 100% similarity); the dated records' citations, which read
against `de053027`; and the compiler, whose two edited comments keep their
line counts so the seed is unchanged.

**What the move found in the instrument**: `records/citations` read the
directory `docs/design/`, dead since 2026-10-05, as alive once `docs/design.md`
existed, because it removed a directory's slash and compared the rest to every
path as a string, and `docs/design.md` begins with `docs/design`. It now keeps
the slash, and one older line, a comment of the instrument itself, cited that
directory with its date a line above and took the date onto its own line.
Holding a token without a slash to the same boundary was measured too and
refused: it turned 16 living lines red, every one a real name.
