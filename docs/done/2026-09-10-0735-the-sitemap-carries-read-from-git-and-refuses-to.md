- [x] **The sitemap carries `<lastmod>`, read from git, and refuses to guess.**
    184 addresses dated over 15 distinct dates, `site/src/lib/lastmod.ts`, where
    it was 0 of 184. A page's date is the last commit touching its OWN sources,
    its prose fragment and its wrapper, never the shared layout, because a shell
    edit would otherwise move all 184 together and be the build-time stamp under
    another name. `fetch-depth: 0` on the deploy checkout, without which a
    shallow clone answers one date for every path. **Two guards, both seen red
    before they were trusted**: the history check at `astro:build:start`, where a
    `--depth 1` clone exits 1 and writes no sitemap, and the audit over the built
    sitemap, where identical dates exit 1 naming the count. The audit reads the
    file that ships and not a list the config kept while building, which is what
    the first version did and why it passed on the one run that mattered.
