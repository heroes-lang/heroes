# Panel 156 — spec-warden brief

Read `docs/panel/156-briefs/00-shared.md` first.

You judge the indicator — design.md §1.2's cost formula and §1.6's budget — and
Principle 0's burden of proof. You have a veto on budget breach.

## The measured count, run 2026-09-16

```
spec/heroes-spec.md
  claude-legacy      5863
  cl100k_base        5989
  maximum            5989
  real               7974   claude-opus-5 — the binding number
```

Ceiling **10240**, headroom 2266, of which the FFI floor mortgages 60, so 2206
net. This is the real count; your verdict need not be provisional on that
ground.

## The sentence that is already there, and whether it is kept

`spec § 6`: *"An abort ends the program at once, **saying why**; no `T?` carries
one."*

That is the whole of the language's promise about a stopping program, as far as
the coordinator could find. **Check that** — grep the document yourself and say
whether anything else bears on it, including § 11's `print` paragraph and § 13.

Then decide the question that makes this your seat's:

- **On Windows the program says nothing**, both streams empty. If `spec § 6`'s
  *saying why* covers a fault inside a C library, then the specification is kept
  on two platforms and broken on one, the compiler has the bug (CLAUDE.md § 12),
  and the repair costs **zero** spec tokens.
- **If it does not cover it** — if *"an abort"* means only the aborts the
  language itself raises, and a C library's fault is outside that — then the
  document is silent, the Windows behaviour is not a spec violation, and any
  promise about it must be **bought**. Price that sentence.

Both readings are available and the choice is yours to argue. Say which, and
give the token delta for the direction that needs one.

## Principle 0

`.claude/rules/platforms.md` says the three platforms are measured before the
commit and CI is the judge; that is process and not Principle 0. What you are
asked: does anything here enter the LANGUAGE, or is all of it runtime and
instrument? If the answer is that the specification should say more about what a
stopping program writes — which the llm-ergonomist is being asked
independently — then Principle 0 applies and the sentence needs a named removal
or a registered prediction.

## The second question, and it is about an instrument rather than the document

`.expected` for this fixture pins one platform's answer. Nothing in the tree
says which platform's answer is the criterion, and the fixture is green here and
red on two legs. **Is a golden that encodes a platform-specific answer a golden
at all**, and if not, what is the shape that replaces it? You have seen this
project's instruments closely; answer from what exists rather than inventing a
mechanism.

## Process

Draft any spec sentence in your scratchpad and measure it there with
`/Users/joseph/Temp/heroes-lang/heroes measure <draft>`. Build only in a copy.
**No command over ~60 seconds** — three of five seats died on the watchdog at
panel 155. Never read `archive/bootstrap-rs/`. Capture exit codes directly.

## Your verdict owes

A verdict per R1-R5 insofar as the document bears on them, the token delta in
both directions, a falsifiable prediction naming its instrument, your condition,
and whether you cast your veto.

Write your report to `docs/panel/156-reports/spec-warden.md` **first**.
