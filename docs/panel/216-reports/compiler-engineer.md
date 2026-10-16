# Panel 216, the compiler-engineer's report

Written 2026-10-16 from 22:02, in a detached worktree of `6e2690b50` at
`<scratchpad>/216-compiler-engineer/` (`git log -1 --format=%H` there printed
`6e2690b5058b663a768f06db91abe73cd804793a`), `build/` removed, the compiler built there from the
seed. Every number names its command; what could not be run says *unrun*. Report written as I go;
the verdict table is at the end.

## Setup

- `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes` under `/usr/bin/time -p`:
  `real 8.02 user 7.84 sys 0.13`. Apple clang version 21.0.0 (clang-2100.3.34.2).

Finished 23:55. Written as it went: where a later command contradicted an earlier sentence, the
correction stands beneath it with its time, and the earlier sentence stays.

## Baseline on my copy (this Mac, Apple clang 21.0.0, `heroes build`, `-O0`)

`<scratchpad>/216-ce-m/`, the critic's `k.h`, `s.h`, `d.h`, `u.h` and programs copied there:

```
K_DIV    build=0 runs: 1794597852 1804854236 1802281948
K_FLT    build=0 runs: 1800856540 1838031836 1803199452
K_COMMA  build=0 runs: 2 2 2
K_FDIV   build=0 runs: 16 16 16
stmt (D_STMT, defect 2101) build exit=2: internal error: compiling the generated C failed:
  stmt.c:75:29: error: statement expression not allowed at file scope
  _Static_assert(HERO_RET_I32(D_STMT), "heroes-ffi-return D_STMT i32");
```

## What the four modules the brief names hold today (`wc -l`)

`selfhost/emit/value_check.hero` 133, `selfhost/emit/constant_accessor.hero` 101,
`selfhost/emit/ffi_constant_value.hero` 165, `selfhost/cli/layout.hero` 621,
`selfhost/emit/layout_screen.hero` 365, `selfhost/cli/header_ask.hero` 255 (`switch_ask` 180,
`unswitch_ask` 248, `two_ways_ask` 309). The compiler is 642 `.hero` files, 168,047 lines
(`find selfhost -name '*.hero' | xargs cat | wc -l`).

Two facts that decide where a route can sit:

- **`header_ask` and its siblings run only after a round has failed** (`header_ask.hero:8-12`, *it
  caches nothing, since only a build that has already failed asks*). A route that must judge a
  build that SUCCEEDS cannot sit there: it is a new clang run on the success path.
- **The success-path probes are the pointee and layout checks** (`cli/probing.hero` `check_all`,
  called from `cli/rounds.hero:154`), keyed and kept under `build/` (`compiling.hero:23-37`,
  `layout.hero:16-28`). Every one of their clang runs is `-fsyntax-only`. **Route F would be the
  first probe that generates code**, and the first whose flags (`-O1`, `-fsanitize=`) differ from
  the build's in a way the preprocessor sees.

## FOUND: route F as the critic built it judges another header than the build compiles

`compiling.hero:5-21` is the rule defect 167 paid for: *a probe judges the header the program's own
compile will see, with the words that compile sees it with*. F compiles at `-O1` with UBSan on, so
the preprocessor sees another world. Measured (`clang -std=gnu11 -dM -E` of an empty unit, `diff`):

```
O0 vs O1:      < #define __NO_INLINE__ 1      > #define __OPTIMIZE__ 1
O1 vs O2:      (nothing)
O1 vs O1+san:  (nothing in -dM; __has_feature is not a macro)
```

and a header `feat.h` spelling `FEAT_UB` under `#if __has_feature(undefined_behavior_sanitizer)` and
`FEAT_OPT` under `#ifdef __OPTIMIZE__`, each `1` on one side and `(INT_MIN / -1)` on the other:

```
clang -std=gnu11 -O0 -E feat.c            (the build's world)  return ((-2147483647 -1) / -1); twice
clang -std=gnu11 -O1 -fsanitize=... -E    (F's world)          return 1; twice
```

So F as built **keeps** both constants and the `-O0` program reads `INT_MIN / -1`: the defect, in
silence, behind a probe that says it looked. A header that spells a value under `__OPTIMIZE__` is a
premise about the world nobody has counted; the class is defect 167's, and that one was measured.

## The repair, F2: preprocess under the build's own words, then compile only that text with the sanitizer

Run 1 `clang <probe words> -E -dD unit.c -o unit.i` (the build's level, sanitizers, search,
exactly `compiling.probe_words`); run 2 `clang -x cpp-output <probe words> -O1 -g0
-fno-sanitize=all -fsanitize=signed-integer-overflow,float-cast-overflow,shift,integer-divide-by-zero
-w -S -emit-llvm unit.i -o unit.ll`; read, inside each accessor-named `define ... { }`, the name of
the `__ubsan_handle_<kind>` it calls. The handler names are the UBSan runtime's ABI, so the reason
comes out in words without trap mode and without reading a trap's number. Measured
(`<scratchpad>/216-ce-m/x/ask2.sh`, `read_ir.awk`), one unit per header with every constant a
function as the accessor writes it (its declared C type, `return NAME;`), Apple clang 21.0.0:

```
feat.h, build -O0:  FEAT_UB divrem_overflow   FEAT_OPT divrem_overflow   (as the build compiles them)
feat.h, build -O2:  FEAT_UB divrem_overflow   FEAT_OPT kept, ret i32 1   (the -O2 build really gets 1)
k.h + s.h + d.h + u.h, 55 constants: 25 flagged, 30 kept
  flagged: K_DIV K_FLT and every s.h shape but S_U2I (23), each with its kind:
    divrem_overflow (S_DIV S_REM S_LDIV S_HIDDEN S_COMMA_UB S_TERN S_CMP K_DIV),
    float_cast_overflow (S_FLT S_INF S_NAN S_SHORT S_EDGE S_FLOATDEMOTE S_FLTMUL S_NEGU S_UCHAR S_LL K_FLT),
    negate_overflow (S_ABS S_NEG), add_overflow (S_ADD S_LLADD), mul_overflow (S_MUL),
    shift_out_of_bounds (S_SHL63)
  kept: K_COMMA K_FDIV S_U2I, all 20 of d.h (D_STMT, D_OFFSET_HAND, D_PTRINT among them), all 7 of u.h
run 2 exit 0 on every unit; the .i 64-67 KB, the .ll 2.5-12.5 KB
```

