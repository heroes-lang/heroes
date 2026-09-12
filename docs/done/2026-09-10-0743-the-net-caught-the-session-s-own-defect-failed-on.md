- [x] **The net caught the session's own defect.** `records/citations` failed on
    a comment in `site/src/lib/lastmod.ts` that named a fragment by the path it
    has under `site/src/html/` rather than the one it has from the root, so the
    citation resolved nowhere. Repointed to `site/src/html/docs/index.html`, and
    it is the reason the full net runs after the last edit rather than after the
    last interesting one.
