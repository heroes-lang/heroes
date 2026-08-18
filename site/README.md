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
| `panel.html` | how a change is decided: the five seats and their differentiated briefs, the four vetoes, the historian's search obligation, the borrowed languages, and who is building this (author request 2026-08-18) |
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
no-JavaScript rule forces. Eight items in a fixed order — Why Heroes · Docs ·
Errors · Self-hosted · Zen · Panel · Log · GitHub — and the current page marks
itself `class="here"`. The current page is marked twice, in colour **and** with a
rule under it: colour alone is a signal a large minority of readers receive less
of. Adding a page means editing that block everywhere; if that ever
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
  car · a new career in a new town · oh! you pretty things · hunky dory ·
  under pressure · five years · look back in anger · absolute beginners.
- Ideas bank, still unspent: **M-selfhost-fixpoint's bootstrap retirement →
  "Ashes to Ashes"**; a 1.0 → "Golden Years". The fixpoint itself landed with
  M-selfhost-port and did *not* spend "Ashes to Ashes" — the retirement is a
  separate event and keeps the nod.

## The visual system — what the art direction pass fixed (2026-08-18)

The register above says *what* the site sounds like; this says what the design
is allowed to do, so a later edit does not spend the same accent twice. The page
is a stage with **one light source, one motif, one quotation colour**, and
everything else is paper and ink.

- **One light.** The red/blue wash lives behind the home hero and nowhere else.
  It is absolutely positioned and wider than the page on purpose, which is why
  `html, body { overflow-x: clip }` exists: without it a phone scrolls sideways
  into empty gradient. `clip` rather than `hidden`, or the sticky nav goes with
  it.
- **One motif, four jobs.** The bolt is the nav mark, the `h2` bullet, the pin on
  each build-log entry, and the full stop on the line above the footer. It is
  drawn from a single `--bolt` token used as a **mask**, so it takes its colour
  from the theme instead of freezing one hex per palette. Anywhere else it would
  be decoration, and an accent used everywhere is an accent used nowhere.
- **Gold is the quotation colour**, and it is used only where something is being
  quoted: Bowie's own quotation marks in the marquee, the pull quotes, the
  section nods (song titles *are* quotations), and the fixpoint hash, which is
  the page's one piece of hard evidence.
- **Two layouts, deliberately different.** The home is a stage: one left-hand
  axis, wide frame, prose held to 41rem inside it. Every reading page is a
  centred column at the book measure with code and figures bleeding
  symmetrically into a wider track, so a 70-column diagnostic keeps its
  alignment without stretching the prose. The grid sizes the **middle** track
  and lets the bleed collapse first; sizing the bleed instead leaves a phone
  with 88px of empty margin and squeezed prose, which is what the first cut did.
- **The marquee prints out of register** — the red plate a hair left, the blue a
  hair right, on the home `h1` only. It is the 1973 cover said in type. Once per
  site; the gold quotation marks stay out of it, because gold printed twice
  reads as dirt.
- **Grain at ≤ 4%**, one 160px `feTurbulence` tile generated in the stylesheet
  (no request, no JavaScript), fixed over the page and `pointer-events: none`.
- **Two blues.** `--blue` draws shapes; `--link` is the text colour, lifted so it
  passes contrast on this background. One token cannot do both jobs.
- **Code blocks say when they scroll**: CSS-only scroll shadows, two `local`
  patches hiding two `scroll` shadows. On narrow screens source blocks scroll
  and never wrap (wrapping 4-space indentation destroys the only structure the
  language has); shell transcripts and diagnostics wrap, because a clipped
  command is a command nobody can type.

**How to verify it, since the design seat judges from renders and not markup:**
Chrome headless on macOS will not open a window narrower than about 590px, so a
`--window-size=390` screenshot silently renders at 590 and clips, which reads as
a layout bug that is not there. Load the page in a **390px iframe** inside a
wider wrapper instead. Light mode has no headless flag either: extract the
`prefers-color-scheme: light` token block into an override stylesheet and load
it after `style.css`.

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
  number of one with the word of the other. The numbers `panel.html` and
  `why.html` added on 2026-08-18: panel sittings are
  `ls docs/panel/[0-9]*.md | wc -l` (83, and note the highest *number* is 085 —
  the sequence has gaps, so the count and the last id are different questions),
  and the runtime is `find runtime -name '*.c' -o -name '*.h' | xargs wc -l`
  (3,139 lines, which includes `runtime/parts/`; a glob of `runtime/*.c` alone
  gives 695 and answers nothing).
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

## Launch order — the repository goes public first

`github.com/giuseppearici/heroes-lang` returned **404** on 2026-08-18. Every
verification path on the site runs through it: the `git clone` line, the GitHub
link in all eight navs, the twelve chapter file links, llms.txt's raw spec link,
and the footer sentence saying the record "is public in the repository". This
site's whole mechanism is *check me*; shipping it while the check 404s turns the
project's best asset into its most visible broken promise. **Repo public first,
site second.** Found by the panel's marketing seat, which tested the URL rather
than assuming it.

## Numbers on the site are re-run, never quoted from a record

The measurement pages had figures from `docs/measurements/002-metric-3.md`
(2026-08-04) presented as "the last full run" while `heroes mutate` gave
different numbers on 2026-08-18. Re-run the tool before publishing any of them,
and **date the result on the page**.

And **never pool the mutation rates into one headline** — `heroes mutate` prints
the prohibition (panel 011) every time it runs, and the site shipped the pooled
number anyway for a full round. Per-operator or nothing. `forget-at-decl` is
excluded from any summary because its catch rate is 100% by construction.

## One thing the publication gate owns

`site/`'s Aladdin Sane bolt is iconography attached to an actively managed
estate — the style guide already keeps lyrics out, and this is the other half.
Recorded in `docs/ROADMAP.md` under M-publication-gate; cheaper to answer
before publication than after.
