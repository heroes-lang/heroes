# Panel 202, llm-ergonomist (blind seat): folder `d3-a`

Copied by the coordinator on 2026-10-09 from the session's `report.md`, unchanged below the rule. A fresh `claude -p` session outside the repository (the author's exception of 2026-10-09), `claude-opus-5-5`, `--max-budget-usd 0.6`; started 23:46:42 and ended 23:47:34 by `date`, on the second run (the first, at 23:31, stopped on the account's session limit with no report); `run.json`: cost USD and turns 0.2473 11. Its inputs in `docs/panel/202-briefs/blind/d3-a/`, the spec the trunk's at `65b78f2e`.

---

# files

Only `right.hero` changes. Here is the whole file:

```
use left

function right(x: i32) -> i32
    return left.twice(x: x.to_i64().must()).to_i32().must()
```

`main.hero`, `left.hero`, `a.h` and `b.h` are unchanged. `b.h` is no longer named by any group.

# choice_points

1. **How to settle the conflict.** clang refuses `a.h` and `b.h` in one unit because each defines a C function named `twice`, with different types (`long` in one, `int` in the other). The diagnostic gives two remedies. The second, a header of my own declaring only what one group binds, does not work here: that header would still declare `twice`, and a declaration would conflict with the `static inline` definition in the other header. So I took the first remedy: the shared C name is bound from one header, by one group. The alternative was to rewrite `right` in plain Heroes (`return x + x`). That prints the same `8`, but it drops the C call the program makes today.

2. **Which header keeps `twice`.** I kept `a.h` (`long twice(long)`) because its type holds every `i32` argument and every doubled result. If I had kept `b.h` instead, `left` would have to narrow its `i64` argument to `i32` and would abort on large inputs that work today.

3. **How `right` reaches the binding.** `right.hero` now has `use left` and calls `left.twice`. An `extern` group is a `Declaration` in the grammar, so I read its members as declarations of `left` that can be called with the module prefix (section 1). The other option was a second `extern "a.h"` group in `right.hero` binding `twice(x: i64) -> i64`. I avoided it because the note says each group's header is included in the unit, and `a.h` has no include guard. Two inclusions would likely bring a redefinition of `twice`.

4. **The signature of `right`.** I kept `right(x: i32) -> i32` and convert at the call. Section 11 says `to_i64` gives a `T?`, so it needs `.must()`, which cannot fail coming from an `i32`. `to_i32().must()` aborts on overflow, which is what Heroes arithmetic does anyway. Changing the signature to `i64` would also print `8` (the literal `4` would take `i64`), but it would change the module's interface for no reason.

`main` still prints `6` and then `8`.

# confidence

Medium. I could not run anything, so this is checked against the spec only. The main risk is point 3: the spec never says outright that one module's `extern` functions can be called as `left.twice` from another module. If they cannot, the fallback is a second `extern "a.h"` group in `right.hero` with `twice(x: i64) -> i64`. If that one is refused for including `a.h` twice, the last option is the plain-Heroes `x + x`.

# context

Nothing outside this directory reached my context. I read only `brief.md`, `spec.md`, `output.txt`, `main.hero`, `left.hero`, `right.hero`, `a.h` and `b.h`. The system context also included the user's account email, which I did not use.
