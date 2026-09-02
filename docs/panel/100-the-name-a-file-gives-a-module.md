# Panel 100 — The name a file gives a module: `as`, and what "unique" is scoped to

**Convened** 2026-09-02, hours after panel 099 landed `use` paths, by author
instruction: *"rivaluta subito la clausola della spec «Last parts are unique»,
introducendo gli alias tipo `as` locali al file"*. **Trigger** surface syntax,
`spec/**`, a diagnostic class (CLAUDE.md §4). **Lane** full — the spec must state
a sentence and a reader can get it wrong. **Status** `provisional — author
ratification pending`.

**What forced it was a measurement, not a preference.** Panel 099's rule landed
at 08:00 and the author asked for the compiler's own 169 modules to be grouped
into directories by subsystem (`check_state.hero` → `check/state.hero`). Under
*"Last parts are unique"* that tree **does not compile**: measured, and the size
depends on the mapping rather than on the tree —

| a family is a prefix with ≥ | families | files that nest | collision groups | files in one |
|---|---|---|---|---|
| 2 | 10 | 132 | **15** | **36** |
| 3 | 9 | 130 | 15 | 36 |
| 4 | 8 | 127 | **13** | **32** |
| 5 | 7 | 123 | 12 | 30 |

The convener briefed 13/32 (families ≥4); the spec-warden independently measured
"15–17 groups over 42–46 files" under its own mapping and said so. **Both are
right and the table is why**: `state` ×4, `decls` ×3, `ops` ×3, `types` ×3,
`builtins` ×3 collide under every mapping. No mapping produces zero.

## The proposal, verbatim

> **Alias, file-local** (measured 3630 → 3639, +9 net, because ", No aliases,"
> goes for −3):
> ```
> - `use geom` binds `geom` … Every module you name needs its own `use`. No wildcard.
> - A `use` may be a path: `use syntax/decl` binds `decl`, or `as name` binds that.
>   Last parts are unique unless renamed. Every `use` starts at the directory of
>   the file you compile.
> ```
> against the **status quo**: no alias, last parts unique across a program, and
> the 32 renames that follow.
>
> The ergonomist received the two label-stripped as `variant-1` (alias) and
> `variant-2` (status quo), each a complete spec, neither marked as anyone's
> preference.

## The verdict table

| judge | verdict | its own finding |
|---|---|---|
| compiler-engineer | **object · approve the renames instead** (reported after the synthesis; appended verbatim, see § A second correction) | built the alias, nested the real `resolve` family, self-hosted both ways: **net +22 code lines**, zero in every `emit_*`, `ir_*`, `cli_*`, and a **C-level fixpoint** on the moved TUs. Then measured what the alias exclusively buys: **39 `use` lines in 37 of 154 files, and 36 of the 37 are the single word `state`** — **five renames take it to zero**, because the 4065 qualified mentions are rewritten by the nesting either way |
| llm-ergonomist | **approve variant-1 with conditions · object variant-2** | the file the panel was convened over is **impossible** under variant-2 — *"no legal program exists, so that harness item scores zero for every model"* — and its refusal's only repair lives in another file plus every importer |
| spec-warden | **VETO on Principle 0** (budget clean, and it says so) | measured the third option nobody tabled: **keep the stems** (`check/check_state.hero`) and the tree nests **today, zero renames, zero spec tokens, zero collisions** — all 169 stems are already unique |
| ffi-pragmatist | **approve with three conditions** | implemented it, **532 tests green**, and proved the C cannot see an alias: the bindings TU is **md5-identical** aliased and unaliased, `#include` order follows the *use-line* order and not the binding name, and the alias token appears **zero** times in the emitted C. Then found the tabled sentence is **unimplementable where the check lives** |
| historian (advisory) | **approve, in Modula-3's shape** | **Wirth kept module aliasing twice** — Oberon-2 (1993) and Oberon-07, the smallest languages he published. **Modula-3 defines `IMPORT I` as sugar for `IMPORT I AS I`** with uniqueness **per compilation unit**. And the one language that made Heroes' current choice stick is **Erlang**, whose price is `snmp_generic` — *"clumsy"*, said its own designer in 2002 |

## The sitting's product: every tabled wording was wrong, and two seats found the right one independently

