# Panel 239, the compiler-engineer's report

Seat: compiler-engineer (design.md §1.1, §1.7, Part 5; veto on a core construct or the ceiling).
Worktree: `.claude/worktrees/239-compiler-engineer/`, detached at `1c4bc5197`; every build and
every run under `build/239/`. Written as it goes; a section marked *unrun* has not been run yet.

## 0. The compilers

- Stage 1: `clang -I runtime seed/heroes.c runtime/runtime.c -o build/239/heroes`, exit 0
  (`build/239/stage1.log`).
- Stage 2 (the unmodified tree, the baseline): `build/239/heroes build selfhost/main.hero -o
  build/239/heroes-base` (`build/239/stage2-base.log`). *Running.*

## 1. What the code does today, read (lines at `1c4bc5197`, `wc -l`)

- `selfhost/emit/externs.hero` (349): `headers_for` (129-161) puts the unit's own groups' headers
  first in written order, then `sort(others)` (158), the other modules' group records the unit
  spells, by declaration index, which is the module walk's order.
- `selfhost/emit/unit_plan.hero` (200): `plan` (62-146) builds `records` and asks `headers_for`.
- `selfhost/cli/probing.hero` (340): `probings_of` groups the plans by header list; `check_all`
  (216-255) runs the pointee check, then the layout check of each list, and pushes each list's
  membership of each record into one list (`pointee_ask.PAD`, 247-250).
- `selfhost/cli/layout.hero` (586): `check_records` (90) runs the screen; `answered` (152-189)
  writes `membership.DECLARED` for every record the screen vouches for (172) and dumps only the
  flagged and the `partial` ones (165-170).
- `selfhost/emit/membership.hero` (233): the membership text is `=` for a vouched record and,
  for a dumped one, the leaves' **paths** (`p <path>`), with byte numbers for bit-fields alone
  (`encoded`, 34-58). **It carries no offset, no size, no alignment.**
- `selfhost/emit/layout_screen.hero` (366): the screen proves membership (a positional
  initialiser under three errors, `sizeof` per field), never an offset.
- `selfhost/cli/rounds.hero` (326): `run` asks, before any unit compiles, `package_stand_in`
  (76) and `bound_macro.told` (86), each across every list; that is where a cross-list check
  stands today.

- Stage 2 baseline built: exit 0, **725,019,903,505 instructions retired**, `real 298.24`, `user
  147.46`, `sys 28.05` at a load of 68 to 97 (`uptime`); `heroes-base --version`'s second line
  `sources 3d44313d62fb67a3`, the brief's key.

## 2. The baseline on this Mac, every 2364 program (`build/239/run2364.sh`, `out-base-mac.txt`)

Each program copied into `build/239/repro/`, built in a fresh `build/` with `heroes-base`, its
binary run under `timeout 10` through `head -c 4096`:

| program | today |
|---|---|
| `two_others/rs_first` | built, `9` |
| `two_others/pw_first` | exit 1, ``ffi_field_type: `Rs.a` is `long long` in `rs.h` ... not `i32` `` |
| `two_others/fixguess/` both | exit 1, ``ffi_field_type: `Rs.a` is `int` ... not `i64` `` |
| `s2364/pack/rp_first` | built, `4 1 1000` |
| `s2364/pack/pk_first` | **built, `4 1 -402653184`** |
| `s2364/own/main` | exit 1, `` `Rs.a` is `long long` `` |
| `s2364/prefix/main` | exit 1, `` `Rs.a` is `int` ... not `i64` `` |
| `s2364/both/pa_first`, `rb_first` | exit 1, `` `Rb.a` ``, `` `Pa.w` is `long long` `` |
| `critic/s2364x/pragma/rp_first` | built, `4 1 1000` |
| `critic/s2364x/pragma/pk_first` | **built, `4 1 -402653184`** |
| `critic/s2364x/partial/rx_first` | built, `4 3 2 302` |
| `critic/s2364x/partial/pw_first` | **built, `4 3 0 300`** |
| `critic/s2364x/const/` both | built, `4 8 8` |

Every row is the brief's.

## 3. The layout, asked of clang in C (`build/239/cdump/`)

