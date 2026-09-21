# DEFECTS — the compiler defects that are still open

Every item is a **measured** failure of the compiler on a program — a crash, a
wrong answer at exit 0, a silence where a message is owed — carrying its
reproducer, its cause where known, and what is owed. **Only open defects live
here**: a repaired one is ticked, gains a *The repair* section with the
measurements that prove it, and moves to `docs/records/done/`. A repair is owed
at the class and not at the witness, with a `tests/golden/fixedbugs/` case per
shape.

**The shape** is `.claude/rules/records.md` § The lists, and § A live list is a
preamble, a count and its items is why this preamble is fifteen lines. **The
next number is READ, never remembered** — `records/numbering` takes one above
the highest issued across this file and `docs/records/done/`. Who issued which
number since 2026-09-08, and why 014 exists twice, is
`docs/records/log/2026-09-16-2200-the-defect-register-leaves-the-list.md`.

Format: `- [ ] **NNN — <title>** | <what it does, in one line> | <where to look>`

*******************************************************************************
**OPEN: 1**

- [ ] **074 — on the Windows CI leg a window opens in which no child process starts, and 106 cases go red** | for part of one net run on `windows-latest`, every `CreateProcess` the harness attempts is refused; the cases report a build that failed and annotations that disagree, and neither happened | GitHub run 35620846459 against 35627328143, the same commit green

    **Origin:** 2026-09-21, the push carrying M-declared-extents step 32. The
    Windows leg read `harness: 1803 passed, 106 failed` where the other three
    legs read 0 failed. **The re-run of that same commit, Windows alone
    (`gh workflow run ci.yml -f only=windows`, run 35627328143), was green at
    1903 passed, 0 failed**, so this is intermittent rather than a regression.
    The author's own Windows box runs the `run` suite at 130 passed, 0 failed
    on the same HEAD.

    **What is measured.** The 101 `run` failures are exactly cases 36 to 136 of
    that directory in walking order — `diff` between the failing set and the
    tail from 36 prints nothing. The 5 `annotations` failures are exactly the
    first 5 of the 10 groups that suite reports on. No suite runs between those
    two. So one window of wall-clock time opened inside `run` and closed inside
    `annotations`, and the cause has a **lifetime** rather than a location.

    **Where it opens.** Case 35 is `c-frees-a-lease-and-the-runtime-names-it`
    and it passed; case 36 is `c-frees-a-lease-on-a-later-call` and it is the
    first failure. Those two and case 37 are the only three cases in the tree
    carrying `!sanitizer:`, which is to say the only three whose programs die by
    heap corruption rather than by an ordered `abort()`. Eighteen `abort-*`
    cases sit inside the first 35 and none of them opens the window.

    **What is NOT measured, and is a question rather than a premise.** Whether
    Windows Error Reporting is what holds the machine. `runtime/parts/os.c:217`
    returns `EXCEPTION_CONTINUE_SEARCH` after the lease report, which lets the
    exception reach the default handler, and `hero_run_go` passes
    `bInheritHandle = TRUE` with the two redirect files opened `FILE_SHARE_READ`
    only — so a surviving grandchild holding them would refuse every later
    `CreateFileA`. **Nobody has seen that process.** Defender is installed on
    `windows-latest` and absent from the author's box (`Get-MpComputerStatus`
    answers *Invalid class* there), and the two machines run clang 20.1.8 and
    23.1.1; each is a difference and none of them is yet a cause.

    **What is owed.** The next occurrence must name its own cause. That is the
    repair landing with this entry: `hero_run_why` carries the operating
    system's own number out of the runtime, and `shell.run` refuses a
    non-start instead of handing back a record whose `code` is -1. What remains
    owed is the cause itself, and the instrument is what will hand it over.

*******************************************************************************
