---
kind: task
area: site
milestone: none
filed: 2026-09-10
commit: d821d7e4f9d00d054d8416d6adb25ee6db42ea66
github: none
---

- [x] **`site/public/CNAME` is deleted, and the hard stop it justified now names
    what actually publishes.** The file was a GitHub Pages artifact on a site
    that ships to Cloudflare Pages by Direct Upload, so it read nothing and did
    nothing, and Astro copied it into the output, where it was served publicly
    at `/CNAME`. Re-measured before touching it: no workflow under `.github/`
    names `deploy-pages`, `upload-pages-artifact`, `configure-pages` or a
    `gh-pages` branch. **The work was never the deletion.** Four living
    documents cited the file as the reason a push is asked for, so they were
    repointed first, at `.github/workflows/deploy-site.yml` and the five paths
    whose push deploys the site: § Hard stops, the step skill's close checklist,
    `docs/ROADMAP.md`, and `site/README.md`'s inventory of `public/`. The new
    sentence is wider than the old one, because a push touching only `examples/`
    publishes the site too and the CNAME sentence never said so. CL-042 was
    corrected **underneath** with today's date rather than rewritten, since its
    conclusion held and only its reason was false. `heroes measure CLAUDE.md`
    read 12 tokens of headroom before and 9 after, and the first draft spent 7
    of the 12 and was rewritten shorter; the named removal for the three spent
    is the sentence replaced. The two surviving citations are in `DESIGN-LOG.md`
    and `docs/contract/case-law.md`, both append-only records the citation check
    skips by construction and both dated history.

## What was done

The site's CNAME file, a GitHub Pages artifact on a site that ships to
Cloudflare Pages by Direct Upload, was deleted, and the four living documents
that cited it as the reason a push is asked for were repointed first, at the
site's deploy workflow and the five paths whose push deploys the site; CL-042
was corrected underneath with the day's date rather than rewritten. Landed at
`d821d7e4`, "A hard stop was resting on a file that never did anything, and
the rule now names what actually publishes".
