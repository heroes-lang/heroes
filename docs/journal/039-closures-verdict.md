# 039 — M-closures-verdict

Closed 2026-09-08, tag `m-closures-verdict`. Three steps, two sittings, four
defects, and the first Part 7 item ever judged.

## Goal

Rule on design.md Part 7 item 1, closures, and item 12, inline blocks. The
milestone's deliverable is a **verdict** and not a feature, which is why it could
close with a refusal and did.

Part 7's preamble had made both admissible since the fixpoint of 2026-08-18, and
item 1 had carried the same sentence since 2026-08-04: *"v1.5, immediately after
the first running program … capture by copy = a record plus a function pointer.
Will delete the handful of named one-line helper functions that currently exist
only to be passed around."* Neither item is on the §1.0 closure list — the
compiler self-hosts with named functions — so the whole warrant was Principle 0's
second branch: a measured Part 11 effect, or a §1 argument a panel accepts.

The milestone also owed a count before it sat. `docs/work/SCHEDULED.md` had held
that item since 2026-09-03, with its own reason written into it: *"a sitting that
rules on a form nobody counted rules on a feeling."*

## What surprised

**Every number in the sentence that deferred closures was wrong, and by more than
a rounding.** *"~150 lines"* is **282 exhaustive-match arms in 74 of 190
modules**, five DECIDED file ceilings breached, and no legal home for the parser —
a module of its own gives `error[module_cycle]`. *"~60 spec tokens"* is **+118**
on one draft and **+141** on a second, independent one, against 70 free. And
*"no shared cells, no lifetime problems, no refcount interaction"* is false:
`selfhost/check/counted.hero:91` answers `false` for `.function_ty`, so a
captured `str` is a leak.

**The subtraction test has an exact answer and it is small.** Under item 1's own
wording — a helper that exists ONLY to be passed around — the whole tree holds
**21 declarations and 66 lines, every one of them in an example program**, in
seven files, four of which are the programs whose *subject* is the higher-order
function. The compiler holds none. It passes a function as a value at **four
production sites in 55,361 lines**, none of them capturing, and it calls `map`,
`filter`, `fold`, `find`, `any` and `all` at **zero** sites: it writes loops.

**A count is an instrument, and the first instrument was thrown away.** The IR's
`.func_ref` is what the compiler itself says about a function used as a value, so
`--dump-ir | grep funcref` is ground truth — for the 150 modules that build
standalone, because `use` starts at the directory of the file you compile. A text
census covers the rest, and its first version pooled names across all 308 files
and reported 217, nearly all of them locals called `at`, `after`, `env` and
`done`. Scoped per file — which is sound because the language forbids the
alternative, `double = 3` under `function double` being
`error[shadowed_binding]` — the two instruments agreed **78 to 78, zero missed
and zero extra**.

**The hazard is not the one the record feared.** History says capture by
*reference* is what languages have had to repair: Go shipped a breaking change at
1.22 in 2024, C# moved the `foreach` variable inside the loop at 5.0 in 2012, and
Java 8 forbade mutable capture outright. **No language that shipped capture by
copy has reversed it.** So the Part 6 row's falsifier deliberately rests on
compiler need and a compiled representation, and never on an argument about
copying.

**The reader-facing cost is the `@` cell, it cannot be refused, and the rule that
would warn about it is the rule that does not fit.** A `total: i64 @ 0` captured
by copy freezes at creation; a *write* to it is refusable and a *read* is not,
because whether the frozen value was wanted is undecidable. Every language a
model has read captures by reference, which makes the accumulating closure the
textbook idiom. The only draft of the clause that states this is the +118 one.

**Refusing is the mainstream position under this language's constraints.** Four
languages with manual memory, a C ABI and no hidden allocation refuse capture in
writing — Zig, Odin, C3, and Oberon since 1988, which self-hosts. And the sharpest
fact in either sitting is about the *cheap* option: `ziglang/zig#1717`, the
capture-free anonymous function, was **accepted in 2020 and rejected in 2023**.

**And the milestone was tuning the wrong clause.** The seat that reads only the
spec reported that what gated all three of its tasks in all five arms was not the
absence of closures: `filter`'s signature is written nowhere. That produced a
second sitting the same day, on the author's instruction, and the second sitting
produced the finding that ended its own ballot — **a signature is byte-identical
for keep and for reject**, so writing the types buys the loud class and nothing of
the silent one. Prose closes it at less than half the price. The spec bought
**+30** of prose and refused the signatures.

## What broke and why

**Two of the coordinator's own numbers, both caught by somebody else.**

The free budget was written as **71** in four records. It is **70**:
`tests/harness/suite_spec.hero:204` reads
`if SPEC_TOKENS + FFI_FLOOR >= CEILING`, so 4096 is itself red and the spec's own
ceiling is 4035. `heroes measure`'s printed sentence is exact; the error was a
subtraction that treated an excluded bound as included. Found by the spec-warden
at the second sitting.

