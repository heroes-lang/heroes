# Panel 101 — What a path binds: the last part, or the whole path joined

**Convened** 2026-09-02, hours after panels 099 and 100, by author question:
*"ma il module name non può essere una combinazione di cartelle e file?"*
**Trigger** surface syntax, `spec/**` (CLAUDE.md §4). **Lane** full, five seats.
**Status** `RATIFIED 2026-09-02 by author instruction` — see § Author's verdict.

**The question, and it is the author's own.** Panels 099 and 100 landed a path
that binds its **last part** (`use syntax/decl` → `decl`), with `as` to rename
and uniqueness scoped per file. The author asked whether the default could be the
**whole path joined** — `use check/state` binding `check_state` — which would
make collisions impossible, because two paths cannot be equal.

**Why it was worth five seats: the numbers were extraordinary.** Measured on the
compiler's own tree before the briefs went out: under a prefix-family nesting,
**127 of 169 modules would bind exactly the name their call sites already
write — 100% of them — and zero bindings would collide.** So nesting `selfhost/`
becomes a 558-line `use` edit with **none** of its **4065** qualified mentions
touched, against 4623 edits plus five renames under the last-part rule.

## The proposal, verbatim

> **(B) the joined path** (measured 3663 → 3665, +2 with panel 100's clauses
> kept; 3641 keeping only `as`; **3624** keeping neither):
> ```
> - A `use` may be a path: `use syntax/decl` binds `syntax_decl`, the parts
>   joined, and `use syntax/decl as sd` binds `sd` instead. …
> ```
> against **(A) the last part**, which shipped this morning and is HEAD.
>
> The ergonomist received the two label-stripped, differing in that one clause
> and nothing else.

## The verdict table