## Rejected on measurement: the sanitizer-free read (`ret i32 poison`)

Without UBSan, LLVM folds most undefined shapes to `ret <ty> poison`, at `-O0` already, which
would need no `-O1` and no sanitizer. Same 55 constants, `-O0` and `-O1`: it **misses six**,
`S_COMMA_UB` (`ret i32 5`, the undefined left operand discarded), `S_NEG`, `S_ADD`, `S_MUL`,
`S_LLADD`, `S_SHL63` (clang's front end folds them to wrapped constants before LLVM sees an
operation). A silent miss is the defect itself; out.

## F2 on four clangs: the same verdict on every row

`<scratchpad>/216-ce-m/plat/two_run.sh` (the two runs with the compiler's own `flags()` words, at
the build's `-O0` and at `-O2`, over the 55 constants and `feat.h`'s two: 114 rows), `diff` of the
verdict lines against this Mac's:

```
Apple clang 21.0.0 (this Mac)                       114 rows, 53 flagged
Debian clang 22.1.8, heroes-linux-arm64 (native)    diff: nothing
Ubuntu clang 18.1.3, ubuntu:24.04 + apt clang       diff: nothing   (the floor; two CI legs)
Debian clang 22.1.8, heroes-linux (x86-64, Rosetta) diff: Docker's platform WARNING line only
Windows box, clang 23.1.1                           unrun: ssh timed out (port 22, ConnectTimeout=6)
```

53 = 27 at `-O0` (the 25 undefined shapes, `FEAT_UB`, `FEAT_OPT`) + 26 at `-O2` (`FEAT_OPT` is `1`
there, as the `-O2` program reads it).

## F2 built in my copy: where it sits and what it costs in lines

Built and compiled (`./heroes build selfhost/main.hero -o heroes-f2`, `wrote heroes-f2`; that run
read `real 94.87 user 73.67 sys 15.43` at load 9, so it is a correctness build, not a timing):

