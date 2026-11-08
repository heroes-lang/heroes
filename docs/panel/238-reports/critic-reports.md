# Panel 238, the completeness critic's second pass: the reports

Read 2026-11-08 from 21:49 (`uptime`: load averages 3.53 3.82 4.06): the repaired briefs at
`27829fbba` (`.claude/worktrees/p238/docs/panel/238-briefs/`, read only) and the four reports in
their seats' worktrees (read only): `compiler-engineer.md` (803 lines), `ffi-pragmatist.md` (416),
`spec-warden.md` (270), `historian.md` (608), each counted by `wc -l`. Every command below was run
in this worktree (`.claude/worktrees/238-critic/`, detached at `d59c34489`, whose `selfhost/` and
`runtime/` are `27829fbba`'s: `git diff --stat d59c34489 27829fbba` touches `docs/panel/238-*`
alone), with programs copied from the seats' `build/238/` into `build/238/second/`. The base
compiler is this worktree's `heroes-next`, `sources 62dba14251e6eafb`, every seat's base key.
One note on my own conduct: before reading the coordinator's rule closely I ran `--version` of the
three seats' `heroes-next` binaries where they stand (each printed `sources 62dba14251e6eafb`; nothing
written); every other run in this pass used this worktree's compilers or copies under
`build/238/second/`.

Attacked: the coordinator's provisional direction (R1 the compiler-engineer's rule, R2 the route in
the same push, R3 the C side per the ffi-pragmatist's item 6 with its items 7.1 to 7.4 filed
`blocking`, R4 the order-pinning goldens renamed, R5 `check --apply` does not sort, R7 no spec
change, R10 and R11 the index at a root `packages/` with the citation collision repaired in
`records/citations`, `index/` named as the alternative).

Verdicts: `reproduces`, `does not reproduce` (with the true value), `contradiction` (with the side
a command supports), `unbuilt route`, `unmeasured` (a claim the synthesis must not adopt without the
run named), `question`.

## Findings

### 1. The route compiler, rebuilt here, and the seats' keys

- Ran: `cmp` of the compiler-engineer's and the ffi-pragmatist's `build/238/prototype.diff` (equal);
  the diff applied with `patch -p1` to a copy of `selfhost/` under `build/238/second/walk/` (seven
  files, exit 0); `./heroes-next build build/238/second/walk/selfhost/main.hero -o
  build/238/second/heroes-walk` under `/usr/bin/time -l`.
