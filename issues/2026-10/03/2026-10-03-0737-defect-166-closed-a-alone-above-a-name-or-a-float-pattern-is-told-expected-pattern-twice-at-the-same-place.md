# Defect 166 closed: a `-` alone above a name or a float pattern is told `expected_pattern` twice at the same place

- [x] **166 — a `-` alone above a name or a float pattern is told `expected_pattern` twice at the same place** | `k = match n` over a line holding only `-` and then `x => "one"` (or `1.5 => "one"`): `continuation_outside_brackets`, then `expected_pattern` twice at the name or the number, the same text at the same column | the pattern's refusal after the join of a `-` line (the same on the compiler before lane h158) · **class: adjacent** · **closed 2026-10-03**

    **Origin:** lane h158's first pass for defect 159, 2026-10-02
    (`scratchpad/lane-h158/d159/s18_name_below.hero`, `s19_float_below.hero`);
    reproduced by the coordinator at 18:37 by `date` on the trunk at
    `4b44f684` (`docs/panel/186-briefs/probes/coordinator/adjacent/`).

    **Why it is a defect.** One mistake told twice at one place (design.md
    §4.17).

    **2026-10-02, lane h158, where the deletion is the one repair the `-` is
    left out of the stream, and the name or the float is told once**:
    repaired at `8cb4ba6c` with defect 165, gated by its case
    `fixedbugs-166-a-minus-alone-above-a-name-or-a-float-is-told-once` and
    the compiler's own tests; the net is owed at the batch's close. The
    second message was a failed arm's recovery, `opening.drop_line`, which
    stops at a line of the text inside a line the lexer joined and reads
    the part below the break again as an arm: `1 |` over `x => "one"` still
    meets it, before this repair and after it, and is reported apart.

    **Its cause found 2026-10-02** by lane h158 (its repair of the `-` shapes,
    `8cb4ba6c`, keeps them away from the cause, not repaired):
    `parse/opening.hero`'s `drop_line` stops a failed arm's recovery at the
    first word of a line of the text even inside a line the lexer joined
    (the stop defect 131 added at `9811d4cd`, for tab margins), so the part
    below the break is read again as an arm and told a second time. Shapes
    that still read two `expected_pattern` at one place, before and after
    `8cb4ba6c`: `1 |` over `x => "one"`, `1 |` over `1.5 => "one"`, `"a" |`
    over `x => "one"`, `1 | -` over `x => "one"`
    (`scratchpad/lane-h158/d166/`). The cause is the recovery cluster's file,
    so it goes with 130 and 131 to the sitting on what a finished recovery
    is.

    **2026-10-03, lane rec187, panel 187's R4: `opening.drop_line` deleted,
    a failed arm's line dropped whole, a line the lexer joined to it
    included** (the four shapes above and nine beside them with its cause, a
    statement `match`, a `match` in an arm, a returned one and a chain of
    lines among them): repaired at `29425af6`, gated by its own cases; the
    rest is owed at the round's gate. The five control-arm messages the
    drop no longer tells are defect 193.

    **Class: adjacent**, 2026-10-02 (`.claude/rules/verification.md` §
    Bounded discovery): a second message for one mistake.

    **Closed 2026-10-03** at the round of 2026-10-03's gate, `b43d224d` (lanes fbrace, flow4, rec187 and cb4), on this Mac: the seed regenerated once, 39,266,477 bytes, SHA-256 beginning `bdf53918a7df9c36`, its fixpoint by `cmp`; the compiler's own tests 1,079 and the net's own 200, all passed; the full net, 26 suites, 4,901 passed and 0 failed at each suite's last run; the censuses of `check --brief` over 1,876 files and of `--emit-c` over 576, every move attributed to its lane; the site's build green. The trunk took the gate's tree by fast-forward. Its repair, lane rec187's V1 (`29425af6`), panel 187's R4; its own cases re-run on the trunk at `b43d224d` between 07:26 and 07:36 by `date`: `check` 2 of 2, `annotations` 2, `fixes` 2, all passed. Not a C-boundary defect, so it closes at the Mac's gate (`.claude/rules/verification.md` § The batch); Linux arm64, the Windows box and Linux under clang 18 run before the push.
