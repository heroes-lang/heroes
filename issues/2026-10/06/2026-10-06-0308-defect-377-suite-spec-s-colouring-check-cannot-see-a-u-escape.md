---
kind: defect
area: harness
milestone: none
filed: 2026-10-06
commit: 3c687c3aff0a548d2a69b79db00015085ed57776
github: none
---

- [x] **377 — suite_spec's colouring check cannot see a `\u{...}` escape** | lane b12-str192, landing panel 192's R5, tested the editor grammar's pattern for `\u{...}` with Python's `re` alone, since `tests/harness/suite_spec.hero`'s colouring check reads no escape (its report, 2026-10-06) | `tests/harness/suite_spec.hero` (the colouring check) · `editors/vscode/syntaxes/heroes.tmLanguage.json` · **class: improvement**

    **Origin:** lane b12-str192, 2026-10-06, found beside panel 192's landing (its report's *found beside*); filed by the coordinator at 03:08.

    **Class: improvement**, 2026-10-06 (`.claude/rules/verification.md` § Bounded discovery): coverage, no program moves.

    Repaired at `3c687c3a`, 2026-10-07 (lane b14-harness-b), gated by its cases and the net's own tests; the net is owed at the batch's close. `tests/harness/textmate.hero` runs the grammar's rules for a string, an `f` literal and a character literal as an editor does, and `suite_spec`'s `escapes_painted` holds them to the lexer on the seven escapes § 2 names and six other spellings of its `\u{1b}`, 39 `heroes lex` runs: on a copy of the tree, three breaks of the `\u{…}` pattern each read spec 21 passed, 0 failed on the base and 22 and 1 with this check, which reads 23 and 0 on the grammar as it is.

    And at `57a95932`, 2026-10-07, before the repair left the lane: the reader named a `\xHH` below the space by a slice of printable text and panicked on `[\x00-\x1f]`; it now writes the byte the code names, four assertions red at `3c687c3a` and green there, the net's own tests 296, all passed.

    And at `b630b275`, 2026-10-07, an `adjacent` the check found in the lane's own file, given to it by the coordinator: the grammar painted `\u{d800}`, a surrogate, and `\u{110000}`, past Unicode, as escapes, which the lexer refuses `unknown_escape`. Its escape by code now admits a Unicode scalar value alone (swept over every code to 0x11000f, 0 mismatches), and `escapes_painted` asks seven edges of a code beside the spec's escapes, red first on the old grammar (spec 22 passed, 1 failed, the two surrogate edges and `\u{110000}`) and 23 and 0 after.

## The repair

Repaired at `3c687c3a`, 2026-10-07 (lane b14-harness-b), gated by its cases and the net's own tests; the net is owed at the batch's close. `tests/harness/textmate.hero` runs the grammar's rules for a string, an `f` literal and a character literal as an editor does, and `suite_spec`'s `escapes_painted` holds them to the lexer on the seven escapes § 2 names and six other spellings of its `\u{1b}`, 39 `heroes lex` runs: on a copy of the tree, three breaks of the `\u{…}` pattern each read spec 21 passed, 0 failed on the base and 22 and 1 with this check, which reads 23 and 0 on the grammar as it is.

**Closed 2026-10-07** with batch 14 (lanes b14-hooks, b14-parse, b14-text, b14-resolve, b14-check, b14-emit, b14-ir, b14-cli, b14-runtime, b14-harness-a, b14-harness-b, b14-mutate, b14-box and b14-m212, merged into one round tree made from the trunk at `affbd872`, the batch closed on the round as it stood when the author asked at about 20:00 to commit and push what was done), its closing gate run on the round at `01995004` (the six suites it read red re-run alone at `8ef1684a`, after their floors and 439's case): the seed regenerated over two generations from the trunk's compiler, the runtime's ABI staying at 29, 41,073,300 bytes, SHA-256 beginning `f1520dbf78a3e91f`, its fixpoint by `cmp`; the compiler's own tests 1,430, all passed; the net's own tests 308, all passed; the full net 6,818 passed over 29 suites, 0 failed, after defect 439's case was moved to the one message defect 458's repair leaves and four floors (`annotations` 4,671, `order` 42 and 36, `runtime` 29, `unseen` 6,381) were raised to the merged tree's counts; the census of `check` over 2,980 tracked files moving 28 verdicts and of `--emit-c` over 1,323 moving the C of all 592 that compile (defects 389's inline reads and 263's `#line`s) and 69 refusals, every move attributed (in `check`, 10 to defect 410's one message for a run of openers, 8 to 439's and 458's, 4 to 252's and 253's Windows names, 4 trunk crashes at exit 134 told at exit 1 by 456's repair, one each to 455 and 460; in the refusals, 32 to 270's note, 23 to a cache path in a clang warning, 8 to 294's excerpt window, 6 to the codes of 252, 253, 439 and 458). By the author's ask of about 23:20 (*be optimistic, push as soon as you can*), panel 187's R2 replay and the formatter's probe by hand finish after the push, their findings filed by class, and Linux arm64 and Windows are the CI's legs.
