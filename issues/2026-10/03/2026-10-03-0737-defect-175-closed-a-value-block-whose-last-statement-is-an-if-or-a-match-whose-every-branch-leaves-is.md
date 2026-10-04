# Defect 175 closed: a value block whose last statement is an `if` or a `match` whose every branch leaves is refused `no_value`, where panel 185's R4, ratified, makes the block leave and its arm a jumping arm

- [x] **175 — a value block whose last statement is an `if` or a `match` whose every branch leaves is refused `no_value`, where panel 185's R4, ratified, makes the block leave and its arm a jumping arm** | lane flow's `a55-value-arm-block-inner-returns.hero`: `check` exit 1, `no_value` at 4:13, *this `match` produces no value — every branch jumps, so there is nothing to bind*; under R4 it checks clean (its source's own expected output, `1`, `2`, `20`, unrun) | spec § 8 `:229-230` (*A jump (`return`, `break`, `continue`) is a valid arm body*) · panel 185's R4 (`docs/panel/185-a-macro-is-named-as-a-macro-an-arm-takes-a-statement-a-leaving-block-leaves-and-a-spaced-sign-has-two-readings.md:169-170`, ratified at `:241-245`; its sentence goes in § 8, `:171-175`) · **class: blocking** · **closed 2026-10-03**

    **Origin:** panel 185, ratified 2026-10-02 at 03:10 (`docs/records/done/2026-10-02-0310-panel-185-ratified-a-macro-named-as-a-macro-an-arm-of-one-statement-a-leaving-block-a-sign-with-two-readings.md`); its landing found unwritten by the coordinator's file-queue agent, 2026-10-02 (`scratchpad/file-queue/unlanded-184/`, 2026-10-02), and re-read on `6bec7c8c` by the coordinator's re-verification agent (2026-10-02, `scratchpad/file-queue/reverify-6bec7c8c.txt`). The sitting predicts *c1, c2, c3, c6 and a55 to b8 (b4, b7 excepted) build and run* (`:231`).

    **2026-10-03, lane flow4, panel 185's R4**: repaired at `937ae12f`
    (`check/walk.hero`'s `valued_tail`, `check/path_end.hero`'s
    `withdrawn`), gated by its own cases, `fixedbugs-175-*` under `check/`
    and `run/`, lane flow's a55, a69, a73, b1, b2, b3, b5, b6 and b8 among
    them; spec § 8's sentence landed in the lane's spec commit; the rest is
    owed at the round's gate.

    **Class: blocking**, 2026-10-02 (`.claude/rules/verification.md` § Bounded discovery): by the ratified language, a correct program refused.

    **Closed 2026-10-03** at the round of 2026-10-03's gate, `b43d224d` (lanes fbrace, flow4, rec187 and cb4), on this Mac: the seed regenerated once, 39,266,477 bytes, SHA-256 beginning `bdf53918a7df9c36`, its fixpoint by `cmp`; the compiler's own tests 1,079 and the net's own 200, all passed; the full net, 26 suites, 4,901 passed and 0 failed at each suite's last run; the censuses of `check --brief` over 1,876 files and of `--emit-c` over 576, every move attributed to its lane; the site's build green. The trunk took the gate's tree by fast-forward. Its repair, lane flow4's `937ae12f`, panel 185's R4; its own cases re-run on the trunk at `b43d224d` between 07:26 and 07:36 by `date`: `check` 1 of 1, `annotations` 1, `fixes` 1, `run` 1 of 1, all passed. Not a C-boundary defect, so it closes at the Mac's gate (`.claude/rules/verification.md` § The batch); Linux arm64, the Windows box and Linux under clang 18 run before the push.
