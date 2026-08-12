# site/ — the showcase site for heroes-lang.org

One self-contained `index.html`: no build step, no external assets, no
JavaScript, light/dark via `prefers-color-scheme`. The domain
(heroes-lang.org) is already owned by the author.

Not deployed yet. Cheapest route when wanted: GitHub Pages publishing this
directory (`CNAME` is already here) + two DNS records at the registrar.
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
- Every claim on the page must match design.md's current state.
- Ideas bank: M-selfhost-fixpoint bootstrap retirement → "Ashes to Ashes"; a 1.0 →
  "Golden Years".

## Keeping it current — on demand, not per milestone

When the author asks for a site refresh (typically before publishing, or
after a stretch of milestones): update the status badge; append one build-log
entry per milestone closed since the last refresh (newest first, postcard
register, distilled from `docs/book/beats.md`, not from the commit log);
re-check "Why these choices" against design.md's current state.
