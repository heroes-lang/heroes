# 028 — M-selfhost-nesting: the prefixes were load-bearing

## Goal

`selfhost/`'s 170 flat modules move into directories by subsystem, by author
instruction of 2026-09-02: *"riscrivere il selfhost con le cartelle al posto dei
prefissi, esempio: check/access o cli/argv"*. Panels 099, 100 and 101 had
already ruled the language this needs — a `use` may be a path, it binds the
path's last part, a file may rename what it binds, and every path starts at the
directory of the file you compile — so nothing about the language was open. The
work was meant to be mechanical: 134 files move, every `use` line and every
qualified mention follows, the seed is regenerated and the fixpoint re-verified.

Panel 100 R6 had priced three roads and left the choice to the author. The
instruction picks one by its own example: `check/access`, not
`check/check_access`. So the road is short names, and the free option — keep the
stems, move nothing else — was excluded by the sentence that opened the
milestone.

## What surprised

**The prefixes were doing a second job nobody had written down.** They were not
only organising the tree; they were keeping module qualifiers out of the
namespace where values live. Nobody names a local `emit_decls`. Everybody names
one `decls`. Drop the prefix and the two namespaces meet, and they met in 23
places on the first attempt.

The measurement is the pair: **0** live collisions on the flat tree, **23** the
moment it nests, the same code both times. That generalises past this compiler —
a naming convention can be enforcing an invariant by accident, and removing the
convention removes the invariant silently.

**Nesting changes no emitted C at all**, and the reason is a rule this project
wrote for another purpose. `h_<module>_<name>` sanitises the module component to
`[A-Za-z0-9]`, so `check_state` and `check/state` are both `checkstate`. Measured
over the seed, **803,134 lines before and after**:

| non-`#line` lines that differ | 7018 |
|---|---|
| naming one of the four renamed modules' symbols | 6670 |
| anonymous-type hashes containing one | 288 |
| the three local renames (`h0_state`, `h7_pieces`, `top`) | 60 |
| **anything else** | **0** |

The assumption was available and the measurement is what makes it a fact — which
is also how panel 101's compiler-engineer got half a prediction right for its own
stated reason while the other half was false by 3429.

**A rule with no executor is not a rule, and FOUR of them surfaced in one
evening, in four different departments.** CLAUDE.md §3 tells this story about the
`DECIDE.md` roll-forward: the rule existed in three documents, no skill performed
it, and the record went untouched for eight days. The same shape, four times:

1. `spec:99` plus *`use` **binds*** already forbade a local taking a module's
   name. Panel 031 R3 chose that verb for exactly that coverage, ratified, at
   `DESIGN-LOG:185`. The compiler never delivered the check, **two of its own
   comments asserted that it had** — including
   `resolve/qualified.hero:96-98`, which states the missing check as an
   accomplished fact — and **a test asserted the defect as correct**.
2. CLAUDE.md §8 says CI asserts every applied `certain` fix compiles.
   `needs_qualifying`'s does not: on a shadowed handle it prepends the qualifier
   the line already has, `window.window.destroy`, then
   `window.window.window.destroy`, without terminating. It shipped because golden
   cases for `needs_qualifying` number **0**, and `.fixed` is single-file by
   design while every module diagnostic needs two files to provoke. The
   instrument is structurally blind to the class.
3. `heroes test tests/harness/main.hero` was red on `main` for **six commits**,
   over an assert whose own comment says *"Both numbers track the table above and
   move with it"* — and the commit that broke it **states both new numbers in
   prose, two screens above the assert**. `DESIGN-LOG` recorded the identical
   failure on 2026-08-31 and gave the lesson no caller: the command lived in
   `docs/ROADMAP.md`'s verify block and in neither CLAUDE.md § Commands nor
   `/step`'s close checklist.
4. A **second** dead number sat inside that same case unseen — `DECIDED.len() ==
   15` against 16 entries — because Heroes stops a case at its first failed
   assert. One stale number hid another.

The common half is not carelessness. In all four the rule is written, correct,
and cited; nothing is obliged to run it. What this milestone adds in each case is
the caller, not the rule.

**A grep of the record is only as wide as the vocabulary of whoever runs it.**
Panel 102 was convened on *"the record says nothing about this"*, after a grep
for `shadowed_binding` and `module_binding_taken`. Panel 031 had ruled it in
other words — `geom = 3`, *"a third namespace"* — and the spec-warden found the
entry in minutes. CLAUDE.md §1 already says a negative claim rests on the
searcher's vocabulary rather than on the world. It says it about searching the
world; it is just as true of searching the record, and that is now the fifth
entry in that section's lineage.

**A panel seat must be told what NOT to run.** The compiler-engineer was killed
by a stream watchdog at 600 s while running the net — the failure that cost panel
087 four of five seats. Its brief carried the cheap route and it took the long
one. Resumed with one instruction (report what is in hand, run nothing) it
delivered the sitting's three most useful measurements in two minutes. Offering a
short path is not the same as forbidding the long one.

## What broke and why

**23 downstream errors, one cause.** The first mechanical nesting produced 23
`needs_qualifying` diagnostics in 11 files — none at the `use` line, all where a
call went through a local that had quietly taken the module's name. The repair
was not to rename 23 things: it was to close the hole first, so the compiler
would report the class **at the binding** and report it completely. With the
fourth shadow stage in place it named **18 errors in 3 files**, and the same
three files a script had found — the compiler and the heuristic agreed, which is
the only reason the heuristic is worth mentioning.

