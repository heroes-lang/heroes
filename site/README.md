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
or library names. Update the status badge as milestones tag.
