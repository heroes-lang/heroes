# M-cleanup-verdict — the ruling on a release bound to a scope


**Scheduled 2026-09-10 by author decision**, from § What production-ready means.
It delivers **a decision, not a feature**, in M-closures-verdict's shape: a form
enters with its own milestone, or it lands as a **Part 6 row naming the program
that would make it wrong**. Full five seats, because a form that enters has
surface.

**What warrants it is a silence, and the vocabulary searched is named**
(2026-09-10): `defer`, `RAII`, `scope guard`, `scope-bound`, `destructor`,
`finally` and `cleanup on`, over design.md, `spec/` and all 125 sittings, return
**nothing about a construct**. The two hits are `design.md:1525`'s *"C++ RAII
with references"*, a row about the ownership model, and the emitter's internal
`h_T_drop`. So the refusal is uncitable today (CLAUDE.md §1), which is the
finding that warranted M-reflection-verdict.

**And the reason is four days old.** `owned <C function>` landed 2026-09-07 and
`lease`/`end_lease` on 2026-09-09, both putting a release obligation on **every
path**, while `?` makes an early return implicit and `spec:246` aborts when
`main` returns on a lease nobody ended. There are no destructors by design
(`selfhost/emit/types.hero:6`), which is where the sitting starts rather than
something it discovers.

**Why here.** With the other rulings and before M-core-packages, on the
2026-09-03 reorder's argument: the packages are the largest body of Heroes that
will be written against the spec after the compiler, and a package over C is
exactly the code that acquires and must release.

**What it does not deliver**: M-deferral-ledger's Part 7 items, and the reason is
the point — a scope-bound release is on no list at all, so a row that inherited
it would report a promise where there is none. The count it opens with is its
`docs/work/SCHEDULED.md (retired 2026-09-12)` item's.

*******************************************************************************
**OPEN: 1**

- [ ] **M-cleanup-verdict** | the sitting, and the count it is handed rather than guesses | `design.md` Part 6, Part 7 · `spec/heroes-spec.md:241-246` · `selfhost/emit/types.hero:6`

    **Origin:** author decision 2026-09-10, out of the session that wrote
    `docs/roadmap/production-ready.md`. The row's own section carries
    why it exists; this item is what the sitting needs in hand.

    **The silence is measured and the vocabulary is named**, so the sitting starts
    from a fact rather than an impression: `defer`, `RAII`, `scope guard`,
    `scope-bound`, `destructor`, `finally` and `cleanup on`, over `design.md`,
    `spec/` and all 125 sittings, return **nothing about a construct**. The two
    hits are `design.md:1525`'s *"C++ RAII with references"*, which is a row about
    the ownership model, and the emitter's internal `h_T_drop`.

    **What is owed at the opening, and it does not exist yet**: how many
    acquire-and-release pairs stand in `selfhost/` and under `examples/`, and how
    many early returns and `?` operators sit between an acquire and its release.
    That count is the argument in both directions, and neither direction may be
    argued without it. The two obligations to count are `owned <C function>`
    (2026-09-07) and `x: cstr @ s.lease()` with `end_lease(@x)` (2026-09-09), and
    `spec:246` is what makes a miss loud: a lease nobody ends aborts when `main`
    returns, saying how many.

    **What the sitting may not do**: take M-deferral-ledger's Part 7 items, which
    are that milestone's, or decide a spelling before it has decided whether a form
    enters at all. Full five seats, because a form that enters has surface.

*******************************************************************************
