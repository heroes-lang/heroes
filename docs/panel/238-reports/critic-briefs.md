# Panel 238, the completeness critic's first pass: the briefs

Read at the briefs' commit `d59c34489` (parent `bd4670da4`), in the worktree
`.claude/worktrees/238-critic/`, on 2026-11-08 from 16:33 (`uptime` read load averages 14.24
14.86 12.57). Every command below was run from that worktree's root unless it names another
folder. The probes of `.claude/worktrees/p238-scratch/probe1/` were copied (the `.hero` files
only, never its `build/`) into `build/238/probe1/` and run there.

The compiler: `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes` (`/usr/bin/time -l`:
`19.30 real 15.58 user`, exit 0), `./heroes --version` second line `sources 1a1e340778db789a`, the
seed's key the brief gives; stage 2 `./heroes build selfhost/main.hero -o heroes-next` in its own
command (result under finding 1).

Verdicts: `reproduces`, `does not reproduce` (with the true value), `question`, `missing`.

## Findings

### 1. The compiler the brief names, and stage 2's key

- Ran: `./heroes build selfhost/main.hero -o heroes-next` under `/usr/bin/time -l` (`158.97 real
  117.68 user 26.89 sys`, `746955029902 instructions retired`, exit 0); `./heroes-next --version`.
- Printed: `heroes 0.6.0` / `sources 62dba14251e6eafb`; the seed's build `sources 1a1e340778db789a`.
- Verdict: **reproduces** (00-shared.md § The tree, frozen).

### 2. F1, `fmt` keeps the source order

- Ran, in `build/238/probe1/`: `../../../heroes-next fmt main.hero`, `../../../heroes-next run
  main.hero`.
- Printed: `use zeta` / `use lib/beta` / `use alpha` in that order, exit 0; the run printed `6`, exit 0.
- Verdict: **reproduces**.

### 3. F2, the policy comment and panel 049's quotation

- Ran: `grep -n 'SOURCE ORDER\|Canonical Heroes source\|gofmt sorts\|and can afford' selfhost/print/fmt.hero`;
  `grep -n 'Source order, not a canonical\|starts deleting' docs/panel/049-the-platform-axis.md`.
- Printed: `fmt.hero` 109 (`# Canonical Heroes source ...`), 112 to 114 (`SOURCE ORDER, never a
  canonical order: reordering is where a / formatter starts deleting the author's remarks` then an
  em dash (U+2014) then `gofmt sorts imports / and can afford to, it has a comment model this one does
  not.`); panel 049 lines 181 and 182.
- Verdict: **reproduces**, with one note: the brief's quotation writes `;` where the file has an em
  dash, so it is not the file's text; a seat that greps for the brief's wording finds nothing. Panel
  049's context (lines 175 to 182) is a hoisting of `link` lines in an `extern` block, where *link
  order matters to a linker, reproduced*: the record's precedent is a reorder that changed a
  program's meaning, which is the strongest form of F2 and worth the historian's and the
  compiler-engineer's eye.

### 4. F3, the grammar puts no `use` first, and a late `use` runs

- Ran: `sed -n 55,59p spec/heroes-spec.md`; in `build/238/probe1/`, `heroes-next run late.hero`,
  `heroes-next fmt late.hero`; `sed -n 138,150p selfhost/cli/response_file.hero`.
- Printed: lines 57 and 58 as quoted; `3`, exit 0; `fmt` printed `use alpha` last, exit 0;
  `response_file.hero` 142 `## Tests`, 144 `use cli/strings`, 145 `use cli/toolchain`.
- Verdict: **reproduces**.

### 5. F4, "`fmt` formats only a file with no diagnostic, so such a file is never reordered"

- Ran, in `build/238/probe1/`: `heroes-next run twice.hero`; `heroes-next fmt twice.hero`;
  `heroes-next check twice.hero`.
- Printed: `run` exit 1, `error[used_twice]` at `twice.hero:2:5`, then `error[unknown_name]` and
  `error[needs_qualifying]` (the brief's three). **But `fmt twice.hero` printed the file and exited
  0.** `selfhost/print/guard.hero:71-79`, `checked`, the one function `fmt` calls
  (`selfhost/cli/syntax_cmds.hero:47`), asks `parse.parse_source(s)` alone and refuses only on the
  PARSER's diagnostics; `fmt` reads the root file alone (`selfhost/cli/input.hero:64`,
  `read_root_only`), so it never resolves a `use` and never sees `used_twice`,
  `module_binding_taken` or any checker diagnostic. `fmt.hero:109-111`'s *Callers must check that
  the file had no diagnostics* means the parser's.
- Consequence, run: two files with the same two lines in the two orders,
  `build/238/probe1/clash_a.hero` (`use zeta` / `use lib/beta as zeta`) and `clash_b.hero` (the two
  swapped), each with `print(zeta.two())`. `heroes-next check clash_a.hero`: ONE error,
  `module_binding_taken` at 2:17, *this file already calls `zeta` by the name `zeta`, so `lib/beta`
  cannot have it too*, with `fix (guess): write `use lib/beta as lib_beta``. `check clash_b.hero`:
  TWO errors, `module_binding_taken` at 2:5 with the roles swapped and no fix, then
  `unknown_in_module`, *module `lib/beta` declares nothing called `two`*. `fmt` exits 0 on both and
  prints each in its own order today. A whole-line byte sort puts `use lib/beta as zeta` first, so
  the new `fmt` would turn `clash_a` into `clash_b`: one diagnostic with a repair becomes two without
  one.
