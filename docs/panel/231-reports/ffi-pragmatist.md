# Panel 231, the ffi-pragmatist's report

## The verdict, in the seat's form

- **verdict**: **object**, no veto. The C arm of the generator works as proposed: six hand-written
  bindings over six headers, and two programs combining them, in five configurations, with no
  refusal of a valid program and no disagreement.
  But three of P5's oracle sentences are wrong at the C boundary as drafted, and each would charge
  the compiler with a fault that is the header's or the oracle's. Nothing here changes how a value
  crosses the boundary, so there is nothing to veto.
- **section**: design.md § 1.11 (the founding constraint, and its point 3, *FFI signatures are verified
  by clang against the real header*), § 1.12 (the boundary complete and defended), § 4.19.
- **experiment**: six generated bindings (`gen/gen1` to `gen6`) and two programs combining them (the
  five in one module, two modules)
  in the run suite's five configurations, all agreeing with the computed answer; 14 C operations at
  their edges (Q2); 9 declared-arm programs (Q3); 972's leak by `otool` and `leaks --atExit` (Q4); 705
  blessed emissions compiled, linked and run (Q5); a group-against-header witness compiled right and
  wrong. Clang accepted every header and every correct witness, and refused the wrong one.
- **argument** (≤120 words): The generator writes both sides of the boundary, so a disagreement it finds
  is the compiler's only if its header is defined C and its group provably matches the header.
  Neither is free: UB in an inlined header printed `6157951600` for an `i32`, an uninitialised heap
  read moved the answer at exit 0 with every sanitiser silent, and `a * b + c` fuses on arm64 and not
  on x86-64. `check` never reads a header, so oracle 4 is false there by design, and `-O2` deletes
  the leak oracle 5 exists for. Each is answered by measured means: the header discipline,
  the C witness, two malloc fills, `-O0`, and `leaks --atExit` on Darwin.
- **prediction**: with the header discipline of Q2 and the witness of Q3, the first 1,000 programs of
  the C arm print one answer in all five configurations on this Mac, and every disagreement among them
  reproduces as a compiler or runtime defect when the header is replaced by its C driver's answer. A
  header written without the discipline (one `a * b + c`) disagrees between the CI's arm64 and x86-64
  legs on its first run there.
- **condition**: I approve when P5's sentences say (1) oracle 4 at the boundary is *build's artifact
  links and agrees with `build`*, not *what `check` accepts*; (2) the C arm's headers obey Q2's list and
  are proved by a C driver and witness before the compiler sees them; (3) oracle 5 names `-O0` and the
  instrument per leg (Q6). A measured header-UB case that the sanitised legs plus the two-fill run do
  not see would make me ask for MemorySanitizer on Linux as well.

**Per question**: Q1 approve (§ 1.11); Q2 object to *UBSan is the check* (§ 1.12); Q3 approve the
declared arm, which found a spec-runtime disagreement (§ 4.19 and spec § 13); Q4 object to *972
settled by defect 390*, settled for `run` on Linux alone (§ 1.12); Q5 approve re-homing 1234, with
`-fsyntax-only` in `emission` (§ 1.11 point 3); Q6 approve with the sentence given there.

**Findings for the coordinator to classify and file** (none filed by this seat):

1. **`==` on a copy of a handle made before its end runs at exit 0** (`q3/d_eq.hero` prints `true`,
   `d_eq_null.hero` `false`, all five configurations), where spec § 13 says it aborts; design.md Part 8
   wart 20 promises only the dead value and the crossing. One document is wrong (Q3).
2. **`consumed_borrowed_handle`'s note** (`selfhost/check/consuming.hero:154`) points at the call
   inside the wrapper and asks for `@` *at the call site*; written there it is `marker_mismatch` (Q1).
3. **`.claude/rules/c-boundary.md:178-182` and `platforms.md:117-123`** say a C leak is invisible on this
   Mac in all three configurations. `leaks --atExit` sees it at `-O0` (Q4).
4. **Heroes' own float arithmetic is unfused** only because the IR is three-address. No rule says
   so; a second backend must keep it (Q2).
5. **972 is owed for the corpus** (20 example directories with an `extern`), and **1234's one live
   witness is `ffi-missing-link`** (Q4, Q5).

**A slip of my own, said once**: I wrote one temporary file at `/tmp/x` (a `heroes fmt` output),
outside the repository, against the brief. I removed it in the next command (`ls /tmp/x`: *No such
file*), and every later scratch file is under `.claude/worktrees/231-ffi-pragmatist-scratch/`.

Seat: ffi-pragmatist. Worktree `.claude/worktrees/231-ffi-pragmatist`, detached at `bd0c4477b`,
compiler `./heroes-next` (`heroes-next --version`: `heroes 0.5.0` / `sources 5e0ca4c634086a7c`).
Scratch: `.claude/worktrees/231-ffi-pragmatist-scratch/`. Written as the work goes; a section
marked *in progress* is unfinished.

