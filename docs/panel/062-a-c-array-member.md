# 062 — A C array member

**Status**: `RATIFIED 2026-08-15` — the author decided entry before the sitting
(*"nel dubbio mettilo, è fondamentale chiamare il C, altrimenti non andiamo da
nessuna parte"*), so the ballot was the **shape**. Landed on the resolution below.
**Lane**: full, five judges, four of them compiling.

**The sitting produced a veto, and the veto was worth more than the question it
interrupted.** Four judges approved the form; the compiler-engineer vetoed the
**shape** and named three reachable paths that could not be emitted soundly — with
the whole harness green through all three. The worst was that **no field assertion
was emitted at all** for an array member, so a wrong width ran at exit 0. That is
the class this milestone exists to prevent, shipped by the milestone that prevents
it, and found because a judge was asked to attack rather than to agree.

## The verdict table

| judge | verdict | Q1 | rests on |
|---|---|---|---|
| **compiler-engineer** | **veto on the shape** | **C**, and R needs a `TokenKind` | §1.7, Part 5 |
| **ffi-pragmatist** | approve, 3 conditions | C | §1.11, §1.12, §4.19 |
| **spec-warden** | object — the wording, not the form | — | §1.6, §12 |
| **llm-ergonomist** | approve **C**, object to R | **C** | the documents alone |
| **historian** *(advisory)* | approve | corrects the ballot | Nim, Dart, Rust, Zig, Go, C#, Swift |

## Q1 — `i32[4]`, and three judges reached it separately

The ballot offered `[i32 4]` (+53), `[i32; 4]` (+54) and `i32[4]` (+52).

The **llm-ergonomist**, blind, wrote the same 48-line program from each and found
the spelling decides what a reader *reproduces without reading*. `[i32; 4]` is
Rust's, and it imports Rust's relation between `[T; N]` and `[T]` — coercion,
`len`, `[v; n]` — which is **the exact relation the sentence exists to deny**. It
wrote `scaleIn: [0.0; 2]`, `len(ev.params)` and `for p in ev.params` under that
document and deleted them on re-reading the prose, not the code: *"a reader who has
the sentence in view is fine, and a reader working from the type alone is not."*
Under `i32[4]` it tried none of them, and says the spelling stopped it rather than
its own caution.

The **compiler-engineer** priced the same conclusion mechanically: `i32[4]` and
`[i32 4]` need no new `TokenKind`, and `[i32; 4]` does — `grep -rc "Semicolon"`
returns **zero files**, so it costs a row in the token enum, a scan row, and a row
in the *"found X"* prose every `expected_*` diagnostic prints, whose own file says
*"these names are output surface: changing one churns every snapshot."*

The **historian** corrected the ballot: it credited `[T; N]` to *"Rust's and
Zig's"*, and **Zig's is `[4]T`** — structurally the `[i32 4]` option. `[i32; 4]`
has one backer, not two. And C# is the only precedent for the postfix order, which
it needed a `fixed` keyword to make legible against `T[] x` — a problem Heroes does
not have, because its dynamic array is `[T]` and the bracket is on the other side.

**The deciding property is transcription.** `float leftLensCenter[2]` becomes
`leftLensCenter: f32[2]` by deletion.

## The veto, and what it caught

Three reachable paths, all green in the harness:

1. **No field assertion at all.** `c_spelling` returns a name the caller suffixes
   with ` *`, and C's pointer-to-array declarator is `T (*)[N]`, never `T[N] *` —
   there is no string for which that composes. The arm answered `None`, and
   `leftLensCenter: f64[2]` over a header's `float[2]` ran at **exit 0 with the
   field unchecked**. It arrived, in the judge's words, *"as a `None` nobody
   chose"* — through the arm whose own comment says the next type to reach it
   should be a `None` somebody did. Closed as a **branch**, not a row.
2. **`hash` disagreeing with `==`.** `_eq` walked elementwise while `_hash` emitted
   `(hero_unreachable(), UINT64_C(0))`, and the only signal was
   `warning: unused variable 'v'` — **which disappears the moment the record has a
   second field.**
3. **The form outside a group**, where it parses, type-checks and can do nothing.
   All three judges who examined Q3 arrived at *refuse* independently.

And the trap it names is worse than illegal C: `a->data == b->data` on a
`uint8_t[16]` field is **legal, zero diagnostics under the twelve flags plus
`-Wextra`**, and is a *pointer* comparison — two identical structs answer `false`
while `eq(a, a)` answers `true`. Reflexivity standing in for equality, silently.

## §1.12 — and the toolchain cannot help

The **historian** established the fact that decides how this is emitted:
AddressSanitizer is documented **blind to an intra-object overflow in a C struct**
— `-fsanitize-address-field-padding` applies to *"a C++ non-standard-layout class
with a destructor"*, never to C, which is all this backend emits. The
**ffi-pragmatist** measured the rest: an unguarded `arr[5]` on a `float[4]` writes
the next field at **exit 0**, ASan silent, UBSan reporting and continuing; and
`-Warray-bounds` does not fire on `int64_t i = 7; arr[i]` at `-O0` **or** `-O2`,
while this emitter always writes a temporary — *"clang provably cannot be the net
for the form this emitter emits."*

So the check is the compiler's: a **constant** index out of range is a compile
error (C# 12's CS9166 is the same decision, and Rust's `unconditional_panic` is
deny-by-default), and a **computed** one carries a guard measured at a **0.94–0.95**
ratio over four million opaque indices — under the noise floor. The subscript stays
on an **array-typed lvalue**, which is the one thing that keeps UBSan's
`array-bounds` able to see it at all.

## Construction — the question with no clean precedent

Nim and Rust both gave the **bare bracket literal to the fixed type** and a sigil
to the heap one (`@[…]`, `vec![…]`). Heroes has already spent the bare literal on
the opposite side, so it can copy neither. **Swift 6.2's `InlineArray` is the only
shipped precedent for sharing**, disambiguating by expected type — which is what
landed: a literal takes the fixed type when the length agrees, and a mismatch names
**both numbers**, because a miscount is invisible in a type name.

The lowering is the part C dictates: no `ValueId` of fixed type may ever be
assigned, because `float t[4]; t = …;` compiles nowhere. `Ty::Fixed` therefore has
**no storage** — the second instance of `ctype.rs`'s unit rule — and the elements
are rendered where they are used.

## Q3 — refused outside a group, and the precedent says the confinement holds

Dart's `Array` is field-of-`Struct`-only, annotation required, **no constructor**,
shipped 2021-05-19 and never widened — extended twice instead. C# `fixed` buffers
were confined for twenty years and then **superseded rather than lifted**: C# 12
added a second, safe form and left the restricted one alone. The historian's
warning is the honest one: *"the cost of confining is a future migration, not a
permanent explanation burden."*

## The counter-precedent, and it is the argument for the whole feature

**Swift refused fixed C arrays and is unwinding it this year.** It imports them as
tuples, and its own documentation concedes the mapping is *"widely recognized as
being far from optimal"* — Swift tuples cannot be indexed by a value known only at
run time. What it told users to do instead was **unchecked pointer arithmetic**:
`withUnsafeMutablePointer(&myTuple) { … }`. SE-0453 and SE-0483 landed
`InlineArray` in Swift 6.2, and a July 2026 pitch imports C arrays as one.

**The refusal cost more memory safety than the feature would have** — and Heroes'
`partial` is structurally Swift's tuple: it keeps the struct bindable by removing
operations, and five raylib structs were paying `==` and `hash` for it. Same trade,
same place, and the language that took it is walking it back.

## The spec — the ballot's own sentence was false

The **spec-warden** found that two of its three clauses were false **against the
compiler being written under it**: it promised indexing, `len` and `for`, and
`c.params[0]` was `not_indexable`, `len` was `bad_operand`, `for` was
`not_iterable`. *"The sitting that repaired the seventh false sentence would write
the eighth, at 2.6× the price of a true one."*

What landed is its wording — **+20 against the ballot's +52** — because
*"never a `[T]`"* forbids all four in five tokens and stays true once indexing
lands: a claim about **representation** rather than a list of operations, which is
panel 048's floor-not-identity one type later.

And it found the **eighth** false sentence, paid as the removal: `spec:204`'s
*"Both are names, never paths"* while `extern "sub/bar.h"` runs at exit 0 **and
`machine_locked_path`'s own note recommends the relative form** — the spec forbade
what the compiler advises. **−6.**

It also ruled against a § Types row: `i32[4]` outside a group is inert, and *"a
table row advertises a type with zero constructors and zero operations"*.

## Two pre-existing defects the sitting found on the way

- **`==` on a *complete* group record holding a `ptr` or `cstr` was exit 134** —
  *"this is a compiler bug, please report it"* — because both fell to the loud arm.
  `spec:72` has said they compare as addresses since panel 053. `Font`, this
  milestone's own golden witness, is in that class and nothing compared it.
- **`nan == nan` is false and `P(a: nan) == P(a: nan)` is false at exit 0**, while
  `spec:71` says *"structural equality on every value"*. Reported, not spent.

## The result

**raylib: all 35 structs, all 349 by-value crossings, complete field lists, zero
`partial`.** `VrStereoConfig` — 304 bytes, eight array members, two of them
`Matrix[2]` — round-trips through `LoadVrStereoConfig`/`UnloadVrStereoConfig`
against the real library. §4.19's ladder rung 5 needs no shim.

## Predictions to score

| judge | prediction | milestone |
|---|---|---|
| ffi-pragmatist | all **349** crossings bind with a complete list and zero `partial`; §4.19's *"write a thin C shim"* becomes reachable-but-unused for raylib. Falsifier: any entry point still needing `partial` or a `.c` | M-ffi-ladder |
| compiler-engineer | a `run/` case with `xs: [i64[4]] @ []` exits **134**, not 1, unless `gate_types::check_element` gains a row; and `structural.rs` is either over 300 or still missing `hash_body`'s row | M-struct-passing close |
| historian | a computed out-of-range write is reported by UBSan's `array-bounds` **iff** the emitted C keeps the subscript on an array-typed lvalue; lower it to pointer arithmetic and `--sanitize` reports clean while the neighbouring field is corrupted | M-ffi-ladder |
| spec-warden | ship `3374`; at M-ffi-ladder `grep -c partial examples/raylib/main.hero` is **0** (was 5) | M-ffi-ladder |
| llm-ergonomist | n=20, header transcription with an array-of-record and a 2-D member: `i32[4]` ≥18/20 correct and **0/20 transposed**; `[i32 4]` ≤12/20; `[i32; 4]` ≥6/20 emit a `[T]`-confusion | when metric 2 exists |
