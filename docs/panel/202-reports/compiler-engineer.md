# Panel 202, compiler-engineer

Copied by the coordinator at 23:59 on 2026-10-09 (`date`) from the seat's final reply (its transcript), verbatim below the rule; the seat's running notes are in its folder under `.claude/worktrees/scratch-b15/` (ignored by git).

---

# Panel 202, compiler-engineer seat

I worked from 23:06 to 23:31 and from 23:46 to 23:58 (`date`). The session limit stopped me between the two. The compiler was built in three stages in my folder: the seed against `b17-rt29/runtime`, then `heroes-s2`, then `heroes`, which printed `heroes 0.2.0` at 23:18. Each prototype is the copy's `selfhost/` with a small edit, built by `heroes-s2`. Notes are in `notes.txt`.

- `verdict`: **object.** I object to (a), (b), (d), (f) and (h). I withdraw my own route (i). I **approve (c) together with (g)**, on the conditions C0 to C4 below. I veto nothing: no route adds a language construct. Part 5 does not say whether a backend unit kind counts as "core", and I am not inventing a rule for it.
- `section`: design.md §1.1 (`:174`), §1.7 (`:410`), §1.12 (`:575`), §4.19 (`:2334`). Also §4.1 `:836-839`, which still says *"The whole program is still emitted as one .c"*. That sentence has been stale since panel 093, and under (c) it must be amended.
- `needed_for_self_hosting`: **no.** `selfhost/` builds module by module, and its headers can share a unit.

## Two shapes nobody listed (both `blocking`, both built and run)

- **`fprec`** is fp plus one group record in `b.h`'s group. Today `build` exits 1 with 538's message, while fp itself builds at 0.
  - Cause: the layout probe compiles every header of the program in one unit.
  - So whether `build` refuses two conflicting headers depends on whether any group anywhere declares a record.
- **`fpat64`** is fp plus `function fill(@x: i64)` over `void fill(int *x)`.
  - Today `build` exits 0, and the program prints `6 4294967299`.
  - The same program without the conflict (`fpat64ctl`) is refused with `ffi_parameter_type` at exit 1.
  - Cause: `selfhost/cli/pointee.hero:200-204` returns `ok()` when the dump unit (every header together) does not compile. Its comment assumes "the build's own compile reports it on the author's line", and that has been false since the build went per module.

## Exit codes per case

The cell order is check / build / test / `--emit-c` / run. `run` always gave the same exit code as `build`. `check` was 0 everywhere.

| case | today | (b) one unit | (i) probe, withdrawn | (c) prototype |
|---|---|---|---|---|
| fp, fp2, nobind | 0/0/1/0/0 | 0/1/1/1/1 | 0/1/1/1/1 | 0 everywhere |
| inc1, inc2 | 0/0/1/0/0, inc1 told falsely | 1, inc1 still false | 1, both orders now true | 0 |
| macro / macro2 | test 1 (false message) / 0 | 1 / 0 | 1 / 0 | 0 / 0 |
| onedef | build 2, test 0 | **0, prints 6 8** | build 2, test 0 | build 2, test 2 |
| extdef, extsame | build 2, test 1 | 1 | 1 | 2 / 2 |
| clash | build 0, **prints `6 4.0 10`** | 1 | 1 | 0, wrong value everywhere |
| fpat64 | build 0, **prints 4294967299** | 1 | 1 | 0, wrong value everywhere |
| fprec | build 1 | 1 | 1 | 1 |
| one / unguard | 1 / 0 | 1 / 0 | 1 / 0 | 1 / 0 |

**Census**: `build` over 1,323 program roots. A root is a tracked `.hero` file with an `extern` and a `main`, plus the 19 roots that `use` an extern module one level down. `selfhost/main.hero` and the harness root were kept apart.
- Today: 595 exit 0, 728 exit 1, 0 exit 2.
- Under (b) and under (i): **0 programs move**, and the first error line is identical in all 1,323.
- The tracked tree holds no program whose headers fail together. The real-world refusals come from the ffi-pragmatist's census (18 cross-library pairs on Mac, 92 on Linux). (i) refuses exactly what (b) refuses, which is why I withdraw it.

## Cost per route

Code lines are counted with an awk mirror of `layout`'s `code_of`, not the suite itself. Instruction counts are the `heroes` process only, because `time -l` does not count clang's child processes. The load average was 10 to 136, so no duration below is a measurement.

**(b), one unit everywhere**
- Prototype: 2 lines changed (`cli/assemble.hero:99`, `cli/compile.hero:245`).
- Full landing would delete `per_module` (49 code lines), `emit/unit.hero` (88) and `emit/members.hero` (159).
- `selfhost/main.hero`:

  | | today | (b) |
  |---|---|---|
  | warm build | 401.01e9 | 263.09e9 |
  | warm build, one string edited | 401.87e9 | 263.56e9 |
  | `--emit-c` | 489.09e9 | 296.28e9 |
  | `check` | 76.35e9 | 76.42e9 |

- What (b) loses is clang's side. Every edit recompiles one unit of 1.5 million lines. Compiling the seed alone as a stand-in: 13.3 s user and 747 MB at `-O0`, 81 s and 1.71 GB at `-O2`, both inflated by the load.

