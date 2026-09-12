# The shape of the chain file, and what each rule cost

`docs/ROADMAP.md` is the status and the table, and nothing else, by author
instruction 2026-09-12: *it is the file I open most, to see where I stand and to
reason, so I want it really simple*. What used to stand above and below the
table is here, and the five rules below are why the table looks the way it does.

Autonomous work sessions need the goal chain **in the repository**: tags say
where the project **is**, not what is **next**. This file is the distillation of
the approved bootstrap plan (revision 2, reviewed by panel 000); the build
order's rationale is design.md Part 10 and the language-level acceptance criteria
are design.md Part 0.

**Five rules hold its shape**, each one bought by a reorganisation this file
needed (author instructions 2026-08-25, 2026-08-26, 2026-09-03 and 2026-09-07,
with what each one measured in the git history and in `DESIGN-LOG.md`):

- **The past never stands in front of the future.** A closed milestone's record
  goes to its own journal, indexed at `docs/records/journal/README.md`; § Where we are is
  held under **15 lines** by `/step`'s checklist and by `suite_records.hero`'s
  `where_we_are`. This file once carried 512 lines about the past before its
  first line about the future, growing about 66 lines per close.
- **§ The chain carries the ORDER and nothing else.** Every scheduling note,
  ratification and author decision that used to live inside a cell is a line
  under the table, keyed **by name** — a reorder moves a number and never a name
  (CLAUDE.md §14).
- **The chain runs closed, then scheduled**, so a reader meets the whole past
  before the first line of the future.
- **One section per milestone that still has something to say**, in the chain's
  order, and a section whose milestone has closed says so in its heading.
- **One copy of a duty.** The two verification blocks used to sit 700 lines
  apart, which is how one of them rots; they are one section under the summary
  table.

---
