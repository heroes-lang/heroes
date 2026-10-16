# Panel 216, the completeness critic, pass 1: the briefs checked

Written 2026-10-16 from 21:41, in a detached worktree of `6e2690b50` at
`<scratchpad>/216-critic/` (`git log -1` there printed
`6e2690b5058b663a768f06db91abe73cd804793a`), with a compiler built there from the seed.
Every verdict below names the command that settled it; what could not be run says *unrun*.

## Setup

- `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes` under `/usr/bin/time -p`:
  `real 8.06 user 7.87 sys 0.16`. The shared brief's *about 8 s*: **verified**.

## FALSE: the Mac does not print `0 0 0` at `-O0`; it prints a different value every run

The shared brief's table (lines 29-34) gives this Mac, `heroes run` (-O0), three runs, `0 0 0`
for `K_DIV` and `K_FLT`, and line 36 concludes that at `-O0` *Linux x86-64* prints a different
value on every run. Re-run in my copy with the coordinator's own `k.h` and `.hero` files, copied
to `<scratchpad>/216-critic-m/`, compiler built from the seed of `6e2690b50`, Apple clang
`21.0.0 (clang-2100.3.34.2)`:

```
heroes run -O0 K_DIV.hero   x3   1860789180  1867948988  1840309180
heroes run -O0 K_FLT.hero   x3   1862656956  1804297148  1864541116
heroes run K_DIV.hero       x3   0 0 0      (no level word)
heroes run K_FLT.hero       x3   0 0 0
heroes build K_DIV.hero -o K_DIV-b; ./K_DIV-b x3   1802790028  1873355916  1793352844, each exit 0
heroes build K_FLT.hero -o K_FLT-b; ./K_FLT-b x3   1808868492  1876583564  1797596300, each exit 0
K_COMMA: 2 2 2 and K_FDIV: 16 16 16 under all three
```

The likely cause of the table's column, an inference: **`heroes run` compiles at `-O2` by
default, not `-O0`** (`selfhost/cli/verbs.hero:62`, `level_from(inv, default_level: "-O2")`;
`heroes build` and `heroes test` default to `-O0`, lines 37 and 113; and
`selfhost/cli/flags.hero:160` says *At `-O2`, where `heroes run` compiles*). So the column
headed `-O0` reads as the `-O2` column twice. **What it changes for the sitting**: the defect
is not a Linux x86-64 artefact. Every `heroes build` on the author's own machine reproduces it,
exit 0, no message; the Mac and Linux x86-64 agree, and only `-O2` happens to print `0` on both.

