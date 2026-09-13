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
**OPEN: 2**

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

- [ ] **M-cleanup-verdict** | a C handle advanced by a loop is a third obligation, and neither `owned` nor `lease` reaches it | `spec/heroes-spec.md` § 13 · `examples/ledger/main.hero:145-160` · `docs/panel/137-the-hole-was-two-operations-wide-and-the-answer-was-a-library-function.md` § Found beside the sitting

    **Origin:** panel 137's ffi-pragmatist, 2026-09-13, measured while pricing an
    iteration rule and recorded here because this milestone owns the question.

    **Measured, not argued, and on the platform where the sanitizer does not
    exist.** Break out of a loop advancing a `sqlite3_stmt *` and ask SQLite
    itself rather than a leak checker, since LeakSanitizer is absent on Darwin
    arm64: `rows seen: 2`, **`statements still open: 1`**, **`sqlite3_close says
    BUSY: true`**.

    **What the existing vocabulary can and cannot express**: `owned <freer>`
    covers a `cstr` result or a `char **` out-parameter — a string, freed once, at
    the call; `lease`/`end_lease` covers a **copy of `str` bytes**, and a lease
    nobody ends aborts when `main` returns. **Neither reaches a handle.** There is
    no `owned` for a `sqlite3_stmt *`, no end-of-scope hook, and `finalize` is an
    ordinary call the programmer must place.

    **The shipped tree already has the shape**, read rather than run:
    `examples/ledger/main.hero` holds four `?` operators inside the loop body at
    `:151,153,154,155` while `_ = sqlite.finalized(statement)` sits at `:159`,
    outside it — so an error path skips the finalise exactly as the measured
    `break` did. This is the sitting's own material: it is a release obligation on
    every path, which is what this milestone exists to rule on.

*******************************************************************************
