# Panel 227, compiler-engineer

Written as it goes, 2026-10-21. Worktree `.claude/worktrees/227-compiler-engineer`, detached at
`63c78c54b` (`git worktree add --detach`, run 12:37). Compiler: `clang -I runtime seed/heroes.c
runtime/runtime.c -o heroes` (`real 8.99`), then `./heroes build selfhost/main.hero -o heroes-next`
(`real 83.50 user 63.11 sys 14.20`); `./heroes-next --version` prints `heroes 0.4.0` / `sources
79fe404274caacd0`, the key the shared brief names. Probe programs are under the worktree's
`build/227probe/`. No credential printed, no paid run, no API call, no commit.

(Sections 1 to 8 are the measurements; § 9 is the verdict in the seat's structure; § 10 says what is left in the worktree.)

## 1. Where a receiverless `slice` is told today (confirmed, with one addition)

`wc -l` on the worktree: `selfhost/check/walk.hero` 2477, `selfhost/contextless_errors.hero` 213,
`selfhost/check/labels.hero` 484, `selfhost/label_errors.hero` 373, `selfhost/check/builtins.hero`
549, `selfhost/inventory.hero` 319, `selfhost/diag.hero` 277.

- `builtin_shape`: pushed at `check/walk.hero:1283`, the last line of `builtin_dispatch`
  (`walk.hero:1226-1284`), built at `contextless_errors.hero:173`. **Confirmed.**
- `wrong_label`: `check/labels.hero:42` (`inventory.labelled_of`) → `:76` →
  `label_errors.hero:42`. **Confirmed.**
- **The cause, which the brief does not name**: `check/labels.hero:51-52`,
  `if params[position].len() == 0` / `return`. `builtin_dispatch` asks `check_label` of each
  argument at `position: collected.len()`, a count that includes the receiver
  (`labels.hero:36-37`, the comment above `check_label`), and `slice`'s position 0 is the receiver, named `""`
  (`inventory.hero:245`). So in `slice(from: 0, to: 5)` the label `from:` at position 0 is
  **never asked** (the early return), `to:` at position 1 is asked against `from` and refused
  `wrong_label`, and `builtin_call` (`builtins.hero:214`, `given.len() == 3` only) falls through
  to `builtin_shape`. The two messages are one fault read twice: the label at the unlabelled
  position is silent, and everything after it is read one position off.
- The same early return is half of item 8 below: `slice(xs: line, from: 0, to: 5)` and
  `slice(s: line, …)` exit 0 through it. The other half is `labels.hero:44-45`: a built-in with no
  `Labelled` entry (every one but `fail` and `slice`, `inventory.hero:243-246`) returns before any
  label is read, so `len(foo: line)` exits 0.
- **A user function checks its count before any label and returns** (`walk.hero:1558-1565`,
  `value_errors.arity`), which is why `cut(from: 0, to: 2)` draws one `wrong_arity`. A built-in
  has no count to check first: the inventory carries a name and a tier only, and
  `check/builtins.hero:8-14` says the rules are hand-written because `print` is variadic and no
  signature table fits. That is the fact route E's cost turns on.

## 2. Today's messages, every shape, measured

`./heroes-next check --brief <f>` and `check --brief --permissive <f>`, each shape as line 4 of a
`main` holding `line = "hello world"` (files `build/227probe/shapes/*.hero`):

| shape | strict (`--brief`) | `--permissive` |
|---|---|---|
| `slice(from: 0, to: 5)` | `builtin_shape` 2 + `wrong_label` | `builtin_shape` |
| `slice(from: 0)` | `builtin_shape` 1 | same |
| `slice(to: 5)` | `builtin_shape` 1 | same |
| `slice()` | `builtin_shape` 0 | same |
| `slice(line, from: 0)` | `builtin_shape` 2 | same |
| `slice(0, 5)` | `builtin_shape` 2 + `needs_label` | `builtin_shape` |
| `slice(from: 0, to: 5, line)` | `bad_operand` + `wrong_label` + `needs_label` + `type_mismatch` | `bad_operand` + `type_mismatch` |
| `slice(to: 5, from: 0)` (not in the brief) | `builtin_shape` 2 | same |
| `line.slice(from: 0)` (not in the brief) | `builtin_shape` 2 | same |
| `line.slice(from: 0, to: 5, 7)` (not in the brief) | `builtin_shape` 4 | same |
| `xs = [1,2,3,4]`; `slice(from: 0, to: 2)` | `builtin_shape` 2 + `wrong_label` | `builtin_shape` |
| user `cut(from: 0, to: 2)` | `wrong_arity` + note `declared at line 1: …` | same |
| `range(to: 3)` | `wrong_arity` + note `declared at <heroes library>:26: …` | same |

### A shape beside it that is a defect of its own (not repaired; a filing)

`slice(from: 0, to: 5, line)` in the full form prints `error[needs_label]: … — this one is \`to\``
with **`fix (certain): write \`to: \``**, and `check --apply` writes `slice(from: 0, to: 5, to:
line)`, a call holding `to:` twice, which `check --brief` still refuses three ways. **It reaches a
user function too**: `function cut(s: str, from: i64, to: i64)` called `cut(from: 0, to: 5, line)`
gets the same `fix (certain): write \`to: \``, and `--apply` writes `cut(from: 0, to: 5, to:
line)` (`build/227probe/shapes/u4_cut_trailing.hero`). Cause: `label_errors.needs_label`
(`label_errors.hero:111-118`) takes its certainty from `others` alone (`label_fixes`, `:150-155`)
and never asks whether the label it writes is already written in the call, the `doubled` question
`wrong_label` asks (`label_errors.hero:50`, defect 135). `.claude/rules/diagnostics-and-goldens.md`
line 23: *a `certain` fix repairs the defect the diagnostic names; a fix that leaves the defect
standing is a `guess`*. Searched first: `gh issue list --state all --search` for `needs_label
certain`, `needs_label doubled`, `label already written`, `label twice` found defect 135 (issue
1415), whose body covers `needs_label`'s *others* and not a label already written. A question
rather than a premise, since the search is my vocabulary.

## 3. The prototypes, built and run

Both live in one new module, `selfhost/check/builtin_count.hero`, called from `builtin_dispatch`
**before any label is read**. No file the call could live in has the room: `check/labels.hero`
measures 297 of 300, `check/builtins.hero` 370 of its `DECIDED` 378, and a module that pushes a
diagnostic cannot be `inventory.hero`. Counts are `code_lines`, the `layout` suite's unit, read
through its declared mirror `.claude/hooks/ceiling.py` (`python3 -I -c '... ceiling.code_lines(...)'`).
Every file, every variant and the pristine copies are kept in the worktree's `build/227variants/{a,e,e2,e4}/`
and `build/227orig/`; outputs in `build/227out/`.

**A first cut returned before the arguments were walked, as `user_call` does, and that hid a
mistake inside one**: `slice(from: "a" + 1, to: 5)` is told `mixed_arithmetic` today and was told
the count alone, the `+` waiting for a second turn (verification.md's `adjacent` shape, *a mistake
told only after the first is fixed*). The prototypes below keep walking the arguments and skip only
the label reading (`if !counted` around `labels.check_label`, `if counted` / `return` after the
loop), so `heroes-a` and `heroes-e` print `wrong_arity` **and** `mixed_arithmetic` on it
(`build/227probe/shapes/in_arg.hero`). **A declared function still hides it**: `cut(from: "a" + 1,
to: 5)` draws `wrong_arity` alone on every compiler here, today's behaviour (`walk.hero:1558-1565`
returns before its arguments), a shape beside to file, not repaired.

