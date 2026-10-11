# Panel 211, compiler-engineer report: `check --apply --in-place` over a tree (defect 607)

Seat: compiler-engineer. Started 02:11 by `date` (2026-10-11). Tree: my copy of
`1dd890751`, `.claude/worktrees/scratch-b15/211-compiler-engineer/tree/`,
checked against the commit at 02:13 (688 files of `selfhost/`, `runtime/`,
`seed/`, `tests/harness/` compared with `git show 1dd890751:<path> | cmp`,
0 differ). Every number below is a command run in that copy this session;
the commands' outputs are in the same directory (`census/`, `*.log`,
`probe/`). The built diff is `route.diff` there.

## The verdict block

- `verdict`: **object** (not veto) on W as the brief words it; **approve** W
  under the four conditions at the end. **Amended 03:20: approve C**
  (consensus over a directory's roots, built at the coordinator's request,
  § Route C below) as the route to adopt; W's conditions stand only if C is
  not taken.
- `section`: design.md §1.1 (the ceiling), §1.7 and Part 5 (core against
  sugar: this is neither, it is tool surface, and adds nothing the checker,
  the lowering or the backend handles); §4.17 for soundness, whose two
  sentences on `not_applied` and `elsewhere` (*"which it never writes"*,
  *"another module's, which `--apply` never writes"*, design.md lines 2235
  and 2246 of my copy) W falsifies.
- `implementation_cost`: route W built, **+307 / -47 lines** over ten files
  (`route.diff`), **211 code lines** in `suite_layout`'s unit under
  `selfhost/` (table below), one new module of 126; no instruction moved on
  either program measured.
- `needed_for_self_hosting`: **no** (Principle 0). `heroes check
  selfhost/main.hero --apply` on the frozen tree writes nothing (it is clean),
  and the census below finds 2 roots in 3,498 where W changes anything by
  the base's first round, 3 by the built rule's every round, all of them
  `surface-fixtures/` cases written to test exactly this note.
- `argument`: W is cheap and inert on every compile path, so the ceiling is
  not my objection. My objection is that a `certain` fix is certain *relative
  to the root it was computed from*, because every `use` starts at the
  directory of the file you compile (spec § 1), and W writes that certainty
  into files the command line never named. Measured: a clean `a.hero` broken
  by `check lib/b.hero --apply --in-place`, which rewrote `lib/m.hero`. The
  class exists today for a named file (measured too), so this is a widening
  rather than a new hole, and its exposure in the tree is one module of 795.
  And the `surface` suite, run under W, rewrote a tracked golden fixture.
- `prediction`: below, § Prediction.
- `condition`: below, § What would change my verdict.

## Base costs, `heroes-base` (`1dd890751`), `/usr/bin/time -l`

Machine load 13 to 20 (`uptime` at 02:14 and 02:32, other seats building), so
`real` is not read; `instructions retired` is the unit.

| run | `heroes-base` | `heroes-w` (route W) |
|---|---|---|
| `heroes check selfhost/main.hero` | 84,719,679,999; 84,669,324,711; 84,884,601,896; 84,842,645,735; 84,856,620,732; 84,832,894,399 | 84,883,283,526; 84,847,502,392; 84,842,794,140 |
| cold `heroes build hello.hero -o hello-bin` (4 lines, `build/` removed before each) | 546,380,446; 534,111,346; 530,152,338; 529,672,941; 537,475,792; 530,275,577 | 531,053,159; 537,941,349; 529,671,936 |
| `heroes check selfhost/main.hero --apply` (clean, no round) | 84,823,248,034 | 84,787,757,216 |
| `heroes check main.hero --apply` on 607's case (`probe/case607`, 02:41) | 78,573,398; 63,935,223 | 108,145,601; 108,172,877 |

The base's own spread is 0.25% on the check (84.67 to 84.88 billion) and 3%
on the cold build; W sits inside both. **No measurable cost**, as the code
predicts: every line W adds runs only inside `--apply` once a stage has
spoken, plus one empty list and one extra call on every module load
(`modules.load_text` now delegates to `load_edited`, and
`reading.text_at` walks an empty list before `source_text`). The one row
that moves is the one where W does work: on 607's case the base writes
nothing and stops after its first round, and W writes `lib/geom.hero` and so
reads and checks the program once more to see that nothing is left, about
1.5 times the base's instructions on a three-file program.

## Route W, built (`route.diff`)

What it does, file by file, and where the lines land:

