# site/ — the showcase site for heroes-lang.org

One self-contained *directory*, shaped like a language site rather than one long
page (author instruction 2026-08-17: *"più a forma di siti di linguaggi, guarda
go, rust — non voglio una mega scroll page"*). Every page carries the same nav,
so no page is a dead end:

**The site is built.** `npm run build` turns `site/src/` into **`site/dist/`**,
and `dist/` is the whole deployable artifact: nothing outside it is uploaded, so
publishing can never leak this README or the panel's briefs. Three directories
hold the source, and each answers a different question:

| directory | what is in it |
|---|---|
| `src/html/` | the pages themselves, one HTML fragment each: the inside of `<main>`, prose and code and footer, exactly as it has always been written |
| `src/pages/` | one tiny `.astro` per page, carrying four facts and no content: its path, its title, its description, which nav entry is current |
| `src/layouts/`, `src/components/` | the head and the nav, written **once** |
| `public/` | the assets, copied into `dist/` untouched: `style.css`, `images/`, `robots.txt`, `llms.txt`, `CNAME`, and the pre-launch parking page |

Beside them, **`site/.claude/skills/site-panel/`** holds the five-seat review
panel as a directory-scoped skill.

**Why the pages are HTML fragments and not markup inside the `.astro` files.**
In an `.astro` template `{` opens a JavaScript expression, and these pages are
full of code examples in Heroes and in C where `{` is a brace. Pasting the
markup in would make the compiler read `{str: i64}` as an expression. Keeping
the content in HTML files and injecting it with `set:html` removes the whole
class of accident, and it has a second effect worth as much: the prose stays in
plain HTML files that anyone can open and edit without knowing anything about
Astro.

## Commands

```
cd site
npm ci                  # exactly the lockfile, and fails if it disagrees with package.json
npm run build           # src/ -> dist/, 44 pages
npm run dev             # the fast loop, on localhost
npm run preview         # dist/ over plain HTTP, as Astro serves it

sudo python3 serve.py --host heroes-lang.org    # dist/ over HTTPS, under the REAL name
```

The last one catches what the others cannot, and § Looking at it before it is
published says why. It needs `npm run build` to have run first.

None of these is a `heroes` subcommand, and CLAUDE.md §10's stopping rule is what
keeps them out: a capability enters that surface only if the fixpoint invocation,
the golden harness or the Part 11 harness must type it, or it has a measured
Part 11 effect. Building a website is none of those.

## The URLs, and why they end in a slash

Every page of this site answers at a path with a trailing slash: `/`, `/why/`,
`/docs/`, `/docs/maps/`, `/it/`, `/it/docs/maps/`. That is `build.format:
'directory'` plus `trailingSlash: 'always'`, the same pair the author's other
site uses, and it is not a preference. **The host decides this, and it was
measured live against this project's own deployment**:

| asked for | Cloudflare Pages answers |
|---|---|
| `/why/`, `/docs/`, `/docs/maps/`, `/it/` | **200** |
| `/why.html`, `/index.html`, `/docs/index.html` | **308**, to the extensionless form |
| `/why`, `/docs`, `/it` | **308**, to the slashed form |

Pages strips a `.html` extension whether the site wants it to or not. A site
that advertises `.html` therefore advertises URLs that redirect, and the
`canonical` on each page points at the URL that redirects away from it, which is
a page arguing with itself in front of a search engine.

**The port got this wrong first.** It ran at `format: 'file'` precisely to keep
the hand-written `.html` URLs unchanged, which looked like the careful choice and
was the opposite: it preserved names the host refuses to serve. The mistake cost
nothing only because the site had never been published — no external link, no
search index, no bookmark. After a launch the same change costs redirects
forever, which is the argument for having found it now.

Two consequences worth knowing:

- **Internal links are absolute** (`/docs/maps/`, `/style.css`), not relative.
  Relative links and directory URLs are a bad pair, because every page sits one
  level deeper than its file did and every `../` would be off by one. An
  absolute path has no depth to get wrong, and it removed the site's most
  confusing detail: there used to be TWO different depths to keep straight, one
  for the stylesheet and one for the nav, because `/it/` is one directory below
  the root and at the top of its own edition at the same time.
- **`format: 'directory'` needs no special case.** `pages/why.astro` writes
  `dist/why/index.html` and `pages/docs/index.astro` writes `dist/docs/index.html`.
  The earlier `format: 'file'` did need one, and getting it wrong put the whole
  Italian edition's landing at `/it.html` with the build log still reporting 44
  pages — a dead link from the language switch of all 22 Italian pages, and
  nothing anywhere said so.

The pages, by the name of their source file. Each one is a fragment in
`src/html/` and a four-line `.astro` beside it in `src/pages/`; the URL each
answers at drops the extension and gains a slash, so `why.html` is `/why/` and
`docs/maps.html` is `/docs/maps/` (§ The URLs, and why they end in a slash).

| source file | what it is |
|---|---|
| `index.html` | the landing: hero, one sample, three cards, the claim. Short on purpose. |
| | The hero says **&ldquo;a compiled programming language&rdquo;** above the name, because a visitor who has never heard of Heroes should not have to infer the category (author instruction 2026-08-17). Its code panel shows **working code, never a diagnostic** — *&ldquo;aprire un sito di un linguaggio con un errore è brutto&rdquo;*, same date. The errors have their own page. In that panel the `bar` names the file, so the figure carries no second caption. |
| | **The three figure cards carry only at-a-glance numbers a stranger parses without context** (author instruction 2026-08-25: *&ldquo;metti solo dei dati a colpo d'occhio facilmente capibili&rdquo;*). The mutation measurement is not one &mdash; *&ldquo;91% contro 0%&rdquo;* needed three sentences of setup and still read as nothing &mdash; so the thesis table lives on `errors.html` only, and the home's § The claim explains the method in prose and links it. The cards are 231 lines · 0 null/exceptions/GC · 1 hash. |
| | The promise line under the name **leans on something that is not an error** (author instruction 2026-08-24: *&ldquo;cambia il claim in home page facendo leva su qualcosa che non sia un errore&rdquo;*): it now reads *&ldquo;Small enough to fit in a prompt, real enough to compile itself&rdquo;*, both halves measured. The thesis has not left the page — it closed the tagline and it still owns § The claim — it just no longer leads. |
| `why.html` | the founding constraint, the thesis, the cost formula, the rules, the objections |
| `errors.html` | diagnostics as a deliverable — real output, fixes, holes, the mutation numbers. Not in the nav: it is a sub-page of `docs/`, linked from the docs landing, with a crumb and `Docs` marked current |
| `selfhost.html` | the fixpoint: the hash, what it took, and what it found |
| `zen.html` | `heroes this`, quoted verbatim — the twenty lines are the binary's own text, lyric fragments included, so the titles-only rule governs the page's nod and not the quotation |
| `panel.html` | how a change is decided: the five seats and their differentiated briefs, the four vetoes, the historian's search obligation, the borrowed languages, and who is building this (author request 2026-08-18) |
| `author.html` | Giuseppe Arici, the long bio in his own voice, the book, and what the book has to do with the language. Governed by § The author and the book below, which is not optional copy guidance |
| `thanks.html` | the credits: the designers of every language the design borrowed from, named one by one, each with the borrowing beside them. Governed by § The thanks page below |
| `log.html` | the build log, one postcard per milestone, newest first |
| `docs/` | the documentation: landing + chapters |
| `style.css` | the only cross-page *stylesheet* |
| `images/` | the one binary asset the site has: `giuseppe-arici.jpg`, the author's portrait, used by both editions of `author.html` |

**No JavaScript and no external assets**, on any page, still: light and dark come
from `prefers-color-scheme` alone. There IS a build step now, and it is the one
thing the move to Astro changed about what a visitor receives — which is nothing.
Astro ships no JavaScript unless a page asks for one, and no page here asks. The
domain (heroes-lang.org) is already owned by the author.

**No Tailwind either, and that is a deliberate difference from the author's other
site**, which uses it. This site has its own hand-written 45 KB `style.css` and
that stylesheet *is* the art direction; a utility framework beside it would be a
second design system earning nothing.

The stylesheet is a file rather than a `<style>` block per page because a
duplicated palette drifts invisibly — one page's dark mode goes stale and nothing
fails. The bolt stays inline SVG in every page, because it is markup.

**The nav is written once, in `src/components/SiteNav.astro`.** It used to be
duplicated in each page's markup, and the cost of that was measured on the day
it ended: **44 pages, 44 distinct nav blocks, no two identical.** They differed
by exactly three mechanical things and nothing else, which is why one component
reproduces all 44: `class="here"` on the current entry, how many `../` the links
climb, and the link to the other edition. Adding a nav item was editing 44 files;
it is now editing one list.

Two depths, and they are not the same number: `/it/index.html` is one directory
below the site root, so its stylesheet is `../style.css`, but it is the TOP of
the Italian edition, so its nav links carry no `../` at all. The stylesheet depth
belongs to the layout and the nav depth to the component, and conflating them is
how the language switch breaks on exactly the Italian pages.

Nine items in a fixed order — Why Heroes · Docs ·
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
of. Adding an entry is now a line in `SiteNav.astro`'s list, and the 44 copies it
used to mean are gone.

**This paragraph used to end differently**, and the sentence it ended with is
worth keeping as a record of how the decision was actually made: *"Adding a page
means editing that block everywhere; if that ever gets painful, the answer is a
generator behind a `heroes` subcommand, not a script (CLAUDE.md §10)."* It got
painful, and the answer was neither — it was Astro, which is a third option that
sentence did not have in view. §10's stopping rule is what rules out the
subcommand and it still does: serving or generating a static site is not a
property of the language, and `heroes build-site` would be the actual breach.

**Syntax colouring is spans in the markup**, for the same reason. The token
classes are the lexer's own tables rather than a guess — `.k` is
`selfhost/keywords.hero`'s `keyword` (it was `crates/heroes/src/lexer/keywords.rs`
until M-bootstrap-archive), `.t` is spec § Types — and a
user's own type is deliberately left uncoloured, because inferring it from a
capital letter is a premise about a convention the spec does not state. Blocks
are generated *from* the source file, so highlighting and byte-fidelity arrive
together.

## Looking at it before it is published — `site/serve.py`

The site answers on **its own name**, locally, over HTTPS, with a certificate the
browser accepts without a word:

    sudo python3 site/serve.py --host heroes-lang.org      # https://heroes-lang.org
    python3 site/serve.py                                  # https://localhost:8443
    python3 site/serve.py --host heroes-lang.org --port 8443   # no sudo
    python3 site/serve.py --it                             # opens the Italian edition

**Why the real name and not localhost.** Every absolute URL on these pages is
`https://heroes-lang.org/...`: the `canonical` of all 44 pages, the three
`hreflang` alternates on each of them, the `og:url`, the sitemap's 44 entries.
Those are generated from one path per page now rather than typed out, which is
what the move to a layout bought: 176 absolute URLs that cannot disagree with
each other.
Under `localhost` none of that is exercised — the canonical points somewhere
else than the page you are reading, and the language switch crosses an origin.
Served under the real name, a mistake in any of it shows up here rather than
after the launch. `sudo` is only there because 443 is a privileged port; the
script refuses it without root and prints both ways out.

The setup, once per machine (2026-08-19):

1. `127.0.0.1  heroes-lang.org` in `/etc/hosts`.
2. `brew install mkcert nss` — `nss` is what makes Firefox trust it too.
3. **`mkcert -install`** — creates a local CA and puts it in the system trust
   store. It asks for your password, and it is the one step the script cannot do
   for you: `security add-trusted-cert` needs a terminal that can prompt.

Then `serve.py` does the rest. Per host, on first run, it writes a certificate
into `site/.cache/` (git-ignored: it is a key, it is per-machine, it expires)
carrying `subjectAltName` for the name, `localhost` and both loopback addresses
— the field everyone forgets, and without which a browser rejects the
certificate outright and makes it look like a server bug. Without `mkcert` it
falls back to a self-signed certificate from `openssl`, which works and warns
once. If `mkcert` made the certificate but its CA is **not** in the trust store,
the script says so and prints the one command that fixes it, because that state
is indistinguishable from a broken server if nobody tells you.

Everything else it does is in service of not lying to you: it serves `dist/`
and nothing else, sends `Cache-Control: no-store` so a reload can never show
yesterday's CSS, logs one line per request, and lives outside `dist/` so it
can never be deployed with the site.

**It serves `dist/` and not the sources**, so what is read here is the artifact
the deploy uploads. Run `npm run build` first; without it the script stops and
prints the two commands rather than serving an empty directory. `npm run dev` is
the faster loop, and it does not replace this one: the dev server does not answer
on the site's real name over HTTPS, which is the only reason this script exists.

**Why it is a script and not a `heroes` subcommand.** CLAUDE.md §10 says every
capability is a subcommand of the one binary, and §10's own stopping rule is
what keeps this one out: a capability enters that surface only if the fixpoint
invocation, the golden harness or the Part 11 harness must type it, or it has a
measured Part 11 effect. Serving a static directory is none of those, and it is
not a property of the language, so `heroes serve` would be the actual breach.

Verified by running it, 2026-08-19: under `heroes-lang.org` the certificate
**verifies against the mkcert CA root** and both editions, the stylesheet, a
nested chapter and `sitemap.xml` answer 200 with the right content type; a
missing page answers 404; the no-store header is present; the key is mode 0600;
and asking for port 443 without root exits with the two commands that work
instead of a stack trace.

Re-verified against `dist/` after the build landed: both landings, both
documentation landings, a nested chapter in each edition, the stylesheet, the
portrait, `robots.txt`, `llms.txt` and both sitemap files answer 200 with the
right content type, and a missing page answers 404. `sitemap.xml` is the one
name in the paragraph above that no longer exists: the sitemap is generated, and
it is `sitemap-index.xml` plus `sitemap-0.xml`.

## Deployment — Cloudflare Pages, behind a gate

The route is **Cloudflare Pages, by Direct Upload from GitHub Actions**, and it
is the same one the author's other site runs on. Its only home is this section.

**The site is deployed and it is not published.** Those are two different acts
here, and keeping them apart is the whole design. CLAUDE.md §14 makes publishing
a hard stop only the author lifts, and § Launch order below says why it is not
lifted: `llms.txt` and every *check me* link on these pages points at a
repository that answers 404 while it is private.

### The gate

`site/functions/[[path]].js` is a Pages Function that matches every URL.
Cloudflare evaluates Functions **before** static assets, so that one file
shadows the entire site: the real pages are uploaded underneath, complete and
warm, and simply unreachable. `/` answers **200** with the holding page,
everything else answers **404** with the same body, and both carry
`x-robots-tag: noindex`.

**Opening the site is deleting two files**, `site/functions/[[path]].js` and
`site/public/_routes.json`. No rebuild of anything else, no domain to move
between projects, no setting to find in a dashboard.

To read the real site before then, visit any URL with `?preview=starman` once:
it plants a cookie and redirects to the clean URL, so the token stops riding in
the address bar. The token is written in a public file on purpose — it is a
speed bump, not protection, and calling it one stops anybody relying on it.

`public/_parking.html` is self-contained, and that is not tidiness.
`_routes.json` excludes that one path from the Function, so a request for
`/style.css` reaches the Function like every other and comes back as the holding
page. A stylesheet link in it would render unstyled.

### The workflows

| file | what it does |
|---|---|
| `.github/workflows/deploy-site.yml` | on push to `main` touching `site/**`, and on demand: `npm ci`, `npm run build`, `wrangler pages deploy dist` |
| `.github/workflows/release-site.yml` | `git tag site-v1 && git push origin site-v1` ships the tree by hand |

Three details in there are load-bearing and each one fails **silently** if moved:

- **`CF_DEPLOY` is computed at job level**, not on the step, because a step's
  own `env` is not visible to that step's `if`. A clone without the secrets
  stays green and skips the deploy rather than failing on a credential it never
  had.
- **The deploy step runs from `site/`** and hands wrangler `dist`. Wrangler
  collects Functions from a `functions/` directory in the working directory it
  is run from, never from inside the output directory it is given. Move
  `site/functions/` without moving the step's `working-directory` and the deploy
  still succeeds, with every URL of the site open and nothing in the log to say
  so.
- **`--branch=main`** marks the upload as production even when the run came from
  a tag.

Direct Upload rather than Cloudflare's native git integration: the native one
spends one of the Free plan's 500 monthly builds on every push to `main`, and
most pushes here touch only the compiler. This path spends none of them, and
about a minute of Actions against 2,000. **Use one path or the other**; the git
integration stays off.

### The credentials

Two repository secrets, `CLOUDFLARE_API_TOKEN` and `CLOUDFLARE_ACCOUNT_ID`.
Their shape, the exact permissions and the traps are documented in `.env.example`
at the repository root, which is the one place they are written down; `.env`
beside it is gitignored and holds the real values for a `wrangler` run started
by hand.

For a deploy that stops at `pages.dev`, the token needs one policy: resource
**Entire Account**, `Cloudflare Pages: Edit` and `Account Settings: Read`. Give
it a short expiry.

### The domain, which has not moved

The Pages project is `heroes-lang-site` and the site answers on
`heroes-lang-site.pages.dev`. **`heroes-lang.org` is not on Cloudflare**:
measured against the `.org` registry, resolver `1.1.1.1`, resolver `8.8.8.8` and
whois, it is served by `dns106.ovh.net` and `ns106.ovh.net` and still points at
OVH's parking address. `public/CNAME` records which domain the site expects;
Cloudflare Pages ignores it.

Attaching the real domain needs the nameservers moved, because **a `CNAME`
cannot exist at the apex of a zone** and Pages is reached by `CNAME`. OVH's DNS
has no `ALIAS` to work around it; Cloudflare's own zone flattens the apex
natively. Two things in that order, and the first one is easy to forget:
**DNSSEC comes off at OVH first** — the domain is signed today, and switching
nameservers while the `DS` record is still published in the registry takes the
domain off the network until the signatures agree again. Then add the zone at
Cloudflare, check the imported records, verify the new nameservers answer
correctly before delegating to them, switch at OVH, wait for **Active**, and
only then attach the custom domain to the Pages project. Attaching while the
zone is still Pending writes no DNS record *and* makes Pages choose HTTP
validation, which parks in a circular stall.

None of that is done, and none of it is owed until the author asks.

## The Italian edition — `site/src/html/it/`

The site ships in two languages (author instruction 2026-08-18: *"traduci tutto
il sito anche in italiano … lascia in inglese i termini tecnici"*). This extends
CLAUDE.md §11's declared exception — which already covers the two books — to the
site, and the same rule applies: **neither edition is a machine translation of
the other**, and where they diverge the Italian is fixed to read better rather
than the English to read more literally, because the author studies from the
Italian.

Mechanics. They were chosen when there was no build step, and the build did not
change one of them, which is the argument for having kept them this simple:

- The Italian edition lives in **`site/src/html/it/`, same basenames**
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
- **Never a bare capital `M` immediately followed by a digit** — the shape
  `M<n>`, with or without a trailing letter. CLAUDE.md §14 says a milestone id
  never reaches user-visible output, and the net enforces it over every living
  file (`tests/harness/suite_records.hero`, `records/numbered`). This is not a
  spelling rule with a lint bolted on: the lowercase form really is one of this
  repository's twelve legacy tags, so nothing reading the token alone can tell
  the milestone from Apple's chip of the same name. That collision is the one
  that will keep arriving, and the answer is to name the **machine**, *a MacBook
  Air*, which tells a reader what the numbers were measured on better than a
  chip name does. Found 2026-08-28, on the page's own hardware sentence — and
  then a second time, three minutes later, because the first draft of this rule
  quoted the very shape it forbids. The check reads a backtick as a word
  boundary, so a living file cannot spell the token even to ban it. That is why
  this bullet describes the shape instead of showing it.
- Nods spent so far: sound and vision · fashion · quicksand, avoided ·
  changes · station to station · rebel rebel · always crashing in the same
  car · a new career in a new town · oh! you pretty things · hunky dory ·
  under pressure · five years · look back in anger · absolute beginners ·
  moonage daydream · lady stardust · sons of the silent age · ashes to ashes ·
  speed of life · repetition.
- **"Ashes to Ashes" is spent** (2026-08-19), on the section of `selfhost.html`
  that reports the bootstrap's retirement — which is the event the bank was
  holding it for. M-bootstrap-archive closed the same day, so the section that
  used to be headed *What is left* now says both owed things are paid.
- Ideas bank, still unspent: a 1.0 → "Golden Years".

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
- **The name is set clean** (author instruction 2026-08-18). The home `h1`
  carried a two-plate misregistration — red a hair left, blue a hair right, the
  1973 cover said in type — and it is gone, along with the `h1 .q` override that
  existed only to keep the gold quotation marks out of it. Nothing sits behind
  those letters now. The two stage colours still carry the page from the wash
  behind the hero and from the bolt, which is where they belong; do not put them
  back into the type.
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
- **The portrait is printed, not pasted** (author instruction 2026-08-19:
  *"usa questa immagine duotonata nella pagina autore"*). The photograph on
  `author.html` is duotoned to two of the site's inks — a deep blue in the
  shadows, cream in the highlights — so it belongs to the palette instead of
  sitting on top of it. **Blue and cream, not blue and red**, because the red/blue
  pair is the hero wash and the first rule in this list spends it there and
  nowhere else.
  - **It is an SVG filter inline in the page, and it has to be.** CSS blend modes
    cannot do a duotone: `lighten` and `multiply` clamp the two ends and leave the
    midtones grey, which is a warm photograph and not two inks. Rendered and
    compared before this was written. The filter is one `feColorMatrix` to
    luminance, one `feComponentTransfer` at `gamma 0.72` to lift the midtones
    (without it the shaded half of the face goes muddy — also rendered and
    compared), and one more mapping black to the dark plate and white to the
    light one. Inline rather than a `data:` URI because a same-document fragment
    is the only filter reference every browser resolves.
  - **The file that ships stays the photograph**: no tool touches it, replacing it
    is a copy, and the JPEG's white ground becomes the cream plate on its own, so
    the frame reads as a print on paper. Verified in both themes: on the dark
    stage the cream frame is the bright object on the page; in light mode it is
    within a few values of the paper, so the portrait reads as printed straight
    onto it with only the hairline border around it.
  - `--plate-light` is a live token (the ground a cut-out would print on).
    `--plate-dark` is **recorded** rather than used: a filter cannot read a custom
    property, so the numbers live in the filter's own tables. Change one, change
    the other.
  - **The prose runs alongside it** (author instruction 2026-08-19: *"fagli
    girare il testo a fianco e dagliela più grande"*), which needs a `float`, and
    a `float` needs a block container: `main` is a grid and **a grid item ignores
    `float` outright**, which is why the first attempt put the portrait above the
    lede. Hence `.byline`, holding the portrait and the lede and nothing else.
    Two things it must keep doing: the `h2` below stays a **direct child of
    `main`**, because the section counter is `main:not(.home) > h2` and nesting it
    drops the number in silence; and the wrapper is `display: flow-root`, so a
    portrait taller than the lede is contained rather than reaching into the
    section below — and contained without clipping, which `overflow: hidden`
    would not manage.
  - **It hangs into the right bleed** by the same amount a code panel does, so
    its outer edge lands on the line every figure on the site is aligned to and
    the reading column only gives up the difference. At 16.5rem the lede runs
    eight lines and ends within two pixels of the portrait's bottom edge — that
    alignment is luck rather than design, and it is worth not disturbing.
  - Three widths, each rendered: at 1000px the float and the bleed; at 860px the
    bleed tracks are gone, so the portrait drops to 13rem and its negative margin
    to zero; under 620px the float goes and it sits above the lede at 11rem,
    because a float there leaves the lede in a three-word column.
  - The image is served at **720×720 and 121 KB** for a frame 264px wide, which
    leaves it sharp on a 2× screen and some headroom above that. It arrived at
    1024×1024 and 837 KB; the original is not in the repository, so ask the author
    for it before enlarging the frame much further.
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

7. **The portrait is the author's own photograph, supplied by them** for this
   page (2026-08-19). It is the only picture of a person on the site, and rule 4
   still governs everything around it: the page names the city and the company
   and links the personal GitHub profile, and nothing else. The alt text
   describes the photograph and names the cap, because the lede's own line is
   *"a cap for every language"* and a reader who cannot see the picture should
   still get the joke.

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
- **The checks are re-run before the page is touched, and the page no longer
  says when.** The copy used to carry *"looked up and found, on 2026-08-18"* and
  the date came out with every other date on the site (`CLAUDE.md` § No dates on
  the page). What the date was buying is still owed: re-run every lookup in the
  session that edits an entry, because a page that names people has no other
  guard.
- **Nobody on the page was asked, and the page says so.** No characterisation of
  a living person beyond the borrowing itself, and no claim that anyone endorses
  this language.

## The claims that have a gate on them

- **The gate on "v1" has opened, and the site still does not use the word**
  (2026-08-19). Both conditions this bullet named are met and were re-measured
  the day it was rewritten: `clang -I runtime seed/heroes.c runtime/runtime.c -o
  heroes` builds the compiler from a clean checkout with nothing but clang, and
  `crates/` is now `archive/bootstrap-rs/`. What changed on the pages is the
  facts — the build instructions, the counts, and `selfhost.html`'s closing
  section — **not** the label: calling the language *v1* in public is a release
  decision and it is the author's, so it waits for them rather than arriving as
  a side effect of a documentation pass.
- **Numbers are re-measured in the session that writes them** (CLAUDE.md §1),
  and **the unit has to match the record's**: `./heroes measure`
  for spec tokens, `find selfhost -name "*.hero" | wc -l` and `| xargs wc -l`
  for the port, `grep -rn "PORT-DEBT" selfhost/ | wc -l` for the workarounds.
  Note that journal 021 and the ROADMAP count *tests* (27,230) while a grep of
  `^test "` counts *test blocks* (447) — different questions, so never mix the
  number of one with the word of the other. The numbers `panel.html` and
  `why.html` added on 2026-08-18: panel sittings are
  `ls docs/panel/[0-9]*.md | wc -l` (83, and note the highest *number* is 085 —
  the sequence has gaps, so the count and the last id are different questions),
  and the runtime is `find runtime -name '*.c' -o -name '*.h' | xargs wc -l`
  (3,417 lines, which includes `runtime/parts/`; a glob of `runtime/*.c` alone
  answers nothing). The compiler is
  `find selfhost -name '*.hero' | wc -l` (165) and `| xargs wc -l` (48,342), and
  the seed is `wc -l -c seed/heroes.c` (798,556 lines, 23,248,564 bytes).
- **Compiler output shown on the page is verbatim**, path and test annotations
  included. A trimmed-for-looks diagnostic is a fabricated diagnostic: the
  first draft of this refresh shortened one and got the caret width, the line
  content and the fix text wrong in the process.

## `site/src/html/docs/` — twelve chapters, written

The landing page lists all twelve chapters and links `errors.html` above them,
which lives at the site root but belongs to this section. **All twelve are
written, in both editions** (2026-08-19), on the conventions
`failure-is-a-value.html` fixed as the exemplar. The rule they all obey:

> Every code block on the site is a file in `examples/`.

**Blocks are generated from the file, never retyped.** The generator that did
it lives outside the repository (a scratchpad script), and the check that it
was worth trusting is that regenerating the exemplar's hand-written blocks
reproduced them byte for byte, which is also the check that its token classes
match the lexer's own tables. Anything that regenerates a block later has the
same obligation: `.k` is `keyword` in `selfhost/keywords.hero`, `.t` is the
spec's type list, and a user's own type stays uncoloured.

**The whole set was regenerated once, and how it was done is the procedure for
next time.** `heroes fmt --in-place` swept every `.hero` file in the repository
into canonical form (panel 095 stage 4), which moved line numbers in ten of the
files the site slices — **88 of 152 figures drifted**, and the drift was entirely
the formatter's: all 152 still matched the file as it stood at the sweep's parent
commit, which is what proves no figure had been hand-edited. So the fix was
mechanical rather than editorial: diff each file against its pre-sweep self,
carry each figure's `data-lines` through that map, take the new slice, and pull
the boundary inward off any blank line the formatter had just inserted. Two
checks make it safe to trust. Every figure now matches its `data-src` slice, and
**all 152 round-trip through the highlighter byte for byte** — including the 64
that had not drifted, which is the gate: a highlighter that reproduces an
untouched block exactly is one that can be let near a changed one. The
`figcaption` is rewritten in the same pass as the block, in either language, so
the caption and the attribute cannot disagree.

**Every diagnostic on a chapter page is real output, produced in the session
that wrote the page**, by copying the gallery file and making one edit — the
edit is named in that page's footer, so a reader can reproduce it. No
diagnostic on this site was typed by hand.

**A chapter carries the previous chapter as well as the next one.** Twelve
chapters read in order need a way back that is not the browser button; the
backward link is set quieter than the forward one, because leaving is not the
default.

The order is fixed by the landing page's list and by the `prev`/`next` links,
which have to agree: adding or moving a chapter means editing three places,
and the link check catches only the dead ones, not the wrong ones.

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

The `in progress` tag and its `.todo` row are kept in the stylesheet although
no chapter uses them now: a thirteenth chapter would be listed that way, with a
dim tag and **no link** — never a dead one. The same rule governed the twelve
while they were being written, and it is why `check.py`'s dead-link pass has
never had anything to report.

## Keeping it current — on demand, not per milestone

When the author asks for a site refresh (typically before publishing, or
after a stretch of milestones): update the badge on `index.html`; append one
entry per milestone closed since the last refresh to `log.html` (newest first,
postcard register, distilled from `docs/book/beats.md`, not from the commit log);
re-check `why.html`'s objections against design.md's current state; re-run every
number; re-check each code block against its `data-src` anchor.

**And re-run every diagnostic whose file moved, because a code block and a
diagnostic drift for the same reason and only one of them is checkable.** The
`data-src` check catches a stale block. Nothing catches a stale `line:column`
inside a `<pre class="diag">`, and after the `fmt` sweep **nine of them were
wrong** across two editions: the hole on `errors.html` said line 26 and the
compiler says 28, `mutation.hero:53` is 55, `generics.hero:16` and `:46` are 18
and 52, `strings.hero:11` and `:39` are 12 and 45, `loops.hero:12` is 13,
`trees.hero:73` is 81, and both holes on `tests-and-holes.html` moved. Each
chapter's footer names the edit that produced its diagnostic, which is what makes
this mechanical: copy the current gallery file, make that one edit, run the
command in the transcript, paste. Two things learned doing it. The FFI
misspelling has to be made **at the call site as well as the declaration**, or
`unknown_name` fires first and the page's `ffi_unknown_name` never appears. And
the diagnostics over `tests/golden/check/` need nothing: that directory was
excluded from the sweep by author decision, so its line numbers are the same ones
the page was written with.

`log.html` is the one page exempt from the invariant that no living file names a
numbered milestone, because its entries are records and keep the identifiers they
were written with — the same footing as `docs/journal/` and `docs/panel/`. Every
other page is watched. The entries carry **no dates** (`CLAUDE.md` § No dates on
the page): the order of the list is the chronology, so a new entry goes on top
and nothing else moves.

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
different numbers on 2026-08-18. Re-run the tool before publishing any of them.
**The result is not dated on the page** (2026-08-28, author instruction; the rule
and its reasons live in `CLAUDE.md` § No dates on the page) — it is dated in the
commit body, where the next session looks for it.

And **never pool the mutation rates into one headline** — `heroes mutate` prints
the prohibition (panel 011) every time it runs, and the site shipped the pooled
number anyway for a full round. Per-operator or nothing. `forget-at-decl` is
excluded from any summary because its catch rate is 100% by construction.

## One thing the publication gate owns

`site/`'s Aladdin Sane bolt is iconography attached to an actively managed
estate — the style guide already keeps lyrics out, and this is the other half.
Recorded in `docs/ROADMAP.md` under M-publication-gate; cheaper to answer
before publication than after.
