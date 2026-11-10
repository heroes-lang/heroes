# 241: the compiler takes no package yet, `crypto/sha256` is Heroes alone, and the package manager's rewrite waits for its packages and a full sitting

**Status** a reduced sitting, the soundness lane, by the author's answer of
2026-11-10, about 07:35, put with the coordinator's recommendation: *first a
reduced sitting* on M-core-packages' step 6, feature 2315. The resolution is
`provisional — author ratification pending`.
**Seats** compiler-engineer, ffi-pragmatist; the completeness critic on the
briefs before any seat and on the reports after. No blind seat, no historian, no
paid run by anyone.
**Base** `87d77dec2a08ad87b1593207e1015dee090bd5bb`, tree
`3f1d4d80cfe88d52d6fb3a80b15949dd5f8a69e9`.
**Briefs** `docs/panel/241-briefs/`, **reports** `docs/panel/241-reports/`,
**evidence** `docs/panel/241-reports/evidence.tar.xz` (its note beside it).

## What the sitting was asked

Feature 2315, *"The package manager rewritten over the core packages, every
golden of its interface unchanged"*, M-core-packages' acceptance condition
(`gh issue view 2315`; its notes: *"What it replaces: the package manager's
parts over C, by packages copied into the compiler's tree under
`selfhost/core/`"*). Three questions, as the critic's first pass reshaped them
(`241-reports/critic-briefs.md`; `241-briefs/00-shared.md` § The three
questions):

1. the compiler carrying `core` as a catalogue entry, a new ruling and not a
   reading of panel 234's R13, so the compiler's admission count is owed;
2. where `crypto/sha256`'s C comes from: (a) `pkgconfig "libcrypto"`, (b) a
   package's own header, (c) a runtime door, (d) Heroes alone;
3. how much the second version replaces: panel 235's R19 ends *"The second
   version replaces the transport alone"* (`docs/panel/235-*.md:796`).

**The lane held only where the adopted resolution moves no spec token**
(`00-shared.md` § Why the sitting sits). What the seats measured put two of the
three questions past that line, onto a Part 1 sentence and a Part 7 one; the
resolution below says where, and adopts nothing there.

## The verdicts

| seat | question 1 | question 2 | question 3 | veto |
|---|---|---|---|---|
| compiler-engineer | **object** to route 1 as scoped: it replaces nothing; approve the hand copies repaired onto `cli/strings` | approve (c) or (d) for what the compiler itself hashes; indifferent for a program's package | approve R19's *"transport alone"*; object to the whole closure | none |
| ffi-pragmatist | no `core` module over C in the compiler's closure (object, not on the table); a Heroes-only module costs the boundary nothing | **approve (d)**; object to (a), (b) and (c) as core's route; *"the compiler keeps what it has"* | **veto** an `os` package's own header with a platform arm; object to a package binding `hero_env_put` from `hero_compiler.h`; approve `hero_env_put` moved into `hero_os.h` over `_wputenv_s` | question 3's header shape |

**The seats do not contradict each other on the compiler's own hashing** (the
critic, `241-reports/critic-reports.md` item 1): both leave the compiler on the
two C doors of `hero_compiler.h` it binds today (`selfhost/module/fingerprint.hero:11-14`).
They differ on a program's package over (c): the compiler-engineer counts its
cost to the compiler (0 lines), the ffi-pragmatist measured what it costs the
package (refused `ffi_unknown_name` against any runtime without the door, so the
package's version is chained to the compiler's).

## What each seat measured

