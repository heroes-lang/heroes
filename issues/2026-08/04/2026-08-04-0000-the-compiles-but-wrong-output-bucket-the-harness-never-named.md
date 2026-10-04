- [ ] **M-thesis-harness** | the compiles-but-wrong-output bucket the harness never named | `docs/panel/009-spec-budget-2000.md:100-103` · `design.md:2851-2853` · `selfhost/mutate/score.hero`

    **Origin:** panel 009's llm-ergonomist, as a condition on its own vote
    2026-08-04. Its home since 2026-09-04, when the panel watch list was
    retired; the condition is the seat's own and it is a condition on the
    instrument, so it belongs to the milestone that builds it.

    **The harness must report a compiles-but-wrong-output bucket, separate from
    the compile-error bucket, and today nothing names one.** The seat's words:
    without it *"the prediction is untestable and the raise unjustified on this
    axis"*, and its scoring line adds *"only if the compiles-but-wrong bucket
    exists"*. **What exists instead, measured**: Part 11 states two gradings,
    *compile* rate and *tests-pass* rate (`design.md:2851-2853`) — which yields
    the bucket by subtraction and never names it, and `git log -S` dates that
    text to 2026-08-03, the day **before** the sitting, so it is not an answer
    to the condition. `selfhost/mutate/score.hero`'s `survived` is a
    compiles-but-presumed-wrong class over mechanical mutants, which is **metric
    3 and not this**: it never compares program output. The golden runner does
    compare output, so the capability is in the tree and the reporting class is
    not.

    **Also owed and cheap**: the sibling condition from the same vote — that
    Part 11 report *class-weighted* numbers — survives in **one** place in this
    repository, the sitting itself, which is the shape §11 calls a premise that
    expires in silence.

    **Where to look also:** `docs/panel/009-spec-budget-2000.md:23` ·
    `tests/harness/suite_run.hero`.
    **Why it matters:** the thesis's central claim is that this language deletes
    SILENT errors, and the instrument that would show it has no column for them.

    **Re-verified 2026-09-10: STILL OPEN, and its own one-place count is
    confirmed.** `class-weighted` survives in exactly **one** place, `docs/panel/009`,
    and *"compiles-but-wrong"* appears nowhere in any instrument — only in that
    sitting, in `docs/panel/010:118`, in the narration and in this item.
    `selfhost/mutate/score.hero`'s `.survived` is still over mechanical mutants
    alone. One pointer moved: Part 11's two-gradings sentence is
    `design.md:3136-3138`, not `:2851-2853`.
