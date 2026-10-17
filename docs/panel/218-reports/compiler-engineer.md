# Panel 218, the compiler-engineer's report

Written 2026-10-17 from 03:20, in a detached worktree of `cbf724983` at
`.claude/worktrees/218-compiler-engineer` (`git log -1 --format=%H` there printed
`cbf7249835060127ec9b45d805dce08d7be31bdf`), `build/` removed, the compiler built there from the
seed. Measurements in `<scratchpad>/218-ce/`. Every number names its command; what could not be
run says *unrun*. Written as I go: where a later command contradicts an earlier sentence, the
correction stands beneath it with its time.

## Setup

- `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes` under `/usr/bin/time -p`:
  `real 7.99 user 7.86 sys 0.13`, load `2.30 1.76 2.10` (`sysctl -n vm.loadavg`). Apple clang
  version 21.0.0 (clang-2100.3.34.2).

## The class on the trunk's compiler, 35 shapes (this Mac, `heroes build`, `-O0`, three runs)

`<scratchpad>/218-ce/sh/f.h`, one program per name (`mk.sh`, `extern "f.h"`, `constant NAME: T`,
`print`), the trunk's compiler built from the seed above (`trunk.txt`):

- **16 undefined, all build at exit 0, each stable on three runs**: enumerators `E_SHIFT` (`1 << 31`),
  `E_DIV` (`INT_MIN / -1`), `E_ADD` (`INT_MAX + 1`), `E_FLT` (`(int)1e20`), `E_NEG` (`-INT_MIN`),
  `E_MUL` (`65536 * 65536`, prints `0`); `static const` objects `S_DIV`, `S_SHIFT`, `S_FLT`,
  `S_FLTIMP` (`= 1e20`, no cast), `S_ADD`, `S_BRIEF` (the brief's, prints `1`), `S_LLDIV` (`i64`);
  records `R_BRACE` (`#define R_BRACE {INT_MIN / -1, 2}`), `R_OBJ` (a `static const struct pt` of the
  same list), `R_OBJF` (`{(int)1e20, 3}`).
- **18 defined (or implementation-defined) values, all build and print what C gives**: `E_UNEG`
  (`-1u`, 4294967295 as `u32`), `E_UMAX` (`UINT_MAX`), `E_B` (`E_A * 2`, 10), `E_SIZE` (`sizeof`, 8),
  `E_OFF` (`offsetof`, 4), `E_30` (`1 << 30`), `E_CHAR` (`'\377'`, -1), `E_COMMA` (`(1, 2)`), `E_FDIV`
  (`(int)(1000.0 / 60)`, 16), `E_HEX` (`0x80000000`, 2147483648 as `u32`), `S_OK`, `S_FROME`
  (`E_B + 1`), `S_WRAP` (`0u - 1`), `S_IMPL` (`(int)3000000000u`, -1294967296), `S_FOK`
  (`= 1000.0 / 60`), `S_NEGOK` (`-2147483647 - 1`), `R_OK`, `R_BRACE_OK`.
- `D_SHIFT` (`#define D_SHIFT (1 << 31)`): exit 1, `ffi_constant_type`, *signed shift result
  (0x80000000) sets the sign bit*, the region of `emit/value_check.hero` (133 lines).

Why the region cannot see them: it opens around the accessor's `return NAME;` alone
(`emit/constant_accessor.hero:71-80`), and an enumerator or object is computed where the header
writes it, in a line no pragma of the accessor reaches; the accessor reads a folded constant (the
emitted C, `p_*/main.c`, line 165: `return E_DIV;`, `return S_DIV;`; a record's
`struct pt hero_constant_value = R_BRACE;`, an aggregate clang folds).

## What clang says at each initialiser (Apple clang 21.0.0, `ask.sh`, `-fsyntax-only` over a unit including `f.h`)

| word set | the class's 16 named | the defined 18 named |
|---|---|---|
| the build's `-Wall` | 6 warnings: `E_ADD E_NEG E_MUL S_ADD S_BRIEF` (`-Winteger-overflow`), `S_FLTIMP` (`-Wliteral-conversion`) | `E_COMMA` (`-Wunused-value`) |
| `-Wall -Wextra` | the same 6 | the same |
| `+` the region's six | 8: the 6 and `E_SHIFT S_SHIFT` (`-Wshift-sign-overflow`) | the same |
| `+ -pedantic` | 10: the 8 and `E_DIV E_FLT` (`-Wgnu-folding-constant`) | `E_COMMA E_FDIV` (`gnu-folding-constant`), `E_UNEG E_UMAX E_HEX` (`-Wc23-extensions`) |
| `-pedantic-errors` (gnu11, and `-std=c11`, identical) | **errors on 5**: `E_DIV E_ADD E_FLT E_NEG E_MUL`; **silent on `E_SHIFT`** and on every object and record | **errors on 5 defined**: `E_UNEG E_UMAX E_HEX` (*enumerator value which exceeds the range of 'int' is a C23 extension*), `E_COMMA E_FDIV` |

The build prints none of the `-Wall` six today (`trunk.txt`, exit 0, nothing said): a warning in a
header is not shown to the author. **Never seen by clang at all, under any word set: `S_DIV`,
`S_FLT`, `S_LLDIV`, `R_BRACE`, `R_OBJ`, `R_OBJF`** (6 of 16).

**C11 and a diagnostic clang must give** (`-pedantic-errors`, clang's own words): § 6.7.2.2 p2 (an
enumerator's value an ICE representable as `int`) is diagnosed for the five non-ICE enumerators,
as *expression is not an integer constant expression; folding it to a constant is a GNU extension*,
and **not** for `1 << 31`, which clang evaluates as an ICE in C. § 6.6 p4 (a constant expression's
value in range of its type), which § 6.7.9 p4 makes binding on a static object's initialiser, is
diagnosed for no object: `S_DIV`, `S_SHIFT`, `S_FLT`, `S_LLDIV` and the records pass
`-pedantic-errors` in silence. So clang's conformance mode is incomplete on the class (it misses
`E_SHIFT` and every object) and wider than it (it refuses `-1u`, `UINT_MAX` and `0x80000000`
enumerators, which C23 and GNU C define and C11 does not, and `(1, 2)` and `(int)(1000.0 / 60)`,
which GNU C folds). My reading of the standard's text, not a measurement: every one of the 16 is a
constraint violation in C11, which is why "C leaves undefined" and "C requires a diagnostic" are
the same set here and clang's diagnostics are not.

**The note that separates them** (`clang -pedantic`, the lines under each warning): the five
undefined enumerators carry *note: value 2147483648 (1.0E+20, 4294967296) is outside the range of
representable values of type 'int'*; `E_COMMA` and `E_FDIV` carry no note. A route could read that
note; it would be reading clang's English, not a flag name.

## R1 alone, and route C on R1 (this Mac, Apple clang 21.0.0, `-O0` and `-O2`)

R1 as panel 216's coordinator ran it on Windows (`<scratchpad>/win216/ux_trap.sh`), ported to this
Mac: the program's own unit (`heroes build --emit-c`), preprocessed first under the program's words
with `-Dmain=hero_probe_hidden_main`, compiled `-x cpp-output` with the four checks in trap mode,
linked with the runtime alone, its accessor called. **Route C** on the same preprocessed text: one
`clang -x cpp-output <words> -fsyntax-only -Xclang -ast-dump=json -Xclang -ast-dump-filter=<NAME>`
(and `=<accessor>` for a macro's brace list), the bound `EnumConstantDecl` or `VarDecl` found by
name, its initialiser's text sliced out of the `.i` by the dump's `offset` (measured: the raw offset
in the `.i`, `"file": "u.i"`, `"presumedFile": "./f.h"`), and one function per value appended,
`__typeof__(NAME) hero_init_NAME_0(void) { return (__typeof__(NAME))(<text>); }`, or per arithmetic
leaf of a brace list `int hero_init_R_BRACE_0(void) { return (int)((-2147483647 -1) / -1); }`; the
probe calls them. Scripts `<scratchpad>/218-ce/tools/route.sh`, `initc.py` (a prototype reader in
Python, standing for the Heroes one; its cost is priced below, not built).

```
                        R1 alone           R1 + route C
16 undefined, -O0       0 trapped          16 trapped (exit 133, SIGTRAP)
16 undefined, -O2       0 trapped          16 trapped
18 defined,   -O0/-O2   18 kept            18 kept
D_SHIFT                 refused before (the region), as today
diff of the -O0 and -O2 verdict lists      nothing
```

The sliced texts read as the program's preprocessor expanded them (`p_*/init.c`): `E_B` is
`(E_A * 2)`, `E_OFF` `(__builtin_offsetof(struct pt, y))`, `S_FLTIMP` `(1e20)` cast to
`__typeof__(S_FLTIMP)`, which is where its undefined conversion happens, `R_OBJF` two leaves
`(int)((int)1e20)` and `(int)(3)`.

## FOUND: route C as the brief framed it misses every shape reached through a reference

The brief's route C reads *the* bound enumerator's or object's initialiser. Ten attack shapes
beside the class (`<scratchpad>/218-ce/atk/f.h`), all built at exit 0 on the trunk with a stable
wrong value (`atk/trunk.txt`): `M_OBJ` (`#define M_OBJ (atk_k)`, `atk_k` a `static const int` of
`INT_MIN / -1`, the critic's `I_STATIC` of panel 216), `EB` (`enum { EA = 1 << 31, EB = EA + 1 }`),
`M_EOR` (`#define M_EOR (EA | 1)`), `R_REF` (`{ EA, 2 }`), `R_DESIG` (`{ .y = 2, .x = INT_MIN / -1 }`),
`TE` (a `typedef enum`), `IN_S` (an enumerator declared inside a `struct`), `E_MACUNDEF` (`SHIFT(31)`,
`SHIFT` `#undef`'d after), `G_EXT` (`const int` with external linkage), `S_REF` (`= atk_k`, which
clang 21 accepts in C as a GNU fold); and four defined beside them, `DB` (`DA + 1`, `DA = 1 << 30`),
`M_DOK`, `R_DESIG_OK`, `M_OBJOK` (`okk + 2`).

```
route C, the bound name's own initialiser (c1-O0.txt):   5 of 10 undefined trapped
   missed in silence: M_OBJ M_EOR (a macro, no declaration of that name), EB R_REF S_REF (the
   initialiser names the folded declaration, whose value is already the wrong one)
route C2, the references followed (c2-O0.txt, c2-O2.txt): 10 of 10 trapped, 4 of 4 defined kept,
   -O0 and -O2 identical; over the 35 shapes of f.h the same 16 trapped and 18 kept as route C
```

**C2**: from the accessor's own body in the dump (`-ast-dump-filter=<accessor>`, the definition and
not the prototype, which the dump prints first), every `DeclRefExpr` whose `referencedDecl` is an
`EnumConstantDecl` or a `VarDecl` is followed: that declaration dumped by its name, found among the
dump's roots by kind and name (the `id`s are one process's addresses and differ in the next run,
measured: matching on them found nothing), its initialiser sliced and probed, and the references
of THAT text followed in turn; a local of the accessor (its `hero_constant_value`) is the
accessor's own brace list and is probed there, never followed. `EB` probes `EA + 1` and then
`1 << 31`; `S_REF` probes `atk_k` and then `(-2147483647 -1) / -1`. Clang runs per bound constant:
2 for a value with no reference or one, 3 for a chain of two (`runs=` in `c2-O0.txt`), each a
`-fsyntax-only` parse of the preprocessed unit.

## The two Linux legs (my own `docker run --rm`, the compiler built in each from the same seed)

`<scratchpad>/218-ce/bundle/run.sh`: the compiler from the seed (`time clang -I runtime seed/heroes.c
runtime/runtime.c -o heroes`), then the trunk's builds of the 35 and 14 shapes, route C2 at `-O0`
and `-O2`, and `ask.sh`; outputs in `<scratchpad>/218-ce/out-d22/` and `out-u18/`.

| | Debian clang 22.1.8 (`heroes-linux-arm64:latest`, native) | Ubuntu clang 18.1.3 (`ubuntu:24.04`, `clang python3` without recommends, 0 `libclang-rt` packages) |
|---|---|---|
| compiler from the seed | `real 17.803` | `real 26.728` |
| the trunk's 49 builds and values | `diff` against this Mac: nothing | nothing |
| C2, 35 shapes, `-O0` / `-O2` | 16 trapped, 18 kept / the same; `diff` against this Mac: nothing | the same, nothing |
| C2, 14 attack shapes | 10 trapped, 4 kept, both levels; nothing | the same, nothing |
| `ask.sh`, every word set | `diff` against this Mac's: nothing | **differs** (below) |
| the separating note | the same 5 notes | **2 notes**, `E_DIV` and `E_FLT` |

**Clang 18 under `-pedantic-errors`** refuses 2 of the class's 16 (`E_DIV`, `E_FLT`) where clang 21
and 22 refuse 5: `E_ADD`, `E_NEG` and `E_MUL` get only the `-Winteger-overflow` warning; and it names
the refusal of `-1u`, `UINT_MAX` and `0x80000000` `-Wpedantic` where 21 and 22 say
`-Wc23-extensions`. So a route that reads clang's diagnostic words at the initialiser reads three
different answers on the project's three clangs, and the floor's is the narrowest. The Windows
box's clang 23.1.1: **unrun** (I did not ask for it; R3 of panel 216 has it measured for R1's recipe
alone).

