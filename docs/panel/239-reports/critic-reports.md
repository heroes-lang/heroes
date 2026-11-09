# Panel 239, the completeness critic's second pass: the reports

Read: the four reports in lane `p239` (`docs/panel/239-reports/compiler-engineer.md`,
`ffi-pragmatist.md`, `spec-warden.md`, `historian.md`). Run: in my worktree
`.claude/worktrees/239-critic/`, everything under `build/239/p2/`. The seats' compilers are copied
there, read-only from their folders, each checked by `--version`:

| copy | from | `sources` |
|---|---|---|
| `p2/bin/ce-base` | CE `heroes-base` | `3d44313d62fb67a3` (the tree's) |
| `p2/bin/ce-agree` | CE `heroes-agree` (the route) | `70f06afb1849e019` |
| `p2/bin/ce-r2` | CE `heroes-r2` (panel 238's R2 alone) | `24a42125021a4e4e` |
| `p2/bin/ce-agree-r2` | CE `heroes-agree-r2` | `1ba611944c64baf1` |
| `p2/bin/ffi-v5` | FFI `route/heroes-route-v5` | `0143800228018d3a` |
| `ltree/heroes-next` (Linux) | the coordinator's | `3d44313d62fb67a3` |
| `ltree/ce-agree` (Linux) | CE `linux/heroes-agree` | `70f06afb1849e019` |
| `ltree/ffi-v5`, `ltree/ffi-v5g` (Linux) | FFI `linux/tree/heroes-route5`, `heroes-routeg5` | `0143800228018d3a`, `1edadba701d9a514` |

The machine read a load of 3.87 to 10.88 when I began (`uptime`). Findings are numbered; the list
the synthesis must not adopt without a further run is at the end.

## The findings that should change the resolution, one line each

1. **V1** v5 accepts `ab_*` in both orders on ld64 while another linked library's own call silently
   reaches the group's `pick` (`1 10 10` for `1 10 20`); refused on Linux. v5 alone does not close
   2363; it needs route B or D beside it.
2. **I3** R2's walk turns `intern_*` on GNU ld from exit 2 in one order into a silent `100 2 7` in
   both; R2 waits for a ruling on that shape too, not only for the 2364 repair (C6).
3. **R3** under the CE route `--emit-c`'s one file is still wrong at exit 0 (`packlib`, both
   platforms); the one-file arm is unbuilt, so the route does not yet close 2364.
4. **C1** refusing both orders of `dup_*` is the robust reading: with a library that calls its own
   `pick`, no order realises the program (`1 10` or `2 20`), so choosing the order (C) is not safe.
5. **C2** the libjpeg pair built as Heroes modules is refused in both orders under the CE route (and
   today), so the ffi-pragmatist's 2364 veto is lifted by its own condition; the two seats agree on
   (A) joined to (B).
6. **C3** v5's cost is about +1.0G on a two-library build (arithmetic over measured parts), not
   +5.1%, and v5 as built meets its author's own *object* condition; the CE's map figure counted the
   clang driver, though `ld` itself confirms the map is free on a small link.
7. **C4** the allocator passes v5 because runtime names are not judged; the rule that would judge them
   is unbuilt, and on this Mac the allocator is not reached at all.
8. **W1/W2** the words of both refusals are unsettled: an 8,113-byte message on libjpeg's struct,
   and linker vocabulary the spec-warden rules out.
9. **C5** the historian's prediction fails: the emitted unit ignores `-Wpragma-pack`.

## 1. What reproduces

**R1. The compiler-engineer's route table reproduces on both platforms.** `p2/runce.sh <compiler>
<tag> <runtime>` builds and runs each program in a fresh `build/`; outputs `p2/ce-agree-mac.txt`,
`ce-agree-linux.txt`, `ce-base-mac.txt`, `ce-base-linux.txt`.

| program | base (Mac = Linux) | CE route (Mac = Linux) |
|---|---|---|
| `two_others/pw_first` | exit 1, false `ffi_field_type` | exit 0, `9` |
| `pack/pk_first` | **exit 0, `4 1 -402653184`** | exit 0, `4 1 1000` |
| `pragma/pk_first` | **exit 0, `4 1 -402653184`** | exit 0, `4 1 1000` |
| `partial/pw_first` | **exit 0, `4 3 0 300`** | exit 0, `4 3 2 302` |
| `both/pa_first` / `rb_first` | exit 1, false `ffi_field_type` on `Rb.a` / `Pa.w` | exit 1, `ffi_record_two_layouts` on **`Rb`** / on **`Pa`** |
| `prefix/main` | exit 1, false *`Rs.a` is `int`* | exit 0, `6` |
| `own/main` | exit 1, false *`Rs.a` is `long long`* | exit 1, `ffi_record_two_layouts` on `Rs` |
| `union2/pw_first` (CE's, found beside) | **exit 0, `4 3 0`** | exit 0, `4 3 1000` |
| `nested/pw_first` (CE's) | exit 1, false `ffi_field_type` | exit 0, `4 7 3 2` |
| every `*_first` that builds today | as today | as today |

`diff` of the Mac and Linux outputs (notes left out, and notes in) differs on one line only, the
`packlib` one file (R3). Verdict: **reproduces**, every row, both platforms.

**R2. The ffi-pragmatist's v5 reproduces on both platforms.** Its libraries rebuilt from its
`c/src/libs.sh` into `p2/ffi/c/out-<os>`, its `hero.sh` run against my copies (`p2/ffi/v5-darwin.txt`,
`base-linux.txt`, `v5-linux.txt`, `v5g-linux.txt`):

| program | base Mac / Linux | v5 Mac / Linux | v5 with R3's words, Linux |
|---|---|---|---|
| `dup_*` static and shared, `sig_*`, `weak_*` | builds, wrong value or 138 / 139 in one order | **exit 1 both orders**, both | exit 1 both orders |
| `one_*` | `2 20` both / exit 2 `internal error` and `2 20` | exit 1 both orders, both | exit 1 |
| `interpose` | 138 / 139 | exit 1 | exit 1 |
| `remath` | `4.0` | `4.0` | `4.0` |
| `alloc_main`, `alloc_bound` | `false` / `true` | `false` / `true` | `true` |
| `runtime_name` | two empty lines | **two empty lines** (not judged) | the same |
| `hero_name` | exit 2 `internal error` | **exit 2** | exit 2 |
| `intern_written` / `_sorted` | `100 2 7` both / exit 2 and `100 2 7` | the same as base | the same |
| `dep_sorted` | `41 2` / exit 2 | the same | **`41 2`** |

`cmp` of each pair's message (`hero-runs/v5-<os>/<p>_written-static.err` against `_sorted`): `dup`,
`sig`, `one`, `weak` and shared `dup` **byte-identical** on both platforms. `fixedbugs-2056-a-header-s-
own-macro-of-a-name-it-binds-stands` (copied from `tests/golden/run/`) on Linux: base, v5 (twice) and
v5 with the words each build and print `1000.0 2.0`, its `.expected`. Verdict: **reproduces**.

**R3. `packlib` reproduces on both platforms: under the CE route `build` is right and `--emit-c`'s one
file is wrong, at exit 0.** `p2/re/packlib/`, `librp.a` built from CE's `rp.c` (padded `Rp`):
`build` under the route `4 1 1000` (base `4 1 -402653184`); `--emit-c` writes the file at exit 0
under both, and the file compiled with the runtime and `-lrp` prints `4 1 -402653184` under both. On
Linux my first compile of the file was refused by its own field assertion (*`heroes-ffi-field Rp c`*):
arm64's `char` is unsigned and the compiler builds with `-fsigned-char` (`selfhost/cli/flags.hero:149`);
compiled with that word, `4 1 -402653184` (`p2/re/packlib/one2-*-linux`). So the one file also needs
the compiler's own words to mean what it says, a fact the CE's reproducer command
(`clang -I runtime -I . one.c runtime/runtime.c -L lib -lrp`) does not carry. Verdict:
**reproduces**; same cause as 2364, seen from `--emit-c`, as the CE says.

**R4. `union2` reproduces as a silent member of 2364 today** (`4 3 0`, both platforms, R1's table),
closed by the CE route. Same cause as 2364 (a third shape a field assertion cannot see: the union's
width moves `t`). **`hero_name`** (exit 2 `internal error`, ld64 `duplicate symbol
'_hero_print_end'`, GNU ld the same exit 2) reproduces under base and v5; **its own cause**: a
runtime name defined by a library, the failure path of a link the linker refuses, 2365's path met by
a duplicate. **`intern_*`** reproduces (Mac `100 2 7` both orders; Linux exit 2 one order, `100 2 7`
the other): 2363's class reached by a name no group binds, untouched by v5, which traces bound names
only.

## 2. Contradictions between seats, and which side a run supports

**C1. The spec-warden objects to refusing `dup_written`, "a program the document calls correct and
the link realises"; v5 refuses it. A run supports the refusal.** The spec-warden (§ 6) objects to
route (B) for refusing `dup_written` and prefers (C) or *"any route that makes the group's library
answer in every order"*; v5 is not (B) but refuses `dup_written` too (R2's table), so the objection
reaches v5 by its own ground. The ground does not hold in general: choosing the order that makes the
**program's** binding right can silently break the **other library**. `p2/self/`: `libbs.a` holds
`pick_b.o` (`pick` returns 2) and `b_self.o`, whose `b_only` returns `10 * pick()`, its own library's
`pick`; the program binds `liba`'s `pick` and `libbs`'s `b_only`. `p2/self/run.sh`:

| | this Mac | Linux arm64 |
|---|---|---|
| C, `-la -lbs` | `pick=1 b_only=10` | `pick=1 b_only=10` |
| C, `-lbs -la` | `pick=2 b_only=20` | `pick=2 b_only=20` |
| Heroes base, `self_written` / `self_sorted` | `1 10` / `2 20` | `1 10` / `2 20` |
| v5, both | exit 1, `ffi_defined_elsewhere` | exit 1 |

No order gives `1 20`: the order (C) would choose makes `libbs`'s own `b_only` read `liba`'s `pick`,
`10` for `20`, at exit 0. So "the link realises `dup_written`" is true of the brief's `libb`, whose
`b_only` does not call `pick`, and of no rule the compiler can check without reading the library's
relocations. **Supported side: the ffi-pragmatist's refusal of both orders**; the spec-warden's
objection rests on a program whose correctness depends on a library's insides, and its condition 4
(*"the synthesis adopting (B) for 2363: N2 becomes (B)'s price"*) then applies to v5 as well, whatever
the route's letter.

**C2. The ffi-pragmatist's 2364 veto of the order search against the compiler-engineer's composer,
which searches orders and judges by layout: the composer meets the veto's own condition, measured.**
The ffi-pragmatist vetoes (A) alone on the libjpeg pair (in C, the one order that compiles reads
`jpeg_decompress_struct` at 624 bytes against the library's 656) and lifts the veto *"when the order
search lands only joined to the layout comparison (B) ... or I would withdraw it if the libjpeg pair,
built as Heroes modules, is refused or builds with the library's own layout under the landing
compiler"*. Nobody built the pair as Heroes modules; I did (`p2/jpeg/`, this Mac, jpeg-turbo by
`pkg-config --cflags --libs libjpeg`): `cb.hero` binds `cfgbool.h` (the seat's file: `HAVE_BOOLEAN`,
`typedef unsigned char boolean;`, plus a record `Cb` and two inline functions to bind); `jp.hero` binds
`stdio.h` then `jpeglib.h` `package "libjpeg"` with `record Dinfo tag jpeg_decompress_struct partial`
(`image_width`, `image_height`, `u32`); the root spells both records and needs a `stdio.h` group of
its own, which today's ruled refusal asks for (first run: `ffi_header_refused`, *unknown type name
'FILE'*, base and route alike).

| program | base (`3d44313d62fb67a3`) | CE route (`ce-agree`) |
|---|---|---|
| `cb_first` (`use cb` / `use jp`) | exit 1, **16** `ffi_declared_two_ways` errors (*`jpeg_add_quant_table` is declared `... unsigned char)` ... and `... int)`*) | exit 1, **`ffi_record_two_layouts`**: *`Dinfo` is laid out two ways: 656 bytes aligned to 8 in `jp.hero`'s unit, 624 bytes aligned to 8 in `cb_first.hero`'s* |
| `jp_first` | exit 1, `ffi_header_refused`, *typedef redefinition with different types ('unsigned char' vs 'int')* | the same |

So the pair is **refused in both orders under the route**, which is the veto's lifting condition,
and it is refused today too: the C-level premise *"the search would choose that order and build"*
does not reach a Heroes program on this library, since `jpeglib.h` declares functions taking
`boolean` and panel 202's `ffi_declared_two_ways` sees them declared two ways. **Supported side: no
contradiction remains between the two seats on 2364**: (A) joined to (B) is what both approve.
Linux arm64 has no libjpeg (the ffi-pragmatist's reading), unrun there. A pair whose headers declare
no function the macro changes, only a struct, is the shape where the veto's premise would hold: that
is `pack/` and `partial/` again, which R1 shows the composer closes.

**C3. "A map costs nothing measurable" (CE) against "a probe costs about 0.45G" (FFI): both true,
of different things; the CE's measurement counted the wrong process.** The ffi-pragmatist measured that
`/usr/bin/time -l` counts the process it starts and not that process's children. The CE's
`repro/link/out-mac/map-cost.txt` reads 77.6M to 80.1M with and without the map, the size of the
clang driver alone. Re-run (`p2/cost/`, `dup_main.o` linked against my `liba.a`, `libb.a`, four
pairs each):

| | without `-map` | with `-map` |
|---|---|---|
| `/usr/bin/time -l clang dup_main.o ... -o x` (the driver) | 75.8M to 79.4M | 75.5M to 75.9M |
| the `ld` command `clang -###` prints, run directly | **320.6M to 323.0M** | **321.9M to 322.6M** |

