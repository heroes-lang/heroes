# Defect 165 closed: an operator alone above an arm's pattern is offered the deletion of its line, and the arm is read without it

- [x] **165 — a `+` or `*` alone on the line above an arm's pattern gets a join that no pattern takes, and a second message** | `k = match n` over a line holding only `+` (or `*`) and then `1 => "one"`: `continuation_outside_brackets` with the guess *write the statement on one line*, which writes `+1 => "one"`, then `expected_pattern` at the `+`; no deletion of the line is offered | the join of a line ending in an operator (lane h158's `sign_above.hero`, beside defect 159) · **class: adjacent** · **closed 2026-10-02**

    **Origin:** lane h158's first pass for defect 159, 2026-10-02
    (`scratchpad/lane-h158/d159/s09_plus.hero`, `s12_star.hero`);
    reproduced by the coordinator at 18:37 by `date` on the trunk at
    `4b44f684` (`docs/panel/186-briefs/probes/coordinator/adjacent/`, kept as
    text since they do not parse).

    **Why it is a defect.** The one fix offered writes a program that is
    refused anew, and one mistake costs two messages (design.md §4.17).

    **2026-10-02, lane h158, an operator alone above an arm is offered the
    deletion of its line alone, `certain`, and the parser reads the arm
    without it**: repaired at `8cb4ba6c` (`selfhost/sign_above.hero`'s
    `offer`; `open_line.goes_on_at_head`, where such a line no longer goes
    on with an arm above it), gated by its case
    `fixedbugs-165-an-operator-alone-above-an-arm-offers-its-deletion` and
    the compiler's own tests; the net is owed at the batch's close.

    **Class: adjacent**, 2026-10-02 (`.claude/rules/verification.md` §
    Bounded discovery): a second message for one mistake and a guess that
    does not compile; no wrong value and no false message.

    **Closed 2026-10-02** at the round of 2026-10-02's third gate, `6bec7c8c` (lanes bounded, h158 and land186), on this Mac: the seed regenerated once, 38,648,442 bytes, SHA-256 beginning `568bce290b6ed3bb`, its fixpoint by `cmp`; the compiler's own tests 1,056 and the net's own 197, all passed; the full net, 26 suites, 4,752 passed and 0 failed at each suite's last run; the censuses of `check --brief` over 1,835 files and of `--emit-c` over 570, every move attributed to its lane. The trunk took the gate's tree by fast-forward at 21:34 by `date`. Re-read by the coordinator on `6bec7c8c` at 21:39 by `date`: one message, `continuation_outside_brackets` with the `certain` deletion of the `+` (and of the `*`), and the applied program checks clean and prints `one` (`docs/panel/186-briefs/probes/coordinator/adjacent/s09_plus.hero.txt`, `s12_star.hero.txt`). Not a C-boundary defect, so it closes at the Mac's gate (`.claude/rules/verification.md` § The batch); Linux arm64 and the Windows box run before the push.
