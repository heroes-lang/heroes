- [x] **panel 155** | The call-site obligation pass is refused and `==` on a bare type parameter is refused in the BODY instead, by the language's own written answer; `float_map_key` through a generic waits under Principle 0 | **RATIFIED 2026-09-16, as adopted, BY DELEGATION** | `docs/panel/155-the-hole-was-never-made-by-the-generic.md`

    **The yes is the assistant's, under an authority the author handed over, on a
    sitting the author has not read.** The instruction, 2026-09-16, in full:
    *continue until the step is finished with zero defects and zero open
    decisions — ratify the panels for me — and at the end push everything and
    check that CI is green on all three systems.* This entry says so rather than
    writing *the author ratified*, because crediting them with a reading that did
    not happen is the same falsehood wearing the flattering sign, and it is the
    one nobody would check (`.claude/rules/records.md`).

    **What the yes settles.** That a body refusal is this language's answer to an
    operation a type parameter may not support — `design.md:1721` says it in so
    many words, *"If an operation on `T` is needed, pass it as a parameter"*, and
    the shipped `sort` diagnostic already recommended it. That `spec § 13` is now
    true as written, at **+0** spec tokens, because the compiler was the thing
    with the bug. That the call-site obligation pass is refused on **soundness**
    and not on price: it needs a second span-keyed table against the standing rule
    at `selfhost/check/state.hero:78-85`, measures 284 code lines against panel
    082's estimate of 90-110, has no node-to-declaration map to build on, and
    reproduces the defect 035 shape in its own walk.

    **What it does not settle.** `float_map_key` through a generic stays open and
    is a position rather than a defect: no compiler need, no spec sentence, and
    the body rule there would delete two goldens. `spec § 10` still owes a
    sentence naming which types may key a map — the llm-ergonomist's condition,
    owed whichever way R1 went. And the depth-bound hole is defect 046, which is
    what the sitting found was actually broken.

    **What landed on it the same day**, so the tick is not read as a plan:
    `generic_equality` in `selfhost/value_errors.hero`, wired at
    `selfhost/check/ops.hero`, with `tests/golden/check/equality-on-a-type-parameter.hero`
    and its three controls; the full net **1892 passed, 0 failed**; and
    `selfhost/discard_errors.hero`, the family the `layout` check forced out of
    `value_errors.hero` when the new diagnostic took it past its ceiling.

    **The conservative resolution is untaken rather than rejected**, and it costs
    nothing to keep available: R2 landed as a body rule, so reversing it is a
    deletion. Panel 155's own file records what it would have been.