## Route B measured: clang's diagnostics at the initialiser

B's best form, the union of every word set above read at the bound declaration's line, names 10 of
the 16 on clang 21 and 22 (`E_SHIFT E_ADD E_NEG E_MUL S_SHIFT S_FLTIMP S_ADD S_BRIEF` by flag,
`E_DIV E_FLT` by `-Wgnu-folding-constant`) and **6 never, on any clang: `S_DIV S_FLT S_LLDIV R_BRACE
R_OBJ R_OBJF`**, the defect in silence. `S_BRIEF` is named only because its outer `+` overflows
after the fold; the same `INT_MIN / -1` alone (`S_DIV`) is not. To keep `E_COMMA` and `E_FDIV`, B
must read the *outside the range* note rather than the flag, and that note is on 5 enumerators on
clang 21 and 22 and on 2 on clang 18 (above). One more fact, measured: the unit's compile already
writes the `-Wall` six into `build/tu-*/warnings-*.txt` (`p_E_ADD/build/`, *overflow in expression*
at `./f.h:7:24`), and the build shows the author none of them.

## Where C2 would land, and what it costs (the live compiler, `selfhost/`)

Counted in the `layout` suite's unit (`tests/harness/suite_layout.hero:966`, `code_lines`: every
non-blank line outside a `test` block), with an `awk` of that rule:

