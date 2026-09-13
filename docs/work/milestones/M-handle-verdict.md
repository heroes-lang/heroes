# M-handle-verdict — the ruling on telling one C handle from another

**Scheduled 2026-09-13 by author decision**, at panel 135's ratification and on the
assistant's recommendation, out of a defect found beside that sitting. It delivers
**a decision, not a feature**, in M-closures-verdict's shape: a form enters with its
own milestone, or the language's answer is written where a reader can check it — a
Part 8 wart with its remedy named, or a Part 6 row naming the program that would make
it wrong (CLAUDE.md § 12). Full five seats, because a form that enters has surface.

**Since 2026-09-13 it warrants THREE defects, and they are one question.** The
milestone was scheduled for defect 029 alone; panels 137 and 139 filed two more
the same night, and the critic of panel 139 separated them cleanly. They are the
three things a `ptr` does not carry:

| defect | what `ptr` lacks | what it costs |
|---|---|---|
| **029** | a **pointee type** — `sqlite3 *` and `sqlite3_stmt *` are one Heroes type | a swapped handle builds at zero diagnostics: **exit 139 on Darwin, and on Linux a wrong answer at exit 0**, `rows: -1` |
| **030** | **identity** — a copy copies the address | two copies of one record advance the same C cursor; § 3 says no aliasing exists anywhere, which is false in **six compiled shapes**, one of them a bare `ptr` parameter with no record at all |
| **031** | **ownership** — nothing models a foreign lifetime | one copy can `free` what every other copy holds: build exit 0, run exit 0, `heap-use-after-free` under ASan, and the shipped SQLite binding has that shape twice |

**This is the sitting's real subject and the reason it is one sitting rather than
three.** A route that gives `ptr` a pointee type closes 029 and leaves 030 and
031 untouched; a sentence closes neither. The sitting must say which of the three
it answers and which it does not, and must not be allowed to look like it
answered all three because it answered the loudest.

**And one thing it must not re-derive**: panel 139 measured that a rule keyed on
*reaches C through a `ptr`* cannot be written from the headers — `sqlite3_column_count`
is **not** const-qualified, compiled, so a mutating step and a pure read have the
identical C signature, and such a rule would refuse **13 of 17** functions in the
shipped binding, four of which mutate nothing. Whatever this sitting adopts, the
ownership signal comes from the binding author or from nowhere.

**What warrants it is a measured crash, not a silence.** `examples/sqlite/main.hero`
with one line changed, `sqlite3_step(db)` for `sqlite3_step(statement)`, **builds at
exit 0 with zero diagnostics and runs to exit 139** (panel 135's ffi-pragmatist,
rebuilt from source and re-run by the coordinator before filing; the unchanged example
prints `rows: 3` at exit 0 with the same binary). That is `docs/work/DEFECTS.md`
item **029**, and design.md §1.12 — a Heroes program must not segfault — is a goal of
the language that `.claude/rules/c-boundary.md` says wins here first.

**The cause is one type doing one job too many.** Every C pointer that is not a `cstr`
is `ptr` (spec § 3), so `sqlite3 *` and `sqlite3_stmt *` are one Heroes type; the
emitted probe is `(void)(sqlite3_step)(a0)` with `a0` a `void *`, and C converts
without a word. The same-typed-argument rule (spec § 9) catches this shape **inside**
one call and cannot reach **across** two: a handle produced by one function and handed
to another is one argument in one position, with nothing to compare it to.

**Why here, immediately after the ledger.** Part 7 item 5's own text keeps the door
open — *"leaves the door open for v2 distinct types (`UserId` and `PostId` both ints
but not interchangeable)"* — and panel 135 ruled `alias` deferred partly because a
**transparent** alias catches none of this. The ledger dates Part 7's items; a new
question found beside one of them is a new question, and widening the ledger to hold
it would be the same mistake the ledger exists to end, in the other direction.

**What it does not deliver**: `alias` itself, which has a dated deferral and a return
condition (panel 135), and Part 7 item 10's C-width vocabulary (`c_int`, `const`),
which is M-core-packages' opening sitting, questions (v) and (vi). A sitting here that
decided item 10 by accident would take a milestone's work without its row.

**A consequence for the milestone before it**: `records/tagged` allows no tag over an
open defect, so **M-deferral-ledger closes untagged** while 029 stands, on
M-anchored-spec's precedent (chain rows 45 to 47), and its tag lands on the first
commit where every list it leaves open is clean — the way `m-cstr-lifetime`'s did on
2026-09-09. It closed that way at `481ec6ae`, 2026-09-13.

**All three defects were re-run at that closing commit rather than inherited**, so
this milestone opens on a measurement and not a memory (2026-09-13, Darwin arm64,
reproducers in the session scratchpad):

- **029** — `examples/sqlite/main.hero` with line 74 reading `sqlite3_step(db)`:
  **build exit 0, zero diagnostics, run exit 139**. Unchanged, and still the one
  of the three that **changes class across platforms** (a wrong answer at exit 0
  on Linux), so a reader must not assume the family behaves uniformly.
- **030** — a `record Cursor { handle: ptr }`, one `sqlite3_stmt *`, one copy:
  **`a sees 1`, `b sees 2`, `same handle: true`, exit 0**, against spec § 3's
  *"No aliasing exists anywhere"*. Two independent copies advancing one cursor.
