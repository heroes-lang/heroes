# Panel 250, compiler-engineer

Read `00-shared.md` first, whole: the base (**rebased to `48f53b343`**, `core` 0.5.0 alone), the words
(*entry*, *module*, *face*, *Go package*, *placed*, *reached*), the measured fact that a `use` written
after a fetch is refused under a placement of the reached set, R2's ruler as panel 242 measured it, the
rulings quoted from the record, the routes (W, M, G, H; F1 to F4; the alternatives; the nine nobody
listed), the rules (**the two compilers keep the names `heroes` and `heroes-next`; every other binary
you build is named `ce-...`**; a suite's output goes into a file, never through `head` or `tail`; every
program that may not end is bounded there) and the platforms (**the Windows box is off**). You judge from
the compiler's side, with a veto on soundness. Your folder is
`.claude/worktrees/p250-scratch/250-compiler-engineer/`, your tree `tree/` in it, a `git archive` of
**`48f53b343`** (tree `d5453ae5f3d91854d12190a4ca02689568b67804`), your compiler built there from the seed
in two stages (`sources 5bac791363384945`). Your report is `docs/panel/250-reports/compiler-engineer.md`
in `.claude/worktrees/p250`, in your definition's output structure (`.claude/agents/compiler-engineer.md`,
from its line 36), written as you go. **The route you recommend, you build in your tree and run on the
cases you name**; a route that does not build is reported as not built, never adopted (`/panel` 3c).
Where a module's length decides anything, ask `tests/harness/suite_layout.hero`'s unit, not `wc -l`
(`.claude/rules/module-shape.md`).

## Where the code is (`wc -l` at the base)

- **The build's reading**: `selfhost/module/packages.hero` 416 (`rooted` 142, `problem_of` 169,
  `placed_said` 215, `changed_said` 249), `fingerprint.hero` 244 (`of` 32, `added` 95),
  `placed.hero` 239 (`key` 118, `keyed` 128), `placed_beside.hero` 378, `standing.hero` 105,
  `unplaced.hero` 111, `offered_modules.hero` 117, `catalogue.hero` 486, `sums.hero` 99, `reached.hero` 40.
- **The verbs**: `selfhost/fetch/`, 27 files, 6,054 lines (`wc -l selfhost/fetch/*.hero | tail -1`);
  among them `placing.hero` 71, `index_source.hero` 475, `git_source.hero` 398, `staging.hero` 448,
  `edited.hero` 128, `leftover.hero` 301, `add.hero` 461, `fetch_all.hero` 276, `verb_place.hero` 119,
  `build_list.hero` 229, `listing_read.hero` 92, `tree_names.hero` 173. None imports a parser, a lexer
  or a discovery module (`grep -h -E '^use ' selfhost/fetch/*.hero | sort | uniq -c`): a fetch-time
  closure is new code here, or a call into discovery.
- **The C side of a fetched unit**: `selfhost/cli/package_units.hero` 353 and `package_reads.hero` 454
  (panel 235's C1 and C2, the `-M` listing the build already keeps, their opening comments).
- **What a program was compiled from**: `selfhost/emit/provenance.hero`, *"`module <name> <key>` for
  every module the root reaches"* (its lines 8-9), and its mirror `.claude/hooks/provenance.py` (231
  lines), which does not follow `use <name>/…` at the base (`grep -n -i -E 'heroes.packages|package'
  .claude/hooks/provenance.py` printed nothing of placed trees; panel 241's compiler-engineer patched it,
  `241-*.md:79-81`, *"`provenance.py` +60 −5"*, carried; read it before you lean on it).
- **The suites that read placed trees**: `tests/harness/suite_packages.hero` 659 (its `placed`,
  `reach`, `ran` checks, 33-43), `suite_fetch.hero` 670 (the placed folders' names at 452-456),
  `placed_trees.hero` 80.

## What you measure, in this order

1. **The compiler's reach, beyond what is already asked.** The coordinator asked the compiler for all 18
   programs with a catalogue (`.claude/worktrees/p250-scratch/briefs/reach/lines48.sh`, 18 of 18 the same
   files as the closure; 00-shared.md § The numbers). Re-run it in your copy if you lean on it; then
   **build the one shape that would part them**: a package module that declares only records, reached
   by a program, and say what the `#line`s, the provenance key and any other output of the compiler
   name for it.
2. **Today's refusal of a pruned tree**, re-run on `examples/ini` at 0.5.0 (00-shared.md: `check` exit 1
   `package_unread`, `fetch` exit 1 `sum_mismatch`), and on `examples/server`.
