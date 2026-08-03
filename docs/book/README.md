# The mini-book — "how this language came to be"

A declared goal of this project (author, 2026-08-03): at the end of the
journey, write a mini-book telling **how the language came to be** — the
adventure, the path, the wrong turns, the decisions and the prompts, the
work done together with an AI assistant. design.md Part 0 already frames the
language as a book's running example; this directory makes the raw material
for that book a first-class output of the process, not an afterthought.

## Sources (already being written — nothing extra to maintain)

| Source | What it gives the book |
|---|---|
| `docs/journal/` | the narrative spine: one lesson per step, predictions vs reality, what broke and why |
| `DESIGN-LOG.md` | every decision, dated, one line, with its reason |
| `docs/panel/` | the arguments — objections, verdicts, and the author's answers |
| `git log` + tags | the true chronology; `git checkout m2` re-opens any chapter's code |
| measurement records (born with the first harness run) | the numbers that make the thesis a claim, not an opinion |
| `docs/book/beats.md` | **story beats** (see below) — the human texture the other sources drop |

## The one new habit: story beats

Technical records forget how things *felt*, and the book needs exactly that.
So every **milestone close** and every panel decision appends one line to
`docs/book/beats.md` (cadence per CLAUDE.md rule 14 — and any day something
genuinely diverged deserves one too):

```
date | milestone | the beat (1–2 sentences, plain language)
```

A beat is a *story* fact, not a technical one: the surprise ("the entry label
warning — the machine found the bug in our plan before we did"), the doubt,
the U-turn (Swift→Rust in one conversation), the small victory (`20` printed
by hand-written C), the deleted darling (QBE). Include memorable prompts —
what the author actually asked, verbatim when it matters (Italian quotes are
welcome inside beats: they are quoted speech, not artifacts).

## Tentative shape (revisit at M5, don't design it now)

1. The itch: a language with no standard library
2. Designing for a reader that isn't human
3. The plan meets five hostile experts
4. Killing QBE (and why performance wasn't the reason)
5. Four programs written by hand before the compiler existed
6. Building the assembly line (M1–M4, the lessons)
7. The first `20` (M5) · 8. The boundary (FFI) · 9. The compiler compiles
itself · 10. What the numbers said

Rule from design.md §"The name" applies to the book too: **personality in the
packaging, precision in the substrate** — Bowie belongs here, not in the
error messages.
