- [ ] **M-typed-inspection** | the claim with no live test, and the harness has no lldb row | `.github/workflows/ci.yml` · `tests/harness/main.hero` · `docs/panel/085`

    **Origin:** measured 2026-09-06.

    **`design.md:616` and `docs/ROADMAP.md:452` both assert that a golden runs
    lldb in batch mode and asserts a breakpoint on a `.hero` line is hit, and
    nothing has run it since 2026-08-19.** The golden is
    `archive/bootstrap-rs/heroes-cli/tests/golden.rs`, which M-bootstrap-archive
    left where nothing builds it; `.github/workflows/ci.yml` installs lldb on
    the Linux leg for it, describes it as executing in four comments, and runs
    no cargo at all; `tests/harness/` has no lldb suite.

    Owed: one harness suite driving lldb through `shell.run`'s argv list and
    watchdog, carrying the three guards the archived test bought with failures
    (lldb wrote nothing on either stream · the breakpoint is pending with no
    locations · the file, the line and `stop reason`) plus the **stepping** half
    that never had a test, and **its own falsifier run once by hand and quoted
    in the commit body** — `-g` deleted from `selfhost/cli/flags.hero`, the
    suite must go red. It also settles `docs/panel/085` B2's own condition,
    which said the lldb class *"was not tried"*. Windows is stated rather than
    silent: `lldb.exe` exits `0xC0000135` before running a command, so what that
    leg gets is a `heroes doctor` row naming the absence.

    **Where to look also:** `CLAUDE.md` §9.
    **Why it matters:** a debugger suite that passes without DWARF is a
    decoration, and a promise with no instrument is how this one went eighteen
    days unnoticed.

    **Re-verified 2026-09-10: STILL OPEN, and half of its cited claim never
    existed.** Still open: `tests/harness/` holds 20 suites and no lldb suite, and CI
    still installs lldb on the Linux leg (`.github/workflows/ci.yml:271`) for a test
    it never runs. **The false half**: *"`design.md:616` and `docs/ROADMAP.md:452`
    both assert that a golden runs lldb in batch mode."* `grep -n "batch mode"
    design.md` is **empty** — design.md never claimed it, its lldb sentences being
    `:614`, `:635-636`, `:664` and `:2886` — and the ROADMAP's claim is at **`:500`**.
    The same stale pair stood in `docs/ROADMAP.md:1737` and in
    `tests/harness/suite_records.hero`, and **the second was repaired on 2026-09-10**
    in the commit that carries this verification.