So the map flag folded into the real link is free within the pairs' spread **on this link**, which
supports the CE's conclusion by the instrument that settles it; the CE's number did not. The 761-object
link of the compiler (3 MB map) is unrun on `ld` itself. The FFI's 0.45G is a **whole extra link**
(driver plus `ld`), and v5 runs one per program naming one library and **two** for two or more
(section 5 of its report). Arithmetic over those measured parts, not a measurement: on `dep_written`
(two libraries) v5 adds +86M in the compiler's process and two probe links of about 0.45G each, about
**+1.0G** on a warm build whose compiler process is 1.68G and whose real link is about 0.45G, so
roughly **+45%**, where the report's headline reads *"+86M (+5.1%)"*. **And the ffi-pragmatist's own
condition** reads *"I would move to object ... if a measured probe costs a warm build more than one
extra link per two libraries"*: v5 as built costs two per two. The shape that meets the condition (the
forward probe folded into the real link, one extra) is **unbuilt** by either seat.

**C4. "A group word to allow replacing a C-library function on purpose would add a core construct"
(CE) against the ffi-pragmatist's acceptance of the allocator: no contradiction in substance, but the
acceptance is acceptance by not looking.** v5 judges bound names only (`supplied.hero`'s probe traces
the names a group binds), so `alloc_main` (binds only `myalloc_served`) passes because `malloc` is
nobody's binding, and `runtime_name` passes for the same reason (R2's table: two empty lines under v5,
both platforms). The rule the ffi-pragmatist would land for the runtime's names (*"refused unless a
group binds that name with that library"*, its § 8) uses the existing `extern` form, so it needs no new
word, and the CE's veto condition is not met; but that rule is **unbuilt**, and under it `alloc_main`
would be refused and `alloc_bound` accepted. And on this Mac the allocator is not reached at all:
`alloc_main` and `alloc_bound` print `false` (base and v5, `p2/ffi/v5-darwin.txt`), `true` on Linux;
*why* is unmeasured in the report and here.

