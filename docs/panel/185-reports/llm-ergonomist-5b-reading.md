# Panel 185, R7's blind reading of (5b)'s wording

Run by the coordinator on 2026-10-02 at 03:34 from `<scratchpad>/185-blind-5b/`, on
the author's ratification of panel 185 with R7's route (a), one fresh session
outside the repository with the panel skill's command, `--model claude-opus-5-5`,
`claude` 2.1.285: 8 turns, 0.40 USD by the CLI's JSON report, `is_error` false.
Inputs: `docs/panel/185-briefs/blind-5b/` (the spec's one marker in
`spec-markers.diff`, panel 184's R1 applied). K is (5b) in the spec-warden's
short wording with R1's escape; M is (5a), panel 184's M; L is (5c), today.
The report below is the session's own file, copied unchanged.

# verdict

- **L**: object.
- **K**: object. It is not a veto, because the meaning is always literal text. Only whether the program compiles depends on bindings outside the line.
- **M**: approve.

# experiment

## Step 1: the program (written before reading o1-check.txt)

The body is the same under L, K and M. The spec gives no candidate a different spelling for the correct program, so I wrote one text and checked it under each.

```
function main()
    scores = [12, 7, 30]
    n = scores.len()
    best: i64 @ scores[0]
    best_at: i64 @ 0
    for i in range(from: 0, to: n)
        s = scores[i]
        print(f"score {i + 1} of {n}: {s}")
        if s > best
            best @ s
            best_at @ i
    print(f"best: {best} (score {best_at + 1})")
    print(f"{{best}} = {best}")
    print("{\"id\": 7}")
```

- Under L it compiles and prints `score 1 of 3: 12`, `score 2 of 3: 7`, `score 3 of 3: 30`, `best: 30 (score 3)`, `{best} = 30`, `{"id": 7}`.
- Under K it compiles and prints the same. The last literal has no f, but its braces hold `"id": 7`, which is not a bound name.
- Under M it compiles and prints the same. The text `"id": 7` is not an Expression (`:` is no operator), so it would not be a hole. I read "would be a hole" syntactically, from the line alone.

## Step 2: task1.hero and task2.hero (written before reading o1-check.txt)

The mistake in both files is the same: an interpolation written without the `f`.

task1.hero:
- Line 13 is `print("{count} rows")`.
- Line 14 is `print("total {total} over {rows.len()} rows")`.
- Line 11 is a correct `f` literal.

How each candidate treats them:
- **L**: both lines are legal plain text. However, `count` (line 6) is then never read, so line 6 is an unused-binding compile error. The program does not compile, but the error names line 6, not the literal. `total` is read at line 10, so line 14 raises nothing. A naive repair (deleting `count`) compiles and prints `row 3: row-3`, `row 5: row-5`, `row 8: row-8`, `{count} rows`, `total {total} over {rows.len()} rows`. That is silently wrong.
- **K**: line 13 is an error (`count` is bound). Line 14 is an error (`total` is bound). `{rows.len()}` is not a bare name, so on its own it would pass. The program does not compile.
- **M**: line 13 is an error. Line 14 is an error twice, since `total` and `rows.len()` are both expressions. The program does not compile.

task2.hero:
- Line 5 is `print("total {total}")`, a missing `f`.
- Line 6 is `print("{\"id\": 7, \"ok\": true}")`. This line is correct: it is intended JSON text.
- Line 7 is a correct `f` literal.

How each candidate treats them:
- **L**: it compiles and prints `total {total}`, `{"id": 7, "ok": true}`, `done: true`. Line 5 is silently wrong. `total` is read at lines 4 and 7, so no unused error saves it.
- **K**: line 5 is an error (`total` is bound). Line 6 is accepted.
- **M**: line 5 is an error. Line 6 is accepted, since `"id": 7, "ok": true` is not an expression.
- With line 5 fixed to `f"total {total}"`, K and M print `total 15`, `{"id": 7, "ok": true}`, `done: true`.

## Step 3: after o1-check.txt

The compiler reported only `unused_binding` for `count` at 6:5, with exit 1. It said nothing about lines 13 and 14, so it behaves as L. The message says "remove the binding, or read it". A model that follows the message removes the binding:

```
function label_for(at: i64) -> str
    return "row-" + to_str(at)

function main()
    rows = [3, 5, 8]
    total: i64 @ 0

    for n in rows
        total @ total + n
        print(f"row {n}: {label_for(n)}")

    print("{count} rows")
    print("total {total} over {rows.len()} rows")
```

Under L this compiles. `count` now appears only inside plain text, so it is not a reference. It prints:

```
row 3: row-3
row 5: row-5
row 8: row-8
{count} rows
total {total} over {rows.len()} rows
```

That is silently wrong on two lines. The intended output was `3 rows` and `total 16 over 3 rows`. Under K the same submission is still an error at line 13 only if `count` were bound. Since it was removed, line 13 now compiles as the text `{count} rows`, and line 14 is an error (`total`). Under M lines 13 and 14 are both errors, whatever is bound.

# choice_points

1. **Score lines.** I chose `f"score {i + 1} of {n}: {s}"`. Writing it without `f`:
   - L compiles and prints `score {i + 1} of {n}: {s}` three times (silent).
   - K is an error (`n` and `s` are bound).
   - M is an error.
2. **Best line.** I chose an f literal. A mixed form, `"best: " + to_str(best) + " (score {best_at + 1})"`:
   - L is silent.
   - K compiles and prints `best: 30 (score {best_at + 1})`, silent, because `best_at + 1` is not a bare name.
   - M is an error.
3. **`{best} = 30` line.** I chose `f"{{best}} = {best}"`. The alternatives:
   - `"{best} = " + to_str(best)`: L is correct. K is an error (`best` is bound). M is an error. The error is a false alarm on a correct program.
   - The same text, with the variable named `top` instead of `best`: K compiles and is correct. M is still an error. So under K, compiling depends on names bound on other lines.
   - `f"{best} = {best}"`: every candidate prints `30 = 30` (silent).
   - `"{best} = {best}"`: L prints `{best} = {best}` (silent). K and M are errors.
4. **JSON line.** I chose plain `"{\"id\": 7}"`. The alternatives:
   - `f"{{\"id\": 7}}"` is correct under all three.
   - `f"{\"id\": 7}"` is an error under all three, because the hole is not an expression.
5. **Index base.** I chose `i + 1`. Writing `{i}` prints `score 0 of 3: 12` under all three (silent, not rule-related).
6. **`range`.** Named arguments are mandatory, since both parameters are `i64`. `range(0, n)` is a compile error under all three.
7. **Comparison and starting value.** I chose `>` and `scores[0]`. `>=` or a start of `0` gives the same output for these scores under all three.

# argument

L lets a missing `f` compile as plain text. task2 then prints `total {total}` silently, and task1 only failed by accident, through an unused binding. The o1 message steered the fix toward deleting `count`, which produced a silently wrong program. K catches bare names but not `{i + 1}`. Whether a line compiles under K depends on what is bound elsewhere in the function or the module: renaming a variable flips a literal between error and accepted. M decides from the line's text alone. It catches every hole-shaped brace and keeps JSON-like text legal. Its cost is that literal `{name}` text needs `f"{{name}}"`. That cost raises a compile error, never a silent wrong output.

# prediction

Take 100 one-turn attempts at tasks that mix interpolation and literal brace text.

Silent wrong programs:
- L: at least 15%.
- K: 5% to 10%, from expression holes and unbound names.
- M: at most 3%, from errors unrelated to the rule.

After a compiler message on the first attempt:
- K and M: at least 90% of repairs fix the missing `f` in one turn.
- L, given an o1-style message: at least 50% of repairs remove or discard the binding and stay silently wrong.

# condition

- I would move M to object if models often write plain `"{x}"` meaning literal text and then fail to repair it within one turn, at a rate above L's silent-wrong rate.
- I would move K to veto if its "bound name" is shown to change a program's output, not only whether it compiles.
- I would move L to approve if silent-wrong rates under L measured within 2 points of M.

# context

Only this directory's files reached my context: brief.md, spec.md, task1.hero, task2.hero and o1-check.txt. The only other context was the system prompt, including an auto-attached user email, which I did not use.