### Route A (`heroes-a`, `sources 501cc868166f815e`)

Trigger: a built-in whose `Labelled` entry names its first position `""` (`slice` alone today), no
receiver, fewer arguments than its positions, and a first argument labelled with a later position's
name (`labels.names_a_position`). **Code: `wrong_arity`**, `value_errors.arity` with no
`DeclaredAt`, and the note `the value \`slice\` works on comes first: \`xs.slice(from: a, to: b)\`,
or \`slice(xs, from: a, to: b)\``, generated from `inventory.labelled_of`'s labels rather than
written per built-in.

Cost, `code_lines`: `check/walk.hero` 1869 → **1874** (a `use`, `counted = …`, `if !counted`, `if
counted` / `return`), **4 over its `DECIDED` row of 1870**; `check/builtin_count.hero` **53** new
(`wc -l` 66). **+58** in the instrument's unit. Lexer, parser, descriptors, ownership, IR, emitter:
**0**. Erased in the checker; nothing reaches the lowering.

### Route E (`heroes-e`, `sources 5666d2623231e108`)

Trigger: a built-in with one count (`inventory.arity_of`, a `match` of 27 names: `slice` 3; `push`,
`join`, `repeat`, `default` 2; `len`, `chars`, `keys`, `sort`, `must`, `is_err`, `cstr`, `lease`,
`end_lease`, `validated_bytes`, `ptr` and the eleven `to_*` 1), called with another, the receiver
counted. Absent for `print` (any number), `ok` and `fail` (constructions, already `wrong_arity` at
`walk.hero:2008`, `:2019`), and the eleven library built-ins (already `wrong_arity` through their
declarations). A test holds every name of `NAMES` to a count or to that list of fourteen, so a
built-in added with no count fails the compiler's own tests instead of falling to `builtin_shape` in
silence (`.claude/rules/module-shape.md`, a premise written as a falsifiable claim). **Code:
`wrong_arity`**; the note, reworded after reading it on `slice(line, from: 0)` where *comes first*
read as a claim about the call: `` `slice` is called `xs.slice(from: a, to: b)` or `slice(xs, from:
a, to: b)`, the value it works on first ``, on a built-in whose first position is unlabelled, and no
note elsewhere (`value_errors.hero:199-201` already rules that a built-in has no declaration to
cite and a note saying so is worse than none).

Cost, `code_lines`: `check/walk.hero` 1869 → **1874**, the same five lines, **4 over its row**;
`check/builtin_count.hero` **51** new; `inventory.hero` 231 → **250** (`arity_of` and its comment,
19; its test, not counted). **+75** in the instrument's unit. Lexer, parser, descriptors,
ownership, IR, emitter: **0**.

