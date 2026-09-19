# Panel 166 — brief for the compiler-engineer

**Read `docs/panel/166-briefs/00-shared.md` first.** Every figure in it was run
while it was written. Contradict it where you can run something that refutes it —
you did exactly that at panel 165 and corrected three of its numbers.

## Your question

Seven routes are on the table for two defects. **Price A, B, C and D**, and say
which of them the compiler can actually carry. Panel 165's resolution named a
repair that turned out not to be implementable because nothing declares which
argument is the extent; your seat is the one that can say that *before* a
resolution is written this time.

## Where to start, located while writing this brief

```
selfhost/check/lend_types.hero        f.ptr() is typed here; the extent never appears
selfhost/emit/field_lend.hero         the lend's rendering, and the copy bug it
                                      already carries a repair for
selfhost/emit/unread.hero             learned at M-readable-bytes that a ptr reads
                                      a PLACE and not its operand
selfhost/check/lending.hero           the POSITION rule: a lend stands only as an
                                      argument of a call
```

`selfhost/emit/gate.hero` is **at its `DECIDED` ceiling** — defect 064's repair
landed there this morning and had to be cut to two lines of comment to fit. If a
route needs a row there, say so and price the split or the raise; your panel 165
prediction about that file was scored **correct** and this is its sequel.

## Specific things to settle, each a question

1. **Route A — refuse `.ptr()` from an immutable binding.** Does the checker have
   the binding's mutability in hand at the point the lend is typed? If it does,
   this is cheap; if the lend is typed before the binding is resolved, say where.
   And measure the over-refusal: how many existing golden and example programs
   lend from a `=` binding and only READ?
2. **Route B — implement panel 164's resolution 2.** The shared brief measures
   that `p: @s.name` is `type_mismatch: expected ptr, found i8[8]` and
   `p: @s.name.ptr()` is `not_a_place`. What is the actual shape of the missing
   rule — a lend that is a place, or a field that types as `ptr` under `@`? Which
   of the two does `check/lending.hero`'s position rule already admit?
3. **Route C — the counted parameter.** A new clause on an `extern` parameter,
   naming a sibling. Price it in the three places a clause lands: the grammar,
   the checker, and `emit/extern_probe.hero`. And say whether the CHECK is
   possible at all — the field's extent is known, the argument's extent is known
   when it is a literal, but what happens when it is a variable?
4. **Route D — `len()` on a fixed field.** `builtins` is at its decided ceiling
   too, by panel 165's own account. Where would it go?

## What your verdict must carry

A verdict (approve / object / veto) per route, the design.md sections it rests
on, a line count per route with files named, and **one falsifiable prediction**.
You hold a veto on soundness.
