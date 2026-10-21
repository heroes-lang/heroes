# The trap families: what the control arm does with each thesis rule

For whoever writes a held-out task (`docs/metrics/writing-a-held-out-task.md`). Every held-out task
is a trap (decision 2172, `docs/metrics/tasks.md`): it tempts a mistake the strict arm refuses and
the control arm runs to a wrong output at exit 0 on a hidden case, never on case 0. This page lists,
from the compiler itself, the 27 rules the control arm drops (`selfhost/diag.hero`'s
`is_thesis_rule`) and what `build --permissive` does with each, read from its witness in
`tests/golden/control/` and that witness's `.expected` (panel 226's R1). **It holds no task and no
idea for one**: which program tempts which mistake is the author's (panel 011). And it decides
nothing: a mutant is a trap when `dry` grades it as its `# control: compiled_wrong case K` line says
(`tests/harness/thesis_mutants.hero`). Written 2026-10-21 (issue 2156).

## The control arm runs it with another meaning than the mistake's writer may have had

The families a trap can be made of: the control arm's meaning is fixed, and it differs from what a
writer who made the mistake plausibly meant.

| rule | the mistake, as its witness writes it | what the control arm does |
|---|---|---|
| `needs_label`, `missing_label`, `wrong_label` | `pair(b: 1, a: 2)` on `function pair(a: i64, b: i64)`, labels swapped; or `pair(1, 2)` with none | reads the arguments by position: `pair(b: 1, a: 2)` computes `pair(1, 2)` and prints `12` (`needs_label-labels`) |
| `shadowed_binding`, an immutable name | `x = 2` inside an `if` over `x = 1` | the inner name inside the block, the outer after it: prints `2`, then `1` (`shadowed_binding-immutable`) |
| `wildcard_on_variant` | `_ => "other"` in a `match` on a variant | a catch-all for every case no arm names: `.blue` prints `other` |
| `discarded_value` | `name()` on a line of its own | the value falls |
| `discarded_failure` | `_ = risky(0 - 1)` | the failure is dropped and the program goes on |
| `unused_binding` | `unused = 5` | nothing: the value is never used |
| `unreachable_statement` | `print(9)` after `return 1` | never runs |
| `marker_mismatch`, a value that is no place | `bump(1)` on `function bump(@n: i64)` | the value is held in a temporary the callee writes and nobody reads: the write falls |
| `hole_without_f` | `print("n is {n}")` | prints the braces as text: `n is {n}` |
| `spaced_minus_element` | `- 2` on its own line inside a list after `3` | a negative element, not a subtraction: the list holds `3` and `-2` |
| `builtin_name_taken`, at the top level | `function print(n: i64) -> i64` | the built-in answers the call, not the function |
| `declared_twice` | a second `function f` in one module | the first wins: prints `1` |

## The control arm ends it loudly

A `panic:` at exit 134 is the bucket `abort`, never `compiled_wrong`, so these are no trap in
decision 2172's shape; a hidden case reaching one is a loud failure in the control arm.

| rule | the mistake | what the control arm does |
|---|---|---|
| `non_exhaustive` | a `match` on a variant missing a case | prints what the arms it takes print, and aborts at the case no arm names: `panic: this match has no arm for .dot` |
| `endless_recursion` | a function that always calls itself | runs until the stack is exhausted: `panic: stack exhausted` |

## The control arm does what the writer most likely meant

Refused by the strict arm, run by the control arm as written; they cost the strict arm a turn and
the control arm nothing, so they cannot end silent.

| rule | the control arm's meaning |
|---|---|
| `cell_redeclared` | `v @= 2` on a declared cell is the write `v @ 2` |
| `shadowed_binding`, a cell | `v = 2` over a cell is the write `v @ 2` |
| `declared_place` | `p.x @= 2` on a field is the write `p.x @ 2` |
| `ufcs_on_mutable` | `v.bump()` with `bump`'s first parameter `@` lends `v`'s place, as `bump(@v)` does |
| `marker_mismatch`, an `@` missing or extra | the parameter's declaration decides |
| `never_rebound` | a cell nothing re-binds is a binding |
| `bound_unit` | `x = greet()` where `greet` answers `()` binds nothing; the call runs |
| `builtin_name_taken`, a local | the local answers inside its scope |
| `declaration_in_arm` | the arm's body is a block of one statement, its name visible nowhere after |

## Not in a held-out task, or not pinned

- `machine_locked_path`, `undefined_header_name`, `unshowable_name`, `package_comparison`: rules
  of the C boundary and of packages, and a held-out task has no `extern` and no `use`.
- `continuation_outside_brackets`: no witness in `tests/golden/control/`, so its control meaning
  is not pinned there; `dry` alone says what a mutant of it does.
