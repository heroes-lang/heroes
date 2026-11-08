# Panel 239, the compiler-engineer's brief

Read `00-shared.md` and `proposal.md` beside this file first. Your worktree is
`.claude/worktrees/239-compiler-engineer/`, detached at the sitting's briefs commit on `p239`; build
your compiler there as `00-shared.md` says. You judge implementation cost and core against sugar
(design.md §1.1, §1.7, Part 5), with veto power; cite files and line counts. **Your build is 2364's
route**; the ffi-pragmatist builds 2363's refusal, and you price where it sits in the compiler.

## Where the code lives (`wc -l` at `0a62bb10c`)

- `selfhost/emit/externs.hero` (349): `headers_for` (129-161), the unit's own groups `sort(mine)`
  then `sort(others)` (line 158); `libraries` and `named` (207-257), the link names in the order
  `p.functions` and then the record-only groups give them.
- `selfhost/emit/unit_plan.hero` (200): `plan` (62-146), which builds `records` (the group records a
  unit spells) and asks `headers_for` (line 144).
- `selfhost/emit/extern_field.hero` (404), `emit/ffi_field.hero` (554), `emit/ffi_layout.hero` (246):
  the per-unit field assertions and how their failures are read into `ffi_field_type`.
- `selfhost/cli/layout.hero` (586), `check_records` (line 90), and `cli/layout_runs.hero`: the
  layout screen and dump, per unit, under `build/layout-<key>/`.
- `selfhost/cli/header_order.hero` (342) and `cli/order_trial.hero` (179): panel 204's order search,
  `search` (line 219), `opening` and `compiled`.
- `selfhost/cli/one_file.hero` (261): panel 202's R3, which already compares a binding's meaning in
  a module's own unit against the fused one.
- `selfhost/cli/link.hero` (688): `link_line` (151-168), `linked_into` (172); `cli/link_warning.hero`
  (163), `run_link` (37); `cli/link_flags.hero` (192); `cli/renamed.hero` (216) and `cli/whose.hero`
  (340), the link's failure path.

## What the sitting needs from you, each measured

1. **2364, the routes of question 4, built**: prototype in a copy of `selfhost/` under
   `build/239/` the route you judge most robust, and at least one other where it is cheaper to
   build than to argue (the order search A, the cross-unit layout check B, the owner's prefix C,
   isolation D). Run every program of `p239-scratch/repro/two_others/`, `repro/s2364/` and the
   critic's `repro/critic/s2364x/pragma/` and `partial/` with it on this Mac and in the Linux arm64
   container: what each prints, and the message where it refuses. **`pack/pk_first`,
   `pragma/pk_first` and `partial/pw_first` must not build silently** under a route you adopt; say
   which route sees each and how (a field-type assertion does not, and a comparison of tokens or
   macros cannot see `pragma`). Read first what the critic found already computed: each unit's
   layout of each record meets in one program-wide list (`cli/probing.hero:236-251`,
   `emit/lend_type.hero:61-70`, last unit wins, read by `emit/padding.hero:171`), which is route B's
   data at no clang cost, and a question of its own (below). A route that does not build is not
   adopted (the `/panel` skill, 3c).
2. **Its cost**, in instructions retired: a warm `heroes build` of a program whose units spell other
   modules' records and build (`rs_first`), of the failure path (`both`), and of `selfhost/main.hero`
   (whose 28 groups in 22 modules name three headers and no library: `git grep -h '^extern' -- selfhost/ | sort | uniq -c` and `git grep -l '^extern' -- selfhost/ | wc -l`), each
   with and without your change, the first run discarded. And what moves in a cache key, so `cache`
   is owed alone or not.
3. **The tree**: which tracked programs' verdicts or emissions the route moves: the `run` goldens
   and `examples/` programs that reach two or more modules with groups (counted at this commit; panel
   238's ffi-pragmatist's item 4 gives its method, `check --json`'s `read` list), and
   `tests/emission/`.
4. **R2's walk**: apply panel 238's `prototype.diff` (in `docs/panel/238-reports/evidence.tar.xz`;
   it applies to `0a62bb10c`, `00-shared.md`) to a copy with and without your route, and say whether
   your route's verdicts and emitted bytes are free of the written order under each, so whether it
   must land before R2, with it, or stands alone.
5. **2363 in the compiler**: where a refusal read from the link would sit (the successful link's
   path in `cli/link.hero`, a second link, a probe before it), what it reads, how it reaches the
   binding's line for the message (the declarations' spans the link names come from,
   `externs.named`), its cost on a warm build of a program with groups and of one with none, how it
   meets 2365's failure path (the critic's `one_written` reaches it by a `multiple definition` on GNU
   ld), and how its lines are kept off the author's screen: `run_link` (`cli/link_warning.hero:37-56`)
   prints every stderr line of the link but glibc's section warnings, GNU ld writes its trace there and
   lld on stdout. The ffi-pragmatist builds the refusal; coordinate through your
   reports only, never through each other's worktrees.
6. **The cases each repair owes** and in which form (`.claude/rules/verification.md` § What gates
   what), given that no golden case builds a C library (`00-shared.md`): a `run` or `unsupported`
   golden, a `.case` folder, the net's own tests, `special`; and on which platform each runs.
7. **The shapes beside, depth one, of your route**: a record reached through another record's field
   (a nested group record), a record a unit spells only through a pointer (`ptr` to it), a `partial`
   record, a union, a record of a `package` group, `heroes test`'s units and `--emit-c`'s one file;
   and the padding table above: a record whose membership another module's macro changes (`partial`),
   lent through `@` in one unit, whose padding writer may take the other unit's membership and write
   zero over a member (read in the code by the critic, unrun).
   A real defect of another cause goes in your report with its reproducer, for the coordinator to
   file.

Your report: `docs/panel/239-reports/compiler-engineer.md` in your worktree, written as you go,
ending as `00-shared.md` § Rules says, and with **the landing your route owes, file by file**.