**C5. The historian's registered prediction is falsified on its first clause, by the unit's own
text.** Prediction: compiling the root unit of `pragma/pk_first` *"as Heroes emits it, with clang's
default warnings"* prints `[-Wpragma-pack]` for `pk.h`; `pack/` and `partial/` print none. Run
(`p2/re/<shape>/build/tu-*/pkfirst.c`, `pwfirst.c`, `clang -fsyntax-only -fsigned-char -I runtime -I
.`): **0** `-Wpragma-pack` lines for all three. The emitted unit opens its header region with `#pragma
clang diagnostic ignored "-Wpragma-pack"` (line 19 of the unit, beside `ignored "-Wall"`), so the
warning cannot reach anyone; a plain C file `#include "pk.h"` / `#include "rp.h"` prints it once
(*"the current #pragma pack alignment value is modified in the included file"*). So *"an existing clang
warning closes one member at no cost"* is false of the compiler as it stands; the other two clauses
(none for `pack`, none for `partial`) hold.

**C6. The CE's R2-alone row reproduces: R2 must not land before the 2364 repair.** `p2/order.sh` over
the CE's `order/<shape>/{written,swapped}/` roots (copied):

| shape | `ce-r2` (R2 alone), written / swapped | `ce-agree-r2` (route with R2) |
|---|---|---|
| `two_others` | exit 1 / exit 1, false `ffi_field_type` | `9` / `9` |
| `renamed` (`pw` named `zz`) | `9` / `9` | `9` / `9` |
| `pack` | **`4 1 -402653184` / `4 1 -402653184`** | `4 1 1000` / `4 1 1000` |
| `pragma` | **`-402653184` both** | `1000` both |
| `partial` | **`4 3 0 300` both** | `4 3 2 302` both |
| `both` | exit 1 both, `Rb.a` | exit 1 both, `ffi_record_two_layouts` on `Rb` both |