The record-layout dump of `emit/layout_text.dump_unit`'s wrapper, `clang -fsyntax-only -Xclang
-fdump-record-layouts`, each record read alone and after the other module's header:

| record | owner's reading | the other unit's reading |
|---|---|---|
| `Rp` (`pack/`) | `c` at 0, `b` at 4, `[sizeof=8, align=4]` | `c` at 0, **`b` at 1**, `[sizeof=5, align=1]` |
| `Rp` (`pragma/`) | the same | the same, **`b` at 1** |
| `Rx` (`partial/`) | `a` 0, `b` 4, `[sizeof=8, align=4]` | `a` 0, **`extra` 4, `b` 8**, `[sizeof=12, align=4]` |
| `Rs` (`two_others/`) | `int a` 0, `b` 4, `[sizeof=8, align=4]` | **`long long a`** 0, `b` 8, `[sizeof=16, align=8]` |

So one comparison of the dump's block sees all four, the three silent members included.

**The critic's "route B's data at no clang cost" does not hold for two of the three silent members**,
read in the code: the program-wide padding list (`cli/probing.hero:247-250`) holds each record's
membership, and the membership of a record the screen vouches for is the constant `=`
(`cli/layout.hero:172`, `emit/membership.hero:24-26`), whatever its offsets; `Rp` has exactly its
declared members in both readings, so both units say `=` and compare equal. For a dumped record
the text is the leaves' **paths** (`membership.encoded`, 34-58), no offset: it would see
`partial/`'s `extra` and nothing of `pack/` or `pragma/`. The data the comparison needs is the
dump's offsets, size and alignment, one clang run per header list that holds such a record.

## 4. Where the route can reach in the tracked tree

- **`selfhost/`**: 28 groups in 22 modules (`git grep -h '^extern' -- selfhost/ | sort | uniq -c`:
  `hero_os.h` 16, `hero_compiler.h` 9, `stdlib.h` 3; `git grep -l '^extern' -- selfhost/ | wc -l`
  22), and **no `record` inside any group** (an awk over those 22 files printed nothing). So no
  unit of the compiler spells another module's group record, and the route asks clang nothing
  when the compiler builds itself.
- **Tracked modules declaring a group record and used by another module** (each file with a
  record inside a group, `docs/` left out, whose stem a tracked `use` line names;
  `build/239/cross-candidates.txt`, `cross-users.txt`): 11 such modules, 24 using modules:
  `examples/ledger/`, `tests/golden/run/fixedbugs-559-*` (3), `fixedbugs-563-a-module-s-own-
  headers-come-before-another-module-s-record`, `fixedbugs-2030`, `-2253`, `-2278` (2), the
  `fixedbugs-2286-*.case`, five `surface-fixtures/`, three `unsupported/` and the `fixedbugs/563`
  case. Each is run under the route below (section 6).

## 5. The route built: B as the judge, A and C as what it tries before refusing

`build/239/sh-agree/` is `selfhost/` at `1c4bc5197` plus one new module and one call:

- **`cli/agreed_layout.hero`** (new, 540 lines formatted, prototype; the landing splits it, see
  section 11): for every unit plan, the group records with fields it spells that another module
  declares (`foreign_of`); each such record's block in clang's record-layout dump of this unit's
  header list, compared with the block in the owner's unit's dump (every member's offset and
  type, the size, the alignment; an anonymous member's place left out). Equal everywhere: nothing
  changes. Different: **the composer** tries the other modules' headers as one block per owning
  module, every order of the blocks (four blocks' 24, past four the written order and its
  reverse), then each block preceded by its owner's own earlier headers (route C, the owner's
  prefix); the first list under which every foreign record agrees becomes the unit's header list
  (`plan.headers`), and the probes are planned again over the changed plans. None agrees:
  `ffi_record_two_layouts`, on the record's declaration, told before any unit compiles. Each
  list's dump is one `clang -fsyntax-only -Xclang -fdump-record-layouts`, kept under
  `build/agreed-<key>/` (the key: compiler, words, dump text, and what the headers resolve to and
  hold, `compiling.reads`), so a warm build asks clang one preprocessing run per such list.
- **`cli/produce.hero`**: the call after `probing.planned`, before `rounds.run`, and the settled
  plans handed to the rounds and to `--emit-c`'s `artifact.write_c` (21 lines added, `diff | grep -c "^>"`).

### 5a. The first build, and a false refusal it found in itself

Stage 2 of the first version: exit 0, 739,887,180,799 instructions retired. Under it,
`pragma/rp_first`, a correct program that builds today, was **refused**: `Rp` read `[sizeof=8,
align=4]` in `rp`'s unit and `[sizeof=8, align=1]` in the root's. The root reads `rp.h` then
`pk.h`, `pk.h` leaves `#pragma pack(1)` set, and the dump's **wrapper** `struct hero_ly_N { Rp v;
}`, written after the includes, was packed by it: the record's members stood where they always
did, the wrapper's alignment moved. The real unit gives that state back after its includes
(`emit/header_state.hero`'s `GIVEN_BACK`, `emit/macro_guard.hero:118-130`); the dump unit did
not. So the comparison's unit now opens as the real one does (`dump_text`: `header_region.
opening`, `header_state.SAVED`, the includes, `GIVEN_BACK`, `CLOSING`, then the wrappers), and is
rebuilt. The same wrapper is written by `emit/layout_text.dump_unit` for the existing layout
check, which reads only the leaves' paths and bytes inside the record, so the packed wrapper moves
nothing it reads (read, not measured beyond `rp_first` building at exit 0 today).

## 6. What every 2364 program does under the route, this Mac (`out-agree-mac.txt`)

Rebuilt with the state given back: stage 2 exit 0, `sources 70f06afb1849e019`, 587,564,857,920
instructions retired (a warmer cache than the baseline's first build, so not a cost; section 8
has the costs). `sh build/239/run2364.sh build/239/heroes-agree build/239/repro
build/239/out-agree-mac`:

| program | today | under the route |
|---|---|---|
| `two_others/rs_first` | `9` | `9` |
| `two_others/pw_first` | exit 1, false `ffi_field_type` | **built, `9`** |
| `two_others/fixguess/` both | exit 1, `` `Rs.a` is `int` ... not `i64` ``, guess `i32` | the same, and the guess now holds: with `i32` both orders build (the rows above) |
| `pack/rp_first` | `4 1 1000` | `4 1 1000` |
| `pack/pk_first` | **`4 1 -402653184`, exit 0** | **built, `4 1 1000`** |
| `own/main` | exit 1, false `` `Rs.a` is `long long` `` | exit 1, `ffi_record_two_layouts`, true (below) |
| `prefix/main` | exit 1, false `` `Rs.a` is `int` `` | **built, `6`** (`rs_make(3 + 1)`: `4 + 2`) |
| `both/pa_first` | exit 1, false `` `Rb.a` `` | exit 1, `ffi_record_two_layouts` on `Rb`, true |
| `both/rb_first` | exit 1, false `` `Pa.w` `` | exit 1, `ffi_record_two_layouts` on `Pa`, true |
| `pragma/rp_first` | `4 1 1000` | `4 1 1000` |
| `pragma/pk_first` | **`4 1 -402653184`, exit 0** | **built, `4 1 1000`** |
| `partial/rx_first` | `4 3 2 302` | `4 3 2 302` |
| `partial/pw_first` | **`4 3 0 300`, exit 0** | **built, `4 3 2 302`** |
| `const/` both | `4 8 8` | `4 8 8` |

So: **the three silent members are closed**, each by the comparison seeing it (`pack` and
`pragma` by `b`'s offset and the alignment, `partial` by `extra` and the size) and the composer
finding the order under which it reads true; **every correct program of the brief builds and
prints its true value**, `prefix` by route C (the owner's `cfg.h` read before `rs.h`); and the
two that no order makes true, `own` and `both`, are refused with a message whose every sentence
is true. `own/main`'s, verbatim:

```
error[ffi_record_two_layouts]: `Rs` is laid out two ways: 8 bytes aligned to 4 in `rs.hero`'s unit, 16 bytes aligned to 8 in `m.hero`'s, which reads `rs.h` after `m.h`
  at rs.hero:2:12
  note: `rs.hero`'s unit reads `rs.h` after no other header: `a` at 0 (`int`), `b` at 4 (`int`)
  note: `m.hero`'s unit reads it after `m.h`: `a` at 0 (`long long`), `b` at 8 (`int`)
  note: a value of `Rs` crosses between the two units, so both must read one layout; no order of the other modules' headers in `m.hero`'s unit, nor each with its own module's earlier headers before it, gives every record it spells its own module's layout
  note: spell `Rs` only in modules whose units read it as `rs.hero` does, or have `rs.hero` bind first the header that configures it, so both units read one layout