- Printed: exit 0, `777303944961 instructions retired`, `sources 01116def3a2e944c`, the
  ffi-pragmatist's route key exactly; the compiler-engineer's own in-place build of the same diff
  reads `7d731d2212cf5abf` (its item 0): one diff, two keys, which suggests the key reads where the
  sources stand as well as their bytes (an inference, not run further). The compiler-engineer's repaired formatter, `build/238/heroes-fix` (`sources
  5db2de271ee28872`), copied here as `build/238/second/heroes-fix` and run only here.
- Verdict: **reproduces** (the two seats' routes are one compiler).

### 2. Contradiction: *the route makes the author's premise true for every output measured* against *the route flips `sig_written` to a crash and `dep_written` to exit 2*

- Ran, on this Mac, in `build/238/second/link/` (the ffi-pragmatist's `src/`, `hero/` and scripts,
  copied): `sh c-link.sh out-darwin`, `sh c-sig.sh out-darwin`, then `HERO_DIR=hero sh hero-sig.sh
  <compiler> out-darwin <tag>` with this worktree's base and with `heroes-walk`, `hero/build`
  removed between the two; `heroes-fix fmt sig_written.hero` and `heroes-walk fmt` of the same,
  each `cmp`'d with `sig_sorted.hero`. In `build/238/second/two_others/` (copied), `run` of
  `rs_first.hero` and `pw_first.hero` under both compilers.
- Printed: C alone, `-la -lw` `pick=1 10` exit 0, `-lw -la` **Segmentation fault, 139**. Heroes:
  base `sig_written` **`1 30`, exit 0**; base `sig_sorted` **Bus error, 138**; route, **both
  Bus error, 138**. `heroes-fix fmt sig_written.hero` is byte for byte `sig_sorted.hero`, and so is
  `heroes-walk fmt`. `two_others`: base `rs_first` **`9`, exit 0**, base `pw_first` exit 1
  `ffi_field_type`, *`Rs.a` is `long long` in `rs.h`*; route, **both exit 1**, the same message.
- Verdict: **contradiction in what each seat calls breaking, not in what each measured.** Both
  literal claims reproduce: under the route every program gives one result in both orders. But the
  compiler-engineer's *the route breaks nothing* and its withdrawal condition (item 2e and its
  `condition`: *a program in the tree or in a release's corpus*) are scoped to tracked programs, of
  which none has two modules with groups (the ffi-pragmatist's item 4, and here: `git ls-files` of
  `examples/` by folder finds one folder with two files holding `extern`, `gallery`, whose
  programs are one module each). **The commands support the ffi-pragmatist on the question the
  synthesis must answer**: landing R2 changes what a working program does, `sig_written` from `1 30`
  to a memory fault and `rs_first` from `9` to refused, each with no `use` order left that escapes.
  The synthesis must restate the compiler-engineer's condition over shapes, not over the tree, and
  record that R2 removes a working program's only lever until the ffi-pragmatist's 7.1 and 7.2 land.

### 3. Contradiction: the compiler-engineer's conservative alternative against the ffi-pragmatist's veto

- Ran: the `cmp` of finding 2 (`fmt` of `sig_written` is `sig_sorted`).
- Printed: as finding 2.
- Verdict: **contradiction, the ffi-pragmatist's side measured.** The compiler-engineer's
  `condition` offers *the rule without the route* if the author rules the outputs outside the
  premise; CLAUDE.md § 4 asks the synthesis to record what the conservative route would have been.
  Under the rule alone `heroes fmt --in-place` turns a program that runs into one that builds at
  exit 0 and dies by a signal, which is the ffi-pragmatist's veto (design.md §1.12). A veto is a
  refusal, not a price (CLAUDE.md § 4): the synthesis must record the conservative route as
  **vetoed**, not as an option the author may pick.

### 4. The spec-warden's own trigger fires on the ffi-pragmatist's table, and R7 rests on a report written before it

- Ran: `grep -n 'fail to link\|had 15'` of the spec-warden's report (lines 208 and 212); in
  `build/238/second/link/onemod/`, one module holding both groups (`link "top"` above `link
  "base"`, and the reverse), each behind a root, built by base and by `heroes-walk` with `--library
  ../out-darwin/lib`, run, and the `-l` words of `build/*/link-words.rsp` read.
- Printed: the spec-warden's section 3: *"With the route, its outcome is fixed by the modules'
  names; if such a program could still fail to link, the spec would owe the reader how to fix it,
  and that is a sentence of § 13 to price then"*, and that the ffi-pragmatist's report *"had 15
  lines when I read it at about 20:05 and does not answer it yet"*. The one-module programs: base and
  route alike, `top` first writes **`"-ltop" "-lbase"`**, `base` first writes **`"-lbase"
  "-ltop"`**, each runs `41 2` on this Mac (ld64 links either order).
- Verdict: **contradiction, R7 unmeasured against the seat's own condition.** The ffi-pragmatist
  measured `dep_written` under the route at exit 2 on GNU ld in both orders (its item 3), which is
  the case the spec-warden says owes § 13 a sentence. The fix the sentence would state exists and is
  measured here as far as words go: inside one module the libraries follow the groups' written
  order, under base and route, and `-ltop -lbase` links on GNU ld (the ffi-pragmatist's C table;
  the Heroes program unrun on Linux by me). Spec line 376 states that order for **headers** only.
  And `sig` under the route is no failure to link but a memory fault decided by module names, a
  case none of the spec-warden's conditions names, because the seat never saw it. **The synthesis
  must not adopt R7 without putting the ffi-pragmatist's item 3 table to the spec-warden**, or
  without R3's group words built and measured (finding 5), which would leave only the `sig` and
  `dup` classes to rule on.

### 5. R3's group words are unbuilt; one of their questions measured here

- Ran: `build/238/second/nest/nest.sh` inside `heroes-linux-arm64` (`docker run --rm -v
  "$PWD/nest":/n heroes-linux-arm64 sh /n/nest.sh`, `TMPDIR` under the mount): `libtop.a` and
  `libbase.a` from the ffi-pragmatist's sources, linked four ways.
- Printed: `GNU ld (GNU Binutils for Debian) 2.44`; bare `-lbase -ltop` **LINK FAILED**; one group
  around both, `41 2`; **a group nested inside the compiler's group (a package whose own words
  carry a group), `41 2`**; two groups side by side, `-lbase`'s then `-ltop`'s, **LINK FAILED**.
- Verdict: **unmeasured as a compiler change.** The words close `dep`, `pp` and `lp` only as one
  group around the package words AND the `link` names together (two groups side by side do not
  resolve across), and a package's own group inside it is accepted by GNU ld 2.44, so nesting is not
  an objection there. Still unrun: the compiler built with the words (`cli/link.hero:151-168`, a
  branch ld64 must not take, which refuses the words); the `run` form and `dep`/`pp`/`lp` on the
  container and on the CI's x86-64 leg; Windows' `lld-link` (the ffi-pragmatist: unrun). **The
  synthesis must not write that R3 closes the link half until that compiler has run them.**

### 6. R3 *closed per item 6* closes less than it says, and *filed `blocking`* holds the milestone

- Ran: in `build/238/second/link/hero/`, `heroes-next build only_top.hero --library
  ../out-darwin/lib -o ../only_top`.
- Printed: **exit 2**, `internal error: linking failed:`, ld64's `"_base_helper", referenced from:
  _top_value in libtop.a[2](top.o)`, then *"the linker refused the objects clang compiled"*.
- Verdict: **unmeasured claim in the direction.** Of the ffi-pragmatist's three repairs only the
  group words can travel in this push; the refusal of a name another library supplied is *"a new
  diagnostic class, so a sitting's"* and the `two_others` repair *"panel 204's family, a
  sitting's"* (its item 6). Item 7.3's `only_top` half is not touched by group words at all: the
  library its group needs is on no line, and the exit 2 above stands. Filing 7.1 to 7.4 `blocking`
  means `records/tagged` refuses M-package-manager's tag (CLAUDE.md § Verification, *a milestone is
  tagged only over a clean list*) until two more sittings land and 7.3's message is repaired; the
  synthesis must say that, or class each with its reason.

### 7. `two_others`: a route nobody built, which keeps the program instead of refusing it truly

- Ran: finding 2's `two_others` runs; `sed -n 1,30p selfhost/cli/header_order.hero` (first pass).
- Printed: base `rs_first` `9`; every other cell exit 1. `header_order.hero`'s comment: panel 204's
  *"a move"*, *"each other group of the refused one's module, in the unit's order, is tried once on
  the other side of it, the whole unit's opening compiled (`cli/order_trial.hero`), and the first
  order that compiles is told"*.
- Verdict: **unbuilt route.** R3 takes *"a true refusal for two_others"*, which tells the truth and
  still refuses a program `run` accepts today in its written order, a `blocking` shape by
  `.claude/rules/verification.md` § Bounded discovery (*a correct program refused*), with no `use`
  order left under R2. The route nobody built: the search panel 204 built for a module's own groups,
  asked of the OTHER modules' records a unit reads, and decided by the compiler rather than told:
  the key's order first, then, where a record's layout differs from its own module's unit
  (`ffi_field_type`'s question), the next order of those modules' headers, the first in which every
  record reads as its own unit reads it taken. `rs.h` before `pw.h` is such an order here, so both
  written orders would build and print `9`. What would have to be true: one clang probe per order
  tried, its cost measured in instructions (rank 5), and a refusal kept where no order works. It is
  panel 204's family, so a sitting's; the synthesis should name it beside the true refusal rather
  than adopt the refusal as the repair.

### 8. R10 and R11: the collision repaired *in `records/citations`* is the exemption list that instrument refuses; `index/` measured clean

- Ran: `git worktree add --detach build/238/second/idxname 27829fbba` (this worktree's
  `heroes-next` copied in, `sources 62dba14251e6eafb`); `git mv examples/index index`; `./heroes-next
  run tests/harness/main.hero -- ./heroes-next records` into `build/238/second/records-index.txt`;
  then `git mv index packages` and the same into `records-packages.txt`; the scratch worktree removed
  with `git worktree remove --force` (`git worktree list | grep -c idxname`: 0).
- Printed: `index/`: **`records: 27 passed, 1 failed`**, `records/citations` naming **2** lines,
  `docs/roadmap/milestones/M-core-packages.md:402` and `tests/harness/suite_packages.hero:8`, both
  `examples/index/`, which any move rewrites (`175701222157 instructions retired`). `packages/`: the
  same 2 and **13** more, `tests/harness/suite_packages.hero:25` to `:56`, `packages/index`,
  `packages/placed`, `packages/reach`, `packages/ran`, `packages/case`, `packages/names`,
  `packages/codes`, `packages/quiet`, `packages/docs`, `packages/tests`, `packages/cards`,
  `packages/copies`, `packages/internal`: the compiler-engineer's 15, reproduced. The check's own
  text, printed beneath: *"that is the evidence this check accepts, and it is why there is no list of
  exemptions to edit."*
- Verdict: **contradiction between R10/R11 and the instrument.** Repairing the collision *in
  `records/citations`* means teaching it that `packages/<check>` is not a path, an exemption list the
  check says it exists not to have. The two repairs that keep the instrument as it is: the 13
  comment lines reworded in `suite_packages.hero` (every later sentence naming a check must then
  avoid the shape), or the name `index/`, measured here with **no** collision. The historian's one
  condition against `packages/` (a planned workspace feature) is answered by the record: panel
  235's R2 is Go's modules *"without workspaces"* and its line 724 keeps *"a workspace or a
  configuration beside the catalogue"* refused (`grep -n -i workspace` of the synthesis), so what
  stays against the name is a reader's expectation (the JavaScript pages, Unity), not a plan. The
  name is the author's; the synthesis now has a measurement for each.

### 9. R1 reproduces rustfmt 6241's open bug, which the historian proposes as *Taken from*

- Ran, in `build/238/second/r6241/` with `heroes-fix fmt`: `run_heading.hero` (`use beta`, `use
  delta`, a blank, `# the two modules that print the report`, `use alpha`, `use gamma`),
  `tight.hero` (the same without the blank), `trailing3720.hero` (`use c  # use c` / `use b  # use
  b` / `use a  # use a`); and over the tree, with `LC_ALL=C`, an `awk` printing each comment run
  glued above a `use` that follows an earlier `use` (`build/238/second/run-heading-shapes.txt`).
- Printed: both comment shapes give `# the two modules that print the report` glued above `use
  alpha` at the top of the file, then `use beta`, `use delta`, `use gamma`, exit 0: the remark that
  headed `alpha` and `gamma` now heads `alpha` alone, at the place the rule's next reading takes for
  the file's header. The trailing shape gives `use a  # use a` / `use b  # use b` / `use c  # use c`,
  each remark on its own line (rustfmt 3720's bug avoided, as the historian's prediction expects).
  The tree holds **3** comment runs glued above a later `use`: `examples/shapes/render/ascii.hero:12`
  (a remark about that one line), `selfhost/emit.hero:15` and `selfhost/emit/members.hero:17` (file
  headers below a first line written above them); none heads a run of two or more.
- Verdict: **contradiction between the historian's candidate line and R1.** Issue 6241 reports
  *"keep the comment attached to the immediate `use` item after it, therefore fundamentally changing
  its meaning"* as a bug, open; R1 does exactly that. A *Taken from* line for 6241 would say the
  resolution rests on a page whose bug it repeats, which `.claude/rules/records.md` § And the record
  says whose idea it was does not admit (*each line says what the cited page itself states*, and the
  form *Taken from* says the resolution adopted something on the page's evidence). The cost today is 0 files of the tree; the synthesis either records
  the reading as a known cost (and drops 6241 as *Taken from*), or changes R1 so a comment with a
  blank line above it rises above the block as the rule's other loose comments do (unbuilt; by the
  `awk` above it would reach one file of the tree, `ascii.hero:12`, whose remark is about its one
  line and would leave it).

### 10. The landing order meets the hooks' compiler, which is `<tree>/heroes`

- Ran: `sed -n 65,67p` and `118,127p .claude/hooks/trees.py`; in `build/238/second/seedfmt/`, a
  file `use <core>/strings`, a blank, `use zeta`, a `main`, through `./heroes fmt` (this worktree's
  seed build, `sources 1a1e340778db789a`) and `./heroes-next fmt`; the `unsorted` column of
  `build/238/census.txt` for each file the landing's first commit edits.
- Printed: `compiler_of` returns `<tree>/heroes`; `rebuild_hint` offers `./heroes build
  selfhost/main.hero -o heroes` or clang on the seed. The seed's build: `error[expected_module_name]`
  at `groups.hero:1:1`, *refusing to format a file with diagnostics*, exit 1 (it reads no bracket,
  defect 2352). Stage 2 before the landing: the two lines printed **with no blank between the
  groups**, exit 0. Unsorted today: `selfhost/modules.hero`, `resolve/top.hero`, `print/guard.hero`,
  `print/fmt.hero`, `probe/judge.hero`, `cli/syntax_cmds.hero`, and `cli/link.hero` (R3's words):
  every one.
- Verdict: **unmeasured landing.** The hooks judge with whatever `<tree>/heroes` a tree holds; the
  seed is not refreshed at the landing (the compiler-engineer's item 3d), and a pre-landing stage 2
  removes the blank the rule writes between the groups. So until each tree rebuilds `heroes` from
  the landed source, its hooks tell *not canonical* of the landing's 9 two-group files outside `check/` and the hoist
  (the compiler-engineer's item 3a),
  and their own remedy (`heroes fmt --in-place`) writes the old form that the landed `canonical`
  refuses. And the first commit of the compiler-engineer's order, with R3's words, edits seven files
  whose blocks are out of order today (six of its item 4's, and `cli/link.hero` for the words):
  committed by a tree whose `heroes` knows the rule, `staged.py`'s `offences` (lines 95-135, read,
  the commit unrun) refuses it unless those seven are formatted in that commit. The synthesis must name the rebuild (`./heroes build
  selfhost/main.hero -o heroes`, the hook's own hint) in every open tree after the merge, decide
  whether the landing refreshes the seed so a fresh tree's `heroes` knows the rule, and let the
  landing lane make the commits before writing their split as settled.

### 11. R2 rebinds a working program's call at the landing, silently

- Ran: `HERO_DIR=hero sh hero-link.sh <compiler> out-darwin <tag>` (copied script) with base and
  `heroes-walk`, `hero/build` removed between them.
- Printed: base `dup_written` **`1 10 20`** static and shared, `dup_sorted` `2 10 20`; route, all
  four **`2 10 20`**.
- Verdict: **reproduces** the ffi-pragmatist, and names a duty the direction omits. A program
  written as `dup_written` calls `liba`'s `pick` today and `libb`'s after the landing, with no
  message: neither a refusal (which CLAUDE.md § 2 says a `0.x` change *is told*) nor anything a suite
  sees. The synthesis must say where a reader is told (the release's notes, the decision issue), and
  the ffi-pragmatist's 7.1 is the repair that ends it whatever the order.

### 12. The route's claim holds on one more output nobody listed; the dumps are order-free

- Ran, in `build/238/second/outs/`: modules `t1.hero` and `t2.hero`, each with a `test`, and two roots
  with a `test` each, `use t2` / `use t1` (`written.hero`) and the swap (`sorted.hero`): `heroes
  test`, `build --dump-ir`, `parse --dump-ast`, `check --dump-scopes`, under base and the route, the
  root's name normalised before `diff`.
- Printed: base `test written.hero` runs `"root"`, `"two is two"`, `"one is one"`; `sorted.hero`
  runs `"root"`, `"one is one"`, `"two is two"`; the route runs `root`, `one`, `two` for both.
  `--dump-ir` and `--dump-scopes`: 0 differing lines under both compilers; `--dump-ast`: 2, the root's
  own two `use` lines, under both. (`build` refuses `--dump-ast` and `--dump-scopes`; they belong to
  `parse` and `check`.)
- Verdict: **reproduces** the compiler-engineer's route claim on a seventh output its list of six
  (item 2a) did not name, `heroes test`'s order across modules, which the route makes order-free;
  the dumps print the root file alone. The brief asked for each `--dump-*` printer (question A); the
  report shows none, and this fills it.

### 13. The design.md draft says more than the measurement

- Ran: `grep -n 'no output can see\|reaches no output but that file'` of the compiler-engineer's
  report (lines 578 and 179); its item 3b read.
- Printed: the §4.15 draft (item 5): *"the reorder is the one rewrite `fmt` makes that no output can
  see"*; item 2c: *"reaches no output but that file's own line numbers"*; item 3b: `adventure` 208,
  `ledger` 366, `markdown` 164, `query` 374, `todo` 98 diff lines in the emission, *"every one a
  number"*.
- Verdict: **does not reproduce** as drafted. The blank line between the groups and the hoist move
  the line numbers of 10 files (the compiler-engineer's item 3a), which the `#line`s, every
  diagnostic's position and five blessed emissions see. The draft should say what item 2c says.

### 14. Smaller claims checked

- **834, not 833.** Ran: `awk '$2>=2' build/238/census.txt | wc -l` and a `grep -c '^use '` loop over
  every tracked `.hero` outside `docs/`: **834** both; the spec-warden says 834, the
  compiler-engineer *"833 tracked files with two or more `use` lines"* (item 1). **Does not
  reproduce** by one; nothing rests on it.
- **The stale blessed keys** (the compiler-engineer's item 3b). Ran: `git merge-base
  --is-ancestor 82f9d0a5a bd4670da4` (true); `git show --stat 82f9d0a5a -- runtime` (`runtime/parts/
  fs.c`, 60 insertions); `git log a789691f1..bd4670da4 -- tests/emission` (nothing). **Reproduces**
  as git facts (the 771 failures unrun here), and it makes the landing's gate wait on batch 25's
  blessing, a lane outside this sitting: the compiler-engineer's prediction (*red on exactly the
  nine*) cannot be scored before it. The synthesis should name that dependency in the landing order.
- **The rule as it would land has not passed the compiler's own tests.** The compiler-engineer's
  item 3e: `heroes-ref test selfhost/main.hero` **1,910 tests, 1 failed**, the failure repaired as
  `heroes-fix`, and the suite **not re-run** (*1.17 trillion instructions*), one `cmp` of one test's
  input standing in; the `layout` suite's growth and directory-budget rules unrun, `use_order.hero`
  24 lines over its threshold. **Unmeasured**: owed by the landing, not by the synthesis, but the
  synthesis must not write *the rule passes its tests*.
- **The ffi-pragmatist's counts.** Ran: `git grep -h '^extern' -- selfhost/ | sort | uniq -c` (9
  `hero_compiler.h`, 16 `hero_os.h`, 3 `stdlib.h`, in 22 files, no `link`, no `package`); the
  per-folder count of `examples/` files holding `extern` (19 folders with one, `gallery` with two
  one-module programs). **Reproduces** (its items 4 and 5).
- **The key's premise, *byte order on the path is the order part by part*** (the compiler-engineer's
  item 1, point 2). Ran, in `build/238/second/chars/`, `heroes-next parse` of a file opening `use
  a$b`, `use a-b`, `use caf\xc3\xa9` and `use a.b`: exit 1 each, `unexpected_character`,
  `expected_declaration`, `unexpected_character`, `module_path_has_no_parts`. No character below `/`
  (`-` 0x2D, `.` 0x2E) and none outside ASCII stands in a part. **Reproduces** for the characters
  tried.
- **R4 pins the key.** Under R2 the renamed cases (560's default, `onefile453type`,
  `onefile453macro`, `unsupported` 555 and 2136) guard by their modules' names sorting in the pinned
  order, so a later change of the key (the historian's 2b: Rust's 2024 edition moved from ASCII order
  to version sorting) reverses them in silence, as F22 found the written order would. A
  **question**: whether each renamed case states in its header that its names are chosen for the
  key, and whether `use_key.hero`'s own test names them.

### 15. The ffi-pragmatist's conditions against the compiler-engineer's landing order

- Ran: findings 2 and 11 (the route on this Mac); `grep -n 'No platform leg before the push'`
  of `.claude/rules/verification.md`.
- Printed: under `heroes-walk`, `sig` exits 138 in both orders, `dup` prints `2` in all four
  builds, `two_others` exits 1 in both; `cside`'s words were the ffi-pragmatist's (`"-lm" "-lc"` both,
  its item 2). The rule file's § The optimistic chain, item 4: *"No platform leg before the push"*.
- Verdict: **contradiction, partly closed.** The veto's first condition (the rule and the route in
  one push, one result per program in both orders) is what the compiler-engineer's three commits
  give, measured here on this Mac. The objection's condition, the GNU ld group words *in the same
  landing*, is in none of its three commits, and R3 must add it; and since the words are unbuilt
  (finding 5) and change every Linux link, the optimistic chain would have the CI's Linux legs be
  their first judge, after the push. The synthesis should either run the landing compiler's `run`
  form and `dep`/`pp`/`lp` in the arm64 container before the push (an exception to the chain, said
  as one), or accept that a red Linux leg is reverted (`git revert`, item 5 of that section).

## What the synthesis must not adopt without a further run

1. **R3 *closes the link half*** until a compiler with the group words has linked `dep`, `pp`, `lp`
   and the `run` form on GNU ld (the arm64 container now, the CI's x86-64 leg after), and Windows'
   `lld-link` has been asked (findings 5, 15).
2. **R7 *no spec change*** until the spec-warden has read the ffi-pragmatist's item 3 table: its own
   trigger (*if such a program could still fail to link*) is met by `dep` under the route without
   the words, and `sig`'s memory fault decided by module names was never put to it (finding 4).
3. **R10/R11's repair *in `records/citations`***: it adds the exemption list the check says it
   refuses; `index/` measured with no collision, `packages/` with 13 (finding 8).
4. **The historian's *Taken from* rustfmt 6241**, while R1 does what 6241 reports as a bug
   (finding 9).
5. **R2 *breaks nothing***: true of the tree, false of shapes: `sig_written`, `rs_first` and
   `dup_written` change what they do at the landing (findings 2, 11). The record must say so, and
   where a reader is told.
6. **The compiler-engineer's landing split**, until the landing lane has made its commits under the
   hooks (finding 10), and **its prediction**, which batch 25's blessing must precede (finding 14).
7. ***The rule passes the compiler's own tests***: `heroes-fix`'s full run is owed (finding 14).
8. **The design.md draft's *no output can see*** (finding 13).

## Count

15 findings. **Contradictions** with the side a command supports: 2, 3, 4, 8, 9, 15 (6).
**Unmeasured or unbuilt** (a claim or a route the synthesis must not adopt as settled): 5, 6, 7, 10
(4). **Does not reproduce**: 13, and the 833 of finding 14. **Reproduces**: 1, 11, 12, and three of
finding 14's six (the stale keys, the ffi-pragmatist's counts, the key's premise); the other two of
finding 14 are unmeasured (the compiler's own tests) and a question (R4 pins the key). Those that
change the resolution: 2, 3, 4, 5, 6, 7, 8, 9, 10, 15.