### Which shape each route tells in one diagnostic (`check --brief`, strict)

| shape | today | route A | route E |
|---|---|---|---|
| `slice(from: 0, to: 5)` | 2 | **1** `wrong_arity` 3/2 + note | **1** `wrong_arity` 3/2 + note |
| `slice(from: 0)` | 1 `builtin_shape` | **1** `wrong_arity` 3/1 + note | **1** `wrong_arity` 3/1 + note |
| `slice(to: 5)` | 1 `builtin_shape` | **1** `wrong_arity` | **1** `wrong_arity` |
| `slice(to: 5, from: 0)` | 1 `builtin_shape` | **1** `wrong_arity` | **1** `wrong_arity` |
| `slice()` | 1 `builtin_shape` | 1 `builtin_shape`, unchanged | **1** `wrong_arity` 3/0 + note |
| `slice(line, from: 0)` | 1 `builtin_shape` | unchanged | **1** `wrong_arity` 3/2 + note |
| `slice(0, 5)` | 2 (`builtin_shape`, `needs_label`) | unchanged, 2 | **1** `wrong_arity` 3/2 + note |
| `slice(from: 0, to: 5, line)` | 4 | unchanged, 4 | unchanged, 4 |
| `line.slice(from: 0)` | 1 `builtin_shape` | unchanged | **1** `wrong_arity` 3/2 + note |
| `line.slice(from: 0, to: 5, 7)` | 1 `builtin_shape` | unchanged | **1** `wrong_arity` 3/4 + note |
| array, `slice(from: 0, to: 2)` | 2 | **1** | **1** |
| `len()`, `push(xs)`, `repeat("a")`, `default(m)`, `to_u64()`, `xs.len(1)`, … (23 probed) | 1 `builtin_shape` each | unchanged | **1** `wrong_arity` each |

Every message text above is in `build/227out/shapes-{base,a,e}.txt` and `arity-heroes-{next,e}.txt`.
**Neither route reaches `slice(from: 0, to: 5, line)`**: its count is right, so no count rule can;
its four messages come from the silent label at position 0 (`labels.hero:51-52`, item 8) and
`needs_label`'s doubled certain fix (§ 2 above). The route that reaches it is item 8's repair.

### The code, and whether a new code is a new class

**`wrong_arity`, an existing code, for both routes.** Three reasons, each a fact read here: (1) the
same mistake on a declared function is already `wrong_arity` (`cut(from: 0, to: 2)`, `range(to:
3)`, and the eleven library built-ins), so a reader who learned one learns both, where today the
language tells one mistake two ways by whether the callee is Tier 1; (2) it is not a thesis rule by
`diag.hero:129-131`'s test (*a rule without which the program has no meaning*: a call short of an
argument has none), so both arms print it, as both print `builtin_shape` today; (3) defect 458's
cases already ruled that *the count is the built-in dispatch's alone to tell*
(`tests/golden/check/fixedbugs-458-*.hero` headers), and route E is that ruling with the count
told before the labels rather than after.

