
# Defect 083 closed: an `@` cell holding a handle or a `ptr` asks the header for depth

2026-09-24, M-agreed-retention step 7, in lane `e3101e87`, merged `5a6f6066`.
Found by panel 176's llm-ergonomist reading the specification alone (its
finding 9, *"mark the parameter `@`" reads as the extern's own parameter*),
reproduced by the coordinator before filing.

- [x] **083 — an `@` cell holding a pointer is accepted against a header parameter that takes `void *`, and C is handed the address of the program's own cell** | C converts `void **` to `void *` in silence, and the width check an `@` cell gets exists for numbers only | **closed 2026-09-24**, M-agreed-retention step 7 (`e3101e87`, merged `5a6f6066`) | `selfhost/emit/extern_probe.hero:172` · `selfhost/cli/pointee.hero`

    **Origin:** panel 176's llm-ergonomist, 2026-09-23, reading nothing but the
    specification: *"mark the parameter `@`" reads as the extern's own
    parameter; on a `void *` handle that becomes `void **`, which C converts
    silently*. Reproduced by the coordinator the same day before filing.

    **The reproducers**, over `void eat(void *p)` that prints what it got and
    frees it, and `void wipe(void *p)` that writes eight zero bytes through it:

        extern "v.h"
            record Mem tag void
            function make() -> Mem acquires eat
            function eat(@p: Mem borrows)

        function main()
            m: Mem @ make()
            eat(@m)

    `check` 0, `build` 0; C prints `made 0x1056b9d70` then `eat got
    0x16af42478`, a stack address, and frees it: 134, three of three, Darwin
    arm64. The same over `record Box tag box` is `error[ffi_parameter_type]` at
    exit 1, because `box **` against `box *` is a type clang refuses. And
    `wipe(@p: ptr)` against `wipe(void *)`: `check` 0, `build` 0, **run 0**, C
    handed the cell and writing into it; `wipe(@n: i64)` against the same header
    is refused, `ffi_parameter_type`.

    **The shape it came from.** The checker asks `@p: Mem consumes` for a
    producer mark (`unmarked_handle_producer`, whose note offers `borrows`), and
    the grammar allows one of `consumes`, `acquires`, `borrows`, so following the
    note writes the shape above.

    **Why it is a defect.** design.md §4.19: a wrong binding is a compile error.
    The probe casts an opaque or numeric `@` argument to `void *` on purpose and
    says what the cast gives up is checked in `cli/pointee.hero` by width and
    sign, which exists for numeric cells; an opaque cell, a `ptr` or a `tag void`
    handle, has nothing that checks the header takes a pointer to a pointer.
    **Unrun:** Linux and Windows; a C function writing more than the cell's eight
    bytes, which would be stack corruption at exit 0.

## The repair

`cli/pointee.hero` asked the header about width and sign for a NUMERIC `@`
cell only; an opaque one — a handle, or a `ptr` — was asked nothing, and the
probe hands both to C as `(void *)` on purpose (panel 103), so `@p: Mem` over
`record Mem tag void` reached a header's `void *p` as `void **`, which C
converts without a word. Now an opaque `@` asks a different question, the
DEPTH of the header's parameter: it must point at a pointer. The unit
`cli/header_types.hero` writes carries one more `_Static_assert`, at the
author's line, with its own marker:

    _Static_assert(!__builtin_types_compatible_p(P, void)
                   && __builtin_classify_type(*(P *)0) == 5, "heroes-ffi-pointee-depth …");

C11 arithmetic on the header's own pointee `P`, never a table this compiler
carries: `__builtin_classify_type` reads 5 for every pointer type, 1 for an
integer, 8 for a real, 12 for a struct — measured on Apple clang 21 and Debian
clang 22 on arm64 and x86-64 — and the `void` guard fails first as the marker
line, since `*(void *)0` is only a warning. `emit/ffi_pointee.hero` reads the
line back onto the parameter as `ffi_parameter_type`, with a `guess` fix that
drops the `@`, because two bindings are correct and only the header's author
knows which. A `record` by value stays out: `@rec: Point` over `Point *p` is the
correct out-parameter for a struct, and depth one is right there.

## The measurements

| program | before (HEAD `7fe2cc48`) | after |
|---|---|---|
| `@p: Mem borrows` over `void eat(void *p)` (the filed shape) | check 0, build 0, **134** three of three, C freed a stack address | build 1, `ffi_parameter_type` at the parameter |
| `@p: ptr` over `void wipe(void *p)` | check 0, build 0, **run 0**, eight bytes written into the cell | build 1, `ffi_parameter_type` |
| `@out: Mem acquires eat` over `void **` and over `typedef void *handle_t; handle_t *` (the control, `tests/golden/run/ffi-out-cell-on-a-handle-the-header-writes-back`) | — | run 0, three of three: `0`, `both given back` |
| `@n: i64` over `void *` (the numeric shape, unchanged) | build 1 | build 1 |

Lane gate, with the compiler rebuilt from the edited source: annotations
174/0, check 135/0, unsupported 15/0, emission 502/0, run 144/0, corpus 55/0,
lines 145/0, warnings 205/0, determinism 174/0, canonical 2/0, layout 2/0,
order 3/0, records 24/0, the net's own tests 167/167, the compiler's 676/676
after one test learned the new ask (its name said the question was asked of a
numeric `@` only). The seed is regenerated and the fixpoint holds byte for
byte; the test edit leaves the emitted seed unchanged. On the merged trunk,
compiler rebuilt from the merged seed: annotations 174/0, check 135/0,
emission 504/0, grammar 9/0, canonical 2/0, records 24/0, the compiler's
tests 676/676.

**Linux arm64, Linux x86-64 and Windows**, each with the compiler built from
the regenerated seed: both refusals `ffi_parameter_type` with the depth
sentence, and the control `0`, `both given back`, three of three plain and
once under `--sanitize`.

The two refusals are `tests/golden/fixedbugs/ffi-out-cell-on-a-handle-the-header-takes-by-value`
and `…-on-a-ptr-…`, each with its diagnostic annotated in the source.