3. **Build route M with fingerprint F1 in your tree**, or the route you would adopt instead with your
   reasons. Say and build: from which roots `heroes fetch` computes the closure (a folder holding several:
   `tests/harness` holds six, `generate`, `issues`, `latency`, `main`, `package_cards`, `thesis`); **where
   the listing stands**, since inside the key folder R6 sums it (`235-*.md:621-622`) and a package may
   publish a file of that name; **what a tree with no listing means** (the 55 hand-placed golden trees and
   every example until re-placed: a fall-back to today's whole-tree check, or every case rewritten);
   `placed_said` holding the listing to `sum` and each placed file to its line; what
   `fingerprint.added`, `staging`, `edited`, `placed_beside`, `standing` and `leftover` become; what
   `offered_modules` offers for a module the version holds and the program has not placed; the message a
   build gives the `use <core>/strconv` written after the fetch (00-shared.md's measured edit), and what
   both of today's messages' advice becomes; **what `fetch` does to a placed tree when the `use` set
   moves** (adds the newly reached files, keeps or removes the newly unreached), read against panel 246's
   R2 (*"never discarded in silence"*, `246-*.md:264-276`) and defect 2493's *"never replaces an edited
   placed tree"*; and the verbs' help (`selfhost/cli/table.hero:194-195`). Count each module's lines
   moved, deleted and added. A git source: no `list` is published, so say what the verb computes and
   writes (R6's sum is over the committed blobs, `235-*.md:612-633`).
4. **Run it**, every suite's output into a file and read from it:
   - every program of `examples/` and `tests/harness` re-placed under your route (a fresh `heroes fetch`
     into a copy), each one's `check`, and its `run` case where it has one; `examples/server`'s binary,
     if you run it, under 00-shared.md's server bound (`timeout -s KILL 60`, clients `curl --max-time 5`
     under `timeout -s KILL 90`);
   - the `fetch` suite and the `packages` suite whole (`./heroes-next run tests/harness/main.hero --
     ./heroes-next fetch > <file>`, then `packages`);
   - the 55 golden cases that hold a placed tree (`git ls-files tests/golden | grep '/heroes.packages/'`
     at the base), each form narrowed with the harness's third word, one run per substring: `check` by
     `package-`, `fixedbugs-2440`, `fixedbugs-2443`, `fixedbugs-2446`, `fixedbugs-2455`, `fixedbugs-2459`
     and `fixedbugs-2506` (its 22); `run` by `fixedbugs-2351`, `fixedbugs-2357`, `fixedbugs-2455`,
     `fixedbugs-2458`, `package-one-index` and `the-platform-layer-is-reached-from-a-fetched` (its 9);
     `unsupported` by `fixedbugs-2351`, `fixedbugs-2357` and `fixedbugs-2364` (its 8); `full` by
     `fixedbugs-2373-a-fetched` and `fixedbugs-2431-a-repair-in-a-placed` (its 2); the `fetch` suite
     holds its 11; the three `surface-fixtures` folders holding one (`path2357`, `standin2357`,
     `tree607`) are the `surface` and `probe` suites' (`.claude/rules/verification.md`'s map, its
     `surface-fixtures` row), run by name only if your route moves them; and the compiler's own tests;
   - **a case of your own that lists the files placed**: the `fetch` form prints placed folders' names
     alone (`suite_fetch.hero:452-456`; 44 of its 77 transcripts print `(none)` and 33 a folder name), so
     no golden pins which files a fetch places today;
   - whether any emitted C moves for a program whose reached modules are the same: `cmp` of `--emit-c`
     from the whole and the pruned tree of `ini` and `server`, the line `hero_program_sources` taken out
     (`.claude/rules/verification.md` § The formatter's probe), and with it in.

   **Say which golden moved and whether its new text is the route's or a defect.**
5. **The falsifier** (`.claude/rules/cli-surface.md:68-74`): run each of its edits against your route
   (catalogue deleted, entry deleted, every field mistyped, the tree edited, a fetched copy left behind)
   and the edits your route adds (a placed file removed, a placed file no reached module needs, a file
   added under the tree, the listing edited or moved, a `use` line added or removed after the fetch):
   for each, *which module compiles* and *whether the program builds*, with the exit and the message.
   One program that compiles another module after such an edit returns the admission to a sitting: say
   so in those words if you find one.
6. **R2's ruler, measured over 0.5.0** (`docs/panel/242-*.md:140-152`; 0.4.0 is gone from the base): in
   your tree the compiler takes `core` 0.5.0 as panel 242's route (A) took 0.2.0 (`heroes add core
   --index file://../packages -C selfhost`, carried from `241-*.md:58-62`; re-run it), the live copies of
   each face's contract it can delete deleted, then the tree counted under W and under your route **in
   every unit the ruler could mean**: lines (`wc -l`), the `layout` suite's code lines, bytes, files, each
   with and without the cards, a placed listing and the compiler's catalogue. **R2 states no unit**:
   panel 242 counted lines of Heroes, the author was given every file's lines (00-shared.md § R2's
   ruler). **Panel 242's own measurement replaces any arithmetic**: route (A) shrank the compiler's
   modules by 14 lines, not 186 (`242-reports/compiler-engineer.md:107-109`), and route (C), exactly the
   reached `strings` files at `selfhost/`'s root, grew the tree by 215 (`:287-296`); both are counts of a
   tree that has moved 235 files since (`git diff --shortstat 1edabb957 48f53b343 -- selfhost`), so
   re-measure them here. **Which *package* R2's *"every live copy of that package's contract"* means** is
   open: count both the entry's (every face of `core`: `strings`, `strconv`, `encoding/json`, `net`,
   `slices`, with `git ls-files selfhost | grep -i json`'s 7 files and every other copy each body read,
   never the name) and the reached Go package's. **Say whether the tree shrinks under any route, in which
   unit, and by which deletions.** Stage 2 and stage 3 with their fixpoint (`cmp` of the two emissions) on
   that tree; the `#line`s naming the placed folder (13 at 0.2.0, carried) counted under each route; and
   `heroes check selfhost/main.hero`'s instructions retired (`/usr/bin/time -l`), five runs each, base
   against the route, never seconds.
