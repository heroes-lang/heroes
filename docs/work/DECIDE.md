# DECIDE — the decisions the compiler is waiting on

Read by **`/decide`**. Every item asks **what should be true**, and until it is
answered the compiler goes on behaving some way by default — so the item names
that default, because it is the cost of leaving the item open.

**Only open items live here.** The moment one is answered it is ticked with the
verdict written into it and moved to `docs/records/done/`, the record. Rank by
what an item blocks, never by age, and verify it against the repository before
putting it to the author: asking a settled question is the one cost this list
cannot pay.

**The shape.** One line per item, and an optional body indented four spaces
under it. The first field is the item's **origin**, and where that origin is a
sitting it is spelled `panel NNN`, padded — because
`tests/harness/suite_records.hero`'s `queued` check reads the `- [ ] ` lines
alone and scans them for exactly that, so a citation that slides into the body
makes every pending sitting report as unqueued, and it fails silently. Nothing
lives outside the two banners: `records/lists` is the executor of that.

Format: `- [ ] **<origin>** | <the question, in one line> | <where to look>`

*******************************************************************************
**OPEN: 1**

- [ ] **the C boundary's closings, 2026-10-03** | a golden case that binds a header or a library one platform does not have, skipped there by name with `ffi_missing_header` at its build: does its defect close on the platforms that have it, with a twin of its shape on a header of the program's own running everywhere, or wait for every platform? | `.claude/rules/verification.md` § The batch (*a defect at the C boundary closes only after the push's platform legs have run its cases*) · defects 143 and 156 · defect 151's closed record (the author's *4a*)

    **Origin:** the coordinator's closings at `02e507bc`, every case read
    one at a time on this Mac, in the Linux arm64 container and on the
    Windows box (2026-10-03, 10:12 to 10:55 by `date`). Every case of 158,
    167, 168 and 191 ran green on all three. Of 143's twelve, four bind
    `sys/wait.h` or `sys/select.h` and are skipped on the box, their build
    reading `ffi_missing_header`; of 156's eight, one binds `curl/curl.h`
    and is skipped there the same way. Until it is answered, 143 and 156
    stay open and `blocking`, every case green wherever its header exists.

    **Recommendation: close on the platforms that have the header, with a
    twin on a header of the program's own wherever the shape can be written
    without it**, on what was run and one fact: `sys/wait.h` is POSIX, and
    the Windows box compiles against Microsoft's C library, which has no
    such header, so under the strict reading 143 never closes; the author's
    *4a* closed 151 this way for SDL3. The twin keeps the defect's own shape
    judged on every platform: 143's is lane ffimsg's in-tree macro case,
    already in its batch; 156's would be a header beside the program
    carrying curl's bit-fields. The strict reading would install curl on
    the box for 156 and leave 143 open for good.

*******************************************************************************
