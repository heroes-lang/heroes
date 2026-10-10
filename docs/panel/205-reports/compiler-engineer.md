# Panel 205, compiler-engineer

Copied by the coordinator at 03:24 on 2026-10-10 (`date`) from the seat's final reply (its transcript), verbatim below the rule; the seat's running notes are in its folder under `.claude/worktrees/scratch-b15/` (ignored by git).

---

# Panel 205, compiler-engineer

I started at 02:51:57 and finished at 03:23:21 (`date`). Everything ran in `/Users/joseph/Temp/heroes/heroes-lang/.claude/worktrees/scratch-b15/205-compiler-engineer/`. The `.git` file was removed from each copy, I ran no git command inside any copy, and I wrote nothing in the frozen tree. The running notes are in `notes.txt` there.

## The route I built: H (for Q2 and defect 570)

H changes two files of the frozen source:
- `tree/selfhost/emit/macro_guard.hero`
- `tree/selfhost/emit/extern_probe.hero`

What the emitted unit gets, around and after the groups' includes:
- **Before them:** `#pragma clang diagnostic push`, then `ignored "-Weverything"`, then `error "-Wmacro-redefined"`. That last line is a new constant, `KEPT`, and it is panel 204's R2.
- **After the guard closes:** `pop`, then one `error` pragma for each warning a check rests on. That list is every `-Werror=` word in `flags.flags()`, read from that one list, plus three warnings clang already makes errors by default: `-Wint-conversion`, `-Wimplicit-function-declaration` and `-Wincompatible-function-pointer-types`.
- **In the probe region:** the same list is raised again.

The compiler already uses this pragma mechanism in three places: `extern_probe.hero:133-143`, `record_constant.hero:113-124` and `layout_screen.hero:85-87`.

**Measured results, frozen compiler against H, this Mac and the Linux arm64 container (Debian clang 22.1.8):**
- **Defect 570 is closed, and so are six shapes beside it.** Each one builds at exit 0 today on both platforms and is refused under H:
  - `leak`
  - `ifd`: the header ignores `-Wimplicit-function-declaration`, so `getpid() -> i64` through a header that never declares it builds and prints `true`. Under H it is refused `ffi_unknown_name`.
  - `icv`: the header ignores `-Wint-conversion`, so `strlen(s: i64)` builds. Under H it is refused `ffi_parameter_type`.
  - `push` (an unbalanced push), `gcc` (the `#pragma GCC` spelling), `warn` (a downgrade to warning) and `mac` (a `_Pragma` inside a constant's macro).
- **Defect 569 is closed.** GMP's `g.hero` prints `6` and libavutil's `av.hero` prints `3998054` on this Mac.
- **The binding checks still fire through GMP.** `gw` (a wrong sign) and `gp` (a pointer declared as an integer) are refused, and `gok` builds.
- **Raw header warnings are gone.** `wh.hero` no longer prints clang's text; it prints `5`.
- **R2 is kept between headers.** `ej` and `je` (Expect with libjpeg) are refused `-Wmacro-redefined`.
- **Q1 does not move.** `sw` and `gnu/plain`/`gnu/own` give the same results as before.
- **The compiler's own tests:** 1,542 passed.
- **Fixpoint:** H builds itself, and the two emissions compare equal with `cmp` (1,531,013 lines).
- **Census of all 1,402 tracked files with an `extern "` group** (from `git --no-optional-locks ls-files` on the trunk at `46c975c4`), `--emit-c` with both compilers. Exactly 7 exit codes move:
  - `tests/golden/unsupported/fixedbugs-360-a-header-only-the-builds-flags-refuse.hero` goes 1 → 0. That case pins the policy H reverses.
  - Six `docs/panel/204-briefs/blind/*/limit_ba.hero` go 0 → 1. This is R2's ratified verdict on `cfgone/main2`, caused by `KEPT`.
  - The emitted text changes in 647 files.

**One residual is still open (a row of 570, same cause).** In `cases/beside/callee.hero` the header defines an object-like macro naming the bound function, `#define my_abs _Pragma("clang diagnostic ignored \"-Wsign-conversion\"") my_abs`. It builds at exit 0 under both compilers, on the Mac and on Linux. I built a fix in `tree2/heroes-p`, which puts the `_Pragma` raise chain between the callee and its first argument. It closes the hole, but every `ffi_parameter_type` message gets worse, from *`x` of `abs`* to *a parameter of `abs`*, because of how `probe_reply.hero` maps clang's column to an argument. So I do not recommend that fix as built.

## Answers to the brief

- **Can the runtime's prefix move?** Not under today's guard. In `c/rb/b.c` the runtime keeps only its types, a group header that includes `<stdint.h>` and `<math.h>` sits inside the guard, and `<stdint.h>`/`<math.h>` come after. The guard's pop then leaves `INT64_C` and `HUGE_VAL` undeclared, and the later `#include` does nothing because of its include guard. Today's order with a hostile `INT64_C` header still prints `42`. `heroes_runtime.h` itself uses `UINT64_C` at `:160`, `:165` and `:186`. So Route B, "groups first" and "a program header before the prefix" all reopen defect 361's guard.
- **Which `-Werror` must a package's own code still meet?** None, as far as the binding checks are concerned. Every check fires on the compiler's own lines, not inside the header. A system directory changes none of their answers: `gw` is refused on Linux, where `gmp.h` lives in `/usr/include`.
- **Panel 198:** the word list stands. Its *as `-I`* reason becomes moot, and the note at `selfhost/cli/header_refused.hero:290` and the premise at `selfhost/cli/package_words.hero:36-44` become false.
- **Route A for Q1** (a package answering `-D`) works today and costs 0 lines. Its `-D` reaches every unit of the program (`selfhost/cli/produce.hero:142`). The unit's cache key names it (`selfhost/cli/units.hero:115-117`), and the test *"a package's -D reaches the unit's compile … (defect 161)"* passed in my run.

## The verdict

- `verdict`: **approve** H for Q2 and 570. **object** on Q1 to any route that reorders the unit or adds a group form; the standing route is a package's `-D`. No veto, because nothing here adds a Part 5 construct.
- `section`: design.md §1.1 (the ceiling), §1.7 and Part 5 (H is emitted text only), §4.19, and `:3744` (no warning level).
- `implementation_cost`:
  - **H:** `macro_guard.hero` goes from 198 to 248 lines and `extern_probe.hero` from 230 to 236, in layout's unit (`.claude/hooks/ceiling.py`'s `code_lines`). The raw diff is +73/−6. Nothing changes in the lexer, the checker, descriptors, ownership or `runtime/`.
  - **The seed** grows by 26 lines, and 72,778 lines differ because `#line` numbers shift.
  - **Build cost:** a cold self-build took 446,955,276,533 instructions on the frozen compiler and 446,893,523,863 on H.
  - **Owed at landing:** the two false texts above, design.md §4.19, goldens for each shape, and the 360 case rewritten by hand.
  - **A Q1 group clause (read, not built)** would touch `keywords.hero` (297 lines) and `head_names.hero` (292), both within 8 lines of the ~300 threshold, plus `ast.hero` 523, `parse/group_head.hero` 111, `print/fmt.hero` 1096, `emit/externs.hero` 192, `emit/decls.hero` 262, and the spec.
