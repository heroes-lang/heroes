# Defect 145 closed: a record clang cannot lay out is told what the header in hand says its type is

- [x] **145 — `ffi_unknown_tag`'s note offers two repairs for a typedef of an anonymous struct, and neither is right: the handle repair builds and aborts at run** | a binding written `record Regex tag regex_t partial` for `typedef struct { ... } regex_t;` is refused with the note's two repairs, a misspelled tag or a handle; the handle checks, builds and aborts at run, *panic: a null pointer was read through, at offset 0x8*; the spelling that works, a record named `regex_t` with no `tag`, is not offered (`div_t`, `ldiv_t`, `lldiv_t` have the same shape) | `selfhost/emit/ctype.hero:149` · the note of `ffi_unknown_tag` · **closed 2026-10-01**

    **Origin:** panel 184's ffi-pragmatist, 2026-09-30 (its
    `work/q1/regex/`, copied to `scratchpad/p184/ffi-side/regex-from-seat/`);
    the handle's abort reproduced by the coordinator at 20:44 on `a294a6ff`;
    both again at 00:17 on 2026-10-01 on `3cc3b553`, the handle `check` 0,
    `build` 0 and its run 134 (`ffi-side/handle.hero`), the note with one
    field under the record (`ffi-side/tag1.hero`; with none, `empty_record`
    speaks first).

    **Why it is a defect.** A §4.17 defect in a note: the repair it offers
    leads to a run-time abort, and the right one is left out.

    **2026-10-01, lane emit, the note says what clang showed about the header
    in hand**: repaired at `8a7e69d7`, gated by its cases and the compiler's
    own tests; the net is owed at the batch's close.

    **The repair**, lane emit, `8a7e69d7`. **The class**: one note covered
    every group record with fields whose C type clang cannot lay out, offering
    a misspelled tag or a handle, so `record Regex tag regex_t partial` over
    `typedef struct { ... } regex_t;` was told to become a handle, which
    checks, builds and aborts at run, and the spelling that works, `record
    regex_t` with no `tag`, was never offered. Clang's first round already
    tells the shapes apart, and the new `selfhost/emit/ffi_incomplete.hero`
    reads it: its *forward declaration of 'struct T'* note points into the
    header for a struct kept opaque (the handle, alone) and at the program's
    line where C made the struct up (never the handle); a refused pointer
    parameter or result shows the header's spelling, a typedef (`record T`)
    or, through `aka`, the struct's real tag (`tag U`). Beside it, a record
    named after a typedef of an opaque struct or of `void` was an internal
    error, exit 2; it is now the same code at exit 1, saying what the typedef
    is.

    **Cases**: nine `unsupported/fixedbugs-145-*`, one per way a header names
    a struct, over `fixedbugs-145-types.h`; `regex_t`, `div_t`, `ldiv_t` and
    `lldiv_t` measured by hand. **Left open, and not of this class's
    falsehood**: where clang shows no parameter or result spelling the type,
    the note gives the two true readings, a misspelled tag or a typedef name;
    naming the exact one needs a second clang round, the same one defect 143
    needs, and goes with it to its sitting.

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
