# Panel 209, the compiler-engineer's brief

Read `00-shared.md` first, then this. Your folder is
`.claude/worktrees/scratch-b15/209-compiler-engineer/`, a detached worktree
of the trunk at `87794631`; run no git in it. Your report is
`docs/panel/209-reports/compiler-engineer.md`, written as you go.

## Your question

**Build R1 far enough that its numbers are real**, in your copy: the lexer
reads `@=` as one token where `=` follows `@` with nothing between; the parser
builds today's `declare` node from `name [: Type] @= value`, the type optional
and inferred from the value as `bind` does it, `cannot_infer` firing where it
fires for `=`; the formatter prints the new form; `v: T @ e` becomes a parse
error with a certain fix; and R1b, a cell nothing re-binds refused with a fix
that writes `=`. Then compile the tree with it (see *Migration*) and report
what moves.

## Where it lives today, by command

- **Tokens**: `selfhost/token.hero:157,179,205,219` carry `.eq` and `.at` in
  their arms, `:261` `.at => "at"`; the punctuation is read in
  `selfhost/punctuation.hero`, `punct_at` at `:26`, longest match first (the
  critic's reading), which is where `@=` joins beside `.colon_colon`,
  `.and_and`, `.shl`. `bump(@=n)` then lexes `@=` whole where today it reads
  `@` then `n`: the form owes a message with a fix.
- **The parser**: `selfhost/grammar_expr.hero:1010` returns
  `.declare(name:, ty:, value: parse_expr(...))`; the file stands at its
  DECIDED ceiling, `"selfhost/grammar_expr.hero 1085"`
  (`tests/harness/suite_layout.hero:477` and following, the table), and the
  ceiling's unit is the suite's, not `wc -l` (1767). `selfhost/parse/annotation.hero:54`
  looks for `.at` or `.eq` at depth 0 to find where an annotation ends.
  `selfhost/parse/at_prefix.hero` (263 lines) reads `@v`, `@@v` and `@return`
  as a sigil mistake with a certain fix (`:63`, its goldens
  `tests/golden/check/fixedbugs-131-*`, `fixedbugs-179-*`): say what `@=`
  does to that recovery, and what `v @= 0` written today costs
  (`expected_expression` at the `=`, measured; the shared brief's probes).
- **The AST**: `selfhost/ast.hero:60-70`, `bind {name, ty: i64?, value}` and
  `declare {name, ty: i64, value}`, the comment above `declare` quoting §4.4;
  ceiling `"selfhost/ast.hero 550"`. Under R1 `declare`'s `ty` becomes `i64?`
  or the two nodes become one with a `mutable` flag: price both.
- **The resolver**: `selfhost/resolve/binding.hero:13-30`, `record Binding`
  with `mutable`, `bind_binding(ty: i64?, ...)`; where `declare`'s type is
  taken as written, and where `cannot_infer` is raised for `=`.
- **The printer**: `selfhost/print/fmt.hero:755` (declare) and `:770`
  (mutate), both `head: lead + " @ "`; ceiling `"selfhost/print/fmt.hero
  1175"`; `selfhost/print/bodies.hero:55,63` (`--dump` rendering);
  `selfhost/print/scopes.hero:110`; `selfhost/ir/print.hero:158`.
- **The grammar**: `heroes grammar` prints the productions it reads from the
  spec (`selfhost/cli/grammar.hero:14-17`; `Binding   =` exists only in
  `spec/heroes-spec.md:150`), and `tests/harness/suite_grammar.hero` compares
  the compiler's `keyword` and `binary_op` arms to what it prints; say whether
  `@=` enters a table that suite reads.
- **The `@` argument** (spec § 9, `Arg = [ ident ":" ] [ "@" ] Expression`):
  6607 sites in `selfhost/`, 500 in `examples/`, 4 in the spec
  (`grep -rhoE '(\(|: |, )@[a-z_]'`, the critic's pattern); untouched by R1,
  and the lexer must keep `(@x` and `, @x` as they are.

## Measurements owed

1. **Lines changed and where**, per file, with each file's position against
   its ceiling (`heroes run tests/harness/main.hero -- ./heroes layout` in your
   copy, narrowed to the file with the harness's third word).
2. **The never-re-bound rule (R1b)**: how many cells in `selfhost/` and
   `examples/` it refuses today, counted by your prototype, and what each is
   (a `=` in disguise, or a cell written only through an `@` argument, which
   spec § 5 `:138` already counts as a use and which the rule must count as a
   re-binding: `db: Db @ nullptr` then `open(@db)`,
   `tests/harness/suite_special.hero:419`). Say whether the rule is the
   `unused_binding` walk extended or a new one.
3. **Inference on cells**: with the annotations stripped where the first value
   says the type, how many of `selfhost/`'s 5926 declarations the prototype
   still refuses, and the classes (the shared brief's 1559 `[]`/`{}`/`fail(`/`ok(`
   lower bound; the cell born narrower than a later write, whose literal case
   is `type_mismatch` at the mutation today). The error's site and its fix
   for the second class. **1190 of the 5926 stand inside `test` blocks**, so
   the count is taken by `heroes test selfhost/main.hero` as well as by the
   build.
4. **Migration**: the rewrite of 1671 `.hero` files under CLAUDE.md § 10
   (never a script, never a second binary), and beyond them 32
   `.applied`/`.fixed` goldens, 839 lines of
   `tests/golden/recovery/plan-singles.jsonl`, 25 lines of `site/src`, 19 of
   `docs/design.md`, the editors' TextMate grammar
   (`heroes.tmLanguage.json:130`) and the metric's operators
   (`docs/metrics/operators.md:13-14`, which name the mandatory type as the
   catching rule). Price the two-step landing: one commit where the parser
   reads both forms and `heroes fmt` prints the new one, so `heroes fmt
   --in-place` over the tree migrates it and the goldens' `.expected` columns
   are re-read (23 files quote a declaration); then the old form refused.
   Name every suite the change owes (`.claude/rules/verification.md` § What
   gates what: a change to what the checker refuses is judged by every
   golden tree) and the seed's regeneration.
5. **Time**: `heroes build selfhost/main.hero` before and after, `/usr/bin/time
   -p`, machine as still as the lanes allow; report the ratio and discard a
   waiting run.
6. **R2 to R11 in the lexer and the parser**: a sentence each on what differs
   from R1 in cost or in a hole it opens (`=@` beside the `@v` recovery when a
   space is missing, and already a planted mutant in `plan-singles.jsonl`;
   `@@` beside `at_prefix`'s doubled sigil; `:=` beside `: Type`; a keyword
   beside `Statement = ident Binding`; R6's cost on the 10225 inferred `=`
   lines; R7 against `op=` habits; R8's walk over every write against §4.5's
   locality; R9's message alone, two `unknown_name` for one mistake today;
   R10 the symbol with the type kept; R11 refused by the recovery already).

Verdict per route, prediction with its instrument, condition. Veto where a
route breaks soundness (§1.12) or cannot be built inside the ceilings without
a seam the language forbids.
