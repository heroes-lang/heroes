# Panel 209, the shared brief: a mutable cell declared by its symbol rather than by its type

Written by the coordinator on 2026-10-10 from 16:07 to 16:30 (`date`), on the
trunk frozen at `87794631` (`git log -1`, 2026-10-10 16:00, *Defects 587 to
589*; `git status --short` shows only panel 208's untracked `briefs/` and
`reports/`, another session's sitting). Convened by the author's request of
this afternoon, after a conversation in which they asked why a mutable
declaration carries a type and an immutable one does not, and proposed the
form the question below carries. Every fact names its command or file; a
number marked **carried** is a report's.

**Lane: full panel.** The proposal changes surface syntax, a spec production
and a diagnostic, so the five seats sit, the blind seat as six fresh sessions
(its brief), the completeness critic before the seats and after them.

**The author's framing, binding on every seat** (2026-10-10, meant as): *the
language is in alpha; we have released only one version; it is not to be
treated as beta; everything may break.* `git tag --list 'v*'` reads `v0.1.0`
and `v0.2.0`, so the last release is v0.2.0. No compatibility with a program,
a golden or a release weighs in a verdict. The cost of migrating the tree is a
cost of work to be measured, never a reason against.

## The question

Today (spec § 5, `spec/heroes-spec.md:129-135` and the production at `:150`):

```
x = 5              # immutable binding, type inferred
v: i64 @ 0         # mutable declaration — the type is REQUIRED
v @ v + 1          # mutation; only a declared @ name can be mutated
```
    Binding   = ( "=" | ":" Type ( "@" | "=" ) ) Expression NEWLINE .

The type on a mutable declaration is the mark that tells a declaration from
a mutation, since both lines carry `@` (design.md §4.4, `docs/design.md:1102-1115`,
*Why the type is mandatory on mutable declaration*: locality, and the typo
`totl @ total + x` that would otherwise declare a new cell). An immutable
name stands left of `=` once in its life, so nothing has to be told apart
there and its type is inferred. **The proposal gives the declaration a symbol
of its own, so the type becomes optional on both forms under one rule.** The
draft, B1 (`.claude/worktrees/scratch-b15/209-coordinator/measure/spec-B1.md`,
the diff below is `diff spec/heroes-spec.md <that file>`):

```
x = 5              # immutable binding, type inferred
v @= 0             # mutable cell, type inferred the same way
v @ v + 1          # mutation; only a cell declared with @= can be mutated
```
`=` binds once, forever. `@=` declares a mutable cell, and `@` re-binds it,
or a field or element inside one.

    Binding   = [ ":" Type ] ( "=" | "@=" ) Expression NEWLINE .

and the four other sites of the document that write a cell's declaration
(`grep -nE '[a-z_][a-z0-9_]*: [^=@]+ @ ' spec/heroes-spec.md`: `:43`
`b: u8 @ 255`, `:291` and `:293` `m: {str: i64} @ {}`, `:410` `x: cstr @
s.lease()`) take `@=`, their annotations kept. **B2** is B1 plus one
sentence after *inside one*: *A cell nothing re-binds is a compile error:
write `=`.* (`spec-B2.md` beside it), because under B1 a `@=` written where
`=` was meant costs one character and compiles, as it does today.

**The routes the sitting prices**, from the author's conversation and
design.md §4.4's journey (`:1086-1091`), **widen the list**:

- **R0** the status quo.
- **R1** `v @= 0` declares, `v @ e` re-binds, `: Type` optional on both `=`
  and `@=` and demanded by `cannot_infer` where the value cannot say it (B1).
- **R1b** R1 and the never-re-bound rule (B2).
- **R2** `v =@ 0`, the author's first spelling, R1's semantics.
- **R3** `v @@ 0`.
- **R4** `v := 0`.
- **R5** a keyword before the name, `var v = 0` or `cell v = 0`, `@` re-binds.
- **R6** the type mandatory on both forms (`x: i64 = 5`), inference only
  inside expressions.
- **R7** the inverse: `v @ 0` declares, `v @= e` re-binds.

## What is measured, with its command

**The shapes in the tree** (`git ls-files <dir> | grep '\.hero$' | xargs
cat | grep -cE <pattern>`, the patterns in
`docs/panel/209-briefs/compiler-engineer.md`; a grep on the line's shape, the
`=` column including `_ =` discards):

| tree | files | `=` inferred | `=` annotated | `@` declarations | mutations |
|---|---|---|---|---|---|
| `selfhost/` | 557 | 10225 | 847 | 5926 | 7553 |
| `examples/` | 120 | 833 | 101 | 777 | 909 |
| `tests/` | 1697 | 8812 | 422 | 2019 | 3649 |

3229 tracked `.hero` files in all (`git ls-files | grep -c '\.hero$'`); 1671
hold at least one `@` declaration; 23 of 1344 `.expected` files quote a line
holding one (`git ls-files tests | grep '\.expected$' | xargs grep -lE
'[a-z_][a-z0-9_]*: [^=@]+ @ '`).

**What a cell's first value is, in `selfhost/`** (the 5926 lines, the value
after ` @ ` classed by its first characters): 2178 a name, a call or a field;
1299 a number; 1223 `[]`; 342 a bool; 324 a string; 277 `{}`; 169 a non-empty
array; 55 a `.case`; 51 `fail(`; 8 `ok(`. So **1551** (`[]`, `{}`, `fail(`)
keep their annotation under R1 by today's `cannot_infer` rule, and an
unknown further share whose first value is narrower than a later write (a
cell born `0` that later holds an `i64?`): only a prototype can count it.

