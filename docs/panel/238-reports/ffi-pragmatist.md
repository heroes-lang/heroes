# Panel 238, the ffi-pragmatist's report

Seat: ffi-pragmatist (design.md §1.11, §4.19; veto on what breaks the C ABI or makes bindings
categorically harder). Worktree `.claude/worktrees/238-ffi-pragmatist/`, detached at `27829fbba`.
Written as I go, 2026-11-08 from about 19:55. Evidence under `build/238/` in this worktree.

## 0. The compilers

- Stage 1: `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`, exit 0
  (`build/238/stage1.log`).
- Stage 2: `./heroes build selfhost/main.hero -o heroes-next` under `/usr/bin/time -l`, exit 0,
  **746,325,266,354 instructions retired**; `./heroes-next --version` second line
  **`sources 62dba14251e6eafb`**, the briefs' own (`build/238/stage2.log`). Called **base** below.
- **The route**: the compiler-engineer's `build/238/prototype.diff` (the formatter rule, `print/
  use_order.hero`, and the route, `use_key.hero` read by `modules.load_edited`, `modules.cycles`
  and `resolve/top.read_uses`), copied to `build/238/prototype.diff` and applied with `patch -p1` to a
  **copy** of `selfhost/` under `build/238/walk/` (`walk-patch.log`), so this worktree's own
  sources stay at the briefs' commit; built by base, `./heroes-next build
  build/238/walk/selfhost/main.hero -o build/238/heroes-walk`, exit 0, **777,795,358,889
  instructions retired**, **`sources 01116def3a2e944c`** (`walk-build.log`). Called **route** below;
  its `fmt` is the rule's, so the text *the rule writes* is `heroes-walk fmt`'s.
- **Linux arm64** (`heroes-linux-arm64`, the worktree mounted at `/w`, every write under it): the
  same two compilers built in the container from a copy of `seed/`, `runtime/`, `selfhost/` and the
  patched copy under `build/238/linux/tree/` (`build/238/linux/build.sh`, `build.out`): stage 1 and
  stage 2 exit 0, base **`sources 62dba14251e6eafb`**, route **`sources 01116def3a2e944c`**, the
  same keys as on this Mac. One note on my own conduct: the first container runs let clang put its
  temporary objects in the container's own `/tmp`, removed with the container (`--rm`); every run
  after `c-more.sh` sets `TMPDIR` under the mount.


## 1. Where the written order reaches C: read in the code first

- **The headers of the one file.** `selfhost/emit/externs.hero:116` (`headers`): every header a
  group names, *"in the order the groups are WRITTEN"*, which is the declarations' index, and a
  declaration's index is its place in the compilation's source, whose file order is the module
  walk's (`selfhost/modules.hero:79-128`, `load_edited`: the root's `use` lines in written order).
  `cli/one_file.hero` asks clang whether that file means what the units mean.
- **The headers of a per-module unit.** `emit/externs.hero:129` (`headers_for`): the module's own
  groups first, in written order (spec section 13, *"C reads a module's headers in the order its
  groups are written"*), then **another module's group record the unit spells, `sort(others)`**,
  so between two OTHER modules' records the order is the walk's again. Panel 204's R1 (defect 563)
  put a module's own headers before another's record; it did not order two others between
  themselves. That is a fact about `build` and `run`, not only `--emit-c` (item 2).
- **The link line.** `selfhost/cli/link.hero:151-168` (`link_line`): the objects, then a
  package's link words (`libraries.link_words`), then `-l<name>` for each `link` name, in the order
  `emit/externs.hero:207-255` (`libraries`, `named`) collects them: `p.functions` first, then the
  record-only groups, each list in the walk's order. So **every package's words stand before every
  `link` library whatever the order written**, and two `link` libraries of two modules stand in the
  walk's order. No `--start-group` (`git grep -n 'start-group' -- selfhost runtime`: nothing).

## 2. A per-module unit whose acceptance follows the written order (`build/238/units/two_others/`)

The shape beside defect 563, depth one: **two other modules' records in a third module's unit**.
`pw.h` defines `TWO_OTHERS_WIDE` and `Pw`; `rs.h` lays `Rs` out `{ long long a; int b; }` under
that macro and `{ int a; int b; }` without it; `pw.hero` binds `Pw`, `rs.hero` binds `Rs` with
`a: i32` (what `rs.h` says read alone, which is how `rs`'s own unit reads it); the root spells both
records and has no group of its own. Base compiler (`sources 62dba14251e6eafb`):

| root | `run` | `build --emit-c` |
|---|---|---|
| `rs_first.hero` (`use rs` / `use pw`) | **`9`, exit 0** | exit 0, 17,476 bytes |
| `pw_first.hero` (`use pw` / `use rs`, the order the rule writes) | **exit 1**, `error[ffi_field_type]: \`Rs.a\` is \`long long\` in \`rs.h\``, at `rs.hero:3:9`, `fix (guess): declare \`a\` with \`i64\`` | exit 1, the same |

