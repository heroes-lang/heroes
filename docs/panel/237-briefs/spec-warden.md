# Panel 237, the spec-warden's brief

Read `00-shared.md` and `proposal.md` beside this file first. Your worktree is
`.claude/worktrees/237-spec-warden/`, detached at `163912031`; build your compiler there as
`00-shared.md` says. You judge the indicator (design.md §1.2's cost formula, §1.6's budget) and
Principle 0's burden of proof (CLAUDE.md § 2), with veto power on a budget breach.

## The counts

`00-shared.md` gives the vendored counts of fences A, B and C, made by the coordinator with
`./heroes-next measure` over drafts that change only the two `sqlite3` lines (`diff` against the
specification prints one line for B and two for C). Re-make them in your worktree. **The real
count** of B and C is `heroes measure spec/heroes-spec.md --refresh` with the fence applied to the
real path and reverted after (`.claude/rules/spec-shape.md` § How a change to the document is made;
`--refresh` refuses any other path), which sends the text to Anthropic's token-counting endpoint
with `ANTHROPIC_API_KEY` from the trunk's `.env` (`. /Users/joseph/Temp/heroes/heroes-lang/.env` in
your worktree; print `${#ANTHROPIC_API_KEY}`, never the value). **Those requests are a paid run in
this project's terms and wait for the author's yes, which the coordinator relays to you by
message**: until it comes, price B and C on the vendored tables, write the real row as unrun, and
say what the vendored numbers cannot decide. Each request you make is named in your report.

## What the sitting needs from you

1. Question 7 on the indicator: B's and C's price, against design.md §1.6's payment rule (a named
   removal, or a registered prediction naming an instrument that exists), and the spec's headroom
   (`heroes measure` prints it).
2. Whether items 1 and 2 owe the specification any text (it names no diagnostic code today,
   `00-shared.md`), or design.md alone (§4.17 for the class and its words, §4.19 for the C boundary):
   draft the design.md sentences question 8 asks, and count them.
3. Principle 0 on item 1: a refusal is no new form, and CLAUDE.md's Precedence, ranks 3 and 4, lets a
   measured class of wrong programs close at once while a new form needs § 2's ticket (panel 215):
   say whether anything in items 1 to 3 is a form, and what its ticket would be.
4. The two thesis contexts: what a change to their bytes would cost the registered measurement that
   pins them (`tests/thesis/contexts/README.md`), read from that file.

Your report: `docs/panel/237-reports/spec-warden.md` in your worktree, written as you go, ending as
`00-shared.md` § Rules says, with your prediction registered before any real count comes back.
