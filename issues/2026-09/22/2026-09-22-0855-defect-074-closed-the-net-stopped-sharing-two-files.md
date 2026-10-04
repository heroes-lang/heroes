- [x] **074 — on the Windows CI leg a window opens in which no child process starts, and 106 cases go red** | one holder of one shared redirect handle refused every later spawn; the cases reported a build that failed and annotations that disagreed, and neither happened | **closed 2026-09-22**, GitHub run 35693306661 green on all four legs | `docs/panel/174-the-two-files-the-whole-net-shares.md`

    **Origin:** 2026-09-21, the push carrying M-declared-extents step 32. The
    Windows leg read `harness: 1803 passed, 106 failed` where the other three
    read 0 failed, and it reproduced twice in three pushes.

    ## The repair, and the measurement that closes it

    **The cause stopped being a hypothesis during panel 174's own sitting.** It
    reproduced in CI while the seats were deliberating — run 35651518558,
    Windows job 106504808513 — and the instrument repaired hours earlier printed
    the number: `the operating system's own reason is 32`,
    ERROR_SHARING_VIOLATION, **120 times, one number and no other**
    (`grep -ao "operating system's own reason is [0-9]*" | sort | uniq -c`).

    `hero_run_go` opened the harness's two redirect files `FILE_SHARE_READ` and
    handed them to every child inheritable, and `tests/harness/shell.hero` gave
    all 81 call sites **the same two paths**. So anything that outlived its
    parent refused every later spawn.

    **What landed**, panel 174's six clauses across commits `497a048f`,
    `eca8d5d1` and `c27deda8`:

    - **B** — one redirect path per spawn, removed once read. Load-bearing: it
      closes the defect *whoever* the holder is, and the holder on the GitHub
      runner was never identified.
    - **F** — `selfhost/cli/toolchain.hero` stops passing `out: ""` to clang,
      which the runtime reads as INHERIT. clang was receiving the harness's own
      capture handle, which is the orphan `tests/harness/shell.hero` recorded in
      August without anybody connecting the two.
    - **the `FILE_SHARE_DELETE` bit alone.** `FILE_SHARE_WRITE` is vetoed: it
      would let the reopen succeed under a live writer, so an orphan's bytes
      land in another case's capture — measured at 150 bytes where 17 were
      owed, on Windows, Darwin and Linux alike.
    - **C** — the process tree killed rather than the process, with the child
      handed the terminal where this process holds it. It closes a **different**
      defect and is recorded as doing so.
    - **G** — `Ran` gains `why`, so the compiler stops blaming the PATH for a
      sharing violation.
    - **D and E refused**, each with a measurement.

    ## The verdict

    **GitHub run 35693306661, green on all four legs**, and the Windows leg
    reads **zero** lines carrying `the operating system's own reason is 32`
    (`grep -ac` over its own log, 148,389 bytes). `harness: 1903 passed, 0
    failed`. The three suites the defect struck:

    | | 2026-09-21 | 2026-09-22 |
    |---|---|---|
    | `run` | 35 passed, **101 failed** | **130 passed, 0 failed** |
    | `annotations` | 164 passed, **19 failed** | **174 passed, 0 failed** |
    | `determinism` | 150 passed, **10 failed** | **160 passed, 0 failed** |

    That is the **second** consecutive Windows leg at zero; the sitting's
    prediction asks for three, and it is scored at the milestone that has them.

    ## What is owed and was NOT done, said rather than hidden

    **There is no `tests/golden/fixedbugs/` case**, and the preamble of this
    list asks for one per shape. The reason is that this defect is not a
    property of a Heroes program: it is a property of the harness that *starts*
    programs, and a golden case is a program with an expected output. Its
    witness is a self-test instead —
    *"the captures are removed, so a run cannot fill the disk with them"*,
    which runs twelve real programs through the real `run` and then asks the
    directory what is in it. It is the 162nd of the net's own tests and it
    passes on all three platforms.

    **And the holder on the runner was never identified.** Route B closes the
    defect without knowing, which is why it was made load-bearing; the cheapest
    instrument that would name it is a `Get-CimInstance Win32_Process` dump
    taken at the first error 32, and nobody has taken one.
