# Panel 229, the historian's brief

Read `00-shared.md` beside this file first (repaired after the critic's first
pass, `../229-reports/critic-briefs.md`). Your seat is advisory and has no
veto; its value is precedent that is **verified**, each claim with the page
that states it, read today, and marked `verified` only where you read the
page itself. An unsourced precedent is inadmissible.

## The question, in the terms other projects met it

An instrument generates program variants exhaustively, one per place a
comment or a line break can stand, and judges each by formatting it and
comparing two readings. On ordinary files it ends; on one test seed of 29
lines, 33,016 bytes and 32,068 tokens, holding chains 2,000 long written as
one statement, a single variant cost the formatter up to 31 s, because the
formatter's cost on a comment inside such a chain grew faster than linearly
with the chain (a repair landed today took that one variant to 10 s). And
the whole run is single-threaded per invocation, about 8 hours of wall clock
for one root even without that seed. The project must keep the run (it found
real formatter defects, every one in the line-break family) and make it end.

Find how others met each half, verified:

- **a formatter's cost on long or deep input**: how gofmt, rustfmt,
  Prettier, Black, clang-format, ocamlformat or dartfmt handled superlinear
  formatting time on long chains, long argument lists or deep nesting, and
  whether any bounds input or bails out (a documented line-length or
  expression-size cutoff, a timeout), and what each paid;
- **bounding or partitioning an exhaustive variant generator**: AFL's
  deterministic stages and effector map, libFuzzer's and AFL++'s handling of
  large inputs, mutant sampling and selective mutation (Offutt, Wong and
  Mathur and later), what fraction of power a sample was measured to keep;
- **property-based testing and reduction**: QuickCheck's and Hypothesis's
  size parameter, C-Reduce's and Hypothesis's shrinking;
- **spreading such a run over cores or machines**: how a large deterministic
  test or fuzz campaign is sharded (by input, by stride, by seed) and kept
  reproducible.

## What your report states

For each precedent: what the cited page itself says, the link, the day read,
`verified` or not; then which route in `00-shared.md` it supports or argues
against, and any route it suggests that nobody listed. Write
`docs/panel/229-reports/historian.md` in the lane as you go. No prediction is
owed from your seat, but one a run could falsify is welcome.
