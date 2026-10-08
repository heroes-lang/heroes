# Panel 199, llm-ergonomist (blind seat): folder `r2-n`, task 2 (`p2.hero`), variant A, today's compiler output on `fact(n)` with no base case (the build silent at exit 0, `panic: stack exhausted in p2.fact`, exit 134)

Copied by the coordinator on 2026-10-08 from `<scratchpad>/readings/r2-n/report.md`,
unchanged below the rule. How it ran: a fresh `claude -p` session outside the
repository and outside any git tree (`<scratchpad>/readings-run.sh`, the skill's
command with the brief's three departures: `claude` 2.1.285, `--model
claude-opus-5-5`, `--max-budget-usd 0.85`, stdin from `/dev/null`), started
09:54:44 and ended 09:55:09 by `date`, exit 0; `run.json`: subtype `success`,
6 turns, `total_cost_usd` 0.1674 (by model: claude-haiku-4-5-20251001 0.0010 USD, claude-opus-5-5 0.1664 USD),
stop reason `end_turn`, no permission denial recorded
(`permission_denials` has 0 entries). Its `context`
answer, read by the coordinator: the folder's own files and the harness's
environment information (working directory, platform, date and the account's
email address), no project rule, contract or memory; the reading stands. The
mapping from the folder's neutral label to task and variant is the
coordinator's, in `docs/panel/199-briefs/llm-ergonomist.md`; the session never
saw it. Not run by the session: it has no shell, so its program is scored by the
coordinator in a copy (the brief's *What is scored*), in a later entry.

---

# program

```
function fact(n: i64) -> i64
    if n <= 1
        return 1
    return n * fact(n - 1)

function main()
    print(fact(5))
```

# choice_points

1. **Base case condition.** I wrote `if n <= 1` and return `1`.
   - Alternative `if n == 0`: compiles, prints `120` for `fact(5)`, but a negative
     argument would recurse until the stack is exhausted (the same abort as in
     `output.txt`). The alternative `n <= 0` also prints `120` and costs one more call.
   - Alternative: abort on a negative `n` with `assert n >= 0`, or make `fact`
     return `i64?` and fail on it. That compiles too, but `main` would then have to
     handle the `T?` (`.must()` or `match`). For 5 the output is the same. I kept
     the plain `i64` signature because the brief only asks for `fact(5)`.
2. **If-without-else, then fall through to `return`.** Section 8 says a function
   with `->` must `return` on every path that reaches its end. The `if` branch
   returns, and the final `return` covers the other path, so no `else` is needed.
   Wrapping the second return in `else` would also compile and do the same thing.
   Leaving out the final `return` would be a compile error.
3. **Recursion vs a loop.** I kept recursion. An iterative version with
   `acc: i64 @ 1` and `for i in range(from: 2, to: n + 1)` would also compile and
   print `120`. Recursion was what the original program did, and depth 5 is safe.
4. **Integer type.** I kept `i64`. Overflow aborts (section 7), so `fact(21)` and
   larger would abort. That does not matter for `fact(5)`. A `u64` would only
   reach one more value and would need `u64` literals at the call.
5. **Printing.** `print(fact(5))` takes an `i64` directly (section 11), so no
   `to_str` is needed. It prints `120` followed by a newline.

# confidence

I am confident the program compiles and prints `120`. In `output.txt`, `heroes
check` and `build` exit 0 and then `./p2` gives `panic: stack exhausted in p2.fact`
(exit 134). That told me the recursion had no base case, and adding one is the
only change I made. The thing I am least sure of is the layout rule: that an
`if` whose block ends in `return`, followed by a further statement in the same
function body, is accepted. Section 8 says only a jump refuses the statement
after it, and here the jump is inside the `if` block, not directly before
the outer `return`, so it should be fine.

# context

Nothing from outside this directory reached my context. I read only `brief.md`,
`spec.md`, `p2.hero` and `output.txt`. The system context also included the
user's email address and environment details such as the working directory path
and platform, and I did not use them.
