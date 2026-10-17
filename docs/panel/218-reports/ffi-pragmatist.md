# Panel 218, the ffi-pragmatist

Written 2026-10-17 from 02:38, in a detached worktree of `cbf724983` at
`.claude/worktrees/218-ffi-pragmatist` (`git log -1 --format=%H` there printed
`cbf7249835060127ec9b45d805dce08d7be31bdf`), `build/` removed, the compiler built there from the
seed: `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes` under `/usr/bin/time -p`,
`real 7.75 user 7.58 sys 0.12`, load 1.88. Containers `ffi218-d22` (`heroes-linux-arm64:latest`,
Debian clang 22.1.8, no `--platform`) and `ffi218-u18` (`ubuntu:24.04`, its `clang` package, Ubuntu
clang 18.1.3), panel 216's libraries installed with `--no-install-recommends` (its `install.sh`,
copied), the worktree copied in and its compiler built from the seed in each (`seed built:` lines of
`linux/install-*.log`). Every script and output named below is in `<scratchpad>/218-ffi/`, where
`<scratchpad>` is
`/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/5f7a29c1-133b-4b38-877a-3b0496db6842/scratchpad`;
the worktree, the containers and the box's folder are removed at the end. Every number names its
command; what could not be run says *unrun*.

**Four harness faults of my own were found and repaired before any number below was read**, each
by the control going wrong: `zsh` does not split `$f` (every multi-flag row of my first table was
one unknown flag, rerun under `bash`); `sort -n -u` merged two flags on one line; an enumerator
with an attribute and no initialiser had its attribute read as its value (26 of curl's
`CURLFORM_*` on Debian, then unasked and miscounted as explicit); a closure memo shared across
constants kept `N_FROM_BAD` from re-asking `N_SHL`. No number below predates its repair.

## 1. The brief's three shapes, re-measured, and two more

`brief/f.h` is the brief's header plus `R_BRACE {(-2147483647 - 1) / -1, 2}` bound to a group
record. `heroes build`, three runs each (`brief/`, this Mac, Apple clang 21.0.0):

| name | build | prints |
|---|---|---|
| `E_SHIFT` (`enum`, `1 << 31`) | 0 | `-2147483648` ×3 |
| `S_DIV` (`static const int`) | 0 | `1` ×3 |
| `D_SHIFT` (`#define`) | 1, `ffi_constant_type` | — |
| `R_BRACE` (brace list) | 0 | `-2147483648` ×3 |