| module | `wc -l` | code lines | what it is to C2 |
|---|---|---|---|
| `emit/value_check.hero` | 133 | 99 | the region; R1 supersedes it |
| `emit/constant_accessor.hero` | 111 | 78 | the accessor C2 starts from |
| `emit/ffi_constant_value.hero` | 208 | 145 | `told`, the message a trapped initialiser reuses |
| `emit/record_constant.hero` | 170 | 119 | a record's accessor, its `hero_constant_value` brace list |
| `emit/layout_screen.hero` | 365 | 270 | `behind_of`, `quoted_after`, `index_from`: JSON read by byte search, *the typedef's `"name"`, then its `"type"` object, which holds no other object* |
| `cli/header_types.hero` | 399 | 280 | the same, `ParmVarDecl`s by their shape |
| `cli/layout.hero`, `cli/pointee.hero` | 586, 615 | 265, 268 | the two other `-ast-dump=json` runs |
| `cli/assemble.hero` | 287 | 250 | where panel 216's F2 was wired, after the units compile |

The whole compiler: **116,707 code lines in 653 files** by that rule (`find selfhost -name '*.hero'`).
**No route here touches the lexer, parser, checker, lowering, IR or runtime**: B is a clang run and a
reader in `cli/`; C and C2 ride R1's probe in `cli/` and `emit/`. So none is core (§1.7, Part 5),
and the veto on the core does not arise. What C2 adds that the tree does not have is a **reader of
clang's JSON as a tree**: every reader today asks a flat object by byte search, and C2 must walk
`inner` arrays to a declaration's first child, take a brace list's leaves in the semantic form, read
a `DeclRefExpr`'s `referencedDecl`, and read a range's nested `begin`/`end` `offset` and `tokLen`.
**Estimated, unbuilt** (R1 is not in the lane's commits: `git log cbf724983..lane-land215` shows
defect 2108's commit and a merge): the tree reader 150 to 220 code lines, the worklist and its runs
beside R1's driver 80 to 120, the appended probe text 30 to 40, the message (the constant's line,
and the declaration whose initialiser traps at its own header line, quoted) 30 to 50, a canary 15
to 25: **about 300 to 450 code lines**, against panel 216's measured F2 of 223 for the macro half.
An estimate from the prototype's shape (`initc2.py`, 102 lines of Python) and the neighbours above,
not a count.

