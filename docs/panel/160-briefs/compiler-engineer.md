# Panel 160 — brief for the compiler-engineer

Read `00-shared.md` first. Your seat is design.md §1.1 (implementation
simplicity sets the ceiling) and §1.7 (core plus elaboration). You have a veto.

## What only you can answer

**R3 is yours and nobody has measured it.** Build option A and option E in a
copy and run `./heroes check selfhost/main.hero` under each. If the compiler
applies `.is_err()`, `.must()`, `.default()` or `?` to a nested fallible anywhere
in its own source, the refusal breaks the bootstrap and the count of sites is the
price. Principle 0 says a form the compiler needs is a form the language keeps;
a refusal the compiler cannot survive is not a refusal.

## Where to build it

- `selfhost/check/builtins.hero`, `fallible_builtin` — three arms, each
  `.fallible o => … o.payload`. The question the rule adds is
  `table.get(c.out.types, o.payload)` matching `.fallible` again.
- `selfhost/check/access.hero`, `try_type` — the `?` reader, same shape.
- A constructor goes in `selfhost/flow_errors.hero` beside `not_fallible`.
  **`selfhost/check/walk.hero` is a ratified knot already above its ceiling and
  `layout` refuses growth there**; last night a 25-line function was moved out
  of it for exactly that reason.
- Format at the moment of writing: `./heroes fmt <file> --in-place`.

## What to measure, each with its command

1. **Line cost** of A and of E, in the unit `layout` counts.
2. **Corpus breakage**: `./heroes check` over every file in `examples/` and
   `tests/golden/run/` and `tests/golden/check/` under each option. The shared
   brief infers the 72 map-read sites in `examples/` are single-level; **run it**
   rather than trusting the inference.
3. **The `find` door**: of the 6 `find(` sites in `examples/`, how many are over
   a `[T?]`? Read the array's type at each.
4. **What the diagnostic can print.** `state.show` renders a nested fallible as
   `i64??`, which `selfhost/parse/type.hero` refuses to read back. Is a message
   that names an unspellable type acceptable here, or does the rule wait on
   panel 158 R1's `(i64?)?`? Say what the message would say under each.
5. **The fix.** Can the diagnostic carry a `certain` fix? A `match` skeleton is
   multi-line and changes control flow, so probably `guess`. Say which and why —
   `.claude/rules/diagnostics-and-goldens.md` § Errors are a deliverable.
6. **Timing**: `/usr/bin/time -p` on `./heroes check selfhost/main.hero` before
   and after, and read the ratio of `real` to `user + sys`.

## A route to look for

Is there a **fifth** reader — anything else in `selfhost/check/` that consumes
a `.fallible` and discards the payload's type the way `.is_err()` does? Last
night's audit for defect 051 found six one-level peelers and judged all six
correct; it did not ask which of them DISCARD. `grep -rn "\.fallible [a-z_]* *=>"
selfhost/check/` is the list.

## Report

`docs/panel/160-reports/compiler-engineer.md`, via `cat >` since your seat has no
Write tool. Verdict · section · cost · prediction · condition, as always.
