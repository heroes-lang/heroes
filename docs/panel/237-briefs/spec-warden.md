# Panel 237, the spec-warden's brief

Read `00-shared.md` and `proposal.md` beside this file first. Your worktree is
`.claude/worktrees/237-spec-warden/`, detached at `163912031`; build your compiler there as
`00-shared.md` says. You judge the indicator (design.md §1.2's cost formula, §1.6's budget) and
Principle 0's burden of proof (CLAUDE.md § 2), with veto power on a budget breach.

## The counts

`00-shared.md` gives the vendored counts of fences A, B, C and D, made with `./heroes-next measure`
over drafts that change only the fence's `sqlite3` lines (`diff` against the specification prints one
line for B, two for C and three for D). Re-make them in your worktree. **The real count** of B, C
and D is `heroes measure spec/heroes-spec.md --refresh` with the fence applied to the
real path and reverted after (`.claude/rules/spec-shape.md` § How a change to the document is made;
`--refresh` refuses any other path), which sends the text to Anthropic's token-counting endpoint
with `ANTHROPIC_API_KEY` from the trunk's `.env` (`. /Users/joseph/Temp/heroes/heroes-lang/.env` in
your worktree; print `${#ANTHROPIC_API_KEY}`, never the value). **Those requests are a paid run in
this project's terms, and the author said yes on 2026-11-08 at 11:44** (a cap of 25 USD for the
sitting's paid runs in all): make the four refreshes, A to confirm the pinned 10904 and B, C and D,
reverting the specification after each, and name every request in your report.

## What the sitting needs from you

1. Question 7 on the indicator: B's, C's and D's price, and what each moves beyond tokens (C breaks
   `special`'s fence program and the two `examples/` bindings' agreement, `00-shared.md`), against design.md §1.6's payment rule (a named
   removal, or a registered prediction naming an instrument that exists), and the spec's headroom
   (`heroes measure` prints it).
2. Whether items 1 and 2 owe the specification any text (it names no diagnostic code today,
   `00-shared.md`), or design.md alone (§4.17 for the class and its words, §4.19 for the C boundary):
   draft the design.md sentences question 8 asks, and count them.
3. Principle 0 on item 1: a refusal is no new form, and CLAUDE.md's Precedence, ranks 3 and 4, lets a
   measured class of wrong programs close at once while a new form needs § 2's ticket (panel 215):
   say whether anything in items 1 to 3 is a form, and what its ticket would be.
4. The two thesis contexts: what a change to their bytes would cost, read from
   `tests/thesis/contexts/README.md` and `tests/thesis/held-out/prediction.md` (the registered block
   names the `spec` context alone; the two are exploratory, `00-shared.md`).

Your report: `docs/panel/237-reports/spec-warden.md` in your worktree, written as you go, ending as
`00-shared.md` § Rules says, with your prediction registered before any real count comes back.
