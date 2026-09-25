# One C function, one contract: two modules declaring one C name must agree

2026-09-25, M-agreed-retention step 12, in lane `572367a8`, merged `3f76a72d`: the
milestone's second item, filed by panel 171's completeness critic on
2026-09-20 and measured at step 1 on every mark.

- [x] **M-agreed-retention** | two modules may declare one C function with contradictory retention marks, and the compiler accepts both at `check` 0 | panel 171's completeness critic, the historian's B.4, `selfhost/check/marks.hero` · **closed 2026-09-25**

    **Origin:** panel 171's completeness critic, 2026-09-20, attacking the
    `lent` rule at the shapes beside it. **Filed rather than fixed**, because it
    needs a rule ACROSS modules that no sitting has priced, and because the mark
    it concerns lands in commit A of the flip and does not exist yet.

    **What it is.** Module one declares `function keep(s: cstr lent)`, module two
    declares `function keep(s: cstr)`, both against the same header. Each is
    internally consistent, the two disagree about C, and nothing compares them.
    **This is the exact shape that broke upstream Clang's `noescape` on its first
    day** (2017-09-19): a third-party re-declaration disagreed with the SDK
    header's mark and the build failed. Heroes has no header to disagree with,
    only two `.hero` files, so the disagreement is silent.

    **Measured 2026-09-23, step 1, every mark and not only `lent`**, Darwin
    arm64, module `alt` against module `main` over one local header:

    | the two modules disagree on | `check` | `run` |
    |---|---|---|
    | `lent` against unmarked, panel 171's shape | 0 | 0 |
    | `owned my_free` against `owned other_free` | 0 | 0 |
    | `owned my_free` against no `owned` | 0 | 0 |
    | two `record`s over one tag | **1**, `duplicate_tag`, program-wide | — |
    | `consumes` against unmarked, the second declaring over `alt.H` | 0 | **134**, the live set |
    | `borrows` against `acquires`, over `alt.H` | 0 | **134**, the live set |
    | `acquires h_close` against `acquires h_close2` | 0 | 0, **and the same inside ONE module**, which is defect 075 |
    | `counted_by` naming two different siblings | UNRUN: it needs two numeric siblings, and none was written | |

    **The census**, `awk` over every `extern` group in `git ls-files '*.hero'`
    outside `archive/`: **423** declarations in **187** files. Inside one program
    no C function is declared twice in `examples/` or `selfhost/`; the one
    program that does is `tests/golden/surface-fixtures/twoarity/`, `printf` at
    two arities with both formats `lent`, **legal by design**: its README is
    panel 094's refutation of a rename clause, *one C symbol, two bindings, no
    new syntax*. So a rule relating two declarations moves no shipped file and
    must admit that fixture, and **it pulls against the third item**, whose
    natural route is exactly panel 094's: one module per retention mode. And it has a place to stand: `checker.hero` checks one resolved
    program, and `one_tag_one_type` (`selfhost/check/decls.hero:334`) is already
    program-wide.

## The rule

`check/contracts.hero`, program-wide like `one_tag_one_type`: two `extern`
declarations of one C name in two files must say the same thing at every
position they share, at one type (the checker's interned id) and one
canonical spelling of the marks (a releaser set sorted, `counted_by`'s sibling
by position, since the two declarations may name their parameters
differently), and the result likewise, its marks and its `when`. The first
disagreement of a pair is one diagnostic, `contract_differs`, on the later
declaration, naming the other's place, what differs and both spellings; its
note says the two repairs, change the one that is wrong about C, or give the
second way a shim with its own name. Within one module the resolver's
`declared_twice` speaks first, so the rule reads pairs from two files only,
and `twoarity`'s two arities of `printf` (panel 094) share one position and
agree on it, so they pass.

## The measurements

| program | before (trunk `517b8e25`) | after |
|---|---|---|
| `surface-fixtures/contract/` (`keep(s: cstr lent)` beside `keep(s: cstr)`) | check **0** | check 1, `contract_differs` naming `lent` against no mark |
| `surface-fixtures/twoarity/` | run 0 | run 0 |

Lane gate: the compiler's 699 tests, the seed regenerated and the fixpoint
held, 546 emissions blessed, the net 2215 passed and 0 failed, the net's own
167.
