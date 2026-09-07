---
name: site-panel
description: Convene the site's five-expert review panel on heroes-lang.org — communication, marketing, PL veteran, graphic/UX design, developer experience — then apply the findings to site/public/. Use when the author asks for a site review or before a publication push.
---

# The site panel — five seats over heroes-lang.org

This is **not** the language panel (CLAUDE.md §4): the site is not spec,
surface, diagnostics or architecture, so no `docs/panel/<NNN>` record and no
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

1. Launch the seats **in parallel**, one agent each. Each prompt: read your
   brief at `site/.claude/skills/site-panel/seat-<name>.md`, read the
   constraints below, read the pages in **`site/src/html/`** and the built site
   in `site/dist/`, and — for any seat that judges appearance — **render before
   judging** (headless Chrome, `--headless --screenshot`, dark and light, 1200px
   and 390px; a width under about 590px has to be an iframe inside a wider
   window, which is what `site/README.md` says).

   **Both editions are judged.** The Italian is the edition the author reads,
   and it was outside this panel's remit until 2026-09-07, which is why the
   constraint list below used to end *English on every page*. A seat that reads
   Italian judges `site/src/html/it/` by the same five bands.
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
- No JavaScript **reaches the visitor**, no webfonts, no external assets;
  light and dark come from `prefers-color-scheme` alone. **There IS a build
  step**: the site is an Astro project and the examples pages are generated
  from `examples/` at build time. This line used to say "no build step", which
  stopped being true when the site was ported.
- The Bowie register per site/README.md § Style guide — bolt, palette, nods
  as song/album titles only. The register is a given, not a finding.
- Every claim must match the repository's measured state: no "v1", no
  bootstrap retirement, no invented numbers, no duration claims.
- Compiler output is verbatim; every code block is a slice of a real
  `examples/` file (`data-src`/`data-lines`), regenerated from the file.
- The hero shows working code, never a diagnostic; says "a compiled
  programming language" before the name; names LLMs plainly (author,
  2026-08-18). A diagnostic may appear in the first value prop UNDER the hero,
  and one does.
- The nav is **seven items** and a page enters it only by displacing one
  (author instruction 2026-09-06). Proposing an eighth means naming which of
  the seven leaves.
- **A number on the page is generated from the tree or written as a threshold**
  (author instruction 2026-09-07, `site/CLAUDE.md`). A seat that asks for an
  exact count has to say what keeps it true next month.
- **The reader is a working programmer, not an expert in language design**
  (author instruction 2026-09-06). Glossing null or a garbage collector is
  noise now; glossing fixpoint or canonical form is still owed.
- **The links into the repository stay clickable while it is private** (author
  decision 2026-09-03, `site/README.md` § Launch order). The addresses are
  final and open with the code, so a live link is a door that is not open
  rather than a wrong address, and on the day it opens there is nothing to put
  back. `github.com/heroes-lang/heroes` therefore answers 404 today, on every
  page, **by decision**. A seat may say the warning around such a link is too
  quiet, which is a finding and was one; a seat may not treat the 404 itself as
  a defect, and the dead-link cap does not fire on it. Proposing to repoint one
  is proposing to reverse an author decision, so it goes to the author with its
  reasoning, never into an edit.

A finding that needs one of these relaxed goes to the author, not into an edit.

## Scoring — the loop's stopping condition

Every seat ends its report with a score out of 100 and the single change that
would raise it most. The panel's number is the **lowest** seat score, not the
average: a site is as good as its worst dimension, and averaging is how a page
with one embarrassing flaw ships.

Each seat scores five bands, 20 points each, in its own domain:

| band | what it asks |
|---|---|
| **truth** | does every claim survive a check against the repository |
| **clarity** | can the intended reader get it on one pass |
| **completeness** | is anything a reader needs simply absent |
| **craft** | is the execution at the level the project deserves |
| **pull** | does it make the reader want the next thing |

**Two caps, and they are hard.** A claim that contradicts the repository caps
that seat at **59** no matter what else is right — the site's whole mechanism is
that it survives cross-checking. A dead link, a broken render, or an unreadable
passage in either theme caps at **74**.

**The loop.** Convene → score → apply → re-convene, and each round tells the
seats their previous score and what was changed, so they judge the delta rather
than re-litigating. Stop when the lowest seat is **≥ 92** and no seat reports a
blocking issue. A seat may not raise a score for a change it has not seen
rendered.

**Scores are never written into the site**, only into the commit body — the
number is an instrument for this loop, not a claim about the product.

## Personalities — because five polite reviewers produce one bland review

Each brief carries a `## Persona` section: a temperament, one pet peeve, and
what earns a yes. The point is differentiation, not theatre — five seats that
all optimise "professional and balanced" collapse into one seat. A persona
never licenses inventing a fact, and never softens a real defect.
