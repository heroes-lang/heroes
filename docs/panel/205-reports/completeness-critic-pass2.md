# Panel 205, completeness critic, second pass

Copied by the coordinator at 03:36 on 2026-10-10 (`date`) from the seat's final reply (its transcript), verbatim below the rule; the seat's running notes are in its folder under `.claude/worktrees/scratch-b15/` (ignored by git).

---

# Panel 205, completeness critic, second pass

Started 03:24:29 and finished about 03:37 (`date`). All work is in `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/scratch-b15/critic-205b/`. I used the compiler-engineer's frozen and route-H binaries (copied in as `heroes0` and `heroesH`), the ffi-pragmatist's `rtP2`, and runtime copies I made myself (`rtS1`, `rtS2`, `rtS3`, `rtY`). Docker `heroes-linux-arm64:latest` ran one container at a time (Debian clang 22.1.8); this Mac runs Apple clang 21. I ran no git inside any copy. One read-only `git log` ran in the main checkout to read the newest commits, and nothing was written there. No paid run.

## Findings, each from a command I ran

**1. Q2: the smallest relaxation, measured.** A "region" below means the groups' headers between the guard's open and its close. The runtime arms change only the guard's open and close headers, picked through `HEROES_RUNTIME`. Every non-H arm keeps R2 (`error "-Wmacro-redefined"`) in the region and raises the P2 list again after the close.

- **S1**, the spec-warden's split, ignores only `-Wsign-conversion` in the region.
- **S3** ignores `-Wall` and `-Wsign-conversion` and keeps `return-type`, `uninitialized` and `conditional-uninitialized` as errors.

Tables are in `t-*.tsv`, `u-*.tsv` and `census/`.

