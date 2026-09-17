# Panel 161 — spec-warden

Read `00-shared.md` in this directory first. This file is your input only. You
have a veto on budget breach, and you carry Principle 0's burden of proof.

## The budget, measured this session

`./heroes measure spec/heroes-spec.md`, run 2026-09-17 while this brief was
written:

```
  claude-legacy      5871   (64995 ranks)
  cl100k_base        5997   (100256 ranks)
  maximum            5997   a lower bound, not the reader's tokeniser
  spread              126   between the two vendored tables (2%)
  real               7984   claude-opus-5, 2026-09-17 — the binding number
```

Ceiling **10240**. Headroom **2256**, of which the FFI floor mortgages 60
(panel 030 R3), so what is measured against the ceiling is 8044.

**A vendored delta is not a price** — author instruction 2026-09-16, in three
words: *always measure with the real*. The two instruments disagree by roughly a
third, so a draft priced at `+11` offline may cost fifteen on the instrument
design.md §1.6 names. `--refresh` refuses every path but `spec/heroes-spec.md`
and `CLAUDE.md`, so a draft is priced one of two ways and there is no third:
apply it to the real path, `--refresh`, revert; or write the number down as a
**lower bound, in those words**. `ANTHROPIC_API_KEY` is in the repository's
`.env` (`. ./.env`); without it `--refresh` exits 2, and an unavailable
instrument is *unrun*, never a licence to promote the vendored figure.

**The working tree is frozen for this sitting.** If you price a draft by
applying it, do so in a copy in your scratchpad, not in the repository.

## What you are asked

**Price each of the four routes in real tokens, and apply Principle 0's burden
to each.** A form enters v1 if the compiler needs it (the closure list) or it
provably serves the thesis — a measured design.md Part 11 effect, or a measured
argument this panel accepts. Neither, and it waits.

The routes are in `00-shared.md` § The routes named so far. For each, write the
spec diff you think it would need, price it, and say whether Principle 0 admits
it. Be specific about which section takes the text: § 13 is FFI, § 3 is the type
table, and `.claude/rules/spec-shape.md` says every rule has exactly one home,
the section of the operation it governs.

## The three questions that decide your verdict

1. **Does any route need spec text at all?** Route 3 — accept `i8` or `u8`
   against a plain `char` — arguably changes only what the compiler accepts, and
   § 13's sentence *"declared at the header's own width and sign"* already
   describes a rule it would now break. Say whether that sentence would become
   FALSE under each route. A specification that no longer describes the compiler
   is the failure your seat exists for.

2. **What does route 1 or 2 cost in § 3's type table?** That table is read by
   `site/src/lib/tables.ts` and by the `named` and `rejected` checks. A ninth
   row is not free and a type usable only inside `extern` may not belong in that
   table at all — which is itself an argument about where the rule lives.

3. **Is there a named removal available?** An addition owes one, or a registered
   falsifiable prediction naming an instrument that exists. If you can find text
   in § 13 that a resolution would make redundant, that is the cheapest honest
   route and it is worth more than a small diff.

## The thing to resist

The cheapest route is 3, and it is cheapest in your unit. CLAUDE.md
§ Precedence puts robustness above token cost and says to take the most robust
resolution, never the cheapest. Your verdict should say what route 3 costs in
tokens **and** that the token cost is not what decides it — or, if you think the
budget genuinely binds here, say so plainly and be ready to be overruled by
rank 3.

## Deliver

Verdict · the section it rests on · the real token count of each draft you
priced, or the words *lower bound* where you could not · a falsifiable
prediction with its milestone · any budget veto.