**The message is false of `rs.h`** (read alone it says `int`, and `rs`'s unit compiles it so), and
**its fix leads nowhere**: `build/238/units/two_others/fixguess/`, `a: i64` and the root's
arithmetic widened to type-check, gives in **both** orders `error[ffi_field_type]: \`Rs.a\` is
\`int\` in \`rs.h\` ... fix (guess): declare \`a\` with \`i32\``, exit 1 (the `rs` unit's
reading). So with `use pw` above `use rs` **no spelling of `Rs` builds**; the one escape is the
order of the two `use` lines. The program is correct by spec section 13 (each module's headers in
its own groups' order; the root has none), and `run` accepts it in one order: a correct program
refused by `build` and `run` in one written order and not the other, with a false message, which
`.claude/rules/verification.md` § Bounded discovery calls **`blocking`**. It exists today; the
rule's `fmt --in-place` writes `pw_first` from `rs_first` (`p` before `r`), and the route walks
`pw` first in both: **route**, `rs_first` and `pw_first` both exit 1, the same false
`ffi_field_type`, so under the route the program that runs today is refused in both orders and no
`use` order escapes. `heroes test` of `pw_first` (base) is refused the same way; `rs_first` holds
no test.

**The critic's programs, re-run here** (`build/238/critic/`, copies of the `.hero` and `.h` files;
`build/238/cside-clean/` for the link words, each build from an empty `build/`, since a build whose
words equal an earlier one's reuses that folder and a newest-file reading is then wrong):

| program | base | route |
|---|---|---|
| `c453/written.hero` (`use b_check` / `use a_mode`) | `run` `7` `2`; `--emit-c` **exit 0** | `run` `7` `2`; `--emit-c` **exit 1**, `ffi_one_file` |
| `c453/sorted.hero` | `run` `7` `2`; `--emit-c` **exit 1**, `ffi_one_file` | `run` `7` `2`; `--emit-c` exit 1, `ffi_one_file` |
| `cside/ab.hero` (`ma`: `math.h`, `link "m"`; `mb`: `stdlib.h`, `link "c"`) | `4.0` `3`, link words `"-lm" "-lc"` | `4.0` `3`, `"-lm" "-lc"` |
| `cside/ba.hero` | `4.0` `3`, `"-lc" "-lm"` | `4.0` `3`, `"-lm" "-lc"` |

**The other header shapes the brief names, base** (`build/238/units/shapes/`): *a type one
header declares and another needs* (`tneed.h` reads `Tq`, which `tdecl.h` declares, each in its
own module): `run` and `--emit-c` **exit 1 in both orders**, `ffi_header_refused`, *unknown type
name 'Tq'*, at `tneed.hero:1:8`: true, and order-free, since `tneed`'s own unit reads `tneed.h`
alone. *A header two modules' groups name* (`shared.h`, which reads `SHARED_KNOB`; `knob_user`
names `pre.h` above it, `plain_user` names it alone): `run` prints `0` `10` in both orders;
`--emit-c` **exit 1 in both**, `ffi_one_file` on `knob`. Both refusals are order-free and true.

## 3. The link line, both linkers (`build/238/link/`)

**What each manual says**, read before running. ld64 (`man ld` on this Mac, `ld -v`: `PROJECT:ld-27106.1`,
Apple clang 21.0.0): *"Unlike traditional linkers, ld will continually search a static library
while linking. There is no need to specify a static library multiple times"*, and *"Both dynamic and
static libraries are searched as they appear on the command line."* GNU ld 2.44 in
`heroes-linux-arm64` (`ld --version`; Debian clang 22.1.8 calls `/usr/bin/ld`, the bfd linker, by
`clang -###`): `-(`, `--start-group` *"Start a group"* (`ld --help`), the traditional single pass.
Heroes passes no group word (item 1).

**The binding C first** (`c-link.sh`, `c-sig.sh`, `c-group.sh`, `c-trace.sh`; sources in `src/`):
`libtop.a` (`top_value` calls `base_helper`) and `libbase.a` (`base_helper` and `base_other`, two
members); `liba.a` and `libb.a` each defining `pick` (1 and 2) in a member of its own, and the same
two as shared libraries; `libw.a` whose `pick` is **`int pick(int *out)`**, against `a.h`'s
`int pick(void)`. `main` calls through `a.h`, `top.h`, `base.h`. Every file compiled by clang,
both platforms:

| shape | order | ld64 (this Mac) | GNU ld 2.44 (Linux arm64) |
|---|---|---|---|
| static, one archive needs the other | `-ltop -lbase` | `41 2` | `41 2` |
| | `-lbase -ltop` | `41 2` | **link fails**, `undefined reference to 'base_helper'` |
| static, one name in two archives | `-la -lb` / `-lb -la` | `pick=1` / **`pick=2`**, no word | `pick=1` / **`pick=2`**, no word |
| shared, one name in two libraries | `-la -lb` / `-lb -la` | `pick=1` / **`pick=2`**, no word | `pick=1` / **`pick=2`**, no word |
| static, one name, **another signature** | `-la -lw` / `-lw -la` | `pick=1` / **Segmentation fault, 139** | `pick=1` / **Segmentation fault, 139** |
| the dependency in `-Wl,--start-group ... --end-group` | both | **refused**, `ld: unknown options: --start-group --end-group` | `41 2` both |
| the dependency under `-fuse-ld=lld` | both | refused, *invalid linker name* (no lld here) | `41 2` both |
| the other signature, grouped, and under lld | `-lw -la` | | **Segmentation fault, 139** both |

So: an archive that needs another is order-free on ld64 and on lld, and on GNU ld only inside a
group; **one name two libraries define binds the first on the line on every linker measured**, a
group or lld changing nothing, and with another signature that is a memory fault behind a header
clang verified. Both linkers can say which library supplied a name **in the same link**: ld64's
`-Wl,-map` lists `_pick` from `liba.a(pick_a.o)` or `libw.a(pick_w.o)`; GNU ld's
`-Wl,--trace-symbol=pick` prints `libw.a(pick_w.o): definition of pick` (`c-trace-*.txt`). For
the shared pair (`c-more.sh`, `c-nm-darwin.txt`): GNU ld's trace says `liba.so: definition of pick`
in one order and `libb.so: definition of pick` in the other; ld64's map lists no imported symbol,
and **`nm -m` of the linked binary** says `(undefined) external _pick (from liba)` against `(from
libb)`, since ld64 records each import's dylib (the two-level namespace, `man ld`). And **lld
accepts the group words**: `clang -fuse-ld=lld ... -Wl,--start-group -lbase -ltop -Wl,--end-group`
prints `41 2` in the container.

**The same libraries bound by two Heroes modules** (`hero/`, `hero-pkg/`; `hero-link.sh`,
`hero-pkg.sh`, `hero-sig.sh`; the Linux leg built its own compilers in the container from the same
sources, base `sources 62dba14251e6eafb` and route `sources 01116def3a2e944c`,
`build/238/linux/build.out`). Modules `top_lib` (`link "top"`), `base_lib` (`link "base"`),
`picker` (`a.h`, `link "a"`, binds `pick`), `extra` (`link "b"`), `aux` (`link "w"`, binds
`w_only` only); packages `ptop`, `pbase` by `.pc` files. *Written* is the order shown; *sorted* is
what `heroes-walk fmt` writes from it (run: `use base_lib` / `use top_lib`, `use extra` /
`use picker`, `use aux` / `use picker`). `link-words.rsp` read beside every binary:

| program | link words | base, Mac | base, Linux | route, Mac | route, Linux |
|---|---|---|---|---|---|
| `dep_written` (`top_lib`, `base_lib`) | `-ltop -lbase`; route `-lbase -ltop` | `41 2` | `41 2` | `41 2` | **exit 2** |
| `dep_sorted` | `-lbase -ltop` | `41 2` | **exit 2** | `41 2` | **exit 2** |
| `dup_written` (`picker`, `extra`), static and shared | `-la -lb`; route `-lb -la` | `1` | `1` | **`2`** | **`2`** |
| `dup_sorted` | `-lb -la` | **`2`** | **`2`** | `2` | `2` |
| `sig_written` (`picker`, `aux`) | `-la -lw`; route `-lw -la` | `1 30` | `1 30` | **Bus error, 138** | **segfault, 139** |
| `sig_sorted` | `-lw -la` | **Bus error, 138** | **segfault, 139** | Bus error, 138 | segfault, 139 |
| `pp_written` (packages `ptop`, `pbase`) | `-ltop -lbase`; route `-lbase -ltop` | `41 2` | `41 2` | `41 2` | **exit 2** |
| `pp_sorted` | `-lbase -ltop` | `41 2` | **exit 2** | `41 2` | **exit 2** |
| `lp_written`, `lp_sorted` (`link "top"`, package `pbase`) | `-lbase -ltop` always, a package's words first | `41 2` | **exit 2** both | `41 2` | **exit 2** both |

**Every exit 2 above is `internal error: linking failed:`** then GNU ld's `undefined reference to
'base_helper'` and *"error: the linker refused the objects clang compiled"*
(`out-linux/base-dep_sorted.err`): the compiler blaming itself for a library order it chose. Every
`sig` crash built at exit 0 with no diagnostic. Read:

- **With the unchanged compiler, the order of two `use` lines changes what a built program does**:
  whether it links on Linux, which C definition a bound name calls on both platforms, and whether it
  runs or dies with a memory fault. The author's premise is false for `build` and `run` here, not
  only for `--emit-c`.
- **The rule alone gives `heroes fmt --in-place` that power**: `sig_written` runs and prints `1 30`;
  the text `fmt` writes from it builds silently and segfaults, on both platforms. design.md §1.12's
  *must not segfault* is broken by a formatter, which no guard, probe or suite of the formatter can
  see (they compare one file's tree).
- **The route makes the order immaterial and the module names decisive**: `dep_written` and
  `pp_written`, which link on Linux today, are refused there in both orders; `dup_written` prints
  `2` where it printed `1`; `sig_written`, which runs today, crashes in both orders. With these
  module names no `use` order escapes; renaming a module, or moving both groups into one module
  (whose groups keep their written order, spec section 13), is what is left.
- **Two of the classes are older than the proposal and order-free on no compiler**: a `link` beside
  a `package` whose library it needs fails on GNU ld in both orders today (a package's words always
  stand first), and one name two named libraries define binds whichever comes first, unrefused.
- **The same message when a library's dependency is not linked at all** (`only_top.hero`, `use
  top_lib` alone, base): **exit 2 on both platforms**, `internal error: linking failed:`, ld64's
  `"_base_helper", referenced from: _top_value in libtop.a[2](top.o)` and GNU ld's `libtop.a(top.o):
  ... undefined reference to 'base_helper'`, then *"the linker refused the objects clang compiled"*.
  The linker names `libtop.a`, which the program's own group links, so the author can be told; the
  rule's narrowing (`.claude/rules/c-boundary.md`, *"a symbol nobody declared stays exit 2"*) keeps
  it the compiler's. Older than this sitting and order-free; found beside, item 7.
- **A named binding, SQLite (§4.19's rung 3)**, `build/238/link/hero-sqlite/`, Linux arm64: module
  `a_sqlite` binds `sqlite3.h` with `link "sqlite3"` (and `math.h` with `link "m"`, which the
  static archive needs: without it both orders fail on `acos` and the rest, my binding's mistake);
  module `notes` binds a static `libnotes.a` whose function calls `sqlite3_complete`, which no group
  binds. Shared and static SQLite (Debian's `libsqlite3.a`, **102 members**, `ar t`), base and
  route, both orders: **all eight link and print `3` `true`**, words `"-lnotes" "-lsqlite3" "-lm"`
  or `"-lsqlite3" "-lm" "-lnotes"`. Its members pull each other: `nm` shows `loadext.o` referencing
  `sqlite3_complete`, so binding any SQLite function pulls the member a library behind it needs.
  So the dependency class bites an archive whose members do not reach each other (the synthetic
  pair above), and **does not bite a SQLite binding**; a shared library early on the line also
  satisfies a later archive (GNU ld, measured: `sq_sorted` with shared SQLite).
- None of the four is a refusal of §4.19's: the headers verify, the signatures match the headers,
  and the link either fails as an `internal error` or succeeds into the wrong definition.

**Searched and not found** (a question, not a premise): `gh search issues` over `link order`,
`two libraries`, `duplicate symbol`, `start-group`, `archive order`, `first library`, `defined by
two` returned no issue of this class; `git grep -i 'start-group\|link order\|archive order'` over
`docs/panel`, `docs/design.md`, `docs/records` finds panel 049's *"link order **matters** to a
linker, reproduced"* (`docs/panel/049-the-platform-axis.md:182`, about `link` lines inside one group)
and panel 198's census of package words (`198-reports/compiler-engineer.md:90`, *"No package on any
machine uses `--start-group`"*), and no ruling on the order of libraries across modules. design.md
§4.19 and spec section 13 order a module's **headers**; neither says anything of the **libraries'**
order on the link line. **The document does not cover this objection.**

## 4. The class counted over the tree

**Which programs can show it at all.** Every root in `examples/` (files opening `function main(`,
`heroes.packages/` and `index/` left out) and in `tests/golden/run/` (its top-level `.hero` files and
`*.case/main.hero`): **763** roots (`build/238/roots.txt`). For each, `heroes-next check --json`'s
`read` list, and how many of those files hold a column-0 `extern` (`build/238/reads.sh`,
`reads.txt`): **0 `examples/` programs reach two or more modules with groups** (21 reach exactly
one), and **12 `tests/golden/run` cases** do (`build/238/multi-extern.txt`): defects 2286, 555,
559 (four), 560 (four), 563, 643. A program with one such module cannot move on the C side by the
order of its `use` lines: its link names and headers are one module's, in its groups' order.

**The 12, three conditions** (`build/238/census-run/`, `census12.txt`): the tracked
`tests/golden/run` copied twice; one copy formatted whole by `heroes-walk fmt --in-place` (722
files, 0 refused, **5 changed**: 2030, 2286, 560 *default*, 560 *two ways*, 563 *own headers*);
each case asked `build --emit-c` and `run` by **base on the written copy**, **base on the formatted
copy** and **the route on the written copy**:

- `run`: **all 12 print the same bytes in all three conditions** (`cmp` of the outputs).
- `--emit-c`: 11 of 12 the same verdict in all three (6 refused `ffi_one_file` in all three,
  5 accepted). **One moves**: `fixedbugs-563-a-module-s-own-headers-come-before-another-module-s-
  record` is **exit 1 `ffi_one_file` as written, exit 0 formatted, exit 0 under the route**.
- The emitted C beyond `hero_program_sources` and the `#line` numbers: 2286 differs by 20 lines
  written against formatted; the route's emission equals the formatted one's (0 lines); the
  others that emit, 0.

**The `examples/` programs the rule moves** (`build/238/census-ex/`): `examples/` copied twice,
one copy formatted by `heroes-walk fmt --in-place` (placed trees and the index left out): **10
files in 7 programs** change (`adventure`, `interpreter`, `ledger`, `markdown`, `query`, `shapes`,
`todo`). Each program built by base from both copies and its binary run with its `main.args` from
a folder where `examples` names the copy, stdin `/dev/null`, `timeout 20`, `head -c 65536`: **7 of 7
build at exit 0 both ways, 7 of 7 outputs byte-identical**, each matching its `main.expected`
(`interpreter`'s apart from the harness's own `!exit: 0` line); no binary left running (`pgrep`).
The other 50 `examples/` folders the rule leaves byte for byte.

**So the tree holds no program the rule or the route breaks on the C side**: the one verdict that
moves moves from refused to accepted, and no `run` output moves. The class lives in programs the
tree does not hold yet: two modules each naming a library (no `examples/` program has two), or two
modules' records whose headers interact (the 563 and 559 families, all pinned in the written
order). **The `blocking` reproducers are therefore the shapes, not a tracked program**: item 2's
`two_others` (a correct program refused by `build` and `run` in one order, a false message) and
item 3's `sig` (a program that builds silently and segfaults in one order).

## 5. The seed and `compat`

- **The compiler's own groups**: `git grep -l '^extern' -- selfhost/` names **22 modules holding 28
  groups**, over three headers, `hero_os.h` (16 groups, one of the two `SEEDS`,
  `emit/externs.hero:86-87`), `hero_compiler.h` (9) and `stdlib.h` (3), and **no `link` and no
  `package`** (`git grep -h '^extern' -- selfhost/ | sort | uniq -c`). So the compiler's link line
  has nothing the walk can order.
- **Its one file's headers do not move**: the include lines of the compiler-engineer's seven
  emissions of `selfhost/main.hero` (`238-compiler-engineer/build/238/emit/`, read, not run there:
  as written, `main.hero` sorted, the tree formatted, the route, stages 2 and 3) are **the same eight
  lines in the same order** in all seven: `heroes_unit.h`, `<heroes_unit.h>`, `<hero_os.h>`, the
  guard, `<stdlib.h>`, `<hero_compiler.h>`, the guard's close, `heroes_standard.h`. What moves is the
  order of definitions and the `#line`s, which C11 reads the same where every function is
  prototyped first. **Nothing the rule or the route does to `selfhost/` changes what the seed's C
  means to clang**; the bytes are the compiler-engineer's (403,084 lines once under the route).
- **`compat`** reads each release's programs from the tag (`suite_compat.hero:339-358`, `git
  archive`), never the formatted tree, so reformatting today moves nothing it reads. At every
  release tag no program reaches two modules with groups: `git grep -l '^extern' <tag> --
  examples/` grouped by folder gives none at `v0.2.0` and, at `v0.3.0` to `v0.6.0`, the gallery's
  `08-ffi.hero` and `13-lease.hero`, two one-module programs with no `use` line. So neither the rule
  nor the route can move a release's `--emit-c` verdict on the C side today; a release that carries
  a program like item 2's or item 3's would, the day a compiler with the route judges a release
  written without it.

## 6. The route: keep the written order, key it, or make it immaterial

**Is a binding whose meaning depends on its modules' order refused today?** Every shape found, the
unchanged compiler, each verdict above run:

| shape | what the order changes | refused today? |
|---|---|---|
| one module's header defines a macro another's reads (c453) | `--emit-c` only | **yes**, `ffi_one_file` where the one file would mean otherwise; `build`/`run` correct in both |
| a header two modules name, one with a macro above it | nothing (refused both) | yes, `ffi_one_file` in both; `run` correct |
| a type one header declares and another needs | nothing | yes, `ffi_header_refused` in both, true |
| two other modules' records in a third's unit (`two_others`) | **`build` and `run`** | refused in one order with a **false** message whose fix ping-pongs; accepted, correctly, in the other |
| an archive needing another (`dep`, `pp`; GNU ld) | **whether it links** | no: `internal error`, exit 2, the compiler's fault |
| a `link` beside a `package` it needs (GNU ld) | nothing (fails both) | no: `internal error`, exit 2 |
| one name two libraries define, same signature (`dup`) | **which definition a call binds** | **no**, silent, both linkers, static and shared |
| one name, another signature (`sig`) | **runs or dies, Bus error 138 / SIGSEGV 139** | **no**, built at exit 0 |

So **the header side is defended**: wherever headers read together would mean something else, the
compiler refuses rather than builds (one refusal tells a falsehood, item 2). **The library side is
not**: §4.19's verification asks clang about the header, and nothing asks which library supplied the
definition a verified prototype calls. design.md §1.12 (*"A Heroes program must not segfault and
must not corrupt memory"*) and §4.19 as design.md:606 states it (*"a wrong type in an `extern` is a
*compile* error, which is this project's thesis applied to the boundary"*) are both broken by `sig`,
with or without the proposal. The written order is today the one lever an author has over it, and it
is written nowhere.

**The three routes, and what each costs a binding author:**

1. **Keep the written order as the program's own, and let `fmt` move it** (the rule alone). Writing
   costs nothing, and every `fmt --in-place` may silently re-link the program: measured, it writes
   `sig_sorted` from `sig_written` (a running program into one that builds silently and crashes, both
   platforms), `dep_sorted` from `dep_written` (exit 2 on Linux), `pw_first` from `rs_first` (a
   false refusal). The author loses the lever and cannot see it go, and no instrument of the
   formatter can see it either. **Refused: this is my veto.**
2. **Key it** (the compiler-engineer's route, item 0's `route`). Every output above becomes the same
   in both orders, measured: link words, `--emit-c` verdicts, units, outputs. `fmt` loses all power
   over the C side, which is what the author's premise asks. But the key chooses, and for these
   shapes it chooses wrong: `dep_written` and `pp_written` (link on Linux today) exit 2 there in
   both orders, `dup_written` now calls `libb`'s `pick`, `sig_written` (runs today) crashes, and
   `rs_first` (runs today) is refused, each with no `use` order that escapes. The lever moves to
   the **module's name**, which is a stranger lever than the written order: renaming `top_lib` so it
   sorts after `base_lib` is what makes a Linux link succeed. The tree holds none of these programs
   (item 4) and no release does (item 5), so the route breaks nothing tracked.
3. **Make the order immaterial on the C side, for real, and key the rest** (route 2 plus three
   repairs). Then the key reaches only the one file's bytes, which program `--emit-c` refuses (a
   refusal, never a wrong file), and the order of messages:
   - **the libraries in a group on GNU ld**: `-Wl,--start-group` before the package words and the
     `link` names, `-Wl,--end-group` after, on the Linux targets. Measured in C: GNU ld 2.44 links the
     dependency in both orders inside a group; lld links it in both orders with or without the words
     and accepts them; ld64 refuses the words (*unknown options*) and needs none, linking both orders
     bare. That closes `dep`, `pp` and `lp` whatever the order. Windows (`lld-link`) is unrun.
     Unbuilt as a compiler change; its place is `cli/link.hero:151-168`, beside `link_flags`'
     platform words;
   - **a refusal of a name a group binds that the link took from another library than its group's**:
     both linkers say which library supplied a name in the same link (GNU ld `--trace-symbol=<name>`,
     archives and shared; ld64 `-map` for archives, `nm -m` of the output for dylibs, measured in
     both orders). Refused at exit 1 on the binding, naming both libraries, this closes `dup` and
     `sig` **whatever the order**, the key's included. Unbuilt; a new diagnostic class, so a
     sitting's; its open question is a library that overrides a name on purpose (an allocator), which
     the robust reading refuses;
   - **a unit reading two other modules' records**: a true refusal (the header that changes `Rs`
     named, `pw.h` read before `rs.h` in the root's unit), or a unit that reads another module's
     record without that module's header beside another's. Unbuilt; panel 204's family, a sitting's.

   What it costs a binding author: nothing to write, and a program that binds one name from two
   libraries is told so instead of linked into one of them.

**My reading**: route 2 is necessary and not sufficient. It is necessary because without it a
cosmetic tool can produce a memory fault at the boundary; that is the veto. It is not sufficient
because the key it installs is as blind to libraries as the written order was, and it removes the
one lever an author had without giving another; the group words close the link half for the price of
two words on one platform's link line, and the refusal of a name supplied by another library closes
the other half for every order there will ever be.

## 7. Found beside, for the coordinator to file (this seat files nothing)

Each with its reproducer under `build/238/`, each older than the proposal (base compiler, the order
written today):

1. **One name two named libraries define binds whichever stands first on the link line; with
   another signature it is a memory fault behind a verified header.** `link/hero/sig_sorted.hero`
   with `link/out-*/lib/` from `link/c-link.sh` and `c-sig.sh`: builds at exit 0, Bus error 138
   (Mac), SIGSEGV 139 (Linux arm64). Class by `.claude/rules/verification.md` § Bounded discovery:
   **`blocking`** (a memory fault).
2. **Two other modules' records in a unit are read in the walk's order, and the refusal is
   false.** `units/two_others/pw_first.hero`: `ffi_field_type`, *`Rs.a` is `long long` in
   `rs.h`*, whose `guess` fix is refused the other way. **`blocking`** (a correct program refused, a
   false message).
3. **An undefined reference inside a library the program links is told `internal error`, exit
   2**, on both platforms (`link/hero/only_top.hero`), and on GNU ld so is a library order the
   compiler chose (`dep_sorted`, `lp_*`). **`blocking`** (an exit 2 where the author can be told),
   and the group words close its order half.
4. **A `link` group's library that needs a `package`'s fails on GNU ld in every order**
   (`link/hero-pkg/lp_*.hero`), a package's words standing before every `link` name
   (`cli/link.hero:162-166`). **`blocking`** (a correct program refused); closed by the group words.
5. Read in the code and **unrun**: inside one module the `link` names follow `p.functions` first
   and the record-only groups after (`emit/externs.hero:213-255`), so a record-only group's library
   stands after every function group's whatever their written order. A question, not a finding.

## 8. The group words, built (after the critic's second pass)

The coordinator's request of the same evening, on the critic's second pass (findings 5 and 6,
`238-critic/docs/panel/238-reports/critic-reports.md`): the synthesis adopts item 6's group words
only if built. The critic measured in C on GNU ld 2.44 that **one** group around the package words
and the `link` names together links `dep` both ways, that a package's own group nested inside it is
accepted, and that two groups side by side do not resolve across.

**The change** (`build/238/group/group.diff`, written by `build/238/group/apply.py` into two
**copies** of the sources, never this worktree's own `selfhost/`; each file then `heroes-next fmt
--in-place`d and canonical by `cmp`):

- `selfhost/cli/link_flags.hero`: `groups_libraries(target)`, true where the triple clang names
  holds `-linux`, asked of the target as `target_words` asks it, never of the host; its comment
  carries the measurements (GNU ld once, lld takes the words, ld64 refuses them and needs none, MSVC
  unasked). Plus a test: two Linux triples true, an Apple, an MSVC and the empty triple false.
- `selfhost/cli/link.hero`, `link_line`: the package's link words and the `-l<name>` words are
  gathered as one list; where it is not empty and the target groups its libraries, the line takes
  `-Wl,--start-group`, the list, `-Wl,--end-group`; else the list as before, so **a line with no
  library, and every Apple or MSVC line, holds the same words as today**. Plus a test: a Linux line
  ends `["-Wl,--start-group", "-lp", "-ltop", "-lbase", "-Wl,--end-group"]`, an Apple line ends
  `["a.o", "-lp", "-ltop", "-lbase"]`, a Linux line with no library holds no group word.
- **By module**: `cli/link.hero` +55 -4 lines (`wc -l` 688 to 739; the `layout` suite's code lines,
  non-blank outside `test` blocks, 269 to 281), `cli/link_flags.hero` +18 -0 (192 to 210; code
  lines 145 to 155); both under the 300 threshold, neither in `DECIDED`. Of `link.hero`'s 12,
  **six are code** (the list's declaration, the branch, its three pushes, its `return`), five the
  comment above the branch and one the doc comment's extra line; of `link_flags.hero`'s 10, **two
  are code** (the function) and eight its comment.
- The same two files are byte-identical in the base copy and the route copy (`diff -q`), the
  prototype touching neither.

**The compilers**, four on each platform, each built by its platform's base (`build/238/group/build-
*.log`, `build/238/linux/group.out`):

| compiler | sources | Mac build, instructions retired | Linux arm64 |
|---|---|---|---|
| base | `62dba14251e6eafb` | 746,325,266,354 (item 0) | built there, same key |
| route | `01116def3a2e944c` | 777,795,358,889 (item 0) | built there, same key |
| **base + groups** | **`02ffeda04877ee3b`** | 755,300,876,297 | built there, **same key** |
| **route + groups** | **`50edfd92476704cb`** | 744,434,281,712 | built there, **same key** |

**Its own tests**: `heroes-group-walk test build/238/group/walk/selfhost/main.hero --only "(panel
238)"`: **2 tests, all passed** on this Mac (998,424,042,787 instructions retired, the whole
compiler built as a test binary) and **2 tests, all passed** in the container
(`build/238/linux/group-test.out`). The compiler's other 1,910 tests were **not run** with the
change (its test asserting an exact Linux line is the second of these two; the three existing tests
that call `link_line`, `cli/link.hero:491, 539, 628`, pass no library word, so the branch leaves
their lines as they were, read in the code, unrun).

