# Panel 170 — brief for the llm-ergonomist

**Read `spec/heroes-spec.md` and nothing else.** Not design.md, not the
repository, not the other briefs, not the other seats' reports, not earlier
panel files. Your verdict carries information precisely because your input is the
document a model is given to write Heroes and nothing more. If you find yourself
wanting repository context, that wanting is a finding: write it down instead of
satisfying it.

Your report is an **experiment**, not an opinion.

## What changed in the document since you last read it

§ 13 now contains one sentence it did not: *A lend lives for its call and no
longer: C keeping the pointer reads bytes the program may have changed or freed
since, and nothing checks it.* **You proposed it.** Reading the document again,
say whether it lands where you meant it and whether it says what you needed.

## The situation, stated without the repository

A C function may **keep** a pointer it is handed and read it later. The document
now says a lend does not live that long, and that nothing checks it. So a program
that needs C to keep bytes must do something else, and the document does not say
what.

## The proposal, as a spec diff, given blind

Two candidate additions to § 13's `CParam` line and its prose. Judge them
label-stripped; they are in no meaningful order.

> **W1.** `CParam` gains `[ "keeps" ident ]`. Prose: *A parameter marked `keeps
> end_fn` is one the call holds on to: a lend is refused there, `x: cstr @
> s.lease()` is what it takes, and `end_fn` and not `end_lease` is what ends
> that lease.*

> **W2.** The `extern` group's head line gains `retains`. Prose: *`extern
> "curl.h" link "curl" retains` says this library may keep what it is given: a
> lend is refused throughout the group, and a lease or a buffer C owns is what
> its functions take.*

## Your four tasks

**Task 1.** For each variant, write a program that binds a C function which
keeps a string, using only what the document contains. Report whether you could
on the first try and what you had to guess.

**Task 2.** For each variant, write the program the mark is supposed to make
impossible, and say whether the document tells you it is impossible.

**Task 3 — the one that decides your verdict.** The document says *nothing
checks it*. Under each variant, **what does a reader believe about the UNMARKED
case?** Specifically: a C function that keeps the pointer and whose declaration
carries no mark. Does the mark's existence make a reader trust the unmarked case
MORE than they do today? That is the question the sitting needs and no other seat
can answer it.

**Task 4.** Is there a third wording that does the job better than both? You
produced the sentence this milestone landed; you are the only seat whose input is
what a reader actually gets.

## What to report

A verdict on each variant (approve · object · VETO, with the ground); first-try
results as counts; what you had to invent; a falsifiable prediction about a
reader's behaviour, with a number.

Write to `docs/panel/170-reports/llm-ergonomist.md`. You hold a veto on non-local
constructs: a rule a reader must hold two files in mind to obey is one.
