# site/ — the showcase site for heroes-lang.org

One self-contained *directory*, shaped like a language site rather than one long
page (author instruction 2026-08-17: *"più a forma di siti di linguaggi, guarda
go, rust — non voglio una mega scroll page"*). Every page carries the same nav,
so no page is a dead end:

Everything that ships lives in **`site/public/`** — that directory is the whole
deployable site, so Pages publishing it can never leak this README or the
panel's briefs. Beside it, **`site/.claude/skills/site-panel/`** holds the
five-seat review panel as a directory-scoped skill.

| file (in `public/`) | what it is |
|---|---|
| `index.html` | the landing: hero, one sample, three cards, the claim. Short on purpose. |
| | The hero says **&ldquo;a compiled programming language&rdquo;** above the name, because a visitor who has never heard of Heroes should not have to infer the category (author instruction 2026-08-17). Its code panel shows **working code, never a diagnostic** — *&ldquo;aprire un sito di un linguaggio con un errore è brutto&rdquo;*, same date. The errors have their own page. In that panel the `bar` names the file, so the figure carries no second caption. |
| `why.html` | the founding constraint, the thesis, the cost formula, the rules, the objections |
| `errors.html` | diagnostics as a deliverable — real output, fixes, holes, the mutation numbers |
| `selfhost.html` | the fixpoint: the hash, what it took, and what it found |
| `zen.html` | `heroes this`, quoted verbatim — the twenty lines are the binary's own text, lyric fragments included, so the titles-only rule governs the page's nod and not the quotation |
| `log.html` | the build log, one postcard per milestone, newest first |
| `docs/` | the documentation: landing + chapters |
| `style.css` | the only cross-page asset |

Still no build step, no JavaScript, no external assets; light/dark via
`prefers-color-scheme`. The domain (heroes-lang.org) is already owned by the
author.

The stylesheet is a file rather than a `<style>` block per page for one reason:
there is no build step to keep copies honest, and a duplicated palette drifts
invisibly — one page's dark mode goes stale and nothing fails. The bolt stays
inline SVG in every page, because it is markup.

**The nav is duplicated in each page's markup**, which is the one repetition the
no-JavaScript rule forces. Seven items in a fixed order — Why Heroes · Docs ·
Errors · Self-hosted · Zen · Log · GitHub — and the current page marks itself
`class="here"`. Adding a page means editing that block everywhere; if that ever
gets painful, the answer is a generator behind a `heroes` subcommand, not a
script (CLAUDE.md §10).

**Syntax colouring is spans in the markup**, for the same reason. The token
classes are the lexer's own tables rather than a guess — `.k` is
`crates/heroes/src/lexer/keywords.rs::keyword`, `.t` is spec § Types — and a
user's own type is deliberately left uncoloured, because inferring it from a
capital letter is a premise about a convention the spec does not state. Blocks
are generated *from* the source file, so highlighting and byte-fidelity arrive
together.

Not deployed yet. Cheapest route when wanted: GitHub Pages publishing
`site/public/` (`CNAME` is already in it) + two DNS records at the registrar.
**Ask before wiring any of it — publishing is an outward-facing act**, one
of the process's few remaining hard stops.

## Style guide — the site's register: 70s-glam Bowie, with discipline

- **One motif**: the Aladdin Sane bolt (1973), red over blue, inline SVG
  only — header mark and marker accents.
- **Palette**: near-black stage / cream daylight; the bolt's red and blue
  are the only loud colours; gold for the quotation marks.
- **Type**: poster-condensed uppercase headings (system stack, never a
  webfont); Georgia serif body. Glam marquee over readable book page.
- **Song/album TITLES as section nods, never lyrics** (copyright — and
  design.md §"The name": personality in the packaging, precision in the
  substrate). If a nod needs explaining, cut it. Puns stay on this page.
  Nods live on the showcase and the docs landing; **chapter pages carry
  none** — a chapter is a book page, not a marquee.
- Every claim on the page must match design.md's current state.
- **No timeline or duration claims anywhere** (author decision 2026-08-17):
  not how long a milestone took, not how long the project has run. Dates in
  the build log are records; a duration is a boast.
- Nods spent so far: sound and vision · fashion · quicksand, avoided ·
  changes · station to station · rebel rebel · always crashing in the same
  car · a new career in a new town · oh! you pretty things · hunky dory.
- Ideas bank, still unspent: **M-selfhost-fixpoint's bootstrap retirement →
  "Ashes to Ashes"**; a 1.0 → "Golden Years". The fixpoint itself landed with
  M-selfhost-port and did *not* spend "Ashes to Ashes" — the retirement is a
  separate event and keeps the nod.

## The claims that have a gate on them

- **"v1"** appears on the site only when `M-selfhost-fixpoint` closes. The
  fixpoint is measured and stable (2026-08-17), but that milestone still owes
  the seed test — `B.c` built from a clean checkout with nothing but a C
  compiler — and then the archive of `crates/heroes`. Until then the page says
  the compiler is written in Heroes and the fixpoint holds, which is what was
  measured, and describes the archive as what remains.
- **Numbers are re-measured in the session that writes them** (CLAUDE.md §1),
  and **the unit has to match the record's**: `./target/debug/heroes measure`
  for spec tokens, `find selfhost -name "*.hero" | wc -l` and `| xargs wc -l`
  for the port, `grep -rn "PORT-DEBT" selfhost/ | wc -l` for the workarounds.
  Note that journal 021 and the ROADMAP count *tests* (27,230) while a grep of
  `^test "` counts *test blocks* (447) — different questions, so never mix the
  number of one with the word of the other.
- **Compiler output shown on the page is verbatim**, path and test annotations
  included. A trimmed-for-looks diagnostic is a fabricated diagnostic: the
  first draft of this refresh shortened one and got the caret width, the line
  content and the fix text wrong in the process.

## `site/public/docs/` — a skeleton, and who owns finishing it

The landing page lists all twelve chapters; **one** (`failure-is-a-value.html`)
is written, as the exemplar that fixes the conventions. **M-documentation-site**
owns writing the rest, and owns the rule that governs them:

> Every code block on the site is a file in `examples/`.

The convention that makes that rule checkable, and which every chapter must
follow:

```html
<figure class="example" data-src="examples/gallery/03-fallible.hero" data-lines="7-11">
<pre><code>… the slice, copied verbatim …</code></pre>
  <figcaption>examples/gallery/03-fallible.hero, lines 7&ndash;11</figcaption>
</figure>
```

`data-src` (and optional `data-lines`, inclusive, 1-based) is the machine-
readable half: the drift check greps these attributes, strips the tags from the
`<pre>`, decodes the entities, and diffs against the file slice — so
documentation drift becomes a test failure. The `figcaption` is the same promise
for readers. Syntax highlighting via `<span class="c">` is allowed because the
check compares *text content*, not markup. Blocks are copied from the file,
never retyped. The showcase page follows the convention too, so the rule has no
exceptions to explain later.

Unwritten chapters are listed with a dim `in progress` tag and **no link** —
never a dead one.

## Keeping it current — on demand, not per milestone

When the author asks for a site refresh (typically before publishing, or
after a stretch of milestones): update the badge on `index.html`; append one
entry per milestone closed since the last refresh to `log.html` (newest first,
postcard register, distilled from `docs/book/beats.md`, not from the commit log);
re-check `why.html`'s objections against design.md's current state; re-run every
number; re-check each code block against its `data-src` anchor.

`log.html` is the one page exempt from the invariant that no living file names a
numbered milestone (`crates/heroes-cli/tests/milestones.rs`), because its entries
are dated records and keep the identifiers they were written with — the same
footing as `docs/journal/` and `docs/panel/`. Every other page is watched.

## One thing the publication gate owns

`site/`'s Aladdin Sane bolt is iconography attached to an actively managed
estate — the style guide already keeps lyrics out, and this is the other half.
Recorded in `docs/ROADMAP.md` under M-publication-gate; cheaper to answer
before publication than after.
