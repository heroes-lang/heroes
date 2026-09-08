# site/ — the showcase site for heroes-lang.org

One self-contained *directory*, shaped like a language site rather than one long
page (author instruction 2026-08-17: *"more the shape of language sites, look at
go, rust — I do not want a mega scroll page"*). Every page carries the same nav,
so no page is a dead end:

**The site is built.** `npm run build` turns `site/src/` into **`site/dist/`**,
and `dist/` is the whole deployable artifact: nothing outside it is uploaded, so
publishing can never leak this README or the panel's briefs. Three directories
hold the source, and each answers a different question:

| directory | what is in it |
|---|---|
| `src/html/` | the pages themselves, one HTML fragment each: the inside of `<main>`, prose and code, and a `<footer>` holding only the closing paragraphs that belong to that page, if it has any |
| `src/pages/` | one tiny `.astro` per page, carrying four facts and no content: its path, its title, its description, which nav entry is current |
| `src/layouts/`, `src/components/` | the head, the nav and the footer's shared tail, written **once** |
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
npm run build           # src/ -> dist/, 180 pages
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
`src/html/docs/maps.html` is `/docs/maps/` (§ The URLs, and why they end in a slash).

| source file | what it is |
|---|---|
| `index.html` | the landing: hero, one sample, three cards, one real diagnostic, the claim. Short on purpose, and measured: 758 words where it was 1,979. |
| | The hero says **&ldquo;a programming language&rdquo;** above the name, because a visitor who has never heard of Heroes should not have to infer the category (author instruction 2026-08-17). It said *compiled* too until 2026-09-08, when the claim under the name started with *A compiled language* and the word would have been said twice in two lines; the category is still stated before the name, and *compiled* is the first thing the claim says. Its code panel shows **working code, never a diagnostic** — *&ldquo;opening a language's site on an error is ugly&rdquo;*, same date. The errors have their own page. In that panel the `bar` names the file, so the figure carries no second caption. |
| | **The three figure cards carry only at-a-glance numbers a stranger parses without context** (author instruction 2026-08-25: *&ldquo;put only at-a-glance figures that are easy to understand&rdquo;*). The mutation measurement is not one &mdash; *&ldquo;91% against 0%&rdquo;* needed three sentences of setup and still read as nothing &mdash; so the thesis table lives on `errors.html` only, and the home's § The claim explains the method in prose and links it. The cards are 231 lines · 0 null/exceptions/GC · 1 hash. |
| | The promise line under the name **leans on something that is not an error** (author instruction 2026-08-24: *&ldquo;change the claim on the home page so it leans on something that is not an error&rdquo;*): it read *&ldquo;Small enough to fit in a prompt, real enough to compile itself&rdquo;*, both halves measured. The thesis has not left the page — it closed the tagline and it still owns § The claim — it just no longer leads. Rewritten by author instruction 2026-09-08, to read as a headline, to say on the line itself what a prompt is, and to name what the language is, compiled and without frills, for the reader who arrives from a link expecting one more language written by a model: *&ldquo;The whole language fits in one short prompt, the text you give a model when you ask it to write code. It compiles to C, has no frills, and the compiler is written in Heroes and compiles itself.&rdquo;* Every clause is a fact a reader can check on this site. The author's brief also offered *fast*, and the line does not say it: no page here measures a program's speed, the only timed fact on the site is the compiler's own build, and an adjective without a number is the first thing that reader would ask to see proved. The line names the mechanism, compiled to C, and leaves the adjective to them. **That line lasted a day.** The author read it as explaining to visitors what they already know, went through two rounds of proposals, and wrote the line themself, in Italian; the English is its translation: *&ldquo;Decades of programming-language history, condensed into fewer than 4,096 tokens. A compiled language, mature enough to compile itself. No interpreter, no virtual machine: underneath, all the power of C.&rdquo;* The token figure is the specification's ceiling from design.md §1.6, which a test holds on every commit; the build reads it from that test and checks that both home pages spell it, so the sentence cannot go stale the way a count would. The author's brief said bytes, and the measurement said tokens, 3,903 of them today against 12,517 bytes, so the line says tokens. The self-hosting half is *mature enough to compile itself*, which says the thing without the term; three earlier cuts, *written in the language itself, compiles itself*, *written in the language it compiles* and *compile its own compiler*, were read aloud and the author set each aside for its sound (2026-09-08). The word *prompt* left the line with the author's version, and the `curl` line under the buttons still says where the prompt is. Three red emphases, placed by the author: the ceiling, *compile itself*, and the closing *underneath, all the power of C*; the rest of the line is already bold by design, so red is the only emphasis the slot has. |
| `why.html` | the founding constraint, the thesis, the cost formula, the rules, the objections |
| `docs/errors.html` | diagnostics as a deliverable: real output, fixes, holes, the mutation numbers. **Chapter 2 of the guide since 2026-09-08** (author instruction), where it was a page of its own at `/errors/` linked from the docs landing. A separate page said errors were beside the language rather than in it, and the guide's own second chapter is where a reader meets them: you write a program, then you learn to read what the compiler says when it is wrong. `_redirects` keeps the old URL answering |
| `start.html` | how to run it: the four lines, where `heroes run` looks for the runtime, your own first program, the eleven verbs |
| `project.html` | three sections, and it replaced three pages (author instruction 2026-09-06): the fixpoint at `#self-hosted`, how a change is decided at `#panel`, and `heroes this` quoted verbatim at `#zen`. The twenty Zen lines are the binary's own text, lyric fragments included, so the titles-only rule governs the page's nod and not the quotation, and they stay English in both editions |
| `about.html` | Giuseppe Arici, the bio in his own voice, the book, and *Who wrote this*, which says who did what. Governed by § The author and the book below, which is not optional copy guidance |
| `about/thanks.html` | the credits: the designers of every language the design borrowed from, named one by one, each with the borrowing beside them. A **sub-page of `/about/`** since 2026-09-07, the way `errors.html` is one of `/docs/`, because the nav holds one entry for the author and the names may not be cut to fit a word budget. Governed by § The thanks page below |
| `log.html` | the build log, one postcard per milestone, newest first. A postcard is the milestone name, the sentence in bold that is the news, one concrete sentence and a link: 1,517 words where it was 4,846 |
| `examples/` | **the examples browser, generated from `examples/` at build time**: an index, one page per program and per gallery file, and `index.json`. Its own § below |
| `docs/` | the documentation: landing + chapters |
| `style.css` | the only cross-page *stylesheet* |
| `images/` | the one binary asset the site has: `giuseppe-arici.jpg`, the author's portrait, used by both editions of `about.html` |

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
it ended: **46 pages, 44 distinct nav blocks, no two identical.** They differed
by exactly three mechanical things and nothing else, which is why one component
reproduces all 44: `class="here"` on the current entry, how many `../` the links
climb, and the link to the other edition. Adding a nav item was editing 44 files;
it is now editing one list.

Two depths, and they are not the same number: `/it/index.html` is one directory
below the site root, so its stylesheet is `../style.css`, but it is the TOP of
the Italian edition, so its nav links carry no `../` at all. The stylesheet depth
belongs to the layout and the nav depth to the component, and conflating them is
how the language switch breaks on exactly the Italian pages.

**Eight items, and each one is a question a visitor has** (author instruction
2026-09-06) — Start · Guide · Examples · Why · Project · Log · Author · Thanks,
plus the language badge and GitHub — and the current page marks itself
`class="here"`.

**The first four were reversed by author instruction on 2026-09-08**, the day
the repository opened. They read Why · Examples · Guide · Start, which is the
order somebody **asks**: why should I care, show me the code, teach it to me,
how do I run it. They now read Start · Guide · Examples · Why, which is the
order somebody **acts**. The old reasoning is kept because it was not wrong, it
was answering a different question, and what changed under it is the site: a nav
sorted by curiosity serves a reader, one sorted by intent serves a user, and
until that morning the *Start* page ended at a command nobody could run. Project
stands before Log by author instruction the same day: the log is the project's
log, so the thing comes before its diary.

It was eight items that were the project's own shelves instead, and neither a
learning entry nor an install entry was among them, which every one of the nine
language sites surveyed while planning has in its top nav.

Three pages became `project.html` and two became `about.html`, which is what
made room. **Measured after the change**, in Chrome, both editions, twelve
widths from 1200px down to 390px:

| | English | Italian |
|---|---|---|
| items | eight | eight |
| one row of chrome (50px) | 801px and up | 880px and up |
| two rows (80px) | 800px and down | 870px and down |

The eighth item is Thanks, put back after Author by author instruction
2026-09-07 (it had been seven since 2026-09-06). The Italian label is
`Grazie`, the page's own title, and not `Ringraziamenti`: the longer word put
the Italian row on two lines at 900px where the English held one. Even so the
Italian row now breaks between 870 and 880px where it used to break with the
English at 800: measured after the change, two rows at 870, one at 880. The
widths in the earlier version of this table, 402px and 426px, were the
seven-item list and are no longer true.
| three rows | never | never |
| the list scrolls sideways | never, down to 390px | at 390px |

The nine-item row was 517px in English and 540px in Italian, so the row is
**115px shorter** and the two-row band starts at the same place it did. The
Italian list is 24px wider than the English one, which is `Esempi`, `Inizia`,
`Progetto` and `Perché` costing what they cost. Both editions reach the
scrolling line at 390px, which is the affordance working rather than a fault:
no item is dropped. The English list gained 8px when the GitHub link stopped
saying only `GitHub ↗` and started saying `GitHub · soon`, which is the visible
half of a warning that used to live in a `title` nobody sees on a phone.

**The row stays at seven, so a page enters it only by displacing one.** That
rule is unchanged from when the row was nine (author instruction 2026-08-18);
only the number moved. The exchange that proved it: `thanks.html` went in and
`errors.html` came out, and a tenth item was once built, measured and removed,
because it produced **three rows** of chrome between 720px and 701px, which is
the state the rule forbids. The first thing tried instead — reaching the page
from every footer plus a sentence inside what is now `project.html` — failed the
only test that counts: the author could not find it. **A page nobody can see
from the chrome is not linked, whatever the link count says.**

Two pages sit under an entry rather than in the row, for that reason and not by
accident: `errors.html` under Docs, and `about/thanks.html` under About. Both
carry a `.crumb` and both mark their parent as current.

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

**The footer is written once too, in `src/components/SiteFooter.astro`**
(author instruction 2026-09-08: the footer was repeated on every page, and
Astro can do better). Measured the day it moved: the 44 hand-written fragments
carried 44 footers, 42 of them distinct, and every one was the same three parts
in the same order, the page's own closing paragraphs, zero to two of them, then
the byline, then the copyright line. Only the first part belongs to a page, so
that is all a fragment keeps, inside the `<footer>` it always had; `BaseLayout`
lifts that element out of the rendered page (`src/lib/footer.ts`) and
`SiteFooter` renders its paragraphs above the shared tail, inside `<main>` where
the CSS expects a footer, so no wrapper under `pages/` changed. The byline had
drifted while it was copied: three wordings in Italian and two in English across
the hand-written pages, the examples and the specification page, none of them
decided. It is now one fragment per edition, `src/html/_byline.html` and
`src/html/it/_byline.html`, a fragment rather than markup in the component
because `records/english` reads every `.astro` file and admits Italian prose
only under `src/html/it/`. A page whose own `<footer>` carries a byline or a
copyright line fails the build, so the old shape cannot be pasted back one page
at a time. Checked the way the layout's own comment asked: the site was built
before and after and all 180 pages compared word by word, ignoring the
whitespace between tags, and outside the footer nothing moved. Inside it, the
byline took the wording the 21 hand-written pages of each edition carried, which
is the one the site was written with; the 67 examples pages per edition had
taken another in the copy pass of the day before, and the Italian specification
page a third, so those read differently now. The about page links the book's
title as every other page does, and the thanks page says it stands on other
people's work like every other page does; both used to leave that out because
the reader was already there, a courtesy two pages paid and the other 178 could
not.

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
`selfhost/keywords.hero`'s `keyword` (it was `crates/heroes/src/lexer/keywords.rs` until 2026-08-19
until M-bootstrap-archive), `.t` is spec § Types — and a
user's own type is deliberately left uncoloured, because inferring it from a
capital letter is a premise about a convention the spec does not state. Blocks
are generated *from* the source file, so highlighting and byte-fidelity arrive
together.