## Q1. A header generated with the program

**The instrument.** `five.sh` in the scratch folder reproduces `tests/harness/suite_run.hero`'s legs
(lines 140 to 330 read): `heroes-next build <case> -o <bin>` then the binary (`-O0`); the same binary
under `env DYLD_INSERT_LIBRARIES=/usr/lib/libgmalloc.dylib MALLOC_LOG_FILE=...` (Guard Malloc);
`heroes-next run <case>` (`-O2`); `env ASAN_OPTIONS=max_free_fill_size=2147483647:free_fill_byte=85
heroes-next run <case> --sanitize` and `... --sanitize -O0` (the fill, `freed_bytes.hero`'s `FILL`).
Checked first on `tests/golden/run/ffi-acquires-and-consumes.hero`: `16` / `both given back`, exit 0,
in all five.

**The five bindings, written by hand as the generator would**, each a header of `static inline`
bodies whose answer was computed apart from the binding (binding 2's by a C oracle,
`gen/oracle2.c`, built with `clang -std=c11 -Wall -Wextra -Werror -fsanitize=undefined`). Files in
`.claude/worktrees/231-ffi-pragmatist-scratch/gen/`: `genN.h`, `genN.hero`, `genN.expected`.

| # | shape | C the header holds | printed, in all five configurations | exit |
|---|---|---|---|---|
| 1 | a struct by value in and out, a nested struct and an `i32[3]` | `struct g1_outer { struct g1_inner inner; int32_t arr[3]; uint8_t t; double d; }`, `g1_step(struct g1_outer, int32_t)` returning one, `g1_sum` in `int64_t` | `2147483647 -32768 2147483647 0 -2147483648 0 1.5` / `2147483000 -2147483648 255` / `2147450878` | 0 |
| 2 | `@` out-parameters (`int64_t *`, `size_t *`) and a `counted_by` array | `g2_divmod(int64_t, int64_t, int64_t *, int64_t *)` refusing `b == 0` and `INT64_MIN / -1`; `g2_fill(int32_t *p, size_t n, uint32_t seed, size_t *written)`, xorshift32 | `0 -3 -1` / `-1 11 22` / `5 5 5 723471715 349883258 2064144800 2008045182 1384820961` / `0 0 0` | 0 |
| 3 | callbacks: a number, a `void *` user datum, `void` result | `int64_t (*)(int64_t)`, `int64_t (*)(void *, int64_t)`, `void (*)(int32_t)` | `295` / `-6` / `seen 3` / `seen 4` / `seen 5` / `after` | 0 |
| 4 | `cstr lent` in, `-> cstr owned free`, a null owned result, a `char` parameter | `char *g4_shout(const char *, int32_t)` (malloc), `g4_maybe(int32_t)` (NULL on 0), `uint64_t g4_count(const char *, char)` | `HéLLO!!` / `BANANA banana` / `given` / `none: null_cstr` / `3` | 0 |
| 5 | a handle, `acquires` on a result and on a `T **` out-parameter, `consumes` | `g5_acc *g5_open(int64_t)` (two mallocs), `g5_open_into(int64_t, g5_acc **)`, `g5_close` (two frees) | `12 -3` / `0 101 false` / `2 1` | 0 |
| 6 | 056's shape L beyond the brief: an anonymous union, a `partial` record over a bit-field, `rest: zero` | `typedef struct { int32_t kind; union { int64_t n; double r; }; int32_t tail; } g6_val`, `struct g6_flags { int32_t id; uint32_t mode : 3; uint32_t on : 1; int64_t weight; }` | `9007199254740992.0 0.25` / `1 -5 7 -5.0` / `1000000000040` | 0 |
| all | the five groups in one module, five headers in one unit | as above | the concatenation of the five, `diff` against `all.expected` empty in each leg | 0 |

**No refusal, no difference between configurations, no sanitiser word** in any of the seven programs (binding 6 after the one refusal below)
(`grep -c "sanitiser spoke" all/five.txt`: 0). Each of the six headers compiles alone, `#include
"genN.h"` and an empty `main` under `clang -std=c11 -Wall -Wextra -Werror -fsyntax-only`, exit 0 and no
output (run for all six after the bindings, header 1 also before).

**What the generator must know that the hand writing found** (each a refusal by the compiler of my
own text, so the generator's rules and not the boundary's): two parameters of one type are labelled
at the call (`error[needs_label]` on `g2_divmod(..., @q, @r)`, spec § 9), and an `@` argument is
labelled `q: @q`; a character above ASCII inside a string is written as itself (`error
[escape_not_needed]` on `"h\u{e9}llo"`); and oracle 3 holds the generator to the canonical layout,
`heroes fmt gen4.hero` adding the blank line after a `match` block (`diff`: `17a18`), the other four
re-printed byte for byte. **A C member named with one of the twenty reserved words**
(`selfhost/keywords.hero:41-61`, `in`, `match`, `test`, `use`, `fail`, ...) is a header the generator
must not write; whether a real header's such member can be bound is probed below. Binding 6's first draft wrote `record G6Val tag g6_val` for a typedef of an
anonymous struct and was refused, `ffi_unknown_tag`, its note naming the repair: such a typedef is
bound by its own name, `record g6_val`, with no `tag` (§ 13). And `(double)` of `9007199254740993`
printed `9007199254740992.0`: an integer converted to `double` past 2^53 rounds as C11 6.3.1.4p2
leaves to the implementation (nearest even here). The generator's value model must round the same
way and render an `f64` as Heroes prints one.

**Across two modules** (`gen/two/`, `shapes.hero` holding groups 1 and 5, `main.hero` a record of
`shapes.G1Outer` and `shapes.G5`): all five configurations print `2147483647 0 2147450878` /
`42 -2147483648` / `1`, exit 0, and `fmt` re-prints both files. **Two rules the generator must
hold, each a refusal of my first draft**: an `extern` is never called across a module
(`error[extern_across_modules]`, *write a Heroes function beside the `extern`*), so a generated group
used elsewhere comes with Heroes wrappers; and a wrapper that ends a handle takes it `@a` and calls
`g5_close(a)` with no `@` (`marker_mismatch` otherwise).

**A finding beside the work, not filed**: `error[consumed_borrowed_handle]` points at `g5_close(a)`
inside the wrapper and its note says *Mark the parameter `@a` and the call site `@a` too*
(`selfhost/check/consuming.hero:154`). The call site it means is the wrapper's caller's. Written at
the call it points at, `g5_close(@a)`, it is `error[marker_mismatch]`. `gh search issues
"consumed_borrowed_handle"` returns defects 031 and 629 and two records, none about this note. It
reads as § Bounded discovery's `adjacent`, *a true message less exact than it could be*, and it is
the coordinator's to file.

**Already filed, not new**: a C member or function spelled with a reserved word. `function test(x:
i32)` is refused with a drafted `static inline` shim. A member `in` is `error[expected_field]`, with no
mention of `partial` (`kw/a.hero`), and `record Kw tag kw partial` naming only `n` builds and prints
`2` (`kw/b.hero`). Defect 2238 (open) and issue 1 (open) carry it (`gh search issues "expected_field"`).

## Q2. What the generator must know about C to know the answer

**The flags the header is compiled under** (`selfhost/cli/flags.hero:130-151`): `-std=gnu11 -Wall
-Werror=return-type -Werror=uninitialized -Werror=format -Werror=conditional-uninitialized
-fno-strict-aliasing -fno-delete-null-pointer-checks -Werror=shorten-64-to-32 -Werror=sign-conversion
... -fsigned-char ...`. `--sanitize` adds `-fsanitize=address,undefined`, with no
`-fno-sanitize-recover` (`:199-200`). So **UBSan reports and goes on, at exit 0**, and only the run
suite's stderr check (`"runtime error:"`, `suite_run.hero`'s `sanitised_failed`) makes it a failure.
The compiler's own refusal of a header (`ffi_header_refused`, measured on `q2/uninit.h`) says *a
header's code is passed over for `-Wall`'s warnings and sign conversions alone*. A real library's
header is therefore not held to `-Werror=sign-conversion`, which answers the worry that the unit's
flags make real bindings harder.