- Verdict: **does not reproduce** (the inference). The true value: `fmt` formats, and under the new
  rule would reorder, every file the parser accepts, including files the checker refuses, and on
  those the order of `use` lines decides which line is blamed, the message's text, the diagnostics
  that follow and whether a `Fix` is offered. The author's premise, *the order does not matter
  anyway, since two equal names cannot be used*, holds for a program that compiles and not for a
  file the formatter is handed. **Changes the brief**: F4 must say this, and question A's premise
  check must include a file that parses and is refused by the checker (a name clash, `used_twice`, a
  late `use` naming a module that does not exist), not only correct programs.

### 6. F5, the census

- Ran: the brief's `awk` exactly, into `build/238/census.txt`; `git ls-files '*.hero' | grep -v
  '^docs/' | wc -l`; a per-directory tabulation of the output; the both-groups loop (`grep -c '^use <'`
  and `grep -c '^use [^<]'` per file).
- Printed: 3,562 rows; 1,068 with a `use`, 6,836 `use` lines, 635 unsorted, 4 late, 8 split; the ten
  both-groups files exactly as listed. `git ls-files` gives **3,564**: the `awk` never sees an empty
  file, and `tests/golden/surface-fixtures/empty109/empty.hero` and `.../empty109/alone/empty.hero`
  are 0 bytes. The last table row does not hold: `control` 29, `emit` **13**, `full` 53, `ir` 29,
  `lldb` **15**, `permissive` **22**, `tests/thesis` 50, not *29 to 53 each*. And *the four outside
  `selfhost/` and the goldens that are unsorted* are the four of `examples/`: `tests/harness` holds
  **25** unsorted files, outside both.
- Verdict: **reproduces** in its totals and its two lists, with three corrections (3,564 tracked,
  the row's range, the 25 of `tests/harness`). Nothing rests on them; the formatter's own count
  (question B) replaces the census anyway.

### 7. F6, the split shapes, and where they come from

- Ran: `sed -n 1,20p selfhost/emit.hero`; `grep -n -B2 -A2 '^use ' selfhost/emit/members.hero`;
  `git log -S'use emit/large_size' -- selfhost/emit.hero selfhost/emit/members.hero`; in
  `build/238/probe1/`, `missing.hero` (`# header`, `use zeta`, a body naming `alpha.one()`) through
  `heroes-next check` and `check --apply`.
- Printed: `use emit/large_size` at line 1 of both files, the header comment below it, the rest of
  the block from line 15 (`emit.hero`) and 17 (`members.hero`); both lines entered in one commit,
  `82a9dc630` (defect 2278, 2026-11-01). `check missing.hero`: `fix (guess): add `use alpha` at the
  top of the file (at missing.hero:1:1)`, that is, above `# header`.
- Verdict: **reproduces**, and a **question** the brief does not ask: two of the three split shapes
  of `selfhost/` are the shape F9's fix writes (offset 0, above the header comment). Whether that
  commit took the fix or a hand wrote the same thing is unrun; either way the comment rule of
  question A decides whether a file header stays a header once a tool or a person writes a `use`
  above it, and the rule should be tried on exactly `missing.hero`'s shape.

### 8. F7, byte order, and which characters each part holds

- Ran: the three `printf ... | LC_ALL=C sort` of the brief; `git grep -n 'lowercase letters,
  digits'` over `selfhost/fetch/` and `selfhost/module/`; in `build/238/probe1/`, `Upper.hero`,
  `_9x.hero` and `cases.hero` (`use Upper` / `use _9x` / `use alpha`) through `heroes-next run`;
  `git grep -n '^use _0opt'`.
- Printed: the three orders exactly as the brief gives them. A package name is *lowercase letters,
  digits and `_`, starting with a letter* (`selfhost/fetch/add.hero:143`,
  `selfhost/module/catalogue.hero:282`; the site holds the same, `site/src/lib/examples-model.ts`'s
  `^[a-z][a-z0-9_]*`). A module part may open with an uppercase letter or `_`: `cases.hero` ran and
  printed `10`, exit 0; the tree holds `use _0opt`
  (`tests/golden/run/fixedbugs-2327-a-module-led-by-an-underscore-and-a-digit.case/main.hero:8`); a
  `use` in another case than its file is refused even on this case-insensitive disk
  (`tests/golden/check/fixedbugs-2290-*`).
- Verdict: **reproduces**, and half of its question is answered here: the bracketed group's key
  never meets an uppercase letter, the program's group does (`use Upper`, `use _9x`, `use alpha`
  sort in that order by bytes and otherwise case-folded). The compiler-engineer can start from this.

### 9. F8, the guard

- Ran: `sed -n 1,35p selfhost/print/guard.hero`.
- Printed: the four checks at lines 12 to 31 as the brief says, and at 19 to 21 *panel 060's hoisted
  `record`, and a record-field remark promoted to documentation*.
- Verdict: **reproduces**. One shape for question A follows from spec section 1 (`spec/heroes-spec.md:49-50`,
  *A comment directly above a declaration documents it*): a late `use` standing between a comment and
  a declaration, hoisted, leaves the comment directly above the declaration, which then documents it;
  the tree changes and the guard exits 2. Worth a probe row of its own.

### 10. F9, and the fix `check --apply` actually writes into a `use` line

- Ran: `sed -n 50,90p selfhost/resolve/missing_use.hero`; `git grep -n '"use " +'` over `selfhost/`;
  `sed -n 255,290p selfhost/parse/use_line.hero`; in `build/238/probe1/`, `check missing.hero
  --apply` (finding 7) and `quoted.hero` (`use "zeta"` / `use alpha`) through `check` and `check
  --apply`.
