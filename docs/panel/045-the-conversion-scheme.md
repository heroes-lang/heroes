# Panel 045 — `fit_<width>` becomes `to_<width>` (retro-record)

**Retro-record**, 2026-08-13. The decision is the author's, taken in
conversation; **no judges were convened, on the author's instruction**, and §4's
rule for this case is followed instead: record the real objections, do not stage
dissent. Convening would have cost four judges for a question the author asked
and answered in one line — *"non era meglio storicamente chiamarlo `to_u8`?"* —
and the evidence was already on the table from panels 042 and 044.

---

## The decision

`fit_i8` … `fit_u64` become `to_i8` … `to_u64`. `to_i64` absorbs the old
`f64` conversion. **`to_str` and `to_f64` keep their plain return; every
`to_<width>` returns `T?`.**

The rule, stated once and applying to conversions that do not exist yet:

> `to_<T>` is fallible exactly when converting **to** `T` can fail.

## Why the shipped name was wrong, and it is a rule this project already had

**Panel 017 renamed `.str()` to `to_str`** for a reason written into the compiler:

> *so the three conversions share one scheme and a model can derive the third
> from the two the spec already lists*

`fit_<width>` broke that. A model knowing `to_i64`, `to_f64` and `to_str` cannot
derive `fit_u8` — it has to be taught. **Panel 044's blind judge proved it
without being asked to**: writing the lexer task it reached for
`to_i64(text[i])` and annotated its own miss (*"second reach was `to_i64` —
wrong builtin"*). It went for the derivable name.

**The argument that had justified `fit_`, and why it does not survive.** Panel
042's Q2 chose distinct names because `to_i64` *aborted* while the width family
returned `T?` — two failure models under one prefix. But the `to_` family
**already had mixed behaviour**: `to_str` total, `to_f64` total, `to_i64`
aborting. The argument was weaker than it was sold as, and the assistant sold it.

## The consequence the rename forced, which was not part of the ask

`to_i64` had to serve two sources — `f64` and every integer width — and could
not carry two failure models without being the thing panel 042 refused. So it is
**fallible from both**: converting to an `i64` can fail whether the source is a
float above 2^63 or a `u64` above it.

That turned a runtime abort into a value. `hero_f64_to_int` used to call
`hero_panic`; the check is now `hero_f64_fits_int`, a **question** the emitter
asks before building an `ok` or a `fail`. The predicate itself is untouched, and
it is the reason its golden exists: `v >= -0x1p63 && v < 0x1p63`, written that
way because `v <= (double)INT64_MAX` accepts 2^63 (the cast rounds **up**) and
`v > -9223372036854775809.0` rejects `INT64_MIN` — and on arm64 an unchecked
cast does not trap but **saturates**, so that predicate is the only thing between
a caller and a silent `INT64_MAX`.

`tests/golden/run/abort-to-int-range.hero` is rewritten as
`to-int-range.hero` — its header records what it used to assert — and it gains a
case nobody had: **NaN**. `to_i64(0.0 / 0.0).is_err()` is `true`, because NaN
satisfies no comparison. It used to abort.

## The objection on the record, unstaged

**The cost is real and it is `to_i64`'s.** Five call sites that used to read
`to_i64(x)` now read `to_i64(x).must()`, and one of them is the spec's own
`print(to_i64(1.9))`. A conversion that aborted is now one that must be
unwrapped, and a reader who liked the old brevity has lost it. Against that: an
abort is an ending the program cannot read, and a `T?` is a value it can — and
the language now has **one rule for every conversion** where it had two.

## 128-bit integers, refused in the same sitting

The author asked why `i128`/`u128` were not in the eight. **Measured rather than
reasoned:**

- `__int128` is a **clang extension**, and CLAUDE.md §7 says C11.
- **C cannot express the literal**: `integer literal is too large to be
  represented in any integer type`. The emitter could not write a constant above
  2^63 without synthesising it from halves.
- **`inttypes.h` has no `PRId128`** — zero occurrences. The runtime could not
  print one without extracting digits by hand.

And §1.11 gives no pull the other way: **no C header declares a 128-bit integer
in a portable API**, which is exactly what justified all eight widths that do
exist. Recorded in design.md §4.3 with the falsifier CLAUDE.md §12 requires —
*a header this language must bind that declares one* — and the falsifier goes in
`tests/golden/` the day it appears.

## Measured

- spec **2963 → 2974**, +11. The clause stopped listing two families and states
  the rule instead, which is what the +11 buys and which covers conversions not
  yet written.
- 15 files renamed; 5 `to_i64` sites gained `.must()`; the built-in registry is
  35 entries.
- 533 tests green, clippy clean.