**The ffi-pragmatist implemented the tabled sentence and it does not work.** *"Last
parts are unique unless renamed"* cannot be checked where the check lives:
`module_paths.last_parts_collide` walks the **file set** and never sees a `use`
line, so two packages both shipping `client`, **both renamed**, are still
`error[module_last_parts_collide]`, exit 1 — measured on its own build. With the
check disabled both link and run at exit 0, so the C side is clean and only the
check is in the wrong place.

**The historian, reading precedent and no code, named the same repair**: Modula-3
enforces uniqueness **per compilation unit**, not per program, and makes the
plain form sugar for the alias rather than the alias an exception to the plain
form.

**That answers the warden's sharpest technical objection rather than dodging
it.** The warden refused *"unique unless renamed"* on §1.3 — it makes
`state.tag()` mean *"whatever a header said"*, which is the ambient-context
category §1.3 names and rejects. Under per-file uniqueness the qualifier means
what **this file's own `use` lines** say, which is the locality `use` already
has: the answer is always in the file on screen, three lines up.

**Measured, this session, by the convener** — the per-file wordings nobody had
priced (`./heroes measure`, binding maximum):

| wording | tokens | net |
|---|---|---|
| spec today | 3630 | — |
| remove ", No aliases," alone | 3627 | −3 |
| warden's `E`, *"Bound names are unique"* — scope **unsaid** | 3638 | +8 |
| **`P3`, *"Names are unique per file."*** | **3639** | **+9** |
| tabled `A`, *"unique unless renamed"* | 3639 | +9 |
| `P2`, *"No file binds one name twice."* | 3640 | +10 |
| `P1`, *"One file's names are its own."* | 3641 | +11 |
| `B`, uniqueness dropped entirely | 3642 | +12 |
| `P4`, Modula-3 spelled out in full | 3644 | +14 |

So the form that **says its scope** costs the same +9 as the tabled form that
left it ambiguous, and one token more than the warden's cheapest, which does not
say it. The ergonomist's condition 4 asked for exactly that scope, of **both**
variants.

## The disagreement that does not dissolve

**The warden and the historian looked at the same artifact and named it opposite
things, and neither is wrong.**

The artifact is a filename with its subsystem in it: `check_state.hero`, or
`check/check_state.hero`. The warden measured that keeping it costs **nothing** —
verified by the convener: `check/check_state.hero`, `ir/ir_state.hero`,
`resolve/resolve_state.hero`, `use check/check_state`, **exit 0, zero renames,
zero spec tokens**, because all 169 stems are already unique. It calls that the
free option and Principle 0's answer.

The historian read the same shape in Erlang's standard library — `snmp_generic`,
`snmp_error`, `snmp_supervisor` — and quoted the designer of that namespace, from
the primary paper: *"The solution adopted by most programmers today is to always
use prefixed names such as `snmp_supervisor`, `snmp_error`, `snmp_generic`, etc.,
but this is clumsy."* The hierarchy that paper proposed shipped experimentally and
was **removed** in 2013 (OTP-10348), so Erlang has lived with the prefixes for 24
more years. It calls that road the one refusing `as` takes.

**Both statements are true of the same tree.** What separates them is what the
directories are *for*: if they are for finding a file, the free option delivers
it completely. If they are for shortening what a call site writes — measured,
**4065 qualified references** to the 127 prefixed modules, against **558** `use`
lines — then the free option delivers none of it, because the qualifier is the
stem either way.

**And the warden's own pincer is the honest statement of the cost**: a file-local
alias buys shorter call sites only if each file picks a *short* name, and then the
same module is `cstate` here and `st` there. Under Modula-3's per-file rule
nothing in the language stops that. Every ecosystem that allows renames answers
it with a **consistency rule enforced by tooling** — `importas`, ruff `ICN001`,
Google's Go style *"always using the same local name"*, Ada 95 Q&S's *"project-wide
standard list of abbreviations"* — never with prohibition, and the historian
predicts this project lands in the same place.

## A CORRECTION TO THE CONVENER'S BRIEF, and it is CLAUDE.md §1's first named shape

The brief told all five seats that panel 032 killed the quoted-path form because
it put *"one module in two lexical classes, so no single textual search finds
every importer"*, **and that an alias does the same thing**. The second half was
an inference presented as a premise, it was flagged as a question rather than
asserted, and the historian falsified it anyway.