**Refuse-and-return turned one mistake into five.** The obvious shape of the new
stage returns a failure, which leaves the name meaning the MODULE — and a module
is not a value, so `examples/json/parse.hero` printed one `shadowed_binding`, one
`unknown_name` and three `module_is_not_a_value`. The compiler-engineer measured
it and the landing form **reports and then binds**, which takes it to one and
leaves the test that had blessed the defect passing untouched. The other three
arms of `declare` may refuse, because the names they collide with are still
values.

**A cascading rewrite, twice.** The transform script replaced qualified mentions
per stem in a loop over one line, so its own output was eaten: `emit_builtins.`
became `builtins.` and then `inventory.`. One pass with a single alternation
fixed it. Then the careful version of the same mistake: a regex that excluded
`state.` to protect `state.Checked` also protected `state.out` and `state.done`,
which are the local's fields and were exactly what had to move. A rewrite whose
protection is *"not followed by a dot"* cannot tell a module from a record.

**`parse/use` cannot exist.** `use` is a keyword, so the last part of `parse/use`
is not a binding — `error[module_path_wants_a_part]`, and the diagnostic was
right. One of 133 tails collided with the keyword table, found by the compiler on
the first check rather than by reading the table first, which would have been
cheaper.

**The net was already red**, and finding that took precedence over everything
else: without a green baseline there is no way to tell a new break from an
inherited one.

**Eight net failures at the end, and every one was a debt rather than a
mystery** — the fixture failing because `./heroes` was still built from the old
seed, a blessed emission carrying a renamed local into the C, a verdict heading
one word off, `SPEC_TOKENS` and the ledger owed the +22, a ceiling the panel's
own repair had pushed past, and two files out of canonical form. Naming them all
before fixing any is what kept the count honest.

**One claim in `docs/work/SCHEDULED.md` was false and the measurement killed it
early**: it said all 301 blessed emissions in `tests/emission/` would change,
because the C component is the sanitised whole path. Zero of them hold a symbol
from a `selfhost/` module — they are emissions of `tests/golden/` programs. The
seed is the only generated C this milestone touches.

## Predictions, scored

| sitting | judge | prediction | scored |
|---|---|---|---|
| 100 | compiler-engineer | `as` count **> 300**, falsified at ≤ 45 | **FALSE at 41.** Its own measured disambiguation need of 39 was nearly exact; its behavioural claim — `as` as a bulk shim over 558 `use` lines — did not happen, because the alias went where the collision was |
| 100 | historian | at least one path bound to two different qualifiers in two files, and the repair reached for will be a consistency rule | **FALSE, 0 — and false by choice.** The hazard was real; what avoided it is a rule this milestone adopted deliberately: alias a module in every file that binds it, never only where the collision bites |
| 101 | compiler-engineer | the diff over `selfhost/` holds **0** changed lines that are neither `^use ` nor a comment | **FALSE by 3429** — 586 `use` lines, 55 comments, 3429 neither. Written for panel 100 R6's keep-the-stems road; the author's instruction chose the other one |
| 101 | compiler-engineer | `--emit-c` before/after differs in **0** non-`#line` lines | **TRUE for the nesting**, which is the half about the backend, and it held for the seat's own reason. The 7018 lines that differ are the four module renames, the three local renames, and 288 type hashes containing one |
| 101 | (return condition) | `as` in `selfhost/` above one line in five | **3.2%** (41 of 1267), far under the 20% bar — the refusal keeps its evidence |
| 102 | spec-warden | the repair needs exactly **1** rename across all 501 `.hero` files, **0** under `selfhost/` | **HOLDS.** `examples/json/parse.hero`'s `value` → `exponent`, and nothing else. `selfhost/`'s three renames are the nesting's, not the reservation's |

## What landed, and what carried forward

Two steps, one sitting, and the milestone's own defect list is longer than its
feature list — which is the honest shape of a mechanical change that turned out
not to be one.

**What is true now.** `selfhost/` is ten directories and 37 flat files; a
qualifier is a module's last part unless the file renamed it, and 41 of 1267
`use` lines do; a local may not take a name a `use` bound, and the compiler says
so at the binding with both repairs in the note. The spec stands at **3685**
tokens of a hard 4096, headroom 411. The seed is 803,134 lines and is what
today's source emits.

**Three findings left the milestone as items rather than repairs**, each in the
list that matches what it asks. `@` on an immutable by-value parameter is
accepted and corrupts the heap — `AddressSanitizer: heap-use-after-free`, and
`spec:90` already forbids the program — with the cause located to one hop:
`resolve/walk.hero`'s `write_root` has exactly one caller, the write statement,
so a call argument's `@` never reaches it. It gets its own step because CLAUDE.md
§1 says a repair is attacked at the shapes next to it, and here they are seven.
The `.fixed` instrument's blindness to every module diagnostic is a decision
about the harness, not a patch. And `heroes mutate` owes a fourteenth operator
that steals a module's name — the payment §1.6 would have preferred over this
milestone's registered prediction — held for M-corpus-coverage, because adding an
operator moves the denominator of the thesis's own score and that discontinuity
should be spent once, against the wider corpus.

**Next in the chain: M-corpus-coverage**, twenty-plus programs for `examples/`
ordered by a measured coverage gap, then **M-documentation-site**, which owes the
one page a reader of the site cannot currently learn from it: where a program's
files go.