**C7. The `own` message's last note leads somewhere; the CE's doubt about it holds for `both` only.**
The note: *"have `rs.hero` bind first the header that configures it"*. Followed literally
(`p2/ownfix/a/`: `rs.hero` binds `m.h` above `rs.h`, `a: i32` kept): exit 1, `ffi_field_type`, *`Rs.a`
is `long long` in `rs.h`*, now **true** of `rs`'s unit, `fix (guess): declare a with i64`; that fix
applied (`ownfix/b/`, and `m.total` adjusted to `i64`): exit 0, `9`. Two steps, each told truly.

## 3. The words

**W1. The CE's `ffi_record_two_layouts` lists every member of both layouts; on a real record it is a
wall.** `p2/jpeg/o-ce-agree-cb_first.txt`: **8,113 bytes**, 9 lines, the two layout notes 3,690 bytes
each, **186** `<member> at <offset>` pairs (`grep -o`), of which the first to differ is `raw_data_out`
(92 against 89). design.md §4.17 asks for everything needed to fix the mistake; a model has to diff
two 93-member lists by eye to find it. The landing's words should name the size, the alignment and the
**first members that differ**, not every member. The base's answer on the same program is worse
(16 errors, 14,841 bytes), so this is not a regression, but the words are unsettled.

**W2. The ffi-pragmatist's message leans on the words the spec-warden counts as absent.** v5's
message (`p2/ffi/hero-runs/v5-Darwin/dup_written-static.err`): *"the linker found a definition in
`.../liba.a(pick_a.o)`"*, *"`-lb` is linked for the group of `b.h`"*, *"taken from the first library on
the **link line** that defines it"*, *"over an **archive** `objcopy --localize-symbol=pick`"*. The
spec-warden (§ 9) measured *linker*, *link line*, *archive* at 0 in the specification and asks that
the refusal *"must not say: archive, member, link line, linker"*. Two seats' pens disagree and neither
saw the other's; the synthesis owes one reading, and the archive paths (`liba.a(pick_a.o)`) are the
one place where the linker's own word is the only exact name of the file.

