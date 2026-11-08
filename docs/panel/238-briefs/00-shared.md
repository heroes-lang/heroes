# Panel 238, what every seat reads first

A **`retro-record`** sitting on the author's instructions of 2026-11-08 (`proposal.md` beside this
file): `heroes fmt` groups the `use` lines, the bracketed ones first, each group in alphabetical
order; and the repository's static index moves to a `packages/` folder at the root. The decision is
the author's. The sitting records the real objections, measures what it costs, settles what the
author left open, and says where each rule lands; nothing lands in the compiler here.

**Repaired on the critic's first pass** (`238-reports/critic-briefs.md`, 29 findings, nine of which
changed a brief), before any seat started. The largest: the author's premise, *the order does not
matter*, is already measured false in four outputs (F4, F19, F20, F21), and the C boundary is
touched (F21), so the ffi-pragmatist sits.

**Seats**: the compiler-engineer (prototype; questions A, B, C and D), the ffi-pragmatist (the C
side of the order, F21 and F22), the spec-warden (whether the specification moves, question C, on
the vendored tables only), the historian (question E), and the completeness critic over these briefs
before any seat started and over the reports after. **No llm-ergonomist**: its reading is a paid
`claude -p` session, and this sitting runs none; the run that would measure the reader-facing half
(F24) is sized in the synthesis for the author to decide.

## The tree, frozen

