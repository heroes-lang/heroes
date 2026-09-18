# Panel 163 — shared brief: a struct with a long array field cannot be built

**Every number here was produced by a command run on 2026-09-18 while this brief
was being written, and the command is named beside it** (CL-077).

## The question

**How does a program obtain a value of a header record with a 256-element array
field, so it can hand it to a C out-parameter?**

## What already landed, so the sitting knows what it is NOT deciding

Panel 162 closed the READ direction hours ago: `f.validated_bytes()` answers a
`str?`, reading to its first zero or the whole field, failing `not_text`. It is
landed, measured and committed; `spec § 13` carries it at **+46 real**. This
sitting is the other half of that milestone and nothing about the read is open.

## The wall, measured

```
u: Utsname @ Utsname(sysname: [0])
error[fixed_array_length]: `i8[256]` holds exactly 256, and this literal has 1
    — a fixed array's length is part of its type, so the two must agree (§4.19)
```

**A full literal is 803 characters on one line**, measured by generating it.
That program builds and runs and prints `Darwin`, so this is a question about
what a reader can bear to write, not about what the compiler can do.

Two escapes are not escapes, both measured: `partial` does not help, because a
declared field's length is checked whether or not the record is marked; and a
`tag` with no fields is accepted and wrong, because `spec § 13` makes that a
HANDLE, C's pointer to the type, so `uname(@u)` would hand across a pointer to a
pointer.

## Why panel 162's answer did not survive implementation

That sitting resolved *widen `repeat(x, n)` to build a fixed array*, priced by
its spec-warden at **+18 real** as a merge into § 13, and chosen because a zero
default would contradict design.md §4.9's *no default values*.

**It cannot be implemented as written, and that was found by trying.**
`repeat(0, 256)` has to learn from its CONTEXT that the `0` is an `i8` and that
256 is the field's length — neither is in its arguments. In this language only
**literals** take their type from context: `selfhost/check/walk.hero` has
**6** such sites, all of them literal forms (`[]`, `{}`, a fixed-array literal
against an expected length). **A call typed by context does not exist here**,
and inventing one is a language change this sitting has to weigh rather than
assume.

The emitter half is the same story one level down. A fixed array has **no C
storage** — `selfhost/emit/ctype.hero` answers `C has no assignable array` on
purpose — so such a value is rendered at its use site by
`selfhost/emit/storageless.hero`, which today has **one** producer of a braced
literal and an explicit arm refusing a `.call` as *not a producer of a fixed
value*. That module's own doc warns why the shape matters: the subscript must
stay on an array-typed lvalue, because **ASan cannot see an intra-object
overflow in a C struct at all**, and UBSan's array-bounds check sees it only
while the C type is still `T[N]`.

## The scale, measured

- `grep -rn "i8\[\|u8\[" examples tests/golden --include='*.hero'` returns
  **9** lines in the whole repository. Every one of them is a short field.
- **Zero** programs in `examples/` bind a struct with a long array field, which
  is why nothing has met this wall before.
- The motivating struct is `struct utsname`, whose arrays are `char[65]` on
  Debian and `char[256]` on Darwin, measured on both — so **that struct needs a
  per-platform source file whatever this sitting decides**, and no resolution
  should be argued as making it portable.

## Routes named, and four is not a claim about the set (CL-057)

1. **A call typed by context**: `repeat(x, n)` against an expected `T[N]`, which
   is panel 162's resolution and needs the checking direction for a call plus a
   third producer in `storageless`.
2. **A zero default for a fixed field in an `extern` record**, which
   design.md §4.9's *no default values* refuses today — so the route is an
   amendment to §4.9 and must be argued as one.
3. **An out-parameter that does not require an initialised value**, which
   `spec § 5`'s *all bindings are initialised* refuses today. Same shape: an
   amendment, argued as one.
4. **Refuse, and say a long-array struct is taken from the library and never
   built.** Held to design.md Part 6's standard: a refusal names the program or
   compiler fact that would make it wrong. Note that `uname()` is exactly a
   function that FILLS a struct the caller owns, so this route owes an answer
   for it.

## Constraints every seat is held to

- **CLAUDE.md § Precedence**: robustness beats elegance, token cost, ergonomics,
  compiler size and speed. Most robust, never cheapest, never a compromise.
- **Principle 0**: the compiler does not need this (it self-hosts), so the
  proposal enters on a measured thesis argument or not at all. **Refusing is a
  live verdict here and not a formality.**
- **The budget, measured this session**: `./heroes measure spec/heroes-spec.md`
  reads **6032** vendored and **8030** real against a ceiling of **10240**.
- **A vendored delta is not a price.** Real, or the words *lower bound*.
- **Build in a copy**: `cp -r`, `rm -rf build`, then
  `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`, seconds. **Never
  rebuild from `selfhost/`.** **Never `archive/bootstrap-rs/`.**
- The working tree is frozen for this sitting.

## What the sitting must deliver

A resolution the coordinator can implement, or a refusal that closes the
question. **Panel 162's resolution was corrected three times at implementation —
it named a library function as a built-in, it assumed a name could be reserved
that a shipped feature depends on, and it priced this half as a widening that
cannot be written.** So this brief asks every seat for one extra thing: **say
what your route costs to IMPLEMENT, not only to state**, and name the file you
expect it to land in.
