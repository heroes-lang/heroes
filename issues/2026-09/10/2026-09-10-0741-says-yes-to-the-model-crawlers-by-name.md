---
kind: task
area: none
milestone: none
filed: 2026-09-10
commit: 44177d3c4394c8f33a5f2ebfc8bae810693e35fa
github: https://github.com/heroes-lang/heroes/issues/1093
---

- [x] **`robots.txt` says yes to the model crawlers by name.** `Allow: /`
    already permitted every one of them, so nothing changes behaviour; what
    changes is that a language whose whole point is being read by a model stops
    leaving its own position to be guessed. The tokens are each vendor's
    documented ones, read from their own pages in this session.

## What was done

The robots file says yes to the model crawlers by name, each by its vendor's documented token read from their own pages; the blanket allow already permitted every one of them, so nothing changed in behaviour, and the site's own position stopped being left to be guessed. Landed at `44177d3c`, "The sitemap could not say what had changed, and the audit written to catch that passed on the one run it existed for".
