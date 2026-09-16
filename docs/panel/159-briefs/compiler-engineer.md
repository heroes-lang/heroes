# Panel 159 — compiler-engineer brief

Read `docs/panel/159-briefs/00-shared.md` first. You judge the ceiling and
implementation cost, and you have a veto on soundness. **This sitting is mostly
the document's**, so your seat is here for three narrow questions the others
cannot answer.

1. **Is `sort` ascending by construction or by accident?** Find the comparator
   the runtime dispatches on and say whether ascending is a decision written
   somewhere or a property of the sort it happens to call. If a later change to
   the sort could reverse it silently, that is a fact the document's sentence
   must be robust to, and it changes what the sentence should say.
2. **Is `xs[i] @ v` a supported form or an accident of the `Place` production?**
   § 5 derives `Place = ident { "." ident | "[" Expression "]" }`. Check what the
   checker and the emitter actually do with an indexed place on an array —
   including the copy-on-write question `spec § 10` raises for `push` through a
   field, which is the neighbouring shape. Measure whether `xs[i] @ v` on an
   array reached through a FIELD copies the whole array, because if it does, the
   sentence the document owes is longer than one clause.
3. **`main_returns` is a refusal with a good message.** Read
   `selfhost/value_errors.hero`'s `main_returns` and say whether the language
   could accept `-> ()?` at all — what would `main` do with a failure, and does
   the runtime have a path for it. If accepting it is cheap, the sitting has a
   fourth option nobody listed: change the compiler rather than the document.

**And the standing question**: what would have to be true for a route nobody
listed to exist? CLAUDE.md § RUN IT says a recommendation is a claim about the
option set.

Your verdict owes R1-R4, measured costs where there is code to measure, a
falsifiable prediction with its instrument, your condition, what you left UNRUN,
and whether you cast your veto. Write to
`docs/panel/159-reports/compiler-engineer.md` **first**.
