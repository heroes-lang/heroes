# Panel 149 — brief for the spec-warden

Read `docs/panel/149-briefs/00-shared.md` first. You judge the indicator —
design.md §1.2's cost formula and §1.6's budget — and Principle 0's burden of
proof. You have a veto on a budget breach.

## The count as it stands, measured this session

`./heroes measure spec/heroes-spec.md`:

```
  claude-legacy      5862   (64995 ranks)
  cl100k_base        5988   (100256 ranks)
  maximum            5988   a lower bound, not the reader's tokeniser
  spread              126
  real               7974   claude-opus-5, 2026-09-14 — the binding number
```

Ceiling **10240**, raised from 8192 by author decision 2026-09-14. Headroom
**2266**, of which the FFI floor mortgages 60 (panel 030 R3). The binding number
is `real`, judged by `tests/harness/suite_spec.hero`'s `contract` check.

The three marks `acquires`, `consumes` and `borrows` cost **+125 vendored and
+168 real**, in two payments at ratios of 1.40 and 1.23.

## The question that is yours, and it has a prior form

**Does this repair cost a spec token at all?** CLAUDE.md § 12 says spec beats
compiler: where the document already states the rule, the compiler has the bug
and the repair is free. The sentence in § 13 is:

> `acquires sqlite3_finalize` after a handle result or `@` out-parameter says
> the call begins that handle's life and names the one that ends it, which the
> program owes it. … `borrows` says the call hands back one it keeps, and where
> a group consumes a handle type every call handing one back says which it is.

Two readings, and the sitting needs you to say which the document supports:

- **It already requires the mark.** *"every call handing one back says which it
  is"* — a call returning a `Pair` that holds a `Slot` hands a `Slot` back. On
  this reading the compiler has the bug, nothing is added, and the cost is zero.
- **It does not.** *"after a handle result"* names the position of the mark and
  a `Pair` result is not a handle result, so the document's own rule stops where
  the compiler stops.

Do not split the difference. Say which reading the text supports, quote it, and
say what a reader would conclude.

## What to measure, and measure it rather than estimating

1. **Count the candidate texts offline.** `heroes measure <file>` counts any
   file, so write each draft to its own file and count it. At minimum:
   - the document unchanged;
   - the document with *"after a handle result or `@` out-parameter"* widened to
     reach a handle through a record's fields;
   - the document with a sentence answering the multi-handle record of R3
     (a record reaching two handle types, where the mark names one releaser).

   Report `real` for each, and the delta. Use `--refresh` only on the real
   document and only if you must; say if you did.

2. **Price the REFUSAL as well as the permission.** If R3 is settled by refusing
   a returned type that reaches more than one handle type, that refusal is a
   sentence too, and design.md Part 6 asks a refusal to name the program fact
   that would make it wrong. Count that sentence.

3. **Merging beats appending** (`.claude/rules/spec-shape.md`, panel 122: three
   drafts measured, the merged one cheapest). For every addition you price, try
   the merged form into a sentence already there, and report both.

## Principle 0

Does this enter v1 because the compiler needs it, or because it provably serves
the thesis? Say which, and if neither, say it waits.

## Predict something falsifiable

One prediction with the command that would settle it.