**What the compiler does today** (the trunk's `./heroes`, built 00:26, newer
than every file under `selfhost/`, `runtime/` and `seed/` by `find -newer`;
the programs and the full output in
`.claude/worktrees/scratch-b15/209-coordinator/probes.txt`):

| program | verdict |
|---|---|
| `totl @ total + x`, `totl` never declared | exit 1, `unknown_name`, *did you mean `total`?*, fix `certain` |
| `v @ 0` with no declaration | exit 1, `unknown_name` at every `v`; the message does not say the declaration's shape |
| `x: i64 = 5` | exit 0: the annotation on `=` is legal and optional |
| `xs = []` | exit 1, `cannot_infer`, fix `guess` *annotate the binding: `xs: [i64] = []`* |
| `v: i64 @ 0` then only `print(v)` | **exit 0**: a cell nothing re-binds is accepted |
| `totl: i64 @ 0` written in a loop, never read | exit 1, `unused_binding`, *written and never read* |
| `v @= 0` | exit 1, `expected_expression` at the `=`: `@` followed by `=` is legal nowhere today |

**The record.** The mandatory type was born in the repository's first commit
(`git log -S"type REQUIRED" -- docs/design.md docs/design/design.md
design.md`: `57d8f27f`, 2026-08-03) and **no sitting has had it on the
table**: searched `docs/panel/`, `issues/`, `docs/learn/`, design.md Parts 6
and 8 for *mandatory annotation*, *optional annotation*, *always typed*,
*untyped mutable*, *declaring or mutating*, `var`, `let mut`, `@=`, `@@`,
`:=`; the only hits are §4.4's journey (`::`/`:=` refused as confusable with
each other, `=`+`var` refused because `=` *doing double duty reopens the typo
hole*, a world where `=` also mutated) and `selfhost/parse/at_prefix.hero:63`,
where `@v` and `@@v` before a name are a known mistake told with a certain
fix (goldens `tests/golden/check/fixedbugs-131-*`, `fixedbugs-179-*`). A
negative rests on the searcher's vocabulary: the critic's first pass re-runs
it. **One premise of §4.4 has moved**: its sentence *`totl` is read on that
very line, so an unused-variable rule wouldn't catch it* predates spec § 5's
*a write is not a use* (`:138`, entered `4139adbf`, 2026-08-04 by `git log
-S"write is not" -- spec/`), and the probe above shows today's compiler
refusing the written-never-read cell. The locality half of §4.4 stands on
its own.

