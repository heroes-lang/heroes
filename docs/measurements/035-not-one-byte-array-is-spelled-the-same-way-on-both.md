# 035 — Not one byte array is spelled the same way on both platforms

Date: 2026-09-18 · M-declared-extents step 1 · **the measurement panel 164
queued route 6 behind**, run at last.

## Why this file exists

Panel 164 refused routes 1, 2 and 5, adopted routes 3 and 4, and wrote of the
sixth: *"Its whole value is a number nobody has measured: how many real headers
spell the parameter as an array. **Queued, not adopted**, because adopting an
unmeasured route is the cheap move."* `docs/ROADMAP.md` row 61 carries the same
sentence. This is that number, and it turned out to be four numbers, because the
question has a platform in it that nobody had noticed.

Route 6 is `function arr_len(s: i8[8])` in an `extern` group: the parameter
carries the extent, as C's own header spells it, and the Heroes compiler
**checks** it. It is the only route of the seven where the extent is checked
rather than trusted.

## The instrument, and why the obvious one cannot answer

**C deletes the number before clang builds a tree.** A parameter written
`T a[N]` is adjusted to `T *a` (C11 §6.7.6.3p7), so the extent is not part of the
type. Measured here first, because it decides the instrument:

```
$ clang -fsyntax-only -Xclang -ast-dump=json probe.c
f_ptr    {"qualType": "char *"}        # void f_ptr(char *b);
f_arr8   {"qualType": "char *"}        # void f_arr8(char b[8]);
f_arrN   {"qualType": "char *"}        # void f_arrN(char b[]);
f_arr2d  {"qualType": "char (*)[8]"}   # void f_arr2d(char b[4][8]);
f_static {"qualType": "char *"}        # void f_static(char b[static 16]);
```

`f_ptr` and `f_arr8` are indistinguishable in the AST, in both the JSON and the
text dump. Only the 2D case keeps a bound, because only the outermost dimension
decays. **So an AST-shaped instrument answers a different question than the one
asked**, and would have answered it as zero.

What survives is the parameter's **source range**, which clang does report. The
census is therefore two stages, and the file is `scratchpad/extents/census.py`,
run identically on every platform:

1. a text screen over every `.h` — strip comments and literals, find balanced
   paren groups containing `[` that are followed by `;` or an attribute;
2. `clang -Xclang -ast-dump=json` per candidate header, then **read the header's
   own bytes back through each `ParmVarDecl`'s source range**.

Coverage is reported rather than assumed, because the first run of stage 2
parsed only 40 of 127 candidates. The 87 failures were diagnosed, not excused:
**80 of them wanted a prerequisite header**, not C++ and not Objective-C. A
prelude of thirteen standard headers took the run to 127 of 127.

## What was counted, and where

| | Darwin arm64 (Xcode SDK) | Linux arm64 (glibc) | Linux x86-64 (glibc) |
|---|---|---|---|
| `.h` files walked | 3120 | 5404 | 5411 |
| candidate headers | 127 | 333 | 334 |
| parsed / failed | **127 / 0** | **333 / 0** | **334 / 0** |
| **parameters spelled as an array** | **82**, in 29 files | **182**, in 44 files | **182**, in 44 files |

**The two Linux legs are the same SET, not merely the same count.** Normalising
the `<triple>/` directory and differencing: 0 rows on each side, `identical sets:
True`. Twelve of the 182 live in an arch-specific directory and all twelve are
POSIX signatures. Array spelling in glibc does not vary by architecture, and that
is measured rather than supposed.

## The split that decides the route

Route 6 can only check an extent the compiler knows. Splitting the parameters by
what stands between the brackets:

| the extent | Darwin | Linux |
|---|---|---|
| **fixed** — a literal, or a macro that is one | **31** | **59** |
| unsized — `[]`, or only a qualifier such as `[__restrict]` | 42 | 115 |
| named in a **sibling parameter** — `[_LIBC_COUNT(n)]`, `[BROTLI_ARRAY_PARAM(sz)]` | 9 | 8 |

All 14 named extents on Linux were opened and read: `L_tmpnam`, `IF_NAMESIZE`,
`SEED_BLOCK_SIZE`, `CAMELLIA_BLOCK_SIZE`, `LLVM_BLAKE3_KEY_LEN` — macro
constants, every one. All 8 `EXPR` extents name a sibling parameter and are
counted with the siblings.

So **roughly a third of array-spelled parameters carry an extent at all**: 31 of
82 on Darwin, 59 of 182 on Linux.

## Two thirds of the Linux advantage is a package, not a platform

