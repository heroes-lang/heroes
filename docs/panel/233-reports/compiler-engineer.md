# Panel 233, the compiler-engineer's report: defect 2278

Written as it went, 2026-11-01, in `.claude/worktrees/233-compiler-engineer/` (a detached checkout
of `0ee14c9be`). No wall clock read: clang's work in instructions retired and peak memory,
`/usr/bin/time -l clang -fintegrated-cc1 <flags> -c <unit>.c -o /dev/null`. One depth-18-sized
compile at a time. Every number below carries the command that produced it.

## 0. Setup

- Stage 1, `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`: `./heroes --version` prints
  `sources 1831f7e156474293`. Stage 2, `./heroes build selfhost/main.hero -o heroes-next`: prints
  `sources 2764fc8525370b80`, copied to `ce/heroes-frozen` before any edit, the compiler every
  "before" below is.
- The words: read from the response file my frozen stage 2 wrote for `build ce/repro/bigframe12.hero
  -O2` (`build/words/words-3e50d17976c1f7f8.rsp`): the nineteen words of `flags()`, `-O2
  -gline-tables-only -I runtime -I <program dir>`, then `-c -fno-crash-diagnostics <unit>.c`. The
  wrapper `ce/m.sh <unit> <dir> <level>` runs exactly those under `/usr/bin/time -l clang
  -fintegrated-cc1 ... -o /dev/null` (`-g` for `-O0`, as `debug_words` says), and leaves out
  `-MD -MT -MF` and `-Xclang -object-file-name=`, which write a dependency file and name the object.
  No `-fstack-clash-protection`: `frame_words()` is empty on this Mac (Darwin probes with
  `___chkstk_darwin`). Apple clang 21.0.0, Apple M3, 24 GiB.
- The units: each compiler's `build <program> -O0` writes its unit under `build/tu-*/`, copied to
  `ce/units/<compiler>/`. The frozen k = 18 unit differs from `repro/bigframe18-unit.c` only in the
  path inside its first comment and its `#line` directives (`diff`), because I build from the
  worktree's root.

## 1. The baseline, measured here

| unit (frozen compiler) | level | instructions retired | peak footprint (bytes) |
|---|---|---:|---:|
| bigframe12 | `-O2` | 3,146,446,609 | 119,308,768 |
| bigframe12 | `-O0` | 243,417,243 | 9,159,064 |
| bigframe16 | `-O2` | 66,181,586,608 | 2,779,826,528 |
| bigframe18 | `-O2` | 314,399,003,206 | 12,305,294,384 |

(`ce/m.sh ce/units/frozen/bigframe<k>.c ce/repro <level>`, each alone, no other clang running by
`pgrep -fl clang`.) The critic's 65,669,753,280 at k = 16 with the build's words reproduces to 0.8 %.

## 2. The filing's other three shapes, written and measured before any change

Written by `ce/gen_shapes.py <k> ce/shapes` in the worktree: `E0` holds one `i64`, `E_k` eight
`E_(k-1)`, so `E_k` is 8^(k+1) bytes (E5 256 KiB, E6 2 MiB, E7 16 MiB, E8 the filing's 128 MiB).
Every program prints `before` and one leaf, `v.a.a...x`, which is `1`.

- **(2) `onefn<k>`**: `main` builds `v0 = E0(x: 1)`, then `v_k = E_k(a: v_(k-1), ..., h: v_(k-1))`.
- **(3) `byval<k>`**: the same `main`, the records in `held.hero`, and `print(held.first(e: v_k))`,
  `first` taking `E_k` by value and returning its leaf.
- **(4a) `levels1_<k>`**: `levels.hero` holds the records and `make_k(v: E_(k-1)) -> E_k` returning
  `E_k(a: v, ..., h: v)`; `main` calls one per level.
- **(4b) `levels8_<k>`**: `levels.hero` holds `make_k(n: i64) -> E_k` returning `E_k(a: make_(k-1)(n:
  n), ..., h: make_(k-1)(n: n))`, eight calls per level; `main` calls `levels.make_k(n: 1)`.

All four pass `heroes check` and print `before`, `1` at depth 3 at `-O0` (frozen compiler). Every
depth up to E8 builds at `-O0`. The units come from `ce/units.sh <compiler> <tag> <program>`, which
builds at `-O0` in a fresh directory (a cold cache every time) and keeps each unit but the library's.

Frozen compiler, `-O2`, the build's words, each unit alone (`ce/mg.sh`, a guard that kills clang past
a resident size or a time bound; the bound is a stop condition, never a measurement):

| shape, unit | instructions retired | peak footprint (bytes) |
|---|---:|---:|
| onefn5 `onefn5.c` | 5,223,700,609 | 162,677,264 |
| byval5 `byval5.c` (the caller) | 82,904,763,110 | 1,412,154,856 |
| byval5 `held.c` (the callee) | 176,270,525 | 9,060,736 |
| levels1_5 `levels.c` | 283,432,766 | 9,699,712 |
| levels1_5 `levels15.c` | 233,696,356 | 9,126,272 |
| levels8_5 `levels.c` | 106,642,238,306 | 1,448,379,856 |
| levels8_5 `levels85.c` | 218,470,546 | 9,208,192 |
| onefn6 `onefn6.c` | 40,647,266,942 | 1,214,645,520 |
| byval6 `byval6.c` | **not finished: stopped by the guard at its 600 s bound**, resident 1,065,024 KB when stopped | |
| byval6 `held.c` | 231,702,994 | 9,044,376 |
| levels1_6 `levels.c` | 571,426,370 | 9,798,040 |
| levels1_6 `levels16.c` | 524,116,932 | 9,290,112 |
| levels8_6 `levels.c` | **1,908,194,029,234** (retaken alone; the first reading was stopped by the coordinator as an orphan when this seat's run was cut by the account's rate limit) | **10,409,681,128** |
| levels8_6 `levels86.c` | 508,902,719 | 9,240,984 |

What these say, before any route:
- **The cost is per unit, and always in the unit that holds the scalar construction and its
  copies**: shape (4)'s caller units stay under 0.6 billion, and shape (3)'s callee does too.
- **Shape (4a), one call per level, is cheap today** (0.57 billion at E6), and **shape (4b), eight
  calls per level, is the reproducer's mechanism** (106.6 billion at E5 for a 256 KiB record, where
  the two-way reproducer at 1 MiB, k = 16, costs 66.2 billion).
- **Shape (3), the value handed whole to a call, is the worst**: 82.9 billion at E5 against 5.2
  billion for the same `main` reading one leaf (shape 2), and past 600 s at E6. A reading of one
  leaf lets clang delete every other leaf early; a value handed to a call keeps every leaf alive
  through every later pass. This is an inference from the two programs' one difference, not a
  measurement of the passes.

