---
kind: decision
area: none
milestone: none
filed: 2026-09-10
commit: 44177d3c4394c8f33a5f2ebfc8bae810693e35fa
github: none
---

- [x] **`/sitemap.xml` answered 404**, which reads as a site with no sitemap at
    all, and now redirects to the generated index. The two share cards
    `errors-en.png` and `errors-it.png` had been referenced by no page since the
    errors page became chapter two of the guide, and are removed with the dead
    `errors` entry in `SECTION_CARDS`.