## What a machine that is not a browser gets

Author instruction 2026-09-07: *all the SEO tags, and push them hard.* Every one
of them is computed in `src/layouts/BaseLayout.astro` from the single `url`
prop, for the reason the canonical and the hreflangs already were: a card, a
breadcrumb and a canonical that disagree are worse than none, and the only way
three derived facts cannot disagree is if one expression derives all three. **No
page passes any of it in**, so a new page cannot forget it and cannot get it
wrong.

**Structured data, and a type that says something.** A site whose pages are all
`WebPage` has told a machine nothing it could not see. The two landings are
`WebSite`, the example pages are `SoftwareSourceCode` and carry
`programmingLanguage`, the chapters and the specification are `TechArticle`, the
author pages are `AboutPage`, and the landings additionally carry a
`ComputerLanguage` block, which is the type schema.org has for exactly this
subject. Every page carries a `BreadcrumbList` built from its own path, so a
page that moves takes its trail with it.

**The share cards.** Ten sections in both editions, twenty files, under
`public/images/card/`, and `BaseLayout` picks one from the first path segment
with the home card as the fallback, so all 180 pages resolve to a real file.
1200x630, drawn from this stylesheet's own tokens and its own h1 face. Two
site-wide cards were tried first and were the wrong shape: a link is a link to
ONE page, and a card that says only the name of the site tells the reader
nothing about the page they were sent. **No card carries a number**, by § A
number on the page in `site/CLAUDE.md`: a measured number inside an image cannot
be checked by whoever reads it, which is why the examples card does not say how
many programs there are. The generator is not in the repository; regenerating a
card is a build of one HTML file and a headless screenshot.