## 4. The tracked tree, re-run

**T1. The ffi-pragmatist's 25 roots: zero false refusals under v5 on this Mac, reproduced.** Its
`runs.sh` and `roots.txt` copied, run from my worktree's root with my compiler and with `ffi-v5`
(`p2/roots-base-mac.txt`, `roots-v5-mac.txt`): 25 of 25 built and ran under both, and `diff` of the
two whole files prints nothing (verdict and the first 100 characters of each output). Linux arm64's
25: not re-run here (the seat's `runs-route5-linux.txt`).

**T2. The compiler-engineer's 21 roots: unmoved under the route, reproduced.** Its `run-tracked.sh`
and `tracked/roots.txt` copied, the roots' folders copied from `git ls-files` into `p2/tracked/`
(`p2/trk-base.txt`, `trk-agree.txt`): `diff` of verdict, output and the digest of every unit's C
prints nothing; `agreed-dirs` is 0 in 17 roots, 1 in 2, 2 in 2, as the seat reports. The six that
exit 1 today (`fixedbugs-563`'s `ffi_header_refused`, two `surface-fixtures`, three `unsupported`,
among them both `fixedbugs-559` refusals) exit 1 the same under the route. So the CE's prediction (the
landing moves no blessed emission) rests on a measurement that reproduces; it is still scored only at
the landing's gate.

