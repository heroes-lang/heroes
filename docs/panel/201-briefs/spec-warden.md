# Panel 201, spec-warden's brief

Read `00-shared.md` beside this file first. Measure the spec in your copy with
`./heroes measure spec/heroes-spec.md` (the vendored maximum; no `--refresh`,
which is a paid run) before and after each draft you price.

- **Q1 (488)**: draft the sentence each reading needs: the spec's as it
  stands (the compiler moves, 0 tokens) or a narrowing written into § 9 (*a
  generic function value takes its type only from ...*), priced; Principle 0's
  burden for each (design.md §1.0, §1.6).
- **Q2 (467)**: draft a rewording of line 22 that a reader of a module file
  cannot take as *every file holds `main`*, priced; and say whether the two
  readings of 12 (measurement 040) meet the burden a sentence owes, or the
  extra `main` is harmless and the sentence stays.
- **Q3 (520)**: does spec `:280`, *Recursion too deep aborts.*, or any other
  sentence change if the rule follows cycles? Panel 199's R4 wrote no
  sentence.

Report: verdict per question, the deltas on the vendored row, the budget left,
the drafts whole, and what you could not run.