- **031** — a `record Holder { cell: ptr }`, the copy closed and the original
  written through: **build exit 0, run exit 0, both prints reached**, and under
  `--sanitize` `heap-use-after-free, WRITE of size 8` naming `uaf.hero:22`.

**One thing the re-run added that the entries did not have.** Writing 030's
reproducer from scratch, the width of `sqlite3_prepare_v2`'s third argument was
got wrong — `length: 0 - 1` for an `i32` — and the compiler refused it with
`error[type_mismatch]: expected \`i32\`, found \`i64\``, pointing at the argument.
**The same program hands the same function a database handle where a statement
handle belongs and is accepted in silence.** The width of a number is checked at
the boundary and the identity of a pointer is not, in one call, in one line. That
contrast is this milestone's argument in miniature and it should open its sitting.

*******************************************************************************
**OPEN: 3**

- [ ] **M-handle-verdict** | the sitting, and the counts it is handed rather than guesses | `docs/work/DEFECTS.md` 029 · `docs/panel/135-the-form-was-cheap-and-the-reasons-under-it-were-borrowed.md` § Found beside the sitting · design.md Part 7 item 5, Part 8 wart 17 · `spec/heroes-spec.md` § 3, § 9, § 13

    **Origin:** author decision 2026-09-13, at panel 135's ratification.

    **What is already measured and must be handed over rather than re-derived**,
    from panel 135's FFI seat: `examples/` binds externs in **18** programs;
    extern functions with two or more `ptr` parameters number **2**, both in
    `examples/sqlite/main.hero:46-47`, and both already write labels at every
    call, because spec § 9 forces them; `ptr` **producers** number 4
    (`curl_easy_init`, `sqlite3_open`'s `@out`, `sqlite3_prepare_v2`'s
    `@statement`/`@tail`, `fopen`); in `examples/sqlite` two C pointee types
    collapse to one `ptr` and **6 call sites hand one on**
    (`main.hero:56,74,75,76,82,89`), in `examples/curl` one type at 2 sites.
    `heroes mutate`'s `swap-args` (`selfhost/mutate/ops.hero:30`) swaps a label
    with its value and **cannot produce this mutant**, so the corpus's own
    instrument is blind to the class.

    **What is owed at the opening and does not exist yet**: the same census over
    `selfhost/` and `tests/golden/run/`, and the count that decides the sitting in
    both directions — how many call sites in the whole tree pass a `ptr` that came
    from a different library function than the one the parameter expects. Neither
    direction may be argued without it.

    **What the boundary needs from a distinct type, stated by the seat that
    compiled it and NOT designed here**: the same C spelling as `ptr` (`void *`,
    8 bytes, no box), `nullptr` as its literal, an `@` out-parameter still emitted
    `(void *)&x`, the `_Static_assert` probe unchanged, and `Db` refused where
    `Stmt` is written, so the build fails at `main.hero:74` instead of at run time.

    **What the sitting inherits and may not re-derive**: panel 109 refused
    `ptr owned` as a new `Ty` case — **179 exhaustive arms in 49 files**, five
    ceilings, an ABI move or a descriptor reuse — and set the condition on which it
    returns. A route that adds a `Ty` case owes that refusal an answer; a route that
    adds none owes the count of what it cannot catch.

- [ ] **M-handle-verdict** | the hazard panel 139 could not file, because its reproducer is not deterministic | `spec/heroes-spec.md` § 7 · `docs/panel/139-the-sentence-was-false-and-so-were-four-of-its-neighbours.md` § Found beside the sitting

    **Origin:** panel 139's completeness critic, 2026-09-13, and the coordinator's
    failure to reproduce it, which is why it is here and not in `DEFECTS.md`.

    § 7 says `==` is structural equality, and that a `ptr` compares as an address.
    The critic measured a record holding a **freed** pointer comparing `==` **true**
    to a record holding a fresh, unrelated allocation, because the allocator had
    reused the address. The coordinator ran the same shape and got **false**. So the
    class is real — an address can be recycled, and equality cannot tell — but the
    reproducer depends on the allocator, and `docs/work/DEFECTS.md` holds measured
    failures with reproducers. **This sitting owns it** because the answer is
    whatever `ptr` ends up carrying: give it identity and the question dissolves;
    leave it an address and § 7 owes a sentence saying equality on one is an address
    test with a recycling hazard, not an identity test.

- [ ] **M-handle-verdict** | whether `heroes mutate` gains the operator that would have caught 029, and what it costs | `selfhost/mutate/ops.hero` · `docs/work/DEFECTS.md` 029

    **Origin:** author decision 2026-09-13, at panel 135's ratification.

    **The class is invisible to the corpus's own instrument**, which is how it
    survived: fifteen mutation operators and none of them substitutes one `ptr`
    for another `ptr` in scope at a call site. An operator that did would have
    produced this exact mutant from `examples/sqlite/main.hero` and reported it
    unkilled.

    **Why it is an item and not a task**: a mutation operator is judged by what it
    kills, and one that produces mutants no rule can ever catch reports a hole
    that is a design decision rather than a defect — which is what this milestone
    is sitting to decide. So the operator's verdict follows the sitting's, and the
    item exists so the question is not lost if the sitting rules that nothing
    enters.

*******************************************************************************