- Every seat measures lane `p238` at the commit that carries these repaired briefs, whose ancestor
  is **`bd4670da4`** (batch 25's round plus a ledger commit). The sitting's own commits touch
  `docs/panel/238-*` alone.
- Each seat works in **its own detached worktree**, `.claude/worktrees/238-<seat>/`, made by the
  coordinator at that commit, and nowhere else: never in `p238`, the trunk, a lane or another seat's
  worktree. (The `/panel` skill names a scratchpad copy; CLAUDE.md forbids a file outside the
  repository's root, so the worktrees stand under `.claude/worktrees/` as panels 235 to 237 did.)
- **Build your own compiler in two stages** in your worktree: `clang -I runtime seed/heroes.c
  runtime/runtime.c -o heroes`, then, **in its own command**, `./heroes build selfhost/main.hero -o
  heroes-next`; judge with `./heroes-next`. The seed alone does not know `use <name>/` nor `heroes
  add`/`fetch` (defect 2352; the critic, finding 20: the seed's build refuses
  `examples/adventure/main.hero:22:1`, `use <core>/strings`, `expected_module_name`, and has no `fetch`
  or `add` verb). Measured: the critic's stage 2 `158.97 real 117.68 user`, `746955029902
  instructions retired`; `./heroes-next --version`'s second line reads **`sources
  62dba14251e6eafb`** (the coordinator's build in `p238` and the critic's), the seed's own build
  `sources 1a1e340778db789a`.
- The machine is shared with other lanes. **A cost is measured in instructions** (`/usr/bin/time
  -l`, *instructions retired*), never in seconds; a count of files or lines is a count.
- Your evidence (programs, scripts, outputs, a prototype's diff) goes under `build/238/` in your
  worktree (`build/` is git-ignored), so the coordinator can archive it at the synthesis. The
  critic's programs are under `.claude/worktrees/238-critic/build/238/` (`probe1/`, `cside/`,
  `c453/`, `c560/`, `spell/`, `census.txt`); read them, copy them into your own `build/`, never run
  inside another seat's worktree.

## What was measured while this brief was written (each with its command)

- **F1, `fmt` keeps the source order today.** In `.claude/worktrees/p238-scratch/probe1/`, three
  modules `zeta.hero`, `alpha.hero`, `lib/beta.hero` and a `main.hero` opening `use zeta` / `use
  lib/beta` / `use alpha`: `heroes-next fmt main.hero` prints the three lines in that order, exit 0,
  and `heroes-next run main.hero` prints `6` (the critic, finding 2, reproduced).
- **F2, the policy that says so.** `selfhost/print/fmt.hero:109-114`, above `format_file`: *"SOURCE
  ORDER, never a canonical order: reordering is where a formatter starts deleting the author's
  remarks"*, then, after a dash, *"gofmt sorts imports and can afford to, it has a comment model
  this one does not."* Panel 049 (`docs/panel/049-the-platform-axis.md:175-182`) quotes the Rust
  bootstrap's rule, *"Source order, not a canonical order"*, against a formatter that would hoist
  `link` lines in an `extern` block, where *"link order **matters** to a linker, reproduced"*: the
  record's own precedent of a reorder that changes a program. The author's instruction reverses
  this policy for `use` lines; the sitting says what makes it safe now, or what it costs.
- **F3, the grammar puts no `use` first.** `spec/heroes-spec.md:57-58`: `File = { NEWLINE } { Use |
  Declaration } .` and `Use = "use" [ "<" ident ">" "/" ] ident { "/" ident } [ "as" ident ] NEWLINE
  .` A `use` after a declaration is accepted: `late.hero` in the same folder (`use zeta`, then
  `function main()` naming `alpha.one()`, then `use alpha` last) runs, prints `3`, and `fmt` keeps
  `use alpha` at the bottom. The tree holds such lines: `selfhost/cli/response_file.hero:144-145`,
  `use cli/strings` and `use cli/toolchain` under its `## Tests` heading (line 142), below
  declarations.
- **F4, `fmt` formats every file the PARSER accepts, and the checker's verdict moves with the
  order** (the critic, finding 5). `selfhost/print/guard.hero:71-79`, `checked`, refuses on the
  parser's diagnostics alone, and `fmt` reads the root file alone (`selfhost/cli/input.hero:64`,
  `read_root_only`), so it never sees `used_twice` or any checker diagnostic: `fmt twice.hero` (`use
  alpha` / `use alpha as a2`) prints the file, exit 0, while `run` exits 1, `error[used_twice]` at
  `twice.hero:2:5`. And on `clash_a.hero` (`use zeta` / `use lib/beta as zeta`) `check` gives one
  error, `module_binding_taken` at 2:17 with `fix (guess): write `use lib/beta as lib_beta``; on
  `clash_b.hero`, the two lines swapped, two errors, the first at 2:5 with no fix, then
  `unknown_in_module`. A whole-line sort turns the first file into the second.
- **F5, the census, a shell approximation and not the formatter.** Over `git ls-files '*.hero' |
  grep -v '^docs/'` (3,564 files, two of them empty and so unseen by `awk`; `docs/` holds 1,092 more,
  the records of earlier sittings, which nobody reformats), one `awk` reading each file's column-0
  `use` lines: *late*, a `use` after a line that is neither blank, a comment nor a `use`; *split*,
  two `use` lines with a blank or a comment line between; *unsorted*, the lines not ascending by
  (bracketed first, then the whole line in byte order):

  ```sh
  git ls-files '*.hero' | grep -v '^docs/' | LC_ALL=C xargs awk '
    FNR==1 { if (NR>1) emit(); F=FILENAME; n=0; late=0; sp=0; uns=0; br=0; gap=0; decl=0; prev="" }
    /^use / { n++; g = ($0 ~ /^use </) ? 0 : 1; br += (g==0); key = g "" $0
              if (decl) late=1; if (gap && n>1) sp=1; if (n>1 && key < prev) uns=1
              prev=key; gap=0; next }
    /^[[:space:]]*$/ || /^[[:space:]]*#/ { if (n>0) gap=1; next }
    { decl=1; gap=0 }
    function emit() { print F, n, late, sp, uns, br }
    END { emit() }'
  ```

  | directory | files | with a `use` | unsorted | late | split |
  |---|---|---|---|---|---|
  | `selfhost` | 749 | 698 | 585 | 1 | 3 |
  | `tests/harness` | 148 | 139 | 25 | 0 | 0 |
  | `examples` (programs) | 121 | 52 | 4 | 0 | 1 |
  | `examples/*/heroes.packages` (placed trees) | 56 | 28 | 0 | 0 | 0 |
  | `examples/index` | 4 | 2 | 0 | 0 | 0 |
  | `tests/golden/check` | 782 | 37 | 7 | 3 | 4 |
  | `tests/golden/run` | 722 | 31 | 5 | 0 | 0 |
  | `tests/golden/surface-fixtures` | 322 | 62 | 5 | 0 | 0 |
  | `tests/golden/unsupported` | 391 | 17 | 4 | 0 | 0 |
  | `tests/golden/fetch`, `fixedbugs` | 14, 42 | 1, 1 | 0 | 0 | 0 |
  | `control` 29, `emit` 13, `full` 53, `ir` 29, `lldb` 15, `permissive` 22, `tests/thesis` 50 | | 0 | 0 | 0 | 0 |

  In all 1,068 files hold 6,836 `use` lines; 635 are unsorted, 4 late, 8 split. The four unsorted
  files of `examples/` are `examples/interpreter/main.hero`, `examples/ledger/main.hero`,
  `examples/ledger/report/lines.hero` and `examples/shapes/main.hero`. **Ten files hold both
  groups** (a blank line between them would add a line): `examples/adventure/game.hero`,
  `adventure/main.hero`, `ledger/main.hero`, `ledger/report/lines.hero`, `markdown/blocks.hero`,
  `query/data/table.hero`, `query/fmt/report.hero`, `todo/main.hero`,
  `tests/golden/check/use-a-package-mark-told.hero`, `tests/golden/surface-fixtures/mark235/main.hero`
  (`grep -c '^use <'` and `grep -c '^use [^<]'` per file, both above 0). In the trees `canonical`
  walks no `use` line carries a trailing comment; the ten that do are `#~` marks in
  `tests/golden/check/` (the critic, finding 26, `xargs grep -n '^use .*#'`).
- **F6, the split shapes in `selfhost/`, and where they come from.** `selfhost/emit.hero:1` and
  `selfhost/emit/members.hero:1` are `use emit/large_size`, then the file's header comment, then the
  rest of the block (from line 15 and 17); both entered in one commit, `82a9dc630` (defect 2278,
  2026-11-01), the shape `missing_use`'s fix writes (F9): its `guess` *add `use alpha` at the top of
  the file* inserts at offset 0, above a `# header` (the critic, finding 7, `missing.hero`).
- **F7, byte order puts the brackets first by itself.** `<` is 0x3C: above the digits (0x30 to
  0x39), below every letter (0x41 up) and `_` (0x5F); `/` is 0x2F, below all three. `printf` of the
  lines through `LC_ALL=C sort` gives `use <core>/strings`, `use Zeta`, `use _x`, `use alpha` in
  that order; `use <core2>/x` before `use <core>/y` before `use <core_b>/z` (`2` is 0x32, `>` is
  0x3E), where a part-by-part order puts `core` first; and `use a/b`, `use a0`, `use a_b`, `use
  ab`. **A package name** is lowercase letters, digits and `_`, opening with a letter
  (`selfhost/fetch/add.hero:143`, `selfhost/module/catalogue.hero:282`); **a module part** may open
  with an uppercase letter or `_` (the critic's `cases.hero`, `use Upper` / `use _9x` / `use alpha`,
  ran, exit 0; the tree holds `use _0opt` in
  `tests/golden/run/fixedbugs-2327-a-module-led-by-an-underscore-and-a-digit.case/main.hero:8`).
- **F8, the guard compares trees and comments.** `selfhost/print/guard.hero:12-31`: `fmt`'s output
  must parse, be a fixpoint, hold the same tree, `dump(text) == dump(fmt(text))`, and keep each
  comment among the same tokens (`print/anchors.hero`); every failure exits 2. A reorder changes the
  dump's order of `use` lines and the tokens around a moved comment. Spec section 1, lines 49-50:
  *"A comment directly above a declaration documents it; `##` is a section heading."* A late `use`
  standing between a comment and a declaration, hoisted, leaves the comment above the declaration,
  which it then documents (the critic, finding 9); a hoist out of `response_file.hero`'s `## Tests`
  takes two lines out of that section.
- **F9, the fixes that write a `use` line.** `selfhost/resolve/missing_use.hero:66`, *add `use
  <module>` at the top of the file*, `guess`, which `check --apply` never applies (`--apply` takes
  `certain` fixes only; the critic's `check missing.hero --apply` printed the file unchanged). **The
  `certain` one is `selfhost/parse/use_line.hero:273`**, `use_wants_a_name` (defect 2308): on
  `quoted.hero` (`use "zeta"` / `use alpha`) `check --apply` prints `use zeta` / `use alpha`, exit 0,
  not canonical under a sorted rule. A third is F4's `module_binding_taken` guess, which changes an
  alias. `git grep -n 'use print/fmt\|fmt\.format\|canonical'` over `selfhost/cli/check.hero`,
  `certain.hero` and `consensus.hero` prints nothing: `--apply` calls no `fmt` (the critic,
  finding 10).
- **F10, what holds the tree canonical.** `tests/harness/suite_canonical.hero:69-79` walks
  `selfhost`, `tests/harness`, `examples`, `tests/golden/run`, `ir`, `emit`, `fixedbugs` and `lldb`,
  and exempts `check`, `unsupported` and `permissive` with reasons at lines 22-45 (their `.expected`
  files cite lines and columns; `UPDATE_GOLDEN=1` does not exist; *the text is the fixture*).
  `.claude/hooks/fmt_check.py:323-351` asks `fmt` of every `.hero` written in a tree and refuses one
  not canonical (349), a marked golden judged by its marks instead; the commit refusal is
  `.claude/hooks/staged.py:98-181`, which `guard_bash.py:332` imports.
- **F11, what a change to `selfhost/print/` owes after the push.** `.claude/rules/verification.md`
  § The formatter's probe: `heroes probe` by hand over `selfhost`, `tests` and `examples`, families
  `single` and `bracket`, which that file carries as about 15 cool CPU hours (panel 229's seats, an
  inference there, carried here and not re-measured); and the walk of
  `.claude/rules/diagnostics-and-goldens.md` § A new surface form lands in every tool.
- **F12, the index.** `git ls-files examples/index` lists 8 files: `core/versions`,
  `core/0.1.0/list` and six under `core/0.1.0/files/` (4 `.hero`, 2 `.md`). 14 catalogues read it
  (`git grep -c 'index = "file://../index"' -- 'examples/*/heroes.toml'`). Every program that reads
  it carries its placed tree: 84 tracked files under 14 folders
  `examples/<program>/heroes.packages/core@0.1.0-db7f072b4678b0b1/` (`git ls-files | grep
  '/heroes.packages/'`, whose other 17 are 14 in seven `tests/golden/check/package-*.case` folders
  and 3 in `tests/golden/run/package-one-index-written-two-ways.case/`).
- **F13, the placed folder's name holds the index address, as written.**
  `selfhost/module/placed.hero:74-80`: the key is `name@version-` and the first 16 hex digits of the
  SHA-256 of `"index\n" + index + "\n" + name + "\n" + version + "\n" + sum + "\n"`, *every field
  that names the package, so a field mistyped names a tree that is not there*. `printf
  'index\nfile://../index\ncore\n0.1.0\nsha256-7324...c9998\n' | shasum -a 256` gives
  `db7f072b4678b0b1`, the folders' name; with `file://../../packages`, `2e3e1bba9c8acea1`. Any
  respelling of the line renames every placed folder (the critic, finding 14: `file://./../idx`
  gives other keys than `file://../idx`).
- **F14, a relative `file://` is the program folder's.** `selfhost/fetch/address.hero:1-8, 17-23`:
  `file://` names only a path inside the program's own repository, the one git answers; a relative
  path is written `file://./x` or `file://../x`; `file://host/x` is refused. A plain `use` cannot
  reach an index's sources at a root `packages/`: `use packages/core/0.1.0/files/strings` is refused
  `module_path_wants_a_part` at the version's dot, and a placed folder at its `@` (the critic,
  finding 15, `build/238/spell/`), which keeps design.md's *a `use` without brackets never reaches
  fetched code* by the index's layout, not by its folder's name.
- **F15, every tracked path that names `examples/index`**, outside `docs/panel/` (`git grep -l
  'examples/index' -- . | grep -v '^docs/panel/'`, 14 files): `.gitattributes:46` (`-text`, so a
  CRLF checkout keeps the bytes the sums name); `docs/records/journal/075-package-manager.md` (a
  record, never rewritten); `docs/roadmap/milestones/M-core-packages.md:402`;
  `examples/packages/README.md:13`; `site/src/lib/examples-model.ts:48` (`const INDEX =
  'examples/index'`, used at 270-274 and 407); `tests/harness/package_rules.hero:24, 628`;
  `tests/harness/placed_trees.hero:62`; `tests/harness/suite_packages.hero:8, 623, 629`;
  `tests/harness/suite_records.hero:1858, 1864`; `tests/harness/suite_unseen.hero:400`. Four more hits
  are the site's `/examples/index.json` and `examples/index.html`, another thing.
  `examples/README.md:119-120` names it as `index/` and `file://../index`. **And eleven harness
  modules walk it through the root `"examples"` without naming it** (the critic, finding 16):
  `absence.hero:1881`, `cards.hero:99`, `platform_skips.hero:206`, `suite_canonical.hero:73`,
  `suite_compat.hero:70`, `suite_corpus.hero:58`, `suite_emission.hero:111`,
  `suite_packages.hero:80`, `suite_probe.hero:191`, `suite_records.hero:2950`,
  `suite_warnings.hero:54`; `probe` over `examples/index` reads 4 seeds and 36 variants, all held;
  `compat` archives `examples/` alone from a release tag (`suite_compat.hero:339-358`); `AREAS` in
  `cards.hero` has no area for a new top-level folder. The instrument for this list is
  `.claude/rules/verification.md` § The map's command, which reads the paths from git.
- **F16, the specification is silent on the formatter, not on comments.** `grep -n -i
  'fmt\|format\|canonical\|sorted\|order' spec/heroes-spec.md` prints eight lines (131, 213, 216,
  279, 286, 333, 341, 376), none about `fmt` or `use` order: 131 *"Declaration order never matters
  but a group's"*, 376 *"reads a module's headers in the order its groups are written"*. Spec line
  164: *"a `use` binds its name for the whole file"*; lines 49-50 give a comment two meanings (F8).
  The site's start page (`site/src/html/start.html:110`, `it/start.html:111`) says *"`heroes fmt`
  formats code using fixed rules for indentation, line breaks and order. The project's source files,
  including the compiler, already follow those rules: the formatter returns them unchanged, byte for
  byte"*: a published claim that the landing order must keep true at every pushed commit.
- **F17, design.md's home.** §4.15 *Layout, formatting, lexing* runs from `docs/design.md:2150` to
  2257; its last bullet, line 2252, *"Canonical formatter, mandatory, `gofmt`-style: there is exactly
  one correct way to write any program"*. design.md §4.1 (lines 962-963): *a comment adjacent to a
  declaration is its documentation; a blank line between comment and declaration makes it an
  ordinary comment*, which `fmt.hero:234-237` already applies to the first `use`.
- **F18, the index's rulings and the site's paths.** Panel 235's synthesis
  (`docs/panel/235-dependencies-named-*.md`): R15 at lines 744-753 (*a local static index read over
  `file://`*), C4 at 676-684 (*`file://` only for a path inside the program's own repository root*;
  the address in the catalogue's `index` line, the Heroes index by default, R5, and on the command
  line for `add` only), R20 at 797-804 (row 78); its restated W4 (line 304) lists the falsifier's
  cases, *each field of an entry mistyped*, and the `index` line is the catalogue's and no entry's (the
  critic's reading, finding 27). `ROADMAP.md:217`, row 78, M-package-index,
  scheduled. `.github/workflows/deploy-site.yml:62-77` lists `examples/**`, `selfhost/**` and
  `docs/panel/**` among the site's paths and no `packages/**`; `site/src/lib/triggers.ts` refuses a
  build that reads a path the list does not name. A seat's worktree has no `site/node_modules`; `npm
  ci` in its `site/` makes the build runnable (free, no paid run). The root already holds `vendor/`
  (`git ls-files vendor`: five files, the tokenisers).
- **F19, diagnostics and holes follow the order** (the critic, finding 22, `build/238/cside/`).
  Two modules with one `type_mismatch` each: `check` of a root naming `use e1` / `use e2` reports
  `e1.hero:2:12` first, the swapped root `e2.hero:2:12` first. Two modules with a hole each: `check
  --json`'s files and holes list `h1` before `h2` for one order and `h2` before `h1` for the other.
- **F20, the one-file emission and the link line follow the order** (the critic, finding 21,
  `build/238/cside/` and `c453/`). Two modules with `extern "math.h" link "m"` and `extern
  "stdlib.h" link "c"`: the two roots' `--emit-c` differ by 65 diff lines beyond
  `hero_program_sources` (`#include <math.h>` and `#include <stdlib.h>` change places, and the
  `_Static_assert` checks, probes and prototypes), and the link words read `"-lm" "-lc"` against
  `"-lc" "-lm"` (`build/*/link-words.rsp`). In `c453/`, `d1.h` defining `MODE` and `d2.h` reading it:
  `written.hero` (`use b_check` / `use a_mode`) and `sorted.hero` both run `7` / `2`, but
  `written.hero --emit-c` exits 0 and **`sorted.hero --emit-c` exits 1**, `error[ffi_one_file]` at
  `b_check.hero:2:14`. design.md §4.1 (lines 936-944): `--emit-c` writes the whole program as one
  `.c`, *the seed's bootstrap*, refused at exit 1 where it cannot mean the program the units build
  (defect 453, `selfhost/cli/one_file.hero`); `compat` asks it of every release program
  (`suite_compat.hero:15`).
- **F21, an `examples/` emission moves with the order** (the critic, finding 23):
  `examples/shapes/main.hero`'s `--emit-c` matches `tests/emission/examples-shapes-main.c` (0 diff
  lines, `hero_program_sources` out); with its five `use` lines sorted (`geom/point` and `geom/area`
  change places) it differs by 50 (the function `h_geompoint_span` and its prototype move).
