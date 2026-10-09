# Panel 202, llm-ergonomist (blind seat): folder `d3-b`

Copied by the coordinator on 2026-10-09 from the session's `report.md`, unchanged below the rule. A fresh `claude -p` session outside the repository (the author's exception of 2026-10-09), `claude-opus-5-5`, `--max-budget-usd 0.6`; started 23:46:42 and ended 23:47:31 by `date`, on the second run (the first, at 23:31, stopped on the account's session limit with no report); `run.json`: cost USD and turns 0.2292 12. Its inputs in `docs/panel/202-briefs/blind/d3-b/`, the spec the trunk's at `65b78f2e`.

---

# files

`right.hero` (changed; the only file changed or added):

```
extern "a.h"
    function twice(x: i64) -> i64

function right(x: i32) -> i32
    return twice(x: x.to_i64().must()).to_i32().must()
```

Unchanged: `main.hero`, `left.hero`, `a.h`, `b.h`.

Why: `a.h` and `b.h` both define a C function named `twice`, one with `long` and one with `int`. A single C unit cannot hold both, so clang refuses the two groups together (`ffi_header_refused`). The compiler's note suggests binding a shared C name from one header. A header of my own would not help: any second declaration of `twice` with a different type would conflict with `a.h` the same way. So `right.hero` now binds `twice` from `a.h` as well. It declares `twice` at that header's width (`long` is `i64`, section 13) and converts at the edges, so `right`'s signature stays `(x: i32) -> i32` and `main.hero` keeps working as it is. `to_i64` and `to_i32` return a `T?` (section 11), so each conversion is unwrapped with `.must()`. Widening an `i32` to `i64` cannot fail. Doubling any `i32` fits in an `i64`. The narrowing back to `i32` aborts only where the original `int` doubling would itself have overflowed.

The built program still prints `6` then `8`, as it did before.

# choice_points

- **Which header both groups use.** I chose `a.h` (`long`) for both. The other option was `b.h` (`int`) for both. That would make `left` narrow its `i64` argument to `i32` before the call, which could abort on large inputs that work today. For the values `main` passes it would print the same `6` and `8`.
- **Whether to keep `right`'s signature.** I kept `right(x: i32) -> i32` and convert inside it. The other option was changing it to `right(x: i64) -> i64` and returning `twice(x: x)` directly. `main`'s literal `4` would type as `i64` and still print `8`, but the module's interface would change.
- **Leaving `b.h` in place.** Nothing names it any more. I left it alone instead of deleting it. Deleting it would make no difference to the build.
- **Assumed platform width.** I assumed `long` is 64 bits (LP64, the platform here is darwin), so `i64` matches `a.h`. `left.hero` already makes the same assumption.

# confidence

Medium-high. The fix is the one the compiler's own note suggests ("bind a C name they share from one header"). The binding copies `left.hero`'s declaration of the same header exactly, and that declaration was not the error reported. Remaining uncertainty: I could not run anything. I have assumed that two groups naming the same header and the same C function is accepted (the note implies it is). I have also assumed that `x.to_i64()` on an `i32` gives an `i64?` as section 11 says.

# context

Nothing besides this directory's files reached my context: `brief.md`, `spec.md`, `output.txt`, `main.hero`, `left.hero`, `right.hero`, `a.h` and `b.h`. The harness also attached the user's account email automatically, and I did not use it. One read failed because I typed the directory path wrong, and the retry used the correct path in this directory.
