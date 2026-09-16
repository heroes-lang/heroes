# Panel 155 — spec-warden brief

Read `docs/panel/155-briefs/00-shared.md` first.

You judge the indicator — design.md §1.2's cost formula and §1.6's spec budget —
and Principle 0's burden of proof. You have a veto on budget breach.

## The measured count, run 2026-09-16 with `./heroes measure spec/heroes-spec.md`

```
spec/heroes-spec.md
  claude-legacy      5863   (64995 ranks)
  cl100k_base        5989   (100256 ranks)
  maximum            5989   a lower bound, not the reader's tokeniser
  spread              126
  real               7974   claude-opus-5, 2026-09-15 — the binding number
```

Ceiling **10240**, raised from 8192 by author decision 2026-09-14. Headroom
**2266**, of which the FFI floor mortgages 60 (panel 030 R3), so **2206** net.

This is the real count, not an estimate, so your verdict need not be provisional
on that ground.

## What the proposal would and would not cost in spec tokens

This is the part you are asked to decide rather than accept. The coordinator's
position is that the proposal adds **no new surface**: no keyword, no syntax, no
new type. It changes which programs are refused. Test that claim.

The specific question: **§ 13 already says a `partial` record's comparison is a
compile error "for it and for any value holding it".** Today it is not, through a
generic. So one of two things is true and you are asked which:

- the compiler is wrong and the specification needs **zero** new tokens, because
  the sentence already promises the behaviour the proposal would deliver
  (CLAUDE.md § 12: spec beats compiler, the compiler has the bug); or
- the specification is wrong as written, and if the proposal is refused the
  sentence must be **qualified** — which costs tokens to say *except through a
  generic*, and that qualification is a cost of REFUSING the proposal, not of
  adopting it.

Price both directions. A refusal that costs spec tokens and an adoption that
costs none is the inverse of the usual shape here, and if that is what the
measurement says, say it plainly.

Do the same for the map rule. § 3's table gives `{K: V}` and § 10 gives the map
operations; find whether any sentence promises anything about which types may be
keys, and if none does, say what the silence means — CLAUDE.md § RUN IT: *a
silence in the spec is often a ruling*, and `docs/records/log/` and `docs/panel/`
are where to check whether this one is.

## Principle 0's burden

`grep -rnE '^function [a-z_]+<' selfhost/` is **0**: the compiler uses no
generics, so the closure list does not need this. Under Principle 0 the proposal
therefore enters only if it **provably serves the thesis** — a measured
design.md Part 11 effect, or a measured argument this panel accepts. State what
that argument would have to be and whether the brief supplies it.

You may read the repository. Measure any draft sentence with
`./heroes measure <file>` on a draft in your scratchpad — it counts any file
offline. Build in a copy if you build at all; the seed is
`clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`, a few seconds.

## Your verdict owes

A verdict per R1-R4, the measured token delta in **both** directions (adopt and
refuse), a falsifiable prediction naming the instrument that would score it, and
the condition under which you would change your vote.

Write your report to `docs/panel/155-reports/spec-warden.md`.
