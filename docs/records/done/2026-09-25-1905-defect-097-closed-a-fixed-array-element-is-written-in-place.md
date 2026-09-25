# Defect 097 closed: one element of a fixed array inside a group record is written in place, through the read's own bound check

2026-09-25, M-agreed-retention step 15, in lane `d64da8ff`, merged `dadba73b`.
Found by the skeptic seat over the landing of route M (must), panel 177's
item 3; reproduced by the coordinator on the trunk before that landing merged.

- [x] **097 — writing one element of a fixed array inside a group record passes `check` and aborts at run time as a compiler bug** | `n.a[0] @ 9` on a `record Nums tag nums` whose field is `a: i64[4]` is `check` 0, and the emitter's element write knows arrays and maps and not a fixed array, so it emits `hero_unreachable()` and the program dies with *entered unreachable code — this is a compiler bug* | `selfhost/emit/container.hero` (`write_element`, the `.index_step` arm) · `selfhost/emit/inst.hero:133` · **closed 2026-09-25**

    **Origin:** the skeptic seat over the landing of route M (must), panel 177's item 3,
    2026-09-25, probing a write that revives one element of a record's handle
    array; reproduced by the coordinator the same day on the trunk's compiler
    at `f1e2132f`, before any lane of that landing merged, so it is older than
    the landing.

    **The reproducer**, numbers only, no handle and no mark:

        extern "nums.h"
            record Nums tag nums
                a: i64[4]
            function nums_sum(n: Nums) -> i64

        function main()
            n: Nums @ Nums(a: [1, 2, 3, 4])
            n.a[0] @ 9
            print("sum: ", nums_sum(n: n))

    with `nums.h` declaring `typedef struct nums { int64_t a[4]; } nums;` and
    the sum. `check` 0; `run` **134** on Darwin arm64 and on Linux x86-64,
    *panic: entered unreachable code — this is a compiler bug*, and the
    emitted C carries `warning: variable 't7' set but not used`, which the
    `warnings` suite would refuse if a golden held the shape. Reading
    `n.a[0]` and `n.a[i]` works, and so does writing the whole record,
    `n @ Nums(a: [9, 2, 3, 4])`; the same element write over a handle array,
    `f.a[0] @ ob_new()`, dies the same way. A fixed array outside a group is
    refused at `check` (`fixed_outside_a_group`), so a group record is the
    only place the shape lives.

    **Why it is a defect.** A correct program is accepted and then killed by
    the compiler's own admission of a bug, at exit 134, on every leg measured.
    `write_element` reads `.fixed` as *not a container step* and fails, and
    the caller turns the failure into a run-time abort rather than a
    compile-time refusal, so the language promises a write it cannot perform.
    The repair writes the element in place with the index checked against the
    fixed length, as a read of it already is, and gives the shape its golden.

## The repair

A fixed array is its elements, inline in the record that holds it, so like a
field it is no indirection: `write_element` in `selfhost/emit/container.hero`
now writes the element where it lies, `h0_n.a[guarded index] = v`, at any
depth of the place, with nothing to unshare. The guarded subscript is one
function, `fixed_subscript`, which the read already went through, so a read
and a write of `n.a[i]` cannot disagree about the bound or the message: an
index past the length aborts with *index out of range for a fixed array*. The
old value is released where it is counted (`replaced`, one home for a field
and an element). And a failed element write can no longer reach run time:
`emit/inst.hero` spells it as an identifier clang refuses, so a place the
emitter cannot write is a build failure naming the compiler, never an abort
on a program the checker accepted.

## The measurements

| program | before (trunk `9e17d471`) | after |
|---|---|---|
| the reproducer, `n.a[0] @ 9` | run **134**, *entered unreachable code* (Darwin arm64, Linux x86-64) | 0, `sum: 18` |
| `run/fixedbugs-a-fixed-array-element-is-written-at-a-computed-index`, `-at-any-depth`, `-of-every-field-kind-is-written` | — | 0 |
| `run/fixedbugs-a-borrowed-handle-is-written-into-a-fixed-array`, `run/fixedbugs-an-acquired-handle-is-written-into-a-fixed-array` | the handle shape died the same way | 0, the acquired four released |
| `run/fixedbugs-an-at-argument-through-a-fixed-array-element-is-written-back` | — | 0 |
| `run/abort-a-fixed-array-element-written-past-its-length` | — | 134, the read's message |

Each plain and with `--sanitize`, the same exits. Lane gate: the compiler's
703 tests; run 173, determinism 203, warnings 234, lines 174, corpus 55,
canonical 2, check 146, annotations 186, records 24, emission 562, layout 2,
the net's own 167; the seed regenerated and the fixpoint held. Linux and
Windows: the run goldens travel with the next platform legs of this
milestone, which measure the merged tree.