## 3. Frames, measured: `-fstack-usage`, and R6 run

`ce/su.sh <unit> <dir> <level>` compiles a unit as the build does with `-fstack-usage` and prints each
function's frame in bytes. `ce/link.sh` links a hand-edited unit with the runtime and library objects
of a frozen `-O2` build of `bigframe12` (the library unit differs between the programs only in its
first comment, `diff`), so a hand experiment runs like a built program.

**Today, frozen compiler** (largest frames):

| unit | level | frames (bytes) |
|---|---|---|
| bigframe16 | `-O0` | `main` 3,145,856, `make16` 3,145,856, `make15` 1,572,992, `make14` 786,560 ... |
| bigframe16 | `-O2` | `make16` 2,359,344 (its out-of-line copy), `make15` 786,480 ...; `main` is not among the eight largest |

At `-O0` each `make_k` holds three times its record (the two halves, the whole, the calls' hidden
temporaries, the compound literal), so the chain from `main` passes 8 MiB at k = 16, which is why the
critic's `-O0` run of k = 16 died in `make14`. **A probe of what clang does with a struct returned by a
call** (`ce/probe/slot.c`, `-S -emit-llvm`): `x = make(n)` into a local gives a hidden temporary and a
`memcpy` at `-O0`, merged at `-O2`; `*p = make(n)` through a pointer keeps the hidden temporary
**even at `-O2`**; only an initialization, `Big x = make(n);`, has the callee write straight into its
destination. So a route that moves large locals off the frame still leaves one full-size hidden
temporary per call site, unless the callee writes into its caller's slot.

**R6 by hand** (the critic's: `__attribute__((noinline))` before every `make` definition, one `sed`,
`ce/units/r6hand/`):

| k | instructions retired | peak footprint | `-O2` frames | `-O2` run |
|---|---:|---:|---|---|
| 16 | 709,396,954 | 10,469,784 | `make16` 1,572,912, `main` 1,048,608, `make15` 786,480 | `before`, `1` |
| 18 | 1,881,302,410 | 10,568,064 | `make18` 6,291,504, `main` 4,194,336, `make17` 3,145,776, `make16` 1,572,912 | **`before`, `panic: stack exhausted in bigframe18.make18`** |

**R6 alone turns today's correct `-O2` run of k = 18 into the stack abort**: a function that is not
inlined keeps its temporaries at `-O2`, about 15 MiB along the chain against Darwin's 8 MiB `main`
stack. Today's `-O2` run prints `1` only because, with everything inlined into `main`, clang folds the
whole record to the one leaf the program reads. The compile numbers reproduce the critic's (1.86 and
0.69 billion).

## 4. A route built by hand first: large values off the frame, written in place

Before writing compiler code, the route was tried on the emitted C by a rewriter,
`ce/offframe.py <in.c> <out.c> <T> [outparam] [fieldwise]`, applied to the frozen units:

- **off-frame**: every local or temporary whose C struct is larger than `T` bytes is declared as a
  pointer to `malloc`'d memory, `R17 *const hero_big_t2 = ...`, and its name is made that memory for
  the function, `#define t2 (*hero_big_t2)`, `lend_local`'s spelling (panel 196's R7); freed by
  `__attribute__((cleanup))` after the return value is copied;
- **outparam**: a function returning a record larger than `T` returns through its caller's slot,
  `void make17(R17 *hero_out, int64_t n)`, `return t7;` becoming `*hero_out = t7; return;` and a
  call `t2 = make17(t1);` becoming `make17(&t2, t1);`;
- **fieldwise**: a construction of a record larger than `T`, `t7 = (R18){.f_left = t2, .f_right =
  t6};`, becomes `t7.f_left = t2; t7.f_right = t6;`, so no compound literal is built on the frame.

Depth 18, `T` = 64 KiB, each step added in turn (`-O2`, the build's words; frames by `ce/su.sh`; runs
linked by `ce/link.sh`):

| hand route at k = 18 | instructions retired | peak footprint | largest `-O2` frame | largest `-O0` frame | `-O2` run | `-O0` run |
|---|---:|---:|---:|---:|---|---|
| frozen (today) | 314,399,003,206 | 12,305,294,384 | (folded into `main`) | 4,194,464 (`main`) | `1` | stack exhausted (critic) |
| off-frame | 562,541,945 | 11,960,728 | 4,194,368 `make18` | 8,388,816 `make18` | **stack exhausted in `make14`**, 134 | |
| off-frame + outparam | 546,135,398 | 11,927,936 | 4,194,368 `make18` | 4,194,480 `make18` | | |
| **off-frame + outparam + fieldwise** | **462,535,262** | **11,813,248** | **147,504** `make12` | **262,288** `make13` | **`before`, `1`, exit 0** | **`before`, `1`, exit 0** |

Off-frame storage alone ends the explosion (SROA never sees a large aggregate, so nothing cascades),
but it is not a route: the call's hidden temporary (§ 3's probe) and the compound literal stay on
the frame, and the `-O2` run aborts. With all three, the k = 18 program compiles in 0.15 % of today's
instructions and runs **at both levels**, where today `-O0` aborts.

The other shapes, the same rewrite at `T` = 64 KiB, then at 4,096 and 512 bytes:

| unit | frozen | `T` = 64 KiB | `T` = 4,096 | `T` = 512 |
|---|---:|---:|---:|---:|
| onefn6 `onefn6.c` | 40,647,266,942 / 1.21 GB | 73,497,796,950 / 1.46 GB | 5,429,246,969 / 64 MB | 856,711,878 / 17 MB |
| byval6 `byval6.c` | past 600 s | 73,540,911,531 / 1.46 GB | 5,453,167,904 / 64 MB | 881,815,107 / 17 MB |
| levels8_6 `levels.c` | 1,908,194,029,234 / 10.4 GB | 103,174,554,181 / 1.42 GB | 6,521,666,777 / 68 MB | 897,220,382 / 18 MB |
| byval5 `byval5.c` | 82,904,763,110 / 1.41 GB | 73,510,358,506 / 1.46 GB | | |
| levels8_5 `levels.c` | 106,642,238,306 / 1.45 GB | 103,196,170,480 / 1.42 GB | | |
| levels1_6 `levels.c` | 571,426,370 | 324,035,391 | | |

(Instructions retired / peak footprint. The callee and caller units the table leaves out stay under
0.26 billion and 10.2 MB in every case.)