**Corrected 04:50, by `wc -l tools/initc2.py`**: the prototype reader is **92** lines of Python, not 102.

**Corrected 03:05, by `date`**: the header's *from 03:20* and the correction's *04:50* above were written without reading the clock; `date` printed 03:04 at that correction, so the report was begun before 03:00 (the worktree was added at the start of the session).

## FOUND: C2 evaluated at the program's level judges another evaluator than C's; compiled at `-O0` it does not

An initialiser is evaluated by clang's **constant evaluator**, at any level; C2's probe evaluates the
same text at run time, and R1 compiles at the program's level. They differ where the text asks
`__builtin_constant_p` of something the constant evaluator cannot fold and the optimiser can (panel
216's `A_BCP` shapes, moved into an initialiser). `<scratchpad>/218-ce/bcp/f.h`, the trunk's builds:

```
S_BCPV   __builtin_constant_p(g_var)   ? (INT_MIN / -1) : 5    prints 5        (defined)
S_BCPF   __builtin_constant_p(g_get()) ? (INT_MIN / -1) : 5    prints 5        (defined)
S_BCPM   __builtin_constant_p(g_var)   ? 5 : (INT_MIN / -1)    prints INT_MIN  (undefined)
S_BCPMF  __builtin_constant_p(g_get()) ? 5 : (INT_MIN / -1)    prints INT_MIN  (undefined)

C2, probe at the program's level   -O0: all four right
                                   -O2: S_BCPV TRAPPED (a value C defines refused),
                                        S_BCPM KEPT (an undefined value, in silence)
C2, probe functions __attribute__((optnone, noinline)), -O2: the same two wrong
   (the IR: `llvm.is.constant` resolved true and the load of `g_var` gone, optnone or not)
C2, the text preprocessed at the program's level, the probe compiled and linked at -O0 always
   (`C2LEVEL=-O0`): all four right at both levels; the 35 and the 14 shapes unchanged
   (c4-O0.txt, c4-O2.txt: 16 and 18, 10 and 4)
```

So **C2's probe is compiled at `-O0` whatever the program's level**, from the text the program's
own preprocessor made (its `__OPTIMIZE__` branches already taken). At `-O0` clang answers
`__builtin_constant_p` by Sema's fold or `0`, which is the constant evaluator's answer. The price:
under a `-O0` build C2 shares R1's compile, link and run; under `-O2` (`heroes run`) it needs its own
`-O0` compile, link and run of the same text, panel 216's measured 0.01-0.02 + 0.02-0.03 + a first
run of 0.18-0.25 s real on this Mac (0.07 s in all on Linux arm64), cold only.

## The corpus census of C2 (this Mac, `-O0`, `<scratchpad>/218-ce/cen-mac/census.txt`)

`tools/census.sh` per root, each built cold in its own folder; every kept unit holding an accessor of
the program's own headers (one carrying `heroes-ffi-return`, the region, or `hero_constant_value`)
preprocessed with its own `build/words/words-*.rsp`; C2's probes appended; the text compiled with
the four checks in trap mode; linked as variant S (`used` taken off `hero_extern_refs`,
`-Wl,-dead_strip`); a baseline run that calls nothing must exit 0 before any verdict is read (my
first census lacked it and read dyld's launch abort on `examples/curl`, exit 134, as two traps on
`10000 + 2` and `0`: an instrument's fault, repaired, rerun).

```
57 roots (54 + the roots of 7 nested modules), 58 units asked, 120 constants asked,
23 initialiser probes, 126 dump runs, 0 TRAPPED, 2 failures
```

The 2 failures are units with 0 asked constants whose program does not build on this Mac
(`fixedbugs-437`, `fixedbugs-589`: *no .i*). The compiler itself: 4 units (`clifiles` 201,338 bytes
preprocessed, `cliruntimeplaces` 133,527, `cliprocess` 247,828, `modulereading` 213,575), 22
constants, every one a macro with a literal value, so 22 dumps and 0 probes; one filtered dump of
`cliprocess` read `real 0.02` three times at load 5.3 to 10.5. The 23 probes are the `094` brace
lists (`fixedbugs-094-a-nested-brace-list...` 8 leaves), two enumerator goldens (`571`, `584`),
`examples/curl`'s `CURLOPT_URL` (`10000 + 2`) and `CURLE_OK`; none trapped.

## The final C2 on the two Linux legs, the census included (`<scratchpad>/218-ce/bundle2/`, `out2-d22/`, `out2-u18/`)

The tree is `git archive cbf724983` of my worktree; the compiler built in each container from its
seed; C2 with the final reader (`initc2.py`: references followed, file-scope search) and the probe at
`-O0` (`C2LEVEL=-O0`); the census as on this Mac.

| | Debian clang 22.1.8 (arm64) | Ubuntu clang 18.1.3 (`ubuntu:24.04`, no recommends) |
|---|---|---|
| compiler from the seed | `real 11.535` | `real 13.121` |
| 35 shapes, `-O0` and `-O2` | 16 trapped, 18 kept; `diff` against this Mac: nothing | the same, nothing |
| 14 attack shapes | 10 and 4; nothing | the same, nothing |
| 4 `bcp` shapes | 2 and 2; nothing | the same, nothing |
| census, 57 roots | 58 units, 124 asked, 27 probes, 133 dumps, **0 trapped**, 0 failures; 7 builds refused (the `check`, `unsupported` and `fixedbugs` goldens refused by design) | 57 units, 108 asked, 25 probes, 114 dumps, **0 trapped**; 12 builds refused, the 7 and 5 whose headers the bare image lacks (`examples/curl`, `examples/sqlite`, `examples/ledger`, `ffi-out-parameter-guard`, `fixedbugs-396-sha256...`), 4 of them the census's *no .i* |

## C2b: one dump per asked unit, not one per name (this Mac)

One filtered dump per name costs a parse each time: SDL3's whole unit, `#include <SDL3/SDL.h>`
(`<scratchpad>/218-ce/big/`), is 407,511 bytes preprocessed with 1,174 enumerators (`grep -c
EnumConstantDecl` of its text dump); a filtered dump reads `real 0.02`, the unfiltered one `real
0.05` and 12,033,808 bytes (load 5.6 to 9.6). A program binding 100 SDL enumerators would pay about
2 s cold by name and 0.05 s of clang by unit. And in one dump `referencedDecl.id` names the
declaration exactly, where across runs I had to match by kind and name. **`initc3.py`** (C2b): one
unfiltered dump, every node indexed by `id`, the same walk. Over the 35, 14 and 4 shapes at both
levels (`c5-O0.txt`, `c5-O2.txt`): **the same verdicts as C2, `diff` nothing** (16/18, 10/4, 2/2).

## The read of C2b in Heroes, and the pre-filter that makes it free where nothing is reached

- **A Heroes pass over a dump, measured**: `<scratchpad>/218-ce/scan/scan.hero` (a measurement program,
  56 lines: `read_file`, one byte loop tracking string state and depth, counting
  `"kind": "EnumConstantDecl"`), built at the compiler's own `-O0`, over SDL3's 12,033,815-byte dump:
  `enumerators 1193` (`grep -c` of the file: 1193), `final depth 0`, **`user` 0.22 to 0.25 s** on
  three runs (`real` 0.44 the first, the system's check of a new binary; load 9.1 to 9.8). So one
  dump per unit costs an SDL-sized unit about 0.05 + 0.25 s cold, read in one pass.
- **The pre-filter, sound in the safe direction**: an accessor whose value, as the program's
  preprocessor expanded it, holds no identifier and no brace list cannot reach an enumerator, an
  object or an initialiser, so it needs no dump (a cast to a typedef has an identifier and goes to
  the dump: more dumps, never fewer). Over the census's 120 asked accessors on this Mac (their
  `.i`, a Python classifier): **96 identifier-free with no brace list**, 9 with an identifier
  (`CURLOPT_URL`, `CURLE_OK`, `CURLE_UNSUPPORTED_PROTOCOL`, two `OLD_LIMIT`, `pt_origin`,
  `make_pt()`, `__builtin_huge_valf()`, `(sqlite3_destructor_type)-1`), 12 brace lists, 3 my
  classifier did not read. **The compiler's own 22: all 22 literal-only**, so C2 adds no clang run
  to the compiler's own cold build.
- **Floating contraction, asked and not found**: `(int)(fa * fb + fc)` over `static const double`
  objects is folded by clang into a static initialiser and an enumerator (no word printed); its
  run-time form emits no `llvm.fmuladd` at `-O0` or `-O2` on this Mac or on Debian clang 22.1.8
  (`grep -c`: 0 each). No condition rests on it.

## How C2 composes with R1 (Ux-trap-S, being built in `lane-land215`)

C2 is not a probe of its own. It rides R1's: the same preprocessed text (R1's `-E` under the
program's words), the same four checks in trap mode, the same link with the runtime alone (variant S),
the same resuming run that names each entry before calling it, the same key (the asked unit's
text), so **a warm build pays nothing**. What it adds: the dump and its read for the asked
accessors that reach a declaration; the appended functions; and, under a `-O2` build only, a second
compile, link and run of the same text at `-O0` (above). A trapped C2 entry is told as R1 tells a
trapped accessor, `ffi_constant_type` on the constant's line, with one difference the message owes:
**it names the declaration whose initialiser traps and quotes that line** (the dump's
`presumedFile` and `presumedLine`), since for `EB` the undefined text is `EA`'s `1 << 31`, one line
away under another name. No new class, no surface, no spec token: by the brief's rule the sitting
need not be re-convened full. It changes what the checker refuses (`enum { E = 1 << 31 }` builds
today), so `.claude/rules/verification.md`'s census over every golden tree is owed at its gate; this
report's census is that, read early, on three clangs, 0 trapped.

**Before R1 lands, C2 has nothing to ride**; built alone it would rebuild R1's compile, link, run
and bounds, which is R1. So it lands with R1 or after it, never before.

## Verdicts, route by route

My seat's ground is design.md §1.1 (*implementation simplicity is a constraint, not a goal. It sets
the ceiling*), §1.7 (*anything in the core must be implemented in the type checker and the lowering
and the backend*) and Part 5 (*this distinction determines the size of your compiler*). **No route
here adds a core construct**: none touches the lexer, parser, checker, lowering, IR or runtime, so
no veto on the core arises. design.md says nothing of a header's enumerator or object as such (grep
of *enumerat* and *static const*: only Heroes' own naming at lines 975 to 987); for closing the
class I stand on §1.12 (*a guarantee that ends quietly is not one*) and CLAUDE.md § Precedence,
ranks 3 and 4 as the author read them on 2026-10-16.

- **A, nothing: object.** 16 + 10 + 2 undefined shapes build at exit 0 with a stable wrong value on
  clang 18, 21 and 22, while the same `1 << 31` as a `#define` is refused today: one header value,
  two answers by its spelling. §1.12.
- **B, clang's diagnostics at the initialiser: object.** 6 of 16 never seen on any clang (every
  object and record holding `INT_MIN / -1` or a float cast); `-pedantic-errors` refuses 5 defined
  enumerators (`-1u`, `UINT_MAX`, `0x80000000`, `(1, 2)`, `(int)(1000.0 / 60)`); and the floor
  speaks differently (clang 18: 2 notes of 5, `-Wpedantic` for `-Wc23-extensions`).
