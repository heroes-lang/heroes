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
| `errors.html` | diagnostics as a deliverable — real output, fixes, holes, the mutation numbers. Not in the nav: it is a sub-page of `docs/`, linked from the docs landing, with a crumb and `Docs` marked current |
| `selfhost.html` | the fixpoint: the hash, what it took, and what it found |
| `zen.html` | `heroes this`, quoted verbatim — the twenty lines are the binary's own text, lyric fragments included, so the titles-only rule governs the page's nod and not the quotation |
| `panel.html` | how a change is decided: the five seats and their differentiated briefs, the four vetoes, the historian's search obligation, the borrowed languages, and who is building this (author request 2026-08-18) |
| `author.html` | Giuseppe Arici, the long bio in his own voice, the book, and what the book has to do with the language. Governed by § The author and the book below, which is not optional copy guidance |
| `thanks.html` | the credits: the designers of every language the design borrowed from, named one by one, each with the borrowing beside them. Governed by § The thanks page below |
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
no-JavaScript rule forces. Nine items in a fixed order — Why Heroes · Docs ·
Self-hosted · Zen · Panel · Thanks · Author · Log · GitHub — and the current page
marks itself `class="here"`. At nine items the row no longer fits a phone, so
under **800px** the list becomes **one horizontally scrolling line** with the mark
and GitHub on the row above it: two rows of sticky chrome instead of three, no
item dropped, and the item clipped at the right edge is the affordance. Adding a
tenth item is the point where this stops working and something has to give.

**That threshold was 700px until the Italian edition measured it** (2026-08-18).
Italian nav labels are wider — *Perché Heroes*, plus the language badge — so the
list goes from 517px to 540px, and at 701px the wrapped layout took **three rows**
of sticky chrome, which is the exact thing the rule above forbids. Moving the
query to 800px removes the wrapped state entirely: measured in both editions, one
row down to 860px, then two rows all the way down, and no three-row band at any
width. It is also **better for the English edition**, which used to spend 98px of
chrome between 701 and 800px and now spends 80px. A layout number that only one
language was ever measured at is a number that has not been measured.

**The row stays at nine, so a page enters it only by displacing one** (author
instruction 2026-08-18). `thanks.html` went in and `errors.html` came out, and
the exchange is the rule rather than the exception: a tenth item was built,
measured and removed. Measured in that sitting, at the home page's own nav:
nine items are a 517px list that stays on **one row down to 800px**, then two
rows to 701px, then the scrolling line; the tenth item took the list to 584px
and produced **three rows** of chrome between 720px and 701px, which is what
"something has to give" meant. The first thing tried instead — reaching the page
from every footer plus a sentence inside `panel.html` — failed the only test that
counts: the author could not find it. **A page nobody can see from the chrome is
not linked, whatever the link count says.**

`errors.html` did not lose anything by moving: it is now a **sub-page of the
documentation**, linked from the docs landing above the chapter list and carrying
a `.crumb` (`Heroes / Docs / errors`) like a chapter does, and its nav marks
**Docs** as the current item. That is the right shelf for it — diagnostics are
documentation, and the page was competing with `why.html` for the same visitor.
Its old routes are untouched: the home page's card, `why.html` and the docs
chapter all still link it.

The current page is marked twice, in colour **and** with a
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

## The Italian edition — `site/public/it/`