**They are not the same thing, and the difference is in the measurement 032
actually took** (verified verbatim after the pushback, `docs/panel/032:82-85`):
under the quoted form the ergonomist wrote `use token` in one file and
`use "lex/token"` in another — **the path text itself differed between
importers**, which is what defeated the search. Under an alias every importer
still writes `use check/state`, character for character; only the local qualifier
varies, so a search for *who imports this module* finds **100%** of them.

Panel 100's ergonomist measured the split independently and it is real but
narrower: the two variants **tie** on *find every importer* and diverge on *find
every use*, where the alias's failure is a silent miss and the status quo's is a
visible false positive. The historian adds that Go's published grep objection
attaches to **dot imports** — unqualified names — and not to renames.

The tell was the connective, the command was `sed -n '77,86p' docs/panel/032-*.md`,
and it was run only after a judge pushed back.

## What the last seat brought, and it arrived after the author had decided

The compiler-engineer reported **after** the author's instruction. It did not
veto, and it said why in its own words: its veto fires on a core construct or a
breached ceiling, *"and I measured both and neither is true. Saying veto here
would be inventing a rationale."* Three of its findings change what this page can
claim, and the last one changes what the milestone should probably do.

**It built the thing and it is small.** Twelve of 169 modules, **+125/−103 code
lines, net +22** on a 30,756-line tree, all frontend: zero lines in
`emit_mangle.hero`, `source.hero`, `cli_units.hero`, and every `emit_*`, `ir_*`,
`cli_*` and `check_*` module except one diagnostic printer. `--dump-ir` shows the
alias gone. **531 selfhost tests, 1080 net checks, 0 failures.** It then nested
the real `resolve` family — 10 modules, 31 `use` lines aliased — and the moved
TUs' C is **byte-identical** between the aliased and nested builds, differing
only in `#line` path strings for the modules the alias never touched. And a fact
that matters for `M-selfhost-nesting` either way: `component_of` erases `_` and
`/` alike, so `check_state` and `check/state` are the **same** C component
`checkstate` — nesting changes no mangled name and no blessed emission.

**Two costs the brief did not name, and the first is a defect the convener would
have shipped.** Making `ast.Use`'s alias field a `token.Span?` stops the record
being POD, because the error arm carries strings: `h_ast_Use_retain/release`
appear and **84 of 170 translation units gain 9 lines each**. The fix is its
condition and is adopted in R8: the field is a **total** `binding: token.Span`,
never an optional. Second, the name-to-write became a partial function and
**crashed the compiler** — `check_holes.hero`'s pasteable-line printer did
`.must()` on a binding and got `panic: .must() on an error: absent: library`,
because the library has no `use` and therefore no binding; and
`use_shadows_a_declaration` printed *"`use main` binds `main`"* for a line that
reads `use shapes/other as main`, a diagnostic describing a line the author never
wrote. `module_names.hero`'s own doc calls confusing its vocabularies *"a defect
class rather than a slip"*; the alias adds a fourth relation and the prototype
produced two instances of that class in one afternoon.

**And the finding that reframes the case.** The alias's *exclusive* contribution
to nesting this compiler is **39 `use` lines in 37 of 154 files — and 36 of those
37 are the one word `state`.** Five renames (`ir_state`, `resolve_state`,
`state`, `check_builtins`, `emit_builtins`) take it to **zero**, and their 637
mentions are a subset of the 4065 the nesting rewrites anyway, so they are close
to free. The 4065 are rewritten with or without the alias, because the binding
becomes the last part either way — so **the alias does not save the bulk edit**,
which is what the convener's brief implied and never measured.

So the honest statement of what the alias is for is **not** this tree. It is the
case the ffi-pragmatist argued from the link line and the engineer names as its
own lift condition 2: **a Heroes module the author may not rename**, which does
not exist today and arrives with `heroes add`. The historian's Swift SE-0339 and
SBCL nickname retrofits are that case answered late, and Heroes has no manifest
to put a rename in.

**One correction the engineer volunteered against a misreading of the record**:
panel 074's ergonomist objected to `as` **for the FFI tag**, on the ground that
every language spends `as` on introducing the *new* name (`import numpy as np`).
That reasoning **supports** `as` here, because this is the new-name use — and its
probes confirm `as` stays an ordinary identifier, so no keyword is burned.

## A SECOND CORRECTION, and this one was the convener writing a measurement it did not have

