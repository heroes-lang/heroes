- [x] **058 — a plain `char` field or parameter has no spelling that binds on every platform** | `i8` bound it on two legs and `u8` on the third, so one `extern` could not be written for all of them, and `tests/golden/run/ffi-a-char-array-member.hero` was accepted on x86-64 and refused on arm64 Linux | `selfhost/cli/flags.hero` · panel 161

    **Origin:** M-arm-platform, 2026-09-17, the fourth leg's first hour.

    **The repair is one string**, `-fsigned-char`, added to `flags()` in
    `selfhost/cli/flags.hero` — panel 161's resolution, adopted 2026-09-18 and
    ratified by delegation the same night. C's plain `char` is now signed in
    every translation unit this compiler emits, on every leg.

    **Why that and not one of the six other routes**, in one line each, with the
    sitting's file carrying the argument: a ninth integer type and a new
    spelling inside `extern` were vetoed by two seats each, on non-locality and
    on cost (11 exhaustiveness errors in 6 files, plus the runtime); accepting
    either spelling makes `spec § 13`'s sentence false and was withdrawn by its
    own proposer in Rust in 25 hours; refusing plain `char` was vetoed by the
    ffi-pragmatist with a measured memory-safety defect in its own escape hatch;
    binding plain `char` as `u8` everywhere cannot return `elf_prpsinfo.pr_nice`
    as −20, because `to_i8(236)` fails by `spec § 11`.

    **The flag removes the divergence instead of describing it.** `spec § 13`'s
    *"declared at the header's own width and sign"* stays true to the letter,
    because the header's own sign — as the translation unit compiling it sees it
    — is now signed on all four legs. **No spec token was spent**, which is why
    the spec-warden's prediction that `--refresh` would read 8005 is **lapsed by
    resolution** rather than scored.

    **The repair measured, on both architectures.**

    | | before | after |
    |---|---|---|
    | arm64 Linux, the net | 1825 passed, **3 failed** | **1830 passed, 0 failed** |
    | arm64 Linux, `ffi-a-char-array-member` | refused `ffi_field_type` | runs, prints `68` and `68 7` |
    | arm64 Linux, the net's own tests | 157 passed | 157 passed |
    | Darwin arm64, the compiler's own tests | 654 passed | 654 passed |
    | arm64 Linux, `corpus` | 53 passed, 0 failed | 53 passed, 0 failed |

    **The hazard was run, not argued.** The historian cited Microsoft's own `/J`
    page, where pinning the sign is documented to break ATL/MFC, and the
    compiler-engineer named the residual: a bound header's `static inline` code
    whose behaviour depends on the sign. The instrument was the corpus on the one
    leg where the flag disagrees with the platform default, and it reads **53
    passed, 0 failed** with all twenty `extern` programs — sqlite, curl, math and
    the rest — building and running. **The historian's prediction 5, that the
    first breakage would be in a third-party header/library pair, is scored: not
    observed.**

    **What the golden case owes, and it is appended rather than rewritten.**
    `tests/golden/run/ffi-a-char-array-member.hero`'s comment lists four things
    the repair must not relax, the first being *"`u8[4]` against `char[4]` ->
    refused, `char` is signed here"*. That sentence was measured on one machine
    and is now true on four by construction rather than by luck; the case's file
    carries the correction underneath with its date, because `tests/golden/` is
    append-only.

    **One prediction is partly falsified and it is scored here rather than
    quietly dropped.** The compiler-engineer predicted the arm64 net would read
    **1828/0** and that *"no suite other than `run`, `determinism`, `emission`
    moves"*. It reads **1830/0**, and `lines` moved too, 120 to 121. The
    direction was right and the enumeration was not: the golden program
    contributed to four suites, not three, being skipped in the fourth rather
    than failing in it.

- [x] **059 — on a platform where `char` is unsigned, the diagnostic names the spelling it has just refused** | `ffi_parameter_type` on `i8` against a `char` parameter said *"Declare it `i8`"*, so an author following the compiler's own note looped forever | `selfhost/emit/c_spellings.hero:59` · panel 161

    **Origin:** M-arm-platform, 2026-09-17, found by the same probe as 058.

    **Closed with no edit at all, and that is the finding.** The obvious repair
    was to make `c_spellings.hero:59` ask the target for plain `char`'s sign the
    way the same file already asks for `long`'s width. Panel 161's completeness
    critic read the row and found it was never wrong code: **it is a sentence
    that was false on one leg**, and 058's flag makes it true on four. `char`
    maps to `i8`, the note says *"Declare it `i8`"*, and now that is the correct
    advice everywhere.

    **Verified on the leg that provoked it**: with the flag, `heroes build` of a
    program binding a plain-`char` parameter as `i8` is accepted on arm64 Linux,
    where it was refused with that note the day before.

    **The lesson is about where a defect lives.** 058 and 059 were filed apart
    because their repairs looked different — one a language question, one a
    table row. They had one repair, and the table row was not where it was.
    That is CL-078's shape, found by the instrument the sitting added for it.
