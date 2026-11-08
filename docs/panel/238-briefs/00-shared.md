# Panel 238, what every seat reads first

A **`retro-record`** sitting on the author's instructions of 2026-11-08 (`proposal.md` beside this
file): `heroes fmt` groups the `use` lines, the bracketed ones first, each group in alphabetical
order; and the repository's static index moves to a `packages/` folder at the root. The decision is
the author's. The sitting records the real objections, measures what it costs, settles what the
author left open, and says where each rule lands; nothing lands in the compiler here.

**Seats**: the compiler-engineer (prototype, questions A, B, D), the historian (question E), a
completeness critic over these briefs before any seat starts and over the reports after. The
spec-warden sits only if a seat finds the specification must move (question C). No ffi-pragmatist
and no llm-ergonomist: the change touches no C boundary and no line of the specification as briefed
(F16); the synthesis says what that gives up.

## The tree, frozen

- Every seat measures lane `p238` at the commit that carries these briefs, whose parent is
  **`bd4670da4`** (batch 25's round plus a ledger commit; `git -C .claude/worktrees/p238 log -1`).
  The sitting's own commits touch `docs/panel/238-*` alone.
- Each seat works in **its own detached worktree**, `.claude/worktrees/238-<seat>/`, made by the
  coordinator at that commit, and nowhere else: never in `p238`, the trunk, a lane or another seat's
  worktree. (The `/panel` skill names a scratchpad copy; CLAUDE.md forbids a file outside the
  repository's root, so the worktrees stand under `.claude/worktrees/` as panels 235 to 237 did.)
- **Build your own compiler in two stages** in your worktree: `clang -I runtime seed/heroes.c
  runtime/runtime.c -o heroes`, then, **in its own command**, `./heroes build selfhost/main.hero -o
  heroes-next`; judge with `./heroes-next`. The seed alone does not know `use <name>/` nor `heroes
  add`/`fetch` (defect 2352). Measured by the coordinator in `p238` at 16:20 with the load average at
  16 (`uptime`): clang on the seed `real 38.53` (`user 17.26`), the second stage past three minutes;
  `./heroes-next --version`'s second line reads **`sources 62dba14251e6eafb`**, the seed's own
  build `sources 1a1e340778db789a`.
- The machine is shared with other lanes. **A cost is measured in instructions** (`/usr/bin/time
  -l`, *instructions retired*), never in seconds; a count of files or lines is a count.
- Your evidence (programs, scripts, outputs, a prototype's diff) goes under `build/238/` in your
  worktree (`build/` is git-ignored), so the coordinator can archive it at the synthesis.

## What was measured while this brief was written (each with its command)

- **F1, `fmt` keeps the source order today.** In
  `.claude/worktrees/p238-scratch/probe1/`, three modules `zeta.hero`, `alpha.hero`, `lib/beta.hero`
  and a `main.hero` opening `use zeta` / `use lib/beta` / `use alpha`: `heroes-next fmt main.hero`
  prints the three lines in that order, exit 0, and `heroes-next run main.hero` prints `6`.
- **F2, the policy that says so.** `selfhost/print/fmt.hero:109-114`, above `format_file`: *"SOURCE
  ORDER, never a canonical order: reordering is where a formatter starts deleting the author's
  remarks; gofmt sorts imports and can afford to, it has a comment model this one does not."*
  Panel 049 (`docs/panel/049-the-platform-axis.md:181-182`) quotes the Rust bootstrap's rule,
  *"Source order, not a canonical order. ... reordering is where a formatter starts deleting the
  author's remarks"*, as the reason a hoisting formatter is a defect. The author's instruction
  reverses this policy for `use` lines; the sitting says what makes it safe now, or what it costs.
- **F3, the grammar puts no `use` first.** `spec/heroes-spec.md:57-58`: `File = { NEWLINE } { Use |
  Declaration } .` and `Use = "use" [ "<" ident ">" "/" ] ident { "/" ident } [ "as" ident ] NEWLINE
  .` A `use` after a declaration is accepted: `late.hero` in the same folder (`use zeta`, then
  `function main()` naming `alpha.one()`, then `use alpha` last) runs, prints `3`, and `fmt` keeps
  `use alpha` at the bottom. The tree holds such lines: `selfhost/cli/response_file.hero:144-145`,
  `use cli/strings` and `use cli/toolchain` under its `## Tests` heading, below declarations.
- **F4, one module twice is refused, at the second line.** `twice.hero` (`use alpha` / `use alpha as
  a2`): `heroes-next run twice.hero` exits 1 with `error[used_twice]` at `twice.hero:2:5`, then two
  more diagnostics. `fmt` formats only a file with no diagnostic (`fmt.hero:109-111`), so such a file
  is never reordered; whether order moves any OTHER diagnostic is question A.
- **F5, the census, a shell approximation and not the formatter.** Over `git ls-files '*.hero' |
  grep -v '^docs/'` (3,562 files; `docs/` holds 1,092 more, the records of earlier sittings, which
  nobody reformats), one `awk` reading each file's column-0 `use` lines: *late*, a `use` after a line
  that is neither blank, a comment nor a `use`; *split*, two `use` lines with a blank or a comment line
  between; *unsorted*, the lines not ascending by (bracketed first, then the whole line in byte
  order):

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
  | `control`, `emit`, `full`, `ir`, `lldb`, `permissive`, `tests/thesis` | 29 to 53 each | 0 | 0 | 0 | 0 |

  In all 1,068 files hold 6,836 `use` lines; 635 are unsorted, 4 late, 8 split. The four outside
  `selfhost/` and the goldens that are unsorted: `examples/interpreter/main.hero`,
  `examples/ledger/main.hero`, `examples/ledger/report/lines.hero`, `examples/shapes/main.hero`.
  **Ten files hold both groups** (a blank line between them would add a line): `examples/adventure/
  game.hero`, `adventure/main.hero`, `ledger/main.hero`, `ledger/report/lines.hero`,
  `markdown/blocks.hero`, `query/data/table.hero`, `query/fmt/report.hero`, `todo/main.hero`,
  `tests/golden/check/use-a-package-mark-told.hero`, `tests/golden/surface-fixtures/mark235/main.hero`
  (`grep -c '^use <'` and `grep -c '^use [^<]'` per file, both above 0).
- **F6, the split shapes in `selfhost/`.** `selfhost/emit.hero:1` is `use emit/large_size`, then the
  file's header comment (lines 2 to 14), then `use ast` from line 15: a comment between two `use`
  lines that is the file's header. `selfhost/emit/members.hero` is the census's third.
- **F7, byte order puts the brackets first by itself.** `<` is 0x3C: above the digits (0x30 to
  0x39), below every letter (0x41 up) and `_` (0x5F); `/` is 0x2F, below all three. `printf` of
  the lines through `LC_ALL=C sort` gives `use <core>/strings`, `use Zeta`, `use _x`, `use alpha`
  in that order (uppercase before `_` before lowercase); `use <core2>/x` before `use <core>/y`
  before `use <core_b>/z` (`2` is 0x32, `>` is 0x3E), where a part-by-part order puts `core` first;
  and `use a/b`, `use a0`, `use a_b`, `use ab`. Which characters a package name and a module part
  may hold (an uppercase letter, a digit, `_`) is question A.
- **F8, the guard compares trees and comments.** `selfhost/print/guard.hero:12-31`: `fmt`'s output
  must parse, be a fixpoint, hold the same tree, `dump(text) == dump(fmt(text))`, and keep each
  comment among the same tokens (`print/anchors.hero`); every failure exits 2. A reorder changes the
  dump's order of `use` lines and the tokens around a moved comment.
- **F9, a fix that writes a `use`.** `selfhost/resolve/missing_use.hero:66`: *add `use <module>` at the
  top of the file*, inserted at the file's first offset, `guess`. Under the new rule the line it
  writes may not be where `fmt` puts it.
- **F10, what holds the tree canonical.** `tests/harness/suite_canonical.hero:69-79` walks
  `selfhost`, `tests/harness`, `examples`, `tests/golden/run`, `ir`, `emit`, `fixedbugs` and `lldb`,
  and exempts `check`, `unsupported` and `permissive` with reasons at lines 22-45 (their `.expected`
  files cite lines and columns; `UPDATE_GOLDEN=1` does not exist; the text is the fixture).
  `.claude/hooks/fmt_check.py:349` refuses the write of a `.hero` that is not canonical, and
  `.claude/hooks/guard_bash.py` a commit whose index holds one (`.claude/rules/verification.md`
  § A suite is the last judge, Layers 0 and 1).
- **F11, what a change to `selfhost/print/` owes after the push.** `.claude/rules/verification.md`
  § The formatter's probe: `heroes probe` by hand over `selfhost`, `tests` and `examples`, families
  `single` and `bracket`, which that file carries as about 15 cool CPU hours (panel 229's seats, an
  inference there, carried here and not re-measured); and the walk
  `.claude/rules/diagnostics-and-goldens.md` § A new surface form lands in every tool owes a change
  to what the formatter prints.
- **F12, the index.** `git ls-files examples/index` lists 8 files: `core/versions`,
  `core/0.1.0/list` and six under `core/0.1.0/files/` (4 `.hero`, 2 `.md`). 14 catalogues read it
  (`git grep -c 'index = "file://../index"' -- 'examples/*/heroes.toml'`). Every program that reads
  it carries its placed tree: 84 tracked files under 14 folders
  `examples/<program>/heroes.packages/core@0.1.0-db7f072b4678b0b1/` (`git ls-files | grep
  '/heroes.packages/'`, whose other 17 are in `tests/golden/check/package-*.case`).
- **F13, the placed folder's name holds the index address.** `selfhost/module/placed.hero:78-80`:
  the key is `name@version-` and the first 16 hex digits of the SHA-256 of `"index\n" + index +
  "\n" + name + "\n" + version + "\n" + sum + "\n"`. Reproduced: `printf
  'index\nfile://../index\ncore\n0.1.0\nsha256-7324...c9998\n' | shasum -a 256` gives
  `db7f072b4678b0b1`, the folders' name; with `file://../../packages` it gives `2e3e1bba9c8acea1`.
  So moving the index renames every placed folder, unless the rule changes.
- **F14, a relative `file://` is the program folder's.** `selfhost/fetch/address.hero:1-8, 17-23`:
  `file://` names only a path inside the program's own repository, the one git answers; a relative
  path is written `file://./x` or `file://../x`, and `file://host/x` is refused.
- **F15, every tracked path that names `examples/index`**, outside `docs/panel/` (`git grep -l
  'examples/index' -- . | grep -v '^docs/panel/'`, 14 files): `.gitattributes:46`;
  `docs/records/journal/075-package-manager.md` (a record, never rewritten);
  `docs/roadmap/milestones/M-core-packages.md:402`; `examples/packages/README.md:13`;
  `site/src/lib/examples-model.ts:48` (`const INDEX = 'examples/index'`, used at 270-274 and 407);
  `tests/harness/package_rules.hero:24, 628`; `tests/harness/placed_trees.hero:62`;
  `tests/harness/suite_packages.hero:8, 623, 629`; `tests/harness/suite_records.hero:1858, 1864`;
  `tests/harness/suite_unseen.hero:400`. Four more hits are a different thing, the site's
  `/examples/index.json` and `examples/index.html` (`site/README.md`, `examples-json.ts`,
  `examples-render.ts`, `llms.txt`). `examples/README.md:119-120` names it as `index/` and
  `file://../index`.
- **F16, the specification is silent on the formatter.** `grep -n -i
  'fmt\|format\|canonical\|sorted\|order' spec/heroes-spec.md` prints seven lines, none about `fmt`
  or `use` order: line 131 *"Declaration order never matters but a group's"*, line 376 *"reads a
  module's headers in the order its groups are written"*, and five about maps, `sort`, the built-ins
  and function types. Spec line 164: *"a `use` binds its name for the whole file"*.
- **F17, design.md's home.** §4.15 *Layout, formatting, lexing* runs from `docs/design.md:2150` to
  2257; its last bullet, line 2252, *"Canonical formatter, mandatory, `gofmt`-style: there is exactly
  one correct way to write any program"*.
- **F18, the index's rulings.** Panel 235's synthesis
  (`docs/panel/235-dependencies-named-*.md`): R15 at lines 744-753 (*a local static index read over
  `file://`*), C4 at 676-683 (*`file://` only for a path inside the program's own repository root*;
  the address given in the catalogue's `index` line, the Heroes index by default, R5), R20 at
  797-804 (row 78). `ROADMAP.md:217`, row 78, M-package-index, scheduled.
  `.github/workflows/deploy-site.yml:62-77` lists `examples/**` among the site's paths and no
  `packages/**`.

## The questions

- **A. The `use` grouping, the exact rule `heroes fmt` writes.** Bracketed first, then the
  program's own, each group sorted: by what key (the whole path as written, byte order or
  case-folded, part by part; F7), and why. A blank line between the two groups or not (goimports
  puts one; what it does to line numbers below, F5's ten files). What happens to a comment above a
  `use` line, beside one, and between two (F6's header comment); to `use` lines separated by blank
  lines or comments today (one canonical block, or each run sorted on its own); to a `use` after a
  declaration (F3: moved to the top, sorted where it stands, or refused, which would be a language
  change); and to every other form a `use` line can take (an alias, `as`; and the refused ones: a
  quoted path, a trailing `/`, a `.` in a path, a bracket with no `/`). **And verify the author's
  premise rather than assume it**: does the order of `use` lines change anything a program does, any
  diagnostic's order or text, the emitted C beyond the line `hero_program_sources`, the module walk's
  order, a cache key, a `--dump-*` output, the `--json` holes list? Run it on real programs with the
  lines reordered by hand.
- **B. Its cost, with a prototype built in the compiler-engineer's worktree.** How many tracked
  `.hero` files are not canonical under the rule, by directory (the census above is an
  approximation to replace with the formatter's own answer); which goldens would see a line move
  (`.expected`, `.fixed`, `.applied`, run outputs naming a line) and so cannot be reformatted without
  touching snapshots; which of them `canonical` walks; what the seed, the fixpoint and the blessed
  emissions (`tests/emission/**`, `tests/golden/emit/**`) do when `selfhost/` is reformatted;
  idempotence; the guard (F8); the probe's reader and families; the formatter's self-check
  (`selfhost/cli/syntax_cmds.hero`); `heroes mutate`; the `Fix` that writes a `use` (F9); the hooks
  (F10), so the landing order is rule and tree in one commit; and the lanes open on the trunk that a
  reformat of 585 `selfhost/` files would meet at their merge. Propose the landing order.
- **C. Where the rule is written.** design.md §4.15 (F17); `fmt.hero`'s own policy comment (F2); the
  specification if anything there moves (F16; if it does, the spec-warden sits and counts with
  `heroes measure`, vendored tables only, no `--refresh`); `heroes fmt`'s help
  (`selfhost/cli/table.hero:182`, *"print the canonical form"*) and the site's pages on `fmt`.
- **D. The index move.** Every path of F15 and what each becomes; whether a `file://` index at
  `../../packages` is accepted (F14; run `heroes add` and `heroes fetch` against a copy of the index
  moved to the root of your worktree); what F13's renaming costs; what `corpus`, `records`,
  `unseen`, `canonical`, `packages`, `fetch` and the site's build do with `.hero` files under a root
  `packages/`; what R15, C4 and R20 say; the name, `packages/` against alternatives, with what would
  make it wrong later (the example program `examples/packages/`, the placement folder
  `heroes.packages/`, the `packages` suite, the word in panel 235's R7); and the other reading of the
  author's third instruction (`proposal.md`), priced.
- **E. Prior art** (the historian): Go's gofmt and goimports, rustfmt, isort and PEP 8, Dart's
  `directives_ordering`, Swift, Zig, Deno; and where other ecosystems keep a repository's own static
  index or vendored registry folder.

## Rules every seat keeps

- **Every number in your report comes from a command you ran while writing it**, the command beside
  it; a negative sentence (*X does not exist*, *no program does Y*) is run or written as a question. A
  figure in this brief is the coordinator's: re-run it before you rest anything on it.
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
