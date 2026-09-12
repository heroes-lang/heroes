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
