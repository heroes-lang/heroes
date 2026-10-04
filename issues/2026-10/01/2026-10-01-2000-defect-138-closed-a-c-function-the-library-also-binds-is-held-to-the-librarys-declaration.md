---
kind: defect
area: check
milestone: none
filed: 2026-09-30
commit: 6708ba0521af542def2be41ff0798db7b2e3d518
github: none
---

# Defect 138 closed: a C function the library also binds is held to the library's declaration, and the caret is the author's

- [x] **138 — a program that declares a C function the Heroes library also binds, with other marks, stops `check` with an internal error** | a five-line file whose `extern "hero_os.h"` group declares `function hero_file_read(path: cstr, @status: i64) -> str`, where the library writes `path: cstr lent`: `heroes check` prints `internal error: a diagnostic landed inside the Heroes library, at its line 112: [contract_differs] ...` and exits 2, with or without `--permissive`, where a `contract_differs` at the author's line 2 and exit 1 are owed | `selfhost/check/contracts.hero:256` (`differs`, which puts the message at `at`) and its caller's choice of `at` · `selfhost/cli/check.hero:73-82` and `selfhost/cli/compile.hero:96` (the guard that turns a diagnostic inside the library into the internal error) · **closed 2026-10-01**

    **Origin:** the coordinator, 2026-09-30 at 16:22, on the trunk's compiler
    at `a294a6ff`, following up what it had noted and not filed the same
    morning: `check --permissive` over
    `archive/bootstrap-rs/heroes/src/library/source.hero`, an old copy of the
    library, exits 2 with the same internal error. Reproducers in
    `/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/edfda945-b355-427c-a48e-2113b1040b67/scratchpad/p184/libclash/`,
    each `check`ed with and without `--permissive`: `clash-plain.hero` (the
    shape above), `clash-otherheader.hero` (the same declaration under
    `extern "stdio.h"`, the same internal error), `n3-mark-added.hero`
    (`hero_str_try_from_cstr(p: cstr lent, ...)` where the library writes no
    mark: the same, at the library's line 123), `n6-called.hero` (the shape
    above, called from `main`: the same). The shapes beside that do NOT stop
    it, exit 0: the same declaration with the library's own marks
    (`clash-same.hero`, `hero_exit(code: i64)`), a constant at another type
    (`constant HERO_OS_OK: i32`), another result type
    (`hero_args_count() -> i32`), another parameter count, and a plain
    function named `hero_exit`; whether those three `extern` ones are
    clang's to refuse at `build`, as a binding's types are, is a question and
    unrun here.

    **Why it is a defect.** The program is the author's and the mistake is in
    the author's line, yet the message names a line of a file the author
    cannot open and the exit says the tool could not run
    (`.claude/rules/cli-surface.md`: exit 1 is *the input has diagnostics*).
    The guard's own comment says why it exists: *a diagnostic pointing into
    the library is the COMPILER being wrong, not the program* (panel 028
    R5). It fires because `contract_differs` compares two declarations of
    one C symbol and, when one of them is the library's, may be put at the
    library's. Read and unrun: which of the two becomes `at` is its caller's
    choice in `selfhost/check/contracts.hero`, and a declaration of the
    author's is always the one to name.

    **2026-10-01, lane emit, the library's declaration is the one the
    program's are held to**: repaired at `9cd31e88`, gated by its cases and
    the compiler's own tests; the net is owed at the batch's close.

    **The repair**, lane emit, `9cd31e88`. **The class**: `contract_differs`
    compared two declarations of one C symbol in file order and put the caret
    on the later one; the Heroes library comes after every file of a program
    and binds eight C functions, so a program declaring one of them with other
    marks (a parameter's `lent` or `@`, a result's `owned`) stopped `check` and
    `build` with *internal error: a diagnostic landed inside the Heroes
    library*, exit 2, with or without `--permissive`. Now the library's
    declaration is the one every other declaration of its C symbol is held
    to, the caret is always the author's, the message names the library by
    what it is, and two of the author's declarations that both disagree with
    it are named in one run. The entry's open question, answered by the
    lane's first pass: `n2` (`-> i32`) and `n4` (an extra parameter) are
    refused at `build` on the author's line, and `n1` builds, `i32` holding
    the header's value.

    **Cases**: four `check/fixedbugs-138-*` (a mark the library writes left
    out, one it does not write, a result mark it does not write, an `@` it
    writes left out), one diagnostic each and no fix; three unit tests in
    `selfhost/check/contracts.hero` (a used module, two declarations against
    the library, two with no library declaration). Beside it, measured and
    held by the same rule: a `use`d module's declaration, CRLF, and
    `archive/bootstrap-rs/heroes/src/library/source.hero`, the defect's own
    origin, which the census now reads at its real diagnostics.

    **The gates.** Each repair by its cases and the compiler's own tests, and
    the lane's `extern` census (377 files) after each: 0 moved for 138, 204
    for 144 (the eight macro lines alone), 1 for 145 (the sqlite3 golden's
    note), 18 for 140 (23 probe lines alone), 0 for `24bbb539`. **The batch
    gate**, lane emit's closing commit `6df3d121`, the trunk merged twice,
    records only (`3c895b69`, `abf39e3d`): the seed regenerated once, the
    fixpoint by `cmp`; the compiler's own tests 967 and the net's own 184,
    all passed; `tests/emission` re-blessed and proved, 302 traces modified,
    2,416 lines the eight result macros and 23 lines a probe's `{0}` to
    `{}`, nothing else; the full net, 25 suites four at a time and `cache`
    alone, every one 0 failed (annotations 413 after its floor rose from
    1,652 to 2,148 as it asked, check 336, emission 646, fixes 586, run 215,
    unsupported 38, warnings 276 among them; `probe` 21 and 1 in the parallel
    pass on the harness's timeout, 24 and 0 alone); the census of `check
    --brief` over 1,484 files, both arms, 9 and 10 moved, every one the
    batch's own cases and the 138 origin above. The trunk fast-forwarded to
    `6df3d121` at 17:44 on 2026-10-01. **Linux x86-64** on `6df3d121`, under
    emulation, Debian clang 22.1.8: the compiler's own tests 967, all passed,
    and 18 of the 19 suites 0 failed; `run` read 210 and 1 in that pass,
    which records counts only, and 211 and 0 alone in the lane's second
    container pass, so the red did not stand alone and its cause is not
    measured. In that second pass: `unsupported` 36 and 0, `run
    fixedbugs-140` 3 and 0 (the three cases a thousand deep build),
    `fixedbugs-144` 1 and 0, `long double` into `f64` refused, the `{}`
    probe compiled with 0 warnings. **Owed before the push**: Linux arm64
    and the Windows box, on the trunk.