```

Two faults of the prototype's words, read in these outputs, for the landing (the words are the
ffi-pragmatist's and the spec-warden's to fix, section 10): **`both` names `Rb` from `pa_first`
and `Pa` from `rb_first`**, so the verdict is free of the written order and the words are not;
the message should name each record that disagrees in some order tried, or R2's walk makes the
first one fixed (section 9). And **the last note's second half is false for `both`**: `rb.hero`
binding `pa.h` first would make its own unit read `Rb` wide, against its `a: i32`.

## 7. The tracked tree under the route (`run-tracked.sh`, `out-tracked-{base,agree}.txt`)

The 21 roots of section 4 (`build/239/tracked/roots.txt`, copied from `git ls-files`), each built
in a fresh `build/` by both compilers, run, and every unit's emitted C (`build/tu-*/*.c`) hashed:
**all 21 read the same verdict, the same output and byte-identical units' C under both**
(`diff` of the first three fields printed nothing). The route asked clang in four of them
(`build/agreed-*` directories: `run/fixedbugs-559-a-record-another-module-reaches-is-read-by-its-
unit` 2, `run/fixedbugs-563-a-module-s-own-headers-come-before-another-module-s-record` 2,
`run/fixedbugs-2286-...case` 1, `fixedbugs/fixedbugs-563-another-module-s-record-is-read-without-
its-module-s-groups` 1), and every record it compared agreed. So no `run`, `unsupported`,
`fixedbugs` or `surface-fixtures` program the route can reach moves, and `tests/emission/` cannot
move by it: the route changes a unit's headers only where a record disagrees, and none of the
tracked programs has one.

**One ruled verdict the route could move and does not**: `fixedbugs/fixedbugs-563-another-
module-s-record-is-read-without-its-module-s-groups` (`ffi_header_refused`, panel 204's R3):
`main` spells `holding.Holder`, whose header compiles only after `holding`'s earlier
`fixedbugs-563-need.h`. The prototype's dump of `main`'s list does not compile, so it says
nothing and today's refusal stands. Route C, applied there too, would build it: the owner's
prefix is exactly what that refusal tells the author to write as a group. That is a change to a
ruled refusal, so it is named here for the synthesis and not done: the most robust reading is
that a foreign record is read as its owner reads it, prefix included, and then 563's case moves
from `fixedbugs/` refused to a run golden.

## 8. What the route costs, in instructions retired (`cost.sh`, this Mac)

Warm builds, the two compilers alternating in one `build/`, each first pair discarded; `heroes-
base` is the tree, `heroes-agree` the route:

| build | without | with the route | delta |
|---|---|---|---|
| `two_others/rs_first` (units spell other modules' records, builds; 4 pairs) | 2,282,122,067 to 2,285,390,903 | 2,325,011,182 to 2,327,101,317 | about **+42M, +1.8%** |
| `both/pa_first` (the failure path; 4 pairs) | 2,605,927,094 to 2,619,002,001 | 706,902,319 to 708,555,225 | about **-1.90G, -73%** |
| `selfhost/main.hero` (2 pairs kept of 3) | 582,626,788,297; 582,085,014,332 | 583,438,934,042; 581,760,125,564 | **+0.14% and -0.06%: none measurable** |
| `two_others/pw_first` through the composer (route only; 3 kept of 4, `cost-pw_first.txt`) | refused today | 2,380,603,445 to 2,386,861,092 | 56M to 62M above `rs_first` under the route |

The failure path is cheaper because the refusal comes before any unit compiles, where today the
units compile, clang refuses the field assertion and the refusal is read back. The warm cost on a
program that builds is the route's own `compiling.reads` (one `clang -M`) per header list it
compares: the probes of `cli/probing.hero` already ran that same preprocessing for those same
lists and words (`probe_with`, `Probe.read`), so the landing reuses `Probe.read` and the warm
delta should fall to the reading of the kept dumps (an inference from the code, unmeasured; the
prediction of section 13 scores it).

**What moves in a cache key**: nothing that exists. The route adds `build/agreed-<key>/`, keyed
by the compiler's fingerprint and key, the words, the dump's text and `compiling.reads`; no
object's, unit's or screen's key names it, and a unit's C moves only where the composer changed
its header list, which then moves that unit's key by its text, as any change of its includes
does. So **`cache` is not owed alone** for the route (`.claude/rules/verification.md`'s
`cli/flags.hero` row is the case where it is: every key moving at once).

The compiler's own build asks the route nothing, as section 4 read (no group record in
`selfhost/`): the two compilers' warm builds differ by less than the pairs differ among
themselves. The cold first runs (733,969,731,683 and 597,978,608,215) are discarded.

### 8a. Linux arm64 (`out-base-linux.txt`, `out-agree-linux.txt`)

Built in `heroes-linux-arm64:latest` from `git ls-files seed runtime selfhost` (`linux/src.tar`,
`COPYFILE_DISABLE=1 tar --no-mac-metadata --no-xattrs`), only `build/239/` mounted, `TMPDIR`
and `HEROES_RUNTIME` under it; GNU ld 2.44, Debian clang 22.1.8 (`linux/toolchain.txt`). The
baseline reads every row of section 2 as this Mac does, and **the route reads every row of
section 6 as this Mac does**, the messages' words included: `diff out-agree-mac.txt out-agree-linux.txt` and `diff
out-base-mac.txt out-base-linux.txt` both print nothing (16 lines each). Costs were not measured
there: the image has no `/usr/bin/time` (`ls`), and no `perf`.

## 9. R2's walk, with and without the route (`order.sh`, `order-all.txt`)

Panel 238's `prototype.diff` applied with `patch -p2` to a copy of the tree (`sh-r2`) and of the
route (`sh-agree-r2`), each built by `heroes-base` (stage 2 exit 0 both; `heroes-r2`
738,212,410,013 instructions). Each shape's two `use` orders written as one root named
`main.hero` (`build/239/order/<shape>/{written,swapped}/`), plus **`renamed`**, `two_others` with
module `pw` named `zz`; each unit's emitted C hashed with its `hero_program_sources` line taken
out (`.claude/rules/verification.md`'s exception: the root's bytes differ by the two lines, and
so does that key):

| shape | today | R2 alone | the route alone | the route with R2 |
|---|---|---|---|---|
| `two_others` | `9` / refused | **refused / refused** | `9` / `9` | `9` / `9`, **one C** |
| `renamed` (`pw` named `zz`) | `9` / refused | **`9` / `9`** | `9` / `9` | `9` / `9`, one C |
| `pack` | `1000` / **`-402653184`** | **`-402653184` / `-402653184`** | `1000` / `1000` | `1000` / `1000`, one C |
| `pragma` | `1000` / **`-402653184`** | **`-402653184` / `-402653184`** | `1000` / `1000` | `1000` / `1000`, one C |
| `partial` | `302` / **`300`** | **`300` / `300`** | `302` / `302` | `302` / `302`, one C |
| `both` | refused `Pa` / refused `Rb` | refused `Rb` / `Rb` | refused `Pa` / **`Rb`** | refused `Rb` / `Rb` |
| `const` | `4 8 8` / `4 8 8` | the same, one C | the same | the same, one C |

Read row by row:

- **R2 alone does what it promises and makes this class worse.** Its verdict and its C are free
  of the written order (one C in every row), and it hands the choice to the modules' names:
  `two_others` is refused in both orders and the same program with `pw` named `zz` builds in
  both, and **`pack`, `pragma` and `partial` print the wrong value at exit 0 in both orders**
  where today one order of two is right, because `pk` sorts before `rp` and `pw` before `rx`.
  So **R2 must not land without the 2364 repair**, the order panel 238's R3 already gave.
- **The route alone** makes the verdict free of the written order and of the names (every row
  the same both ways), and leaves the bytes to the walk (`two_others` builds both ways with two
  C texts, the composer having found the agreeing list from the other end) and the words of
  `both` to the walk (`Pa` one way, `Rb` the other).
- **The route with R2**: verdicts, words and every unit's C free of the written order, and the
  verdict free of the names. Its C for the swapped order is the route alone's swapped C, and the
  composer's choice is then made over a walk that no `use` line moves.

So the route **stands alone** for what the program means, and **lands with or after R2** for
the bytes and the words; R2 must not land before it. Order of landing: the route first or in the
same round, R2 after or with it.

## 10. The routes of question 4, each measured or argued from a measurement

| route | built | the three silent members | `pw_first`, `prefix` (correct) | `own`, `both` | free of the written order |
|---|---|---|---|---|---|
| **A alone**, order search triggered by a refusal, its criterion clang compiling the unit | argued | **silent**: `pk_first`, `pragma/pk_first`, `partial/pw_first` compile in their default order today (section 2), so no refusal starts a search | `pw_first` built by the search; `prefix` refused: no order of `rs.h` alone reads `cfg.h` | refused, with the field-type words | verdict yes for these shapes |
| **B alone**, the layout comparison, refusal where it differs (`heroes-verify`, `out-verify-mac.txt`; stage 2 749,010,411,773) | yes | **refused, true** | **refused, five correct programs**: `two_others/pw_first`, `pack/pk_first`, `pragma/pk_first`, `partial/pw_first`, `prefix/main` | refused, true | **no**: `rs_first` builds, `pw_first` refused |
| **B with A and C** (the route, `heroes-agree`) | yes | **built, true values** | **built, true values** | refused, true | **yes** (section 9) |
| **C alone**, the owner's prefix always | argued | silent: prefixing adds the owner's earlier headers and moves nothing a later foreign header does (`pk.h`, `pw.h` are other modules') | `prefix` builds; `pw_first` refused | refused | no |
| **D**, isolation, no foreign header beside the record | argued | would close them | builds | `both`: the root spells `Pa` and `Rb` in one function, so one unit must read both headers; D is impossible there without a unit per record, which panel 204's R4 refused (per-group units) | yes |

So B is the only route that **sees** the silent members (A and C never ask about them, since
nothing fails), and B alone refuses correct programs by the written order; **the composer (A
and C) is what turns B's refusals of correct programs into builds**, and its verdict is then free
of the order because "some list agrees" does not depend on which list was tried first.

**Panel 238's direction for 2364 as written, "the compiler decides the order ... (panel 204's
order search), with a true refusal where no order works", is wrong in its criterion, not its
shape**: panel 204's search (`cli/header_order.hero:219`, `cli/order_trial.hero`) asks whether
the unit **compiles** (`order_trial.compiled`), and the three silent members compile in every
order. The search has to be asked "does every foreign record read as its owner reads it" (B's
question, a dump per list), it has to try the owner's prefix (C) for `prefix`, and it has to run
**before** any unit compiles, on every build that spells another module's record, since nothing
else fails to trigger it.

## 11. The shapes beside the route, depth one (`build/239/shapes/`)

| shape | today | under the route |
|---|---|---|
| **a record reached through another record's field** (`nested/`: `Outer { int c; Rs r; }`, the root spells only `Outer`) | exit 1, false `` `Rs.a` is `long long` `` | **built, `4 7 3 2`**: `Rs` is in the root's reach through `Outer`'s field (`members.declared_reach`), and `Outer`'s dump carries `Rs`'s members inline |
| **a `partial` union** (`union/`: `Un { int i; long long wide under the macro }`) | built, `4 3 3`, the layouts 4 against 8 bytes, right by the calling convention alone | built, `4 3 3`, the root's list recomposed to `un.h`, `pw.h` (`grep '#include' build/tu-*/pwfirst.c`) |
| **that union inside a record, a field after it** (`union2/`, **new, same cause**) | **built, `4 3 0` at exit 0** (`b.t` read at 8 where it is at 4) | **built, `4 3 1000`** |
| **`heroes test`'s units** (`test/`: `pw_first` with a `test` block) | exit 1, false `ffi_field_type` | **exit 0, `1 test, all passed`**: the route runs in `produce`, which `test` builds through |
| **`--emit-c`'s one file**, header-only (`pack/pk_first`, `union2`, `prefix`) | `pk_first` and `union2` written at exit 0 | written at exit 0; compiled with the runtime, they print `4 1 1000`, `4 3 1000`, `6`: inside one file every function reads one layout |
| **`--emit-c` where the record's function is a library's** (`packlib/`: `rp_make` in `librp.a`, `link "rp"`) | `build`: `4 1 -402653184`; one file: the same | **`build`: `4 1 1000`; the one file, written at exit 0 and compiled with `-lrp`: `4 1 -402653184`** |
| `--emit-c` of `two_others/pw_first`, `partial/pw_first` | refused, false `ffi_field_type`; `ffi_one_file` | **refused `ffi_one_file`** (the one file does not compile; `rx_make` reads otherwise): true, and a program `build` builds |
| a record spelled only through `ptr` | read, unrun: `ptr` carries no type (`spec/heroes-spec.md` § 13, `holder_need(n: ptr)`), so a unit that holds only a `ptr` names no record and reads no foreign header for it | |
| a record of a `package` group | **unrun**: the comparison asks each list under its own words (`words_of`, the same as `probing.probings_of`'s), so a package's compile words reach both readings; a package whose words differ between the owner's unit and another's would be a disagreement the composer cannot fix (words, not order) and is refused, which is true of it | |
| the padding table (`cli/probing.hero:247-250`, `emit/lend_type.hero:61-70`, last unit wins) | read: one record's membership from two units, the last standing for all | read, and run on `partial/`: under the route every unit that spells a record reads it as its owner does (or the build is refused), so every unit's membership of it is the same text and the fold cannot pick a wrong one; `partial/pw_first` with `rx.sum`'s `@` lend prints `302` |

**`packlib` is a defect beside the route, of the same cause seen from `--emit-c`**: the one
file reads every module's headers in declaration order (`emit/externs.headers`, `cli/one_file.
hero:52`), so `pk.h` packs `Rp` for the whole file, and `cli/one_file.hero`'s two questions (the
file compiles; each bound **name** means what its module's unit makes it, lines 12-24) never ask
a **record's** layout. Today it is hidden behind `build`'s own silent wrong value; with the route
`build` is right and `--emit-c` alone is wrong, at exit 0. Reproducer:
`.claude/worktrees/239-compiler-engineer/build/239/shapes/packlib/` (`sh`: `heroes build
pk_first.hero --library lib --emit-c > one.c`, then `clang -I runtime -I . one.c
runtime/runtime.c -L lib -lrp`). **The route owes the one file the same comparison** (the fused
list is one more list: each record against its owner's), and the composer over the fused list's
module blocks, `ffi_one_file` where none agrees. So it is in this repair, not filed apart.

## 12. 2363 in the compiler: where a refusal read from the link sits, and its price

Read and run on this Mac; the refusal itself is the ffi-pragmatist's to build.

- **Where.** `cli/link.hero` `link_objects` (48): the linker writes into a private
  `<binary>.<pid>-<n>.tmp/` and **everything in it is published beside the binary** (36-43,
  117-131), so the check sits between `run_link`'s success (108-113) and the publishing loop:
  refused there, the stage is removed and no binary lands. Its map must be written under
  `build/`, never into the stage, or it is published beside the binary. `linked_into` (172), the
  link written "straight into `at`", is the second place, and the same rule holds.
- **What it reads.** ld64: `-Wl,-map,<file>` (a file: nothing reaches the author's screen); lld
  and lld-link: `-Map`/`/MAP:` and `--why-extract=<file>` (files); GNU ld: `-Map=<file>` names
  the archive member each section came from, and only `--trace-symbol=<name>` names a **shared**
  library's definition, on stderr, which `run_link` (`cli/link_warning.hero:37-56`) prints but
  for glibc's section warnings; so its lines are taken out the way `without_section_warnings`
  (60-78) takes those out, by their shape, `<file>: definition of <name>` and `<file>: reference
  to <name>`, for the names the line traced and no other.
- **How it reaches the binding's line.** `emit/externs.named` (213-257) gives library names and
  drops which declaration named each; the refusal needs, per bound C name, its group's library
  and its declaration's span: a walk of `p.functions` with `extern_spans(decls, f)` (42-53), which
  holds both, plus the record-only groups' arm (238-255) for a `link` named only by a record
  group. About 40 lines beside `named`, read, not built.
- **Its cost, measured on ld64**: `-Wl,-map` beside a Heroes program's link (`dup_written`, six
  pairs, `out-mac/map-cost.txt`): 77.6M to 80.1M instructions either way; beside the compiler's
  own link (761 objects, four pairs, `map-cost-self.txt`): 101.0M to 101.8M either way after the
  first, cold, plain run, **no measurable cost**, a map of 3,013,120 bytes and 54,574 lines. Its
  reading is not built, so unmeasured; it is owed only where the line carries a `-l` or a
  package's link words (no other library can define a bound name; two units defining one is
  `ffi_defined_twice`'s), so **the compiler's own build, which links no library, pays nothing**.
  GNU ld's and lld's costs are **unrun**: the image has no `/usr/bin/time`.
- **Route (D), a second link with every archive whole**: ld64 `-Wl,-all_load` on `dup_written`
  (four runs): 78.4M to 82.2M instructions, exit 1, `duplicate symbol '_pick' in: .../liba.a[2]
  (pick_a.o) .../libb.a[2](pick_b.o)`, both members named, in either order. Its price is one
  more link per build that names a library: about +3.4% on a program of `rs_first`'s size
  (78M on 2.28G), and it sees no shared library and no C library name.
- **2365's path meets it** where GNU ld refuses the duplicate itself (`one_written`, `multiple
  definition of 'pick'`): `link_code` (194) makes that `clang_link`, `cli/whose.hero` reads it
  for a missing `link` (`cli/renamed.hero`) and, finding none, says `internal error` at exit 2.
  ld64's `duplicate symbol '_X'` and GNU ld's `multiple definition of 'X'` name the C name, so
  the refusal (F) reads them there and tells it on the binding, the same code as the successful
  link's refusal; it is a reading of the failure path, about the size of `renamed.hero`'s reader.
- **No symbol reader is started today** (`grep -rhoE 'program: "[^"]+"' selfhost`: clang 59,
  pkg-config 5, git 3, then fixtures; no `nm`, `otool`, `objdump`, `readelf`; and no reader of
  `!<arch>`, ELF or Mach-O: the one `grep` hit, `cli/link_flags.hero:71`, is the word `.dynsym` in a
  comment on `dladdr`). Route (B), every library on the line that defines a bound name whatever the linker
  took, needs one: either `nm` started (a tool the Windows leg must have, `llvm-nm` beside clang)
  or the archive symbol index (`!<arch>`'s first member) read in Heroes, plus ELF `.dynsym`,
  Mach-O exports and `.tbd` text for shared libraries. That is the largest of the 2363 routes in
  code, and the only one whose verdict needs no link order at all.

**My seat's reading of question 1**: (A) alone is out by the criterion, as the brief says, and
I add that **R2 makes it worse, not better**, by the measurement of section 9 seen from 2363's
side: with the line's order a function of the modules' names, `dup_written` and `dup_sorted`
become one program whose verdict a module's rename flips. The ceiling allows (A) joined to (D)
(one map reader, one second link; both are files, nothing reaches the screen) or (A) joined to
(B) (one map reader, one symbol reader). Neither adds a core construct. **A group word to let a
library replace a C library name on purpose (an allocator) would be one**: a contextual word in
`selfhost/foreign_words.hero`, the parser's group head, the checker, the formatter and every
re-printer (`.claude/rules/diagnostics-and-goldens.md`'s walk), for a program the compiler does
not need (design.md §1.7, Principle 0); refuse the replacement now, and let a sitting rule the
word when a program needs it.

## 13. The other questions, from this seat

- **Question 5, the words where no order works.** `ffi_field_type`'s *"in `rb.h`"* cannot be
  made true by rewording it: the field is `int` in `rb.h` read alone and `long long` read after
  `pa.h`, and the field-type assertion sees only the second. The disagreement is between two
  units, so a code of its own is owed; the prototype's is `ffi_record_two_layouts`, told on the
  record's declaration in its own module, both layouts in its notes (sizes, alignments, every
  member's offset and type) and the headers read before the record's in each unit. Two of its
  sentences need the ffi-pragmatist's and the spec-warden's pen (section 6): name every record
  that disagrees in some list tried (`both` names one of two today, by the walk), and drop the
  half of the last note that is false for `both`. **`own` is refused, and should be**: the only
  list that agrees puts `rs.h` before `m.h`, `m`'s own header, which is what `emit/externs.hero:
  124-128` forbids so that no other module's header changes what one of the module's own means;
  the composer never puts a block before the unit's own headers.
- **Question 6, the order of landing.** Measured in section 9: the route stands alone for every
  verdict and is free of the written order and of the names; **R2 must not land before it**
  (R2 alone makes `pack`, `pragma` and `partial` wrong in both orders); landed together or the
  route first, the bytes and the words are free of the written order too. The route touches no
  link line, so it meets R3's group words nowhere.
- **Question 7, the cases.** Section 14.
- **Question 8, the text.** No new surface: no lexer, parser or checker change, no word. The
  rule is design.md §4.19's (a group record another module declares is read as its own module's
  unit reads it, or the program is refused), `.claude/rules/c-boundary.md`'s member list gains
  `ffi_record_two_layouts` (it names `ffi_defined_twice` once today, `git grep -c`), and whether
  `spec/heroes-spec.md` § 13 says it is the spec-warden's count.

## 14. The cases each repair owes, and their form (`.claude/rules/verification.md` § What gates what)

No golden builds a C library (`00-shared.md`), and **2364's shapes need none**: every header is
`static inline`, so they are goldens, run on the CI's four legs.

- **`run`**, a folder of modules beside the roots as `run/fixedbugs559/` and `run/fixedbugs563/`
  are today: `two_others` in both orders (`9`), `pack` and `pragma` `pk_first` and `rp_first`
  (`4 1 1000`), `partial` both (`4 3 2 302`), `prefix` (`6`), `nested` (`4 7 3 2`), `union2`
  both (`4 3 1000`): each **silent** today or refused falsely, each with its value, both orders
  pinned so a later change cannot make the verdict follow the `use` lines again. They move `run`,
  `emission` (their `--emit-c` blessed, which pins the fused list's order the one-file composer
  chooses), `canonical`, `determinism`, `lines`, `warnings`.
- **`unsupported`**: `own` and `both` in both orders, `#~ ffi_record_two_layouts` annotated on
  the record's declaration in the owner's module file (a non-root module carries its mark, as
  `fixedbugs/fixedbugs-563-.../holding.hero` does today), `full` for one of them to pin the
  notes.