**What the threshold does**: what is left is the cost of the largest record still on the frame,
which clang still scalarizes and then copies, field by field, into memory off the frame. At 64 KiB
that is `E4`, 32 KiB and 4,096 leaves, and eight such copies per construction cost 73 to 103 billion
whatever the whole record's size (byval5 and byval6 read the same). At 4,096 bytes the largest record
left on the frame is `E3`, 512 leaves, and the cost falls to 5.4 to 6.5 billion; at 512 bytes, about
`-O0`'s. So the route bounds the cost by `T`, never by the record, and `T` is the price paid.

**At `T` = 4,096 bytes, every shape at every size, compiled and run** (hand route, `ce/units/off4096/`
and `ce/units/in4096/`; runs linked with frozen `-O2` and `-O0` runtime and library objects, `timeout
120`, output read whole):

| program | instructions retired | peak footprint | `-O2` run | `-O0` run |
|---|---:|---:|---|---|
| bigframe16 | 487,441,131 | 11,682,176 | `before`, `1` | `before`, `1` |
| bigframe18 | 444,663,077 | 12,042,624 | `before`, `1` | `before`, `1` |
| onefn7 (16 MiB) | 5,475,795,316 | 64,209,352 | `before`, `1` | `before`, `1` |
| onefn8 (128 MiB) | 5,526,148,074 | 65,733,040 | `before`, `1` | `before`, `1` |
| levels8_7 | 6,555,561,467 | 68,846,024 | `before`, `1` | `before`, `1` |
| levels8_8 (128 MiB) | 6,580,010,331 | 67,846,576 | `before`, `1` | `before`, `1` |
| levels1_7 | 395,810,150 | 10,944,896 | `before`, `1` | `before`, `1` |
| byval7, byval8, levels1_8, three parts | 5.68, 6.95, 0.46 billion | ≤ 66 MB | **stack exhausted in `<module>.main`**, 134 | the same |
| byval7, + inparam | 5,536,845,026 | 64,127,408 | `before`, `1` | `before`, `1` |
| byval8 (128 MiB), + inparam | 5,520,699,577 | 64,569,776 | `before`, `1` | `before`, `1` |
| levels1_8 (128 MiB), + inparam | 364,082,615 | 10,830,208 | `before`, `1` | `before`, `1` |

(The caller's or callee's other unit stays under 0.48 billion and 10.4 MB in every row.) The three
that still aborted share one thing: a record past the stack handed **by value** to a parameter,
whose copy C's ABI makes in the caller's frame. So the out-parameter has a mirror, **inparam**: a
parameter larger than `T` is handed as `const T *` to the copy the caller's IR already made, and the
callee's name for it is `#define h0_e (*hero_in_h0_e)`. With the four parts, **the filing's 128 MiB
record, built in one function, handed by value to another module, or built level by level in a module
of its own with one call per level or eight, compiles in under 7 billion instructions and 70 MB and
runs at `-O2` and at `-O0`**, where today `-O2` does not finish and `-O0` aborts.

## 5. The route, built in the compiler and measured