- **C as the brief framed it: object, on soundness.** 5 of 10 shapes reached through a reference
  missed in silence (`M_OBJ`, `M_EOR`, `EB`, `R_REF`, `S_REF`).
- **C2 (this report's repair: references followed, the probe at `-O0`, one dump per unit behind the
  pre-filter): adopt**, with the conditions below. 16/16 and 10/10 and 2/2 undefined trapped,
  18/18 and 4/4 and 2/2 defined kept, at `-O0` and `-O2`, on Apple clang 21.0.0, Debian clang 22.1.8
  and Ubuntu clang 18.1.3 alike; the corpus 0 trapped on all three.
- **C2 at the program's level: object** (`S_BCPV` refused, `S_BCPM` kept at `-O2`); **with
  `optnone`: object** (the same two, measured).
- **An evaluator of the compiler's own over the initialiser text (panel 216's J by another name):
  veto**, on the ceiling (§1.1, §1.11): it would own C's expression grammar and arithmetic. C2
  slices clang's text and lets clang evaluate it; it never parses C.

## The seat's answer

- **verdict**: **adopt C2** (route C repaired: references followed transitively from the accessor,
  the probe compiled at `-O0` from the program-level text, one dump per asked unit behind the
  identifier pre-filter, riding R1); **object** to A, B, C as framed, C2 at the program's level and
  C2 with `optnone`; **veto** an evaluator of the compiler's own.