| file | layout unit, base to W | `+` / `-` in the diff | what |
|---|---|---|---|
| `selfhost/cli/apply_modules.hero` (new) | 0 to 126 | +138 | a round over every module but the root; landings asked before any write; two names landing on one string refused; a second (hard) name refused before any write; each file written whole, the ones already written named on a failure |
| `selfhost/cli/certain.hero` | 237 to 270 | +41 -8 | `applied_to(diagnostics, s, file)`, `applied` its root case; `inside`, `home_of`, `homeless`; `outcomes` marks a module's fix by its own file's choice, `elsewhere` only for a fix crossing files |
| `selfhost/cli/apply_answer.hero` | 113 to 140 | +42 -13 | `Settled.modules`; the write of root plus modules; the stdout note; `"modules"` in the `--json` document |
| `selfhost/cli/check.hero` | 267 to 274 | +18 -11 | `settle` carries the modules' edited texts across rounds and stops when neither root nor modules moved |
| `selfhost/modules.hero` | **319 to 324, past its `DECIDED` 320** | +6 -1 | `load_edited`, a module read from the round's text, not the disk |
| `selfhost/module/reading.hero` | 183 to 191 | +8 | `text_at` |
| `selfhost/source_extent.hero` | 44 to 54 | +13 -3 | `end_of`, and `faulted` moved here |
| `selfhost/cli/check_json.hero` | 134 to 124 | +1 -11 | `faulted` moved out |
| `selfhost/cli/files.hero` | 177 to 182 | +5 | `is_file` |
| `tests/harness/suite_fixes.hero` | (tests) | +35 | the case copied afresh before every root; a module a root's run rewrote is held to its own `.fixed` or `.applied` |

Gates run on it, in my copy, with `heroes-w` built from those sources by
`heroes-base` (`build-w3.log`, 02:27):

- `heroes-w check selfhost/main.hero --brief`: clean. (The first build was
  refused by the compiler, `shadowed_binding` on `files` against `use
  cli/files`; repaired with `as fs`. The language caught my mistake.)
- the compiler's own tests: **1608 tests, all passed** (`test-selfhost-w.log`).
- `fixes` form: **977 passed, 1 failed** (`fixes-w.log`): `applylater/main.hero`,
  *main.applied checks clean, so ... its answer is a `.fixed`*. W writes
  `geom.hero`'s `fn` too, so that fixture's answer kind moves, and the `.fixed`
  judge (`judge_fixture_fixed`) writes the root's `--apply` stdout into the copy
  and rechecks, which a W `.fixed` cannot satisfy without writing the modules
  as well. Owed by the landing, unbuilt: about 15 lines there, and the
  comments that say a module's fix is not written: five files of
  `applylater/` and `suite_fixes.hero`'s own (*`applyx/main.hero`, which is
  left as it is*), by `grep -l "another module\|never writes\|left as it is"`
  at 02:43. `applyx/main.hero` passed: its run rewrote `geom.hero` in the
  copy, and the new judge found it equal to `geom.fixed`.
- `layout`: **5 passed, 1 failed** (`layout-w.log`): `selfhost/modules.hero`
  324 against 320. Owed: a seam (`open_directory` and `directory_text`, the
  root's directory as text, 40 lines that read no module, beside
  `module/paths.hero`'s `uses_of`), or the row raised with its reason. I did
  not trim comments to fit: that is a count gamed, not a seam.
- the net's own tests (`heroes-w test tests/harness/main.hero`, 02:39 to
  02:41, `test-harness-w.log`): **336 tests, 1 failed**, *a dead citation is
  caught, and the anchored rule ignores what it must*
  (`suite_records.hero:6181`, `excused["CLAUDE.md"]`, which asks git what it
  ignores). The same harness under `heroes-base` (02:41 to 02:43,
  `test-harness-base.log`): the same 336 and the same 1. My copy is a plain
  copy inside `.claude/worktrees/`, which the trunk's git ignores, so this is
  where the copy stands and not route W.
- `surface`: see § The harness writes the tree below.

### The shapes, each run with `heroes-w` (`probe/`)

