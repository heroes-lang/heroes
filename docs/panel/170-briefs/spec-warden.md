# Panel 170 — brief for the spec-warden

Read `docs/panel/170-briefs/00-shared.md` first.

## Your axis, and the two predictions of yours this milestone has scored

design.md §1.2 and §1.6, and Principle 0's burden of proof.

At panel 167 you registered *"routes A, B and C each close zero of two
reproductions when landed alone"*. Panel 168 **scored it CORRECT** on the
repository's own shipped example, after two sittings and a defect entry had said
the opposite. At panel 169 you found the missing lifetime sentence and priced
it; it landed today at **+47 real**, and the draft you were not shown cost +55.
This sitting asks for the same kind of finding a third time.

## The measured baseline, run while this brief was written

`./heroes measure spec/heroes-spec.md`: **real 8201**, vendored **6159**, ceiling
**10240**, headroom **2039**, FFI floor 60. Digest `6c3a27eb1b8ac830`. `.env` is
present, so `--refresh` works: apply a draft to the real path, measure, revert.
Do it in a COPY.

`DELTA_GATE` is **50 and it is measured in VENDORED tokens**
(`tests/harness/suite_spec.hero`), which the ledger's last two rows both state.

## The questions

1. **Price the mark as a `spec § 13` diff**, in real tokens, in each candidate
   spelling, and say for each what sentence it CHANGES rather than only what it
   adds. Merging beats appending by 36%, measured at panel 122 and again today.
2. **What does it buy per token?** A mark that closes one defect and leaves three
   is priced differently from one that closes four.
3. **Is there a sentence already in § 13 that, read strictly, already says what
   the mark would say and is simply not enforced?** You found exactly that at
   panel 169 for `spec § 3`'s no-aliasing clause, and it was the cheapest finding
   of the sitting. Look again.
4. **Principle 0.** `grep` for real call sites: does the compiler itself need
   this? At panel 169 you measured zero `.ptr()` and zero `.lease()` in
   `selfhost/`. Re-run it, and say which branch of Principle 0 the mark enters on.
5. **Register a falsifiable prediction**, and make it the kind that gets read.

Report to `docs/panel/170-reports/spec-warden.md`. Veto on budget breach.