- Printed: F9's fix at line 66, `certainty: .guess`, as the brief says. `check missing.hero --apply`
  printed the file **unchanged**, exit 0: `--apply` takes `certain` fixes only (CLAUDE.md § 8), so F9
  never reaches its output. The certain fix that writes into a `use` line is
  `selfhost/parse/use_line.hero:273`, `use_wants_a_name` (defect 2308): `check quoted.hero` gives
  `fix (certain): write `use zeta``, and `check quoted.hero --apply` printed `use zeta` / `use
  alpha`, exit 0, which under a sorted rule is not canonical. A third writer is the
  `module_binding_taken` fix of finding 5 (`write `use lib/beta as lib_beta``, a guess), which changes
  an alias and so, under a key that reads the alias, a line's place.
- Verdict: **reproduces** as a location, **does not reproduce** as the brief uses it: question B's
  *what the `Fix` of F9 must write so that `check --apply`'s output is canonical* names a fix
  `--apply` never applies. **Changes the brief**: name `use_line.hero:273`'s certain repair as the one
  whose output `check --apply` writes, and ask whether `--apply` must re-sort or the rule accepts an
  `--apply` output that `fmt` would change (no `fmt` call is in `selfhost/cli/check.hero`,
  `certain.hero` or `consensus.hero`: `git grep -n 'use print/fmt\|fmt\.format\|canonical'` over the
  three printed nothing).

### 11. F10, what holds the tree canonical

- Ran: `sed -n 18,48p` and `sed -n 66,82p tests/harness/suite_canonical.hero`; `grep -n` of
  `SOURCE_DIRS` and of the hook's message; `sed -n 300,355p .claude/hooks/fmt_check.py`; `grep -ln
  canonical .claude/hooks/*.py`.
- Printed: `SOURCE_DIRS` at 69 to 79, the reasons at 22 to 45, the hook's refusal at 349. The commit
  refusal lives in `.claude/hooks/staged.py` (lines 98 to 181), which `guard_bash.py:332` imports, not
  in `guard_bash.py` itself; and `fmt_check.py:323-351` asks `fmt` of every `.hero` written in a
  tree, not only those `canonical` walks, a marked golden being judged by its marks instead.
- Verdict: **reproduces**, with the commit refusal's file corrected (`staged.py`).

### 12. F11, the probe and the walk

- Ran: `grep -n '15 cool CPU hours\|bracket` 10.2' .claude/rules/verification.md`; `grep -n '^## '
  .claude/rules/diagnostics-and-goldens.md`.
- Printed: lines 382 and 383; the walk's section at line 96.
- Verdict: **reproduces** (the 15 hours stay panel 229's inference, as the brief says).

### 13. F12, the index and the placed trees

- Ran: `git ls-files examples/index`; `git grep -c 'index = "file://../index"' -- 'examples/*/heroes.toml'`;
  `git ls-files | grep '/heroes.packages/'`, split by `^examples/`, by folder and by placed name.
- Printed: 8 files as listed; 14 catalogues; 101 placed files, 84 under 14 folders of `examples/`,
  every one `core@0.1.0-db7f072b4678b0b1`. The other 17 are **14** in seven
  `tests/golden/check/package-*.case` folders and **3** in
  `tests/golden/run/package-one-index-written-two-ways.case/heroes.packages/`.
- Verdict: **reproduces**, except *whose other 17 are in `tests/golden/check/package-*.case`*: three
  are a `run` case, whose name is about the index's address.

### 14. F13, the placed folder's name holds the index address, as written

- Ran: the brief's `printf ... | shasum -a 256` with the sum read from `examples/adventure/heroes.toml`
  (`sha256-732461c3...c9998`), for `file://../index` and `file://../../packages`; the same for the
  `run` case of finding 13, its `json` and `text` entries keyed with `file://../idx` and with
  `file://./../idx`.
- Printed: `db7f072b4678b0b1` and `2e3e1bba9c8acea1`, as the brief says. The `run` case's folders
  are `json@1.0.0-504c8a7afd955b82` and `text@2.0.0-272d9556f3386f0e`, which are the keys of
  `file://../idx`, the program catalogue's text; `file://./../idx` would give `7aa93f40a52b0f79` and
  `016e3a8e4a832f0c`.
- Verdict: **reproduces**, and the key reads the `index` line's text as written, not a normalised
  path: any respelling of the line, not only a move, renames every placed folder.

### 15. F14, a relative `file://` is the program folder's, and a bare `use` cannot reach the moved index

- Ran: `sed -n 1,30p selfhost/fetch/address.hero`; in `build/238/spell/`, a copy of
  `examples/index/core/0.1.0/files/strings.hero` under `packages/core/0.1.0/files/`, and a `main.hero`
  opening `use packages/core/0.1.0/files/strings`, through `heroes-next check`; a second,
  `placed.hero`, opening `use packages/core@0.1.0-db7f072b4678b0b1/strings`.
- Printed: lines 1 to 8 and 17 to 23 as quoted. The first program: `error[module_path_wants_a_part]`
  at `main.hero:1:18`, exit 1; the second: `error[expected_declaration]` at 1:18, *found `@`*, exit 1.
- Verdict: **reproduces**, and a property the brief does not ask about is measured to hold: design.md
  lines 909 to 911 make the catalogue's falsifier hold *only where a `use` without brackets never
  reaches fetched code*; a root `packages/` keeps that, because the version folder's dots (and a
  placed folder's `@`) cannot be spelled in a `use`. It holds by the index's layout, not by the
  folder's name, so it binds any future layout of the index too.

### 16. F15, the paths that name the index, and the walks that reach it without naming it