**The experiment** (`q2/ub.h`, `q2/more.h`, one program per operation fed the edge operand, the five
configurations by `five.sh`, output in `q2/q2-five.txt`):

| C operation in the header | class in C11 | -O0 | Guard Malloc | -O2 | --sanitize | --sanitize -O0 |
|---|---|---|---|---|---|---|
| `int32_t a + b`, `2147483647 + 1` | undefined | -2147483648 | same | same | same, **UBSan speaks** | same, **UBSan speaks** |
| `uint16_t a * b`, `65535 * 65535` (promoted to `int`) | undefined | 4294836225 | same | same | same, **UBSan speaks** | same, **UBSan speaks** |
| `int32_t a << s`, `1 << 31` | undefined | -2147483648 | same | same | same, **UBSan** | same, **UBSan** |
| `int32_t a << s`, `1 << 32` | undefined | **1** | 1 | **0** | **6157951600**, UBSan | 1, UBSan |
| `int64_t a / b`, `INT64_MIN / -1` | undefined | -9223372036854775808 | same | **0** | **6129509008**, UBSan | MIN, UBSan |
| `(int32_t)d`, `3e9` | undefined | 2147483647 | same | **0** | **6163538576**, UBSan | 2147483647, UBSan |
| `t[i]`, `t[4]` of a `static const int32_t[4]` | undefined | -1 | -1 | 0 | 0, UBSan | **exit 134**, ASan global-buffer-overflow |
| a `malloc`'d `int64_t` read before written | undefined | **5** | 5 | **-1** | **-4702111234474983741**, silent | same, **silent** |
| `(int16_t)a`, `32768` | implementation-defined | -32768 | same | same | same, silent | same, silent |
| `int32_t a >> s`, `-8 >> 1` | implementation-defined | -4 | same | same | same, silent | same, silent |
| `uint64_t n - 1u`, `0` | defined (wraps) | 18446744073709551615 | same | same | same, silent | same, silent |
| `double a / b`, `1.0 / 0.0` | defined (Annex F) | inf | same | same | same | same |
| `double a * b + c` in **one expression**, `(1+2^-27)(1-2^-27) - 1` | defined; contraction allowed (6.5p8) | **-5.551115123125783e-17** | same | same | same | same |
| a stack local read on one path before written | undefined | **refused at build**: `ffi_header_refused`, `-Wsometimes-uninitialized` | | | | |