- **The one file with a library** (`packlib`): not a golden. A test of the compiler beside the
  one-file comparison, as `cli/layout.hero`'s tests write headers and run clang (303-428): a
  header, a `.c` built into a shared library by clang (`-shared`, no `ar`), `--emit-c` asked and
  refused, or composed and compiled and run. Mac and Linux by the compiler's own tests; Windows
  by the box, its DLL and import library being another shape (`tests/harness/shell.hero:2332`).
- **2363**'s cases need libraries, so they are the net's own tests or the compiler's (as above),
  never a golden, until a golden form can build one.
- **Platforms**: the comparison reads clang's dump on each leg against the same leg's dump, so
  its format never crosses a platform; the MSVC target's dump prints differently, and **the
  Windows leg is unrun** for the route: the first CI run after the landing push scores it.

## 15. The landing this route owes, file by file

At `0a62bb10c`'s lines (`wc -l`; code lines, comments and blanks left out, in parentheses):

1. **`selfhost/cli/agreed_layout.hero`** (new): `settled`, the foreign records of each plan, the
   owner's block against this list's, the dump per list kept under `build/agreed-<key>/`.
   **Reuses each probing's `Probe.read`** for the lists the probes already preprocessed (the
   prototype runs `compiling.reads` again, the +42M of section 8). About 230 code lines.