- `needed_for_self_hosting`: no.
- `argument`: H adds no construct. It is emitted text in the one place that already wraps the groups, using a mechanism the compiler already uses three times. A header's own code is judged the way clang judges a system header, which Linux already does for every apt library, so the verdict stops depending on where the library is installed. Every warning a check rests on becomes an error again after the header region, which closes 570 and six shapes beside it on two platforms. For Q1, every route that reorders the unit breaks defect 361's guard (measured), and a group form lands in two modules already at the ceiling, for a function a package's `-D` already reaches.
- `prediction`: at H's landing gate, the census of the 1,402 tracked files moves exactly these 7 programs and no other; `selfhost/` grows by at most 70 lines in layout's unit over at most 3 files; the compiler's own tests all pass.
- `condition`: any of these would change my verdict:
  - an eighth program moves at that census;
  - the CI's clang 18 prints `-Wunknown-warning-option` for any name in the raise list;
  - a correct program is measured that no package's `-D` can serve, which would make me approve a form, priced first against those two ceilings;
  - R2 inside the region refuses a correct pair of headers.

## What I did not run

- Windows, Linux x86-64, and the CI's Ubuntu clang 18.
- The critic's routes D and E, and Route B or a group clause built in the compiler.
- H with `KEPT` empty, meaning R2 not adopted. My inference, unrun: `ej`, `je` and the six `limit_ba` would build silently.
- The lend check at call sites after a constant's `_Pragma`.
- The `layout` suite itself (I used its mirror in `.claude/hooks/ceiling.py`), the full net, and `heroes measure`.
- The deprecation warning on the compiler's own line (`uc.hero`). H does not change it, so a correct program still prints a raw clang warning at exit 0. I did not check whether that is already filed.

## A possible breach

At about 03:00 I ran `pkill -f "census.sh"` and `pkill -f "xargs -P 2"`. Both patterns can match other sessions' processes. Lane b18-infer runs a `./census.sh` (PID 58854, seen at 03:09, started after my kill). I cannot tell whether one of its processes, or another session's `xargs`, was running at 03:00 and was killed.
