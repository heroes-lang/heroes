# M-documentation-site — the whole language, anchored to programs that run *(closed 2026-09-02)*


**The record is [journal 030](journal/030-documentation-site.md).** The four
bullets below were the milestone's **brief**, written while it was scheduled;
what landed is the journal's to say, and the site's own register rules live in
`site/README.md` § Style guide. Two of the four bind **any later work on the
site** and are why this section is kept rather than folded into the journal: a
code block comes from `examples/`, and publishing is a hard stop.

- **The language documented in full**, page by page, for someone who has not read
  `spec/heroes-spec.md` — the spec is the control instrument, not the teaching
  text, and it is budgeted precisely so that it can never become one.
- **Every code block on the site is a file in `examples/`**, not a snippet typed
  into HTML. M-program-corpus is what makes this possible, and it converts
  documentation drift into a test failure: a check asserts that each block
  matches a program in the repository that compiles and runs. Documentation that
  cannot rot is worth more than documentation that is merely current.
- **A history of the language**, distilled from `DESIGN-LOG.md`, `docs/panel/`
  and the journals: what was decided, what was refused, and the U-turns —
  including the ones that look bad in retrospect, which are the ones worth
  reading.
- **Publishing stays a hard stop** (CLAUDE.md §14): the site is built here and
  goes outward only when the author says so.

*******************************************************************************
**OPEN: 0**

*******************************************************************************
