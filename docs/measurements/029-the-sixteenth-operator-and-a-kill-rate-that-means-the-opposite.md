# 029 — The sixteenth operator, and a kill rate that means the opposite of what it reads

Date: 2026-09-13 · M-handle-verdict step 3 · **the number panel 145 ordered built
before the form it judges.**

## Why this file exists

Panel 145 ruled that defect 029's repair ENTERS — a fieldless `extern` record is
a handle — and then ordered the instrument **first**. Four seats had disagreed
on how many such mutants `examples/` even holds (5, 6, 7 and ~14), and the
spec-warden had written that the whole §1.2 case for the form rests on an
after-number **nobody had run**. So the operator lands, the before-number is
measured, and the form is judged against a figure rather than an argument.

`swap-ptr` is the sixteenth operator: inside one function, replace a bare
`ptr`-typed argument name with another `ptr`-typed name in scope there. It
imitates one C handle handed where another belongs — `sqlite3_step(db)` for
`sqlite3_step(statement)`, which builds at zero diagnostics and exits 139 on
Darwin, `rows: -1` at exit 0 on Linux.

**Adding an operator moves the denominator of the thesis's own score**, so the
discontinuity is spent once and recorded here, as `docs/measurements/014` and
`016` were for the thirteenth and fourteenth.

## Provenance

| what | value |
|---|---|
| compiler | the tree at this commit, rebuilt through the fixpoint (seed 798,362 lines, byte-identical on re-emission) |
| corpus | `examples/` — **120** programs |
| command | `heroes mutate examples --operator swap-ptr --survivors` |
| platform | Darwin arm64, Apple clang 21 |

## The headline, and why it is a lie

```
| operator | mutants | excluded | killed (check) | killed (--permissive) |
| swap-ptr |      15 |        0 |        8 (53%) |               6 (40%) |
```

**Eight of fifteen die, and not one of them dies because the compiler can tell
one handle from another.** It cannot: every C pointer that is not a `cstr` is
one Heroes type (spec § 3), which is the whole of defect 029. The eight die on
two rules that fire on the **shape of the edit** rather than on the identity of
the value, and both were run to confirm it:

| how it dies | example | the rule |
|---|---|---|
| `error[unused_binding]` | `db: db` → `db: statement` leaves the parameter `db` never read | §4.5, a bound value must be used |
| `error[aliased_mutable_arguments]` | `statement: @statement` → `statement: @tail` beside `tail: @tail` puts two `@` arguments on one place | panel 010's rule, `resolve/writes.hero` |

Both are honest diagnostics catching real mistakes. Neither is about handles.
**Where the swap is a pure USE — an argument read and nothing orphaned, which is
exactly defect 029's shape — the program compiles every time.**

## The number that matters: 0 of 7

| site | mutant | `heroes check` |
|---|---|---|
| `examples/sqlite/main.hero:74` | `sqlite3_step(db)` | **exit 0** — this is defect 029, verbatim |
| `examples/sqlite/main.hero:74` | `sqlite3_step(tail)` | exit 0 |
| `:75` | `sqlite3_column_int(db, 0)` | exit 0 |
| `:75` | `sqlite3_column_int(tail, 0)` | exit 0 |
| `:76` | `sqlite3_finalize(db)` | exit 0 |
| `:76` | `sqlite3_finalize(tail)` | exit 0 |
| `examples/ledger/db/sqlite.hero:268` | `Statement(handle: tail)` | exit 0 |

**Seven programs this compiler accepts and should not**, one of them the filed
defect itself. That is the before-number, and it is the one the handle form is
judged against — not the 53%.

## Where the fifteen come from, and where they do not

| binding | mutants | why |
|---|---|---|
| `examples/sqlite/main.hero` | **12** | `first_int` holds **three** handles at once — `db`, `statement` and `tail` — and six of its arguments are bare handle names, so six sites × two alternatives |
| `examples/ledger/db/sqlite.hero` | **3** | `prepared` holds `statement` and `tail`; its `db` is a `Db` **record**, invisible to this operator |
| `examples/curl/main.hero` | **0** | one handle per function, so no second operand exists |
| everything else in the 120 | **0** | — |

**Four seats guessed 5, 6, 7 and ~14. The answer is 15, and nobody had it.** The
FFI seat counted *sites where a mutant can be written* (5) where the engineer
counted *mutants* (~14) and neither named its unit, which panel 144 had already
recorded as CL-017's shape.

**And the ledger's own discipline hides its exposure from the instrument.**
`examples/ledger/db/sqlite.hero` wraps its handles in `record Db` and
`record Statement`, so 17 of its 18 `ptr`-passing sites are reached through a
field path and this operator is blind to them — by design, and the row in
`harness/mutations/operators.md` says so. The hand-written newtype idiom is
therefore invisible to the measurement **and** to the swap; the form panel 145
adopted replaces it with something the header checks.

## What this predicts, and how to falsify it

When the handle form lands (step 4), rewriting `examples/sqlite/main.hero` to
declare `record Db tag sqlite3` and `record Stmt tag sqlite3_stmt` should move
**all seven survivors to killed**, because the argument's type stops being `ptr`
and nominal inequality does the rest — the engineer compiled that refusal at
panel 145 (`type_mismatch: expected Stmt, found Db`). The headline will read
15 of 15; **the honest half is 7 of 7**, and this file exists so the day it is
read nobody mistakes the eight accidents for a defence.

**It is falsified** if any of the seven still compiles after the rewrite, or if
the rewrite kills fewer than seven, or if the total is no longer 15 — which
would mean the operator's site set moved and the two numbers are not comparable.

## What was not measured

- **`--permissive` reads 6 rather than 8**, so two of the eight die on a rule
  `diag.is_thesis_rule` counts as a thesis rule. Which two was not run.
- **Windows and Linux**: the mutants are judged by `heroes check`, which is
  platform-independent, but that was not confirmed on either.
- The operator is blind to a handle behind a field path, an untyped `=`
  binding and a UFCS receiver. How many sites those hide across the corpus was
  not counted; in `examples/ledger` alone it is 17.