**(i), mine, withdrawn**
- +18 / −4 code lines in `cli/compiling.hero` and `cli/produce.hero`: the existing per-build probe becomes `-fsyntax-only`.
- Cost in clang instructions per build, measured with `-fintegrated-cc1`: +5% to +20% on one run.

**(g), 550**
- Built and run in `heroes-i2`: +13 / −4 code lines in `cli/header_refused.hero`, plus the flag `-fdiagnostics-show-note-include-stack`.
- inc1 and inc2 now get the same true words in both orders: "`a.h` and `b.h` … at `a_impl.h` line 1 and `b.h` line 1".
- Under (c), the flag also goes into `flags.hero`, so every cache key moves once. That part is unrun.

**(c), per module**
- The one-line `test` prototype is **wrong**. A test binary built per module links the program's `main`, so `main` runs before every test block. In `cases/fptest`, "6 8" is printed before each `ok` line, and the conflict-free control does the same. `emit/unit.hero`'s root shim has to carry the test runner instead; that is unbuilt.
- The conditions, all unbuilt:
  - **C0, probes per module.** This repairs fpat64 and fprec. `selfhost/` has 6 distinct module header sets, so about 7 probe runs instead of 1, each 0.155e9 to 0.311e9 clang instructions: under 0.5% of a warm self-build. Sites: `pointee`, `layout`, `compiling.probe`, `artifact.hero:80`.
  - **C1, clash.** Compare the C type of every external-linkage declaration that two or more groups bind, using `-ast-dump=json`. Measured at the C level: clash reads `long (long)` extern against `double (double)` extern, while fp's two definitions are static and are skipped. This has to run at `build`: `check` asks clang nothing, and §13's widening means the Heroes types alone would refuse correct programs.
  - **C2, onedef and extdef at exit 1.** The same dump shows `c.h`'s `twice` as an extern definition, so the duplicate can be told before the link, with no linker parsing.
  - **C3, 538's note.** Its remedy becomes "bind one of the two from a module of its own".
  - **C4, `--emit-c`.** Panel 200 R1's compile of the fused file catches fp (`--emit-c` exited 1 in the (b) prototype). No compile catches macro3 or cfg, so a meaning check is still owed.
- My estimate for the whole of (c), an inference from the sites: 200 to 350 code lines.

**(f), per group: unbuilt**
- Census of the 1,356 tracked files with a group:
  - 773 have a group record with fields, which (f) has to mirror in each module's unit;
  - 220 have a `partial` record;
  - 112 have a `constant`;
  - 950 function lines pass or return such a record by value.
- The FFI emission code is 34 modules and 4,439 code lines, more than the whole of Pascal-P4.
- `--emit-c` could no longer be one file, which changes the seed's bootstrap.
- (f) still needs C1 and C2.
- What it buys over (c): order-freedom inside one module. 107 tracked files name two or more distinct headers in one module.

**(a)**
- `check.hero` uses no toolchain today.
- A single probe costs 2 to 4 times what `check` itself spends on a small program (0.077e9, `heroes` process).
- It refuses the same real library pairs as (b).

**(d)**
- It leaves clash and fpat64 printing wrong values at exit 0.

**538's class (repair 5)**: by its text it is a seventh member, two headers refused together, where the sixth says "on its own". It landed on defect 060's precedent, because who is blamed moved and what is accepted did not. This sitting should write it into `c-boundary.md:47`.

## Argument (≤120 words)

Today's `build` is not per module: its probes fuse every header, so fprec is refused and fpat64 ships a wrong value. (b) and my (i) make the four verbs agree, but they refuse real library pairs that C accepts in two modules, so I withdraw (i). (c) adds no construct: it extends what `build` already claims to be, for an estimated 200 to 350 lines, provided its probes become per module (C0). (f) rewrites the FFI's layout path, mirroring 773 files' records, to buy order-freedom inside one module. Under (c), that freedom is one `use` line away. The ceiling favours (c) plus (g), with C0 to C4 built before 453 lands.

## Prediction

At the closing commit of the batch that lands (c):
- `fpat64` builds at exit 1 with `ffi_parameter_type`;
- `clash`, `onedef` and `extdef` exit 1 in build, test and run;
- `fp` exits 0 printing `6 8` in all three;
- the `selfhost/` diff is at most 400 code lines.

If (f) is chosen instead, its landing touches at least 15 of the 34 `emit/ffi*` and `extern*` modules.

## Condition

Any of these would change my verdict:
- a real program that needs two headers that cannot share a unit inside one module, where moving one group to its own module is impossible;
- an (f) prototype that mirrors records, `partial` and union fields included, in 400 code lines or fewer, and passes the `run` FFI goldens;
- C0 measured at more than about 150 lines.

## Not run

- Linux and Windows.
- C0 to C4 and the test shim inside the compiler.
- (f) and the meaning check for `--emit-c`.
- Any suite on any prototype.
- A census of `test` under (c).
- Any duration.

Everything is in `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/scratch-b15/202-compiler-engineer/`:
- `notes.txt`
- `runs/{base,rb,ri,rc}.summary`
- `cases/` (fprec, fpat64, fpat64ctl, fptest, fptestctl)
- `census/{base,rb,ri}/`
- `rb/`, `rc/`, `ri/`, `ri2/` (the prototypes)
- `tmp/warm-base.txt`, `tmp/warm-rb.txt`, `tmp/seedO0.txt`, `tmp/seedO2.txt`
