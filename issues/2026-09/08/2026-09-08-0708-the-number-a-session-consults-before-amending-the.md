- [x] **017 — `heroes measure` overstates the spec's usable room by 60 tokens** | the number a session consults before amending the spec is 60 larger than the number the net goes red at, and both are printed by this repository | `selfhost/cli/measure.hero:157` · `tests/harness/suite_spec.hero:203`

    **Origin:** the spec-warden seat, panel 118, 2026-09-08, which said "the warden's own instrument overstates usable room by 60 tokens — file it"; verified by the coordinator by reading both sites, 2026-09-08.

    **Reproducer**, run 2026-09-08 from the seed-built compiler:

        $ ./heroes measure spec/heroes-spec.md
          maximum            3903   the binding number
        Above the soft 2000: an addition needs a named removal or a
        pre-registered falsifiable prediction (panel 012). Headroom: 193.

    **The wrong answer at exit 0.** `selfhost/cli/measure.hero:157` prints
    `CEILING - highest`, which is 4096 - 3903 = 193. The net's own check is
    `SPEC_TOKENS + FFI_FLOOR >= CEILING` at `tests/harness/suite_spec.hero:203`,
    with `FFI_FLOOR` 60, so the largest green `SPEC_TOKENS` is 4035 and the real
    room is **132**. A session that trusts the tool it is told to trust can spend
    193 and find out from a red suite, which is the failure the tool exists to
    prevent.

    **Cause.** `measure` is a general instrument and knows nothing of a
    spec-specific mortgage; the mortgage lives only in the harness. So the
    number is right for any other file and wrong for the one document whose
    budget it was built to settle.

    **What is owed.** Not obviously the tool: making `measure` subtract a
    harness constant couples the compiler to the test suite. The cheaper repair
    is for `measure` to name the mortgage where it knows the file, or for the
    printed line to stop calling itself headroom. Either way it is a **class**
    and not a witness: any second claim on the same ceiling has the same shape.
    A `tests/harness/` case that asserts the two numbers agree is the instrument.

    **The repair, 2026-09-08, M-discard-refusal step 4.** `measure` stops
    printing a subtraction of its own and states the SUM this check compares:
    *"Headroom: 131 against the 4096 ceiling — but the FFI floor mortgages 60 of
    it (panel 030 R3), so what is measured against the ceiling is 4025 and the
    check goes red at 4096."* The mortgage is a constant in
    `selfhost/cli/measure.hero` beside the ceiling it qualifies, and it is named
    only for the spec, because saying it of every file would be a claim about a
    ceiling those files do not share.

    **The repair's own first version was wrong, in the direction the defect is
    about.** It printed *"and 71 usable"*, computed as `CEILING - FFI_FLOOR -
    highest`. The harness goes red at `SPEC_TOKENS + FFI_FLOOR >= CEILING`, so
    the largest green addition is **70**, not 71: a "usable room" figure is off
    by one the moment that `>=` is read as `>`. Caught by comparing the printed
    number against the check's actual condition rather than against the
    intention. Stating both sides of the comparison leaves no arithmetic for
    anybody to get wrong, which is why that is what shipped.

    **The instrument is `spec/spendable`** in `tests/harness/suite_spec.hero`,
    which asks that the sum `measure` prints is the sum the suite computes, and
    it carries a test that makes it fire, with a stub compiler that prints a
    drifted sum. The `spec` suite goes 9 checks to 10 and the net's own tests
    121 to 122.