2. **`selfhost/cli/agreed_order.hero`** (new): the composer, blocks by owning module, every order
   up to four blocks, then each with its owner's prefix; never a block before the unit's own
   headers. About 110.
3. **`selfhost/cli/agreed_told.hero`** (new): `ffi_record_two_layouts`'s words, every record that
   disagrees in a list tried, the two layouts, no false fix. About 110. (The prototype is the
   three in one, `build/239/sh-agree/cli/agreed_layout.hero`, 540 lines, 371 of code.)
4. **`selfhost/emit/layout_text.hero`** (377): the dump unit with the header state given back
   after the includes (`header_state.SAVED`, `GIVEN_BACK`), the prototype's `dump_text`; whether
   the existing layout check reads its dump the same way is one question with it (section 5a).
5. **`selfhost/cli/probing.hero`** (340, 202 of code): the words of a plan's list factored out of
   `probings_of` (107-115) into one function the route calls, so the two cannot drift.
6. **`selfhost/cli/produce.hero`** (445, 255 of code): the call between `probing.planned` and
   `rounds.run`, the settled plans to the rounds and to `artifact.write_c` (+21, 270 of code).
7. **`selfhost/cli/one_file.hero`** (261, 171 of code): the fused list compared and composed as
   one more list, `ffi_one_file` where no order agrees (`packlib`, section 11).
8. **The goldens and tests of section 14**; **`.claude/rules/c-boundary.md`**'s member list;
   **design.md §4.19**'s sentence; the spec as the spec-warden counts it.
9. **Not touched**: the lexer, the parser, the checker, the descriptors, ownership, the emitter's
   C (a unit's includes change only through `plan.headers`, which `emit/unit.hero` already
   writes), `cli/flags.hero`, any cache key that exists.

Seed: the route enters `selfhost/` only; no new surface form, so no seed refresh is owed before
it (`.claude/rules/verification.md` § The batch).

## 16. Verdict

- **verdict**: **object**, to panel 238's direction for 2364 as worded ("panel 204's order
  search, with a true refusal where no order works"), and **approve** the amended route this
  seat built: **the layout comparison (B) is the judge; the order search (A) and the owner's
  prefix (C) are what it tries before refusing; the fused one file is one more list; R2 never
  before it.** No veto: it adds no core construct (Part 5's seven are untouched; nothing in the
  lexer, checker, descriptors, ownership or emitter), and it fits the ceiling.
- **section**: design.md §1.12 (the three silent members and `union2`, a wrong value at exit 0,
  are what the route exists for, and the reason B cannot be left out), §4.19 (FFI), with §1.1
  and §1.7 for its size.
- **implementation cost**: about 450 code lines in three new `cli/` modules (prototype 540 lines,
  371 of code, `build/239/sh-agree/cli/agreed_layout.hero`), +21 in `cli/produce.hero`, a
  factoring in `cli/probing.hero`, a dump variant in `emit/layout_text.hero`, an arm in
  `cli/one_file.hero`. Measured: warm `rs_first` +42M instructions (+1.8%, before the `Probe.read`
  reuse), warm through the composer 56M to 62M more, the failure path **-1.90G (-73%)**, the
  compiler's own warm build within 0.14% either way; 21 tracked programs unmoved in verdict,
  output and units' C.
