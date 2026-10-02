# Panel 185, the blind seat (llm-ergonomist)

Run by the coordinator on 2026-10-02 at 01:07 from `<scratchpad>/185-llm-ergonomist/`,
one fresh session outside the repository with the panel skill's command,
`--model claude-opus-5-5`, `claude` 2.1.285: 7 turns, 0.61 USD by the CLI's
JSON report, `is_error` false; the sitting's one paid run, named in
`docs/panel/185-briefs/llm-ergonomist.md`. Its inputs are `docs/panel/185-briefs/blind/`
(the spec's markers in `spec-markers.diff`). The candidates map to the
sitting's routes: A1 (2a), A2 (2b), A3 (2c); B1 (3a), B2 (3b); C1 (5a), C2 (5d),
C3 (5c). The report below is the session's own file, copied unchanged.

# Report

## A

### verdict

- **A1**: approve
- **A2**: object
- **A3**: approve

### experiment

Under A1 (shortest form A1 allows):

```
variant Shape
    dot
    line
    ring

function tally(ss: [Shape]) -> i64
    dots: i64 @ 0
    rings: i64 @ 0
    for s in ss
        match s
            .dot => dots @ dots + 1
            .line =>
                _ = 0
            .ring => while rings < 6
                rings @ rings + 2
    return dots * 10 + rings
```

Under A2 (a mutation and a loop are not `Inline`, so both go in blocks):

```
variant Shape
    dot
    line
    ring

function tally(ss: [Shape]) -> i64
    dots: i64 @ 0
    rings: i64 @ 0
    for s in ss
        match s
            .dot =>
                dots @ dots + 1
            .line =>
                _ = 0
            .ring =>
                while rings < 6
                    rings @ rings + 2
    return dots * 10 + rings
```

Under A3 (the mutation inline, the loop as the arm's block):

```
variant Shape
    dot
    line
    ring

function tally(ss: [Shape]) -> i64
    dots: i64 @ 0
    rings: i64 @ 0
    for s in ss
        match s
            .dot => dots @ dots + 1
            .line =>
                _ = 0
            .ring =>
                while rings < 6
                    rings @ rings + 2
    return dots * 10 + rings
```

### choice_points

1. **The do-nothing arm.** I chose a block holding `_ = 0`, which section 8 names as the do-nothing arm. The other options:
   - `.line => _ = 0` inline. Under A2 and A3 this is a compile error, because a binding is not in `Inline`. Under A1 it is the production `ident "=" Expression`, one of the two A1 excludes, so it is a compile error too. Section 5 says `_` "binds nothing", though, so an implementer could read A1 as admitting it. The specification leaves that open.
   - `.line => 0` inline. This is allowed by the grammar under all three, since `Expression NEWLINE` is always `Inline`. It is probably a compile error under all three, either because "a line that computes a value must use it" or because its `i64` disagrees with the `()` of the other arms. The specification does not say whether the arms of a `match` used as a statement must agree in type.
   - `.line => continue`. This compiles under all three. Here it does the same thing, because nothing follows the `match` in the loop. Section 8 says it is not a do-nothing arm: if a line were added after the `match`, the program would silently skip it for that case.
2. **Inline loop.** Under A1 I wrote `.ring => while rings < 6` on the arm line. Under A2 and A3 that same line is a compile error. Under A1 the block form is also legal and means the same thing.
3. **Inline mutation.** `.dot => dots @ dots + 1` is a compile error under A2 (`dots` parses as an `Expression` and then `@` is unexpected). It is legal under A1 and A3.
4. **Indentation of the loop body under A1.** The body goes one level (4 spaces) below the arm line. If it is aligned to the column of `while`, the indentation is not a multiple of 4, so it is a compile error. If it is two levels deeper, it is an unexpected indent, also a compile error.
5. **Reading the counters.** Writes do not count as a use, so the function must read `dots` and `rings`. Returning them is the read. A function that returns nothing and only mutates them is a compile error under all three (unused binding).
6. **Loop condition.** `while rings < 6` stops at 6. `rings <= 6` compiles under all three and silently overshoots to 8. This comes from the task, not from the rule, and is the same under every candidate.

### argument

None of the three makes a line's meaning or whether it compiles depend on anything but the arm's own line. All three are local and purely syntactic. The difference is how often a model's habitual form is refused. Rust, Scala, Kotlin and Swift all accept an assignment after `=>` or its equivalent, so A2 refuses the most common arm a model writes (`=> count @ count + 1`). The repair is one turn, but it is frequent. A3 refuses only an inline loop, which no mainstream language lets a model rehearse; Python forbids a compound statement after `:`. A1 accepts that too, at no cost in meaning. None of them produces a silently wrong program from this rule.

### prediction

Over 100 one-turn generations of task A:

- **First-attempt compile errors caused by the `Inline` rule:** A2 at least 30%, A3 at most 5%, A1 at most 3%.
- **One-turn repairs:** at least 95% of those errors under every candidate.
- **Silently wrong programs attributable to the rule:** 0% under all three.

### condition

- I would move A2 to approve if fewer than 10% of A2 generations put a mutation inline.
- I would move A1 to object if models under A1 misplace statements meant to follow an inline loop (indenting them into the loop body) in more than 5% of cases, compared with the block form.

### task-a.hero under each candidate

- **Line 11** `.red => reds @ reds + 1`: accepted under A1 and A3. A compile error under A2.
- **Line 12** `.green => seen: i64 = 1`: a compile error under all three. A1 excludes declarations, and A2 and A3 do not list them. Even in a block, `seen` would be an unused binding.
- **Lines 13-14** `.blue => while spins < 3` with its body: accepted under A1. A compile error under A2 and A3.
- **Output:** the program compiles under no candidate, so it prints nothing. If line 12 were made a block holding `_ = 0`, it would compile under A1 only and print `23`: two reds give 20, and the first blue drives `spins` to 3.

## B

### verdict

- **B1**: approve
- **B2**: object

### experiment

Under B1:

```
function rank(k: i64, j: i64) -> i64
    x = match k
        0 =>
            if j == 0
                return 1
            else
                return 2
        _ => 20
    return x
```

Under B2 (the jump written on the arm itself):

```
function rank(k: i64, j: i64) -> i64
    x = match k
        0 => return match j
            0 => 1
            _ => 2
        _ => 20
    return x
```

The B2 version compiles under B1 as well.

### choice_points

1. **Branches that all leave, as the arm's last line.** Under B1 I wrote an `if` whose every branch returns. Under B2 that is a compile error. Writing it as a `match j` whose arms all `return` gives the same result: accepted under B1, a compile error under B2.
2. **One branch giving a value.** For example, `if j == 0` / `return 1` / `else` / `2`, or an inner match with `0 => return 1` and `_ => 2`. This compiles under both, and `x` takes 2, which is then returned. The meaning is the same as the experiment's.
3. **No `else`, followed by a trailing jump.** For example, `if j == 0` / `return 1` and then `return 2` as the block's last line. Under B1 this is a block whose last statement is a jump, so it leaves: accepted. Under B2 the specification does not say whether a block ending in a bare jump (as opposed to an arm that is the jump) gives no value, so it may compile or may be an error. I avoided it.
4. **No `else` and no trailing jump.** The block then yields the `()` of an `if` without `else`, which disagrees with `20`. That is a compile error under both, not a silent wrong.
5. **Hoisting the test.** `if k == 0` ... `return` placed before a `match k` that has only `_ => 20` is legal under both, since `_` is allowed on `i64`. That changes the shape of the program, not its result.
6. **Inline `return match j` with arms indented below the arm line.** I read the arms as the `match`'s block, one level below the arm line, under "a statement whose last part is a block ends with that block". If an implementation refused that reading, the B2 experiment would need the hoisted form from point 5.

### argument

B1 lets a model write the shape it already knows from Rust: nested branches that each `return`, inside a bound `match`. Whether such a block is a jump depends only on its own branches, so the check is local. B2 refuses that shape. Whether a given `if` or `match` compiles then depends on whether the enclosing block's value is used, and that is decided on a different line (here line 2's `x =`, possibly far above). This concerns compiling, not meaning, so it is not a veto. B2's repair also invites a silent wrong: turning `return 1` into `1` gives the same result here, but it changes the result wherever the bound name is transformed before being returned.

### prediction

Over 100 one-turn generations of task B:

- **First-attempt compile errors from the rule:** B2 at least 40% (models write nested returns), B1 0%.
- **Silently wrong programs:** in a variant whose function returns `x + 1`, B2 repairs that replace the returns with values will be silently wrong in at least 10% of repaired programs. Under B1 the rate is 0%.

### condition

- I would move B2 to approve if repairs under B2 never change a program's result across a set of tasks where the bound value is transformed after the `match`.
- I would move B1 to object if a model produces a program that compiles under B1 and returns something other than intended because a block silently counted as a jump.

### task-b.hero under each candidate

- **Lines 4-6**, the inner `match j` whose arms both `return`, as the last line of the arm block of a bound `match`: accepted under B1, where it counts as a jump. A compile error under B2, reported on line 4 and caused by line 2's binding.
- **Output:** under B1 it prints `1`, `2`, `20`. Under B2 it does not compile and prints nothing.

## C

### verdict

- **C1**: approve
- **C2**: object
- **C3**: object

### experiment

This form compiles under all three candidates with the same output:

```
function main()
    total: i64 @ 0
    for n in [4, 5, 6]
        total @ total + n
    print(f"total {total}")
    print(f"{{\"id\": 7, \"ok\": true}}")
    print(f"done: {total > 10}")
```

Under C2 and C3 only, line 6 may also be the plain `print("{\"id\": 7, \"ok\": true}")`. It compiles and prints the text exactly.

### choice_points

1. **`total 15` as a plain literal.** For example, `"total {total}"`. Under C1 and C2 this is a compile error. Under C3 it compiles and prints `total {total}`, which is silently wrong.
2. **`total 15` written with `print`'s arguments.** `print("total ", total)` works under all three. `print("total", total)` prints `total15` (print adds no separator), which is silently wrong under all three.
3. **The JSON text as a plain literal.** Under C2 and C3 it is unchanged and correct. Under C1 it depends on reading "would be a hole":
   - If it means any `{` with a closing `}`, it is a compile error.
   - If it means only a `{` whose contents parse as an expression, it is accepted. `"id": 7, ...` is not an `Expression`, since an `Entry` is not one.

   The specification leaves this open. My f form with doubled braces sidesteps it.
4. **The JSON text in an `f` literal with single braces.** The hole would hold `\"id\": 7, ...`, which is not an expression, so it is a compile error under all three.
5. **The JSON text in a plain literal with doubled braces**, as a Python habit would write it outside an f-string. Under C1 the outer `{` closes at the last `}`, so this is a compile error. Under C2 the `{` is followed by `{`, not a name, so the literal is unchanged and prints `{{"id": 7, "ok": true}}`, which is silently wrong. Under C3 it is the same silent wrong.
6. **What counts as "a name" in C2.** I read it as syntactic: any identifier. If it meant a name in scope, whether a literal compiles would depend on bindings made on other lines.
7. **Forgetting the `f` before an expression hole.** For example, `"done: {total > 10}"` without the `f`. Under C1 this is a compile error. Under C2 it is not a name, so the literal is unchanged and prints `done: {total > 10}`, which is silently wrong. Under C3 it is the same silent wrong.
8. **The accumulator.** `total = 0` followed by `total @ ...` is a compile error under all three, because only a name declared with `@` can be mutated.

### argument

Forgetting the `f` is the commonest interpolation slip, because a model trained on Python, JavaScript and Kotlin sees `{x}` and `${x}` in plain-looking literals. C3 turns every such slip into a program that compiles and prints the hole's source. C2 catches only bare names and dotted calls. It leaves `{a + b}`, `{x > 10}` and Python-style `{{...}}` silently printed as text. C1 catches all of them, and its cost (a JSON brace pair in a plain literal) is a compile error with a one-turn repair. Each candidate's verdict is fixed by the literal's own text, so none earns a veto. Only C2's scope-sensitive reading would make compiling depend on other lines.

### prediction

Over 100 one-turn generations of task C:

- **Silently wrong programs from a missing `f` or from misplaced doubled braces:** C3 at least 10%, C2 at least 3%, C1 0%.
- **First-attempt compile errors under C1** (JSON text in a plain literal): at least 30% under the broad reading, with at least 95% repaired in one turn.

### condition

- I would move C1 to object if repairs of its JSON error produced wrong output, for example a single brace dropped, in more than 5% of cases.
- I would move C2 to approve if models' missing-`f` slips were at least 95% bare names or dotted calls.
- I would move C3 to approve only if models never omit the `f` before a hole.

### task-c.hero under each candidate

- **Line 5** `print("total {total}")`: a compile error under C1 and C2. Under C3 it compiles and prints `total {total}`.
- **Line 6** `print("{\"id\": 7, \"ok\": true}")`: under C1 it is a compile error (broad reading) or unchanged (narrow reading). Under C2 and C3 it is unchanged and prints `{"id": 7, "ok": true}`.
- **Line 7** `print(f"done: {total > 10}")`: the same under all three, printing `done: true`.
- **Output:** under C1 and C2 the program does not compile, so it prints nothing. Under C3 it prints `total {total}`, then `{"id": 7, "ok": true}`, then `done: true`. The first line is silently wrong.

## context

I read only `brief.md`, `spec.md`, `task-a.hero`, `task-b.hero` and `task-c.hero` from this directory. Two other things reached my context and were not used in the judgment:

- System-provided session information: the working directory path, platform, date and model identity, and the general instructions of the agent harness.
- A system note stating the user's account email address.

No other file, document or tool output reached my context.
