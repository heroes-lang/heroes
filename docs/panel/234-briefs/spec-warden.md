# Panel 234, the spec-warden's brief

Read `00-shared.md` first, then `proposal.md`, `conventions-draft.md`,
`probe/README.md` and `blind/README.md`, with the materials under `blind/S/`
and `blind/Q/`, which stand complete when you start. You judge the indicator
(design.md §1.2's cost formula, §1.6's budget) and Principle 0's burden of
proof, and you have a veto on a budget breach. Your report is
`/Users/joseph/Temp/heroes/heroes-lang/docs/panel/234-reports/spec-warden.md`,
written as you go. **Your first deliverable is the reader test's predictions,
written at the top of your report before anything else**, because the
coordinator starts the sessions only once they stand there.

**The specification today**, `./heroes measure spec/heroes-spec.md` run on
2026-11-05 at f0c4c99d2: **8019** on the vendored tables (`cl100k_base`
8019, `claude-legacy` 7876), **10,514 real** on `claude-opus-5` as recorded on
2026-10-30, against the ceiling of **16384**; headroom 5870, of which the FFI
floor mortgages 60. **The real count is yours to take**: `heroes measure
spec/heroes-spec.md --refresh` reads it from the token-counting endpoint
(source the trunk's `.env` for `ANTHROPIC_API_KEY` and never print it; panel
232's warden was named for the same call). **Your worktree is
`.claude/worktrees/234-spec-warden/`**, made by the coordinator; build its
compiler from the seed (`00-shared.md`, *The tree*). `--refresh` refuses every
path but the literal relative paths of the specification and `CLAUDE.md`
(`selfhost/measure/pinned.hero:106-111`; `selfhost/measure/judged.hero:27-28`),
so a draft is priced for real by writing it into **your worktree's**
`spec/heroes-spec.md`, running `--refresh` from your worktree's root, and
restoring the file with `git checkout -- spec/heroes-spec.md` there (F25); a
card's count is the vendored one, written as a lower bound in those words
(`.claude/rules/spec-shape.md`).

## Your items

**The reader test's predictions, first.** **Before registering, read the
materials**: `blind/S/check.txt` shows the two `strings` variants and their
cards differ in four names alone; read both cards and both modules yourself
and say whether either card leans (F22: the coordinator wrote S, whose own
draft adopts H); and read `blind/Q/thin/card.md` and `blind/Q/thick/card.md`,
the ffi-pragmatist's. Then, for each comparison of `blind/README.md`
§ *The comparisons the sitting reads*, register, before any session runs,
**as shares** (F19: a cell is one arm on one task on one model, five
samples, and the README may cut samples to four or three before the run, so a
count would break): a point and a band for `right@1` and `right@2` per cell
or pooled over the two models, and the effect each comparison predicts, with
the rule that scores a hit or a miss. **Name the cells you expect at a
ceiling and at a floor** (F20: both readers right, or both wrong, show
nothing), as panel 232's warden did for its task O. Then the sessions run.
Panel 232's 80 sessions cost 1.5590 USD; say what you expect these 180 to
cost, for the projection the first sessions check.

**A and the frame: Principle 0's reach.** The milestone page says *Principle
0 binds nothing in this milestone; what binds it is §1.11*. Judge it: does a
package (no form, no built-in, no spec token) owe Principle 0 anything, and
does a package's function owe the admission test of `conventions-draft.md`
§6 in its place.

**E, the card's budget.** Measure `blind/S/card-H.md`, `blind/S/card-G.md`,
`blind/Q/thin/card.md` and `blind/Q/thick/card.md` with `heroes measure`
(vendored, a lower bound), and propose a budget per package and for the
whole tree of cards the milestone writes (the page's table names twenty
packages, F30), with what it is measured against: a card is read beside the
specification, so say what share of the specification's real count the cards
together may take and why.

**The specification's sentences each route would move**, priced: item F
(spec § 13 names `hero_os.h` as the language's; a platform layer beside it
would be named or not), item H (a `tag` on an `extern function`, and a
sentence naming the contextual words' family, if either enters), item I (§ 13
per group or per module), item J (`alias`, if it enters), item L (§ 11 saying
`write_file` replaces whole, which must carry the four routes on which it
writes in place, `runtime/parts/write.c:233`, `:241`, `:251`, `:259`, or be
false on them, F29), item M (`sort_by` in § 11's `Built-ins:`
sentence, against a package that costs no spec token). For each, the draft
sentence you would write, its vendored count, and, for the one or two the
sitting is likeliest to adopt, the real count by the route above, in your
worktree. **You do not edit the trunk's specification.**

**H and J and M, Principle 0's ticket.** For `tag` on an `extern function`
(issue 1), `alias` (issue 2209: does condition (b), met by the letter twice,
measure what it was meant to?), and a built-in `sort_by` (issue 1154): which
of the three tickets each holds, measured, or none.

**K, the repairs.** Read the shared brief's six findings and say for each the
sentence design.md should read; they are documentation and need no ticket.

**N, the measurement of the packages' effect** (question (xiii)). Design the
before-and-after the author authorised, with the thesis harness: which tasks
(a package task needs a program that would use a package; say whether the
held-out suite or the assistant's suite can serve or a new suite is owed), which
arms (the author said *before and after*; a paired run at the close, the same
compiler with and without the packages, is a route nobody listed until this
brief: weigh it), which models, the budget you would ask for, and the
prediction you would register, in the form `preregister` takes.

## Report shape

Predictions first. Then per item: **verdict** (approve, object, or veto, on
which route), **section**, **cost** (tokens, vendored and real, each named),
**prediction** (falsifiable, with when it is checked), **condition**.
