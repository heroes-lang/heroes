
# Defect 078 closed: `owned` on a cell the header spells `const char **` is refused on the author's line

2026-09-24, M-agreed-retention step 9, in lane `cc295158`, merged `f7a2161a`,
landing panel 176's resolution item 8 on the sitting's provisional default.
Found by panel 175's spec-warden (its F4), reproduced by the coordinator before
filing; the repair is panel 176's compiler-engineer's prototype for its
question 4, option 2, which the sitting's critic measured holding on three
typedef spellings.

- [x] **078 — `owned` on an out-parameter the header spells `const char **` stops the build with `internal error`, exit 2, instead of an `ffi_` diagnostic** | the author's binding disagrees with the header on one qualifier, and the compiler reports itself as broken | **closed 2026-09-24**, M-agreed-retention step 9 (`cc295158`, merged `f7a2161a`) | `selfhost/emit/` · `.claude/rules/c-boundary.md`

    **Origin:** panel 175's spec-warden, 2026-09-23 (its F4), reproduced by the
    coordinator the same day before filing.

    **The reproducer**, over `r1.h`'s `fill_out(const char **out)` and
    `free_out(const char *p)`:

        extern "r1.h"
            function fill_out(@out: cstr owned free_out)
            function free_out(p: cstr)

        function main()
            s: str? @ fail(code: "none", msg: "nothing yet")
            fill_out(out: @s)
            print("after")

    `check` 0; `build -O0` exit **2**, `internal error: compiling the generated
    C failed`, and inside it clang's own sentence, *passing 'char **' to
    parameter of type 'const char **' discards qualifiers in nested pointer
    types*, pointing at `hero_ffi_probe_h_s4cstrowned_fill_out`. The same cell
    without `owned` builds (panel 172's `b_out`).

    **Why it is a defect.** `.claude/rules/c-boundary.md` names the one class of
    clang failure that is the author's and not the compiler's, the author's own
    `extern`, and this is that class reported as the other. design.md §4.17 asks
    for a diagnostic that says what to change without opening another file.

    **Linux, run by the coordinator:** the same `internal error`, exit 2, on
    arm64 and x86-64. **Unrun:** Windows; whether a `const char **` cell can be
    `owned` at all, which is the question the diagnostic has to answer.

## The repair

`emit/ffi_mutable.hero`: the `char **` carve-out stepped aside for every
`owned` cell, so a header that spells the cell `const char **` reached clang,
whose `discards qualifiers in nested pointer types` no reader knew, and the
compiler reported itself as broken at exit 2. The carve-out now steps aside
only for a header that says `char **`, and for `const char **` reports
`ffi_owned_const_cell` on the declaration: the header's const says C lends the
bytes, the mark says C gives them, and only the author can say which is true,
so the note offers both repairs and no fix is `certain`. The type compared is
the one clang prints, which is the canonical spelling, so `cstring_t *` and
`const gchar **` arrive as `const char **`.

## The measurements

| program | before | after |
|---|---|---|
| `@out: cstr owned free_out` over `const char **` (the filed shape) | exit 2, `internal error` | exit 1, `ffi_owned_const_cell` |
| the same over `typedef const char *cstring_t; cstring_t *`, `const gchar **`, `char const **` (the critic's three) | exit 2 | exit 1, `ffi_owned_const_cell` |
| `@out: cstr owned release` over `char **` (the carve-out) | builds | builds |

Two goldens in `tests/golden/fixedbugs/`, the plain spelling and the typedef,
each with its diagnostic annotated in the source.

Lane gate, with the compiler rebuilt from the edited source: annotations
174/0, check 135/0, unsupported 15/0, emission 506/0, run 145/0, corpus 55/0,
canonical 2/0, layout 2/0, order 3/0, records 24/0, the net's own tests
167/167, the compiler's 676/676. The seed is regenerated and the fixpoint
holds byte for byte. On the merged trunk, compiler rebuilt from the merged
seed: annotations 174/0, check 135/0, emission 508/0, canonical 2/0, records
24/0, the compiler's tests 676/676.

**Linux arm64, Linux x86-64 and Windows**, each with the compiler built from
the regenerated seed: both goldens refused with `ffi_owned_const_cell` at exit
1, and the owned `char **` control (`ffi-owned-cell-is-freed`) runs `all is
well`, three of three.