| judge | verdict | its own finding |
|---|---|---|
| llm-ergonomist | **VETO of (A)** · approve (B) with a two-word condition | from the spec alone: `state.enter(x)` cannot be resolved from the line plus the enclosing signature when three modules end in `state` and all declare `enter()` — *"meaning not determined by the line plus the signature, and the wrong determination compiles: that is the criterion exactly"*. Counted four hesitation points under (A), **two of them silent**, against one under (B), loud |
| spec-warden | **object to the tabled wording** · approve (B)'s direction | proved a **theorem** and then ran the instrument on it: because panel 032 R5 sanitises the whole path to `[A-Za-z0-9]`, any joined collision is already a **component** collision, so under (B) the uniqueness sentence and `as` become unnecessary and the spec **shrinks to 3624, −39**. Verified `a_b/c` vs `a/b_c` is already `module_names_collide`, exit 1 |
| ffi-pragmatist | **approve (B)** | enumerated **162,165** path pairs: joined-collides-while-component-does-not, **zero**; last-part-collides-while-component-does-not, **1970**. Three surface spellings of one SQLite binding emit **one md5**. And packages decide it: under (A) the collision fires *"on code the consumer did not write and cannot rename"* |
| historian (advisory) | **object to (B)** · support (A) with one condition | found **no language-level precedent for literal (B)** — the only instance is Dune's `Wmo__Cumulus`, a build system, with a **double** separator. Ada and Haskell bind the whole path and **both spent the uniqueness on renaming**: Ada ships `Text_IO renames Ada.Text_IO` in its **obsolescent** annex, and Haskell was offered implicit last-part aliasing in 2001 and **declined** |
| compiler-engineer | **approve (B)** (reported after the synthesis and after the author's decision; appended verbatim) | *"I went looking for a veto and the measurements went the other way: the joined default makes the compiler SMALLER."* Prototyped it: **three files of 169, +14 lines and −3 CODE lines**, because `module_errors.suggestion` already computed the joined name as its `as` suggestion — §1.7's subtraction test passed literally. Nested `resolve/` for real: **62 changed lines, every one a `use`, 0 of 245 qualified mentions**, and over the whole emitted C **21,514 differing lines of which 0 are C**. And it solved the span problem with no new field: the binding has no `as` exactly when `used.binding.end == used.name.end` |

## The disagreement, and it is a real one

**FOUR seats for (B), one against, and the one against has the argument no
other seat touched: (A) is reversible and (B) is not.** (The compiler-engineer
reported last, after the decision, and made it four — it says in its own words
that it went looking for a veto and the measurements went the other way.) Adding Idris-style
*"an unambiguous suffix is enough"* on top of (A) later is **additive**; changing
the default afterwards rewrites every call site in every program ever written.
The historian put that beside a search that came back empty — no language binds a
whole path joined by a separator that does not survive as structure — and beside
two languages that do bind the whole path and immediately built the escape hatch
into their standards.

**Against that, the two seats with the sharpest instruments both landed on (B),
from opposite ends.** The ergonomist, reading only the document, vetoed (A) on
§1.3's own test. The ffi-pragmatist, compiling, found the packages case decisive:
a consumer who installs two libraries each shipping a module ending in `client`
gets a diagnostic about files they do not own and cannot rename, and under (B)
both authors can ship without knowing what else is installed.

**And the warden's theorem is the finding that outlives whichever way this
goes**: `component(p) = alnum_filter(joined(p))`, so a joined collision is always
a component collision, and `module_names_collide` — which has existed since
M-module-namespace for the linker's sake — already refuses every one. The
ffi-pragmatist then enumerated it: **zero** counterexamples in 162,165 pairs. The
author's own worry, `aa_bb.hero` beside `aa/bb.hero`, is that theorem's most
obvious instance and is **already exit 1 today**.

## What decided it, and it is not a count of seats

The author's instruction was *"nel dubbio seguiamo la storia"* — when in doubt,
follow history. The seat that read history is the seat that dissented, and the
decision takes its side. Two consequences are worth writing down because a later
sitting will otherwise re-derive them:

**A veto is being overruled, and it is the thesis seat's.** The
llm-ergonomist vetoed **(A)** — the rule that now stands — on the criterion its
seat exists to apply: a line whose meaning is not determined by itself plus the
enclosing signature, whose misreading **compiles**. That veto is not lifted by
this ratification; it is **outstanding**, and R5 below is what it is traded
against. Its own lift condition is a measurement, not an argument: *"P2 measures
0/20 silent wrong-module programs on a tree with three same-basename modules each
declaring the called function"*.

**The four seats for (B) are not wrong about anything they measured.** Nothing
in this resolution disputes the 100%, the zero collisions, the 162,165 pairs or
the −39 tokens. What the decision says is that a **reversible** rule with a
measured hazard beats an **irreversible** rule with a measured saving, when the
hazard has a repair and the saving can be recovered later.

## Resolution — `RATIFIED, author instruction 2026-09-02`

**R1 — (A) stands: a path binds its last part.** The joined default is refused,
and refused on the historian's ground rather than on the other seats' numbers.
Panel 032 R4 is therefore **not** reversed, and panel 099 R3 and panel 100 stand
as written.

**R2 — the spec is unchanged by this sitting.** HEAD's 3663 is the landing
figure, and no ledger row is owed: nothing moved.

**R3 — the refusal is CONDITIONAL and its return conditions are named**, because
four seats measured real costs and a refusal that pretends they do not exist
will be re-litigated from scratch. (B) returns if any one of these arrives:
- **the ergonomist's P2 measures a non-zero silent rate under (A)** — models
  writing the wrong module and compiling, on a tree with three same-basename
  modules. That is the veto's own falsifier and it decides the thesis question
  this sitting could not;
- **a package ecosystem exists and the ffi-pragmatist's case fires in it**: a
  consumer whose two installed libraries collide on a last part, with no file
  they may rename. Measured at M-package-manager;
- **`as` usage in `selfhost/` exceeds one line in five** after nesting. The
  historian asked for this ratio and could not run it; the convener did, from the
  compiler-engineer's prototype: **39 of 558 `use` lines, 7.0%**, which is under
  the historian's own 20% bar and is therefore evidence FOR this resolution
  rather than against it. The number is registered so the next sitting scores it
  rather than re-measuring the argument.

**R4 — the historian's condition is answered, and it is answered with a
disagreement.** It asked that the same-leaf case be *"a diagnostic naming `as` as
a `certain` fix (Java's rule, Go's rule)"*. `module_binding_taken` names `as` and
offers the repair — but as a **guess**, not certain, and it stays a guess for a
reason the historian itself supplied: Go's own documentation **contradicted
itself** about which side of a collision to rename (golang/go#34603, closed
not-planned), and Google's style guide has to instruct *"prefer to rename the
most local or project-specific import"*. Which module gives up its name is the
author's choice, and §8 makes `certain` machine-applicable — a machine choosing
arbitrarily between two modules is the silent wrong-module outcome the
ergonomist's veto is about. The condition is met in substance (the repair is
named, on the line the author can fix) and declined in its letter, on the
evidence the seat brought.

**R5 — what (A) owes in exchange for standing over a veto.** The hazard the
ergonomist measured is real and this resolution does not wave it away:
- the same-leaf collision is refused **per file**, on the `use` line, with the
  repair named (landed at M-package-layout step 3);
- and the remaining silent shape — the one the ergonomist ranked above the whole
  question — is `use state` from inside `check/` binding the **root's** `state`
  while `check/state.hero` sits beside it. Reproduced with the real compiler at
  panel 100 and recorded there with why the loud repair was refused: Heroes has
  no warning severity, and a refusal would leave a file inside `check/` with no
  way at all to name the root's module. What is owed is a self-test pinning the
  behaviour and one sentence on the site's modules page.

