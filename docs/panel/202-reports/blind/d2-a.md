# Panel 202, llm-ergonomist (blind seat): folder `d2-a`

Copied by the coordinator on 2026-10-09 from the session's `report.md`, unchanged below the rule. A fresh `claude -p` session outside the repository (the author's exception of 2026-10-09), `claude-opus-5-5`, `--max-budget-usd 0.6`; started 23:46:42 and ended 23:47:57 by `date`, on the second run (the first, at 23:31, stopped on the account's session limit with no report); `run.json`: cost USD and turns 0.2945 11. Its inputs in `docs/panel/202-briefs/blind/d2-a/`, the spec the trunk's at `65b78f2e`.

---

# Report

## The program in brief

- `main.hero` uses three modules: `left`, `right` and `mid`. It prints `left.left(x: 3)`, `right.right(x: 4.0)` and `mid.mid(x: 5)`.
- Each module declares an extern group with a function named `twice` and wraps it:
  - `left.hero`: group `"a.h"`, `twice(x: i64) -> i64`. `a.h` only declares `long twice(long x);`.
  - `right.hero`: group `"b.h"`, `twice(x: f64) -> f64`. `b.h` only declares `double twice(double x);`.
  - `mid.hero`: group `"c.h"`, `twice(x: i64) -> i64`. `c.h` defines `long twice(long x) { return 2 * x; }`.
- No group has `link` or `package`. The only definition of `twice` anywhere is the body in `c.h`, and it takes and returns `long`.

On the Heroes side every file is valid. Each `twice` is bound only in its own file, so there is no shadowing. Calls are written qualified (`left.left`). `twice(x: x)` is allowed by `Arg = [ ident ":" ] [ "@" ] Expression`. On darwin, which is LP64, `long` is 64-bit signed, so `i64` matches it. `f64` matches `double`. Each declaration agrees with its own header.

The problem is on the C side. C has a single namespace for external function names. This program declares that one symbol, `twice`, both as `long(long)` and as `double(double)`. In one translation unit that is a constraint violation, and clang reports "conflicting types for 'twice'" (C11 6.7p4, 6.2.7p2). Across separate translation units it is undefined behavior, and the linker would not notice. The specification has no rule about extern names that clash between modules. My answers depend on how I resolved that gap; see `choice_points`.

## check

- **Exit code:** 0
- **Output:** nothing
- **Why:** Every Heroes rule is met. Each extern group is checked against its own header, and every one agrees:
  - `a.h` says `long(long)` and the group says `i64 -> i64`.
  - `b.h` says `double(double)` and the group says `f64 -> f64`.
  - `c.h` says `long(long)` and the group says `i64 -> i64`.
- **Spec sentences relied on:**
  - "clang checks every result type, constant and record field against that header": the check is made per group, against that group's header.
  - "A **parameter** and a **field** are declared at the header's own width and sign": `long` on LP64 is `i64`.
  - "`use geom` binds `geom` to `geom.hero`'s declarations, written qualified": each `twice` stays inside its own module.
  - "Shadowing is a compile error": it does not apply here, because no file binds `twice` twice.
  - The program has no `???`, no unused binding and no unused parameter.

## build_and_run

- **`heroes build main.hero -o main`**
  - **Exit code:** nonzero (1 assumed; the spec gives no number).
  - **Output:** a C compile error on stderr, in substance `error: conflicting types for 'twice'`. It points at `b.h` line 1 (`double twice(double x);`) against the earlier `long twice(long x);` from `a.h` or `c.h`.
  - No binary `main` is produced.
- **`./main`**
  - Because `main` does not exist, the shell prints `zsh: no such file or directory: ./main` and exits with 127.
- **Why:**
  - The built program needs a single C symbol `twice`, and its only definition is `long twice(long)` from `c.h`.
  - The generated C has to include all three headers. Doing so declares `twice` with two incompatible types, which clang refuses.
  - No Heroes rule could make `right.right` call a `double(double)` function, because none exists in the program.
- **C facts relied on:**
  - C has one external identifier namespace and no overloading.
  - Two declarations of one function with incompatible types in one translation unit are an error (C11 6.7p4).
  - Across translation units they are undefined behavior (C11 6.2.7p2).
  - LP64 `long` is 64 bits.

## test

- **Exit code:** nonzero (1 assumed).
- **Output:** the same C error as `build`, `conflicting types for 'twice'`. No test runs.
- **Why:**
  - The program declares no `test` blocks.
  - Running tests still requires compiling the program with its extern groups, so the same C conflict stops it.
  - Spec: "`test` blocks run only when asked for; ordinary builds ignore them." Nothing here says a test build skips the C stage.

## choice_points

1. **Clashing extern names across modules.**
   - **Gap:** the spec never says whether two groups in different modules may name the same C function with different signatures.
   - **My choice:** the clash surfaces where the C compiler sees all the declarations together, at build (and test).
   - **Other options:**
     - **(a)** The Heroes compiler refuses the clash itself, at `check`. Then all three commands exit nonzero with a Heroes diagnostic, something like "twice declared with different types in a.h/left.hero and b.h/right.hero".
     - **(b)** Each module becomes its own translation unit, so clang never sees the conflict and the link succeeds against the single `long twice(long)` from `c.h`. `./main` then exits 0 and prints:
       - `6` (left: 2 * 3)
       - a value that depends on registers for `right.right(x: 4.0)`
       - `10` (mid: 2 * 5)

       On arm64 darwin the caller passes 4.0 in `d0`, while the callee doubles whatever is in `x0` and never touches `d0`. The caller would then read back `d0` and most likely print `4.0` rather than `8.0`. On x86-64 the equivalent registers are `xmm0` and `rdi`, with the same result. This is undefined behavior, not a promise.

       Under (b), `test` exits 0 with nothing or a "0 tests" summary, since no test blocks exist.
2. **Does `check` run the header comparison?** I took it that `check` includes clang's per-header checks. Each header passes on its own, so the answer is the same either way unless `check` puts all headers into one translation unit. In that case `check` would also fail with `conflicting types for 'twice'`.
3. **Exit codes and message text.** The spec does not fix them. I assumed 1 for a compile failure and clang's usual wording.

## confidence

- **Low to moderate** on the exact outcome. The Heroes-level validity of every file and the per-header agreement are solid.
- The facts that C has a single `twice` symbol and that its only definition is `long(long)` are certain.
- Which tool stage reports the clash, or whether anything does, is not fixed by the spec. I would put about 50% on my main answer (build fails with a C conflicting-types error), 30% on (a), where the Heroes compiler refuses at check, and 20% on (b), separate translation units with undefined behavior at run time.

## context

Only this directory's files reached my context: `brief.md`, `spec.md`, `main.hero`, `left.hero`, `right.hero`, `mid.hero`, `a.h`, `b.h` and `c.h`. The harness also attached environment information, namely the working directory path, the platform (darwin), the date and the user's account email. I used only the platform, for the LP64 and calling-convention facts. I read no other file and ran nothing.