**Every program, both ways, four compilers, each built from an empty `build/`**
(`build/238/link/run-all.sh`; outputs `group-darwin-*.txt`, `build/238/linux/group.out`). The words
are every `-l`, package `-L` and group word of the program's own `link-words.rsp`, in order: (`--start-group` and `--end-group` below stand for the words `-Wl,--start-group` and
`-Wl,--end-group`, `-L...` for a package's `-L` with its path cut)

*Linux arm64, GNU ld 2.44:*

| program | base | route | base + groups | route + groups |
|---|---|---|---|---|
| `dep_written` | `41 2` | exit 2 | `41 2`, `--start-group -ltop -lbase --end-group` | `41 2`, `--start-group -lbase -ltop --end-group` |
| `dep_sorted` | exit 2 | exit 2 | **`41 2`**, `--start-group -lbase -ltop --end-group` | **`41 2`**, the same |
| `pp_written` | `41 2` | exit 2 | `41 2`, `--start-group -L... -ltop -L... -lbase --end-group` | **`41 2`**, `--start-group -L... -lbase -L... -ltop --end-group` |
| `pp_sorted` | exit 2 | exit 2 | **`41 2`** | **`41 2`** |
| `lp_written` | exit 2 | exit 2 | **`41 2`**, `--start-group -L... -lbase -ltop --end-group` | **`41 2`**, the same |
| `lp_sorted` | exit 2 | exit 2 | **`41 2`** | **`41 2`** |
| `dup_written`, static and shared | `1` | `2` | `1`, `--start-group -la -lb --end-group` | `2`, `--start-group -lb -la --end-group` |
| `dup_sorted`, static and shared | `2` | `2` | `2` | `2` |
| `sig_written` | `1 30` | SIGSEGV, 139 | `1 30` | **SIGSEGV, 139** |
| `sig_sorted` | SIGSEGV, 139 | SIGSEGV, 139 | **SIGSEGV, 139** | **SIGSEGV, 139** |
| `only_top` | exit 2 | exit 2 | **exit 2**, `--start-group -ltop --end-group` | **exit 2** |

