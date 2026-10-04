---
kind: task
area: process
milestone: M-compatibility-promise
filed: 2026-09-10
commit: none
github: none
---

- [ ] **M-compatibility-promise** | the paragraph, the suite that makes a broken promise red, and the sentence §10 forces | `README.md:117` · `.claude/rules/records.md` § Release tags · `docs/ROADMAP.md` § M-publication-gate

    **Origin:** author decision 2026-09-10, § What production-ready means row 1.
    M-publication-gate owns the paragraph as one bullet of its checklist; this row
    delivers the paragraph **and** the instrument, and discharges that bullet.

    **The suite, and both halves already exist.** The corpus at a release tag,
    recompiled by today's compiler: a program in `examples/` at `v0.2.0` that stops
    compiling on `main` is a broken promise rather than a discovery. And the
    trigger is decided — `git diff vA vB -- spec/heroes-spec.md` moves `Y` and its
    emptiness moves `Z` — so no new verb and no new vocabulary is owed.

    **The hardest sentence, and it is a consequence rather than a choice**: a
    program cannot declare the language version it was written for, because §10
    admits no fourth input class and panel 056 refused a per-project file. So the
    promise is **one-directional**: the compiler must not break old programs, and
    an old program cannot ask for an old compiler. That belongs in the paragraph,
    not in a footnote.

    **And the paragraph says the deployment model out loud**: crash and be
    restarted. Part 6 refuses exceptions permanently, and `.must()` on an error, an
    out-of-range index, an overflow at any width and a division by zero all abort
    with nothing catching them (`spec:166-169`). That is defensible as a model and
    indefensible as a surprise.
