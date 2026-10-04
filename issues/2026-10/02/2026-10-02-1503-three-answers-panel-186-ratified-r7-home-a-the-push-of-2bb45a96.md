---
kind: decision
area: none
milestone: none
filed: 2026-10-02
commit: 4d0f27a1d630ff8b5ea95fb609ef734c6494e9cc
github: none
---

# Three answers: panel 186 ratified, R7's home (a), the push of 2bb45a96

2026-10-02 at 15:03, the author's answer to three recommendations, meant as:
*1a 2a 3a*.

## The decision

| | |
|---|---|
| date | 2026-10-02 |
| decision | 1a: panel 186 ratified, R1 to R10 as written, R3's one comparison rule moving panel 077's two union goldens; 2a: R7's home (a), `check` stops refusing a field omitted from a group record's construction and `build` judges it from clang's layout, prototyped and read blind before the spec takes O_cover; 3a: the push of `2bb45a96` once its Linux arm64 and Windows legs are green (40 commits, one touching `spec/`, so the site is published) |
| reason | the route built and read on five clangs; the second blind reading's approval of the text (1h) needs; the public CI red on Linux arm64 alone since the push of 11:01, repaired by lane ci-probe |
| design.md § | §1.11, §1.12, §4.19 |
| panel | 186 |

## What it leaves open

Nothing in `docs/work/DECIDE.md`. The landing is lane land186's; the
push waits on its two platform legs.
