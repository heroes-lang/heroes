# Panel 208, compiler-engineer's report

Seat: compiler-engineer (design.md §1.1, §1.7, Part 5; veto on soundness).
Tree: `.claude/worktrees/scratch-b15/208-compiler-engineer/tree/`, `git archive
391628b6`. Started 15:09 by `date`. Written as I go; a section marked
*in progress* is not finished.

## 0. State

- 15:09 seed build started (`clang -I runtime seed/heroes.c runtime/runtime.c`),
  load average 52 at 15:12 (`uptime`): nine seed compiles on the machine at
  once, so no duration this report writes is a duration; instructions retired
  (`/usr/bin/time -l`) are the cost unit, being what a busy machine does not
  move much.

## 1. The asymmetry: a fortified macro, not clang's system-header rule

Measured with clang alone (Apple clang 21, this Mac's SDK), C files in
`208-compiler-engineer/ctest/`, `clang -std=gnu11 -fsyntax-only`:

| C written | warns? |
|---|---|
| `#include <stdio.h>` then `sprintf(p, "%d", 1)` | **no** |
| the same with `-U_FORTIFY_SOURCE -D_FORTIFY_SOURCE=0` | yes, `'sprintf' is deprecated` |
| the same call written `(sprintf)(p, "%d", 1)` | yes |
| `mktemp`, `tmpnam`, `vfork`, `daemon`, `syscall` from `<stdlib.h>`, `<stdio.h>`, `<unistd.h>`, called plainly | yes, all five (5 warnings) |
| `dep.h`'s `twice(2)`, `dep.h` reached by `-isystem sys` | yes |
| the same, `dep.h` reached by `-I sys` | yes |

**Cause**: `_types.h:64-69` defines `_FORTIFY_SOURCE 2` *on by default*
whenever the deployment target is 10.5 or later, at every optimisation level,
and `secure/_stdio.h` then makes `sprintf` a function-like macro:
`clang -E` of the plain call reads `__builtin___sprintf_chk (p, 0,
__builtin_object_size (p, ...), "%d", 1)`. The builtin carries no
`deprecated` attribute, so the call names nothing deprecated. A function-like
macro is not expanded where its name is not followed by `(`, which is why the
trunk's compiler at `9743597b` warned at the binding's probe line (the probe
names `sprintf` without a call) and why the brief's `call.hero` is silent at
`391628b6`: there the only line naming `sprintf` bare is the compiler's own
(the probe, `extern_refs`' held address), quiet since defect 571.

**Not the system-header rule**: clang locates `-Wdeprecated-declarations` at
the USE, and a use in the unit is never in a system header, so where the
deprecated declaration lives does not matter: `dep.h` warns as a system
header and as a user one, and five SDK functions warn from system headers.
A package's header is a system header when found in a system directory
(`/usr/include`, `-isystem`, `C_INCLUDE_PATH`) and a user one through
`-I` (pkg-config's `--cflags`), panel 205's R2 record; for THIS warning it
makes no difference, measured above.

So the measured asymmetry is a property of one SDK function family, not of
the guard or the region: `sprintf`, `vsprintf` (the `secure/_stdio.h`
macros) are silent, every non-macro deprecated function warns. *(The emitted
unit's confirmation is §1b.)*

## 1b. The emitted unit confirms it

Baseline compiler built in `tree/` at 15:24 (`heroes-seed build
selfhost/main.hero`: 474,175,025,984 instructions retired, `real 656.38`
against `user 185.80` + `sys 54.04`, so the duration is discarded: the
machine was waiting). `heroes build s10_sprintf.hero --emit-c` (the brief's
`call.hero`), `out0/s10.c`:

- line 28, after the guard's close: `#pragma clang diagnostic ignored
  "-Wdeprecated-declarations"` (571's QUIET);
- line 118-119, `#line 5 "s10_sprintf.hero"` then the probe
  `(void)(sprintf)(a0, a1, a2);`: the parenthesised name reaches the real
  declaration, which is the warning the trunk's compiler printed at the
  binding line before 571, and it is now inside the quiet region;
- line 146, `#pragma clang diagnostic warning "-Wdeprecated-declarations"`
  (SPOKEN), then line 231, the program's call, `t16 = sprintf(t12,
  hero_cstr_nonnull(t14), t15);`, which the SDK's macro rewrites to
  `__builtin___sprintf_chk`: nothing deprecated is named, so nothing is said;
- line 255, QUIET again.

So **the call was never able to warn on this SDK**; what 571 removed was the
probe's warning, the one line of the unit that named the declaration.

## 1c. Which clang group each shape is, measured with clang alone

`ctest/h.c`, `k.c`, `m.c`, `u.c`: `dep.h` and `more.h` (the shapes beside,
written for this sitting: `unavailable`, `availability(macos,
deprecated=10.5)`, `#pragma clang deprecated(OLD_MAC, ...)`, C23
`[[deprecated]]`), each name used once in a region under
`#pragma clang diagnostic ignored "-Wdeprecated-declarations"` (571's QUIET)
and once under `warning` (571's SPOKEN), `clang -std=gnu11 -c`:

| shape | group | quiet region | spoken region | level-dependent |
|---|---|---|---|---|
| function called (`twice`, `version`) | `-Wdeprecated-declarations` | silent | warns | no |
| record used (`struct old_pair`) | same | silent | warns | no |
| enumerator read (`OLD_LIMIT`) | same | silent | warns | no |
| `availability(macos, deprecated=10.5)` | same | silent | warns, *first deprecated in macOS 10.5 - use avail2* | no |
| C23 `[[deprecated("...")]]` under `-std=gnu11` | same | silent | warns | no |
| macro under `#pragma clang deprecated` | **`-Wdeprecated-pragma`** | **warns** | warns | no |
| `__attribute__((warning("careful")))` | **`-Wattribute-warning`**, a backend diagnostic | **warns** | warns | **yes: warns at `-O0`, silent at `-O2`** (`m.c`, the call inlined away) |
| `unavailable("gone for good")` | none: a hard error | error | error | no |

Two findings the brief's list did not hold:

- **571's QUIET does not cover a deprecated macro**: `-Wdeprecated-pragma`
  is its own group, so a binding of such a macro as a `constant` warns at
  the compiler's own accessor line today. Whatever the route, the region
  must name both groups.
- **`warning(...)` cannot be a verdict**: its diagnostic comes from code
  generation and vanishes when the call is inlined, so refusing on it would
  give one program two verdicts by optimisation level, the shape defect 562
  closed (`emit/extern_refs.hero`'s doc). It is not a deprecation either.
  It belongs with (S) under every route.

## 2. The shapes, with the frozen tree's compiler (571 landed)

Cases in `208-compiler-engineer/shapes/`, `heroes build <case>.hero`, six at a
time (`run_shapes.sh`), outputs in `out0/`:

| case | shape | exit | what is printed |
|---|---|---|---|
| s1_call | `dep.h` `twice` called | 0 | raw clang warning at `s1_call.hero:5:10` |
| s2_bound | `twice` bound, not called (571's) | 0 | nothing |
| s3_record | `struct old_pair` in a signature and a construction | 0 | raw warning at `s3_record.hero:5:33` **and at `s3record.c:67:12`**, the compiler's own prologue `struct old_pair t2;` in `main`: 571's `names_a_group_type` missed this temporary |
| s4_enum | deprecated enumerator read as `constant` | 0 | nothing (the accessor's QUIET covers the only line naming it) |
| s5_warning | `warning("careful")` called | 0 | raw `-Wattribute-warning` at `s5_warning.hero:5:10` |
| s6_unavailable | `unavailable` called | **2** | **`internal error: compiling the generated C failed`**, clang's `'gone' is unavailable: gone for good` at `build/tu-*/s6unavailable.c:49:29`: the compiler blames itself for the header's refusal |
| s7_availability | `availability(macos, deprecated=10.5)` called | 0 | raw warning at the `.hero` line |
| s8_macro | `#pragma clang deprecated` macro bound as `constant` | 0 | **two raw `-Wdeprecated-pragma` warnings at `build/tu-*/s8macro.c:49` and `:50`**, compiler lines |
| s9_c23 | C23 `[[deprecated]]` called | 0 | raw warning at the `.hero` line |
| s10_sprintf | `<stdio.h>` `sprintf` called (the brief's `call.hero`) | 0 | nothing (the fortify macro, §1) |
| s11_mktemp | `<stdlib.h>` `mktemp` bound, not called | 0 | nothing |
| s12_mktemp_call | `mktemp` called | 0 | raw warning at `s12_mktemp_call.hero:8:19` |
| s13_openssl | OpenSSL 4.0.3 `RSA_new`, `RSA_free` called (`package "openssl"`) | 0 | raw warnings at `.hero:6:18` and `:7:11` |
| s14_openssl_bound | `RSA_new` bound, not called | 0 | nothing |

Three defects beside the sitting's question, each a gap of a landed repair
or a misblame, whatever route is chosen: **s6** (exit 2 on a header's
`unavailable`, `blocking` by the class list: *an exit 2 where the author can
be told*), **s8** (a raw warning at the compiler's lines, 571's QUIET names
one group of two), **s3's second line** (a raw warning at a compiler line,
571's prologue walk misses `main`'s temporary). Unfiled; the coordinator's.

## 3. The two routes that need no new form, built

Both built in clones of `tree/` (`treeS/`, `treeR/`), each compiler by
`heroes-seed build selfhost/main.hero` at exit 0 (`wrote heroes`). Diffs by
`git diff --no-index --numstat tree/selfhost tree<X>/selfhost`; the full
diffs are `208-compiler-engineer/routeS.diff` and `routeR.diff`.

### (S) silence: 5 files, +5 / -114 lines

The guard's close already writes QUIET (`emit/macro_guard.hero:102`); the
route deletes every line that speaks again: the SPOKEN before the program's
definitions and the QUIET after them in `emit/unit.hero` (-11) and the fused
unit's `emit.hero` (-10), the accessor's pair in
`emit/constant_accessor.hero` (-7), the prologue's pair in `emit/body.hero`
(-5), and in `emit/deprecation.hero` everything but `QUIET` (-86 +5:
`SPOKEN`, `say`, `group_constant`, `names_a_group_type`, `before_prologue`,
`after_prologue`, `binds_a_group`). Net -109 lines. **What else it
silences: nothing.** The one pragma names `-Wdeprecated-declarations`, and
`emit/header_region.hero` (panel 205's R2, the `-Wall` region) is not in the
diff, so it cannot have widened.

### (R) refuse: 3 files, +95 / -2 lines

- `emit/deprecation.hero`: `SPOKEN` becomes `#pragma clang diagnostic error
  "-Wdeprecated-declarations"` (2 lines, its test with it), so the program's
  definitions are compiled with the deprecation an error and the compiler's
  lines stay quiet as 571 left them;
- `emit/ffi_deprecated.hero`, new, 88 lines: reads clang's `error: 'X' is
  deprecated[: <words>]` at a location `ffi_site.location` accepts (a line of
  this program), narrowed by `ffi_lookup.declaration` (a name a group of
  THIS program declares, the class's own narrowing), and tells
  `ffi_deprecated` at the call when an IR call of `X` stands on that line,
  else at the declaration; the header's words, clang's `[-Werror,...]` cut;
- `emit/ffi.hero`: the reader asked in the recovery loop (+5).

**Where the verdict is read**: clang's diagnostic recovered at the `.hero`
line, as the class's nine members are (`.claude/rules/c-boundary.md` § A
clang failure that the author's own extern caused); no probe asks the header.
Rendered, `outR/s1_call.log`:

```
error[ffi_deprecated]: `dep.h` marks `twice` deprecated: use twice2
  at s1_call.hero:5:11
  5 |     print(twice(x: 21))
```

Both compilers build: R's compiler built the compiler again
(`treeR/heroes build selfhost/main.hero -o heroes-gen2`, `wrote
heroes-gen2`, 428,637,115,313 instructions), so the compiler itself names
no deprecated declaration under it. The 21 `examples/` files with an
`extern` (the brief's `grep -rl`), built by their roots with the frozen
compiler and with R's: 0 moved (`census_base.txt`, `census_R.txt`; the one
exit 1 in both is `examples/ledger/db/sqlite.hero`, a module built alone).
**The compiler's own tests**, `heroes test selfhost/main.hero` in each
clone: S `1560 tests, all passed`, exit 0; R `1560 tests, all passed`, exit
0 (`testS.log`, `testR.log`). *Not run*: the full net, the `check`, `run`
and `emit` golden forms, the census of `check`.

### Cases and verdicts per shape

| case | frozen tree (571) | (S) | (R) | R exact? |
|---|---|---|---|---|
| s1 `twice` called | 0, raw warning | 0, silent | **1**, `ffi_deprecated` at the call | yes |
| s2 `twice` bound only | 0, silent | 0, silent | 0, silent | yes (R speaks of uses) |
| s3 deprecated record in a signature | 0, two raw warnings, one at a compiler line | 0, silent | **1**, at the `record` declaration (no call to point at) | yes, but the span is the declaration, not the signature |
| s4 enumerator read as `constant` | 0, silent | 0, silent | 0, **silent** | **no**: the only line naming it is the compiler's accessor, quiet since 571 |
| s5 `warning("careful")` | 0, raw warning | 0, **raw warning** | 0, **raw warning** | not a deprecation (§1c) |
| s6 `unavailable` | **2, internal error** | **2** | **2** | no route's business; its own defect |
| s7 `availability(..., deprecated=10.5)` | 0, raw warning | 0, silent | **1**, *first deprecated in macOS 10.5 - use avail2* | yes |
| s8 `#pragma clang deprecated` macro | 0, raw warnings at compiler lines | 0, **raw** | 0, **raw** | **no**: `-Wdeprecated-pragma` is neither route's group |
| s9 C23 `[[deprecated]]` | 0, raw warning | 0, silent | **1** | yes |
| s10 `sprintf` called (fortify macro) | 0, silent | 0, silent | 0, **silent** | **no**: the macro hides the name (§1) |
| s11 `mktemp` bound only | 0, silent | 0, silent | 0, silent | yes |
| s12 `mktemp` called | 0, raw warning | 0, silent | **1**, the header's whole sentence | yes |
| s13 OpenSSL `RSA_new`, `RSA_free` called | 0, two raw warnings | 0, silent | **1**, two diagnostics, one per call | yes |
| s14 OpenSSL `RSA_new` bound only | 0, silent | 0, silent | 0, silent | yes |

**(S) is exact on its own definition over every `-Wdeprecated-declarations`
shape** (s1, s3, s7, s9, s12, s13 silent; nothing it should not touch
moved) and leaves s5, s6 and s8 as they were. **(R) is exact on 7 of 9 use
shapes** and wrong in the silent direction on two: s4 (a constant read) and
s10 (a name the SDK shadows with a macro, `sprintf` and `vsprintf` here,
any fortified function on a platform that fortifies). Neither is a false
refusal; both are a deprecated use accepted while its neighbours are
refused, so R's verdict depends on how the header IMPLEMENTS a name, a fact
the author cannot see from the binding.

What exact R costs, priced and unbuilt (an inference from the files read):
s4 needs the accessor's `return <NAME>;` written at the declaration's line
inside the error region instead of at the generated line under QUIET
(`emit/constant_accessor.hero`, 49 lines, and `record_constant`'s accessor),
which refuses a bound constant where it is declared, read or not, a
different rule from *uses*; s10 needs the bare name asked at each call's
line (`(void)sizeof(&(X));` beside the call: clang warns on the bare name
in an unevaluated operand, `sprintf` included, measured in `ctest/n.c`),
about 20 lines in the body's call emission, or the probe's region
made an error, which refuses a bound name never called. Each is a ruling,
not only a cost: whether R speaks of *uses* or of *bindings*.

## 4. Cost of each built route

`/usr/bin/time -l`, instructions retired as it reports them; a cold build
is a fresh directory holding the case and `dep.h` (`cost.sh`, `cost2-*`).
The load average fell from 52 to 8 by 15:31 (`uptime`), and `check`'s
`real` sits within 0.3 s of `user`+`sys` on every run below, so its
durations are readable; the routes' differences are inside the spread of
two runs of one compiler, which is the finding.

| measurement | frozen tree | (S) | (R) |
|---|---|---|---|
| `heroes check selfhost/main.hero` | 78,858,701,048 and 78,521,092,373 (two runs), `real` 8.28 | 78,450,491,394, `real` 8.47 | 78,678,221,380, `real` 8.62 |
| cold `heroes build` of `s10_sprintf.hero` (accepted by all three) | 740,044,090 and 741,017,199 | 740,343,991 | 746,047,377 |
| cold `heroes build` of `s2_bound.hero` (accepted by all three) | 603,077,462 and 594,788,534 | 595,360,432 | 595,961,186 |
| cold `heroes build` of the reproducer `dep_call.hero` | 599,646,321 and 601,884,301, exit 0 | 632,417,433, exit 0 | 712,462,119, **exit 1** (the recovery's work on a refused program, not a like-for-like row) |

`check` cannot move by construction (both routes touch only the emitter,
which `check` does not run), and the measurement agrees: the three are within
0.5% and the frozen tree's own two runs differ by 0.4%. On an accepted
program R costs +0.7% on `s10` and +0.2% on `s2` against the nearer base
run, inside the base's own 1.4% spread on `s2`. **Neither route slows the
compiler by anything these instruments can separate from noise.**

## 5. Pricing (M) and (F), unbuilt

**(M) refuse with a way out, a word on the `extern` declaration.** Every
number here is a file's measured length (`wc -l`); the share each would take
is an inference from reading them, unbuilt:

- the word: `selfhost/keywords.hero` (425 lines) gains a contextual word, so
  `grammar` and `spec`'s keyword tables move (`.claude/rules/verification.md`'s
  row for that file);
- the parse: a member's trailing word, beside the marks
  (`selfhost/parse/marks.hero`, 342; `parse/member_lines.hero`, 488), about
  15 to 25 lines; the AST field on three declaration kinds
  (`selfhost/ast.hero`, 689), a few lines plus every constructor site;
- the re-printers: the formatter (`selfhost/print/marks.hero`, 100, or
  `print/signature.hero`, 153) and the walk CLAUDE.md § 9 says a new surface
  form owes every tool that re-prints a program, `probe` and `surface`
  fixtures with it;
- the checker's half, which is the expensive one: a word that says *I know
  this is deprecated* on a binding whose header does NOT mark it is a false
  statement the compiler would keep for ever, so it must be refused, and only
  clang knows the answer. That is a probe compiled with the deprecation an
  error whose SUCCESS is the refusal, a second clang question per marked
  binding, beside `ffi_asked`'s (`emit/ffi_call.hero` 361, `check/ffi.hero`
  412 are its neighbours): about 40 to 60 lines and a unit per build;
- the emitter's half: clang's pragma state is positional, not per name, so a
  marked name's uses need their own quiet lines around each statement that
  names it inside R's error region, in the body's call emission
  (`emit/inst.hero`, 421): about 15 to 25 lines;
- (R) whole underneath it, and a spec sentence and a grammar production,
  priced in tokens by `heroes measure`: unmeasured here.

So (M) is **(R) plus about 100 to 150 lines over six to eight modules in four
passes (lexer table, parser, formatter, checker probe, emitter) and the
spec**, by inference. By §1.7 it is not a core construct (it lowers to
nothing in the IR), but it is surface: every pass that reads or re-prints a
declaration carries it, which is the cost §1.7 calls the size of the
compiler.

**(F) `--allow-deprecated`.** About 10 lines in `selfhost/cli/flags.hero`
(587) and the argv table `selfhost/cli/table.hero` (266), and the word
threaded to the unit's emission (`emit/unit.hero`, `emit.hero`), and the
cache key moves with it (the `selfhost/cli/flags.hero` row: every unit's key,
`cache` alone). **The stopping rule refuses it** (`.claude/rules/cli-surface.md`,
panel 016): a capability enters only if the fixpoint, the golden harness or
the Part 11 harness must type it, or it has a measured Part 11 effect. The
fixpoint does not (R's compiler built the compiler at exit 0, §3), no golden
needs a deprecated name accepted that S or R could not give it, and no Part 11
effect is measured. The one flag that changes a verdict, `--permissive`
(`selfhost/cli/table.hero:94`), entered because the Part 11 harness types it
as its control arm, which is exactly what (F) lacks. And it would make one
program's verdict a fact of the command line, the shape design.md's *exit 1
or nothing* refuses one level up.

## 6. A route nobody listed

What would have to be true for one to exist: a verdict read somewhere other
than at a use. One is measured above: **(B) refuse at the binding**, the
probe's region made an error for the deprecation, as the trunk's probe once
warned at the binding line (§1b). It is exact on `sprintf` (the probe's
parenthesised name bypasses the macro) where (R) is not, and it refuses s2,
s11 and s14 (bound, never called), which (R) accepts; it misses what has no
probe today, a function with no parameter (`version()`, `RSA_new()`) and a
constant, unless probes are added. Unbuilt; its rule (*a binding names*,
not *a use names*) is the ruling (R) also owes before it can be exact.

## 7. Verdicts

- **(S) silence: approve.** -109 lines in 5 emitter files, no construct,
  nothing else silenced, exact over every `-Wdeprecated-declarations`
  shape, 1560 tests passed, no measurable cost. It is the only route whose
  verdict does not depend on how a header implements a name. Owed beside
  it, measured gaps of 571 that it does not close: `-Wdeprecated-pragma`
  (s8) and `-Wattribute-warning` (s5) quiet from the same close (built
  as S2, §9), and s6's exit 2, which only a reader closes (R2's half, §9).
- **(R) refuse: object, no veto.** Within the ceiling (+95 lines, one new
  emitter module, no checker or grammar change, no core construct by
  design.md §1.7 and Part 5), and not unsound by §1.12: nothing it accepts or
  refuses corrupts memory. The objection is exactness: as built it refuses
  `twice` and accepts `sprintf`, the very program this sitting was called
  for, and accepts a deprecated enumerator, because the verdict reads a
  name's spelling after the header's macros; making it exact is a ruling
  (uses or bindings) plus 20 to 40 lines. It also refuses programs C builds
  and runs correctly (864 OpenSSL 3 deprecation lines, `grep -c`), which the
  class list calls *a correct program refused* unless the sitting rules a
  deprecated use incorrect, a ruling design.md does not make (no sentence
  names `deprecated`; I searched design.md and the spec, not other trees).
- **(M) a word on the binding: object.** (R) plus 100 to 150 lines over six
  to eight modules and four passes, a surface form with the formatter's and
  the probe's walk, and a checker probe whose success is the refusal;
  priced by inference, unbuilt. Not core by §1.7, but the most compiler for
  the least measured need.
- **(F) `--allow-deprecated`: object.** Refused by the stopping rule: no
  harness types it and the fixpoint does not need it (R's compiler built the
  compiler). A verdict that is a fact of the command line.
- **(N) a note at exit 0: object.** A warning level by another name,
  against design.md Part 8's *exit 1 or nothing* (`docs/design.md:3800` at
  `391628b6`; the brief cites `:3744` on another tree).

**Recommendation: (S), with the two beside groups and s6 repaired.** What
would make it wrong: a measured Part 11 effect showing that models call
deprecated C names and that a refusal at the use changes what they write
(the thesis, *every plausible mistake a compile error*, is the argument for
(R), and this sitting runs no blind seat, so it is unmeasured here); then
(R) is right, and only in its exact form, ruled on uses or bindings first.

## 8. Prediction

At the landing of whichever route, on this Mac's SDK: **`heroes build` of
`208-compiler-engineer/shapes/s10_sprintf.hero` exits 0 and prints nothing
under (S), and exits 0 and prints nothing under (R) as built here**; an (R)
that lands claiming *a call naming a deprecated name is exit 1* and that
build exits 1 has added the exactness this report prices (the bare name
asked at the call, or the binding's probe). Scored by that one command at
the landing commit.

## 9. Completed forms, built 15:57 to 15:58

`stage/apply_plus.py` over each clone: `emit/deprecation.hero` gains
`BESIDE`, the quiet lines for `-Wdeprecated-pragma` and
`-Wattribute-warning`, written by `emit/macro_guard.hero` after QUIET at the
guard's close and never spoken again; in R2 the reader also reads clang's
`' is unavailable` and says *marks `X` unavailable*. Each compiler by
`heroes-seed build selfhost/main.hero`, `wrote heroes`. Diffs against
`tree/selfhost` (`git diff --no-index --numstat`): **S2 6 files, +22 / -119;
R2 4 files, +124 / -6.** `heroes test` not run on S2 and R2 (S and R passed
1560 each; the two new lines' test is `macro_guard`'s, amended).

| case | S2 | R2 |
|---|---|---|
| s1, s3, s7, s9, s12, s13 (deprecated uses) | 0, silent | 1, `ffi_deprecated` each, as R |
| s2, s11, s14 (bound only) | 0, silent | 0, silent |
| s4 enumerator, s10 `sprintf` | 0, silent | 0, **silent** (R's two misses stand) |
| s5 `warning("careful")` | 0, **silent** | 0, **silent** |
| s6 `unavailable` | **2, internal error** (no reader) | **1**, `` `more.h` marks `gone` unavailable: gone for good ``, **told twice** for one call (a second message for one mistake, unexamined) |
| s8 deprecated macro | 0, **silent** | 0, **silent** |

So the complete (S) is S2 plus R2's eleven-line `unavailable` reader alone
(a header's hard refusal is exit 1 under every route, and it is exit 2 at
the frozen tree), and the complete (R) is R2 plus the exactness ruling of §3.
Finished 15:59 by `date`.
