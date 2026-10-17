# Panel 218, the completeness critic

Written 2026-10-17 from before 09:52 (the lane compiler's file time, `ls -la`), in a detached worktree of the lane's commit `b4ae85f63`
(`.claude/worktrees/218-critic`; `git log -1 --format=%H` there printed
`b4ae85f63f7e3eb9754d3216568fba6849123be7`), `build/` removed. Two compilers built there: the seed's,
`clang -I runtime seed/heroes.c runtime/runtime.c -o heroes-seed` (`real 8.24 user 7.99 sys 0.14`,
load 1.95), which is the trunk's compiler since the lane's seed was not regenerated (`grep -c
value_ask seed/heroes.c` prints 0); and **the lane's, R1 built**, `./heroes-seed build
selfhost/main.hero -o heroes` (`real 74.71 user 58.48 sys 11.69`). Apple clang 21.0.0
(clang-2100.3.34.2). Every script and output is under `<scratchpad>/218-critic/`, `<scratchpad>`
being `/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/5f7a29c1-133b-4b38-877a-3b0496db6842/scratchpad`.
Every number names its command; what I could not run says *unrun*. Written as I go.

**A harness fault of my own, repaired before any number was read**: my first loop ran under zsh,
which does not split `"$nt"`, and every row was garbage; rerun under `bash`.

## 1. R1 as the lane built it, on the brief's shapes and on `__builtin_constant_p` (question 3)

`r1/x.h`, one program per name (`r1/mk.sh`: `extern "x.h"`, `constant NAME: T`, `print`), built with
the lane's compiler at `-O0` (`heroes build`) and `-O2` (`heroes build -O2`, the level `heroes run`
compiles at), each binary run three times (`r1/r1.txt`):

| name | header | `-O0` | `-O2` |
|---|---|---|---|
| `E_SHIFT` | `enum { E_SHIFT = 1 << 31 }` | 0, `-2147483648` x3 | 0, the same |
| `S_DIV` | the brief's `static const` | 0, `1` x3 | 0, the same |
| `D_SHIFT` | `#define D_SHIFT (1 << 31)` | 1, `ffi_constant_type` (the region) | 1, the same |
| `R_BRACE` | `#define R_BRACE {(-2147483647 - 1) / -1, 2}` | 0, `-2147483648 2` x3 | 0, the same |
| `R_FLT` | `{(int)1e20, 3}` | 0, `2147483647 3` x3 | 0, the same |
| `M_BCPV` | `#define (__builtin_constant_p(g_var) ? (INT_MIN / -1) : 5)`, `static int g_var = 3` | 0, `5` x3 | **1**, R1's `ffi_constant_type` |
| `M_BCPM` | the mirror, `? 5 : (INT_MIN / -1)` | **1**, R1 | 0, `5` x3 |
| `M_BCPF`, `M_BCPMF` | the same over `g_get()`, a `static inline` returning 4 | 0 (`5`); 1 | 0 (`5`); 1 |
| `M_BCPO` | `#define (__builtin_constant_p(2147483647 + g_one) ? 7 : 7)`, `g_one` 1 | **1**, R1 | **1**, R1 |
| `M_BCPZ` | `(__builtin_constant_p(g_one / 0) ? 7 : 7)` | 1, the region's *division by zero* | 1, the same |

So the brief's table holds on the lane's compiler: R1 changes none of `E_SHIFT`, `S_DIV`,
`R_BRACE`, `D_SHIFT`.

**Is R1 sound on the compiler-engineer's `__builtin_constant_p` shape at `-O2`? Yes, measured.** The
program without R1 (the seed's compiler, `r1/seed/seed.txt`) prints `5` for `M_BCPV` at both
levels, which reads at first as R1 refusing a defined value at `-O2`. It is not: the defined twin
`#define M_BCPT (__builtin_constant_p(g_var) ? 9 : 5)` prints **`5` at `-O0` and `9` at `-O2`**, so
the program at `-O2` takes the branch that divides `INT_MIN` by `-1`, and its `5` is the optimiser
discarding an undefined branch. The IR says the same (`clang @words -S -emit-llvm` of the program's
own unit and of R1's preprocessed probe text, `r1/seed/p_M_BCPV-O2/*.ll`): the program's accessor is
`ret i32 5`; the same text without the checks, `ret i32 5`; with the four checks, an unconditional
`llvm.ubsantrap`. R1 compiles a macro's accessor at the program's level, and a macro's value IS
computed at that level by the program's own code, so R1 refusing `M_BCPV` at `-O2` and `M_BCPM` at
`-O0` is the program's own verdict, level by level.

**FOUND: R1 refuses a value C defines, at both levels: an undefined operation inside
`__builtin_constant_p`'s argument.** `M_BCPO` is `7` whichever way the builtin answers, and clang
treats the argument as unevaluated (the seed's compiler builds it, `7 7 7` at both levels). R1
refuses it: its probe's `verdict.txt` reads `trap 2`, and the probe's accessor at `-O0` holds
`llvm.sadd.with.overflow.i32(i32 2147483647, ...)` and `llvm.ubsantrap` before `llvm.is.constant`
(`clang -x cpp-output ... -O0 <the four checks> -S -emit-llvm` of the lane's own `probe.i`): clang
computes the argument, under the checks, to hand it to `llvm.is.constant`, at `-O0` too. The told
words are *C leaves it undefined*, which is false. New with R1: the trunk builds it. Its population
is panel 216's: 0 of 41,114, 43,827 and 42,086 bound real macros flagged, so no real header measured
holds it. The region has the older twin, `M_BCPZ`, refused on the trunk as well.

## 2. FOUND: R1 as built says nothing when its probe does not link (`unread`)

`cli/value_ask.hero`'s `asked` returns `fail(code: "unread")` when the unit does not preprocess or
the probe does not compile or link, and `told` reads only `broken` as a failure: an unread probe
`continue`s, telling nothing (lines 97 to 102, 163 to 165, 232 to 234). Measured
(`unread/ctor/`): a header whose constructor calls a library function,

```c
#include <limits.h>
#include <zlib.h>
static void __attribute__((constructor)) hero_lib_init(void) { (void)zlibVersion(); }
#define K_BAD (INT_MIN / -1)
```

bound by `extern "u.h" link "z"`: the lane's `heroes build` **exits 0 and prints `1861247980
1797694444 1800987628`** over three runs at `-O0`, and `0 0 0` at `-O2`: defect 2075's own symptom,
through R1. The same header without the constructor (`unread/plain/`): exit 1, R1's
`ffi_constant_type`, at both levels. Why, by hand over the lane's own `probe.i` and `driver.c`: the
compile exits 0 and the link says `Undefined symbols for architecture arm64: "_zlibVersion",
referenced from: _hero_lib_init in p.o`, since a constructor is a root `-dead_strip` keeps, and the
probe links the runtime alone. No `verdict.txt` is kept, so every build asks again and is silent
again. Every route of this sitting rides this probe, so each inherits the hole.

## 3. The two seats' prototypes, attacked with the shapes beside (questions 2 and 3)

`atk/f.h`, 22 names (`atk/names.txt`), one program per name (`atk/mk.sh`, every header copied);
today's verdict from the seed's compiler (`atk/today.txt`) and the lane's (`atk/lane.txt`):
**`diff` of the two prints nothing**, so R1 as built moves none of them. Then the compiler-engineer's
C2 as he left it (`218-ce/tools/route.sh`, `READER=initc3.py C2LEVEL=-O0`, at program levels `-O0`
and `-O2`: `atk/c2-O0.txt`, `atk/c2-O2.txt`) and the ffi-pragmatist's C (`218-ffi/r1c.py`, compiled
at the program's level: `atk/ffi-O0.txt`, `atk/ffi-O2.txt`), both unchanged, both fed by the seed's
`--emit-c`:

| name | the header | C's value, today's print | C2 `-O0`/`-O2` | ffi C `-O0`/`-O2` |
|---|---|---|---|---|
| `S_BCPV` | `static const int = __builtin_constant_p(g_var) ? (INT_MIN / -1) : 5` | defined, `5` | kept / kept | kept / **trap** |
| `S_BCPM` | the mirror | undefined, `-2147483648` | trap / trap | trap / **kept** |
| `S_BCPO`, `E_BCPO` | `__builtin_constant_p(2147483647 + g_one) ? 7 : 7`, object and enumerator | defined, `7` | **trap / trap** | **trap / trap** |
| `TB` | `enum T { TA = 2147483647, TB = TA + 1, TBIG = 0x100000000LL }` | undefined, `-2147483648` | **kept / kept** | **kept / kept** |
| `T3B` | `T3A = INT_MIN, T3B = T3A - 1`, same `long` enum | undefined, `2147483647` | **kept / kept** | **kept / kept** |
| `T4B` | `T4A = 65536, T4B = T4A * 65536`, same | undefined, `0` | **kept / kept** | **kept / kept** |
| `B1` | `ch_b.h`: `B1 = A1 + 1`; `ch_a.h`: `A1 = 1 << 31` (two headers) | undefined, `-2147483647` | trap / trap | trap / trap |
| `IN_ANON` | `struct holder { enum { IN_ANON = INT_MAX + 1 } k; int v; }` | undefined | trap / trap | trap / trap |
| `EU_PROMO`, `S_UPROMO` | `(unsigned short)65535 * (unsigned short)65535` (promoted to `int`) | undefined | trap / trap | trap / trap |
| `EU_WRAP`, `S_UWRAP`, `S_UPROMO_OK` | `(int)(0xFFFFFFFFu + 2u)`, `0u - 1u`, `(unsigned short)65535 * 65535u` | defined | kept / kept | kept / kept |
| `E_LVL` | `1 << LVL_SHIFT`, 31 under `__OPTIMIZE__`, else 30 | `1073741824` / `-2147483648` | kept / trap | kept / trap |
| `E_SZ`, `E_DEAD`, `S_AND`, `M_SZ`, `M_DEAD` | `sizeof(EA)`, `0 ? EA : 5`, `0 && EA`, the two as macros; `EA = 1 << 31` | `4`, `5`, `0`, `4`, `5` | trap (EA's) | trap (EA's) |
| `R_CALL` | `#define R_CALL { g_divmin(), 2 }`, a `static inline` dividing `INT_MIN` by `-1` | `-2147483648 2` at `-O0`, **`0 2` at `-O2`** | trap / trap | trap / trap |
| `R_CALL_OK` | `{ g_ok(), 2 }`, `g_ok` returns 7 | `7 2` | kept / kept | kept / kept |

What it says:

- **The conflict between the seats is settled by the ffi-pragmatist's own prototype**: compiled at
  the program's level it refuses `S_BCPV` (C defines 5) and keeps `S_BCPM` (undefined) at `-O2`,
  the compiler-engineer's finding reproduced on the other seat's code. An initialiser's value is
  computed by clang's constant evaluator, where `__builtin_constant_p` of what it cannot fold is
  false at every level; a macro's value is computed by the program's code, at its level (section 1).
  So **R1 at the program's level and C2 at `-O0` are each right for their own evaluator, and they
  do not conflict**: the condition is that C's probe is `-O0` and R1's stays at the level.
- **FOUND, both prototypes: the enumerator whose type moves when its enum completes is missed in
  silence.** Measured with `clang -Xclang -ast-dump` and `_Generic` (`probe/t.c`): inside `enum T`'s
  body `TA` is `int`, so `TA + 1` overflows (`-Winteger-overflow`, value `-2147483648`); after the
  `}` every enumerator of `T` is `long`, and `TA + 1` at the end of the unit is `2147483648`. Both
  prototypes append the sliced text `TA + 1` at the end of the unit (C2's `p_TB/init.c`:
  `return (__typeof__(TB))(TA + 1);`), so they ask another expression than the one clang folded.
  The text moved out of its context is the root; `__builtin_constant_p` is one instance, this is
  another. Real headers measured: unrun (no seat's population counts an enum whose type is wider
  than `int` and whose body does arithmetic on an earlier member).
- **Both prototypes refuse `S_BCPO`/`E_BCPO`**, the false refusal R1 makes of `M_BCPO` (section 1).
- **References whose value is never read** (`sizeof(EA)`, a dead branch, `0 &&`) are refused by
  both, through `EA`'s own trap. Whether that is a false refusal is **a question rather than a
  premise**: the header holds `EA = 1 << 31`, a constraint violation of C11 § 6.6 p4 for the whole
  unit (my reading, not a run), and the value bound is one C would compute. Neither seat names it.
- **Measured right by both**: the chain through two headers, the anonymous enumerator inside a
  struct, the promotion of `unsigned short` to `int`, unsigned arithmetic C defines, an enumerator
  whose value the level's macro changes.
- **`R_CALL` is outside R1 as the lane built it**: both seats' prototypes ask a record accessor
  (their `R1` column traps it), the lane's `emit/value_probe.hero` `judged` returns false for every
  record (`record_constant.is_record_value`, line 96), and the lane builds `R_CALL` at exit 0 with a
  value that moves by level. Panel 216's R1 text says *an integer, `f64` or record constant*. And a
  record whose leaf is a call builds at all (`R_CALL_OK`, `7 2`), where `#define M_CALL (g_ok())` is
  refused `ffi_not_constant` (`r1/seed/`, the seed's compiler): the asserted constness that refuses
  a scalar call has no record twin.

## 4. `-fwrapv` (question 4), with the box answering

The box answered `ssh win` this morning, before 10:15 by `date` (clang 23.1.1, `x86_64-pc-windows-msvc`). Everything I ran
there read stdin and wrote stdout: no file written on the box.

- **Which clangs add it.** `clang --target=x86_64-pc-windows-msvc -### -c` (`wrapv/s.c`): Apple
  clang 21.0.0 here and Debian clang 22.1.8 (`heroes-linux-arm64:latest`) add **no** `-fwrapv` for
  that target, nor for `-gnu`, Linux or Darwin; the box's 23.1.1 adds `-fms-compatibility -fwrapv`
  (`echo 'int a;' | ssh win 'clang -x c - -### -c'`). So issue 2114's split is a clang version's,
  not only a target's: a Windows leg on clang 22 or older refuses `D_SHIFT` as this Mac does. **The
  CI's Windows leg's clang version: unrun** (`gh api .../jobs/117254053272/logs` returned 0 bytes;
  the Linux legs' kept logs read Ubuntu clang 18.1.3).
- **What `-fwrapv` does to R1's four checks**, IR of one function per operation with constant
  operands (`wrapv/ovf.c`, `-O2`, the four checks in trap mode), on the box under its default and
  under `-fno-wrapv`, and on this Mac under `-fwrapv` and `-fno-wrapv`:

  | | `INT_MAX + 1` | `-INT_MIN` | `65536 * 65536` | `INT_MIN / -1` | `1 << 31` |
  |---|---|---|---|---|---|
  | box default (`-fwrapv`), Mac `-fwrapv` | trap | trap | trap | trap | **kept** |
  | box and Mac `-fno-wrapv` | trap | trap | trap | trap | trap |

  At `-O0` the Mac's `-fno-wrapv` IR holds the sign-bit test (`lshr`, `icmp eq 0`) and `-fwrapv`'s
  does not, on the box as here; run on this Mac, `1 << k` exits 0 under `-fwrapv` and 133 under
  `-fno-wrapv`, while `INT_MAX + 1` exits 133 under both. The region's `-Wshift-sign-overflow` on
  the box: 0 warnings by default, 1 under `-fno-wrapv`.
- **So the `-fwrapv` reading cannot be what R1 tells.** `-fwrapv` defines `INT_MAX + 1`, `-INT_MIN`
  and `65536 * 65536` as wrapping, and R1's `signed-integer-overflow` traps them under it all the
  same; it keeps `1 << 31` alone. An author's ruling that `-fwrapv` is Windows' meaning (the
  ffi-pragmatist's turning point) would need R1 to keep three shapes its checks refuse, which no
  word of clang's sanitizer separates from `INT_MIN / -1` (the same check). Under `-fno-wrapv` the
  four checks trap all five on the box as on this Mac (the table); the lane's R1 itself run on the
  box: **unrun** (it would write files there).
- **What it costs, counted wider than one spelling.** The ffi-pragmatist's `grep` for `1 << 31`
  over the SDK reproduces (`5` lines, three names). Every object-like `#define` with a literal
  shifted by 24 to 31 (`wrapv/sdk-shifts.txt`, 45 lines, filtered by the arithmetic in Python)
  adds **`D3DSTREAMSOURCE_INSTANCEDATA (2<<30)`, `shared/d3d9types.h:800`**, outside the seat's 23
  SDK labels. So four names a Windows binding builds today and would refuse: three macros (R1's) and
  one enumerator (this sitting's). Other spellings (`<< 0x1F`, a shift of a macro's value): unsearched.
  R1's own population over the SDK's macros under `-fno-wrapv`: **unrun** (the seat's Windows
  population was enumerators, const objects and brace macros).
- **What each route does on the box**: today's region keeps `1 << 31` there (measured: 0 warnings);
  B keeps it (the ffi-pragmatist's measurement, no warning fires); the ffi's C keeps it (his
  measurement, 21 of 28 under the target's default). R1 as built and C2 keep it **by inference, not
  run**: the probe compiles under the unit's words (`value_ask.hero:152-154`, no `-fno-wrapv`) and the
  box's IR under its default words holds no sign-bit test (above). Each, with `-fno-wrapv` in the
  probe's words alone, would trap it as on the three other legs, by the same IR. The region cannot
  take the word without changing the program's own compile, since it IS the program's compile: the
  ffi-pragmatist's condition 5 (*the same word owed to R1's probe and to today's region*) asks the
  program's own compile to change after saying it stays untouched. Once R1 judges every macro the
  region does, the region's silence on the box costs nothing; until then it is issue 2114.

## 5. FOUND: R1 as built compiles another program than the build does, by `-dD`

The lane preprocesses the unit with `clang -E -dD` (`value_ask.hero:180`), keeping every `#define`
in the text so `value_told.hero` can quote the header's line, and compiles that text as
`-x cpp-output`, which **expands the macros a second time**. A self-referential macro then grows by
one expansion (`dd/d.c`): `enum { K_W = 2147483646 }; #define K_W (K_W + 1)` reads `(K_W + 1)` after
`-E`, and the `-dD` text compiled again reads `((K_W + 1) + 1)`: `return K_W;` returns
**-2147483648** from the `-dD` text (482 `#define` lines) and **2147483647** from the plain `-E`
text (0 `#define` lines), `clang -x cpp-output` of each, linked and run. End to end (`dd/`): the
seed's compiler builds `K_W` and prints `2147483647` x3 at both levels; **the lane's refuses it at
both levels, *C leaves it undefined***, a value C defines told as undefined. `K_V` (`#define K_V
(K_V * 2)`, enum 5) prints `10` on both, its second expansion not overflowing. A second expansion
only adds operations, so it can only add traps: false refusals, never a silent miss (my reasoning
from the shape, not a run over shapes). The identity macros the C library writes (`#define
MS_NOUSER MS_NOUSER`) expand to themselves and are untouched. Real headers with a non-identity
self-reference: unsearched. Both seats' prototypes of this sitting preprocess with `-E` alone
(`route.sh` and `r1c.py`, read), so their numbers cannot show it.

**And it reaches the repository's own programs, unseen by the lane's gate.** Every `run` golden and
example that binds a constant through a group, 43 files (`grep` of `constant NAME:` and `extern`),
built with the lane's compiler at `-O0` (`gate/table.txt`): 41 exit 0, 2 exit 1 (`fixedbugs-437`,
`fixedbugs-589`, `sys/prctl.h` absent here); **33 probe folders, 32 verdicts kept, one probe
unread** with the build at exit 0: `run/fixedbugs-361-a-header-s-macros-never-reach-the-program-s-own-c`.
Its header defines `hero_print_int(x)` as `nothing`, which the unit guards with `push_macro` and
`pop_macro`; the `-dD` text carries that `#define`, the second pass rewrites the program's own
calls, and the probe's compile says `use of undeclared identifier 'nothing'` three times (by hand,
with the lane's own `words-*.rsp`). The same unit preprocessed without `-dD` (0 `#define` lines)
compiles with 0 errors; its link and run, unrun. So its constant `LIMIT` was never judged, and the
lane's gate line *run 531/0 (every run program's probe, none refused)* counts an unread probe as
judged: section 2's silence and this section's second expansion meet in a case the tree already
holds.

## 6. Numbers re-run, and what held

- **The brief's table**, on the lane's compiler and the seed's (section 1): held.
- **The compiler-engineer's 35 and 14 shapes** (`rerun/`, his `f.h`, `names.txt`, `mk.sh` and
  `route.sh` unchanged, `READER=initc3.py C2LEVEL=-O0`): the lane builds 34 of 35, refusing `D_SHIFT`
  alone, and all 14; C2 traps 16 and keeps 18, and 10 and 4, at both levels; **`diff` of the
  verdicts, name by name, against his `c5-O0.txt` and `c5-O2.txt`: 0 lines**, both sets, both levels.
- **His clang 18 row** (`critic218-u18`, `ubuntu:24.04` with `clang libc6-dev` without recommends,
  Ubuntu clang 18.1.3; `clang -std=gnu11 -fsyntax-only -pedantic-errors` over his `f.h`): clang 21
  refuses `E_MUL E_DIV E_ADD E_FLT E_NEG` of the class, clang 18 `E_DIV E_FLT`; both refuse the five
  defined `E_UNEG E_UMAX E_COMMA E_FDIV E_HEX`. Held.
- **His size**: 116,707 code lines in 653 files at `cbf724983` by the `layout` rule (an `awk` of
  `code_lines`, `git archive cbf724983 selfhost`): held. The lane: 117,279 in 656; R1's three new
  modules **518** (`value_ask` 263, `value_told` 77, `value_probe` 178). His estimate of Ux-whole at
  panel 216 was *F2's 239 plus 40*; R1 is Ux-whole with the trap mode, variant S and the resuming
  probe, so not the same thing, but it calibrates his 300 to 450 for C2: the last estimate of this
  kind was about half the built size. `initc2.py` 92 lines and `scan.hero` 58: held.
- **His SDL3 count**: both numbers reproduce (`clang -ast-dump` of his `big/SDL3_SDL_h.i`: 1,174
  `EnumConstantDecl` lines; `-ast-dump=json`: 12,033,808 bytes, 1,193 `"kind": "EnumConstantDecl"`).
  They are not two counts of enumerators: the JSON's 1,193 holds the `referencedDecl` stubs a
  `DeclRefExpr` carries, which share their declaration's `id`. So *enumerators 1193* in his scan
  is enumerators plus references, and **a one-pass byte reader keyed on `kind` and `id` meets the
  stub as well as the declaration**; `initc3.py` indexes only the `inner` tree, which is why it is
  right.
- **The ffi-pragmatist's**: `MS_NOUSER = 1 << 31` at glibc 2.41's `sys/mount.h:124`, `#define
  MS_NOUSER MS_NOUSER` under it, no warning under `-Wall -Wextra` nor `-Wshift-sign-overflow`;
  `EPOLLET = 1u << 31` at `sys/epoll.h:66`; `__STDC_IEC_559__` 1 on Debian, 0 here;
  `-Wshift-count-overflow` and `-Wshift-negative-value` silent on an enumerator and firing in a
  function, on 21 and 18; the box's `-fwrapv` and the region's silence there; the SDK's three
  `1 << 31` names. All held.
- **Unrun by me**: the populations (33,043 members; panel 216's 41,114, 43,827 and 42,086 macros),
  the census of 57 roots, every timing (the machine ran my background jobs; nothing was timed), the
  Windows population, and anything on the box that would write a file there.

## 7. Routes nobody listed

- **K23, C23's `constexpr`** (executes nothing; `k23/`): `constexpr <T> hero_k = (TEXT);` under
  `-std=c23`. Over 10 undefined and 12 defined shapes, Apple clang 21 and Debian clang 22 alike:
  refuses 9 of 10, **misses `1 << 31`** (the K-init hole), keeps 11 of 12 (`2.9` into `int` refused
  only because I converted it implicitly). Beside them (`k23/ctx.c`, `ctxm.c`): keeps `S_BCPV` and
  `S_BCPO` right, since it evaluates in a constant context as the initialiser did; **keeps
  `S_BCPM`'s undefined `-2147483648`**, because clang folds a `__builtin_constant_p(...) ? a : b`
  permissively even in a `constexpr` (`0 ? 5 : (INT_MIN / -1)` is refused); keeps `TB` (the text
  still moved). **Ubuntu clang 18.1.3: `unknown type name 'constexpr'` on all 22.** Set aside: the
  floor lacks it, and it misses the brief's first shape.
- **The value-agreement guard** (a repair of C, measured; `agree/agree.sh`): every probe that does
  not trap is compared byte for byte with the declaration it re-asks, whose value is clang's fold.
  Over C2's own probes at `-O0`, 43 did not trap: **40 agree (19 of the compiler-engineer's 35-shape
  set, 6 of his 14, 15 of mine) and the 3 that disagree are `TB`, `T3B`, `T4B`**; compiled at `-O2`
  (the ffi-pragmatist's level), `S_BCPM` disagrees as well, and nothing else; no NaN false alarm on arm64 or x86-64 (`agree/nan.c`, a literal `0.0 / 0.0` and
  one read through a `static const`). It cannot rescue a probe that traps where the fold had no
  undefined step (`S_BCPV` at `-O2`, `S_BCPO`), so it stands beside the `-O0` condition, not instead.
- **References at their own type** (a repair of the enum drift, measured by hand; `probe/fix.c`):
  the dump gives each `DeclRefExpr`'s type at the reference (`TA` is `int` inside `TB`'s initialiser,
  `TB` itself `long`); spliced at its offset as `((int)(TA))`, the probe traps (exit 133) where the
  sliced text returns 2147483648. It is slicing by offset, not parsing C.
- **R1 repaired, three ways** (sections 2, 5 and 3): an unread probe refuses rather than passes;
  the probe compiles the plain `-E` text and the `#define` line is read from a second run; record
  accessors are asked.
- **The `__builtin_constant_p` argument, for C only, unbuilt**: the dump's `CallExpr` range would let
  the probe splice `__builtin_choose_expr(__builtin_constant_p(ARG), 1, 0)`, which clang must answer
  in a constant context, as the initialiser did. Unrun; for R1 (macros) no such repair exists, since
  the program answers the builtin at run time.

## 8. What I falsified, what held, and the conditions I set

**Falsified, each by a run named above:**

1. **The ffi-pragmatist's C compiled at the program's level**: on his own `r1c.py`, `S_BCPV` (C
   defines 5) trapped and `S_BCPM` (undefined) kept at `-O2` (section 3). The seats' conflict is
   resolved by measurement in the compiler-engineer's favour, for initialisers only.
2. **Both seats' *every attack right***: the enumerator whose type moves when its enum completes
   (`TB`, `T3B`, `T4B`) is kept in silence by both prototypes at both levels; an undefined operation
   in `__builtin_constant_p`'s unevaluated argument (`S_BCPO`, `E_BCPO`, C defines 7) is refused by
   both with *C leaves it undefined* (section 3).
3. **R1 as built, which every route rides**: a probe that does not link passes in silence (`K_BAD`
   under a constructor builds and prints a new value per run, section 2); the `-dD` text expands a
   self-referential macro twice (`K_W` refused, C defines 2147483647, section 5) and leaves the
   repository's own `run/fixedbugs-361-...-own-c` probe unread while the gate counts it judged
   (section 5); the argument of
   `__builtin_constant_p` is computed under the checks (`M_BCPO` refused, section 1); a record is
   never asked (`R_CALL` builds, its value moving by level, section 3), where panel 216's R1 names
   records.
4. **The ffi-pragmatist's cost of `-fno-wrapv`, *the three SDK values***: a fourth,
   `D3DSTREAMSOURCE_INSTANCEDATA (2<<30)` at `shared/d3d9types.h:800` (section 4); and his
   condition 5's half on *today's region* cannot hold with the program's compile untouched, as the
   region is that compile.
5. **The ffi-pragmatist's turning point** (an author's ruling that `-fwrapv` is Windows' meaning):
   R1's checks cannot tell that reading, since under `-fwrapv` they trap `INT_MAX + 1`, `-INT_MIN`
   and `65536 * 65536`, which `-fwrapv` defines (section 4).
6. **The compiler-engineer's *enumerators 1193***: 1,174 declarations and 19 `referencedDecl` stubs
   (section 6).

**Held**: the brief's table; R1 sound on the compiler-engineer's `__builtin_constant_p` shape at the
program's level (the program at `-O2` takes the undefined branch, `M_BCPT` 9); C2 at `-O0` right on
both `__builtin_constant_p` initialisers at both levels; the compiler-engineer's 35- and 14-shape
verdicts to the line, his clang 18 row, his code-line count, his SDL3 bytes; the ffi-pragmatist's
glibc, IEC 559, shift-warning, `-fwrapv` and SDK `1 << 31` facts; the chain through two headers,
the anonymous enumerator in a struct, the promoted `unsigned short`, unsigned wrap and a level's
macro, measured right by both prototypes.

**Conditions I set on adopting C (the compiler-engineer's C2), in addition to the seats' own:**

1. **C's probe compiled at `-O0`, R1's at the program's level**: the two evaluators, not a conflict
   (sections 1, 3). The ffi-pragmatist's *at the level* is refused for C.
2. **The value-agreement guard in every C probe**: a probe that does not trap must equal the
   declaration it re-asks, byte for byte, or the constant is refused with words saying its
   initialiser reads otherwise outside its declaration, never *C leaves it undefined*; and the
   references spliced at their dump type, so the guard is the canary and not the verdict. Measured:
   catches `TB`, `T3B`, `T4B` and the `-O2` drift, 0 false alarms in 40 (section 7). Without it, I
   object: the class closes with a hole that keeps undefined values in silence.
3. **R1 never passes a probe it could not run**: one that does not preprocess, compile or link
   refuses the build, naming the step and, for a link, the symbol. Its cost is measured only in part:
   panel 216's 84, 88 and 88 corpus units linked under variant S; the ffi-pragmatist's 111, 136 and
   134 libraries linked with `-undefined dynamic_lookup` or `--unresolved-symbols=ignore-all`
   (`r1c.py:220`, `pop.py:358`), which the lane does not pass, so they do not measure it; over the
   repository's 33 probes here, the one unread is section 5's, whose compile condition 4 repairs
   (its link and run unrun). A
   precondition of every route here, so the coordinator's before landing R1.
4. **R1's probe compiles the plain `-E` text**, the `#define` line read from a second `-E -dD` run
   when a refusal is told (the lane already preprocesses a kept refusal again).
5. **R1 asks record accessors**, as its text says, and the sitting rules the record whose leaf is a
   call (`R_CALL_OK` builds where a scalar call is `ffi_not_constant`).
6. **`-fno-wrapv` in the probe's words on every target, never in the program's compile**, and R1's
   own population over the Windows SDK's macros under it measured before landing (unrun); the cost
   written as four names until then.
7. **The reader tells a declaration from a `referencedDecl` stub**, and its canary (the
   compiler-engineer's condition 4) plants an enum-drift shape beside the two he names.
8. **Questions for the synthesis, not premises**: whether a value whose header names an undefined
   enumerator it never reads (`sizeof(EA)`, `0 ? EA : 5`) is refused (both prototypes refuse it);
   and what R1 tells for `M_BCPO`, whose message is false and whose repair for a macro I did not
   find (for C, the constant-context splice of section 7, unbuilt).

**Found beside the sitting, filed by nobody, for the coordinator**: R1's silent `unread` (section 2),
its `-dD` second expansion (section 5), its refusal through `__builtin_constant_p`'s argument
(section 1, and the region's older `M_BCPZ`), its unasked records and the record call leaf (section
3), the fourth SDK name and clang 21 and 22 adding no `-fwrapv` for the MSVC target (section 4).
Each reproducer is in `<scratchpad>/218-critic/`.

## 9. Housekeeping

Worktree `.claude/worktrees/218-critic` at `b4ae85f63`, removed at the end with `git worktree remove
--force`; container `critic218-u18` (`ubuntu:24.04`, clang 18.1.3 installed without recommends)
stopped and removed; my own `docker run --rm` of `heroes-linux-arm64:latest` and
`heroes-linux:latest`. The box: `ssh win` commands reading stdin and writing stdout only, no file
written there. No paid run, no API call; `gh` read-only (`issue view`, `run list`, `run view`, `api`
GET). Nothing written in the trunk but this file. The lane was read through `git show` and my
worktree, never touched.
