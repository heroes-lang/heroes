# Panel 241, the compiler-engineer's report

Seat: compiler-engineer (the ceiling: design.md §1.1, §1.7, Part 5). Worktree
`.claude/worktrees/241-compiler-engineer`, detached at `87d77dec2`. Scratch and logs:
`.claude/worktrees/p241-scratch/compiler-engineer/`. Written as it goes; every number names the
command that produced it.

## Status

- 07:51 stage 1 from the seed and stage 2 started in the worktree (load averages 2.38 3.41 4.28 by
  `uptime` at the start).
- 07:53 baseline stage 1 `real 9.55` (`sources 5aae00294f7cd6fa`); stage 2 `real 112.07 user 85.74
  sys 18.98` cold (`sources 110fad075b8d3bca`), machine not still (other seats), planning figure only.
- route 1 applied: `heroes add core --index file://../packages -C selfhost` exit 0 in `real 0.12`,
  wrote `selfhost/heroes.toml` and placed `selfhost/heroes.packages/core@0.2.0-4029f4f2486e6ca7/`
  (13 files, 10 modules, 2,334 lines by `wc -l`). The key is `sha256("index\nfile://../packages\ncore\n
  0.2.0\n<sum>\n")[:16]` by `module/placed.hero:108-110`, recomputed in Python: `4029f4f2486e6ca7`;
  `examples/`' spelling `file://../../packages` gives `8c07af1fca61d171`.
- 33 calls moved in 10 modules of `selfhost/fetch/` (script `move_calls.py` in scratch): `starts_with`
  22 to `has_prefix`, `ends_with` 6 to `has_suffix`, `holds` 5 to `contains`, labels `text:` to `s:`
  and `needle:` to `substr:`. 8 modules swap `use cli/strings` for `use <core>/strings`; `git_source`
  and `transport` keep `cli/strings` for `trimmed`/`with_all` and add `use <core>/strings as
  core_strings`; `made_repo` untouched. `heroes fmt --in-place` on each: `git diff --stat` 10 files,
  +53 -42.
- route 1 two-stage build: stage 2 by stage 1 `real 48.40` (warm cache), `sources 618783256086fff0`;
  stage 3 by stage 2 `real 47.71`, same key; `--emit-c` of both 33,318,988 bytes, `cmp` exit 0.
  **The fixpoint holds.**