The claimed silent-wrong class was an inference dressed as a measurement. The
brief told three seats that `examples/interpreter/run/expr.hero` would go wrong
under capture by copy, because the left operand is evaluated with `f` and the
right with `first.frame`. **`f` is a plain immutable parameter** — `grep -c '@f:'`
is 0 — so a captured copy can never diverge, and the mistake described is a
different binding being named, writable today with no closures. Found by the
compiler seat, which then produced the class that is real: the `@` cell.

**Four defects, and the class none of the three suites was watching: a wrong
MESSAGE is not a wrong answer.** All four exited 1 carrying a diagnostic, which is
exactly what the goldens check, and all four misinformed inside it. `<` named two
of the ten types it accepts, at three sites, while the sibling `mixed_arithmetic`
had been saying the true thing all along. The `guess` fix offered
`fit_<width>(x)`, a family that shipped at M-sized-integers and was withdrawn for
breaking panel 017's naming rule, so following the compiler's advice produced
`unknown_name`. `print(HERO_OS_OK)` printed `0`, because panel 033 R5's rule had
been applied to one kind of extern and not the other. And a six-line program with
an ordinary generic function was shown `#0`, an index into the checker's own
tables — **while the module doc called that case unreachable and a test pinned its
output**, which is how the dead premise stayed comfortable.

**A wart this milestone introduced, and it is recorded rather than left.**
Repairing the constant leak made a constant reach a message reading *"an extern is
never CALLABLE across a module boundary"*, and a constant is read rather than
called. It says *reachable* now, and the three golden lines pinning the old word
were hand-edited, never regenerated.

**A gap filed rather than tapped shut.** Nothing here watches the TEXT of a
`guess` fix: `.expected` compares `check --brief`'s stderr, which carries no
`fix (…)` line, and `suite_fixes` tests only `certain` fixes through `.fixed`. So
the `fit_` fix could have stood indefinitely, and the four cases written here pin
every message and not one fix title.

**And the coordinator claimed a green net once on the strength of the wrong
command.** `./heroes run … | grep -oE "^  [a-z_]+:"` threw the counts away and
left twenty suite names and no numbers; the exit code belonged to the pipeline.
The net was then run once properly, captured to a file, and read line by line.

## Predictions scored

| Prediction | Verdict |
|---|---|
| compiler-engineer, panel 120: after the `render.hero` repair, `grep -c '"#" +' selfhost/check/render.hero` goes 1 → 0 | **CONFIRMED**, run: 0 |
| compiler-engineer, panel 120: with `range` last and no type in a space-free code span, `spec/offered` and `spec/names` pass | **CONFIRMED** by the suite: spec 10 of 10, and the full net green |
| the same prediction's *"exactly 24 harvested names"* | **NOT INDEPENDENTLY REPRODUCED**, and that is the honest entry. Two attempts at replicating `offered_builtins` by hand gave 28 and 22. The harness is the authority and it is green; the rule is not reproducible by eye, which is why the seat's constraint was worth writing down rather than remembering |
| compiler-engineer, panel 119: design.md's *"no refcount interaction"* is measurably false | **CONFIRMED** — `check/counted.hero:91` answers `false` for `.function_ty`. The instrument the seat proposed was unscoreable as written (the sentence wraps across two lines, so its grep was already 0) and was restated before being scored |
| historian, panel 120: after a signature-only spend the spec would answer neither *does `filter` keep or drop* nor *what code does `find` fail with* | **DISCHARGED, not falsified**: the resolution took its advice, so the spend was not signature-only and both questions are now answered at `spec:188` and `spec:160` |

Everything else registered by the two sittings is scored later, and each names its
milestone: M-web-framework for the ffi seat's two, M-guide-book for the warden's
two, M-thesis-harness for the ergonomist's, M-deferral-ledger and
M-check-completeness for the historian's.

## What landed, and what carried forward

Nothing entered the language. Part 7 item 1 left for a **Part 6 row** refused on
cost alone, with its falsifier; item 12 left for the ***unplaced*** paragraph in
`comptime`'s shape, with three joint return conditions. Both numbers are kept
struck and never reused, because Part 7's numbers are cited across the record. The
2026-08-04 paragraph that priced closures stands with its correction beside it.

The **spec** grew by 30 tokens and none of them are a type: *no anonymous
functions* joins the enumeration of absences, `filter` *keeps what the function
accepts*, `find` gives *the first it accepts, or an error*, and `not_found` sits
beside `missing_key`. It measures **3995** of a hard 4096, with **40** free.

**Carried forward**, each one filed rather than remembered: the capture-free
narrowing, named in the Part 6 row as the form that returns on a measured Part 11
effect, carrying the module-plus-index naming rule so a future sitting re-derives
nothing; inline blocks' three return conditions; the instrument for a `guess`
fix's text; and a live condition of the author's own 2026-08-26 decision, that
o200k is vendored *"if the headroom ever falls below the spread"* — the spread is
78 and the headroom is 40, so it is met, reported and not acted on. Seven items
wait in `docs/work/DECIDE.md`, four of them panel 118's and two of them this
milestone's own ratifications.

The chain's next entry is **M-interpolation-verdict**, the second of the four
rulings, and it inherits a shape this one did not have when it opened: a verdict
milestone can close by refusing, and the refusal costs a Part 6 row with a
falsifier that somebody can run.
