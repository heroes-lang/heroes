# Panel 238, the compiler-engineer's report

Seat: compiler-engineer (design.md §1.1, §1.7, Part 5; veto on a core construct or the ceiling).
Worktree `.claude/worktrees/238-compiler-engineer/`, detached at `27829fbba` (the briefs' commit,
ancestor `bd4670da4`). Written as I go, 2026-11-08 from 17:00; `uptime` read load averages 43.59
35.35 26.18 at the start, so every cost below is a count or *instructions retired*, never seconds.
Evidence under `build/238/` in this worktree.

The compilers: `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes` (`/usr/bin/time -l`:
exit 0, `build/238/stage1.log`); then, in its own command, `./heroes build selfhost/main.hero -o
heroes-next` (result in item 0 below). `heroes-next` unmodified is kept as `build/238/heroes-base`.
Four more compilers, each named where it is used: `heroes-proto` (the rule alone), `heroes-walk`
(the rule and the route), `heroes-ref` and `heroes-ref3` (stages 2 and 3 of the tree reformatted),
`heroes-fix` (the rule repaired, item 3f).

## 0. The compilers

- Stage 2, unmodified: `./heroes build selfhost/main.hero -o heroes-next` under `/usr/bin/time -l`:
  exit 0, `6751.70 real 179.34 user 32.24 sys`, **725,108,581,706 instructions retired**
  (`build/238/stage2.log`; the ratio says the run waited, the machine at load 43 to 59);
  `--version` second line `sources 62dba14251e6eafb`, the briefs' own. Kept as
  `build/238/heroes-base`.
- The prototype, built by stage 1 over the warm cache: `./heroes build selfhost/main.hero -o
  build/238/heroes-proto`, exit 0, `88.94 real 72.23 user`, **570,591,339,901 instructions
  retired**, `sources 6346874401eb7915` (`build/238/proto-build.log`).
- The rule and the route, `build/238/heroes-walk`: **571,798,067,345**, `sources 7d731d2212cf5abf`
  (item 2c). The tree reformatted, stage 2 `heroes-ref` **717,580,304,668** and stage 3
  `heroes-ref3` **595,963,755,682**, both `sources 3810029c6e6810d1` (item 3d). The repair,
  `heroes-fix`: **569,790,160,111**, `sources 5db2de271ee28872` (item 3f).

## 1. Question A, the rule, prototyped

**The rule as built** (`selfhost/print/use_order.hero`, new; `git diff` in
`build/238/prototype.diff`):

1. Every `use` line of the file joins **one block**: the bracketed group (`use <name>/...`) first,
   then the program's own, **a blank line between the two groups**.
2. Inside a group the key is **the path with a package's `<` `>` taken out, compared by bytes,
   then the alias**, a tie in source order. Because `/` (0x2F) sorts below every character a part
   may hold (digits 0x30-0x39, letters, `_` 0x5F; F7), byte order on that string **is** the order
   part by part: `<core>/y` before `<core2>/x` (the whole-line byte order puts `<core2>` first,
   `>` being 0x3E, F7), `a/b` before `a0` before `a_b` before `ab`. Case is not folded: `use Zeta`
   before `use alpha` (`LC_ALL=C sort`'s order and gofmt's string comparison). The bound name is
   not the key: it scatters `use check/state` under `s` and `use emit/decls as emit_decls` under `e`
   away from `emit/`, and the reader who scans a block scans it by path.
3. **Comments, design.md §4.1 as the parser reads a doc** (`doc_comments.take_docs`): a run of
   column-0 comments directly above a `use`, no blank line between, stopped by a `##` heading, is
   that line's and moves with it; a comment on the line itself moves with it. A moved run gets a
   blank line above it unless it opens its group, **and never lands glued under another comment**
   (the repair of item 3f: opening the block under a glued header, it would be read as the header).
   Two other readings: **a run that opens the file
   and touches its first `use` is the file's header and stays on top** (123 of the 125 files whose
   first `use` has a comment glued above it are that shape: `build/238/glued-first.txt`,
   `glued-header-files.txt`, from an `awk` over every tracked `.hero` outside `docs/`); **every other comment between the block's first and last line** (a run with a
   blank line under it, a heading, an indented one) **rises above the block**, in its order, a blank
   line under each run (0 files in the tree hold one: `build/238/comment-census.txt`, column 6).