**The sitemap's four tiers** are this site's own ranking of itself: the two
landings at 1.0, the pages the nav points at at 0.8, the example programs at
0.7, the chapters at 0.6. The examples tier is the one it needed and did not
have: 134 of the 180 pages sat at 0.8, the same figure as the seven pages in the
nav, which told a crawler that `examples/nqueens/` matters as much as the front
door of the documentation.

**One guard runs at the end of every build.** `astro:build:done` walks the
output and unlinks every `.DS_Store`. Astro copies `public/` verbatim and macOS
writes that file into any directory Finder has looked at, and `.gitignore`
carries a bare `.DS_Store` so it appears in nobody's `git status`. What this does
NOT claim: nothing was published. `deploy-site.yml` checks the repository out on
a Linux runner and builds there, no `.DS_Store` is tracked and none was ever
committed, so the automated path never had one to copy. The path the guard
closes is the hand-run `wrangler` deploy from a local `dist` described under §
The credentials, which would carry it. A `.DS_Store` is a directory listing,
naming every file that was in the folder including the ones since deleted.

## Looking at it before it is published — `site/serve.py`

The site answers on **its own name**, locally, over HTTPS, with a certificate the
browser accepts without a word:

    sudo python3 site/serve.py --host heroes-lang.org      # https://heroes-lang.org
    python3 site/serve.py                                  # https://localhost:8443
    python3 site/serve.py --host heroes-lang.org --port 8443   # no sudo
    python3 site/serve.py --it                             # opens the Italian edition

**Why the real name and not localhost.** Every absolute URL on these pages is
`https://heroes-lang.org/...`: the `canonical` of all 180 pages, the three
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

## Deployment — Cloudflare Pages

The route is **Cloudflare Pages, by Direct Upload from GitHub Actions**, and it
is the same one the author's other site runs on. Its only home is this section.