**A record constant is outside R1 in every spelling, the float casts included.** The trunk's own
emitted unit for six record constants (`shapes/recs.hero`: a brace list and a compound literal,
each with `INT_MIN / -1` and with `(int)1e20`, and a `static const struct` of each kind) built at
exit 0 and printed `-2147483648` or `2147483647` three times; compiled with R1's four checks in
trap mode (`clang -fsanitize=… -fsanitize-trap=… -S -emit-llvm`), **every one of the six accessors
holds 0 traps at `-O0` and at `-O2`** (`shapes/recs-O0.ll`, `recs-O2.ll`, an `awk` count of
`llvm.ubsantrap` per `define`). Clang folds an aggregate initialiser in its front end and emits a
constant, whatever its leaves. Panel 216's compiler-engineer table had `T_PTX ((struct pt){ (int)(1e20),
3 })` *flagged if asked*; asked through the accessor `emit/record_constant.hero` writes
(`struct pt hero_constant_value = NAME; return hero_constant_value;`) it is not. So the record row
of this class is wider than the brief says: not only `INT_MIN / -1` in a brace list, but every
undefined leaf of every record constant.

## 2. What clang says of each initialiser (the brief's first unrun question)

`shapes/sh.h` holds 56 shapes beside the class (the control below); `shapes/diag.sh <clang> sh.h`
compiles it `-fsyntax-only` under five flag sets and lists the lines that warn, with the flag
(`diag-mac21.txt`, `diag-ffi218-d22.txt`, `diag-ffi218-u18.txt`; the Windows box below).

- **Default and `-Wall -Wextra`** (identical but for `-Wunused-const-variable`): only
  `-Winteger-overflow` on `INT_MAX + 1`, `-INT_MIN`, `65536 * 65536` (enumerator and object alike) and
  `LLONG_MAX + 1`, and `-Wliteral-conversion` on `static const int = 1e20` **and on `= 2.9`, a value
  C defines**. Nothing on `1 << 31`, `INT_MIN / -1`, `% -1`, `(int)1e20`, `1 << 40`, `-1 << 3`, a
  hidden or ternary division, a double from `INT_MIN / -1`, or any record. Same on 18, 21, 22.
- **The region's six warnings, raised over the header itself**: adds `-Wshift-sign-overflow` on the
  enumerator and the object `1 << 31`; **`-Wshift-count-overflow` does not fire on `enum { E = 1 <<
  40 }` and `-Wshift-negative-value` not on `enum { E = -1 << 3 }`** (both fire inside a function).
  Same on 18, 21, 22.
- **`-pedantic`** (clang's reading of C's required diagnostics): `-Wgnu-folding-constant` on every
  enumerator that is not an integer constant expression (`INT_MIN / -1`, `% -1`, the overflows,
  `(int)1e20`, `1 << 40`, `-1 << 3`, the hidden and ternary divisions) on 21 and 22; **on 18,
  nothing on `1 << 40` and `-1 << 3`**. Nothing on any `static const` initialiser, on any clang.
  An enumerator past `int` (`-1u`, `1u << 31`, `0x80000000`, `1LL << 40`, the implicit one after
  `INT_MAX`) is `-Wc23-extensions` on 21 and 22 and `-Wpedantic` on 18, which also prints an
  unflagged *overflow in enumeration value* on the implicit one; its value is `2147483648` on all
  three (`t_U_ENUM_IMPLICIT`, built and run in both containers).
- **Whether C11 requires a diagnostic** (§ 6.6 p4, § 6.7.2.2 p2): *a reading, not a run.* The
  instrument I can run is clang's `-pedantic-errors`, above: it refuses the non-ICE enumerators and
  the enumerators past `int` and **no `static const` initialiser at all**, so if C11 requires one
  there, clang does not give it.

**This Mac's clang does not define `__STDC_IEC_559__`; both Linux legs do** (`#include <stdio.h>`
then `-dM`, grepped): a float `static const` that overflows (`1e308 * 10`, `(float)1e40`, `0.0f /
0.0f`) is ISO-undefined on this Mac and Annex-F-defined on Linux. Clang's own model (UBSan's
*range of representable values … is [-inf, +inf]*) defines them everywhere, R1's four checks keep
them, and so does every route below; I count them as defined and name the boundary here.

## 3. Today, on the control and on a real header

`shapes/today.sh`, one program per name, `heroes build`, three runs (`today-mac.tsv`): **all 28
undefined control shapes build at exit 0 and print a stable value** (enumerators, `static const`
integers and doubles, chains through an undefined enumerator, brace lists, compound literals,
`static const` records); 27 of the 28 defined ones build and print C's value, and `D_ENUM_LL`
(`1LL << 40`) is refused *`unsigned long` in `sh.h`, not `i64`* with *fix (guess): declare it
`u64`*, which is right (clang types that enumerator `unsigned long`); as `u64` it builds.

**A real one: glibc's `<sys/mount.h>` line 124, `MS_NOUSER = 1 << 31`** (an enumerator, with
`#define MS_NOUSER MS_NOUSER` under it; `grep -n -B2 -A3` in the container). `linux/ms/ms.hero`
binds it with `MS_RDONLY`: **`heroes build` exit 0 and `-2147483648` three times, on Debian clang
22.1.8 and on Ubuntu clang 18.1.3**, where `#define X (1 << 31)` is refused today. That is defect
2109's class in the C library itself.

## 4. The population: every enumerator, const object and brace macro the real headers bring

`pop.py` (usage in its docstring) takes panel 216's library lists unchanged (`spec-mac.txt`,
`spec-d22.txt`, `spec-u18.txt`: the C library and 60-odd libraries with their `pkg-config` words),
reads each in the context a unit reads a group's header in, **preprocessed under the build's words
at `-O0` and again at `-O2`** (R1's own step), line markers emptied so offsets keep their place,
and lists from clang's JSON dump every `EnumConstantDecl` at file scope (inside a record too, where
C puts it), every file-scope `VarDecl` whose type is const and which has an initialiser (its
class), and every object-like macro whose expansion is one brace list or compound literal. Members
of the runtime's own headers are left out. Counted unique by (header file, name), and summed per
library as panel 216 counted macros (`analyze.py <tsv>`, `*-analysis.txt`):

