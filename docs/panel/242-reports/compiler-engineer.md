# Panel 242, the compiler-engineer's report

Seat: compiler-engineer (the ceiling: design.md §1.1, §1.7, Part 5). Worktree
`.claude/worktrees/242-compiler-engineer`, detached at `1edabb957`. Scratch and logs:
`.claude/worktrees/p242-scratch/compiler-engineer/` (written `$S` below). Written as it goes; every
number names the command that produced it. Seconds are planning figures only (the ffi-pragmatist
builds on this Mac at the same time); costs are in instructions retired where a cost is measured.

## Status, as it goes

- 2026-11-10, start. Briefs read (`00-shared.md`, `compiler-engineer.md`), the critic's first pass
  (`critic-briefs.md`), panel 241's file and its compiler-engineer report, its evidence extracted to
  `$S/ev241/`.
- Stage 1 from the seed in the worktree: `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`
  exit 0, `real 13.33 user 12.09` (planning figure), `./heroes --version` prints `heroes 0.6.0`,
  `sources 5aae00294f7cd6fa` (the seed's key, as the critic read for `heroes-seed-o2`).
- Stage 2 (`./heroes build selfhost/main.hero -o heroes-next`) started.
- Stage 2 by stage 1: exit 0, `real 177.86 user 117.80` cold at load 10 (planning figure);
  `./heroes-next --version` prints `sources 45824e7205ca021e`, the tree's key, which the trunk's
  hook mirror also computes for this worktree (`provenance.tree_sources`). Kept as
  `$S/heroes-base`.
- Baseline stage 3 (`./heroes-next build selfhost/main.hero --emit-c`): **33,255,004 bytes, 318,015
  lines, 67,434 `#line`s** (`wc -c`, `wc -l`, `grep -c '^#line'`).
- Baseline's own tests, in `$S/base/` (a `git archive 1edabb957` of the whole tree, so the fixtures
  are there): **`1995 tests, all passed`**, exit 0, `1,152,424,692,543 instructions retired`
  (`/usr/bin/time -l`), `real 259.85` (planning figure).

## Question 1, route (A) scoped to replace: built

### The admission count again, at `1edabb957`

**How it was enumerated** (from the tree, not from panel 241's list): every function of
`selfhost/` with the shape `(str, str) -> bool`, `grep -rnE '^function [a-z_0-9]+\([a-z_0-9]+: str,
[a-z_0-9]+: str\) -> bool' selfhost --include='*.hero'` -> **75** in 61 modules; each body printed
whole (`$S/bodies.py`, output `$S/strstr-bodies.txt`, 970 lines) and read, never the name; marked
LIVE or TEST by its line against its module's `## Tests` (the critic's ruler in 241), 55 and 20.

`core` 0.2.0's `strings` exports `has_prefix`, `has_suffix`, `contains`, `trim_space`, `fields`,
`split_lines`, `split_once` (`grep -n '^function' packages/core/0.2.0/files/strings.hero`). Of the
75, **32 have the contract of `has_prefix`, `has_suffix` or `contains`**: `cli/strings`' three, and
**29 copies outside it, 13 live and 16 test support** (241 counted 36 with `cli/strings`' three; the
b28 round deleted four, `holds`, `has`, `has`, `opens_with`, the critic's `git diff`; 36 - 4 = 32).

| contract | live copies outside `cli/strings` | test-support copies |
|---|---|---|
| `contains` | 6: `foreign_words.strings_has`, `cli/clang_died.has_text`, `cli/grammar.strings_contains`, `mutate/ops.contains`, `mutate/judged.contains`, `emit/ffi_lookup.has` | 16: `diag_render.diag_has`, `discard_errors.has`, `emit.has`, `cli/lex.has_text`, `cli/help.has`, `cli/doctor.has_text`, `cli/check.has_text`, `cli/libraries.has_text`, `emit/extern_record.has`, `emit/extern_assert.has`, `emit/decls.has`, `emit/extern_union.has`, `emit/c_main.has`, `emit/c_words.found`, `emit/ffi_constant_value.strings_hold`, `check/holes.has_text` |
| `has_prefix` | 5: `cli/standard_name.starts`, `mutate/op_brackets.starts`, `emit/layout_rows.starts`, `emit/ffi_incomplete.begins`, `emit/ffi_answer.begins` | 0 |
| `has_suffix` | 2: `cli/standard_name.ends_with`, `mutate/op_brackets.ends` | 0 |

Read and **not** counted (their answers differ from `core`'s on some input): `head_windows.same_letters`,
`pkgconfig_head.lower_is`, `near_names.same_ignoring_case`, `spelling.same_but_case` (case folded);
the two `before` (an order); `clang_name.printed_as`; every other body of the 75 (a different
question). **One reading owed**: nine of the counted copies compare `.chars()`, not bytes
(`has_text`, `has`, `diag_has`, and `ffi_lookup.has` and both `begins` over
`ffi_lookup.char_index_of`). `hero_str_chars` cuts at each UTF-8 sequence and gives an ill-formed
byte alone (`runtime/parts/text.c:57-94`), so a character match is a byte match at a boundary: they
agree with `core`'s byte loop on every needle that is well-formed UTF-8, whatever the text, and
differ only on a needle that is itself ill-formed (a lone continuation byte, which the byte loop finds
inside a character and the character loop does not). Whether any needle at a moved call site can
be ill-formed is **unread**: some are built from a program's words (`emit/ffi_build.hero:124`,
`"library '" + name + "' not found"`), so the move is a change of answer only where such a word
holds an ill-formed byte, and there the byte loop's answer is the true one.

**The calls these contracts carry** (`$S/calls.py`, `strings.<f>(` outside comment lines, by each
module's `## Tests`): `starts_with` 142 (110 live) in 66 modules, `ends_with` 68 (38 live) in 39,
`holds` 209 (46 live) in 60; **419 calls in 101 modules**. `ffi_lookup.has` carries **187 more in 40
modules** (`grep -rhoE '\bffi_lookup\.has\('`), the largest copy by reach.

**The `as` count**: 125 modules `use cli/strings` (`grep -rlE '^use cli/strings( as [a-z_]+)?$'`,
none aliased); 39 call both a helper `core` has a contract for and one it has none for (`with_all`
54 modules, `trimmed` 17, `without_returns` 3; two hits of my first count, `strings.h` in a literal
of `cli/own_names.hero:338` and the field `p.strings` at `ir/print.hero:56`, set aside by reading);
62 call only the three; 24 only the rest (`$S/permod.py`).

### Route (A) built: what moved, what was deleted

- **The package**: `./heroes-next add core --index file://../packages -C selfhost`, exit 0 in
  `real 0.16`; wrote `selfhost/heroes.toml` (5 lines: `index = "file://../packages"`,
  `[dependencies.core]`, `version = "0.2.0"`, `sum = "sha256-8a5f9d20...eabf4"`) and placed
  `selfhost/heroes.packages/core@0.2.0-4029f4f2486e6ca7/`, the same key as panel 241's: 13 files,
  10 modules, **2,334 lines of Heroes** and 55 of `.md` (`find ... -name '*.hero' -exec cat {} + |
  wc -l`).
- **The move** (`$S/route_a.py`, phase 1, then `$S/fix_leftovers.py` for two calls the first pass
  missed: one after the literal `"\\"` on `cli/shadowed.hero:475`, and the 13 of
  `emit/extern_probe.hero`, which imports `emit/ffi_lookup as ffi_lookup`): **654 calls moved**,
  419 of `cli/strings`' three and 235 of the 13 copies (40 inside their own modules, 195 in others);
  labels `text:` to `s:`, `needle:`/`word:` to `substr:`, `head:` to `prefix:`, `tail:` to
  `suffix:`. `grep -rnE` for any old spelling afterwards: 0.
- **Deleted: 16 functions**, `cli/strings`' `starts_with`, `holds`, `ends_with` (49 lines with their
  comments) and the 13 live copies (137 lines with their comments, `$S/route_a-phase1.out`): **186
  lines**; then **41 lines of `cli/strings`' tests** (`git diff` lines after its `## Tests`): three
  whole tests with their comments, defect 296's case and defects 2409 to 2411's among them, and four
  asserts out of two others; `core`'s own tests hold
  the multibyte shapes only in part, `!contains(s: "caffè", substr: "e")`, `has_suffix(s: "caffè",
  suffix: "è")`), and `cli/strings`' `use bytes`, dead once `holds` went. `cli/strings.hero` 218
  to 126 lines (`git diff --numstat`: 0 added, 92 deleted).