## 5. A route nobody built: both repairs, with R2's walk and R3's group words

**I1. The four changes compose on this Mac; nothing either repair decides moves.** CE's route (its
`sh-agree/cli/agreed_layout.hero` and `produce.hero`), the ffi-pragmatist's `route/diff/route.diff`
(`assemble.hero`, `supplied.hero`, `whose.hero`), panel 238's `prototype.diff` (R2) and `group.diff`
(R3) touch disjoint files; applied to one copy of `selfhost/` from `git ls-files` (`p2/combo/root/`,
`patch` exit 0 each, no offset or fuzz in the logs), stage 2 built by my compiler: exit 0,
**772,522,685,623 instructions retired**, `sources 6377104d6b4a4f68` (`p2/combo/build.log`).

- **2364's programs** (`p2/combo-2364-mac.txt`): every line the CE route's but one, `both/rb_first`,
  which now names `Rb` as `pa_first` does: R2's walk fixes the word, as the CE predicted (its § 9).
  The order variants (`p2/combo-order-mac.txt`): `two_others`, `renamed`, `pack`, `pragma`,
  `partial` build with their true values in both orders, `both` refused on `Rb` in both.
- **2363's programs** (`p2/ffi/combo-darwin.txt`): the first 60 columns of every line equal v5's;
  `self_*` refused in both orders; `dep_sorted` `41 2` (this Mac links it bare too).
- **Linux arm64**: the combined compiler built in the container from the same copy; results in I2
  below.

So the two repairs do not meet on this Mac, and the order of landing the CE measured (the route
before or with R2) holds with v5 beside it. A program holding both a 2363 duplicate and a 2364
disagreement is told the 2364 refusal first (it comes before any unit compiles) and the 2363 one
only after the first is fixed: read from where each sits, not run.

## 6. A hole in v5, at depth one, the same cause as 2363

**V1. On ld64 the group's library can win the link in both orders while another library the
program names silently reaches it for its own call; v5 accepts the program in both orders.**
`p2/self/run2.sh`: `libab.a` holds one member, `ab.o`, defining `a_only` and `pick` (returns 1); the
group `extern "a.h" link "ab"` binds both; `libbs.a` holds `pick_b.o` (`pick` returns 2) and `b_self.o`,
whose `b_only` returns `10 * pick()`, its own library's; another group binds `b_only` with `link "bs"`.