- the guard: the unpatched mirror keys the route-1 tree `58c7670a691b397b`, the compiler
  `618783256086fff0`. Patch in the worktree's `.claude/hooks/provenance.py` (+59 -2 by `git diff
  --stat`, 65 changed lines with the docstrings): `PACKAGE_USE`, `reached_of`, `package_root` (catalogue
  by `tomllib`, folder by `placed.key`'s rule, a `path` entry by its folder, a git entry and a
  package's own bracketed `use` raised `Unmirrored`, which the guard reads as a refusal), `module_rows`
  following them. Patched, it keys the tree `618783256086fff0`. One case added to
  `test_hooks.py`'s `MirrorHeldToCompiler` (+20 -2), a `path` package reached by `use <pk>/...` and
  `as`, its bare `use` from its own root, held to the tree's compiler. Hooks' own tests: 215 run, 214
  pass; the one red, `TrackedAsks.test_the_program_asks_nothing_and_a_push_is_still_asked`, is red at
  `87d77dec2` unpatched as well (`orig-trackedasks.log`: 3 run, 1 failure; the commit's
  `.claude/settings.json:54-60` holds five push rules where the test pins one). **A finding outside
  this sitting: the hooks' own tests are red at `87d77dec2`.**
- the guard on route 1, reproduced once: `./heroes-next run tests/harness/main.hero -- ./heroes-next
  order` refused, *"keys 58c7670a691b397b by the hook's mirror"*. So `order`, `probe`, `spec`,
  `layout`, `canonical`, `seed`, `records`, `unseen` and `packages` on route 1 are **unrun**; the guard
  was not got around. The emitted C's key is `618783256086fff0` (`grep hero_program_sources
  stage2.c`), the arbiter's, which the patched mirror reproduces.
- the compiler's own tests on route 1 (`./heroes-next test selfhost/main.hero`): `2006 tests, all
  passed`, `real 193.37 user 132.42 sys 48.38` (shared machine). The +14 are `core`'s own tests run as
  the compiler's: `grep -c '^test '` gives 11 in `strings.hero` and 3 in `strings/internal/scan.hero`.
- `#line`s naming the placed folder in route 1's stage-2 C: **13** (`grep -c '^#line.*heroes.packages'
  stage2.c`), 10 for `strings.hero`, 3 for `strings/internal/scan.hero`.
- R13 literally (`core`'s files copied to `selfhost/core/`, `use core/strings`, scratch copy `r13/`):
  `heroes check selfhost/main.hero` exit 1, `unknown_module` (`use strings/internal/scan` read from
  `selfhost/`) and `private_module`. A symlink `selfhost/strings -> core/strings`: exit 1,
  `private_module` still. A catalogue `path` entry, `[dependencies.core] path = "core"`, with `use
  <core>/strings`: **exit 0**, printing on every check *"note: the package `core` is read from the
  folder `selfhost/core/`, a `path` entry of the catalogue: unverified, no fingerprint holds it"*.

## Findings outside the brief's list, each run

1. **Two live panics of the compiler, exit 134, in hand-written text helpers that slice a text at
   another text's length** (the shape defects 296 and 2306 repaired in test helpers and in
   `examples/`; `gh issue list --search` finds no open issue naming these modules). Both reproduce
   with the trunk's compiler (`heroes-before`, `sources 110fad075b8d3bca`) as with route 1's:
   - `heroes build` of a binding whose header names `struct café` and declares a field of it with
     the wrong type: *"panic: string slice splits a character at emit/ffi_nested.hero:248:12, in
     ffi_nested.has"* (scratch `crash/`);
   - `heroes check` of a program with a bracketed `use` whose `heroes.toml` holds the row
     `[dependencieXé]`: *"panic: string slice splits a character at module/catalogue.hero:268:39, in
     catalogue.table"* (scratch `cat/`), the reader every build with a package passes through.
   The shape stands on 18 code lines of `selfhost/` outside tests (`slice(from: <at>, to: <at> +
   <x>.len())` and `slice(from: 0, to: <x>.len())`, by `git grep` and an awk that leaves `test`
   blocks out), `fetch/catalogue_edit.hero:197,219` among them; each line's inputs are unread
   beyond the two above. **Class by `.claude/rules/verification.md` § Bounded discovery: `blocking`
   (a crash).** Not filed: this seat writes nothing to GitHub. `cli/strings`' `starts_with`,
   `ends_with` and `holds` already compare bytes and are total, so the repair needs no package.
2. **The hooks' own tests are red at `87d77dec2`**: `TrackedAsks.test_the_program_asks_nothing_and_
   a_push_is_still_asked` pins one push rule and the commit's `.claude/settings.json:54-60` holds
   five (run unpatched from `git archive 87d77dec2 .claude`).
3. **The ROADMAP's compiler count is stale at `87d77dec2`**: `ROADMAP.md:21` says 205,397 lines in
   786 modules by `find selfhost -name '*.hero'`; the same command over the commit's tree gives
   786 and **205,539**.

## Question 1's admission count (`core` 0.2.0 against `selfhost/` at `87d77dec2`)

**How it was enumerated**: every top-level function of `core` 0.2.0's three packages (`awk
'/^function /'` above each `## Tests`): `strings`' `has_prefix`, `has_suffix`, `contains`,
`trim_space`, `fields`, `split_lines`, `split_once`; `strconv`'s `atoi`; `encoding/json`'s
`parse`, `render`, `field`. Then every `selfhost/` function of the same signature shapes,
`(str, str) -> bool` (77), `(str) -> [str]` (77), `(str) -> i64` or `i64?` (70), by `grep -rnE
'^function ...'`, and each candidate's BODY read (scratch `body.sh`), never its name. A copy is
counted where its answer is `core`'s on every input a `str` can hold. Line counts are the
function's own lines by the extractor (doc comments above it not counted), file `copies.txt`.