**Is a new code a new diagnostic class in CLAUDE.md § 4's sense?** The rules do not define it (the
critic's search, C3; mine, run: `grep -n -i 'diagnostic class'` finds `CLAUDE.md:178` and
`.claude/skills/panel/SKILL.md` lines 3 and 26, each naming the class as a trigger and none
defining it; `grep -n -i class .claude/rules/diagnostics-and-goldens.md` prints nothing; the
same grep of `docs/design.md` prints nothing). My answer, from what keys on a code:
`diag.is_thesis_rule` partitions the arms by code (`diag.hero:192-228`), the `annotations` suite
asks a witness per thesis code, every golden's `#~` mark is a code, and `check --json` publishes it
as `code` (design.md §4.17's schema 2). A new code is therefore a new name in the language's public
vocabulary of mistakes, and **I read it as a new class** whenever it names a mistake no existing
code names; a reworded message or a new note under an existing code is not. Neither route needs
one, so the question does not bind this sitting; it is the coordinator's to write down if the
synthesis takes my reading.

**Fixes: none, on either route.** The value sliced is not in the call (`slice(from: 0, to: 5)`), so
no edit can be `certain`, and a `guess` would have to invent a name; the note shows the shape in
prose instead, which is what `value_errors.arity` does for a declared function.

### Paying the four lines instead of raising the row (variant E2, `heroes-e2`, `sources 6758e04670a04ec3`)

`builtin_dispatch` already holds a rule of the same concern, *what a built-in call's arguments are
refused for before they are typed*: §4.8's `@` marker on a built-in's argument
(`walk.hero:1234-1247`, a comment of 8 lines and the rule's 5). Moved unchanged into the new module,
renamed `check/builtin_args.hero` for its two rules, it pays route E's lines along that seam:
**`check/walk.hero` 1869 → 1862, under its row with 8 to spare**; `check/builtin_args.hero` 63;
`inventory.hero` 250. **+75** in the unit, the same as E, and **no `DECIDED` row moves**. The same
move pays route A's lines too (not built; its walk.hero lines are E's five).

## 4. Which arm sees the new message, and what Part 11 can see

Measured with each compiler's `check --brief --permissive` on every shape
(`build/227out/shapes-{base,a,e}.txt`): **under either route the strict and the control arm print
the same one message on every shape the route reaches**, `wrong_arity`, which is not a thesis rule
(`diag.hero:193-221` lists 27 codes; `wrong_arity` and `builtin_shape` are not among them, and by
`diag.hero:129-131`'s test cannot be: a call short of its value has no meaning without the rule).
Today the strict arm prints one more on `slice(from: 0, to: 5)` and `slice(0, 5)`, a `wrong_label`
or `needs_label` the control arm drops; the routes remove a strict-only message, add none, and move
no code into or out of `is_thesis_rule`. **The `annotations` suite does not move for a thesis
reason**: it reads 1195 and 0 under A and 1196 and 0 under E, one case more per case added (A adds
one, E two; the base is 1194 by subtraction, not run), and `wrong_label` keeps its witnesses elsewhere.

**What that means for strict minus control, measured on 050's own run** (read-only,
`.claude/worktrees/thesis/build/thesis-runs/00fb571c-v3/verdicts.jsonl`, 270 rows: `brief`,
`strict` and `control`, 90 each, one turn-1 generation graded by each arm; a Python tally of each
row's turn-1 `codes`): Haiku's 9 samples drawing `builtin_shape` at turn 1 went green at turn 2 in
**8 of 9 in every arm**, `control` included, where no `wrong_label` was ever printed; the ninth is
`6f236494` sample 2 in all three arms, which also held `repeat`'s count, compiled at turn 2 and ran
wrong. So **the strict arm's extra, misleading `wrong_label` cost nothing measurable in 050**, and a
message that removes it has, in this data, nothing left to move: turn 1 is refused in both arms
before and after (no route changes whether the program is refused), and turn 2 was already 8 of 9
in both. Sonnet 5.5 and Opus 5.5 drew no `builtin_shape` at all.

## 5. What the change owes, run

| run | route A (`heroes-a`) | route E (`heroes-e`) | E2 paid (`heroes-e2`) | E4 = E2 + § 6 note (`heroes-e4`) |
|---|---|---|---|---|
| `check` | 704 / 0 | 704 / 0 (699 / 4 before the 4 cases moved) | 704 / 0 | 704 / 0 |
| `full` | 40 / 0 | 41 / 0 | 41 / 0 | 42 / 0 |
| `permissive` | 22 / 0 | 22 / 0 | 22 / 0 | 22 / 0 |
| `fixes` | 1004 / 0 | 1004 / 0 | 1004 / 0 | 1004 / 0 |
| `annotations` | 1195 / 0 | 1196 / 0 | 1196 / 0 | 1197 / 0 |
| compiler's own tests | 1739 passed | 1740 passed | 1740 passed | 1740 passed |
| `layout` `check/walk.hero` | **red, 1874 past 1870** | **red, 1874 past 1870** | green | (E2's files) |
| `layout` new module, `inventory.hero` | green | green, green | green, green | |

Every count is `./<compiler> run tests/harness/main.hero -- ./<compiler> <form>` into
`build/227out/suite-<variant>-<form>.txt`, or `./<compiler> test selfhost/main.hero`; the base
compiler's own-test count was not run here (the tree held a variant's sources at every gate, and
`.claude/rules/verification.md` § A suite is the last judge says the guard refuses a compiler built
from other sources than the tree's; I did not trigger it). The one line `internal error: linking failed` in
each own-test output is a test corrupting its own cached objects on purpose
(`selftest-e.txt:95-100`).

**Golden cost.** Route A: **no existing golden moves**, one case added
(`check/fixedbugs-2175-a-slice-with-nothing-to-slice-is-told-its-count`). Route E: the four cases
that pin a built-in's count as `builtin_shape` move, mark and line (8 files, 14 lines, defect 458's
and 439's, whose headers already say *the count is the built-in dispatch's alone to tell*), and two
are added (`check/fixedbugs-2175-a-built-in-given-the-wrong-count-is-told-its-count`,
`full/fixedbugs-2175-a-slice-with-nothing-to-slice-names-its-shape`). **The second witness worked
during this sitting**: my first route-A case left `xs` unread, the resolver's `unused_binding`
stopped the checker, the `.expected` I took from the run held only that, and `check` passed it;
`annotations` refused it (1194 / 1).

**Route A's sentence in `spec/heroes-spec.md`** (`after-a.md` copied over it, `cmp` of `before.md`
against the worktree's spec first: identical), judged by `heroes-e`, whose sources the spec does not
enter: `special` 10 / 0, `grammar` 9 / 0, `fixes` 1004 / 0, `unseen` 3 / 0, and **`spec` 27 / 4**,
the four being the pins the change owes in its own commit and nothing else: `spec/budget` (*the
spec measures 7856 and the record says 7846 — a delta of 10*), `spec/spendable` and `spec/real`
(`measure`'s printed `real 10286` pin, `selfhost/cli/measure.hero`), `spec/ledger` (the ledger's
newest row 7846). **`spec/offered` passes, and it reads nothing of `slice`**: `spec_text.hero:164`
keeps a fragment only where it holds no space, so `slice(from:, to:)` today and `xs.slice(from:,
to:)` after are both skipped, as are `join(xs, sep)` and `repeat(s, n)`. **The critic's G9, run**:
with `` `len(xs)` `` in place of `` `len` `` the same suite prints *FAIL spec/offered: the spec
offers these and the compiler has no such built-in: len(xs)* (26 / 5), so **route B owes a change to
`tests/harness/spec_text.hero`** as well as its tokens.

**Panel 187's R2 corpus.** Read, not run (the mutants' messages are not stored; the run is the
census-scale `mutate` differential): a Python scan of `tests/golden/recovery/plan-{singles,pairs,base}.jsonl`
finds 86 single mutants whose rewritten text holds `slice(`, 0 pairs, 0 base, and 5 holding a
`slice(` with no `.` before it; none is a `slice` short of its value (one has `xs.` on the line
above, four pass the value first), and no operator of the 93 removes a receiver. So **route A moves
0 mutants there** by construction of its trigger. Route E moves any mutant that reaches the checker
with a built-in's count wrong; how many is **unrun**.

## 6. `repeat`'s count: the conversion note (prototyped and gated)

Today an integer of another width is told `expected \`u64\`, found \`i64\`` and nothing else **in
every context**, `repeat`'s count, `m: u64 = n` and a declared `u64` parameter alike, while a `u64?`
there already draws `fix (guess): .must()` (`check/mismatch.hero:35-38`;
`build/227probe/repeat/r1.hero`). So the note belongs to integer-width mismatch, not to `repeat`,
and it is **within §4.17's promise by §4.17's own example** (§4.17 opens at `docs/design.md:2267`; its model
message ends `possible fixes:` / `- convert:  to_i64(user.id)`, lines 2294-2295).

The prototype, `check/mismatch.hero`'s `compare`: where both sides are integers, the note `` `to_u64(x)`
converts it to `u64?`, as every integer conversion gives a `T?`: `.must()` aborts on a value that
does not fit, `.default(v)` gives `v` instead ``, and no fix, since whether a value that does not fit
aborts or defaults is the author's. **Two traps the first draft walked into, each found by running
it**: (1) worded *since the value may not fit*, it was false of `u8` to `u64`, a widening where every
value fits, and a false message is `blocking` by verification.md's list; (2) it stacked on panel
209's width-class note (`full/inferred-cell-width-class-names-its-birth` went red, 40 / 1) and
offered a repair at the write where that sitting ruled the repair is the declaration. The gated
version rewords the note and makes `check/width_class.hero`'s `annotated` replace the notes rather
than append (two lines changed, `.push(` … `)` to `[` … `]`). **Cost: `check/mismatch.hero` 44 → 56 (+12), `width_class.hero`
76 → 76; one `full/` case added; no existing golden moves**; `full` 42 / 0, `check` 704 / 0,
`fixes` 1004 / 0, `permissive` 22 / 0, `annotations` 1197 / 0, 1740 tests passed (`heroes-e4`,
`sources 2cd2fb7a6cf485a6`).

**What it can move, in 050: nothing measured.** The 13 strict turn-1 refusals holding a
`type_mismatch` come from 2 tasks (`6f236494`, `c7a30e5b`); each strict row's turn-2 text, found by
matching key, `input` and `output` against `turns/*.json` (one match each, a Python scan, read-only):
**12 of 13 wrote `to_u64(...).must()` at turn 2 with no note telling them to**, and the 13th
(`c7a30e5b` Sonnet s1) went green with no `to_u64` the pattern finds. 11 went green; the other 2
(`6f236494` Haiku s2 and Sonnet s3) wrote the conversion and **ran wrong**, so their failure is the
program's logic, which no message about the count reaches. The note is a §4.17
repair of a message, priced at 12 lines; it is not evidence for the spec's `(\`n\` a \`u64\`)`.

## 7. Route D: `repeat` taking an `i64` (read and counted, not built)

Where it lands: the checker's two `repeat` sites (`check/builtins.hero:74-75`, the count's
expectation, and `:262-268`, the type rule); the runtime, `hero_str_repeat(HeroStr s, uint64_t n)`
in `runtime/heroes_runtime.h:373` and `runtime/parts/str.c:251`, whose parameter would become
`int64_t` with a negative check of its own, because **as it stands a negative `i64` reaching it
converts to a huge `u64`**: with `s` empty it returns `""` in silence (`n == 0 || s.len == 0` is
asked first), and otherwise it panics `string length overflow`, naming the multiply and not the
count (both by reading `parts/str.c:251-263`, not run). The emitter names the function only (`emit/builtins.hero:151-152`). **And every call already
written the ratified way breaks**: `git ls-files '*.hero' | xargs grep -h 'repeat(' | grep -c
to_u64` per tree reads **36 lines in `selfhost/`, 7 in `examples/`, 9 in `tests/`**, 52 of the 162
lines holding `repeat(`, each a `u64` handed where an `i64` would be asked; the 36 in the compiler
mean a seed regenerated in the same change. `tests/golden/check/repeat-count-is-unsigned.hero`
inverts.