| | Apple clang 21.0.0 | Debian clang 22.1.8 | Ubuntu clang 18.1.3 |
|---|---|---|---|
| libraries whose context compiled | 110 of 115 (as 216) | 135 of 136 (raylib not packaged) | 133 of 136 (raylib, SDL3, mpdecimal) |
| enumerators with an initialiser | 5,792 | 5,903 | 5,248 |
| enumerators without one | 5,055 | 5,751 | 5,027 |
| brace-list or compound-literal macros | 129 | 71 | 67 |
| file-scope const objects with an initialiser (integer, float, record) | **0** | **0** | **0** |
| **unique, in scope** | **10,976** | **11,725** | **10,342** |
| summed per library | 12,233 | 15,421 | 14,206 |

The same counts at `-O2`. **No real header of the 110 to 135 defines a `static const` scalar or
record object**: the dump found none, and a second instrument agrees (`grep` of `^static const …
=` over every preprocessed Ubuntu unit found only my control's lines; this Mac adds one `const`
array, out of scope). So the `static const` shape of defect 2109 exists in the corpus of planted
cases and in headers a program writes for itself, not in these libraries.

**Every one of them binds today**, which my count assumed and then measured: `bindcheck.py`
(panel 216's `uxcorpus.py` loop) writes per library ONE program binding every enumerator the sweep
found there (`i64`, `u64` past `INT64_MAX`) and builds it `--emit-c` (clang's build rounds, no
link): **12,011 of 12,013 on this Mac (56 libraries carry enumerators beyond the context), 15,326
of 15,326 on Debian (135), 14,115 of 14,115 on Ubuntu (133)**, `MS_NOUSER` among the bound. The two
refused are one name, Darwin's `MACH_VM_RANGE_NONE`, whose `enum` has the fixed underlying type
`uint64_t`: *is `uint64_t` in `mach/vm_types.h`, not `i64`*, which is right, my generator having
typed it by its value 0.

**The corpus** (`corpus.py`, the 169 group constants `consts.awk` finds in the tree's 2,905
tracked `.hero` files outside `archive/` and `docs/`): 127 name macros, **5 enumerators on this Mac
and 7 on Linux** (`CURLOPT_URL`, `CURLE_OK`, `CURLE_UNSUPPORTED_PROTOCOL`, two `OLD_LIMIT`; glibc
adds `SOCK_STREAM` and `RLIMIT_FSIZE`), 19 brace-list or compound-literal record macros, 5
statement expressions, 1 macro naming a `static const struct` (`PT_OBJ`, `pt_origin`), 2 absent by
design, and 10 constants in 6 files whose header did not open with the file's directory alone.

## 5. The routes, over the whole population, on three clangs, at both levels

**A**, nothing. **B**, route B in four forms, each compiled over the same marker-free unit (so a
system header's warnings are not hidden), an error owned by a member when its offset lies between
the enumerator's name and its initialiser's end: `B6` the region's six warnings as errors; `B7`
plus `-Wliteral-conversion`; `Bf` the six plus `-Wgnu-folding-constant` (C's own ICE rule for an
enumerator, the strongest B I could build); `Bp` `-Wgnu-folding-constant` and `-Wc23-extensions`.
**C**, each member's initialiser re-asked as `__typeof__(NAME) q(void) { return (TEXT); }`, the text
read by offset from the unit clang read, a record leaf by leaf at the leaf's own type, compiled at
the level with R1's four checks in trap mode, `-ffunction-sections`, linked with nothing of a
program's and a dead-strip, run by a probe that writes each index before the call and starts again
after one that trapped; **C-closure** also refuses a member whose initialiser names (through any
chain) an enumerator or object that trapped. Every probe of every library linked, loaded and
finished on all three legs (`probes: ok` 111, 136, 134 per level); every one of the 154 traps is signal 5 (`trap exit -5` 50, 52, 52 across both levels, the control's 25 per level among them), no death of another kind.

| real headers, unique members refused | Apple 21 | Debian 22 | Ubuntu 18 |
|---|---|---|---|
| A | 0 | 0 | 0 |
| B6, B7, Bf | 0 | 1: `MS_NOUSER` | 1: `MS_NOUSER` |
| Bp | 0 | 1: **`EPOLLET = 1u << 31`, a value C defines** | 0 |
| C, C-closure, `-O0` and `-O2` | 0 | 1: `MS_NOUSER` | 1: `MS_NOUSER` |
| a value C defines refused, any route but Bp | 0 | 0 | 0 |

`MS_NOUSER` is the one undefined member of the 33,043 the three legs hold between them (unique per leg, summed); it is the last enumerator of its `enum`
(no implicit successor inherits it), and nothing else in any header names it. `EPOLLET` builds today
and prints `2147483648` on both Linux legs (`linux/ms/ep.hero`).

**The control** (`sh.h`, 61 members in scope: 28 that C leaves undefined, 33 it defines, the 28
naming shapes in every row of the class: enumerator, `static const` integer and double, chains,
brace lists, compound literals, `static const` records), same pipeline:

| control | Apple 21 | Debian 22 | Ubuntu 18 | defined refused |
|---|---|---|---|---|
| B6 | 7 of 28 | 7 | 7 | 0 |
| B7 | 8 of 28 | 8 | 8 | 1, `static const int = 2.9` |
| Bf | 14 of 28 | 14 | **12** (misses `1 << 40`, `-1 << 3`) | 0 |
| Bp | 10 of 28 | 10 | 5 | 5 on 21/22 (`-1u`, `1u << 31`, `0x80000000`, `1LL << 40`, the implicit past `INT_MAX`), 0 on 18 |
| C | 25 of 28 (misses the three chains) | 25 | 25 | 0 |
| C-closure | **28 of 28** | **28** | **28** | **0** |

Every B misses every `static const` division and float cast and every record, on every clang:
clang has no warning for them to raise. The same verdicts at `-O0` and `-O2` on all three.

**The attack shapes** (`atk/`, through `pop.py` on all three clangs, identical):

| shape | today | B6/B7 | Bf | Bp | C-closure |
|---|---|---|---|---|---|
| `lvl.h`, `1 << 31` under `__OPTIMIZE__`, else `1 << 30` | `heroes build` `1073741824`, `heroes run` **`-2147483648`**, both exit 0 | `-O2` only | `-O2` only | kept | **`-O2` only** |
| `wd.h`, `1 << W_SHIFT`, 31 when an earlier group defines it | `-2147483648` with `gs.h` first, `1073741824` without | right both | right | kept | **right both** |
| `(1, 2)`, `(int)(1000.0 / 60)`, a `static const int` read + 1 (C defines 2, 16, 8) | build | kept | **refused** | **refused** | kept |
| `static const int = N_SHL + 0`, `N_SHL = 1 << 31` | `-2147483648` | kept | kept | kept | **refused** |
| `enum { E = N_SHL \| 1 }` | `-2147483647` | kept | refused (as non-ICE) | refused | **refused** |
| `enum { IA = 1 << 31, IB, IC }`, bound `IB`, `IC` | `-2147483647`, `-2147483646` | not asked | not asked | not asked | **refused** (`r1c.py`'s predecessor rule, section 6) |
| `0.0f / 0.0f`, `(double)(1 << 30) * 4` | `nan`, `4294967296.0` | kept | kept | kept | kept |
| unsigned wrap `-1u`, `4294967295u + 1u`, `1u << 31`; negatives `-1`, `-2147483647 - 1`, `-2147483648` | C's value | kept | kept | `-1u` and `1u << 31` refused on 21 and 22, kept on 18 | kept |
| a record from enumerators `{ U_ENUM_SHL31, 2 }`, `{ D_ENUM_NEG, D_ENUM_SIZEOF }` | `-2147483648`, `-1` | kept, kept | kept, kept | kept, kept | **refused**, kept |

**A route nobody listed, measured and set aside: K-init.** Panel 216's K (clang's constant evaluator,
reached inside gnu11 through `diagnose_if`) asked not of the bound name, which clang has already
folded, but of the same re-extracted initialiser texts C runs: `int k(int x)
__attribute__((diagnose_if(!(((TEXT)) == ((TEXT))) || 1, "k", "error")));`, read by `#line`
position (`pop.py`, rerun over everything as `*k.tsv`). It executes nothing at build time, which is
why it was worth asking. **Control: 21 of 28 undefined, 0 defined, on all three clangs; the real
headers: 0 refused on all three, so `MS_NOUSER` is kept.** It misses `1 << 31` in every spelling
(the evaluator reads the shift as C++20 does), the implicit conversion of `static const int =
1e20` (the question compares doubles), and the chains; and it refuses `0.0f / 0.0f`, which Annex F
defines on Linux. It keeps the one undefined member the real headers hold, so it is not a route.

## 6. Route C as the compiler would build it, on programs' own emitted units

The brief's C (*the initialiser's text compiled as an accessor's `return`*) is not enough as
written, measured three ways above: a record's `return` is folded (0 traps), a chain hides the
undefined value one name away, and an enumerator with no initialiser carries its predecessor's
value. So `r1c.py` builds C rooted at the program's own accessor: `heroes build --emit-c`; R1's
preprocessing (the build's words, the level, `main` renamed); **clang's JSON dump filtered on the
accessor's own name** (`-ast-dump=json -ast-dump-filter`, the instrument `cli/pointee.hero:186-188`
already uses with `hero_ty_`); every `EnumConstantDecl` and file-scope `VarDecl` the accessor's
value names, and every one their initialisers name, re-asked by their own initialiser's text; an
enumerator with no initialiser asks its predecessor in its place (C11 6.7.2.2 p3's rule, read; the
whole unit's dump is read once for it); a record, the accessor's own `hero_constant_value` or a
const record object, leaf by leaf at the leaf's converted type; all of it appended to R1's probe
unit and run by the resuming trap probe, R1's own accessors called in the same probe.

| emitted units, both levels | Apple 21 | Debian 22 | Ubuntu 18 |
|---|---|---|---|
| control programs (`ctlprog/`, one per name, 56) | 28 of 28 undefined refused, 0 of 28 defined; **R1: 0 of 56** | same | same |
| `MS_NOUSER` / `MS_RDONLY` / `EPOLLET` | — | refused / kept / kept | refused / kept / kept |
| `L_OPT`, `W_ENUM` on and off, `IB`, `IC`, `JB` | as the table above | same | same |
| corpus: 85 files binding a constant; emitted / refused at emission today | 47 / 37 (and `fixedbugs-396`, whose OpenSSL header needs its package's `-I`, which my script does not pass; its two constants are macros) | 50 / 35 | 50 / 35 |
| corpus initialisers asked by C (both levels) / refused | 54 / **0** | 68 / **0** | 68 / **0** |

(`r1c-mac.tsv`, `r1c-d22.tsv`, `r1c-u18.tsv`, `r1c-corpus-*.tsv`; `sumr1c.py`. A nested
`selfhost/` module cannot emit alone, which is most of the corpus files that do not emit; their
group constants are `hero_os.h`'s, the runtime's own, which R1 does not ask either.)

**What it costs.** `cost.py <label dir> <name>`, CPU seconds of the children (`getrusage`), median
of three, on nine real units of this Mac (the load average read 33.69 at the start, so `real` is
not read): a filtered dump **0.023 to 0.047 s** per unit, a whole-unit dump 0.032 to 0.112 s,
against `-fsyntax-only` of the same text at 0.019 to 0.047 s and a plain `-O0` compile at 0.020 to
0.047 s (`sqlite3` 90 KB to OpenSSL 605 KB). The accessors C adds go into the probe unit R1 already
compiles, at no extra process; the probe compile with every label's accessors cost 0.041 to 0.074
s. So C costs one syntax-sized clang run per unit holding a constant that names an enumerator, a
const object or a record (two where an implicit enumerator needs the whole dump), cold; warm,
nothing, the verdict kept by R1's key. **On a real program**: `examples/curl/main.hero`, whose `CURLOPT_URL` and `CURLE_*` are enumerators in `curl.h`, builds cold in 0.58 to 0.61 s of CPU (`/usr/bin/time -p`, three runs, `user + sys`); one whole dump of its 221 KB unit costs 0.058 s (median of five), about +10 %, and a filtered one 0.022 to 0.025 s. B costs the same one run (`Bf` 0.021 to 0.041 s) and still
needs C's dump to know which offsets a bound member owns.

**What it would tell.** The dump carries the initialiser's header and line (`presumedFile`,
`presumedLine`), so the refusal can be R1's own, `ffi_constant_type` on the constant's line, with
the header's line quoted: *`MS_NOUSER` is `1 << 31` in `sys/mount.h:124`, a value C leaves
undefined*, and for a chain the line of the member that trapped, not the bound one. Unbuilt in the
compiler; the probe here prints which member trapped, and the trap's immediate names the check
(panel 216, pass 2).

## 7. The Windows box: clang 23.1.1, `x86_64-pc-windows-msvc`, SDK 10.0.26100.0

Reached by `ssh win` (it answered at 03:2x); everything I wrote there is under
`~/ffi218` (removed at the end). The box has no Python and installing one would change the
author's machine, so the population runs as a relay: `win/send/stage1.sh` on the box (preprocess
at both levels, line markers emptied by `sed`, the JSON dump, route B's four compiles, the brace
macros' expansions), `win/send2/stage2.pl` on the box (`pop.py`'s member reading ported to the Perl
5.42 and JSON::PP 4.16 that Git for Windows carries; **validated here first: on this Mac it writes
byte-identical accessor tails to the Python, on the control, curl and libuv, and the same route B
attribution on all 48 control members**), `win/send2/stage3.sh` on the box (the trap probes,
resumed after each trap), only the small tables copied back (the link carried 51,200 bytes/s,
measured over 20 s, so the 398,834,152-byte stage-1 archive could not travel). The compiler was
built there from the seed in my folder (`git archive 71c762929 seed runtime` read from the box's
own checkout, which `git diff --stat 71c762929 cbf724983 -- selfhost runtime seed` shows
identical to mine; `clang -I runtime seed/heroes.c runtime/runtime.c -Wl,/STACK:67108864`, 18 s,
`heroes 0.4.0`).

**What clang 23 says of each initialiser** (`shapes/diag-win23.txt`, `diag.sh` on the box): as
21 and 22, **except that `-Wshift-sign-overflow` never fires on `1 << 31`**, enumerator or object,
not even under `-Weverything`.

**Why, measured**: the MSVC target's driver adds `-fwrapv` (with `-fms-compatibility`; `clang
-### -c` on the box). The same clang 23 for `x86_64-pc-windows-gnu`, `x86_64-unknown-linux-gnu`
and `aarch64-apple-darwin` warns twice on `int a = 1 << 31;`, for the MSVC target 0 times, and
with `-fno-wrapv` twice again. At run time (`shapes/sgn.sh`, `1 << k` and `3 << k` with `k = 31`
under `-fsanitize=shift -fsanitize-trap=shift`): **exit 0 and `-2147483648` on the box under every
`-std` from `c11` to `gnu2y`**, exit 132 with `-fno-wrapv`; exit 133 (a trap) on Apple 21, Debian
22 and Ubuntu 18 under every `-std`. `INT_MAX + 1` traps on the box either way.

**So today's region does not hold on Windows.** `win/today/run.sh`, the trunk's compiler on the
box, one program per name: **`#define D_SHIFT (1 << 31)` builds at exit 0 and prints
`-2147483648`**, refused on the other three clangs; the 28 undefined control shapes build as
elsewhere. **And the MSVC target keeps every enumerator `int`**: `0x80000000`, `1u << 31` and the
implicit one past `INT_MAX` print `-2147483648`, `-1u` prints `-1` (`2147483648` and `4294967295`
on this Mac), the header's own value on that platform, and a binding declared `u32` there would
meet `int`.

**The Windows SDK's own instances** (`grep -rn` over `Include/10.0.26100.0`): two macros spelt
`(1 << 31)`, `VRL_ENABLE_KERNEL_BREAKS` (`winnt.h:23723`, which `windows.h` includes) and
`MI_FLAG_ADOPT` (`mi.h:607`), and one enumerator, **`eLEXTYPE_PRIVATE20 = ( 1L << 31 )` in
`sapi.h:6016`** (and `sapi51.h`, `sapi53.h`), undefined in ISO C where `long` is 32 bits, as it is
on Windows. Built today on the box (`win/sapi/sapi.hero`): **exit 0, both print `-2147483648`**.
Route C's accessor for each (`win/sapi/c.c`): **kept under the target's default, trapped (exit
132) at `-O0` and `-O2` under `-fno-wrapv`**.

Searched before calling it new: `gh issue list --state all --search` for `fwrapv`, `shift sign
windows`, `shift-sign-overflow windows`, `ms-compatibility`, `msvc enum int`, `1 << 31 windows`;
`grep -rn -i fwrapv docs/`: panel 216 weighed `-fwrapv` as its route G and refused it, and nothing
says the MSVC target already carries it. A question for the coordinator, not filed by me; by
`.claude/rules/verification.md` § Bounded discovery it reads as `blocking` (a program refused on
three legs is accepted on the fourth).

**The population on the box** (`win/send/labels.txt`: 22 C-runtime headers, 23 SDK labels from
`windows.h` to `d3d11.h`, `dxgi.h` and `xinput.h`, `hero_os.h`; all 46 contexts compiled at both
levels; `win3.py` assembles the box's tables, `analyze.py` reads them, `win/win23*-analysis.txt`),
stage 3 run twice, as the target's driver words it and again with `-fno-wrapv`:

| Windows clang 23.1.1 | the target's default (`-fwrapv`) | `-fno-wrapv` |
|---|---|---|
| unique members in scope (summed per library) | 9,981 (78,063): 7,984 enumerators with an initialiser, 1,872 without, 120 brace macros, **3 const integer objects and 2 const records** | the same |
| real members refused, B6, B7, Bf, Bp, C, C-closure | **0** | **0** |
| control, C-closure | **21 of 28** (keeps `1 << 31` in every spelling, `-1 << 3`, and the chains through them) | **28 of 28**, as on the other three |
| control, defined refused by C | 0 | 0 |
| attacks, C-closure | keeps `L_OPT` at `-O2`, `W_ENUM` with `gs.h`, `N_SHL` and both chains | **as on the other three clangs** |
| probes linked and finished (51 labels with the attacks, two levels); traps, all exit 132 | 102 of 102; 42 | 102 of 102; 55 |

The first real const objects of this whole sweep are Windows': `dwmapi.h`'s `c_DwmMaxAdapters`,
`c_DwmMaxMonitors` (16) and `c_DwmMaxQueuedBuffers` (8), `static const unsigned int`, and two
`const GUID` records in `shlguid.h`, `CLSID_CUrlHistory` and `CLSID_CUrlHistoryBoth`; all defined,
all kept, the GUIDs asked leaf by leaf. `sapi.h` was not among the labels; its enumerator was
asked by hand above. On the MSVC target the implicit enumerator after `INT_MAX` is `int`
`-2147483648` (`win/today-win.tsv`), where 18, 21 and 22 give `2147483648`; route C does not ask
it (its predecessor `2147483647` is kept), and `Bp` refuses it.

**What it means for the routes**: on Windows, as the driver words it, a sign-bit shift is not
undefined, so B and C keep `1 << 31` there while refusing it on three legs, and so does today's
region (for a `#define`) and, by the same flag, R1's probe as panel 216 specified it (its four
checks do not include `-fno-wrapv`). Asked under `-fno-wrapv` the box agrees with the other three
on every control, attack and real member measured; the cost to Windows bindings is the three SDK
values found by `grep` above (`VRL_ENABLE_KERNEL_BREAKS` and `MI_FLAG_ADOPT`, macros, R1's class;
`eLEXTYPE_PRIVATE20`, an enumerator, this one), refused where they build today.

**Not run on Windows**: `r1c.py` (it needs Python; the population relay asks the same initialisers
by the same accessor, the closure read by `analyze.py`), and the corpus there.

## 8. Verdicts

`section`: design.md §1.11 (*anything a real program needs … comes from C libraries through the
FFI*), §1.12 (*any C library must be bindable*; *a guarantee that ends quietly is not one*), §4.19
(*a `constant` … its value is the header's*, panel 038). **What design.md does not say**: whether
*the header's value* is the folded one or the one its initialiser computes. For a `#define` the
two are one text, which is why R1 can ask it; for an enumerator, a const object or a brace list
they are two, and the document is silent on which a check reads. I read §4.19 as the initialiser's,
since clang's fold of an undefined value is clang's choice and not the header's.

| route | verdict | the reason, measured |
|---|---|---|
| A, nothing | **object** (§1.12) | refuses no binding, so no veto; but glibc's `MS_NOUSER` builds and prints `-2147483648` at exit 0 on Debian 22 and Ubuntu 18 while `#define (1 << 31)` is refused, and so do the 28 control shapes: the spelling decides the verdict |
| B6, B7 (the brief's B) | **object** (§1.12) | the real headers: refuses `MS_NOUSER` only, no false refusal; but 7 and 8 of 28 control shapes, every `static const` division and float cast and every record missed on every clang (clang has no warning to raise), `= 2.9` refused by B7; and it needs C's dump anyway to know which offsets a bound member owns |
| Bf (B with C's ICE rule) | **object** (§1.12) | 14 of 28 on 21 and 22, **12 on 18** (`1 << 40`, `-1 << 3` silent there): not one rule across the floor; refuses `(1, 2)`, `(int)(1000.0 / 60)` and a const read + 1 as enumerators, values C defines (none in the measured headers) |
| Bp (B with the C23 word) | **veto** (§1.11) | refuses `EPOLLET = 1u << 31` on Debian clang 22, a value C defines that every epoll binding names, and five more defined control shapes; on 18 it does not: no spelling of that binding builds on 22 |
| K-init (new) | **object** (§1.12) | executes nothing and refuses no real member, but keeps `MS_NOUSER`, `1 << 31` in every spelling, and refuses `0.0f / 0.0f` |
| C as the brief wrote it | **object** (§1.12) | 0 false refusals, but a record's `return` is folded (0 traps), a chain hides the value one name away, an enumerator with no initialiser carries its predecessor's: 25 of 28, and the 3 it misses are silent |
| **C as built here** (rooted at the accessor, closure, predecessor rule, records leaf by leaf, inside R1's probe) | **approve, with the conditions below** (§1.11, §1.12, §4.19) | of 33,043 real members on three clangs at both levels, refuses `MS_NOUSER` and nothing else, and none of the box's 9,981 under either reading of the shift; control 28 of 28 and 0 of 28 defined; every attack right; the corpus 0 of 54 and 68 initialisers asked; no library loaded and no sanitizer runtime (R1's trap probe, variant S); the dump is the instrument `cli/pointee.hero` already uses; one syntax-sized clang run per unit, cold |

**No route here needs surface, a new diagnostic class or a spec token**: C's refusal is
`ffi_constant_type`, R1's own; what it owes is a §4.19 sentence saying the value asked is the
initialiser's.

**Conditions, which turn C to approve**:
1. **Inside R1's probe, never beside it**: the same preprocessed unit, the program's level and
   words, the four checks in trap mode, the resuming probe, variant S's link. My two vetoes of panel
   216 carry over unchanged: a form that must load the program's libraries, or that needs the
   sanitizer runtime, is vetoed.
2. **Rooted at the accessor's own dump**, the closure through every `DeclRefExpr` to an
   `EnumConstantDecl` or a const `VarDecl`, the predecessor rule, a record leaf by leaf at the
   member's converted type; a leaf that is not a scalar is not asked, and the compiler's comment says
   so.
3. **A two-sided canary of the dump step** in every probe that asks one: a planted `1 << 31`
   enumerator whose re-asked text must trap and a planted `1 << 30` that must not; a dump that yields
   no text for a bound enumerator refuses with words naming the dump and clang's version, never
   *C leaves the value undefined*.
4. **The message names the member that trapped and its header line** (`presumedFile:presumedLine`
   of the dump), at the constant's own line: for `IB`, *`IA = 1 << 31` at `impl.h:2`*.
5. **The constant question asked under `-fno-wrapv` on every target**, the probe's words only, the
   program's own compile untouched: the MSVC target's driver adds `-fwrapv`, under which clang 23
   neither warns on nor traps `1 << 31`, and C keeps 7 of the control's 28 there (section 7). With
   it, the box agrees with the other three legs everywhere measured. The same word is owed to R1's
   probe and to today's region, or a `#define (1 << 31)` stays accepted on Windows alone.

**Prediction**: built into the compiler as 1 to 5, over the same lists: `MS_NOUSER` refused on
Debian 22 and Ubuntu 18 and nothing else of the 33,043 members, at `-O0` and `-O2`, and nothing of
the box's 9,981; on the box the control's 28 refused, `win/sapi/sapi.hero` refused naming
`sapi.h:6016`; the 56 control
programs 28 refused and 28 built; `linux/ms/ms.hero` exits 1 with `ffi_constant_type` naming
`sys/mount.h:124` and `linux/ms/ep.hero` builds and prints `2147483648`; no program of the corpus
that builds today stops building. Instrument: the built `heroes` over `ctlprog/`, `atk/`,
`linux/ms/`, `pop.py`'s lists through `uxcorpus.py`'s binding generator, and the net's `run` and
`emission`. Any one failing falsifies it.

**What would turn me**: to object, a real member C defines that C traps on any leg, the Windows box
included, a clang on the floor whose dump loses an initialiser's text, or an author's ruling that
the MSVC target's `-fwrapv` reading is the program's meaning on Windows (then condition 5 falls and
C is right on three legs only, which I would then record as a known platform split); to veto, any form that
needs a library loaded or a runtime installed at build time, or that refuses a correct real
binding. B would turn approve only if clang gained a warning for `INT_MIN / -1` and a float cast in
an initialiser on the floor's clang, which none of 18, 21, 22 has.

## 9. Housekeeping

Worktree `.claude/worktrees/218-ffi-pragmatist` removed (`git worktree remove --force`);
containers `ffi218-d22` and `ffi218-u18` stopped and removed; the box's `~/ffi218` removed, the
box's own checkout `C:\w\heroes` read by `git archive` and `git cat-file` only. No paid run, no
API call, no credential printed; `gh` used for read-only searches. Nothing written in the trunk but
this file.