- **needed for self-hosting**: no (Principle 0): `selfhost/` declares no group record; it serves
  §1.12, a measured wrong value at exit 0 in four shapes.
- **prediction**: **the landing moves no blessed emission**: at the landing round's gate,
  `emission` and `emit` report zero moved files attributable to the route, because none of the
  21 tracked programs that can reach another module's group record disagrees (section 7). A moved
  emission in `tests/emission/` or `tests/golden/emit/` that the route caused falsifies it; the
  round's closing commit body scores it.
- **condition**: this seat turns to veto if the landing drops B (an order search whose criterion
  is "the unit compiles" leaves `pack`, `pragma`, `partial` and `union2` silent, measured), or lets
  R2 land first (it makes three of them wrong in both orders, measured). It turns against its own
  route on any of: a correct program the comparison refuses that some list makes true and the
  composer does not try; a disagreement in ABI the dump does not print (two layouts whose blocks
  read alike and whose values cross wrongly); or the Windows leg's dump making two units of one
  record read differently with nothing changed but the target.

## 17. After the critic's second pass

Asked by the coordinator after `239-reports/critic-reports.md` (lane `p239`), findings R1, R3, C2,
C3, C6, W1, I1 to I3, its § 7 and its closing list; its programs read from
`.claude/worktrees/239-critic/build/239/p2/` and copied before any run. Built in this worktree on
the route's sources: **`build/239/sh-agree2/`**, the first version (`sh-agree/`) plus the
changes below; stage 2 by `heroes-base`, exit 0, 585,012,931,679 instructions, `sources
2ed90c391b3feb46` (`stage2-agree2.log`); the same copy built in `heroes-linux-arm64:latest` by
`linux/build.sh agree2`, exit 0.

### 17a. What changed from the first version, file by file

- **`cli/agreed_layout.hero`** (911 lines formatted): the comparison asks clang only where two
  readings can differ (a record read after exactly the headers its own module reads before it,
  under the same words, is the same by construction and not dumped); the composer's blocks are
  sorted by their modules' names, so what is tried and the list chosen owe nothing to the walk;
  the refusal names **every record that disagrees in some list tried**, sorted by name, at the
  first one's declaration; the words are the specification's (17c); and **`fused`**, the one-file
  arm: `--emit-c`'s list compared as one more list, the modules' own blocks tried in every order
  where the written one disagrees, `ffi_one_file` where none agrees.
- **`emit/layout_text.hero`** (377 to 396): `dump_unit` gives the header state back after the
  includes (`header_state.SAVED`, `GIVEN_BACK`, inside `header_region`), the existing layout
  check's dump and the route's alike (17d).
- **`emit.hero`** (434 to 453): `emit_listed`, the one file emitted under a given header list;
  `emit_told` calls it with today's.
- **`cli/artifact.hero`** (155 to 158) and **`cli/one_file.hero`** (261 to 259): the one file is
  written, and asked of clang, under the settled list.
- **`cli/produce.hero`** (445 to 481): the one-file arm's call and its refusal before
  `artifact.write_c`.

### 17b. Every program through `build` and `--emit-c` (`out-agree2-mac.txt`, `shapes-agree2-mac.txt`, `emitc-mac.txt`, `emitc-agree2-mac.txt`)

A file `--emit-c` writes is compiled with the compiler's own words (`build/239/cwords.txt`, read
from a build's own response file: `-std=gnu11 ... -fsigned-char ... -O0 -g`, the critic's R3),
the runtime and, for `packlib`, `-lrp`, then run. This Mac:

| program | today `build` | today `--emit-c` | route `build` | route `--emit-c` |
|---|---|---|---|---|
| `two_others/rs_first` | `9` | `9` | `9` | `9` |
| `two_others/pw_first` | refused, false `ffi_field_type` | refused, the same | `9` | **`9`** |
| `pack/rp_first`, `pragma/rp_first` | `4 1 1000` | `4 1 1000` | `4 1 1000` | `4 1 1000` |
| `pack/pk_first`, `pragma/pk_first` | **`4 1 -402653184`** | `4 1 1000` | `4 1 1000` | `4 1 1000` |
| `partial/rx_first` | `4 3 2 302` | `4 3 2 302` | the same | the same |
| `partial/pw_first` | **`4 3 0 300`** | refused `ffi_one_file` | `4 3 2 302` | **`4 3 2 302`** |
| `prefix/main` | refused, false | refused, false | `6` | **`6`** |
| `nested/pw_first` | refused, false | refused, false | `4 7 3 2` | **`4 7 3 2`** |
| `union2/pw_first` | **`4 3 0`** | `4 3 1000` | `4 3 1000` | `4 3 1000` |
| `union2/un_first` | `4 3 1000` | `4 3 1000` | the same | the same |
| **`packlib/pk_first`** (`librp.a`, `link "rp"`) | **`4 1 -402653184`** | **`4 1 -402653184`, exit 0** | `4 1 1000` | **`4 1 1000`** |
| `packlib/rp_first` | `4 1 1000` | `4 1 1000` | the same | the same |
| `own/main` | refused, false | refused, false | refused, true (17c) | refused, the same words |
| `both/pa_first`, `rb_first` | refused, false, two records | refused, false | refused, true, one text | refused, the same |
| `fixguess/` both | refused, guess in a circle | | refused `ffi_field_type`, true of `rs`'s own reading, its guess `i32` builds | |
| `const/` both | `4 8 8` | | the same | |

**R3 is closed on this Mac**: `packlib`'s one file is written in the order `rp.h`, `pk.h` and
prints the true value; every file the route writes means what `build` builds, and every
program `build` refuses is refused by `--emit-c` in the same words. Linux arm64: 17e.

### 17c. The words, verbatim (W1, R1, this report's section 6 faults; the spec-warden's § 9 and § 7(d))

No *unit*, *linker*, *archive*, *offset*, *align* or *laid* (each `grep -ciw` 0 over
`spec/heroes-spec.md`; *size*, *bytes*, *byte*, *reads*, *module* present, *layout* once); a module
*reads* a header; the note cites **spec § 13**; the size, the alignment (in plain words, *at a
multiple of 8*, the specification having none) and **the first two fields that differ**, never
every field. `own/main` (915 bytes):

```
error[ffi_record_two_layouts]: `Rs` is 16 bytes, at a multiple of 8, where module `m` reads `rs.h` after `m.h`, and 8 bytes, at a multiple of 4, where module `rs` reads it
  at rs.hero:2:12
  note: `Rs.a` is `long long` at byte 0 where module `m` reads it, and `int` at byte 0 where module `rs` does
  note: `Rs.b` is at byte 8 where module `m` reads it, and at byte 4 where module `rs` does
  note: a group's `record` is its header's struct as its own module reads it (spec § 13), and a value of `Rs` passes between module `m` and module `rs`, so both must read it alike; no order of the headers module `m` reads, nor module `rs`'s earlier headers before `rs.h`, makes them alike
  note: module `m`'s own `m.h` changes `Rs`: spell `Rs` only in modules that read it as module `rs` does, or have module `rs` read first the header that changes it, so both read it alike
```

`both`, the root named `main.hero` in both orders (`order/both/{written,swapped}/`): **`cmp`
prints nothing, 1,032 bytes each**, both records named, each with its own cause:

```
error[ffi_record_two_layouts]: `Pa` is 8 bytes, at a multiple of 8, where module `main` reads `pa.h` after `rb.h`, and 4 bytes, at a multiple of 4, where module `pa` reads it
  at pa.hero:2:12
  note: `Pa.w` is `long long` at byte 0 where module `main` reads it, and `int` at byte 0 where module `pa` does
  note: `Rb` is 8 bytes, at a multiple of 8, where module `main` reads `rb.h` after `pa.h`, and 4 bytes, at a multiple of 4, where module `rb` reads it
  note: `Rb.a` is `long long` at byte 0 where module `main` reads it, and `int` at byte 0 where module `rb` does
  note: a group's `record` is its header's struct as its own module reads it (spec § 13), and a value of `Pa` passes between module `main` and module `pa`, so both must read it alike; no order of the headers module `main` reads, nor module `pa`'s earlier headers before `pa.h`, makes them alike
  note: spell `Pa` only in modules that do not also read `rb.h`, and `Rb` only in modules that do not also read `pa.h`
```

