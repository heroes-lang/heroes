# Panel 202, llm-ergonomist (blind seat): folder `d2-b`

Copied by the coordinator on 2026-10-09 from the session's `report.md`, unchanged below the rule. A fresh `claude -p` session outside the repository (the author's exception of 2026-10-09), `claude-opus-5-5`, `--max-budget-usd 0.6`; started 23:46:42 and ended 23:48:04 by `date`, on the second run (the first, at 23:31, stopped on the account's session limit with no report); `run.json`: cost USD and turns 0.3054 11. Its inputs in `docs/panel/202-briefs/blind/d2-b/`, the spec the trunk's at `65b78f2e`.

---

# Report

Files read: `brief.md`, `spec.md`, `main.hero`, `left.hero`, `right.hero`, `mid.hero`, `a.h`, `b.h`, `c.h`.

The program: `main.hero` uses three modules. Each one has its own `extern` group that declares a C function called `twice`:

- `left.hero`: `extern "a.h"`, `twice(x: i64) -> i64`. `a.h` says `long twice(long x);`
- `right.hero`: `extern "b.h"`, `twice(x: f64) -> f64`. `b.h` says `double twice(double x);`
- `mid.hero`: `extern "c.h"`, `twice(x: i64) -> i64`. `c.h` *defines* `long twice(long x) { return 2 * x; }`

`main` prints `left.left(x: 3)`, `right.right(x: 4.0)` and `mid.mid(x: 5)`.

The main point: C has one global namespace for functions. All three groups name the same linker symbol `twice`. Only one definition exists, the integer one in `c.h`, and two of the declarations contradict each other (`long(long)` against `double(double)`). Each group agrees with its own header. The specification has no rule about two groups that name the same C function with different types. It does have such a rule for records ("two records may not name one tag but `void`"), but not for functions.

## check

- **Exit code:** 0
- **Output:** nothing. The specification does not say what a successful check prints. I assume it prints nothing.
- **Why:**
  - Each module is fine on its own terms. `use left`, `use right` and `use mid` bind three different names (section 1: "Every module you name needs its own `use`"; "A file may not bind one name twice"). Calls are qualified: `left.left(x: 3)`. The function `left` inside `left.hero` does not shadow anything, because `left.hero` has no `use left`.
  - The literals take the types their context asks for (section 2: "A literal takes the type its context asks for"). So `3` and `5` are `i64`, and `4.0` is an `f64` for `right.right(x: f64)`. No conversion is needed (section 3: "No implicit conversions").
  - `twice(x: x)` uses an optional argument name (section 9: `Arg = [ ident ":" ] ...`). Each parameter `x` is used, so nothing is unused (section 5).
  - Each function with `->` returns on its only path (section 8). `main` takes nothing and returns nothing (section 1). `print` takes numbers (section 11).
  - FFI: "clang checks every result type, constant and record field against that header", and "A **parameter** ... [is] declared at the header's own width and sign". Each group is checked against its own header, and each one matches. On LP64 (this machine is Darwin), C `long` is 64-bit signed, which is `i64`. `double` is `f64`, which section 3 also says ("`f32` is C's `float`", so `f64` is `double`). No group has `link`, and none needs it: the symbol's definition comes from `c.h` itself.
  - No `???` appears, so check reports no holes.

## build_and_run

- **Build:** exit 0, no output. A binary `main` is produced (this rests on choice point 1).
- **Run `./main`:** exit 0. The output is:

```
6
4.0
10
```

- **Why, line by line:**
  - `left.left(x: 3)` calls the C symbol `twice` with the `long` 3. The only definition is the one in `c.h`, `return 2 * x;`, so the result is `6`. The declaration in `a.h` has the same type as that definition, so this call is well defined.
  - `mid.mid(x: 5)` calls the same definition and gets `2 * 5 = 10`.
  - `right.right(x: 4.0)` is the problem line. `right.hero`'s code was checked against `b.h` and calls `twice` as `double(double)`. At link time, though, the symbol resolves to the one definition, `long twice(long)` from `c.h`. In C, calling a function through a type that is incompatible with its definition is undefined behavior. Concretely, on both arm64 (Darwin) and x86-64:
    - The caller passes 4.0 in a floating-point register (`d0` or `xmm0`).
    - The callee reads an integer register (`x0` or `rdi`) that holds whatever was left there, doubles it, and returns it in `x0` or `rax`.
    - The caller then reads its `f64` result from `d0` or `xmm0`. The callee never touched that register, so it still holds the argument.
    - So `right` most likely returns `4.0`, not the `8.0` the source suggests.
    - It prints as `4.0`, because "A float prints a point or exponent (`1.0`, ...)".
    - Doubling a garbage integer is `2 * x` in C, not Heroes arithmetic, so the Heroes rule that "Overflow aborts at every width" does not apply. Without UBSan or `-ftrapv` it does not trap.
  - `main` returns normally. No leases or handles are involved, so nothing aborts at the end (section 13). Exit code 0.

## test

- **Exit code:** 0
- **Output:** no test results, because none of the four files contains a `test` block. The specification gives no format for the runner's output. It might print nothing, or a summary saying zero tests ran. I assume nothing, or at most a zero-test summary.
- **Why:** "`test` blocks run only when asked for; ordinary builds ignore them." `heroes test` asks for them, and there are none. `main` is not run, so `twice` is never called. That means the undefined behavior in `right` does not come up, even if the test build links the same way as the normal build.

## choice_points

1. **Several groups naming one C function with different types.** The specification checks each group "against that header" and says nothing about two groups whose headers contradict each other on one symbol. The only cross-group rule is the one about record tags.
   - *My choice:* each group is checked and compiled with only its own header (one C translation unit per module, or an equivalent). Check and build then pass, the link resolves every `twice` to the definition in `c.h`, and the run prints `6`, `4.0`, `10` with exit 0.
   - *Other option A:* the implementation puts all headers into one C translation unit, or compares extern signatures across the program. Then clang (or the checker) reports conflicting types for `twice` between `a.h`/`c.h` and `b.h`. `heroes check` and `heroes build` both fail with a nonzero exit code and print that diagnostic. No binary is produced, so `./main` cannot run (the shell reports that the command was not found). `heroes test` fails the same way, nonzero with the same diagnostic.
   - *Other option B:* the compiler keeps per-module C files but notices at link time that one symbol has two types. The result is the same as option A, but only for `build` and `test`, while `check` passes.
2. **The value printed by `right.right(x: 4.0)`.** This is C undefined behavior, not a choice the specification makes. I chose the likely result from the calling convention, `4.0`. Other plausible outputs are any value the implementation leaves in the float return register. It would be `8.0` only if something called a real `double` version, and none is defined anywhere in these files.
3. **Output on success of `check`, `build` and `test`.** The specification does not say. I chose silence. The other choice is a short status or summary line, with the same exit codes.
4. **Width of `long`.** I assumed LP64, which matches the Darwin environment. On LLP64 (Windows), `long` is 32-bit, so `i64` against `long` would be refused, and check would fail for `left` and `mid`.

## confidence

- Low to moderate overall.
- High for the facts about the individual modules (types, scoping, literals) and for `test` having nothing to run.
- Moderate for check passing, since the specification has no rule that forbids this.
- Low for the build succeeding, because it depends on how the implementation organises its C translation units (choice point 1).
- Low for the exact middle line `4.0`, because it is undefined behavior.

## context

Nothing outside this directory's files reached my context, apart from the harness's own system information: environment details, the platform (Darwin), the date, and an attached account email that I did not use. I read only the files listed at the top and ran nothing.
