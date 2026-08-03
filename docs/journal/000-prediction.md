# 000 — Prediction (author)

Low-typing protocol (CLAUDE.md rule 3): the questions are committed, the
author's raw answers are not — they live in `docs/journal/private/`
(git-ignored, local to the author). The SHA-256 seal below proves the
recorded prediction predates the implementation without publishing it.

## The program

```
main = function: ()
    i: int @ 0
    total: int @ 0
    for i < 5
        total @ total + i
        i @ i + 1
    print(total)
```

## The questions (answered 2026-08-03, before the spike was opened)

1. How many basic blocks? — 3 / 4 / 5
2. Where does the `i < 5` test live? — in the entry block, with the
   initialisations / in a block of its own / at the end of the body block
3. How many arrows enter the block holding the test? — 1 / 2 / 3
4. What does `print(total)` print? — 5 / 10 / 15

## Seal

```
sha256(docs/journal/private/000-prediction-raw.md) =
bd0dacdaf61c47c732b56a08373ef035a8cebacb4a29b63d98bc6e680fbb0470
```

## Where the lesson lives

Divergences, recorded impersonally: `000-setup.md` §3. Distilled concept:
`docs/glossary/000-basic-block.md`.