(every exit 2 is `internal error: linking failed:`, then GNU ld's `undefined reference to
'base_helper'`; every other build's stderr is its own `wrote <path>` line and nothing else, under
the grouped compilers as under base.)

*This Mac, ld64:* **base + groups gives exactly base's words and outputs, and route + groups
exactly the route's**, on all thirteen builds of each (`group-darwin-base.txt`, `group-darwin-walk.txt`): no
group word on any line; `dep_*`, `pp_*`, `lp_*` `41 2` under all four; `dup` `1`/`2` under base and
base + groups, `2`/`2` under the route and route + groups; `sig_sorted` Bus error 138 under all four
and `sig_written` under the two route compilers; `only_top` exit 2 under all four. And **the whole
`link-words.rsp` is byte-identical** between base and base + groups for `dep_written` and
`pp_written`, the object paths under `build/` normalised (`build/238/link/rsp-darwin/`):
`... "-O0" "-g" "-rdynamic" "-Wl,-reproducible" "-L" "../out-darwin/lib" <5 objects>
"-L.../link/out-darwin/lib" "-ltop" "-L.../link/out-darwin/lib" "-lbase"`.

**Read**: the group words, built, **close the link half on Linux whatever the order**: `dep`, `pp`
and `lp` link in both orders under both group compilers, the six exit 2s of the route and the four
of base among them; the route + groups compiler gives every one of these programs the same words,
verdict and output in both written orders. The Mac line does not move. The words change nothing
else: which `pick` binds still follows the order or the key, and the critic's nesting result was not
re-run here (no package of mine answers with a group).

**What stays open**, each measured above or unrun:

- **7.1, `dup` and `sig`**: one name two libraries define binds the first one inside the group as
  outside it; under route + groups `sig_written` crashes in both orders on both platforms (139
  Linux, 138 Mac). Its closing route is the refusal of item 6.3, a new diagnostic class, a sitting's.
- **7.2, `two_others`**: untouched by link words (a header order inside a unit).
- **7.3, `only_top`**: exit 2 `internal error` under all four compilers on both platforms; the
  library its group needs is on no line, so no group finds it. Its order half (`dep_sorted`'s exit
  2) is closed by the words. Its repair is the message (the linker named `libtop.a`, which the
  program links), a change of who is blamed on defect 360's precedent.
- **7.4, `lp`**: **closed** by the group words (both orders link on Linux under both group
  compilers).
- **Windows `lld-link`**: unasked and unrun; `groups_libraries` is false for an MSVC triple, so its
  line is unchanged.
- **The CI's Linux x86-64 leg** (GNU ld there unread): unrun. The predicate asks `-linux` of the
  triple, so it takes the words there.

**What `blocking` does to the tag** (the critic's finding 6): `records/tagged` refuses
M-package-manager's tag over an open `blocking` defect (CLAUDE.md § Verification). 7.1 is a memory
fault, 7.2 a correct program refused with a false message, 7.3 an exit 2 where the author can be
told: each is `blocking` by `.claude/rules/verification.md` § Bounded discovery's own list, and each
is older than the proposal and than the milestone. The class follows that list; which milestone owns
them, and whether the tag waits, is the coordinator's and the author's, and this seat does not bend
the class to free it. 7.4 needs no filing if the words land.

## Verdict

(Updated after section 8: the group words are built and measured. What changed is marked.)

- `verdict`: **veto** on the rule landing without the route (unchanged). **Changed**: with the
  route **and the group words, now built** (section 8), in the same push, **approve**; items 7.1,
  7.2 and 7.3 filed with the class the rule's list gives them, `blocking`, whatever that does to the
  tag (section 8, last paragraph).
- `section`: design.md §1.12 (*"A Heroes program must not segfault and must not corrupt memory"*)
  and §4.19 as design.md:606 states its thesis at the boundary; §1.11 for whose programs these are.
  **The document does not cover the libraries' order**: §4.19 and spec section 13 order a module's
  headers, nothing orders the link line across modules, and no sitting has ruled it (item 3's
  search).
- `experiment`: the binding C in `build/238/link/src/` (`libtop.a` needing `libbase.a`; `liba.a`,
  `libb.a` and their shared twins defining `pick`; `libw.a` whose `pick(int *)` meets `a.h`'s
  `pick(void)`; `libnotes.a` calling SQLite), compiled and linked by clang in both orders: **accepted
  by clang every time**; ld64 links every dependency order and refuses `--start-group`; GNU ld 2.44
  refuses `-lbase -ltop` and links it in a group; lld links both; every linker binds the first
  `pick`, silently, and `-lw -la` segfaults on both. The same libraries bound by two Heroes modules
  (`hero/`, `hero-pkg/`, `hero-sqlite/`), base and route, Mac and Linux arm64: item 3's table.
  **Added**: the group words built into base and route (`build/238/group/group.diff`, `cli/link.hero`
  +55 -4, `cli/link_flags.hero` +18 -0, six and two lines of code), keys `02ffeda04877ee3b` and
  `50edfd92476704cb` on both platforms, their two tests passing on both: on Linux `dep`, `pp`, `lp`
  link in both orders, on this Mac the `link-words.rsp` is byte-identical to base's; `dup`, `sig`
  and `only_top` as before. Over the tree: 763 roots, **12** reach two modules with groups (all
  `tests/golden/run`, 0 `examples/`), **0** `run` outputs move, **1** `--emit-c` verdict moves (563,
  refused to accepted), 7 of 7 moved `examples/` programs identical; releases `v0.2.0` to `v0.6.0`:
  0 such programs; the compiler's own headers do not move and it links no library.
- `argument`: The written order of `use` lines is today the only lever over the link line, and it is
  written nowhere. The rule hands it to `fmt`: measured, one `fmt --in-place` turns a program printing
  `1 30` into one that builds silently and segfaults, on both platforms, and a Linux link into an
  `internal error`. A formatter must not produce a memory fault at the boundary. The route removes
  that power; the group words, now built, make archive order immaterial on Linux and leave the Mac
  line byte-identical. One name two libraries define still binds whichever comes first, and only a
  refusal closes that.
- `prediction`: **changed** to the built compiler. Under route + groups (`50edfd92476704cb`) on the
  CI's Linux x86-64 leg, unrun here, `build/238/link/`'s `dep_*`, `pp_*` and `lp_*` link and print
  `41 2` in both written orders, as on arm64; `sig_written` still dies by SIGSEGV and `only_top`
  still exits 2; on `macos-26` every `link-words.rsp` equals base's. Scored by the landing lane's run
  of these programs on the CI's legs at M-package-manager's push; an x86-64 GNU ld that refuses a
  grouped `dep_sorted`, or a `sig_written` that does not crash there, falsifies it.
- `condition`: the veto lifts when the rule and the route travel in one push and, under the landing
  compiler, `cside`, `link/hero/` and `units/two_others/` give the same link words, verdicts and
  outputs in both written orders. **Changed**: the objection's condition is met by section 8's build
  if those six code lines travel in the same push; I would return to **object** if the CI's x86-64
  leg refuses the words or a package's own group nested inside them, or if 7.1 to 7.3 are filed under
  a class their reproducers do not support. I would withdraw the group words as needed if the
  x86-64 GNU ld links `dep_sorted` bare.
