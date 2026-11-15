# Panel 250, compiler-engineer

Read `00-shared.md` first, whole: the base, the words (*entry*, *module*, *face*, *Go package*,
*placed*, *reached*), the routes (W, M, G, H; F1 to F4; the alternatives), the rulings quoted from the
base, the rules (**your binaries are named `ce-...`**, every program that may not end is bounded there)
and the platforms (**the Windows box is off**). You judge from the compiler's side, with a veto on
soundness. Your folder is `.claude/worktrees/p250-scratch/250-compiler-engineer/`, your tree `tree/` in
it, a `git archive` of **`722ae76bf`** (tree `ce81c0b18b1fbeb616f95aeef619d5b8cd23bfbe`), your compiler
built there from the seed in two stages (`sources 2ab7ca0a644692a1`). Your report is
`docs/panel/250-reports/compiler-engineer.md` in `.claude/worktrees/p250`, in your definition's output
structure (`.claude/agents/compiler-engineer.md`, from its line 36), written as you go. **The route you
recommend, you build in your tree and run on the cases you name**; a route that does not build is
reported as not built, never adopted (`/panel` 3c). Where a module's length decides anything, ask
`tests/harness/suite_layout.hero`'s unit, not `wc -l` (`.claude/rules/module-shape.md`).

## Where the code is (`wc -l` at the base)

- **The build's reading**: `selfhost/module/packages.hero` 416 (`rooted` 142, `problem_of` 169,
  `placed_said` 215, `changed_said` 249), `fingerprint.hero` 244 (`of` 32, `added` 95),
  `placed.hero` 239 (`key` 118), `placed_beside.hero` 378, `standing.hero` 105, `unplaced.hero` 111,
  `offered_modules.hero` 117, `catalogue.hero` 486, `sums.hero` 99, `reached.hero` 40.
- **The verbs**: `selfhost/fetch/`, 27 files, 6,054 lines (`wc -l selfhost/fetch/*.hero | tail -1`);
  among them `placing.hero` 71, `index_source.hero` 475, `git_source.hero` 398, `staging.hero` 448,
  `edited.hero` 128, `leftover.hero` 301, `add.hero` 461, `fetch_all.hero` 276, `verb_place.hero` 119,
  `build_list.hero` 229, `listing_read.hero` 92, `tree_names.hero` 173.
- **The C side of a fetched unit**: `selfhost/cli/package_units.hero` 353 and `package_reads.hero` 454 (panel
  235's C1 and C2, the `-M` listing the build already keeps, their opening comments).
- **What a program was compiled from**: `selfhost/emit/provenance.hero`, *"`module <name> <key>` for
  every module the root reaches"* (its lines 8-9), and its mirror `.claude/hooks/provenance.py` (231
  lines), which does not follow `use <name>/…` at the base (panel 241's compiler-engineer patched it,
  `241-*.md:79-81`, *"`provenance.py` +60 −5"*, carried; read it before you lean on it).
- **The suites that read placed trees**: `tests/harness/suite_packages.hero` 659 (its `placed`,
  `reach`, `ran` checks, 33-43), `suite_fetch.hero` 533, `placed_trees.hero` 80.

## What you measure, in this order

1. **The base's numbers, again** (00-shared.md § The numbers): copy
   `.claude/worktrees/p250-scratch/briefs/reach/closure.sh` and `programs.sh` into your folder and run
   them on your tree; then **ask the compiler**, for every program with a catalogue (16 under `examples/`
   and `tests/harness`), which placed files it reaches: the `#line` paths of `--emit-c`, and where a
   module declares only records, whatever else the compiler can be made to say (its discovery's list,
   `--dump-scopes`, or a debug print in your copy). A program whose compiled set differs from the
   textual closure is a finding.
2. **Today's refusal of a pruned tree**, re-run on `examples/ini` (00-shared.md: `check` exit 1
   `package_unread`, `fetch` exit 1 `sum_mismatch`), and on `examples/server`.
