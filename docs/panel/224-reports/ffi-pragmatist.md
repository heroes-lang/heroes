# Panel 224, the ffi-pragmatist

Written 2026-10-17 from 18:07. My copy: `.claude/worktrees/224-ffi-pragmatist/` (`git -C
<lane> archive 3c478500b | tar -x`), stage 1 from the seed and stage 2 from it, both exit 0. On the
box: `/c/w/224-ffi-pragmatist/tree` (`git --git-dir=/c/w/heroes.git archive 3c478500b`; the box's
repository held `3c478500b` by then, `cat-file -t` printing `commit`), stage 1 with
`-Wl,/STACK:67108864` and stage 2, both exit 0. Every script named below is in my copy's `x/`
(`box_*.sh` run on the box from `/c/w/224-ffi-pragmatist/x`, `mac_*.sh` here). Compile words are
`flags()`'s list as written in `selfhost/cli/flags.hero` (`x/words.sh`), then the level and its
debug word, `-I runtime`, `-I <source dir>`, as `compiling.unit_words` assembles them. No paid
run, no credential printed, nothing installed on the box. Clangs: the box's `clang version 23.1.1`
(`x86_64-pc-windows-msvc`); this Mac's `Apple clang version 21.0.0 (clang-2100.3.34.2)`; MSVC
`19.44.35229` (`cl.exe` of BuildTools 14.44.35207).

## 0. Measurement 1, reproduced

`./heroes-next.exe run tests/harness/main.hero -- ./heroes-next.exe unsupported fixedbugs-2072` on
the box: `harness: 0 passed, 1 failed`, exit 1. A direct `heroes-next.exe build` of the case: exit
1, four `error[` lines, naming `SHIFT_PAST`, `SHIFT_BY_NEGATIVE`, `DIVIDED_BY_ZERO`,
`REMAINDER_BY_ZERO`; `SHIFT_NEGATIVE` and `SHIFT_INTO_SIGN` absent.

## 1. Item 2: what `-fno-wrapv` does to the C a Windows binding reaches