The container carries 140 OpenSSL headers; the Xcode SDK carries **0**. Twenty of
Linux's byte cases are `openssl/modes.h`'s `unsigned char ivec[16]`. Separating
base system headers from installed packages:

| fixed-extent parameters | Darwin | Linux |
|---|---|---|
| all headers present | 31 | 59 |
| **base system headers only** | **20** | **25** |

The two numbers a Heroes program meets before it installs anything are therefore
**20 and 25**, and they are close. The gap between 82 and 182 is Debian's package
list.

## The finding, and it is not a count

**Of the fixed-extent parameters that take BYTES, the set both platforms spell as
an array is EMPTY.**

- byte-typed, fixed extent: **12** on Darwin, **31** on Linux;
- function names common to both: **none**.

The eight fixed-extent functions both platforms do spell as arrays are
`erand48`, `jrand48`, `nrand48`, `seed48`, `lcong48` (`unsigned short[3]` and
`[7]`), `pipe` (`int[2]`), `futimens` and `utimensat` (`struct timespec[2]`). Not
one of them takes bytes — and bytes are the whole reason panel 164 sat.

**The same function is spelled two ways, and the extent is a different number.**

```
Darwin  _stdio.h:289   char *_LIBC_CSTR tmpnam(char *_LIBC_COUNT(L_tmpnam));
glibc   stdio.h:211    extern char *tmpnam (char[L_tmpnam]) __THROW __wur;

$ ./sizes       (C, compiled and run on each)
Darwin        L_tmpnam=1024  IF_NAMESIZE=16
Linux glibc   L_tmpnam=20    IF_NAMESIZE=16
```

`L_tmpnam` is **1024 on Darwin and 20 on glibc**. A Heroes author reading the
glibc header would write `function tmpnam(s: i8[20])`, and the compiler would
check the 20 — confidently, and wrong by a factor of 51 on macOS. The check is
sound about the program and false about the world.

`if_indextoname` is the same shape from the other side: glibc writes
`char __ifname[IF_NAMESIZE]`, Darwin's `net/if.h:448` writes
`char *if_indextoname(unsigned int, char *)`. One POSIX function, one extent
(16 on both, measured), and only one vendor spells it.

## What the language already does, run on both platforms

`spec § 13`: *"A group's `constant` has no body: the header holds the value."*
So the portable extent is already reachable, and this is the program, not a
paraphrase of one:

```
extern "stdio.h"
    constant L_tmpnam: i64

extern "net/if.h"
    constant IF_NAMESIZE: i64

function main()
    print("L_tmpnam=", to_str(L_tmpnam), " IF_NAMESIZE=", to_str(IF_NAMESIZE))
```

```
Darwin arm64, ./heroes run          L_tmpnam=1024 IF_NAMESIZE=16
Linux arm64, built from the seed     L_tmpnam=20   IF_NAMESIZE=16
```

The Linux leg is a container build of `clang -I runtime seed/heroes.c
runtime/runtime.c -o heroes`, exit 0, then `./heroes run extent.hero`. **The
mechanism that route 6 would duplicate as a literal already exists and is already
portable**, and the value it yields is the one the platform actually has.

## Principle 0, counted from this tree

```sh
grep -rhn '^    function [a-zA-Z_]' --include='*.hero' . | sed -E 's/.*function ([a-zA-Z0-9_]*).*/\1/' | sort -u
```

**172** `extern` functions are declared in this repository. Intersected with both
censuses, **exactly one** is spelled with an array parameter by a real header:
`tmpnam`, on glibc only. That one declaration is
`tests/golden/check/owned-freer-must-be-declarable.hero:37`, a diagnostic fixture
that declares `buffer: ptr` and exists to provoke `freer_arity`.

**So no working binding in this tree would change under route 6, and none would
have been written differently.** Zero corpus programs, which is what panel 164's
compiler-engineer and spec-warden said of route 3 before it was adopted on other
grounds.

## What this measurement does NOT settle

- **It does not refuse route 6.** It prices it: in the base C the language meets,
  the route serves `pipe(int[2])` and the `drand48` family well, serves no byte
  parameter on both platforms, and invites a platform-varying literal at the one
  place an author is most tempted to write one. The verdict is a panel's.
- **The corpus is two platforms of four.** Windows and its headers were not
  walked; `.claude/rules/platforms.md` would have them run rather than reasoned
  about, and they are **unrun** here.
- **`_LIBC_COUNT` is not counted as a spelling.** Darwin states extents in an
  annotation on a pointer, 9 times in this corpus. Whether Heroes should read
  that annotation is a different route, which no sitting has listed.
- **The screen's false-negative rate is not measured.** A declaration produced
  entirely by a macro body would be attributed to the macro's own file; none was
  seen, and none was looked for with an instrument.
