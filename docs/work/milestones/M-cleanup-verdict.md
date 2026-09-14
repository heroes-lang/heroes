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

**Both items closed 2026-09-14 at this milestone's own close**, and their record
is
`docs/records/done/2026-09-14-1200-m-cleanup-verdict-both-items-closed-and-the-count-falsified-the-question.md`.
The count this file demanded before any argument is
`docs/measurements/030-three-release-obligations-and-only-one-of-them-is-silent.md`,
and it falsified two thirds of the premise above: `owned` puts no obligation on
any path and a missed `lease` is loud, so the C handle is the only silent one.
The ruling is `docs/panel/147-the-obligation-is-created-by-a-call-and-not-by-a-type.md`,
and the form it admitted is `docs/work/milestones/M-marked-acquisition.md`.

*******************************************************************************
**OPEN: 0**

*******************************************************************************
