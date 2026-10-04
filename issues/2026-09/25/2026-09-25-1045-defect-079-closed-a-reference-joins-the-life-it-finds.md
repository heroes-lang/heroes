---
kind: defect
area: runtime
milestone: none
filed: 2026-09-23
commit: 517b8e25692a4cf0075c04fd306e5017fd428cd4
github: none
---

# Defect 079 closed: a reference joins the life it finds, and the program owes one more release

2026-09-25, M-agreed-retention step 11, in lane `9f813de2`, merged `62324531`:
panel 176's item 4 as its completeness critic corrected it, and panel 177's
item 8, on the sittings' provisional resolutions. Found by panel 175's
ffi-pragmatist attacking route A at the shape beside it.

- [x] **079 — a reference-counted C handle aborts a correct program whichever way its extra reference is declared** | the live set holds one life per address, so a second reference to one object has nowhere to live, and its correct release is reported as a release of something never taken | `runtime/parts/alloc.c:411` · `spec § 13` · **closed 2026-09-25**

    **Origin:** panel 175's ffi-pragmatist, 2026-09-23, attacking route A at the
    shape beside it; reproduced by the coordinator the same day before filing.

    **The reproducer**, the shape of `CFRetain`/`CFRelease`,
    `g_object_ref`/`g_object_unref`, `X509_up_ref`/`X509_free`: a C object whose
    `obj_unref` frees it when its count reaches zero.

        extern "rc.h"
            record Obj tag obj
            function obj_new() -> Obj acquires obj_unref
            function obj_ref(o: Obj) -> Obj acquires obj_unref
            function obj_unref(o: Obj consumes)

        function main()
            a = obj_new()
            b = obj_ref(o: a)
            obj_unref(o: a)
            obj_unref(o: b)
            print("both references given back")

    `check` 0; `run` **134**, 396 bytes, *1 C handle(s) given back that were
    never taken*, three of three on Darwin arm64, and the same under
    `--sanitize`. With `obj_ref(o: Obj) -> Obj borrows` it prints its line, C
    frees the object at count zero, and it still exits 134 with *2 C handle(s)
    given back that were never taken*. **There is no spelling that runs.**

    **Why it is a defect.** The program is correct C and correct Heroes; the
    runtime aborts it. The set's own comment at `alloc.c:411-414` says a second
    acquisition of a live address *is lost here*, which is right for C handing
    an address out again after a leak and wrong for a second reference. The
    seat's count per slot (`<scratchpad>/175-ffi-pragmatist/runtimeA2/`) runs
    the program at 0 and still stops `handle.hero`'s double release, measured by
    the seat and not yet by the coordinator.

    **Unrun by the coordinator:** Linux and Windows.

## The repair

`retains <set>` after a result or a parameter says the call adds a reference
to a handle. The live set counts references per address: a reference on a live
address is one more, the set KEEPS the releasers the acquiring mark named, and
the reference's own set must share a name with them, or the runtime refuses the
reference itself before the wrong release can run (`hero_handle_retained`,
panel 176's critic: the prototype REPLACED the set, so a wrong release through
a reference ran silent and the correct one aborted); on an address nothing
began, a `borrows` producer's, it begins a life owed to the mark. A release
takes one reference away and ends the life at zero, which is what C's own
refcount does. Both of C's shapes are spelled: the returned reference (json-c's
`json_object_get`, `retains` on the result) and the status-returning one
(OpenSSL's `X509_up_ref`, `retains` on the parameter with `when 1` on the
result, panel 177's item 6, so the reference is counted only when C says it
took one), and the reference handed out through an `@` cell. The runtime ABI
moves 23 → 24.

The same step lands `transfers <set>` (panel 176's item 2, panel 177's item 7:
a transfer must have a receiver that outlives the call, or `check` refuses it
as `transfer_without_receiver`; a transfer into a handle the call hands back is
not made when that handle is NULL) and the success clause `when <literal>`
(`success_without_end`, `when_on_void`, `success_value_type`,
`success_value_range`), one word per position in any order
(`one_mark_per_position`, `repeated_mark`), and the runtime's pending count of
the ends a call announces, which is defect 084's stop before C.

**The landing's own review.** Five finder seats over the lane's diff, two
refuters per finding (2026-09-24): the first shape of the landing lost the
reopened stream of `freopen` (the acquisition was told before the end), let
`SSL_set_bio(s, b, b)` reach C, never told the runtime about a discarded
result, registered a `retains` out-cell at the cell's address, emitted the
success literal as written (`1_000`, `0o17`, a bare u64 past `long long`), and
let two `transfers` parameters be each other's receiver. Each is a golden now.

## The spec

§ 13's mark paragraph gains the `transfers`, `retains` and `when` sentences
and the productions follow. Priced on the real instrument: 6344 → **6643**
vendored, 8443 → **8805** real (`claude-opus-5`, 2026-09-25), digest
`293f81d403914a7d`; ledger row 6643, paid on the author's word by two of panel
177's registered predictions and one registered in the row.

## The measurements

| program | before (trunk `fb0b7cb6`) | after |
|---|---|---|
| the filed reproducer, `obj_ref -> Obj acquires obj_unref` | run **134**, *1 C handle(s) given back that were never taken* (re-run on this Mac 2026-09-24) | the spelling is `retains obj_unref`, below |
| `run/handle-reference-joins-the-life-it-finds` (json-c's and OpenSSL's shapes in one) | no spelling ran | **0**, three releases as C expects |
| `run/handle-a-reference-through-an-out-cell`, `run/handle-a-discarded-reference-is-still-owed`, `run/handle-two-references-at-two-consuming-positions` | — | 0 |
| `run/fixedbugs-a-reference-that-names-another-releaser-is-refused`, `run/fixedbugs-a-reference-beside-an-end-in-one-call-is-checked-first` | — | 134 on the reference, before the wrong release |
| `run/handle-transfer-only-on-success-keeps-the-obligation`, `run/handle-end-only-on-success-keeps-the-connection-owed`, `run/handle-transfer-into-a-null-result-is-not-made`, `run/handle-success-value-at-the-width-of-the-result` | — | 0 |
| `run/fixedbugs-a-transfer-that-failed-and-nobody-ended-is-owed-at-exit`, `run/fixedbugs-a-discarded-acquisition-is-owed-at-exit` | — | 134 at exit, owed |
| `run/fixedbugs-a-transfer-into-a-value-that-ends-it-wrongly-is-stopped-before-c` (`BIO_new_fp` over `popen`), `run/fixedbugs-a-release-after-a-transfer-is-a-stray` | — | 134 before C |
| `check/transfer-without-receiver`, `check/success-clause-misuse` and the check goldens of the review's codes | — | annotated, `.expected` |

Lane gate: the compiler's 698 tests, 546 emissions blessed, the net 2214 passed
and 0 failed, the net's own 167. Seed regenerated at ABI 24, fixpoint held.
Linux ×2: 68 runs each leg, every exit and every line as expected, twice.
Windows: the same 34 runs, every line as expected, the aborts at the box's 127.