Built in this worktree, `selfhost/` and `runtime/`, then compiled by the stage 1 compiler into
`ce/heroes-route` (`sources 1c8b20b0133fa971`). Its two generations emit the compiler as the same
28,992,484 bytes (`cmp` of `build selfhost/main.hero --emit-c`, stage 2 against stage 3, on the
route's previous build; repeated on the final build in § 8). **The compiler's own C holds no large
value**: `hero_frame_take`, `hero_out` and `hero_in_` appear in it only as the emitter's own string
literals (`grep -n`), so no record of `selfhost/` passes 4,096 bytes and self-hosting neither needs
nor feels the route.

What it emits, one rule each (`selfhost/emit/large_value.hero`, new):

- **the threshold**: a type of the program's own (`byte_copy.own_struct`: a record that is no group's,
  a variant, a `T?`, a case's payload) whose bytes, counted as fields summed with no padding, pass
  4,096. Counted once per build in one pass over `c.type_order`, which the checker builds with every
  record after what it holds (`check/sized.hero`), so no recursion walks a chain; carried in
  `ctype.Names.large` and asked by `ctype.is_large`;
- **off the frame**: a slot or temporary of such a type takes its room from a per-thread stack in the
  runtime (`runtime/parts/frame.c`, new: a bump pointer in chunks, the last taken given back first,
  a give out of order a loud internal error, the chunks from `hero_malloc_raw`), its name `#define`d
  to the room for the body and `#undef`d after it, as `emit/lend_local.hero` does for a lent local;
  zeroed where it holds a counted value; given back at the one exit, in reverse;
- **the caller's slot**: a Heroes function returning such a type is `void f(T *hero_out, ...)`, and
  its return writes `if (hero_out != NULL) *hero_out = t;` before any room is given back; a direct
  call passes `&target`, or `NULL` where the result is discarded;
- **the caller's copy**: a plain parameter of such a type is `const T *hero_in_<name>`, its name
  `#define`d to `(*hero_in_<name>)`; a direct call passes the address of the copy the caller's IR
  already made, which rule 1 of `ir/own.hero` (a plain parameter is borrowed, never released or
  written) makes the same value;
- **in place**: a construction of such a record writes each field into its destination, a fixed
  array's braced list copied from a literal of the member's own type, a struct field past
  `typeorder`'s depth by `HERO_COPY` as today (defect 522);
- **only direct calls change**: a function value, a callback handed to C and an `extern` keep C's
  convention (one typedef spells a function type for both), so a function whose direct signature
  changes is reached as a value through a twin of C's convention, `hero_value_<name>`, the
  library's own mechanism since panel 230. The twin takes its result's room off the frame and returns
  `*hero_result`, given back by `__attribute__((cleanup(hero_frame_give_at)))` after the return has
  copied it. **Measured why**: a record returned from a named local costs clang's front end in
  proportion to its leaves, 0.24, 1.11 and 8.09 billion instructions at 2, 16 and 128 MiB
  (`ce/probe/ret*.c`, the build's `-O2 -fno-strict-aliasing`), and 0.12 billion returned through a
  pointer; the route's first build returned a named local and its 128 MiB `levels.c` cost 9.45
  billion, 9.22 of them clang's front end (`-ftime-report`, a diagnosis of where, not a cost read).
  The same probes found an assignment through pointers costs nothing (0.12 billion at 128 MiB), and
  one between two named objects 16.05 billion: the route assigns large values only through pointers.

**Every shape, built by the route compiler itself** (`ce/judge.sh`: the units from a `-O0` build,
each measured alone at `-O2` under the guard; then `heroes build -O2` and `-O0` and each binary run
under `timeout 120`, its output read whole):

| program | frozen, main unit | route, main unit | route, other unit | `-O2` run | `-O0` run |
|---|---|---|---|---|---|
| bigframe16 (1 MiB) | 66,181,586,608 / 2.78 GB | **412,968,857 / 11.4 MB** | | `before`, `1` | `before`, `1` (today: stack exhausted) |
| bigframe18 (4 MiB) | 314,399,003,206 / 12.31 GB | **438,942,805 / 11.6 MB** | | `before`, `1` | `before`, `1` (today: stack exhausted) |
| onefn5 (256 KiB) | 5,223,700,609 / 163 MB | 3,641,703,557 / 63 MB | | `before`, `1` | `before`, `1` |
| onefn6 (2 MiB) | 40,647,266,942 / 1.21 GB | 3,669,329,169 / 63 MB | | `before`, `1` | `before`, `1` |
| onefn7 (16 MiB) | (not run: about 9 GB by ratio) | 3,688,003,669 / 63 MB | | `before`, `1` | `before`, `1` |
| onefn8 (128 MiB) | (not run) | 3,711,094,226 / 64 MB | | `before`, `1` | `before`, `1` |
| byval5 | 82,904,763,110 / 1.41 GB | 3,644,555,166 / 63 MB | 177,761,336 | `before`, `1` | `before`, `1` |
| byval6 | past 600 s | 3,663,838,711 / 63 MB | 179,674,286 | `before`, `1` | `before`, `1` |
| byval7 | (not run) | 3,687,546,151 / 63 MB | 181,169,137 | `before`, `1` | `before`, `1` |
| byval8 (128 MiB) | (not run) | 3,713,224,423 / 64 MB | 182,696,191 | `before`, `1` | `before`, `1` |
| levels1_5, `levels.c` | 283,432,766 | 277,850,347 / 10 MB | 197,772,030 | `before`, `1` | `before`, `1` |
| levels1_6, `levels.c` | 571,426,370 | 299,804,127 / 10 MB | 202,970,592 | `before`, `1` | `before`, `1` |
| levels1_7, `levels.c` | (not run) | 322,497,128 / 10 MB | 209,128,386 | `before`, `1` | `before`, `1` |
| levels1_8, `levels.c` (128 MiB) | (not run) | 345,536,751 / 10 MB | 215,696,014 | `before`, `1` | `before`, `1` |
| levels8_5, `levels.c` | 106,642,238,306 / 1.45 GB | 4,963,786,799 / 67 MB | 185,798,718 | `before`, `1` | `before`, `1` |
| levels8_6, `levels.c` | 1,908,194,029,234 / 10.41 GB | 4,984,484,492 / 66 MB | 187,112,872 | `before`, `1` | `before`, `1` |
| levels8_7, `levels.c` | (not run) | 5,010,225,149 / 67 MB | 188,800,073 | `before`, `1` | `before`, `1` |
| levels8_8, `levels.c` (128 MiB) | (not run) | 5,028,968,768 / 67 MB | 190,048,452 | `before`, `1` | `before`, `1` |

(Instructions retired / peak footprint; the frozen rows are § 1 and § 2's. "Not run" rows were not
compiled with the frozen compiler because of their memory, by the critic's ratio of x7.6 per x8 bytes
an inference.) **The cost no longer depends on the record's size**: from 256 KiB to 128 MiB each
shape moves under 2 %, and what is left is the cost of the records still on the frame below 4,096
bytes. **Every program runs at both levels**, the filing's 128 MiB record in each of its shapes
included, where today `-O2` does not finish and `-O0` aborts `stack exhausted`.

## 6. The routes beside it, measured on the frames they leave

| route | k | instructions retired | largest `-O2` frames | `-O2` run |
|---|---|---:|---|---|
| today | 16 | 66,181,586,608 | (folded) | `before`, `1` |
| today | 18 | 314,399,003,206 | (folded) | `before`, `1` |
| R6, `noinline` on every `make` (§ 3) | 16 | 709,396,954 | `make16` 1.5 MiB, `main` 1 MiB | `before`, `1` |
| R6 | 18 | 1,881,302,410 | `make18` 6 MiB, `main` 4 MiB, `make17` 3 MiB | **stack exhausted in `make18`** |
| R2, `__attribute__((optnone, noinline))` on every `make` and `main` (`ce/units/r2hand/`) | 16 | 692,019,186 | `make16` 3 MiB, `main` 3 MiB, `make15` 1.5 MiB | **stack exhausted in `make14`**, 134 |
| R2 | 18 | 2,015,014,405 | `make18` 12 MiB, `main` 12 MiB, `make17` 6 MiB | **stack exhausted in `main`**, 134 |
| off-frame alone (§ 4, by hand) | 18 | 562,541,945 | `make18` 4 MiB (the calls' hidden temporaries) | **stack exhausted in `make14`**, 134 |
| **the route (§ 5, compiled)** | 16 | **412,968,857** | (none past 147 KiB, § 4) | **`before`, `1`**, and at `-O0` |
| **the route** | 18 | **438,942,805** | | **`before`, `1`**, and at `-O0` |

Every route that stops clang's work by leaving the values where the frame holds them (R6, R2, and R1,
whose escape pins every temporary as R2 does: an inference from R2's frames, R1 not built here) turns
today's slow but correct `-O2` run into the stack abort, because today's run prints `1` only by
clang folding the whole record into the one leaf read. Only a route that moves the values off the
frame AND writes them in place keeps every run, and it is also the only one that answers the
filing's one-function shape, which no inlining route reaches (the critic's E6: 42.3 billion with
inlining or without).

**What the route costs at run time**, `/usr/bin/time -l` on the binaries (the whole process, the
runtime's start included):

| binary | instructions retired | maximum resident |
|---|---:|---:|
| bigframe16 `-O2`, today | 17,852,466 | 1,785,856 |
| bigframe16 `-O2`, route | 24,442,073 | 8,224,768 |
| bigframe18 `-O2`, route | 50,222,503 | 29,425,664 |
| onefn8 `-O2` / `-O0`, route | 317,092,359 / 314,701,851 | 462,749,696 |
| byval8 `-O2` / `-O0`, route | 403,608,596 / 405,687,623 | 597,000,192 |
| levels1_8 `-O2` / `-O0`, route | 428,112,848 / 425,401,125 | 615,989,248 |
| levels8_8 `-O2` / `-O0`, route | 562,012,046 / 1,362,493,728 | 596,672,512 |

Today's k = 16 binary costs nothing because clang computed the answer at compile time; the route's
builds the record, 6.6 million instructions and 6.4 MB more. The 128 MiB programs hold the record
and every large temporary the IR keeps for the function's life, as the frame held them at `-O0`:
463 to 616 MB of the heap, where the frame could hold none. **A program with no value past 4,096
bytes is emitted byte for byte as today** (the rule is keyed on the size alone; § 7 counts it), so it
pays nothing.

**The shape beside the reproducer that soundness turns on: a large record of counted values**
(`ce/shapes/counted/`): `S0` holds a `str` and an `i64`, `S_k` eight `S_(k-1)`, so `S3` holds 512
strings, about 12 KiB; it is built from copies, handed by value to another module's `grow`, copied
into a cell there, one string changed and the copy returned (`retag`), and three leaves printed. Its
route units take rooms (`grep -c hero_frame_take`: `held.c` 4, `counted.c` 1), so zeroing, the
field-by-field construction, the parameter's pointer, the caller's slot and the releases all carry
counted values. Frozen and route print the same, `leaf`, `new!`, `14`, exit 0, at `-O0` and `-O2`;
the emitted `main` calls `hero_runtime_check_leaks()` (`grep -n`), so exit 0 says no block leaked;
and the route's `--sanitize` builds (ASan and UBSan) of this program and of `bigframe16` print the
same, exit 0.

## 7. What the route changes in the tree, and what it opens or closes

**Emissions, counted with the gate's own invocation** (CL-074): `ce/heroes-route run
tests/harness/main.hero -- ce/heroes-route emission`, output read whole (`ce/emission-route.txt`):
`emission: 1512 passed, 708 failed`, 4 `SKIP` (`sys/prctl.h`, absent on this Mac). **Every one of
the 708 differs first at its `hero_program_sources` line** (a script over the suite's report), the
key of the sources a program was built from, which covers the runtime it links (panel 222's R4), and
none changed its line count (708 counts read, 0 differ); that line stands beside the C `main`, after
every Heroes function. **So the route moves no blessed emission's code: no program of
`tests/emission`, `tests/golden/{emit,fixedbugs,ir,run}` or `examples` holds a value past 4,096
bytes.** Any change to `runtime/` moves those 708 keys, the route's or any other (the batch's
`b80265fe6` re-blessed them for its runtime change), so landing it re-blesses 708 files by their key
line alone. The corpus therefore tests none of the route: its cases must be added (§ 8).

**The compiler's own tests**, `ce/heroes-route test selfhost/main.hero`: 1,792 tests, 1 failed on the
route's first build, `emit/macro_guard.hero`'s *the list holds every word the runtime's headers
write*, which named the three new runtime functions missing from `emit/guarded_names.hero` (a group's
header may not `#define` a word the runtime's header writes). Added; the final build's run is below.

**Robustness (design.md §1.12), class by class**:

- **Closed, the frame face of large values**: no value of the program's own past 4,096 bytes is
  ever a local of a C frame, so defect 2253's frame larger than the stack cannot be built from the
  program's own records (it still can from a group's record, whose struct is the header's: 2253's
  and 2277's cases hold the header's `Held`, which the route leaves on the frame, so their aborts and
  goldens stay; their emissions moved by the key line alone). Today's `-O0` aborts of the reproducer
  at k = 16 and 18 become runs.
- **Closed, the compile cost**: bounded by the threshold, not the record (§ 5).
- **Opened and bounded, a recursion of large values**: such a recursion took the stack and ended
  `stack exhausted`; it now takes the room, which is bounded at one gibibyte a thread and aborts with
  the guard's own words, `panic: stack exhausted in <module.function>` on POSIX (the function by
  `dladdr` on the prologue's return address), `panic: stack exhausted` on Windows as its guard says.
  Measured (`ce/shapes/recur/`, a recursion copying a 32 KiB record at every level): 100 deep, today
  `-O0` `stack exhausted in recur100.down` and `-O2` `101`, the route `101` at both levels; 20,000 deep,
  today and the route `stack exhausted in recur20000.down`, 134, at both levels (the route with 182 to
  303 MB of room in use when the stack's guard fired); 100,000 deep, the route `stack exhausted in
  recur100000.down` at both levels, `-O2` peaking at 1,072,816 KB resident, the room's bound firing.
  Without the bound that run would have asked about 6.4 GB of the heap (100,000 levels of two 32 KiB
  rooms, arithmetic), and on Linux the OOM killer ends a process with nothing said. **The price**: a
  runaway recursion of large values now holds up to a gibibyte of heap before it stops, where the
  stack held 8 MiB; the number is the sitting's to set.
- **Kept, an order the compiler owes**: a give that is not the newest take stops with `internal
  error: a large value's room was given back out of order (compiler bug)`, never a lost value. A C
  library that `longjmp`s across Heroes frames would skip their gives and meet that stop at the next
  give; it already skips their releases and lend regions (the C boundary's, unrun here).
- **Kept, the leak gate and the sanitizers** (§ 5's counted record): chunks come from
  `hero_malloc_raw`, which neither counter weighs, and a spawned thread's chunks go back with it
  (`spawn.c` calls `hero_frame_thread_leave` beside `hero_lend_thread_leave`).
- **A debugger's view moves**: an author's large binding shows as its pointer, `hero_big_h0_r`,
  dereferenced to read it; a temporary stays hidden. None of the 15 programs of `tests/golden/lldb/`
  holds such a value (`ce/heroes-route build <program> --emit-c`, `grep -c 'hero_frame_take(sizeof'`
  0 for each), so no pinned session moves; a session over a large value is unrun.

## 8. The final build, and the answers

**The final build**, after § 7's three corrections (the runtime words added to
`emit/guarded_names.hero` and both guard headers, the room's bound, `is_large` moved out of
`emit/ctype.hero`): `ce/heroes-route`, `sources 588572956d7a94d0`. Its fixpoint holds (stage 2 and
stage 3 emit the compiler as the same 28,994,161 bytes, `cmp`); the compiler's own tests read
`1792 tests, all passed`; `layout` reads `6 passed, 0 failed` once `emit/ctype.hero`'s decided
ceiling moves 400 to 404 (one field, its comment, the empty map and the argument, with a dated
reason beside panel 194's for the same file, `tests/harness/suite_layout.hero`). Judged again on the
final compiler (`ce/final-all.txt`): bigframe16 412,679,877 / 11.4 MB, bigframe18 438,273,884 /
11.8 MB, onefn6 3,666,658,794, byval6 3,664,594,802, levels1_6 300,148,296, levels8_6 4,985,217,008,
onefn8 3,713,122,533, byval8 3,710,783,274, levels1_8 345,683,329, levels8_8 5,031,881,133 (each
under 67 MB), every one printing `before`, `1` at `-O2` and `-O0`; the counted record `leaf`, `new!`,
`14` at both, its `held.c` 7,180,918,378 against today's 7,653,355,450 (the cost there is its
per-type walks of 512 strings, not the frame).

**Q1, compiler-need.** Not Principle 0's: the compiler's own C holds no large value (the route's names
appear in it only as the emitter's string literals), so self-hosting neither needs nor feels it. It
**is** CLAUDE.md § 13's: a legal program of twenty lines cannot be run by `heroes run` (a 2 MiB
record built eight calls to a level costs clang 1.9 trillion instructions and 10.4 GB, one handed by
value did not finish in 600 s, the filing's 128 MiB ones were never asked of today's compiler for
their memory), and its `-O0` binary aborts. A refusal (R4) would be the wrong answer, not only a
full panel's: the route makes every such program compile in a bounded cost and run at both levels,
and the spec states no size limit for a record (`spec/heroes-spec.md:81`, by value) while it does
promise that running out of stack aborts (`:289-290`), which the room's bound keeps.

**Q2, the route: large values off the frame, written in place** (§ 5): one threshold, four parts
(the room, the caller's slot, the caller's copy, construction in place), direct calls only, a twin of
C's convention for a function's address, and a bounded room that aborts as the stack does. In the
lane: no surface, no diagnostic, no spec token (a record is still held by value; where a local's
bytes live is not a language fact), and R3's heap representation is not this: nothing is shared or
copied on write. Every alternative measured here or by the critic fails one of the two faces: R6 and
R2 turn today's correct `-O2` run of k = 18 into the stack abort (§ 6), the room alone does the same
(§ 4), and no inlining route reaches the one-function shape (the critic's E6).

**Q3, the threshold: bytes as the route counts them** (fields summed, no padding, a variant's widest
case and its tag; the same number on every target), **past 4,096.** The measured curve (§ 4): a 2 MiB
record built in one function cost 73.5 billion instructions with the line at 64 KiB, 5.4 billion at
4,096 and 0.86 billion at 512, whatever the whole record's size; 4,096 keeps the cost of a value
just under the line to a few billion while leaving every record of the tree alone. **Nearest in the
tree**: above, none (the emission census, § 7); below, the largest record a corpus program declares is
about 98 bytes (`examples/interpreter`'s `Request`, `h_runvalue_Request`, estimated from the blessed C
typedefs of `tests/emission/` by the route's summing rule, variants left out), forty times under the
line; the compiler's own, none past it. The nearest programs on each side are this sitting's: E3
(4,096 bytes, stays) and E4 (32 KiB, moves); R8 (4 KiB, stays) and R9 (8 KiB, moves). **The unit's
limit, said**: a record of 4,096 one-byte fields stays under the line with 4,096 leaves, eight times
E3's 512, so its cascade is bounded by the threshold and not by the record but costs more than the
i64 shapes; a count of leaves beside the bytes would bound that too, and was not built.

**Cost in lines** (`wc -l`, `git diff --stat`): new, `selfhost/emit/large_value.hero` 308 (164 of
code before its test) and `runtime/parts/frame.c` 143 (83 not comments); changed, 19 files, +243
-48: `emit/signature.hero` 84, `emit/prologue.hero` 49, `emit/construct.hero` 30, `emit/body.hero`
20, `emit/inst.hero` 18, `emit/term.hero` 18, `emit/func_ref.hero` 12, `emit/callback_thunk.hero` 8,
`emit/ctype.hero` 6, `emit.hero` 6, `emit/unit.hero` 4, `emit/members.hero` 3,
`emit/guarded_names.hero` 1, `runtime/heroes_runtime.h` 6, `runtime/parts/spawn.c` 5,
`runtime/runtime.c` 5, the two guard headers 3 each, `tests/harness/suite_layout.hero` 10. Where it
lands: the emitter and the runtime only, no lexer, parser, checker, IR or ownership line; it is
neither core nor sugar but a lowering, and no construct the checker must handle is added. What the
landing still owes and this seat did not build: run cases of the filing's four shapes and the
reproducer (at 2 MiB and 128 MiB), the counted record and the recursion, annotated, in `tests/golden/run/`;
the 708 emissions re-blessed by their key line; the seed refreshed only if the two-stage build asks;
and the platform legs, where Linux's `-fstack-clash-protection` and Windows' clang are unrun here.

**Prediction.** At the `m-latency-ceiling` tag, with the route landed and the filing's four shapes
and the reproducer at 128 MiB and 4 MiB committed as `run` cases: each case's unit compiles at `-O2`
in under 8 billion instructions and under 100 MB peak footprint on this Mac's Apple clang 21
(`/usr/bin/time -l clang -fintegrated-cc1` with the build's words, § 0), every case prints `before`,
`1` at `-O0` and `-O2` on the CI's four legs, and the `emission` suite's re-bless moves each of the
708 blessed files at its `hero_program_sources` line and at no other. Falsified by any case past
8 billion or 100 MB here, any leg's run that aborts, or any blessed file moved elsewhere.

**Condition under which this verdict is wrong.** (1) A program the project needs, `selfhost/` or an
`examples/` program, comes to hold a value past 4,096 bytes on a hot path and the room's traffic or
the lost register promotion costs it past the latency ledger's floor of effect (`latency.hero
compare`); then the threshold moves, or the route is wrong for that program. (2) A CI leg's clang,
unrun here, prices the same C differently (Linux under `-fstack-clash-protection`, Windows' MSVC
target with `__attribute__((cleanup))` and `_Thread_local`), so that a case passes 8 billion or a run
aborts. (3) The full panel reads spec § 3's *by value* as fixing where a local's bytes live, which
would take the route out of this lane. (4) A shape is found whose cost below the line still grows
with its record, which would make the unit leaves rather than bytes.

## 9. The critic's repairs, built

Asked by the coordinator after the critic's second pass (`critic-reports.md`, read whole first); the
critic's programs copied from `.claude/worktrees/233-critic/critic/` into this worktree's
`ce/critic2/`, never edited there. The repaired compiler: `ce/heroes-route`, `sources
2141412850b671c8`, stage 1 from the frozen seed.

**(1) A large record through a function value (critic item 6): built, measured.** A function type
that names a large program record now takes the direct convention, its typedef `typedef void
(*h_0fn_20cc271d)(h_viaval7_E7 *, int64_t);` (`emit/types.hero`'s `function_type`), and an indirect
call passes its arguments the same way (`large_value.direct_arguments` and `direct_target` now take
`.indirect` with `.heroes_fn`). No wrapper: the twin is the library's alone again (panel 230), its
header in the same convention as the typedef so its address still assigns to a value of the type;
`func_ref.hero` and `unit.hero` are back to the frozen text. **The premise, verified with this
compiler**: `ce/heroes-route check` refuses both of the critic's callback probes,
`error[ffi_type]: (function(E4) -> i64) cannot cross the FFI boundary ... E4 inside it cannot
(§4.19)` and the same for the 8-byte `E0`: any program record inside an `extern`'s callback type,
large or small, so such a typedef never reaches C. **Measured** (`ce/judge.sh`, the unit at `-O2`
and the runs): `viaval7` (16 MiB returned through `pick() -> (function(i64) -> E7)`) **4,306,552,144
instructions / 65 MB, `before`, `5` at `-O2` and at `-O0`**, where the route of § 8 aborted `stack
exhausted in viaval7.go` at both; `direct7` 4,303,508,624, `before`, `5` at both; the critic's
`valsplit` (a 32 KiB `E4` returned and taken through values) `10 2010 14` at both levels, its two
typedefs `void (*)(h_valsplit_E4 *, int64_t)` and `int64_t (*)(const h_valsplit_E4 *)`;
`grep -c hero_value_` in `viaval7.c`: 0.

**(2) A program record holding a `partial` group record (critic item 2): built, measured, with a
cost the sitting must weigh.** The size table now records which declarations are, or hold by value, a
group's `partial` record (`Sizes.opaque`), and a type of the program's own that does is counted
large whatever its named fields sum to: a rule about the program, never the target. That alone left
`grpwrap` aborting at `-O0` in `make_wrap`, because the construction `Wrap(tag: n, inner: Held(first:
n + 1, rest: zero))` builds the 128 MiB `Held` as a temporary first. So the one table now carries two
values: `true` for the program's large types (off the frame and passed in place) and `false` for a
group record that is or holds a `partial` one (off the frame alone, its C convention the header's:
an `extern` still takes it by value); `is_large` reads `true`, the new `off_frame` either; and a
`rest: zero` construction into a target off the frame is written whole by the memset alone, as past
`typeorder`'s depth (`emit/zeroed.hero`, `in_room`), since `(struct heroes_held){0}` would be a
128 MiB literal of the frame. **Measured**: `grpwrap` **`before`, `13` at `-O0` and at `-O2`**
(today and § 8: `stack exhausted in grpwrap.go` at `-O0`), its `-O0` frames 112 bytes (`make_wrap`)
and 80 (`go`), the `Held` temporary `struct heroes_held *const hero_big_t5 = (struct heroes_held
*)hero_frame_take(...)`; no `extern`'s declaration or call text changes. Every `run` case that holds
a `partial` record passes with today's expected output (`ce/heroes-route run tests/harness/main.hero
-- ce/heroes-route run <name>`): 2253's three (they still abort `stack exhausted` where they hand the
record to C by value, which C's ABI copies onto the frame, as the cases mean), 2277's one,
`rest-zero`'s eight, and five other FFI cases, one each. **What it leaves**: `grpret`, a Heroes
function returning the `partial` record itself, still aborts `stack exhausted in grpret.go` at both
levels (today: `go` at `-O0`, `main` at `-O2`): a group record keeps C's by-value convention, since a
function type that names one may stand in a callback. **What it costs, measured**: the `lldb` suite
reads `16 passed, 1 failed`, the failure `formatter-steps-aside`, whose `Holder` is a program record
holding a `partial` `statvfs` (about a hundred bytes in fact): `holder` now lives off the frame, and
`frame variable h5_holder.vfs.f_bsize h5_holder.label` answers `error: use of undeclared identifier
'h5_holder'`. So the rule costs the debugger view, and the room's traffic, of every binding of a
program record that wraps a `partial` header struct, a common FFI shape, small or not. The two
choices, both robust (neither crashes nor corrupts): adopt it and re-pin that session (the binding
read through `*hero_big_h5_holder`), or count a `partial` record by its named fields as before and
leave `Wrap` with today's named `-O0` abort. I recommend the first, robustness of the run ranking above
the debugger's convenience, with a follow-up that teaches `editors/lldb/heroes_lldb.py` to show a
binding through its room's pointer (unbuilt here).

**(3) The room's bound, per process and told truly (critic item 3): built, measured.** One gibibyte
for every thread together, `static _Atomic size_t hero_frame_held`, taken by `atomic_fetch_add` and
rolled back past the bound, given back by `atomic_fetch_sub`. The message keeps the guard's opening
words and names the room: **`panic: stack exhausted in <module.function>: the room large values take
off the stack is full, 1 GiB for the whole program`** (Windows: the first sentence and the room, as
its guard names no function); one thread tells it, a second that runs out meanwhile waits for the
process to end, so two messages never interleave. **Measured**, `-O2`, `/usr/bin/time -l` over
`ce/rg.sh` and the binary it waits on, maximum resident:

| program | today (critic) | § 8's route (critic) | repaired route |
|---|---|---|---|
| one (main, 100,000 deep) | stack exhausted, 10,223,616 | stack exhausted, 1,098,629,120 | the room's message, 134, 1,098,645,504 |
| three (two spawned and main, 100,000 deep each) | stack exhausted, 27,066,368 | stack exhausted, **3,289,038,848** | the room's message, 134, **1,098,727,424** |
| four_bounded (three spawned and main, 6,000 deep each) | stack exhausted | **runs, 2,402,844,672** | **the room's message, 134, 1,098,776,576** |

So a program of N threads now holds at most a gibibyte of room together, and `four_bounded`, which
aborts today on the stack, aborts again, on the room, at about 4,096 levels a thread instead of 6,000
(arithmetic: 1 GiB over 64 KiB a level is 16,384 levels for the four together), with words that say which resource ran out.

**(4) The checker's rule pinned, the thunk's comment corrected: built.** `large_value.hero`'s first
test checks a program whose `extern` takes `(function(E0) -> i64)` and asserts `error[ffi_type]`, with
a comment naming what depends on it (`types.function_type`'s convention and `callback_thunk.header`'s);
`callback_thunk.hero`'s comment above `header` now says the thunk keeps C's convention, spelled by
`signature.one_param` and `ctype.c_result`, which is the function's own signature for every function C
can be handed, and why a converted function is never thunked. The compiler's own tests: **1,793
tests, all passed** (the new test among them).

**(5) The run cost per construction (critic item 4): measured.** The critic's `loop8k` (an 8 KiB
record, two `E3`, built 2,000 times) and `loop32k` (a 32 KiB `E4`, the same), both compilers at
`-O2`, five runs each, instructions retired of the whole process:

| program | today (five runs) | route (five runs) | route minus today, medians | per construction |
|---|---|---|---:|---:|
| loop8k | 17,127,706 to 17,151,978 | 24,476,397 to 24,582,967 | 7,369,561 | about 3,700 |
| loop32k | 17,128,163 to 18,451,910 | 71,071,631 to 71,151,602 | 53,959,542 | about 27,000 |
| bigframe16, built once (§ 6) | 17,852,466 | 24,442,073 | 6,589,607 | (1 MiB once) |

Both print the same totals (`4000000`, `3998000`). Today's `-O2` builds neither record (clang folds
the loop to the two leaves read), so the route's price is the copies it actually makes: about 0.45
instructions a byte at 8 KiB and 0.8 at 32 KiB, per construction. These programs compile easily today
(the critic: 8.36 and 8.49 billion, against 5.63 and 5.64 under the route), so between the line and
the size where today's clang chokes a program pays this per construction for a compile it did not
need; a higher line would spare them and cost more compile (§ 4: 73 to 103 billion at a 64 KiB line).
This is the price the threshold sets, stated beside depth 16's, which pays it once.

**The repaired route, judged again** (`ce/r9-all.txt`, unit at `-O2` / peak; every run `before`, `1`
at `-O2` and `-O0`): bigframe16 380,278,582 / 10.9 MB, bigframe18 398,162,158 / 11.3 MB (below § 8's,
the twins gone), onefn6 3,666,877,045, byval6 3,668,657,489, levels8_6 4,971,396,498, onefn8
3,712,171,125, byval8 3,713,244,922, levels1_8 320,211,653, levels8_8 5,014,270,879 (each under 67
MB); the counted record `leaf`, `new!`, `14` at both levels, its `held.c` 7,176,343,177 / 105.6 MB.
**Its fixpoint holds** (stage 2 and stage 3 emit the compiler as the same 29,009,622 bytes, `cmp`; the
route's names in it are the emitter's string literals alone), **the compiler's own tests read 1,793
passed**, **`layout` 6 passed, 0 failed.**

**Lines** (`wc -l`, `git diff --stat`): new, `selfhost/emit/large_value.hero` 385 (190 of code before
its tests) and `runtime/parts/frame.c` 156 (88 not comments); changed, 19 files, +276 -41:
`signature.hero` 61, `zeroed.hero` 59 (four lines and the formatter splitting three calls),
`prologue.hero` 49, `construct.hero` 33, `body.hero` 20, `types.hero` 19, `term.hero` 18,
`callback_thunk.hero` 9 (its comment), `ctype.hero` 6, `inst.hero` 5, `members.hero` 3, `emit.hero`
2, `guarded_names.hero` 1, the runtime's header 6, `spawn.c` 5, `runtime.c` 5, the guard headers 3
each, `suite_layout.hero` 10. `func_ref.hero` and `unit.hero` no longer change.

**Emissions, counted again with the gate's own invocation** (`ce/heroes-route run
tests/harness/main.hero -- ce/heroes-route emission`, `ce/emission-route2.txt`): `emission: 1512
passed, 708 failed`; **682 differ at their key line alone, 26 beyond it** (and those 26 change their
line count), every one a program that holds a group's `partial` record, moved by repair 2's rule:
`fixedbugs/a-header-field-that-is-not-const`, `run/ffi-a-filled-record-is-asked-for`,
`run/ffi-a-lent-field-counted-by-a-constant`, `run/ffi-a-record-holding-a-lock-is-lent-in-place`,
`run/ffi-a-typedef-the-note-names-binds-by-its-own-name`, `run/ffi-rest-zero-a-partial-record`,
`run/ffi-the-bits-beside-a-bit-field-are-zero`, two of `run/fixedbugs-094-*`,
`run/fixedbugs-156-records-beside-bit-fields-build`, `run/fixedbugs-173-*`, two of
`run/fixedbugs-2062-*`, three of `run/fixedbugs-2253-*`, `run/fixedbugs-2277-*`, three of
`run/fixedbugs-396-*` (SHA-256's state), two of `run/fixedbugs-413-*` (libuv, zlib),
`run/fixedbugs-a-tag-names-a-handle-and-a-record`, two of `run/lent-a-record-holding-*`, and
`examples/ctime/main`, whose `made` returns `tm?`, a type of the program's own holding a `partial`
record, now through `hero_out`. **Every one of those that runs passes the `run` form with its
expected output** (`run <name>`: 094 8, 156 1, 173 2, 2062 3, 396 15, 413 6, the tag case 1, the two
lent cases 2, 2253 3, 2277 1, `rest-zero` 8 and the others 1 each), and `examples/ctime` prints the
same under both compilers (`diff` of the two runs). The structs these hold are mostly small in fact
(`passwd`, `statvfs`, `regex_t`, a SHA-256 state): the rule cannot know a `partial` record's size,
which is the point of it and its price. **Determinism**: four programs that use the route
(`bigframe12`, `grpwrap`, `valsplit`, the counted record), each emitted twice with `--emit-c`: the
same bytes (`cmp`).

**What changes in this seat's answers.**

- **The verdict stands**, compiler-need and the route alike, the route now with all five repairs. Two
  things are said that § 8 did not say: **the route does not close defect 2278 for chains of GROUP
  records** (the ffi seat's array chain at 1 MiB, 446.9 billion instructions and 6.98 GB, which the
  critic re-ran under the route unchanged): that half wants a carve-out filed as its own defect or
  `noinline` on functions that build or return a large group record beside this route (the critic's
  route 3, unbuilt here); and **repair 2 has a price** (26 emissions, one `lldb` session, the room's
  traffic for every wrapper of a `partial` header struct), against the alternative of leaving `Wrap`
  with today's named `-O0` abort.
- **The prediction changes in one clause and gains two**: the filing's four shapes at 128 MiB and the
  reproducer at 4 MiB still compile under 8 billion instructions and 100 MB each (measured: at most
  5,014,270,879 and 66.7 MB); **`viaval7` and `grpwrap`, committed as `run` cases, print their answers
  at both levels on the CI's four legs**; and the `emission` re-bless moves the 708 key lines and,
  beyond them, **exactly the 26 programs listed above and no other** (§ 8 said none: repair 2 made it
  26); the `lldb` suite reads all passed once `formatter-steps-aside` is re-pinned. The counted record,
  which the landing owes as a case, peaks at 105.6 MB: its line is under 8 billion and under 120 MB,
  stated apart rather than folded into the 100 MB line it would cross.
- **The condition gains two clauses**: (5) if the sitting judges repair 2's price (the debugger view
  and room traffic of every wrapper of a `partial` header struct) worse than `Wrap`'s named `-O0`
  abort, the narrower rule (a `partial` record counted by its named fields) is the right one, and the
  route's claim must then exclude a program record that holds one; (6) if the sitting reads defect
  2278 as including chains of group records, this route alone does not close it.
