---
paths:
  - "runtime/**"
  - "seed/**"
  - "examples/**"
  - ".github/**"
  - "docs/platforms/**"
---

# The three platforms

Home of CLAUDE.md § Verification's platform rules since 2026-09-07. What each
rule cost to learn is in `docs/records/contract/case-law.md`, cited as `CL-NNN`.

## A platform fact is run on a platform, or it is an inference

**The three platforms are measured from this Mac, before the commit**, and CI
stays the judge (CL-048).

**One moment since 2026-10-02, by author instruction** (meant as: *keep
only the Mac as the development environment, the other platforms only
before the final push, Linux arm64 in Docker rather than the emulated
x86*): **Linux arm64 and the Windows box run once, before the push**, the
emulated Linux x86-64 container leaves the routine, and the CI's native
Linux x86-64 leg judges after the push; a defect at the C boundary closes
only after the push's legs ran its cases. What it gives up, unmeasured on
the day: whether the arm64 container runs the `--sanitize` cases
LeakSanitizer needs (CL-055 put that leg on x86-64). The rule of
2026-09-29 below stands as history.

**A case on a header or a library one platform does not have** (the
author's answer *A*, 2026-10-03): skipped there by name, its build reading
`ffi_missing_header` or `ffi_package`, it is judged on the platforms that
have the header, each read one case at a time; and where the defect's
shape can be written on a header of the program's own, a twin of the case
is written and runs on every platform. Measured the day it was given: four
of defect 143's twelve cases bind `sys/wait.h` or `sys/select.h`, and the
Windows box's C library carries neither (its `sys/` holds `locking.h`,
`stat.h`, `timeb.h`, `types.h` and `utime.h`), so under the strict reading
143 could never close; the author's *4a* had closed 151 this way for SDL3
that morning
(`docs/records/log/2026-10-03-1123-the-author-answers-a-a-case-a-platform-cannot-run-is-judged-where-its-header-is.md`).

**The Linux arm64 leg runs its suites four at a time inside the container**,
the compiler's own tests alone first and `cache` alone after, each suite its
own harness process, a red in the parallel pass run again alone before it is
read (§ What may run beside a gate in `.claude/rules/verification.md`, the
same shape this Mac's gate has). Measured 2026-10-03, on the author's
noticing that Docker showed one CPU of eight busy: the sequential leg held
one (`docker stats`, 100% of 800%, memory 299 MiB of 7.65 GiB), so more
memory or CPUs would have bought nothing; the parallel leg on `02e507bc`
read 400% and its 21 count lines were the sequential leg's on the same
commit, line for line by `diff`, the suites in 11 minutes against the
sequential 24 to 27 of 2026-10-02, on a busy Mac. Independent invocations
inside any container run the same way, `xargs -P 4`, each into its own file.

**A leg on a slow machine starts as soon as its input exists, and this Mac
never waits idle on it** (author instruction 2026-10-06, meant as: *you must
learn to parallelise a lot, above all when things run on Windows, on those slow
machines*). The Windows box and the containers run beside everything else: a
pre-leg on a round's code before its last lanes land is allowed and finds a
platform's failure early, and the final leg runs on the closing commit all the
same. Measured the day it was given: batch 12's Windows pre-leg ran while its
lanes were still landing, and its first run read 437 failures from `._*`
entries (below), found and repaired before the closing gate.

**Two moments since 2026-09-29, by author instruction** (*Windows and arm Linux
before the push*; CL-079): **Linux x86-64 runs when a batch closes**, from this
Mac, in the container below, since it is CI's own architecture and the leg
where LeakSanitizer lives (CL-055); **Linux arm64 and Windows run once, before
the push**, with CI as the judge after it. Until that day this paragraph said
*before the commit*, and the practice had already moved: 31 commits that no
platform but this Mac had seen stood on the trunk on 2026-09-28
(`docs/records/log/2026-09-28-1640-m-agreed-retention-closes-over-the-defects-found-after-its-last-eight.md`).
What the record shows the other platforms finding, every case, is a fact at the
C boundary: a mechanism with no Windows arm, a missing include under the Linux
sanitiser, 40 leaked bytes, a sharing violation on the Windows runner. So the
one exception keeps that shape: **a defect found on a platform, or a repair
under `runtime/`, `seed/` or an `examples/` program with an `extern`, runs its
own cases on that platform when it is repaired**, not at the push. macOS is this machine. Windows is a real box,
`docs/platforms/windows/WINDOWS-MACHINE.md`. Linux is a container of the CI
leg's own architecture, `docs/platforms/linux/LINUX-MACHINE.md`, built from
the `Dockerfile` beside it.

These two files are **the hunt, not the judge**. A comment, a `#ifdef` or a
sentence that names three platforms and was checked on one is the defect this
rule exists for, and the Windows leg of CI is what read it the first time.

## The Windows box

Paid by the hour, usually off, and **only the author can start it**: ask, and do
the machine-free work meanwhile (CL-049).

Committed work reaches it with `git push win main:main`. Uncommitted work goes
by `COPYFILE_DISABLE=1 tar --no-xattrs`, because macOS's tar otherwise writes
`._name` AppleDouble entries that the harness globs as programs: 172 of them on
2026-09-03, and seven false failures. **And `--no-mac-metadata` beside it since
2026-10-06**: batch 12's first Windows pre-leg, its archive made without
these flags, read 437 failures in `order` from `._*` entries; with
`COPYFILE_DISABLE=1 tar --no-mac-metadata --no-xattrs` the rerun read 27 of 29
suites green, the two left the batch's own.

The build line there is `seed/README.md`'s, which adds a stack flag and states
why. **The CI leg builds both** that line and the plain one, and asserts the
plain one still compiles a real module, so the two cannot drift in silence
(CL-065).

## Linux is where a leak in a binding is visible

**A program that declares an `extern` runs its Linux leg under `--sanitize`**
(CL-055). LeakSanitizer exists on that leg and on no other, so a leak in a C
binding is invisible on this Mac in all three configurations. The rule is narrow
on purpose: a program with no `extern` cannot leak from the C side, since its own
allocations are counted on every platform by the runtime's own check.

## Measuring in a copy

A copy of the tree without `.git` cannot run the net's `records` suite, because
it reads commits and tags. Measure single suites in a copy; gate a commit on the
real tree (CL-050).