**The spec's price** (`heroes measure`, the real row by `--refresh` in the
coordinator's measure folder, the vendored rows offline):

| document | real (claude-opus-5) | claude-legacy | cl100k_base |
|---|---|---|---|
| trunk `87794631` | 9847 (refreshed 2026-10-09) | 7348 | 7479 |
| B1 | **9844** | 7356 | 7487 |
| B2 | **9865** | 7372 | 7503 |
| round b18's spec (`lane-round-b18` at `f6528c53`, 112 commits ahead, its own `./heroes`) | 10021 (refreshed 2026-10-10) | | 7622 |

The ceiling is 10240 with the FFI floor mortgaging 60 (`heroes measure`'s
own last line). **The vendored tables disagree with the reader's in sign on
B1** (+8 against −3): the real row is the one that judges
(`.claude/rules/spec-shape.md`). The round touches `spec/heroes-spec.md` (22
lines) and none of `selfhost/lexer.hero`, `selfhost/parse/`,
`selfhost/print/`, `selfhost/ast.hero` (`git diff --stat
87794631...lane-round-b18 -- <those>`).

**What a program pays today for the annotations**: the 5926 `@` declaration
lines of `selfhost/` measure 65322 claude-legacy and 72855 cl100k tokens as
written, 48677 and 56590 rewritten `name @= value` (the two files under
`209-coordinator/tokens/`, `heroes measure` from the root); vendored, since
`--refresh` prices only the two judged documents.

## The rules every seat works under

As panel 207's: your folder is `.claude/worktrees/scratch-b15/209-<seat>/`, a
detached `git worktree` of the trunk at `87794631` made by the coordinator;
**run no git in it**; build your compiler there from the seed (`clang -I
runtime seed/heroes.c runtime/runtime.c -o heroes`, about 3 s) and after an
edit to `selfhost/` with `./heroes build selfhost/main.hero -o heroes` (about
60 s); lanes `lane-b18-*`, `lane-b19-*` and `lane-round-b18` and panel 208's
seats work beside you: never touch them, never the trunk; at most three
processes; never kill a process you did not start; **no paid run** of any
kind; **no file outside the repository's root**, `/tmp` and `/private/tmp`
included, not for a moment; notes in your folder as you go; **your report is
`docs/panel/209-reports/<seat>.md`, written as you go and not only at the
end**. A `.hero` you write that uses a form the trunk's compiler does not read
is written with a shell heredoc, since the Write tool's hook judges it with
that compiler. **Time box**: report within 45 minutes; what you did not reach
is written as unrun, in those words. Every number you write names the command
that produced it. Read the clock with `date` before writing a time.

**What the synthesis will take**: the most robust and complete resolution,
never the cheapest and never a compromise (CLAUDE.md § 4); a route that does
not build in a seat's copy is not adopted. Each seat ends with a verdict per
route, a falsifiable prediction with the instrument that scores it, and the
condition that would change its verdict.

## Corrections and additions from the critic's first pass, binding

Read at 16:48 (`date`): `docs/panel/209-reports/completeness-critic-pass1.md`,
every one from a command it ran in its own worktree; applied before any seat
starts and binding over the text above where they disagree. Read it whole;
its routes R8 to R11 and its questions are part of your brief. In short:

- **The four patterns of the shapes table**, each over `git ls-files <dir> |
  grep '\.hero$' | xargs cat | grep -cE '<pattern>'`: `=` inferred
  `^[[:space:]]+[a-z_][a-z0-9_]* = `; `=` annotated `^[[:space:]]+[a-z_][a-z0-9_]*: [^=@]+ = `;
  `@` declarations `^[[:space:]]+[a-z_][a-z0-9_]*: [^=@]+ @ `; mutations
  `^[[:space:]]+[a-z_][a-z0-9_]*(\.[a-z_][a-z0-9_]*|\[[^]]*\])* @ `. Unanchored
  the declaration count reads 6161 in `selfhost/`; the critic's own patterns
  read mutations 7549 / 909 / 3631 and `xargs cat` joins a file lacking a
  final newline to the next, so the table is a reading of these patterns and
  not a count of the language's nodes: a seat that needs the exact number
  counts AST nodes with its compiler.
