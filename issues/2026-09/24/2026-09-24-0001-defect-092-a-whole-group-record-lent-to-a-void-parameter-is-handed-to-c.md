---
kind: defect
area: emit
milestone: none
filed: 2026-09-24
commit: none
github: none
---

- [ ] **092 — a whole group record lent to a `void *` parameter is handed to C with no bound on the count C is told, and C writes past it** | a record behind `@` against a `void *` passes every pointee check, so `read(fd, buf: @h, n: 4096)` into a 48-byte record is `check` 0 and a stack overflow at run time | `selfhost/emit/extern_probe.hero` · `selfhost/cli/pointee.hero` · `tests/golden/fixedbugs/ffi-pointee-void.hero` · `spec § 13` · **class: blocking**

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
