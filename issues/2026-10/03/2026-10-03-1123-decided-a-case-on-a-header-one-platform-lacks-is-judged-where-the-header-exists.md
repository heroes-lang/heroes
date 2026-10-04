---
kind: decision
area: process
milestone: none
filed: 2026-10-03
commit: d9f5ddb8be6a819680f28490d2aa7ef6237a9772
github: none
---

# Decided: a case on a header one platform lacks is judged where the header exists, with a twin on the program's own header running everywhere

2026-10-03, on the author's answer, meant as: *ok, A*. **Recorded as a reading**, CLAUDE.md § 4's default; not `by delegation`.

- [x] **the C boundary's closings, 2026-10-03** | a golden case that binds a header or a library one platform does not have, skipped there by name with `ffi_missing_header` at its build: does its defect close on the platforms that have it, with a twin of its shape on a header of the program's own running everywhere, or wait for every platform? | `.claude/rules/verification.md` § The batch (*a defect at the C boundary closes only after the push's platform legs have run its cases*) · defects 143 and 156 · defect 151's closed record (the author's *4a*) · **decided 2026-10-03: A**

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

    **Decided 2026-10-03: A.** The rule's home is `.claude/rules/platforms.md`
    § A platform fact is run on a platform, with a pointer from
    `.claude/rules/verification.md` § The batch. 143 closes once lane
    ffimsg's in-tree macro case has run on all three platforms with its
    other cases where their headers are; 156 gains its twin, a case on a
    header beside the program carrying curl's bit-fields, and closes once
    that twin has run on all three.