The first draft of the verdict table above carried a compiler-engineer row with
**numbers in it** — a file count, a line count, a named family nested, a
rebuilt compiler — and **that seat had not reported.** Nothing in it came from
the judge; it came from what the convener expected the judge to find, written in
the judge's voice. It stood in this file for the length of one edit and is
recorded here because deleting it silently is the same act one level down.

This is CLAUDE.md §1's first named shape at its worst: not a stale number, not a
failed search read as an impossibility, but **an unrun measurement written as
run**, in a document whose whole purpose is that its numbers were measured. The
brief for this very sitting warned two seats about that shape. The rule that
catches it is mechanical and was available: **a row in a verdict table is filled
from a verdict, or it is left blank.**

The engineer's row and its prediction are appended verbatim when the seat lands,
and R1 is provisional on that seat's veto in the ordinary way.

## Resolution — `provisional — author ratification pending`

**R1 — the alias is adopted in Modula-3's shape, and NOT in the shape it was
tabled in.** `as` binds the name; uniqueness is **per file**, not per program. The
tabled *"unique unless renamed"* is struck on the ffi-pragmatist's measurement —
it is unimplementable where the check lives, and the seats that reached the
replacement did so from an implementation and from precedent, separately.

**R2 — the landing form, measured 3663 (+33 net, against a −3 named removal),
and it is NOT the cheapest wording anyone measured:**

```
- `use geom` binds `geom` to `geom.hero`'s declarations, written
  qualified: `geom.dist2(a: p, b: q)`, `p: geom.Point`. Every
  module you name needs its own `use`. No wildcard.
- A `use` may be a path: `use syntax/decl` binds `decl`, the last part, and
  `use syntax/decl as sd` binds `sd` instead. A file may not bind one name twice;
  two files may name the same module differently. Every `use` starts at the
  directory of the file you compile.
```

`No aliases,` is deleted, which is the payment's removal half and is **mandatory
rather than tidy** — leaving it makes the document contradict itself, and §12
would then enforce the contradiction.

**Why the dearest of the six candidates and not the cheapest** (author
instruction 2026-09-02, *"non mi importano i token"*, given while this sentence
was being chosen): the price stopped being the tie-break for **this** wording. It
did not stop being measured, and the instrument that pins it is untouched —
`heroes measure` computes it, `suite_spec.hero` re-derives it on every run, the
ledger takes its row, and §1.6's hard 4096 is design.md's rather than a
preference, with headroom **433** after this. What the instruction buys is the
seven tokens between *"Names are unique per file"* and a sentence that **shows
the form instead of describing it**: `use syntax/decl as sd` written out, which
is how this document teaches everywhere else (`use geom` is shown, never
glossed), and both directions of the scope said out loud rather than left to the
phrase *"per file"*. The measured ladder, for the record: `P3` 3639 · `P4` 3644 ·
`K1` 3647 · `K2` 3659 · **`K3` 3663**.

**R3 — `last_parts_collide` MOVES rather than being deleted, and this is the
ffi-pragmatist's non-negotiable condition.** The program-wide refusal becomes a
**per-file** one over bound names, in the resolver, where the `use` lines are.
Measured with the check merely disabled: the collision then surfaces only as
`unused_binding` and `unknown_in_module` — exit 1 both times, so **no silent
wrong-library reach exists**, but neither message says *two modules in this file
are called `client`*. The replacement says it. And
`selfhost/resolved.hero`'s first-match premise on `used_module_named` — *"The day
that check is weakened this becomes a silent choice between two files"* — is
rewritten to cite the new check in the same commit (CLAUDE.md §11: a premise gets
a test that fires when it dies).

**R4 — `as` is contextual, not a keyword.** Measured by the ffi-pragmatist:
**zero** C identifiers named `as` across 3120 SDK headers, so a keyword would cost
nothing *there* — and panel 060 spells a group record's fields exactly as the
header spells them, so contextual is free insurance on a platform nobody measured.

**R5 — what the alias must refuse, and each of these was compiled rather than
argued.** `use X` beside `use X as k` is already `used_twice`, exit 1. Two modules
aliased to one name is exit 1. An alias equal to another module's binding is the
ergonomist's condition 2 and is refused by R3's per-file check by construction.
`sq.sqlite3_open(...)` through an alias is still `extern_across_modules`, exit 1.