**The headers.** The box has none of sqlite, curl, raylib, SDL3 (the critic's § 2.6); I installed
nothing. So the set is the UCRT's and the SDK's: `x/hdrs.c` includes `winsock2.h`, `ws2tcpip.h`,
`windows.h` (whole, no `WIN32_LEAN_AND_MEAN`), `intsafe.h`, `strsafe.h`, `shlwapi.h`, `shellapi.h`,
27 UCRT headers (`stdio.h` to `complex.h`, `io.h`, `process.h`, `direct.h`, `conio.h`...; 35
`#include <` lines in all, `grep -c`),
`intrin.h`, and the project's `hero_os.h`, which every example binds.

| what | command (`x/box_item2.sh`, `x/box_item2b.sh`) | result |
|---|---|---|
| the box's words carry `-fwrapv` | `clang $W -O2 -### -c hdrs.c \| grep -c '"-fwrapv"'` | 1 |
| the unit compiles both ways | `clang $W -O2 [-fno-wrapv] -femit-all-decls -S hdrs.c` | exit 0, 0 errors, 0 warnings, each |
| every inline function's IR | `clang $W -O0 -Xclang -disable-O0-optnone -femit-all-decls [-fno-wrapv] -S -emit-llvm hdrs.c` | **602** definitions each; ` nsw` 0 against **6**; 1,063,019 against 1,063,043 bytes, the 24 bytes being the six ` nsw` |
| the functions holding them | `awk` over the `-fno-wrapv` IR | `UInt8Add` (2), `UShortAdd` (2), `UInt8Sub`, `UShortSub`, all `intsafe.h`: an 8 or 16-bit unsigned value promoted to `int`, which cannot overflow |
| the machine code, every one of the 602 | each IR's linkage made external (`sed`), `clang -O2 -S`, then `cmp` | **identical**: 602 definitions, 603 `.def`s each |
| the runtime | `clang $W -O2 [-fno-wrapv] -S runtime/runtime.c`, compared per function (`x/split.py`) | 283 functions, **8 differ**: `hero_array_push_owned`, `hero_array_sort`, `hero_fs_remove`, `hero_lend_local_name`, `hero_lend_tell`, `hero_map_new`, `hero_stack_veh`, `hero_str_chars` (their diffs unread) |

So on the box, `-fno-wrapv` changes the meaning of six header operations that cannot overflow and
the machine code of **no** header inline function; it changes 8 runtime functions' code, toward
the meaning the other three legs already compile the runtime under, their words carrying no
`-fwrapv`.

**The §4.19 ladder's own headers, on this Mac** (`x/mac_ladder2.sh`): `sqlite3.h` and `curl/curl.h`
from the SDK, `raylib.h`, `raymath.h` and `SDL3/SDL.h` from Homebrew. `-femit-all-decls` emits
only Darwin's `_OSSwapInt*` here, so every inline function with a body was read from clang's JSON
AST (`x/inl.py`: **228**) and its address taken in the unit. 42 definitions at `-O0`; ` nsw` 0
against 4, in `SDL_MostSignificantBitIndex32` (1), `SDL_PointInRect` (2, `r->x + r->w`) and
Darwin's `__sputc` (1); at `-O2`, 18 functions emitted both ways, **0 differ** (`x/splitmo.py`),
the two SDL3 functions among them. This is Apple clang on arm64, not the box: SDL3 on the box is
unrun.

**And on this Mac, route A is a no-op on a real binding** (`x/mac_a.sh`): the SQLite example's
unit (`build/tu-68c8cdd8636872aa/main.c`, three integer constants, so it holds a region) compiled
with and without `-fno-wrapv` at `-O0 -g` and at `-O2 -gline-tables-only`: exit 0 each, `cmp`
silent, objects identical; `runtime.o` at `-O2` identical too.

## 2. Is `-fno-wrapv` the C standard's meaning on every CI target, and can an MSVC header rely on wrap?

- **The standard.** C11 6.5p5 leaves an exceptional signed result undefined; 6.5.7p3 and p4 a
  shift by a negative or too-wide count, or of a negative value, or whose result the type cannot
  represent; 6.5.5p5 a division or remainder by zero. `-fno-wrapv` is clang taking exactly that;
  `-fwrapv` defines what the standard leaves undefined, an extension. Measured: under `-fno-wrapv`
  clang marks `add` and `mul` `nsw` and folds `(x * 2) / 2` to `x` (`x/shl.c`, `ret` against
  `sbfx w0, w0, #0, #31`); **a signed `<<` is a plain `shl` under both words**, so for the two values
  2120 is about the words move clang's warnings and no code.
- **The four legs.** Apple clang 21's `-###` adds no `-fwrapv` for `x86_64-linux-gnu`,
  `aarch64-linux-gnu`, `arm64-apple-macos` or `x86_64-pc-windows-msvc` (0 each). The CI's own clangs
  (Linux legs, and Windows' 20.1.8, the critic's reading of the log) I did not run: that they add
  none is an inference from the case reading green there. So `-fno-wrapv` is each leg's default
  today; only clang 23 with `-fms-compatibility` (the box) departs from it.
- **MSVC does not give wrap either.** `cl -nologo -c -O2 -FAs msvc_ovf.c` on the box
  (`x/box_msvc.sh`): `int mul(int x) { return (x * 2) / 2; }` is `mov eax, ecx; ret`, folded on the
  assumption that `x * 2` cannot overflow; `(x << 1) >> 1` kept as `lea; sar`. So a header written
  for MSVC cannot rely on a signed multiply wrapping, because MSVC itself does not; a header relying
  on a sign-bit shift would keep its code under both clang words (above). The reach of a binding
  into a header is its constants (the region and the probe) and its inline functions (§ 1: no code
  moves); a function-like macro is not bindable (design.md §4.19).

## 3. The routes, built where they touch the boundary

**E, on the box's real unit** (`x/box_e2.sh`; the unit
`build/tu-208926e47a656344/fixedbugs2072avaluecleavesundefinedistold.c` my stage 2 wrote):

| compile | exit | `error:` lines |
|---|---|---|
| the unit's own, the box's words, `-O0 -g -c` | 1 | 7: four region warnings, three static assertions (`DIVIDED_BY_ZERO`'s and `REMAINDER_BY_ZERO`'s constancy, `REMAINDER_BY_ZERO`'s type) |
| E: the same words, `-fno-wrapv -fsyntax-only` | 1 | 9: the same seven, plus *shifting a negative signed value is undefined [-Werror,-Wshift-negative-value]* and *signed shift result (0x80000000) sets the sign bit of the shift expression's type ('int') and becomes negative [-Werror,-Wshift-sign-overflow]*, **the `.expected`'s own words** |

**E over the SDK values panel 218 names** (`x/sdk_acc.c`, the region's accessor shape over
`windows.h` and `d3d9types.h`): under the box's words exit 0, no error, so today they wait for the
probe; under E's pass exit 1, two errors, `VRL_ENABLE_KERNEL_BREAKS` and
`D3DSTREAMSOURCE_INSTANCEDATA`, each *signed shift result (0x80000000) sets the sign bit ...*; the
controls `MB_OK` and `INVALID_FILE_ATTRIBUTES` pass; `windows.h` raises **0 warnings** under
`-fno-wrapv`. So E refuses what R3 already refuses, sooner and in clang's words, and nothing else.

**E's cost** (`x/box_t.sh`, three runs, `real` only, since Git Bash's `time` reads no child CPU on
Windows; other seats' folders stood on the box and its load was not read, so these are
indicative, not a timing): the 2072 unit 0.047 to 0.087 s compile, 0.050 to 0.076 s
pass; the `windows.h` + UCRT unit (`hdrs.c`) **0.609 to 0.753 s compile, 0.628 to 0.645 s pass**,
so about one more front end per unit holding a region, cold. On this Mac (`x/mac_e.sh`), the SQLite
unit, 2,964 lines preprocessed: 0.02 to 0.03 s compile, 0.02 s pass. A warm build compiles nothing
and would pass nothing, if the pass is keyed with the object (unbuilt).