Against panel 054 (`docs/panel/054-a-cost-with-no-exit.md`, the case's header quotes it): the
sitting had *return `""`*, *abort at run time* and *refuse it in the type* in front of it and took
the third by design.md §1.12, *a defensive check must surface a defect, not hide it*; Go's signed
`strings.Repeat` and oh-my-posh #4135 are its cited crash. Route D turns a refusal at the value
into an abort inside the primitive or a silent `""`, the two answers that sitting refused, to save
a conversion 12 of 13 strict samples wrote unprompted at turn 2 (§ 6). **No compiler measurement
favours it.** It adds no core construct, so it is not my veto's; my objection rests on §1.12 and on
the 52 lines.

## 8. The label a built-in accepts: a defect, not repaired; what I would file

Confirmed (`build/227probe/shapes/s14_label_xs.hero`, `s15_len_foo.hero`, `l1_repeat_join.hero`):
`slice(xs: line, from: 0, to: 5)`, `len(foo: line)`, `repeat(s: "ab", n: 3)` and `join(xs: ["a"],
sep: ",")` exit 0, where `f(b: 1)` on `function f(a: i64)` draws `wrong_label` with `fix (certain):
write \`a:\``. **Two causes, both in `check/labels.hero`'s `check_label`**: `:44-45` returns for every
built-in with no `Labelled` entry (39 of 41 names), and `:51-52` returns at a position the entry
names `""` (`slice`'s receiver). The specification says nothing of a label naming no parameter
(`grep -n -i label spec/heroes-spec.md` → line 263 alone, the mandatory rule); the compiler has
ruled on it for the other two kinds of callee, a declared function (`wrong_label`) and a function
value (panel 128's `label_on_function_value`, whose builder says *a label claims a check nobody is
doing*, `label_errors.hero:75`, the builder at `:78`). **No program in the tree writes one**: a grep of the tracked
`.hero` files for a counted built-in's name followed by `(<word>: ` finds 7 lines, every one a
function declaration (`function to_text(source: str)`) or a comment, so a repair refuses nothing
that exists.

**I would file it `adjacent`**: the program is accepted and means what it says, since `slice`'s two
labelled positions are still asked and every other built-in's types differ position to position, so
no accepted program computes another value; the defect is a false claim the source keeps, a check a
reader believes was made. It would be `blocking` the day a shape is found where the silent label
changes what an accepted program computes; I searched for one and found none. **Its consequence is
the reason it matters here**: the silent label at `slice`'s position 0 is what reads `slice(from:
0, to: 5)` one position off (2175's second message) and what reads `slice(from: 0, to: 5, line)`'s
value as `to` (§ 2). Draft item: `**A label on a built-in's argument is read only where the built-in
names that position** | \`len(foo: line)\`, \`repeat(s: "ab", n: 3)\` and \`slice(xs: line, from: 0,
to: 5)\` check at exit 0, where a declared function refuses \`f(b: 1)\` \`wrong_label\` and a function
value refuses \`label_on_function_value\` | \`selfhost/check/labels.hero\` \`check_label\`, lines 44-45
and 51-52 · **class: adjacent**`. The repair's shape, for whoever takes it: a label at a position
the built-in does not name is refused as `label_on_function_value` refuses it (a `certain` removal,
the value staying where it is), and at `slice`'s first position a label naming a later position is
the value missing, which route E already tells when the count is short.

**Two more filings found beside this sitting's item** (§ 2, § 3), neither repaired:

- `needs_label`'s certain fix writes a label the call already holds: `cut(from: 0, to: 5, line)`
  on a declared function, and `slice(from: 0, to: 5, line)`, take `fix (certain): write \`to: \``,
  and `--apply` writes `to:` twice (`label_errors.hero:111-118`, `:150-155`; `wrong_label` asks
  `doubled` at `:50`, `needs_label` never does). Defect 135's own rule (*certain only where the
  position is the one reading*) and `.claude/rules/diagnostics-and-goldens.md` line 23 make it a
  `guess`. **I would file it `blocking`**, by the list's *a `certain` fix that writes a program
  meaning something else* read with line 23 (*a fix that leaves the defect standing is a `guess`*):
  the written program is still refused, so it means nothing else, and the class turns on that
  reading, which is the coordinator's.
