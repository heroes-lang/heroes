---
kind: defect
area: emit
milestone: none
filed: 2026-09-24
commit: f66ccb77fecaa219049b9aa2fb1dae3abeb688bf
github: none
---

- [x] **092 — a whole group record lent to a `void *` parameter is handed to C with no bound on the count C is told, and C writes past it** | a record behind `@` against a `void *` passes every pointee check, so `read(fd, buf: @h, n: 4096)` into a 48-byte record is `check` 0 and a stack overflow at run time | `selfhost/emit/extern_probe.hero` · `selfhost/cli/pointee.hero` · `tests/golden/fixedbugs/ffi-pointee-void.hero` · `spec § 13` · **class: blocking**

    **Origin:** panel 178's ffi-pragmatist, 2026-09-24 (its § 8 F1), found
    while binding the census structs; reproduced by the coordinator the same
    day on all three legs before filing.

    **Class: blocking**, 2026-10-06 (`.claude/rules/verification.md` § Bounded discovery), classed the day it reached the trunk: a memory fault and a crash at `check` 0, design.md §1.12's own case.

    **The reproducer** (`docs/panel/178-reports/ffi-pragmatist-work/rec_overwrite.hero`):

        extern "netdb.h"
            record Hints tag addrinfo partial
                ai_family: i32
                ai_socktype: i32

        extern "unistd.h"
            function read(fd: i32, @buf: Hints, n: u64) -> i64

        function main()
            h: Hints @ Hints(ai_family: 0, ai_socktype: 1)
            n = read(fd: 0, buf: @h, n: 4096)
            print(n)
            print(h.ai_family)

    With 4096 bytes on standard input: `check` **0**; run **138** on Darwin
    arm64, **135** (Bus error) on Linux arm64, **139** (segmentation fault) on
    Linux x86-64, three of three on each, after printing `4096`.
    `--sanitize`: `AddressSanitizer: stack-buffer-overflow`, *WRITE of size
    4096*, on all three. The seat measured that the same shape through `write`
    hands C's reader the stack beyond the record (`rec_overread.hero`).

    **Why it is a defect.** design.md §1.12: a Heroes program must not
    segfault and must not corrupt memory, and this one does both at `check` 0.
    It is defect 010's sibling on another shape: `@value: i32` against
    `void *` is refused (`tests/golden/fixedbugs/ffi-pointee-void.hero`), and a
    whole record in the same position is not. The count that bounds the write
    is a C argument the program supplies, which is the shape `counted_by`
    relates for a lent field; nothing relates it for a record.

    Re-run 2026-10-06 on `bef739dd`: this worktree's compiler, built from the round's seed, over the reproducer as `docs/panel/178-reports/ffi-pragmatist-work/rec_overwrite.hero` holds it, 4096 bytes of `A` on standard input, in a scratch directory from 11:37 by the clock: `check` 0; `run` 138 three of three on this Mac, after printing `4096` and `1094795585`, which is `0x41414141`, four of the bytes C wrote over the record; `run --sanitize` exit 1, *AddressSanitizer: stack-buffer-overflow*, *WRITE of size 4096*. Still broken. Linux arm64 and x86-64 unrun that day.

    Evidence for panel 194, 2026-10-06, lane ffi13 of batch 12: `docs/panel/194-evidence/092-routes.md`, with its programs and the prototypes' diff under `docs/panel/194-evidence/092/`. Two routes were built as prototypes and not landed, a count declared on the lent record with an undeclared lend to `void *` refused, and `x.ptr()` lending a whole record to a counted `ptr`; each was run on every shape beside the reproducer on this Mac and Linux arm64, with what it changes in the tracked programs (none of the 9 holding the 12 whole-record lends), what it costs in instructions, and what it leaves open: a count of records against the record's own pointer type, and a count C reads through a pointer. The item stays open for the sitting.

    **2026-10-06, panel 194** (`docs/panel/194-the-rest-is-zero-where-a-group-s-construction-says-so-and-a-lend-s-count-is-in-c-s-own-unit.md`), ratified by delegation the same day, R7: a lend's count is compared in C's own unit, elements of the header's pointee type and bytes for `void *`; an undeclared whole-record lend to `void *` is refused, naming the `counted_by` that admits it, and `counted_by` accepts a count read through an `@` cell; route B is refused; no interim. The landing, which builds route C, is batch 13's and repairs defect 395 with this item.

    Repaired at `f66ccb77`, 2026-10-06 (lane b13-unit), gated by its cases and the compiler's own tests; the net is owed at the batch's close, and the push's platform legs, this being at the C boundary. Panel 194's R7, route C, in one repair with defect 395: a group's record lent whole through `@` takes `counted_by`, its count may be an `@` cell C reads (`getsockopt`'s length), and the call is held to C's `sizeof` of the record in the unit the header's pointer counts, at build time for a constant and before C runs otherwise; an undeclared one the header takes as `void *` is refused on the parameter, naming the `counted_by`. Every shape of `docs/panel/194-evidence/092-routes.md` stops before C runs or is refused, but the two `today-typed-pointer-*`, a record lent undeclared to its own pointer type, which are defect 396's question; run on this Mac, Linux arm64 and x86-64, its C on the Windows box by hand (clang 23.1.1, `x86_64-pc-windows-msvc`).