**The site is published.** It was deployed and shut for the whole time before
that, and keeping those two acts apart is what let every page be written
finished. CLAUDE.md §14 made publishing a hard stop only the author lifts, and
the author lifted it: the site goes out ahead of the repository, with the
private repository stated on the pages that promise a download. § Launch order
below is the record of that decision and of what it obliges the copy to say.

### The gate, disarmed

The gate that held the site shut is three files, and they are **dormant in
`site/parking/`** rather than deleted. That directory's own README holds the
three paths, how to re-arm them and the two ways the move fails silently; the
mechanism is worth keeping, because the next thing written before it is meant to
be read gets the same door for free. It is also what the author's other site did
at its own launch, and `giuseppearici.com`'s `site/parking/` holds the same three
files for the same reason.

How it worked while it was armed, because § The workflows below rests on it.
`parking/[[path]].js` is a Pages Function matching every URL, and Cloudflare
evaluates Functions **before** static assets, so that one file shadowed the
entire site: the real pages were uploaded underneath, complete and warm, and
simply unreachable. `/` answered **200** with the holding page, everything else
**404** with the same body, and both carried `x-robots-tag: noindex`. Any URL
visited once with `?preview=starman` planted a cookie and redirected to the
clean URL, so the token stopped riding in the address bar. The token is written
in the clear on purpose: it is a speed bump, not protection, and calling it one
is what stops anybody relying on it.

**Arming and disarming is moving files, and nothing else.** No rebuild of
anything, no domain to move between projects, no setting to find in a dashboard.
That is the whole reason the gate was built this way rather than as a password or
a separate staging project.

### The workflows

| file | what it does |
|---|---|
| `.github/workflows/deploy-site.yml` | on push to `main` touching `site/**`, `examples/**`, `selfhost/keywords.hero` or `spec/heroes-spec.md`, and on demand: `npm ci`, `npm run build`, `wrangler pages deploy dist`. The three paths outside `site/` are there because the examples browser reads them, so a change to a program changes a page |
| `.github/workflows/release-site.yml` | `git tag site-v1 && git push origin site-v1` ships the tree by hand |

Three details in there are load-bearing and each one fails **silently** if moved:

- **`CF_DEPLOY` is computed at job level**, not on the step, because a step's
  own `env` is not visible to that step's `if`. A clone without the secrets
  stays green and skips the deploy rather than failing on a credential it never
  had.
- **The deploy step runs from `site/`** and hands wrangler `dist`. Wrangler
  collects Functions from a `functions/` directory in the working directory it
  is run from, never from inside the output directory it is given. That is what
  makes the gate a file move, and it is the trap on the day it is re-armed: put
  the catch-all anywhere but `site/functions/` (disarmed 2026-09-03), or move this step's
  `working-directory` away from it, and the deploy still succeeds with every URL
  of the site open and nothing in the log to say so.
- **`--branch=main`** marks the upload as production even when the run came from
  a tag.

Direct Upload rather than Cloudflare's native git integration: the native one
spends one of the Free plan's 500 monthly builds on every push to `main`, and
most pushes here touch only the compiler. This path spends none of them, and
about a minute of Actions against 2,000. **Use one path or the other**; the git
integration stays off.

**And the traffic goes the other way too.** `ci.yml`, the compiler's own corpus,
carries `paths-ignore: ['site/**']` and the two site workflows, so a commit that
changes only a paragraph here no longer starts a seven-minute run building the
compiler from seed and running the net to answer a question no file under
`site/` can ask. `paths-ignore` rather than a `paths` allow-list on purpose: an
ignore list is safe by default, and a new directory of real code stays covered
without anybody remembering to add it.

`deploy-site.yml` runs `npm run build` on every push that touches `site/`, so a
site that does not build is a red workflow rather than a bad deploy. Now that the
site is public that is the only thing standing between a bad commit and a live
page, which is worth knowing before editing a template.

### The credentials

Two repository secrets, `CLOUDFLARE_API_TOKEN` and `CLOUDFLARE_ACCOUNT_ID`.
Their shape, the exact permissions and the traps are documented in `.env.example`
at the repository root, which is the one place they are written down; `.env`
beside it is gitignored and holds the real values for a `wrangler` run started
by hand.

For a deploy that stops at `pages.dev`, the token needs one policy: resource
**Entire Account**, `Cloudflare Pages: Edit` and `Account Settings: Read`. Give
it a short expiry.

### The domain

The Pages project is `heroes-lang-site`, and the site answers on
`heroes-lang.org` as well as on `heroes-lang-site.pages.dev`. The zone moved
from OVH to Cloudflare, which was not optional: **a `CNAME` cannot exist at the
apex of a zone**, Pages is reached by `CNAME`, and OVH's DNS has no `ALIAS` to
work around it. Cloudflare flattens the apex natively.

**One hostname on the project, exactly as the author's other site is set up**:
`giuseppearici.com` is the only custom domain on `giuseppearici-site`, and
`heroes-lang.org` is the only one here. `www` is not on the project. It is a
proxied placeholder record at `192.0.2.1` — RFC 5737 documentation space, never
contacted — plus a Redirect Rule that answers **301** to the apex, path kept.
One canonical hostname, and no second copy of the site to explain to a crawler.

Four things learned doing it, each of which cost time:

