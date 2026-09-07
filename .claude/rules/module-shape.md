---
paths:
  - "selfhost/**"
  - "runtime/**"
  - "tests/harness/**"
  - "examples/**"
---

# The shape of a file, and what a narrowing may rest on

Home of CLAUDE.md § 11's code conventions since 2026-09-07. What each rule cost
to learn is in `docs/contract/case-law.md`, cited as `CL-NNN`.

## Code is written to be read

Files stay short and single-concern. Every file opens with a module doc stating
its role and citing its design.md sections. Comments teach the invariant and the
why, never the diff. The author must be able to open any file and read it
without drowning (CL-001).

**The threshold is ~300 lines, in the unit `tests/harness/suite_layout.hero`
counts**, which is that instrument's own measure and not `wc -l`. Ask the
instrument, not the shell: by `wc -l`, 71 of 189 `.hero` files under `selfhost/`
pass 300 while the suite is green, because the suite counts something narrower
and carries a per-file `DECIDED` table for the exceptions it has ratified
(measured 2026-09-07, CL-011).

Three things that threshold is not:

- **It binds the compiler's own code and not its tests.** A test file is read
  one case at a time and a case is self-contained (CL-011).
- **It is a number to think at, not a limit to round.** A file split against its
  own seam is harder to read than a long one, which is why one 400-line module
  was ratified where the cut ran through a shared cache key (CL-011).
- **It yields entirely where the language forbids the seam.** Heroes refuses
  module cycles, and a recursive-descent grammar is mutually recursive by
  construction, so its knots cannot be split at all. A rule that cannot be
  obeyed is not a standard but a lie (CL-020).

**What is owed in exchange, and it is what the threshold actually protects**: a
knot carries a module doc naming its cycle, its entry points, and which function
calls which, so a reader opens it and finds a map rather than drowning.

## A narrowing asks the value, never the world

A filter, an allow-list of kinds, or a `_ =>` arm is a decision, and its
correctness rests on something. Rest it on a fact about the value in hand, *this*
expression's extent or *this* declaration's fields, and never on a premise about
the world around it (CL-004).

**A fact about the value cannot expire. A premise about the world expires
silently**, and the comment justifying it goes on reading as correct, because the
argument stays valid and only the premise died. That is why a convention about
comment style catches neither.

Where a premise is unavoidable, two things are owed:

- write it as a **falsifiable claim**, not a justification;
- give it **a test that fires when it dies**, whose failure message names what
  depends on it. One premise, three dependants in three modules, one test.

And put the fallback in the **loud** direction: `_ => hero_unreachable()` beat
`_ => false` by a whole class of defect, and the same lesson was paid for twice
(CL-004).
