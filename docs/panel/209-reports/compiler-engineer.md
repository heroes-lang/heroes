# Panel 209, the compiler-engineer: R1 built far enough that its numbers are real

Started 2026-10-10 16:48:10 CEST (`date`). Folder:
`.claude/worktrees/scratch-b15/209-compiler-engineer/`, a detached worktree of
the trunk at `87794631` made by the coordinator; no git command run in it.
Compiler: `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes` in that
folder (its timing in § 1 below); after every edit to `selfhost/`,
`./heroes build selfhost/main.hero -o heroes` in the same folder. Every
`./heroes` below is that one, never the trunk's. Time box 45 minutes, to about
17:33; what is not reached is written as **unrun**, in that word.

Written as it goes, sections appended in the order the work ran. Every number
names its command.

## 1. The compiler, and the time it takes (measurement 5)

- Seed build in my folder: `/usr/bin/time -p clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`: `real 9.21 user 8.78 sys 0.22`, binary 16:48:25.
- **Before**, the pristine tree (a copy of `selfhost/` taken before any edit,
  `209-notes/selfhost-pristine/`), built by the seed compiler:
  `/usr/bin/time -p ./heroes-seed build 209-notes/selfhost-pristine/main.hero -o heroes-pristine`:
  `real 137.44 user 101.64 sys 23.95`, exit 0.
- **After**, the R1 tree, same compiler, run right after it with nothing else
  of mine running: `/usr/bin/time -p ./heroes-seed build selfhost/main.hero -o heroes-r1`:
  `real 128.55 user 97.10 sys 22.66`, exit 0. `uptime` at 16:58 read load
  averages `10.11 13.13 28.37` on this 8-core Mac: the lanes' load is in both
  numbers, the ratio real to user+sys is 1.09 before and 1.07 after, so
  neither run waited; the record's 41.6 s cold on a still machine says what
  the load costs. **R1 does not slow the build**: the after is 4.5 % under
  the before, inside the noise of a loaded machine.
- `./heroes check selfhost/main.hero` on the edited tree: exit 0, 0
  diagnostics, `real 12.99 user 11.32` (`209-notes/check-step1.txt`).

## 2. What R1 touched: the edits, by file (measurement 1, first half)

Step one of the two-step landing, built: the lexer reads `@=` as one token;
`name @= value` and `name: Type @= value` both build today's `declare` node,
its `ty` now `i64?`; `name: Type @ value` is still accepted in silence (the
migration's step), the refusal with its certain fix being step two (§ 6
below). All edits by `perl` in place, then `heroes fmt --in-place` on each;
the full diff against the pristine copy is `209-notes/diff-step1.txt`.

| file | what |
|---|---|
| `selfhost/token.hero` | `at_eq` added to `variant TokenKind` after `at` (line 61), `.at_eq => "at_eq"` in the name table, `.at_eq` beside `.at` in the three enumerated tables (`begins_an_expression`, `continues_a_line`, `of_a_pattern`) |
| `selfhost/punctuation.hero` | the two-byte match `'@' '='` → `Punct(kind: .at_eq, width: 2)` before the one-byte table (longest match first, as the critic placed it); two asserts, `@=` width 2 and `@ =` width 1 |
| seven enumerated `TokenKind` tables elsewhere | `.at_eq` beside `.at`: `brace_layout.hero`, `line_opens.hero`, `layout.hero`, `operand_families.hero` (4 arms), `next_line.hero`, `print/groups.hero`, `describe.hero` (its own arm, `` `@=` ``; the automatic rewrite had named it `` `@` ``, caught on review) |
| `selfhost/ast.hero` | `declare.ty: i64` → `i64?`, the §4.4 comment replaced |
| `selfhost/grammar_expr.hero` | `statement_kind`: `ident @=` → new `cell_stmt` (4 lines, `ty: fail("inferred")` as `bind_stmt` does); `annotated`: `eat(.at_eq)` → `declare(ty: ok(ty))`, `eat(.at)` kept for step one |
| `selfhost/parse/annotation.hero` | `symbol_after` also stops at `.at_eq` |
| `selfhost/resolve/binding.hero`, `resolve/walk.hero`, `resolve/state.hero` | `cell_binding(ty: i64?)`; the type resolved only when written; one test literal |
| `selfhost/check/walk.hero` | the `declare` arm mirrors the `bind` arm: written type lowered and checked against, else `synth`; the unit refusal copied |
| `selfhost/print/fmt.hero` | the declare head prints `name` or `name: T` and ` @= ` |
| `selfhost/print/bodies.hero` | the `--dump` rendering, `declare name[: T] @= ` |
| `selfhost/mutate/handles.hero` | the `ptr` sweep matches on the optional type as the `bind` arm beside it does |