| `core` function | hand copies with its contract | their lines | notes |
|---|---|---|---|
| `strings.contains` | **26** (`has_text` 6, `has` 11, `holds` 2, `contains` 2, `strings_contains`, `diag_has`, `strings_hold`, `strings_has`, `c_words.found`) | 479 | 3 of them slice at every byte and panic on a text above ASCII (`symbolized.hero:69`, `ffi_field_told.hero:161`, `ffi_nested.hero:244`); `core`'s answers where they abort |
| `strings.has_prefix` | **7** (`cli/strings.starts_with`, `standard_name.starts`, `layout_rows.starts`, `op_brackets.starts`, `ffi_answer.begins`, `ffi_incomplete.begins`, `parse/records.opens_with`) | 25 | `opens_with` slices and is called from tests only |
| `strings.has_suffix` | **3** (`cli/strings.ends_with`, `standard_name.ends_with`, `op_brackets.ends`) | 16 | |
| `strings.split_lines` | **0** | | none strips `\r` before `\n`; 5 agree on text without `\r` (`clang_told`, `catalogue_edit`, `index_source`, `recovery`, `score` `lines_of`), 2 keep an empty last line (`deps.lines`, `c_words.lines_of`), 2 answer `[""]` for `""` (`model_lines`, `cut_lines`) |
| `strings.trim_space` | **0** read | | `cli/strings.trimmed` cuts `' '`, `'\t'`, `'\n'` only (`cli/strings.hero:52-68`), `core`'s Unicode white space with `\r` and U+00A0 (`strings.hero:47-50`); the other five `trimmed` (`header_types`, `reach_ask`, `ffi_lookup`, `ir/questions`, `print/anchors`) unread |
| `strings.fields`, `strings.split_once` | not counted | | `fields` candidates' bodies unread; no `(str, str) -> <pair>?` function found by the signature grep |
| `strconv.atoi` | **0** | | `runtime_abi.number` refuses a leading zero and 19 digits, `ffi_lookup.number` skips spaces, `clang_tree.digits` answers -1, `digits.decode_int` reads the literal syntax; none takes a sign |
| `encoding/json.*` | **0** | | `cli/clang_tree.hero` (320 lines) reads deeper than any guard by design (its lines 22-25), `core` stops at 512 (`tokens.hero:16-26`); `mutate/json_read.hero` (407) keeps a number's text, `core` reads an `f64` and objects as a map (`json.hero:40-60`) |

**So `core` 0.2.0 has a contract for 36 hand copies in the compiler, 520 lines, all three of them
`strings`' `has_prefix`, `has_suffix` and `contains`.** Route 1 as scoped touches **none of the
36**: `fetch/` holds no copy, it calls `cli/strings`, so the 33 calls move from one byte-safe helper
to a byte-identical one (`cli/strings.hero:112-122` and `core`'s `strings.hero:17-27` are the same
loop), and `cli/strings`' three stay for **370 calls in other modules** (`starts_with` 109 in 49,
`ends_with` 60 in 32, `holds` 201 in 54, by `grep -rnoE` with `--exclude-dir=fetch`). Route 1
deletes 0 lines and leaves two copies of each contract in one binary. The placed tree is 2,334
lines, of which the compiler reaches 368 (`strings.hero` 258, `strings/internal/scan.hero` 110);
`encoding/json` (1,845) and `strconv` (121) are placed, fingerprinted and counted as the compiler's
without being reached.

**`cli/strings`' `trimmed` and `with_all` have no `core` contract**: `trimmed` against
`trim_space` above; `with_all` is one list after another and `core` 0.2.0 has no list package
(`git ls-files packages` holds `strings`, `strconv`, `encoding/json` only). The scoping's "no"
stands, re-read.

## Question 4: R13 literally, a copy at `selfhost/core/`

Reproduced (scratch `r13/`, `core` 0.2.0's `files/` copied to `selfhost/core/`, the 33 calls reached
by `use core/strings`): `heroes check selfhost/main.hero` **exit 1**, two errors at
`selfhost/core/strings.hero:8:5`: `unknown_module` (`use strings/internal/scan` is read from the
directory of `selfhost/main.hero`) and `private_module` (the module is private to a `strings` the
copy is not). Tried short of editing the copy's bytes:

- a symlink `selfhost/strings -> core/strings`: exit 1, `private_module` remains, since the module
  that names it is `core/strings`;
- a catalogue `path` entry, `selfhost/heroes.toml` holding `[dependencies.core]` and `path =
  "core"`, the calls reached by `use <core>/strings`: **exit 0**. It is route 1 without the
  fingerprint: every check and build prints *"note: the package `core` is read from the folder
  `selfhost/core/`, a `path` entry of the catalogue: unverified, no fingerprint holds it"* (R6), and
  nothing holds the copy to the index.

