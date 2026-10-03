# The author answers A: a case a platform cannot run is judged where its header is, with a twin on the program's own header everywhere

2026-10-03, written at 11:23 by the clock (`date`). The coordinator put one
decision to the author while lane win214's gate ran, with its
recommendation and its reason, after reading the cases of defects 143, 156,
158, 167, 168 and 191 one at a time on this Mac, in the Linux arm64
container (under clang 22.1.8 and again under 18.1.8) and on the Windows
box, all at `02e507bc`. The author's answer, meant as: *ok, A*. Recorded as
a reading.

**What was measured.** Every case of 158, 167, 168 and 191 green on all
three platforms, none skipped. Of 143's twelve cases four bind `sys/wait.h`
or `sys/select.h`, and of 156's eight one binds `curl/curl.h`; each of
those five is skipped on the Windows box, its build reading
`ffi_missing_header`, and green on this Mac and on Linux arm64. The box's C
library carries `locking.h`, `stat.h`, `timeb.h`, `types.h` and `utime.h`
under `sys/`, and no `wait.h` or `select.h` exists in its Windows Kits,
MSVC or clang include trees (listed on the box the same morning).

**What A rules.** A case that binds a header or a library a platform does
not have, skipped there by name, is judged on the platforms that have it,
each read one case at a time; and where the defect's shape can be written
on a header of the program's own, a twin of the case is written and runs
on every platform. It generalises the author's *4a* of the same morning,
which closed 151 on the platforms carrying SDL3. Written into
`.claude/rules/platforms.md`, with a pointer from
`.claude/rules/verification.md` § The batch.

**What it does to the list.** 143 waits for lane ffimsg's in-tree macro
case, its twin; 156 gains a twin to write, a header beside the program
carrying curl's bit-fields. 158, 167, 168 and 191 were never held by this
question and close after the push and the CI's Linux x86-64 job, as
`.claude/rules/platforms.md` already says.