- **The order is DNS first, custom domain second.** Pages validates a custom
  domain over HTTP, so it has to reach the name; with an empty zone it cannot,
  and the domain sits at `pending` forever. Detaching and re-attaching does not
  help, because the missing piece is the record.
- **Attaching over the API does not create the DNS record.** The dashboard flow
  does; `POST /pages/projects/<p>/domains` only registers the domain with the
  project. The record is a separate act.
- **A leftover record blocks it.** The zone imported from OVH still had `A`
  records for the apex and `www` pointing at OVH's server, proxied, so
  Cloudflare dutifully forwarded to a machine that answered nothing: **521**,
  then **525** once the SSL mode tried HTTPS to it. Both are the same fact.
- **Every Redirect Rule carries `and not starts_with(http.request.uri.path,
  "/.well-known/")`**, or Universal SSL cannot complete its challenge and parks
  in `Pending Validation` for good.

Verified live once it was up, while the gate was still armed, against four
independent public resolvers: `/` answered 200 with the holding page, every
other path 404, both with `x-robots-tag: noindex`, `?preview=` opened the real
site in both editions, `www` answered 301 to the apex, and the certificate was
issued to `heroes-lang.org`. **Three of those six facts were about the gate and
are gone with it.** What still has to be true after a publish, and is worth
re-running rather than assuming: both landings and a nested chapter in each
edition answer 200, a missing page answers 404, `robots.txt` and
`sitemap-index.xml` answer 200, no response carries `noindex`, `www` still
answers 301 to the apex, and the certificate still names the apex.

## The Italian edition — `site/src/html/it/`

