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

## Keeping it current (part of closing a milestone)

- Update the status badge in the header.
- Append ONE entry to "The build log" (newest first): date + milestone, a bold
  one-liner, 3–5 sentences in the postcard register — distilled from that
  milestone's story beats (`docs/book/beats.md`), not from the commit log.
  Plain language, no jargon the page hasn't introduced.
- If a panel decision changed the language surface, check whether a "Why these
  choices" entry needs adding or amending — the reasons on the page must match
  design.md's current state.