4. **A late `use` joins the block**, its run with it, and leaves a blank line where it stood, so a
   comment above it never becomes the documentation of the declaration below (F8's shape). With
   no `use` before the first declaration the block stands above that declaration's documentation,
   a blank line on each side. Spec line 164, *a `use` binds its name for the whole file*, is why the
   hoist changes no program; the `## Tests` heading of `response_file.hero` loses its two lines,
   which is the one thing a reader loses.
5. **The alias** is printed as written (`fmt.use_line`, unchanged) and read only as the tie-break.

**Why a pass over the text and not the printer's walk.** `format_lexed` prints its items and the
comment stream in source order, the stream moving one way only (`page.comments_before`,
`selfhost/print/page.hero:216-237`), which is exactly F2's *"it has a comment model this one does
not"*. Reordering the items there would print each comment where the next item asked for it. So the
pass rewrites the root file's lines, comments with them, and the printer reads a file already in its
order, in source order as ever: **F2's policy stands for the printer, and the reorder is a separate,
separately judged step**. `print/guard.hero`'s `checked` (the one function `fmt` and the probe call)
runs it first and judges it with `use_order.refused`: the lines written are the lines read, a blank
one aside, each as many times; and `dump(text)` against `dump(order)` with the `use` rows read as a
set, every declaration and its doc as it was (a hoist that made a comment a declaration's doc fails
here, exit 2). The guard's fixpoint check also asks that the pass leave the output unchanged.

**One canonical text: measured, not argued.** `build/238/perm/`: five entries (`<core>/strings`,
`<core2>/x`, `lib/beta as b2` with a glued doc comment, `alpha`, `Zeta`) in all 120 orders, each
in three layouts (a blank under the header and none between the lines; a blank between every two
lines; the header glued to the first line), plus one with `alpha` written after `main`: 361 files,
`heroes-proto fmt` exit 0 on all 361. The first two layouts and the late variant, 241 files, give
**one text** (`md5` `1b9af9d9...`). The glued layout gives two: 96 files one text, and the **24
whose first entry is the commented `lib/beta`**, where `# header` and `# why beta` form one run
glued to the top of the file, read as the header (`p0146.out`). That is the one shape the text
cannot settle (a comment glued under a glued header is the header or the next line's), and the rule
reads it one way, deterministically, losing nothing. The alternative that removes it, `fmt`
writing a blank line under every header glued to a block, adds one line to 123 files, 113 of them
under `tests/golden/`, whose `.expected` files cite lines: priced and not taken. (The same 361
files through `heroes-fix`, the repaired rule: the same three classes, 241, 96 and 24.)

**The routes not taken**, each named: the whole line by bytes (puts `<core2>` before `<core>`;
the two orders differ in 2 of the 833 tracked files with two or more `use` lines, both under
`tests/golden/check/`: a `python3 -I` sort of each file's `use` lines by both keys); case-folded (needs a tie-break; 3 `use` lines in the
tree open a part with an uppercase letter or `_`, all fixtures:
`git grep -h -o '^use [A-Z_][^ ]*\|^use [a-z0-9_/]*/[A-Z_][^ ]*' -- '*.hero' ':!docs'`); the bound
name (above); `<core>` as a third group (CLAUDE.md § 13 refuses a standard library, the critic's
A3); a commented block left in source order (two canonical texts, design.md:2252); the order as a
checker rule (a new diagnostic class refusing programs for their look, Principle 0).

## 2. Question A, the premise, and the route that makes it true

### 2a. The premise, re-run in this worktree with `build/238/heroes-base` (`sources 62dba14251e6eafb`)

The critic's programs copied into `build/238/{probe1,cside,c453,c560}/` (the `.hero`, `.h` files
only) and run here. Every row reproduces:

| finding | what was run | written order | the other order |
|---|---|---|---|
| F4 | `check clash_a.hero` / `clash_b.hero` | 1 error, `module_binding_taken`, `fix (guess): write use lib/beta as lib_beta`, exit 1 | 2 errors (`module_binding_taken` blaming the other line, no fix; `unknown_in_module`), exit 1 |
| F4 | `check twice.hero` | `used_twice`, `unknown_name`, `needs_qualifying`, exit 1; `fmt` exit 0 | (one order) |
| F19 | `check d12.hero` / `d21.hero` | `e1.hero:2:12` first | `e2.hero:2:12` first |
| F19 | `check --json g12.hero` / `g21.hero` | files and holes `h1` before `h2` | `h2` before `h1` |
| F20 | `build --emit-c ab.hero` / `ba.hero` | both exit 0 | **36 lines differ** once the root's own name is normalised and `hero_program_sources` taken out (`diff \| grep -c '^[<>]'`) |
| F20 | `build -o`, `build/*/link-words.rsp` | `"-lm" "-lc"` | `"-lc" "-lm"` |
| F20 | `run` / `build --emit-c` of `c453/written.hero`, `sorted.hero` | `7` `2`; `--emit-c` **exit 0** | `7` `2`; `--emit-c` **exit 1**, `error[ffi_one_file]` |
| F22 | `c560/written.hero`, `sorted.hero`: `run`, `test`, `--emit-c` | `3` `10`; 1 test passed; `--emit-c` exit 1, `ffi_one_file` at `cap_ten.hero:4:14` | `3` `10`; 1 test passed; `--emit-c` exit 1, `'LIMIT' macro redefined` at `cap_ten.hero:3:8` |

So the author's premise holds for **what a built program does** (every `run` and `test` above
prints the same in both orders) and fails for six things the compiler says about it: which line a
clash blames and whether it offers a fix, the order of diagnostics across modules, the order of
holes, the one-file emission's bytes, the link line's order, and **whether `--emit-c` accepts the
program at all** (`c453`). The last is the one that matters: a formatter that reorders by rule
turns `written.hero`, whose `--emit-c` exits 0, into `sorted.hero`, whose `--emit-c` exits 1, by
one `heroes fmt --in-place`. design.md §4.15 (*"any textual difference between two versions is
semantic"*, quoted at `fmt.hero:330-332`) is the formatter's own promise in reverse, and the guard
cannot see it: it compares one file's tree, and the change is in what the other modules' headers
do beside it.

### 2b. Where the written order reaches an output: one walk, one binding loop, one cycle walk

Read in the code, then each confirmed by a build (2c):

- **The module walk**, `selfhost/modules.hero:79-128` (`load_edited`, 445 lines): depth first over
  `paths.uses_of(files[index].text)` in written order, *"Both orders are output, not bookkeeping"*
  (its own comment). The compilation's file order is this walk's, and every output below is read
  off the file order: the `Source`'s offsets, so `diag`'s sort by position (F19's diagnostics) and
  the holes' list; the units and their order; the one-file emission's module order, hence its
  `#include`s, probes and prototypes (F20, F21, F22); the link words, gathered module by module.
- **The binding of names**, `selfhost/resolve/top.hero:114-160` (`read_uses`, 411 lines): the
  `use` lines in written order, the second of two that bind one name blamed (F4).
- **The cycle walk**, `selfhost/modules.hero:277-376` (`cycles`): its edges in written order, so
  which cycle is told, and from where.

That is the whole list I found by reading `selfhost/modules.hero`, `selfhost/module/` (15 modules)
and `selfhost/resolve/top.hero`; the question *is there a fourth* is answered by 2c's build, not by
this reading: with the three ordered, the programs of 2a are run again below.

### 2c. The route, built: one key, read by the walk, the binding loop and the cycle walk

Built in this worktree as `build/238/heroes-walk` (`./heroes build selfhost/main.hero -o
build/238/heroes-walk`, exit 0, `147.66 real 81.32 user`, **571,798,067,345 instructions
retired**, `sources 7d731d2212cf5abf`; `build/238/walk-build.log`):

- **`selfhost/use_key.hero`, new, 126 lines** (its test included): the key in one place,
  `group_of`, `path_key`, `bytes_before`, `before`, `in_order` (the paths as written, for the walk)
  and `ordered_uses` (a compilation's `ast.Use` list, each file's in the one order, for the
  binding loop and the cycle walk). `print/use_order.hero` reads its key from here, so the
  formatter and the compiler cannot disagree on the order.
- **`selfhost/modules.hero`**: `use use_key`, and two lines changed: `named =
  use_key.in_order(paths.uses_of(...))` in `load_edited` (line 83) and `for used in
  use_key.ordered_uses(s, uses)` in `cycles`. The file grows by one line (445 to 446).
- **`selfhost/resolve/top.hero`**: `use use_key`, and `for used in use_key.ordered_uses(s, uses)`
  in `read_uses`. 411 to 412.

Everything else follows the file order the walk makes, which is why three call sites are the whole
route. Each row of 2a again, `heroes-walk`:

| finding | written order | the other order |
|---|---|---|
| F4 clash | `module_binding_taken` at `clash_a.hero:1:5`, then `unknown_in_module` | the same two, at `clash_b.hero:2:5` (the line `use zeta` stands on) |
| F19 diagnostics | `e1.hero:2:12` then `e2.hero:2:12` | the same |
| F19 holes | `h1` then `h2` | the same |
| F20 link words | `"-lm" "-lc"` | `"-lm" "-lc"` |
| F20 emission | | **16 lines differ, every one the root's own name** (`h_ab_main` against `h_ba_main`, `#line N "ab.c"` against `"ba.c"`) |
| c453 `--emit-c` | exit 1 | exit 1 |
| c560 `--emit-c` | exit 1, `'LIMIT' macro redefined` | the same |

So the route **makes the author's premise true for every output measured**: the order a file's
`use` lines are written in reaches no output but that file's own line numbers. Two things it
changes that a reader should be told:

1. **c453's `written.hero` is refused by `--emit-c` in both orders now.** Before, an author could
   make `--emit-c` accept that program by choosing the order; under the route the outcome is the
   modules' names', and `build`, which compiles a unit per module, accepts both (`7` `2`, 2a). The
   escape was never written down as a feature: design.md §4.1 (lines 936-944) says `--emit-c`
   refuses where the one file cannot mean what the units mean, and the route keeps that true.
2. **The clash pair now always takes `clash_b`'s outcome**, two errors and no fix, because
   `lib/beta as zeta` sorts before `zeta` and the line that sorts second, here the plain one, is
   blamed. The better repair exists in
   one order only today; an order-free one is to blame the aliased line of the two wherever it
   stands, since its alias is what the author can change (a `module_binding_taken` change in
   `resolve/top.hero`, a few lines, **unbuilt**): an `adjacent` item whichever route lands, since
   the formatter alone moves every formatted file to the same outcome.

### 2d. What each choice moves: the compiler's own C (`build/238/emit/`)

Four emissions of `selfhost/main.hero --emit-c`, each `diff` taken with the line
`hero_program_sources` out of both files, every one exit 0:

| compiler | tree | against | diff lines (`grep -c '^[<>]'`) | of which `#line` |
|---|---|---|---|---|
| `heroes-proto` (formatter rule, written-order walk) | as written | (E1, 31,161,486 bytes, 904,504,662,669 instructions retired) | | |
| `heroes-proto` | **`main.hero`'s block alone sorted** by hand | E1 | **155,956** | 45,064 |
| `heroes-proto` | **every file of `selfhost/` formatted** | E1 | **403,086** | 94,226 |
| `heroes-walk` (the route) | as written | E1 | **403,084** | |
| `heroes-walk` | as written | `heroes-proto`, formatted | **226**, every one a line of `cli/response_file.hero`, whose code moves two lines down when its two late lines join the block | |

Read together: **without the route**, sorting one file's block moves the compiler's C by 155,956
lines, and the reformat by 403,086, because the written order of every `use` line is the walk's
order. **With the route**, the C moves once, by 403,084 lines, when the route lands, and the
reformat after it moves 226 lines, all of them line numbers of the one file whose lines shift.
Either way the compiler's C moves by about 403,000 lines once; the difference is that without
the route **every later `fmt --in-place` of an unsorted file moves it again** (and moves any
multi-module program's emission, link line and `--emit-c` verdict, 2a), and with it none does.

The measurement of units rebuilt when `main.hero`'s block alone is sorted (the brief's *"the build
cache's keys under `build/`"*) read 1,165 new `tu-*` folders, and is **discarded**: the `emission`
suite was building golden programs into the same `build/` at the same time (204 of the new units
are a `library.c`, many are `fixedbugs*` cases). Not re-run.

### 2e. The route's whole cost, and whether the rule may land without it

Everything the route moves, measured in items 2d, 3b, 3c and 3e (one place to read it):

| what | moved by the route | by the rule alone, once the tree is formatted |
|---|---|---|
| the compiler's own one-file C | 403,084 lines, once | 403,086 at the reformat, and again at every later `fmt` of an unsorted file |
| blessed emissions | 5 (`interpreter`, `ledger`, `shapes`, 2030, 2286); the reformat after it, line numbers in 5 (`adventure`, `ledger`, `markdown`, `query`, `todo`) | the same 5 by module order and the same 5 by line numbers at once: 9 files either way, `ledger` in both |
| `check` goldens | 0 (751 passed) | 0 |
| `unsupported` goldens | 3 (555 and 2136 report another pair, 2143 another order) | 0 while they stay unformatted, since `canonical` exempts them |
| `surface` fixtures and their marks | 2 (`onefile453type` now accepted, correctly; `onefile453macro` refused with clang's plainer message), plus `mark235`, which is the rule's | 1, `mark235` (a form asserts `fmt`'s output on it) |
| `run` cases that stop guarding | 1 (560's default case), unless renamed | the same 1 once formatted, unless renamed or exempt |
| which line a clash blames | the line whose key sorts second, whatever the order written (2c: in the measured pair, the plain one) | the formatted file's order, which is the same key |

**May the rule land without the route?** It may, in the sense that nothing breaks at the commit:
every count above is the rule's too once the tree is formatted. **It should not**, because
without the route the formatter keeps a power the author's premise denies it: any later `heroes fmt
--in-place` of a file whose block is out of order changes the module walk, so it can move a
program's emission and link line, reorder its first diagnostics, and turn `--emit-c` from exit 0 to
exit 1 (c453), and neither the guard, the probe nor any suite of the formatter sees it, since they
compare one file's text and tree. With the route the order an author writes reaches no output but
that file's own line numbers, by construction, for 92 lines. **The robust choice is the route, in
the same push as the rule.** What the conservative choice would have been, for the author: the rule
alone, the six outputs of 2a recorded as moving with `fmt`, and `--emit-c`'s refusal documented as
order-dependent.

## 3. Question B, the cost, with the prototype

### 3a. The census, the formatter's own answer (replacing F5)

`build/238/census/one.sh` over every path of `git ls-files '*.hero' | grep -v '^docs/'` (**3,564**
files, `build/238/tracked-hero.txt`), `xargs -P 6`: each file through `heroes-base fmt` and
`heroes-proto fmt`, compared with the file by `cmp`, and each file the prototype changes formatted
a second time (`build/238/census/rows.txt`, one row per file):

- **Exit codes: identical under both compilers on every file**: 3,076 exit 0, 488 exit 1 (files
  the parser refuses, every one under `tests/golden/`: `check` 404, `surface-fixtures` 53, `full` 15,
  `permissive` 13, `control` 3). **No exit 2 anywhere**: the guard, the pass's own
  judge included, refused nothing.
- **Idempotence: all 827 files the prototype changes are a fixpoint** on the second pass.
- 189 files are not canonical today either (all in the exempt or fixture trees: `check` 46,
  `full` 3, `surface-fixtures` 131, `unsupported` 9); on every one of them the prototype writes
  byte for byte what the baseline writes, so **the rule moves 638 files, and only those**:

| directory | files the rule moves | of which a line below the block moves |
|---|---|---|
| `selfhost` | 586 | 1 (`cli/response_file.hero`, the hoist out of `## Tests`, 376 to 375 lines) |
| `tests/harness` | 25 | 0 |
| `examples` (programs) | 10 | 8 (the blank line between the groups: `adventure/main`, `adventure/game`, `ledger/main`, `ledger/report/lines`, `markdown/blocks`, `query/fmt/report`, `query/data/table`, `todo/main`, one line each) |
| `examples/index`, `examples/*/heroes.packages` | 0 | 0 |
| `tests/golden/run` | 5 | 0 |
| `tests/golden/surface-fixtures` | 6 | 1 (`mark235/main.hero`, the group blank line) |
| `tests/golden/unsupported` | 4 | 0 |
| `tests/golden/check` | 2 | 0 |
| every other golden directory, `tests/thesis` | 0 | 0 |

"A line below the block moves" is measured by a `python3 -I` comparison of each file with its
output from its first line that is neither blank, a comment, nor a `use`: same index and same
text to the end of the file, or not. So **628 of the 638 keep every declaration on its line**,
which is what a `.expected` naming a line, a `#line` directive and a `file:line` in a run's output
see; the 10 that do not are the 9 files holding both groups outside the exempt `check/` (one blank
line added) and the one hoist. (`selfhost/modules.hero` and `resolve/top.hero` read the same at `HEAD`; the census ran
before item 2's route touched them.)

**Which `canonical` walks** (`tests/harness/suite_canonical.hero:69-79`, `SOURCE_DIRS`): of the
638, the 626 under `selfhost`, `tests/harness`, `examples` and `tests/golden/run`, which a landing
must reformat or the suite goes red. The other 12 (`surface-fixtures` 6, `unsupported` 4, `check`
2) are in trees `canonical` does not walk (`check` and `unsupported` exempt by name, lines 22-45;
`surface-fixtures` never named), whose text is the fixture: left as written, except where a form
asserts `fmt`'s output on one (`mark235`, item 3e).

### 3b. What moves in the blessed emissions (`tests/emission/**`, which holds `tests/golden/emit/**`'s too)

**The `emission` suite cannot answer this at the briefs' commit**: with `heroes-walk` it read
**1,627 passed, 771 failed** (`build/238/suites/walk-emission.txt`), and the single-module
`run-abort-array-index.c`, which no walk can move, failed on `hero_program_sources` alone
(`blessed ... "1cf9869816cd01df"`, `now ... "1650f3fa52c533be"`). The baseline compiler writes the
same `1650f3fa52c533be`: the key holds the runtime's bytes (`selfhost/emit/provenance.hero:9-15`),
the runtime moved at `82f9d0a5a` (batch 25, defect 2350, 14:50) after the last blessing
(`a789691f1`, batch 24, 14:04), so every blessed key is stale until batch 25's close blesses it.
Not this sitting's finding, and nothing a seat should bless. So the measurement is by hand
(`build/238/esweep/one.sh`): each of the 780 blessed files, its program emitted by `heroes-base`
and by `heroes-walk` (tree as written), each compared with the blessed bytes and with each other,
`hero_program_sources` out of all three:

- **The route moves 5 of the 778 programs that emit**: `examples/interpreter` (1,996 diff lines),
  `examples/ledger` (532), `examples/shapes` (38), `run/fixedbugs-2030-...` (76), `run/fixedbugs-2286-...`
  (20), exactly the programs whose blocks are unsorted. The other 773 are byte for byte the same
  under both walks. Two (`fixedbugs-437`, `fixedbugs-589`, Linux-only) exit 1 under both.
- The baseline itself differs from **68** blessed files beyond `hero_program_sources` (65 `run`, 1
  `emit`, 1 `examples`, 1 `fixedbugs`): batch 25's pending blessing again, not the rule's.
- **The reformat on top of the route** (`heroes-ref`, item 3d, over the reformatted programs):
  `adventure` 208, `ledger` 366, `markdown` 164, `query` 374, `todo` 98 diff lines against the
  route's own emission before the reformat, and **every one a number** (`sed -E 's/[0-9]+/N/g'` on
  both sides leaves 0 differing lines): the line numbers of the files whose lines shift. `interpreter`,
  `shapes`, 2030 and 2286: 0.

So a landing re-blesses **nine** files (`interpreter`, `ledger`, `shapes`, `adventure`, `markdown`,
`query`, `todo`, 2030, 2286), with the route or without it: without it the reformat moves the
same programs by their module order and their line numbers at once (F21's `shapes`).

### 3c. The three goldens whose subject is the order (F22), against a compiler from before the repairs

The parents of the repairs (`1062f34f1^` = `b4cb695dc` for defect 560, `1eed23d94^` = `9559f9940`
for 563) carry a seed older than their runtime (`clang` on `b4cb695dc`'s seed: `static assertion
failed due to requirement '30 == 26': heroes_runtime.h is from another compiler`), and a stage 2
built there costs about as much as this worktree's (item 0). So the compiler before both repairs
is the seed of the last commit that wrote one before them: `git log -1 --format=%h b4cb695dc --
seed/heroes.c` and the same for `9559f9940` both give **`55d20a362`** (2026-09-28 22:25), an
ancestor of both repairs (`git merge-base --is-ancestor`), built in a scratch worktree
`build/238/oldseed/` (`clang ...`, exit 0, `heroes 0.2.0`; the worktree removed with `git worktree
remove` when done) and run with `HEROES_RUNTIME` set to its own runtime. It is twelve days older than the repairs, so it shows the defect's class, not the
day-before compiler. Each case copied with its modules and headers to `build/238/oldcases/`, its
sorted form written by `heroes-proto fmt`:

| case | old compiler, written order | old compiler, sorted | today's, both orders |
|---|---|---|---|
| 560, *a default another module defines is not read under test* (`heroes test`) | **fails**: `assert failed ... left: 50 right: 10` | **passes**: `1 test, all passed` | `1 test, all passed` |
| 560, *two modules binding twice two ways* (`heroes test`) | refused, `ffi_return_type` | refused, `ffi_parameter_type` | `3 tests, all passed` |
| 563, *a module's own headers come before another module's record* (`heroes run`) | refused, `ffi_field_type` (`Sk.a`) | refused, the same | `7` |

So **one of the three stops guarding when sorted**: the 560 default case, whose defect is the
first module's header defining `LIMIT` for the second; sorted, `cap_ten` is read first and the
defect cannot show. The other two still catch their defect in either order.

**The route**: exempting the case as `check/` is exempt keeps it guarding only while the compiler
follows the written order; **under item 2's route the written order never reaches the compiler**,
so an exempt case written unsorted would be walked sorted and stop guarding all the same. The one
route that survives both is **renaming its modules so the pinned order is the sorted one**:
`fixedbugs560/cap_ten.hero` to `then_cap_ten.hero` and the case's body with it
(`build/238/oldcases/renamed-560-fmt.hero`). Measured: the renamed case is canonical as written
(`heroes-proto fmt` leaves it byte for byte, `cmp`), the old compiler **fails** it (`assert failed:
then_cap_ten.capped(v: 50) == 10 left: 50 right: 10`, `1 test, 1 failed`), today's passes it. The
recovery corpus holds its own frozen copy and its own `fixedbugs560/`
(`tests/golden/recovery/corpus/tests/golden/run/`), so the rename touches the live case alone. The
two others need nothing; the 563 case's comment (*"`wide`'s first where this program names it
first"*) becomes false of the sorted text and wants one sentence.

### 3d. The tree reformatted, rebuilt in two stages

In this worktree, by the prototype with the route (`heroes-walk`, then `heroes-ref`):
`selfhost/` and `tests/harness/` (`xargs -P 6 -n 1 ... fmt --in-place` over their 899 tracked and
new `.hero` files: **611 changed**, the census's 586 and 25), then `examples/` (placed trees left
out) and `tests/golden/run/` (845 files: **15 changed**, the census's 10 and 5), defect 560's
default case taking the rename of 3c (`git mv fixedbugs560/cap_ten.hero then_cap_ten.hero`, the
body's `cap_ten` made `then_cap_ten`), so `fmt` left its order as written.

- **Stage 2**, `./heroes build selfhost/main.hero -o build/238/heroes-ref` (stage 1 from the old
  seed, which walks in written order, building the route's sources): exit 0, **717,580,304,668
  instructions retired**, `sources 3810029c6e6810d1` (`build/238/ref-build.log`).
- **Stage 3**, `build/238/heroes-ref build selfhost/main.hero -o build/238/heroes-ref3`: exit 0,
  **595,963,755,682 instructions retired**, `sources 3810029c6e6810d1`.
- **The fixpoint**: `heroes-ref` and `heroes-ref3` each `build selfhost/main.hero --emit-c`, exit 0
  both, **`cmp` silent**, 31,161,539 bytes each (`build/238/emit/stage2.c`, `stage3.c`).
- **The compiler's C beyond `hero_program_sources`**, from item 2d: under the route the reformat
  moves 226 lines, every one a line number of `cli/response_file.hero`
  (`build/238/emit/walk-unf-vs-fmt.diff`: 86 lines naming `cli/response_file.hero`, 38 naming
  `selfhost/cli/response_file.hero`, the rest the `#line`s beside them); without the route it
  moves 403,086.
- **The seed**: not regenerated (the rhythm of `.claude/rules/verification.md` § The batch: the
  two-stage build did not go red). A release's refresh would carry the route's 403,084 lines once.

### 3e. The forms over the reformatted tree, `heroes-ref` (`./heroes-next run tests/harness/main.hero -- build/238/heroes-ref <form>`, output in `build/238/suites/`)

The hook refused the harness run I asked of `heroes-walk` once the tree's sources had moved (*"says it
was compiled from sources 7d731d2212cf5abf, and the tree this run judges ... keys
28f578b8719183ed"*), as it should; so the forms judge the reformatted tree with its own compiler,
which carries the rule and the route together.

- **`canonical`: 2 passed, 0 failed.**
- **`fixes`: 1,061 passed, 0 failed.**
- **`check`: 751 passed, 0 failed**: the route moves no `check` golden.
- **`run`, narrowed to the four reformatted defects' cases**: `fixedbugs-560` 4 passed, `fixedbugs-563`
  6, `fixedbugs-2030` 1, `fixedbugs-2286` 13, **0 failed** (560's renamed default case among them).
- **The compiler's own tests** (`heroes-ref test selfhost/main.hero`): **1,910 tests, 1 failed**,
  1,168,181,453,785 instructions retired. The one failure is my own test of the pass, *the header
  stays above, a run above a later line moves with it*, and it was right: item 3f. **Not re-run
  after the repair** (1.17 trillion instructions); instead `heroes-fix fmt` of the test's own input
  (`build/238/shapes/hdr.hero`) is compared by `cmp` with the test's expected text
  (`hdr.expected`): equal.
- **`annotations`: 1,263 passed, 2 failed**, `onefile453macro` (the mark on `narrow.hero:4`, the
  diagnostic now on `hundred.hero:3`) and `onefile453type` (the mark on `num.hero:4`, no diagnostic,
  `--emit-c` accepting): the two `surface` findings below, seen from their marks.
- **`unsupported`: 371 passed, 3 failed**, all the route's: `fixedbugs-555-one-c-function-two-groups-
  bind-two-ways-is-told` and `fixedbugs-2136-...` report **another pair** (`defines.h` against
  `double.h`, caret at `fixedbugs555/defines.hero:4:8`, where the `.expected` and the `#~` mark say
  `double.h` against `long.h` at the root's line 16 and 13; both read by hand with `heroes-walk`,
  `build/238/suites/heroes-walk-*.err`), since the sorted walk meets `defines`
  before `long_twice`; `fixedbugs-2143-every-unit-clang-refuses-is-told-in-one-build` tells the same
  two diagnostics in the other order (`vals` before `wide`). The first two pin the walk order
  (rename, as 3c); the third's subject, *every unit is told*, holds, and its `.expected` takes the
  new order.
- **`surface`: 437 passed, 3 failed**, each a finding:
  1. *a package's mark is printed back byte for byte* (`surface-fixtures/mark235/main.hero`):
     **the rule's**. The fixture holds both groups with no blank line between and asserts `fmt`
     returns it unchanged; the rule writes the blank. The fixture is rewritten in the canonical
     form, one line, and its probe rows read it again.
  2. *a binding whose types another module's header changes in the one file is refused by
     `--emit-c`* (`onefile453type`): **the route's**, and the one that had to be settled before
     anything else, since under the route `--emit-c` **exits 0** where the fixture says 1.
     Settled by running it: the fixture's `wide.h` defines `ONEFILE_WIDE`, which makes `num.h`'s
     `onefile_num` a `long long` when read first; sorted, `num` is walked first, the one file reads
     `num.h` as `num`'s unit does, and the accepted C, compiled by `clang -I runtime` with the
     runtime and run, prints **`4` `64`, what `heroes run` prints** (`build/238/of453/`). So the
     acceptance is correct, and the fixture pins the written order, as 560's case does (3c).
  3. *a binding another module's macro rewrites in the one file is refused by `--emit-c`*
     (`onefile453macro`): **the route's**. Still refused, exit 1, in both walks, but under the route
     the refusal is clang's own *"expected identifier or '('"* at `hundred.hero:3:8`, where the
     written order gives the precise *"`twice`, which this group binds, is not what this module's
     own unit makes of it"* at `narrow.hero:4:14`. Sound, less exact.
  For 2 and 3 the route is 3c's: rename the modules so the order the fixture pins is the sorted
  one (`wide` and `narrow` before the other), and, for 3, a second fixture in the sorted order if
  the plainer refusal is worth pinning.

**F9, `check --apply`** (`build/238/probe1/quoted.hero`, `use "zeta"` / `use alpha`):
`heroes-ref check quoted.hero --apply` prints `use zeta` / `use alpha`, exit 0, and `heroes-ref fmt`
of that text prints `use alpha` / `use zeta`. So an `--apply` output is not canonical under the
rule. **I recommend the rule accept it**, as the tree already does: `--apply` repairs meaning and
`fmt` repairs form, `.fixed` and `.applied` files live under `tests/golden/check/`, which
`canonical` exempts, and making `--apply` call the pass would put the formatter inside the fix
writer (`cli/check.hero`, `certain.hero`, `consensus.hero`, none of which calls it today, F9) for
the one certain fix known to touch a `use` line (`use_line.hero:273`). What it costs: a file in this repository written by `--apply` and then by a hand is
refused by the hook until `fmt --in-place` runs, the same as any unsorted write.

### 3f. The prototype's lines by module, `heroes mutate`, the probe's reader

By the `layout` suite's own measure (`tests/harness/suite_layout.hero:1030`, `code_lines`: every
non-blank line outside a `test` block, comments included), `HEAD` against now:

| module | `HEAD` | now | delta | ceiling | `wc -l` now |
|---|---|---|---|---|---|
| `selfhost/print/use_order.hero` (new) | 0 | **324** | +324 | 300 | 443 |
| `selfhost/print/guard.hero` | 168 | 196 | +28 | 300 | 295 |
| `selfhost/probe/judge.hero` | 237 | 244 | +7 | 300 | 340 |
| `selfhost/print/fmt.hero` | 1,099 | 1,101 | +2 | 1,175 (`DECIDED`) | 1,828 |
| **the rule** | | | **+361** | | |
| `selfhost/use_key.hero` (new) | 0 | **90** | +90 | 300 | 126 |
| `selfhost/modules.hero` | 312 | 313 | +1 | 320 (`DECIDED`) | 446 |
| `selfhost/resolve/top.hero` | 207 | 208 | +1 | 300 | 412 |
| **the route** | | | **+92** | | |

`use_order.hero` is **24 over the threshold**; a landing moves the pass's judge (`refused`,
`kept`, `is_blank`, `settled`: 44 of the 324 by the same count) into a module of its own or into `guard.hero`
(196 + 44 stays under 300). The `layout` suite's growth and directory-budget rules were not run.

- **`heroes mutate` needs nothing**: it writes its mutants by text edits
  (`selfhost/mutate/applied.hero`) and no module under `selfhost/mutate/` or `cli/mutate*.hero`
  names `print/fmt` or `print/guard` (`git grep -ln`). Whether its kill counts move under the
  route (a clash mutant blamed on the other line is refused either way) is unrun.
- **The probe's reader** (`selfhost/probe/reader.hero`) needs its input changed, not its rule:
  `probe/judge.hero` hands it the text the printer read, the block already in order
  (`Checked.ordered` and `ordered_input`, two new fields), since the reader pairs tokens in order and
  a reordered block would read as every comment after it moved. The reorder itself is judged by
  `use_order.refused` inside `guard.checked`, which the probe calls as `fmt` does.
- **What no judge sees**, found by my own test (item 3e, the compiler's own tests): a judge of
  places cannot see a comment that stays where it was put and is read differently. The pass wrote a
  moved run glued under a glued header, where the next reading takes it for the header; the guard,
  the pass's judge, the fixpoint check and the probe's reader all hold on that text. Repaired (a
  blank line between them) and pinned by the test; the class is the reason the rule's tests must
  name readings, not only places.

### 3g. The probe over unsorted seeds

`heroes probe build/238/probeseeds --family single` (a comment at every place one can stand,
each variant through `guard.checked`, the pass and its judge included, then the independent
reader): 14 seeds, the **committed** text (`git show HEAD:`) of `examples/shapes/main.hero`,
`examples/adventure/main.hero`, `examples/ledger/report/lines.hero`,
`selfhost/cli/response_file.hero`, `selfhost/emit.hero`, `selfhost/emit/members.hero` and the
defect 563 case, every one unsorted or split, plus item 7's six shapes and `probe1/late.hero`.
With `heroes-ref`: **5,034 variants generated, 5,034 judged, 5,034 held, 0 refused by the guard, 0
by the reader**, exit 0, 1,946,308,577,865 instructions retired
(`build/238/suites/probe-single.txt`). So on every one of those variants a comment written between,
above or beside a `use` line moved with the rule and the printer, the pass's judge and the reader
agreed. **Again with the repaired compiler** (`build/238/heroes-fix`, `sources 5db2de271ee28872`,
569,790,160,111 instructions to build; item 3f's repair), the seeds plus `build/238/shapes/hdr.hero`,
the shape the repair is for: 15 seeds, **5,055 variants, 5,055 held, 0 refused by either judge**,
1,947,871,832,702 instructions retired (`build/238/suites/probe-single-fix.txt`). The repair changes
**0 of the 1,744** files of the four reformatted roots (each formatted again by `heroes-fix` and
compared by `cmp`) and leaves item 1's permutation classes as they were (241, 96, 24).

The `bracket` family and the tree-wide runs `.claude/rules/verification.md` § The
formatter's probe owes after a push touching `selfhost/print/` were not run here (about 15 CPU
hours by that section's own inference).

## 4. Question B, the landing order

Two facts fix the order. **The hooks refuse a write of a file the tree's compiler calls not
canonical** (F10, `.claude/hooks/fmt_check.py:349`, and a commit of one, `staged.py`), so from the
moment a lane's compiler knows the rule, every unsorted file it writes is refused until formatted.
**The site says the tree is canonical byte for byte** (F16), and a push touching `selfhost/**` or
`examples/**` publishes the site (`.github/workflows/deploy-site.yml`'s `paths:`), so no pushed
commit may hold the rule without the tree formatted by it. Hence **one lane, one push, three
commits**, in this order:

1. **The rule and the route, with their tests.** `selfhost/use_key.hero`; the three call sites of
   item 2c (`modules.hero` twice, `resolve/top.hero` once); `print/use_order.hero` (its judge moved
   out to keep it under 300, item 3); `print/guard.hero`'s `checked`; `print/fmt.hero`'s one blank
   line rule and its policy comment (item 5); `probe/judge.hero`'s reader input; the help line if
   taken; `syntax_cmds.hero`'s self-check gains the pair *a block out of order* / *the same block
   in order* (the walk of `.claude/rules/diagnostics-and-goldens.md` § A new surface form, which
   asks it of a re-printing tool). design.md §4.15's sentence. **What the route moves at this
   commit is repaired in it** (item 2e): the 5 blessed emissions it reorders, re-blessed; the
   `unsupported` cases 555 and 2136 and the `surface` fixtures `onefile453type` and
   `onefile453macro` renamed so the order each pins is the sorted one (their `#~` marks with them),
   2143's `.expected` taking the new order; defect 560's default case renamed (item 3c), since from
   this commit the written order no longer reaches the compiler and the case would stop guarding;
   `mark235` rewritten in the canonical form (the rule's). So `emission`, `unsupported`, `surface`,
   `annotations` and `run` are green at this commit; the compiler's own `--emit-c` moves too
   (item 2: 403,084 diff lines for `selfhost/`), which the seed rhythm leaves alone (the seed is
   refreshed where the two-stage build goes red or a release closes; stage 1 from the old seed
   builds stage 2 from the new sources, and the fixpoint is stage 2 against stage 3, both new).
2. **The tree formatted**, by the compiler of commit 1, in one commit: `selfhost/` (586 files),
   `tests/harness/` (25), the 10 `examples/` programs and the 4 other `tests/golden/run/` cases
   (560's took its rename in commit 1). Nothing in the trees `canonical` does not walk (`check`,
   `unsupported`, `permissive`, `full`, `surface-fixtures`), whose text is the fixture. Under the
   route this commit moves **no** emission but the line numbers of the 5 example programs whose files
   shift (item 3b: `adventure`, `ledger`, `markdown`, `query`, `todo`), blessed here.
3. **The records**: the sitting's file, the decision issue, the journal's line.

**The index's move is its own commit, and may travel in the same push or another** (item 6): it
shares two files with the rule's reformat, `tests/harness/suite_packages.hero` and
`suite_unseen.hero` (both in the census's list, each touched at a different line), and renames the
14 programs' placed folders, and it makes `packages/**` a site path (the site's build reads the index
once `checkIndex()` is called directly, item 6b), so `deploy-site.yml` gains the line in that
commit and the push that carries it publishes the site.

Then **one push**, asked for as every push is, with the count of site commits it carries (all
three touch a site path: `selfhost/**`, `examples/**`, `docs/panel/**`). The formatter's probe by
hand is owed after it (`selfhost/print/` moved), and the census of `check` (the route moves which
line a clash blames, so what the checker says moves) and panel 187's R2 (the diagnostics' order of
multi-module corpus programs moves with the walk).

**A lane opened before the landing**, at its next merge of the trunk: every file both sides touched
meets a conflict in its `use` block wherever the lane added or removed a line there (F23, three
dots from `bd4670da4`, read 2026-11-08 at about 19:10: `b25-round`, `b25-seeddoc` and
`p237-land-cli` each touch 7 of the 638 files the rule moves, `p237-land-spec` 6, `235-base` 13,
`b25-misc` and `p238` none; `git diff --name-only bd4670da4...<branch> | grep -Fx -f
build/238/census/rule-moves.txt`). **The rule for the lane: never resolve a `use` block by hand;
take the lane's side, rebuild the lane's compiler from the merged tree, and run `heroes fmt
--in-place` on the conflicted files**, which writes the one order whatever either side wrote; then
its gate as ever. A lane that does not merge before its next write is told by the hook, on the
first unsorted file it writes, after it rebuilds its compiler, and never before: its own compiler
does not know the rule until it merges.

## 5. Question C, where the rule is written (drafts; the spec is the spec-warden's)

**design.md §4.15**, a sentence appended to the bullet at line 2252 (*"Canonical formatter,
mandatory, `gofmt`-style"*), which is where *exactly one correct way* already lives:

> **The `use` lines are one block in one order** (panel 238, the author's instruction of
> 2026-11-08): the bracketed group first, then the program's own, a blank line between the two,
> each group by its path part by part, each part by its bytes, the alias breaking a tie. A `use`
> written after a declaration joins the block, since a `use` binds its name for the whole file. A
> comment glued above a `use` moves with it (§4.1's reading of a doc), the file's header stays on
> top, and any other comment in the block rises above it. **The compiler follows the same order**
> (`selfhost/use_key.hero`): it walks the modules, binds the names and reads the cycles by that key
> and never by the order written, so a formatted file and its source are one program by
> construction and the reorder is the one rewrite `fmt` makes that no output can see. Until that
> sitting the formatter kept source order (*"reordering is where a formatter starts deleting the
> author's remarks"*); the reorder is a pass over the text (`print/use_order.hero`), judged by its
> own guard before the printer, which still prints in source order.

**`selfhost/print/fmt.hero:109-114`**, the policy comment above `format_file`, replaced by:

> Canonical Heroes source for a parsed file. Callers must check that the file had no
> diagnostics first: formatting a tree built out of recovery guesses would rewrite the author's
> file into the parser's guess. The PRINTER keeps source order: reordering items is where a
> formatter starts deleting the author's remarks, since the comment stream here moves one way.
> The one reorder `fmt` makes, the `use` block's (panel 238), happens before this, over the text,
> in `print/use_order.hero`, which `print/guard.hero`'s `checked` calls and judges first.

**The help line**, `selfhost/cli/table.hero:182`: `summary: "print the canonical form"` stays
true and says nothing of order, which is right for a one-line summary; if the sitting wants the
order findable from the command line, `"print the canonical form, the use lines grouped and
sorted"` (one string; the `cli-surface` rules' catalogue would move with it, unrun).

**The site** (`git grep -n 'heroes fmt' -- site/src`, five hits in the English and Italian
editions): `start.html:110` / `it/start.html:111` already say *"fixed rules for indentation, line
breaks and order"* and *"already follow those rules ... byte for byte"*, and
`docs/a-first-program.html:50` (and `it/`) *"Files already in that format remain unchanged, byte
for byte"*. **No page's words move**; what binds is item 4's landing order, which keeps *byte for
byte* true at every pushed commit. `docs/bindings-and-mutation.html:14` is a comment about trailing
comments, untouched.

## 6. Question D, the index at a root `packages/`

### 6a. `fetch` and `add` against the moved index (a scratch copy, `build/238/idx/`)

`examples/index` copied to `build/238/idx/packages`, and `examples/adventure` and `examples/todo`
(their tracked files, the placed trees left out) to `build/238/idx/examples/`; each catalogue's
line rewritten `index = "file://../../packages"`; `heroes-base` (`sources 62dba14251e6eafb`):

- `heroes fetch` in each: `placed core 0.1.0 heroes.packages/core@0.1.0-2e3e1bba9c8acea1`, exit 0,
  both. **F14's rule accepts it**: `../../packages` from `examples/<program>/` is inside the
  repository git answers (the scratch copy stands inside this worktree). The folder is F13's
  predicted key, `2e3e1bba9c8acea1`, where the tree's is `db7f072b4678b0b1`.
- `heroes run main.hero < /dev/null`: `adventure` prints byte for byte what the tree's
  `examples/adventure` prints (`diff` of the two runs empty), exit 0; `todo` prints its usage at
  exit 2 in both places.
- `heroes add core --index file://../../packages` in a fresh `build/238/idx/examples/scratch/`
  (`use <core>/strings`, `print(strings.has_prefix(s: "heroes", prefix: "her"))`): `added core
  0.1.0`, `wrote heroes.toml` (the `index` line as given, the same `sum` as the tree's
  catalogues), the folder `core@0.1.0-2e3e1bba9c8acea1`, exit 0; `heroes run` prints `true`, exit 0.

**What F13 renames**: every placed folder of the 14 programs, 84 files under 14 folders, from
`core@0.1.0-db7f072b4678b0b1` to `core@0.1.0-2e3e1bba9c8acea1`, by `heroes fetch` in each (the
old folder stays: measured on a full copy of `examples/adventure`, `build/238/idx/examples/adventure2/`:
with the line rewritten and nothing fetched, `run` exits 1, `error[package_unread]` naming
`heroes.packages/core@0.1.0-2e3e1bba9c8acea1/` and `heroes fetch` (the key doing its job); after
`fetch`, both folders stand and `run` exits 0, so the move's commit `git rm`s the 14 old folders by
hand). The churn is 84 files renamed, byte for byte the same contents.

### 6b. The move itself, in a scratch worktree at the briefs' commit

Item 3 had reformatted this worktree, so the move was measured apart from it: `git worktree add
--detach build/238/idxmove 27829fbba`, whose own compiler is the baseline (`sources
62dba14251e6eafb`, the hook accepting it), and `build/238/move-index.sh` there: `git mv
examples/index packages`; the 14 catalogues' line to `index = "file://../../packages"`;
`.gitattributes:46` to `packages/** -text`; the literal path in the five harness modules F15 names
and `site/src/lib/examples-model.ts:48`; `heroes fetch` in each of the 14 programs and the old
placed folder `git rm`ed. `git status --short`: **92 renames** (the index's 8 files and the 84
placed files, which git reads as renames since their bytes do not change), **21 files modified**.

**The map's command over the moved tree** (`.claude/rules/verification.md` § The map, run verbatim,
`build/238/map-before.txt` against `map-after.txt`): two rows move, `packages` gains `"packages"`
and `package_rules` reads `"packages"` for `"examples/index"`. **No other suite names the new
folder**, so the walks that reached the index through `"examples"` stop reaching it in silence:
`canonical` (`SOURCE_DIRS`), `probe` (its `examples, multi` row: 4 seeds and 36 variants fewer,
F15), `corpus`, `compat` (which archives `examples` alone from a release tag), and `cards`'
`AREAS`, which has no area for a root `packages/`. A landing adds `"packages"` to each that should
keep reading it; the command cannot say which should, only that none does.

**And one literal the map cannot see, because it is an address and not a path**:
`tests/harness/suite_packages.hero:91`, `INDEX_LINE = "file://../index"`. With the move and nothing
else, **`packages`: 12 passed, 1 failed**, `packages/placed`, one line per program, *reads the
index `file://../../packages`, and a program here reads the local one, `file://../index`*. `git
grep -n 'file://\.\./index'` finds it, `package_rules.hero:22, 600-605` (a comment and a test's
own catalogue text, which stand), and `examples/README.md:120`, `examples/packages/README.md:43`.

**The site** (`npm ci`, exit 0; `npm run build` in the moved tree's `site/`): with
`examples-model.ts`'s `INDEX` changed and nothing else, **exit 0, and falsely green**: the index's
shape check `checkIndex()` is called only from the loop over `examples/` when a folder equals
`INDEX` (`examples-model.ts:406-409`, its one call), so with the index at the root it never runs.
Calling it directly, the build **exits 1**: `heroes:triggers`, *"1 path(s) this build read that no
push publishing the site would touch: 'packages'"*. With `- 'packages/**'` added to
`.github/workflows/deploy-site.yml`'s `paths:` as well, **exit 0**, *"triggers: 1514 paths read,
every one under the 17 that publish the site"*. So the move owes the site two lines beyond
`INDEX`, and makes the index a site path: a push touching `packages/` publishes the site.

**The suites in the moved tree** (`./heroes run tests/harness/main.hero -- ./heroes <suite>` from
`build/238/idxmove/`, output in `build/238/idxsuites/`):

| suite | result | read |
|---|---|---|
| `fetch` | 14 passed, 0 failed | |
| `canonical` | 2 passed, 0 failed | green, and blind to the index: `SOURCE_DIRS` names no `packages` |
| `unseen` | 3 passed, 0 failed | reads every file git names, the moved ones included |
| `corpus` | 71 passed, 0 failed | |
| `packages` | 12 passed, 1 failed; **13 passed, 0 failed** after `INDEX_LINE` (line 91) reads `file://../../packages` | the address literal above |
| `records` | **27 passed, 1 failed**, `records/citations`, 15 lines | below |
| `probe` | takes no case filter (`harness: probe walks no cases`, exit 2); asked directly instead: `heroes probe examples --family multi` 177 seeds, **1,593 variants, all held**, and `heroes probe packages --family multi` 4 seeds, **36, all held**, exit 0 both | the `examples, multi` row loses the index's 4 seeds and 36 variants (the critic's 181 and 1,629 before), still above its floor of 159 and 1,431 |
| `compat` | **not run** | it archives `examples` from the newest release tag (`suite_compat.hero:339-358`), which the move does not change; a release that carries the move archives programs whose placed trees build without the index (R8: `build` never fetches) |
| the net's own tests | **not run** | owed by the five harness modules the move edits |

**`records/citations` is a finding on the name.** Its 15 lines: the two living files that still
say `examples/index/` (`docs/roadmap/milestones/M-core-packages.md:402`,
`tests/harness/suite_packages.hero:8`), which a landing rewrites, and **13 of the `packages` suite's
own check labels**, `packages/cards`, `packages/case`, `packages/codes`, `packages/copies`,
`packages/docs`, `packages/index`, `packages/internal`, `packages/names`, `packages/placed`,
`packages/quiet`, `packages/ran`, `packages/reach`, `packages/tests`, written in that suite's
comments the way every suite names its checks (`<suite>/<check>`), **which the citation check now
reads as paths under a tracked root `packages/` that do not exist**. Before the move `packages`
named no tracked path and the labels were text. So the name `packages/` collides with the
harness's naming of the `packages` suite's checks: every living sentence that names one of them
needs a date, a rewording, or the check's own exemption, for as long as both names stand. A name
that no suite bears (the historian's question; `index/` at the root is one, unrun) would not
collide; `vendor/` is taken (F18).

### 6c. The routes of `00-shared.md` § D, priced

- **D1, a placed key without the index address.** `selfhost/module/placed.hero:74-80` hashes
  `"index\n" + index + ...` because *"every field that names the package, so a field mistyped names
  a tree that is not there"*; panel 235's R7 (its synthesis, lines 635-650) asks that a placed tree
  be checked against its fingerprint at every build, and W4 as restated (line 304) lists `sum`,
  `version`, `tag`, `commit`, the tree edited and a copy left: **not the `index` line**. Measured
  in 6a: with the line rewritten and nothing fetched, `run` exits 1 at `package_unread`; that
  refusal is all the index in the key buys, and the `sum` alone fixes the bytes a build reads. What
  D1 loses: a respelled or moved index no longer forces a fetch, so a mistyped `index` line goes
  unseen until the next `fetch` or `add` reads it. What it buys: this move, and every later one,
  renames no folder. The price is one line of `placed.hero` and the renaming of every placed folder
  once (the 101 tracked files under `heroes.packages/`, F12), the same churn as this move, plus a
  restatement of R7's reason, which is panel 235's ruling and so **a sitting's, not this one's**.
  `tests/golden/run/package-one-index-written-two-ways.case` (whose folders are keyed by
  `file://../idx` as written, F13's critic note) would be renamed with the rest. I do not
  recommend it inside this sitting; it is sound, and I would put it to the author as the question
  *should the next move of the index rename nothing*.
- **The other reading, a catalogue with no `index` line defaulting to a project-root
  `packages/`.** It changes `catalogue.hero`'s default (today the Heroes index, R5) and makes
  `file://` reach a path no catalogue wrote: a surface change of `fetch`, a panel's, and the
  default would differ between a program in this repository and one outside it. It renames the
  same 14 folders (the address text changes from the written `file://../index` to whatever the
  default spells). Not priced in code here; I would not take it, because the default is what a
  stranger's program gets, and a stranger has no `packages/` at their root.
- **The third reading (the critic's D3), `heroes.packages/` renamed `packages/`.** R7's reason for
  the dotted name, *placed where no `use` can spell them*, survives by the version folder's `@`
  (F14), but it renames the folder of every program and every golden that holds one, and the
  author said *index*: the least likely reading, priced at its file count (101 tracked files in 23
  folders: `git ls-files | grep '/heroes.packages/'`, the folder part `sort -u`) and not built.
- **The name.** Against `vendor/`: `git ls-files vendor` lists the tokenisers; a root `packages/`
  beside a root `vendor/` is two folders of other people's code with two meanings, the historian's
  question (precedent). **Against the harness, a measured cost** (6b): the `packages` suite's check
  labels, `packages/<check>`, become citations of paths under the root folder that the `records`
  suite refuses (13 today), and every later living sentence naming one inherits the collision. A
  name no suite bears avoids it; which one is the author's, with the historian's precedent.

## 7. The shapes beside it, depth one (`build/238/shapes/`, `heroes-proto fmt`, each output formatted again and compared by `cmp`, the runnable ones run by `heroes-base` before and after)

| shape | what `fmt` writes | exit | fixpoint | run before / after |
|---|---|---|---|---|
| a file of only `use` lines (`use zeta` / `use alpha`) | `use alpha` / `use zeta` | 0 | yes | (no `main`) |
| trailing comments (`use zeta  # the last letter` / `use alpha  # the first`) | each comment kept on its own line, the lines swapped | 0 | yes | `3` / `3` |
| an alias that would sort elsewhere (`use zeta as a_first` / `use alpha as z_last`) | `use alpha as z_last` / `use zeta as a_first`: the path decides, the alias only breaks a tie | 0 | yes | `3` / `3` |
| two groups in the wrong order, a comment glued above the second (`use zeta`, blank, `# the package group, written second` / `use <core>/strings`) | the comment and `use <core>/strings` first, a blank line, `use zeta` | 0 | yes | (needs a catalogue; not run) |
| a late `use` between a comment and a declaration, all glued (F8) | the comment travels with the line to the top; `helper` gets no doc | 0 | yes | `3` / `3` |
| a late `use` under a remark that has a blank line under it, a doc glued below it | the remark stays where it was, the doc stays `helper`'s, the blank left behind keeps them apart | 0 | yes | `3` / `3` |
| a package's own module | the tree's index and its 14 placed copies hold one `use` each, unbracketed (`use strings/internal/scan`, `use strconv/internal/shown`; `git grep -h '^use ' -- 'examples/index/*' 'examples/*/heroes.packages/*' \| sort \| uniq -c`: 15 each); the census of item 3 changed none of them | 0 | n/a | |

Two readings a reader should know, both from the rows above. The comment glued above a line that
sorts first ends at the top of the file, glued to the block, where the next `fmt` reads it as the
file's header: the same place, read differently, and nothing moves on the second pass. And a
`use` written under a `## ` heading leaves the section (`selfhost/cli/response_file.hero`'s
`## Tests`): the heading stays, the line joins the block.

**No defect of another cause was found** in these shapes or the census (no exit 2 over 3,564
files, item 3). The clash pair's lost fix (2c, item 2) is the one message-quality finding,
`adjacent`.

## Verdict

- `verdict`: **object** (no veto: the rule adds no core construct, and the ceiling holds).
- `section`: design.md §4.15, the canonical-formatter bullet (line 2252): *"exactly one correct
  way to write any program"* and *"it makes any textual difference between two versions
  semantic"*; §1.1 and §1.7 for the cost. The objection is the second clause read backwards: a
  formatter whose rewrite changes what the compiler says breaks the promise the formatter exists
  for, and the document does not say so in those words; it is my reading of §4.15, stated as one.
- `implementation_cost`: **the rule, +361 lines** by the `layout` suite's measure: `selfhost/print/
  use_order.hero` (new, 324, over the 300 threshold by 24, so its 44-line judge moves out at
  landing), `print/guard.hero` +28, `probe/judge.hero` +7, `print/fmt.hero` +2. **The route,
  +92**: `selfhost/use_key.hero` (new, 90), `selfhost/modules.hero` +1 (3 lines changed),
  `selfhost/resolve/top.hero` +1 (1 changed). Frontend and tools only: no checker, lowering or
  backend construct, so it is not core (Part 5's question is not reached). **The tree**: 627 files
  reformatted (586 `selfhost`, 25 `tests/harness`, 10 `examples`, 5 `tests/golden/run`, 1 module
  renamed); 9 blessed emissions re-blessed; the compiler's own C moves 403,084 lines once; 8 fixtures
  move, 7 of them because they pin the written order (560's default case, `onefile453type`, `onefile453macro`,
  `unsupported` 555, 2136, 2143, the 563 case's comment) and `mark235` because it pins `fmt`'s
  output, each renamed, rewritten or re-blessed. Stage 2 of the reformatted tree
  717,580,304,668 instructions; the fixpoint holds (`cmp` silent). **Question D, the index**: no
  compiler line; 92 renames and 21 files modified, plus three lines the instruction did not name
  (`checkIndex()` called directly and `packages/**` in `deploy-site.yml`, without which the site's
  build is falsely green; `INDEX_LINE` in `suite_packages.hero`), the walks that reached the index
  through `examples` (`canonical`, `probe`, `corpus`, `compat`, `cards`) told to name it, and the
  name's collision with 13 `packages/<check>` labels that `records/citations` refuses (6b).
- `needed_for_self_hosting`: **no** (Principle 0: the compiler compiles itself today in either
  order; the rule serves the canonical form, design.md §4.15, not the closure list).
- `argument`: The author's premise is false in this compiler today. The written order of `use`
  lines is the module walk's, so a sorting `fmt` changes the diagnostics' and holes' order, the
  link line, the one-file emission, which line a clash blames, and whether `--emit-c` accepts a
  program (c453: exit 0 written, exit 1 sorted). A formatter that changes what the compiler says
  breaks §4.15's promise in reverse, unseen by its guard. The rule is cheap, not core, and fits one
  person. The robust route makes the premise true by construction: one key (`use_key.hero`, 90
  lines) read by the walk, the binding loop and the cycle walk; measured, both orders then emit one
  program. Land the rule only with it.
- `prediction`: at the landing commit's gate (M-package-manager, once batch 25 has blessed the
  runtime's key), the `emission` suite with the rule and the route reads red on **exactly the nine
  programs of item 3b** (`interpreter`, `ledger`, `shapes`, `adventure`, `markdown`, `query`, `todo`,
  defects 2030 and 2286) before their re-blessing, and on no other. A tenth falsifies my reading that
  the walk, the binding loop and the cycle walk are the whole route.
- `condition`: I approve the rule **without** the route if the author rules that the six outputs of
  item 2a are outside *"the order does not matter"*, accepting that `fmt --in-place` may turn an
  `--emit-c`-accepted program into a refused one and move a multi-module program's emission, link
  line and first diagnostic. I withdraw the route as the recommendation, and ask for its own sitting,
  if a program in the tree or in a release's corpus is found whose `build` or `run` output, not its
  `--emit-c` verdict or its messages' order, changes under the route.