- A declared function given the wrong count hides a mistake inside an argument:
  `cut(from: "a" + 1, to: 5)` draws `wrong_arity` alone on every compiler here, and the `+` is told
  only once the count is right (`walk.hero:1558-1565` returns before walking the arguments).
  **`adjacent`**, by its own words in the list. Route E's prototype walks a built-in's arguments
  for exactly this reason; the same three lines in `user_call` would repair it.

## 9. Verdicts

**Core or sugar, every route: neither.** No route adds a construct to design.md Part 5's seven
(`docs/design.md:3032`); every compiler change here is a diagnostic in the checker, read before the
IR exists, and touches no lexer, parser, descriptor, ownership pass, lowering, emitter or runtime
line (route D excepted, § 7). So **this seat's veto does not engage on any route**: it holds for a
core construct or a breach of the ceiling, and the one ceiling a route touched, `check/walk.hero`'s
`DECIDED` row, is paid in variant E2 without moving it.

### Route A, the proposal: **object** (the message half; no objection from this seat to the sentence)

- `section`: design.md §1.7 (`docs/design.md:422`), *does it remove a special case from the
  compiler*, and §4.17 (`:2267`).
- `implementation_cost`: the sentence, 0 compiler lines; `spec` owes 4 pins in the same commit
  (`spec/budget` 7846 → 7856, `spec/spendable`, `spec/real`, `spec/ledger`; measured), `special`,
  `grammar`, `fixes`, `unseen` green. The message: **+58** `code_lines`, `check/walk.hero` 1869 →
  1874 (**4 over its `DECIDED` 1870**, payable as E2 pays it) and a new `check/builtin_count.hero`
  of 53; 0 goldens moved, 1 added.
