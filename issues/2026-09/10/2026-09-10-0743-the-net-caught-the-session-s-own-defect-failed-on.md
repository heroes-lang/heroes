---
kind: defect
area: none
milestone: none
filed: 2026-09-10
commit: 44177d3c4394c8f33a5f2ebfc8bae810693e35fa
github: none
---

- [x] **The net caught the session's own defect.** `records/citations` failed on
    a comment in `site/src/lib/lastmod.ts` that named a fragment by the path it
    has under `site/src/html/` rather than the one it has from the root, so the
    citation resolved nowhere. Repointed to `site/src/html/docs/index.html`, and
    it is the reason the full net runs after the last edit rather than after the
    last interesting one.