7. **The routes nobody listed in the first briefs** (00-shared.md's nine), from the compiler's side, each
   built far enough to say or said why not: **2**, W kept and R2 met by its defect branch: a differential
   run of each live copy in `selfhost/` against `core`'s function on the inputs its callers reach (panel
   242 found none crashing or wrong at `1edabb957`, *"0 `.slice(` calls among them"*, `242-*.md:56-58`);
   **3**, M for the compiler alone (what it costs the compiler, against panel 242's R3, *"no special
   case"*); **5**, a key over the reached set (what moves in the `#line`s and the seed on each change of
   the compiler's `use <core>` set; F3 moves the key the same way, since `placed.hero:128-134` hashes the
   `sum`); **9**, the compiler reading `core` from `packages/core/<v>/files/` (what R15's *"never `path`
   entries"* and R6's unverified `path` make of it); and **R2 counted by the modules reached** (what the
   compiler would count for `core` 0.5.0 under that ruler, and by which instrument: the provenance key's
   rows, `#line`s, a new one). **G and M** place the same `.hero` files for every program here, and **H**
   is M with the cards (00-shared.md § The routes, `ls` of each `internal/` folder): name the shape that
   parts them in a later version instead of building three. **The split** (`core` as entries of its
   own): what moves in `selfhost/module/` and `selfhost/fetch/`, if anything, and in the 18 catalogues.
8. **The cost of a build**: instructions retired by `heroes check main.hero` in `examples/ini` and
   `examples/json`, three runs each, the whole tree at the base against your route's pruned tree; and
   what the per-build check reads under F1 against W.
9. **What 2315 replaces**: its notes still name the form panel 241's R3 withdrew (*"by packages copied
   into the compiler's tree under `selfhost/core/`"*, `gh issue view 2315 --json body`, its line 22), and
   panel 242's R7 names `crypto/sha256` as a candidate, which no version of `core` holds (`git ls-tree -r
   --name-only 48f53b343 packages/core | grep -c crypto`: 0). Say which of the package manager's copies
   0.5.0 could replace under your route, by count.

## What your verdict owes

The route you would adopt (W, M, G, H, one of the nine, or another you name; F1 to F4 or another), built
or not; its lines per file; what moves (the compiler's own tests, the `fetch` and `packages` suites, the
golden cases named above, the 55 hand-placed trees, the seed); what it refuses that builds today (the
measured `use <core>/strconv` edit is one) and what it builds that is refused today; its cost in
instructions; **whether R2 is met under it for `core` 0.5.0, measured, in the unit you name**; whether it
needs a spec sentence (panel 244's R1 ruled `heroes fetch` and `heroes.packages/` out of the spec), a
change to a program's catalogue, a widened refusal or a changed message (00-shared.md's escape: say so in
those words); and a veto, if you cast one, names the program that compiles another module, or corrupts
the tree in silence, under the route vetoed, built and run.