The site ships in two languages (author instruction 2026-08-18: *"traduci tutto
il sito anche in italiano … lascia in inglese i termini tecnici"*). This extends
CLAUDE.md §11's declared exception — which already covers the two books — to the
site, and the same rule applies: **neither edition is a machine translation of
the other**, and where they diverge the Italian is fixed to read better rather
than the English to read more literally, because the author studies from the
Italian.

Mechanics, all of them chosen so there is still no build step:

- The Italian edition lives in **`site/public/it/`, same basenames**
  (`it/why.html` is `why.html`), so the two trees map one to one and a missing
  page is obvious. Italian pages link `../style.css` — one stylesheet for both.
- **`lang="it"`** on the root element, and every page carries the three
  `hreflang` alternates (`en`, `it`, `x-default` → the English page).
- The switch is the **`a.lang` badge** in the nav beside the GitHub link, `IT` on
  English pages and `EN` on Italian ones, with `hreflang`/`lang` on the anchor
  and an Italian/English `title`. It is **not** a nav item: the row is full at
  nine, and a language is not a page.
- Nav labels: Italian where Italian is what a reader would say (Perché Heroes ·
  Grazie · Autore), English where the English word *is* the Italian technical
  usage (Docs · Self-hosted · Zen · Panel · Log). Keeping those also keeps the
  row the same width, which is what the nine-item measurement was about.
- Accented characters are **real UTF-8 characters** (`è`, not `&egrave;`); HTML
  entities stay for typography only (`&mdash;`, `&rsquo;`, `&ldquo;`,
  `&middot;`). The pages are `charset=utf-8` and an entity-per-accent source is
  unreadable for the one person who has to proofread it.

What must **never** be translated, because translating it would make the page
lie:

- **Code blocks.** They are slices of repository files and must keep matching
  their `data-src` byte for byte, English comments inside the code included. The
  drift check runs against the Italian pages too, for free. Only the
  `figcaption` word changes (`lines` → `righe`).
- **Compiler output, diagnostics, file paths, commands and keywords.**
- **The twenty Zen lines.** They are what `heroes this` prints, and they carry
  Bowie fragments; an Italian rendering would be both a false quotation of the
  binary and a derivative of a lyric, which § Style guide refuses. `it/zen.html`
  keeps the list in English under `lang="en"`, says in one paragraph why, and
  explains the lines in Italian prose underneath. **It does not gloss them line
  by line** — that was considered and refused for the same reason.
- **The section nods**, which are song titles.

Two things that are easy to get wrong and are therefore rules:

- **Numbers take Italian conventions**: `34.812`, `2.570`, `0,51`, `20,8
  megabyte`. ISO dates stay ISO (`2026-08-18`) because they are records. A
  column-aligned block inside `<pre>` is **re-aligned on the rendered
  characters**, not on the source: an `&rsquo;` is one glyph and eight
  characters, and the first Italian mutation table was written straight over the
  English column stops and did not line up.
- **The book is named in Italian**: *Gli eroi del codice* is the edition of
  record. The release wording rule in § The author and the book applies
  unchanged in Italian: before 20 September 2026, *"in uscita il 20 settembre
  2026"* — never *pubblicato*, *disponibile*, *in vendita*, *acquista*.

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
  under pressure · five years · look back in anger · absolute beginners ·
  moonage daydream · lady stardust · sons of the silent age.
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
- **Long pages are given parts.** A reading page numbers its own sections with
  a CSS counter, set large and faint in the margin the bleed track leaves free
  (it disappears under 900px, where that margin does not exist), and the rule
  over each section carries the two stage colours through a `border-image`
  gradient before fading into the hairline. `why.html`'s ten objections are a
  numbered list rather than ten paragraphs in a row: a reader can enter
  anywhere. This is what the author asked for on 2026-08-18, *"meno pagine di
  testo lunghissimo"*, and it is a layout answer, not a copy answer, though the
  copy was cut too.
- **The bolt once more, as architecture**: blown up behind every reading page's
  title at 9% and cropped by the head's own box. That is its fifth job and the
  last one it gets.
- **Evidence blocks get their own surface** (`.stats`, `ul.seats`): a tinted
  card with a red left edge, so the measured parts of a page stop reading as
  more prose.
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

## The author and the book — the rules this copy is under

From the author's own brief, 2026-08-18. These are not style preferences, they
are constraints on what the site may say, and every one of them is a way the
copy could be wrong later:

1. **One book, not two.** *Heroes of code* (Italian original: *Gli eroi del
   codice*) is a history of programming languages whose *form* is a dream
   journey with Bowie as the guide. Never write copy implying a Bowie book and a
   separate history book. The Italian is the edition of record; the English is
   the author's own translation.
2. **Wording on the release.** Before **20 September 2026**: *"out on 20
   September 2026"*. Never "published", "available", "on sale", "buy now". From
   that date onward "published" is correct and an Amazon link by ISBN may go
   live. An ISBN existing is not evidence a book is on sale: KDP issues them
   when a listing is created, drafts included.
3. **Never link or mention the book's repository.** It is private. No URL, no
   `sameAs` in any structured data, and no phrasing implying openness: not "open
   source", not "written in the open", not "source available". The *personal*
   GitHub profile is a different thing and is fine
   (`github.com/giuseppearici`, verified live 2026-08-18).
4. **No personal data, and less of it than the brief allows** (author
   instruction 2026-08-18, tightening their own brief). The site names the city
   (Brescia) and the company (Codermine) and links the personal GitHub profile.
   **No email addresses**: both were on the page for one revision and were
   removed. No date of birth, no home address, no tax code.
5. **Keep "he had never written a compiler before this one."** It is true and it
   turns the project's premise into a choice rather than a gap. Do not soften it
   into vagueness about "learning compilers", and do not inflate his compiler
   experience to compensate.
6. **No event agenda on this site** (author instruction 2026-08-18). A talk of
   the same name exists, with its own thesis, and it had a section here for one
   revision. It is gone: a language's site is not a speaking calendar, and a
   calendar is the part of a page that goes stale on its own. If it ever comes
   back, two things hold: the talk is never described as the book, and Italian
   Agile Days is an unsent proposal that must not appear.

**The book has one domain per edition** (author instruction 2026-08-18):
`glieroidelcodice.it` for the Italian, `heroesofcode.com` for the English, each
linked from its own edition's `author.html` on the title. **Both 301 to
`giuseppearici.com` pages that returned 404 when this was written**, measured the
same day, so today they are dead ends the way the parking pages below are. They
are in the markup on the author's instruction, which outranks the rule under it;
what that costs is a pre-publication check: **the two book domains must resolve
before the site goes public**, and if they do not, the two links come out again.

**Other author links are omitted on purpose.** `giuseppearici.com/en/` and
`giuseppearici.com/en/books/heroes-of-code/` both returned **404** on
2026-08-18, and `heroesofcode.com` 301s to the second of them, so the site is
still parked. The brief's own condition applies: omit rather than send visitors
to a parking page. When the launch commit ships, add them to `author.html` and
to the footer line. Test the URL first, the way the marketing seat tested the
repository's.

## The thanks page — why every link on it was fetched

`thanks.html` names people, which makes it the page where being wrong costs
somebody else something. Two claims are made about each entry, and both are
checked rather than recalled (CLAUDE.md §1):

1. **Who designed what** comes from the language's own Wikipedia infobox
   (`designer` / `developer`, read as wikitext on 2026-08-18) or from the paper
   design.md already cites — never from the assistant's memory. That is where
   *Tucker Taft for Ada 95 through 2012*, *Ulf Norell with Catarina Coquand on
   Agda 1.0*, and the five authors of the mutable-value-semantics paper
   (Racordon, Shabalin, Zheng, Abrahams, Saeta — first names from Crossref) came
   from.
2. **That the link points at the right human** comes from
   `action=query&prop=description&titles=…&redirects=1`, which catches both ways
   this goes wrong: a missing article, and — the nastier one — a title that
   *resolves* to a different person. Nine real traps in one sitting: Andreas
   Rumpf is a German classical archaeologist who died in 1966 (Wikipedia's Nim
   infobox carries an editor's comment saying exactly *"Do not WP:LINK to late
   German classical archaeologist"*), Edwin Brady is an Australian poet, Dan
   Grossman is an American politician, James Cheney redirects to James Chaney,
   Graydon Hoare redirects to the Rust article, Andrew Kelley to a soldier who
   died in 1918, Jack Little and Robert Bradshaw are disambiguation pages, and
   *Hylo* is a hamlet in Alberta. A page written from memory would have shipped
   most of those as confident links.

Three rules follow, and they bind every later edit:

- **A missing link is written as a failed search, never as an impossibility.**
  The page says the search came up empty; it never says the person has no
  article. That is §1's negative-claim rule, and the reason it matters here is
  that absence rests on the searcher's vocabulary. The best available evidence
  for absence is the **language's own infobox**: Wikipedia links a designer who
  has an article, so an unlinked name there is the encyclopedia's answer rather
  than ours.
- **The date on the page is part of the claim.** *"looked up and found, on
  2026-08-18"* is in the copy. Re-run the checks before changing it, and change
  it only after re-running them.
- **Nobody on the page was asked, and the page says so.** No characterisation of
  a living person beyond the borrowing itself, and no claim that anyone endorses
  this language.

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

The landing page lists all twelve chapters, and above them links `errors.html`,
which lives at the site root but belongs to this section; **one chapter**
(`failure-is-a-value.html`) is written, as the exemplar that fixes the
conventions. **M-documentation-site**
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
