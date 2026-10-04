---
kind: defect
area: emit
milestone: none
filed: 2026-09-23
commit: 7dc739251188afc0670eb51771b8350e376b1cfd
github: none
---

# Defect 088 closed: a handle handed to a call after the call that ended its life is refused at check through its name, and stopped before C through a copy

2026-09-25, M-agreed-retention step 14, panel 177's items 1 to 3 on their
provisional resolution, in the integration lane `a7388f1f`, merged `0e7a61be`.
Found by panel 176's completeness critic (its § 7).

- [x] **088 — a handle handed to a call after the call that ended its life is `check` 0 and `run` 0, and C is handed freed memory** | § 13 says a `consumes` call ends the value's life, and nothing reads the value as dead afterwards: the checker does not, and the live set is asked only by a consuming call | `spec § 13` · `selfhost/emit/handle_traffic.hero` · `runtime/parts/alloc.c` · **closed 2026-09-25**

    **Origin:** panel 176's completeness critic, 2026-09-23 (its § 7), which
    searched this file for `use.after`, `survive` and `after consum` and found
    nothing. Reproduced by the coordinator on 2026-09-24 before filing, from the
    critic's own files copied out of its directory.

    **The reproducer**, over a three-function cJSON in miniature (`CreateObject`,
    `AddItemToObject` that links `item` under `object`, `Delete` that frees a
    node and its children), declared with `acquires cJSON_Delete` on the
    creator, `item: Json consumes` on the adder and `item: Json consumes` on
    `Delete`:

        b = cJSON_CreateObject()
        cJSON_Delete(item: b)
        c = cJSON_CreateObject()
        _ = cJSON_AddItemToObject(object: b, string: "c".cstr(), item: c)
        print("wrote into a deleted object")

    `check` 0; **run 0, three of three, on Darwin arm64, Linux x86-64 and Linux
    arm64**, printing its line after C wrote into freed memory; with
    `--sanitize`, `heap-use-after-free` on all three (134 on Darwin, 1 on
    Linux). Windows unrun: the box was off.

    **The shapes beside it, and why they are not this defect.** The same dead
    handle given to a CONSUMING parameter is stopped before C on all three
    legs, 134 with the runtime's *given back that were never taken* — a true
    line, since the address left the set at the first release. And through a
    helper, `finish(@b)` releasing it and the caller reusing `b` as `item`, the
    same 134. So the silence is exactly where the handle reaches a parameter
    that does not consume: no check reads it there.

    **Why it is a defect.** design.md §1.12: a Heroes program must not corrupt
    memory, and this one does at exit 0 with no C in it but the library's own.
    It belongs with defect 077, the other shape where the live set cannot tell
    a handle that is over from one that is not, which is why panel 177 takes
    both.

    **Corrected 2026-09-24, before panel 177 sat: this shape is inside a class
    design.md already states, and the filing missed it.** Part 8 wart 20 — *one
    copy of a value holding a `ptr` can free what every other copy holds, at
    exit 0* — says the half `consumes` does not reach is *a copy made BEFORE
    the call still holds the freed address*, and that the class is the affine
    handle, which Part 6's borrow-checker row refuses on COST since 2026-09-13.
    `b` above is that copy. The critic searched this file and the coordinator
    searched nothing else; `grep -n "wart 20\|affine" docs/design/design.md`
    finds it, and panel 153 had declined to file a shape of the same class on
    that ground and said so. **It stays filed, and this is why**: the wart
    admits the class *only while the cheapest guard is being built rather than
    argued about*, and the live set that panel 150 added since gives this
    shape a guard the wart could not name — the dead handle is not in the set
    when it reaches a parameter that does not consume. Whether that guard is
    the cheapest, and what it does to a `borrows` handle that was never in the
    set, is panel 177's question. The author can disagree and close it on the
    wart, as panel 153 put it.

## The repair

The same three halves that close defect 077, and this defect is the one each
of them was measured against first. The checker's flow analysis refuses `b`
at the read after `cJSON_Delete(item: b)` ended it, whatever the parameter it
reaches: the silence was that a parameter which does not consume asked
nothing, and the read is now refused where it is written, before any
parameter is involved. Where the checker cannot follow the value, the poison
stops it at the crossing (a helper that ends its parameter, a field of a cell),
and the dead set stops a copy made before the end.

**The shape beside it that moved with it**: a transfer. Panel 176's historian
named *using the child after the transfer* as a question and neither sitting
ruled it; the landing decides it the robust way, and § 13 says so — *once
made, the program's own name for it has ended as if given back* — queued for
the author's ratification with panel 177
(`docs/records/log/2026-09-25-2222-a-transfer-that-was-made-ends-the-name.md`).

## The measurements

Measured by the coordinator on the trunk after the merge, `./heroes` rebuilt
from the merged seed, Darwin arm64, each plain and with `--sanitize` alike. The string parameter is
declared `lent`: without it the checker refuses `.cstr()` at that argument
first, `lend_kept`, and the read this defect is about is never reached.

| program | before (trunk, 2026-09-24) | after |
|---|---|---|
| the reproducer, `cJSON_AddItemToObject(object: b, …)` after `cJSON_Delete(item: b)` | `check` 0, run 0 on three legs, C writing into freed memory; `--sanitize` *heap-use-after-free* | `check` **1**, `handle_used_after_end` at 11:39 |
| the same through a copy `b2 = b` made before the end | — | `check` 0, run **134** before C: *a C handle reached the argument `object` of `cJSON_AddItemToObject` at an address a call marked `consumes` or `transfers` ended* |
| `check/fixedbugs-a-handle-read-after-the-call-that-ended-it` | — | refused at the read, annotated |
| `run/dead-address-copy-made-before-the-end-reaches-c`, `run/handle-a-helper-that-ends-its-parameter-is-the-runtime-s` | — | **134** before C, the dead set's and the poison's messages |

The platform legs, the gate and the merge are defect 077's record's, which
closed in the same commit.