**The compiler-engineer** (`241-reports/compiler-engineer.md`), in its own
worktree with its own compiler built from the seed (stage 2 `sources
110fad075b8d3bca`, the trunk's key):

- **Route 1 builds and holds its fixpoint**: `heroes add core --index
  file://../packages -C selfhost` placed `core@0.2.0-4029f4f2486e6ca7/` (13
  files, 10 modules, 2,334 lines), 33 calls in 10 modules of `selfhost/fetch/`
  moved to `use <core>/strings` (+53 −42), stage 2 and stage 3 emitting
  33,318,988 bytes each, `cmp` silent; the compiler's own tests 2,006 passed
  (1,992 at the base plus `core`'s 14).
- **And replaces nothing**: the 33 calls move from `cli/strings`' byte-safe
  `starts_with`, `ends_with` and `holds` to `core`'s byte-identical loops
  (`cli/strings.hero:112-122`, `packages/core/0.2.0/files/strings.hero:17-27`);
  `cli/strings` keeps all three for 370 calls elsewhere; 0 functions deleted;
  the compiler reaches 368 of the placed tree's 2,334 lines.
- **What it costs**: every `heroes check selfhost/main.hero` +0.285 % of its
  instructions retired (110,503,846,740 to 110,818,910,521, five runs each,
  every route-1 run above every base run), under decision 2282's floor of
  effect; the clock read no verdict either way on a machine that was not still
  (median +60 ms on 8,250, sign tail 6.25 %); 13 `#line`s of stage 2 naming the
  placed folder, whose name moves with every version, sum or `index` spelling
  (`module/placed.hero:108-110`), so **every `core` version the compiler takes
  is a seed refresh** (the rule of `seed/README.md`, applied, not observed);
  `packages` holds no `selfhost/` tree to the index (`suite_packages.hero:82-93`);
  the walks that would learn the placed tree, 31 to about 95 lines by one
  precedent (`277676555`), an estimate; the trunk's hook mirror patched in the
  worktree (`provenance.py` +60 −5, its test +18 −2), without which the guard
  refuses every net suite on route 1, so those suites stayed unrun.
- **The admission count**: `core` 0.2.0 has a contract for 36 hand copies in
  `selfhost/`, 520 lines, all of `has_prefix`, `has_suffix` and `contains`;
  none for `split_lines`, `trim_space`, `fields`, `atoi` or `encoding/json`
  (each body read, never the name); `cli/strings`' `trimmed` and `with_all`
  have no `core` contract.
- **R13 literally**, `core`'s files at `selfhost/core/` reached by `use
  core/strings`: `heroes check` exit 1, `unknown_module` and `private_module`;
  a symlink, exit 1; a catalogue `path` entry, exit 0 with *"unverified, no
  fingerprint holds it"* on every check. So the form 2315's notes name does not
  compile as written; edited, it is a fork of `core`.
- **Two live panics**, exit 134, *string slice splits a character*, at
  `emit/ffi_nested.hero:248` and `module/catalogue.hero:268`, both with the
  trunk's compiler.
- **Question 3**: `cli/process` is used by 96 modules, 11 in `fetch/`, so an
  `os/exec` package either binds `hero_os.h`'s doors a second time beside it or
  puts the compiler's whole process layer behind a placed tree. Route (c)'s
  door, built: 8 lines in `parts/sha256.c` and 3 in `hero_compiler.h`, the
  digest of `0 1 2 255 254` and of no byte equal to `shasum -a 256`,
  `HERO_RUNTIME_ABI` 32 unmoved.

**The ffi-pragmatist** (`241-reports/ffi-pragmatist.md`), each route a package
in a scratch index, placed by `heroes add` and reached as `use
<name>/crypto/sha256`, each reading the file's bytes through `stdio.h` with
§ 13's fill form, 65,536 bytes a read:

- **All four routes give `shasum -a 256`'s digest on 14 files** (3,001 bytes of
  `/dev/urandom`, empty, 4 MiB, and eleven at the block and read edges, 55 to
  131,073 bytes), at `-O0` and `-O2`, on this Mac and in the Linux arm64
  container; routes (b), (c) and (d) on the Windows box too, 14 of 14 against
  its own `sha256sum`.
- **(a)**: 96 lines of Heroes, 0 of C, nine members; clang refuses a planted
  `@s: u64` against `unsigned int *`; unchanged from OpenSSL 4.1.0 here to
  3.5.7 in the container. Refused on the box, `ffi_pkgconfig`, no `pkg-config`
  there; the Mac binary names Homebrew's absolute `libcrypto.4.dylib`. Two
  traps: `EVP_sha256() -> Md` is refused (`const EVP_MD *`, and the spec has no
  `const` handle result, a ruled refusal by panels 083 and 112, the critic's
  item 15), so `EVP_MD_fetch`; `SHA256_Init` and its siblings are
  `OSSL_DEPRECATEDIN_3_0`. A compiler over (a) is not buildable from C alone:
  `'openssl/evp.h' file not found`, then five undefined `_EVP_*`.
- **(b)**: 69 lines of Heroes and 122 of portable C11, every function `static
  inline`, no platform arm; a planted `used: u32` against the package's
  `uint64_t` refused `ffi_field_type`.
- **(c)**: 7 lines in `parts/sha256.c`, 2 in `hero_compiler.h`; the package is
  `ffi_unknown_name` against the unmodified runtime.
- **(d)**: 288 lines of Heroes, 0 of C, `compress` and `hex8` copied from
  `selfhost/cli/sha256.hero`.
- **The clocks**, best `real` of six after one discarded, the machine not
  still (load 3.10 to 4.08): on 64 MiB, (d) 9.60 s at `-O0` and 0.81 s at
  `-O2`, against (a) 0.33 and 0.20, (b) 1.24 and 0.37, (c) 1.16 and 0.33.
- **The environment's write**: on the box, `hero_env_put`
  (`SetEnvironmentVariableW`) writes the process block a child inherits and
  leaves the CRT's copy, so the program's own `getenv` reads `null_cstr`, as
  the runtime's comment says it does (`runtime/parts/os.c:900-905`); one line,
  `_wputenv_s`, makes parent, block and child read `café-2`. An `os` package's
  own header with a planted wrong Windows arm, `_putenv_s(name, value, 1)`,
  builds and runs green on this Mac and in Linux arm64 and is refused only on
  the box.
- **The move into `hero_os.h`, performed and restored**: four names, 19 lines
  out of `hero_compiler.h`, 23 into `hero_os.h`, 4 each into
  `heroes_guard_open.h` and `heroes_guard_close.h`, 4 words of
  `emit/guarded_names.hero`; `HERO_RUNTIME_ABI` unmoved; the compiler's own
  tests 1,992 passed on the moved tree; no binding edited.

**The completeness critic** (`241-reports/critic-briefs.md`, before the seats;
`241-reports/critic-reports.md`, after), whose second pass is what this file
rests on where it differs from the seats:

- the approved repair onto `cli/strings` closes `ffi_nested`'s panic and not
  `catalogue.table`'s, which is one of 15 inline cuts outside the copies
  (`pass2/slice-lines.txt`, 19 lines with one comment); and **a third panic**,
  `heroes add` at `fetch/catalogue_edit.hero:197`, after writing the placed
  tree and leaving the catalogue unedited, which the coordinator ran again
  (`pass2/rerun/`, exit 134, 13 files placed, the catalogue `cmp`-equal);
- 17 of the 36 copies, 351 of their 520 lines, are test support: the live
  copies outside `cli/strings` are **16, 137 lines**;
- route 1 does not do what 2315's own notes say it does: every C group of
  `fetch/` and `module/fingerprint.hero` stays (`index_source.hero:39,43`,
  `made_repo.hero:20`, `transport.hero:46`, `fingerprint.hero:23,27`, and
  `cli/process` in 11 of `fetch/`'s 21 modules);
- the package the milestone page names likeliest for the compiler,
  `path/filepath` (`M-core-packages.md:446-452`), is not in 0.2.0, and defect
  2406 lists `path`-shaped copies in `fetch/` no seat counted;
- 114 modules `use cli/strings`, and 64 of them call a helper `core` has no
  contract for, so a whole-compiler route 1 holds both modules, both named
  `strings`, in 64 modules;
- the hooks' own tests red at the base (also the compiler-engineer's, at its
  start), filed as defect 2412.

## The resolution adopted, provisionally

`provisional — author ratification pending`. The most robust and complete
resolution (CLAUDE.md § 4), and where it stops at the lane's edge it says so
rather than adopting past it.

- **R1, the compiler takes no package of `core` 0.2.0 at this step.** Route 1
  as scoped deletes no line and replaces no function; it adds 2,334 lines, a
  fingerprint read on every check (+0.285 % of the work), 13 seed `#line`s and a
  seed refresh for every `core` version, a hook change without which the net
  cannot run on it, and walks to teach (the compiler-engineer, measured; the
  critic, confirmed). Principle 0 admits a form the compiler needs or one with
  a measured effect; this one has neither.
- **R2, what route 1 was to buy is bought now, without a package.** Every live
  slicing helper and inline cut of `selfhost/` that can meet a text past ASCII
  is repaired onto `cli/strings`' byte-safe helpers, in its own round with its
  own cases: defects **2409** (`emit/ffi_nested.hero`), **2410**
  (`module/catalogue.hero`) and **2411** (`fetch/catalogue_edit.hero`), all
  `blocking`, and every line of the critic's list covered or written down with
  why it cannot be reached (lane `b28-slice`, opened 2026-11-10). This is the
  compiler-engineer's approved route, with the critic's correction that the
  inline cuts are in it too.
- **R3, the form 2315's notes name is withdrawn.** A copy of a package at
  `selfhost/core/` reached by plain `use` lines does not compile (exit 1,
  measured), and a `path` entry over it is a package with no fingerprint, told
  *unverified* on every check. **When the compiler takes a package, it takes it
  as route 1 takes `core`**: a catalogue entry, a placed tree, a fingerprint.
  And only scoped to **replace**: the whole contract's live copies and
  `cli/strings`' functions it matches deleted, lines removed, the `as` the 64
  modules owe written; with, before the step lands, the trunk's provenance
  mirror following `use <name>/...` (the compiler-engineer's patch, held by the
  hooks' own tests), `order`, `layout`, `spec` and `packages` taught
  `selfhost/`'s placed tree and the net green on it, and `seed/README.md`
  saying that each version the compiler takes is a refresh.
- **R4, whether the compiler may carry a package at all is a Part 1 question,
  past this lane.** design.md:570-571, *"No package is compiled into the
  compiler or the runtime (panel 097's condition 5: such source is the standard
  library this section forbids)"*, reads by its letter against any catalogue
  entry of the compiler's; its source, panel 097's condition 5, is *"Nothing
  reaches `selfhost/library_source.hero`"* (`docs/panel/097-*.md:128-129`), and
  panel 234's R13 already let the compiler take a checked copy. Reading that
  sentence as its source reads, or amending it, is a sentence of design.md
  Part 1, which CLAUDE.md § 4 sends to the full panel. So R3's route is
  recorded, with its costs, for that sitting, and not adopted here.
- **R5, `crypto/sha256` is Heroes alone.** When the package is written, route
  (d): streaming over `[u8]`, the file read through `stdio.h` with § 13's fill
  form, no C, admitted by the first program that needs it (`examples/packages/README.md`
  § 7). It is correct on all three machines with nothing installed (14 of 14
  files each), and design.md:568-569 asks for it, *"written in Heroes alone
  where Heroes can say it"*; its cost is speed, 0.81 s for 64 MiB at `-O2`, 4.0
  times libcrypto, which stops no program anyone named (CLAUDE.md § 13). So
  M-core-packages' page row *"`crypto/sha256`, `crypto/hmac` | `pkgconfig
  "libcrypto"`"* (line 32) is answered: its own route is (d), and (a) is the
  route a program takes by binding libcrypto itself, with the traps the
  ffi-pragmatist recorded. **(b) is not core's route**: 122 lines of a hashing
  algorithm in C is no shim and no hard case (design.md:599-601, *"shims are
  for the hard cases only"*), it is a standard library in C beneath §1.12's
  robustness, read here in words and not amended. **(c) is refused for a
  program's package**: it chains the package's version to the compiler's
  (measured), and design.md:575 says *"nothing a program could carry
  instead"*, which (a), (b) and (d) are. The compiler keeps its two C doors in
  `hero_compiler.h` where panel 235's R6 put them; whether its own fingerprint
  moves to (d) goes with R4, carrying the critic's arithmetic, about 0.14 % of
  a check, unrun.
- **R6, question 3 has no route adopted in this lane.** R19's *"the transport
  alone"* stands as written, and neither seat built a transport over a core
  package: `os/exec` binds `hero_os.h` a second time beside `cli/process` or
  moves the compiler's process layer behind a placed tree (the
  compiler-engineer, by count), and a transport over libcurl meets panel 235's
  R10, *"The second version, over the core packages, meets the seed's question
  again"*, unrun for libcurl and unread on the box. The milestone page's own
  rows (`os` as `cli/process.hero:37-56` lifted out, line 26; `net/http` over
  libcurl, line 33) go to the full sitting with R4.
- **R7, the environment's write a transport needs.** **An `os` package's own
  header with a platform arm is refused** (the ffi-pragmatist's veto,
  design.md:572 and panel 234's R6: the planted dead arm green on two machines
  of three). **A package binding `hero_env_put` from `hero_compiler.h` is
  refused** (an unpromised header whose own words are *"which no program
  needs"*, and on Windows a value its program's `getenv` cannot see). **The
  route the ffi-pragmatist approves, `hero_env_put` moved into `hero_os.h`
  with its Windows arm over `_wputenv_s`, is recorded and not adopted**: it
  adds a door to decision 2228's promise (design.md:3641-3645, Part 7), task
  2263 owes a stated ownership for every `HeroStr` result of that header, and
  the empty value is unrun on the box, which a caller in the tree passes
  (`tests/harness/case_setting.hero:212-226` restores an unset variable as
  `""`; the UCRT's `_putenv_s` reads an empty value as a removal, the critic
  reading its documentation). Today's `hero_env_put` keeps its stated
  contract, a value every child inherits, and is no defect.
- **R8, feature 2315 cannot be delivered over `core` 0.2.0, and its step opens
  with a full sitting.** No package of 0.2.0 covers any of the package
  manager's C groups (the critic's list above, item 4 of its second pass), and the form its
  notes name does not compile (R3). What it needs first: `path/filepath`
  (decision 2404's reading, the likeliest package), `os`, `crypto/sha256` (R5)
  and a transport, each written and admitted by the package manager's own
  need; then the full sitting on R4, R6 and R7, the blind seat's paid run put
  to the author first. M-core-packages' acceptance condition stays open, and
  2315 takes a dated comment saying so.

**What the cheaper reading would have been**, recorded so the author can
choose it: route 1 as scoped, landed now, which closes 2315 by its title and
replaces nothing; and the `hero_env_put` move adopted here as the
ffi-pragmatist approved it, without the full sitting on the promise.

## Disagreements, not smoothed

- **The repair's reach.** The compiler-engineer: the copies repaired onto
  `cli/strings` *"close the two live panics"*. The critic: `catalogue.table`'s
  panic is an inline cut, not a copy, so the repair as worded closes one of
  them, and a third stands in `fetch/`. Checkable, and R2 takes the critic's
  reading.
- **The counts.** *"36 copies, 520 lines"* and *"33 copies, 488 lines"* (the
  compiler-engineer) against **16 live copies, 137 lines** (the critic, by each
  copy's line against its module's `## Tests`). Both are measurements of
  different sets; the synthesis uses the critic's for live code. The *"33 hand
  copies"* and route 1's *"33 calls"* are two counts with one number.
- **Question 3.** The compiler-engineer approves *"the transport alone"* and
  objects to the only move of it anyone sketched; the critic: that approval
  names no route. R6 adopts none.
- **`hero_env_put`.** The ffi-pragmatist approves the move into `hero_os.h`;
  the critic places it on decision 2228's promise and task 2263. R7 records
  the route and does not take it in this lane.

## Left open, as questions

- (d) as the compiler's own fingerprint: about 157 million instructions a check
  by arithmetic (93,113 bytes in the placed tree at 1,686 to 1,695
  instructions a byte at `-O0`, `selfhost/cli/sha256.hero:12-20`), about
  0.14 % of a check; unrun.
- `_wputenv_s(name, L"")` on the box: does it remove the variable, where
  `setenv(name, "", 1)` sets it empty? Unrun; the run behind the
  ffi-pragmatist's `_wputenv_s` table is not on disk (only the changed line,
  `runtime-e`).
- libcurl on the seed's build line, and on the box; unrun.
- Stage 2 over a runtime with the four doors moved; stage 1 builds over it
  (the critic, `pass2/seed-m.log`, exit 0), stage 2 unrun.
- A path past ASCII opened by the three package routes' `fopen` on the box;
  unrun (every file hashed there was ASCII-named).
- The `heroes run --sanitize` reach of `cli/symbolized.hero:73`'s cut on
  Windows with a long `ASAN_OPTIONS` past ASCII; read, not run (R2's round
  covers the line).
- Two diagnostic findings of the ffi-pragmatist, unfiled by the seat: a wrong
  arm of a package's own header told as *two headers refusing each other*
  (`ffi_header_refused`), where the mistake is one header's; and a group naming
  `stdio.h` for a function the runtime's header declares building, its
  signature held and its header's name unchecked. Filed by the coordinator as
  `adjacent` defects **2414** and **2413**, the second reproduced on this Mac,
  the first unrun by the coordinator (`ssh win` timed out at 09:05).

## Predictions to score

- **compiler-engineer**, conditional on a route 1 landing: the first commit
  that moves the compiler's `core` entry past 0.2.0, or its `index` line,
  either rewrites `seed/heroes.c` in the same round or reads `records` red on a
  `#line` naming the old placed folder. Checkable at the milestone that first
  takes a `core` into the compiler; unscorable while R1 stands.
- **compiler-engineer**, conditional likewise: a close's `compare --clock all`
  reads no loss for a compiler-side `core` whatever it costs, since both
  compilers check the same tree; only `record`'s row held to the level can see
  it.
- **ffi-pragmatist**: at M-core-packages' close, a `crypto/sha256` in Heroes
  alone gives `shasum -a 256`'s digest for every file of its tests on all four
  CI legs with no line of C and nothing installed, within 5 times a nine-member
  libcrypto binding at `-O2` on 64 MiB on the macOS leg. Its second half, the
  `hero_env_put` move landing with the ABI unmoved and Windows' `getenv`
  reading what it wrote, rides on R7's full sitting and is scored there.

## Process notes

- Reduced by the author's answer: two seats, the critic on both passes, no
  blind seat, no historian, no paid run. The critic's first pass found the
  first briefs' third fact false (*no program reads a file's bytes*: the
  critic's own probe read `5 0 1 2 255 254` through `stdio.h`) and three
  framings wrong; the briefs were rewritten before any seat started, and the
  rewrite lists what changed (`00-shared.md`, last section).
- Each seat worked in its own detached worktree at the base with its own
  compiler from the seed (`.claude/worktrees/241-compiler-engineer`,
  `241-ffi-pragmatist`), the ffi-pragmatist also in `C:/w/p241-ffi/` on the box
  and in the Linux arm64 container; the critic's second pass in a worktree of
  its own, removed when done. The ffi-pragmatist restored its worktree to the
  base (`git status --short` empty); the compiler-engineer's holds route 1
  uncommitted, its diff kept in the evidence.
- The trunk was not touched by any seat; its `.claude/settings.json` held a
  local change of this session's coordinator throughout (the author's
  exception for pushes, 2026-11-10), which the critic ruled out of item 11 by
  running the hooks' tests in a clean worktree.
- One slip by the critic: a list redirected to `/tmp/x` for one command and
  removed by the same command; nothing else outside the repository.
- No panel file is a design change: R1 to R8 land nothing in `spec/` or
  `design.md`. The repairs R2 names are defects, gated and closed by their own
  round.

## Author's verdict

Pending. The sitting's decision issue, `Panel 241:`, is filed open. **What a
yes settles**: R1 to R8 as adopted, so the compiler takes no `core` package at
step 6, the slicing repairs land as defects 2409 to 2411, `crypto/sha256` is
written in Heroes alone when a program first needs it, an `os` package with a
platform arm and a package binding `hero_compiler.h` are refused, and feature
2315 waits for its packages and a full sitting, M-core-packages' acceptance
condition staying open. **What it does not settle**: R4's reading of
design.md:571, R6's transport and R7's move of `hero_env_put` into
`hero_os.h`, each the full sitting's; the blind seat's paid run of that
sitting, put to the author before it runs; and the predictions above, on their
own clocks.