- **F22, three goldens whose subject is the order** (the critic, finding 25), in `tests/golden/run/`,
  which `canonical` walks, all three unsorted:
  `fixedbugs-560-a-default-another-module-defines-is-not-read-under-test.hero` (`use
  fixedbugs560/limit_half` / `use fixedbugs560/cap_ten`, *"Until 2026-10-10 `heroes test` read both
  in one unit, the first module's first, and failed the correct test below"*),
  `fixedbugs-560-two-modules-binding-twice-two-ways-are-tested-as-they-build.hero`, and
  `fixedbugs-563-a-module-s-own-headers-come-before-another-module-s-record.hero` (*"`wide`'s first
  where this program names it first"*). Each is written in the order that once failed; a sorted
  reformat reverses it. On a copy of the first (`build/238/c560/`), both orders run `3` / `10` and
  pass `heroes test`, and `--emit-c` is refused in both with different diagnostics.
- **F23, the lanes a reformat would meet.** `git diff --stat bd4670da4 <branch>` counts the trunk's
  own changes for a lane behind it (the critic, finding 29: `b23-cli` 145 `selfhost/` files by two
  dots, 0 by three); the measure is `git branch --no-merged bd4670da4`, then `git diff --name-only
  bd4670da4...<branch> -- selfhost/` from the merge base.
- **F24, the reader-facing half.** 635 of the 1,068 files with a `use` are unsorted by F5's key, in
  a tree written with no order rule. The specification and the README hold no block of `use` lines;
  the site's `docs/modules.html` holds one, four lines, already in whole-line order; the program
  generator writes its `use` lines in the order of its shapes (`tests/harness/gen_text.hero:273-281`)
  and its oracle 3 asks that `fmt`'s output be a fixed point that builds and prints what the program
  printed (`gen_judge.hero:319-348`) (the critic, finding 28).

## The questions

- **A. The `use` grouping, the exact rule `heroes fmt` writes.** Bracketed first, then the
  program's own, each group sorted: by what key, and why. The routes to measure (the critic, finding
  26): the whole line in byte order (F7); the path part by part; case-folded; **the name the line
  binds** (the alias, else the last part; the author's reason is about names); and refused but to be
  named: `<core>` as a third group (CLAUDE.md § 13 refuses a standard library), a commented block
  left in source order (design.md:2252, *exactly one correct way*), the order as a checker rule
  (Principle 0). A blank line between the two groups or not. A comment above, beside and between `use`
  lines, with design.md §4.1's rule (F17) as the default to measure against (a comment glued to a
  `use` is that line's; one followed by a blank line is the file's), `selfhost/emit.hero`'s header
  (F6) its first case, and F8's documentation and section shapes. Runs separated by blank lines or
  comments; a `use` after a declaration (moved, sorted where it stands, or refused, a language
  change); every other form a `use` line takes. **And the author's premise, measured**: F4, F19, F20,
  F21 and F22 already show the order moving the checker's verdict, the diagnostics' and holes'
  order, the emitted C, whether `--emit-c` accepts a program, and the link line. For each, whether it
  is accepted under the rule, or whether the robust route is **to make the premise true in the
  compiler** (modules walked, units emitted, headers and libraries ordered, diagnostics and holes
  listed, by a key independent of the written order), and at what cost.
- **B. Its cost, with a prototype built in the compiler-engineer's worktree.** The files not
  canonical under the rule, by directory (the formatter's own answer replacing F5); which goldens see
  a line move (`.expected`, `.fixed`, `.applied`, a run output naming a line) and **which goldens are
  about the order** (F22); which `canonical` walks; what moves when `selfhost/`, `examples/` and
  `tests/golden/run/` are reformatted (the seed, the fixpoint, `tests/emission/**`,
  `tests/golden/emit/**`, F21); idempotence; the guard (F8); the probe's reader and families; the
  formatter's self-check; `heroes mutate`; what `check --apply` writes (F9); the hooks (F10); the
  lanes (F23); the site's claim (F16). Propose the landing order.
- **C. Where the rule is written.** design.md §4.15 (F17); `fmt.hero`'s policy comment (F2); `heroes
  fmt`'s help (`selfhost/cli/table.hero:182`, *"print the canonical form"*); the site's pages on
  `fmt` (F16); and **whether the specification moves** (F8, F16, F24): the spec-warden's question.
- **D. The index move.** Every path of F15, by the map's command over the moved tree, and what each
  becomes; whether a `file://` index at `../../packages` is accepted (F14; run `heroes add` and
  `heroes fetch` against the moved copy); F13's renaming, and the route of a key that does not hold
  the index address (F18's W4); what the suites and the site's build do with a root `packages/`; the
  name, `packages/` against alternatives (`vendor/` already stands at the root, F18; the example
  program `examples/packages/`, the placement folder `heroes.packages/`, the `packages` suite); and
  the other readings of the author's third instruction, priced: a catalogue with no `index` line
  defaulting to a project-root `packages/`, and (the critic, finding 27) `heroes.packages/` renamed
  `packages/`.
- **E. Prior art** (the historian).

## Rules every seat keeps

- **Every number in your report comes from a command you ran while writing it**, the command beside
  it; a negative sentence (*X does not exist*, *no program does Y*) is run or written as a question. A
  figure in this brief is the coordinator's or the critic's: re-run it before you rest anything on it.
- **No paid run of any kind**: no `claude -p` session, no API call, no `heroes measure --refresh`, no
  cloud run. One you find worth running goes in your report with its size, and the coordinator
  decides.
- **No credential printed**: never `gh auth token`; `gh auth status` masks it. Only `gh` reads (`issue
  view`, `issue list`, `search`), never a write.
- **No file outside `/Users/joseph/Temp/heroes/heroes-lang`**: no `/tmp`, no home directory.
- **A program that may not end** is built and its binary run under `timeout`, its output to
  `/dev/null` or through `head -c 65536` into a file; never `timeout heroes run`.
- **Write your report as you go** to `docs/panel/238-reports/<seat>.md` **in your own worktree**
  (the coordinator copies it into lane `p238`), so a seat stopped early leaves what it had.
- Your report ends with: your verdict (approve, object, or veto with its condition), the design.md
  section it rests on, the cost or delta you measured, **one falsifiable prediction** with the
  milestone or run that scores it, and the condition that would change your verdict.
- No em dashes.