- `needed_for_self_hosting`: no.
- `argument`: Route A's trigger is a special case keyed to one built-in's label table. Built and run,
  it tells `slice(from: 0)` as `wrong_arity` and `line.slice(from: 0)`, the same short count with
  the value written, as `builtin_shape`: one mistake, two codes, chosen by whether a receiver was
  written. It leaves `slice()`, `slice(line, from: 0)`, `slice(0, 5)` and 26 other built-ins as
  today. For 17 lines more, route E reaches all of them with the rule a declared function already
  follows, so A adds the special case that E removes. Measured in 050, the misleading `wrong_label`
  cost nothing: 8 of 9 green at turn 2 in strict and in control alike.
- `prediction`: route A's message lands at **+58 ± 10** `code_lines`, and either raises
  `check/walk.hero`'s `DECIDED` row to 1874 or moves the `@`-marker rule out as E2 does, with
  **0 existing goldens moved** and `line.slice(from: 0)` still printing `builtin_shape`. Checkable by
  `layout` and `check --brief` at panel 227's landing commit in M-thesis-harness.
- `condition`: a measurement showing two codes for one count cost nothing, e.g. a trial reading
  where `builtin_shape` on a receiver-present short `slice` is repaired at turn 2 as often as
  `wrong_arity` on a receiverless one; or a reason, from the rules, that the 26 other built-ins
  should keep `builtin_shape`.

### Route B: **object**

- `section`: design.md does not cover this objection directly. It rests on panel 120's *the spec
  buys prose and not signatures* (`.claude/rules/spec-shape.md` § What was refused) and on two
  measurements here.