**R6 — three corrections the sitting produced, all against briefs and documents
rather than against judges.** Go does **not** bind a path's last element: it
binds the name in the imported package's own `package` clause, so
`gopkg.in/yaml.v2` binds `yaml` — the convener's brief said otherwise, and the
correction opens a fifth option nobody has costed (the imported unit declares its
own short name, which Heroes cannot copy without a name declaration in every
file). Python's whole-path form is documented as a **cache invariant** rather than
an ergonomic choice. And Erlang's package removal, which panel 100 leaned on,
killed *hierarchy whose tooling did not understand it* — Erlang packages already
had the short form, so that precedent does not adjudicate (A) against (B) and
should stop being cited as if it did.

**R7 — the compiler-engineer's three conditions are landed in this milestone,
and all three were defects in work that had already shipped rather than
consequences of the proposal.** They are here because a refusal is not a reason
to drop what the sitting found. (1) `module_binding_taken`'s **`guess` fix did
not parse** on the shape where the second `use` already said `as`: the
replacement was built for a line with no alias, so it produced `as state as
ir_state`. It now branches on the seat's own exact predicate — the binding has
no alias exactly when `used.binding.end == used.name.end` — and is withheld
entirely where it would change nothing, which the corpus provoked while
`examples/shapes/` was being written. (2) **The premise nothing had ever tested**
now has a test that names its dependants: `component_of` erases `_` and `/`
alike, which is what makes `aa_bb.hero` beside `aa/bb.hero` a compile error and
what the warden's theorem rests on. (3) **Two comments written the same day were
already false** and are corrected in place rather than deleted:
`examples/shapes/main.hero` said *"last parts are unique across a program"*
hours after panel 100 repealed it, and
`tests/golden/check/use-has-a-path.hero` claimed `use a / b` *"is legal"* — read
off the parser rather than run, and it is `error[expected_declaration]` at
column 7.

**R8 — a briefing error of the convener's, recorded because it cost measurements.**
The brief told the engineer and the ffi-pragmatist that panels 099 and 100 were
*"both in HEAD"*. Panel 100's implementation was **uncommitted** at that moment,
so `git archive HEAD` gave both seats a compiler without `as`; both found it,
both re-copied from the working tree, and both said so. The rule this breaks is
the one about briefs carrying facts rather than intentions — the work was
committed an hour later, which is exactly why the sentence read as true when it
was written.

## Predictions to score

| judge | prediction | checkable at |
|---|---|---|
| llm-ergonomist | under (A), a read-and-extend task on a tree with three same-basename modules yields **≥20%** accepted programs that call the wrong module and still compile; under (B), 0% by construction. **This is the veto's falsifier** | harness run |
| llm-ergonomist | under (A), a write task needing two colliding imports is **≤60%** first-try correct, the dominant failure being two bare `use` lines binding one name | harness run |
| spec-warden | if (B) ever lands in its own form, `module_binding_taken` becomes **unreachable** — zero programs can provoke it — and `SPEC_TOKENS` moves to 3624 | the milestone that lands it |
| ffi-pragmatist | no path pair exists with joined(P)==joined(Q) and component(P)≠component(Q) — **zero in 162,165 enumerated pairs plus the real 169-module tree**, so one counterexample kills the theorem this sitting rests part of its comfort on | standing |
| historian | if (B) ships, **more than half** the `use` lines naming a path in `selfhost/` carry an `as` within one milestone — Ada's style guide's instruction and Haskell's community outcome. Falsified under 50% | the milestone that lands it |
| compiler-engineer | at M-selfhost-nesting close the diff over `selfhost/` holds **0** changed lines that are neither `^use ` nor a comment, and `--emit-c` before/after differs in **0** non-`#line` lines. Falsified by one qualified mention needing an edit | M-selfhost-nesting |

## What a veto would compel

**The ergonomist's veto of (A) is live and unlifted.** It compels exactly what R3
and R5 record: the hazard is refused where it can be (per file, on the `use`
line), the one silent shape is pinned by a test and documented, and the veto's
own measurement is registered as the thing that decides whether (B) returns. If
P2 measures a non-zero silent rate, this resolution is wrong and (B) is the
answer — and the cost of having chosen (A) first is exactly the reversibility the
historian argued for, which is the trade the author took.

## Author's verdict

**RATIFIED 2026-09-02**, by author instruction *"nel dubbio seguiamo la storia"*,
given after four of five verdicts were reported and summarised to the author,
including the 3-1 split and the ergonomist's veto.

**What the yes settles**: (A) stands, the spec does not move, panel 032 R4 is not
reversed, and (B) is refused **conditionally** on R3's three return conditions.

**What it does not settle**: the ergonomist's veto, which stays outstanding with
its falsifier registered; and whether
`M-selfhost-nesting` nests at all — which remains panel 100 R6's three-road
choice, now with the fourth road (the joined rule) priced and refused.
