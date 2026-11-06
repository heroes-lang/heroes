# Panel 234, the compiler-engineer's report

Written as it goes, 2026-11-05. Seat: the ceiling (design.md §1.1, §1.7,
Part 5). Every command below was run in this session, in the worktree
`.claude/worktrees/234-compiler-engineer/` (detached at c935ff008, the
briefs' commit), with the compiler built there from the seed:

```
$ clang -I runtime seed/heroes.c runtime/runtime.c -o heroes
$ ./heroes --version
heroes 0.6.0
sources 1a1e340778db789a
```

Scratch programs live in `.claude/worktrees/234-compiler-engineer-scratch/`,
one folder per run, named beside each result. Line counts are `wc -l`
unless a section says `code lines` (the `layout` suite's unit). A
negative sentence is a command that was run, or it is written as a
question. No clock was started and no paid run. Files were written only in
the worktree (the compiler and its ignored `build/`), the scratch, and this
report; the Linux arm64 runs wrote into the scratch through mounts or into a
`--tmpfs` discarded with the container (Docker's own bookkeeping of a
container is its, in its VM).

## B. Where packages live

**Verdict.** **Approve B2 with B3**: the spelling `use core/<path>`, and the
tree placed under each program's root (only the packages that program
reaches), held byte-identical to one canonical copy by a check of the net
until `heroes fetch` does the placing. No compiler change. **Object** to B1,
B4 and B6. **Veto** B5. Header lookup: **leave it** while no package ships
its own header (item F's answer); price below if one does.

**Section.** design.md §1.1, §1.7 (the ceiling), §1.11 (*"Heroes ships a
minimal runtime and nothing else"*, read with panel 097's condition 5 for
B5); spec § Files and layout; panel 099 R1 and R4 (no climb, a module above
the root belongs to another program).

### What the compiler does today (probes 1 and 2, re-run here)

All seven commands of probes 1 and 2 reproduce in this worktree, word for
word and exit for exit (`runlog.sh`, the scratch's `bin/`, prints the whole
output less blank lines). Module discovery turns a module name into a file
in **one place**: `selfhost/modules.hero:107-116`,
`open_directory(path) + module + ".hero"`, `path` being the compiled file
(`grep -rn -E '\+ "\.hero"' selfhost` finds no other site that builds a
module's path from its name; the rest are mutation fixtures and a test).
`modules.hero` is 435 lines, **303 code lines** by the layout's rule
(`tests/harness/suite_layout.hero:1015-1030`, mirrored in the scratch) against
its decided ceiling, `"selfhost/modules.hero 320"` (`:526`): 17 lines of room,
and its two neighbours were split out when it filled (`module/paths.hero` and
`module/reading.hero` say so in their headers), so a route that changes
discovery by more than a few lines adds a module.

### The C names each route gives (F5, extended)

Scratch `routes/`: the probe's two packages laid out per route, each program
run and its C read with `heroes build main.hero --emit-c`.

| route | layout | runs | C names | `#line` names the package as |
|---|---|---|---|---|
| B1 | `strings.hero`, `net/http.hero` at the root | `true true`, exit 0 | `h_strings_contains`, `h_nethttp_is_get` | `strings.hero`, `net/http.hero` |
| B2 | `core/strings.hero`, `core/net/http.hero` | exit 0 | `h_corestrings_contains`, `h_corenethttp_is_get` | `core/strings.hero` |
| B3 | a copy under `apps/server/` and `apps/client/` | exit 0, each | the same as B2 in each program | `core/strings.hero`, relative to each program |
| B6 | `apps/server/core -> ../../canon/core` | exit 0 | the same as B2 | `core/strings.hero`, the link's path |

Collisions (scratch `collide1/`), each refused loud, none silent:

- B2, a program module `core_strings.hero` beside `core/strings`:
  `error[module_names_collide]: modules core/strings and core_strings are one
  name to the compiler ... both become h_corestrings_…`, exit 1;
- B1, a program module `net_http.hero` beside the package `net/http`: the
  same error, `h_nethttp_…`, exit 1;
- B2, the program's own `strings.hero` used from a second file while
  `main.hero` uses `core/strings`: runs (`true false`, exit 0) and the C holds
  `h_corestrings_contains` and `h_strings_contains`, F5 reproduced.

So **B1 takes every package's name out of every program's root**: a
program's own `strings.hero` *is* the package's path, and adding the tree to
a program that has one either replaces it or is refused, by which file is
there first. B2 leaves the program's names alone and reserves one directory,
`core/`, by convention only: a program's own `core/model.hero` beside the
packages runs (scratch `owncore/`, `true` and `42`, exit 0).

### Case (item C §1.5, measured here because it is discovery's)

This Mac's volume is case-insensitive (`touch probe_lower.txt; ls
PROBE_LOWER.TXT` finds it; `diskutil info /`: APFS). Scratch `case1/`:

- `use Core/Strings` with the file at `core/strings.hero`: **runs, exit 0**,
  and its C is `h_CoreStrings_contains`;
- one program reaching that file as `core/strings` from one module and
  `Core/Strings` from another: **runs, exit 0, and compiles the file twice**,
  `h_CoreStrings_contains` and `h_corestrings_contains`, and `heroes test`
  runs its test twice (`ok "contains"` two times).

Nothing refuses either. `component_of` keeps case (`selfhost/module/names.hero:82-99`
filters with `bytes.is_alnum`), so `Corestrings` and `corestrings` are two
components and `collisions` (`modules.hero:220-258`) sees no clash. **On
Linux arm64, run** (the compiler built from the seed in
`heroes-linux-arm64:latest`, `sources 1a1e340778db789a`, the files copied into
a `--tmpfs` inside the container, which `touch probe_lower; ls PROBE_LOWER`
shows case-sensitive): both programs are **`unknown_module: there is no
module Core/Strings`**, exit 1; and a program holding two files
`core/strings.hero` and `core/Strings.hero`, `use`d as two modules, **runs and
prints `3`**, where this Mac's volume holds one file for both names. So one
tree is a different program on the two machines, silently on each. (A first
run on a bind mount of this Mac's scratch printed `true` and `4`: Docker's
bind mount keeps the Mac's case rule, so a file-name fact measured in the
container on a mounted tree is this Mac's fact.) **What a refusal would cost and refuse
today**: of the 964 distinct `use` paths in tracked `.hero` files (`git
ls-files '*.hero' | xargs grep -h -o -E '^use [A-Za-z0-9_/]+'`), **none**
holds a capital and **none** matches a tracked file only case-insensitively
(each path grepped against `git ls-files '*.hero'` as `(^|/)<path>\.hero$`,
once exactly and once with `-i`: no path matched only the second time). Two
shapes of rule: (a) refuse two modules of one program whose components are
equal ignoring case, a lowered comparison inside `collisions`, about 5 lines
and a message, which catches the twice-compiled file and not the single
`use Core/Strings`; (b) refuse a `use` whose spelling differs from the name
the directory lists, one `hero_dir_scan` per module directory
(`runtime/parts/dir.c`), about 30 to 40 lines in a new module beside
`module/reading.hero`, which catches both. Either is a new refusal, a
diagnostic class (CLAUDE.md § 4), so its own sitting; I would take (b).

### The routes, priced

**B1** (tree at the root, `use strings`). Compiler: nothing. Cost: the
namespace capture above, and the placement limit every route shares but
B3 to B6: **one tree serves only programs whose root file stands beside
it**. The corpus keeps one program per directory
(`tests/harness/suite_corpus.hero:500-507`, `program_directories`, a
`main.hero` per directory), so one tree serves one program, or is copied.

**B2** (`core/` at the root, `use core/strings`). Compiler: nothing. The
same placement limit. It keeps the program's names free and makes the
package visible at the `use` line.

**B3** (a copy under each program's root). Compiler: nothing. **How many
copies, measured**: the program directories of `examples/` that hold a hand
copy of a `strings` or `strconv` operation (`grep -rl -E '^function
(trimmed|is_space|split_lines|split_on|split|ends_with|starts_with|split_words|words_of|number_of|int_of)\('
examples | awk -F/ '{print $2}' | sort -u | wc -l`) are **17**: 16 programs
and `gallery/`. The author's instruction 3(d), *no copy by hand survives*,
moves each of them to the package, so B3 places `core/strings.hero` 17 times
where 448 lines of hand copies stand today (item A). That is more bytes in
the tree than today, and it is the shape `heroes fetch` will produce
(`docs/roadmap/milestones/M-package-manager.md`: *"`heroes fetch` under the
root"*), so the program's `use` lines never change when fetch arrives. What
it owes the net is below (the nine suites); the one owed check is
**byte-identity of every copy to the canonical tree**, about 40 to 60 lines
of harness (a walk of `examples/*/core/` against the canonical path, the
shape of `suite_canonical.hero`'s walk; estimated, not written).

**B4** (a root the compiler knows). Compiler, by flag or by environment:
the fall-back lookup beside `modules.hero:107` (17 lines of room under
its 320, or a new module), the root threaded to discovery's **7 callers**
(`grep -rn -E 'modules\.load_(text|edited)\('`: `cli/input.hero:45`,
`cli/check.hero:196` and `:369`, `cli/consensus.hero:209`,
`mutate/cut_corpus.hero:113`, `mutate/score.hero:124`,
`rename_fit.hero:239`), `unknown_module`'s message naming both places
(`modules.hero:186-209`), a flag in `selfhost/cli/flags.hero` (584 lines),
and the header lookup below becoming **required** (a package's own header no
longer sits under the root's `-I`). About 80 to 150 lines and their goldens,
estimated, not prototyped. Harness: every suite that compiles a program over
a package passes the flag or sets the variable, and `absence.hero`'s
missing-library witness, which looks for the group among the modules under
the program's directory (`suite_corpus.hero:290-296`,
`absence.machine_lacks_among(... modules: p.modules ...)`), must learn the
root, or a machine lacking `sqlite3.h` turns a skip into a red; `compat`
compiles a release's corpus with the newer compiler only, so the flag must
be one every later compiler keeps. Rulings: panel 099 R1 and R4 and its
premise, *no manifest, no search path* (`docs/panel/099-where-a-use-path-starts.md:95-99`,
`:117-146`), are reversed by it; panel 028 R3's trust boundary applies less
than it does to the prelude (a package is checked by the checker like any
module; the prelude's diagnostics are compiler bugs by its own header,
`library_source.hero:56-57` of the text), and frame 6 holds: a reader of one
file no longer learns from the tree which file `use core/strings` opens.
By project file: refused until panel 056's three conditions hold.

**B5** (the tree compiled into the compiler). **Veto.** Panel 097's
condition 5, ratified 2026-08-30, reads *"Nothing reaches
`selfhost/library_source.hero` — the prelude is the standard library
design.md §1.11:443-445 forbids permanently"*
(`docs/panel/097-the-platform-arm-belongs-to-the-runtime.md:129-130`): the
sitting's reason for closing that file is that **source compiled into the
compiler is the forbidden standard library**, and B5 is that reason applied
to twenty packages rather than one file; CLAUDE.md § 13 lists *"A standard
library"* under *Where not to go*. Its price, for the record: the embedded
text costs the seed about **3.4 bytes per byte** of quoted source (the
library's 236 quoted lines, 13,864 bytes, `awk 'NR>=40 && NR<=275'
selfhost/library_source.hero | wc -c`, cost 18,895 bytes of string statics,
a 7,760-byte static array at `seed/heroes.c:31592` and a 20,320-byte
fallback builder at `:31598-31842`), so a tree of 5,000 lines would add
roughly half a megabyte to a 29,043,650-byte seed (arithmetic on one ratio,
an inference); `core` would become a reserved module name, as `library` is
(`modules.hero:224-233`, `module_name_reserved`); a package fix would be a
compiler release; the compatibility promise of `1.0.0` would bind every
package's API; a package shipping its own header (item F's P3) has no file
for clang to open, so the compiler would write it into `build/`; and
`heroes fetch` would have nothing to fetch for `core/`. **What would lift
the veto**: a ratified reading of §1.11 and panel 097 C5 that the packages
are not a standard library when compiled in, which the sitting would have to
write and the author ratify.

**B6** (a link per program). Runs on this Mac (scratch `routes/b6/`, exit 0).
Three costs. Windows: the repository tracks **no** symbolic link (`git
ls-files -s | awk '$1==120000' | wc -l`: 0), and `.github/workflows/ci.yml`'s
checkouts set no `core.symlinks` (grep: no line); what git gives a Windows
checkout without it is a file holding the link's text, and `use
core/strings` would then read a directory that is a file: **unrun, a
question**. The walkers: the runtime's listing walks into a link to a
directory, *"as every listing here always has"* (`runtime/parts/dir.c:131-138`,
`hero_dir_scan` passes `follow` 1), and the harness's `shell.list_tree`
calls it (`tests/harness/shell.hero:725-755`), so `canonical`, `corpus`'s
`module_texts` and `probe` read the canonical tree once per link. And
nothing is gained over B3 that a check does not give.

### The cache under `build/`, and `module_of`

No route changes what the cache shares, because **a unit is keyed per root**
(`selfhost/cli/units.hero`'s header: *"a unit's text names its root source in
its first line, so no two directories met under one key"*). Measured, scratch
`cache1/`, one `build/` for both: the B3 server and client give **two**
objects for `core/strings` (`tu-0d20.../corestrings-...o`,
`tu-62aa.../corestrings-...o`), and a second root file in the server's own
directory a **third**; the library is compiled once per root likewise. So one
tree (B4, B5, B6) and a copy per program (B3) cost the cache the same: one
object per package per root. And **`module_of` names a package tested alone
by its stem** (`selfhost/module/names.hero:66-67`, *"the root's module is its
stem, never a path"*): `heroes test b3/apps/server/core/strings.hero` compiled
`tu-30fe.../strings-...o`, where the same file reached from a root is
`corestrings`; two C spellings of one file in two builds, harmless since no
build holds both.

### The root driver against `heroes test` per package (F31)

Scratch `driver/`, the B2 tree: `heroes test core/strings.hero` runs its test
(exit 0); `heroes test core/net/http.hero` is `unknown_module` (*reads
core/net/core/strings.hero*), so **a package that `use`s another is never
tested alone**; a driver that only `use`s is refused twice
`unused_binding`; a driver naming `http.is_get` alone runs both packages'
tests, transitively (`ok "is_get"`, `ok "contains"`). So the driver names one
thing from each package no other package reaches, and nothing tells a
package it forgot: `heroes test` prints tests, not modules. **Price**: one
file per canonical tree, one `use` line and one named call per top package;
and a check that every `core/**.hero` file is reached, which
`heroes build <driver> --emit-c | grep -o -E '^#line [0-9]+ "[^"]*\.hero"'`
answers in one command (run on the driver above: `core/net/http.hero`,
`core/strings.hero`, `driver_top.hero`). `heroes test` per package costs nothing to build and
reaches the leaves only.

### The nine suites that name `examples` (F12)

Their code, by the verification map's own `code_of` filter
(`.claude/rules/verification.md`), names `"examples"` in `canonical compat
corpus emission probe records special surface warnings` and in `cards`,
`gallery`, `platform_skips`. What each does with **(i)** a canonical tree
under `examples/` with no `main.hero`, **(ii)** that tree with a driver
`main.hero` (conventions §1.6), and **(iii)** a copy inside each program:

| suite | how it walks `examples/` | (i) tree, no driver | (ii) with a driver | (iii) a copy per program |
|---|---|---|---|---|
| `canonical` | every `.hero` under it, `list_tree` (`suite_canonical.hero:69-79`, `:118-120`) | each package held to `fmt` | the same | each copy held to `fmt`, once per copy |
| `corpus` | directories holding `main.hero` (`suite_corpus.hero:500-507`), each `heroes test main.hero` at three levels (`:264-322`), check 2 over `module_texts`, `list_tree` (`:522-532`) | skipped, as `gallery/` is | a corpus program: its driver's tests run three times | **check 2 goes blind**: a program with no test of its own passes on its copy's tests; repair: ask check 2 of the modules outside `core/` (about 5 lines) |
| `emission` | `main.hero` or `whole.hero` per directory (`suite_emission.hero:103-104`, `:265-276`), a blessed emission each | nothing | one blessed emission more | each program's emission carries its packages, so **a package edit re-blesses every program that reaches it** (true of every route) |
| `warnings` | `main.hero` per directory, built at its levels (`suite_warnings.hero:66-67`, `:142`) | nothing | built | copies built inside each program |
| `compat` | every `.hero` that opens `function main(` in a release's `examples/` (`suite_compat.hero:369-371`) | nothing (a package has no `main`) | the driver is a program | nothing new: each release program compiles against its own copy, so **a package change cannot break a released program**, which B4 and B5 cannot say |
| `probe` | every `.hero` under `examples/` as a `multi` seed, floored at 120 seeds (`suite_probe.hero:186`) | each package a seed | the same | **each copy a seed**: the floor moves by copies times files |
| `records`, `special`, `surface`, `cards`, `gallery`, `platform_skips` | fixed files or citation homes (`suite_records.hero:2904`, `suite_special.hero:40-542` name `gallery/00-first.hero` and the calculator, `cards.hero:98`, `:601`, `gallery.hero:28`, `platform_skips.hero:191`) | nothing | nothing | nothing |

### The header lookup (F15)

Reproduced (scratch `nestedheader/`): `core/net/net.hero` with `extern
"net.h"` beside `core/net/net.h` is `ffi_missing_header` from the root (its
note: *install the library's development files*), passes alone, and runs
from the root with `--include core/net`; spelled `"core/net/net.h"` it runs
from the root and is refused alone. Why: the group's header is written
`#include <...>` (`selfhost/emit/c_text.hero:183`), so clang searches the
`-I` list alone, and that list is one per build, the compiled file's
directory last (`selfhost/cli/compiling.hero:73-87`, `search_words`).

**The route nobody listed, priced** (the declaring module's directory first,
C's rule for a quoted `#include`). A per-unit `-I` cannot do it: the fused
unit (`--emit-c`, the seed) holds every module's groups in one file, and a
module's unit also includes the header of **another** module's group record
it reaches (`selfhost/emit/externs.hero:121-129`, `headers_for`). So it is a
**spelling**: a group's header resolved once, where its string is read, to
`<module dir>/<header>` when that file exists under the root. Where that
string is read: `externs.unquoted` has **36 call sites in 25 files**
(`grep -rn -E 'externs\.unquoted\(' selfhost`), and it also reads the
`link` and `package` words, so the resolution is a second function and each
site is re-read to say which one it means; every probe of the program's
headers asks the list `compiling.probe` is handed (`compiling.hero:103-120`),
and the probes must ask the resolved spelling or they judge another header,
the class defects 163, 167 and 168 closed (that file's header). Then
`ffi_missing_header`'s message names the two places it looked, the harness's
`absence.hero` repeats the search it mirrors, and the emission asks the disk,
which the seed survives only while no `selfhost/` directory holds a file named
like a header its modules bind (a premise, written where it is relied on).
About 100 to 200 lines over 5 to 8 files plus a `run` and an `unsupported`
golden, estimated. **Which routes it serves**: required by B4; makes B1, B2,
B3 and B6 independent of where the tree sits; serves nothing under B5 (no
directory). **Leaving it**: a package that ships a header spells it from the
root (`core/net/net.h`), works under B2, B3 and B6 unchanged (the tree sits at
one relative place in every program), and is tested through the driver
rather than alone. Item F's verdict (no package header) makes leaving it
free.

### What `heroes fetch` can later serve without changing a `use` line

M-package-manager says fetch places *"a tree under the root"*. So it serves
**B3**, and B1 or B2 as the spelling inside that tree: `use core/strings`
reads the fetched `core/strings.hero` exactly as it reads the checked copy
today. **B4** it serves only as a machine-wide cache the compiler searches,
which holds one version per machine; pinning a version per program, Goose's
`import x from "owner/repo@v1"` read on that page, writes the version **into
the `use` line**, a change of surface. **B5** it cannot serve: there is
nothing on disk to fetch.

**Prediction.** If B2 with B3 is adopted, `selfhost/` gains no line for
packages at M-core-packages' close (`git diff --stat <opening> <close> --
selfhost` shows no file under `selfhost/module/` or `selfhost/modules.hero`
moved for placement), and the trunk at that close holds at least 10 copies
of `core/strings.hero` byte-identical to the canonical one (`cmp` over
`find examples .claude/hooks -path '*/core/strings.hero'`).

**Condition.** A measured case where a copy drifted past the check, or a
program the corpus must hold that cannot carry its own tree, would move me to
B4 by flag (never by project file); a ratified reading of §1.11 and panel 097
C5 would lift the veto on B5.

## C. What the compiler enforces of the conventions, and what only a check could

**Verdict.** **Approve** the draft as a conventions page, with three
amendments: §2.3 extended to **module bindings** (refuse `use sort` beside
the built-in, priced below, so the compiler's own words *"taken everywhere"*
become true); §1.5 enforced by a check of the net now and by the compiler at
its own sitting; and **one new suite, `packages`**, holding every rule marked
*checkable* below, rather than ten checks scattered over `corpus`, `records`
and `layout`. **Object** to leaving §1.3 a convention with no instrument.

**Section.** design.md §1.1 (simplicity is the ceiling), §4.17 (a diagnostic
carries its repair), spec § Files and layout; CLAUDE.md § 4 (a diagnostic
class is a sitting's).

Scratch: `conventions/`, `builtin_module/`, `case1/`. Every *not enforced*
below is a program the compiler accepted, named.

| rule | standing today | instrument that could hold it |
|---|---|---|
| §1.1 the tree under `core/`, `use core/<path>` binds the last part | binding **enforced** (probe 2; `module_binding_taken` on two in one file) | placement: `packages` (every package file under a `core/`) |
| §1.2 one package, one file; nothing re-exports | re-export **impossible**: `unknown_in_module` (the critic's `reexport/` run) | one file per card: `packages` against item E's cards |
| §1.3 private modules nobody `use`s | **not enforced**: the language has no visibility (no spec line matches `visib|private|pub|export`; design.md:3255 refuses `private`, the critic's grep) | `packages`: every `use core/...` outside `core/` names a path that has a card; or a path rule, `core/<pkg>/internal/`, Go's own word, which needs no list |
| §1.4 a package's C in one module of its own | wrapper-beside-extern **enforced**, `extern_across_modules` (probe 3) | the module's name and placement: `packages` |
| §1.5 no two module names differ only in case | **not enforced**: on this Mac `use Core/Strings` runs against `core/strings.hero`, and one file reached under two spellings compiles twice (`h_CoreStrings_contains`, `h_corestrings_contains`, its test run twice); on Linux arm64 the same programs are `unknown_module`, and two files differing in case are two modules (§ B, run) | `packages` or `records`: `git ls-files '*.hero' \| tr A-Z a-z \| sort \| uniq -d` prints nothing today; the compiler's own refusal (§ B, shapes a and b) is a new diagnostic, its own sitting |
| §1.6 a root driver | the naming **enforced** (`unused_binding`); a package that `use`s another is `unknown_module` alone | completeness: `packages`, the `#line` listing of § B against `find core -name '*.hero'` |
| §2.1 module names lower case | **not enforced** (`use Core/Strings` accepted) | `packages` |
| §2.2 Go's name where the contract is Go's | **convention only** (a reading of a contract); Go's contract on a `T?` is refused `bad_operand` (probe 4) | the reader test; a near-name suggestion, priced below |
| §2.3 a function never takes a built-in's name | **enforced**, `builtin_name_taken`, all eight (F6) | none |
| §2.3 a module may take one | **allowed**, measured below | the compiler, priced below |
| §2.4 parameters named for the call | `needs_label`, `needs_qualifying` **enforced** (probe 5); the names chosen: convention | the cards |
| §2.5 records PascalCase, constants SCREAMING_CASE | **not enforced**: `record point`, `constant max_size`, `function Shout` run, exit 0 (`case_names.hero`) | `packages` |
| §3.1 a failing function answers `T?` | **convention only**: no instrument tells a sentinel `-1` from a value | the cards, read by a person |
| §3.2 codes snake_case, listed on the card | **not enforced**: `fail(code: "Not Found!", ...)` runs (`code_shape.hero`) | `packages`: every `fail(code: "<literal>"` in `core/` against `[a-z][a-z0-9_]*` and the card; a code computed at run time escapes it |
| §3.3 a message says what was asked and found | **convention only** | none |
| §3.4 abort only on a broken precondition | **convention only** (`.must()` is legal in a package) | none |
| §3.5 a C status never reaches the caller as a number | **convention only** | none |
| §4.1 a new value; `@` only by nature | **convention only** | none |
| §4.2 no package prints, exits or reads `args()` | **not enforced**: a function calling `print` and `exit` checks clean (`prints_exits.hero`, exit 0) | `packages`: the three calls named in `core/` outside `log` and `os` |
| §4.3 a C handle as `record ... tag` with `acquires`/`consumes` | the marks' consequences **enforced** (spec § 13: a handle never ended aborts at exit); using them: convention | none |
| §4.4 generic, never `any` | **enforced** by absence: `any` is a built-in function, `error[not_a_type]: any is a function, not a type` (`any_type.hero`) | none |
| §5.1 a doc comment on every function | **not enforced** (`len.hero` in `builtin_module/` has none and runs) | `packages`: a `#` line above each top-level `function` in `core/` |
| §5.2 `test` blocks in every module | **not enforced** (the same file) | `packages`: each `core/**.hero` holds a `test "` (`corpus`'s check 2 is the shape, `suite_corpus.hero:131-139`) |
| §6.1 admission (a measured copy, Goose's test) | **process** | a record beside each package |
| §7.1 a card per package | none | item E |

**The `packages` suite, priced.** One walk of every `core/` tree in the
repository, ten properties (B3's byte-identity, the driver's reach, §1.3,
§1.5, §2.1, §2.5, §3.2, §4.2, §5.1, §5.2) and E's card. On the shape of
`tests/harness/suite_canonical.hero` (209 lines, one walk and one property),
about 300 to 500 lines with its tests, one row in `tests/harness/main.hero`;
estimated, not written. It reads files and builds nothing but the driver's
`--emit-c`.

### §2.3: a module named like a built-in, measured

Scratch `builtin_module/`, a module `sort.hero` declaring `by`:

- `use sort`, then `sort(xs)`, `xs.sort()` and `sort.by(xs, less)` in **one
  file**: all three run, `1 1 1`, exit 0 (`with_use.hero`); the same with a
  module `map.hero`: `map(xs, inc)`, `xs.map(inc)`, `map.twice(xs)` run
  (`with_map.hero`, `2 2 1`).
- The mistakes a reader makes, each refused, none silent: `sort.sort(xs)` is
  `unknown_in_module: module sort declares nothing called sort`, with no word
  that the built-in is `sort(xs)`; `sort(xs, less)` is `wrong_arity`, but only
  after `use sort` is reported unused (the file names nothing from it);
  `sort.by(...)` with the `use` forgotten is `unknown_function: no function
  named by — x.by(…) means by(x, …)` and an `extern` note, **the missing `use`
  never named**; a local named `sort` draws three errors for one mistake, the
  third a `guess` fix *write `sort.by`* on a line that already says `sort.by`,
  the shape `selfhost/module/diagnostics.hero:101-125` records (the
  `window.window.destroy()` fix, downgraded to `guess` by panel 101 R4), so a
  known cost and not a new defect.
- **The compiler's own source has the hazard**: `selfhost/check/walk.hero:57`
  `use check/join` and 23 `join.<x>` on 22 lines beside `lines.join("\n")` at `:2435`,
  the built-in, in one file.

So it is a reader's hazard and not a soundness hole (no wrong program was
accepted), and the compiler contradicts itself: `builtin_name_taken` says the
name *"is taken everywhere"* (`selfhost/resolve/errors.hero:154-158`; the
local's check at `resolve/state.hero:350-360` and the top level's at
`resolve/top.hero:80-94`), while `read_uses` checks a binding against other
bindings and top-level names and never against `builtin_names.taken`
(`resolve/top.hero:130-160`). **Price of the refusal**: about 5 lines in
`read_uses` and a message naming `as`, one `check/` golden; **what it refuses
in the tree today**, by the 43 built-in names (the inventory's `NAMES`,
`selfhost/inventory.hero:141`, and the library's functions) against every
`use` binding of every tracked `.hero` (903 distinct bindings): `use
check/join` in `selfhost/check/decls.hero:30`, `path_end.hero:41`,
`walk.hero:57`, `use ir/print` in `selfhost/cli/compile.hero:35`, and the
probe's `use sort`, so 4 `use` lines of the compiler gain `as` and their 28
qualified uses on 27 lines are renamed (`grep -o -E '\bjoin\.[A-Za-z_]+'`: 1,
3 and 23 in `decls`, `path_end` and `walk`; `print.dump`: 1), about 31 lines
of mechanical change. It shrinks the rule rather than growing it: one sentence, *a built-in's
name is taken everywhere*, with no exception left to state. It is a refusal
that widens, so it owes the census over every golden tree
(`.claude/rules/verification.md` § A new checker rule).

### The near-name suggestion for Go's `index` against `index_of` (price, not land)

`selfhost/resolve/near_names.hero` (483 lines, **280 code lines** by the
layout's rule, `tests/harness/suite_layout.hero:1015-1030`, of a ceiling of
300) offers a candidate within two edits, one under four letters, a
case-only twin winning alone (`nearest_in`, `:40-60`); `index` (5 letters)
and `index_of` (8) are three apart, so nothing is offered, as probe 4 shows.
`unknown_in_module`'s message and its one `guess` fix are
`selfhost/module/diagnostics.hero:85-98` (`not_in_module`, the file at 270
code lines). A suggestion would add a **word-part rule**, the name's
`_`-separated parts opening a candidate's (`index` in `index_of`,
`last_index` in `last_index_of`, `split` in `split_once`), asked only at the
module-member site so locals and fields keep today's lists: about 12 to 20
lines for the predicate, which `near_names.hero`'s 20 lines of room barely
hold (a new module otherwise, as on 2026-09-30), 3 to 5 at the caller, and
**the candidate's signature in the message**, so the reader sees `-> i64?`
and the reason the name differs (the author's rule, said by the compiler):
about 10 to 20 lines and a `check/` golden of two files. Always `guess`:
`rename_fit.hero` already refuses certainty where the written program would
be refused anew (defect 220), and a contract change is that. About 30 to 50
lines in all. **A second suggestion is worth more to packages and nobody
listed it**: the missing `use`. Measured, a plain module `strs.hero` beside
the file and `strs.holds(...)` with no `use strs` draws `unknown_name: nothing
named strs is in scope — the closest names in scope are cstr, sort, ptr` and
`unknown_function ... did you mean fold?` with a `guess` fix renaming `holds`
to `fold` (`m5_missing_use_plain.hero`): forgetting `use core/strings` is the
mistake a model makes most with a package, and the compiler sends it to
`fold`. Pricing it: on the error path only, look for `<q>.hero` under the
root (discovery loads only what is `use`d), about 30 to 50 lines and a
golden. Both are diagnostic changes, a sitting each.

**Prediction.** If §2.3 is extended to bindings, the census at its landing
reports exactly the 5 files named above refused (the 4 of `selfhost/` before
their `as`, and the probe), and no other tracked `.hero` file; checkable at
that landing's census (`.claude/rules/verification.md` § The batch, step 3).

**Condition.** A program the refusal of `use sort` would stop that `as`
cannot repair, or a measured reader test where the binding beside the built-in
cost nothing, would move me to leave §2.3 as a convention of the packages
alone.

## E. The card

**Verdict.** **Approve the third route**: a program of the harness that
**writes** each card from its module and **checks** the stored card against
what it would write, run by the net (the `packages` suite of item C). **Object**
to a card made by the compiler in this milestone, by flag or by verb. A card
written by hand is acceptable only where its one-line text *is* the doc
comment's first sentence, at which point it is the third route with a hand
step.

**Section.** CLAUDE.md § 10 and `.claude/rules/cli-surface.md` (the stopping
rule, *"a capability enters the surface only if the fixpoint invocation, the
golden harness or the design.md Part 11 harness must type it, or it has a
measured Part 11 effect"*; *"a new top-level verb needs a proven overload"*);
design.md §1.1.

**What a card needs, and where it already is.** Its `use` line, each
function's signature and one line, each code (conventions §7.1). Measured:

- `heroes check <module> --dump-scopes` prints names and parameter names,
  no type, no result, no code, no comment (F32, re-read: `selfhost/cli/table.hero:91`
  says *"print the symbols and every binding"*);
- `heroes parse <module> --dump-ast` prints **every signature with its types
  and its doc comment** (`function index_of(s: str, part: str) -> i64?` and
  `doc # Where part first starts in s, failing not_found.` for the probe's
  package), a variant's arms and fields (`examples/json/value.hero`), and the
  literal `fail(code: "not_found", ...)` in the body; its spelling is the
  dump's (`case boolean` / `field b: bool`), not the source's;
- **the canonical source text alone carries all of it**: no function header
  in `selfhost/`, `examples/` or `tests/harness/` ends on an open bracket
  (`grep -h -E '^function [A-Za-z_0-9<>, ]+\($'`: 0), the longest is 307
  characters on one line, and the `canonical` suite holds every file of
  `examples/` to `fmt`, so a card's signature lines are the module's own
  `^function ` lines, its one line the comment above each, its codes the
  `fail(code: "` literals of the body.

What no route has without an analysis: the codes a function can fail with
**through `?`**, which are its callees' and theirs; the compiler could compute
them over the call graph (a fixpoint, about 100 lines, unpriced in detail),
and nothing else could. Conventions §5.1 already asks the doc comment to name
them, so the third route checks the literal ones and the doc comment carries
the rest.

| route | cost | tells a drifted card | § 10 |
|---|---|---|---|
| the compiler: `check --dump-interface`, or `heroes doc` | a printer of declaration headers and first doc sentences in a new `cli/` module, reusing `print/` to spell them, about 100 to 150 lines, a row in `selfhost/cli/table.hero` (13 `Command(` rows today, `:155-167`), a dispatch, goldens; `heroes doc` is M-doc-generator's (row 79) | on demand, yes; a stored card still needs a check | a flag only if a harness must type it, and the thesis harness can read a stored card, so no; a verb is no overload of an existing one, so the author's decision alone (as `probe` entered) |
| by hand, held by a check | 20 cards written by hand; a check of signatures and codes, about 100 to 150 lines of harness | signatures and codes yes; the one-line text **no**, unless it is the doc comment's first sentence | admitted, no surface |
| a program of the harness writes and checks | about 150 to 250 lines (`write`, `check`, their tests), the shape of `tests/harness/latency.hero`; **named apart from `tests/harness/cards.hero`**, which is the issues' areas (887 lines) | **yes, by construction**: the module is the one place the card's truth lives | admitted, no surface |

**The budget.** `heroes measure <card>` counts any file with the two vendored
tables and says *"No ceiling judges ... this is a count and not a verdict"*
(run on the probe's package: `maximum 127`, *a lower bound, not the reader's
tokeniser*); the real tokeniser (`--refresh`) is pinned to `spec` and
`contract` (`selfhost/measure/pinned.hero:106-111`, F25). So a card's budget
is stated in the vendored `maximum` and judged by the harness program, or
`measure` learns a third judged path, which is a change to
`selfhost/measure/judged.hero` and its own question.

**Prediction.** Under the third route, no file under `selfhost/cli/` changes
for cards at M-core-packages' close (`git diff --stat` over `selfhost/cli`
between the opening and the close shows no card printer), and the `packages`
suite refuses a card one signature behind its module, at the first package's
step.

**Condition.** A card property the module's text cannot carry (a code
reached through `?` that a reader must see and that a doc comment is measured
to miss) would move me to the compiler's route, by a flag, at M-doc-generator.

## F. The platform layer, its cost in the compiler and the runtime

**Verdict.** **Approve P1'**: the runtime's one object behind a **family of
headers** (`hero_net.h`, `hero_time.h`), each named by the group that binds
it and **none of them a seed**, every one listed in `runtime.c` and the
runtime's coverage test extended to headers. **Object** to P2 and P2' (a
second object), to P3 for the platform's C (it owes panel 114 R3's second
instrument, unmet today, measured), and to P4. **Separately, approve as a
task**: dead-stripping the link, measured below on both machines here,
which answers P1's growth for every program at once.

**Section.** design.md §1.11 (*"a minimal runtime"*), §4.19 (a wrong FFI
signature is a compile error), §1.12; CLAUDE.md § 7 (`HERO_RUNTIME_ABI`);
panel 097 (the platform arm belongs to the runtime), panel 114 R2 and R3.

### How the runtime is found, keyed, compiled and linked, today

`runtime.c` is one translation unit including every file of `runtime/`
(7 headers, each named in its list: `grep -c '^#include "<name>"'` is 1 for
each of `ls runtime/*.h`; 29 parts, 13,070 lines, `wc -l runtime/parts/*.c`).
Its key is each of those files keyed alone and joined, the list read from
`runtime.c` and not a glob (`selfhost/cli/runtime_key.hero:54-63`); its object
is compiled once per fingerprint, compiler, level and key into
`build/runtime-<key>-<listing>.o` (`selfhost/cli/toolchain.hero:267-309`), its
dependency record **skipping the runtime's own directory** as already keyed
(`:296`, `skip: t.runtime + "/"`); every unit `_Static_assert`s
`HERO_RUNTIME_ABI` (32, `runtime/heroes_runtime.h:108`;
`selfhost/emit/runtime_abi.hero`). Thirteen modules name the object, its key,
its places or its stamp, 47 sites (`grep -rn -E 'runtime_object\(|runtime_key\.|each_file\(|HERO_RUNTIME_ABI|runtime_places\.'`,
asserts and comments left out: `toolchain` 15, `emit/runtime_abi` 10, `link`
6, `runtime_places` 3, `runtime_key` 3, and eight more with one or two).

**The linker drops nothing today.** Scratch `nm/`, `function main() /
print(1)`: the binary is **237,992 bytes with 525 defined text symbols, 522
of them the runtime's** (`nm -U hello`), `hero_thread_spawn`,
`hero_dir_scan`, `hero_file_write` among them. The same three objects
relinked by hand without a flag give the same 237,992 bytes; with
`-Wl,-dead_strip` **78,128 bytes and 63 runtime functions**, and it still
prints `1`. On Linux arm64 (`heroes-linux-arm64:latest`, Debian clang
22.1.8; the program's fused C and `runtime.c`): **235,312 bytes and 515**
plain, **79,760 and 63** with `-ffunction-sections -fdata-sections` and
`-Wl,--gc-sections`, both printing `1`. Windows: unrun. **The compiler
already knows the words**, for one link of its own: the value probe of a
header constant links with the runtime alone and leaves libraries out by
`leave_out(target)`, `-Wl,-dead_strip` on Darwin and `-Wl,--gc-sections` on
Linux, compiling its probe with `-ffunction-sections -fdata-sections`
(`selfhost/cli/value_run.hero:16-21`, `:136-143`, `:155`; its header records
the ffi-pragmatist's corpus measurement, 84 of 84 and 88 of 88, and on
Windows `/FORCE:UNRESOLVED`). `grep -rn -E
'dead_strip|gc-sections|function-sections|data-sections' selfhost` finds them
in that file and nowhere else: a program's own link and the runtime's
compile use none of them.

### P1 in the other direction: what the one object grows by

Whatever joins the runtime joins **every** binary while the link keeps the
whole object: the platform-facing parts today (`os.c` 1018, `replace.c`
1069, `run.c` 1643, `spawn.c` 444, `fs.c` 359, `dir.c` 317, `write.c` 270,
`thread.c` 99) are 5,219 of the 13,070 lines, and `print(1)` carries all of
them. Sockets and clocks with three arms each would add several hundred lines
more to every program (an estimate; nothing written). **Dead-stripping
answers it for every route at once**: `leave_out`'s words reused on the
program's link (`selfhost/cli/link.hero`, `link_line`, `:151`), and on ELF
`-ffunction-sections -fdata-sections` in the units' and the runtime's compile
words (`selfhost/cli/flags.hero`, `level_words`), since Darwin's
`-dead_strip` drops the runtime's functions from the object as it is
compiled today (measured above) and `--gc-sections` does not: run in the
container, the runtime compiled as today and the link with `--gc-sections`
gives **235,184 bytes and 515 runtime functions**, `hero_thread_spawn` kept.
About 10 to 20 lines.
Its cost is the verification map's for that file: *every unit's and the
runtime's key moves, so every build directory goes cold once*, and `run`
whole and `cache` alone judge it. Its risk is a symbol reached only by name;
the corpus under it is unrun here.

### P1': one object, a family of headers, and the seeds

Compiler cost: **none**. The emitter seeds every unit with `heroes_unit.h`
and `hero_os.h` before any group (`selfhost/emit/externs.hero:86-87`,
`SEEDS`), because *"the generated main calls hero_args_set"* (confirmed:
line 212 of `hello`'s C, `hero_args_set(argc, argv);`, declared at
`runtime/hero_os.h:116`). So the family must stay **out of the seeds and out
of `hero_os.h`'s includes**: a member `hero_os.h` included would be a seed by
inclusion, and every unit would see it whatever its groups name, F9's shape
widened. Kept out, a group binds `hero_socket_open` only by naming
`hero_net.h`, which is item I's per-group authority for the new functions at
no cost. Two premises it must keep, each with a line: every family header is
named in `runtime.c`, or neither the key (`runtime_key.each_file`) nor the
dependency record (`toolchain.hero:296`, which skips the directory) sees an
edit of it, a stale object; and the coverage test that holds this today for
parts only (`runtime_key.hero:179-197`, *"every file under the runtime's parts
is included by runtime.c"*) extends to `runtime/*.h`, about 6 lines.
`HERO_RUNTIME_ABI` moves with each header, as CLAUDE.md § 7 says.

### P2 and P2': a second object

**What the compiler knows at link time**: everything. `produce.hero` holds
the program's declarations when it asks for the runtime's object
(`selfhost/cli/produce.hero:127-133`, `runtime_bound.stopped(s,
art.must().decls, ...)` then `toolchain.runtime_object`), and
`externs.headers` lists every header a group names, so *"linked only where a
group names its header"* is a predicate of about 10 lines. The object itself
costs a parallel of most of the 47 sites: an object function of
`runtime_object`'s shape (43 lines), a key over its own sources, its own
`_Static_assert` stamp and its check in `emit/runtime_abi.hero`, its place
and its release check (`runtime_places.hero`), `doctor`, the link list;
about 150 to 250 lines. **And the author's split moves processes, files,
directories and spawning, which the compiler itself binds** (12 modules of
`selfhost/` hold an `extern "hero_os.h"` group, `grep -rl`), so the
compiler's own build would link the second object and the one-line bootstrap,
`clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`, changes: it
stands in **307 tracked files, 351 times**, of which 24 are living (`site/`
10, `.claude/` 7, `.github/` 3, `tests/`, `seed/`, `README.md`, `CLAUDE.md`
one each) and 283 dated records under `docs/` (`git grep -l -E
'seed/heroes\.c runtime/runtime\.c'`). What it buys over P1' with
dead-stripping: no byte (the stripping drops the same code) and no check (the
C's arms are compiled on each CI leg either way). **P2'** (a second object
behind `hero_os.h`): `hero_os.h` is a seed and declares `hero_args_set`,
which every `main` calls, so an object holding it is linked into every
program, and *"only where a group names it"* cannot be asked of a seed.

### P3: a header of `static inline` functions a package ships

**No compiler change, confirmed**: 567 headers under `tests/golden/` (F17),
the `run` group's 262 of them running; and **the cache follows the header**:
scratch `nestedheader/`, `main2.hero` built and run (`42`), the shipped
header edited to return 43 and nothing else touched, rebuilt and run: `43`;
four dependency records under `build/` name `core/net/net.h`
(`grep -l`), so a unit is recompiled when its header moves. Its placement
meets item B's header lookup (F15): root-relative spelling under B2 and B3.
**What it owes**: panel 114 R3, *"R2 does not land without them"*
(`docs/panel/114-the-question-was-not-which-platform.md:192-212`), measured
here:

- the third instrument **stands**: a shipped header with a syntax error is
  `error[ffi_header_refused]: shipped.h, the header this group names, does
  not compile: clang refuses it at line 2: expected ';' after return
  statement`, exit 1 (scratch `r3/syntax.hero`), not an internal error;
- the second **does not**: `twoarm.h` declares `armed` as `int32_t
  armed(int32_t)` under `#if defined(_WIN32)` and `int64_t armed(int64_t)`
  otherwise; `function armed(x: i64) -> i64` **builds and runs, `42`, exit 0,
  and nothing says the Windows arm was never checked** (`r3/twoarm.hero`).
  What it would take: after a build, for each header the program ships, the
  conditional regions of its text (`#if`/`#ifdef`/`#elif`/`#else`, nested,
  the include guard excepted) intersected with the names its groups declare
  and with the lines `clang -E` kept, each declared name found in a dropped
  region told as *checked for this machine's arm only*; a new `cli/` module
  of about 150 to 250 lines, a warning's shape (exit 0), goldens in
  `warnings`, and §4.19's promise said with its limit. Cross-preprocessing
  for the other legs' targets needs their system headers, which this Mac
  lacks, so the instrument reports rather than checks.

**The runtime routes owe none of R3**: `runtime/hero_os.h`'s only
conditional is its include guard (`grep -c -E '^\s*#\s*(if|ifdef|ifndef|elif)'`:
1, line 31), so every declaration is one prototype on every platform, a
binding of it is checked against that prototype on each CI leg, and the arms
live in `runtime/parts/*.c`, compiled and run by each leg. That asymmetry,
about 150 to 250 lines owed by P3 and none by P1', is the ceiling's case.

### P4: `compile "shim.c"`, either shape

A per-package C unit is a new artifact the compiler compiles, keys, records
and links: P2's plumbing once per package rather than once, plus panel 036's
spec-warden veto on the group clause (`docs/panel/036-the-ffi-ladder.md:39`,
`:153`). For the core packages P1' leaves it nothing to do, which is issue
927's own closing condition (*"if it does not appear in a milestone about
packages that compose, that is the answer"*).

**Prediction.** If P1' is adopted, M-core-packages closes with **no new
object** in `build/` beside `runtime-*.o` and the unit objects (`find build
-name '*.o'` after `heroes build` of the loopback server shows one runtime
object), and `HERO_RUNTIME_ABI` above 32; if dead-stripping lands, `print(1)`'s
binary on Darwin arm64 stands under 100,000 bytes (`stat -f %z`).

**Condition.** A platform capability whose C cannot stand behind one
prototype per function (a type that differs by platform in a **signature**,
not inside a part) would put R3's instrument in front of P1' too, and move me
to price it as owed there; a measured cost of the runtime's growth that
dead-stripping does not remove would reopen P2.

## I. A group's header

**Verdict.** **Approve** writing into spec § 13 what runs, sharpened by one
check: **a group's declarations are checked against the module's headers
read in the order its groups are written, after the language's two**, and
a name declared only by a header written **after** its group is refused
(the in-order check). It refuses **nothing in the tree today**, measured.
**Object** to both *alone* readings (a group's header by itself, with or
without the seeds): each refuses the two goldens of § 13's own sentence
*"one that needs another's names comes after it"*, so each contradicts the
spec it would enforce.

**Section.** spec § 13 (`spec/heroes-spec.md:357-360`, *"C reads a module's
headers in the order its groups are written: one that needs another's names
comes after it ... clang checks every result type, constant and record field
against that header"*); design.md §4.19.

**What the compiler does, measured.** A unit opens with `heroes_unit.h` and
`hero_os.h`, then every group's header between the guards (read off
`--emit-c`, lines 2-4 and 21-23 of the unit), and an undeclared name is read
from clang's verdict on that whole unit (`selfhost/emit/ffi_declared.hero`'s
header, *"unknown_name ... matches clang's own call to undeclared
function"*). So the authority is **the module's union plus the seeds, in any
order**: scratch `later/later3.hero`, `extern "a.h"` declaring `fb`, which
only `b.h` declares, and `extern "b.h"` written after it: **runs, prints 6,
exit 0**.

**The count, by a command.** `bin/pergroup.py` in the scratch extracts every
`extern "<header>"` group of the tracked `.hero` files and the functions and
constants under it (records not asked), and compiles one probe per group with
`clang -fsyntax-only`, each name asked `(void)(<name>);`: once with the
group's header alone, once after the seeds as a unit opens; the program's
root and the case's group directory on `-I`, as the compiler's search puts
them. Over the 488 groups of `selfhost` (19), `examples` (22),
`tests/harness` (7) and the golden groups `run` (433), `emit` (1) and `ir`
(6, whose cases are lowered and never compiled):
3 are unrun here (two `sys/prctl.h` groups and a header including it, Linux
only), and 1 is left out (`tests/golden/ir/owned-cell.hero:43` binds
`describe` from the system's `<err.h>`, which declares none; an `ir` golden
is never compiled, and a build would refuse it today). Then:

| reading | refuses in the tree | which |
|---|---|---|
| per module and the seeds, any order (today) | 0 | none |
| **per group, in written order, with the seeds** | **0** | (refuses `later3.hero`'s shape, which nothing in the tree has: every group the alone probe refused depends on an **earlier** header, and both compile in order, `inorder-2056.c` and `inorder-563.c`, exit 0) |
| per group, in written order, no seeds | 8 | the run goldens whose headers use the runtime's types without including them, `fixedbugs-245` (2), `-314` (3), `-462` (3): `unknown type name 'HeroStr'`, `HeroMapHeader`, `HeroArrayHeader` |
| per group alone, with the seeds | 2 | `fixedbugs-2056-...-header-s-own.hero:17` (`EXTRA` exists only under the switch group's macro) and `fixedbugs-563-...-where-it-is-written.hero:15` (`user.h` needs `need.h`'s `Need`): **the two goldens of § 13's order sentence** |
| per group alone, no seeds | 11 | `selfhost/cli/process.hero:66` (`hero_dir_at`, `hero_dir_release`, `hero_dir_scan` under `stdlib.h`), the 8, the 2 |

**What this changes about the witness.** `process.hero:66` is refused only by
the last reading. **In written order it is legal without any seed**: its
module's `extern "hero_os.h"` group stands at `:40`, before it. The seeds'
own witness is the critic's `pergroup/` program, a module whose only group is
`stdlib.h`; only a reading without the seeds refuses it, and that reading
also refuses 8 goldens that test the runtime. So the milestone page's
alternative, *"the spec narrows to what runs, or the check tightens and
`process.hero` is the first thing it refuses"*, has no third branch in which
the spec's order sentence stands and `process.hero:66` is refused.

**Cost of each check.**

- **The in-order check, by emission**: write each group's declaration checks
  right after its own `#include`, so clang itself reports a name used before
  the header that declares it; no clang run added. In `emit/externs.hero`
  (251 code lines) and `emit/macro_guard.hero`, about 20 to 60 lines,
  unprototyped: the guards wrap every group's include today, so where a
  check may stand between two includes is the part to prove. Every emitted
  unit with a group moves, so the seed and the blessed emissions move once,
  and `emission`, `emit`, `determinism`, `wholes`, `descriptors` and `lldb`
  judge it (`.claude/rules/verification.md`, the `selfhost/emit/**` row).
- **Any per-group check, by probe**: the machinery exists on the failure
  path, `selfhost/cli/header_alone.hero` (61 lines, `compiles(t, under,
  headers)`, every probe opening with the seeds); a success-path probe per
  group of each module holding two or more, its verdict mapped back as
  `ffi_declared.hero` maps `unknown_name` (that file at 282 code lines of
  300), and a message naming the header that does declare the name: about 80
  to 150 lines in a new module, goldens, and one clang run per group per
  cold build.

**Prediction.** If the in-order check lands, the census at its landing reports
no refused file among the tracked `.hero` programs that build today, and the
run goldens of defects 563 and 2056 keep passing unchanged.

**Condition.** A program in the tree whose group's name is declared only by a
header written after it would make the in-order check a refusal with a
repair to price; a sitting ruling that the seeds are not part of any group's
authority would move me to the no-seeds reading in order, with its 8 goldens'
headers including the runtime's header.

## K. design.md's repairs (issue 970), each confirmed by its own command

**Verdict.** **Approve** all six as the shared brief states them, and add a
clause to the `#include` paragraph (below). Compiler cost: none; these are
sentences.

**Section.** design.md §1.11, §3.5, §4.1, §4.19, Part 7 item 4.

1. **Done, the `#include` order** (panel 204): `sed -n '1019,1023p'
   docs/design.md` reads *"C reads the module's headers in the order its groups
   are written (§4.19), so a header that needs another's names is written below
   it"*, and `:2583-2600` the whole order. **Does `:2583`'s paragraph name the
   seeds? Yes**: `:2586-2588`, *"A module's unit opens with the compiler's own
   headers, its prefix: `heroes_unit.h`, which reads `heroes_runtime.h` ... then
   `hero_os.h` (`SEEDS` in `emit/externs.hero`)"*, and `:2592-2593` name
   `heroes_standard.h` after the guard's close. **What it does not say is the
   consequence item I measured**: every group sees the seeds' declarations
   whatever header it names, so `hero_dir_release` binds under `stdlib.h`.
   `grep -n -i -E 'whatever (header|its group)|names no header|seed.*authority|any group may bind'
   docs/design.md`: no line (exit 1), a negative on those words only. The
   repair: one clause after `:2588`, *"so a group may bind a name of either
   whatever header it names"*, or item I's in-order sentence, whichever the
   sitting adopts.
2. **Owed, Part 7 item 4**: `grep -n 'no aliases, no package hierarchy'
   docs/design.md` → `:3426`. False twice: `use syntax/decl as sd` binds `sd`
   (`spec/heroes-spec.md:28`) and paths are a hierarchy since M-package-layout.
3. **Owed, §4.1**: `grep -n 'there are no aliases and no wildcard'` → `:896`;
   the same `as`.
4. **Owed, §3.5**: `sed -n '870,886p' docs/design.md | grep -n -i -E
   'return|condition|project file|panel 056'`: no line (exit 1). The section
   says *"a v1 dependency is a link flag declared in the source"* and names
   `heroes add`/`heroes fetch`, and nothing of panel 056's three return
   conditions, which `docs/roadmap/milestones/M-package-manager.md` carries.
5. **Owed, §1.11's Tier-2 list** (`:521-524`): `map, filter, fold, find, any,
   all, range, the T? helpers, read_file, write_file, args`. The library's
   functions today (`awk` over `LINES` in `selfhost/library_source.hero`):
   `range map filter fold find any all validated read_file write_file args
   args_checked exit`; so the list omits `validated` and `args_checked`, both
   entered 2026-08-24 (`git log --reverse -S'"function <name>('`: `ff7f9d57a`,
   `1998b6f54`), **and `exit`** (item 6).
6. **Owed, §1.11's edge sentence** (`:534-535`, *"The one edge that cannot be
   Tier 2 is `exit(code)`"*): `git log --reverse -S'"function exit('` →
   `7327bd9bf 2026-08-18`, F28's date confirmed; task 2250 changed its contract
   (`library_source.hero`, the comment above `function exit`) and not its tier.

**Prediction.** After the repair commit, `grep -n -E 'there are no aliases and no wildcard|The one edge that cannot be Tier 2' docs/design.md`
prints nothing (the two living sentences of §4.1 and §1.11 corrected in place).

**Condition.** None for 2 to 6; for 1, the sitting's ruling on item I decides
which clause is written.

## M. `sort_by`: the library, a package, or neither

**Verdict.** **Approve the package**, under item B's route: an ordinary
generic in a `core/` package, which the compiler reaches as every program
does, **by carrying a checked copy under its own root** (`selfhost/core/`),
and named so that its binding is no built-in's (§ C: `use core/sort` binds
`sort` beside the built-in `sort`). **Object** to the library: it reopens
panel 097's condition 5 and the file has no room. **Object** to *neither*: five
hand-written comparator sorts stand in `selfhost/` today, measured.

**Section.** design.md §1.11 and panel 097 C5 (ratified 2026-08-30); panel
137 (ratified 2026-09-13, issue 1154); panel 068 R4 (*"`sort_by(xs, less)` is
priced at +47..+55 and refused for now"*, recorded at
`tests/golden/check/sort-without-an-order.hero:20`, a third ruling the brief
does not name); CLAUDE.md § 2.

### Issue 1154's four sorts, re-verified

`gh issue view 1154` names `selfhost/modules.hero:167-190`,
`resolve.hero:82-101`, `resolve/cycles.hero:77-98` and `ir/verify.hero:150-168`
(24, 20, 22 and 19 lines, 2026-09-13). Today:

| 2026-09-13 | today | command |
|---|---|---|
| `modules.hero:167-190` | **still hand-written**, `:149-170`, 22 lines, *"Insertion sort by span start — the stable-sort idiom"* | `grep -n 'Insertion sort' selfhost/modules.hero`; `sed -n '149,170p' \| wc -l` |
| `resolve.hero:82-101` | gone: `diag.by_position` (`selfhost/diag.hero:275`) orders by `stable.order` | `grep -n by_position selfhost/resolve.hero` → `:119` |
| `resolve/cycles.hero:77-98` | gone: `extents` moved to `resolve/constant_body.hero:40`, on `stable.order` (`:48`); `cycles.hero:94-99` calls the built-in `sort` on `[i64]` | `grep -n -E 'extents|stable\.order'` |
| `ir/verify.hero:150-168` | gone: the built-in, `sort(blk.preds)` at `:139` | `grep -n sort selfhost/ir/verify.hero` |

**The compiler took a route of its own**: `selfhost/stable.hero`, 105 lines,
`order(by: [i64]) -> [i64]`, a stable order of indices by an integer key, no
comparator; `use`d by 8 modules, 12 calls (`grep -rl '^use stable$'`;
`grep -c 'stable\.order('`). So panel 137's prediction, *at most 20 lines
added, at least 60 deleted across the four*, **cannot be met against the
four**: 22 of their 85 lines remain.

**What a comparator sort would delete today.** Searched by name and by
comment (`git grep -n -i -E 'insertion sort|merge sort|bubble|selection
sort|^function [a-z_]*(sort|sorted|merge)[a-z_]*[(<]'`, so a sort written
without those words is not counted), the hand loops that order by something
other than an integer key:

| where | lines | orders |
|---|---|---|
| `selfhost/modules.hero:149-170` | 22 | diagnostics by span start |
| `selfhost/emit/gate.hero:153-170` | 18 | gate notes by span start |
| `selfhost/check/contracts.hero:214` `sorted` | 18 | names by its own `before(a:, b:)` |
| `selfhost/mutate/mutant.hero:53` `ordered` | 14 | hunks by `(start, end > start)` |
| `selfhost/mutate/hunks.hero:102` `sorted` | 12 | placed hunks |
| `tests/harness/suite_compat.hero:265` `sorted_versions` | 12 | versions |
| `examples/query/run/engine.hero:114`, `:140` | 22 + 9 | records, a merge sort, *"`sort` ... cannot order records"* (its `:16-20`) |

**84 lines in five places of the compiler**, 12 in the harness, 31 in one
program. Each becomes one call and a top-level `lt` of two or three lines,
since a function value here is a top-level function and captures nothing; for
the same reason `stable.order`'s callers, which sort indices by a key array,
stay as they are. Panel 137's figures, restated against today: about 84
deleted, 25 to 35 written back (the five `lt`s and calls), net **about 50 to
60** in `selfhost/`.

### The three homes, priced

**(a) The library** (`selfhost/library_source.hero`). About 15 to 25 lines of
library text, a stable merge sort over `lt(a:, b:)` (panel 137's form; the
2026-10-25 re-check ran it as an ordinary generic, `1 2 3`), **appended after
`exit`**, since every line above shifts every program's `#line "<heroes
library>"` (the file's own header, `:10-15`). No checker or emitter change.
But: **the file is at 298 code lines of 300** (§ C's count by the layout rule;
`layout` narrowed to it, 4 passed, 0 failed), with no `DECIDED` row
(`tests/harness/suite_layout.hero:506-529`), so it needs a decided ceiling or
a split of `LINES`; the name enters the spec's built-ins (panel 068's
+47..+55, the warden's to re-measure) and design.md §1.11's list; no program
defines `sort_by` today (`git ls-files '*.hero' | xargs grep -l -E
'^function sort_by[(<]'`: 0), so taking the name refuses none; a library
function need not enter `selfhost/inventory.hero`'s `NAMES` (`validated` and
`args_checked` are not there). **And it reopens panel 097's condition 5**,
*"Nothing reaches `selfhost/library_source.hero` — the prelude is the
standard library design.md §1.11 forbids permanently"*, which panel 137 never
cited (F27): the sitting would have to rule that C5 does not bind it.

**(b) A package.** `core/<name>.hero` with `by<A>(xs: [A], lt: ...)`, about
20 to 30 lines and its tests: no built-in, no spec line, C5 untouched, frame 1
kept. **The compiler cannot `use` it from outside its root** (probe 1; the
compiler is the program rooted at `selfhost/main.hero`). Under B5 it would
reach it from its own embedded table, and B5 is vetoed (§ B). **Under B3 it
needs nothing new**: a copy at `selfhost/core/<name>.hero`, held
byte-identical by the `packages` check like every program's copy, `use
core/<name>` from any module of the compiler reading it from `selfhost/`
(the base is the compiled file's directory). Cost: the copy's lines in
`selfhost/` and in the seed, the five call sites. **The name**: Go's package
is `sort`, and `use core/sort` binds `sort` beside the built-in (§ C,
measured: three meanings of one word in a file); Go's own later home for a
comparator sort is `slices` (`slices.SortStableFunc`, Go 1.21, a fact for the
historian to verify, not run here), whose binding is free.

**(c) Neither.** The 84 + 12 + 31 lines stay, and `examples/query` keeps the
reason a model writes a merge sort by hand.

**Prediction.** Under (b), at M-core-packages' close `grep -c '"function
sort_by' selfhost/library_source.hero` is 0, `selfhost/core/` holds the
package's copy, and `git diff --stat <opening> <close> -- selfhost/modules.hero
selfhost/emit/gate.hero selfhost/check/contracts.hero selfhost/mutate/mutant.hero
selfhost/mutate/hunks.hero` shows at least 50 lines deleted net.

**Condition.** A ratified ruling that panel 097's C5 does not bind a generic
of the library, with a decided ceiling for the file, would make (a) as cheap
as (b), and I would weigh it again on the spec's tokens alone.

## N. The instrument: can a thesis task carry modules beside its answer?

**Verdict.** **Not today, measured by reading the grader; approve the
smallest route**: the run, not the task, places the canonical tree beside
each answer, by a word of the instrument for the *after* run, and the cards
reach the model as a variant context (a path the instrument already takes).
No compiler change.

**Section.** design.md Part 11 (the thesis harness); panel 226 R11 (the
sandbox).

**What the grader does.** `tests/harness/thesis_grade.hero`: the
pre-registered extraction rule, *"the LAST fenced block ... that holds
`function main` ... A block's text is written as `answer.hero` in the
sample's scratch and nothing else is read of the answer"* (`:29-32`);
`graded` refuses a program that binds C (`refused_extern`, `:144-147`),
writes `answer.hero` (`:149-150`), runs `heroes check` and `heroes build` on
it in the sample's scratch, and runs the binary under the sandbox
(`tests/harness/thesis_sandbox.hero`, which confines the **binary** to its
scratch and has no say over compiling). A task is `task.md`, `cases/`,
`reference.hero` and `mutants/` (`ls tests/thesis/03`); the `Task` record
holds number, dir, prose, cases, two digests and a canary
(`tests/harness/thesis_tasks.hero:35-44`), and **no field for modules**. So an
answer that writes `use core/strings` is read from the scratch, where no
`core/` stands: `unknown_module`, graded `compile_error` (probe 1 is that
refusal; the grader itself was not run here, since a run grades a sealed
suite).

**What it would take.**

- **Place the tree**: a word of the run naming the tree, the scratch's copy
  of it written before `check` in `graded`, about 10 to 20 lines in
  `thesis_grade.hero` and `thesis_args.hero` with their tests; or a task
  field reading `<task>/core/**`, about 20 to 30 lines in `thesis_tasks.hero`
  and the task's digest covering it, which makes every task carry a copy
  (item B's B3 again) and is the worse of the two for a before-and-after of
  **unchanged** tasks.
- **Tell the model**: a context is `spec`, `none` or a variant
  specification's path (`thesis_prompt.hero:21-88`, the shared brief), so
  *spec plus the cards* is a file and no code.
- **What it must not touch**: the extraction rule (still the answer's last
  block, still nothing else read *of the answer*), and `refused_extern`, which
  reads the answer's text alone (`binds_c(text)`), so a package over C in the
  placed tree is compiled and linked while the answer may still not bind C;
  an `extern` of the tree is never callable from the answer
  (`extern_across_modules`, probe 3). That the placed tree changes what a run
  measures is a sentence the pre-registration owes (`preregister`) before the
  *after* run, the protocol's own rule.
- **The references**: the dry run grades each `reference.hero` exactly as an
  answer, its text wrapped by `thesis_grade.as_answer` and handed to `graded`,
  which writes it into a scratch (`tests/harness/thesis.hero:181-203`), so the
  one word that places the tree serves the references too; and `compat`
  reads `tests/thesis/` references from each release
  (`tests/harness/suite_compat.hero:73-77`), so a reference using a package
  carries its copy into the release, item B's rule again.

**Prediction.** The *before* run of question (xiii) needs no change to the
instrument; the *after* run is possible only after a commit touching
`tests/harness/thesis_grade.hero` or `thesis_args.hero`, which `git log`
of those files will show before the run's `preregister`.

**Condition.** A task whose statement must name a package (rather than a run
that offers one) would move me to the task field.

## A. The demand: issue 805's count, extended

**Verdict.** **Approve `strings` and `strconv` first**, on the demand
measured below; and record, for the conventions' admission rule (§6.1), that
the copies **already disagree**, which is Goose's second clause measured
rather than argued.

**Section.** design.md §1.11 (where a package falls, item A's question);
`docs/roadmap/milestones/M-core-packages.md` (Goose's admission test).

**Issue 805's command, re-run** (`grep -rl '^function <name>(' examples`):
`split_lines` 9 files, `is_space` 9, `trimmed` 7, `index_of` 4; and
`grep -c '^function ' tests/harness/strings.hero`: 15. All as the brief says.

**The harness's 15, against `examples/`**, by name (`awk` over each file,
`bin/fnlen.awk`, a function's header and indented body) and, where the corpus
spells the operation otherwise, by signature (read with `grep -h -E
'^function ...'`):

| harness function (lines) | copies in `examples/`, same operation | lines each | note |
|---|---|---|---|
| `trimmed` (12) | 7 | 12 | one body, seven times |
| `is_space` (2) | 9 | 2 | two contracts, below |
| `lines` (6) | 9, as `split_lines` | 13 to 15 (126 in all) | three shapes, below |
| `split_on` (12) | 2 as `split_on`, 1 as `split(text, at: u8)` | 15, 15, 13 | |
| `starts_with` (11) | 1 (`ini/main.hero:138`) | 5 | |
| `ends_with` (12) | 1 (`ini/main.hero:144`) | 5 | |
| `words` (18) | 2 as `split_words`, 3 as `words_of` | 16, 16; 13, 16, 16 | |
| `to_number` (24) | 4 as `number_of(...: str) -> i64?`, 1 as `int_of` | 9, 25, 11, 20; 25 | `assembler`'s takes a second parameter, `query`'s a line |
| `index_of` (21) | **0 of the same contract**: 3 copies are `index_of(text: str, c: u8) -> i64` answering `0 - 1` when absent (`logs/entry.hero:67-75`), Go's sentinel; 1 is a grid's | 9 | the author's naming rule's own case, in the corpus |
| `contains` (21) | **0**: the 2 copies are list membership (`[i64]`, `[str]`) | 6 | |
| `without_suffix`, `boundary_at_or_below`, `base_name`, `without_prefix`, `split_text` | 0 | none | |

**448 lines of hand copies in 17 directories of `examples/`** (16 programs
and `gallery/`), the sum of `trimmed` 84, `is_space` 18, `split_lines` 126,
`split_on` 30, `split` 13, `ends_with` 5, `starts_with` 5, `split_words` 32,
`words_of` 45 and the five `str -> i64?` parsers 90; 461 with the two
`number_of`s of another contract (`interpreter` takes a `Value`, `markdown`
answers a plain `i64`).

**The copies disagree, run** (scratch `copies/variants2.hero`, three
`split_lines` and two `is_space` lifted unchanged and called side by side):

```
$ heroes run variants2.hero
1:a / 2:a| / 1:a          split_lines("a\n"): ini, query/fmt/report, adventure
0: / 1: / 0:              split_lines("")
2:a|b / 2:a|b / 2:a|b     split_lines("a\nb")
true                      is_space('\n') in tally
false                     is_space('\n') in todo
(exit 0)
```

So `query`'s copy gives a text ending in a newline a last empty line and the
empty text one empty line, where the other eight give none; and two of the
nine `is_space` count a newline as space (`tally`, `json`) and seven do not.
**`is_space` fails Goose's first clause** (two lines, not more than three)
**and passes the second**, *easy to get subtly wrong*, by this run.

**Prediction.** When `strings` lands with *no copy by hand survives*
(instruction 3(d)), `grep -rl -E '^function (split_lines|is_space|trimmed)\('
examples` prints nothing, and at least one corpus program's `main.expected`
moves or is shown not to (the two contracts above meet one function).

**Condition.** None: these are counts.

## The seat's verdict on the proposal, in its own shape

- **verdict**: **approve** the proposal (ordinary Heroes modules, no form, no
  built-in) on the routes above; **veto B5**; **object** to B1, B4, B6, P2,
  P2', P3 for the platform's C, P4, `sort_by` in the library, both *alone*
  readings of item I, and a card made by the compiler now.
- **section**: design.md §1.1 and §1.7 (the ceiling), §1.11 with panel 097's
  condition 5 (the veto), Part 5 (nothing here is core: every approved route
  is erased before the checker or lives outside the compiler).
- **implementation_cost**: on the approved routes, **no line in the lexer,
  the parser, the checker, the IR or the emitter** for packages, placement,
  cards or the platform: B2 with B3 is 0 compiler lines and about 40 to 60
  of harness (the copies' check); the `packages` suite about 300 to 500 of
  harness; the card program about 150 to 250 of harness; P1' 0 compiler lines,
  the runtime's headers and about 6 lines of its coverage test. Costed
  separately, each its own decision: §2.3's binding refusal about 5 lines in
  `selfhost/resolve/top.hero` and about 31 of renames in four modules; the
  in-order header check about 20 to 60 in `selfhost/emit/`, the seed and the
  emissions moving once; dead-stripping about 10 to 20 in `selfhost/cli/`,
  the cache cold once; the `sort` package's copy about 40 to 60 lines in
  `selfhost/core/`, for about 50 to 60 deleted from five modules. Every
  estimate is marked as one where it stands.
- **needed_for_self_hosting**: **no**. The compiler compiles itself today
  with none of it; the nearest is the comparator sort, which the compiler
  replaced with `selfhost/stable.hero` and five hand loops.
- **argument**: Every route that keeps a package an ordinary module costs the
  compiler nothing, because discovery already reads `core/strings` from the
  root (`modules.hero:107`), mangles it apart (`h_corestrings_…`), refuses
  its collisions loudly, and tracks a shipped header in the cache. The
  routes that cost the compiler are the ones that make a package something
  else: B5 makes it the standard library panel 097 C5 forbids, B4 makes a
  `use` read outside the program, P2 doubles the runtime's 47 sites and its
  bootstrap line in 24 living files, P3 owes an instrument that does not
  exist (measured). The cheap routes are also the robust ones: a program
  carries its tree, so a package change cannot break a released program.
- **prediction**: at M-core-packages' close, `git diff --stat <opening>
  <close> -- selfhost/lexer.hero selfhost/parse selfhost/check
  selfhost/resolve selfhost/ir selfhost/emit` shows no file changed for
  placement, cards or the platform layer (a change landed by a separate
  decision, §2.3's refusal or the in-order check, is named in its own
  commit and excepted by name).
- **condition**: a package the corpus needs that cannot be placed under its
  program's root, or a platform capability whose C cannot stand behind one
  prototype per function, would move me off B3 or P1' respectively; a
  ratified reading of §1.11 and C5 lifts the veto.

## What I could not run, and why

- **Windows and Linux x86-64**: not machines of this sitting. Unrun there:
  B6's checkout (what git writes for a link without `core.symlinks`, and
  whether the CI's Windows runner sets it), dead-stripping's effect on a
  Windows binary, the case rules of a Windows volume, and the dead arm of
  `r3/twoarm.h` that a Windows leg would compile.
- **Linux arm64 was run in `heroes-linux-arm64:latest`** for three things
  only: dead-stripping (§ F), `--gc-sections` against the runtime as compiled
  today (§ F), and the case rules (§ B), the last inside a `--tmpfs`, since a
  bind mount of this Mac's scratch keeps the Mac's case rule (measured, and
  recorded in § B). The container wrote into my scratch through mounts and
  into memory it discarded; nothing else.
- **No prototype in `selfhost/`**: the B4 lookup, the module-relative header
  lookup, the in-order header check, the word-part near-name rule, the
  missing-`use` suggestion, panel 114 R3's second instrument, the `packages`
  suite and the card program are priced by reading the modules they would
  touch; every count of their lines is an estimate and says so. B5's size is
  arithmetic on one measured ratio, not a build.
- **The thesis grader on a task**: it grades a sealed suite, and the run is
  the author's (journal 069); § N is read, with probe 1 as the refusal it
  would meet.
- **`heroes measure --refresh` on a card**: an API call, not mine to start.
- **No suite of the net beyond `layout` narrowed to one file** (4 passed, 0
  failed): the corpus and the net under dead-stripping, and the census of
  §2.3's refusal, need the change written first.
- **No clock** was started; every cost here is lines, files, bytes or
  symbols.

