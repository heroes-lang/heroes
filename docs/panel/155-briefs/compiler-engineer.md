# Panel 155 — compiler-engineer brief

Read `docs/panel/155-briefs/00-shared.md` first. It carries the question, the
measurements, and the finding.

You judge the ceiling: design.md §1.1, §1.7, Part 5. Implementation cost, and
core-vs-sugar. You have a veto on soundness.

## Pointers into the live compiler

All paths are `selfhost/` — **never `archive/bootstrap-rs/`**.

| what | where |
|---|---|
| the call site that binds a generic and records the instantiation | `selfhost/check/walk.hero:1660-1803`, and `c.out.instantiations[span.end] @ resolved_args` at `:1802` |
| the existing walk over every instantiation | `selfhost/check/walk.hero:2225` (inside a test, but it is the shape) |
| the binding algorithm | `selfhost/check/generics.hero`, `bind` and `substitute`, 413 lines |
| the checker's state record | `selfhost/check/state.hero:86`, `instantiations: {i64: [i64]}` |
| the map-key rule and its give-up arm | `selfhost/check/map_keys.hero`, `refuse` at `:77`, `reaches_float` at `:118`, and the type-parameter give-up |
| the `partial` rule and its give-up arm | `selfhost/check/partial.hero:84`, where `.generic` sits in the give-up arm |
| the closed sortable rule, for the shape that worked | `selfhost/check/ordering.hero:45-48` |
| monomorphisation, which reads the same instantiations | `selfhost/ir/mono.hero:170`, `:298` |

## File sizes, measured 2026-09-16 by `wc -l`

`check/walk.hero` **2290**, `check/builtins.hero` 544, `check/table.hero` 539,
`check/generics.hero` 413, `check/ffi.hero` 384, `check/map_keys.hero` 225,
`check/partial.hero` (see the tree), `check/ordering.hero` 219.

**`check/walk.hero` is the constraint and the milestone file says so**: it
measures **1708 code lines against a DECIDED ceiling of exactly 1708** in the
unit `tests/harness/suite_layout.hero` counts, which is not `wc -l`. So a
~90-110 line pass has nowhere to go inside it without a ceiling raise, argued.
Verify that number yourself with the suite rather than trusting this line —
`./heroes run tests/harness/main.hero -- ./heroes layout`.

## What you are asked to measure, not to estimate

1. **The pass, prototyped far enough that the line count is real.** In a copy.
   Where does it live — a new module, or an existing one? What does the
   `(decl, param) -> obligations` store cost, and what does the fixpoint cost?
2. **Termination.** The brief asserts the fixpoint is finite because the
   `(decl, param)` set is. Check that a generic calling itself, and two generics
   calling each other, both terminate, and say what the bound actually is.
3. **The blast radius.** Run the full net against a prototype, or failing that
   name exactly which suites and which files would go red. The brief's own
   finding is that `tests/golden/run/abort-map-key-nan.hero` dies. Find the
   others — `examples/` holds 17 generics, and
   `.claude/rules/verification.md` carries the rule that **a change to what the
   checker REFUSES is judged by every golden tree, not by the `selfhost/**`
   row**, which cost six red checks at the last milestone that ignored it.
4. **Core or sugar** by §1.7's test, and whether the obligation store is a
   second answer to a question the checker already answers.
5. **The alternative nobody has costed**: is there a route that closes the
   check/build gap *without* refusing the call — for instance making `build`
   accept what `check` accepts, rather than making `check` refuse what `build`
   refuses? Say what that would cost and why it is or is not admissible. The
   milestone has only ever considered tightening `check`.

## The route the sitting has NOT been given, and you should look for it

Every document on this question assumes the fix is a refusal. CLAUDE.md § RUN IT
says a recommendation is a claim about the option **set**, and asks what would
have to be true for a route nobody listed to exist. Two that occur to the
coordinator and are handed to you unpriced rather than as recommendations: a
diagnostic that fires at the call **as a warning** rather than an error, if this
language has such a thing (check — it may not); and leaving the rules alone and
instead making the RUNTIME abort name the call site, which turns a silent wrong
answer into a located one at zero surface cost. Neither is endorsed here.

## Your verdict owes

A verdict per R1-R4, a section citation, a measured cost or delta, a falsifiable
prediction naming the instrument that would score it, and the condition under
which you would change your vote.

Write your report to `docs/panel/155-reports/compiler-engineer.md`.