The same `-O0` instability on Linux arm64, which the table leaves out: the emitted C
(`heroes build K_DIV.hero --emit-c`, byte-identical to the coordinator's `x86/K_DIV.c` by `cmp`)
compiled with `clang -O0` in `heroes-linux-arm64` printed `-890121108 -223416532 -602723492`
for `K_DIV` and `-765994324 -822901924 -475885236` for `K_FLT`, each exit 0; `-O2` printed
`0 0 0` for both. **So all three platforms measured print a run-dependent value at `-O0`.**

## FALSE as worded: `--sanitize` does not trap either value; it names it and the program runs on, exit 0

Route A rests on *`--sanitize` traps both at run time* (`value_check.hero:41`, quoted by the
shared brief at line 66 and marked unrun by the coordinator). Run in my copy on this Mac:

```
heroes run -O0 --sanitize K_DIV.hero   exit=0
  K_DIV.hero:2:12: runtime error: division of -2147483648 by -1 cannot be represented in type 'int'
  SUMMARY: UndefinedBehaviorSanitizer: undefined-behavior K_DIV.hero:2:12
  -1969390727
heroes run -O2 --sanitize K_DIV.hero   exit=0, the same two lines, then 0
heroes run -O0 --sanitize K_FLT.hero   exit=0
  K_FLT.hero:2:12: runtime error: 1e+20 is outside the range of representable values of type 'int'
  1376057149
heroes run -O2 --sanitize K_FLT.hero   exit=0, the same, then 0
K_COMMA and K_FDIV under --sanitize at both levels: 2 and 16, exit 0, no runtime error line
```

UndefinedBehaviorSanitizer reports and **recovers**: `grep -rn -E 'UBSAN_OPTIONS|halt_on_error|
sanitize-recover|__ubsan_default_options|sanitize-trap' runtime selfhost tests/harness` printed
nothing, and `flags.hero`'s `sanitizers` adds `-fsanitize=address,undefined` alone. What does
turn the line into a failure is the **net**: `suite_run.hero:295` and `suite_corpus.hero:317,381`
read `runtime error:` on stderr as a refusal. So *traps* is true of the harness and false of a
program a person runs. Two things this gives the sitting: route A's *the value stays undefined in
every other build* understates it, since it stays undefined under `--sanitize` too; and the
message UBSan prints (*division of -2147483648 by -1 cannot be represented in type 'int'*,
pointed at the constant's own `.hero` line) is a ready model for what a compile-time refusal
could say.

## ANSWERED, one of the brief's open questions: clang 18 has no C23 `constexpr`

The shared brief (line 53) leaves *whether clang 18 accepts C23's `constexpr`* unrun. Run:
`docker run --rm -v <216-critic-m>:/s:ro ubuntu:24.04 bash -c 'apt-get update && apt-get install
-y clang; ...'` (arm64, native), which installed `Ubuntu clang version 18.1.3 (1ubuntu1)`, the
version `clang_floor.hero:29` names as the CI's Ubuntu leg:

```
-std=c23  c23.c   c23.c:2:1: error: unknown type name 'constexpr'   (and lines 3, 4, 5: all four)
-std=c2x  c23.c   the same four errors
-std=gnu11 -Werror=gnu-folding-constant ice.c   all four refused, as on 21 and 22
-x c++ -std=c++17 cx.cpp   K_DIV and K_FLT refused with the same notes; K_COMMA and K_FDIV kept
```

So **route C, asked as C23, refuses every constant on clang 18**, the floor and two of the four
CI legs (`ubuntu-latest`, `ubuntu-24.04-arm`, `ci.yml:298-300`). Route C then needs one of: the
floor raised to the first clang with C23 `constexpr` (which version that is: unrun, a question;
19, 20 and 21 were not installed), a fall-back when clang does not know the word (a premise about
the world a narrowing must not rest on, `.claude/rules/module-shape.md`), or not to be C23. Route
D (C++17) gives the same split on 18.1.3, 21.0.0 and 22.1.8.

## A trap in the brief's own C23 line: `constexpr long long` refuses correct unsigned values

The shared brief's question 2 is `constexpr long long v = (K);`. C23 requires a `constexpr`
object's initializer to be exactly representable in its type, so the line refuses an unsigned
64-bit value above `LLONG_MAX`, which a `constant K: u64` may legally bind
(`HERO_RET_U64` in the emitted C). A header `u.h` of seven values C defines, asked both ways
(`<scratchpad>/216-critic-m/u23.c`, `tu23.c`, `ucx.cpp`, `tucx.cpp`):

```
clang -fsyntax-only -std=c23 u23.c   (constexpr long long)
  error: constexpr initializer evaluates to 18446744073709551615 which is not exactly representable in type 'const long long'
  for ULLONG_MAX, SIZE_MAX and ~0ULL: 3 errors on Apple clang 21.0.0, 3 on Debian clang 22.1.8 arm64
clang -fsyntax-only -std=c23 tu23.c  (constexpr typeof(K))     0 errors on both
clang -fsyntax-only -x c++ -std=c++17 ucx.cpp tucx.cpp          0 errors on both
```

The four shapes the brief measured never reach this, being `int`. A seat that builds route C on
the brief's line as written refuses correct bindings; the line must declare the constant's own
type (`typeof(K)` in C23, `decltype(K)` or `auto` in C++). The other four values in `u.h`,
`18446744073709551615ULL + 1`, `1u << 31`, `(short)70000` and `(1 == 1)`, were kept by every form.

## NOT ASKED by the briefs: the class is wider than two shapes

The shared brief says *two shapes are known*. CL-061 asks for the shapes beside them. A header
`s.h` of 24 undefined neighbours (`<scratchpad>/216-critic-m/s.h`), bound through the trunk's
compiler (`heroes build shapes.hero`, then the survivors in `shapes2.hero`, this Mac, `-O0`):

- **Refused today by the region** (exit 1, `ffi_constant_type`, clang's words):
  `-INT_MIN`, `INT_MAX + 1`, `INT_MIN * -1`, `LLONG_MAX + 1`, `1LL << 63`; `__builtin_abs(INT_MIN)`
  is refused as `ffi_not_constant`.
- **Built at exit 0 and printed what the machine held** (`./shapes2-b`, twice): `INT_MIN / -1`
  (1794532300, then 1838621644), and **`INT_MIN % -1`, `LLONG_MIN / -1`, `(int)(1.0 / 0.0)`,
  `(int)(0.0 / 0.0)`, `(short)(1e10)`, `(int)2147483648.0`, `(long long)(1e30)`,
  `(int)(float)(1e39)`, `(int)(1e308 * 10)`, `(unsigned)(-1.0)`, `(unsigned char)(300.0)`**, and
  the undefined value hidden under a defined one: **`(INT_MIN / -1) * 0`, `(1 ? INT_MIN / -1 : 0)`,
  `((INT_MIN / -1) == 0)`**. Most printed `10` both times (the byte the previous `print` left in
  the register, an inference), `(unsigned char)(300.0)` printed `0`: run-to-run stability is not
  evidence that a value is defined.
- **The three questions over all 24** (`q3.sh`, `-ferror-limit=0`, Apple clang 21.0.0; the first
  run without that flag stopped at clang's default limit of 20 errors and read the last five as
  kept, a trap for any seat that asks many constants in one unit): the ICE rule, C23 `constexpr
  typeof(K)` and C++17 `constexpr decltype(K)` **agree on every row**, refusing 22 and keeping
  `1LL << 63` (which the region already refuses) and `(int)4294967295u` (implementation-defined,
  correctly kept).

So a route judged on the four shapes is judged on the narrowest sample of its own class. The
corpus the seats are asked to run is the other half: every value C defines must still build.

## NOT ASKED: values C defines that C23 and C++ refuse too, and an exit 2 found beside them

A header `d.h` of 20 values that are not all integer constant expressions, each bound alone
through the trunk's compiler (`mk6.sh`, `heroes build`, this Mac), then asked the three questions
(`q4.sh`, Apple clang 21.0.0):

```
name           trunk today          ICE    C23    C++
D_COMMA        (1, 2)               2      refuse keep   keep
D_FDIV         ((int)(1000.0/60))   16     refuse keep   keep
D_FCMP         (1.5 > 1 ? 3 : 4)    3      refuse keep   keep
D_STRSUB       ("abc"[1])           98     refuse keep   keep
D_PTRCMP       ((void*)0==(void*)0) 1      refuse keep   keep
D_FLOATCAST    ((int)(float)3.7)    3      refuse keep   keep
D_LDBL         ((int)(1.0L/3*9))    3      refuse keep   keep
D_OFFSET_HAND  ((int)(size_t)&((struct d_s *)0)->b)   8   refuse refuse refuse
D_PTRINT       ((int)(intptr_t)(void *)16)            16  refuse refuse refuse
D_STMT         (__extension__ ({ 3; }))   exit 2       refuse refuse refuse
(and ten integer constant expressions, offsetof, sizeof, enum |, '\377', a u32 wrap,
 (int)4294967295u, (short)70000, (int)1e9, __builtin_clz(16), _Generic: kept by all three)
```

What it gives the sitting:

- **Routes C and D are not "refuse only what C leaves undefined"** either: both refuse the
  hand-written `offsetof` idiom (strictly undefined in ISO C, folded by every compiler, and how
  older headers spelled it) and an integer made from a pointer cast (implementation-defined, not
  undefined), which the trunk builds today and prints `8` and `16`. Whether any header the
  project binds spells a constant so is the ffi-pragmatist's corpus question; nobody has asked it.
- **`(__extension__ ({ 3; }))` exits 2 today**, `internal error: compiling the generated C
  failed`, clang's *statement expression not allowed at file scope* at the type assertion
  `_Static_assert(HERO_RET_I32(D_STMT), ...)`. `gh issue list --state all --search` for
  *statement expression*, *"file scope"* and *`__extension__`* found no defect naming it: a
  question, not a premise. An exit 2 where the author could be told is `blocking` by
  `.claude/rules/verification.md` § Bounded discovery. It is beside the sitting, not its question,
  but a route that reads clang's refusals back by position meets the same file-scope wall.

## NOT ASKED: the same undefined value bound as `f64`, as a record, or widened to `i64`

`value_check.hero:43-46` says what the region does not ask: a floating constant, a record's.
The shared brief's question is *a group's integer `constant`*. The shapes beside it in type,
bound through the trunk's compiler (`<scratchpad>/216-critic-m/ty/t.hero`, `heroes build`,
exit 0, run three times on this Mac at `-O0`):

```
constant T_F64: f64   #define T_F64 ((double)(INT_MIN / -1))   3.010904203e-314, 3.0467883293e-314, 3.0154939425e-314
constant T_F64B: f64  #define T_F64B ((double)(int)(1e20))     4.2081722396e-314 (x3)
constant T_PT: Pt     #define T_PT {INT_MIN / -1, 2}           .x = -2147483648 (x3)
constant T_PTX: Pt    #define T_PTX ((struct pt){ (int)(1e20), 3 })  .x = 2147483647 (x3)
constant T_I64: i64   #define T_I64 ((int64_t)(INT_MIN / -1))  10 (x3)
```

So an `f64` constant carries defect 2075 unchanged (a value that differs per run, exit 0) and no
route as framed reaches it; and a record constant gets **clang's folded value** (`INT_MIN`,
`INT_MAX`, the same values clang's GNU folding gave the enum in my C11 test), stable but still a
value C leaves undefined, accepted in silence. One header expression, three behaviours by the
declared type. Whether the sitting's resolution covers floats and records, or leaves them as a
named gap, is a question the briefs do not ask.

## Routes the briefs do not list

**F. The trap read at build time** (measured; route E's ingredients moved before the link). For
each constant, a probe function `long long probe(void) { return (long long)(K); }` in a unit of
its own, compiled `-std=gnu11 -O1 -S -emit-llvm` with
`-fsanitize=signed-integer-overflow,float-cast-overflow,shift,integer-divide-by-zero` and
`-fsanitize-trap=` the same list; a `call void @llvm.ubsantrap` left in the function means clang
could not compute the value without undefined behaviour (`<scratchpad>/216-critic-m/probe/run_probe.sh`).
Over the 44 shapes of `s.h` and `d.h`:

- **23 undefined shapes leave a trap, 21 defined ones leave none**, the split identical on Apple
  clang 21.0.0 (this Mac), Debian clang 22.1.8 (`heroes-linux-arm64` and `heroes-linux`) and
  **Ubuntu clang 18.1.3** (`ubuntu:24.04` with its `clang` package): `diff` of the `-O1` verdicts
  printed nothing on all three. Windows clang 23: unrun (the box timed out).
- It **keeps** `(1, 2)`, `((int)(1000.0 / 60))` and every value C23 and C++ keep, **and also**
  the hand-written `offsetof`, the pointer-cast integer and the statement expression, which C23
  and C++ refuse.
- At `-O0` the verdict is not readable: an in-range float conversion (`D_FDIV`, `D_FINRANGE`,
  `D_FLOATCAST`, `D_LDBL`) leaves a conditional check there; `-O1` removes it. So the route rests
  on a premise about LLVM's optimiser folding a constant condition, measured on three versions
  and not on 23.
- Without trap mode the handler named in the IR is the reason in words:
  `divrem_overflow` (`S_DIV`, `S_REM`, `S_HIDDEN`, `S_CMP`), `float_cast_overflow` (`S_FLT`,
  `S_NEGU`), `shift_out_of_bounds` (`S_SHL63`), `negate_overflow` (`S_NEG`); none for the defined
  ones. The trap-mode probe of `S_DIV` at `-O1` is the whole function
  `tail call void @llvm.ubsantrap(i8 3)` then `unreachable`.
- Unmeasured and for a seat: its cost per build (one more clang run with code generation over
  the unit's headers), whether it subsumes the region's six warnings, the `(long long)` cast in
  my probe (which a real one would replace with the constant's own type), and a header that
  does not compile with the sanitizer on.

**G. Flags that define the behaviour instead of refusing it.** `-fno-strict-float-cast-overflow`
makes the float half saturate: the coordinator's `K_FLT.c` built with it printed `2147483647`
three times at `-O0` and at `-O2` on this Mac. `-fwrapv` does **not** define `INT_MIN / -1`:
`K_DIV` with both flags printed `1863197628 1836868540 1796203452` at `-O0` and `0 0 0` at `-O2`.
So this route covers half the class, and, on my reading, sits against spec § 4 line 115 (a
written constant's aborting operator is a compile error) by giving the header's value a meaning
instead of refusing it.

**H. Two stages: the ICE rule as a filter, a second question only for what it refuses.** The ICE
check is cheap and already in the unit's dialect; every constant it accepts is kept as today,
and only its refusals go to C++ (or F). This bounds a C++ route's exposure to headers that do not
compile as C++: only a header that also spells a non-ICE constant is ever compiled so. Not built.

**I. The folded value as a literal.** Clang's GNU folding gives a stable value for every shape
(the enum test: `INT_MIN / -1` folds to `-2147483648`, `(int)(1e20)` to `2147483647`), and
records already get it (above). Emitting the folded value as the accessor's literal ends the
run-to-run variance and refuses nothing. It is the conservative route the panel rule asks to be
recorded beside the robust one; it accepts a value C leaves undefined in silence.

**J. The compiler's own evaluator** over the preprocessed expansion, refusing overflow and
out-of-range conversion itself. Independent of the clang version, and against design.md §1.11's
*everything comes from C*; named so the option set is complete, not recommended. Not built.

## Questions the sitting should be asking and is not

1. **Spec § 4, line 115** already rules for a written constant: *an integer operator in it that
   would abort, run or not, is a compile error at least where its operands are known*. A header
   constant is the FFI twin of that rule, and it argues for a build-time refusal over route E's
   run-time trap and route I's silent fold. Neither brief cites it (`grep -n 'run or not'
   spec/heroes-spec.md` prints line 115, under `## 4. Top-level declarations`, line 90).
2. **Route E may not be soundness-lane at all**: a program that builds today and would then abort
   when it reads a constant is a behaviour change a program sees, which spec § 13 does not
   describe (it says only *the header holds the value*, line 400). Whether that needs a spec
   sentence, which would re-convene the sitting full, is unasked.
3. **What does the route do at `-O2`?** `heroes run` compiles at `-O2` and prints `0`, stable and
   still undefined. The briefs frame the defect as *a value that changes from run to run*; the
   stable `0` is the same defect wearing a quieter face.
4. **Route C needs the floor raised** (clang 18.1.3 has no C23 `constexpr`, above), and the CI's
   two Linux legs install exactly that clang. Raising the floor is a change to the tool surface
   (`clang_floor.hero`, `ci.yml`, and `doctor`'s answer), which CLAUDE.md § 4 names among the
   panel triggers (*architecture (backend, IR, tool surface)*). Which clang first knows C23
   `constexpr`: unrun.
5. **A unit of questions needs `-ferror-limit=0`**: clang stops at 20 errors by default, and a
   reader that takes *no error* as *kept* reads every constant after the twentieth as kept. My own
   first table did exactly that. The trunk's ask units already pass it
   (`selfhost/cli/header_ask.hero:87`, `switch_ask.hero:117`, `unswitch_ask.hero:209`,
   `two_ways_ask.hero:154`); the region's own compile is the main unit's.
6. **`value_check.hero:41`'s sentence is false as worded** (*`--sanitize` traps both at run
   time*; it reports and the program runs on, exit 0). A comment in the compiler that a sitting
   then quotes as a route's floor: whether it is corrected in the same change is unasked.
7. **The population.** The briefs name 932 files with an `extern` line. The routes touch only
   integer and `bool` header constants: my awk over `git ls-files '*.hero'` (an `extern` at
   column 0, then indented `constant NAME: <i8..u64|bool>` lines) found **132 such constants in
   65 files** (55 under `tests/golden`, 3 `selfhost/cli`, 2 `tests/harness`, one each in
   `selfhost/module`, `examples/sqlite`, `examples/ledger`, `examples/curl`,
   `archive/bootstrap-rs`), naming 19 times `hero_os.h` and the system headers `unistd.h`,
   `stdlib.h`, `stdio.h`, `sqlite3.h`, `signal.h`, `time.h`, `sys/prctl.h` and others. An
   approximation by its own command; the seat's corpus run is the measurement.
8. **A model the compiler-engineer's brief does not point at**: the trunk already runs units of
   questions of their own against a program's headers, read back error by error,
   `selfhost/cli/header_ask.hero` and its siblings `switch_ask`, `unswitch_ask`, `two_ways_ask`.
   The brief points at `layout.hero` and a lane-only `layout_runs.hero`.

## Verified (command, what it printed)

- **The trunk commit.** `git log -1 --format=%H` in my worktree printed
  `6e2690b5058b663a768f06db91abe73cd804793a`. The trunk's `git status` now shows
  `?? docs/panel/216-briefs/` (the briefs themselves), consistent with the brief's *printed
  nothing* before they were written.
- **`ffi_constant_type` at `selfhost/emit/ffi_constant_value.hero:90`** and **`told` at line 79**:
  `sed -n 75,95p` shows `function told(...)` on 79 and `code: "ffi_constant_type",` on 90.
- **`accessor` at `selfhost/emit/constant_accessor.hero:48`**: `sed -n 40,56p` shows
  `function accessor(@w: writer.Writer, ...)` on 48.
- **`value_check.hero`'s head, lines 35-42, says what it does not catch**: `cat -n` shows the
  paragraph *WHAT IT DOES NOT CATCH, measured 2026-10-14 over 31 shapes* on lines 35-41 (42 is a
  bare `#`), naming `INT_MIN / -1`, a float cast out of range, the ICE rule refusing `(1, 2)` and
  `((int)(1000.0 / 60))`, and *`--sanitize` traps both at run time* (line 41). The brief quotes
  it faithfully. Note what the head's own measurement covered: *Apple clang 21 and Linux arm64's
  clang 22.1.8*, not x86-64, not clang 18.
- **The clang floor is 18 at `selfhost/cli/clang_floor.hero:36-37`**: `sed -n 25,45p` shows
  `constant FLOOR: i64` on 36 and `18` on 37. Its comment (lines 28-30) names **18.1.3 as the CI's
  Ubuntu leg**.
- **`.github/workflows/ci.yml:332` installs the distribution's clang**: line 332 is
  `sudo apt-get install -y clang lldb pkg-config libsqlite3-dev libcurl4-openssl-dev`. The Linux
  legs are `ubuntu-latest` and `ubuntu-24.04-arm` (lines 298-301), so *the distribution's clang*
  is Ubuntu 24.04's, which `clang_floor.hero:29` records as 18.1.3: **the clang 18 question is
  not a floor hypothetical, it is two of the four CI legs today**.
- **`selfhost/cli/layout_runs.hero` is not on the frozen trunk**: `[ -e ]` printed MISSING.
- **`selfhost/cli/layout.hero`** exists (621 lines, `wc -l`); **`layout_screen.hero`** exists,
  but at `selfhost/emit/layout_screen.hero` (`git ls-files | grep layout_screen`); the shared
  brief names it without a directory.
- **The Docker images**: `docker image inspect` printed `amd64 linux` for `heroes-linux` and
  `arm64 linux` for `heroes-linux-arm64`; inside, `uname -m` printed `x86_64` and `aarch64`, and
  `clang --version` printed `Debian clang version 22.1.8` in both.
- **The Windows box's clang 23**: a document claim, `docs/platforms/windows/WINDOWS-MACHINE.md:357`
  (`LLVM / clang | **23.1.1**`). **Unrun on the box**: `timeout 15 ssh -o ConnectTimeout=6 win
  'clang --version'` printed `ssh: connect to host apponfly-vps port 22: Operation timed out`.
- **`heroes build` compiles at `-O0`** (shared brief line 36): `verbs.hero:37`.
- **The Linux x86-64 columns** (shared brief lines 31-32): my own `--emit-c` of the four programs,
  compiled with plain `clang -O0` / `-O2` and the trunk's `runtime/` in `heroes-linux`
  (`Debian clang version 22.1.8 (++20260613...)`), printed `-230200 -230280 -230296` (`K_DIV`)
  and `-230312 -230456 -230216` (`K_FLT`) at `-O0`, `0 0 0` for both at `-O2`, `2` and `16`
  throughout, each exit 0. Different digits from the brief's, as a value that varies must give;
  the shape **verified**.
- **The three clang questions** (shared brief lines 39-51): `clang -fsyntax-only` on the
  coordinator's `ice.c`, `c23.c` and `cx.cpp` (copied), on Apple clang 21.0.0, Debian clang
  22.1.8 arm64 and Debian clang 22.1.8 x86-64: question 1 refused all four with the quoted words;
  questions 2 and 3 refused `K_DIV` and `K_FLT` and kept `K_COMMA` and `K_FDIV`, on all three.
  **One correction of emphasis**: the brief gives the note naming the value (*value 2147483648 is
  outside the range of representable values of type 'int'*) to C++ alone; C23 prints the same two
  notes, on all three clangs. Both routes can say the value.
- **Defect 2075** exists, open, labelled `kind:defect`, `class:systemic`, `area:emit`
  (`gh issue view 2075 --json ...`).
- **Spec § 13's sentence**: `spec/heroes-spec.md:352-353`, *clang checks every result type,
  constant and record field against that header*.
- **A correct program refused is `blocking`**: `.claude/rules/verification.md` § Bounded
  discovery lists *a correct program refused* under `blocking`. And the trunk holds one that
  route B would refuse: `tests/golden/run/fixedbugs-2072-values-c-defines-still-build.h`
  defines `FROM_A_FLOAT ((int)(1000.0 / 60))`, a `run` golden. Route B therefore turns a
  golden red before any corpus is read.
- **`ffi_constant_type` is the class that tells such a value**: both
  `tests/golden/unsupported/fixedbugs-2072-a-value-c-leaves-undefined-is-told.expected` (six
  errors) and `fixedbugs-2071-a-value-c-cannot-read-is-told.expected` (three) carry it. The 2072
  golden's comment says an overflow is *refused the same way and not pinned here: its words carry
  the value, which clang 21 writes with digit separators*; my build printed
  `-2'147'483'648` for `INT_MAX + 1`. A route whose message quotes clang's value inherits that.
- **The existing region holds on clang 18**: the CI's Linux x86-64 leg at `3e389b024`
  (run 38924878278) printed `ok clang Ubuntu clang version 18.1.3 (1ubuntu1)` and
  `unsupported: 318 passed, 0 failed` (`gh run view --job ... --log | grep`), after the region
  landed at `25b03974b` (2026-10-14).
- **`layout_screen.hero` reads by position**: its head, lines 35-37, *Each function sits at
  `#line <decl + 1> "hero_ly_screen"` ... nothing is read from clang's prose but that position*.
- **The forms the compiler-engineer is told to run exist as selectors**: `run`, `emit`,
  `unsupported` are in `tests/harness/main.hero:570`'s list.
- **`doctor` reports the floor**: `selfhost/cli/doctor.hero:99-106` checks `clang_floor.FLOOR`.
- **`ubuntu:24.04` with its `clang` package gives clang 18, and the network answers**: the
  install above printed `Ubuntu clang version 18.1.3 (1ubuntu1)`.
- **`about 8 s`** for the seed build: `real 8.06` (Setup).

## Could not verify, or unrun

- The Windows box's clang 23 and anything on it: `ssh win` timed out (above).
- Which clang version first knows C23 `constexpr`: no 19, 20 or 21 Linux clang was installed.
- What any route costs a cold build: not measured here, the seats' question.
- Whether the corpus's headers compile as C23 or as C++: the ffi-pragmatist's question, unrun
  here.
- What exact command produced the brief's Mac columns: not recorded in the brief or in
  `p2075/`; the inference that it was `heroes run` without `-O0` rests on the default level and
  on my numbers, not on the coordinator's shell history.

## Housekeeping

Files written outside the trunk: `<scratchpad>/216-critic/` (the worktree, removed at the end)
and `<scratchpad>/216-critic-m/` (my measurements). One mistake, corrected: a `sed` of mine made
the Mac's run of `run_probe.sh` write 46 files (`s.h`, `d.h`, `p_*.c`) into the Mac's `/tmp`;
they were deleted by name and time (`find /tmp -maxdepth 1 ... -newermt 2026-10-16T21:50:00
-delete`, 46 deleted, 0 older matches) and the run was repeated inside the scratchpad.

---

# Pass 2, 2026-10-17: F2, its canary and α2 attacked

Written from 2026-10-16 23:57 into 2026-10-17, at the coordinator's request, after the
compiler-engineer's report (`compiler-engineer.md`). Pass 1 above stands as written. Every number
names its command; what could not be run says *unrun*.

## Setup

- A detached worktree of `6e2690b50` at `<scratchpad>/216-critic2/`, `rm -rf build`, then
  `git apply --check` and `git apply` of `<scratchpad>/216-ce-m/patches/f2-canary-alpha2.patch`
  (both silent; `git status --short` then listed the four modified and two new modules the report
  names). The trunk's compiler from the seed (`heroes-seed`), then
  `./heroes-seed build selfhost/main.hero -o heroes-a2` under `/usr/bin/time -p`: `wrote
  heroes-a2`, `real 71.33 user 56.02 sys 11.01`, exit 0. This is **α2**: F2, its canaries, its loud
  exits and the 2101 narrowing, as the report's last columns judge it.
- My measurements are in `<scratchpad>/216-critic2-m/`. `cmp.sh` builds each one-constant program
  with the trunk's compiler (folder `t/`) and with α2 (folder `a/`), each with `heroes build`
  (`-O0`), and runs the binary three times.
- **The report's headline numbers, spot-checked on my α2**: `K_DIV` and `K_FLT` exit 1 with
  *C leaves the header's value undefined*; `K_COMMA` prints `2 2 2`, `K_FDIV` `16 16 16`;
  `D_STMT` exit 0, `3 3 3`. As reported.

## F2 still misses, silent: a value the accessor's own position or the optimiser decides

The report's repair of F was right about the preprocessor and stops there. Run 1 reproduces
the build's *words*. It does not reproduce the build's *place*: the probe is a reconstructed
unit (`bound_ask.closed_opening` and then one-line accessors with no `#line`), where the
program's accessor sits at `#line <n> "<file>.hero"` in its own unit. And run 2 judges at
`-O1`, where the program is `-O0` by default (`heroes build`, `heroes test`). Header
`<scratchpad>/216-critic2-m/atk.h`, each name bound alone as `i32`:

```
name            spelling                                                   trunk          α2
A_LINE_MISS     (__LINE__ < 20 ? (INT_MIN / -1) : 0)                       exit 0, varies  exit 0, varies (1799791596 1801102316 1874486252)
A_FILE_MISS     (sizeof(__FILE__) < 24 ? (INT_MIN / -1) : 0)               exit 0, varies  exit 0, varies (1875239916 1839981548 1842963436)
A_BCP_MISS      (__builtin_constant_p(atk_five()) ? 0 : (INT_MIN / -1))    exit 0, 1 1 1   exit 0, 1 1 1
                (atk_five a `static inline __attribute__((const))` function returning 5)
```

Each was then run as `<compiler> run -O0 --sanitize <name>.hero`. Under both compilers each
printed `runtime error: division of -2147483648 by -1 cannot be represented in type 'int'`
(the `A_BCP_MISS` values were `-1001057603` and `-496981827`). **The program α2 builds executes
`INT_MIN / -1`, exit 0, no message: defect 2075, behind a probe that says it looked.** In the
program's unit `__LINE__` is the constant's `.hero` line (2) and `__FILE__` is
`A_LINE_MISS.hero`; in the probe both are the probe's own, so the probe takes the defined
branch. `__builtin_constant_p` of a call clang cannot fold in its front end becomes
`llvm.is.constant`. That is false at `-O0` and true at `-O1` once the call is inlined, so the
probe judges the other branch.

The `__builtin_constant_p` miss reproduced by hand on three clangs, without the compiler. Run 1:
`clang -std=gnu11 -O0 -E -dD`. Run 2: `clang -x cpp-output -O1 -g0 -fno-sanitize=all
-fsanitize=<the four> -w -S -emit-llvm`, as `value_ask.SECOND` writes them. Then the same name
printed by a `-O0 -fsanitize=undefined` program (`<scratchpad>/216-critic2-m/hand/hand.sh`):

```
                    Apple clang 21.0.0 / Debian clang 22.1.8 arm64 / Ubuntu clang 18.1.3
A_BCP_MISS   F2 reads: no handler                    the -O0 program: runtime error: division of -2147483648 by -1
A_BCP_LOUD   F2 reads: __ubsan_handle_divrem_overflow    the -O0 program: 0
```

Identical on all three (`ubuntu:24.04` with its `clang` package for 18.1.3). So the miss is the
optimiser's semantics, not a vendor artefact.

**At `-O2` α2 is right on both**: `heroes run -O2 --sanitize` printed `0` and no runtime error for
`A_BCP_MISS` (kept, correctly), and a runtime error for `A_BCP_LOUD`, which α2 refuses
(correctly). **F2's verdict is the optimised program's verdict, applied to every level.**

## F2 refuses, loud: values the program's own unit defines

```
name            spelling                                                   trunk                         α2
A_COUNTER_MISS  (__COUNTER__ == 0 ? (INT_MIN / -1) : 0)                    exit 0, 0 0 0; --sanitize: no runtime error   exit 1, ffi_constant_type
A_BCP_LOUD      (__builtin_constant_p(atk_five()) ? (INT_MIN / -1) : 0)    exit 0, 0 0 0; -O0 --sanitize: no runtime error exit 1, ffi_constant_type
```

The trunk's `-O0` binary of each reads a defined `0`, and UBSan at `-O0` finds nothing.
α2 tells both *C leaves the header's value undefined*: for `A_COUNTER_MISS` that is false at every
level (`__COUNTER__` is the preprocessor's, and the unit's count differs from the probe's), and
for `A_BCP_LOUD` it is false at `-O0` and true at `-O2`. A false message and a correct program
refused are both `blocking` (`.claude/rules/verification.md` § Bounded discovery). The shapes are
contrived. That is not a defence for a probe whose whole job is to see what the program sees.
They are the measured proof that it does not.

## F2's scope leaves two parts of defect 2075's class standing

- **Floating constants, unbuilt.** The report says `f64` is *flagged if asked*, about 3 lines,
  not built. On α2 (`ini.h`): `constant I_F64: f64` = `((double)(INT_MIN / -1))` printed
  `3.0311249977e-314 3.04716074e-314 3.0086620107e-314`; `I_F64B` = `((double)(int)(1e20))` and
  `I_F32: f32` = `((float)(INT_MIN / -1))` (`2.2066993e+27 4.268325e+27 2.2415568e+26`) the same:
  run-varying values at exit 0, exactly as on the trunk. **The route as built leaves the defect
  open for every floating constant.**
- **Integers clang folds in an initializer: no operation, so no check.** On α2 and the trunk
  alike, each prints `-2147483648` three times at exit 0: `enum { I_ENUM = INT_MIN / -1 }`, a
  `static const int atk_k = INT_MIN / -1;` bound through `#define I_STATIC (atk_k)`, and the
  brace-list record `I_REC {INT_MIN / -1, 2}` (the report's named gap). And the commonest real
  shape of all, `enum { S2_ENUM31 = 1 << 31 }`, prints `-2147483648` at exit 0 under both,
  while the *same value* spelt `#define SH_MACRO31 (1 << 31)` is refused by the trunk's region
  (`signed shift result (0x80000000) sets the sign bit`). One header value, two answers by
  spelling. These are integer constants, inside the question's own frame, and F2 cannot reach
  them by construction: clang computes them in Sema, emits a constant and no operation, and
  UBSan has nothing to check. What clang can say about them: the region's six warnings raised
  over `en.h` itself refuse the shift and `INT_MAX + 1` enumerators and stay silent on
  `INT_MIN / -1` and `(int)1e20`. `-pedantic` warns `gnu-folding-constant` on every non-ICE
  enumerator, and C requires an enumerator to be an ICE (C11 6.7.2.2p2), so on an enumerator
  route B's question is C's own rule. On `static const int en_static = INT_MIN / -1;` clang
  printed nothing under `-pedantic`.

**Corrected 2026-10-17, by the output already above**: the sentence *`-pedantic` warns
`gnu-folding-constant` on every non-ICE enumerator* says more than the run showed. `clang
-fsyntax-only -std=gnu11 -pedantic en.h` warned on lines 3 to 7 (`INT_MIN / -1`, `INT_MAX + 1`,
`(int)1e20`, `(1, 2)`, `(int)(1000.0 / 60)`) and **not on line 2, `1 << 31`**, which clang
treats as an integer constant expression. The region's `-Wshift-sign-overflow` refuses that
line instead.

## The canary is falsifiable in one direction only

The canary's premise as `value_probe.hero` writes it: *clang emits a check for an operation on
constant operands, and the optimiser keeps the handler its value reaches*. F2 needs a second half
it does not write: *and removes a handler its value does not reach*. Without it, every defined
float conversion is refused. I simulated each death with a `clang` wrapper first on `PATH`
(`<scratchpad>/216-critic2-m/wrapQ/clang`, `wrapL/clang`). Each alters only the probe's second
run, the one clang run that names `-fno-sanitize=all` (in argv or in its `@` response file),
by appending a word, the last of which wins. Each logged 4 altered runs for 4 builds
(`sim.sh`, α2, `heroes build`):

```
wrapQ (+ -fno-sanitize=all: the checks die)   K_DIV, K_FLT, K_FDIV, K_COMMA: exit 2 each,
    "clang compiled the probe of the header's constants and its canaries, values C leaves
    undefined, call no check ..."                                    the canary fires, as designed
wrapL (+ -O0: the folding dies)                K_DIV exit 1, K_FLT exit 1 (right), K_COMMA exit 0,
    K_FDIV ((int)(1000.0 / 60)), a value C defines: exit 1, "C leaves the header's value undefined
    at line 5: a floating value converted to an integer type that cannot hold it"   (false)
```

So the canary catches the death that would be silent and is blind to the death that would be
false. A false message on a correct program is `blocking`, and the author would be told to
correct a header that is right. The fix is cheap and of the same kind: **defined canaries
that must call no handler** (`(int)(1000.0 / 60)`, an in-range division), one calling a
handler stopping the build as the compiler's failure. And the two undefined canaries test
two literal shapes. The premise is per shape (the ternary, the comma, the comparison, the
value hidden under `* 0`), so **one canary per measured shape of the class**, about 16 one-line
functions in the same probe and no added run, is what makes *the probe sees this class* a
claim the build re-tests rather than a claim measured once. I found no real clang on which a
per-shape death happens: the four clangs agree on every shape. That limit is stated, not
measured.

## α2: seven exit 2 rows of a `blocking` defect, not one

Defect 2101 is open and classed `blocking` (`gh issue view 2101`: `kind:defect`,
`class:blocking`, *an exit 2 where the author can be told*). The report counts one shape left
at exit 2 (`ST_SIDE`). Over `st.h`'s ten statement expressions and five other non-constant
values, each bound alone as `i32` (`st_run.sh`, trunk and α2):

```
told on both:   NC_CALL (st_f()), NC_EXT (an extern int), NC_ERRNO (errno), NC_COUNTER (a static int):
                exit 1, ffi_not_constant, the same words; ST_VOID ({ (void)0; }): exit 1, ffi_constant_type
fixed by α2:    ST_PURE ({ 3; }) exit 2 -> exit 0, 3 3 3;  ST_TWO ({ 3; 4; }) exit 2 -> exit 0, 4 4 4
exit 2 on both: ST_SIDE ({ st_counter++; }), ST_DECL ({ int st_x = 3; st_x; }), ST_CALL ({ st_f(); }),
                ST_CONSTFN ({ st_cf(); }, a const function), ST_STATIC ({ static int st_q = 1; st_q; }),
                ST_NESTED ({ int a = ({ 2; }); a + 1; }), ST_READ ({ st_counter; })
both:           NC_COMPLIT ((int){5}) exit 0, 5 5 5
```

All seven have one cause. Clang's *static assertion failed due to requirement
'__builtin_constant_p((__extension__ ({* is printed over several lines, so the one-line reader
never finds `heroes-ffi-const` (`grep` of each `.build`). α2 moves 2 of the 9 rows that exit 2
today. No regression: the five values the trunk tells are told the same. **The leftover is
`blocking` by the rule's own words**: a recovery item cannot close over a row that exits 2
(`.claude/rules/verification.md` § Bounded discovery, *never a row that breaks design.md §4.17's
promise (... an exit 2), which is `blocking`*). So α2 does not close defect 2101 until the
multi-line reader is built and these seven are told (`ffi_not_constant`, as `NC_CALL` is).
The report's *10 to 20 lines* is an estimate and stays one.

## What a route nobody listed would have to look like, and a prototype of it

Every miss and false refusal above comes from one gap. F2 judges a **reconstruction** of the
accessor (another place, another level, another reading of the optimiser's output), and
reconstructions differ from the original exactly where a header is clever. The route that
removes the gap judges **the original**: the program's own accessor text, at the program's own
level, by executing it under UndefinedBehaviorSanitizer. Its premise is then UBSan's
documented runtime behaviour (every executed operation of the four kinds is checked), not the
optimiser's folding.

Prototype, by hand, no compiler change (`<scratchpad>/216-critic2-m/ux/ux.sh`). The trunk's own
emitted unit (`heroes-seed build NAME.hero --emit-c`) is compiled with `-Dmain=
hero_probe_hidden_main -c` at the program's level, with the compiler's `flags()` words and
`-fsanitize=<the four>`, then linked with a two-line `main` that calls the accessor and run.
A `NAME.hero:2:12: runtime error` names the constant on its own line. 48 shapes (`k.h` 4,
`s.h` 18 the trunk builds, `d.h` 19, `ini.h` 5, `sh2.h` 2) at `-O0` and `-O2` (`uxall.sh`,
`ux/all.txt`):

- **22 flagged and 26 kept at each level**, the same 48 verdicts at both. Flagged: every
  undefined integer shape the trunk builds (19, `S_CMP` declared `i32` among them) and the
  three floating constants F2 does not ask (`I_F64`, `I_F64B`, `I_F32`). Kept: all 22 values C
  defines (`D_OFFSET_HAND`, `D_PTRINT`, `D_COMMA`, `D_FDIV` among them), and the four
  initializer-folded constants (`I_ENUM`, `I_STATIC`, `S2_ENUM31`, `S2_ENUMDIV`): **missed, as
  F2 misses them**. Execution sees no operation there either.
- **The six attack shapes, at both levels, all nine cells right** against the programs' own
  `--sanitize` runs: at `-O0` `A_BCP_MISS`, `A_LINE_MISS` and `A_FILE_MISS` are flagged, and
  `A_BCP_LOUD`, `A_COUNTER_MISS` and `A_BCPG_MISS` kept; at `-O2` `A_BCP_MISS` is kept,
  `A_BCP_LOUD` flagged, `A_COUNTER_MISS` kept.
- **Cost on this Mac** (`time3.sh`, `/usr/bin/time -p`, three runs each, load 1.8 to 2.2): compiling
  `K_DIV`'s 12 KB unit 0.02 s, linking 0.03 s, running 0.00 s, **and 0.18 s on the first run of
  each new binary** (macOS checks a new executable once; every cold build makes a new one).
  The runtime object was compiled once beforehand.

What it would owe before it is a route rather than a prototype, each unbuilt and unmeasured:

- **The accessors in a unit of their own**, which the program compiles and links as today
  and the probe recompiles under UBSan, so the probe's text *is* the program's text (its
  `#line`, its `__COUNTER__`, its kept names). Recompiling the whole unit, as my prototype did,
  would cost a large program a second compile of its biggest units. That moves emitted C,
  like α2's 34 re-blessed files: architecture, which CLAUDE.md § 4 names as a panel trigger.
- **Execution at build time.** `heroes` has no flag that builds for another machine (`grep -n -i
  'target\|cross' selfhost/cli/table.hero` and a search for a `--target` spelling under
  `selfhost/cli`: nothing), so the probe runs where it is built. A future cross build would
  break this premise. The UBSan runtime must link on every platform: `--sanitize` already
  links it on three, and the Windows box was unreachable (unrun).
- **A two-sided canary of its own**: a known-undefined accessor that must report and a
  known-defined one that must not, in the same probe.
- **Not a reason to drop F2's reading where execution is impossible.** A cheaper step toward
  it: F2 can at least *see* the level dependence. In the `-O0` code of `A_BCP_MISS` and
  `A_BCP_LOUD` the probe function holds one `llvm.is.constant` call, and at `-O1` none
  (`clang -x cpp-output -O0|-O1 -S -emit-llvm`, `grep -c`). A third read at the build's level
  that refuses loudly any accessor holding `llvm.is.constant` or `llvm.objectsize` would turn
  the `__builtin_constant_p` miss into a told refusal. Unbuilt.

And neither F2 nor execution reaches the initializer-folded class. Its question is a third
one. For an enumerator it is C's own ICE rule, read by position on the bound enumerator alone.
For a `static const` object, clang printed nothing even under `-pedantic`.

## Pass 2's reading, and the conditions

- **F2 as built is not yet sound against its own question.** It is right on the 16 undefined and
  25 defined shapes the report measured, and right at `-O2` on every shape here. At the
  default `-O0` it misses three measured shapes in silence (`A_BCP_MISS`, `A_LINE_MISS`,
  `A_FILE_MISS`), and refuses two whose `-O0` program is defined (`A_COUNTER_MISS` at every
  level, `A_BCP_LOUD` at `-O0`). It leaves `f32`/`f64` constants with run-varying values. The
  shapes are contrived; the gap they expose is structural.
  **Conditions**: (1) floating constants asked, built and measured; (2) the probe's accessor at
  the unit's own `#line`, or the probe made from the unit's own text; (3) the level dependence
  seen, by a reading at the build's level or by execution, never judged at `-O1` alone; (4) the
  initializer-folded class named in the resolution as outside the route, with its question
  queued, since `enum { E = 1 << 31 }` builds silently where `#define E (1 << 31)` is refused.
- **The canary as written**: falsifiable for the quiet direction (measured, `wrapQ`), blind to
  the false one (measured, `wrapL`). **Conditions**: defined canaries that must call no handler,
  and one undefined canary per measured shape kind.
- **α2**: no regression on the five told values; closes 2 of 9 exit 2 rows of `blocking`
  defect 2101. **Condition**: the multi-line reader built, and the seven rows above told,
  before 2101 closes. Without it, 2101 stays open with them.

## Housekeeping (pass 2)

- Worktree `<scratchpad>/216-critic2/` (`6e2690b50` plus the patch), removed at the end with
  `git worktree remove --force`. Measurements in `<scratchpad>/216-critic2-m/`. Containers
  `heroes-linux-arm64` and `ubuntu:24.04`, run with `--rm`. No paid run, no credential printed.
- One file my own zsh slip made in the scratchpad, a `.hero` named after the whole list of
  names, was deleted (`find . -maxdepth 2 -name '* *' -delete`, 1). Nothing was written in the
  trunk but this section.
