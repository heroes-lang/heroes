# Panel 253, compiler-engineer

Seat folder `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/rebel-scratch/253-compiler-engineer/`
(called `ce/` below), a `git archive` of `181555e9a32d0b18878f9f10e48d2eadd939c285`, 20,037 files by
`find -type f -not -path "$F/tmp/*" | wc -l` (the brief's count). Started 15:55 CET (`date`), load 5.90
(`uptime`).

**Cuts taken** (the brief's order): **(a)** Q5's (W) built on this Mac only, its Linux arm64 rows left to
the ffi-pragmatist's 5.1, and (P) priced from the emitted C's call counts and panel 230's carried
measurement, not built (so its `--emit-c | wc -c` with and without is owed); **(b)** Q6's index
publication and `fetch`'s placement by the list, priced, not built, so `fetch`'s instructions under W and
under the list are owed; **(c)** Q2's widening to `heroes check`, not counted. Nothing else cut: every
question has its block. § Cuts at the end says why each.

**Premises of the briefs found false**: § Premises, at the end, each with its command.

**The blocks' order departs from the brief's once, said here**: Q4; then Q6's refusal and `path` question
before Q2, since Q2's run over the 149 faces twice held the machine for about half an hour and Q6's cases
were checks that could be read beside it; Q2 whole; Q1; the rest of Q6; Q5 last. Each block was written
whole before the next began.

## Build, and the base everything is measured against

`clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`, then `./heroes build selfhost/main.hero -o
heroes-next`, exit 0; **`./heroes-next --version` prints `sources 23c7d2a626b29fd5`**, the frozen key
(`ce/tmp/stage2.log`). Kept as `ce/ce-base`. Every prototype below is built by `ce-base` from my copy's
`selfhost/`, one build at a time. Line counts of a change are `diff` of my file against the writer's
pristine `bw/tree/` (the same commit, read-only), `+added -removed`.

## Question 4: a skipped outcome for `heroes test`

**Built** (`ce/ce-q4`, `sources 07403ec0083e5dce`; an earlier build with the name `skip`, `ce/ce-skip`,
`sources 6965810333a1586a`, kept to count what the name refuses).

**The channel, chosen and measured.** No exit status can carry a skip: `exit(code:)` takes every one
of 0 to 255 (spec § 11, `selfhost/library_source.hero:269-275`), an abort is 134. So the channel is
**the runner's own file**: `heroes test` empties `<binary>.skip` before each test and passes its path as
the binary's second word; `hero_test_index` keeps it (`runtime/parts/panic.c`); `skip_test(reason:)`
is a `.heroes`-tier built-in written in the library over one `_Noreturn` extern, `hero_test_skip`
(`runtime/parts/os.c`, beside `hero_exit`), which writes `skipped\n<reason>` there, checks every
write, and exits 0; a write that fails, or a skip with no runner to tell (the binary run alone), stops
as a panic does, so **a skip never reads as a pass**. The runner reads a skip only from **exit 0 and
the file's `skipped` line together**: `ok` is exit 0 and an empty file, anything else `FAIL`. Measured
on `ce/q4/t.hero` (`ce/q4/t.q4.out`):

```
ok   "a: passes"
FAIL "b: fails"                                   (assert, 134)
SKIP "c: skips": the network is down
ok   "d: exits 0 having printed a skip line"      (prints `skipped`, exit(code: 0))
FAIL "e: exits 77, automake's skip status"
FAIL "f: aborts"                                  (index out of range, 134)
SKIP "g: writes the runner's file by hand and exits 0": forged
7 tests, 3 failed, 2 skipped                      exit 1
```

No exit, print, abort or assert gives a skip (d, e, f). **One forgery stands, and only deliberately**:
a test that reads `args()[1]` and writes `skipped\n` into it (g), since `hero_args_set` hands a test
binary's whole argv to `args()`. Closing it is one line in `selfhost/emit/c_main.hero`'s `test_shim`
(the test binary's `args()` given the program name alone); not built.

**A run where every test skipped is a failure**, exit 1, `2 tests, every one skipped: none ran its
subject` (`ce/q4/all.q4.out`): the arm says *"fails no run for it"*, and I depart from it on two rules
already in the tree, `verbs.hero`'s own `--only` (*"One selecting none is refused, a run that tested
nothing being no green"*) and the harness's floor (`tests/harness/report.hero:15-21`, defect 246). A run
with a skip beside a pass prints `N tests, k skipped`, exit 0; beside a failure, `N tests, f failed, k
skipped`, exit 1.

**The refusal outside a test** (`ce/q4/outside.hero`): `error[skip_outside_test]: \`skip_test\` ends a
\`test\` block as skipped, and this is not one`, at the call, with a note saying where the skip belongs;
one call-site check in a new module (`selfhost/check/test_only.hero`, reading `Checker.in_test`, which
`check/decls.hero:190-196` already sets). No `Fix`: there is no certain one (a guess would delete the
call), so the note is the repair. **A line after a skip is not refused, and neither is one after
`exit` today** (`ce/q4/after_exit.hero` checks clean with `ce-base`): `skip_test` ends a path exactly as
`exit` does (`check/path_end.hero`, `emit/heroes_call.hero`'s `hero_unreachable()` after the call), no
more.

**The name, counted by the compiler** (`ce-skip check` over every root that holds the word, against
`ce-base`): `skip` taken everywhere refuses **45 sites**: Rebel 26 (one root `use`-ing all 149 faces,
`ce/packages/rebel/0.1.0/files/zz_every_face.hero`; base: only that root's 149 unused `use` lines), plus
**9 calls** of the refused declarations told `unknown_in_module`; `selfhost/` 10 (+1 call); the harness 7
distinct (over `main`, `issues`, `thesis`, `latency`, `generate` and every root with a `main`);
`examples/markdown` 1; `.claude/hooks/discards.hero` 1. Of them **9 are function declarations, not 7**:
the brief's `grep '^ *function skip\('` misses the generic `function skip<T>(` in Rebel's `lists.hero:81`
and `lists/internal/ends.hero:38`, both public in `lists`. **`skip_test` appears in no `.hero` file**
(`grep -lw`), so it refuses nothing. And with `skip` taken, `heroes test` of any Rebel module that reaches
`io` (`websocket`, `http`), `lists`, `protobuf` or `archive` stops at the name before a test runs, which
is why the five tests below were run under `ce-q4`.

**The five of the 106, rewritten** (`ce/packages/rebel/0.1.0/files/acme.hero`, three
`.err _ => print("...: skipped, ...")` arms; `mongodb.hero:1819`, printed, and `:2381`, one of the
silent 51, `.err _ => _ = 0`; each now `.err _ => skip_test(reason: "...")`; originals in `ce/tmp/`), run
with `--only`, no whole package's tests, `ACME_DIRECTORY` and `REBEL_MONGODB_URL` unset:

```
SKIP "connect: the directory of a test server": ACME_DIRECTORY is not set or nothing answers there
1 test, every one skipped: none ran its subject                                     exit 1
SKIP "new_account: an account for a new key": ACME_DIRECTORY is not set or nothing answers there
SKIP "new_account_bound: an external account": ACME_DIRECTORY, ACME_EAB_KID or ACME_EAB_KEY is not set
2 tests, every one skipped: none ran its subject                                     exit 1
```
```
SKIP "live: CRUD, cursors across batches, ... against the server REBEL_MONGODB_URL names": REBEL_MONGODB_URL names no server
SKIP "connect: a client of the server REBEL_MONGODB_URL names": REBEL_MONGODB_URL names no server
2 tests, every one skipped: none ran its subject                                     exit 1
```

The second is one of the 51 that printed nothing and read `ok` (`ce/q4/rebel-mongodb.out`).

**The conservative costs no compiler line**, and what it costs is elsewhere: the 51 silent mongodb tests
made to print (51 edits in one file), a convention in Rebel's README, and a check to enforce it that does
not exist (no harness module reads Rebel's tests, `00-shared.md` § Question 2); and the 106 still read
`ok` to every reader that counts outcomes, the CI's included.

- `verdict`: **object** to the arm as written (`skip(reason:)`); **approve** the outcome spelled
  `skip_test(reason:)`, with an all-skipped run failing.
- `section`: design.md §1.7 and Part 5 (it is not core: no IR construct, no emitter case, a library
  function over one extern, as `exit` is); §1.2's cost formula for the name.
- `implementation_cost`: **selfhost +43 lines (3 of them replacing a line each) and a new module of 39**:
  `cli/verbs.hero` +33 -4 (405 now), `library_source.hero` +5 (307), `check/walk.hero` +2 (2,478),
  `check/test_only.hero` 39 new, `check/path_end.hero` and `emit/heroes_call.hero` one line each,
  `emit/guarded_names.hero` one word; **runtime +34 C lines**: `parts/os.c` +20 (1,160), `parts/panic.c`
  +9 (168), `hero_os.h` +3, the two guard headers +1 each (`ce/q4/verbs.diff`, `ce/q4/runtime.diff`).
  Owed and not written: a `run`-form golden for the three outcomes, a `check` golden for
  `skip_outside_test` (annotated), the net's `spec` table row for the new built-in.
  **And the emissions, found after**: `library_source.hero`'s header says the strings *are* the
  library's source, so a line inserted moves every `#line "<heroes library>"` after it in every program's
  C (*"five lines of prose moved ... all 441 emission traces"*). My extern line went into the existing
  `hero_os.h` group at `:159`, which moves the lines of `args_checked`, `exit` and the rest in every
  emission that reaches them; the landing puts `hero_test_skip` in an `extern "hero_os.h"` group of its
  own **after** `exit`, with `skip_test`, which moves no existing line.
- `needed_for_self_hosting`: no.
- `argument`: A third outcome is cheap and belongs to the runner, not the core: one library function,
  one extern, one call-site check, one reader in `heroes test`. The channel must be the runner's own,
  because every status is a program's to give; measured, no exit, print, abort or assert forges it. The
  name is the cost: `skip` taken everywhere refuses 45 sites the compiler counts, 9 declarations among
  them, and turns a parser's most natural helper name into `builtin_name_taken` in every program written
  after it, a correction round per §1.2 bought for nothing; `skip_test` refuses none. A run that skipped
  everything judged nothing, and the tree's own two rules already call that red.
- `prediction`: at the landing, `grep -c` of `builtin_name_taken` over `ce-base` against the landed
  compiler, every tracked root checked, moves by **0** if `skip_test` lands and by **45** (±2 for growth
  since `181555e9a`) if `skip` lands; and the landed `selfhost/` diff for the outcome stays under **120
  lines** with its tests.
- `condition`: the blind seat's t4 showing `skip_test` read worse than `skip` by its rule (a model
  writing `skip(` against a spec that says `skip_test(`), or a measurement that the 45 renames cost less
  than one refused helper per program; then `skip`.

## Question 6, its spec-facing half: the refusal over a `path` entry, and the `path` question (items 1 and 6)

**Built** (`ce/ce-q26`, `sources 3355136a0f6addbc`, which also holds Q2's verb): an entry's `packages =
["json", "strings"]` line, read by `module/catalogue.hero` through a new one-line array reader in
`module/toml_line.hero` (bracketed, each name `quoted`'s rule, none empty, none twice, written once an
entry; `packages = []` refused with its words); kept **beside** the entries, `Catalogue.listings`, and
handed to the compilation as `reached.Reached.listings`, so `catalogue.Entry`'s 43 constructions
(`grep -c`) stay as they are; and `module/unlisted.hero`, asked by `modules.errors` beside
`private.told`: a `use <entry>/<path>` written in **one of the program's own modules** whose entry has a
listing not naming `<path>` is `package_not_listed`. A package's own files are never judged, since a face
reaches what it needs through its own bare `use` lines (R4).

Measured over a `path` entry (`ce/q6/prog/`, `path = "../../packages/rebel/0.1.0/files"`, `packages =
["json", "strings"]`): `listed.hero` (`use <rebel>/json`, `use <rebel>/strings`) checks at exit 0;
`unlisted.hero` adds `use <rebel>/flags`:

```
error[package_not_listed]: `<rebel>/flags` is not among the packages the entry `rebel` places
  at unlisted.hero:3:5
  note: heroes.toml's `[dependencies.rebel]` places `json`, `strings` and the packages they use;
        `heroes add rebel/flags` adds `flags` to its `packages` line                        exit 1
```

**The arm's *"`certain` fix naming the `heroes add`"* cannot be a `Fix`.** A `Fix` edits a file of the
compilation, and the catalogue is none: panel 235's R5, written at `module/package_told.hero`'s header
(*"told with its line and never applied, the catalogue being no file of the compilation, which a `Fix`
edits"*). The repair is a command, so it is a note; the spec-warden prices the arm's sentence as it
stands, which promises a `certain` fix the compiler's own rule refuses.

**The falsifier, as four `check` runs** (`ce/q6/f-*/`, `listed.hero` under each catalogue): a name
deleted (`["json"]`) refuses `use <rebel>/strings`, exit 1; a name mistyped (`["jsno", "strings"]`)
refuses `use <rebel>/json`, exit 1; the line deleted builds, exit 0 (the entry places whole, W); `[]` is
`catalogue_refused`. In none does another module compile in the named one's place: each edit changes
only whether the program builds, as `.claude/rules/cli-surface.md`'s falsifier asks. One residue: the
mistyped `jsno` is told at the `use` it fails, never as a name no face holds; a reader asking the face
of each listed name would tell it at its line (unbuilt, about 10 lines).

**A finding the arm does not see: 16 of the 161 programs use a module that is not a package.** The
`use <rebel>/` lines of `examples-rebel/*.hero` name **163 distinct paths** (`grep -h -o | sort -u`):
**147 faces and 16 of the platform layer's bindings**, `net/hero_net`, `os/hero_sys`, `crypto/hero_crypto`,
`sql/odbc/hero_odbc` and twelve more, each `use`d directly by one `reach_hero_*` program (`grep -l`: 16
files). By the brief's own grain (`closure.py`: a face is a `.hero` not under `internal/` and **not
`hero_*`**) they are no package, so a `packages` list of faces refuses them: under `q6/er-b/heroes.toml`
(the 147 faces) `reach_hero_net.hero` is `package_not_listed` at `<rebel>/net/hero_net`, exit 1, a
correct `use` refused; under `q6/er-a/` (all 163 paths) it checks at exit 0, as do `reach_strings` and
`jq`. So the rule needs a sentence the arm lacks: either a `hero_*` module is named by its own path in
the list, or it belongs to the face owning its folder (a rule on the module's name, which the spec would
then state).

**The `path` question, both readings built over `examples-rebel/`'s one entry:**
- **(a) `packages` binds a `path` entry** (what the prototype does): `examples-rebel/heroes.toml` must list
  the union of its 161 programs, 163 paths; every program then builds (sampled: `reach_hero_net`,
  `reach_strings`, `jq` at exit 0 under `q6/er-a/`), and **the refusal checks nothing for any one of
  them**, since each program's own lines are a subset of a union written for all; it catches only a path
  no program of the folder used before.
- **(b) a `path` entry is exempt** (the line refused on it, or ignored): equal to (a) with no line, which
  is W, measured above (`f-noline`, exit 0); the refusal then belongs to a fetched entry alone, and every
  build this sitting ran, the brief's and mine, exercised none of it.

**I adopt (a)**: one rule for every entry, so a program moved from a `path` entry to a fetched one is
refused by the same lines it was refused by before; the shared-folder weakness is the folder's, not the
rule's, and a program with its own catalogue (every program outside this repository) gets the whole
check. (b) leaves a rule that no test in this tree can reach until a fetched `rebel` exists.

- `verdict` (the refusal and the `path` question): **approve (a)**, **object** to the arm's `certain`
  fix (R5 refuses it) and to its silence on the 16 `hero_*` modules.
- `section`: design.md §1.7 (not core: a reader, a field and one graph check; nothing below the
  module graph moves) and §4.17 (the message names the command).
- `implementation_cost` (so far, the refusal alone): `module/unlisted.hero` 60 new; `module/toml_line.hero`
  +52 (106 now); `module/catalogue.hero` +44 -3 (528, past its 487 and past §11's ~300 already);
  `module/reached.hero` +10 -1; `modules.hero` +3 -1; four constructions of `Catalogue` and `Reached` one
  word each (`fetch/add.hero`, `module/path_entries.hero`, `module/placed_beside.hero`,
  `cli/unverified.hero`, `cli/check_json.hero`). **About 170 lines in 10 files.** Not yet: `heroes add
  rebel/csv` and `fetch`'s placement (below).
- `needed_for_self_hosting`: no.

## Question 2: `heroes doc`

**Built** (`ce/ce-q26`): `heroes doc <module.hero>` and `heroes doc <name>/<module>`, the seventeenth
`Command(` row. It runs **every stage `heroes check` runs, through `check.report`**, and a module that does
not check prints its diagnostics and no reference; then, for the target module alone, in source order:
the module doc, each declaration with its signature as `print/signature.hero`'s `one_line` prints it (a
record, a variant or a constant as written), its comment (`ast.Decl.doc`, the parser's own run of
comment lines), **the call, labelled exactly where `check/type_fit.hero`'s `shared` asks, over the types
the checker interned for the written parameters** (`Checked.written_types`), and its examples, the
documented `test` blocks under `## Examples` whose title opens with its name. **A package operand is read
as a package module, never as a root**: the verb compiles a one-line root, `use <rebel>/compress/lz4`,
in memory (no file written, `ls`), so the face's bare `use` lines start at its package's root (R4) and the
catalogue is read as a build reads it; a nested face read as a root file fails at its first `use`
(`ce-base check packages/.../compress/lz4.hero`: `unknown_module` `bytes`, measured).

**The three refusals, built** (`ce/q2/wrong.hero` → `ce/q2/wrong.out`, exit 1, no reference printed):

```
error[doc_parameter_order]: the comment of `joined` names `first` after `second`, and the signature takes `first` first
error[doc_unknown_parameter]: the comment of `sum` names a parameter `c`, and it has none of that name
  fix (guess): name `b`, the parameter no line names
error[doc_example_unknown]: the example "concat: two texts" names `concat`, and this module declares no function of that name
```

Each at the comment's own bytes, with a note. **Only one carries a `Fix`, and it is a `guess`**: renaming
a line's name to the parameter no line names may be wrong (the line may describe a parameter the
signature lost). The order's repair is a move of lines, which `diag.Fix`'s one span holds only where the
lines are contiguous and one each; unbuilt, so no `certain` fix is claimed. `ce/q2/good.out` is the
reference of a clean module (its call `good.joined(first: first, second: second, sep: sep)`, three `str`
labelled by the rule, `good.count_words(text)` bare).

**Over Rebel's 149 faces** (`ce/q2/run1/`, `heroes doc '<rebel>/<face>'` from `examples-rebel/`):
**142 print a reference, 7 are refused**, 19 refusals: **18 `doc_unknown_parameter`, every one an `@`
parameter written with its mark** (`- \`@doc\`:` 9, `@c` 4, `@r` 3, `@held`, `@layer`; in `config`,
`csv`, `flags`, `ini`, `markdown`, `tokenizer`), and **1 `doc_example_unknown`**, `jobs/redis_store`'s
`"PREFIX: the keys of the store"`, an example of a constant; **0 `doc_parameter_order`**. The 142
references are 4,717,217 bytes (`cat *.out | wc -c`), the largest `http`'s 148,519.

**Where the compiler's reader and the writer's reader of lines differ**, counted over the 142 references
by `ce/tmp/ref_census.py`, which reads the signatures the printer wrote on one line: 5,053 functions,
4,811 with parameters; **661 with parameters and no parameter line** (the census: 626 over all 149);
**4,149 whose lines are exactly the parameters**, an `@` mark read as the name; **1 whose lines differ**
without being refused, `crypto/seal.from_secret`, which documents `secret` and not `algorithm`; **976
with no example** (the census: 929). The differences, and who is right:
- **an `@` mark**: the census's `` `([a-z0-9_]+)` `` never matches `` `@r` ``, so it counted such a function
  as having no line, or as mismatched; the compiler reads the line and refuses it, since § A7 says
  *"every parameter named by its own name"* and `r` is the name (`ast.Param.name`, the `@` being
  `mutable`). Which is right is a sentence the shape owes: refuse `@r`, or read it as `r`;
- **a signature on several lines**: the census's `param_rx` reads a function's parameters from its own
  line alone, so a broken signature reads as one of no parameters; the compiler reads every one;
- **an example**: the census takes any commented test titled `name:` anywhere in the face, the verb only
  those under `## Examples` (§ A7: *"under a `## Examples` heading at the end"*), so more functions read as
  having none (976 against 929);
- **what neither refuses**: a missing line. § A7 asks a line for *"every parameter"*, and the arm's three
  refusals do not include it; 661 functions plus `from_secret` would be refused by a fourth (unbuilt,
  about 15 lines in `cli/doc_shape.hero`). **The arm's refusals miss Rebel's commonest departure.**

**The same bytes every run**: run twice over the 149 faces (`ce/q2/run1/`, `ce/q2/run2/`), **149 of 149
references `cmp`-identical, 149 of 149 diagnostic streams identical** once `time`'s own lines are cut
(`sed '/^ *[0-9.]* real /,$d'`), every exit the same. One platform: Linux and Windows are owed.

**Its cost, by instructions retired** (`/usr/bin/time -l` on the compiler itself, N = 2 per face, the
first discarded, so the second run's figure; the two agree within 0.11% for `mcp`): **`mcp`
104,176,278,848** (run 1: 104,282,316,190), the face with the largest
closure (187,420 lines, `bw/q6/perpkg.txt`), `openapi` 93.5 G, `web` 72.4 G, `compress/lz4`
9,930,434,011, the smallest `math/libm` 273,857,286. The verb checks the whole closure, so its cost is
`heroes check`'s on a program that `use`s the face.

**What a golden form `doc` owes** (read in `tests/harness/suite_golden.hero`, 1,081 lines, and
`tests/harness/main.hero`'s `forms`): a `Shape` holds one `want_code` and one stream, so **two forms**:
`doc` (`tests/golden/doc/`, stdout, exit 0, each case a module and its `.expected` reference) and
`doc_refused` (stderr, exit 1, each refusal annotated `#~v <code>` on the line above the comment it
names, since a `#~` on the comment line would join the comment's words); about 13 lines per `Shape`,
two lines in `main.hero`'s `forms` list and its names list (`:250-264`), a determinism case (two runs,
`cmp`) in the `determinism` suite, and `canonical` and `annotations` reach the new directories by their
walks. **About 50 lines of harness and a directory of cases**, the fifteenth and sixteenth groups beside
`ls tests/golden | wc -l`'s 14. And the site: `site/src/lib/claims.ts:264` counts `Command(` rows, so
`start.html` and `log.html` in both editions say *seventeen*; the site's build owes it before the push.

**The conservative, no verb** (`tests/harness/package_card.hero` 452 lines, `package_cards.hero` 115,
`module_docs.hero` 212 grown): a second reader of comment lines beside the compiler, the one whose
three mistakes are counted above (an `@` mark, a broken signature, an example's place), and a reference
no program outside this repository can print (CLAUDE.md § 10). Growing it to the verb's output costs
about what the verb costs, without the checker's types for the call: the card's `call_of` compares
parameter types by their written text (`package_card.hero:164-184`), so `x: json.Value` and `y: Value`
in one face would read as two types.

- `verdict`: **approve** the verb as a reader of the frontend; **object** to the arm's three refusals as
  the whole rule (the missing line, 661 functions, is the shape's commonest departure and none refuses it).
- `section`: design.md §1.7 (no core construct: no type, no IR, no emitter case; it reads the parse and the
  checker's types, as `--dump-scopes` does), Part 5; CLAUDE.md § 10 for the verb.
- `implementation_cost`: **3 new modules, 511 lines, 350 of code** (`cli/doc.hero` 170, 118 of code;
  `cli/doc_text.hero` 168, 113; `cli/doc_shape.hero` 173, 119), split so none passes §11's ~300; **+10 -4
  in 5 dispatch files** (`cli/table.hero` +2, 312; `main.hero` +4 -2, 331; `cli/file_operand.hero` +2 -1;
  `cli/unverified.hero` +1 -1; `cli/argv.hero` +1). The golden forms about 50 lines of harness; the site
  four pages.
- `needed_for_self_hosting`: no.
- `argument`: The verb is a printer over what the frontend already holds: the parser keeps every
  declaration's comment run, the printer its signature, the checker the types the label rule compares.
  Nothing enters the core, and the reader is the compiler's, which measured three ways the line reader is
  wrong. It costs a check of the face's closure, up to 104 G instructions for `mcp`. The refusals as posed
  are the wrong three: over Rebel they find 18 `@` marks and one constant's example, and miss 661
  functions with no parameter line, the departure § A7's first rule exists for.
- `prediction`: at the verb's landing, `heroes doc` over Rebel's faces at the landing commit refuses **0
  faces for `doc_parameter_order`** and, if a fourth refusal for a missing line lands, **at least 600
  functions** before Rebel's comments are repaired; and the three modules stay under **400 lines of code**.
- `condition`: a ruling that the shape tolerates a missing parameter line (then the three suffice), or a
  measurement that the verb's check of the closure is too slow to print a reference (`mcp` past a still
  machine's budget), which would move it to read the parse alone.

## Question 1, the compiler's part: the Zen's two homes and `selfhost/library_source.hero`

**Law 18 lives in four places of the tree, not two** (`grep -rn 'man who sold'`): `selfhost/cli/doctor.hero:254`
(`zen()`, what `heroes this` prints) and `:287` (its test, pinned byte for byte), `seed/heroes.c:365`
(`HERO_STR_STATIC(hero_str_669c9582, ...)`, the seed's copy of the same string), and
`site/src/html/project.html:95` with `it/project.html:95`; and `site/src/lib/claims.ts:274-282` counts the
Zen's lines from `doctor.hero`'s `function zen()`, so a law reworded keeps the count (twenty) and moves
nothing there.

**What changing it costs**: two lines of `doctor.hero` (the string and its test, one edit each), a
change that leaves the two-stage build green, since stage 2 is built from `selfhost/` and its fixpoint
compares stage 2's emission with stage 3's, both from the new text. So **the seed is not regenerated for
it** (`.claude/rules/verification.md` § The batch: *"the seed regenerated only where that build went red
or the batch closes a release"*), and until the next refresh `./heroes this` from the clang-built seed
prints the old law while `heroes-next this` prints the new: one string two compilers disagree on, and no
suite asks (`seed`'s question is the C the compiler reads of itself, not its output). The release that
refreshes the seed closes it. Not a cost in lines; a window, named.

**Three comments of the compiler and the harness state the old founding constraint** (`grep -rn
'standard library'` over `selfhost/` and `tests/harness/`): `selfhost/resolve/errors.hero:78`,
`tests/harness/shell.hero:7` (*"Heroes has no standard library, so a process ..."*),
`tests/harness/strings.hero:24` (*"exactly what design.md §1.11 says"*); `selfhost/cli/measure.hero:26` is
the Rust bootstrap's. Decision 10's living texts rewritten clean reach these three comments too; none
changes a byte of emitted C but its module's `#line`s where a line count moves.

**`selfhost/library_source.hero`** (302 lines at the frozen commit, `wc -l` of `bw/tree/`): the Heroes
library embedded in the compiler, one constant `LINES` whose strings are its source, joined by `text()`;
it holds **13 functions** (`all`, `any`, `args`, `args_checked`, `exit`, `filter`, `find`, `fold`, `map`,
`range`, `read_file`, `validated`, `write_file`) and **one `extern "hero_os.h"` group**, no package and
no `use`. **Decision 6 does not touch it, and neither does decision 9**: Rebel is a package a program
names in its catalogue (spec § 1), and this file is the language's built-ins written in Heroes, which
panel 097's condition 5 and panel 234's B5 veto keep free of any package source; decision 6 (*"The
compiler does not use Rebel to write itself"*) is that veto restated for Rebel. The one thing this sitting
adds to the file is Q4's `skip_test`, a built-in, not a package (above).

- `verdict`: **approve** (nothing of the compiler's ceiling moves).
- `section`: design.md §1.11 (the founding constraint being reworded), §1.7.
- `implementation_cost`: `selfhost/cli/doctor.hero` 2 lines changed; `seed/heroes.c` 0 now, its string
  refreshed with the next seed; three comments; `library_source.hero` 0.
- `needed_for_self_hosting`: no.
- `argument`: The compiler holds law 18 in one string and its test; the seed's copy follows at the next
  refresh, the rhythm the project chose, and the window between is two compilers printing two laws,
  which is worth one sentence in the register rather than a seed of its own. The built-in library is the
  language's, not a library a program takes, and both decisions leave it as it is.
- `prediction`: at the commit that rewords law 18, `git diff --stat` touches `selfhost/cli/doctor.hero` by
  exactly 2 lines and `seed/heroes.c` by 0, and the two-stage fixpoint `cmp` stays empty; the seed's
  string moves only at the next `seed/` refresh.
- `condition`: a suite that holds the seed's `heroes this` to stage 2's (none found: `grep -rn 'man who
  sold' tests/`, empty), which would make the window red and the seed's refresh owed in the same commit.

## Question 6, the rest: 250's findings under the list, the cost, M' and E, and what R1 and R2 still bind

**250's two findings, re-measured under the list.**
- *"A correct `use` refused, 17 of 17"*: under M the refusal came from a placement computed earlier, so a
  `use` written after `add` met a tree missing its module (`module_unplaced`, which the frozen tree does
  not hold, `grep -rn`). Under the list the refusal is the list's, told at the `use` with its command, and
  the 161 programs read: **0 refused** under one catalogue listing the 163 paths their lines name
  (sampled three, above), **0 by construction** under a catalogue per program made by `heroes add
  rebel/<face>` per `use`; **16 refused under a list of faces alone**, the `reach_hero_*` programs (above).
  So the finding moves from the placement to the grain: a list of faces must say what a `hero_*` module
  is.
- *"Placement a function of the order the verbs ran"*: under the list, **the placement is a function of the
  list**, whichever verb ran first: `add rebel/csv` before or after the `use` writes the same names. One
  residue in my prototype: `add` appends, so two orders write the same set in two orders of bytes;
  `catalogue_edit.hero`'s `sorted(entries)` already sorts entries, and the landing sorts the list the same
  way (one line).

**The cost** (`implementation_cost` below), and what the programs place, reproduced byte for byte from the
writer's `closure.py` over `bw/tree` (`diff` of my `ce/q6/closure-161.txt` against
`bw/q6/examples-rebel.txt`: the same but the folder's path): **W places 1,639 files, 38,309,694 bytes, for
every program**; the list (the package grain, each listed face with its Rebel closure) places **median 104
files and 1,707,683 bytes (4.5%), at most 513 files and 8,244,160 bytes (21.5%)** over the 161 real
programs. `heroes fetch`'s instructions under each are **cut (b)**: `fetch` placing by the list is not
built (below).

**What I did not build, priced** (cut (b), the brief's order):
- **`fetch` placing the listed faces and their closure**: the closure is the faces' own `use` lines, read
  from the placed version's files before they are placed, which means lexing every listed face's modules
  at `fetch` time (`fetch/fetch_all.hero` 276, `placing.hero` 71, `listing_read.hero` 92 today) and a walk
  of bare `use` lines from the package's root (R4), the same walk `modules.hero` makes at build time:
  **about 150 lines** in `fetch/`, plus each placed file held to the version's listing as today
  (`listing_read.hero`'s per-file sums already exist, `packages/core/0.6.0/list`). The ffi-pragmatist's
  250 veto on computing a package's C closure reaches no file of Rebel's (`find ... ! -name '*.hero' !
  -name '*.ledger'`: nothing, the brief's fact re-run in my copy).
- **Rebel in the index**: `packages/rebel/versions` and `0.1.0/list`, as `packages/core/` has them: one
  `sha256  <path>` line per file (1,639 lines) and the version's `sum` over the list, written by the same
  hashing `module/sums.hero` (99) reads; no compiler line, a release step.

**The routes between, priced, not built.**
- **(M')**, the placement computed from the `use` lines at `add` and `fetch`, no list: it is 250's M at
  the package grain, and 250 measured M at **+294 code lines and +389 lines of compiler in 12 files, a
  module cycle broken by a function value, and a new diagnostic class** (`docs/panel/250-*.md:303-306`);
  the package grain removes no part of that (the walk and the unplaced refusal are the same), and it
  brings back both findings: a `use` written after `fetch` is refused until `fetch` runs again, and the
  catalogue no longer says what is placed, so the falsifier's *"deleting one of its entries ... never
  changes which module compiles"* is asked of the program's `use` lines rather than of the catalogue.
- **(E)**, one index entry per package at one version the index enforces: a program of 40 faces writes
  40 entries, each a version and a 64-digit `sum` (`catalogue.Entry`), against one entry and one line;
  `build_list.hero` (229) gains a rule that every entry of one family shares a version, which the index
  must name (a family field the index format lacks); and **`use <rebel>/csv` would name the entry `rebel`
  that no longer exists**: the grammar's `"<" ident ">"` (spec line 61) names one entry, so E either
  spells `use <csv>/csv` (every `use` line of the 161 programs rewritten, 163 paths) or the grammar
  learns a family, a change to the module graph's naming. Neither is core; both cost more than the list.

**What panel 242's R2 and panel 250's R1 still bind, after decision 6.** R2's ruler is about *the
compiler* taking a package: *"it takes one only where its own tree, every line of every placed file
counted, is smaller after the step than before"* (`docs/design.md:578-579`). Decision 6 says the compiler
never takes Rebel, so R2 never meets Rebel; it stands for any package the compiler would take. R1 (*"W
stays, every version placed whole"*) rested on three grounds (`docs/panel/250-*.md:299-320`): no placement
meets R2 for the compiler (**moot for Rebel**, by decision 6); M's own costs and its two findings (**not
the list's**: the list is not M, and both findings were re-measured above); and the veto on computing a
package's C closure (**reaches no file of Rebel's**, which ships no C). So R1 binds where its grounds hold,
an entry with no `packages` line, which stays W; an entry with one is the case R1 did not see, and the
sitting that adopts the list amends R1 by name rather than leaving two rulings that read as opposite.

- `verdict`: **approve** the list over a `path` entry and a fetched one alike, reading (a); **object** to
  three things in the arm: the `certain` fix R5 refuses, the silence on the 16 `hero_*` modules, and
  *"`heroes fetch` places the listed packages and their Rebel closure"* written as if it were small (it
  is the walk 250 measured, about 150 lines in `fetch/`).
- `section`: design.md §1.7 and Part 5 (no core construct: a catalogue field, a graph check, a verb's
  edit, a placement walk; nothing reaches the checker, the IR or the emitter); §1.11's placement
  paragraph for R1.
- `implementation_cost`: **built**: the refusal, about 170 lines in 10 files (above), and `heroes add
  <entry>/<face>`, `fetch/add_package.hero` 113 new and `fetch/add.hero` +16 -1 (`ce/ce-q6`, `sources
  7d08367c4ad0a3bd`; measured in `ce/q6/addtest/`: `add rebel/flags` writes `packages = ["json", "strings",
  "flags"]` and the refused program then checks at exit 0; again, *already places*, exit 0; `add
  rebel/nosuch` refused, *its tree ... holds no `nosuch.hero`*, at exit 2 where it owes 1, its code not yet
  in `fetch/told.hero`'s `INPUT_CODES`, one line); **priced**: `fetch`'s
  closure placement about 150 lines in `fetch/`; the index's listing, a release step; a `check` golden
  per falsifier edit (four, `ce/q6/f-*`), a `fetch` golden for the placed set.
- `needed_for_self_hosting`: no (decision 6: the compiler never takes Rebel).
- `argument`: One version, one entry and a list is the smallest shape that places a subset: the list is
  data the catalogue already reads lines of, the refusal is one graph check on the program's own `use`
  lines, and the placement stays a function of the catalogue, which keeps the falsifier and the four
  input classes. M' re-buys 250's measured failures and E rewrites every `use` line or the grammar. What
  the arm leaves out is where the cost hides: the `fetch` walk, the 16 modules that are no package, and a
  `certain` fix the compiler's own R5 refuses.
- `prediction`: at the landing, over the 161 programs of `examples-rebel/` each given a catalogue made by
  `heroes add rebel/<path>` for its own `use` lines, `heroes check` refuses **0**; and the same with a list
  of faces alone refuses **exactly the 16 `reach_hero_*` programs**, unless the landing makes a `hero_*`
  module its folder's face's.
- `condition`: a measurement that the `fetch` closure walk costs more than W's 38.3 MB per program is
  worth (a program's cold `fetch` instructions under the list against W, owed), or the author ruling a
  shared catalogue (`examples-rebel/`) the normal case, which makes (a)'s check empty and (b) the honest
  reading.

## Question 5: the whole stack of calls when a program aborts

**Built, on this Mac (cut (a) taken: Linux arm64 is the ffi-pragmatist's 5.1)**: route (W) in the runtime
alone, no compiler line. `hero_stack_say_calls` (`runtime/parts/stack.c`, POSIX branch, beside the
guard's `hero_stack_names`): from `hero_abort`'s own frame up the frame records, inside the current
thread's stack bounds (the guard's, or `pthread_get_stackaddr_np`'s where the guard set none), each return
address inside a Heroes function told as `  called from <module>.<name>, <image>+0x<offset>`, by the
guard's own `hero_stack_symbol` and `hero_stack_say_heroes_name`, `write(2)` from the stack, nothing
allocated, at most 16; the first Heroes frame is the one whose place the report already names and is not
told again; `hero_abort` (`parts/panic.c`), which every stop of the runtime funnels through, calls it once
before `abort()`; the Windows branch is an empty function (owed to the box). Every program built after it
carries it, since the runtime is compiled per build from `runtime/`.

**Measured** (`ce/q5/`, each binary under `timeout -s KILL 20`):

```
deep.hero, -O0:                                 deep.hero, -O2 (and `heroes run`, the same):
panic: array index out of range                 panic: array index out of range
  at deep.hero:3:12, in deep.pick                 at deep.hero:3:12, in deep.pick
  called from deep.middle, deep-O0+0x2101b        called from deep.main, deep-O2+0x18a2f
  called from deep.outer, deep-O0+0x2110f       exit 134
  called from deep.main, deep-O0+0x2117f
exit 134

rebel/main.hero (an assert inside Rebel's strings), -O0 and -O2 alike:
assert failed: replacements.len() == patterns.texts.len()
  at <rebel>/strings.hero:921:12, in strings.replace_patterns_all
  left:  1
  right: 2
  called from main.swap_words, inrebel-O0+0x2115b
  called from main.main, inrebel-O0+0x21297
exit 134
```

`deepmust.hero` adds `called from deepmust.main` at both levels. **The `.hero` lines come from the line
tables every build already carries**, read here by `xcrun atos -o <binary> -l 0x100000000 <addresses>`,
which finds them through the binary's debug map in the build's objects (`dsymutil -s | grep -c N_OSO`: 2):
`deep.hero:6`, `:10`, `:14` for the three calls at `-O0`, and **`main.hero:6` and `main.hero:9` for the
Rebel case at both levels, the program's own call into Rebel that the brief measured named nowhere**.
`xcrun atos` costs 207,660,061 instructions for three addresses.

**An inlined frame at `-O2` is lost, measured**: `middle` and `outer` inlined into `main`, the walk tells
`deep.main` alone, and `atos -i` over the same address gives `deep.hero:14` and no inlined frame, so
`-gline-tables-only` as built gives no inline chain here. Two of `deep`'s three calls are gone at `-O2`;
none of the Rebel case's two.

**Its cost**: **0 instructions on a program's happy path** by construction (it runs inside `hero_abort`
alone); **+756 bytes of `__text`** at both levels (`size -m`: 134,032 against 133,276 at `-O0`, 99,348
against 98,592 at `-O2`, the runtime's two files put back for the second build, `ce/q5/deep-noW-*`);
**the abort itself +510,193 instructions at `-O0`** (19,306,209 against 18,796,016) and **+216,074 at
`-O2`** (18,929,815 against 18,713,741), best of three, the first of four discarded; `runtime/parts/stack.c`
+64 lines (1,379 then 1,443), `parts/panic.c` +2.

**`-fno-omit-frame-pointer` where it is needed**, measured in the amd64 image (bytes only, instructions
under emulation measure nothing): **its clang 22.1.8 says `-mframe-pointer=none` at `-O2`** (`clang -###`,
the brief's *"unrun"* settled for that image, not for the CI's), and with the flag
`examples/interpreter`'s emitted C (901,512 bytes) compiles to **313,768 bytes of text against 336,979
without it, 23,211 smaller**, the runtime to 136,980 against 136,070, **910 larger**. The instructions it
costs a program are owed to a still x86-64 machine. The flag is one word in `selfhost/cli/flags.hero`'s
compile words (`:133-153`), x86-64 alone or every target.

**(A), the lines, priced, not built**: the runtime prints the addresses (built, above); `heroes run` and
`heroes test`, the program's parent, would read its stderr and append each `called from` line's `.hero`
line by asking the platform's symboliser of the binary they built, `atos` on Darwin (through `xcrun`,
which `heroes doctor` already finds), `llvm-symbolizer` or `addr2line` on Linux, where the ELF carries the
line tables itself (by the flags; unrun here), the PDB on Windows (owed). The runner today hands the child
its own stderr (`process.run_given(... err: "")`, `cli/verbs.hero:178-184`), so it would capture and
re-print it: about **60 to 90 lines in `cli/`**, and a binary run alone shows the names and the
addresses, as measured above.

**(P), priced from the emitted C, not built**: the whole chain needs each call to hand the callee a frame
of its own, `{site, caller}` on the caller's stack, beside a site record per call (panel 230's library
route, `emit/stop_place.hero:50-61`, hands only one level). In `examples/interpreter`'s 901,512 bytes of
C there are **3,245 calls to Heroes functions against 791 abort sites** (`grep -o` inside function
bodies), so P writes about four records per abort site where panel 230 measured a record **per abort
site** at **+7.4% of the seed's bytes and 416 KiB rebased at load** (carried, `stop_place.hero:24-26`),
plus two stores and an argument on every call of every happy path. By that arithmetic, an inference, P
costs several times what panel 230 refused for one level; its `--emit-c | wc -c` with and without and
one program's instructions are owed to a P built in the emitter.

**(B), the program's call into a package alone**: `stop_place.in_library` widened to a package's
functions, each handed `hero_caller` as the library's are, and the function-value twin of
`emit/signature.hero` with it, about 30 lines; it prints `main.hero:6` **in place of**
`<rebel>/strings.hero:921`, one line for another, where W with (A) gives both, measured.

**The rules every route keeps** (panel 230's): the first line and the place are written from the fixed
buffer and flushed before the walk starts (`hero_panic_at` writes and flushes, then `hero_abort` walks),
and the stop stays final (`SIGABRT` set back, `abort()`). **Defect 2587** (a panic never says its line
while another thread is blocked writing stdout): the walk starts after the report is written, so it
cannot delay the line; it adds one new way to wait, `dladdr` taking the loader's lock where another
thread holds it, which would leave the program hung after its report rather than stopped. Unmeasured;
a question for the landing, whose answer may be to read symbols without `dladdr` (`parts/symbols.c`
already does on Linux).

**Windows**: (W) needs dbghelp initialised before the fault and a PDB beside the binary
(`stack.c:1231`, `:1300-1301`), owed to the box; (A) needs the PDB the link writes where its line carries
`-g` (`flags.hero:210-213`), owed; (P) and (B) need nothing platform-specific. Knowing the box is not
mine today, I adopt **(W) now on both POSIX platforms, its lines by (A) in the parent**, and Windows
keeping today's one place until the box measures dbghelp, said in the report as today's
*"Windows has no walk"* says it.

- `verdict`: **approve (W) with (A)**; **object** to (P) on the ceiling (a frame and a record per call on
  every happy path, to buy what the frame records already hold for free); (B) is redundant beside W.
- `section`: design.md §1.7 and Part 5 (W and A add no construct and no emitter case; P adds a parameter
  to every function the emitter writes, a change to the calling convention of the whole backend);
  §1.12 (the stop stays final, the report first); §4.17.
- `implementation_cost`: **W built: `runtime/parts/stack.c` +64 (1,443), `parts/panic.c` +2, 0 compiler
  lines, +756 bytes of `__text`, 0 happy-path instructions**; A priced at 60 to 90 lines in `cli/`; the
  x86-64 flag one word in `cli/flags.hero`; P an emitter-wide parameter, unbuilt; B about 30 lines.
- `needed_for_self_hosting`: no.
- `argument`: The frame records are already there on both arm64 targets and cost nothing to keep; the
  guard already reads them and already names Heroes functions. Telling them at an abort is 64 lines of
  runtime, free until the program dies, and the build's own line tables already know every call's line,
  as `atos` showed for the Rebel case the brief found silent. P would pay on every call of every program
  to rebuild what the machine keeps. What W cannot do is see an inlined call at `-O2`, measured: two of
  three gone. That is a known limit to state, not a reason for P.
- `prediction`: at the landing, every abort of the `run` golden form at `-O0` names at least one
  `called from` line wherever its place is not in `main`, and the happy-path instructions of the `run`
  form's programs move by **0.0%** (the walk sits in `hero_abort` alone), measured by `latency`'s
  `compare` on the compiler's own build, which aborts nowhere.
- `condition`: a measurement that `dladdr` at the abort hangs a program another thread holds the loader
  for (then the walk reads symbols without it before it lands), or a still x86-64 machine measuring
  `-fno-omit-frame-pointer` past the latency floor (then the walk on x86-64 waits, as Windows does).

## Premises of the briefs found false

1. **`skip` refuses 7 declarations** (`00-shared.md` § Question 4; `compiler-engineer.md` Q4.2): **9**. The
   brief's `grep -rn -E '^ *function skip\('` misses the generic `function skip<T>(` at Rebel's
   `lists.hero:81` and `lists/internal/ends.hero:38` (`grep -rnE '^ *function skip[<(]'` prints 9). By the
   compiler with the built-in on: **45 sites** refused `builtin_name_taken` and **10 calls** of the refused
   declarations `unknown_in_module` (Q4's block).
2. **The arm's *"`certain` fix naming the `heroes add`"*** (`00-shared.md` § Question 6, the robust arm):
   a `Fix` edits a file of the compilation, and panel 235's R5 says the catalogue is none
   (`selfhost/module/package_told.hero`'s header, read). The refusal's repair is a note naming the command.
3. **The 161 programs' `use <rebel>/` lines name 163 paths, and the `packages` list would *"list every
   face"*** (`00-shared.md` § Question 6): **147 are faces and 16 are the platform layer's `hero_*`
   bindings** (`net/hero_net`, `os/hero_sys`, ...), each `use`d by a `reach_hero_*` program (`comm` of the
   paths against the faces; `grep -l`: 16 files), which the brief's own grain (`closure.py`'s `is_face`)
   says are no package. A list of faces refuses those 16 correct programs (measured, `q6/er-b/`).
4. **`tests/harness/package_card.hero`'s call is *"derived by the checker's label rule"*** (`00-shared.md`
   § Question 2): it applies the rule to the parameter types **as written text** (`call_of`,
   `package_card.hero:164-184`, `q.ty == p.ty` over strings), not the checker's interned types; the proof
   build (`package_judge.hero`) catches what that gets wrong, after the fact. The verb built here reads the
   checker's types (`Checked.written_types`).
5. **The 642 functions whose parameter lines disagree with their signature** (`compiler-engineer.md`
   Q2.2, from `bw/tools/doc_census.py`): the census reads a signature from its own line alone and a
   parameter line only as `` `[a-z0-9_]+` ``, so a broken signature reads as one of no parameters and an
   `` `@r` `` line as no line. Over the 142 faces that print a reference, the compiler's reading gives 661
   with no parameter line, 1 that differs, and 18 `@`-marked lines refused (Q2's block); the two counts are
   of different things and the brief's is not the compiler's.
6. **`builtin_name_taken` lives in `resolve/top.hero` and `resolve/decls.hero`** (`compiler-engineer.md`
   Q4): incomplete rather than false: a local or a parameter is refused in `selfhost/resolve/state.hero:355`
   (`grep -rn builtin_name_taken selfhost`); how many of the 45 sites each of the three tells is uncounted.

Settled where the brief said unrun: the amd64 image's clang (22.1.8) keeps no frame pointer at `-O2`
(`clang -###`: `-mframe-pointer=none`); the Rebel abort's own call **is** in the build's line tables
(`atos`: `main.hero:6`).

## Cuts, and why

- **(a) Q5**: (W) built and run on this Mac at both levels over the brief's three programs; its Linux arm64
  rows are the ffi-pragmatist's 5.1, which builds `deep.hero` in both images already. (P) is an emitter-wide
  parameter whose prototype would cost a compiler build and a reading of every call the emitter writes;
  priced from counts and panel 230's carried figure instead. Owed: P's `--emit-c | wc -c` with and
  without, its happy-path instructions; `-fno-omit-frame-pointer`'s instructions on a still x86-64 machine.
- **(b) Q6**: `fetch` placing the list's closure and Rebel's index listing, priced (about 150 lines in
  `fetch/`, a release step); owed: `fetch`'s cold instructions under W and under the list for the 161
  programs and the five sets.
- **(c) Q2.5**: the refusals widened to `heroes check` over `selfhost/` and `examples/`, not counted. The
  verb is built, so the count is a `check` build with `doc_shape.refusals` asked of every module, one
  build away; I stopped at the verb's own run over Rebel, which showed the three refusals are the wrong
  three before any widening.
- **Not cut, and not done**: the golden forms `doc` and `doc_refused`, the `run` and `check` goldens for
  `skip_test`, and the falsifier's four `check` cases as golden files are priced, not written; they are
  the landing's, and every case they hold was run above as a command with its output in `ce/`.

**Added 2026-11-21.** The folders under `.claude/worktrees/` this file names were never part of
the repository, git ignoring that directory, and were removed that day by the author's
instruction, meant as: *delete every worktree that is not active; where one is cited, find a
solution*. What a seat or the coordinator kept under `rebel-scratch/` for this sitting is read
from `docs/panel/253-reports/evidence.tar.xz`, its paths relative to `rebel-scratch/`; `evidence.md`
beside it says what was kept and what was left out.