**Three facts the table makes. First**, undefined behaviour in a header puts **values no Heroes type
can hold** into the program: an `i32` printed `6157951600`, which is a stack address, since the
`static inline` body is inlined and `sext` of clang's poison is anything. So a header's UB
is not a wrong answer of the header's. It voids the oracle for the whole program, and **oracle 2 would
report it as the compiler's wrong code**. **Second**, `-fsanitize=undefined` sees exactly the
undefined rows except one: the **uninitialised heap read is silent in all five legs** and shows only as
a disagreement between them (5 / -1 / the fill). Implementation-defined rows and the contraction row
are silent by design, and are *the same in every configuration on this Mac*, so they hide. **Third**,
contraction is **a platform divergence**, not a configuration one. `clang -target <t> -S` of
`double joined(a,b,c) { return a * b + c; }` emits one `fmadd` on `arm64-apple-macos` and
`aarch64-unknown-linux-gnu` at `-O0` and `-O2`, and two separate multiplies and no fused instruction
on `x86_64-unknown-linux-gnu` and `x86_64-pc-windows-msvc`. The split form `double t = a * b; return
t + c;` is never fused on any of the four. So the CI's x86-64 legs would print `0` where this Mac
prints `-5.551115123125783e-17`: **an inference from the instructions, unrun**, since Rosetta is
absent here (`arch -x86_64 /usr/bin/true`: *Bad CPU type*). **Heroes' own `a * b + c` is unfused** in
all five configurations (`q2/fmaown.hero` prints `0.0` twice), because the emitter writes three-address
C (`t3 = t1 * t2; ... t5 = t3 + t4;`, `fmaown.c:112`). That is a property of the IR's shape that no
rule states: `grep -iE "fp-contract|FP_CONTRACT|fused multiply|contraction"` over design.md, the spec
and `selfhost/cli/flags.hero` finds nothing, and `gh search issues fma` / `"fused multiply"` /
`"contraction float"` return none. Defect 582 (closed) restores the floating-point state a header
leaves set (`emit/header_state.hero`) and does not touch this.

**What the header generator may write in the clean arm**, each kept defined by the value it tracks,
as YARPGen does for its own C (056 § 1.2), and never by a sanitiser:

- types: the `<stdint.h>` fixed widths, `size_t`, `double`, `char` (signed, `-fsigned-char`, bound
  `i8`), structs of them, pointers to them;
- unsigned `+ - *` on `uint32_t` and `uint64_t`, the wrap modelled modulo 2^N; **never on `uint8_t`
  or `uint16_t` operands without a `(uint32_t)` first** (the promotion row);
- signed `+ - *` only where the tracked result is in range; `/` and `%` only with a non-zero divisor
  and never `MIN / -1`;
- shifts only by `0 .. width-1`, left only on unsigned, right only on unsigned, or the
  implementation-defined rows modelled as clang defines them (modulo, arithmetic) and stated so;
- conversions: widening freely; to unsigned modulo; to a narrower signed type or from `double` to an
  integer **only in range**;
- floats: `+ - * /` and `sqrt`, **one operation per statement**; no other `<math.h>` function, since
  libm differs across the four legs' C libraries (an inference: not run here); an integer converted to
  `double` past 2^53 rounds to nearest even (binding 6: `9007199254740993` → `9007199254740992.0`), and
  the value model rounds the same way;
- memory: every byte read was written first, every index inside its extent, nothing kept past a call
  the binding does not say keeps it; no two side effects on one object in one expression.

**Is UBSan the check that it did?** **No: it is a backstop for a named subset.** It sees signed
overflow (promoted too), shifts, division overflow, float-to-int overflow and static array bounds, and
ASan the rest of the bounds. It does not see uninitialised memory, implementation-defined conversions,
or contraction, and it reports at exit 0. **What closes the uninitialised row is measured**: the
sanitised leg run twice under two malloc fills, `ASAN_OPTIONS=max_malloc_fill_size=2147483647:
malloc_fill_byte=0` and `=190`, printed `5` and `-4702111234474983741` for the uninitialised read.
Clean bindings 4 and 5, which `malloc` and write every byte, printed the same bytes under both (`md5`
equal). It is the run suite's freed-byte fill (`freed_bytes.hero`, defect 321) applied to bytes never
written. **What closes the contraction row is a rule of the header generator**, one float operation per
statement. **And the generator should prove its header before the compiler sees it**: a C driver
calling each function at the chosen operands, built `clang -std=gnu11 -fsigned-char
-fsanitize=undefined -fno-sanitize-recover=all`, its output the `.expected`'s C half. That is how
`gen/oracle2.c` computed binding 2's answer, so a header fault is the header's and never charged to
the compiler.