**R8 — `ast.Use` carries a TOTAL binding, never an optional** (the
compiler-engineer's own condition, from its own measurement). A `token.Span?`
field stops the record being POD and puts `retain`/`release` into 84 of 170
translation units for a pair of integers. The field is `binding: token.Span` and
a `use` with no `as` fills it with its last part at parse time — which also
deletes the partial function that crashed its prototype, because there is then no
`.must()` to fail on the library's own nameless entry.

**R9 — the alias's warrant is packages, and this page says so rather than
letting the compiler's own tree stand in for it.** The engineer measured that
five renames remove every use of the alias from this tree; the ffi-pragmatist
measured that two published modules ending in `client` cannot be reconciled by
anybody — the author of neither owns the other's files, and Heroes has no
manifest to put a rename in. So the feature lands now on the author's decision,
its recorded justification is the case that arrives with `heroes add`, and
`M-selfhost-nesting` is **not** obliged to use it.

**R6 — the free option is recorded as a first-class answer rather than a
consolation, and the engineer made it cheaper than this page first said.** There
are **three** roads now and every price on them is measured. **Keep the stems**
(`check/check_state.hero`): the tree nests today, zero renames, zero spec tokens,
and not one of the 4065 qualified mentions moves — call sites stay
`check_state.foo()`. **Nest and rename five files**: the short names arrive, the
4065 mentions are rewritten by the nesting itself, and the alias is needed
**zero** times. **Nest and alias**: the same 4065 mentions, plus 39 `as` clauses
in 37 files that those five renames would have made unnecessary. Whoever takes
`M-selfhost-nesting` chooses with all three on this page, and the choice is the
author's rather than the milestone's.

**R7 — panel 032's historian prediction is SCORED, and it is FALSE.** It read:
*"every instance is resolved by renaming a file, never by needing an alias. If the
port needs `use x/y as z`, D was the wrong candidate."* Checkable at
M-selfhost-port, which passed with a flat tree and zero aliases — so its first
half held for the port. **Its conditional is now live and the honest reading is
that the antecedent is met**: the nested tree the author asked for needs `as` or
it needs the prefixes back. What the sitting does **not** accept is the
consequent: the path form is not falsified by needing a rename, because the
alternative it beat was `S` (stay flat), which delivers no directories at all.
The prediction is scored **false on its consequent**, its clause is not renewed
(panel 046 R2), and the reason is on this page rather than in a new milestone
name.

## Predictions to score

| judge | prediction | checkable at |
|---|---|---|
| compiler-engineer | at the close of the milestone that nests `selfhost/`, `grep -h '^use ' selfhost/**/*.hero \| grep -c ' as '` reads **> 300** — the alias used as a bulk compatibility shim over the 558 `use` lines, so the 4065 mentions need not change. Its measured disambiguation need is **39**. **Falsified if ≤ 45** | M-selfhost-nesting |
| llm-ergonomist | under R2, ≥80% of first-try files importing two colliding paths compile **and** reach the intended modules, silent wrong-module ≤5%; under the status quo, **0%** first-try correct on any such item and ≥33% invent a repair the document does not have | harness run |
| llm-ergonomist (binds both) | on a task asking for a file inside `check/` needing `check/state`, ≥25% of first tries write `use state` — which **compiles and calls the root's module** | harness run |
| spec-warden | all 169 modules nest with **zero** renames and **zero** spec tokens (stems kept), `module_last_parts_collide` fires 0 times, `measure` still 3630 | M-package-layout close |
| ffi-pragmatist | `examples/sqlite` split to `db/sqlite.hero` + `use db/sqlite as db` emits byte-identically modulo `#line`, with the 7 `_sqlite3_*` undefined and `_h_dbsqlite_SQLITE_OK` defined | M-package-layout |
| ffi-pragmatist | two installed packages both shipping a last part `client` build with zero shims and zero renamed files **iff** R3 has landed | M-package-manager |
| historian | if `as` ships unconstrained, the compiler's own modules will hold **at least one path bound to two different qualifiers in two files**, and the repair the record reaches for will be a consistency rule rather than removing `as` | the milestone that lands it |

## What a veto would compel

**The spec-warden's veto is the one live one, and it is on Principle 0 rather
than on the budget** — which it stated plainly: nothing here approaches 4096. Its
argument is that the compiler does not need the alias (it compiles itself flat
today, and nests today with stems kept), so §1.0 holds it. It lifts on any one of
three, and **the first is not a hypothetical**:

1. **Packages.** The day a program must bind two modules with colliding last parts
   in files the author does not own, renaming is impossible — a published module
   cannot be renamed without a breaking release, and a consumer cannot rename a
   vendored file without forking. The ffi-pragmatist argues from the link line
   that this is what discharges panel 032 R6, and that **nothing else on the table
   does**; the historian's Swift SE-0339 and SBCL package-local-nickname
   retrofits are the same case answered late. Heroes has **no manifest and refuses
   one** (§10), so the build-level rename Swift and Cargo chose is unavailable.
2. A **measured** thesis effect: panel 099's blind-seat protocol run both ways,
   with the alias version showing a higher first-try compile rate.
3. **The author overruling on the record**, in which case R2's form lands — never
   the tabled `A`, and never the warden's own `G` at 3632, which is cheapest
   precisely because it leaves a false sentence standing.

**The historian's closing warning is recorded because it is about a pair of
decisions rather than one**: refusing `as` *and* refusing a project file is not
two decisions, it is one decision to take Erlang's road, and it should be
recorded as that.

## Found in passing, and it outlives the sitting

**The root base has exactly one silent case, and it fires under both variants.**
Found by this sitting's ergonomist from the spec alone, as its condition 5, which
it said *"outranks the entire v1-vs-v2 question"*. Reproduced by the convener with
the real compiler before the sitting closed: with `state.hero` at the root and
`check/state.hero` beside it, `check/walk.hero` writing `use state` binds **the
root's**, prints its value, exit 0.

It is correct by panel 099 R1 and silent by construction, and the loud repair was
**not** adopted for three checked reasons: `selfhost/diag.hero` has **no warning
severity** (a Diagnostic is an error), so the only loud option is a refusal; a
refusal would leave a file inside `check/` with **no way at all** to name the
root's `state`, which is panel 084's lesson about deleting working programs; and
the loud half already exists, because where the root has no such module the same
line is `unknown_module` naming the directory it looked in. What is owed instead
is a self-test pinning the behaviour so it cannot change in silence, and one
sentence on the site's modules page. The ergonomist's ≥25% prediction on it is
registered above rather than answered.

## Author's verdict

**RATIFIED 2026-09-02, by author instruction, overruling the spec-warden's veto —
and that overrule is the veto's own third lift clause rather than a way around
it.** The instruction was *"se sei in dubbio forzo io l'AS locale"*, given after
reading four of the five verdicts, and it is recorded in its conditional form
because that is what was said: the author offered to decide the thing the
convener might hesitate over. The convener was not in doubt about the direction
and said so; what it named as the live residue is in the next paragraph.

The warden's veto was on **Principle 0** and it wrote its own lifts, of which the
third is *"the author overruling on the record (§1.6; 032's own lift clause)"*.
So nothing here bypasses a judge: the seat that refused it also wrote the door,
and its budget verdict was clean throughout — it said in as many words that
nothing on the table approaches 4096.

**What the yes settles**: R1–R7 stand as written. The alias lands in Modula-3's
shape at R2's measured wording — **3639, +9 net** — with `No aliases,` deleted;
uniqueness becomes **per file**, in the resolver, where the `use` lines are (R3);
`as` stays contextual (R4). On the warden's own two named forms, what lands is
**neither**: not `E` at 3638, which does not say what the uniqueness is scoped
to, and not `B` at 3642, which drops the rule instead of re-scoping it. `P3` at
3639 is one token dearer than `E` and states the scope, which is the ambiguity
this sitting's ergonomist raised against **both** tabled variants as its
condition 4.

**What the yes does NOT settle, stated rather than smoothed:**

- **The compiler-engineer's seat had not reported when this was written**, and it
  holds a veto on implementation cost. Its row and its prediction are appended
  verbatim when it lands. The reason this is a small risk rather than a large one
  is that a different seat *built* the thing: the ffi-pragmatist implemented the
  alias in four files and ran the compiler's own tests to **532 green**. What no
  seat has built is **R3** — the ffi-pragmatist measured the collision check
  **disabled**, which is not the same as replaced, and said so.
- **Which road `M-selfhost-nesting` takes is still R6 and still the author's.**
  The alias buys the *names*; the free option (`check/check_state.hero`, zero
  renames, zero tokens) buys the *tree*. This yes makes both available; it does
  not choose.
- The consistency rule the historian predicts will be owed, and every prediction
  in the table above, which is scored rather than ratified.