**Not touched, and owed by the same change** (`.claude/rules/diagnostics-and-goldens.md`
§ A new surface form): `selfhost/probe/reader.hero` (built and `check`ed
clean, so it does not enumerate `TokenKind`; whether it counts `@=` as a
token `fmt` can neither add nor drop is unrun), `heroes mutate`'s operators
(`mutate/edits.hero:73` rewrites `name: T @ ` to `name = ` by span and keeps
working under `@=`; the operator table `docs/metrics/operators.md:13-14`
names the mandatory type as the catching rule and is prose), the TextMate
grammar and `site/src/lib/highlight.ts`, `selfhost/ir/print.hero:158` and
`print/scopes.hero:110` (read, neither prints a declaration's symbol).

## 3. What the R1 compiler says (probes, `./heroes-r1 run`, `209-notes/probes/`)

| program | verdict |
|---|---|
| p1 `v @= 0`, `v @ v + 1`, `print(v)` | exit 0, prints `1` |
| p2 `v: i64 @= 0` | exit 0, prints `1` |
| p3 `xs @= []` | exit 1, `cannot_infer` at the declaration, fix `guess` *annotate the binding: `xs: [i64] = []`*: **the fix names `=` for a cell**; the text is `contextless_errors.hero:18-28`, which knows neither the name nor the symbol, so the fix owes a `what` parameter (about 4 lines, plus the `=` caller) |
| p4 `v: i64 @ 0` (today's form) | exit 0 in step one, as designed |
| p5 `bump(@=n)` | exit 1, **one** message, `expected_expression` *found `@=`*, no fix (today: two messages, the ffi-pragmatist's p10) |
| p6 `v @ = 0` | exit 1, `expected_expression` at the `=`, as today |
| p7 `total @= 0` then `totl @ total + x` | exit 1, one `unknown_name`, fix `certain` *rename to `total`*; nothing else (R1b not yet in) |
| p8 `b @= 255`, `b @ big` with `big = 300` | exit 0, prints `300`: the cell is `i64`, which is what the author wrote; today's `u8` case needs the annotation and keeps it |
| p9 `n @= 0` then `n @ maybe(3)` (an `i64?`) | exit 1, `type_mismatch` **at the mutation**, *expected `i64`, found `i64?`*, fix `guess` `.must()`: the cell born narrower than a later write is told two lines from its birth, with a fix that changes meaning and no word of the declaration (design.md §4.17's cost of inference on a cell; §4.5 `:1144` *errors stay local* is kept by the letter, the error being in the block, and lost in spirit) |
| p10 `v @= 5`, `print(v)` | exit 0 (R1b not yet in) |
| p11 `db @= 0`, `open(@db)` | exit 0 |
| p12 `v @= nothing()` | exit 1, `unused_binding` (the resolver's sweep speaks before the checker's unit refusal) |

The blind seat's B programs (`209-blind/t1-B/report.md`, `t2-B/report.md`,
read only): **unrun under `heroes-r1` as of this section**, run in § 5.

## 4. The ceilings (measurement 1, second half)

`./heroes run tests/harness/main.hero -- ./heroes layout` with the R1 build
installed as `./heroes` (`209-notes/layout-after.txt`): **`layout: 5
passed, 1 failed`**:

    selfhost/check/walk.hero: 1875 lines of code, past the 1870 it measured when this check was written — a file already over the ceiling may not grow further
    selfhost/grammar_expr.hero: 1093 lines of code, past the 1085 it measured when this check was written — a file already over the ceiling may not grow further

The pristine tree reads `layout: 6 passed, 0 failed` with the seed compiler
(`209-notes/layout-before.txt`). So step one of R1 alone grows the two knots
by 8 and 5 lines in the suite's unit (`suite_layout.hero:955`, `code_lines`,
test blocks left out), and both are at their `DECIDED` ceilings
(`suite_layout.hero:479,482`). Neither seam is one the language forbids: the
`bind` and `declare` arms of `check/walk.hero:793-830` now share eight lines
(lower the written type and check against it, else `synth`; refuse unit),
which is one leaf helper, `check/bound.hero` or an arm of `check/lower`,
taking about ten lines out of the knot; and `cell_stmt` and `bind_stmt` in
`grammar_expr.hero` differ by the node they return, so one function with a
`cell: bool` parameter saves three. **Estimate, unbuilt**: under both
ceilings with one new leaf module of about 25 lines. Step 1b (R1b, the
`nullptr` birth, the `bump(@=n)` fix) adds no line to either knot but the two
of the `nullptr` test in `walk.hero`, which the same leaf takes.

Lines changed by step one, `diff` against the pristine copy, `<` and `>`
lines counted, `wc -l` before and after (`209-notes/diff-step1.txt`, 180
lines in all over 19 files): `token.hero` 83 (three tables re-aligned by
`fmt`, 334 → 337), `grammar_expr.hero` 16 (1767 → 1779), `check/walk.hero`
15 (2408 → 2417), `print/fmt.hero` 10 (1732 → 1736), `operand_families.hero`
8, `ast.hero` 7, `mutate/handles.hero` 7, `print/bodies.hero` 6,
`punctuation.hero` 5, `resolve/binding.hero` 4, `resolve/walk.hero` 4, and 2
each in `brace_layout`, `layout`, `line_opens`, `next_line`, `print/groups`,
`parse/annotation`, `resolve/state`, 1 in `describe`. **The net growth of
the compiler by `wc -l` is 30 lines** (pristine 7365 for the twelve files
the brief named; the sum of the `->` column minus the `<-` column over the
19 files is +30). Step 1b adds 69 more (`at_prefix.hero` +19,
`contextless_errors.hero` +17, `unused_sweep.hero` +15, `resolve/errors.hero`
+10, `walk.hero` +6, `grammar_expr.hero` +2 by `wc -l`; 249 diff lines over
23 files). **About a hundred lines, in the lexer's table, the parser, the
resolver, the checker and two printers: core, not sugar.** `@=` is a token
the lexer, the parser, the AST (its `ty` optional), the resolver, the checker
and the formatter all handle; nothing erases it in the frontend, because the
mutability it declares is what the checker and the resolver's write rule
read. Pascal-P4 scale is kept: 30 lines in a compiler of 7365 for the brief's
twelve files.

## 5. The migration, as measured (measurement 4)

**The silent step-one migration by `heroes fmt --in-place` does not exist**
(`209-notes/migrate.log`, 17:01:48 to 17:02:10, `./heroes-r1 fmt <file>
--in-place` over every `.hero` of three copies):

| tree | formatted | refused |
|---|---|---|
| `209-notes/selfhost-migrated` (557) | 58 | **499** |
| `209-notes/examples-migrated` (120) | 30 | 90 |
| `tests` (1697, in the worktree) | 1014 | 683 |

The refusal, read on `selfhost/resolve/errors.hero` and on a pristine
`parse/annotation.hero` (`./heroes-r1 fmt <file> --in-place`, exit 2):

    error: `fmt` would move the comment on line 41 of `selfhost/resolve/errors.hero` away from the code it was written beside
    error: this is a compiler bug — `selfhost/resolve/errors.hero` was NOT changed

It is the formatter's own guard (`.claude/rules/diagnostics-and-goldens.md`
§ A new surface form, the item *the formatter's own self-check*): it compares
the comments' places against the token stream, the `@` of an old-form cell
is printed `@=`, and every comment after such a cell reads as moved. A probe
with no comment formats fine (`p4_old_form.hero`, exit 0, printed `v: i64
@= 0`). After the pass the copies held: `selfhost` 5883 old-form lines and 47
new, `examples` 721 and 56, `tests` 1539 and 480 (the anchored patterns of
the shared brief, `@=` in the second). The seed compiler formats my edited
files, which carry only old-form cells, at exit 0.

**So the migrator is step two's certain fix, not step one's printer.** Under
CLAUDE.md § 10 the instrument that rewrites 1671 files is `heroes check
--apply` over the tree with the old form refused by `expected_binding_symbol`
and a `certain` fix that writes `=` after the `@` (the shape
`parse/at_prefix.hero` already uses for `@v = e`, `.fixed` goldens and
all); it goes through no comment guard, and a second `--apply` changes
nothing, which `suite_fixes.hero` already asserts of every case. Step one
(both forms accepted in silence) is then not a landing at all but the state
of my prototype, kept only so the tree compiles while it is rewritten. The
guard itself, or `probe/reader.hero`, owes the mapping old `@` to `@=` only
if the formatter is ever asked to migrate, which this route does not ask.

What the migration touches beyond the 1671 `.hero` files, from the critic's
list: 23 `.expected` and 32 `.applied`/`.fixed` goldens quoting a
declaration are re-read by hand or by their suites' output (never
`UPDATE_GOLDEN=1`, which does not exist); 839 planted mutants of
`tests/golden/recovery/plan-singles.jsonl` are re-planted by `heroes mutate`
over the migrated corpus; `docs/metrics/operators.md:13-14` is prose and
takes a new operator; 25 lines of `site/src` and 19 of `docs/design.md` are
documents; the TextMate grammar and `highlight.ts` take `@=` beside `@`. The
suites the change owes, by `.claude/rules/verification.md` § What gates
what: a change to what the checker refuses is judged by every golden tree,
so `check` `run` `emission` `determinism` `corpus` and `fixes`, plus the
`selfhost/**` row (`canonical` `layout` `order` `probe` `records` `spec`, the
compiler's own tests), the `selfhost/print/**` and `parse/**` row (`probe`
`surface` `grammar`, the formatter's probe by hand), `annotations` (a new
diagnostic code owes a witness in `selfhost/diag.hero`), `spec` `special`
`grammar` `unseen` for the spec, the site's build, and the seed regenerated
at the batch's close with its fixpoint by `cmp`. **The seed**: `seed/heroes.c`
is emitted C and carries no `@`, so it regenerates as any batch's does; the
only order that matters is that the compiler which emits the new seed must
read the migrated `selfhost/`, which both my `heroes-r1` and `heroes-r1b` do.

## 6. R1b, built, and the two findings the coordinator sent (measurement 2)

**Where it lives**: `resolve/unused_sweep.hero`'s `report`, the walk that
already tells *written and never read*: one helper `rebound_nowhere` (13
lines) asked of every local that IS read, and `resolve/errors.never_rebound`
(10 lines). It is the `unused_binding` walk extended, not a new one: the
`Local` record already counts `writes` (`resolved.hero:103`), incremented by
`writes.write_root` for the left of a mutation and by `writes.inout_root` for
an `@` argument (`resolve/walk.hero:231,256`), so **a write through an `@`
argument counts as a re-binding with no new line**: `p11` (`db @= 0`,
`open(@db)`) exits 0 under `heroes-r1b`, and `suite_special.hero:419`'s `db:
Db @ nullptr` re-bound only through `@db` is not among the harness's five
refusals below. Spec § 5 `:138`'s *a write is not a use* and B2's sentence
meet here: the draft owes the words *re-bound, by `@` or by `f(@v)`*.

**The blind seat's two-message finding, answered by the sweep's own rule.**
`rebound_nowhere` returns false when the cell's name stands in `r.suggested`
for its module, the table the sweep already consults so that a name offered
as a did-you-mean is not also told unused (*already spoken for*,
`unused_sweep.hero`). Measured: `p7_totl.hero` (`total @= 0`, `totl @ total +
x`) under `heroes-r1b` prints **one** diagnostic, `unknown_name` at 4:9 with
the certain fix *rename to `total`*, and nothing at line 2
(`209-notes/probes/results-r1b.txt`). What line 4's message should carry
beside the rename: a second fix, `guess`, *declare a new cell: `totl @=
total + x`*, since under R1 that is the one-character alternative the author
may have meant; it costs about three lines in `resolve/errors.hero` where
`unknown_name` is built for a mutation's place, and it is the repair R9 asks
for today's `v @ 0` message, which names no declaration shape (the shared
brief's probe). Deciding R1b after the certain fixes are applied is the
weaker route: it would need a second resolve of the file and still say
nothing when the author declines the rename.

**What R1b refuses in the tree today** (`./heroes-r1b check <root>`, the
trunk's sources with their old-form cells, which the prototype reads; counts
of `error[never_rebound]`):

| tree | cells refused | where |
|---|---|---|
| `selfhost/main.hero` | **151** | 120 inside `test` blocks, 31 in code (`awk` on the enclosing top-level line); the most in `check/labels.hero` 6, `check/generics.hero` 6, `parse/braced_lines.hero` 5, `check/join.hero` 5 |
| `examples/` | 19 sites in 10 files (`tree/node.hero` 7, `widths/main.hero` 3, `interpreter/run/value.hero` 2, one each in `checksum/crc`, `checksum/adler`, `tree/main`, `pipeline/write/source`, `nbody/main`, `gallery/12-interpolation`, `spectral/main`) | |
| `tests/harness/main.hero` | **5** (`suite_records.hero:2769,5987,5988,6165`, `suite_grammar.hero:588`): **the net does not compile under R1b until they are written `=`** | |
| `tests/golden/**` | unrun (time box) | |

What they are: the first 16 of the 151, read at their lines, are every one a
`=` in disguise, a cell born of a call or a literal and never written again
(`s: source.Source @ read.must()`, `c: Cursor @ new_cursor(tokens)`, `shift:
BinaryOp @ .shl`); by the value's first two characters 73 are a call or a
name (`ch`, `re`, `so`, `ne`...), 9 `{}`, 4 `ok(`, 4 a non-empty array. None
of the 16 is a cell written only through an `@` argument: those pass. The
message today offers no fix because the `@=`'s span is not in the `Local`;
the certain fix (`@=` → `=`, the annotation kept) costs a `symbol:
token.Span` on `declare`, `Binding` and `Local`, about ten lines across
`ast.hero`, `grammar_expr.hero`, `resolve/binding.hero`, `resolved.hero` and
`resolve/state.hero`, unbuilt. **Ordering**: a cell never read is told
`unused_binding` and not `never_rebound` (the `reads > 0` test comes first),
so one cell gets one message.

**The ffi-pragmatist's two conditions, built**:

- *`bump(@=n)` one message with a certain fix*: `parse/at_prefix.lent_with_eq`
  (17 lines), reached from `grammar_expr.primary`'s existing sigil line
  (`k == .at || k == .at_eq`, no new line in the knot). `p5` under
  `heroes-r1b`: one `expected_expression` at 6:10, *found `@=` before `n` — an
  argument lent to an `@` parameter is written `@n`; `@=` declares a cell*,
  **fix `certain`: write `@n`**. Under `heroes-r1` (before it) one message and
  no fix; today two messages.
- *a bare `nullptr` birth under `@=` fires `cannot_infer`*: in
  `check/walk.hero`'s `declare` arm, two lines asking the value's own text
  (`source.span_text(s, a.exprs[d.value].span) == "nullptr"`, a fact about the
  value, no `_` arm: the checker refused my first draft's wildcard over
  `ExprKind` with `wildcard_on_variant`), and `contextless_errors.cannot_infer_nullptr`
  (12 lines). `p13` under `heroes-r1b`: `cannot_infer` **at the declaration**,
  8:11, *`nullptr` alone says `ptr`, not which handle `db` will hold — write the
  handle's type on the declaration*, fix `guess` *annotate the cell: `db:
  <Handle> @= nullptr`*. The fix can name the cell and the symbol; it cannot
  name the handle type, which only the first `@db` call knows (that is the
  checker's `expected Db` two lines down), so a `certain` fix would need the
  call's signature read ahead, which §4.5 forbids. **One line still owed**:
  the call's `type_mismatch` (9:11, *expected `Db`, found `ptr`*) still
  follows, two messages for one birth; poisoning the cell with
  `state.error_ty_of(c)` after the `cannot_infer`, as the `bind` arm does for
  unit, silences it. Under `heroes-r1` alone `p13` is the ffi-pragmatist's
  shape, `type_mismatch` at the call, no fix, no word of the declaration.

## 7. Inference on cells: what the tree keeps (measurement 3)

A pristine copy stripped by `perl` (`209-notes/selfhost-stripped`): every
old-form declaration whose value opens with `[]`, `{}`, `fail(`, `ok(`,
`nullptr` or a `[` ending its line keeps its annotation and takes `@=`
(**1579**); every other one loses it (**4347**); 5926 in all. Two lines
inside string literals of `print/bodies.hero` were mangled by the regex and
restored from the pristine file. Then `./heroes-r1 check
209-notes/selfhost-stripped/main.hero`:

1. First pass: exit 1, **10 `unused_binding` on `use` lines** of 9 files
   (`cli/check.hero:22` `use check/state`...). Real: `c: state.Checked @
   checker.check(...)` was the file's only mention of `check/state`;
   annotation gone, the import is unused. **Stripping a cell's type unreads
   imports**; the migration pays it as 10 `use` lines removed, and the
   resolver speaks before the checker (`p14`: a program with an unused cell
   and a `type_mismatch` is told only the first), so the pass hides the
   checker.
2. Those ten lines deleted, second pass (`209-notes/check-stripped2.txt`):
   exit 1, **78 errors: 63 `cannot_infer`, 13 `type_mismatch`, 2
   `mixed_arithmetic`**, `real 4.50 user 4.47`. The 63: **55 a case name**
   (`verdict @= .ok_exit`, `k @= .lparen`, `certainty @= .certain`: the
   shared brief's 55 `.case` births, exact) and **8 a non-empty array**
   whose elements cannot say their type alone. The 15: **every one a width
   cell**, born of a literal annotated `u8` or `u64` and refused lines away
   at its first use or mutation (`next_line.hero:37` `else if b == quote`,
   `expected u8, found i64`; `:42` `quote @ b`, `expected i64, found u8`;
   `digits.hero:157-161` a `u64` accumulator, `mixed_arithmetic` and
   `expected u64, found i64`), ten of them in `next_line.hero` alone, **no
   fix offered on any of the 13 `type_mismatch`**, and the message names
   neither the cell's birth nor the annotation that repairs it.

So, as `heroes check` counts them (test blocks included, since the 151 of
§ 6 counted 120 in test blocks by the same command; whether `check` stops
early inside a function is unrun, so these are lower bounds): **of 5926
cells, 1579 + 78 = 1657 keep their annotation and 4269 (72 %) may drop it.**
The second class, the cell born narrower, is 15 sites in 4 files for the
compiler's own tree, 0.25 % of its cells, and it is the class whose
diagnostic is worst: told at a distance, no fix. The repair the brief asks
for: when a mutation's value has a width type and the cell's type was
inferred from a literal, the `type_mismatch` names the cell's declaration
line and offers the guess fix *annotate the cell: `quote: u8 @= 0`*; the
checker has the local's `ty` and `name` span (`state.bind_local`) and the
mutation's expected type, so it is a note and a fix in `check/type_fit` or
`value_errors`, about ten lines, unbuilt. The literal-width case the critic
measured (`b: u8 @ 255` then `b @ big`) keeps its answer, since that cell is
annotated.

The blind seat's B programs under `heroes-r1b`: `t1B` (`best @= words[0]`)
exits 0 and prints `abcd`; `t2B` (`counts: {str: i64} @= {}`) exits 0 and
prints `2` (`209-notes/probes/results-r1b.txt`), the same answers the
coordinator read for the A programs under the trunk.

## 8. The ceilings after step 1b, and R2 to R11 in the lexer and the parser (measurement 6)

`./heroes-r1b run tests/harness/main.hero -- ./heroes-r1b layout`, after the
harness's five cells were written `=` by `sed` so that it compiles
(`209-notes/layout-after-1b.txt`): **`layout: 4 passed, 2 failed`**,
`selfhost/check/walk.hero` 1880 (the `nullptr` birth's two lines and their
blank, on top of step one's five), `selfhost/grammar_expr.hero` 1093
(unchanged by 1b), and **`selfhost/parse/`: 8714 lines of code, past its
budget of 8699**: `lent_with_eq`'s 17 lines in `parse/at_prefix.hero`
breach the directory's budget by 15. So the landing owes a third seam, or a
DECIDED row that says why the budget moves: the `@=`-where-an-argument-stands
message belongs with the sigil habits of `at_prefix.hero`, and the
directory's budget is a number a sitting ratified, not the language.

One sentence each on what differs from R1, read in the lexer and the parser:

- **R2 `=@`**: the same two-byte match in `punctuation.hero`, but today `=`
  then `@` is two tokens that `at_prefix.in_value` already reads as Ruby's
  habit in a value (`x = @v`, `at_prefix.hero:213`, a certain deletion), and
  `out=@db` is a planted mutant of `named-arg-equals`
  (`plan-singles.jsonl:4460`): a token `=@` eats a recovery that exists and
  reads a space-missing `f(x =@y)` as a declaration's symbol. Object.
- **R3 `@@`**: `@@v` is the doubled sigil `at_prefix.sigils` counts
  (`at_prefix.hero:74-82`, `fixedbugs-131-a-sigil-*`), Ruby's class variable
  told with a certain fix; a token `@@` breaks that recovery for no gain over
  `@=`. Object.
- **R4 `:=`**: `annotation.symbol_after` (`annotation.hero:43-58`) finds the
  symbol a `: Type` owes at depth 0; a `:=` token makes `v:= 0` and `v: T =
  0` two readings of one colon, and design.md §4.4's journey (`:1086-1091`)
  refused `:=` beside `::` as confusable; cost R1's plus a split of
  `statement_kind`'s `.colon` branch. Object.
- **R5 `var v = 0`**: a keyword enters `keywords.hero` and every enumerated
  keyword table of `token.hero` (six, by the `.colon_colon` count), the
  `grammar` suite's keyword arms, the editors' word lists, and `Statement =
  ident Binding` gains a second production; `@` stays for the mutation, so
  the thing it buys over `@=` is one more reserved word in 3229 files
  (whether `var` or `cell` is a name in the tree: unrun). Object.
- **R6 the type mandatory on both**: 10225 inferred `=` lines in `selfhost/`
  take an annotation the checker knows and no instrument today writes
  (a `certain` fix rendering generic and record types is a new printer
  path); it is §4.5 inverted, and the ceiling cost is the printer's, not the
  parser's. Object.
- **R7 `v @ 0` declares, `v @= e` re-binds**: 7553 mutation lines of
  `selfhost/` rewritten against 5926 declarations, `@=` read as C's `op=`
  by habit, and §4.4's typo hole reopened: `totl @ total + x` declares a
  new cell, caught only by the unused rule's *written and never read*, at a
  distance. Object.
- **R8 the cell typed from all its writes in the block**: a pre-walk over
  the block's mutations before the cell's type is fixed, inside
  `check/walk.hero`'s knot (1880, over its 1870), with a join over widths
  and fallibles the checker does not have, and the silent widening of `p9`'s
  shape that design.md `:1144` *errors stay local* refuses: a core
  mechanism, not a token. **Veto** (§1.1, §1.7: it adds a construct the
  checker must handle, in a knot past its ceiling, and no self-hosting need
  asks it).
- **R9 the `v @ 0` message repaired, R0 kept**: `resolve/errors.hero`'s
  `unknown_name` for a mutation's place, about five lines, one message
  naming the declaration shape and a `guess` fix writing it; the metric's
  operator `mutate-undeclared` (`operators.md:14`) scores it. Approve, with
  any route.
- **R10 `v: i64 @= 0`, the symbol with the type kept**: exactly my step one
  minus `cell_stmt` and the `declare` arm's `synth` (about 40 of the 180 diff
  lines), the token, the printer and the migration; the inference is then a
  second commit of about 20 lines whose effect the blind seat can read
  apart. Approve as R1's first commit.
- **R11 `@v = 0`**: occupied by `at_prefix.told`'s recovery and its goldens
  (`fixedbugs-131-a-sigil-before-a-name-*`, `fixedbugs-179`): a legal form
  there would silence a certain fix the recovery instrument scores. Refuse.

## 9. Verdicts, prediction, condition

- `verdict`: **approve R1 with R1b and R9, landed as R10 first; object R0,
  R2, R3, R4, R5, R6, R7, R11; veto R8.**
- `section`: design.md §1.1 (simplicity sets the ceiling), §1.7 (core plus
  elaboration), §4.4 `:1102-1115` (the mandatory type's two reasons, one of
  which, the unused rule, spec § 5 `:138` and my `p7` show is already
  served), §4.5 `:1144` (errors stay local), §4.17 (one message, a fix that
  repairs it). The document does not cover the formatter's comment guard
  as a migration instrument; `.claude/rules/diagnostics-and-goldens.md` § A
  new surface form does, and § 5 above rests on it.
- `implementation_cost`: **core, not sugar**, since the token reaches the
  lexer, the AST, the resolver, the checker and two printers; step one 180
  diff lines over 19 files, +30 by `wc -l`, the biggest `token.hero` 83
  (tables re-aligned), `grammar_expr.hero` 16, `check/walk.hero` 15,
  `print/fmt.hero` 10 (§ 4); step 1b 249 over 23, +99. Built, `check` and
  `build` green, `layout` red on `check/walk.hero` (+5, then +10 over
  1870), `grammar_expr.hero` (+8 over 1085) and `selfhost/parse/` (+15 over
  8699), each with a seam the language allows (§ 4, § 8). Build time
  unchanged within the load's noise (§ 1).
- `needed_for_self_hosting`: **no.** The compiler compiles itself today;
  the claim rests on Principle 0's second clause, a measured thesis effect,
  which is the blind seat's and the ergonomist's to show.
- `argument`: R1 is about a hundred lines, lands in the places a new token
  must, and erases nothing anyone needs; what it costs is three ceilings by
  5 to 15 lines each, with seams, and one new class of distant error, the
  cell born narrower, 15 sites in 4 files of `selfhost/`, every one a width
  cell with no fix today. R1b is a thirteen-line extension of the unused
  walk that already counts `@` writes and already suppresses a cell whose
  name was offered as a repair, so the blind seat's two messages become one
  by construction (`p7`); it refuses 151 + 19 + 5 cells in the tree, all
  `=` in disguise where I read. The migrator is step two's certain fix
  through `heroes check --apply`, because `fmt`'s comment guard refuses 499
  of 557 files. R8 adds a flow-sensitive join to a knot over its ceiling:
  that is the one route that moves the ceiling, so it is the one I veto.
- `prediction`: at the landing's batch gate, `heroes run tests/harness/main.hero
  -- ./heroes layout` reads **0 failed** with `check/walk.hero` at or under
  1870, `grammar_expr.hero` at or under 1085 and `selfhost/parse/` at or
  under 8699 in `code_lines`, through one new leaf module under
  `selfhost/check/` of at most 40 lines and `lent_with_eq` placed outside
  `selfhost/parse/` or a DECIDED row moving that budget; and the census of
  `check` over the migrated tree refuses in `selfhost/` exactly 55 case
  births, 8 arrays and 15 width cells where an annotation was dropped, and
  0 `never_rebound` after 151 + 19 + 5 rewrites to `=`. A `layout` red, or
  a count off by more than the 10 unused imports, falsifies it.
- `condition`: I turn R1 to **object** if the leaf helper cannot bring
  `check/walk.hero` under 1870 by `layout`; if `heroes check --apply` does not
  migrate the 1671 files in one pass (a second `--apply` moving any file,
  `suite_fixes`' own assertion); or if the whole tree's narrower-born count,
  tests and examples included, passes about one cell in a hundred, since
  the distant `type_mismatch` with no fix is then a class and §4.17 owes it
  a note and a fix before the symbol lands. I turn R1b to **object** if any of
  the 175 refused cells is not a `=` in disguise (a lane reads all 175; I
  read 16). I lift the veto on R8 if a route is shown that types a cell
  from its writes without a second pass over the block and without widening
  `p9` in silence.

**Unrun, in that word**: `tests/golden/**` under R1b; the step-two refusal
of `v: T @ e` with its certain fix (priced at about ten lines in
`parse/annotation.hero` beside `no_binding_symbol`, whose message already
names both shapes, and three in `grammar_expr.annotated`); the `heroes
check --apply` pass over the tree; `probe/reader.hero` against a file with
`@=`; the `grammar` suite; `heroes test` under `heroes-r1b` (the compiler's
own 151 cells refuse it until rewritten); whether `var` or `cell` is a name
in the tree; the real token rows. One thing to say plainly: the migration
pass of § 5 ran as a background command of this tool, whose own log went to
the session's scratchpad under `/private/tmp` by the tool's doing, not by a
command of mine; I ran nothing else in the background after reading that.

## 10. The compiler's own tests under R1 (run after § 9 was written)

`/usr/bin/time -p ./heroes-r1 test selfhost/main.hero` over the step-1b
sources (`209-notes/selftest-r1.txt`, 17:11:59 to 17:13:11): **1542 tests,
6 failed**, `real 72.25 user 55.53`, plus one *internal error: linking
failed* whose six lines above it say *the cached object ... is not the one
its compile wrote: its bytes changed after it was written*: the shared
`build/` written by my parallel `heroes-r1b check` runs at the same moment,
the false red `.claude/rules/verification.md` § What may run beside a gate
describes for `cache`, not R1's. The six, each read at its assertion:

| test | module | cause |
|---|---|---|
| *the dump proves precedence, labels statements, and keeps control shapes* | `print/bodies.hero` | the expected text pins `declare v: i64 @ 0`; the dump now prints `@=`. An expectation to move with the form |
| *arguments carry their labels and markers, patterns their bindings* | `print/bodies.hero` | the same, `declare sh: Shape @= .box(w: 3)` |
| *a control form at a binary's right edge keeps its body* | `print/fmt.hero` | the expected text pins `out: str @ ""`; `fmt` prints `@=`. The same |
| *an expression begins exactly where begins_an_expression says it can* | `grammar_expr.hero` | **`panic: stack exhausted in grammarexpr.primary`**: my edit of line 411 read `k == .at \|\| k == .at_eq && in_value(...)`, `&&` binding tighter, so a bare `@` at an expression's start re-entered `primary` without advancing. My slip, not R1's; only this table test feeds a bare `@` to `parse_expr` (a call's `@x` is the argument parser's), which is why every probe and the harness passed. Repaired with parentheses, rebuilt and re-run below. Noted in passing: the compiler accepted the mixed `\|\|`/`&&` without parentheses in silence |
| *an @ handed to a construction is told at the name stage* | `resolve.hero` | expects one `marker_mismatch` for `n: i64 @ 1` then `.one(x: @n)`; got two: **R1b fires on `n`**, whose only write is an `@` at a site the resolver refused (a construction takes no `@`), so `writes` stayed 0. A shape beside the repaired one: `rebound_nowhere` must also stay silent where the resolver already spoke of that name, or `inout_root` must count the refused `@` as the write it was meant to be. About three lines, unbuilt |
| *a certain fix enclosing another is made again from the text the inner one left* | `cli/check.hero` | `settled_text(... "print(bump(k).must())", stage: 4)` expected `print(bump(@k))`: unread within the box, a question for the lane (whether the `@` insertion fix now meets the `@=` lexing when it lands before `k`, or the cache red above) |

**Re-run after the parentheses**: see the postscript below if it landed
inside the box; otherwise unrun.

**Postscript, the re-run** (`209-notes/selftest-r1-2.txt`, 17:14:40 to
17:16:21, line 411 now `(k == .at || k == .at_eq) && at_prefix.in_value(@c,
text)`, `heroes-r1b` rebuilt at exit 0 in between): **1542 tests, 5
failed**, `real 71.87 user 54.95`; the table test passes, the five above
stand as read. The *linking failed* lines recur with nothing else of mine
running: they sit under `build/selftest-objects-443/`, right after *end to
end: the port compiles a Heroes program and runs it*, and the message is
`selfhost/cli/served.hero:112`'s own words for an object whose bytes changed,
so they read as a test's planted scenario rather than a red; no `FAIL` is
attached to them and the count says five. Whether a pristine run prints the
same lines is unrun. So the compiler's own tests under R1 cost three
expectation texts moved to `@=` and two real shapes, R1b beside a refused
`@` (three lines) and the enclosing-fix case (unread).

What the box leaves in my folder for the lane that lands this:
`selfhost/` with step 1b built and formatted (its `grammar_expr.hero`,
`at_prefix.hero`, `contextless_errors.hero`, `check/walk.hero`,
`resolve/errors.hero`, `unused_sweep.hero` formatted by the seed compiler,
which prints the old form these files still carry); `heroes-seed`,
`heroes-pristine`, `heroes-r1` (step one), `heroes-r1b` (step 1b);
`209-notes/` with the pristine, migrated and stripped copies, every output
file named above, and `probes/` with the fourteen programs; the harness's
five cells written `=` in `tests/harness/suite_records.hero` and
`suite_grammar.hero`. No git command was run in the folder.

Finished 2026-10-10 17:17 CEST (`date` read at the last command 17:16:35,
the writing after it); started 16:48:10.