| shape | result |
|---|---|
| 607's own case (`case607`, copied to `w607`) | `main.hero unchanged: none of the diagnostics is in it` / `rewrote lib/geom.hero`, exit 0; `geom.hero:5` now `y: i64 = util.one(x)`; `check main.hero` exit 0 after |
| `--apply` alone (stdout) | the root printed as before, and on stderr *note: 1 other module(s) hold certain fixes, which `--in-place` writes and `--json` holds: lib/geom.hero* |
| `--apply --json`, and with `--in-place` | `"modules": [{"file": "lib/geom.hero", ...}]`, `"not_applied": []`; `check --json` alone now says `"first_round": "written"` for that fix, where the base said `elsewhere` |
| root and two modules with fixes (`p10`) | three `rewrote` lines in load order, exit 0, the copy checks clean |
| a chain of three `.must()` in a module (`p9`) | written over three rounds, the module's own text carried between them, never the disk; clean after |
| a module not writable, the root holding a fix too (`p3`, mode 444) | *error: cannot write lib/geom.hero: it cannot be opened for writing ... untouched*, exit 2, **and the root untouched** (asked before any write) |
| a module with a second name, a hard link (`p6`) | refused before any write, exit 2, nothing written |
| a module reached under two names on this volume (`diskutil info /`: *File System Personality: APFS*, not case-sensitive; `ls p4/lib` holds `geom.hero` alone), `use lib/geom` and `use lib/Geom as g2` (`p4`) | the base `check` reports the diagnostic twice; W writes the one file twice, `rewrote lib/geom.hero` and `rewrote lib/Geom.hero`, the same bytes. **The landing guard compares strings and does not see this**; the outcome was right because both copies hold one text and one root directory, so one set of fixes |
| a module that is a symbolic link to a file outside the root's directory (`p5`, `lib/geom.hero -> ../../outside/geom.hero`) | W writes `outside/geom.hero` through the link, the link kept, and says only `rewrote lib/geom.hero`. A file outside the root's directory, named by no argument |
| M-inferred-cell's own migration in a module, `v: i64 @ 1` (`probe/old`, 02:44) | the base's `check` speaks at the parse, `error[old_cell_symbol] ... write @=`; W: `rewrote lib/m.hero`, now `v: i64 @= 1`, clean after. A parse fix, so W1 below covers it |
| a module that is not UTF-8 (`p7`) | never lexed, no fix, untouched; the root's fix written |
| **the same module meaning two things to two roots (`p2`)** | below |

### The finding: certainty is relative to the root

`d/a.hero` says `use lib/m`; `d/lib/b.hero` says `use m`; both reach
`d/lib/m.hero`, which says `use util` and `return util.count().must()`.
From `d/`, `util` is `d/util.hero`, `count() -> i64?`, and `a.hero` checks
clean (exit 0). From `d/lib/`, `util` is `d/lib/util.hero`, `count() -> i64`,
and `m.hero` takes `error[bad_operand] ... fix (certain): remove the
.must(…)` (spec § 1: *Every `use` starts at the directory of the file you
compile*).

- base, `check lib/b.hero --apply --in-place`: the fix is
  `"first_round": "elsewhere"`, nothing written.
- **W, the same command: `rewrote lib/m.hero`, exit 0, and `check a.hero`
  is then `lib/m.hero:4:12: error[type_mismatch]: expected i64, found i64?`,
  exit 1.** A program that compiled, broken by a command that named neither
  it nor the file it wrote.
- **base, `check lib/m.hero --apply --in-place` from `d/`: `rewrote
  lib/m.hero`, and `a.hero` broken the same way** (`probe/p2base`). So the
  class is today's, for a file the author named; W extends it to every file
  the root reaches.

How often the tree holds that shape, by a census of `heroes-base check
<file> --json` over every tracked `.hero` at `1dd890751` (3,498 files,
`hero-files.txt`; 2,034 exit 1, 1,464 exit 0, every answer a JSON document;
`analyze.py` over `census/`): **795 files are read as a module by some other
file checked as a root, 447 by two roots or more, and 1 from two root
directories**: `examples/shapes/render/scale.hero`, reached from
`examples/shapes/` and `examples/shapes/render/`, the example that exists to
show the resolution differ. It holds no fix. **The roots where W changes
anything at all today: 2 of 3,498** by the base's first round
(`"first_round": "elsewhere"`), `surface-fixtures/applyx/main.hero` and
`applylater/other.hero`, both fixtures for this very note.

**With the built rule**, `heroes-w check <file> --apply --json` over the same
3,498 files (`census-w/`, `analyze-w.py`, 02:37 to 02:39; 3,494 exit 0, 4
exit 1, the four `fixedbugs-227-*` roots that are not UTF-8, answered as
diagnostics as the base answers them): **3 roots would write another module,
2 distinct module files**, `applylater/main.hero` (through its second round,
which the first-round census cannot see), `applylater/other.hero` and
`applyx/main.hero`, onto `applylater/geom.hero` and `applyx/geom.hero`; **0
roots keep a `not_applied` entry**. Every one is a `surface-fixtures/` case.
Afterwards the 3,498 tracked files of my copy matched `1dd890751` but for
the nine I edited (`cmp` each, 02:40): `--apply` without `--in-place` wrote
nothing.

### The harness writes the tree