- **1190 of `selfhost/`'s 5926 declarations stand inside `test` blocks**,
  which `heroes build` never compiles: a prototype is judged by `heroes test
  selfhost/main.hero` as well as by the build.
- **The lower bound is 1559, not 1551**: `x = ok(5)` fires `cannot_infer`
  today too, so the 8 `ok(` cells keep their annotation.
- **The `@` argument counts** with `, @x` included (`(\(|: |, )@[a-z_]`):
  6607 in `selfhost/`, 500 in `examples/`, 4 in the spec.
- **Where `@=` joins is `selfhost/punctuation.hero:26`**, `punct_at`, longest
  match first, not `lexer.hero`.
- **The migration's surface beyond `.hero` files**: 32 `.applied`/`.fixed`
  goldens quote a declaration, 839 lines of
  `tests/golden/recovery/plan-singles.jsonl`, 25 lines of `site/src`
  (outward-facing: the site's build), 19 of `docs/design.md`, and the editors'
  TextMate grammar matches `@` alone (`editors/.../heroes.tmLanguage.json:130`).
  The round's `selfhost/` holds 6264 anchored declarations against the trunk's
  5926 (100 files differ); **the trunk is the tree this sitting prices**, the
  round's figure says how fast the number moves.
- **The metric's operators cite the rule being changed**:
  `docs/metrics/operators.md:13-14`, `forget-at-decl` and `mutate-undeclared`,
  name the mandatory type as the catching rule; R1 owes new operators and the
  corpus's 839 planted declarations re-planted, and `=@` already appears there
  as a planted mutant (`plan-singles.jsonl:4460,4552`, `named-arg-equals`), a
  fact for R2.
- **B2's sentence against the spec's own example**: `suite_special.hero:419`
  builds `db: Db @ nullptr` re-bound only through `@db`; the never-re-bound
  rule must count a write through an `@` argument as a re-binding, as spec
  § 5 `:138` counts it as a use, and the draft does not say so yet.
- **`bump(@=n)`**: under R1 the lexer reads `@=` whole where today it reads
  `@` then `n`; the form owes a message with a fix.
- **The `spec` suite with B2 in place** (the critic's copy, its compiler not
  reading `@=`): 19 passed, 4 failed, all four the count and pin rows;
  `named` and `rejected` stay green, so no `spec` row waits on the lexer.
- **`b: u8 @ 255` then `b @ big`** is `type_mismatch` at the mutation today
  (the critic's `cell_narrow.hero`): the literal-width case of the narrower
  cell has its answer; the call-result case is still the prototype's.
- **Line numbers corrected**: design.md's *Exactly two cases are not* is
  `:1150` (the heading is `:1142`); `record Param` is `selfhost/ast.hero:361`
  (`:631` is a constructor in a helper); B1 changes 8 lines and B2 9 (seven
  and eight were hunks); the spec-warden's fence grep reads 4 with
  `^[[:space:]]*`. `docs/panel/209-briefs/blind/` exists on the trunk (the
  critic looked in its worktree, which holds no brief).
- **Four routes added**: **R8** the cell's type inferred from all its writes
  in the block, against design.md `:1144` *errors stay local* and the silent
  widening today's `type_mismatch` refuses; **R9** keep R0 and repair the `v @
  0` message, which today is two `unknown_name` for one mistake and no word of
  `name: Type @ value`, at no spec token; **R10** `v: i64 @= 0`, the symbol
  without the optional type, since B1 bundles two changes and a reading that
  moves under B2 cannot say which moved it; **R11** `@v = 0`, occupied by
  `at_prefix`'s recovery, listed to be refused. The blind seat keeps A against
  B2: what would land is the bundle, and the seats' reasoning separates R10
  from R1 where the readings cannot.
- Unverified by the critic, standing as the coordinator's: the real rows 9844
  and 9865 (a paid refresh), the two commits `57d8f27f` and `4139adbf` (no
  git in its pass), the `grammar` suite with B2 (time box).