3. **Build route M with fingerprint F1 in your tree**, or the route you would adopt instead with your
   reasons: `heroes fetch` and `heroes add` placing the reached closure (say from which roots, a folder
   holding several, and what the verb does with a `use` added or removed since the last fetch); the
   listing placed beside the files; `placed_said` holding the listing to `sum` and each placed file to
   its line; what `fingerprint.added`, `staging`, `edited`, `placed_beside`, `standing` and `leftover`
   become; what `offered_modules` offers for a module the version holds and the program has not placed;
   and the message a build gives a `use <core>/net` written after the fetch. Count each module's lines
   moved, deleted and added. A git source: no `list` is published, so say what the verb computes and
   writes (R6's sum is over the committed blobs, `235-*.md:612-633`).
4. **Run it**: every program of `examples/` and `tests/harness` re-placed under your route (fresh
   `heroes fetch` into a copy), each one's `check` and its `run` case; the `fetch` suite and the
   `packages` suite whole (`./heroes-next run tests/harness/main.hero -- ./heroes-next fetch`, then
   `packages`); the 55 golden cases that hold a placed tree (`git ls-files tests/golden | grep
   '/heroes.packages/'`), each form narrowed with the harness's third word, one run per substring: `check`
   by `package-`, `fixedbugs-2440`, `fixedbugs-2443`, `fixedbugs-2446`, `fixedbugs-2455`, `fixedbugs-2459`
   and `fixedbugs-2506` (its 22); `run` by `fixedbugs-2351`, `fixedbugs-2357`, `fixedbugs-2455`,
   `fixedbugs-2458`, `package-one-index` and `the-platform-layer-is-reached-from-a-fetched` (its 9);
   `unsupported` by `fixedbugs-2351`, `fixedbugs-2357` and `fixedbugs-2364` (its 8); `full` by
   `fixedbugs-2373-a-fetched` and `fixedbugs-2431-a-repair-in-a-placed` (its 2); the `fetch` suite holds
   its 11; the three `surface-fixtures` folders holding one (`path2357`, `standin2357`, `tree607`) are
   the `surface` and `probe` suites' (`.claude/rules/verification.md`'s map, its `surface-fixtures`
   row), run by name only if your route moves them; and the compiler's own tests. **Say which golden
   moved and whether its new text is the route's or a defect.** Whether any emitted C moves for a program whose reached modules
   are the same: `cmp` of `--emit-c` from the whole and the pruned tree of `ini` and `server`, the line
   `hero_program_sources` taken out (`.claude/rules/verification.md` § The formatter's probe), and
   with it in.
5. **The falsifier** (`.claude/rules/cli-surface.md:68-74`): run each of its edits against your route
   (catalogue deleted, entry deleted, every field mistyped, the tree edited, a fetched copy left behind)
   and the edits your route adds (a placed file removed, a placed file no reached module needs, a file
   added under the tree, the listing edited, a `use` line added or removed after the fetch): for each,
   *which module compiles* and *whether the program builds*, with the exit and the message. One program
   that compiles another module after such an edit returns the admission to a sitting: say so in those
   words if you find one.
6. **R2's ruler, measured** (`docs/panel/242-*.md:140-152`): in your tree, the compiler takes `core`
   0.4.0 as panel 242's route (A) took 0.2.0 (`heroes add core --index file://../packages -C selfhost`,
   carried from `241-*.md:58-62`; re-run it), the live copies of each face's contract it can delete
   deleted (`cli/strings`' copies of `strings`; and say whether any of `strconv`'s or
   `encoding/json`'s contract has a live copy in `selfhost/`, the 7 JSON files `git ls-files selfhost |
   grep -i json` lists, each body read and never the name), then the tree counted three ways under W
   and under your route: lines (`wc -l`), the `layout` suite's code lines, files. **Say whether the tree
   shrinks under any route**, and by which deletions; 00-shared.md's arithmetic (368 reached lines
   against 186 deleted, carried) is yours to replace with a measurement. Stage 2 and stage 3 with their
   fixpoint (`cmp` of the two emissions) on that tree; the `#line`s naming the placed folder (13 at 0.2.0,
   carried) counted under each route; and `heroes check selfhost/main.hero`'s instructions retired
   (`/usr/bin/time -l`), five runs each, base against the route, never seconds.
7. **The alternatives from the compiler's side**: R2 counted by the modules reached (what the compiler
   would count for `core` 0.4.0 under that ruler, measured on your step 6 tree, and what instrument would
   count it: the provenance key's rows, `#line`s, a new one); `core` split into entries of its own (what
   moves in `selfhost/module/` and `selfhost/fetch/`, if anything, and in the 17 catalogues; built far
   enough to say, or said why not); and route G against M: by 00-shared.md's table every reached face's whole `internal/`
   subtree is reached in 0.4.0 (`encoding/json/internal/` 7 of 7 files, `net/http/internal/` 4 of 4,
   `net/internal/`, `strconv/internal/` and `strings/internal/` 1 of 1 each, `ls` of each folder against
   the closure), so G and M place the same `.hero` files for every program here: say whether that
   holds, and what would part them in a later version.
8. **The cost of a build**: instructions retired by `heroes check main.hero` in `examples/ini` and
   `examples/json`, three runs each, the whole tree at the base against your route's pruned tree; and
   what the per-build check reads under F1 against W.

## What your verdict owes

The route you would adopt (W, M, G, H, or another you name; F1 to F4 or another), built or not; its
lines per file; what moves (the compiler's own tests, the `fetch` and `packages` suites, the golden
cases named above, the 55 that hold placed trees where they move, the seed); what it refuses that
builds today and what it builds that is refused today; its cost in instructions; **whether R2 is met
under it for `core` 0.4.0, measured**; whether it needs a spec sentence, a change to a program's
catalogue, or a widened diagnostic (00-shared.md's escape: say so in those words); and a veto, if you
cast one, names the program that compiles another module, or corrupts the tree in silence, under the
route vetoed, built and run.
