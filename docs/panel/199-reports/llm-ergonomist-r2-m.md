# Panel 199, llm-ergonomist (blind seat): folder `r2-m`, task 2 (`p2.hero`), variant B, the compiler-engineer's prototype on `fact(n)` with no base case, under the reading where an overflow is no path end: `check` exit 1 with `error[endless_recursion]`, one note, file `B-p2.txt`

Copied by the coordinator on 2026-10-08 from `<scratchpad>/readings/r2-m/report.md`,
unchanged below the rule. How it ran: a fresh `claude -p` session outside the
repository and outside any git tree (`<scratchpad>/readings-run.sh`, the skill's
command with the brief's three departures: `claude` 2.1.285, `--model
claude-opus-5-5`, `--max-budget-usd 0.85`, stdin from `/dev/null`), started
10:41:50 and ended 10:42:18 by `date`, exit 0; `run.json`: subtype `success`,
6 turns, `total_cost_usd` 0.1615 (by model: claude-haiku-4-5-20251001 0.0010 USD, claude-opus-5-5 0.1605 USD),
stop reason `end_turn`, 0 permission denials. The folder's
`output.txt` is the prototype's transcript copied unchanged from
`<scratchpad>/199-compiler-engineer/`; the spec is the frozen one, unchanged.
Its `context` answer, read by the coordinator: the folder's own files and the
harness's environment information (the account's email address among it), no
project rule, contract or memory; the reading stands. The mapping from the
folder's neutral label to task and variant is the coordinator's, in
`docs/panel/199-briefs/llm-ergonomist.md`. The coordinator's scoring of its
program is in `llm-ergonomist-scoring.md`.

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

1. **The base case's condition.** I chose `n <= 1`. With `n == 0` instead, the program also compiles and prints `120`, but `fact` of a negative number would recurse until "Recursion too deep aborts" (section 9). With `n <= 1`, a negative `n` returns 1, which is not a factorial either, but it ends. Another option is `n < 0` returning `fail(...)` with `-> i64?`. That changes the signature, and `main` would then have to handle the `T?` (for example `.must()`), because `print` does not take a fallible value. I did not do that because the brief says only that `fact(n)` gives the factorial.
2. **The shape of the branch.** I used an early `return` inside `if`, followed by the recursive `return`. Another option is `if ... else` with a `return` in each branch. That also compiles, because section 8 says a block leaves once it holds an `if` every branch of which leaves. It behaves the same way. Writing the recursive `return` after an `if` that has no `else` is valid because only a jump refuses the statement after it, and the `if` is not a jump.
3. **Recursion or a loop.** I kept the recursion. A loop with `acc: i64 @ 1` and `for i in range(from: 2, to: n + 1)` would also compile and print `120`. It does not keep the program's structure, so I kept the recursion and only added the missing path.
4. **The integer type.** I kept `i64`. 5! = 120 fits easily. Overflow aborts (section 7), so any `n` above 20 aborts either way.
5. **The literal `1`.** It takes `i64` from the return type (section 2). Nothing else could be chosen here.

# confidence

I believe the program compiles and prints `120`. The `error[endless_recursion]` at `p2.hero:2:16` in `output.txt` told me what to change. Its note asks for "a path that returns before the call", and the `if n <= 1` / `return 1` path is exactly that. I am least sure whether the checker will accept the base-case path as one that "returns before the call" when that return sits inside an `if` with no `else`. I expect it to, since that path does return without calling `fact`. I am also unsure how a negative `n` should behave, which the brief does not say.

# context

Nothing reached my context except this directory's files (`brief.md`, `spec.md`, `p2.hero`, `output.txt`) and the harness's own system context. That context included an account email address, which I did not use.