- **section**: design.md §1.1, §1.7, Part 5 for the cost (no core construct; under the ceiling);
  §1.12 for closing the class. design.md is silent on a header's enumerator or object (searched).
- **implementation_cost**: no lexer, parser, checker, lowering, IR or runtime line. **Estimated,
  unbuilt** (R1, which it rides, is not committed): about 300 to 450 code lines in `selfhost/cli/` and
  `selfhost/emit/` (a one-pass reader of clang's JSON as a tree, the worklist, the appended text, the
  message, a canary), beside `emit/constant_accessor.hero` (78 code lines), `emit/ffi_constant_value.hero`
  (145), `emit/record_constant.hero` (119) and the flat JSON readers `emit/layout_screen.hero` (270)
  and `cli/header_types.hero` (280); about 0.3 to 0.4% of the compiler's 116,707 code lines. Time,
  cold only: per asked unit with a reached declaration, one dump (0.02 to 0.05 s) and one Heroes
  pass (0.22 to 0.25 s `user` for a 12 MB dump); none for the 96 of 120 corpus accessors and the
  compiler's own 22 the pre-filter passes; under `-O2` builds, one more compile, link and first run
  (0.02 to 0.03, 0.03 to 0.04, 0.20 to 0.24 s real on this Mac). Warm: nothing.
