# Panel 209, the completeness critic, pass 1: the briefs against the world

Started 2026-10-10 16:32:38 CEST (`date`). Folder:
`.claude/worktrees/scratch-b15/209-critic/`, a detached worktree of the trunk
at `87794631` made by the coordinator; no git command run in it. Compiler:
`clang -I runtime seed/heroes.c runtime/runtime.c -o heroes` in that folder,
`real 38.17 user 12.68 sys 0.51` (`/usr/bin/time -p`; the ratio says the
machine is loaded by the lanes, the build itself is 12.7 s of CPU), binary
dated 16:33. Every `./heroes` below is that one. Time box 25 minutes, so to
about 16:58; what is not reached is written as unrun.

Written as it goes; sections appended in the order the checks ran. Every
count below is `find <dir> -name '*.hero' | xargs cat | grep -cE <pattern>`
in my copy unless another command is named; `git ls-files` was not available
to me (no git), and `find` reads the same 3229 files once my eight probe
copies under `critic-probes/` are subtracted (3237 read).

## 1. False or imprecise, with the correct reading

1. **The shapes table's patterns are not where the shared brief says.**
   `00-shared.md:80` sends the reader to `compiler-engineer.md` for them;
   that brief holds no declaration, `=` or mutation pattern (its only greps
   are the `@`-argument one and the width one). The one declaration pattern
   printed anywhere, `[a-z_][a-z0-9_]*: [^=@]+ @ ` (`00-shared.md:55,92`),
   is unanchored and reads **6161** over `selfhost/`, not 5926; the table's
   5926 is the line-anchored `^[[:space:]]*[a-z_][a-z0-9_]*: [^=@]+ @ `.
   With patterns of my own the other columns reproduce only approximately:
   `^[[:space:]]*[a-z_][a-z0-9_]* = ` reads 10225 / 833 / **8814** (table
   8812); `^[[:space:]]*[a-z_][a-z0-9_]*: [^=@]+ = ` reads 847 / 101 / 422
   exactly; `^[[:space:]]*[a-z_][a-z0-9_.]*(\[[^]]*\])*(\.[a-z_][a-z0-9_]*)* @ `
   reads **7549 / 909 / 3631** (table 7553 / 909 / 3649). The method itself
   is fragile by one: `xargs cat` joins a file with no final newline to the
   next file's first line, and `examples/` read 776 or 777 declarations by
   file order. Repair: print the four patterns in the brief.
2. **`spec-warden.md:19` "seven [changed lines] for B1, eight for B2"**:
   `diff spec/heroes-spec.md spec-B1.md | grep -cE '^[<>]'` reads 16, eight
   lines each side (43, 131, 132, 134, 150, 291, 293, 410), and B2 reads 18,
   nine lines (135 added). Seven and eight are the HUNK counts
   (`grep -cE '^[0-9]'`).
3. **`spec-warden.md:38` "design.md §4.5 (`docs/design.md:1142`, *exactly two
   cases are not*)"**: 1142 is the heading `### 4.5 Type inference`; the
   sentence `**Exactly two cases are not**, and both are "empty container"`
   is at **1150** (`grep -n 'Exactly two cases' docs/design.md`).
4. **`ffi-pragmatist.md:27` "`selfhost/ast.hero:631`, the `Param` record"**:
   631 is a `Param(...)` constructor inside `ps: [Param] = [` (630), a test
   helper; `record Param` is at **361** (`grep -n '^record Param'`), its
   marks `owned` 370, `consumes` 373, `acquires` 378, `transfers` 379 on.
5. **`llm-ergonomist.md:83` "the three briefs' texts are kept in
   `docs/panel/209-briefs/blind/`"**: `ls` says No such file or directory.
   They exist only as `209-blind/brief-t{1,2,3}.md`.
6. **The `@`-argument count** (`compiler-engineer.md:54`,
   `ffi-pragmatist.md:24`: 5559 / 438 / 3 by `(\(|: )@[a-z_]`): the pattern
   misses an `@` argument after a comma. `(\(|: |, )@[a-z_]` reads **6607 /
   500 / 4**. True of the pattern, 19% low as a count of sites.
7. **`00-shared.md:98` "1551 (`[]`, `{}`, `fail(`) keep their annotation under
   R1 by today's `cannot_infer` rule"**: `x = ok(5)` fires `cannot_infer` today
   too (`critic-probes/born_ok2.hero`, exit 1, *`ok(…)` builds a fallible
   value, and which one comes from the context*), so the 8 `ok(` cells keep
   it: the lower bound is **1559**. The classes otherwise hold: `[]` 1223,
   `{}` 277, `fail(` 51, `ok(` 8, bool 342, string 324, `.case` 55 exact;
   number 1298 (table 1299); non-empty array 150 by `^\[[^]]` (table 169, the
   19 a `[` ending its line); lowercase names and calls 2129 plus 62 uppercase
   constructors (table 2178). The sum is 5926.