**G, `-fno-ms-compatibility`, on the box** (`x/box_g.sh`, `x/box_g2.sh`, `x/box_g3.sh`): it does
drop `-fwrapv` (`-###` keeps `-fms-extensions` and `-fms-compatibility-version=19.44.35229`), and
`hdrs.c` and `runtime.c` still compile. But the predefined macros change: `__STDC__ 1`,
`__GNUC__ 4`, `__GNUC_MINOR__ 2`, `__GNUC_PATCHLEVEL__ 1`, `__GNUC_STDC_INLINE__`,
`__GXX_ABI_VERSION`, and eleven `__GCC_ATOMIC_*` appear (`diff` of `-dM -E`); the header unit's
inline set moves from 602 to 598 definitions (104 IR lines differ; `fstat`, `cprintf`, `cscanf`
gone). **A binding's C, written as the emitter writes it** (`x/bind_fstat.c`: `#include
<sys/stat.h>`, a probe pointer `int (*)(int, struct stat *)` initialised with `fstat`, a call):
under the box's words exit 0 and `fstat 0 size 718 sizeof(struct stat) 48`; under G **three errors**,
*use of undeclared identifier 'fstat'*, *variable has incomplete type 'struct stat'*, *call to
undeclared function 'fstat'*. `__STDC__` sets `_CRT_INTERNAL_NONSTDC_NAMES` to 0
(`ucrt/corecrt.h:413-417`), which guards the POSIX names (`ucrt/sys/stat.h:216` for `fstat`):
`x/g_names.c` taking the address of `fileno`, `strdup`, `open`, `close`, `getcwd`, `getpid`,
`fstat`, `unlink` reads 0 errors under the box's words and all eight undeclared under G. And
`__GNUC__` sends any header that tests it down its GCC branch on a target whose ABI is MSVC's;
which headers, unmeasured.

**A's one gap that is not a cost**: a package's compile words come after `flags()`
(`compiling.search_words`), and a later `-fwrapv` wins, so a package carrying one would undo A for
its units in silence. None does here: `grep -rl -- -fwrapv` over 990 `.pc` files of this Mac's
Homebrew: none; `pkg-config --cflags python3-embed`: `-I` only. A word placed last in E's pass
carries no such gap.

**E'**: unbuilt. Its pass is E's with an object kept, so it costs E's front end plus an object and
a link input, and it is the one route that compiles one module's headers under two meanings into
one program: an MSVC-mode inline function is `linkonce_odr` with a comdat (§ 1's IR), so if both
units emitted one, the linker would choose its meaning. Today an accessor's `return NAME;` emits no
header function (a call is refused by the constancy check), so the hazard is latent: an inference,
unrun.

**B, C, D, F** touch no word of the program's compile, so they change no binding's C. D: I tried no
spelling beyond the critic's nine, and `shl` carrying no `nsw` (§ 2) says the silence is in clang's
warnings, not its code. B and F on the box tell the two in the probe's words (the critic's § 3),
so the 2072 case's one `.expected` cannot hold on both platforms under B or F alone; under E it
does. The Mac shapes (the critic's § 6) have no `-fwrapv`, so E does not touch them; that B or F
reaches `both.hero` and `mm/`, and neither `missing.hero`, is my reading of their mechanism, unrun.

## 4. Verdicts per route

| route | verdict | section | the reason, in one line | condition that would change it |
|---|---|---|---|---|
| A, `-fno-wrapv` in every compile | **object** | §4.19; panel 218 R3 (ratified), this seat's own 218 condition *the program's compile untouched* | it changes 8 runtime functions' code on the box and every key on every platform to move WHEN a diagnostic is told, where E tells the same six in the same words and changes no object byte; a package's later `-fwrapv` undoes it | E's pass measured too slow for a real Windows program (its cold build more than, say, 1.5 times A's), or the author ruling the program's meaning ISO's on every target; then A is acceptable to this seat: 0 of 602 header inline functions change code |
| B, the probe over a refused unit | **approve, as the order's repair beside E**; object to it as 2120's answer alone | §4.19 (no word of the program's compile moves); §4.17 for the words | FFI-neutral; alone it tells the box's two in other words than the Mac's for one header line | none for the FFI; the words are the comprehension seats' |
| C, open and said | **object** | §1.11 with §4.17 | a binding author on Windows pays a second build for values the toolchain could tell in the first; the CI leg turns red the day `windows-latest` carries clang 23 (critic, inference) | a ruling that two builds is this class's accepted cost, in §4.17's count, by a full sitting |
| D, a per-region word | **object** (nothing to build) | none: design.md is silent on per-function meanings | the critic's nine spellings and none found; no FFI harm if one existed | a spelling that brings both warnings back with the object byte-identical |
| E, a `-fsyntax-only -fno-wrapv` pass | **approve** | §1.11, §4.19, §1.12; consistent with panel 218 R3 (the word in a pass that makes no object) | all six told in the first build on the box in the `.expected`'s words; no object byte moves; the SDK values R3 refuses are refused sooner, nothing else | the conditions below unmet, or one object byte moving under it |
| E', the accessors in their own unit | **object** | §4.19 | dominated by E: the same pass plus an object, a link input and one module's headers under two meanings in one program | an accessor unit proved to emit no header function by construction, and its cost below E's |
| F, region warnings read back | **approve, as the order's repair beside E** | §4.19; panel 221's precedent | FFI-neutral; on the box, alone, the two in the probe's words | as B |
| G, `-fno-ms-compatibility` | **veto** | §1.11, §4.19 | it breaks a binding that builds today: `fstat` from `sys/stat.h` refused by three errors, `__STDC__` withholding the UCRT's POSIX names (eight of eight tried), `__GNUC__` re-routing the branches of any header that tests it | none: a binding that compiles today and is refused under it is categorical |

**Conditions on E, this seat's**: (1) diagnostic-only, the object compiled under the target's own
words, so a header's inline code keeps the platform's meaning; (2) `-fno-wrapv` the pass's LAST
word, after the package words, so no package undoes it; (3) the pass read for the region's six
(`UNDEFINED`) alone, every other diagnostic left to the unit's own compile, so a header's reaction
to the word can refuse nothing new; (4) only units holding a region, keyed with the object, so a
warm build and a program binding no header value pay nothing (the program binding none must not
move: unmeasured by me, the compiler-engineer's). The lane holds: E writes no spec token and no
§4.17 promise; a §4.19 sentence saying the pass exists is a description, not a promise.

## The seat's block

- `verdict`: **approve E** (with B or F for the order); object A, C, D, E'; **veto G**.
- `section`: design.md §1.11 and §4.19 (a binding's C is compiled as the platform's compiler
  compiles it, and a wrong one is a compile error); panel 218 R3. design.md says nothing of
  `-fwrapv` (`grep -n -i wrapv docs/design.md`: none), so the meaning of a header's inline code
  under a non-default word is uncovered there, and this seat stands on the measurement.
- `experiment`: § 1 to § 3, every one compiled with clang against the real header; accepted: the
  602 UCRT and SDK inline functions both ways (identical code), the ladder's 18 (identical), the
  SQLite unit both ways (identical objects), E's pass (six region errors, the `.expected`'s words),
  the SDK accessors under E (two refused, controls pass); refused: the `fstat` binding under G.
- `argument`: The thesis at the boundary is that the program's C is the platform's C, checked
  against the real header. E keeps that: it asks the standard's question in a pass that writes
  nothing, so every object is the one the platform's clang makes, and all six reach the author in
  the first build in clang's words. A asks the same question by rewriting every object's meaning;
  measured harmless today, it reverses a ratified ruling and moves every key to buy what E buys
  free. G is categorical: it removes declarations a binding needs.
- `prediction`: under E as conditioned, on the box, a program binding `VRL_ENABLE_KERNEL_BREAKS`
  from `windows.h` beside `MB_OK` is refused in its FIRST build at the constant's line in clang's
  words (*signed shift result (0x80000000) sets the sign bit ...*), the 2072 case reads `1 passed`
  with its `.expected` unchanged, and the SQLite example's objects on this Mac are byte-identical
  (`cmp`) to those built without E. One moved object byte falsifies *E changes no binding's code*.
- `condition`: E's four conditions above; G's veto stands on any evidence short of the `fstat`
  binding building under it; A becomes acceptable if E's measured cold cost on a real Windows
  program exceeds A's by half or more.