## The repair

Repaired at `f66ccb77`, 2026-10-06 (lane b13-unit), gated by its cases and the compiler's own tests; the net is owed at the batch's close, and the push's platform legs, this being at the C boundary. Panel 194's R7, route C, in one repair with defect 395: a group's record lent whole through `@` takes `counted_by`, its count may be an `@` cell C reads (`getsockopt`'s length), and the call is held to C's `sizeof` of the record in the unit the header's pointer counts, at build time for a constant and before C runs otherwise; an undeclared one the header takes as `void *` is refused on the parameter, naming the `counted_by`. Every shape of `docs/panel/194-evidence/092-routes.md` stops before C runs or is refused, but the two `today-typed-pointer-*`, a record lent undeclared to its own pointer type, which are defect 396's question; run on this Mac, Linux arm64 and x86-64, its C on the Windows box by hand (clang 23.1.1, `x86_64-pc-windows-msvc`).

**Gated 2026-10-07** with batch 13 (lanes b13-c382, b13-bs, b13-unit, b13-front, b13-run400, b13-zero401, b13-gen402, b13-fixed405, b13-tmpl407, b13-w411, b13-land-addr and b13-land-buf, and panel 196's sitting, merged into one round tree made from the trunk at `74bc6308`), its closing gate run on the round's head `06cf8cd4`: the seed regenerated over two generations from the trunk's compiler, the runtime's ABI moving from 27 to 29, 40,044,821 bytes, SHA-256 beginning `2ba1ae96d684aa2c`, its fixpoint by `cmp`; the compiler's own tests 1,357, all passed; the net's own tests 289, all passed; the full net 6,581 passed over 29 suites, 0 failed but `records` 27 and 1 on the ROADMAP's count, defect 442 being filed during the gate and recounted in the closing commit; the census of `check` over 2,892 tracked files moving 180 verdicts and of `--emit-c` over 1,305 moving 213 refusals, every move attributed (123 and 85 to `rest: zero`, the rest to panel 196's R5, to the fixed array's wording of 399, 405, 408, 423 and 432, to the reproducers of 397 and 398 now refused, to defect 092's route C refusing the 14 probes of its own fault, and to a cache path in 19 warnings); panel 187's R2 replay of 13,594 mutants and 16,041 pairs against the trunk's run, 0 findings, 23 mutants moving from several messages to one (defect 391's); Linux arm64 on the same tree, the compiler's own tests 1,357 and 20 suites, every one 0 failed.

**Closed 2026-10-07** after the push's platform legs, a defect at the C boundary closing only after them (`.claude/rules/verification.md` § The batch), the last Windows leg sent to the CI by the author at 10:19 (`issues/2026-10/07/2026-10-07-1019-the-author-sends-batch-13-s-last-windows-leg-to-the-ci.md`): the CI's run 37603092655 on the pushed `34f95b71`, every job a success, read at 13:43; Linux x86-64 the compiler's own tests 1,357, the full net 6,536 passed and the net's own tests 289, 0 failed; Linux arm64 1,357, 6,536 and 289; Darwin arm64 1,357, 6,571 and 289; Windows x86-64 1,357, 6,422 and 289. The cases whose header Windows lacks were skipped there by name and run where the header is: defect 092's `getsockopt`, 396's POSIX and SHA-256 buffers and 436's stopped child on both Linux legs and Darwin, 413's zlib case on the three and its libuv case on Darwin alone, `pkg-config` on both Linux legs not knowing `libuv`.