| case | today | H | P2 | S1 | S3 |
|---|---|---|---|---|---|
| gmp, avutil, avcodec, avformat, swscale, libfdt | all 6 refused, `sign-conversion` (gmp.h:1882, common.h:212, libfdt.h:138) | 6 build | 6 build | 6 build | 6 build |
| `own.hero` (missing return, truncation, uninitialised read) | refused `-Wreturn-type` | **builds, prints 1** | refused | refused | refused |
| `own2`: uninitialised read only | refused | **builds, prints 1** | refused | refused | refused |
| `own4`: missing return only | refused | **builds, prints 1** | refused | refused | refused |
| `own3`: 64-to-32 truncation only | refused | builds, 705032704 | **builds, 705032704** | refused | refused |
| the 12 shapes of 570 that build today | 12 build (`rp` aborts 134 at run time) | 12 refused | 11 refused, **`mac` builds** | the same as P2 | the same as P2 |
| `ej` and `je` (Expect with libjpeg) | exit 0, raw warning | refused, macro-redefined | refused | refused | refused |
| the 4 correct programs, `gok`, `gwidth` | build | build | build | build | build |
| `wh`: raw `-Wall` text from a header | printed | silent | silent | **printed** | silent |
| 182-header Mac census (the ffi-pragmatist's lists) | 16 FAIL, 2 raw | — | 10 FAIL, 0 raw | 10 FAIL, 2 raw (jq.h, jv.h) | **10 FAIL, 0 raw, identical to P2 header by header** |
| 85 Linux headers copied as `-I` | — | — | 83 OK | — | 83 OK, the same 2 FAIL |

S3 gave the same verdict as P2 on all 40 cases of my first matrix. It differs from P2 only on `own3`.

- **Answer:** S1 is the smallest relaxation that meets the brief's criteria. S3 is the smallest that also keeps raw clang text off a correct program.
- H accepts undefined behaviour in a header of the program's own (`own2`, `own4`), as the spec-warden said.
- P2's extra ignores (`shorten-64-to-32` and the two pointer-type warnings) bought nothing on 6 + 182 + 85 headers. They only let `own3` through. That is an implementation-defined truncation, not undefined behaviour (C11 6.3.1.3p3), so it is a policy choice and not a soundness one.

**1b. A contradiction with the ffi-pragmatist's claim.** Its claim that P2 plus C keeps every wrong binding refused is false for the `mac` shape. That shape is a constant whose macro carries `_Pragma(... ignored "-Wsign-conversion")`, so the pragma expands after the close. It builds under P2, S1, S2 and S3 (prints 10, with a raw `-Wabsolute-value`), and only H refuses it. Route C must therefore also raise again in the probe region, which is H's `extern_probe.hero` half, and not only after the close.

The adoptable route is a combination nobody built:
- H's two raise points;
- the region's ignore list narrowed to S1 or S3, plus `KEPT`.

That it would refuse `mac` is an inference from H's result, not a measurement.

**2. Q1: route B, and which unit is real.**
- Both seats' units reproduce.
  - The engineer's `b.c` has 2 errors: `INT64_C` and `HUGE_VAL` undeclared.
  - The ffi-pragmatist's `td.c` with `mac_sw.h` hides `strlcpy` as C means, and with `mac_plain.h` builds at rc 0.
- **Neither is the emitted unit.** Both are 8- and 12-line hand-written files.
  - `td.c` includes neither the guard nor `heroes_runtime.h` (grep finds 0 of either), so it cannot see defect 361 at all.
  - `b.c` keeps the guard, so it models the real unit's guard.
  - The real unit, lines 2-9 of `q1/sw/sw.c` from the frozen compiler's `--emit-c`, is: `heroes_runtime.h`, `<math.h>`, `hero_os.h`, `heroes_runtime.h`, the guard open, the groups, the guard close.
- **A variant keeps 361's guard and still lets the first group's header set the switch**, at C level on the real units. I call it Y (`rtY`, `q1/y`, `lin/`):
  - `heroes_runtime.h` takes its integer types from clang's builtins (`typedef __INT64_TYPE__ int64_t;` and the rest) and includes no `<stdint.h>`;
  - its 3 `UINT64_C` and 1 `INT64_MAX` become `__UINT64_C` and `__INT64_MAX__`;
  - SEEDS' `<math.h>` moves out of the prefix;
  - after the close come `#include <stdint.h>`, `#include <math.h>`, then `#ifndef INT64_C` / `#define INT64_C(c) __INT64_C(c)`, and the same for `UINT64_C`.
- Measured under Y:
  - **Mac:** with the switch first, `strlcpy` is refused as C means (the unit's one error); without the switch, rc 0. With a hostile group `#define INT64_C(c) 0` placed after it, `_Static_assert(INT64_C(42) == 42)` holds; with the hostile group first, the switch is latched, as the first-group rule predicts.
  - **Linux arm64:** the `first` and `own` units compile at rc 0 with `sched_getcpu` declared, and the assertion holds with a hostile group added. `plain` is refused (`sched_getcpu` undeclared), which is C's meaning.
  - `runtime.c` under `rtY` compiles on the Mac with 0 warnings.
- Y's cost is unrun past this point. Every guarded standard name the emitted C uses needs a fallback after the close. The seed shows `INT64_C` 21,961 times, `UINT64_C` 4,504 and `INT64_MIN` 133, plus single hits for `isnan`, `fpclassify`, `FP_*` and `HUGE_VAL*`. Some of those single hits may be the guard's own name list. `FP_*` values differ between glibc and Darwin, so they cannot come from builtins: a question.

**3. Route A does not serve what readers write.**
- None of the 8 blind readers wrote a `package` clause or a `.pc` file. m1-b's report rejects `package` in so many words.
- Today all 8 readers' programs are refused `ffi_unknown_name`, the message false in 5 of them, where the header does declare `sched_getcpu` once its `_GNU_SOURCE` is honoured.
- Under Y, the 5 programs with a header of their own (m1-c and m2-a to m2-d) compile unchanged at unit level. The other 3 bind `sched.h` plainly and are refused, which is C's meaning.
- I reproduced route A's program-wide reach on Linux:
  - module `w` binds `wcwidth(c: i32)` from `wchar.h`;
  - without `main` naming `ncursesw` it is refused with *`wchar.h` declares no `wcwidth`*, which is false;
  - with `main` naming it, the declaration is found and `w`'s `i32` is refused `ffi_parameter_type` against `unsigned int`;
  - so another module's package decides this module's verdict.

**4. The residual and the deprecation warning.** Neither is filed: grep over `issues/`, where `_Pragma` appears only in defect 361's file and `deprecated` only in defect 192's.
- `callee` builds and prints 5 under the frozen compiler, H and S3. Its cause is 570's: a header's pragma reaches the compiler's lines. Defect 570's text names only the plain `ignored` shape, not the other 11 rows, nor `mac`, nor `callee`.
- `uc`'s `-Wdeprecated-declarations` fires at the compiler's own probe line, `_Static_assert(HERO_RET_I32(getcontext((void *)0)), ...)` at line 44 of H's unit, under every arm. That is a different cause, a header attribute on a name the compiler's own line uses. By the rules' classes it is "a clang warning on a correct program", so `blocking`.

## Routes nobody built
- Y as a compiler change, with the guard pushing only names defined when it opens and the emitter using builtin-backed fallbacks.
- H's two raise points combined with the S1 or S3 region list.
- For `callee`: ask clang whether a bound function's name is an object-like macro whose expansion holds `_Pragma`, and refuse it or probe its expansion, instead of H's `tree2` chain, which degrades the messages. Unrun, a question.

## Questions the sitting has not asked
- Is a 64-to-32 truncation in a header's own code a refusal (S1, S3) or not (P2)? Is a sign conversion inside the program's own `static inline` judged at all? Under P2 and S3 it is not, as the ffi-pragmatist's `inl.h` showed.
- Route A puts `PKG_CONFIG_PATH`, an environment variable, in the path of a verdict. Is that the input class `.claude/rules/cli-surface.md` refuses? That file is unread by me.
- `rp` (`read_p(p: i64)` against `int *`) builds today and aborts 134 at run time. Should 570's item name its worst row?
- Should 570 be widened to its 14 same-cause rows (the 12 here, plus `mac` and `callee`), and the `uc` row filed as its own defect?

## What I did not run
- Windows, Linux x86-64, the CI's clang 18 (including whether it defines `__INT64_C`).
- Y linked, run, over the full net, or with every guarded name.
- The combination of H's raise points with the S1 or S3 list built as a compiler.
- S1 and S3 at the Heroes level on Linux (C-level census only), and Expect with libjpeg on Linux.
- `heroes measure`, and any fix for `callee`.