- **The checker's part**: `./heroes-next check selfhost/main.hero --brief` exit 1 with **8
  `unused_binding` errors**, a `use` the move left dead (`emit/ffi_lookup` in 6 modules,
  `cli/standard_name` in `cli/own_names`, `bytes` in `mutate/op_brackets`); removed; exit 0. Then
  `heroes fmt --in-place` on all 144 touched modules, 0 failures.
- **The `use` lines**: 142 modules gain a `<core>` line (`grep -c '^+use <core>/strings'`): **62 swap
  `use cli/strings` for `use <core>/strings`; 43 keep `cli/strings` and add `use <core>/strings as
  core_strings`** (the 39 counted above, and four that call a moved copy as well); **37 had no
  `cli/strings` and add `use <core>/strings`**, 30 of them under `emit/` (counted over the diff).
  The formatter writes a blank line between the `<core>` line and a module's own
  `use` lines (140 blank lines added, `grep -c '^+$'`).
- **The whole diff of the compiler's own modules**: `git diff --shortstat -- selfhost` -> **144
  files changed, 950 insertions(+), 964 deletions(-)**: **14 lines fewer**, against **2,334 placed
  and 5 of catalogue added**. The compiler's tree grows by 2,325 lines of Heroes, net.

### Question 1 (i): the compiler depending on a package is outside B5, by a compiler fact

B5 was *"the canonical tree compiled into the compiler"* for programs (`sed -n 200,201p
docs/panel/234-briefs/00-shared.md`), P-embed applied to `core/`; panel 234's compiler-engineer
vetoed it as panel 097's condition 5 *"applied to twenty packages rather than one file"*, the file
being `library_source.hero`, whose text *"is compiled with every program"*
(`docs/panel/234-reports/compiler-engineer.md:180-203`). **The fact that decides it is what a
program gets**, so I ran it (`$S/noreach/`), the baseline and route (A)'s stage 2 each:

- a program with `use <core>/strings` and no catalogue: both exit 1, `error[unknown_entry]: <core>
  names no entry: there is no catalogue heroes.toml beside main.hero`;
- `use core/strings`, B5's own form: both exit 1, `unknown_module`;
- no `use`, calling `has_prefix`: both exit 1, `unknown_name`;
- the three logs `cmp`-identical between the two compilers.

And what the binary carries (bytes searched with Python, `ugrep -c` printing nothing on a binary):
the prelude's embedded text, *"This file is compiled with every program"*, **1 time in each
binary**; `core`'s source line `function has_prefix(s: str, prefix: str) -> bool` and its doc comment
**0 times in each**. Route (A)'s binary is 22,868,520 bytes against 22,866,872, **+1,648**.

**So route (A) is not B5**: B5 puts source in the compiler that programs reach without carrying it;
route (A) compiles `core`'s functions into the compiler's own machine code, which no program reaches
(measured), and every program still carries its own entry, tree and fingerprint. **But design.md:570's
letter, *"No package is compiled into the compiler"*, does cover route (A)**: `core`'s code is in the
binary. The reading back to the source would be a sentence, *no package is compiled into the
compiler for programs to use*, which is a Part 1 sentence and so a wording for the sitting, not a
fact of mine. Route (A) as an amendment reverses nothing panel 234 ratified; as a reading it needs
the sentence written.

### The guard, and what it leaves unrun