The site ships in two languages (author instruction 2026-08-18: *"translate the whole
site into Italian as well … leave the technical terms in English"*). It is
CLAUDE.md §11's **second declared exception**, named there beside the two books
since 2026-09-04 — until that day §11 said there was one, and this edition had
been 46 tracked files for 17 days. The same rule applies: **neither edition is a machine translation of
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
  binary and a derivative of a lyric, which § Style guide refuses. `it/project.html`
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
  record, and its twenty-one chapter titles on `it/about.html` are the book's
  own, copied from the manuscript rather than back-translated from the English
  page. The release wording rule in § The author and the book applies unchanged
  in Italian: *"pubblicato su Amazon"*, no day, and never *in libreria*.

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
- **The site says whose idea it was, and where it was the author's it says so**
  (author instruction 2026-09-06, *"in the end change the attributions on the
  site and the site's docs too"*). CLAUDE.md §14 gained that rule on 2026-09-05
  for the records, because a public project that reads as merely vibe-coded
  misrepresents who wrote it; this is the same rule on the pages a stranger
  actually reads. **It cuts both ways and that is what keeps it honest**: name
  the author where a question, a correction or a refusal of theirs produced the
  finding, name the panel seat where a seat found it, and name nobody where the
  work was ordinary. Crediting the author for something the assistant found is
  the same falsehood wearing the flattering sign, and it is the one nobody would
  check. Measured the day this was written: `thanks.html` had a strong
  attribution culture pointing OUTWARD, stating its own standard for the prior
  art it stands on, and nothing at all pointing in. The section that answers it
  is *Who wrote this* on the author page, in both editions, and it carries
  examples rather than a claim: a question of the author's that moved a safety
  check from three wrong placements to the right one, and a sitting the author
  ordered against the assistant's advice whose five judges then refused it.
- Nods spent so far: sound and vision · fashion · quicksand, avoided ·
  changes · station to station · rebel rebel · always crashing in the same
  car · a new career in a new town · oh! you pretty things · hunky dory ·
  under pressure · five years · look back in anger · absolute beginners ·
  moonage daydream · lady stardust · sons of the silent age · ashes to ashes ·
  speed of life · repetition.
- **"Ashes to Ashes" is spent** (2026-08-19), on the section of `selfhost.html`
  (since 2026-09-06 the `#self-hosted` section of `project.html`)
  that reports the bootstrap's retirement — which is the event the bank was
  holding it for. M-bootstrap-archive closed the same day, so the section that
  used to be headed *What is left* now says both owed things are paid.
- Ideas bank, still unspent: a 1.0 → "Golden Years".

## The visual system — what the art direction pass fixed (2026-08-18)

The register above says *what* the site sounds like; this says what the design
is allowed to do, so a later edit does not spend the same accent twice. The page
is a stage with **one light source, one motif, one quotation colour**, and
everything else is paper and ink.

- **One light.** The red/blue wash is drawn behind the home hero and nowhere
  else, and its tail reaches under the three stat cards below it: the rule is
  145% of the hero's height, so the ground under the cards measures about eight
  levels of 255 off the paper, inside the grain budget below. Measured by the
  design seat at 1200px light; recorded rather than corrected, because a light
  that stopped dead at the hero's edge would read as a box.
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
  *"use this image duotoned on the author page"*). The photograph on
  `about.html`, which absorbed the author page on 2026-09-06, is duotoned to two of the site's inks — a deep blue in the
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
  - **The prose runs alongside it** (author instruction 2026-08-19: *"make the text
    run alongside it and give it to me bigger"*), which needs a `float`, and
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
  language has); shell transcripts wrap, because a clipped command is a command
  nobody can type. A diagnostic wraps its MESSAGE line only, marked
  `span.msg`: the excerpt and caret rows underneath stay rigid, because an
  earlier design sitting rendered `pre-wrap` on the whole block at 390px and the
  source line wrapped while the caret stayed put, pointing at nothing.

**How to verify it, since the design seat judges from renders and not markup:**
Chrome headless on macOS will not open a window narrower than about 590px, so a
`--window-size=390` screenshot silently renders at 590 and clips, which reads as
a layout bug that is not there. Load the page in a **390px iframe** inside a
wider wrapper instead. Light mode does have a flag, verified by run:
`--blink-settings=preferredColorScheme=1` renders the light palette. The
override-stylesheet method this paragraph used to prescribe still works and is
no longer needed.

## The author and the book — the rules this copy is under

From the author's own brief, 2026-08-18. These are not style preferences, they
are constraints on what the site may say, and every one of them is a way the
copy could be wrong later:

1. **One book, not two.** *Heroes of code* (Italian original: *Gli eroi del
   codice*) is a history of programming languages whose *form* is a dream
   journey with Bowie as the guide. Never write copy implying a Bowie book and a
   separate history book. The Italian is the edition of record; the English is
   the author's own translation.
2. **Wording on the release: the book is published, and the date is off the
   page** (author decision 2026-08-29, superseding the dated rule this line used
   to carry). **Both editions are now on sale on Amazon, Italian and English**
   (author, 2026-09-02), which turns the copy this rule already required into a
   plain statement of fact rather than a bet on a launch day. The copy says
   *"published on Amazon"* and names no day. Two things made the old rule expire
   at once. The site sat behind a holding page with
   the same preview token as the author's own site (`starman`), so both were
   written in their finished state and opened within days of each other, which is
   what removed the risk the dated wording existed to cover; the author confirmed
   on 2026-09-03 that both editions are on sale. And § No dates on the page in
   `CLAUDE.md` had already taken every other date off the site; the release
   wording was its one standing exception, and it no longer needs to be one.
   What survives from the old rule is the reason under it: **an ISBN existing is
   not evidence a book is on sale** (KDP issues them when a listing is created,
   drafts included), so *"published"* rests on the author's decision to open the
   two sites together, not on the ISBN. Say **Amazon**, never "in bookshops":
   the A5 trim with a free KDP ISBN is outside Expanded Distribution
   (`printing/kdp/form-en.md`), so Amazon is the whole of it.
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

8. **No page count and no word count for the book, anywhere on the site**
   (author instruction 2026-09-02: *"take out the numbers for the book's words and
   pages, because they keep changing"*). The book keeps being revised, and a
   figure that changes with the next print is a figure the site cannot keep
   true, which is CLAUDE.md §11's expiring-premise rule applied to somebody
   else's manuscript. The `.stats` block on `author.html` carried **599 pages**
   in English and **593** in Italian, two numbers for one book, which is the
   tell on its own. They are gone; the A5 trim moved into the prose beside the
   imprint, because the format is fixed and § The author and the book rule 2
   leans on it to explain why Amazon is the whole distribution. What stays in
   that block is what does not move: 21 chapters, 2 editions, 1 night. The
   ISBNs stay too, for the same reason: an identifier is not a measurement.

**The book has one domain per edition** (author instruction 2026-08-18):
`glieroidelcodice.it` for the Italian, `heroesofcode.com` for the English, each
linked from its own edition's `author.html` on the title. **Both are live end to end, and the caveat this sentence used to carry is
spent** (re-measured 2026-09-03). It said the two domains 301 to
`giuseppearici.com` pages answering **404**, which was true while that site sat
behind its own holding page. That site is published now, and the books are on
sale on Amazon (author, 2026-09-03). One hop each, and a **200** at the end:
`glieroidelcodice.it` to `/it/libri/eroi-del-codice/` and `heroesofcode.com` to
`/en/books/heroes-of-code/`.

**The author's own site is linked, and the bet that link was is now settled**
(author decision 2026-08-29). It used to be omitted because
`giuseppearici.com/en/` answered **404**, and the rule was to omit rather than
send visitors to a parking page. The decision to link it anyway rested on both
sites being behind the same holding page with the same preview token
(`starman`), written finished and opening together, which makes a link between
them not a dead end but a door not yet open. **That site opened first, and both
locales answer 200**, measured 2026-09-03: `/it/` and `/en/`. So those links now
point at live pages, and the reasoning above survives as the precedent this site
leans on for its own GitHub links in § Launch order. The two places are the ones
the old rule named: the *Elsewhere / Altrove* line on `about.html` (then
`author.html`), and the byline in the footer, where the name itself is the link,
written once per edition in `src/html/_byline.html` and
`src/html/it/_byline.html` and rendered on all 180 pages by
`src/components/SiteFooter.astro` (§ The footer is written once, under the nav
above). Two rules on those links. Each
edition points at its own locale (`/en/`, `/it/`), because
`giuseppearici.com`'s bilingual splash sits at `/` and would make the reader
choose a language twice. And **no `?preview=` in the markup, ever**: the token
belongs to a browser session, and a preview URL committed to a page is a private
door written into a public file.

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

- **One name links to a project rather than to a person, on purpose** (author
  instruction: *"link nim's site on rumpf's name too"*). Andreas Rumpf
  is the trap named above, so there is no biography to point at, and the link on
  his name goes to `nim-lang.org`, the language's official site, verified live in
  the session that added it. It is the only link of this shape on the page.
  Do not remove it as a mistake, and do not repoint it at the Wikipedia title,
  which is the archaeologist.
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
  `crates/` has been `archive/bootstrap-rs/` since 2026-08-19. What changed on the pages is the
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
- **A number in prose is a claim, and the build checks the ones it knows.**
  `src/lib/claims.ts` reads a fact from the one place the tree keeps it and
  asserts the page spells it, in the edition's own number word: the seats of the
  panel and how many carry a veto from `.claude/agents/`, the words that can
  begin a top-level line from the parser, the verbs of the one command from the
  argv table, the lines of the Zen from the string `heroes this` prints, the
  chapters from the directory. Twenty claims across fourteen fragments today,
  run by `page()` on every hand-written page, plus three checks that are not
  counts: both start pages must name every verb the table declares, each as
  `<code>verb</code>`, because a count survives a rename; every chapter's footer
  sentence "The N diagnostics are" is compared with the `heroes check` and
  `heroes build` transcripts on that page; and the words that can begin a line
  are read twice from the parser, from its dispatch loop and from its
  `expected_declaration` message, and asserted equal, so a disagreement is the
  compiler disagreeing with itself. An `astro:build:done` hook then asserts
  every page the table names actually rendered, because a renamed fragment
  would otherwise take its rows with it in silence, and prints what the table
  holds so the build log shows the check ran. What provoked all of it is one
  sentence from the language veteran's seat after four sittings: every false
  claim it had found was in hand-written prose making a claim about a magnitude
  or a named thing, while everything a machine checked was right. **A claim not
  in the table is a claim only a reader checks**, so a sentence that names a
  count enters the table when it is written. Every check was broken on purpose
  to prove it fires, and each names the page, the claim, the tree's number and
  the shape it expected.
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
no chapter uses them now: a fourteenth chapter would be listed that way, with a
dim tag and **no link** — never a dead one. The same rule governed the thirteen
while they were being written.

**The rule above is now enforced, and this paragraph is the record of how long
it was not** (2026-09-07). It used to end *"Nothing enforces it"*, and to say
that it had once credited a `check.py` dead-link pass that does not exist in
this repository, which is the more expensive half of the failure, because a
sentence naming an instrument stops anybody from writing it. The instrument is
`site/src/lib/figures.ts`, wired into all 46 hand-written wrappers, one line
each. It re-cuts every figure's slice from the file at build time and refuses
the build when the two differ, naming the page, the figure's position, the file,
the range and the first line that disagrees. It checks the `figcaption` too,
which this file also asked for and nothing checked: a caption may not name a
different file or a different range from the attribute beside it. It was made to
fire rather than trusted, by planting one changed character.

## `site/src/html/examples/` and the examples browser

**Every program under `examples/` is a page on this site, generated from the
file at build time** (author instruction 2026-09-06: the examples belong on the
site, with a hash so a changed one can be noticed, and never a link out to
GitHub). One index, one page per program, one per gallery file, in both
editions, plus `/examples/index.json`.

**There is no copy of a program under `site/`.** The pages are cut from
`examples/` when the site is built, so nothing here can go stale, and the hash
is not a drift detector: it is there to be quotable. What that costs is the one
thing an Astro project does not normally do, reading files outside its own
directory, and it is done in one place.

| module | what it does |
|---|---|
| `src/lib/repo.ts` | finds the repository root by walking up from the working directory to the directory holding `examples/README.md`, `spec/heroes-spec.md` and `CLAUDE.md` together, and reads bytes from there. The only module that reads outside `src/` |
| `src/lib/tables.ts` | parses the keyword table out of `selfhost/keywords.hero` and the type words out of `spec/heroes-spec.md` § Types, with a floor and a shape assertion on each, so the site holds no copy of the language |
| `src/lib/highlight.ts` | the tokenizer, a reading of `selfhost/scan.hero`, `number.hero` and `literals.hero`, plus `assertRoundTrip`: strip the spans, decode the three entities, and the source must come back byte for byte |
| `src/lib/expectation.ts` | the JavaScript twin of `tests/harness/expectation.hero`. The harness's ending marker never reaches a reader: `!exit: 1` renders as an exit status |
| `src/lib/fingerprint.ts` | sha256 per file, then sha256 of the manifest in `shasum`'s own line format, and the one command that reproduces it |
| `src/lib/examples-model.ts` | discovery, the run command, the badges from the program's tokens, the floors |
| `src/lib/descriptions.ts` | the prose fragments, and the completeness check in both directions |
| `src/lib/examples-render.ts` | the pages, in either language, off one reading of the corpus |
| `src/lib/figures.ts` | the drift check on every hand-written figure, § above |
| `src/data/examples.ts` | the shelves and the reading order. No prose |
| `src/lib/i18n.ts` | the short labels a generated page needs in both languages. Nothing long, because this tree is not one where the net accepts Italian |

**Why the root is found by walking up rather than taken from Astro.** Astro
exposes it through `astro:config/server`, which works inside a build and not
inside a plain `node` script. The round-trip gate is such a script, and so is
every other check here, so tying the loader to Astro would put them out of
reach.

**What the build asserts, and each one throws rather than warning**: both tables
parse and clear their floor; all 118 `.hero` files round-trip; a directory under
`examples/` that is neither a program nor the gallery is an error and never a
skip; an expectation with a marker on the wrong line is an error; every example
has a fragment in both editions, each with a tagline, and every fragment names
an example; a `main.args` that exists and is empty is an error; and every
hand-written figure still matches its file.

**Each `.hero` file is highlighted once and both editions embed that one
string**, so an Italian page cannot show a different program from its English
twin. Measured: 181 code blocks compared between the editions, zero
differences. What does differ, by design, is the `figcaption`, where `lines`
becomes `righe`.

**The prose lives in `src/html/examples/`**, one fragment per example per
edition, each a `<p class="tagline">` and at most two short paragraphs. The
tagline is required, because it is the sentence under the title, the card's line
on the index and the page's own meta description. Three shared fragments say why
a page shows no output, and one is the footer.

**A program page shows** the badges that are true of it, the exact command a
reader would type, the output that program really prints, `main.hero` open with
every further module in a closed `<details>`, the input files, the fingerprint,
and previous and next. The gallery pages show no output pane, because no test
records what those files print, and they say so.

**`/examples/index.json`** carries the same corpus as data, including a hash per
file, for whatever reads this site next. It has no date field: the hashes are
the timestamp.

The deploy workflow triggers on `examples/**`, `selfhost/keywords.hero` and
`spec/heroes-spec.md` as well as on `site/**`, because a change to any of those
changes a page.

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

## Launch order — the site went first, and says so

**Closed on 2026-09-08: the repository is public** (M-open-repository), verified
unauthenticated at 200, and the price described below is paid. Every *not yet*
sentence is gone, the two documentation notices with them, and the nav's
`GitHub · private` is `GitHub` on all 180 pages, which is the repair
`SiteNav.astro`'s comment had predicted word for word. **What replaced the
warning is a status**, in the same slot on both landings and on both start
pages, so a reader meets it where the caveat used to be: the language is in
beta, the version is generated from the compiler, and `0.x`'s one-sentence
promise is spelled without jargon. The home footers now say the licence and the
contribution policy, which no page had ever said to a human. And the site gained
the check it lacked: `checkRepositoryIsOpen` in `src/lib/claims.ts` fails the
build on thirteen phrases that promise a repository nobody can reach, seven
English and six Italian, its falsifier run by hand before it was trusted.

**The one thing that went wrong is the ordering, and it went wrong by
accident.** A push during that milestone carried its commits to origin and fired
this deploy three times, so the site said the code was open while
`github.com/heroes-lang/heroes` still answered 404 to a stranger. The plan had
ordered the switch first and the deploy second, for exactly the reason the
finding below gives, and the gap it was meant to hold to about a minute lasted
**close to nine hours**: the first deploy carrying the new text ran at 00:01Z
and the repository answered 200 at 08:56Z. Nobody arrives at four in the
morning, which is luck and not a defence.

**The author reversed this section on 2026-09-03 and the site was published with
the repository still private**, because consolidation work had to finish before
the code could be public and the site did not have to wait for it. The finding
below is not deleted, because it was right and it is what the reversal had to
pay for: the site's whole mechanism is *check me*, and every check 404d.

**What the site owes in exchange, and this is the whole of the price.** The
pages that promise something reachable say plainly that it is not reachable yet:
a standing notice at the head of § Run it on both landings, one on both
documentation landings where the specification link and the per-chapter file
links sit, and a paragraph in `llms.txt` above the spec link it offers a model.
Every sentence claiming the record *is public* was rewritten to say it is kept
there and opens with the code, in all four places it appeared: the two home
footers and both editions of `panel.html`. Measured unauthenticated the day of
the decision, and the reason the notice is not optional:
`github.com/heroes-lang/heroes` **404**, `raw.githubusercontent.com/...
/spec/heroes-spec.md` **404**, the old `giuseppearici/heroes-lang` **404**.

**The links stay clickable** (author decision, same day). The addresses are
final and open with the code, so a live link is a door that is not open rather
than a wrong address, and the day the repository opens there is nothing to put
back. That is the same reasoning that put the author's own site in the footer
while it was still shut, one section above.

The finding that stood here, and its measurement, unchanged:

`github.com/heroes-lang/heroes` returned **404** to an unauthenticated request on
2026-08-30, because the repository is private. Every
verification path on the site runs through it: the `git clone` line, the GitHub
link in all eight navs, the twelve chapter file links, llms.txt's raw spec link,
and the footer sentence saying the record "is public in the repository". This
site's whole mechanism is *check me*; shipping it while the check 404s turns the
project's best asset into its most visible broken promise. **Repo public first,
site second.** Found by the panel's marketing seat, which tested the URL rather
than assuming it, when the address was still `giuseppearici/heroes-lang` and the
measurement was the same.

**The address moved on 2026-08-30**, to the organisation, and the site's 20 links
plus llms.txt's raw link were repointed in the same commit. GitHub redirects the
old address, but **the redirect is not something this site can lean on**: to an
unauthenticated request a private repository is a 404 either way, measured at both
addresses that day. So the rule above is unchanged, and one line is added to it:
the old name is **never recreated** (`CLAUDE.md` §14), because creating a
repository there is the single act that deletes those redirects permanently.

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
excluded from any summary because the mistakes it plants are caught by
construction: the ones it misses are mutations that changed nothing.

## One thing the publication gate owns

`site/`'s Aladdin Sane bolt is iconography attached to an actively managed
estate — the style guide already keeps lyrics out, and this is the other half.
Recorded in `docs/ROADMAP.md` under M-publication-gate; cheaper to answer
before publication than after.
