# 018 — Struct passing: the layout that is not ours

The milestone that was scheduled against Principle 0's silence. The closure list
binds only `hero_os.h`, which declares no struct, so §1.0's compiler-need branch
was empty and stayed empty; this entered on design.md §1.12's completeness clause
— *any C library must be bindable, because one Heroes cannot reach is C code the
author keeps writing by hand* — which the author had ratified the day before.

## The goal

§4.19's ladder promises *"real struct passing"* at rung 5, in ratified text, and
the language could not keep it. **349 of raylib's 601 entry points pass or return
a struct by value**, and `ClearBackground(Color)` did not compile:
`error[ffi_type]: `Color` cannot cross the FFI boundary`. The wall was honest
rather than silent — nobody got a wrong program, they got a *cannot* — and it was
a third of the boundary.

The goal was to make a struct cross, and to make a wrong binding a compile error
on the author's own line rather than a plausible number.

Both halves happened, and the second one is the milestone. `ColorToInt` on opaque
red returns `-16776961`; `GetSplinePointLinear` returns a `Vector2` **by value**
and its fields read back exact. Zero lines of shim C.

## What surprised

**The mechanism is that there is no mechanism.** A `record` declared inside an
`extern` group gets **no typedef emitted at all**: the generated unit uses the
header's own `Color`, so size, padding, and — on arm64 — the register file each
field travels in are C's answers and never this compiler's. One suppressed line
is the whole of it, and the property it buys is the one that matters: *the emitter
cannot get a layout wrong that it never states.*

The alternative — declare a Heroes struct, assert `sizeof` and `offsetof` — was
vetoed on a measurement rather than a principle, and the measurement is worth
keeping because the obvious counterexample **does not work**. `{int32_t; float}`
against `{int32_t; int32_t}` passes correctly on both architectures: neither is a
homogeneous float aggregate, so both travel in the integer registers. The example
that kills it is `{float; float}` against `{int32_t; int32_t}` — which *is*
`Vector2` — where every assertion that mechanism can emit is green and the value
arrives as `(0.0, -1.13e35)`, because the callee reads `s0`/`s1` and the caller
wrote `x0`.

**Two things the sitting treated as concessions turned out to be free
consequences of the same property.** Because the layout is never ours, a partial
field list is ABI-safe to *read* (an `SDL_Event` with 1 of ~40 members named
round-trips through real SDL3), and an `f64` field over a header `float` is exact
in both directions. Neither was believed until it was compiled.

**A refusal is only as good as the question it asks.** The field probe asks type
identity — `_Generic` — and not size, because size cannot see `float` against
`int32_t`. And it asks it of the field's **address**, not its value, because
`_Generic` applies array-to-pointer decay to its controlling expression: the value
form binds a `float[4]` as an 8-byte `ptr` and says nothing. Two panels' judges
measured those two holes from opposite directions, in two different probe shapes,
and the fix for both is one `&`.

**The spec grew a sentence and lost it the same day.** *"A C `float` field is
`f64`, read exactly and written rounded"* was scaffolding for deferring `f32`.
`f32` landed by author decision, the probe asks identity, and the compiler began
refusing what that sentence permits — so §12 removed it. A spec sentence that
survives its own reason is how this document has acquired five false clauses.

## What broke and why

**Three catch-all arms, in one day, none of them a slip.** Each was written when
the case it now swallows did not exist, and each stayed correct until the day it
did not.

`printer/fmt_extern.rs` answered *"not in a group"* for a record, so `heroes fmt`
hoisted it to the top level and emitted **a different program that still parses**
— the worst shape a formatter's defect can take. Predicted by a judge and
reproduced before the prediction was read.

`emit/convert.rs` had an arm for `Ty::Float(FloatKind::F64)` while the checker had
been widened to either width, so an `f32` fell to `_ => return`, emitted **nothing**,
and left the destination `T?` at its zero-initialiser — `tag = 0`, which is `ok`.
`to_i64` on an `f32` returned `ok(0)` for every input at exit 0.
`-Werror=uninitialized` cannot see it, and that is not a gap: the zero-initialiser
is panel 021's deliberate rule, so the defence that made cleanup unconditional is
the one that hid this.

`emit/assert_spelling.rs` fell back to `void *`, so the result assertion passed
`(void *)0` where a `Color` was wanted. clang refused it **correctly** — and at
exit 2, which is the compiler blaming itself for a binding the author was entitled
to write, the exact failure CLAUDE.md §7's named exception exists to prevent.

The shape all three share is CLAUDE.md §11's, arriving through a door the rule
does not name: a `_` arm is not a premise *stated*, it is a premise *unstated*,
and the rename that turned `Ty::F64` into `Ty::Float(FloatKind)` produced **2**
rustc errors precisely because nineteen sites were already hiding inside one.

**A diagnostic that was confidently wrong.** The first field refusal printed
`str`'s note — reference count, bytes, `to_str` copies — underneath an `[i64]`
field. A wrong repair delivered with full confidence is worse than no note, so the
note is now chosen by the type: an array is told that a C struct holds a *pointer*
to its elements and never the elements.

**And a lie that compiled.** Before the field assertion existed, `r: i32` where
`raylib.h` says `unsigned char` compiled, ran, and printed the *right answer*. The
layout was the header's, so nothing was corrupt — but the declared type was false,
and `r @ 300` would have truncated to 44 at exit 0. It is the class
`-Wshorten-64-to-32` catches at a parameter and nothing caught at a field.