**The libjpeg pair** (`p2/jpeg/` copied to `repro/jpeg/`, jpeg-turbo 3.2.1 by `pkg-config
--modversion libjpeg`, this Mac only: the Linux image has none): `cb_first` exit 1, **940
bytes** (today 14,841 bytes and 16 `ffi_declared_two_ways` errors; the first version 8,113):

```
error[ffi_record_two_layouts]: `Dinfo` is 624 bytes, at a multiple of 8, where module `cb_first` reads `jpeglib.h` after `stdio.h`, `cfgbool.h`, and 656 bytes, at a multiple of 8, where module `jp` reads it after `stdio.h`
  at jp.hero:5:12
  note: `Dinfo.raw_data_out` is at byte 89 where module `cb_first` reads it, and at byte 92 where module `jp` does
  note: `Dinfo.dct_method` is at byte 92 where module `cb_first` reads it, and at byte 96 where module `jp` does
  note: a group's `record` is its header's struct as its own module reads it (spec § 13), ...
  note: spell `Dinfo` only in modules that do not also read `cfgbool.h`
```

`jp_first` exit 1, `ffi_header_refused` (*typedef redefinition with different types*), as
today and as the first version. **A false sentence this run found in my own words and repaired
before writing them here**: the first build of this version said *"module `cb_first`'s own
`stdio.h` changes `Dinfo`"*; `jp` reads `stdio.h` before `jpeglib.h` too. What changes a record
is what its reader reads before its header **and its own module does not** (`cause_of`), here
`cfgbool.h`; rebuilt, and every run above is of the rebuilt compiler.

### 17d. The existing layout check's dump unit (the critic's § 7, this report's § 5a)

Run, not read: `shapes/pragma_partial/` (`pragma/` with `record Rp partial`, so the existing
check dumps it), `rp_first`. Today's compiler keeps two dumps (`build/layout-*/clang-dump-*.txt`):
`Rp`'s wrapper `[sizeof=8, align=4]` in `rp`'s list and **`[sizeof=8, align=1]` in the root's**,
whose `pk.h` leaves `#pragma pack(1)` set: the dump unit gives no state back. So § 5a's *"moves
nothing it reads"* is now measured on its one reader of a size row: `layout_text.hero:212` reads
it as a block's end and `union_sizes` (304-317) reads an anonymous union's own block, laid out
where the header declares it, so the wrapper's alignment reaches no verdict; the program prints
`4 1 1000` under both. **Repaired in the route** (17a): under `heroes-agree2` both kept dumps read
`[sizeof=8, align=4]`, `rp_first`, `pk_first` and `pragma/rp_first` print `4 1 1000`. The dump's
text changes, so the layout check's kept answers go cold once.

### 17e. Linux arm64

`linux/after2.sh` in the container (the programs copied to `linux/repro-l/`, `librp.a` rebuilt
there from `rp.c` by the image's clang and `ar`): **every result reads as on this Mac**, by
command, not by eye: `diff` of the 16 build rows (`out-agree2-mac.txt`, `out-agree2-linux.txt`),
of both compilers' `--emit-c` rows (`emitc-mac.txt`, `emitc-agree2-mac.txt`, `emitc-linux.txt`)
and of the shapes (`shapes-agree2-mac.txt`, `shapes-linux.txt`) print nothing, and `cmp` of `own`'s
and `both`'s words against this Mac's prints nothing; `both`'s two orders, `cmp` exit 0 there too
(`both-linux-cmp.txt`). Linux's own rows of note: today's compiler builds `union2/pw_first` at exit
0 printing `4 3 0` and `packlib/pk_first` printing `4 1 -402653184`, and its one file of `packlib`
compiled with `-fsigned-char` prints `4 1 -402653184`; the route prints `4 3 1000`, `4 1 1000`
and `4 1 1000`. The libjpeg pair is unrun there (no libjpeg in the image).

### 17f. The tracked tree through `--emit-c` (`tracked-emitc.txt`, `emission-cross.txt`)

The 21 roots of section 7 through `--emit-c` under `heroes-base` and `heroes-agree2`: **20 read
the same exit and the same bytes** (`shasum` of the written file), and **one moves**:
`run/fixedbugs-563-a-module-s-own-headers-come-before-another-module-s-record`, refused today
(`ffi_one_file`: the one file reads `fixedbugs-563-wide.h` before `fixedbugs-563-skew.h` and lays
`Sk` wide), **written by the route** with `skew.h` before `wide.h`; compiled with the compiler's
words and run, it prints `7`, its `.expected`. A census of the programs `emission` covers
(`tests/golden/{run,emit,fixedbugs,ir}` and `examples/`, every tracked `.hero` no other uses, 898,
each `use` closure walked; a group record is matched by a regular expression, so the census is
an approximation of the set the arm reaches): **6** reach two or more modules with groups and a
group record with fields; five are among the 21, and the sixth,
`run/fixedbugs-559-a-record-beside-another-module-s-header-builds`, reads `6 9` under both and is
refused `ffi_one_file` by both (another cause). So **the landing blesses one new emission and
takes one program from `emission`'s refused count** (`LEAST_REFUSED`, 42 in
`tests/harness/suite_emission.hero:192`): where the count stands at its floor, the closing commit
moves the floor, which is a decision the batch records. **This falsifies the letter of my
prediction of section 16** (*"moves no blessed emission"*): no blessed file changes its bytes, and
one emission is added. The prediction is restated in 17j.

### 17g. What it costs (`cost2-small.txt`, `cost2-selfhost.txt`), instructions retired, warm, first pair discarded

| build | today | the route, second version | delta |
|---|---|---|---|
| `two_others/rs_first` (3 pairs) | 2,276,478,071 to 2,280,129,125 | 2,311,981,827 to 2,313,226,710 | **+33M, +1.5%** (first version +42M) |
| `two_others/pw_first` (3 pairs) | refused, 2,643,252,964 to 2,645,903,829 | builds, 2,378,525,740 to 2,385,703,844 | |
| `both/pa_first`, the failure path (3 pairs) | 2,609,535,870 to 2,617,294,848 | 718,947,959 to 719,725,260 | **-1.89G, -72%** |
| `two_others/rs_first --emit-c` (3 pairs) | 4,837,939,949 to 4,846,930,516 | 4,905,761,090 to 4,914,706,080 | **+68M, +1.4%** |
| `two_others/pw_first --emit-c` (3 pairs) | refused, 2,664,160,149 to 2,666,100,974 | written, 5,030,927,130 to 5,041,648,056 | |
| `selfhost/main.hero`, build (2 pairs kept of 3) | 580,738,624,200; 581,266,742,759 | 580,939,355,378; 581,060,573,506 | **+0.03%, -0.04%: none measurable** |
| `selfhost/main.hero --emit-c`, the seed's text (2 pairs kept of 3) | 691,384,487,918; 690,560,589,156 | 692,142,176,714; 691,536,714,565 | **+0.11%, +0.14%**, both above, inside the baseline's own spread of 0.8G |

**The compiler's own `--emit-c` is byte-identical under both** (`cmp` of `self-emit-base.c` and
`self-emit-agree2.c`, 31,510,752 bytes): the one-file arm writes the seed's text unchanged. The
`--emit-c` delta, if real, is `fused` building its `Asking` (the records' C names,
`layout_text.asked`) before it finds that the program has no group record with fields; the
landing asks that first, by a walk of the declarations that names nothing (an inference from
the code, unmeasured).


### 17h. A correction to section 12, the critic's C3: my map figures counted the clang driver