| | this Mac |
|---|---|
| C, `-lab -lbs` and `-lbs -lab` | `pick=1 a_only=10 b_only=10`, both |
| base, `ab_written` / `ab_sorted` | `1 10 10` / `1 10 10` |
| **v5**, both | **built, `1 10 10` / `1 10 10`**, no word |
| combined compiler, both | built, `1 10 10` both |

`b_only` should be `20` by its own library. ld64 loads `ab.o` for `a_only` before it asks for `pick`
(the name order of A2 in my first pass), so `pick_b.o` is never loaded and no probe in either order
names it; v5 asks only *who supplied the bound name*, and the answer is the group's own library both
times. This is C11 6.9p5's two definitions all the same, the class v5 exists to refuse, and only a
reader of every library's definitions (route B) or a link that loads every member (route D: `-all_load`
would load `pick_b.o` and refuse *duplicate symbol*) can see it. Linux arm64: in I2.

**V1, on Linux arm64** (`p2/self/ab-linux.txt`): C `-lab -lbs` gives `pick=1 a_only=10 b_only=10`,
`-lbs -lab` is refused by GNU ld (*multiple definition of 'pick'*); base builds `ab_written` silent
(`1 10 10`) and fails `ab_sorted` at exit 2 `internal error`; **v5 and the combined compiler refuse
both orders** (*"reaches `libab.a(ab.o)` in one order of the libraries, and in the other the linker
refuses two definitions"*). So **v5's verdict on one program differs by platform**: accepted
silently wrong on this Mac, refused on Linux. It falsifies, on ld64, the ffi-pragmatist's sentence
the order-freedom rests on (its § 2, item 3: *"Any name two of the libraries define reaches the
other one in one of the two orders"*): here `libbs`'s `pick` is reached in neither.

**I2. The combination on Linux arm64** (built in the container from the same copy, `sources
6377104d6b4a4f68`, `p2/combo/build-linux.log`; outputs `p2/combo-2364-linux.txt`,
`combo-order-linux.txt`, `ffi/combo-linux.txt`): every 2364 line equals this Mac's (`diff`, notes
left out, `packlib` aside: my hand compile again lacked `-fsigned-char`, R3); the order variants as
on this Mac; every 2363 program as v5 with R3's words (`ffi/v5g-linux.txt`) but one.

**I3. R2's walk turns a loud failure of 2363's reach into a silent wrong value in both orders, on
GNU ld.** That one program is `intern_written` (`libtop`'s `top_value` calls `base_helper`, which
`libbase` defines as 40 and `libother`'s single member, loaded for `other_only`, defines as 99):

| `intern_written` / `intern_sorted`, Linux arm64 | |
|---|---|
| base | exit 2 `internal error` / `100 2 7` |
| v5 | exit 2 / `100 2 7` |
| v5 with R3's words | exit 2 / `100 2 7` |
| **the combination (with R2)** | **`100 2 7` / `100 2 7`** |

`100` is `libother`'s `base_helper` reaching `libtop` (`41` with `libbase`'s, as `dep_*` prints). The
CE route touches no link line and R3's words alone keep the exit 2, so the move is R2's: its key
orders the modules `base_lib`, `other_lib`, `top_lib` whatever the `use` lines say, the order in which
`intern_sorted` is already silent (that it is R2 is an inference by elimination; R2 was not built
alone in the container). On this Mac both orders were silent already (`100 2 7`, base and v5). This is
the CE's § 9 finding for 2364 (*R2 alone makes the class worse*) met again on 2363's side, in the shape
no seat's repair reaches (the ffi-pragmatist's *"a question for later"*, its § 8). **R2 must not land
before the synthesis decides `intern_*`**: refused (route B or D), or recorded as a known silent shape
with its reason, since after R2 no order of the program shows it loudly any more.

## 7. Four more readings, short

- **The CE's "no measurable cost" of the map on the compiler's own 761-object link** counted the
  clang driver (101M); the `ld` process of that link is unrun (C3).