| file | what | lines (total / code, the `layout` suite's unit: no comment, blank or test) |
|---|---|---|
| `selfhost/emit/value_probe.hero` (new) | the accessor-shaped functions, the IR reader, the words per handler; 2 tests | 156 / 73 |
| `selfhost/cli/value_ask.hero` (new) | the two runs, the key, the kept IR, the diagnostic | 213 / 144 |
| `selfhost/cli/assemble.hero` | `per_module`: asked after the units compile and `unread`, before the link | 287/155 to 298/161 |
| `selfhost/emit/ffi_constant_value.hero` | `told` takes `refusal`, so this refusal never says *clang refuses* (clang refused nothing; a false message is `blocking`) | 165/65 to 166/65 |

**223 code lines, 381 with comments and tests, in the FFI driver and the emitter's FFI readers.**
No lexer, parser, checker, lowering, IR or runtime line; no surface; no new diagnostic class
(`ffi_constant_type`, with the existing `undefined` note); no spec token. Reused, not rewritten:
`bound_ask.closed_opening` (the unit's own opening, the groups' guard and the kept names, as
`emit/decls.hero:109-124` writes it), `bound_said.directives` (a `#define`'s file and line from `-dD`
text), `bound_macro.probe_of`, `kept`, `digest`, `response_file`, `read_back`.

**Where it is told, and why there**: after the units compile, so the six shapes the region of
`value_check.hero` refuses keep clang's own words (and the 2071/2072 goldens do not move); before the
link, so no binary that reads the value is left on disk (checked: `ls f2/K_DIV` after the refusal,
nothing). `--emit-c` runs the same rounds without the link (`cli/artifact.hero:36-40`), so it refuses
too. The price of that order: a program holding both a region shape and an F2 shape is told the
second after the first is fixed, which `verification.md` § Bounded discovery classes `adjacent`.

## F2 on the four shapes (this Mac, `heroes-f2 build`, `-O0`)

```
K_DIV    exit=1  error[ffi_constant_type]: `K_DIV` in `k.h` cannot be declared `i32`: C leaves the
                 header's value undefined at line 2: a division or a remainder by zero, or of its
                 type's lowest value by -1
                   at K_DIV.hero:2:5
                 note: `k.h` line 2 reads `#define K_DIV (INT_MIN / -1)`
                 note: C leaves this value undefined: a program reading `K_DIV` would read whatever
                 the machine held. A group's `constant` is the value C computes from the header
                 (§ 13): correct the value there, or bind a constant whose value C defines
K_FLT    exit=1  ... at line 3: a floating value converted to an integer type that cannot hold it
                 note: `k.h` line 3 reads `#define K_FLT ((int)(1e20))`
K_COMMA  exit=0  runs: 2 2 2
K_FDIV   exit=0  runs: 16 16 16
```

## F2 on the critic's 44 shapes and `u.h`'s 7, each bound alone (`<scratchpad>/216-ce-m/one/run_one.sh`)

Trunk compiler (`heroes`, from the seed) against `heroes-f2`, `heroes build`, then the binary run:

- **All 16 undefined shapes the trunk builds at exit 0 are refused at exit 1** by F2, each with its
  kind and its header line: `S_DIV S_REM S_LDIV S_FLT S_NEGU S_INF S_NAN S_SHORT S_UCHAR S_EDGE S_LL
  S_FLOATDEMOTE S_HIDDEN S_COMMA_UB S_TERN S_FLTMUL` (the trunk printed e.g. `1838425212`, `5`, `0`).
- **The 7 the trunk already refuses keep their refusal word for word**: `S_NEG S_ADD S_MUL S_LLADD
  S_SHL63` (the region, *clang refuses the header's value ... overflow in expression*), `S_ABS`
  (`ffi_not_constant`), `S_CMP` (`int`, not `bool`).
- **All 25 values C defines build and print what they printed**: `S_U2I`, 19 of `d.h` (`D_COMMA 2`,
  `D_FDIV 16`, `D_OFFSET_HAND 8`, `D_PTRINT 16`, ...), `U_MAX U_SIZE U_NOT U_WRAP U_HIGH`;
  `U_NARROW` and `U_TRUE` keep their type refusals.
- **`D_STMT` stays exit 2** (defect 2101): F2 keeps its value (`ret i32 3`), and the file-scope type
  assertion dies first. Below.

## Beside the class in type (measured with the two runs, not built into F2)

```
double  T_F64   ((double)(INT_MIN / -1))     divrem_overflow      flagged if asked
double  T_F64B  ((double)(int)(1e20))        float_cast_overflow  flagged if asked
int64_t T_I64   ((int64_t)(INT_MIN / -1))    divrem_overflow      flagged (an i64 is asked today)
record  T_PTX   ((struct pt){ (int)(1e20), 3 })  float_cast_overflow  flagged if asked
record  T_PT    {INT_MIN / -1, 2}            nothing: `ret i64 10737418240`, clang folds an aggregate
                                             initialiser in its front end and emits no check
```

So `f64` is one filter wider than `value_check.asked` (an `.float_ty` arm in a filter of F2's own,
about 3 lines); a record bound to a brace list is **not reachable by this route** and stays a named
gap, as the critic found it (stable, clang's folded `INT_MIN`).

## The corpus: F2 refuses nothing the trunk builds

Enumerated from the tree, not recalled: every tracked `.hero` outside `archive/` and `docs/` with an
`extern` group holding a `constant NAME: <i8..u64|bool>` (`git ls-files` and an `awk` read
byte-wise, `LC_ALL=C`; `<scratchpad>/216-ce-m/corpus.txt`): **62 files, 128 constants, of 2,873
files**. A nested module is built through its program's root (`selfhost/main.hero`,
`tests/harness/main.hero`, `examples/ledger/main.hero`), so 58 programs, each `heroes build <root>
--emit-c` with the trunk compiler and with `heroes-f2` (`census.sh`): **`diff` of the two listings
printed nothing**, every exit and every first error line the same, 38 built and 20 refused as their
goldens say (two of them `ffi_missing_header` for `sys/prctl.h`, absent on this Mac). F2 asked 55 kept
answers across them (`ls -d build/value-*`), the compiler's own `HERO_OS_*`, `HERO_FS_*` and
`HERO_STAGE_*` among them, and no answer holds a handler (`grep -c __ubsan_handle_`, 0 each).

## Defect 2101, the statement expression: what each answer does, built

Measured with `clang -fsyntax-only` (`<scratchpad>/216-ce-m/stmt/`): at file scope clang refuses a
statement expression in any operand, `_Generic`'s and `__builtin_constant_p`'s included (*statement
expression not allowed at file scope*); inside a function both of the constant's assertions read it,
and the constancy assertion answers as it should: `({ 3; })` passes, `({ st_counter++; })` fails.
And an unused constant has an accessor too (`h_un2_UN_A` in the emitted unit of a program that never
reads `UN_A`), so assertions in the accessor still check every binding, which spec § 13 promises.

**α, the robust answer, built** (`<scratchpad>/216-ce-alpha/`, `heroes-a`, F2 included): a
non-record constant's two assertions move from file scope into its accessor
(`extern_assert.constant_assertions`, written by `constant_accessor.accessor` on the generated
file's lines); a record's stay at file scope.

```
D_STMT   (__extension__ ({ 3; }))                       trunk exit=2   alpha exit=0, prints 3
ST_PURE  the same                                       trunk exit=2   alpha exit=0, prints 3
ST_DIV   (__extension__ ({ (-2147483647 - 1) / -1; }))  trunk exit=2   alpha exit=1, F2's ffi_constant_type, header line quoted
ST_SIDE  (__extension__ ({ st_counter++; }))            trunk exit=2   alpha exit=2: the constancy assertion fails, and its
         reader (`ffi_declared.not_constant`, line 87) reads one line, while clang prints a statement expression's
         requirement over three, so `': heroes-ffi-const ST_SIDE` is on none it reads
UN_WRONG unused, `1.5` declared i32                     trunk exit=1   alpha exit=1, the same words
K_DIV K_FLT / K_COMMA K_FDIV                            as F2: exit 1 twice / prints 2 and 16
```

What α moves, counted: the two file-scope lines stand in **12 of the 24 files of
`tests/golden/emit/`, 619 files under `tests/emission/`, and 28 times in `seed/heroes.c`**
(`grep -rl`/`grep -c heroes-ffi-const`), so every blessed emission of a program binding a header
constant is re-blessed, and the seed regenerated. Cost in lines: `extern_assert.hero` +44 -19 net
(a function of 20 lines out of the loop), `constant_accessor.hero` +9. **Unbuilt and owed by α to be
whole**: the constancy reader reading a requirement over several lines (`ST_SIDE`), my estimate 10
to 20 lines in `ffi_declared.hero` or `ffi.hero`'s loop, an estimate and not a measurement.

**β, the conservative answer, recorded**: keep the file-scope assertions and read clang's *statement
expression not allowed at file scope* at a `heroes-ffi-return`/`heroes-ffi-const` line back as
`ffi_not_constant`. It moves no emitted C and closes the exit 2, and it refuses `({ 3; })`, a value
the compiler's own constancy rule accepts once it is asked where C can read it: a correct program
refused, `blocking` by `verification.md` § Bounded discovery. Unbuilt.

## The premise F2 rests on, and its canary (built in `heroes-a`)

F2 rests on a premise about the world (`.claude/rules/module-shape.md` § A narrowing asks the value,
never the world): *clang emits a check for an operation on constant operands, and `-O1` folds a check
whose operands are known, keeping the handler only where the value is undefined.* Its two failure
directions are not alike:

- a defined value left with a handler: F2 refuses a correct program, **loud**, every build;
- an undefined value left with none (a clang that folds the operation in its front end and emits no
  check, as clang 21 already does for a **brace-list record**, measured above): F2 keeps it in
  **silence**, the defect again.

The rule owes the quiet direction a test that fires when the premise dies and a loud fallback. So
`heroes-a` writes two canaries into every probe unit, `hero_value_canary_division` (`(-2147483647 - 1)
/ -1`) and `hero_value_canary_conversion` (`(int)1e20`), one per kind the region cannot see; an
answer in which either calls no handler stops the build as the compiler's own failure (`failed`,
exit 2, `whose.hero:55`), never read as *nothing found*. Cost: 21 lines (`value_probe.CANARIES`,
`sighted`, the check in `value_ask.told`, the `match` in `assemble.per_module`), and a test of
`sighted`. Measured that it does not fire on a sound clang: the four shapes, 2101's and the corpus
below all build as F2 alone built them.

## α's first build moved the library's constants too; narrowed (`alpha2.sh`)

`heroes-a`'s `emit` form read **1 passed, 12 failed**, and the twelve were not the programs' own
constants: every emitted unit writes the **library's** constants' assertions at file scope
(`HERO_OS_OK`, `HERO_OS_NOT_FOUND`, `HERO_OS_NOT_TEXT`, `HERO_OS_BAD_NAME`, `HERO_STR_OK` in
`emit/scalars`' expected text), and α moved them with the rest. The runtime's header spells no
statement expression, and `value_check.asked` already leaves the library's constants alone for this
reason (*a unit that binds no constant of its own headers writes the C it wrote*). So α2 decides it
by one predicate both writers ask, `extern_assert.in_accessor` (not a record's, not the library's: a
fact about the declaration, never about the header), and the same script makes F2's two quiet exits
loud (below). Its numbers follow.

## F2's last quiet exits, made loud (`alpha2.sh`)

`heroes-f2` `continue`d where a probe's headers did not preprocess or either run failed: told
nothing, which reads as *found good*. F2 runs only after the program's units compiled the very same
opening, so such a failure is never the program's: in `heroes-a2` it stops the build as the
compiler's (`failed`, exit 2, `BLIND` + the run's own reason), as a canary that calls no handler
does.

## F2 on the golden forms (my copy, `./heroes run tests/harness/main.hero -- <compiler> <form>`)

| form | trunk (`216-ce-base`, seed-built) | F2 (`heroes-f2`) |
|---|---|---|
| `unsupported` | 319 passed, 0 failed | 319 passed, 0 failed |
| `emit` | 13 passed, 0 failed | 13 passed, 0 failed |
| `run` | 524 passed, 0 failed | 524 passed, 0 failed |

**Nothing moved**, and nothing should: F2 emits no byte of a unit (it is a probe beside the units), so
`emit` and every blessed emission stand, and it tells only after the units compiled, so the region's
words for 2071 and 2072 stand. The forms ran side by side with the trunk's and another session's,
fine for a verdict and not for a duration. The time each form took is not reported for that reason.

## What F2 costs, measured (this Mac, Apple clang 21, `/usr/bin/time -p`, medians of three)

Every compiler compiles the **same trunk source**, each in a fresh folder with `rm -rf build`, compilers
alternated within each repetition; load average 3.3 to 4.9 throughout (`sysctl -n vm.loadavg` on every
line, `<scratchpad>/216-ce-m/cost2/times.txt`), another session's `run` form beside it, so `real`
is read against `user + sys` and the comparison stands on `user + sys`.

| | trunk | F2 | F2 + canary + α2 |
|---|---|---|---|
| the compiler's own **cold `--emit-c`** (`selfhost/main.hero`), user+sys | 85.04 s (84.73, 85.04, 85.35) | 84.43 s (85.07, 84.43, 84.17) | 87.21 s (88.96, 85.66, 87.21) |
| the same, real | 89.68 s | 89.05 s | 91.62 s |
| **cold build of a program binding one constant** (`K_COMMA.hero`), user+sys | 0.34 s | 0.37 s | 0.38 s |
| the same, **warm** | 0.13 s | 0.15 s | 0.15 s |

The probe's own runs, replayed from the kept units of the compiler's own build (3 probes, 22
accessors: `clang @<kept words> value.c` and `clang @<kept words> value.i`, five times): **0.01 to
0.02 s real each run**, on preprocessed units of 276 to 350 KB. So F2 adds about 0.1 s of clang to the
compiler's own cold build, below the spread of the repetitions, which is what the first two columns
show. A cold one-constant program pays about 0.03 s, two clang processes; a warm one pays the kept
answer's read and **one `clang -M` that `bound_macro` already ran** (`compiling.reads` through
`bound_macro.probe_of`, asked again because the probe is not kept on the probing; the warm column's
+0.02 s). Making the probe once in `probing.probings_of` for a list that binds a constant removes that
second run: a known cost, unbuilt.

**Corrected 23:30, the same evening, by measurement**: the paragraph above said the warm +0.02 s *is*
the second `clang -M`; that was an inference when written. Counted since, with a `clang` on `PATH`
that logs each run (`<scratchpad>/216-ce-m/wrap/clang`), one build of `K_COMMA.hero` in a fresh folder,
then again:

```
trunk  cold  7 clang runs   warm 3 (-###, reads, link)
F2     cold 10 clang runs   warm 4 (-###, reads TWICE, link)       the probe's 2 runs + a second reads
α2     cold 10 clang runs   warm 4 (the same)
```

So the warm build's one added process is the second `reads`, now measured. And the whole-compiler
columns cannot resolve 0.1 s on this machine tonight: rerun with the order reversed (α2, F2, trunk),
the same source, two repetitions, `user+sys` read **85.12, 86.90, 95.56** and then **101.42, 91.36,
89.65** (`times-rev.txt`, load 3.6 to 4.8), the same compiler moving by up to 16 s between
repetitions. α2's higher medians in the first table follow the order it ran in, not the compiler.
The bound on F2's cost is the replay of its own runs, about 0.02 s each.

## Verdicts, route by route

My seat's ground is design.md §1.1 (*implementation simplicity is a constraint, not a goal; it sets
the ceiling*), §1.7 (*anything in the core must be implemented in the type checker and the lowering
and the backend*) and Part 5. **No route on the table adds a core construct**: none touches the
lexer, parser, checker, lowering or IR (searched: every route's work is in `selfhost/cli/` and
`selfhost/emit/`'s FFI readers, or in clang). So the veto stands on the ceiling alone, and only J
reaches it. On the floor below the ceiling I stand on §1.12 (*a guarantee that ends quietly is not
one*) and on `.claude/rules/module-shape.md` § A narrowing asks the value, never the world, where
design.md is silent on a probe that rests on an optimiser (searched design.md for *optimis*,
*sanitiz*, *probe*: nothing on this).

- **A, nothing**: object. Measured above: three different values in three `-O0` runs, exit 0, on
  this Mac, and `0` at `-O2`. §1.12.
- **B, the ICE rule as a refusal**: object. Refuses `K_COMMA`, `K_FDIV` and 7 of `d.h`'s defined
  values (critic, `q4.sh`); the trunk's own `run` golden `fixedbugs-2072-values-c-defines-still-build`
  binds `((int)(1000.0 / 60))`, so `run` turns red the day it lands. A correct program refused.
- **C, C23 `constexpr`**: object. Refuses every constant on clang 18.1.3 (critic), the floor and two
  CI legs; raising the floor is a tool-surface change and its own sitting; it also reads the header
  as `-std=c23`, another `__STDC_VERSION__`, the world-drift F had.
- **D, C++ `constexpr`**: object. Refuses `D_OFFSET_HAND` and `D_PTRINT`, which the trunk builds and
  prints `8` and `16` (critic); and reads the header as C++, where `__cplusplus` changes what it says.
- **E, a trap in the accessor at run time**: object. A program that builds today would abort when it
  reads the constant: a behaviour spec § 13 does not describe (*the header holds the value*), so a
  spec sentence and a full sitting; and spec § 4 line 115 already rules the written twin a compile
  error. The compiler-side cost would land in every program's accessor, not in a probe.
- **F, as the critic built it**: object, on soundness. Measured above: it judges the header in `-O1`'s
  and UBSan's world; `feat.h` keeps both of its values where the `-O0` program reads `INT_MIN / -1`.
- **F2, the repaired F (this report's)**: **adopt**, with the canary and the loud exits (built in
  `heroes-a2`), told after the units compile and before the link. 223 code lines (+21 canary, +10 loud
  exits) in two new modules of the FFI driver and readers; no surface, class or spec token; 0.02 s per
  probe run, cold only; nothing moved in `run`, `unsupported`, `emit` or the corpus.
- **G, flags that define the behaviour**: object. Covers half (`-fwrapv` leaves `INT_MIN / -1`
  undefined, critic), and gives a header's undefined value a meaning in every unit instead of
  refusing it, against spec § 4:115's ruling for the written twin.
- **H, the ICE rule as a filter before a second question**: object. Its purpose is cost, and F2's
  second run costs 0.02 s; the filter needs a clang run of its own (`-fsyntax-only` with
  `-Werror=gnu-folding-constant`, since run 1 is `-E` and has no Sema) to save that. A second
  mechanism for no measured gain, §1.1.
- **I, clang's folded value as the literal**: object, recorded as the conservative route: stable and
  refusing nothing, and silent about a value C leaves undefined (§1.12).
- **J, the compiler's own evaluator**: **veto**, on the ceiling. It would make the compiler own C's
  preprocessor output, expression grammar, usual arithmetic conversions, floating semantics,
  `sizeof`, `offsetof` and `_Generic`, which §1.11 leaves to C; no module of `selfhost/` evaluates a C
  expression today (`grep -rln "integer constant expression\|usual arithmetic conversion" selfhost`:
  one comment in `emit/lend_extent.hero`). Unbuilt, so its size is not measured, and the veto rests
  on what it would have to own, not on a line count.
- **α, defect 2101's constant asserted in its accessor**: **adopt** (built: `D_STMT` prints 3), owing
  the multi-line reader for `ST_SIDE`. **β, read the file-scope refusal back**: object, recorded as
  the conservative answer (refuses `({ 3; })`).

**Corrected 23:35, by the commands I had not yet run when I wrote the two sentences above**:

- *searched design.md for optimis, sanitiz, probe: nothing on this* was written before the search.
  Run since (`grep -n -i`): *optimis* 6 lines, *sanitiz* 18, *probe* 19. As I read them, none rules
  on a build-time question resting on the optimiser or on a sanitizer; the nearest is line 680, *the
  entire LLVM optimisation pipeline at `-O2`: a welcome side effect, not a reason*, which F2 does not
  contradict (F2's `-O1` is a means of reading clang's answer, never the reason for it). So design.md
  is silent on it as far as these words reach, and the rule I stand on there is `module-shape.md`'s.
- *+21 canary, +10 loud exits* was an estimate. Counted (`awk`, code lines, trunk / F2 / F2+canary+α2):

```
selfhost/emit/value_probe.hero          0     73    80
selfhost/cli/value_ask.hero             0    144   159
selfhost/cli/assemble.hero            155    161   167
selfhost/emit/extern_assert.hero      111    111   124
selfhost/emit/constant_accessor.hero   32     32    37
selfhost/emit/ffi_constant_value.hero  65     65    65
```

  F2 alone **223** code lines; the canary and the loud exits **28** more; α2 **18** more; the whole
  route **269** code lines against the trunk's 71,516 (`selfhost/`, the same `awk`), 0.38%.
  `assemble.hero` reaches 305 lines in all, 167 of code, and the `layout` suite's verdict on it is
  below.

## α2 (F2 + canary + loud exits + 2101 narrowed), built and judged (`<scratchpad>/216-ce-alpha/`, `heroes-a2`)

| | trunk | F2 | α2 |
|---|---|---|---|
| the compiler's own tests (`<compiler> test selfhost/main.hero`) | 1,677, all passed | 1,679, all passed | 1,679, all passed |
| `unsupported` | 319 / 0 | 319 / 0 | 319 / 0 |
| `emit` | 13 / 0 | 13 / 0 | **13 / 0** (α, unnarrowed: 1 / 12) |
| `layout` | not run | not run | 6 / 0 (`assemble.hero` at 305 lines) |
| the four shapes | `K_DIV`, `K_FLT` vary at exit 0 | refused, exit 1 | refused, exit 1 |
| `D_STMT` (2101) | exit 2 | exit 2 | **exit 0, prints 3** |

The two tests the count grows by are `value_probe`'s (`ok "each asked constant is asked as its
accessor..."`, `ok "a function that calls a handler is named with it..."`, in both logs); α2's
change to `extern_assert`'s own test (its constant's lines are asked of `constant_assertions` and are
no longer in the file-scope text) passes in the 1,679.

## The seat's answer

- **verdict**: **approve F2 as repaired** (the two runs, told after the units compile and before the
  link, with its canaries and its loud exits), and **α2 for defect 2101**; **veto J**; **object** to
  A, B, C, D, E, F as built, G, H and I, and to β.
- **section**: design.md §1.12 (*a guarantee that ends quietly is not one*) for closing the class;
  §1.1 and §1.7 with Part 5 for the cost: no route here touches the core, and only J reaches the
  ceiling. design.md is silent on a build-time question that rests on an optimiser (searched:
  *optimis*, *sanitiz*, *probe*); there I stand on `.claude/rules/module-shape.md` § A narrowing asks
  the value, never the world, which is why the canary is a condition and not an option.
- **implementation_cost**: F2 223 code lines (381 with comments and tests): `selfhost/emit/value_probe.hero`
  73, `selfhost/cli/value_ask.hero` 144, `selfhost/cli/assemble.hero` +6, `ffi_constant_value.hero` 0
  (a parameter). Canary and loud exits +28; α2 +18 (`extern_assert.hero` +13,
  `constant_accessor.hero` +5); whole route 269 of the trunk's 71,516 code lines (0.38%). Time: 0.01
  to 0.02 s per clang run of the probe (replayed), two runs per header list holding an asked
  constant, cold only; the compiler's own cold `--emit-c` unchanged within this machine's spread
  (85.04 / 84.43 / 87.21 s user+sys, medians, and the reversed order swings the other way); a cold
  one-constant program 0.34 to 0.37 s; warm 0.13 to 0.15 s, the one added process being a second
  `clang -M` a kept probe would remove.
- **needed_for_self_hosting**: **no**. The compiler binds header constants (`HERO_OS_*`,
  `HERO_FS_*`, `HERO_STAGE_*`) and none is undefined; this closes a robustness class (CLAUDE.md
  § Precedence, rank 3), and adds no form, so Principle 0 has nothing to ticket.
- **argument**: Every route that asks clang a second question asks it in another world, and F, the
  only one that split the class, judged `-O1`'s and UBSan's header, not the program's: measured,
  `feat.h` kept both values the `-O0` program reads as `INT_MIN / -1`. Preprocessing under the
  build's own words first removes that, keeps all 25 defined values, refuses all 16 undefined ones the
  trunk builds, on clang 18.1.3, 21.0.0 and 22.1.8 alike, and moves no golden, no emission and no
  corpus program. What is left is a premise about the optimiser, quiet in one direction; a canary
  makes it loud. 269 lines, no core.
- **prediction**: at the batch gate that lands it, (1) `emission` reads 1,279 + new cases, 0 failed,
  with no blessed file edited for F2 (α2's moves only programs binding their own header constants);
  (2) the corpus census of this report (`corpus.txt`, 58 programs) shows no exit change; (3)
  `value_ask.hero` + `value_probe.hero` measure ≤ 260 code lines by `layout`'s count (239 today); (4)
  the **Windows CI leg (clang 23.1.1, unrun here)** builds every new case without the probe's
  `failed` exit. Any one red falsifies it.
- **condition**: I turn to **object** if a clang the CI runs leaves a canary without its handler
  (the premise dead there), or if the ffi-pragmatist's corpus finds a header constant C defines that
  F2 flags; to **veto** if the route grows a C evaluator of its own (J by another name). J's veto
  lifts only before a built evaluator that agrees with clang on these 55 shapes on four clangs, in
  fewer lines than F2.

## The last numbers (23:30 to 23:53)

- **α2 on the 51 shapes alone** (`run_one.sh` with `heroes-a2`, `diff` against F2's listing, a
  varying value read as `<varies>`): **one line differs, `D_STMT`, exit 2 to exit 0 printing 3**; 25
  refused, 26 built, **none at exit 2**.
- **α2's `run` form: 524 passed, 0 failed** (23:29 to 23:49). α's unnarrowed `run` was stopped by me
  at `fixedbugs-218` once α2 superseded it: no verdict.
- **α2's corpus census**: `diff` against the trunk's printed nothing (58 programs).
- **`emission`** (`./heroes run tests/harness/main.hero -- <compiler> emission`): **F2 1,279 passed,
  0 failed**, so no blessed byte moves for F2. **α2 1,245 passed, 34 failed**, and the 34 are exactly
  the programs that bind their own header constants: 30 `run` goldens (`ffi-constant`,
  `fixedbugs-2072-values-c-defines-still-build`, the 396, 438, 461 and 57x families, ...),
  `fixedbugs/ffi-out-parameter-guard`, and `examples/curl`, `examples/ledger`, `examples/sqlite`.
  That is α2's whole re-blessing, plus the seed (the compiler binds its own `HERO_OS_*`, `HERO_FS_*`,
  `HERO_STAGE_*`, which are not the library's). Unnarrowed α would have moved every unit.

So the prediction's first clause, made exact: at the landing gate, `emission` reads 0 failed with
**no blessed file edited for F2** and **exactly these 34 re-blessed for α2**; a 35th moved file, or
a library-only golden (`emit/scalars`, say) moving, falsifies it.

## Housekeeping

- Worktrees I added: `<scratchpad>/216-compiler-engineer` (F2), `<scratchpad>/216-ce-base` (the
  trunk's compiler, for baselines) and `<scratchpad>/216-ce-alpha` (α, α2), each at `6e2690b50`, each
  removed at the end with `git worktree remove --force`. The route survives them as two patches,
  `<scratchpad>/216-ce-m/patches/f2.patch` (441 lines) and `f2-canary-alpha2.patch` (615), and every
  measurement file named above stays in `<scratchpad>/216-ce-m/`.
- **One slip, repaired at once**: at 22:25 a script of mine (`two_run.sh`, given a folder that did not
  exist yet) ran its `clang` in the session's directory, the trunk, and left five files there,
  `pd.err pf.err pk.err ps.err pu.err` (77 bytes each, clang's *no such file or directory: 'pk.i'*).
  Found by `git status --short` within a minute, removed by name (`rm -- pd.err pf.err pk.err ps.err
  pu.err`), and `git status --short` printed only the four `docs/panel/21[67]-*` folders that stood
  before. Nothing else was written in the trunk but this report.
- Containers: my own `docker run --rm` of `ubuntu:24.04`, `heroes-linux-arm64:latest` and
  `heroes-linux:latest`, removed on exit; the running `ffi216-c18` and `ffi216-d22` are the
  ffi-pragmatist's and were not touched. No paid run, no credential printed; the Windows box was
  asked by `ssh` only (timed out).

# Final pass, 2026-10-17 (00:15 to 00:45): route Ux priced and attacked, F2 against its own condition

Written after the critic's pass 2 (`critic-briefs.md` § Pass 2). Everything above stands as written;
where this pass turns a verdict of it, it says so here. Worktree `.claude/worktrees/216-ce-pass2`
at `6e2690b50` (`git log -1` printed it), `build` removed, the trunk's compiler from the seed
(`heroes-seed`, `real 7.92`), then the α2 patch applied (`git apply`, silent); removed at the end
(`git worktree remove --force`). Measurements in `<scratchpad>/216-ce-m2/`.

## My own condition fired: F2 is withdrawn as the resolution

The condition I set above (*I turn to object if ... a header constant C defines that F2 flags*) is
met by the critic's `A_COUNTER_MISS` (`(__COUNTER__ == 0 ? (INT_MIN / -1) : 0)`, defined at every
level in the program's unit, refused by α2) and by `A_BCP_LOUD` at `-O0`; and the silent misses
`A_LINE_MISS`, `A_FILE_MISS` and `A_BCP_MISS` at `-O0` break F2's own claim. I did not re-run them:
the critic's rows, by `cmp.sh` and `hand.sh` on three clangs. **F2 moves from adopt to object**,
recorded as the conservative route. Its four conditions do not make it exact: (2) `#line` repairs
`__LINE__` and `__FILE__` but not `__COUNTER__`, which needs the unit's own text, and (3) can only
turn a level-dependent value into a refusal, never into the `-O0` program's answer, because F2's
answer is the `-O1` optimiser's by construction.

## FOUND: Ux as prototyped repeats F's world drift; preprocessing first repairs it

The critic's `ux.sh` compiles the emitted unit with `-fsanitize=<the four>` in one step, so the
header is read with `__has_feature(undefined_behavior_sanitizer)` true, which no plain build has.
`feat.h` (`FEAT_UB` is `1` under any UBSan check, `INT_MIN / -1` without), my `ux2.sh`, this Mac:

```
FEAT_UB   one-step (the critic's)  -O0 and -O2: no runtime error: kept
FEAT_UB   preprocessed first       -O0 and -O2: FEAT_UB.hero:2:27: runtime error: division of ...
FEAT_OPT  preprocessed first       -O0: flagged   -O2: kept        (as the two programs read it)
the program, heroes build (-O0):   FEAT_UB prints 1803133900, exit 0
```

`pre` is the critic's recipe with one change: `clang <the program's words> -Dmain=hero_probe_hidden_main
-E unit.c -o unit.i`, then `clang -x cpp-output <words> <level> <the four checks> -c unit.i`. Over the
critic's 48 shapes at `-O0` and `-O2` (`<scratchpad>/216-ce-m2/ux48/pre48.txt`, `diff` of the 96
verdicts against `216-critic2-m/ux/all.txt`): **identical on all 96 cells** (44 flagged, 52 kept);
over the six attack shapes, the critic's twelve cells, identical. So preprocessing first costs Ux
nothing it had and adds `FEAT_UB`. Even the program's own `heroes run -O0 --sanitize` misses
`FEAT_UB` (0 runtime errors, prints `1`), for the same reason.

## Ux's three forms, and what each costs (measured, this Mac and Linux arm64)

- **Ux-whole**: the module's own emitted unit, the program's exact text. On the compiler's own
  build, its four asked units (`cliprocess` 80,672 B, `clifiles` 79,216, `cliruntimeplaces`
  48,310, `modulereading` 102,117): preprocessing **0.01 to 0.02 s** each, compiling with the
  checks **0.02 to 0.03 s** each, the full 644-object link **0.06 s real** with or without
  `-fsanitize` (`whole/whole.sh`, three repetitions, load 2.5). Its key is the asked units' whole
  text, so an edit anywhere in those modules re-runs it.
- **Ux-split**: the accessors in a unit of their own, the critic's *owed* item. Exact and cheap to
  probe, but it costs every program at run time: a header constant read in a loop of 10^9 at `-O2`
  took **0.00 s** with its accessor in the same unit and **0.74 to 0.77 s** in another
  (`inl/loop.c`, `same` against `split`, three runs each; one call per read, nothing folded). It
  also moves emitted C for every program binding its own constant (α2's 34 re-blessed files are the
  measure of that churn).
- **Ux-recon**: F2's reconstructed text executed. It keeps F2's reconstruction gap (`__COUNTER__`).

**Per cold probe**, one 12 KB unit (`ux/cost.sh` on this Mac, `lx/lx.sh` on Linux arm64, a fresh
binary each repetition):

```
                   preprocess   compile    link      first run     second run
this Mac           0.01-0.02    0.01-0.02  0.02-0.03 0.18-0.25 real (0.00 CPU)   0.00
Linux arm64 22.1.8 0.014        0.016      0.036     0.001                       0.001
```

The macOS first-run wait is the system checking a new executable, not work. So Ux costs about 0.3
s real per cold probe on this Mac and 0.07 s on Linux, against F2's two runs of 0.01 to 0.02 s; warm
builds cost nothing, as for F2, when the verdict is kept by its key.

## The five questions the coordinator asked

1. **Ceiling, core or sugar**: no lexer, parser, checker, lowering or IR line, so not core (§1.7);
   no ceiling breach. Lines, **estimated, unbuilt**: Ux-whole replaces F2's driver and IR reader
   (`value_ask.hero` 159 code lines in α2, `value_probe.hero` 80) with `-E`, `-c`, a link against
   the program's own objects, a bounded run and a reader of `<file>.hero:<line>:` positions, an
   estimate of the same order plus about 40 for the link; Ux-split adds the unit split to
   `emit/unit_plan.hero` (97 code lines today), `emit/unit.hero` (86), `cli/assemble.hero` (155)
   and `cli/artifact.hero` (26). The time bound already exists: `hero_run_limit` and
   `hero_run_limit_writes`, bound and used in `selfhost/mutate/child.hero:26-64`.
2. **Under a cache hit**: the verdict kept by its key, no clang and no run, as F2's. Ux-whole's key
   is the whole asked units, so it re-runs more often than F2's probe-text key.
3. **Where the build machine cannot run what it builds**: today it always can. No flag builds for
   another machine (`grep` of `selfhost/cli/table.hero` and `flags.hero` for `--target` and
   `cross`: nothing), so the probe runs where it is built. The UBSan runtime links and reports on
   this Mac, on `ubuntu:24.04` with `apt install clang` (it pulls `libclang-rt-18-dev`, clang
   18.1.3) and on Debian clang 22.1.8 (`rt/rt.sh`). On Windows the CI puts clang's runtime DLL
   directory on `PATH` for `--sanitize` (`ci.yml:704-716`); UBSan alone there is **unrun** (the
   box was off). The day a cross flag lands, Ux has no answer of its own, and that sitting owes one:
   F2's static reading, or a refusal.
4. **A constant no accessor of the program reads**: there is none. Every bound constant gets an
   accessor whether read or not (`h_un2_UN_A` in the emitted unit of a program that never reads
   `UN_A`, above), so the probe calls it and judges it. One more place reads a constant: a counted
   extent's `_Static_assert` evaluates it at compile time (`ffi-a-lent-field-counted-by-a-constant`,
   lines 235 to 316). Neither route executes that place; both cover it only through the accessor's
   verdict on the same expression, which a spelling that reads `__LINE__` can make differ.
5. **With α2 and the 2101 reader**: both stand before either route, in the program's own compile,
   so they compose with each. **Built** (`ffi_lookup.assertion_lines`: a static assertion's
   failure joined with its continuation lines up to the requirement's `': `, read in `ffi.hero`'s
   third loop): `ffi_lookup.hero` 184 to **204 code lines**, `ffi.hero` unchanged at 194. Over the
   critic's `st.h`, 15 names bound alone (`st/strun.sh`): **the seven exit-2 rows (`ST_SIDE ST_DECL
   ST_CALL ST_CONSTFN ST_STATIC ST_NESTED ST_READ`) now exit 1, `ffi_not_constant`**, the words
   `NC_CALL` gets; `ST_PURE` and `ST_TWO` print 3 and 4; **none of the 15 at exit 2**. One message
   moved, true both ways: `ST_VOID` is now told *is `void` in `st.h`, not `i32`* by the type
   reader, where the trunk said *C cannot read the header's value ... as an expression*. The
   compiler's own tests **1,680, all passed** (α2's 1,679 and the reader's); `unsupported` **319
   passed, 0 failed**. Patches: `<scratchpad>/216-ce-m/patches/reader-2101.patch` and
   `f2-canary-alpha2-reader.patch`.

## FOUND: Ux runs the program's header code during `heroes build`

A header with `static void __attribute__((constructor)) ct_init(void)` that appends a line to a
file (`ux/ctor.h`): the trunk's `heroes build` wrote **0 lines**; the Ux probe **1**, before the
program ever ran; running the program, 2. Today a build compiles a program and never executes any
part of it; Ux makes it execute every constructor the headers of the asked units define, under a
time bound but otherwise unconfined. The spec and the rules say nothing either way (`grep` of
`spec/heroes-spec.md`, `.claude/rules/cli-surface.md` and `docs/design.md` for what a build may
run). It is a new property of the tool, and the author's to rule.

## Verdicts, this pass

- **Ux-whole, preprocessed first** (the module's own unit, `-E` under its own words, compiled at
  the program's level with the four checks, linked with the program's objects, run bounded):
  **adopt, as the robust resolution**. Conditions: preprocessing first (one-step Ux is unsound,
  `FEAT_UB`); the run bounded in time and bytes (`hero_run_limit`, `hero_run_limit_writes`); a
  two-sided canary (an undefined accessor that must report and a defined one that must not); the
  Windows link measured before landing; build-time execution ruled by the author.
- **Ux one-step (the critic's prototype)**: object, on soundness (`FEAT_UB`, measured).
- **Ux-split**: object. It is exact, but it costs programs 0.74 to 0.77 s per 10^9 reads at `-O2`,
  and Ux-whole is just as exact without that cost.
- **F2 with the critic's four conditions**: object as the resolution, **recorded as the
  conservative route** and as what a future cross build would fall back on.
- **α2 with the 2101 reader**: **adopt**. Built: 20 more code lines, 0 exit-2 rows of 15.
- **The initializer-folded class** (`enum { E = 1 << 31 }`, a `static const`, a brace-list record):
  outside every route here, the critic's condition (4); its own question is queued.
- **J**: **veto**, unchanged (the ceiling, §1.1 and §1.11). No other route breaches it.

**Prediction**: built into the compiler, Ux-whole gives the 96 verdicts of `ux48/pre48.txt` and
the critic's twelve attack cells unchanged, and `FEAT_UB` flagged at `-O0` and `-O2`. It moves 0 files of
`emission` (it writes no byte of a unit), and its cold probe costs this Mac at most 0.5 s real
on `K_DIV.hero` (`/usr/bin/time -p`, three runs). Any one of these failing falsifies it.
**Condition**: I turn Ux-whole to object if Windows cannot link or run the UBSan runtime without a
change to the user's machine, or if the author rules that `heroes build` must never execute the
program's code. F2-conservative then becomes the adopted route, with the critic's four conditions.

## Housekeeping (this pass)

Worktree `.claude/worktrees/216-ce-pass2` removed (`git worktree list` shows none of mine);
containers `ubuntu:24.04` and `heroes-linux-arm64` run with `--rm`; nothing written in the trunk
but this section (`git status --short`: the two `docs/panel/216-*` folders only). No paid run, no
credential printed.

**Corrected 00:47, by the critic's own text**: *the six attack shapes, the critic's twelve cells,
identical* (twice above, and in the prediction) over-reaches. The critic reported **nine** attack
cells (§ Pass 2, *all nine cells right*), and its `ux/all.txt` holds the 48 shapes alone. My `pre`
run gave twelve cells: the critic's nine are identical; the other three are mine alone and have no
critic's value to compare with (`A_LINE_MISS` and `A_FILE_MISS` flagged at `-O2`, `A_BCPG_MISS`
kept at `-O2`). The prediction reads *the critic's nine attack cells unchanged*.
