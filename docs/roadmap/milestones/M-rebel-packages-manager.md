# M-rebel-packages-manager — the package manager written with Rebel


**Entered 2026-11-19 at row 78, by the author's decision 4 of 2026-11-15**
(`docs/roadmap/milestones/M-rebel-library.md`), at M-rebel-library's landing:
M-core-packages and M-doc-generator were enclosed in M-rebel-library, and one
thing of theirs stays open on a row of its own, the package manager written
with Rebel. **Scheduled, no warrant.** Its name is the one milestone
identifier of three words after `M-`, the author's choice of the same day
(`.claude/rules/records.md` § Milestone identifiers).

**What it delivers**: feature 2315, `heroes add` and `heroes fetch` written
again in Heroes with Rebel, every golden of their interface
(`tests/golden/fetch/`) passing unchanged. It is the one part of the
project's tools the library reaches: the compiler does not use Rebel to write
itself, and *only the package manager will, on its own row* (decision 6).

**Where it comes from.** M-package-manager (row 77) delivered the first
version over C, `git` and `curl` run as programs, and planned the work in
three tempos, the third the package manager rewritten over the core packages,
its interface unchanged (the author's instructions of 2026-11-06, that row's
page). Panel 242's R7 made feature 2315 wait for the compiler's first package;
panel 250's answer *(b)* moved it to M-package-index on the morning of
2026-11-15 (decision 2564), since `core` held none of the parts it would
replace; and decision 4 brought it here the same evening, Rebel being the
library that holds them. M-package-index, row 79, comes after this row (that
row's page, 2026-11-17).

**What the record leaves for its opening**, each a question rather than a
premise:

- **Panel 242's R2 stands** (ratified 2026-11-10; panel 253's spec-warden
  reads it unfalsified): *the compiler takes a package only where its own tree
  shrinks or a measured defect closes*, placed files counted. `heroes add` and
  `heroes fetch` are verbs of the compiler, so this row takes Rebel's packages
  under that ruler, placed package by package as panel 253's R6 made an entry's
  `packages` list place them. Whether any route meets it is unmeasured.
- **What the program would carry**, measured once (M-rebel-library's
  decision 35, 2026-11-17): the closure of the packages a package manager
  written with Rebel would use, each counted by its face, its `internal/` and
  its floors, was 82,716 lines, 26% of the 76 packages Rebel then held with a
  face. Rebel has grown since; the number is that evening's.
- **The parts it would replace**, as panel 250 named them: `git` and `curl`
  launched as programs and the index read over HTTP. Which Rebel packages
  replace each, and on which platforms, is read from Rebel's ledgers at the
  opening, never from this page.