- **The CE's § 5a**: the existing layout check's dump unit writes its wrappers after the includes
  without giving the header state back, as the CE's first prototype did; *"moves nothing it reads"*
  is read, not run, beyond `rp_first` building. Unrun here too.
- **The spec-warden's § 8 observation reproduces**: `.claude/rules/c-boundary.md`'s `paths:` are
  `runtime/**`, `examples/**`, `selfhost/emit/ffi*`, `selfhost/parse/group.hero` (`sed -n 1,10p`),
  and both repairs live in `selfhost/cli/` (`agreed_layout.hero`, `supplied.hero`), so the rule does
  not load when they are written.
- **Windows** is unrun by every seat and here; v5 asks no probe there (its § 11), so on that leg the
  2363 class stays open under v5, by design, and the CE route's dump format is unrun on the MSVC
  target.

## What the synthesis must not adopt without a further run

1. **v5 as closing 2363.** It misses `ab_*` on ld64 (V1: a second library's own call silently
   reaches the group's definition, accepted in both orders, refused on Linux). Owed before adoption:
   v5 joined to a reader that sees every library's definitions of a bound name (route B, a symbol
   reader) or to the whole-archive probe (route D, which loads `pick_b.o`), run on `ab_*`, `self_*`,
   the 25 roots and glibc's pairs, both platforms.
2. **v5's cost as "+5.1%".** The measured parts say about +1.0G on a two-library program whose
   compiler process is 1.68G and whose link is about 0.45G (C3, arithmetic over measured parts), and
   the ffi-pragmatist's own condition (*"more than one extra link per two libraries"*) is met by v5 as
   built. Owed: the folded shape (the forward probe read from the real link, one extra link) built
   and its total measured on `ld` and the driver, not on the compiler's process alone; and whether a
   probe's verdict can be cached by the link's key on a warm build, unasked by either seat.
3. **The CE route as closing 2364.** `build` is right and `--emit-c`'s one file is still wrong at exit
   0 (`packlib`, R3, both platforms); the one-file arm the CE lists (its landing item 7) is unbuilt.
   Owed: that arm built and `packlib` run, plus `union2`, `nested` and the libjpeg pair, before the
   class is called closed.
4. **R2 before both repairs, and before a ruling on `intern_*`.** R2 alone makes `pack`, `pragma`,
   `partial` wrong in both orders (C6) and `intern_*` silent in both orders on GNU ld (I3).
5. **The words of either refusal as built.** `ffi_record_two_layouts` lists every member (8,113 bytes
   on libjpeg's struct, W1), names `Pa` or `Rb` by the walk without R2 (R1), and its last note is
   false for `both` (the CE's own reading); `ffi_defined_elsewhere` uses *linker*, *link line*,
   *archive*, which the spec-warden asks it not to (W2). Owed: one set of words, read against the
   specification's vocabulary, run on `both`, `own`, `dup`, `one`, `interpose` and the libjpeg pair.
6. **The allocator as settled.** v5 accepts it by not judging the runtime's names (C4); the rule
   that would judge them (a runtime name a library supplies is refused unless a group binds it there)
   is unbuilt, would refuse `alloc_main` and accept `alloc_bound`, and on this Mac the allocator is not
   reached at all (`false`), unexplained. `runtime_name` and `hero_name` stay silent and exit 2.
7. **"0 spec tokens" for 2363 on the spec-warden's reading that `dup_written` is a correct program
   the link realises.** C1: with a library that calls its own `pick`, no order realises the program
   (`1 10` or `2 20`, never `1 20`), so the refusal of both orders is the robust reading; the
   spec-warden's condition 4 (N2 becomes the price of refusing `dup_written`) is then live for v5,
   and a sentence or none must be decided on that ground, not on (B)'s letter.
8. **The historian's "an existing clang warning closes one member at no cost"** (C5): the emitted
   unit ignores `-Wpragma-pack` on purpose.
9. **Anything on Windows**: unrun by every seat.
