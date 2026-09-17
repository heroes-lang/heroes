# Panel 160 — brief for the spec-warden

Read `00-shared.md` first. Your seat is design.md §1.6 (the budget, the
instrument, the unconditional payment rule) and §1.2. You have a veto on budget
breach.

## The instrument, and the rule that changed last night

**A vendored delta is not a price** — `.claude/rules/spec-shape.md` § How a
change to the document is made, author instruction 2026-09-16. At panel 159 you
found that `heroes measure <draft>` reaches only a lower bound, because
`--refresh` refuses every path but `spec/heroes-spec.md` and `CLAUDE.md`. That
was the eighth brief correction, and the rule now says what a seat does about
it: **apply the draft to the real path in a copy, and `--refresh` there.**

So, in your scratchpad copy of the tree:

```
cd <your copy>
set -a; . ./.env; set +a          # loads ANTHROPIC_API_KEY; prints nothing
./heroes measure spec/heroes-spec.md --refresh
```

That path is the one `--refresh` accepts, and inside the copy it is your draft.
**Never print the key** — a hook refuses any command that expands it into the
transcript. Print `${#ANTHROPIC_API_KEY}` if you need to know it loaded.

Panel 159's package measured **+15 vendored and +24 real**, a ratio of 1.60. The
vendored figure understated by 60%. Report BOTH numbers for every draft, and if
`--refresh` fails, say UNRUN and give the vendored one as a lower bound in those
words.

## Base, measured 2026-09-16 after panel 159 landed

`claude-legacy 5879 · cl100k_base 6004 · real 7998 (claude-opus-5) · ceiling
10240 · 2242 free, 2182 net of the FFI floor`. Digest `7a8fb4400c7ed444`.

## Drafts to price

1. Panel 158's unlanded R3 sentence, the ergonomist's draft merged into § 6's
   opening or § 10's map sentence — *"`T` may itself be fallible — `m[k]` on a
   `{str: i64?}` is an `i64??` — and every operation below peels one."* You
   priced the cheapest honest merge at **+9 vendored** then. Take the real.
2. The same sentence with the refusal named: *"… `match` names both levels;
   `.is_err()` asks only the outer one and is refused on such a value."*
3. A sentence for option A (all four readers refused) if it differs from 2.
4. **Whether option B needs a sentence at all.** § 10 says `m[k]` is a `V?`. If
   `{K: V?}` is refused at the declaration, is § 10 already true and complete,
   or does it owe the refusal one clause?

## The payment question

design.md §1.6: an addition owes a named removal or a registered falsifiable
prediction naming an instrument that exists today. Panel 159 spent the one −14
duplicate you found. **Is another true duplicate left?** You named § 7's *so
`x != x` asks whether it is one* at −12 last time and called it content. Look
once more, and say what you find or that you found nothing.

## What you must check about the ledger

`docs/measurements/010-spec-budget-ledger.md` row 78 landed last night and is the
newest. Your prediction from panel 158 — *"with a +0 resolution, `measure
--refresh` reads 7974 / 5989"* — is now unscorable as written, because panel 159
moved both numbers first. Say so in your report so the synthesis can lapse it
honestly rather than score it against the wrong base.

## Report

`docs/panel/160-reports/spec-warden.md`, via `cat >`. Verdict · section ·
`spec_token_delta` with both instruments · removal · needed_for_self_hosting ·
prediction · condition.
