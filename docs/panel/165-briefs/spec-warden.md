# Panel 165 — brief for the spec-warden

**Read `docs/panel/165-briefs/00-shared.md` first.** Everything in it was run.

## The baseline, measured while writing this brief

```
$ ./heroes measure spec/heroes-spec.md
  claude-legacy      5965   (64995 ranks)
  cl100k_base        6089   (100256 ranks)
  maximum            6089   a lower bound, not the reader's tokeniser
  real               8106   claude-opus-5, 2026-09-18 — the binding number

Headroom: 2134 against the 10240 ceiling — but the FFI floor mortgages 60 of it
(panel 030 R3), so what is measured against the ceiling is 8166.
```

**Note the trap this session already fell into and record it if you hit it.** The
`heroes` binary is `.gitignore`d. A stale one carries a stale pin and reports
`STALE: the recorded count is for <old digest>`, which reads as *the document
moved* when the truth is *your compiler is old*. Rebuild from the seed (about
3 s) before you trust any number `measure` prints.

## Your question

Route 6 adds one admissible parameter type to `spec § 13`. **Price the wording**,
and rule on whether §1.6's payment is met.

`.claude/rules/spec-shape.md` binds you and is worth re-reading on one point: the
**real** count is the binding one, `--refresh` refuses every path but
`spec/heroes-spec.md` and `CLAUDE.md`, and it exits 2 with no `ANTHROPIC_API_KEY`.
So either apply a draft to the real path, `--refresh`, and revert; or write your
number down as a **lower bound, in those words**. A vendored delta is not a price.

Draft at least two wordings and price each alone:

1. a **merge** into the existing sentence that already lists what a parameter may
   be (`.claude/rules/spec-shape.md`: merging beats appending, panel 122 measured
   three drafts and the merged one was cheapest);
2. a standalone clause that also states the checking rule and the
   platform caveat.

## The question that is yours and nobody else's

Principle 0 says a form enters v1 if the compiler needs it **or** it provably
serves the thesis. The shared brief measures that **no working binding in this
tree would change** — 172 `extern` functions, one array-spelled hit, and that one
a diagnostic fixture. Route 3 already carries the crossing and is in the document.

**So state plainly whether Principle 0 is met, and if it is not, say what
measurement would meet it.** Panel 164's warden approved route 4 and objected to
route 3 on exactly this ground and route 3 was adopted anyway on robustness; say
whether the same reasoning transfers here or whether it does not, and why.

## The specific thing to weigh against the cost

Any wording that admits a fixed-array parameter probably has to say something
about the extent being the **header's**, because the shared brief measures
`L_tmpnam` at 1024 on Darwin and 20 on glibc. A clause that does not warn is
cheaper and leaves a reader to write a portable-looking declaration that is not.
Price both, and say which you would take.

## What your verdict must carry

A verdict (approve / object / veto), the exact token delta for each draft with
the instrument named and whether it is real or a lower bound, the ledger row you
would write, and **one falsifiable prediction**. You hold a veto on a budget
breach.