## Q3. The shapes refused on purpose, and the declared arm of the C boundary

**The four refusals of spec § 13, each written and judged** (`q3/r_*.hero`, `heroes-next check` then
`build`):

| shape | `check` | `build` | told |
|---|---|---|---|
| a parameter at another width (`k: i64` where C says `int32_t`) | **0** | 1 | `ffi_parameter_type`, *declared wider than the header's `int`* |
| a `partial` record compared | 1 | 1 | `ffi_partial_operation` |
| a handle as a map key | 1 | 1 | `handle_map_key` |
| a lend escaping (`.cstr()` to a parameter not `lent`) | 1 | 1 | `lend_kept` |
| a header that does not exist | **0** | 1 | `ffi_missing_header` |
| a name the header lacks | **0** | 1 | `ffi_unknown_name` |

**`check` never reads the header**, which is the design: clang's verification of a group is the
build's (§ 1.11 point 3), and `heroes check` is *check names and types* (`heroes-next --help`). **So
P5's oracle 4 as worded, *what `heroes check` accepts, `heroes build` compiles and links*, is false by
design at the C boundary** for every group its header refutes, and it cannot tell the compiler's fault
from the generator's there. Its sentence should say what it promises: at the C boundary a
generated program's `build` at exit 0, the group proved against its header *by the generator*
before the compiler sees it (Q2's C driver), so a `build` refusal is the compiler's (shape E) and never
the generator's. **The witness, compiled** (`witness/gen12.witness.c`): one line per binding,
`#define SAME(f, T) _Static_assert(__builtin_types_compatible_p(__typeof__(&f), T), #f " is not " #T)`
then `SAME(g1_step, struct g1_outer (*)(struct g1_outer, int32_t));` and the like for `g1_sum`,
`g2_divmod`, `g2_fill` and `g3_fold`, plus `sizeof` and `offsetof` asserts for `g1_inner` (8) and
`g1_outer` (`arr` at 8, `t` at 20, `d` at 24, size 32). `clang -std=gnu11 -fsigned-char -fsyntax-only`
accepts it at exit 0. A wrong one (`wrong.witness.c`: `int64_t` for `g1_step`'s `int32_t`) is refused,
*static assertion failed*, exit 1. **One trap it showed**: the wrong file's second line, `g2_fill` as
`uint64_t (*)(int32_t *, uint64_t, uint32_t, uint64_t *)`, also failed, because on Darwin `size_t` is
`unsigned long` and `uint64_t` is `unsigned long long`, two C types of one width. So the witness spells
the header's own C types, which the generator knows because it wrote them, while the group says `u64`
for both, as spec § 13 has it and binding 2 built.

**The declared arm has a use, and it found something on its first hand-written try.** Each abort § 13
names, written as the generator would and run in the five configurations (`q3/q3-five.txt`):

| program (`q3/`) | the abort § 13 names | in all five configurations |
|---|---|---|
| `d_never` | a handle nobody consumes | stdout `3`, `panic: 1 C handle(s) never given back — ...`, exit 134 |
| `d_twice` | a handle given back twice (through a copy) | `0`, `panic: a C handle reached the argument `a` of `g5_close` at an address a call marked `consumes` ... ended`, 134 |
| `d_lease` | a lease nobody ends (`x: cstr = s.lease()`) | `3`, `panic: 1 lease(s) never ended — ...`, 134 |
| `d_zero` | `.cstr()` of a `str` holding a NUL | `before 3`, `panic: `.cstr()` lends to C a str holding a NUL byte, at index 1 of its 3 bytes ...`, 134 |
| `d_over` | C reaching past a `counted_by` extent (C writes 5, declared 4) | `before`, `panic: C reached past the 4 elements lent to `p` of `over_fill5` ...`, 134 at `-O0`, Guard Malloc and `-O2`; under both sanitised legs **ASan's `BUS on unknown address`** at 134 instead, as `fixedbugs-396-a-buffer-c-overruns-aborts.expected` pins with `!sanitizer: on unknown address` |
| `d_callback` | the core's own abort inside a callback C called | `before`, `panic: integer overflow` at `d_callback.hero:5:12, in d_callback.big`, 134 |
| `d_eq_cross` | a copy made before the end, handed to C | `0` / `false`, then `panic: a C handle reached the argument ... a copy made before the ending c[all]`, 134 |
| **`d_eq_null`**, **`d_eq`** | **`==` on a copy made before the end** | **`0` / `false` / `compared a copy made before, and nothing said`, exit 0**; `d_eq`'s `kept == other` prints **`true`, exit 0** |

**The finding.** Spec § 13 (`spec/heroes-spec.md:439`): *Giving one back ends it: handing it to C
again aborts, `==` on it aborts, and so does a copy made before, unless C has since handed its address
out again or a million other lives have ended.* In `d_eq` and `d_eq_null` nothing hands an address
out after `g5_close`, and `==` on the copy runs at exit 0 in all five configurations.
`hero_handle_eq` (`runtime/parts/alloc.c:902-913`) refuses only the dead page, the binding the ending
call overwrote (route P). It does not ask the remembered set (route T), which `hero_handle_alive` asks
at every crossing into C, and `d_eq_cross` shows that set refusing the same copy one line later.
design.md Part 8 wart 20 (`docs/design.md:4226-4235`) says *`==` on a dead value aborts* and *a copy
made before the call is refused at its first crossing*. Read that way, the runtime matches design.md
and **the spec's sentence promises more than either**. The `limit-*` cases pin `==` on a stale copy only
after C handed the address out again (`limit-a-stale-copy-...`, `kept == nullptr` → `false`), which is
the spec's own exception. `gh search issues` for `"hero_handle_eq"`, `"== dead handle copy"`,
`"handle equality stale copy"`, `"== on a copy made before"`: none. **One of the two documents is
wrong**: either the spec's sentence (an LLM reading it writes `d_eq` and expects the abort) or the
runtime (no check where the spec promises one). It reads abort-free, it reads no memory, and so it
corrupts nothing. Its class is the coordinator's: *a false message* in the spec is § Bounded discovery's
`blocking`.

**What the declared arm must know, measured**:

- **The checker refuses the plain spelling of two aborts statically**: `==` on the handle itself after
  its end is `handle_used_after_end`, decided *one function at a time, and only where the life ended on
  every path*. A lease cell written `@=` and never rebound is `never_rebound`. The declared arm writes
  the runtime's shape, through a copy, a branch on a C function's result, or a helper, as the
  `dead-handle-*` goldens do (`if one() == 1`).
- **One message per abort across the five, except the overrun**, whose sanitised legs are ASan's. The
  declared arm's `.expected` carries `!sanitizer: on unknown address` there, as the run form already
  allows.
- **What a valid-program generator cannot write, the mutation instrument keeps**: the six refusals in
  the first table. The declared arm writes *valid programs whose run aborts*, which is a different
  class.

**Verdict on Q3: approve the declared arm for the C boundary**, the runtime's aborts as its
expected outcomes, written through the shapes the checker leaves to the runtime. The first two
hand-written tries found a spec-runtime disagreement. That measures the arm's yield; it is not an argument for it.

## Q4. Task 972: `--sanitize` at `-O2` deletes the leak

**972's experiment, on this Mac** (`q4/`): `leak.h` holds `leak_tested(n)`, which is `malloc` whose only
use is a null test, 972's shape, and `leak_escaped(n)`, which returns the pointer as a `ptr` the
program null-tests and drops. Each program calls it 200 times with 21 bytes.

| measurement (command) | `tested` -O0 | `tested` -O2 | `escaped` -O0 | `escaped` -O2 |
|---|---|---|---|---|
| stdout, exit | `200`, 0 | `200`, 0 | `200`, 0 | `200`, 0 |
| `otool -tV` of `h_<prog>_main`: calls to `leak_*` | 1 | **0** | 1 | **0** |
| the out-of-line `leak_*` body at -O2 | | `mov w0, #0x1; ret` (no `malloc`) | | still calls `malloc`, and nothing calls it |
| **`leaks --atExit -- <binary>`** | **200 leaks, 6400 bytes**, exit 1 | **0 leaks**, exit 0 | **200 leaks, 6400 bytes**, exit 1 | **0 leaks**, exit 0 |

So **972's finding holds on this Mac, read from the binary**: at `-O2` the allocation is gone, and the
`static inline` body is inlined into the unit, where a pointer used only for a null test lets LLVM
delete the `malloc`. *A pointer that escapes to the program* is not enough to save it, since the program's
use is the same null test. **Darwin's ASan has no leak detector**, re-measured: `ASAN_OPTIONS=detect_leaks=1
heroes-next run tested.hero --sanitize -O0` exits 134, *AddressSanitizer: detect_leaks is not supported
on this platform*; without the option it prints `200` at exit 0 and an empty stderr.

**But Darwin has a leak instrument, and it is unused by the net.** `leaks(1)` (`/usr/bin/leaks`,
`leaks --atExit -- <command>`) on the plain, unsanitised binary measured:

- 972's leak: 200 leaks, 6400 bytes at `-O0`, and 0 at `-O2` (above);
- **one** 21-byte leak (`q4/single.hero`, `-O0`): `1 leak for 32 total leaked bytes`, exit 1. 972's
  note feared a conservative scan misses a single allocation; here it did not;
- **the binding mistake a real writer makes**, binding 4 with `owned free` left off its `-> cstr`
  (`q4/noowned.hero`): `1 leak for 16 total leaked bytes` at **both** `-O0` and `-O2`, exit 0 from the
  program, since its bytes are read and nothing is elided. The same binding with `owned free`: 0 at both;
- **no false positive**: 0 leaks on the seven clean generated programs (`bin/gen1` to `gen5`, `all`,
  the two-module `main`), about one second each (wall, `date +%s` around each, an indication).

`grep -rln "leaks --atExit\|leaks(1)\|MallocStackLogging"` over `tests`, `selfhost`, `runtime` and
`.claude` finds nothing. Sittings 170, 175 and 176 used it by hand (`docs/panel/170-reports/ffi-pragmatist.md:88`,
`docs/panel/176-reports/ffi-pragmatist.md:410`). So **`.claude/rules/c-boundary.md:178-182` and
`.claude/rules/platforms.md:117-123`**, *LeakSanitizer exists on that leg and on no other, so a leak in a
C binding is invisible on this Mac in all three configurations*, are true of ASan and false of the Mac.
Their correction is owed, and it is a record, not a language change. Unrun: `leaks` on the CI's `macos-26`
runner.

**Linux, unrun here**: `docker info` answers *Cannot connect to the Docker daemon*, and I did not start
it on a machine three seats share. What the Linux legs must run for a program that declares an
`extern`: `heroes run <main> --sanitize -O0` (LeakSanitizer is on by default with ASan on Linux),
judged by the leak report on stderr. The `--sanitize` leg at `-O2` is no leak judge for a `static
inline` allocation, by the disassembly above.

**Settled, owed, or re-homed:**

- **The run form: the instrument exists** since defect 390 (`tests/harness/suite_run.hero:195-204` runs
  `["--sanitize"]` and `["--sanitize", "-O0"]`). That is settled *for the Linux legs by inference*: the
  suite refuses a stderr holding `AddressSanitizer`, and LeakSanitizer under ASan writes its `SUMMARY:
  AddressSanitizer: ... leaked` line there (unrun on Linux by me). On Darwin no leg of `run` sees a C leak.
- **The corpus: owed.** `tests/harness/suite_corpus.hero:427-428` is still `["-O0", "-O2",
  "--sanitize"]`, and `--sanitize` is `run`, so `-O2`. `grep -rl "^extern " examples | grep "\.hero$" |
  sed 's:^examples/\([^/]*\)/.*:\1:' | sort -u | wc -l`: **20** example directories declare an
  `extern` (21 files).
- **The generator: oracle 5 is where it lands**, and it must name the level. A generated C-boundary
  program is judged at `-O0`: on Linux under `--sanitize -O0`, on Darwin by `leaks --atExit` on the
  `-O0` binary. Otherwise the arm meant to find leaks through C (056's shape U) is the arm whose
  optimiser deletes them.

**Verdict on Q4: object** to closing 972 as *settled by defect 390*. It is settled for `run` on
Linux alone. **Recommend** the corpus's `--sanitize -O0` arm for the 20 directories (the 2026-10-25
comment's fifteen lines), and oracle 5 per leg as Q6 states, with `leaks --atExit` added for Darwin.

## Q6. Oracle 5 across the four legs

The legs are `ci.yml:317-328`'s four: Linux x86-64 (`ubuntu-26.04`), Linux arm64
(`ubuntu-26.04-arm`), Darwin arm64 (`macos-26`), Windows x86-64 (`windows-latest`).
**`hero_runtime_check_leaks`** (`runtime/parts/alloc.c`, the last call of every generated `main`)
counts the runtime's own heap blocks (*a missing decref — this is a compiler bug*), the scratch of
`hero_run_*` and `hero_dir_*`, held lease bytes, and the lent regions it audits. The handle and lease
gates count the lives a mark began. **It never counts a byte C's own allocator handed out.**

| leg | the program's own values, handles, leases | a C allocation the program lost |
|---|---|---|
| Darwin arm64 | every configuration: measured here, `d_never` and `d_lease` exit 134 with their counts in all five | **ASan: nothing** (`detect_leaks` refused, measured). **`leaks --atExit` on the plain `-O0` binary: sees it**, measured (200/6400, 1/32, 1/16 bytes; 0 on seven clean programs); not in the net; unrun on `macos-26` |
| Linux arm64 | every configuration (the same runtime C, an inference) | LeakSanitizer under `--sanitize -O0`; at `-O2` only what the optimiser left. 77 bytes reported in the arm64 container on 2026-10-07 (`.claude/rules/platforms.md:28-30`); unrun by me |
| Linux x86-64 | same, an inference | same, an inference |
| Windows x86-64 | same, an inference | **nothing known**: the sanitised legs run (`ci.yml:747-759` puts clang's ASan DLL on `PATH`), and I know of no LeakSanitizer there, unrun and unsourced here |

**The oracle's sentence, so it says what it promises where**:

> **No leak.** On every leg and in every configuration, the runtime's count is zero at exit: no value,
> handle, lease or lent region the program held is left. A byte C allocated and the program lost is
> seen on Linux by LeakSanitizer under `--sanitize -O0`, and on Darwin by `leaks --atExit` on the
> `-O0` binary; at `-O2` only where clang kept the allocation; and on Windows by nothing.

**A program with no `extern` is judged completely by the first sentence on every leg**, since it
reaches C only through `hero_os.h`, whose calls the runtime counts. That is CL-055's narrowing,
unchanged. The generator's C arm is where the second sentence matters, and the leg that judges it
must be named in each witness committed to the net (P7). A witness whose leak only Linux sees is red
on the CI after the push, which § The optimistic chain already accepts.

## Q5. Task 1234: nothing compiles or links what `--emit-c` wrote

**Re-measured today, every blessed emission**: `ls tests/emission/*.c` 693, by group (the file name's
first word) `run` 592, `examples` 55, `ir` 26, `emit` 12, `fixedbugs` 8; plus `tests/golden/emit/*.expected`
12, which are C (`-x c`). Each file's first line names its source, 693 of 693. Each was compiled under
**the compiler's own words** (`flags.hero:130-151`, `-O0 -g`), with `-I runtime -I <source's
directory>`, by `q5/syn.sh` and `q5/link.sh`, four at a time (`xargs -P 4`).

| pass | result | wall (an indication: other seats' builds ran beside it) |
|---|---|---|
| `clang -fsyntax-only` | **692 of 705 pass**; the 13 that fail are `fatal error: '<header>' file not found`: raylib 5, SDL3 2, `openssl/sha.h` 3, `uv.h` 1, `sys/prctl.h` 2 | `real 6.91`, `user 15.45`, `sys 9.05` |
| the 13 again with `pkg-config --cflags raylib sdl3 openssl libuv` | **11 pass**; the 2 left need `sys/prctl.h`, a Linux header, blessed on Linux | |
| compile, then link with the runtime and the program's own `link`/`package` words (`use` lines followed, spec § 1) | **685 of 705 link.** 2 do not compile (`sys/prctl.h`). 18 do not link: **17 `ir-*`**, which are goldens with no `function main` (`grep -L "^function main" tests/golden/ir/*.hero`: 19 of 29), so not programs; and **`fixedbugs-ffi-missing-link`**, `Undefined symbols ... _sqlite3_libversion_number` | `real 72.57`, `user 83.34`, `sys 37.99`, the run below included |
| run each linked `run-*` binary from the worktree root, `timeout 60`, stdout through `head -c 1048576` | **590 ran.** Compared with the `heroes-next build` route run the same way (`q5/twoway.sh` on the 60 my first comparator flagged), **all 60 agree in exit, stdout and stderr** (addresses masked). My comparator's two mistakes: a `!panic:` line is a substring of stderr, not always after `panic: `; and the `c-frees-a-lease-*` cases die by `SIGTRAP`, exit 133 under `timeout`, by both routes alike | |

**So 1234's premise is not what it was.** *28 of 240 failed `-fsyntax-only`* (2026-09-16) is today
**0 of 705 failing for a reason of the emission's own**: every failure is a header absent from this
machine. The one real witness stands: **`tests/golden/fixedbugs/ffi-missing-link.hero`, `build
--emit-c` exit 0, `build` exit 1** (`ffi_missing_link`), re-run with `heroes-next` today. Its artifact
does not link, as the 2026-10-25 comment measured. And **the two routes agree on all 590 run programs**
whose artifact links, which is the differential 1234 asks for, run once by hand.

**Where the leg belongs, measured rather than argued**:

- **`-fsyntax-only` in `emission`.** 7 s of wall four wide over the 705, and it would have caught the
  28 of 2026-09-16, which the byte comparison passed. Its cost is the include path: the compiler's
  `package` answer (pkg-config) for 11 files, and a platform skip for 2.
- **The link and the two-route run in the generator's oracle 4 and in P7's fixed sample.** A generated
  program's link words are the generator's own, since it wrote the group, so it needs no `use` walk and
  no pkg-config guess. 72.57 s four wide for the 705, about 121 s of CPU (`user + sys`), is more than
  the net should pay on every batch for a class whose one live witness is already a committed case.
- **Oracle 4's sentence**, read with Q3's finding that `check` never reads a header: at the C boundary,
  *what `heroes build` builds, the artifact `--emit-c` wrote compiles, links and runs to the same exit,
  stdout and stderr*. What `check` accepts is not a promise there.

**Verdict on Q5: approve re-homing 1234** to the generator's oracle 4 and P7, with the `-fsyntax-only`
pass added to `emission` now that it costs 7 s. **Prediction:** the first 1,000 generated C-boundary
programs whose `build` exits 0 produce an artifact that links and agrees with `build` on all 1,000;
the class is closed for valid programs, and `ffi-missing-link` stays the only shape where the routes
disagree (an invalid program, `--emit-c` writing it anyway).
