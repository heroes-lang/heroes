# Panel 209, the spec-warden's brief

Read `00-shared.md` first, then this. Your folder is
`.claude/worktrees/scratch-b15/209-spec-warden/`, a detached worktree of the
trunk at `87794631`; run no git in it. Your report is
`docs/panel/209-reports/spec-warden.md`, written as you go.

## The numbers, every one from a command run today

| document | real (claude-opus-5) | claude-legacy | cl100k_base |
|---|---|---|---|
| trunk `87794631` | 9847 (refreshed 2026-10-09, digest current) | 7348 | 7479 |
| B1, the operator alone | **9844** | 7356 | 7487 |
| B2, B1 and the never-re-bound sentence | **9865** | 7372 | 7503 |
| round b18's spec, `lane-round-b18` at `f6528c53` | 10021 (refreshed 2026-10-10 in that worktree) | | 7622 |

The two drafts are `.claude/worktrees/scratch-b15/209-coordinator/measure/spec-B1.md`
and `spec-B2.md`; `diff spec/heroes-spec.md <draft>` shows every changed
line (eight for B1, nine for B2; the critic's count). The real rows came from `heroes measure
spec/heroes-spec.md --refresh` run in a folder holding only
`vendor/tokenizers/` and the draft at `spec/heroes-spec.md`, the key in the
environment, exit 0 each. The ceiling is 10240 with the FFI floor
mortgaging 60; against the trunk the headroom is 333, against the round 159.
**The vendored tables and the reader's disagree in sign on B1** (+8 against
−3), which is `.claude/rules/spec-shape.md`'s warning in one row.

## Your question

1. **Is B1 the cheapest correct statement of R1, and B2 of R1b?** Write the
   drafts you would land, each measured on the vendored tables in your copy
   (`heroes measure <file>`); **you cannot refresh** (no paid run; and
   `--refresh` prices only the real path), so write each number as *vendored,
   a lower bound of unknown sign* and name the coordinator's real rows as the
   anchor. Where a sentence of yours merges into one already there, say which.
2. **What the document loses with the type**: today's `v: i64 @ 0` shows a
   reader the cell's type on its line; under R1 the reader infers it as for
   `=`. Spec § 5 `:136` *Signatures are always explicit; inference is local
   only* and design.md §4.5 (heading `docs/design.md:1142`, *Exactly two
   cases are not* inferable at `:1150`, both empty containers) already govern
   `=`: does R1 need a sentence about a cell born narrower than a later write
   (`b @= 255` then `b @ some_u8`, which today's compiler refuses as
   `type_mismatch` at the mutation, the critic's probe), or does the
   mutation's type error already say it? Measure the sentence if one is
   needed. **And B2's sentence must count a write through an `@` argument as
   a re-binding** (`db: Db @ nullptr` re-bound only through `@db`,
   `tests/harness/suite_special.hero:419`): draft and measure that wording.
3. **The instruments**: the `named` and `rejected` checks of
   `tests/harness/suite_spec.hero` read every code span and every indented
   production; `@=` is not a word. Run `heroes run tests/harness/main.hero --
   ./heroes spec` in your copy with B2 in place of the spec (your copy's
   compiler does not read `@=`, so say which rows go red and which of them
   would go green once the lexer does) and `grammar` likewise. `heroes
   grammar` prints the productions from the document (`selfhost/cli/grammar.hero:14-17`).
4. **The other routes' price**: a draft each for R2 (`=@`), R3 (`@@`), R5 (a
   keyword, which also enters the `named` check's reserved words), R6 (type
   mandatory on both: § 5 loses *type inferred* and §4.5's two cases, and
   every example line of the document that writes `x = e` without a type
   gains one: count them with `grep -cE '^[[:space:]]*[a-z_][a-z0-9_]* = ' spec/heroes-spec.md`
   inside fences, 4 by the critic's reading), each measured vendored. The
   `spec` suite with B2 in place read 19 passed and 4 failed in the critic's
   copy, the four being the count and pin rows: confirm, and say what the
   `grammar` suite reads with B2.
5. **Principle 0's burden** (design.md §1.0): R1 adds no capability; it
   enters only on a measured thesis effect. Say what measurement would
   discharge it, naming an instrument that exists (`docs/metrics/`, the
   mutation operators of `docs/metrics/operators.md`, the blind seat's six
   sessions), and what result would falsify it.

Verdict per route, a falsifiable prediction with the instrument that scores
it, the condition. Veto on a breach of the ceiling against the round's 10021
or on a draft that states a rule in two homes.