- Ran: `git grep -l 'examples/index' -- . | grep -v '^docs/panel/'` (14 files, the brief's), each
  hit line; then `git grep -n '"examples"\|"examples/"' -- 'tests/harness/*.hero'`; `timeout 300
  ./heroes-next probe examples/index --family multi`; `timeout 900 ./heroes-next probe examples
  --family multi` (output in `build/238/probe-examples-multi.txt`); `sed -n 1,30p` and `335,360p` of
  `tests/harness/suite_compat.hero`; `sed -n 80,110p tests/harness/cards.hero`.
- Printed: the 14 files as listed. Eleven harness modules name the root `"examples"`, which walks the
  index without the literal `examples/index`: `absence.hero:1881`, `cards.hero:99`,
  `platform_skips.hero:206`, `suite_canonical.hero:73`, `suite_compat.hero:70`, `suite_corpus.hero:58`,
  `suite_emission.hero:111`, `suite_packages.hero:80`, `suite_probe.hero:191`,
  `suite_records.hero:2950`, `suite_warnings.hero:54`. The probe: `examples/index` alone is **4 seeds,
  36 variants, 36 held**; `examples` whole is **181 seeds, 1,629 variants, 1,629 held, 0 refused**,
  against the row's floor of 159 and 1,431 (`suite_probe.hero:191`, whose comment at 182 to 186
  counts *the core packages' canonical tree* in those seeds). `compat` unpacks `git archive <tag>
  examples <references>` and nothing else (lines 339 to 358). `AREAS` in `cards.hero` has no area
  for a new top-level folder.
- Verdict: **missing**. Question D lists `corpus`, `records`, `unseen`, `canonical`, `packages`,
  `fetch` and the site; it omits `probe` (the move takes 4 seeds and 36 variants out of the `examples` row:
  181 to 177 stays above the floor, so no red, and the index's modules lose the probe unless a row
  names `packages`), `canonical` by its `SOURCE_DIRS` (no `packages`, so the index leaves the walk
  unless named), `compat` (a release that carries the move archives `examples/` without its index),
  `cards` (the area a defect in `packages/` is filed under) and `.gitattributes:46`'s `-text`, which
  must follow the files or a CRLF checkout changes the bytes their sums name. The instrument for this
  list is `.claude/rules/verification.md` § The map's command, which reads the paths from git; the
  brief should ask for its output over the moved tree, not a hand list.

### 17. F16, *the specification is silent on the formatter*

- Ran: the brief's `grep -n -i 'fmt\|format\|canonical\|sorted\|order' spec/heroes-spec.md`, and
  the same pattern with `grep -c -i`; `sed -n 21,60p spec/heroes-spec.md`; `grep -n '^ *use '
  spec/heroes-spec.md`.
- Printed: **8** lines, not seven: 131, 213, 216, **279** (*Labels keep the declared order*, of a
  record's construction), 286, 333, 341, 376. No line names `fmt` or an order of `use` lines, and the
  spec holds no block of `use` lines (the last grep printed nothing). But section 1, lines 49 and 50:
  *`#` comments to end of line ... A comment directly above a declaration documents it; `##` is a
  section heading.*
- Verdict: **does not reproduce** as a count (8, not 7). As a negative sentence it is a **question**:
  the spec says nothing of `fmt`, but it gives a comment two meanings a reorder can change, a
  documentation (finding 9) and a section (`selfhost/cli/response_file.hero`'s two `use` lines under
  `## Tests`, which a hoist takes out of that section). Whether the spec moves therefore depends on
  question A's comment rule, which no seat is asked to carry to the spec (finding 24).

### 18. F17, design.md's home

- Ran: `grep -n '^### 4.15\|^### 4.16' docs/design.md`; `grep -n 'Canonical formatter, mandatory'
  docs/design.md`.
- Printed: 2150, 2258; 2252.
- Verdict: **reproduces** (2150 to 2257).

### 19. F18, the index's rulings and the site's paths

- Ran: `sed -n` of the synthesis at 744-753, 676-690 and 797-804; `sed -n 217p ROADMAP.md`; `sed -n
  55,80p .github/workflows/deploy-site.yml`; `head -40 site/src/lib/triggers.ts`; `ls
  site/node_modules` in this worktree and in the trunk.
- Printed: R15, C4 (whose sentence ends at 684, *and on the command line for `add` only*) and R20 as
  quoted; row 78; the `paths:` list holds `examples/**` and no `packages/**`. It also holds
  `selfhost/**` and **`docs/panel/**`**. `triggers.ts` refuses a build that reads a path the list
  does not name. `site/node_modules` is absent from this worktree and present in the trunk.
- Verdict: **reproduces**, with two consequences the briefs do not draw. (a) The sitting's own commits
  (`docs/panel/238-*`) and the landing's reformat of `selfhost/` and `examples/` each publish the
  site at their push, so each is a site push to announce before committing (CLAUDE.md § Hard stops).
  (b) A fresh seat worktree never has `site/node_modules`, so the compiler-engineer's item 5 condition
  makes the site's build unrun by construction; `npm ci` in the seat's `site/` (free, no paid run)
  makes it runnable.

### 20. The negative sentence *the seed alone does not know `use <name>/` nor `heroes add`/`fetch`*

- Ran: `./heroes check examples/adventure/main.hero` (the seed's build) and `./heroes-next check` of
  the same; `./heroes fetch`; `./heroes add`.
- Printed: the seed's build, `error[expected_module_name]` at `examples/adventure/main.hero:22:1`
  (`use <core>/strings`), exit 1; stage 2, exit 0; `fetch` and `add`: *unknown command*, the list
  of thirteen commands without either.
- Verdict: **reproduces**.

### 21. The negative sentence *the change touches no C boundary* (00-shared.md, Seats)

- Ran, in `build/238/cside/`: `ma.hero` (`extern "math.h" link "m"`, `sqrt`), `mb.hero` (`extern
  "stdlib.h" link "c"`, `labs`), and two roots differing only in the order of `use ma` / `use mb`
  (`ab.hero`, `ba.hero`), each through `heroes-next run`, `heroes-next build --emit-c` and
  `heroes-next build -o`; `diff` of the two emissions with `grep -v 'hero_program_sources(void) {'`
  on each; the `-l` words of each build's `build/*/link-words.rsp`.
- Printed: both runs `4.0` / `3`, exit 0. The emissions differ in **65** diff lines beyond
  `hero_program_sources` (a few of them only the root file's own name): `#include <math.h>` and `#include
  <stdlib.h>` change places, and so do the `_Static_assert` FFI return checks, the FFI probes and the
  prototypes. The link words read `"-lm" "-lc"` for one order and `"-lc" "-lm"` for the other.
  A header that needs another (`needs_file.h`, using `FILE` without including `stdio.h`, beside a
  module whose group names `stdio.h`) was refused in BOTH orders (`ffi_header_refused`, each module's
  unit reads its own headers), so the include order across modules did not decide compilation here.
- Then the one-file emission, which design.md §4.1 (lines 938 to 942) says `--emit-c` writes and
  refuses at exit 1 *where a module's binding means something else in it* (defect 453,
  `cli/one_file.hero`). Ran, in `build/238/c453/`: `d1.h` (`#define MODE 7`, `f1` returns `MODE`),
  `d2.h` (`f2` returns 99 under `#ifdef MODE`, else 2), `a_mode.hero` binding `f1` from `d1.h`,
  `b_check.hero` binding `f2` from `d2.h`, and two roots, `written.hero` (`use b_check` / `use
  a_mode`) and `sorted.hero` (the two in byte order), each through `heroes-next run` and
  `heroes-next build --emit-c`. Printed: both runs `7` / `2`, exit 0; `written.hero --emit-c` **exit
  0**; `sorted.hero --emit-c` **exit 1**, `error[ffi_one_file]` at `b_check.hero:2:14`, *`f2`, which
  this group binds, is not what this module's own unit makes of it*. And on the tree's own defect 560
  case (copied to `build/238/c560/`, finding 25): both orders run `3` / `10` and pass `heroes test`,
  and `--emit-c` is refused in both with **different** diagnostics, the written order's
  `ffi_one_file` at `cap_ten.hero:4:14` (*`cap` ... is not what this module's own unit makes of it*),
  the sorted order's at `cap_ten.hero:3:8` (*cannot be compiled together: ... 'LIMIT' macro
  redefined*).
- Verdict: **does not reproduce**. The order of `use` lines decides the order of the C headers in the
  whole-program unit and the order of the libraries on the link line; a sorted `fmt` therefore
  changes both for every program whose modules carry `extern` groups. Whether a library order can
  change whether a program links or what it binds (static archives, GNU ld on the Linux legs) is
  still a question; that the order changes whether `build --emit-c` accepts a program is now
  measured, above, and `--emit-c` is what makes the seed and what `compat` asks of every release
  program (`suite_compat.hero:15`; `seed/README.md:157`). The formatter's guard compares trees and comments and cannot
  see it. This is exactly panel 049's precedent (*link order matters to a linker, reproduced*), and it is the
  ffi-pragmatist's question. **Changes the brief**: the ffi-pragmatist's absence rests on a premise
  this run falsifies; either the seat sits on this one question, or the compiler-engineer's item 2
  names it with a program whose two modules link two static archives, run on a Linux container.

### 22. The author's premise, other outputs it already fails on (pre-measured for question A)

- Ran, in `build/238/cside/`: `e1.hero` and `e2.hero`, each with one `type_mismatch`, used by
  `d12.hero` (`use e1` / `use e2`) and `d21.hero` (swapped), through `heroes-next check`; `h1.hero`
  and `h2.hero`, each with a hole `???`, used by `g12.hero` and `g21.hero`, through `heroes-next check
  --json`.
- Printed: `d12` reports `e1.hero:2:12` first, `d21` reports `e2.hero:2:12` first. `g12`'s JSON lists
  the files `g12.hero, h1.hero, h2.hero` and the holes `h1` then `h2`; `g21`'s lists `h2` before
  `h1`, and the text line reads `hole at h2.hero:2:12` first.
- Verdict: **question**, not a brief error: the brief asks the compiler-engineer exactly this, and
  three of the outputs it lists (diagnostics' order, the holes list, the emitted C) already move with the
  order, with findings 5 and 21. What the brief does not ask is the consequence: after the rule, the
  first diagnostic a reader (a model included) sees in a multi-module program is chosen by the
  alphabet rather than by the author's order. The synthesis should state whether that is accepted or
  whether the compiler's own walk becomes order-free (route A2, finding 26).

### 23. The blessed emissions an `examples/` reformat moves (question B names only `selfhost/`'s)

- Ran: `./heroes-next build examples/shapes/main.hero --emit-c` against
  `tests/emission/examples-shapes-main.c` (both with `hero_program_sources` taken out); then the five
  `use` lines of `examples/shapes/main.hero` sorted in place by a three-line `python3 -I` script (only
  `geom/point` and `geom/area` change places), the emission taken again, and the file restored with
  `git checkout -- examples/shapes/main.hero` (`git status --short examples/` then printed nothing);
  `ls tests/emission | grep -E 'examples-(interpreter|ledger|shapes|adventure|markdown|query|todo)'`.
- Printed: before, **0** diff lines against the blessed file; after the sort, **50** diff lines (the
  function `h_geompoint_span` and its prototype move), exit 0 both times. Blessed emissions exist for
  all seven examples programs among F5's unsorted and both-groups files that the grep names.
- Verdict: **missing**. Question B asks what the blessed emissions do *when `selfhost/` is
  reformatted*. `shapes` moved its own, measured; by the same mechanism (findings 21 and 22, unrun on
  them) so would the other three unsorted `examples/` files and the unsorted `tests/golden/run`
  cases that have one (`ls tests/emission | grep -c run-<case>`: defect 2030's 1, 2286's 13, 563's
  5, the two of 560 none). The `emission` suite then turns red at the reformat until they are
  blessed, and the landing order must say in which commit.

### 24. Question C has no seat

- Ran: `grep -n 'question C\|questions A, B, D\|(question E)' docs/panel/238-briefs/*.md`.
- Printed: `00-shared.md:9-12` gives the compiler-engineer A, B and D, the historian E, and the
  spec-warden a seat *only if a seat finds the specification must move (question C)*;
  `compiler-engineer.md` lists items for A, B and D only; `historian.md` for A (precedent), D and E.
- Verdict: **missing**. No seat is asked C (design.md §4.15's text, `fmt.hero`'s policy comment, the
  help line `selfhost/cli/table.hero:182`, the site's pages), so the trigger that seats the
  spec-warden is owned by nobody, and finding 17 shows the trigger is live: the spec's two comment
  meanings meet question A's comment rule. The site's start page (`site/src/html/start.html:110`, and
  `it/start.html:111`) says *`heroes fmt` formats code using fixed rules for indentation, line breaks
  and order. The project's source files, including the compiler, already follow those rules: the
  formatter returns them unchanged, byte for byte*: a published claim that is false at any pushed
  commit where the rule has landed and the tree is not yet reformatted, which binds the landing order
  (question B) as much as question C.

### 25. Goldens whose subject is the order of their `use` lines, inside `canonical`'s walk

- Ran: `awk '$5==1 && $1 ~ /^tests\/golden\/run\//{print $1}' build/238/census.txt` (the five unsorted
  `run` cases); `grep -n -i 'order\|first\|^use '` of each; `sed -n 1,12p` of the two defect 560
  cases; `head -20` of the defect 563 case; `ls tests/emission | grep -c "run-<case>"` for each.
- Printed: three of the five pin a cross-module order in their own words.
  `fixedbugs-560-a-default-another-module-defines-is-not-read-under-test.hero` (`use
  fixedbugs560/limit_half` / `use fixedbugs560/cap_ten`): *Until 2026-10-10 `heroes test` read both
  in one unit, the first module's first, and failed the correct test below, left: 50 right: 10*.
  `fixedbugs-560-two-modules-binding-twice-two-ways-are-tested-as-they-build.hero` (`long_twice`
  then `int_twice`): the whole program as one unit refused it. `fixedbugs-563-a-module-s-own-headers-
  come-before-another-module-s-record.hero` (`wide` then `narrow`): *a unit's records were read in
  declaration order across modules, `wide`'s first where this program names it first, and `Sk.a:
  i32` was refused*. Each is written in the order that once failed, and each is unsorted, so a
  sorted reformat reverses it. Blessed emissions: 0, 0 and 5 files in `tests/emission`.
- Verdict: **missing**, and the strongest item for question B after finding 21. Defects 560 and 563
  are this compiler's own record of the order of `use` lines deciding a program's meaning (a macro's
  value) and whether it compiled, repaired by giving each module its own unit. Their regression cases
  live in `tests/golden/run/`, which `canonical` walks, so the rule as briefed rewrites them into the
  order that always passed. Whether a reversed case still catches the defect's return is a
  **question** settled by one run: the compiler at the commit before each repair, on both orders. The
  routes: exempt such a case as `check/` is exempt (*the text is the fixture*), or rename its modules
  so the order it pins is also the sorted one, a route nobody listed. The brief asks *which goldens see a
  line move*; it must also ask *which goldens are about the order*.

### 26. Routes nobody listed for question A

Each with what would have to be true, and whether it is.

- **A1, the key is the name the line binds** (the alias, else the last part), not the path. The
  author's own reason is about names (*two equal names cannot be used*), and the bound name is what a
  reader searching for `names.` in a body looks up. Would have to be true: the formatter knows the
  alias, and the names are unique. Both hold: `fmt.hero`'s `use_line` (line 338) prints the alias
  kept, and spec line 164 (*a `use` binds its name for the whole file*) makes the bound names
  distinct in every file the checker accepts; ties exist only in refused files (finding 5), which
  need a tie-break (the path, then the source order). Against it: `use render/scale as near_scale`
  would sort far from `use render/ascii`. Unmeasured; the compiler-engineer can price it beside the
  path keys.
- **A2, make the premise true in the compiler**: walk modules, emit units, list diagnostics and holes,
  and order headers and libraries by a key independent of the `use` order, so a reorder by `fmt` moves
  nothing but text. Would have to be true: no program depends on the cross-module order. Findings 21
  and 25 say it is not obviously true for the link line, and defects 560 and 563 say it was false for
  headers until 2026-10-10; this is the route that turns the author's premise from an assumption into
  a property, and its price is a compiler change the sitting does not convene for. Unmeasured.
- **A3, a third group for `<core>`**, as goimports puts the standard library first. Would have to be
  true: `core` is special. It is not: panel 235's R15 makes it one catalogue entry of an index, so
  this route does not exist without a ruling that `core` is a standard library, which CLAUDE.md § 13
  (*Where not to go*: *A standard library*) refuses.
- **A4, leave a block that holds a comment in source order, sort only a block without one.** Would
  have to be true: two canonical texts for one set of lines are acceptable. design.md:2252 (*exactly
  one correct way*) refuses it, so this is the conservative route the synthesis should name and price
  as refused, not leave unlisted.
- **A5, the order as a checker rule with a `certain` fix instead of a formatter rule.** Would have to
  be true: the order is a compile error. It would keep F2's *`fmt` never reorders* intact, but it is a
  new diagnostic class (CLAUDE.md § 4) and refuses programs for appearance, which Principle 0 (§ 2)
  does not admit; listed so the synthesis can say why not.

- **A6, the comment rule the formatter already holds.** `selfhost/print/fmt.hero:234-237` (read with
  `sed -n 215,248p`): *a blank line between a comment and the first use is what makes that comment a
  remark about the file instead of documentation for the line under it*, design.md §4.1's rule
  (lines 962 to 963, *A comment adjacent to a declaration is its documentation ... A blank line
  between comment and declaration makes it an ordinary comment*). So the printer already reads a
  comment glued to a `use` as that line's own and a comment followed by a blank line as the file's.
  Would have to be true for the obvious rule (a glued comment travels with its line, a separated one
  stays where it is): the dump and `anchors.hero` carry the same reading. Not a new route so much as
  the one the formatter's own model implies; the brief should hand it to the compiler-engineer as the
  default to measure against, with `selfhost/emit.hero`'s header (finding 7) as its first case.

A fact for the comment rule: in the trees `canonical` walks, no `use` line carries a trailing comment;
the ten that do are all `#~` marks in `tests/golden/check/` (`git ls-files '*.hero' | grep -v
'^docs/' | xargs grep -n '^use .*#'`), so *beside* is a fixture's shape only.

### 27. Routes nobody listed for question D

- **D1, a key that does not hold the index address.** `placed.hero:74-77` says why it holds it:
  *every field that names the package, so a field mistyped names a tree that is not there*. Panel
  235's restated W4 (its line 304) lists the cases that falsifier needs: the catalogue deleted, an
  entry deleted, `sum`, `version`, `tag` and `commit` mistyped, the tree edited, a copy left; an
  `index` line mistyped is not among them, and the `sum` alone fixes the bytes. Would have to be true:
  a mistyped `index` line may go unnoticed at build, since `build` never reads the index. Then this
  move, and every later one, renames nothing after one last rename. A **question** for the
  compiler-engineer, who must read R7's reasons before pricing it; it changes `placed.hero`, so it is
  a compiler change.
- **D2, one spelling for every catalogue whatever its depth**, a repository-relative `file://` form.
  It does not exist: `address.hero:17-23` reads a relative path from the program's folder and an
  absolute one as the machine's (its test at 272). It would be a new address form, a surface of
  `fetch`, so a panel's; listed because it is what would make *the folder at the project's root* one
  line instead of 14 depth-dependent ones.
- **D3, a third reading of instruction 3**: *the folder at the project's root, something like
  `packages`* as the PLACEMENT folder, `heroes.packages/` renamed `packages/` at each program's root,
  as npm's `node_modules/`. Would have to be true: R7's reason for a dotted name (*placed where no
  `use` can spell them*) survives the rename. It does by the subfolders alone: `use
  packages/core@0.1.0-db7f072b4678b0b1/strings` is refused at the `@` (finding 15). The author said
  *index*, so this is the least likely reading; it is listed because the proposal names two readings
  and the words fit three, and the author may correct the coordinator's.
- **On the name**: the root already holds `vendor/` (`git ls-files vendor`: five files, the
  tokenisers), the word several ecosystems use for the folder the historian is asked about; a root
  `packages/` beside a root `vendor/` that means something else is a reader's question the historian's
  item 4 should name.

### 28. The reader-facing half, and the seats it needs

- Ran: the census of finding 6; `git grep -n -i '"fmt"\|canonical' -- tests/harness/thesis*.hero
  tests/harness/generate*.hero tests/harness/latency*.hero`; `grep -n '^ *use ' spec/heroes-spec.md
  README.md`; `git grep -n '^ *use ' -- 'site/src/**'`; `sed -n 265,290p tests/harness/gen_text.hero`
  and `sed -n 319,360p tests/harness/gen_judge.hero`.
- Printed: 635 of the 1,068 files with a `use` are unsorted by the census's key, in a tree written
  with no order rule, largely by this project's assistant. The thesis instrument names no `fmt`. The
  spec and the README hold no block of `use` lines; the site's `docs/modules.html` (and `it/`) holds
  one, four lines, already in whole-line order. The program generator writes its `use` lines in the
  order of its shapes (`gen_text.hero:273-281`), and its oracle 3 (`gen_judge.hero:319-348`) asks
  that `fmt`'s output be a fixed point that builds and prints what the program printed.
- Verdict: **missing**. (a) What a model writes: with the rule in `fmt` and nowhere a model reads
  (the spec states no order, and the llm-ergonomist reads only the spec), a model's file with several
  `use` lines will usually not be canonical, and `fmt_check.py:349` refuses each such write in this
  repository until `fmt --in-place` runs; the tree's 635 of 1,068 is the measured base rate, at no
  cost. Whether to state the order where a model reads it (the spec, `heroes fmt`'s help, a
  diagnostic) is a question for the seat the sitting left out; a paid run that measured it (the spec
  and a task naming three modules, N samples, the share written sorted) goes to the coordinator, not
  run here. (b) What `check --apply` writes and what a `Fix` inserts: findings 10 and 7. (c) A free
  instrument for question A's premise exists: the generator's oracle 3 already asks *`fmt`'s output
  means what the program meant*, over the 128 seeds the net judges; whether its programs ever hold
  two `use` lines (`gen_cmodule.hero:88` adds one module per shape) is a question the
  compiler-engineer can answer by reading a generated sample before trusting it.

### 29. The lanes a reformat would meet: the brief's command overcounts

- Ran, in the trunk (read only): `git worktree list | wc -l` (49 lines); for four lane branches,
  `git diff --name-only bd4670da4 <branch> -- selfhost/ | wc -l` against `git diff --name-only
  bd4670da4...<branch> -- selfhost/ | wc -l`; `git branch --no-merged bd4670da4`, each with the
  three-dot count.
- Printed: `b23-cli` **145** by the brief's two-dot form, **0** by three dots; `pm-use` 74 against 0;
  `pm-close` 9 against 0; `b25-misc` 0 and 0. Branches not merged into `bd4670da4`: `235-base` 0,
  `b25-misc` 0, `b25-round` 9, `b25-seeddoc` 6, `p237-land-cli` 9, `p237-land-spec` 6, `p238` 0
  (`235-base` with *multiple merge bases*).
- Verdict: **does not reproduce** as a method: `git diff --stat` of a lane branch against
  `bd4670da4` counts the trunk's own changes for every lane behind the trunk. **Changes the brief**:
  ask for `git branch --no-merged bd4670da4`, then `git diff --stat bd4670da4...<branch> --
  selfhost/` (from the merge base), and re-read it on the day the landing merges, since the list
  moves.

## Count

29 findings. **Reproduce** as written or with a minor correction: 16 (1, 2, 3, 4, 6, 7, 8, 9, 11,
12, 13, 14, 15, 18, 19, 20), several with a question beside them. **Do not reproduce**: 5 (5, 10,
17's count, 21, 29). **Missing** from the briefs: 5 (16, 23, 24, 25, 28). **Questions and routes**:
3 (22, 26, 27). Nine change a brief: 5, 10, 16, 21, 23, 24, 25, 28 and 29.

## Repairs recommended to the briefs, most important first

1. **Seat the ffi-pragmatist, or hand its question to the compiler-engineer by name** (finding 21).
   *The change touches no C boundary* is falsified: the order of `use` lines orders the headers of
   the one-file emission and the libraries of the link line, and a sorted order turned a program
   whose `--emit-c` exits 0 into `ffi_one_file`, exit 1 (`build/238/c453/`). `--emit-c` makes the
   seed and judges `compat`. Ask: which programs a sorted `fmt` turns from accepted to refused by
   `--emit-c`, or from linking to not linking on a GNU ld leg (a Linux container, free), and whether
   the rule may land with that class open: a correct program refused is `blocking`
   (`.claude/rules/verification.md` § Bounded discovery).
2. **Name the goldens whose subject is the order** (finding 25): defects 560 and 563's three `run`
   cases are written in the order that once failed, sit in `canonical`'s walk and are unsorted. Ask
   question B for *which goldens are about the order*, the run of the pre-repair compiler on both
   orders that settles whether a reversed case still guards, and the route (exempt, or rename the
   modules so the pinned order sorts first).
3. **Correct F4** (finding 5): `fmt` formats every file the parser accepts, checker-refused ones
   included, and on those the order picks the blamed line, the text, the follow-on diagnostics and
   the `Fix` (`clash_a` against `clash_b`). Question A's premise check must include such files.
4. **Correct F9's use in question B** (finding 10): `check --apply` applies `certain` fixes only;
   the one that writes into a `use` line is `selfhost/parse/use_line.hero:273`, and its output
   (`use zeta` / `use alpha`) is not canonical under a sorted rule. Ask whether `--apply` sorts.
5. **Give question C a seat** (finding 24), and with it the trigger for the spec-warden: spec section
   1 gives a comment two meanings a hoist can change (finding 17), and the site's start page claims
   the tree is already canonical *byte for byte*, which the landing order must keep true at every
   pushed commit.
6. **Widen question B's emissions to `examples/` and `tests/golden/run/`** (finding 23):
   `examples/shapes/main.hero` sorted moves its blessed emission by 50 diff lines; the `emission`
   suite is red at the reformat until they are blessed, in a commit the landing order names.
7. **Replace question D's suite list with the map's command over the moved tree** (finding 16):
   `probe`, `canonical`'s `SOURCE_DIRS`, `compat`, `cards`' `AREAS` and `.gitattributes:46` reach the
   index through `examples` or by its path and are not on the list.
8. **Correct the lane count's command** (finding 29): three dots from the merge base, over `git
   branch --no-merged bd4670da4`; the two-dot form counted 145 files for a lane that changed none.
9. **Name the reader-facing half and what it costs** (finding 28): the tree's base rate is 635 of
   1,068 files unsorted; the hook refuses each such write once the rule lands; whether the order is
   stated where a model reads it is the llm-ergonomist's question, and a paid run that would measure
   it goes to the coordinator with its size.
10. **Hand question A its routes** (finding 26: the bound name as key, the order made immaterial in
    the compiler, `<core>` first refused by CLAUDE.md § 13, the commented block left alone refused by
    design.md:2252, a checker rule refused by Principle 0, and the comment model the printer already
    holds) and **question D its** (finding 27: a key without the index address, which W4's cases do
    not exclude; a repository-relative `file://` form, which does not exist; the placement-folder
    reading of instruction 3; the root's existing `vendor/`).
11. **Smaller corrections**: F5's 3,564 tracked files, its last row's range, the 25 unsorted files of
    `tests/harness` (finding 6); F12's 17 placed files, 14 in `check` and 3 in a `run` case (finding
    13); F16's 8 lines (finding 17); F10's commit refusal in `staged.py` (finding 11); F2's quotation
    differs from the file at the em dash (finding 3); and `npm ci` in a seat's `site/`, so the site's
    build is not unrun by construction (finding 19), with the note that the sitting's own commits
    touch `docs/panel/**`, a site path.
