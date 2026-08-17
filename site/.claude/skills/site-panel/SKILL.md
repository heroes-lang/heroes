---
name: site-panel
description: Convene the site's five-expert review panel on heroes-lang.org — communication, marketing, PL veteran, graphic/UX design, developer experience — then apply the findings to site/public/. Use when the author asks for a site review or before a publication push.
---

# The site panel — five seats over heroes-lang.org

This is **not** the language panel (CLAUDE.md §4): the site is not spec,
surface, diagnostics or architecture, so no `docs/panel/NNN` record and no
DESIGN-LOG line. The deliverable is the site getting better, and the record is
the commit that does it.

## The seats

One brief per file, beside this skill. Each names what its seat is uniquely
positioned to see.

| brief | seat |
|---|---|
| `seat-communication.md` | does every page say one clear thing, in the right order, to a first-time reader |
| `seat-marketing.md` | does the site make the right visitor want the language, with a next step always in reach |
| `seat-languages.md` | technical credibility — where a PL veteran's skepticism fires, and what they cannot find |
| `seat-design.md` | visual hierarchy, typography, colour — judged from RENDERS, both themes, both widths |
| `seat-devex.md` | can a developer get from landing to running a program; where the trail goes cold |

## How to convene

1. Launch five agents **in parallel**, one per seat. Each prompt: read your
   brief at `site/.claude/skills/site-panel/seat-<name>.md`, read the
   constraints below, read every page in `site/public/`, and — for any seat
   that judges appearance — **render before judging** (headless Chrome,
   `--headless --screenshot`, dark and light, 1200px and 390px).
2. Demand ranked findings, each with the exact current text/CSS and the
   concrete replacement. A finding without a proposed edit is an opinion.
   Ask each seat for two things that already work and must not be broken.
3. Synthesize: where seats conflict, communication beats marketing on copy,
   design beats everyone on legibility, devex beats marketing on honesty.
4. Apply the accepted findings to `site/public/` only, re-verify (blocks
   against `data-src`, links with file:// semantics, both themes rendered),
   and commit with the findings quoted in the body.

## Constraints every seat inherits (not up for debate)

- Writes stay inside `site/` — another session owns the rest of the tree.
- No JavaScript, no build step, no webfonts, no external assets; light/dark
  via `prefers-color-scheme` (site/README.md).
- The Bowie register per site/README.md § Style guide — bolt, palette, nods
  as song/album titles only. The register is a given, not a finding.
- Every claim must match the repository's measured state: no "v1", no
  bootstrap retirement, no invented numbers, no duration claims.
- Compiler output is verbatim; every code block is a slice of a real
  `examples/` file (`data-src`/`data-lines`), regenerated from the file.
- The hero shows working code, never a diagnostic; says "a compiled
  programming language" before the name; names LLMs plainly (author,
  2026-08-18).
- English on every page.

A finding that needs one of these relaxed goes to the author, not into an edit.