- **needed_for_self_hosting**: **no**. The compiler's own 22 header constants are literal macros;
  this closes a robustness class (rank 3) and adds no form.
- **argument**: An enumerator's or object's value is computed by clang's constant evaluator, which
  folds `1 << 31` and `INT_MIN / -1` to stable wrong values that no run-time check sees, so R1 cannot
  reach it, and B reads three different word sets on three clangs and never sees an object. Slicing
  the initialiser out of the program's own preprocessed text and running it under R1's checks
  catches all of them, once two holes are shut: values reached through a reference (5 of 10 missed
  as framed) and the evaluator's drift at `-O2` (`__builtin_constant_p`). No core, no parser of C,
  no new class.
- **prediction**: built into R1's lane, C2 gives the verdicts of `c4-O0.txt`/`c4-O2.txt` (35, 14
  and 4 shapes) unchanged on the CI's legs; its census over the 57 roots traps nothing on any leg;
  the compiler's own cold `heroes build selfhost/main.hero` runs **0** C2 dumps (counted with a
  logging `clang` first on `PATH`, as panel 216 counted F2's); its new modules measure at most 450
  code lines by `layout`'s `code_lines`. Checked at R1+C2's landing gate; any one fails it.
- **condition**: I turn to **object** if a value C defines is refused on any CI leg or on the
  Windows box's clang 23.1.1 (**unrun here**), if the canary cannot be made two-sided, or if R1 is
  not adopted (C2 would then be R1 rebuilt). I turn to **veto** if the reader grows into parsing C
  text rather than slicing clang's offsets, or if the cost crosses 450 code lines without a
  sitting.