8. **`spec-warden.md:54`'s R6 count `grep -cE '^[a-z_][a-z0-9_]* = '`** reads
   2 (`:130`, `:226`) and misses the two indented ones inside the § 4 fence
   (`:98` `dx = a.x - b.x`, `:99`); `^[[:space:]]*` reads 4. Backticked prose
   `b = a` (`:76`) and `_ = …` (`:141`, `:143`, `:237`) are outside both.
9. **`00-shared.md:86` prices the trunk; the change lands on the round.**
   `lane-round-b18` is at `.claude/worktrees/lane-round-b18/` (not under
   `scratch-b15/`). Its `selfhost/` holds **6264** line-anchored declarations
   against the trunk's 5926 (+338), 100 files differing (`diff -rq`). The
   migration and R1b counts priced on `87794631` undercount the tree.
10. **1190 of the 5926 `selfhost/` declarations stand inside `test` blocks**
    (awk dropping each file's `test "` bodies; the complement counted), and
    `heroes build` never compiles a test block: `critic-probes/decl_in_test.hero`
    runs exit 0 with a cell in its test, and `heroes test` compiles it (1
    test, passed). "Compile the tree with it" (compiler-engineer measurements
    2 and 3) misses 20% unless `heroes test selfhost/main.hero` runs too.
11. **What else quotes a declaration and nobody's migration list names**:
    32 `.applied`/`.fixed` goldens (`find tests -name '*.applied' -o -name
    '*.fixed' | xargs grep -lE '^[[:space:]]*[a-z_][a-z0-9_]*: [^=@]+ @ '`),
    beside the 23 `.expected`; **839** lines of
    `tests/golden/recovery/plan-singles.jsonl` embed one; `docs/design.md` 19
    lines, `site/src` **25** (both editions' docs pages: `total: i64 @ 0`,
    `out: [B] @ []`; a push publishes them), `docs/learn` 3, `docs/records` 2,
    `docs/roadmap` 1; and `editors/vscode/syntaxes/heroes.tmLanguage.json:130`
    matches `@` alone (`keyword.operator.mutable.hero`).
12. **Where `@=` would join**: `compiler-engineer.md:22` says the lexer names
    no `"@"` and to find the arm; the place is `selfhost/punctuation.hero:26`,
    `punct_at(text, at)` reading `b0 b1 b2`, *longest match first* (its
    comment at `:24`), not `lexer.hero`.

## 2. Holds, each with its command

- Spec lines, `sed -n`: `:43` `b: u8 @ 255`; `:129-133` the fence, `:134-135`
  the prose; `:136` *Signatures are always explicit; inference is local only*;
  `:138` *a write is not [a use]*; `:150` the `Binding` production; `:291`,
  `:293`, `:410`; `## 13. FFI` at `:346`, its first fence `:358` opens the
  sqlite extern group, `end_lease(@x)` `:411`; the five lines the pattern
  prints are 43, 131, 291, 293, 410 (four beside 131).
- design.md: `:1086-1091` the journey; `:1102-1115` *Why the type is
  mandatory* with the `totl` example at 1106-1110 and *read on that very line*
  at 1112; headings §1.0 112, §1.11 467, §1.12 575, §4.4 1072, §4.5 1142,
  §4.17 2124; the words *type REQUIRED* / *mandatory on* only at 1078, 1102,
  1113.
- `selfhost/token.hero:157,179,205,219` carry `.eq | .at`; `:261`
  `.at => "at"`; `.colon_colon` 67, `.and_and` 92. `lexer.hero` 1218 lines,
  0 `"@"`. `grammar_expr.hero:1010` the `.declare(...)` return, 1767 lines;
  `suite_layout.hero:480,482,484` the three ceilings 1175 / 1085 / 550.
  `parse/annotation.hero:54`; `parse/at_prefix.hero` 263 lines, `:62-63` the
  `@v`/`@@v` comment; `ast.hero:60-70` with the §4.4 comment at 64-66;
  `resolve/binding.hero:13-30`; `print/fmt.hero:755,770` `" @ "`;
  `print/bodies.hero:55,63`; `print/scopes.hero:110`; `ir/print.hero:158`;
  `cli/grammar.hero:14-17`; `./heroes grammar` prints `Binding   =` (its line
  64); `suite_spec.hero:366,368` `named(@r,…)`, `rejected(@r,…)`;
  `suite_special.hero:385-419` builds the spec's sqlite block. Goldens
  `fixedbugs-131-a-sigil-*` (4 of about 60 `131` cases) and
  `fixedbugs-179-a-sigil-before-a-statement-s-word-is-one-message` exist.
  `docs/metrics/operators.md` exists.
- Counts: files 557 / 120 / 1697; 3229 `.hero`; 1671 files with an anchored
  declaration (1678 unanchored); 1344 `.expected`, 23 quoting one; `_ =` 353 /
  47 / 1013 inside the inferred column; no declaration ends its line in `@`
  (0, value on the next line does not occur); the 24 `ident: X@…` lines with
  no space are record fields like `result: apply(@c, …)`, not declarations;
  `: (ptr|cstr) @ ` 6; `^extern` 21 files; the width pattern with a literal
  first value reads 25 of 31 non-`i64`-width declarations in `examples/`.
- Measures (`./heroes measure`): trunk 7348 / 7479, pinned real 9847
  claude-opus-5 2026-10-09, *Headroom: 393 … FFI floor mortgages 60 …
  measured against the ceiling is 9907 … red at 10240* (so 333 holds); B1
  7356 / 7487; B2 7372 / 7503 (+8 vendored on B1); round's spec, copied,
  7488 / 7622, its own binary printing real 10021 2026-10-10 (219 headroom,
  159 net); `tokens/as_is.hero` and `at_eq.hero` 5926 lines each, 65322 /
  72855 and 48677 / 56590. Tags `v0.1.0`, `v0.2.0` (`.git/packed-refs`).
  The trunk's `heroes` is dated Oct 10 00:26 (`ls -la`).
- The round touches none of `selfhost/lexer.hero`, `selfhost/ast.hero`
  (`cmp` identical), `selfhost/parse/`, `selfhost/print/` (`diff -rq` empty);
  its spec diff is 22 lines (`<` + `>`) in 4 hunks.
- Probes, all eight re-run (`./heroes run`, exits 0 1 1 0 0 1 1 1 in the
  folder's order): `x: i64 = 5` prints 12; `v @= 0` `expected_expression` at
  2:8; `xs = []` `cannot_infer` fix `guess`; `v: i64 @ 0` then `print(v)` exit
  0; `t3_repaired` prints 34; `totl` `unknown_name` 4:9 fix `certain`; `v @ 0`
  `unknown_name` at 2:5 and 3:5 and no word of the declaration's shape;
  `totl: i64 @ 0` written never read `unused_binding`. t3-A: exit 1,
  `unknown_name` 4:9 certain fix `total`, nothing else; t3-B:
  `expected_expression` at 2:12, 8:12, 10:10; the program's declarations are
  on lines 2, 8, 9, 10, the re-binding 12, the attempt 4. Blind folders: A's
  `spec.md` `cmp`-identical to the trunk's, B's to `spec-B2.md`, briefs
  identical across A and B, `t3-*/main.hero` present; `run.sh` has
  `claude-opus-5-5`, `--restricted --safe-mode --strict-mcp-config`,
  `Read,Write`, `--max-budget-usd 0.6`; `claude --version` 2.1.286.
- **`@` followed by `=` is legal nowhere today**: beside `v @= 0`, an `@`
  argument `bump(@=n)`, `v @ = 0` and `v @ =1` each read `expected_expression`
  at the `=` (`critic-probes/at_eq_in_call.hero`, `at_eq_spaced.hero`,
  `at_space_eq_mut.hero`). `grep -rF '@='` over the whole tree minus `.git`
  and the 209 folders: 0; per tree (spec, selfhost, tests/golden, design.md,
  docs/panel, runtime, examples, seed, editors, site/src, docs/learn, issues,
  docs/metrics, tests/harness): 0 each. `=@` as a byte pair exists:
  `tests/golden/recovery/plan-singles.jsonl:4460,4552` (`out=@db`,
  `tail=@tail`), planted by the operator `named-arg-equals`.
- **No sitting has had the mandatory type on the table**: eleven patterns of
  mine (`type is REQUIRED`, `type is mandatory`, `mutable declaration`,
  `(cell|mutable).{0,40}infer`, `let mut`, `declaration.{0,30}annotation`,
  `optional (type|annotation)`, `infer.{0,30}(declaration|@)`,
  `declar.{0,40}without (a |the )?type`, `(drop|omit|strip).{0,30}(type|annotation).{0,40}(cell|mutable|@)`,
  `symbol.{0,30}declar|declar.{0,30}symbol`), `grep -rniE` over
  `docs/panel/[0-9]*.md`, `issues/`, `docs/learn/`, `docs/records/journal/`,
  `docs/records/contract/`: every hit is about something else (HeroStr in C,
  function-type parentheses, generics' `cannot_infer`, a C symbol declared
  twice). `Binding   =` (three spaces) lives in `spec:150` alone; the quotes
  in panel reports and the ledger write `Binding =`.
- `b: u8 @ 255` then `b @ big` with `big = 300` (an `i64`): `type_mismatch`
  at the mutation, 4:9 (`cell_narrow.hero`); `b = 255` passed to a `u8`
  parameter: `type_mismatch` at the use (`u8_from_i64_binding.hero`).
- **The `spec` suite with B2 in place of the spec, in my copy** (output in
  `critic-probes/spec-suite-B2.out`, 16:39:51 to 16:40:59): `spec: 19 passed,
  4 failed`, the four `spec/budget` (7503 against 7479), `spec/spendable`,
  `spec/real`, `spec/ledger`, all the count and the pin; `named` and
  `rejected` stay green with `@=` in code spans under today's lexer. No row
  waits on the lexer.

## 3. Could not run, and why

- The real rows B1 **9844**, B2 **9865** and the −3: a paid `--refresh`; they
  stand unverified. The round's 10021 is read from its binary's pin, not
  refreshed by me.
- `57d8f27f` 2026-08-03 (`git log -S"type REQUIRED"`) and `4139adbf`
  2026-08-04 (`-S"write is not"`): no git in my pass; a read-only `git log -S`
  on the trunk settles both in seconds.
- The `grammar` suite with B2 in place: unrun, time box.
- The historian's external facts (PEP 465's year, `unused_mut` a warning,
  Swift's wording): no web tool in this pass; the seat's own duty.
- `209-coordinator/probes.txt` exists; not re-read line by line, the eight
  `.hero` files reproduce it.

## 4. Routes the eight do not cover, and questions not asked

- **R8, a cell typed from all its writes in the block** (the brief's first):
  design.md `:1144` *Inference applies only to locals … errors stay local*
  and §4.4's own reason are against a type that depends on a line below;
  the price is the one `cell_narrow.hero` shows moving: today's
  `type_mismatch` at the mutation would become a type chosen silently. The
  instrument is the prototype counting cells whose later write is wider than
  the first value, which nothing counts today.
- **R9, keep R0 and repair the `v @ 0` message** (the brief's second): today
  two `unknown_name` messages for one mistake (2:5 and 3:5), neither naming
  `name: Type @ value`, against design.md §4.17's promise and the `adjacent`
  class's own definition (a second message for one mistake). No spec token.
  Instrument: a `check` golden and `operators.md:13`'s `forget-at-decl`.
- **R10, the symbol without the inference**: `v: i64 @= 0`, the type kept.
  B1 bundles two changes, a new declaration symbol and an optional type, so
  the blind seat's A/B cannot say which one moved a reading; a third variant
  or a route that lands one at a time would.
- **R11, `@v = 0`** (the sigil on the name): already occupied by
  `at_prefix.hero`'s recovery and `fixedbugs-131-a-sigil-before-a-name-*`;
  list it to refuse it with that reason.
- **B2's sentence against the spec's own example**: `suite_special.hero:419`
  builds the spec's sqlite block with `db: Db @ nullptr` then
  `sqlite3_open(path: …, @db)`: a cell re-bound only through an `@`
  argument. *A cell nothing re-binds is a compile error* does not say whether
  `@db` re-binds; `:138` says a write through an `@` parameter is a use,
  which is a different sentence. Under B2 as written the harness's own main
  for the spec is refused or not by a rule the document does not state.
- **The `@=` token where an argument stands**: the compiler-engineer's brief
  keeps `(@x` three tokens and says nothing of `(@=x`; `bump(@=n)` is
  `expected_expression` today and under R1 lexes `@=` whole, so the parser
  owes *an `@=` where an argument stands* with its fix, a shape beside the
  one repaired.
- **The metric's operators move with R0**: `docs/metrics/operators.md:13-14`
  name `forget-at-decl` (`v: int @ 0` → `v = 0`) and `mutate-undeclared`
  with *mandatory type on declaration (§4.4)* as the catching rule; under R1
  a new operator is owed (`v @= 0` → `v @ 0`, and `v @= 0` → `v = 0` with a
  later `v @ …`), and the recovery corpus's 839 embedded declarations are
  re-planted. Which seat re-reads the table is unasked.
- **Which tree is priced**: the round at 6264 declarations and the trunk at
  5926; the 1190 inside test blocks; the 32 `.applied`/`.fixed` goldens; the
  site's 25 outward-facing lines and design.md's 19; the editors' grammar.
  None of the eight routes' prices names these, and the two-step landing
  (`compiler-engineer.md:74-78`) counts `.expected` alone.

Finished 2026-10-10 16:46 CEST (`date` read at the last command 16:43:24,
the writing after it).

