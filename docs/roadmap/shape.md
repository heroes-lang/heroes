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
  held under **32 lines** by `/step`'s checklist and by `suite_records.hero`'s
  `where_we_are` (**15 until 2026-09-12**, when the section measured exactly 15
  and the next line a close added would have gone red; the number lives on
  `WHERE_CEILING` and so does the reason). This file once carried 512 lines about the past before its
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

## Decisions about the table

What the chain decided about itself, here since 2026-10-05 (they were
`docs/roadmap/decisions.md` from 2026-09-12, the ROADMAP's own § Decisions this
file records before that).

### `scheduled, no warrant` is not decoration

Part 7's preamble defers everything on its list until the closure list compiles
itself, and **a place in the table is not a warrant**. Measurement 003 rider 3 is
the standing precedent: this file scheduled `outline` and `explain`, and
CLAUDE.md §10's stopping rule refused them.

### Two milestones were asked for and neither was added

Asked 2026-08-12; the table is unchanged and this is why (panels 033 and 034).

**Visibility**: three tiers, and two of them were never visibility questions —
private record fields are an opaque type (§4.9 makes construction impossible from
outside, and §4.20 makes a shim read the field anyway) and private variant cases
are `#[non_exhaustive]`, which Rust deleted in 2014, re-added per type in 2019
and documents as costing exhaustiveness. Both are now **Part 6, permanently**.
The third, `private` on a declaration, is **Part 7 item 14** at a pre-fixed +18;
M-selfhost-probe was assigned to decide it and **did, at its close 2026-08-15: no
blockage, so it stays Part 7** — eleven modules ported, every cross-module read
intended, and the rule's own words (*"a blockage there puts it on the closure
list, a wish does not"*) made the close mechanical.

**Errors**: the Rust shape landed at M-optional-map — `T?` is `Result<T,E>`, `?`
is `?`, `.must()` is `.unwrap()` — and the part Rust has that Heroes does not,
the typed error, stays **Part 8 wart 5** rather than becoming a deferral, because
it loses on §4.12's positive rule as well as on simplicity. What is real
underneath the question is measured: `docs/measurements/004-error-codes.md`,
**25 mutants, 0 caught**, and the answer is a `constant`, not a feature.
