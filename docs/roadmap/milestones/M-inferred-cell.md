# M-inferred-cell — a mutable cell declared by its own symbol, `@=`, and typed by its value

**Opened 2026-10-10 by the author's ratification of panel 209** (17:57, in
conversation, recorded as a reading in
`issues/2026-10/10/2026-10-10-1736-panel-209-ratify-amend-or-overturn-a-cell-declared-by-its-own-symbol.md`),
with the order *push, then wait for batch 19*: the lane opened at 19:01, after
session heroes-lang-98 reported `lane-round-b19:main` on origin and its four
lanes quiet. Row 84 of the chain. The sitting is
`docs/panel/209-a-cell-is-declared-by-its-own-symbol-and-typed-by-its-value.md`;
its § The resolution and § The landing are this page's reasoning, not
restated here. The name is the coordinator's default, two words for the
deliverable, the inferred cell; the author may rename it before the first
tag, and nothing cites it but this page, the chain and the issues' cards.

**What it delivers.** Three commits in the order the sitting measured, each
gated by its own cases and the compiler's own tests, the batch gated once:

1. **A cell nothing re-binds is a compile error** (`never_rebound`), with a
   certain fix writing `=` where the `@` is, the annotation kept; a write by
   `@` to the name, a field or an element, or through an `@` argument, is a
   re-binding; the rule is silent on a name offered as a repair and on a
   cell whose only `@` a construction refused; a cell never read keeps
   `unused_binding`. And the mutation of a name nothing binds says how a
   cell is declared (R9). **Landed at step 1**, with the tree's 391 cells
   rewritten `=` in the same commit (156 in the compiler, 121 of them in
   `test` blocks; 18 in the examples; 5 in the harness; 212 in the goldens).
2. **The `@=` token** declares a cell, `v: T @= e`, the type kept (R10): the
   lexer's table, the parser, the printers, the formatter, the probe's
   reader, `heroes mutate`, the editors' grammars; `v: T @ e` refused with a
   certain fix; `(@=x` one message with a certain fix; the three ceilings'
   seams; and the whole tree rewritten in the same commit, since `canonical`
   is red between the token and the rewrite.
3. **The type optional on `@=`** and inferred from the value as `=`'s is;
   `cannot_infer` where the value cannot say it, a bare `nullptr` included,
   told at the declaration with a `guess` fix and the cell poisoned; the
   width class's `type_mismatch` gaining a note and a fix; `v @= v + 1` one
   message. No annotation the tree holds is stripped: the inference is for
   programs written from now on.

Then the documents and the instruments: the spec's text W1b-min (9850 real
by the sitting's refresh), design.md §4.4 and §4.5, `docs/metrics/operators.md`,
the site's 25 lines, `keywords.hero`'s messages, the seed.

**What it must not do.** Rewrite history (`merge, not rebase`); strip an
annotation; land the token without the rewrite; move a ceiling without a
seam or a DECIDED row that says why; push without the author's yes, a push
touching `site/` publishing the site.

**The measurements step 1 made** (every number its command's, the commit
body carries them): `layout` 6 passed, 0 failed with `grammar_expr.hero` one
line longer; the rule's eight shapes probed; `check --apply --in-place`
writes the root module only and notes the others' fixes, so the 391 were
applied from `check --json`'s byte spans by a one-off in the lane's scratch,
and the instrument the landing owes is a task of this milestone.

**Predictions to score at the batch's gate** (the sitting's table): `layout`
0 failed with the knots at or under their ceilings; a second `--apply` moving
nothing; 0 `never_rebound` in the census; the mutation operators' rows; the
ffi seat's 6 and 26 annotated `@= nullptr` cells and 0 bare.
