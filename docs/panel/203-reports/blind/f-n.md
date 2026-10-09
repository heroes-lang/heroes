# Panel 203, llm-ergonomist (blind seat): folder `f-n`

Copied by the coordinator on 2026-10-10 from the session's `report.md`, unchanged below the rule. A fresh `claude -p` session outside the repository (the author's exception of 2026-10-09), `claude-opus-5-5`, `--max-budget-usd 0.6`; started 00:00:40 and ended 00:01:13 by `date`; `run.json`: cost USD and turns 0.1815 6. Its inputs in `docs/panel/203-briefs/blind/f-n/`, the trunk's spec.

---

# program

```
function go(n: i64, f: (function(i64) -> i64)) -> i64
    return f(n)

function step(n: i64) -> i64
    if n <= 0
        return 0
    return go(n: n - 1, f: step)

function main()
    print(step(1))
```

Expected output when run: `0` followed by a newline.

# choice_points

1. **What to change to fix `endless_recursion`.** The compiler said every path through `step` calls `step` again (through `go`, which always calls `f`) and that no call can return. The diagnostic offers two ways out: give `step` a path that returns before the call, or give `go` a path that returns before calling `f`. I chose to give `step` a base case. `go` reads as a plain "apply `f` to `n`" helper, so adding a condition there would change what it means. Changing `go` would also build, but `go` would then have to choose a value based on `n`, which is a decision the program gives no hint about.

2. **The base case condition and its value (`if n <= 0` / `return 0`).** The program gives no hint what `step` should compute. I chose the simplest terminating countdown: stop at zero and return 0. I used `<= 0` rather than `== 0` so a negative argument also stops right away instead of counting down until overflow aborts. Other options, such as counting up to some bound (`if n >= 10 return n`, with `n + 1`), would build too but would print something else (for example `10`). Any such bound would be invented.

3. **Changing the recursive argument to `n - 1`.** A base case alone is not enough. With `go(n: n, f: step)` unchanged, `step(1)` would call `step(1)` forever. The program would build, because a path out now exists, but at run time it would abort once the recursion got too deep. Decreasing `n` makes the recursion reach the base case. Increasing it with a bound at the top would be the other choice, as noted in point 2.

4. **Named arguments at `go(n: n - 1, f: step)`.** The two parameters have different types, so names are optional (section 9). I kept them as the original wrote them. Leaving them out would compile the same way.

# confidence

I believe the program builds. `step` now has a path that returns before the recursive call, every path in a `->` function ends in `return`, nothing is unused, and the types line up: `step` is `(function(i64) -> i64)`, and the literal `0` and `n - 1` are `i64`. The `if` is a statement with no `else`, which section 8 allows, and the `return` after it is reachable. At run time `step(1)` calls `go(0, step)`, which calls `step(0)`, which returns 0, so the program prints `0`.

What I am least sure of is the intended meaning. The original program carries no sign of what `step` was meant to compute, so the base case, its return value and the direction of the count are my guesses. What I am sure of is that the fixed program builds and terminates. I am not sure it gives the value the author wanted.

# context

Nothing reached my context besides this directory's files (`brief.md`, `spec.md`, `main.hero`, `output.txt`) and the harness's own system context, which included an account email address that I did not use. I read no other files and ran nothing.
