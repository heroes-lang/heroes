- [x] **panel 174** | the net gives every spawn its own redirect files, clang stops inheriting the harness's capture, and the process tree is killed rather than the process — with the `FILE_SHARE_WRITE` bit vetoed | **ratified 2026-09-21**, as a reading | `docs/panel/174-the-two-files-the-whole-net-shares.md`

    **Origin:** defect 074, 2026-09-21, which reproduced in CI twice in three
    pushes and printed `the operating system's own reason is 32` a hundred and
    twenty times, one number and no other.

    **The ratification, in the author's own words:** *I have read the panel, so
    I decide to ratify.* Recorded as a **reading** of the sitting's own file,
    which is CLAUDE.md § 4's default and which the author has now stated three
    times. Not `by delegation`.

    **What it ratifies**, the robust reading rather than the conservative one,
    because the author named no preference and § 4 requires the most robust and
    complete resolution where the two disagree:

    - **B** — one redirect path per spawn, removed once read. Load-bearing: it
      closes the defect whoever the holder turns out to be, which matters
      because the holder on the GitHub runner is still unidentified.
    - **F** — `selfhost/cli/toolchain.hero:79` stops passing `out: ""` to
      clang, which `runtime/parts/run.c:131-133` reads as *inherit*. That is
      the mechanism behind the only holder this project has ever named.
    - **the `FILE_SHARE_DELETE` bit alone**, so `remove_tree` stops failing
      after a green line. `FILE_SHARE_WRITE` is **vetoed** and a ratification
      cannot lift a veto.
    - **C** — the process tree killed, in the ffi seat's form: the Windows Job
      Object unconditional and verified nested inside an existing job, the
      POSIX group swept on ordinary exit and not only on timeout. Recorded as
      closing the **watchdog** defect and not 074, on the timestamps that
      exclude the watchdog for the failing run.
    - **G** — `selfhost/cli/process.hero`'s `Ran` gains `why`, so the compiler
      stops printing *"clang is not on this machine's PATH"* for a sharing
      violation.
    - **D and E refused**, each with a measurement rather than an opinion.

    **What the yes does not settle**, named rather than bundled: what holds the
    handle on the runner; route H, the pipe the parent owns, deferred with its
    price; the 457 `is_err()` branches that discard an error value; and whether
    a machine that cannot start a process should produce a case verdict at all.
