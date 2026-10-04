- [x] **069 — a C function name passed as a callback builds and then aborts, blaming the compiler** | **closed 2026-09-20**, M-declared-extents step 12, on panel 169's resolution item 6 | **The repair is emission and not refusal, and it is the arm moving to a file of its own.** `emit/inst.hero`'s `.func_ref` arm wrote `hero_unreachable(); /* the gate refuses this form */` for an `extern_fn` callee and the emitter then emitted the CALL four lines later anyway, passing a temporary nothing had assigned — so `check` was 0, `build` was 0, and the program aborted at run time saying *"this is a compiler bug, please report it"*: a true sentence pointing at the wrong line, for a form the checker admits and `spec § 13` promises (*"a callback is a **parameter**, never a result"*, with `atexit(f: (function() -> ()))` as its own example). **A gate that refuses a form and then emits it is not a gate.** Spec beats compiler, CLAUDE.md § 12, so no sitting was convened for the repair itself | `selfhost/emit/func_ref.hero` · `tests/golden/run/fixedbugs-a-c-function-crosses-as-a-callback.hero` · `docs/panel/169-two-defects-two-classes-and-the-one-that-was-never-searched-where-it-happens.md` | 169

    **Origin:** panel 169's ffi-pragmatist, 2026-09-20, reproduced by that
    sitting's completeness critic and again by the coordinator before filing.
    Promoted from a diagnostics defect to **the milestone's blocking item** by
    the same sitting, because a destructor callback is the one mechanism every
    real C library offers for the retention problem defects 066 and 068 are
    about — `SQLITE_TRANSIENT`, `sqlite3_free`, `curl_easy_setopt`'s write
    callbacks — and Heroes could not pass one.

    ## The repair

    **`emit/func_ref.hero`, a new module**, because the arm could not grow where
    it stood: `emit/inst.hero` measured **350 of a `DECIDED` 350** at panel 169,
    one of four files the sitting found at zero headroom. The seam is not length:
    **naming a function is a question about the DECLARATION, while everything
    else in that file's dispatch is a question about the instruction's
    operands** — the same seam `emit/access.hero` sits on. `designator()` answers
    the name or fails, and the two cases that fail say why: a built-in is a shape
    the emitter expands rather than a symbol, and an `.indirect` is already a
    pointer in a temporary so its reference would be the address of an address,
    which `ir.hero`'s own comment refuses.

    **UNMANGLED, by design** (CLAUDE.md §7, and `emit/ops.hero`'s own
    `.extern_fn` arm says it for the call): the name in the `.hero` file IS the C
    function's name, and the `#include` is what declares it.

    ## The measurements that prove it

    On a compiler built from `selfhost/` with the repair in:

    | | before | after |
    |---|---|---|
    | `heroes check` | 0 | 0 |
    | `heroes build` | 0 | 0 |
    | `heroes run`, three times | **134 134 134**, `panic: entered unreachable code` | **0 0 0**, prints `7` then `1` |
    | `--sanitize` | — | **zero AddressSanitizer lines**, exit 0 |

    The emitted C is `t4 = counting_free;` where it was
    `hero_unreachable(); /* the gate refuses this form */`, and the call four
    lines later is unchanged.

    ## The shapes beside it, attacked rather than assumed

    | shape | result |
    |---|---|
    | a **mismatched** callback signature | **exit 1**, `error[type_mismatch]: expected (function(ptr) -> ()), found (function() -> i64)` — caught by **Heroes' own type system**, before clang sees it |
    | a **Heroes** function as the callback | unchanged, works, prints `1` |
    | a C function **bound to a name first**, then passed — the `.indirect` case | works, prints `1` |
    | the original reproducer, a `malloc`'d buffer with a printing destructor | works, prints *"C called the destructor"* |

    The first row is better than panel 169's engineer predicted: the seat
    measured that clang would refuse a mismatch with `incompatible function
    pointer types`, and the repair never reaches clang for that case because the
    checker already holds the function type.

    ## The gates

    `run` **130**, `determinism` **159**, `lines` **131**, `warnings` **190**,
    `canonical` **2**, `layout` **2**, `order` **3**, `emission` **470**, the
    compiler's own **673**, all 0 failed. `emission` needed one deliberate
    blessing, which is what that suite's own doc calls for on a **new** program,
    and the diff is one added file: **no existing emission changed.** The seed
    was regenerated in the same commit and the fixpoint verified
    **byte-identical**.