So the literal R13 compiles only as a package by `path`; as plain modules of the compiler it needs
its `use` lines edited, which makes it a fork of `core`, not a copy.

## Question 5: the transport as an `os/exec` package, and route (c)

**What the transport reaches.** `fetch/` reaches the system through `cli/process` alone (507
lines, `extern "hero_os.h"` for `hero_run_*`, `hero_fs_*`, `hero_env_shown`, `hero_os_pid`,
`hero_run_serial`, `cli/process.hero:40-69`), and through `hero_env_put` from `hero_compiler.h`.
Its calls by module (`grep -oE 'process\.[a-z_]+\('`): `run` 7 (`transport` 2, `made_repo` 5),
`walk` 5, `mkdir_all` 22, `remove_tree` 29, `path_exists` 18, `is_directory` 3, `rename_over` 3,
`discard` 8, `env`, `pid`, `serial`, `exe_suffix`, `because` 1 each. **`cli/process` is used by 96
modules of the compiler, 11 of them in `fetch/`** (`grep -rl '^use cli/process$'`), every clang
invocation among the other 85. So an `os/exec` package either binds the same `hero_os.h` doors a
second time for `fetch/`'s 11 modules (two bindings of `hero_run_go` in one binary), or puts the
compiler's whole process layer behind a placed tree, every build of the compiler then depending on
a fetched package and every `core` version on a seed refresh.

**`hero_env_put`'s callers**: three compiler modules, two live, `fetch/transport.hero:162-163`
(`git_ran`, before its `## Tests` at 314) and `cli/symbolized.hero:56`, one test support only,
`fetch/made_repo.hero:68-69` (its lines 1-5); and two of the harness, `tests/harness/
case_setting.hero:214,223` and `suite_fetch.hero:83`. It is declared in `hero_compiler.h:28`, the
header no promise covers (its lines 7-17). A package binding it binds that header; moving it to
`hero_os.h` costs four places, as one existing name shows (`git grep hero_env_shown`): the
declaration, `heroes_guard_open.h`'s `push_macro`, `heroes_guard_close.h`'s `pop_macro`, and a word
of `selfhost/emit/guarded_names.hero`, an emitter file `macro_guard`'s tests hold to both headers.

**Route (c), built** (scratch `routec/`): a copy of `runtime/` gains `hero_bytes_sha256(const
unsigned char *p, int64_t n)` over the static `hero_sha256_bytes` (8 lines in `parts/sha256.c`) and
its declaration in `hero_compiler.h` (3 lines); a program binds it as `function hero_bytes_sha256(
@data: [u8] counted_by n lent, n: i64) -> str`, § 13's fill form with `N` the array's length, and
`HEROES_RUNTIME=<copy> heroes build` builds it at exit 0. It prints
`aa5cd9acfab25f643fb1cedb67f8770417ac9ce0b02cfe72a62fa1ec20e9f60a` for the bytes `0 1 2 255 254`
and `e3b0c442...b855` for none, both equal to `shasum -a 256`, and the array still holds 5 bytes.
**The compiler needed nothing beyond the declaration**, and the build ran against
`HERO_RUNTIME_ABI` 32 unchanged; the stamp moves *"whenever the declarations change shape"*
(`heroes_runtime.h:19`) and `hero_compiler.h:15-16` records panel 089's reading that an added
function moves none. **The seed's route**: a door only programs bind is not compiled into the seed's
compiler, so no refresh is owed by it (an inference from `seed/README.md` § When this file must be
regenerated; a stage 1 built with the changed runtime is unrun). If the compiler itself called it,
the README's order applies, *"anything the compiler's own code uses, enters `selfhost/` only after
the refresh whose seed knows it"*; unrun.

