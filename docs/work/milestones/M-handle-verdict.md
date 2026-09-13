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

**Step 1, 2026-09-13: defect 030 closes, before the sitting.** Panel 139's
four-act repair and panel 144's § 10 correction had both been adopted and both
sat behind the one network call the spec's digest needs; the author authorised
it the evening M-deferral-ledger closed and everything queued landed in one
commit. § 3 now says a `ptr` is a copied address wherever it sits, § 10 says a
push through a field copies the array and names the cheap spelling, § 4 lost the
half of a sentence the grammar beneath it already said, and
`examples/ledger/db/sqlite.hero`'s `stepped` takes `@statement`. Real count
7531 → 7610 against 8192. **Two things the sitting had not found**: the harness
held one refresh date for two independent documents (split), and the warden's
landing prediction is falsified at +79 real against ≤ +45. **What remains for
the sitting is exactly what the sentence now names and nothing refuses**: 031,
one copy freeing what every other copy holds, and 029, the swapped handle. Open
defects go from three to **two**.

**Step 2, 2026-09-13: the sitting (panel 145,
`docs/panel/145-a-handle-is-a-pointer-with-a-name-and-the-compiler-already-reads-the-name.md`).**
Five seats plus the critic, and every brief on disk for the first time. **029
ENTERS**: a fieldless `extern` record is a handle spelled as the header's own type
name starred; the checker already refuses the swap once the parameter type is
nominal, and clang refuses it too under a flag already carried; zero new `Ty`
cases. **030 REFUSED as a form**, its home the Part 6 borrow-checker row, zero
words added; the affine handle unpriced and owed a count. **031 WART adopted,
defect not closed**: the consume mark with the borrowed-parameter rule is priced
at 120-200 lines and scheduled as a prototype, because the critic asked whether a
wart is admissible for a §1.12 violation at exit 0 and the honest answer is *only
while the guard is being built*. **The critic corrected the coordinator's brief
four ways** and the form's spelling once (`struct <tag> *` reaches one of three
handle types; `CURL` is `typedef void`). **Not decided, said rather than omitted**:
the type's name, `unsafe_ptr` or a family, reserved by panel 139 and handed to no
seat. **The steps this sitting orders**: 3 the `swap-ptr` operator (the number
before the form), 4 the form with § 13's fence rewritten and the three repairs the
route exposed (`ffi_tag.hero` incomplete-as-absent, `freer.hero`'s silent retype
of `owned` on a `ptr`, `empty_record`'s caret) plus CL-036's re-printer walk, 5
rule (i)'s prototype and the affine handle's copy-site count.

**Step 4's scouting, 2026-09-13, and the form NARROWS before it lands**
(`docs/records/log/2026-09-13-1300-the-form-narrows-before-it-lands-and-the-false-clause-was-the-coordinators-not-the-sittings.md`).
Five scouts plus a completeness critic mapped the change before a line was
written, and two author decisions came out of it. **A handle does NOT convert to
`ptr`**: the clause saying otherwise was in the **coordinator's brief**, not in
the ratified resolution — checked line by line — and the critic ran it
(`error[type_mismatch]: expected ptr, found Db`). Implementing it would have
re-opened the class through every function still declared `ptr`. Struck at zero
cost, since it is today's behaviour; the price is that a parameter receiving a
handle is retyped, 7 sites in `examples/sqlite`, 18 in the ledger, 2 in curl.
**And three shapes the sitting never ruled on are REFUSED rather than
implemented**: a handle as a map key (the checker and emitter scouts had
opposite plans and panel 145 says `hash` zero times), two records sharing one
tag (two Heroes types, one C type, so a swap between them is a mutant clang
accepts and nothing could ever kill), and a `tag` naming a type the header
declares COMPLETE (silently a pointer where the author meant the struct). **The
engineer's 150-line condition had fired at the scouts' scope** — ≈203 for the
form alone, with `emit/ctype.hero` at 371/380 `DECIDED` and
`emit/structural.hero` at 293/300 — and the narrowing is what answers it rather
than a retreat to WART or a build that decides three reserved questions by
omission.

**What step 4 still owes, as one coherent diff**: the form; the three refusals;
the three repairs the route exposed; § 13's sentence **with the fence rewritten
to use it**; CL-036's walk; `examples/sqlite` rewritten; the seed; and the
after-number, which must read **7 of 7** on the class `docs/measurements/029`
measured at 0 of 7. **The handle predicate is written ONCE** and called from the
parser, the checker, the emitter and `mutate`: the scouts proposed four
definitions in four homes, which is the contract's opening rule broken four ways
inside one step. **And `selfhost/cli/pointee.hero:116` is the file nobody
named**: `.named` answers *not numeric*, so `@out: Db` gets no pointee width
assertion and the out-parameter story rests on the `_Static_assert` probe alone.

**Step 4, 2026-09-13: the form lands and DEFECT 029 CLOSES**
(`docs/records/log/2026-09-13-1400-…`). `sqlite3_step(db)` is
`error[type_mismatch]: expected Stmt, found Db` on its own line, and the
unchanged example prints `rows: 3` byte-identical. **The measurement**:
`swap-ptr` read 15 mutants, 8 killed and **0 of the 7 that were the class**
before; **15 of 15** and **7 of 7** after, with the denominator unmoved because
the operator learned the form in the same commit. **The form is ~170 lines**
against the scouts' ~203 and with both ceilings untouched: the predicate written
once in `selfhost/handles.hero`, the form in `parse/tails.hero`, the spelling in
`emit/ctype.hero`, `nullptr` adopting a handle in `check/contextual.hero`, and
`const_pointer` at the five sites where `const T *` qualified the pointee.
**Zero new `Ty` cases, `check/table.hero` untouched** — the engineer's own
prediction, held. **Two refusals landed** (a handle as a map key, two records
one tag) **and the third was withdrawn on evidence**: `record File tag FILE`
compiles, runs and writes through `FILE *`, so refusing a complete tag would
break a legitimate binding — the fieldless form MEANS the pointer. Spec § 13
gains the sentence **and its fence is rewritten to use it**, +76 vendored net,
real 7610 → 7721.

**Step 5, 2026-09-13: DEFECT 031 CLOSES, and both lists reach zero.**
`consumes` after a C parameter says the call ends that value's life, and a call
that ends a life may not be handed one the caller borrowed. The filed reproducer
is `error[consumed_borrowed_handle]` on the line that frees. **The condition
panel 145 wrote is met exactly** — a prototype at ≤ 150 code lines refusing it in
`heroes check` — at **114**. **Two seats contradicted each other and the run
settled it**: the engineer predicted 0 of 17 refusals in the shipped binding, the
critic said 2, and it is 2, the two the critic named. **A third cost neither
counted**: the mark propagates, 24 lines across `examples/ledger/` for two marks,
because `@` obliges every caller to hold a cell. **And 2 of 17 fired the Part 6
borrow-checker row's own falsifier** (below 13 of 17), so that row's ground moved
from soundness to cost the same day. **What is closed is the defect as filed**:
repaired to `closed(@h)`, `--sanitize` still fires, and Part 8 wart 20 states
that class in the present tense with this measurement in it. The two diagnostic
repairs landed with it, and the formatter was taught the mark after it was caught
dropping it in silence.

**What step 4 owes and did not do**, filed rather than half-done: the two
repairs panel 145 charged to it — `emit/ffi_tag.hero` reading clang's
*incomplete definition* as *absent*, and `check/freer.hero` retyping any `owned`
parameter to `str?` so a `ptr` gets two diagnostics about a string. They are the
milestone's fifth item.

*******************************************************************************
**OPEN: 1**

- [ ] **M-handle-verdict** | whether `ptr` keeps its name, and it may not sit before the handle form has landed | `docs/panel/139-the-sentence-was-false-and-so-were-four-of-its-neighbours.md` § the reserved question · `docs/panel/145-a-handle-is-a-pointer-with-a-name-and-the-compiler-already-reads-the-name.md` § Author's verdict · `spec/heroes-spec.md` § 3, § 13

    **Origin:** author decision 2026-09-13, ratifying panel 145. Panel 139 had
    reserved the type's name — `unsafe_ptr`, or a family — for that sitting *by
    name*, and no seat's brief carried it; the sitting recorded that it was not
    deciding it rather than deciding it by omission, and put it to the author.

    **The author declined both offered answers** — keep `ptr`, or rename now —
    **and ordered a sitting of its own, after step 4.** The reason is the
    strongest thing said about the question all evening: today a rename would be
    argued over **27 `extern` lines in `examples/` that name a `ptr`**, and the
    handle form removes most reasons to write a bare `ptr` at all. So the sitting
    that argues the name should argue over **what is left**, which is a number
    nobody can have until the form has landed and the bindings are rewritten.

    **What it may not do**: sit before step 4. **What it is owed at its
    opening**: the count of bare `ptr` remaining in `examples/`, `selfhost/` and
    `tests/golden/run/` after the rewrite, per position — parameter, result,
    field, binding — because the argument for a name that says *unsafe* rests on
    how often a reader still meets one. **What it inherits**: the historian's
    survey at panel 145, whose finding was about **typing** and not naming —
    Swift's regret is that `OpaquePointer` cannot tell two handles apart, not
    that it is called `OpaquePointer` — so the precedent does not transfer and
    the sitting must find its own.

*******************************************************************************