`tests/harness/suite_surface.hero:684` runs `check
tests/golden/surface-fixtures/applyx/main.hero --apply --in-place` **on the
tracked tree, not on a copy**, and expects the 621 note. Under the base it
writes nothing, because the root is unchanged and the module is never written.
Under W it rewrote `tests/golden/surface-fixtures/applyx/geom.hero` in the
tree the suite was judging, measured in my copy: § Surface under W below.

## The other routes, priced

**N, a new flag** (say `--modules`) beside `--in-place`: every line of
`route.diff`, plus one `plain(...)` row in `selfhost/cli/table.hero` (266
lines), a refusal in `selfhost/cli/check_flags.hero` (58 lines) for the flag
without `--apply --in-place`, about 10 lines, and two `surface` rows. It keeps
`--in-place` meaning the file the command line names, so `suite_surface.hero:684`
and every caller of today's `--in-place` stay as they are, and the widening is
asked for. Same soundness class as W, but opted into.

**S, the root only and the message names the modules**: zero lines; it is
what `1dd890751` does (621's note, reproduced at 02:16). It does not close 607:
the lane's measurement stands in my copy, no invocation writes a fix in a
nested module with a `use` line.

**L, a package's library modules left alone**: zero sites today. A `use` path
is `ident {"/" ident}` (spec § 1's grammar; `selfhost/module/paths.hero`'s
`path_at`), opened as `open_directory(root) + path + ".hero"`
(`selfhost/modules.hero:127` of the base), and a path that climbs is refused
(`probe/up`, `use ../ex/m`: `error[module_path_climbs]`, exit 1, run at
02:38), so `examples/` cannot be reached from `tests/` by `use` and every
module is under the root's directory by name, and no package search path exists in the loader (grepped
`package` in `modules.hero`, `module/reading.hero`, `module/names.hero`: two
comment lines naming M-package-layout). The only way out is a link (`p5`).
L is therefore best had by construction: **W refuses to write a module whose
landing is not under the root's directory**, which covers `p5` today and
`heroes add`'s packages the day they arrive. Unbuilt; about 10 lines, and it
needs the root's directory and the landing compared as canonical paths, which
`publish.landing_for` does not give (it follows the last part's links only,
`runtime/parts/replace.c`'s `hero_fs_landing`).

**Routes nobody listed**:

- **R, `check <file> --from <dir>`**: check a nested module with its `use`
  lines resolved from a named directory, so it can be named and fixed alone.
  Only named files are ever written, and 607's gap closes, at one command per
  file (the very cost 607 names). A flag plus the directory threaded into
  `load_text`'s `open_directory(path)`, about 20 lines in `modules.hero`,
  which has 1 line of room.
- **W1, W for the first stage only**: write a module's fixes only where the
  stage that spoke is the parse (stage 1, `cli/check.hero`'s `said_at`), whose
  fixes read one file's bytes and nothing else, so they mean the same from
  every root. Closes the hazard entirely; covers M-inferred-cell's `@` to `@=`
  migration (a parse fix, `parse/annotation.hero`'s `cell_symbol`) and not its
  `@=` to `=` one (`never_rebound`, the checker). One condition in
  `apply_modules.round_over`, under 5 lines.
- **Identity, not spelling**: the guard against two names for one file needs
  the file's identity (device and inode), which `selfhost/cli/files.hero`
  does not ask (its doors, read at 02:22: landing, kind, links, writable,
  stage, replace, flush, unstage, mode, set_mode, link, symlink,
  deny_delete, flagged, set_flag). A runtime door, about 15 lines of C in
  `runtime/parts/replace.c` and 5 of Heroes, under the C boundary rules.

## Questions the brief asks every seat

1. **Flag meaning or new capability.** W changes how one question (*the
   program with every certain fix applied*) is answered about the same input,
   the root, so by `.claude/rules/cli-surface.md`'s stopping rule it is
   admissible with no new entry; N is a new flag nothing must type today.
   But `--in-place` naming files the command line did not name changes WHAT
   the flag writes, not where: `--apply` prints one file and `--apply
   --in-place` writes N. The contract's three channels hold in what I built
   (the artifact on stdout, every `rewrote` on stderr, exit 0, 1 and 2 as
   before), but the parallel of *`--json` says how, never what* is broken for
   `--in-place`. That is the spec-warden's ground; I record that the code can
   keep it either way for ten lines.
2. **The routes**: verdicts below.
3. **Atomicity and safety**: each file whole (`publish.rewrite`, untouched);
   the set refused together for every refusal knowable before a write
   (landing, writability, a second name: `p3`, `p6`), and written one by one
   after, a failure naming what was already written. A full disk mid-set
   leaves a mixed set, each file whole: not run here. A link is followed out
   of the root's directory (`p5`): refuse it (condition 3). The same module
   twice is harmless when the two copies hold one text (`p4`) but the guard is
   by spelling. `--json` gains `"modules"` and `not_applied` narrows to fixes
   that cross files: panel 193's R4 field changes meaning.
4. **Does the spec or design.md say what `--apply` writes**: the spec holds no
   `--apply` (grep `apply\|in-place` in `spec/heroes-spec.md`: two unrelated
   lines, 274 and 303). design.md §4.17 says it twice, both falsified by W:
   *"as `"not_applied"` the certain fixes in another module, which it never
   writes"* and *"`elsewhere`, another module's, which `--apply` never
   writes"*. The cost is two sentences rewritten, and I would add the third
   the finding owes: **a certain fix in a module is certain for the programs
   compiled from this root's directory**, with `p2` as its pinned case.

### Why W rather than N, from this seat

`selfhost/cli/check.hero`'s own comment on `settle` (lines 159 to 162 of the
base) states the flag's contract: *"Fixes outside the file being written are
said out loud rather than dropped ... and this flag's contract is EVERY
certain fix."* The root-only write was the implementation's limit, said out
loud since 621, not the contract. W meets the contract; N would make *every*
mean *every, with a second flag*. N costs about ten lines more and buys
consent, which is what the conservative record should keep; robust is W with
the hazard written, pinned and guarded (conditions 1 to 4), since what N
protects (the named-file rule) does not protect the named file from the same
hazard (`probe/p2base`).

## Verdict per route

| route | verdict | why |
|---|---|---|
| W, widen `--in-place` | **object; approve under conditions 1 to 4** | inert on cost; widens a measured soundness class to unnamed files; the harness runs it on the tracked tree |
| N, a new flag | **approve** as the conservative record | same code plus about 10 lines; the widening is asked for, `--in-place` and its callers unchanged |
| S, root only, named in the message | **object** | the status quo; does not close 607, measured |
| L, packages left alone | **approve, by construction inside W** | zero sites today; the landing-under-root guard is L for links now and packages later |
| R, `--from <dir>` | object | writes only named files, but one command per file is 607's own complaint |
| W1, stage 1 only in modules | approve as a fallback | sound for every root; covers one of the two migrations that motivated 607 |
| **C, consensus over a directory's roots** (added 03:20, built) | **approve, recommended** | file operand untouched (schema 2 as ratified); 607 closed in one command; `p2` refused and named; all gates green; § Route C |

## What would change my verdict

To **approve W** outright, the landing carries:

1. `suite_surface.hero:684`'s run moved into a copy (or onto a fixture whose
   module holds no fix), so no suite writes the tracked tree;
2. the hazard written down: design.md §4.17's two sentences replaced and the
   third added (*certain for programs compiled from this root's directory*),
   and `p2`'s shape pinned as a `surface-fixtures/` case whose header says so;
3. a module whose landing is not under the root's directory refused, named, not
   written (`p5`; route L by construction);
4. `modules.hero` paid by a seam, not a raised row, and the `.fixed` judge of
   `suite_fixes.hero` applying in place, so `applylater/main` takes its
   `.fixed`.

To **veto**, a measurement I did not find: a root in the tree whose W run
writes a module a second root then refuses (my census found the shape in one
file, holding no fix), or a mid-set failure that leaves a file cut rather
than whole.

## Prediction

At the batch that lands defect 607 by W or N, measured against `1dd890751`:
`selfhost/` grows by **150 to 300 code lines** in `suite_layout`'s unit
(this build: 211); `heroes check selfhost/main.hero` moves by **under 0.5%**
in instructions retired; and the `fixes` form's changed cases are **exactly
the two module fixtures**, `applylater` and `applyx`, and no other. Falsified
by any one of the three.

## Surface under W

`./heroes-w run tests/harness/main.hero -- ./heroes-w surface` in my copy,
02:34 to 02:36 (`surface-w.log`): **401 passed, 7 failed**, and
**`tests/golden/surface-fixtures/applyx/geom.hero` rewritten in the tree the
suite was judging**: `shasum` 0aadf567... before the run, 9e02a3b0... after,
line 3 `return total` become `return totl`. That is `suite_surface.hero:684`'s
row, which runs `--apply --in-place` on the tracked file. On the trunk the
first net after W lands would leave a golden fixture modified in the working
tree, and the `fixes` and `annotations` forms would then judge the modified
one. (My copy's file put back from `git show 1dd890751:` at 02:37.)

The other six failures are the rows that pin what W changes on purpose, each
owed a rewrite by the landing: the `first_round` of a module's fix, `elsewhere`
to `written` (one row); the stdout note's words (two rows); `"modules": []`
in every `applied` document (two rows); the 621 line with `--in-place` (the
row above). So the landing's harness bill is **7 `surface` rows, the `fixes`
`.fixed` judge, and 6 comments** (five `applylater/` files and
`suite_fixes.hero`'s), beyond `route.diff`.

## Route C, consensus, built at the coordinator's request (02:58)

The completeness critic's pass (`docs/panel/211-reports/critic.md`) named it
and found two things in my W that I accept: **W changed plain `check --json`
under schema 2** (a module's fix `elsewhere` to `written`, so every consumer
obeying §4.17's *write the `written` ones* would write modules), and **my `p2`
is panel 099's leaf hazard** (R5, `tests/golden/surface-fixtures/nested/decoy/`,
`suite_surface.hero:674`), which I did not cite. C is built in my copy from the
base, with W's reusable pieces only; W's `route.diff` stays beside it.

**What C is.** `heroes check <directory> --apply --in-place` finds every
root under the directory (a `.hero` file declaring `function main` at its
margin, read off the tokens; hidden directories and `build/` passed over, so
`.git` and `.claude/worktrees` are never walked into roots), asks every root
the stage `run` would report (`check.spoken`, which asks `said_at` stage by
stage, so the stages' order stays `run`'s alone), and writes a file only
where **every root that reads it makes the same text of it**, a root with no
fix for it voting for the text as it stands. Rounds repeat over the texts the
rounds before agreed, held in memory (`modules.load_edited`), until a round
agrees on nothing. Every write is then planned and refused together before
the first byte; each file is written whole (`publish.rewrite`).

**What C leaves alone.** A file operand, every flag: `check main.hero
--apply --in-place` on 607's case answers the base's two lines, `"not_applied":
["lib/geom.hero:5"]`, `"first_round": "elsewhere"`, no `"modules"` key
(`probe-c3/f607`, 03:09). `certain.outcomes`, `apply_answer.hero`, `settle`
and `suite_fixes.hero` are the base's, byte for byte. A directory with
anything but `--apply --in-place` is told unreadable as defect 295 left it;
with `--json` as well it is refused at exit 2 (*a directory has many
answers*).

**`routeC.diff`**, against `1dd890751`: **+456 / -48 over 11 files**
(`patch -p1 --dry-run` on `git archive 1dd890751 selfhost`: 11 files apply).
In `suite_layout`'s unit:

| file | base to C | what |
|---|---|---|
| `selfhost/cli/consensus.hero` (new) | 0 to 239 | roots, rounds, votes, agreement, the disputes told |
| `selfhost/cli/agreed_writes.hero` (new) | 0 to 94 | the plan: inside the directory, no link on the way, one write per file, landings and second names asked before any write; the writes |
| `selfhost/modules.hero` | **319 to 303** | `load_edited`; `directory_text` moved out to pay for it |
| `selfhost/module/paths.hero` | 105 to 128 | `directory_text`, a path's text read before any file opens |
| `selfhost/cli/certain.hero` | 237 to 248 | `applied_to` and `inside`; `outcomes` untouched |
| `selfhost/cli/check.hero` | 267 to 277 | `spoken` |
| `selfhost/main.hero` | 297 to **300** | the dispatch, at the ceiling |
| `source_extent`, `module/reading`, `cli/files`, `cli/check_json` | 44 to 54, 183 to 191, 177 to 182, 134 to 124 | as in W |

Net **+377 code lines**, 333 of them in the two new modules; W was 211.

### C's gates, my copy, `heroes-c4` built by `heroes-base` from the final sources

Run 03:14 to 03:20 (`build-c4.log`, `test-selfhost-c4.log`, `surface-c4.log`,
`layout-c4.log`, `fixes-c4.log`):

- `heroes-c4 check selfhost/main.hero --brief`: clean.
- the compiler's own tests: **1610 tests, all passed** (the base's and C's
  three).
- `surface`: **408 passed, 0 failed**, and every `.hero` under
  `tests/golden/surface-fixtures/` byte for byte as before the run (`shasum`
  of each, before and after, `cmp`: equal). The first C (`heroes-c3`) read 407
  and 1: defect 295's row, `heroes check tests/golden` told *cannot read*,
  which C had answered with its own refusal; so only `--apply --in-place`
  on a directory reaches C now, and that row stands unmoved.
- `layout`: **6 passed, 0 failed**. The first C read 4 and 2
  (`consensus.hero` past 300, `main.hero` at 301, `modules.hero` at 324, a
  `+` in a loop): split along the plan's seam into `agreed_writes.hero`,
  `directory_text` moved to `module/paths.hero`, the loop joined once.
- `fixes`: **978 passed, 0 failed**, the base's count: no fixture moves.
- 607 and `p2` again on `heroes-c4` (`probe-c4/`, 03:20): the same answers as
  below; `check c --apply` without `--in-place` says *cannot read `c`*, exit 2.

### The shapes against C (`probe-c3/`, `heroes-c3`, 03:08; `heroes-c4` differs from it only in the dispatch's condition, and 607 and `p2` were run again on it)

| shape | C's answer |
|---|---|
| 607's case, `check c607 --apply --in-place` | `rewrote c607/lib/geom.hero`, exit 0, `y: i64 = util.one(x)`, the root clean after: **607 closed, one command for the directory** |
| **`p2`, operand `p2/d`** | *note: p2/d/lib/m.hero was not written: the roots that read it compute different certain fixes for it (changing it: p2/d/lib/b.hero; leaving it: p2/d/a.hero) ...*, exit 0, `a.hero` still clean. **Refused, named, with the roots** |
| `p2`, operand `p2/d/lib` (`p2sub`) | `rewrote p2sub/d/lib/m.hero`, and `a.hero` exits 1 on `type_mismatch`. **C is sound within its operand only**: `a.hero` stands above the directory named and is not asked |
| a broken root sharing a module (`stuck`: `b.hero` holds a parse error with no certain fix) | `m.hero` not written, named (*changing it: stuck/a.hero; leaving it: stuck/b.hero*): a root stopped at an earlier stage votes for the text as it stands, so a shared module's later-stage fix waits until every reader reaches it. Conservative by construction |
| two roots agreeing (`shared`) | `rewrote shared/lib/m.hero` once, both roots clean |
| root and two modules (`p10`), a three-round chain in a module (`p9`), the old cell symbol in a module (`old`) | written; every root clean after |
| a module not writable, the root holding a fix (`p3`) | refused before any write, exit 2, the root untouched |
| a hard link (`p6`) | refused before any write, exit 2 |
| a module that is a link out of the directory (`p5`) | *cannot write p5/r/lib/geom.hero: p5/r/lib/geom.hero is a symbolic link ... nothing was written*, exit 2, the target untouched |
| a directory on the way that is a link (`p5d`, `r/lib -> ../real/lib`) | *p5d/r/lib is a symbolic link ...*, refused, exit 2 |
| one file under two spellings (`p4`, `lib/geom` and `lib/Geom`, this volume ignoring case) | `rewrote p4/lib/geom.hero` **once** |
| a root under a hidden directory (`hid/.lane/main.hero` beside `hid/x/main.hero`) | only `hid/x/lib/geom.hero` written; the `.lane` copy untouched |
| a module that is not text (`p7`) | nothing written: its root stops at the lexer's `not_text`, as the base does |

### C's cost (`/usr/bin/time -l`, instructions retired)

| run | `heroes-base` | `heroes-c3` |
|---|---|---|
| `check selfhost/main.hero`, three pairs | 85,076,509,515; 85,014,492,848; 84,965,452,533 | 85,095,500,559; 85,001,261,800; 84,978,308,738 |
| cold `build hello.hero`, three pairs | 542,971,110; 529,933,649; 533,354,021 | 542,635,377; 530,094,694; 533,696,387 |
| `check <607's directory> --apply --in-place` | (no such answer) | 290,171,676; 290,457,637 |
| `check examples --apply --in-place` on `git archive 1dd890751 examples` (120 `.hero`) | (none) | 27,623,141,690, 3.05 s user; nothing written, nothing disputed |

Nothing moves on the paths the base has. A directory costs every root's
check per round: on 607's case about four times the base's file answer,
because `spoken` asks `said_at` up to four times and each asking parses
again, and a second round checks that nothing is left. A `spoken` that ran
the stages once, as `run` does, would save most of it at the price of a
second reader of the stage order (unbuilt, about 15 lines and a test that
the two agree).

### The whole tree under C

`heroes-c check fulltree --apply --in-place` on `git archive 1dd890751`
(3,498 `.hero`), 03:03 to 03:16 (`fulltree-c.err`): **exit 0, 773 files
rewritten, 0 disputes**; `real 819.96`, `user 687.18` (the machine shared
with the suites above, so `real` is not read), **6,224,959,987,565
instructions retired**, 499,400,704 bytes at most resident. Where they land:
581 under `docs/` (443 in `docs/panel/178-reports/` alone: the programs the
sittings kept as evidence, 854 tracked `.hero` under `docs/`) and 192 under
`tests/` (152 `tests/golden/check/`, 34 `surface-fixtures/`, 4 `full/`, 2
`permissive/`); none under `selfhost/` or `examples/`. Of the 773, the files
that are no root of their own are the **same 2 fixtures as W**,
`applylater/geom.hero` and `applyx/geom.hero` (and 3 `fixedbugs-251` cases
whose `function main(` my `grep` did not match past their control bytes);
every other write is a root's own fix, which `check <root> --apply
--in-place` writes today. **0 disputes across 447 shared modules** agrees
with the critic's `disagree.py` over my census.

What this says about the operand: **a whole-repository run rewrites records
and golden cases**, `docs/panel/` being append-only (`.claude/rules/records.md`)
and `tests/golden/check/` existing to hold certain fixes. The author names
the trees a migration means (`selfhost`, `tests/harness`, `examples`, ...),
and a landing should either say so in `--in-place`'s help or pass over
`docs/` and `tests/golden/` as it passes over hidden directories and
`build/` (a premise about the world, owed a test that fires when it dies:
unbuilt, priced at under 10 lines).

### What C does not settle

- **The operand is the program set.** A root above the directory named reads
  files inside it and is not asked (`p2sub`). Run over the whole repository
  it is complete; run over a subdirectory it is as sound as W for the roots
  above. The message cannot know they exist; §4.17 would have to say it.
- **A broken root freezes the modules it reads** (`stuck`), by design: the
  price of never writing a fix some reader has not computed.
- **The certain fixes of the golden cases are fixes too.** `check tests
  --apply --in-place` would write every certain fix of `tests/golden/check/`,
  whose cases exist to hold them; the operand is the author's to choose, as
  it was for M-inferred-cell's one-root-at-a-time runs.
- **Identity** is still by spelling plus case folding; a hard link is refused
  by its count, a link on the way by `fs.entry`, both before any write.
- **The published text** the critic found (`site/src/llms.txt:63-66`) stays
  true under C: a file operand answers as before.

### Verdict on C

**Approve, as the route the sitting adopts**, from this seat. It adds no
core construct (cli and loader only), costs nothing measurable on any path
the base has, keeps panel 193's ratified schema-2 meaning for a file operand
by construction (the base's `outcomes`, `apply_answer`, `settle` and fixes
suite untouched), closes 607 with one command per directory, and turns the
root-relative hazard from a silent wrong write (W, `p2`) into a named refusal.
Its residual, the roots above the operand, is the same hazard the base has
for a named file and smaller than W's. No veto.

## Progress log

- 02:13 tree verified; reading 607, the CLI rules, the apply path.
- 02:15 base compiler built from the seed in my copy (`heroes-base`).
- 02:16 607 reproduced with `heroes-base` in `probe/case607/` (root `use
  lib/geom`, `lib/geom.hero` holding `y: i64 @= util.one(x)` beside `use
  lib/util`): `check main.hero --apply --in-place` prints *note: 1 certain
  fix(es) are in another module and were not applied: lib/geom.hero:5*, exit
  0, nothing written; `check geom.hero --apply --in-place` from `lib/` prints
  *geom.hero unchanged: no `certain` fix applies to its 1 diagnostic(s)*,
  exit 0, nothing written.
- 02:17 base costs measured; 02:18 to 02:22 route W written.
- 02:23 base census started (3,498 files, `xargs -P 4`), ended by 02:25.
- 02:27 `heroes-w` rebuilt after the compiler refused my `files` binding.
- 02:26 to 02:30 shapes probed; the compiler's own tests and `fixes` run.
- 02:32 W's costs measured; 02:33 `layout`; 02:34 `surface` started,
  ended 02:36 with the fixture rewritten; 02:37 my copy's fixture restored.
- 02:37 to 02:39 the census with the built rule; 02:39 `route.diff` dry-run
  by `patch -p1 --dry-run` on `git archive 1dd890751`: all ten files apply.
- 02:39 to 02:43 the net's own tests under W and under the base, the same
  one failure in both; 02:44 the old cell symbol in a module.
- 02:55 the coordinator's message and the critic's pass read; route C built
  from the base (02:58 to 03:14: `heroes-c` 03:02, split for `layout` and
  rebuilt `heroes-c3` 03:07, the dispatch narrowed to `--apply --in-place`
  after `surface`'s defect-295 row, rebuilt `heroes-c4` 03:15).
- 03:03 the whole tree under C started on `git archive 1dd890751`, ended
  03:16: 773 written, 0 disputes.
- 03:14 to 03:20 `heroes-c4`'s gates: 1610 tests, `surface` 408 and 0,
  `layout` 6 and 0, `fixes` 978 and 0; `routeC.diff` regenerated at 03:14
  from the final sources (11 files apply to `git archive 1dd890751`).
