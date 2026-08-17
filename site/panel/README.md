# site/panel/ — the site's own review panel

Five expert briefs for reviewing heroes-lang.org, convened on the author's
request (2026-08-18). This is **not** the language panel (CLAUDE.md §4): the
site is not spec, surface, diagnostics or architecture, so no `docs/panel/NNN`
record and no DESIGN-LOG line — the deliverable is the site getting better.

Each file is one seat: a role, what that seat is uniquely positioned to see,
and the constraints that are not up for debate. To convene, give each seat its
brief plus the whole `site/` tree, tell it to *render the pages* (headless
Chrome) rather than imagine them, and ask for ranked, concrete findings — a
finding without a proposed edit is an opinion.

The seats:

| file | seat | judges |
|---|---|---|
| `seat-communication.md` | communication | does every page say one clear thing, in the right order, to a first-time reader |
| `seat-marketing.md` | marketing & positioning | does the site make anyone *want* the language, and does the funnel have a next step |
| `seat-languages.md` | programming-language veteran | technical credibility: would a PL person respect or dismiss this site |
| `seat-design.md` | graphic & UX design | visual hierarchy, typography, colour, both themes, both widths |
| `seat-devex.md` | developer experience | can a developer get from landing to writing a program; docs, examples, friction |

## Constraints every seat inherits (not up for debate)

- No JavaScript, no build step, no webfonts, no external assets; light/dark via
  `prefers-color-scheme` (site/README.md).
- The Bowie register per site/README.md § Style guide — bolt, palette, nods as
  song/album titles only. The register is a given, not a finding.
- Every claim must match the repository's measured state: no "v1", no bootstrap
  retirement, no invented numbers, no duration claims.
- Compiler output is verbatim; every code block is a slice of a real
  `examples/` file (`data-src`/`data-lines`).
- The hero shows working code, never a diagnostic (author, 2026-08-18), says
  "a compiled programming language" before the name, and names LLMs plainly.
- English on every page.

A finding that needs one of these relaxed is out of scope for the seat and goes
to the author instead.
