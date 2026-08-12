# 015 — M-sized-integers: eight widths, and `int` stops being a word

## Goal

`i8 i16 i32 i64 u8 u16 u32 u64`, first-class, with explicit conversions and
overflow aborting at every one. Part 7 item 10 had called this *"the highest-risk
addition in the language… never in v1"* and design.md:833 called its conversion
rules *"the most expensive spec item that exists and the number-one error source
in C"*. The author overturned the deferral on 2026-08-12, over panel 042's
two vetoes, and the row is retired rather than reinterpreted.

The warrant is §1.11's: the FFI is this language's standard library, and a
standard library that cannot say `size_t` has a hole in it. `strlen` could not be
bound by any spelling.

## What surprised

**Half the expensive thing was already paid, and the spec came in under the first
draft's estimate.** design.md's fear of conversion rules is a fear of C's
promotion lattice — the silent widening that decides what `a + b` means before
you have read either name. §4.3 forbids implicit conversions outright, so there
is no lattice to specify. What was left was eight names, a literal rule and a
conversion family: **+188 net**, against +189 measured for a first draft that
said three fewer rules.

**Representation decided whether a ruling was true.** Eight `Ty` variants against
one `Ty::Int(IntKind)` looks like taste, and the probe made it arithmetic: eight
produced **2** rustc errors and left **18** silent sites; one produced **20**. The
line that settles it is `emit/ops.rs`'s `operands == Ty::Int`, which chooses
whether to emit `__builtin_add_overflow`. Under eight variants a `u8` simply
makes it `false` — the guard is dropped, `t0 = l + r` is emitted bare, and
*"overflow aborts at every width"* is silently untrue at exit 0. Under one
variant it is a compile error. A ruling about runtime behaviour turned out to
depend on a choice of enum shape, which is not where anyone would look for it.

**The plan's order was wrong and the compiler said so in thirty seconds.** Widths
were step 4 and the runtime, FFI and ABI were step 7. They are one step: a width
that exists in the type table with no descriptor, no `_Generic` row and no
`to_str` is not emittable at all. The three arms made exhaustive at step 2 named
exactly those three gaps on the first build after the widths landed. The safety
net worked against its author, which is the only real test of one.

**A byte saying it is a byte made programs shorter, not longer.** `s[i] -> u8`
broke six programs, and three of the repairs were signatures:

```
function is_digit(c: i64) -> bool   ->   function is_digit(c: u8) -> bool
function digit_of(c: i64) -> i64?   ->   function digit_of(c: u8) -> i64?
function here(l: Lex) -> i64        ->   function here(l: Lex) -> u8
```

Three functions that took or returned bytes and declared 64-bit integers. The
precise type did not tax the programs; it made approximate signatures true. The
spec got **three tokens shorter** for the same reason: *"yields an `i64` in
0..255 (a byte)"* is a type, a range and a gloss, where *"yields a `u8`"* is a
type.

**And the deletion of `int` dissolved a veto instead of answering it.** Panel
042's compiler-engineer vetoed the transparent alias on §4.15 grounds, having
found that two type renderers meet inside one diagnostic — one mapping a `TyId`,
one echoing source bytes — so `expects i64, found int` was constructible with no
new code. The historian then showed the proposed cure was Go's reversed design.
Deleting the word made all of it moot: with no alias there is no second
spelling, `types/lower.rs` kept its 87 lines and its 17 context-free call sites,
and Go's ten-year-old bug stopped being relevant.

## What broke and why

**A silent truncation, inside the function that exists to prevent silent
truncations.** `fit_i8(-129)` was accepted. The range test was derived by hand
from `signed()` and `bits()`, and it emitted the low bound only where the
target's floor was zero — so a *signed narrower* target, whose floor is -128,
was checked at the top and nowhere else. Found by this milestone's own
adversarial case, which is precisely what §9's five per milestone are for: they
did not confirm the feature, they refuted it. The repair asks the two **ranges**
rather than the two shapes, so it cannot drift again.

**Three self-inflicted corruptions, all from blanket renames crossing a language
boundary.** `: int` → `: i64` turned two comments into `panic: i64eger overflow`.
A blanket `"int"` → `"i64"` rewrote the foreign-word registry's own key, so the
compiler rejected `i64` saying it is not a word in this language. And renaming
inside `tests/golden/` turned `_Generic((c), … int:1 …)` into `i64:1`, because
those files hold C. Each was caught by a test within minutes, which is the
argument for the goldens rather than against the method — but the pattern is
worth naming: **a mechanical rename is safe only inside one language, and this
repository holds four** (Heroes, Rust, C, and English prose).

**A rule I got wrong by reaching for the world instead of the value.** A first
draft of the foreign-word registry reserved `integer`, `long`, `short`, `byte`,
`uint`, `usize` and `isize` alongside `int`. `byte` is a plausible identifier — a
lexer writes `byte = s[i]` — and `integer` is an ordinary English word this
project's own specification uses in prose. That is CLAUDE.md §11's named failure
mode, committed in the same session that found three instances of it in the tree.
Only `int` is reserved.

**And the author's own ruling had to be bent, on a measurement.** Conversions
were to return `T?`, uniformly. Applied literally, `s[i] -> u8` put this on the
calculator's lexer:

```
v @ v * 10 + fit_i64(l.here() - '0').must()
```

A `.must()` on a conversion that cannot fail, at the hottest line of the shape
the closure list is made of. `fit_<w>` now returns `T` where the source width is
provably contained in the target. That bends the ruling and is marked pending
ratification — but it does not bend panel 042's Q2, which refused one name with
two result types on the ground of *unpredictability*. This is predictable: the
result is a function of the argument's width, and it is fallible exactly when it
can fail. The llm-ergonomist asked for it and was overruled for uniformity; the
measurement went its way.