The trunk's mirror (`.claude/hooks/provenance.py`, read only) keys one root, `ROOT =
selfhost/main.hero` (line 49), and its `USE` pattern (line 51) matches a path opening with a letter
or `_`, so a `use <core>/strings` line is silently not followed. On route (A): the mirror keys the
tree **`4eed491c39f6b330`**, the compiler says **`57ad6537316279a5`** (`provenance.tree_sources`,
`compiler_said`). `./heroes-a2 run tests/harness/main.hero -- ./heroes-a2 order` was **refused by
the trunk's PreToolUse hook** (*"keys 4eed491c39f6b330 by the hook's mirror"*), and not got around.
So on route (A) **every net suite is unrun**, the `selfhost/**` row's `canonical`, `layout`,
`order`, `probe`, `records`, `seed` and `spec` first, and `packages`, `unseen`, `emission`,
`descriptors` and `wholes` with them. The patch that would let them run is panel 241's
(`$S/ev241/evidence/compiler-engineer/route1.diff`, `provenance.py` +60 -5 and its test +18 -2),
not applied to the trunk.

### Route (C): a copy as the compiler's own modules, built and checked

**What panel 241 measured refused** was the copy under `selfhost/core/`: its own `use
strings/internal/scan` reads from `selfhost/`, and `internal` is private to a `strings` the copy is
not. **A placement nobody listed compiles with no byte of the copy edited**: `core`'s files at
`selfhost/`'s own root, `selfhost/strings.hero` and `selfhost/strings/internal/scan.hero`, where
the package's root-relative `use` lines (`grep -rn '^use ' packages/core/0.2.0/files`: every one
names its own package's path) resolve as written. `selfhost/` holds no `strings`, `strconv` or
`encoding` at its root (`ls`, 1edabb957). Run on a scratch copy of route (A)'s tree
(`$S/routeC/`): catalogue and placed folder removed, the two files copied (`cmp` against
`packages/core/0.2.0/files/strings.hero`, silent), and the 142 importers' line rewritten from `use
<core>/strings` to `use strings` (the 43 `as core_strings` kept): `$S/heroes-base check
selfhost/main.hero --brief` **exit 0**.

**Is it (A) without the fingerprint?** It is (A)'s deletions and moves, call for call, without
four of (A)'s costs, and with one cost (A) does not have:

| | (A), catalogue entry | (C) at the root |
|---|---|---|
| deleted, moved | 16 functions, 654 calls | the same |
| added to the tree | 2,334 lines placed (`encoding/json` and `strconv` among them, unreached) and 5 of catalogue | `strings` alone, 368 lines (`wc -l`), the only package of `core` the compiler reaches |
| the fingerprint | the placed tree fingerprinted at every build, 36 million instructions a check with the catalogue's reading (below) | none; the tree's own cost, +0.30 % a check, stays (below) |
| the seed's `#line`s | 13 name `heroes.packages/core@0.2.0-4029f4f2486e6ca7/`, a name that moves with every version, sum or `index` spelling (`module/placed.hero:108-117`), so every version is a refresh | they name `selfhost/strings.hero`, which no version renames |
| the trunk's hook mirror | refuses every net run until it follows `use <core>/` (measured above) | follows `use strings`, a path opening with a letter (`provenance.py:51`); measured below |
| held to the index | by the fingerprint at every build, and by no suite (`packages` pins `INDEX_LINE` to `file://../../packages`, `suite_packages.hero:93-94`) | by **nothing until a suite is written**: a check in `suite_packages.hero` beside `placed` (lines 312-389, 78 lines, which holds every catalogue under `examples/` to the index byte for byte) naming the version and holding the two files `cmp`-equal to its `files/`, about 30 to 40 lines with its test, by that precedent; an estimate |
| the walks | must learn `selfhost/heroes.packages/` (241: `order`, `layout`, `spec`; 31 to about 95 lines, estimated) | read the copy as the compiler's own modules, with no line taught; whether the package passes the compiler's own walks is run below |

So (C) at the root is **not** (A) without the fingerprint: it moves the check from every build of
every user of the compiler to one suite of the net, and it takes the package as code of the
compiler, held to the index by a check rather than by the language's own verification. That is
what panel 234's R13 asked (*"the compiler takes a copy under `selfhost/core/` only if the
admission test admits it"*), at another folder.

### Route (A): the two stages, the fixpoint, the tests, and every check's cost

- **Stage 2 by the seed's compiler**: `./heroes build selfhost/main.hero -o heroes-a2` exit 0,
  `sources 57ad6537316279a5`; **stage 3 by stage 2**: exit 0, the same key; `--emit-c` of both
  **33,266,740 bytes each, `cmp` exit 0: the fixpoint holds.** The emitted key,
  `hero_program_sources(...) "57ad6537316279a5"`, is the one `--version` says, so by panel 222's R2
  the compiler is the arbiter and the trunk's mirror is the one that is wrong.
- **The seed it would carry**: 33,266,740 bytes, 318,118 lines, 67,430 `#line`s, against the
  baseline's 33,255,004, 318,015, 67,434: **+11,736 bytes, +103 lines**; **13 `#line`s name the
  placed folder** (10 `strings.hero`, 3 `strings/internal/scan.hero`), as in panel 241.
- **The binary**: 22,868,520 bytes against 22,866,872, +1,648.
- **The compiler's own tests**: `./heroes-a2 test selfhost/main.hero` **`2006 tests, all passed`**,
  exit 0, against the baseline's 1,995: 1,995, less the three whole tests of `cli/strings` that
  went, plus `core`'s 14 run as the compiler's (`grep -c '^test '`: 11 in `strings.hero`, 3 in
  `scan.hero`).
- **Every `heroes check selfhost/main.hero`**, in instructions retired (`/usr/bin/time -l`), each
  compiler before its own tree (`$S/instr.sh`: the baseline binary in `$S/base/`, route (A)'s in the
  worktree), six alternated pairs, the first discarded: baseline median **110,667,202,906**, route
  (A) **110,880,894,822**: **+213,691,916, +0.193 %**, and every route-(A) run above every baseline
  run by at least 151,752,681 (`$S/instr-check.txt`). Panel 241's route 1, which deleted nothing,
  read +0.285 %; replacing the copies buys back about a third of it and leaves the rest.
  Decision 2282's floor of effect is a median 0.5 %, so no close's `compare` would call this a
  loss; and a close's `compare` runs both compilers on one tree (`latency_runs.hero:28,39`, read by
  panel 241), so it cannot see the tree's half at all. **The batches move**: the baseline's own
  count reads 110.49 to 110.72 billion between batches, so only pairs inside one alternated batch
  compare.
- **Where the cost lives** (`$S/instr2.sh`, four rounds in one batch, each round the four runs
  below in turn; medians, `$S/instr-halves.txt`): the baseline binary on the base tree
  110,520,836,370; **the baseline binary on route (A)'s tree +366,494,415 (+0.332 %)**; on route
  (C)'s tree, the copy at the root (`$S/routeC/`, before the formatter took out its 139 blank lines),
  +330,304,196 (+0.299 %); **route (A)'s binary on the base tree
  +61,017,180 (+0.055 %), inside its own spread of 138,323,947**. So the cost is the tree's, not the
  binary's: the checker reads 368 more lines of `core` and 142 more `use` lines than it reads lines
  deleted. And **the catalogue, the fingerprint and the placed folder together cost
  36,190,218 a check** (route (A)'s tree less route (C)'s, one binary), which matches the C door's
  fingerprint of the placed tree, measured below at 29.4 million. Removing the fingerprint, which is
  what (C) does, saves about a tenth of (A)'s per-check cost; the rest is what any copy of a package
  costs the compiler, by any route.

## Question 1 (ii): `crypto/sha256` and the verifier among the verified

**What the compiler hashes today** (`git grep` at 1edabb957): `cli/sha256.hero`, 292 lines (256
before its tests), the Heroes SHA-256 of a `str`, called live by `module/placed.hero:112,117` (the
placed folder's key) and `cli/check_json.hero:132` (a digest in `check --json`), and by a test,
`module/fingerprint.hero:76`, which asserts it agrees with the C door; and the two C doors of
`hero_compiler.h`, `hero_file_sha256` and `hero_text_sha256` (`runtime/hero_compiler.h:52,56`, over
`runtime/parts/sha256.c`, 126 lines, whose only callers are those two doors: `grep -rn
'hero_sha256_' runtime`), bound in `module/fingerprint.hero:23-25` (every fingerprint at every build)
and `fetch/index_source.hero:39-41` (a fetched tree held to its sum). The sources key every program
carries uses another door, `hero_text_key` (`emit/provenance.hero:35`), not SHA-256.

**What a Heroes-alone `crypto/sha256` would delete, if the compiler took it** (no such package is in
the index; panel 241's route (d), 288 lines, `$S/ev241/.../index/shad/`, is the measured stand-in):
at most `cli/sha256.hero` (292), `runtime/parts/sha256.c` (126) with its two declarations in
`hero_compiler.h`, and the two `extern` groups (8 lines): **about 430 lines**, against **288
placed** and a catalogue entry; whether the package's `sum256(data: [u8])` serves the compiler's
`of(text: str)` call sites without a conversion of each `str` to `[u8]` is unread.

**What the fingerprint would then cost**, measured (`$S/fp/`): two probe programs built at the
compiler's own default level (`-O0`), each hashing the 13 files of `core` 0.2.0's placed tree twenty
times, every digest equal to `shasum -a 256`'s: route (d) median **3,618,485,479** instructions, the
C door **607,524,549**, the empty program about 18.6 million, so **1,933 instructions a byte in
Heroes against 316 through the C door**, and **one fingerprint of `core` 0.2.0's tree (93,113
bytes) 180.0 million against 29.4 million**: +150.6 million a check, **+0.14 %** of a check, which
confirms panel 241's arithmetic (157 million) by a run. Every program with a placed tree pays it at
every build, in proportion to its packages' bytes.

**Can a compiler verify the placed tree of the package that implements its verification?** A
question of trust I can answer from the compiler's facts, not from a run of the attack:

- **No compiler ever verifies a tree with code from that same tree.** The verifier that runs is
  the binary's compiled code, and the binary was built from the tree before it (the seed, or the
  previous stage). Stage 1 (clang on the seed) holds stage 2's placed `crypto` tree to the
  catalogue's `sum` with the seed's own copy of the verifier; stage 2 holds stage 3's with the code
  compiled from the tree it just verified; the fixpoint's `cmp` then holds stage 2's C to stage 3's.
  There is no plugin and no load of source at run time (the binary holds no `core` source text,
  measured above), so no circle closes inside one process.
- **What the seed and the first fetch check, and against what**: the first `heroes add crypto`
  runs a compiler that does not yet carry the package, so the fetched files are held to the index's
  `versions` sum by today's C door; the sum was written by whoever published the version, with
  their compiler's verifier. From then on the catalogue's `sum` is the anchor, and each stage
  checks the next stage's tree against it. A package whose SHA-256 is wrong is caught loudly, at
  stage 3: stage 2 carries the wrong code, computes another fingerprint for the placed tree than
  the catalogue's `sum`, and refuses its own tree. And it is caught earlier by its own FIPS 180-4
  vectors among the compiler's tests, as `cli/sha256.hero:262` holds them today.
- **What it does not close, and does not open**: a compiler that verifies dishonestly, and builds
  itself so as to stay dishonest, is Thompson's trusting-trust compiler. That is true of every
  self-hosting compiler and of today's `module/fingerprint.hero` too: it is already compiled into
  the compiler it verifies for, and held only by the fixpoint and by review. Moving the verifier
  into a package adds no new hole of that kind. **It adds one coupling**: every `crypto` version the
  compiler takes renames the placed folder in the seed's `#line`s, so it is a seed refresh (13
  `#line`s for `core`, measured above), and a broken version refuses every program's placed tree
  at once, the compiler's own first.

So **no veto on soundness**. The objection is the ceiling's: the move deletes about 430 lines and
adds 288 placed and +0.14 % to every check of the compiler, and the same in proportion to every
program with packages, to replace C that is correct, tested against the Heroes copy
(`fingerprint.hero:76`) and promised by no header a program binds.

### Route (C) in the worktree, so the trunk's guard judges it

Route (A)'s state saved first (`$S/routeA-final.diff`, 6,490 lines; `$S/routeA-untracked.tar`, the
catalogue and the placed tree; `$S/heroes-a2`). Then in the worktree: the catalogue and the placed
folder removed, `core`'s two `strings` files copied to `selfhost/`'s root (`cmp`-identical to the
index's), the 142 importers' line rewritten to `use strings`, `heroes fmt --in-place` on the 144
touched modules (0 failures; the copy itself is canonical as published, `heroes fmt` exit 0 and the
file unchanged), `heroes check selfhost/main.hero` **exit 0**. The formatter sorts a plain `use
strings` among the module's own lines and writes no blank line after it, so **the importers' diff is
+811 −964, 153 lines fewer than the base**, and the tree gains the copy's 368: **+215 lines net,
against route (A)'s +2,325.**

- Stage 2 by the seed's compiler: exit 0, **`sources 6eb7ef290fd22e94`**; **the trunk's mirror keys
  the tree `6eb7ef290fd22e94`**, the same, with no line of the hook changed, because `use strings`
  is a path its `USE` pattern follows. So the guard admits the net on route (C), and refuses it on
  route (A).
- **Unrun on route (C)**: stage 3 and its fixpoint, and the compiler's own tests. Its code is route
  (A)'s call for call, the modules' paths aside, and route (A)'s are green; that is an inference.
- The net on route (C): below, as the runs end.

## Question 4: the compiler's `path`-shaped helpers, counted as question 1 was

**How it was enumerated**: every function of `selfhost/` (base archive) whose parameters are all
`str`, whose result is `str`, `str?`, `bool`, `i64`, `i64?` or `[str]`, and whose code (comments
left out) names `'/'`, `"/"`, `'\\'`, `"."` or `".."`: **157** (`$S/pathfns.py`, output
`$S/pathfns.txt`, 2,207 lines), 150 live; the **63** whose name or shape could be a Go path contract
read body by body (48 in `$S/pathfns-read1.txt`, then 15; `grep -c '^==== '`); the rest are messages, URLs, C
spellings, module names. A copy is counted where its answer equals Go's on every input, as in
question 1.

| Go contract | exact copies in `selfhost/` | lines | near copies (agree on some inputs only) |
|---|---|---|---|
| `path.Split`, the file half | **3**: `check/render.module_last_part`, `mutate/own.base_name`, `module/names.binding_of` | 26 | `fetch/add.last_part` (trailing `/` dropped first, `"/"` gives `""`), `fetch/made_repo.last_part` (defect 2406's) |
| `path.Split`, the directory half | **1**: `module/paths.open_directory` | 20 | `module/paths.directory_text` (also cuts at `\`) |
| `path.Dir` | **0** | | `cli/shadowed_tree.folder_of` (Go's on clean paths; `a//b` gives `a/`), `mutate/planted.directory_of`, `module/reading.holder`, and defect 2406's three `directory_of` copies and four `last_slash` |
| `path.Clean` | **0** | | `cli/package_reads.lexical` (Go's after `\` becomes `/`), `module/sums.folded` (`""` gives `""`, `/..` gives `/..`), `cli/group_file.settled` (no `..`) |
| `path.Base`, `Join`, `Ext`, `IsAbs`, `Rel` | **0** | | `clang_tree.joined`, `planted.beside`, `cut_workers.beside` (no clean); `system_paths.absolute`, `head_names.rooted` (a drive letter on every platform); `stand_in.relative` |
| any `path/filepath` function | **0** | | seven cut at both `/` and `\` **on every platform** (`link_output.last_part`, `link_output.with_suffix`, `library_needs.base_of`, `supplied_names.base_of`, `paths.directory_text`, `names.stem_of`, `order_groups.stem`): Go's `filepath` on Windows, and neither `path` nor `filepath` on Unix, where `\` is a byte of a name |
| `strings.Split(p, "/")`, which `core` 0.2.0's `strings` lacks | **4**: `cli/agreed_writes.parts_of`, `fetch/local_path.parts_of`, `cli/package_reads.pieces`, `module/spelling.split` | 45 | |

**So, counted as question 1 was, `path/filepath` has no copy in the compiler whose answer is Go's on
every input**, and Go's `path` has four (46 lines), all `Split`'s halves. The rest are near copies
whose answers differ from Go's at the edges (defect 2406 measured eight of them against Go 1.27.3),
so moving one onto a package changes its answer, and each move owes its callers read, as defect
2406 read its eight. Decision 2404's lane counted by intent instead (21 in `selfhost/` that mean
`Base`, `Dir`, `IsAbs` or `Clean`, nine reading `\`, `gh issue view 2404`), which is the right
count for what a package should offer, and not the count of what it would delete. Its reasoning
that the compiler's paths are operating-system paths, so `filepath` and not `path`, meets a fact
of the compiler: **its helpers cut at `\` on Unix too**, which is neither package's contract. **So
the compiler would take first** not `path/filepath` but `strings.Split` (four exact copies, 45
lines) as a function of `core/strings` 0.3.0, beside `path.Split` (four, 46 lines) if `path` is
written. Each is a deletion of under 50 lines, at route (A)'s costs or route (C)'s.

### The net on route (C), the `selfhost/**` row, compiler `./heroes-c2`, one suite per harness process

| suite | reading | what it says |
|---|---|---|
| `order` | 3 passed, 0 failed | the copy has no unmarked order (only `strings` copied; 241's red was `encoding/json.hero:186`) |
| `spec` | 35 passed, 0 failed | |
| `canonical` | 2 passed, 0 failed | |
| `seed` | 1 passed, 0 failed | |
| `unseen` | 3 passed, 0 failed | |
| `packages` | 13 passed, 0 failed | **green because blind**: it holds nothing under `selfhost/` to the index, so the copy could drift and stay green |
| **`layout`** | **5 passed, 1 failed** | five modules at **301 lines of code, past §11's 300**: `cli/package_reads`, `cli/toolchain`, `cli/units` (each keeps `cli/strings` and gains a second `strings` line), `emit/ffi_field`, `emit/ffi_narrowed` (each gains its first); every one stood at exactly 300 at the base (`ceiling.code_lines`, the hooks' declared mirror of `layout`'s count, read only) |
| **`records`** | **27 passed, 1 failed** | `records/areas`: *"a directory of selfhost/ that `cards.AREAS` does not name"*, `selfhost/strings/`; one line of `tests/harness/cards.hero` |
| `probe` | 31 passed, 0 failed | no floor told (`grep FLOOR` over every log: none) |

**Route (A) would read the same `layout` red**: route (A)'s tree rebuilt in scratch from the saved diff
(`$S/routeA-tree/`, `patch` exit 0) and counted with the same mirror: **the same five modules, 300 to
301**, and no other (both lists computed over every module of `selfhost/`). On route (A) that is a
prediction, since its `layout` is behind the guard. So **any route that moves the compiler onto a
package costs five splits or five allowances before it lands**, the seams the module-shape rule asks
for (`.claude/rules/module-shape.md`), a cost no seat counted in panel 241.

## Question 1: what route (B) costs

If the compiler takes no package, the measured state stands: `cli/strings` in 125 modules, and the
**29 copies of `core/strings`' contracts outside it (13 live, 137 lines with their comments; 16 test
support)** stay, each repaired onto `cli/strings`' byte-safe three when it is found slicing, as panel
241's R2 did for four of them. What (B) owes in words:

- **Feature 2315** (`gh issue view 2315`, open, M-core-packages): its title, *"The package manager
  rewritten over the core packages"*, cannot be delivered, because the package manager is
  `selfhost/fetch/`, a part of the compiler; its notes' *"What it replaces: the package manager's
  parts over C, by packages copied into the compiler's tree under `selfhost/core/`"* is already
  withdrawn by panel 241's R3. Under (B) it is **closed as not planned** or **rewritten as a program
  outside the compiler**, which is route (D).
- **The milestone's acceptance condition** (`M-core-packages.md:413-417`, 5 lines, *"the package
  manager rewritten over the core packages, every golden of its interface passing unchanged"*): struck
  or rewritten, and **the author's instruction 3 (d) on the same page, *"no copy by hand survives a
  package's arrival"* (`M-core-packages.md:323`)**, scoped away from the compiler: `core` 0.2.0 has
  arrived and its contracts survive by hand in the compiler 29 times. That is a sentence of the
  author's, and narrowing it is the author's to do.
- design.md:570-572 stands by its letter, and no sentence of Part 1 moves.

## Question 2 from the compiler's side

**Route (b), an `os` package the compiler's `cli/process` moves onto.** `cli/process.hero` is 507
lines, 311 before its tests: 19 functions and 2 records over **18 functions and 2 constants of
`hero_os.h`** (`process.hero:40-64`; the critic's first pass, item 10, says 21, recounted here by
`grep -c '^    function'` over those lines) and 3 directory functions bound under `extern
"stdlib.h"` (`:66-69`, defect 2413's shape, open). Read body by body:

- **an `os` package would hold 14 of the 19, and both records**: `run` with `Ran` (Go's `os/exec`),
  `discard` (`os.DevNull`), `exe_suffix`, `mkdir_all` (`os.MkdirAll`), `path_exists` and
  `is_directory` (`os.Stat`), `remove_file` (`os.Remove`), `remove_tree` (`os.RemoveAll`),
  `rename_over` (`os.Rename`), `walk` and `files_under` with `Walked` (Go's `filepath.WalkDir`, a
  `path/filepath` function), `env` and `is_set` (`os.LookupEnv`), `pid` (`os.Getpid`);
- **the compiler's own are 5**: `because` (the clause every refusal ends with), `unnamed_said` (*"this
  compiler opens a file by a name it can hold: rename it"*), `env_written` and `written_value` (a
  value quoted byte by byte through `not_text`), and `serial` (a number no earlier call answered, for
  the cache's private files, defect 134);
- **how many of the 96 modules move**: all 96, and their 764 calls (`grep -rhoE
  '\bprocess\.[a-z_]+\('`, base archive): `mkdir_all` 176 in 71 modules, `remove_tree` 145 in 38,
  `discard` 98, `run` 82 in 43, every clang invocation among them. A move scoped to replace (panel
  241's R3) puts the compiler's whole process layer behind a placed tree; one that keeps
  `cli/process` as the compiler's wrapper over the package deletes nothing. Its messages move too:
  a package's failures carry the package's words, so each refusal the compiler tells through
  `because` is re-worded or wrapped (1 golden under `tests/golden/` holds *"the operating system's own
  reason is"*, `grep -rlF`). Route (C)'s placement does not help (b): `selfhost/` has no `os` at its
  root, so a copy there compiles, but it is the compiler's process layer, copied.

**Route (c), libcurl for the index read.** `git` stays a program under every route (panel 235's R19
and R10, carried). What the compiler's read gains: today `transport.got` runs `curl` with 21 words
(`fetch/transport.hero:60-119`), **2 + N runs for one package** (`index_source.hero:76`, the
versions; `:146`, the list; `:181`, each file), so 15 processes for one `heroes add core` of 0.2.0's
13 files, each with two scratch files for its status and its words, and curl's exits 22, 63 and the
rest mapped back to `absent`, `too_large` and `unreachable`. libcurl would give those as `CURLcode`
values in the process. That is a speed and a tidiness, and CLAUDE.md § 13 says speed is no
justification unless a program cannot run. What the seed's line becomes, measured on this Mac
(`$S/curlprobe/`): a C program over `curl/curl.h` builds with `-lcurl` and nothing more (macOS
carries `/usr/lib/libcurl.4.dylib`, `libcurl/8.7.1`), refused without it (*"Undefined symbols ...
_curl_easy_cleanup"*), and **loads libcurl beside `libSystem`, where `heroes` loads `libSystem` alone**
(`otool -L`, both binaries). So the line becomes `clang -I runtime seed/heroes.c runtime/runtime.c
-lcurl -o heroes` on macOS; Linux needs the library's development package; the Windows leg has none
(`ci.yml:517`, the critic's). The binding shape (defect 2229's `#undef`) is the ffi-pragmatist's.

## Question 3 from the compiler's side

The compiler's callers of `hero_env_put` are **3 modules, 5 calls**: `cli/symbolized.hero:57`
(`ASAN_OPTIONS`), `fetch/transport.hero:160-161` (`GIT_TERMINAL_PROMPT`, `GCM_INTERACTIVE`), and
`fetch/made_repo.hero:68-69` (two dates, test support), each **on the line before its child starts**
(read), each binding `hero_compiler.h` in an `extern` group of its own (`symbolized.hero:33`,
`made_repo.hero:21`, `transport.hero:47`). So **route (e), the environment given at the spawn, costs
the compiler a staging function in `cli/process` beside `hero_run_arg`**, about 5 lines, the 5 calls
moved onto it, and the 3 `extern` groups deleted; no caller in the compiler writes a variable it
reads back itself. The child is started by `fork` and `execvp` on POSIX (`runtime/parts/run.c:1083`),
and the runtime can start a thread of its own (`run.c:731`, `pthread_create`); where one runs, only
async-signal-safe calls may follow a fork, and `setenv` is not one (POSIX's `fork`, read, not run
here), so the child's environment is built before the fork and handed to an `exec` that takes one.
That runtime half, and Windows' block, are the ffi-pragmatist's; I have not built either.

## Robustness: what replacing the copies would close

Read from the 13 live copies' bodies (`$S/strstr-bodies.txt`) and counted: **none slices**, 0 `.slice(`
calls in the 13 and in the helpers they call (`char_index_of`, `byte_index`, `bytes.bytes_at`), comment
lines left out. Each walks bytes
(`text[at + i]`, `bytes.bytes_at`), or characters through `.chars()` and
`ffi_lookup.char_index_of`; the slicing copies were the four the b28 round deleted (defects 2409 to
2411). So at `1edabb957` **route (A) or (C) closes no crash and no wrong answer that I found**: the
character-based copies (`ffi_lookup.has`, 187 calls, `clang_died.has_text`, both `begins`) differ
from `core`'s byte loop only on a needle that is itself ill-formed UTF-8, where the byte loop's
answer is the true one, and they build an array of one-character texts at every call
(`hero_str_chars`, `runtime/parts/text.c:72-94`), which is speed, CLAUDE.md § 13's non-goal. Precedence's rank 3
does not decide question 1; rank 4, Principle 0, does.

## Verdicts

**Question 1 (i), whether the compiler depending on a package falls inside panel 234's R1 veto of
B5: it does not, by a compiler fact; no veto.** B5 is source in the compiler that programs reach
without carrying it, P-embed's mechanism, `library_source.hero`'s. Route (A)'s compiler gives a
program nothing: three programs with no catalogue are refused with `cmp`-identical diagnostics by
both compilers, and the binary holds the prelude's text once and `core`'s source never. So question 1
is **a reading of design.md:570-572 back to its source, written in words** (*no package is compiled
into the compiler for programs to use*), not a reversal of a ratified veto. The sentence's letter,
*"No package is compiled into the compiler"*, does cover (A) and (C), so the sitting owes that
sentence either way.

**Question 1, the routes: object to (A); if the compiler is to carry a package, approve (C) at
`selfhost/`'s root over (A); no veto.** No route adds a core construct (no checker, lowering or
backend change: two-stage builds, fixpoints and the compiler's own tests green, measured), so my veto
does not reach it, and design.md §1.7's test decides: does it remove a special case from the
compiler? Measured at 1edabb957, **(A) scoped to replace deletes 16 functions, 186 lines, and moves
654 calls; it adds 2,334 placed lines and 5 of catalogue, so the compiler's tree grows by 2,325
lines net**; every check costs +0.19 % of its instructions (+0.33 % in the second batch, the tree's
half); 13 seed `#line`s make every `core` version a seed refresh; the trunk's guard refuses every net
suite until the mirror follows `use <core>/`; five modules cross §11's 300; and the copies it
replaces close no crash and no wrong answer that I found. **(C) at the root deletes and moves the
same, adds 368 lines (+215 net), needs no hook (the mirror keys its tree as the compiler does,
measured), names a stable path in the seed, and costs every check +0.30 %**. The fingerprint, which
it drops, is 36 million instructions of (A)'s 366 million. It owes the net a check holding the copy
to the index (about 30 to 40 lines, estimated), one line of `cards.AREAS`, and the same five
splits; of the `selfhost/**` row and `packages` and `unseen`, the net reads six suites green and two
red, on exactly those two; `probe` is the seventh green. **(B) costs 0 lines** and three
sentences: feature 2315 closed or moved, the acceptance condition (`M-core-packages.md:413-417`)
struck, and the author's instruction 3 (d), *"no copy by hand survives a package's arrival"*
(`:323`), scoped away from the compiler. **Principle 0** (CLAUDE.md § 2): the compiler self-hosts
without any of them (the baseline's fixpoint), and no Part 11 effect is measured, so neither (A) nor
(C) enters on need. What would bring one in is the author's 3 (d) read as binding the compiler: that
is the author's sentence and the author's call, and if it binds, (C) is the cheaper route by every
count above.

**Question 1 (ii), `crypto/sha256` as the compiler's fingerprint: object; no veto on soundness.** The
verifier never checks the tree it was compiled from in the same process, and the anchor is the
catalogue's `sum`, so the trust closes as it closes today. Taken into the compiler, it deletes about
430 lines (292 Heroes, 126 C, 8 binding lines) and adds 288 placed, a seed refresh per version, and
**+150.6 million instructions to every fingerprint of `core` 0.2.0's tree (180.0 million in Heroes
at `-O0` against 29.4 million through the C door, measured), +0.14 % of a check**. Every program with
packages pays it too, in proportion to their bytes, to replace C that is correct and tested against
the Heroes copy.

**Route (D): approve as the first admission program.** The net or the hooks' rewrite (issue 2082)
taking a package costs design.md:570 nothing and the seed nothing, and it costs the trunk's hook
nothing, measured: with `core` in the net's own catalogue the mirror's key did not move and the guard
admitted `order`, green (the mirror keys `selfhost/main.hero` alone, `provenance.py:49`). It
replaces more than the compiler would: the net's own `strings` module holds `starts_with`,
`ends_with` and `contains`, called 1,459 times from up to 116 modules.

**Question 2: approve (a); object to (b), (c) and (d) as the compiler's.** (b) puts 96 modules and
764 calls, every clang invocation among them, behind a placed tree, or deletes nothing if
`cli/process` stays as a wrapper; (d) is (b)'s process half by the same count. (c) gains the
compiler 15 fewer processes for one `heroes add core`, no program it cannot run (§ 13), and costs
`-lcurl` on the seed's line, a library loaded by every verb, and the Windows leg (no curl library).

**Question 3, from the compiler's side: approve (e)**, the environment given at the spawn: 5 calls
in 3 modules, each on the line before its child starts, a staging function of about 5 lines beside
`hero_run_arg`, and 3 `extern "hero_compiler.h"` groups deleted. This is conditional on the runtime
half (built before the fork, an environment block on Windows), which is the ffi-pragmatist's, unbuilt
by me. **Object to moving `hero_env_put` into `hero_os.h`** from the compiler's side: it promises a
process-wide write to every program for 5 calls that each precede a spawn.

**Question 4, the order: (D) first, then for the compiler, if at all, `strings.Split` before any
`path/filepath`.** `path/filepath` has 0 copies in the compiler whose answer is Go's on every input,
`path` has 4 (`Split`'s halves, 46 lines), and `strings.Split` has 4 (45 lines). The compiler's
helpers cut at `\` on Unix too, which neither Go package does. `crypto/sha256` stays out of the
compiler (above).

## The seat's block

- `verdict`: **object** (question 1, route (A); question 1 (ii); question 2's (b), (c), (d)); approve
  (C) at `selfhost/`'s root over (A) if the compiler carries a package at all; approve (D) first;
  approve question 2 (a) and question 3 (e) from the compiler's side; **no veto**.
- `section`: design.md §1.7 (*"does it remove a special case from the compiler?"*), §1.1 (the
  ceiling), §1.11:570-572 (*"No package is compiled into the compiler or the runtime"*, whose letter
  (A) and (C) breach and whose source, panel 234's R1 veto of B5, neither reaches); CLAUDE.md § 2
  (Principle 0). Not covered by design.md: whether the author's instruction 3 (d) on the milestone
  page binds the compiler; that is the author's, and I do not invent a rationale for it.
- `implementation_cost`: route (A), `git diff --shortstat -- selfhost` 144 files +950 −964, 16
  functions deleted (186 lines) and 41 lines of tests, 654 calls moved, 142 `use <core>` lines, 43 of
  them `as core_strings`; placed tree +2,334 lines (13 files) and a 5-line catalogue; seed +11,736
  bytes, +103 lines, 13 `#line`s naming the placed folder; binary +1,648 bytes; every
  `heroes check selfhost/main.hero` +213,691,916 instructions (+0.193 %); the trunk's mirror patch
  (panel 241's, `provenance.py` +60 −5, test +18 −2) owed before any net suite runs; 5 modules past
  §11's 300. Route (C), +811 −964 over 144 files and +368 copied, +0.30 % a check, no hook change, a
  check of about 30 to 40 lines in `suite_packages.hero` (estimated), 1 line of `cards.AREAS`, the
  same 5 modules. Question 1 (ii): about −430 lines, +288 placed, +150.6 million instructions a
  fingerprint of `core` 0.2.0.
- `needed_for_self_hosting`: **no**. The baseline self-hosts at 1edabb957 (stage 2 is the tree's key
  `45824e7205ca021e`, its tests 1,995 passed).
- `argument`: Measured at 1edabb957, the compiler's copies of `core`'s contracts are correct: none
  of the 13 live ones slices. So carrying `core` replaces 186 working lines. By catalogue (A), that
  costs 2,334 placed lines, +0.19 % on every check, a seed refresh per version, a guard that blocks
  the whole net, and five modules over the ceiling. The B5 veto does not reach it, since no program
  reaches the compiler's copy (measured). But no route is needed either. If the author's "no copy by
  hand survives" binds the compiler, the copy at `selfhost/`'s root does the same deletion for 368
  lines, with no hook and no per-version refresh.
- `prediction`: if any route moves the compiler onto `core/strings` before M-core-packages closes,
  the commit that lands it either splits or grants an allowance to `cli/package_reads`,
  `cli/toolchain`, `cli/units`, `emit/ffi_field` and `emit/ffi_narrowed` (each at exactly 300 lines
  of code at 1edabb957, 301 on both routes), or the net's `layout` reads red on them. And
  `heroes check selfhost/main.hero` on the landing commit's tree retires between 0.1 % and 0.5 %
  more instructions than on its parent's (each compiler before its own tree, five alternated pairs),
  under decision 2282's floor of effect, so that close's `compare` names no loss for it. Checkable at
  the landing commit, and at M-core-packages' close at the latest.
- `condition`: a live copy measured crashing or answering wrong on an input a caller reaches,
  which `core`'s function answers rightly (then replacing it is robustness, rank 3, and (C) lands);
  a measured design.md Part 11 effect of the compiler using its own packages; or the author's reading
  that instruction 3 (d) binds the compiler (then (C) over (A)). For (A) over (C): a copied package
  measured drifting from the index past the net's check into a build.

## Route (D), built in scratch

`$S/routeD/`, a copy of the base's `tests/harness/` and `packages/`: `heroes add core --index
file://../../packages -C tests/harness` exit 0, `added core 0.2.0`, a catalogue whose `index` line
is the one `suite_packages.hero`'s `INDEX_LINE` already pins for `examples/`; one call of
`suite_order.hero` moved onto `use <core>/strings as core_strings` (the net has its own module
`strings`, `tests/harness/strings.hero`, so the `as` is owed there as in the compiler);
`heroes check tests/harness/main.hero` **exit 0**, `heroes build` **exit 0**. What the net would
then delete: its own `starts_with`, `ends_with` and `contains` (`tests/harness/strings.hero:22,138,161`),
moving 1,459 calls. The mirror keys `selfhost/main.hero` alone (`provenance.py:49` and
`tree_sources`, read), so a catalogue under `tests/harness/` should move no key the guard asks.
That is run in the worktree below.
- **Run in the worktree** (on route (C)'s tree, compiler `./heroes-c2`): `./heroes-c2 add core
  --index file://../../packages -C tests/harness` exit 0; the same call of `suite_order.hero` moved
  onto `core_strings.has_prefix`, canonical by `heroes fmt` (exit 0); the trunk's mirror keys the
  tree `6eb7ef290fd22e94`, **unmoved**, the compiler the same; `./heroes-c2 run tests/harness/main.hero
  -- ./heroes-c2 order` **admitted by the guard, 3 passed, 0 failed**, the net built over `core` and
  judging with it. **So route (D) costs the trunk's hook 0 lines, measured.**
- On the same state, `packages` **13 passed, 0 failed**, and `records` 27 passed, 1 failed, its one red
  route (C)'s `selfhost/strings/` and nothing of the net's catalogue or placed tree. `packages` is
  green because it holds catalogues under `examples/` alone (its `placed`, `suite_packages.hero:312`),
  so route (D) owes that check a second root, as route (C) owes it the copy. Then the probe's edits
  were taken out (`suite_order.hero` copied back from the base archive, the net's catalogue and
  placed tree removed; `git status --short -- tests` empty).

## What stayed unrun, and why

- Every net suite on route (A): the trunk's guard refuses them (mirror `4eed491c39f6b330`, compiler
  `57ad6537316279a5`), and I did not get around it. Route (A)'s `layout` red is predicted by the
  hooks' mirror of its count, not run.
- On route (C): stage 3, its fixpoint and its own tests; and the `selfhost/emit/**` row's suites,
  `emission`, `emit`, `determinism`, `wholes`, `descriptors` and `lldb`, which it owes, since it
  touches 55 modules under `selfhost/emit/` (`git diff --name-only`); the same holds for route (A).
- `latency.hero -- compare`, the one comparison that declares a breach: not asked (the shared brief),
  and the clock is not still (the ffi-pragmatist builds beside me).
- The `heroes build` clock and its instructions on any route.
- A `crypto/sha256` the compiler takes: the package does not exist in the index, so question 1 (ii)
  is measured on panel 241's stand-in, its fingerprint's cost, and its deletions counted, not built.
- Route (b)'s `os` package and route (c)'s libcurl read, beyond the seed-line probe; route (e)'s
  runtime half.
- Linux and Windows: nothing of mine ran on either.

## State left

- Worktree `.claude/worktrees/242-compiler-engineer`, detached at `1edabb957`: **route (C)**
  uncommitted (144 modules of `selfhost/`, `selfhost/strings.hero` and `selfhost/strings/`), the net
  unchanged; binaries `heroes` (stage 1 from the seed), `heroes-next` (the baseline's stage 2),
  `heroes-a2`, `heroes-a3` (route (A)), `heroes-c2` (route (C)).
- Scratch `.claude/worktrees/p242-scratch/compiler-engineer/`: route (A) whole as `routeA-final.diff`
  and `routeA-untracked.tar`, and rebuilt as `routeA-tree/`; `base/` (a `git archive 1edabb957`),
  `routeC/`, `routeD/`, `fp/` (the fingerprint probes), `noreach/` (the B5 probes), `curlprobe/`; the
  scripts `route_a.py`, `fix_leftovers.py`, `helpers.py`, `bodies.py`, `calls.py`, `permod.py`,
  `pathfns.py`, `instr.sh`, `instr2.sh`; every log named above; `heroes-base`, `heroes-a2`.
- Nothing written outside the repository, nothing to GitHub, no commit, the trunk's hooks untouched.