Section 12 said ld64's map costs *"no measurable"* instructions, on a small link and on the
compiler's own. The critic found that `/usr/bin/time -l clang ...` counted the driver, and that is
what my figures were. Re-run on the `ld` command `clang -###` prints, run directly
(`map-cost-ld-self.txt`, `repro/link/out-mac/map-cost-ld.txt`), first pair discarded:

| link | `ld` without `-map` | `ld` with `-map` | delta |
|---|---|---|---|
| `dup_written` (five objects, `-la -lb`; 3 pairs) | 337,314,552 to 337,907,785 | 340,504,584 to 343,192,025 | **+3M to +6M, about +1%** |
| the compiler's own (761 objects, no library; 2 pairs) | 1,861,526,662 to 1,867,822,549 | 2,052,557,753 to 2,053,424,488 | **about +190M, +10%** |
| `dup_written` with `-all_load` (route D's second link; 3 runs) | | 325,755,335 to 330,714,866, exit 1 | **one more `ld`, about 0.33G, plus the driver** |

So the map is about free on a program's small link and about a tenth of the link on the
compiler's; a first attempt wrapped in `sh` read 96M for that link, the shell's count and not
`ld`'s, and is discarded. **What follows for 2363's price**: the map is asked only where the line
carries a `-l` or a package's link words (section 12), so the compiler's own build, which links no
library, still pays nothing; a program with libraries pays its link's map, about 1% on a small
one, the share growing with the symbols linked. Route D's extra link is about 0.33G on `ld` plus
the driver (about 78M, section 12's figure, which is the driver's), so about 0.4G, the critic's
order of magnitude.

### 17i. `intern_*` and R2 (the critic's I3): R2 waits for 2363's ruling too

Nothing built; read from the critic's outputs (`p2/ffi/base-linux.txt`, `v5-linux.txt`,
`v5g-linux.txt`, `combo-linux.txt`, `v5-darwin.txt`, `combo-darwin.txt`): on Linux arm64,
`intern_written` exits 2 (`internal error: linking failed`) and `intern_sorted` prints `100 2 7`
under today's compiler, v5 and v5 with R3's words; **the combined compiler, which carries R2,
prints `100 2 7` in both**; on this Mac every one of them prints `100 2 7` in both orders. My
route changes a unit's header list and no link line (17a; `cli/link.hero` is untouched), so the
move is not the route's; that it is R2's is the critic's inference by elimination, which I share
and did not measure (R2 alone was not built in the container by either of us).

It is section 9's mechanism met on the link's side: **R2 fixes one order for every program,
chosen by the modules' names, so wherever today's verdict differs by the written order R2 can
keep the loud order or remove it, and never adds one**. In section 9 it removed the loud
outcome for `pack`, `pragma` and `partial` (wrong values in both orders); here it removes
GNU ld's exit 2 for `intern_*`. So the order rule widens from *"R2 never before the 2364
route"* to: **R2 lands only after every class whose verdict today depends on the order has a
verdict that does not**: the 2364 route, 2363's refusal of bound names, and a ruling on
`intern_*` (refused by a reader of every library's definitions or by route D, whose
`-all_load` loads `libother`'s member and refuses two `base_helper`s, or written down as a known
silent shape with its reason, since after R2 no order shows it loudly).

### 17j. The landing (section 15) and the verdict (section 16), as they move

**Landing, file by file**, replacing section 15's items 1 to 7 (code lines counted without
comments and blanks, `grep -cvE '^\s*(#|$)'`):

1. **`selfhost/cli/agreed_layout.hero`** (new): `settled`, `fused`, the comparison
   (`disagreeing` with the same-prefix shortcut, `owner_block`), the kept dump
   (`listed_reading`, reusing `Probe.read` where a probing already preprocessed the list),
   `block_of`. About 220 code lines.
2. **`selfhost/cli/agreed_order.hero`** (new): the composers over a unit's blocks and the one
   file's, the blocks sorted by module name, `orders`, the owners' prefixes. About 160.
3. **`selfhost/cli/agreed_told.hero`** (new): the words of 17c, `cause_of`, the first two fields
   that differ. About 220. (The prototype is the three in one: `build/239/sh-agree2/cli/
   agreed_layout.hero`, 911 lines, 617 of code.)
4. **`selfhost/emit/layout_text.hero`** (211 to 221 code): `dump_unit` gives the header state
   back; the layout check's kept answers go cold once.
5. **`selfhost/emit.hero`** (292 to 307): `emit_listed`.
6. **`selfhost/cli/artifact.hero`** (26 to 27) and **`selfhost/cli/one_file.hero`** (171 to 169):
   the one file written and asked under the settled list.
7. **`selfhost/cli/produce.hero`** (255 to 284): the route's two calls.
8. **`selfhost/cli/probing.hero`**: a plan's words factored out of `probings_of` (107-115), so the
   route's `plan_words` and the probes cannot drift.
9. **`tests/harness/suite_emission.hero`**: the blessed emission of
   `run/fixedbugs-563-a-module-s-own-headers-come-before-another-module-s-record`, and its
   `LEAST_REFUSED` where the refused count stands at its floor (17f).
10. The goldens and tests of section 14, with `packlib`'s library built by a compiler test; the
    words' cases (`own`, `both` in both orders in `full`); `.claude/rules/c-boundary.md`'s list and,
    since its `paths:` do not reach `selfhost/cli/` (the critic's § 7 reading), its paths with it.

In all about 600 code lines in three new `cli/` modules and about 55 elsewhere; no lexer,
parser, checker, descriptor, ownership or emitter-of-statements change, no new word.

**Verdict, unchanged in kind, stronger in ground**: **object** to panel 238's direction for 2364
as worded, **approve** the amended route, now built whole: the layout comparison (B) as the
judge, the order search (A) and the owners' prefixes (C) as what it tries, the blocks in the
modules' names' order, `--emit-c`'s one file as one more list, the words of 17c; **R2 after the
route and after a ruling on `intern_*`** (17i). No veto: no core construct, about 650 code lines
in `cli/`. design.md §1.12 (four silent shapes and `packlib`'s one file, each a wrong value at
exit 0 today, each closed on both platforms), §4.19, §4.17 (the words), §1.1 and §1.7 (the size).

**The prediction, restated** (17f falsified section 16's letter): **at the landing round's gate,
`emission` reports exactly one program newly blessed,
`run/fixedbugs-563-a-module-s-own-headers-come-before-another-module-s-record`, and one fewer
refusal, and no blessed file changes its bytes**, because of the route; any other moved,
added or retired emission the route causes falsifies it. Scored by the gate's `emission` run and
the round's closing commit.

**The condition**, as section 16, and one more: this seat turns against the one-file arm if a
program the composer writes under a reordered list means otherwise than `build` in a bound name
that `cli/one_file.hero`'s two questions do not ask (the arm leans on them for every name but a
record's layout).

### 17k. The critic's other findings, from this seat

- **C2** (the ffi-pragmatist's 2364 veto and its lifting condition): run again under the second
  version, the libjpeg pair is refused in both orders (`cb_first` `ffi_record_two_layouts`, 940
  bytes, true; `jp_first` `ffi_header_refused`, as today), so the condition holds of the route as
  it now stands, on this Mac; unrun on Linux.
- **C6** (R2 alone, reproduced by the critic): the same rows as section 9; nothing to add.
- **I1, I2** (the four changes composed): the critic's combination carried the first version;
  the second version touches the same files and four more (`emit.hero`, `emit/layout_text.hero`,
  `cli/artifact.hero`, `cli/one_file.hero`), none of them in v5's (`assemble.hero`,
  `supplied.hero`, `whose.hero`), R2's or R3's lists (read from the critic's I1); the combination
  with the second version is **unbuilt**.
- **W2** (2363's words): not this seat's pen; 17c's vocabulary is the one the spec-warden counted,
  and the same counts reach `ffi_defined_elsewhere`'s *linker*, *link line* and *archive*.
- **Windows**: unrun by this seat, as before; the route compares two dumps on one target, so its
  premise there is that the MSVC target's dump prints the wrapper's block with a `| struct
  hero_ly_N` head and a `[sizeof=` row, which the existing layout check already relies on.
