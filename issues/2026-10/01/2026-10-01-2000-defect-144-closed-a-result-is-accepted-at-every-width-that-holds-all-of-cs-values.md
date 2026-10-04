# Defect 144 closed: a result is accepted at every width that holds all of C's values

- [x] **144 — a result wider than C's is refused** | `extern "arpa/inet.h"` with `function htonl(x: u32) -> u64`: `build` exit 1, `ffi_return_type`, *`htonl` does not return `u64`*, where spec § 13 says *a result may be wider than C's* | `selfhost/emit/extern_assert.hero:50-79` (*wider* read for `i64` and `f64` only, the seat's reading) · **closed 2026-10-01**

    **Origin:** panel 184's ffi-pragmatist, 2026-09-30 (`dbus_bool_t`, a
    `uint32_t`); reproduced by the coordinator at 20:44 on `a294a6ff`
    (`scratchpad/p184/ffi-side/wider.hero`); again at 00:17 on 2026-10-01 on
    `3cc3b553`, exit 1 and `ffi_return_type`.

    **Why it is a defect.** Spec beats compiler (CLAUDE.md § 12).

    **2026-10-01, lane emit, one rule for every width, a result that holds
    every value of C's type**: repaired at `d4512560`, gated by its cases and
    the compiler's own tests; the net is owed at the batch's close.

    **The repair**, lane emit, `d4512560`. **The class**: the result
    assertions read *wider* for `i64` and `f64` only, each narrower width
    taking exactly its own size, so `htonl` declared `-> u64` over `uint32_t`
    was refused at `build`, exit 1, against spec § 13's *a result may be
    wider than C's*, and so were 21 more lossless pairs on every platform
    (`unsigned char` into `u16` to `i64`, `char` into `i64`, `_Bool` into any
    integer but `u8`) and two more on Windows, where `unsigned long` is 32
    bits; the lane measured all 165 pairs of 15 C result types and 11
    declared types on four targets with `clang -target`. Beside it, the other
    direction: `long double` was accepted as `f64` everywhere, a narrowing on
    both Linux targets. Now one rule: a declared result holds C's when it
    holds every value of C's type, asked of `sizeof` and of `_Bool`'s one-bit
    width, and `long double` reaches `f64` only where it is no wider than
    `double`. Eight macro lines of the emitted prelude change text and none
    changes count, so no `#line` restore moves.

    **Cases**: `run/fixedbugs-144-every-wider-result-builds` (the 44 pairs
    accepted on all four targets, each printing its value) and eleven
    `unsupported/fixedbugs-144-what-a(n)-<type>-result-refuses` (117
    refusals); the four pairs whose verdict is the platform's are measured by
    `clang -target`, not held, since no case carries a per-platform
    expectation. The seven `tests/golden/emit` expectations carrying the
    macros were regenerated, each keeping its line count and moving exactly
    the eight lines. On macOS `htonl` is a macro, so the entry's own
    reproducer there meets defect 143; on Linux it builds and prints
    16777216.

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
