# Defect 063 closes, and the number is C's three ways

2026-09-19, M-declared-extents step 6. Found by panel 165's spec-warden and
ffi-pragmatist while pricing route 6, the write half by that sitting's
completeness critic; route C adopted by panel 166 in its C-emitted form and
ratified by the author, reading.

- [x] **063 — an overstated extent on `f.ptr()` corrupts memory at exit 0** | `f.ptr()` lends a field's address and the CALL states the extent, and nothing checks that number against the field: a larger one reads and WRITES past the field, silently | `selfhost/check/lend_types.hero`, `spec § 13`'s `f.ptr()` sentence

    **Origin:** panel 165, 2026-09-19, found independently by the spec-warden
    and the ffi-pragmatist while pricing route 6, and the write half by the
    completeness critic. Shipped at `ef7b013b` (defect 061's repair), so it is
    eight commits old.

    **Reproducer**, an `i8[8]` field and a C function told it is longer:

        s: Sl @ sl_make()                  # struct sl { char name[8]; int id; }
        print(to_str(s.id.to_i64().must()))       # 7
        sl_fill(p: s.name.ptr(), n: 64)
        print(to_str(s.id.to_i64().must()))       # 1094795585 == 0x41414141

    Read direction: `sum_n(p: t.name.ptr(), n: 4096)` returns a sum over 4096
    bytes, exit 0. Under `./heroes run --sanitize`, `AddressSanitizer:
    stack-buffer-overflow`, `This frame has 1 object(s): [32, 44) 'h0_t' <==
    Memory access at offset 44 overflows this variable`.

    **Why it is rank 3 and not a price.** design.md §1.12 says a Heroes program
    must not corrupt memory, and this corrupts a sibling field of a record the
    program owns, with every suite green. CLAUDE.md § Precedence puts that above
    elegance, token cost, ergonomics, compiler size and speed.

    **What is owed.** A refusal at the class, not at the witness: the extent
    argument is checked against the field's own extent wherever it is a
    compile-time constant, and the shapes beside it are attacked — a literal, a
    header `constant`, an expression, a value read at run time, `[]` and a
    nested field. The ffi-pragmatist's cheaper half is worth pricing first:
    `t.name.len()` as a compile-time constant, so the honest call needs no
    literal at all.

    **CORRECTION, 2026-09-19, by the session that wrote the line above.** The
    first sentence is **not implementable as written**, measured by attempting
    it: **nothing declares which argument is the extent.** `sum_n(p: ptr, n:
    i64)` is two independent parameters and the compiler has no relation between
    them to check, so there is no *"the extent argument"* to find. What is
    actually owed is one of two things, and both are sittings: **route 13**,
    declaring on the parameter which sibling carries the extent, which is the
    only form where the number can be checked at all; or the `len()` widening,
    which removes the literal without checking anything and is a change to
    `spec § 11`'s built-in.

    **Closed by** `M-declared-extents step 6`, with the relation declared —
    which is what the correction above said was the only form where the number
    can be checked at all.

## The relation, and the three places the number is judged

`counted_by n` on a group's `ptr` parameter names the sibling that carries the
count. It is `owned`'s and `acquires`'s shape — a declaration-site mark the
checker reads and the lowering drops — and it is the spelling Apple's SDK and
the Linux kernel already use for the same relation. With it in hand the number
is judged three ways, and none of them is a width table this compiler carries:

| the extent the call states | what judges it | what the author sees |
|---|---|---|
| a constant: a literal, a group `constant`, arithmetic over them | `_Static_assert` in the emitted C, against `sizeof` of the field | `error[field_lend_extent]`, exit 1, on the number |
| read at run time | one compare before the call | `panic:` naming the file, the callee and the field's bytes, exit 134 |
| no mark on the parameter at all | `check`, before any C is written | `error[field_lend_uncounted]`, exit 1, on the lend |

**The first is right where a Heroes-side check is wrong twice, and both were
measured rather than argued.** A `partial` record's size is C's and not the
field list's: `sum_n(p: b.name.ptr(), n: 24)` on a `Both` Heroes knows one field
of is refused, `24` against C's `8`. And a group `constant` lowers to a CALL of
its accessor, so a check wanting a literal would refuse the careful reader and
accept the careless one — here `SL_NAME_LEN` reaches the assertion as a NAME and
C compares its own value: `(int64_t)(SL_NAME_LEN) <= (int64_t)sizeof(h0_s.name)`,
in the blessed emission.

## The half the resolution left open, and why it did not stay open

Panel 166 adopted route C's check *"only where the extent is a compile-time
constant"*, and its own compiler-engineer had made approval conditional on the
opposite: *"a variable extent is unknown at check time and must be refused, not
admitted. Admitting it silently is the defect wearing a mark."*

**Measured with the mark in place and no runtime half**: `sum_n(p:
s.name.ptr(), n: k)` with `k` a parameter read **4096 bytes of the stack at exit
0**, and so did `to_u64(64).must()`, a constant no folding here can see. That is
defect 063 surviving under its own repair, which §1.12 does not allow at rank 3.

**So the compare lands.** One `if` before the call, against the same `sizeof`,
and `hero_panic` — the shape `guard_arguments`' null-`cstr` guard has one
boundary over, with no new runtime entry point and no ABI change.

**What it costs, measured here rather than recalled.** Two binaries from the one
emitted C, `-O2`, `noinline` callee, 200 million lends, the machine still:

| | real | user |
|---|---|---|
| unguarded | 0.82 | 0.81 |
| guarded | 0.81 | 0.81 |

Indistinguishable at this resolution, and both print `1600000000`. The
ffi-pragmatist had priced it at 1% from its own bench; this run cannot see 1%.
**What conservative would have been** is the engineer's condition read literally
— refuse a non-constant extent at `check` — and it refuses `wrap(s: Sl, k: i64)`,
a count the program computes and gets right, which trades a corruption for an
over-refusal against §4.19's completeness clause.

## Attacked at the shapes beside it (CL-061)

Every row run on `arm64-apple-darwin25.6.0`, on the compiler built from the
regenerated seed:

| shape | verdict |
|---|---|
| `n: 8`, the field's own length | **runs**, exit 0 |
| `n: 0` | **runs**, exit 0 — zero bytes is a legal read |
| `n: 0 - 1` | **refused**, the guard's low half |
| `n: SL_NAME_LEN`, a group constant | **runs**, exit 0 |
| `n: SL_NAME_LEN - 4` | **runs**, exit 0 |
| `n: SL_NAME_LEN * 8` | **refused**, `states NAME_LEN * 8` |
| `n: k`, a parameter | **aborts**, exit 134 |
| `n: to_u64(64).must()`, an unsigned run-time value | **aborts**, exit 134 |
| two lends in one call, one honest one not | **refused**, at the dishonest one alone |
| two lends in one call, both honest | **runs**, exit 0 |
| inside a `while` | **refused**, once |
| a `partial` record's field | **refused**, `24` against C's `8` |

## What the mark itself is held to

Three facts about the declaration in hand, one code (`counted_by_shape`): the
marked parameter is a `ptr`, the name is another parameter of the same
signature, and that parameter is an integer C takes by value. `@n` is refused
as an extent — it is C's out-parameter and carries no count in — and so is
`counted_by p` naming its own parameter.

## Cases

- `tests/golden/run/ffi-a-lent-field-counted-by-a-constant.hero` — the legal
  four spellings of an honest extent, a `partial` record among them, and a
  run-time count that passes. Seven lines of output.
- `tests/golden/fixedbugs/ffi-a-lent-field-extent-overstated.hero` — the
  defect's own three shapes, each annotated, with a `surface` row asserting the
  sentence and that neither `internal error` nor `static assertion` reaches the
  author.
- `tests/golden/check/ffi-counted-by-shape.hero` — the four wrong marks and the
  uncounted landing, five annotations.
- The four goldens that already lent gained the mark, which is the migration
  panel 166's critic measured at four call sites in one file.

## What it did not cost

`examples/` is untouched: its four `ptr` externs are a callback, an opaque
context, an error slot and a destructor, and not one takes a counted buffer —
the ffi-pragmatist's prediction, scored below. No runtime entry point, no ABI
change, no new keyword the lexer reserves.
