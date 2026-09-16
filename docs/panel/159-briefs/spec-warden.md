# Panel 159 — spec-warden brief

Read `docs/panel/159-briefs/00-shared.md` first. You judge §1.6's budget and
§1.2's cost formula, and you have a veto on budget breach. **This sitting is
mostly yours**: every candidate answer is a spec sentence and nothing else.

Measured 2026-09-16: `cl100k_base` **5989**, `real` **7974**, ceiling **10240**,
headroom 2266 / 2206 net of the FFI floor.

## Price all three, and price the MERGES against the additions

For each of the three — `sort`'s direction, `xs[i] @ v`, `main`'s result — draft
two versions: the shortest free-standing sentence, and the shortest merge into a
sentence that already exists. Measure each with `./heroes measure <draft>` on a
whole-document copy in your scratchpad, as the ledger's own method requires.
`spec-shape.md` records that merging beat appending at panel 122 on three
measured drafts; test whether it holds again here.

**And price the removals.** §1.6's payment rule is unconditional: each addition
owes a named removal or a registered prediction naming an instrument that
exists. Say what the removal is for each, or say the prediction and which
instrument scores it. A prediction naming an instrument that does not exist is
what panel 118 recorded and must not be repeated.

## The question that may be yours alone

**Is `sort`'s direction already stated?** § 11 says `sort` orders *a number,
`str` or `bool`*; § 10 says `for k in sort(keys(m))` walks them *in order*. Does
"in order" pick a direction for a reader, or does it only promise determinism?
Grep the document for every occurrence of "order" and decide. If it is already
stated, the cost is zero and the sitting's headline question dissolves.

**And the one that decides R2.** § 5's `Place` production is
`ident { "." ident | "[" Expression "]" }`, and § 5's statement list has
`Place "@" Expression`. **So the grammar may already derive `xs[i] @ v`.** If it
does, the document permits the form and only the PROSE omits it — which is a
different and cheaper repair than adding a rule. Derive it or refute it by hand
and say which.

## Your verdict owes

R1-R4, the measured deltas for six drafts (three free-standing, three merged),
the named removal or registered prediction for each, a falsifiable prediction
with its instrument, your condition, what you left UNRUN, and whether you cast
your veto.

Write to `docs/panel/159-reports/spec-warden.md` **first**.