- `implementation_cost`: 0 lines in `selfhost/`; **`tests/harness/spec_text.hero` must change**,
  measured: `` `len(xs)` `` in the sentence turns `spec/offered` red (*the compiler has no such
  built-in: len(xs)*). Tokens unpriced (the spec-warden's).
- `needed_for_self_hosting`: no.
- `argument`: B writes every built-in's signature into the spec, a second home the compiler cannot
  check. The compiler holds no signature table, by design (`check/builtins.hero:8-14`), and the one
  instrument that reads the sentence, `spec/offered`, reads names only and skips any fragment
  holding a space (`spec_text.hero:164`). 2174 is exactly that drift: `slice(from:, to:)` against
  `inventory.hero:245`'s `["", "from", "to"]`, and no suite saw it. B multiplies that unwatched
  surface from 3 spellings to every built-in.
- `prediction`: a route-B sentence lands with `spec/offered` reading no parameter list; the first
  signature edited in `selfhost/check/builtins.hero` afterwards leaves the sentence's copy wrong
  with every suite green. Checkable by `grep` of the sentence against the arms at M-thesis-harness's
  close.
- `condition`: an instrument that holds each signature the sentence writes to the checker's rule for
  that built-in, built before the sentence lands.

### Route C, the message alone: **object** as framed (A's message); **approve** with E's rule as the message

- `section`: §1.7 and §4.17, as for A.
- `implementation_cost`: A's +58 or E's +75; 0 spec tokens.
- `needed_for_self_hosting`: no.
- `argument`: Leaving the specification untouched is the only route whose turn-1 effect is zero by
  construction. That is neutral from this seat: 050 gives the message nothing to move at turn 2
  (8 of 9 in both arms). The message C carries should be E's rule, for A's reason.
- `prediction`, `condition`: as for E.

### Route D: **object** (no veto: it adds no core construct)

- `section`: design.md §1.12 (`docs/design.md:587`), as panel 054 applied it.
- `implementation_cost`: two checker sites (`check/builtins.hero:74-75`, `:262-268`); the runtime's
  `hero_str_repeat` parameter `uint64_t` → `int64_t` and a negative check (`heroes_runtime.h:373`,
  `parts/str.c:251`); **52 existing lines holding `repeat(` and `to_u64` become type errors, 36 of
  them in `selfhost/`**, so the seed is regenerated in the change;
  `check/repeat-count-is-unsigned` inverts.
- `needed_for_self_hosting`: no.
- `argument`: Today a negative count is refused at the value, or aborts at the subtraction that made
  it. D moves it inside the primitive. As the runtime reads (not run), a negative `i64` there becomes a
  huge `u64`, so `repeat("", n)` returns `""` silently and `repeat("a", n)` panics *string length
  overflow*, the two answers panel 054 refused. And 12 of 13 strict refusals wrote the conversion
  unprompted at turn 2.
- `prediction`: if D were built, `repeat("ab", 0 - 1)` with an `i64` count would check at exit 0, as
  `repeat-count-is-unsigned.hero` shows `0 - 1` does today, and its run would no longer abort at the
  subtraction. Checkable by `run` on that case at any milestone that built D.
- `condition`: a measured class of correct programs the `u64` count refuses that no conversion
  reaches.

### Route E, built as E2: **approve**

- `section`: design.md §1.7 (`:422`), *remove a special case from the compiler*: a wrong count is
  told one way whether the callee is a declared function, a library built-in or a Tier-1 one.
  §4.17 (`:2267`). And `check/builtins.hero:152-154`'s own rule, *the CALLER reports the arity, so
  that "how many arguments" is answered in one place*.
- `implementation_cost`: **+75** `code_lines`, all in the checker. `check/builtin_args.hero` 63 new
  (77 by `wc -l`, 38 lines of code); `inventory.hero` 231 → 250 (`arity_of`, 27 names); `check/walk.hero`
  1869 → **1862**, the `@`-marker rule moved out along the seam it shares, **no `DECIDED` row
  moved**. Lexer, parser, descriptors, ownership, IR, emitter and runtime: 0. Goldens: 4 cases move
  (8 files, defect 458's and 439's); 2 added. Gated: `check` 704 / 0, `full` 41 / 0, `permissive`
  22 / 0, `fixes` 1004 / 0, `annotations` 1196 / 0, 1740 tests passed, `layout` green on the three
  files. **The seed builds it and the fixpoint holds**: on the final tree (E2 plus the § 6
  note), `./heroes build selfhost/main.hero -o build/227fx/s2` from the seed's compiler, `s2` building
  `s3`, and the two `--emit-c` outputs `cmp`-identical, 26,759,615 bytes each (`build/227fx/log.txt`). The § 6 note, separable: +12 in `check/mismatch.hero`, 1 case, gated in E4.
- `needed_for_self_hosting`: no. The compiler compiles itself with `builtin_shape` today. E repairs
  an `adjacent` defect's message and adds no form, so Principle 0's ticket is not what admits it.
- `argument`: The strongest case against E is that it writes each built-in's count a second time:
  `arity_of` stands beside every arm's `given.len() == n`. A stale row would refuse a correct call,
  which is the `blocking` class. Two witnesses hold the two together today. `heroes-e`, built with
  the rule, compiled the whole compiler without one `wrong_arity`, and the compiler holds 18,482
  calls of these built-ins (`grep -oE` of their call shapes over `selfhost/`'s non-comment lines;
  reserved names, so every hit is a built-in's). And a test holds every name in `NAMES` to a count or to the 14 that have none. E
  changes which message a refused program gets, never whether it is refused: every arm asks the
  exact count.
- `prediction`: (1) **line count**: E lands at +75 ± 10 `code_lines` across those three modules,
  with `check/walk.hero` at or under 1870 and no `DECIDED` row raised. Checkable by `layout` narrowed
  to them at panel 227's landing commit in M-thesis-harness. (2) **harness metric**: at the next run
  of the trial's protocol in M-thesis-harness, among samples whose turn 1 holds a `slice` short of
  its value, the strict and control arms' green-at-turn-2 counts differ by at most one sample, as in
  050 (8 of 9 each). The message moves strict minus control on this shape by 0.
- `condition`: a built-in whose type rule accepts two counts (then `arity_of` must hold a set, or the
  arms must lose their own counts so the table is the one home, the follow-up I would ask for);
  panel 187's R2 differential at the landing round naming a mutant told worse under E; or a census
  of `check` over the tracked files finding a program E refuses that today's compiler accepts. By
  reading the arms that is impossible, and that reading is an inference until the census runs.

## 10. What the worktree holds for the coordinator

`.claude/worktrees/227-compiler-engineer`, detached at `63c78c54b`, nothing committed. Its tree is
**route E2 with the § 6 note** (`git status --porcelain`: `selfhost/check/walk.hero`, `selfhost/inventory.hero`,
`selfhost/check/mismatch.hero` and `selfhost/check/width_class.hero` modified, `selfhost/check/builtin_args.hero` new, the four moved
`check/` cases, three new cases), and `spec/heroes-spec.md` is the trunk's, byte for byte (route A's
sentence was run and taken out). Compilers at its root: `heroes` (the seed), `heroes-next` (stage 2,
the base, `sources 79fe404274caacd0`), `heroes-a` (route A), `heroes-e` (route E), `heroes-e2` (E
paid), `heroes-e3` (the note's first draft, superseded), `heroes-e4` (E2 plus the note, the tree as
it stands, `sources 2cd2fb7a6cf485a6`). Every variant's files are in `build/227variants/{a,e,e2,e4}/`
and the pristine ones in `build/227orig/`; probes in `build/227probe/`; every suite's whole output
in `build/227out/`; the fixpoint in `build/227fx/`.
