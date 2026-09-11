# 129 — the names inside the type

Convened and closed 2026-09-11 · M-labelled-types · **full lane, five seats**
· status: **provisional — author ratification pending**
(`docs/work/DECIDE.md`)

Panel 128 vetoed three routes and adopted a small pair, and recorded what
conservative would have been with the measurement that would lift its own veto.
The author then said, the same day, to attack the defect. **So this sitting
judged a thing that already existed**, which is a different sitting from one
that judges a proposal: three of the five seats sent the work back, and what
landed is not what they were handed.

## What was built

A function type carries its parameter names. Mandatory exactly at the positions
that share a type with another, refused at every other position, and part of the
type's identity. A declared function used as a value carries its own names on
the same condition. A call through such a value is checked as a call to a
declaration is: `needs_label` for a missing one, `wrong_label` for one naming
another position, that fix a `guess` (panel 127's rule). A generic parameter type
is untouched, because `check/generics.hero`'s `bind` descends structurally and
never compares whole interned ids.

## What each seat found, and three of them sent it back

**compiler-engineer — veto, and it was right twice.** *The design survived every
attack I could build. The tree did not.* The tree **did not compile itself**:
`selfhost/check/table.hero:154` declared `function label_key` and line 259 bound
a local of that name, and the seed compiler refused the same source with the same
message, so it was not bootstrap skew. `stat` said the binary had been built
sixteen minutes before the file was last written — **the coordinator had been
measuring with a stale compiler and had misread a failed build as a passing one**,
which is CL-054 with the coordinator in the chair. Three rows of the brief's own
table were therefore unreproducible: `629 pass` was exit 1 with zero tests run,
the spec row was `STALE: the recorded count is for 222ad91e3a6b7aa3`, and the
line count was not +178. It also refused the count itself: **+178 was `grep`'s
number, and the repository's own instrument — `code_lines`,
`tests/harness/suite_layout.hero` — said +329**, which `.claude/rules/module-shape.md`
already tells every session to ask instead of the shell. *"My veto is not about
whether it is worth 329 lines. It is about the build being broken, and about 329
being a number the author has never been shown."*

It then separated one typo from a design failure: with the rename applied in a
copy of the tree, the tests read 629 with one failure, and a `git archive HEAD`
control under the seed gave the same single failure. **Soundness: it could not
break it.** Six laundering routes — assignment, record field, array element, map,
return value, generic — all refused with `type_mismatch`; the label interning is
injective by construction and it checked the argument rather than the comment. It
withdrew its panel 128 claim that this inverts a shipped `certain` fix. And it
found **two warts of one class**: `(function(a: str, a: str) -> str)` and
`(function(_: str, _: str) -> str)` were both accepted, and the swap inside the
second was accepted at exit 0 — the names were mandatory and **not required to
distinguish**, which is the whole of what they are for.

**ffi-pragmatist — object, on a regression it built.** Five bindings run:
`atexit` and `sqlite3_busy_handler` byte-identical before and after, which is its
panel 128 prediction HELD on the built thing; `curl_write_callback` and zlib's
`free_func` bind and run under the new spelling; `qsort` is `ffi_callback_type`
on both. **Its panel 128 prediction HELD and harder than it had claimed**:
`curl_write_callback` with `size` and `nitems` transposed against `curl.h` runs at
exit 0, `_Generic` selects the real typedef for the transposed one, and
`clang -Xclang -ast-dump` keeps **no `ParmVarDecl`** for a typedef at all — so the
names are erased by the front end rather than merely unread. *"The rule buys
agreement between two lines of one Heroes file, and nothing against the header."*

Its object was a program: `zwild.hero`, two implementations of zlib's one
`free_func` handed to one extern, the ordinary one naming the argument it ignores
`_`. It **ran at exit 0 before and was refused after**, with no fix offered.
*Ignoring an argument is what C callbacks do.* And its section-5 table is the
honest scorecard at the boundary: of three transpositions, the Heroes signature
is killed, the extern's labels are killed, and **both transposed together —
the author who read `zlib.h` backwards — survives at exit 0 with an ASan
`bad-free`**.

**spec-warden — veto, on one sentence, for panel 128's reason.** Not on the
budget: it measured the ceiling by grep, measured the document twice, once with
`heroes measure --refresh` and once straight at `count_tokens` on `git show HEAD:`
so the request offset cancels, and reported 466 free. **A reading of the drafted
sentence was false against a running program.** *Where no two share a type,
neither names any* mirrors the bullet's opening clause about signatures, and at a
signature a label where nothing is confusable is legal, optional, partial and
checked — `show(n: 1, s: "x")` prints `x1` at exit 0. It also priced the gross,
which nobody had, and refused the arithmetic the coordinator had assumed: **the
removal is spent producing the net and cannot also pay for it**, or any clause
buys itself by deleting a smaller neighbour. And it dominated the wording:
**eleven tokens cheaper, saying strictly more**.

**llm-ergonomist — object, on three silences, one of which is the sitting's
subject.** It wrote four programs from the text alone and never once hesitated
about where names go, which is the mechanics working. But **the text did not say
whether the function PASSED must declare the type's names, nor whether a call
through the type binds by name or by position** — three readings all consistent
with it, and under one of them a comparator declared with its names transposed
compiles and answers backwards in silence. So the construct bought a label and
not a check, which is exactly the gap this seat measured at panel 128. Second
silence: with two of four parameters sharing a type, *named arguments are
mandatory at the call site* never says **all** or **only those**, because the
text's one example has two parameters and the readings coincide. Third, incidental
and outside this milestone: the text gives no operation producing a `ptr` from a
Heroes value, so its `qsort` task was unwritable as posed.

**historian — approve, advisory, and it corrected its own panel 128 record.**
Two corrections filed underneath it: *removed* is too flat — SE-0111 as written
proposed removal, and the core team **accepted it with a revision keeping labels
as documentation**, then eight days later required `_` before each one, which is
a third outcome and the one Heroes has refused; and the first recorded regression
is **2016-10-04**, three months after acceptance rather than seven.

**The fact that decides whether Swift's experience transfers**: SE-0111's
Motivation names *subtyping between function types with and without argument
labels* as the disease. Heroes has no subtyping between function types, no
overloading, and generics never compare whole function types. **The cause Swift
cited is not present here**, and Swift's rejected alternative (b) — keep the
labels, prohibit the implicit subtyping — is the closest thing to what was built,
rejected as *less ideal* rather than as unsound. Two live languages keep names in
the arrow's identity and have not retreated: **OCaml since 2000-04-26**, whose
type error is this one's shape, at the cost of an escape hatch that still ships
(`ocamlc -nolabels`); and **Hylo today**, a Swift-descended language designed
after SE-0111 whose subtyping rule reads *the same number of parameters and
labels*. **Ada is the half-measure**, RM 6.4(7-8) and 6.3.1(17) against
6.3.1(18.1): the type carries names, the call may use them, assignment cannot
check them — thirty-one years, and **no recorded casualty found**, which the seat
flags as the strongest available argument that this is complexity spent on a rare
event. And the negative result, said plainly: **no language found makes names in
a function type mandatory conditioned on two parameters sharing a type.** C++
Core Guidelines I.24 has the exact trigger and is a guideline on declarations;
clang-tidy enforces it as a warning with five suppression heuristics; Python's
callback protocols check names but opt out by declarer. **A first is not a virtue
and not a vice; it is a reason to expect the failure modes to be undocumented.**

## Resolution adopted, provisional

Four changes, and three of them are a seat's objection answered rather than a
preference.

1. **The build, and the number.** The shadowing is fixed and the tree compiles
   itself; the compiler's own tests read **631, all passed**. The cost is
   restated in the repository's own unit and not in `grep`'s: **432 lines** by
   `code_lines` with comments counted, **240** with comments and cases out. The
   compiler seat's condition was *about 150*. **It is not met, and the number
   goes to the author rather than into a footnote.**
2. **A name is written at the confusable positions and nowhere else** — the
   ergonomist's second silence and the warden's unstated *all or none* rule, both
   gone at once: `(function(ptr, n: u64, size: u64) -> ())` names the two `u64`
   and leaves the `ptr` alone, and a call through it labels exactly those two.
   One rule now, in the type and at the call, and it is §4.9's own.
3. **`_` is not a name.** A parameter a declaration calls `_` contributes an
   empty name, and an empty name fits whatever the expected type calls that
   position. `table.fits` is an id comparison in every case but that one, and it
   fills the blanks and interns again rather than admitting a second notion of
   sameness. `zwild.hero` prints `both freed`.
4. **A name used twice is an error**, `repeated_parameter_name`, which closes both
   of the compiler seat's warts with one rule: `(function(a: str, a: str) -> str)`
   and the `_` twin are refused at the type.

**The document takes the warden's wording**, at **+38 vendored and +39 real**
against the draft's +54, and it says strictly more: § 3's row goes back to what it
said before panel 128, and § 9 carries the whole rule including the identity half
and the ergonomist's missing sentence — *the names are part of the type: the other
order is another type, and a call through a value names each in its own position*.
**Ledger row 66 says what paid: nothing.**

**What conservative would have been**, recorded so the author can choose it:
stopping at panel 128's small pair and leaving defect 026 open, which costs
nothing and leaves the inversion at 50% through a function value.

## The measurements

| what | before | after |
|---|---|---|
| `heroes mutate --operator swap-args`, four programs of calls through function values | 1 of 6 killed, **17%** | 6 of 6, **100%** |
| the same operator over `examples/` | 1733 of 1910, 96% | **identical to the mutant** |
| compiler's own tests | 628 | **631** |
| emission traces | 441 green | **441 green**, 9 re-blessed |
| `selfhost/check/walk.hero` | 1751 | **1842** |
| `selfhost/ast.hero` | 484 | **491** |
| `selfhost/check/table.hero` | under 300 | **371**, a new decided row |
| spec, real on `claude-opus-5` | 5624 | **5663**, digest `0d61cc71040884f6` |

**The headline does not move and the record says so first.** `examples/` holds no
function type with two parameters of one type, so the project's own corpus cannot
move; what moved is a corpus written for the shape, and panel 046 R1 makes that
an observation rather than a payment.

## Predictions to score

| seat | prediction | state |
|---|---|---|
| compiler-engineer (panel 128) | the net names both `check/walk.hero` and `ast.hero`, two or more ceilings rise, walk exceeds 1780 | **HELD on all three**, and it named a file the prediction did not: `check/table.hero` |
| ffi-pragmatist (panel 128) | `curl_write_callback` declared with `size` and `nitems`, the checker accepting them transposed | **HELD**, and harder: the front end keeps no `ParmVarDecl` for a typedef at all |
| ffi-pragmatist | a transposed FFI corpus reads below 100% with zero diagnostics | open, scored at M-check-completeness |
| historian H1 | same-typed function types in the tree reach 8-20 by M-check-completeness, and at least one mismatch is repaired by renaming a DECLARATION to match an annotation | open |
| historian H2 | pure-Heroes swap-args stays 100%, an FFI corpus does not | open, and it is the ffi seat's row by another road |
| historian H3 | at least one same-typed function type carries `a`/`b` or `left`/`right` on a commutative operation — a name paid for and buying nothing | open |
| llm-ergonomist | ≥3 silent reversals in 20 comparator tasks under the old text, 0 with the sentence that landed | open, and metric 2 still does not exist |
| spec-warden | — | its veto lifted inside the sitting, on the wording it priced |

## Author's verdict

**Ratified as adopted, 2026-09-11** (the author's word, `Ratifico com'è`, given the
same day the sitting closed, with the 432 lines and the unmoved headline metric
both in front of them). The names are in the type and in its identity, the
document says so at +39 real paid by nothing, and defect 026 is closed.

**And on the second question the author did not take the conservative answer.**
The sitting classified the `fold` role inversion as the price of §4.9's criterion
rather than a defect against it, and offered three ways to treat it. The author
chose the third: **build the route that closes it** — names allowed on any
function type, an expected type carrying none accepting a value whose type does,
and the generic path comparing the names it is given. The compiler seat's
objection to that route is on the record above and stands unsmoothed: it turns 26
id comparisons across 20 files into a relation, it introduces variance into a
language that has none, and **it does not close the class** — it gives a careful
author a way to close it one function at a time. The author decided knowing all
three, which is CLAUDE.md §12's *measurement beats opinion, including the
author's* running the other way: this is a judgement, not a measurement, and the
judgement is theirs.

That work is `docs/work/SCHEDULED.md`'s and its own milestone's; this sitting is
closed.

## What this sitting did not do

It did not close the role inversion through a **generic** callback. `fold` handed
a function that reads its two parameters the other way round still prints `cba`
for `abc` at exit 0, and the compiler seat gave two independent grounds for
calling that the price of a ruling rather than a defect: design.md:1432-1435
excludes monomorphised types explicitly and says why, and it is a parameter-role
inversion at a DEFINITION rather than an argument inversion at a call, so no rule
of §4.9's shape can reach it. It goes to `docs/work/DECIDE.md` as a question about
that criterion, not to `docs/work/DEFECTS.md`.

It did not close the boundary. The ffi seat's third transposition — the Heroes
signature and the extern's labels both read backwards, self-consistently — is
accepted at exit 0 with an ASan `bad-free`, and nothing in the language can see
it, because nothing reads a parameter name out of a C header.

And it did not make the cost small. **432 lines in the repository's own unit, for
a rule whose headline metric over the project's own corpus does not move at all.**
The compiler seat, which vetoed on that number's absence and not on its size,
said the trade is right — panel 128 measured this class reaching a
heap-use-after-free at exit 0, and CLAUDE.md § Precedence rank 3 puts robustness
above compiler size by name. The author decides.