Correction, 08:12: the patch to `provenance.py` I wrote above as `+59 -2` is **`+60 -5`**, and
`test_hooks.py` `+18 -2`, by `git diff --numstat` after the `uses_of` repair (the first version
changed `uses_of`'s return shape, which `Digest.test_a_use_line_is_read_as_the_compiler_reads_its_
tokens` pins; it now keeps `uses_of` and reads package lines in `reached_of`).

## Question 3's costs of route 1, each measured where it could be

- **The seed.** Route 1's stage-2 C is 33,318,988 bytes and names the placed folder on 13 `#line`s.
  The folder's name is the SHA-256 of the catalogue's `index` spelling, name, version and sum
  (`placed.hero:108-110`), so a new `core` version, a new sum or another `index` spelling renames
  it, and by the rule of 2026-11-10 (`seed/README.md`, *"a module of `selfhost/` renamed or deleted
  refreshes this file in the same round"*, since `records/citations` reads each `#line` as a path
  the tree must hold) **every `core` the compiler takes is a seed refresh**. That last step is the
  rule applied, not a run: no second `core` version was taken here.
- **The instruments.** Already leaving any `heroes.packages/` out by name: `canonical`, `corpus`,
  `packages`, `records`, `unseen` (`placed_trees.hero:1-15`), 0 lines each. Walking `selfhost/`
  whole with `skip: nothing`: `order` (red on `core`'s `encoding/json.hero:186`, `names =
  sort(keys(o.fields))` with no `# ORDER:` mark, the scoping's red, read here and not run),
  `layout` (`suite_layout.hero:735`; its per-file ceiling holds by its declared mirror,
  `ceiling.code_lines`, largest `decimal.hero` 241 of 300; `ceiling.might_grow` is true for
  `numeral.hero`, so its growth check asks an allowance, unrun), `spec`'s anchors
  (`suite_spec.hero:914`, would judge `core`'s nine `spec §` citations for the first time, unrun),
  `seed` (reads 10 more files for a binding none holds, 0 lines), `probe`'s `selfhost, multi` (10
  more seeds against its floor of 758, which since defect 536 is told `FLOOR` and raised, 1 line).
  `packages` holds placed trees to the index only under `examples/` and pins every catalogue's
  index line to `file://../../packages` (`suite_packages.hero:82-93`), where the compiler's is
  `file://../packages`, so nothing in the net would hold `selfhost/`'s tree to the index. The
  measured price of teaching one walk the folder is `277676555` (batch 26, `canonical`): `+31 -1`
  in the suite, `+2 -1` in `placed_trees.hero`. For `order`, `layout`, `spec` and `probe` that is
  about 130 lines by that one precedent, an estimate; `packages`' second root is unestimated. And the
  trunk's hook: the mirror patch above, `+60 -5` and its test `+18 -2`, with every net suite on
  route 1 refused until it lands.
- **The site and the ROADMAP**, by their own rules (`sizes.ts:37-40,62-63`, every `.hero` under
  `selfhost/` and its newlines; `ROADMAP.md:21`'s `find`): 786 modules and 205,539 lines at
  `87d77dec2`, **796 and 207,884** on route 1, of which 10 and 2,334 are `core`'s; the site's
  floored `{{modules}}` would read 790 for 780 and `{{linesK}}` 207k for 205k.
- **The latency, the instrument as the brief names it**: `heroes run tests/harness/latency.hero --
  compare heroes-before ./heroes-next --clock check --runs 11`, from the route-1 worktree, 08:09 to
  08:12, load 4.03 before and 3.72 after (other seats working, so **the machine was not still**):
  before best 8.39 s, after 8.42 s; *"after slower in 7 of 9 untied pairs kept, the median by 0.04 s,
  0.4 %; the sign test's tail 9.0 %, not under 5 %: no verdict of slower"*, exit 0
  (`latency-compare-check.log`). **What this reads is the binaries' half only**: `latency_runs.hero:
  28,39` runs both compilers on `selfhost/main.hero` in one directory, so both checked the route-1
  tree, catalogue and fingerprint included. Route 1's own cost is the tree's, so the pairs below put
  each compiler in front of its own tree.
- Added to the admission table, 08:14: **`strings.fields`: 0 copies with its contract.** Read:
  `standard_name.words` and `ffi_lookup.words` (and `own_names.words_of`, `ffi_built.bare_words`
  over it) cut `' '` and `'\t'`, `supplied_windows.words_of` `' '`, `'\t'`, `'\r'`,
  `clang_added.words_of` `' '`; `core`'s cuts Unicode white space, `'\n'` and U+00A0 among it;
  `supplied_judged.fields` splits on a tab and holds a row to seven.

## Question 2 from the compiler's side: where `crypto/sha256`'s C comes from

For a PROGRAM's package, none of the four routes costs the compiler a line: (c) is measured above
(a declaration and nothing else); (a) is the `pkgconfig` mark panel 240 named, already compiled;
(b) is `fixedbugs-2351`'s package header (the critic, item 8, carried); (d) is Heroes. The ceiling
does not choose between them there.

It chooses where feature 2315 puts the package under the COMPILER. The compiler hashes a file's
bytes at every build that reads a placed tree (`module/fingerprint.hero:11-18`, through
`hero_file_sha256`), and already carries SHA-256 twice, `cli/sha256.hero` in Heroes (292 lines,
for `check --json`, `placed`, `fingerprint`) and the runtime's C door. So for the compiler's own
fingerprint: (a) over `libcrypto` adds `-lcrypto` to the seed's one clang line and leaves Windows,
where the page itself says *"Mac and Linux"* (`M-core-packages.md:32`), without the compiler's
package manager; (b) puts a package's header into the set of files the seed's C includes; (c) and
(d) cost the compiler nothing it does not already hold. (a) and (b) as the compiler's are
inferences from the seed's command and the include rule, not builds.
- **The latency, the tree's half** (scratch `pairs.sh`, `paired.py`: each compiler on its own tree,
  `heroes-before` on `base/`, a `git archive 87d77dec2` of `selfhost` and `runtime`, `heroes-next`
  on the route-1 worktree; 11 pairs, the order alternating as `latency.hero`'s, read by
  `latency_clock.paired`'s rule, which the script reproduces: pair 0 dropped, a run waiting more
  than 0.1 s dropped, sign tail under 5 % over at least 5 untied pairs, median past 0.5 %), 08:12 to
  08:16, load 3.64 then 5.00, **not still**: 7 pairs kept, 3 dropped as waiting, after slower in 6
  of 7, differences `-10, 240, 40, 130, 70, 30, 60` ms, median **+60 ms on 8,250 (0.73 %)**, sign
  tail 6.25 %: **no verdict of slower**; bests 8.21 s and 8.28 s. Not the instrument's own run,
  which cannot put two compilers before two trees; its rule applied by hand.
- **The same, in work** (`/usr/bin/time -l`, *instructions retired*, which a busy machine does not
  move as it moves `real`; five runs each, alternated): before median 110,503,846,740, after
  110,818,910,521, **+315,063,781 instructions, +0.285 %**, and every route-1 run above every
  baseline run by at least 158,163,134. **Route 1 costs every `heroes check selfhost/main.hero`
  about 0.29 % of its work**, under decision 2282's floor of effect, against a level of 8.9 s
  (`suite_latency.hero:51`) that these runs, 8.2 to 9.2 s, already stand close to. Every
  layer-0 hook check of a `selfhost/` write pays it too (`.claude/rules/verification.md` § A suite
  is the last judge: that hook runs `heroes check selfhost/main.hero`).
- **What the close's own comparison can see** (read, not run): `compare` at a milestone's close
  runs the previous tag's compiler and the new one on the tree checked out
  (`latency_runs.hero:28,39`), so both pay a cost that lives in the tree, and route 1's catalogue,
  fingerprint and two more modules read as zero there whatever they cost. Only `record`'s row held
  to the level sees it.

## Verdicts

**Question 1, the compiler carrying `core` as a catalogue entry: object to route 1 as scoped; no
veto.** design.md §1.7's criterion, *"does it remove a special case from the compiler?"*, and §1.1's
ceiling. It is no core construct (no checker, lowering or backend change: `use <name>/` exists and
the two-stage build and fixpoint pass, measured), so my veto does not reach it. But as scoped it
replaces **nothing**: 33 calls move from `cli/strings`' byte-safe helpers to byte-identical ones,
`cli/strings` keeps all three for 370 other calls, 0 of the 36 hand copies go, and the compiler's
tree gains 2,334 lines of which it reaches 368, a fingerprint on every check (+0.285 % of its work),
13 seed `#line`s that make every `core` version a seed refresh, four walks to teach (about 130 lines
by `277676555`'s precedent), `packages` blind to the tree, and a trunk hook that refuses every net
suite until it learns `use <name>/` (`+60 -5`, `+18 -2`). **The route I approve for the robustness
it closes**: the 33 hand copies outside `cli/strings` (488 lines) repaired onto `cli/strings`' three,
which closes the two live panics found here with no package, no seed coupling and no hook. **Route 1
becomes the right one** only scoped to the whole compiler (the 36 copies and `cli/strings`' three
replaced, 520 lines, which R1's admission test admits on the count and on the two panics), with the
mirror and the walks landed first, and design.md:571's *"No package is compiled into the compiler"*
read in the record as panel 097's condition 5 reads it (*"Nothing reaches
`selfhost/library_source.hero`"*), since by its letter route 1 breaches it.

**Question 2, where `crypto/sha256`'s C comes from: approve (c) or (d) for anything the compiler
itself hashes, and stay indifferent for a program's package.** For a program, all four cost the
compiler 0 lines ((c) measured: a declaration only, ABI 32 unmoved). For the compiler's own
fingerprint, (a) adds `-lcrypto` to the seed's one clang line and has no Windows by the page's own
row, and (b) puts a package header into the seed's includes (both inferences); (c) and (d) cost
nothing it does not hold already. Whether (c) breaches design.md:575 (*"the language still ships its
runtime and nothing a program could carry instead"*) is Part 1, beyond this lane; I note it, I do
not rule it.

**Question 3, how much the second version replaces: approve R19 as written, "the transport alone",
and object to the whole closure.** An `os/exec` package for the transport either binds `hero_os.h`'s
doors a second time beside `cli/process` (96 modules use it, 85 outside `fetch/`), or puts the
compiler's whole process layer, every clang invocation, behind a placed tree.

**Question 4, R13 literally: object.** A copy at `selfhost/core/` is refused at exit 1 (measured);
nothing short of editing its bytes compiles it except a `path` entry, which is route 1 without the
fingerprint, told *"unverified"* on every check. Edited, it is a fork of `core`.

**Question 5, from the compiler's side: the transport's move costs what question 3 says; route (c)
needs nothing in the compiler beyond a declaration (built and run), moves no `HERO_RUNTIME_ABI` at
the build, and owes the seed nothing while only programs bind it (an inference); moving
`hero_env_put` into `hero_os.h` costs four places, one of them an emitter file.**

## The seat's block

- `verdict`: **object** (question 1, route 1 as scoped); approve R19's *transport alone* (q3);
  approve (c) or (d) for the compiler's own hash (q2); object to R13 literally (q4); no veto.
- `section`: design.md §1.7 (*"does it ... remove a special case from the compiler?"*), §1.1 (the
  ceiling), §1.11:571-575 (*"No package is compiled into the compiler or the runtime"*, whose
  letter route 1 breaches and whose source, panel 097's condition 5, it does not).
- `implementation_cost`: compiler `selfhost/fetch/` +53 -42 over 10 files (`git diff --stat`); placed
  tree +13 files, 2,334 lines (`wc -l`), 368 reached; seed +13 `#line`s; hooks `provenance.py` +60 -5,
  `test_hooks.py` +18 -2; instruments about 130 lines for `order`, `layout`, `spec`, `probe` by one
  precedent (estimate), `packages` unestimated; check +0.285 % of its instructions; deleted: 0.
- `needed_for_self_hosting`: **no**. The compiler self-hosts at `87d77dec2` without it (the
  baseline's stage 2 is the trunk's key, `110fad075b8d3bca`).
- `argument`: Route 1 as scoped deletes no line. `fetch/` holds none of the 36 hand copies `core`'s
  contracts match; it already calls `cli/strings`, whose three bodies are `core`'s loops. So the
  binary gains a second copy of each, the tree 2,334 lines it mostly never reaches, every check a
  fingerprint, and every future `core` version a seed refresh, while two live panics stand in
  copies route 1 never touches. Robustness closes those panics; Principle 0 says how: onto the
  helper the compiler already trusts. Carrying `core` earns its cost only when it replaces the
  copies, all of them.
- `prediction`: if route 1 lands as scoped, the first commit that moves the compiler's `core` entry
  past 0.2.0, or its `index` line, either rewrites `seed/heroes.c` in the same round or reads
  `records` red on a `#line` naming `selfhost/heroes.packages/core@0.2.0-4029f4f2486e6ca7/`;
  checkable at the milestone that takes the next `core` into the compiler. And M-core-packages'
  closing `compare --clock all` will read no loss for route 1 whatever it costs, the tree's half
  being invisible to it; only `record`'s check row against 8.9 s can show the +0.29 %.
- `condition`: the admission scoped to the whole compiler (the 36 copies and `cli/strings`' three
  replaced, 520 lines deleted), with the trunk's mirror following `use <name>/` and `order`,
  `layout`, `spec` and `packages` taught `selfhost/`'s placed tree before the step lands and the net
  green on it; or a measured design.md Part 11 effect of the compiler using its own packages.

Correction, 08:24, to *"about 130 lines for `order`, `layout`, `spec`, `probe`"* above (twice):
`probe` probes placed trees on purpose (its `examples, multi` floor was raised at step 9 for *"the
fifteen trees of core 0.2.0 that programs carry"*, `suite_probe.hero:198-202`), so it owes its
`selfhost` floor raised, 1 line, and no walk taught. Of the walks, `order`'s red is known (read on
`json.hero:186`, the scoping's run); `layout`'s and `spec`'s are unrun behind the guard. So the
instruments owe about 31 lines for `order` by `277676555`'s precedent, the same again for `layout`
and for `spec` only where their runs read red, 1 line for `probe`, and `packages`' second root
unestimated: **31 to about 95 lines**, an estimate on one precedent.
- **The warm build, the tree's half** (the same scripts, `build selfhost/main.hero -o
  <scratch>/timed-<side>`, 7 pairs), 08:18 to 08:29, load 5.71 then 2.00, **not still**: 3 pairs
  kept, 3 dropped as waiting, differences `280, 70, -60` ms, median +70 ms on 45,290 (0.15 %):
  **fewer than 5 untied pairs, no verdict**; bests 45.10 s and 45.36 s, against a level of 49.1 s.
  The instrument's own `--clock build`, which would read the binaries' half only, was not run.
- **The seed's size**: the baseline stage 2's `--emit-c` over `base/` is 33,282,580 bytes, 318,138
  lines, 67,454 `#line`s; route 1's 33,318,988, 318,542 and 67,548 (`wc -c`, `wc -l`, `grep -c
  '^#line'`): **+36,408 bytes and +404 lines** carried by the next refresh, 13 of the `#line`s naming
  the placed folder.
- **The baseline's own tests**, measured here (a full `git archive 87d77dec2` in scratch
  `basefull/`, the before compiler): `1992 tests, all passed`, `real 200.39`. Route 1's 2,006 is that
  plus `core`'s 14. (A first run in `base/`, which holds only `selfhost/` and `runtime/`, failed on
  the fixtures and spec it lacked, and is not a reading.)

## What stayed unrun, and why

- The net's `order`, `probe`, `spec`, `layout`, `canonical`, `seed`, `records`, `unseen` and
  `packages` on route 1: the trunk's guard refuses them (its mirror keys the tree
  `58c7670a691b397b`, the compiler `618783256086fff0`), and was not got around. Their expected
  readings above are reads of their code, `order`'s red on `json.hero:186` included.
- The mirror patch in the trunk's hooks: written and tested in this worktree only (214 of 215,
  the red one red without it); the trunk's `.claude/hooks/` was not touched.
- A second `core` version taken by the compiler, so the seed refresh per version is the rule of
  2026-11-10 applied, not observed.
- The instrument's `compare --clock build` (the binaries' half), and any instruction count of the
  build clock; the build's two-tree pairs kept too few pairs for a verdict.
- Route (c) with a stage 1 built from the seed over the changed runtime, and the compiler itself
  calling the new door; (a) and (b) as the compiler's own hash.
- `fields`' and five `trimmed` candidates beyond those read; the 18 slicing lines' inputs beyond the
  two panics reproduced; anything on Linux or Windows.

## State left

- Worktree `.claude/worktrees/241-compiler-engineer`: route 1 applied and uncommitted
  (`selfhost/heroes.toml`, `selfhost/heroes.packages/`, 10 modules of `selfhost/fetch/`), the
  mirror patch in its `.claude/hooks/provenance.py` and `test_hooks.py`; `heroes` is stage 1 from
  the seed, `heroes-next` route 1's stage 2.
- Scratch `.claude/worktrees/p241-scratch/compiler-engineer/`: every log named above, the
  baseline compiler `heroes-before`, `base/` and `basefull/` (archives of `87d77dec2`), `r13/`,
  `routec/`, `crash/`, `cat/`, `slice/`, the scripts `body.sh`, `move_calls.py`, `pairs.sh`,
  `paired.py`.
1.2G scratch: basefull 557M, base 441M (du -sh, 08:37)
