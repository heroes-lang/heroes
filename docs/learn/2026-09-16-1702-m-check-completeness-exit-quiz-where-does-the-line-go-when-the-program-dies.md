- [ ] **M-check-completeness exit quiz** | A program prints `7`, then hands C a null pointer that C reads through. It dies. **Before looking: say whether the `7` reaches the terminal, and whether your answer is the same on macOS and on Linux.** Then say which of the two behaviours is a bug. | `runtime/parts/os.c`, `hero_streams_survive_abort` · `docs/panel/156-reports/historian.md` items 3 and 4

    **Where to look after answering:** the historian's report, items 3 and 3b —
    they carry the clause numbers — then `runtime/parts/os.c`'s comment on the
    function above.

    **Why it matters.** The honest answer is that **neither is a bug**. C
    § 7.22.4.1 makes flushing on `abort()` implementation-defined; POSIX Issue 8
    *downgraded its own text* from *"shall include the effect of `fclose()`"* to
    *"may"*, and says in its rationale that it did so because `abort()` must be
    async-signal-safe; POSIX § 2.4.3's async-signal-safe list contains `write()`
    and does **not** contain `fflush()`. glibc flushed on abort for decades and
    **removed it at 2.27**, citing *"deadlocks and data corruption"*; FreeBSD
    still flushes, under a source comment reading `XXX ISO C requires that
    abort() be async-signal-safe`. Two libcs, two legal answers, and one of them
    changed sides.

    **So the repair could not be "flush before dying"**, which is what a first
    guess reaches for and what the sitting's own coordinator would have written.
    It is to leave nothing in the buffer instead.

    **The question to carry away, and it is the harder half.** The obvious repair
    was then `setvbuf(stdout, NULL, _IOLBF, 0)` — correct on POSIX, measured
    green on two machines. On Windows that call **kills the process**, silently,
    at exit 127, and the compiler built from it did not survive `heroes doctor`.
    Ask what kind of knowledge would have predicted that, and then ask why
    `.claude/rules/platforms.md` states its rule as *a platform fact is run on a
    platform, or it is an inference* rather than as *test on three platforms* —
    the two sound alike and only one of them catches this.
