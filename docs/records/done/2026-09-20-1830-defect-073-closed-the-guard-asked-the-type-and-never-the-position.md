- [x] **073 — `owned <fn>` on an INPUT parameter is admitted by the grammar and crashes the backend** | **closed 2026-09-20**, M-declared-extents step 19 | **The guard asked the TYPE and had never asked the POSITION**, and the note beside it had named the position rule since the mark shipped. One condition, one function, the same diagnostic code | `selfhost/check/freer.hero` · `tests/golden/check/fixedbugs-owned-on-a-by-value-parameter.hero` | 170

    **Origin:** panel 170's spec-warden, 2026-09-20, reproduced by that sitting's
    completeness critic and again by the coordinator before filing.

    ## The repair

    `spec § 13` has always said `owned` belongs *"after a `cstr` result or a
    `char **` out-parameter"*, and `wrong_owned_type` asks whether the type is a
    `cstr`. **A by-value `cstr` is a `cstr`, so it walked straight past** — and
    `handed_back_param` then retyped the parameter to `str?` anyway, which is why
    the reproducer's argument has to be `ok("12")` to reach `check` at all.

    **The same diagnostic code and not a sibling**, because it is one question —
    *does this mark say anything the compiler can act on?* — and
    `check/lending.hero`'s reasoning at defect 067 is that **a second name for
    one question is a panel path**. The code's name records the half it was
    written for; its note has always covered both.

    **And no fix, deliberately.** The repair puts a `@` in front of the
    parameter's NAME and the diagnostic points at the MARK, so any replacement
    would land in the wrong place. `.claude/rules/diagnostics-and-goldens.md`: a
    fix that leaves the defect standing is a guess at best. The note names both
    routes instead, which is what design.md §4.17 asks of it.

    ## The measurements

    | | before | after |
    |---|---|---|
    | the reproducer | `check` **0**, `build` **2**, `internal error: compiling the generated C failed` | **`check` 1**, `error[owned_needs_cstr]`, caret on the mark |
    | `examples/ledger/main.hero`, which writes `@error: cstr owned sqlite3_free` | `check` 0 | **`check` 0, unchanged** |
    | `tests/golden/run/ffi-owned-cell-is-freed.hero`, which writes `@out: cstr owned free` | `check` 0 | **`check` 0, unchanged** |

    The last two rows are the cheapest evidence the repair is narrow, and they
    were the evidence that diagnosed it: **every `owned` parameter in this
    repository is already a cell.**

    ## The third round of one shape, and the file records the other two itself

    `check/freer.hero`'s own comments carry them. `unknown_freer` asked whether
    the NAME resolves and nothing asked what the mark was **on** — repaired
    2026-09-13. Then it asked the PARAMETER and not the RESULT — defect 036,
    repaired 2026-09-15, and that comment says *"the guard existed and watched
    the wrong half, which is why the repair is one call site and a parameter
    rather than a new refusal"*. **Each time the note beside the guard already
    named the rule the guard was not asking.**

    ## The gates

    The whole golden tree, because a change to what the checker refuses is judged
    by every one of them: `check` **131**, `annotations` **170**, `canonical`
    **2**, `fixes` **25**, `unsupported` **15**, `run`, `emission`,
    `determinism`, `corpus`, `records`, `layout`, `order`, the compiler's own and
    the net's own — all 0 failed. The seed was regenerated in the same commit and
    the fixpoint verified byte-identical.
