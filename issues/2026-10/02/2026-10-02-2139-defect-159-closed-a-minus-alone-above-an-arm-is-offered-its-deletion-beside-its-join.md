# Defect 159 closed: a `-` alone above an arm's pattern is offered its deletion beside its join, two guesses

- [x] **159 — a `-` alone on the line above an arm's pattern is joined to it with certainty, though deleting it is as likely a reading** | `k = match n` over a line holding only `-` and then `1 => "one"`, `_ => "many"`, with `n = 1`: `continuation_outside_brackets` with the certain fix *write the statement on one line*, and `check --apply` writes `-1 => "one"`, which checks clean and prints `many`; deleting the `-` gives `1 => "one"` and prints `one` | the lexer's join of a line ending in an operator (`continuation_outside_brackets`'s certain fix) · design.md §4.8 (*two readings make two guesses*) · defect 135's class, closed 2026-10-02 · **class: blocking** · **closed 2026-10-02**

    **Origin:** lane arm's first pass, 2026-10-02
    (`scratchpad/lane-arm/pass1/r6/u14_lexer_split_int.hero`), raised again
    by lane recovery-b8 as a question; reproduced by the coordinator before
    16:39, the filing commit's time, on the trunk at `6c4da49b`
    (`docs/panel/186-briefs/probes/coordinator/u14_minus_above_an_arm.hero.txt`,
    kept as text since it does not parse, which is the defect).

    **Why it is a defect.** A `certain` fix is machine-applicable
    (`.claude/rules/diagnostics-and-goldens.md`), and this one writes a
    program that checks clean and means something else where another
    reading is as likely; panel 185's R6 made the same two readings two
    guesses for a spaced `-` on the arm's own line.

    **2026-10-02, lane h158, a `-` alone above an arm is offered its
    deletion beside its join, two guesses where both are programs**:
    repaired at `c936bd28` (`selfhost/sign_above.hero` and
    `selfhost/join_fix.hero`, called by `selfhost/open_line.hero`), gated by
    its cases, two `fixedbugs-159-*` goldens and the five that pinned the old
    join, each corrected under its header; the net is owed at the batch's
    close.

    **Class: blocking**, 2026-10-02 (the author's *D1a*,
    `.claude/rules/verification.md` § Bounded discovery): a `certain` fix that
    writes a program meaning something else.

    **Closed 2026-10-02** at the round of 2026-10-02's third gate, `6bec7c8c` (lanes bounded, h158 and land186), on this Mac: the seed regenerated once, 38,648,442 bytes, SHA-256 beginning `568bce290b6ed3bb`, its fixpoint by `cmp`; the compiler's own tests 1,056 and the net's own 197, all passed; the full net, 26 suites, 4,752 passed and 0 failed at each suite's last run; the censuses of `check --brief` over 1,835 files and of `--emit-c` over 570, every move attributed to its lane. The trunk took the gate's tree by fast-forward at 21:34 by `date`. Re-read by the coordinator on `6bec7c8c` at 21:39 by `date`: `continuation_outside_brackets` with two guesses, *write the statement on one line* and *delete the `-`*, and `check --apply` leaves the text as it is (`docs/panel/186-briefs/probes/coordinator/u14_minus_above_an_arm.hero.txt`). Not a C-boundary defect, so it closes at the Mac's gate (`.claude/rules/verification.md` § The batch); Linux arm64 and the Windows box run before the push.
