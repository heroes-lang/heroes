# M-compatibility-promise — the paragraph `1.0.0` rests on, and its instrument


**Scheduled 2026-09-10 by author decision**, from § What production-ready means
row 1. M-publication-gate owns the paragraph — *"Silence reads as a promise"* —
and `README.md:117` already says the version stays below `1.0.0` until it is
written; this row delivers the paragraph **and** the instrument, and discharges
that bullet.

**What warrants the instrument.** Go 1 published *Go 1 and the Future of Go
Programs*, Nim 1.0 promised that code compiling under 1.0 compiles under any 1.x,
and Rust 1.0 put *stability without stagnation* on a train where an unstable
feature is an error on stable. **In all three the sentence has a mechanism**, and
in a repository whose contract is that a claim is written down only after the
command that settles it has run, a paragraph alone is the one unrun claim left.

**What it delivers**, and both halves exist already: the corpus at a release tag
recompiled by today's compiler, so a program that stops compiling is a **broken
promise** rather than a discovery; and the trigger, since `git diff vA vB --
spec/heroes-spec.md` already moves `Y` and its emptiness moves `Z`
(`.claude/rules/records.md` § Release tags). No new verb is owed.

**The hardest sentence is a consequence of §10.** A program cannot declare the
language version it was written for — no fourth input class, and panel 056
refused a per-project file — so the promise is **one-directional**: the compiler
must not break old programs, and an old program cannot ask for an old compiler.
**And the paragraph says the deployment model out loud**: crash and be restarted,
because Part 6 refuses exceptions permanently and every abort `spec:166-169`
lists is uncatchable. Defensible as a model, indefensible as a surprise.

**Why here.** Immediately before the gate, which then publishes the `1.0.0` this
row earns.

**What it does not deliver**: the `1.0.0` tag itself, which is the author's act
on a clean `main` and never a milestone's.

*******************************************************************************
**OPEN: 1**

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

*******************************************************************************
