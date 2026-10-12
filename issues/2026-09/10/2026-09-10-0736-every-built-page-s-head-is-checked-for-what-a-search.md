---
kind: task
area: none
milestone: none
filed: 2026-09-10
commit: 44177d3c4394c8f33a5f2ebfc8bae810693e35fa
github: https://github.com/heroes-lang/heroes/issues/1088
---

- [x] **Every built page's head is checked for what a search result uses**,
    `site/src/lib/seo.ts`: title, description and canonical present; description
    at most 160 characters and title at most 60, which is where a result is cut
    rather than where a sentence is long; canonical equal to the page's own
    address; no two pages sharing a title or a description. 186 pages pass. Four
    sabotages were run one at a time and each was seen red with its own message.

## What was done

Every built page's head is checked for title, description and canonical present, a description of at most 160 characters and a title of at most 60, a canonical equal to the page's own address and no two pages sharing a title or a description; 186 pages passed, and four sabotages run one at a time were each seen red with their own message. Landed at `44177d3c`, "The sitemap could not say what had changed, and the audit written to catch that passed on the one run it existed for".
