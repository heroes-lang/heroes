# site/ — the showcase site for heroes-lang.org

One self-contained `index.html`: no build step, no external assets, no
JavaScript, light/dark via `prefers-color-scheme`. The domain
(heroes-lang.org) is already owned by the author.

Not deployed yet. When the author wants it live, the cheapest route is GitHub
Pages via an Actions workflow that publishes this directory (the `CNAME` file
is already here), plus the two DNS records at the registrar (apex A/ALIAS →
GitHub Pages, `www` CNAME). Netlify/Cloudflare Pages work identically. Ask
before wiring any of it — publishing is an outward-facing act.

Content rule (design.md §"The name"): personality in the packaging, precision
in the substrate — Bowie lives here and in the book, never in error messages
or library names.

## Style guide: Bowie, tastefully (the site's register)

The site's visual and verbal register is **70s-glam Bowie**, applied with
discipline:

- **The bolt.** The Aladdin Sane lightning bolt (1973) is the site's one
  visual motif: red over blue, slightly rotated, as the header mark and as
  bullet/marker accents (inline SVG only — the site stays self-contained).
- **Palette.** Near-black stage / cream daylight, with the bolt's electric
  red (`--red`) and blue (`--blue`) as the only loud colours, gold for the
  quotation marks. Both themes via `prefers-color-scheme`.
- **Type.** Poster-condensed uppercase for headings (system stack: Avenir
  Next Condensed → Arial Narrow — never a webfont); Georgia serif for body.
  Glam marquee over readable book page.
- **Song-title nods, never lyrics.** Section headings may carry a small
  `.nod` subtitle that is a Bowie song/album TITLE ("Sound and Vision",
  "Fashion", "Changes", "Station to Station", "Where are we now?"). Titles
  are fair game; **quoting lyrics is not** (copyright — and design.md's
  discipline). If a nod needs explaining, cut it.
- **Puns stay on this page.** The footer says it: you will never meet Bowie
  in an error message. Content precision is unchanged by the styling — every
  claim on the page must match design.md's current state.
- Ideas bank for future entries: the M8 bootstrap-retirement log entry wants
  "Ashes to Ashes"; a future 1.0 wants "Golden Years".

## Keeping it current (part of closing a milestone)

- Update the status badge in the header.
- Append ONE entry to "The build log" (newest first): date + milestone, a bold
  one-liner, 3–5 sentences in the postcard register — distilled from that
  milestone's story beats (`docs/book/beats.md`), not from the commit log.
  Plain language, no jargon the page hasn't introduced.
- If a panel decision changed the language surface, check whether a "Why these
  choices" entry needs adding or amending — the reasons on the page must match
  design.md's current state.