## Conditions on C2, in one place

1. **References followed** transitively from the accessor's own body, every `DeclRefExpr` to an
   `EnumConstantDecl` or an initialised `VarDecl`; the accessor's own locals excluded.
2. **The probe compiled and linked at `-O0`** from the text preprocessed at the program's level, whatever
   the program's level.
3. **One unfiltered dump per asked unit at most, ids matched within it**, and none for an accessor
   whose expanded value holds no identifier and no brace list.
4. **A two-sided canary of its own**, written into the probe text: an enumerator of an undefined
   value reached through a second enumerator must trap, a defined one must not; either failing
   stops the build as the compiler's own failure, never read as *nothing found* (the reader rests on
   clang's JSON: `offset`, `tokLen`, `inner`, `referencedDecl`, measured on 18, 21 and 22).
5. **The message names the declaration whose initialiser traps**, its header line quoted, on the
   constant's line, class `ffi_constant_type`.
6. **Lands with R1 or after it**, and its gate owes the census over every golden tree.

**Corrected 03:14, by `wc -l scan/scan.hero`**: the measurement program is **58** lines, not 56.

## Housekeeping

- Worktree `.claude/worktrees/218-compiler-engineer` at `cbf724983`, removed at the end with `git worktree remove --force`; every measurement stays in `<scratchpad>/218-ce/` (`sh/`, `atk/`, `bcp/`, `fma/`, `big/`, `scan/`, `cen-mac/`, `out-d22/`, `out-u18/`, `out2-d22/`, `out2-u18/`, `tools/`).
- Containers: my own `docker run --rm` of `heroes-linux-arm64:latest` and `ubuntu:24.04`; the running `ffi218-u18` and `ffi218-d22` are the ffi-pragmatist's and were not touched. No paid run, no credential printed, the Windows box not asked.
- Two slips of mine, repaired: my first census's `pkill` of `census.sh` left its child `heroes build selfhost/main.hero` (pids 83328, 83329, my worktree's binary, output in my census folder) running, stopped by `kill` of those two pids; and three timings taken under zsh with unsplit words timed clang refusing its arguments, discarded and retaken under bash. Nothing was written in the trunk but this report (`git status --short`: the `218-*` and `219-*` panel folders only).
