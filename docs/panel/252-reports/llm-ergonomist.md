# Panel 252, llm-ergonomist: the blind seat's 51 sessions, run and graded

Written 2026-11-19 by the coordinator, who ran `run.sh` and graded the reports by the rows and the
decision rules `docs/panel/252-briefs/llm-ergonomist.md` registered before any run. Every number below
comes from a command run while this file was written; the commands are named beside them.

## How the sessions were run

- **The runner** started at 16:08:22, when the spec-warden's report held `## Registered predictions` with
  lines under it (the coordinator's wait loop polled for it; the runner's own gate checked it again and
  copied the report to `warden-at-start.md`), ran the free preflight, then the 50 graded sessions and the
  verdict session. The key's value was never printed.
- **The free preflight** (a key the API refuses): its `run.json` reads as a failure by the three-field
  test below, as it must; cost 0; the runner went on only because its own scan read the preflight's
  transcript clean.
- **Every paid session** ran with standard input from `/dev/null`, the area as an empty git repository
  of its own, `HOME` inside the area, and `TMPDIR` and `CLAUDE_CODE_TMPDIR` inside it.
- **51 of 51 sessions ended well by three fields**: `subtype` `success`, `is_error` false, and
  `terminal_reason` `completed` (a python reading of every `run.json`). No session was set aside under
  `failed/`; none was capped.
- **The spend: 7.081615 USD** (`tools/spent.py`), within the 10 USD the author approved for this sitting.
- **What reached the sessions, by their transcripts**: the runner's own scan, `scan exit 0 (51
  transcripts read, 0 failed a check; failures by kind: {})`. No transcript under `~/.claude/projects/`
  names the area (`ls | grep -c 252-llm`: 0), and no `cache-break-state` file was left in
  `/private/tmp/claude-501/` (0). **No session is voided**; every `context` answer is in the appendix.

## The grades

The arms by the draw (`key.txt`): `now` y, `q1-c` j, `q2-c` g, `q3-r` f, `q4-r` w, `q5-c` x. The mechanical
half was done by `coord/252-grade/grade.py` (each answer's files under the paths they name, built with the
brief writer's frozen stage 2, `252-briefwriter/tree/heroes-next`, `sources 23c7d2a626b29fd5`, and for task 1
also with its probe `252-briefwriter/q1/heroes-nocycle`, each binary run 3 times under `timeout -s KILL 20`;
its whole output is `coord/252-grade/grade-out.txt`); predictions and words were read by the coordinator.

| task | arm | right | how it was graded | not right, and what was recorded |
|---|---|---|---|---|
| t1 | now | **5 of 5** (frozen) | the three files build and print `26`, exit 0, 3 of 3 | **regraded for Q1-R under the probe: 5 of 5**. **No answer forms the ring**: `area.hero` uses `shapes` in 0 of 5; each writes the area over two `i64`s (`rect_area(w: i64, h: i64)`), not over the face's `Rect` |
| t1 | q1-c | **5 of 5** (frozen) | as above | 0 of 5 form the ring; the same `rect_area(w, h)` |
| t2 | now | **0 of 5** by the row | every one predicts **accepted**, `2` then `1`, a normal end | **regraded for Q2-R: 5 of 5** (accepted, `2`, `1`, normal) |
| t2 | q2-c | **5 of 5** | each predicts the build **refused** at the second `print`, `print(nodes({"leaf": []}))`, quoted, the first line not refused | three give the line as **12** where the task's program has it at 7, each quoting it word for word (`t2-g-2`, `t2-g-4`, `t2-g-5`), graded right on the quote; the same three name the other reading as a conditional, recorded **hedged** |
| t3 | now | **5 of 5** by the rubric | R1 to R4 read from the text; the five also build under the frozen stage 2 and print `3`, `4`, `5`, `green`, `blue` (recorded, not graded) | **the hazard: 5 of 5** hold the list apart from its state (a `RingPos` or `Ring` record, or an index, beside the program's list), so one ring's state can be handed another ring's list and still compile; `t3-y-1` gives its build as conditional on its choice points, recorded **hedged** |
| t3 | q3-r | **5 of 5** by the rubric | one `record Ring<T>` written once, used as `Ring<i64>` and `Ring<str>`; an add that pushes while not full, then overwrites the oldest slot and moves the start; the five lines predicted | none |
| t4 | now | **5 of 5** | build, `u 3`, `t 3`, `f 5`, `l 4`, exit 0, 3 of 3 | **silent wrong: 0 of 5** |
| t4 | q4-r | **5 of 5** | as above | none graded refused: no answer prints a `u8` in a hole or `print` |
| t5 | now | **4 of 5** (frozen) | build, `true`, `false`, exit 0, 3 of 3 | `t5-y-2` writes `1e-9`, refused `error[exponent_literal]`; **regraded for Q5-R** (exponents written out by `t5_digits.py`): **5 of 5** |
| t5 | q5-c | **5 of 5** | as above | no answer writes an exponent |

## The decision rules, as registered

- **Q1, task 1. The refusal does not meet readers**: `now` under the probe 5 against `now` frozen 5, a
  difference of 0. **Q1-C's sentence does not carry**: `q1-c` 5 against `now` 5. Neither fires, so panel
  031's R6 ruling stands on this reading. **What the count cannot see**: no reader formed the ring; all ten
  computed the area over two integers, the workaround the brief's census counts in 32 of Rebel's packages
(carried from `00-shared.md`, not re-run here), so
  the task's design let readers step around the refusal without meeting it, a ceiling of its own kind.
- **Q2, task 2. The spec reads as Q2-R**: 5 of `now`'s 5 predict both lines accepted, at least 4. **Q2-C's
  sentence carries**: 5 of `q2-c`'s 5 right by its row, at least 4. Today's compiler surprises every reader of
  today's spec: each expects the build that Q2-R would give.
- **Q3, task 3. Q3-R does not serve the thesis**: 5 against 5, a difference of 0. **Recorded: 5 of `now`'s
  5 hold an API that accepts a mismatched pair**, so the rule records *"readers write the hazard
  `containers` wrote"*.
- **Q4, task 4. The trap does not meet readers**: 0 silent wrong of 5, under 2. **Q4-R does not serve the
  thesis**: 5 against 5. Turned from silent to loud by construction: 0.
- **Q5, task 5. The refusal does not meet readers**: 1 of `now`'s 5 writes an exponent literal, under 3.
  **Q5-C's sentence does not carry**: 5 against 4, a difference of 1. **Q5-R is not read as needed**: `now`
  regraded 5 against frozen 4, a difference of 1.

## The spec-warden's predictions, scored as registered

From `warden-at-start.md`, the copy the runner made at its start; written at 16:08 by the seat's report.

| prediction | as worded | the count | holds |
|---|---|---|---|
| W1 | at least 3 of `now`'s 5 form the ring, and *"the refusal meets readers"* fires | 0, and does not fire | **no** |
| W2 | *"Q1-C's sentence carries"* does not fire | does not fire | yes |
| W3 | *"the spec reads as Q2-R"* fires | 5 of 5 | yes |
| W4 | *"Q2-C's sentence carries"* fires | 5 of 5 | yes |
| W5 | *"Q3-R serves the thesis"* does not fire, and at least 3 of `now`'s answers hold the mismatched-pair hazard | does not fire; 5 | yes |
| W6 | *"the trap meets readers"* fires (at least 2 silent wrong) | 0 | **no** |
| W7 | *"Q4-R serves the thesis"* does not fire | does not fire | yes |
| W8 | *"the refusal meets readers"* fires (at least 3 write an exponent) | 1 | **no** |
| W9 | *"Q5-R is read as needed"* fires | difference 1 | **no** |
| W10 | *"Q5-C's sentence carries"* fires | difference 1 | **no** |
| W11, W12 | the seat's own refreshes | held, by the seat's report | the seat's |

## What the counts cannot see, recorded and not graded

- **Two tasks met no hazard because the readers stepped around it**: task 1's readers wrote the area over
  integers (no ring to refuse), task 4's took the first letter as text (nine of ten by `slice(from: 0, to: 1)`, one from the word's
characters; no `u8` to print as a number), and
  task 5's wrote the tolerance out in four answers of five. Each rule that needed readers to meet the shape
  read zero or one; whether readers meet it when the program's natural form leads into it (an area over
  the face's `Rect`; a byte compared, then printed) is a question for a task built to lead there.
- **What did meet readers**: task 2's refusal surprises every reader of today's spec, and task 3's
  hazard is written by every one of them.

## The verdict session, recorded whole and grading nothing

Its `verdict`, quoted: *"**approve**"* (over task 3, the generic record, the one task its folder holds).
Its `experiment`, `choice_points`, `argument`, `condition` and `context` are in the appendix.

## Where the evidence is

The blind area, `.claude/worktrees/rebel-scratch/252-llm-ergonomist/` (`run.sh`, `run.out`, `run.log`,
`warden-at-start.md`, `s/preflight/`, `key.txt`, `order.txt`, every session's folder with its `run.json`,
`run.err` and `result.md`, the transcripts under `home/`); the grading's own files,
`.claude/worktrees/rebel-scratch/coord/252-grade/` (`grade.py`, `grade-out.txt`, each session's extracted
files and builds). Both are outside git and go into the sitting's evidence archive.

## Appendix: every session's cost and grade, then its report whole

| session | arm | exit | subtype | cost, USD | grade |
|---|---|---|---|---|---|
| `t1-j-1` | q1-c | 0 | success | 0.1311 | right |
| `t1-j-2` | q1-c | 0 | success | 0.1353 | right |
| `t1-j-3` | q1-c | 0 | success | 0.1287 | right |
| `t1-j-4` | q1-c | 0 | success | 0.1419 | right |
| `t1-j-5` | q1-c | 0 | success | 0.1325 | right |
| `t1-y-1` | now | 0 | success | 0.1318 | right |
| `t1-y-2` | now | 0 | success | 0.1394 | right |
| `t1-y-3` | now | 0 | success | 0.1343 | right |
| `t1-y-4` | now | 0 | success | 0.1322 | right |
| `t1-y-5` | now | 0 | success | 0.1328 | right |
| `t2-g-1` | q2-c | 0 | success | 0.1293 | right |
| `t2-g-2` | q2-c | 0 | success | 0.1235 | right |
| `t2-g-3` | q2-c | 0 | success | 0.1326 | right |
| `t2-g-4` | q2-c | 0 | success | 0.1274 | right |
| `t2-g-5` | q2-c | 0 | success | 0.1149 | right |
| `t2-y-1` | now | 0 | success | 0.1252 | wrong by the row: predicts accepted (right for Q2-R) |
| `t2-y-2` | now | 0 | success | 0.1291 | wrong by the row: predicts accepted (right for Q2-R) |
| `t2-y-3` | now | 0 | success | 0.1326 | wrong by the row: predicts accepted (right for Q2-R) |
| `t2-y-4` | now | 0 | success | 0.1196 | wrong by the row: predicts accepted (right for Q2-R) |
| `t2-y-5` | now | 0 | success | 0.1238 | wrong by the row: predicts accepted (right for Q2-R) |
| `t3-f-1` | q3-r | 0 | success | 0.1738 | right |
| `t3-f-2` | q3-r | 0 | success | 0.1539 | right |
| `t3-f-3` | q3-r | 0 | success | 0.1612 | right |
| `t3-f-4` | q3-r | 0 | success | 0.1658 | right |
| `t3-f-5` | q3-r | 0 | success | 0.1741 | right |
| `t3-y-1` | now | 0 | success | 0.1588 | right |
| `t3-y-2` | now | 0 | success | 0.1667 | right |
| `t3-y-3` | now | 0 | success | 0.1715 | right |
| `t3-y-4` | now | 0 | success | 0.2497 | right |
| `t3-y-5` | now | 0 | success | 0.1642 | right |
| `t4-w-1` | q4-r | 0 | success | 0.1350 | right |
| `t4-w-2` | q4-r | 0 | success | 0.1238 | right |
| `t4-w-3` | q4-r | 0 | success | 0.1308 | right |
| `t4-w-4` | q4-r | 0 | success | 0.1372 | right |
| `t4-w-5` | q4-r | 0 | success | 0.1258 | right |
| `t4-y-1` | now | 0 | success | 0.1145 | right |
| `t4-y-2` | now | 0 | success | 0.1364 | right |
| `t4-y-3` | now | 0 | success | 0.1241 | right |
| `t4-y-4` | now | 0 | success | 0.1403 | right |
| `t4-y-5` | now | 0 | success | 0.1210 | right |
| `t5-x-1` | q5-c | 0 | success | 0.1247 | right |
| `t5-x-2` | q5-c | 0 | success | 0.1230 | right |
| `t5-x-3` | q5-c | 0 | success | 0.1237 | right |
| `t5-x-4` | q5-c | 0 | success | 0.1222 | right |
| `t5-x-5` | q5-c | 0 | success | 0.1147 | right |
| `t5-y-1` | now | 0 | success | 0.1212 | right |
| `t5-y-2` | now | 0 | success | 0.1173 | wrong: writes 1e-9, refused (right once regraded) |
| `t5-y-3` | now | 0 | success | 0.1229 | right |
| `t5-y-4` | now | 0 | success | 0.1198 | right |
| `t5-y-5` | now | 0 | success | 0.1153 | right |
| `verdict` | verdict | 0 | success | 0.2504 | not graded |

The sum of the 51 costs in `run.log`, the 50 graded sessions and the verdict: 7.081615 USD.

### `t1-j-1`, q1-c

~~~~~ markdown
I can't write `report.md`: the only tool I have here reads files. As the brief allows, the whole report is below.

# report.md

## answer

**Files.** `main.hero` stays as the task gives it. I wrote these two:

`shapes.hero`
```
use shapes/internal/area

record Rect
    w: i64
    h: i64

function total(rs: [Rect]) -> i64
    t @= 0
    for r in rs
        t @ t + area.of(w: r.w, h: r.h)
    return t
```

`shapes/internal/area.hero`
```
function of(w: i64, h: i64) -> i64
    return w * h
```

**Does `heroes build main.hero` accept the three files?** Yes. Here is why:
- **The `use` path:** every `use` starts at the folder of the file being compiled (`main.hero`'s folder), even inside `shapes.hero`. So `shapes/internal/area` finds `shapes/internal/area.hero` and binds the name `area`.
- **Access to `internal`:** the part before `internal` is `shapes`, so `shapes.hero` is allowed to name that module. `main.hero` never names it.
- **No cycle:** `area.hero` uses nothing.
- **Named arguments:** the two parameters of `of` share the type `i64`, so the call must name them. It does: `area.of(w: …, h: …)`.
- **No unused names:** `t`, `r` and `rs` are all used, and `t` is re-bound.
- **Return on every path:** both functions end in `return`.
- **Only two declarations:** `shapes.hero` declares only `Rect` and `total`. A `use` is not a declaration.

**Every line the program prints:**
```
26
```
(2·3 + 4·5 = 6 + 20 = 26)

**How it ends:** normally, after printing that one line.

## choice_points

1. **Where `Rect` lives for the area function.** I made `area.of` take two `i64` values instead of a `Rect`.
   - The other choice: if `area.hero` wrote `use shapes` to take a `shapes.Rect`, the modules would form a cycle. The spec says a module under `internal` cannot use the module that names it, so **the compiler refuses** it.
   - Declaring a separate `Rect` inside `area.hero` would make a different type. Passing `r` to it would then be **refused by the compiler** as a type mismatch.
2. **How the `use` path is written in `shapes.hero`.** I wrote `use shapes/internal/area`, counted from the folder of the compiled file.
   - The other choice: `use internal/area`, counted from `shapes.hero`'s own place. It would look for `internal/area.hero` beside `main.hero`, which doesn't exist, so **the compiler refuses** it.
3. **Named arguments in the call.** I wrote `area.of(w: r.w, h: r.h)`.
   - The other choice: `area.of(r.w, r.h)` leaves out the names that two same-typed parameters require, so **the compiler refuses** it.
4. **Loop or `fold` for the sum.** I used a cell and a `for` loop.
   - The other choice: `rs.fold(0, f)` needs a top-level function of `(acc: i64, item: Rect)`. It would have to live in `shapes.hero`, which the task says declares nothing besides `Rect` and `total`. The compiler would accept it, but **it breaks the task's rule**. Putting that function in `area.hero` instead brings back the cycle from point 1, so **the compiler refuses** it.
5. **The name of the area function.** I chose `of`, so the call reads `area.of`.
   - The other choice: calling it `area`, so the call reads `area.area(…)`, would also compile. The function name only appears qualified, so nothing is shadowed. Same output, so this choice only affects readability.
6. **The type of the sum cell.** I let `t @= 0` take its type from the literal, which gives `i64` by default.
   - The other choice: annotating it, as in `t: i64 @= 0`, gives the same program.
   - Making it a narrower type such as `i32` would make `t + area.of(...)` mix two integer widths, so **the compiler refuses** it.

## prediction

The compiler accepts the program. It prints exactly one line, `26`, and ends normally. There is no abort.

## context

Besides the files in this directory (`brief.md`, `task.md`, `heroes-spec.md`), my context had only the general system setup. That setup said I'm a Claude agent working in this folder, and gave the date and the tool list. No other project instruction, rule, contract, memory or document reached me. The folder's path (`heroes-lang/.claude/worktrees/...`) was visible, but I opened nothing outside this directory.
~~~~~

### `t1-j-2`, q1-c

~~~~~ markdown
I couldn't write `report.md` because I only have a file-reading tool here. As the brief allows, the full report is below.

# report.md

## answer

**Files.** `main.hero` stays as given. The two files I wrote, in full:

`shapes.hero`
```
use shapes/internal/area

record Rect
    w: i64
    h: i64

# The sum of the rectangles' areas.
function total(rs: [Rect]) -> i64
    acc @= 0
    for r in rs
        acc @ acc + area.rect_area(w: r.w, h: r.h)
    return acc
```

`shapes/internal/area.hero`
```
# The area of one rectangle, given its two sides.
function rect_area(w: i64, h: i64) -> i64
    return w * h
```

**Does `heroes build main.hero` accept the three files?** Yes.

**Every line the program prints:** `26`. That is 2·3 + 4·5 = 6 + 20.

**How it ends:** normally, after printing that one line.

## choice_points

1. **What `rect_area` takes as input.** I chose two `i64` sides instead of a `Rect`.
   - `area.hero` would need `use shapes` to name `Rect`. But `shapes.hero` already uses `area`, and §1 says "a module under `internal` cannot use the module that names it" because modules may not form a cycle.
   - The other choice (`use shapes` in `area.hero` and `rect_area(r: shapes.Rect)`) would be refused by the compiler as a cycle.
   - Moving `Rect` into `area.hero` would break the task, because `shapes.hero` must declare `Rect`.

2. **Labels at the call.** Both parameters are `i64`, so §9 makes labels mandatory. I wrote `area.rect_area(w: r.w, h: r.h)`, in the declared order.
   - `area.rect_area(r.w, r.h)` would be refused by the compiler.
   - Labels in the other order (`h:` before `w:`) would break "Labels keep the declared order" and, as I read it, would be refused.

3. **Where the `use` path starts.** §1 says a `use` starts at the directory of the file you compile, here `main.hero`'s folder. So `use shapes/internal/area` in `shapes.hero` finds `shapes/internal/area.hero`.
   - That folder is also `shapes.hero`'s own folder, so reading the path relative to `shapes.hero` gives the same file. Nothing changes either way.
   - Writing `use internal/area` would be refused: no such module exists from the root, and the `internal` rule only allows the path to be named from `shapes`.

4. **Who may name `internal`.** The part of the path before `internal` is `shapes`, so `shapes.hero` may use the module. `main.hero` may not, and it doesn't. Since `main.hero` stays as given, there was no other choice to make here.

5. **Loop or `fold`.** I used a mutable cell `acc @= 0`, updated with `@` in a `for` loop.
   - The literal `0` defaults to `i64`, which matches the return type.
   - `rs.fold(0, f)` would need a separate top-level function with parameters named `acc` and `item`. That function would be another declaration in `shapes.hero`, which the task forbids ("It declares nothing else"). It also adds risk around argument labels.
   - Any binding nothing re-binds would be refused as an unused cell. `acc` is re-bound in the loop, so it's fine.

6. **Names.**
   - I avoided naming the function `area`, which would give `area.area`. That might well compile, but it reads ambiguously.
   - I avoided naming the cell `sum`, in case it clashes with something. The spec lists no built-in called `sum`, so either name would probably compile.
   - Shadowing is an error, so no local is named `area` or `shapes`.

7. **Doc comments.** These are optional. Leaving them out changes nothing.

## prediction

- **Compiler:** accepts the program.
  - Every binding is used, there are no cycles, the `internal` rule is respected, the labels are present, and every type is `i64`.
  - `shapes.hero` declares only `Rect` and `total`. A `use` line is not a declaration.
- **Output:** one line, `26`.
- **Ending:** normal, with no abort. 26 is far from any overflow.

## context

The only files that reached me were the ones in this directory: `brief.md`, `task.md` and `heroes-spec.md`. No other instruction, rule, contract, memory or document did. The one thing outside them was the system setup itself, which gave the working directory and the date.
~~~~~

### `t1-j-3`, q1-c

~~~~~ markdown
I only have a file-reading tool, so I could not create `report.md`. The brief says to give the whole report as the final answer in that case, so here it is.

# report.md

## answer

**Question 1: the two files.**

`shapes.hero`:

```
use shapes/internal/area

record Rect
    w: i64
    h: i64

function total(rs: [Rect]) -> i64
    acc @= 0
    for r in rs
        acc @ acc + area.of(w: r.w, h: r.h)
    return acc
```

`shapes/internal/area.hero`:

```
function of(w: i64, h: i64) -> i64
    return w * h
```

`main.hero` (given, unchanged):

```
use shapes

function main()
    rs = [shapes.Rect(w: 2, h: 3), shapes.Rect(w: 4, h: 5)]
    print(shapes.total(rs))
```

**Question 2: does `heroes build main.hero` accept the three files?** Yes.
- The `internal` rule allows the `use`. In the path `shapes/internal/area`, `internal` comes after the first part. The part before it, `shapes`, names `shapes.hero`, so `shapes.hero` may use this module.
- The `use` finds the file. A `use` starts at the directory of the compiled file, which is where both `main.hero` and `shapes.hero` sit, so it resolves to `shapes/internal/area.hero`.
- There is no cycle, because `area.hero` uses nothing.

**Question 3: every line the program prints.** One line: `26` (2·3 + 4·5).

**Question 4: how it ends.** Normally, when `main` returns.

## choice_points

1. **What the area function takes.** I passed it two `i64` values instead of a `Rect`.
   - `Rect` lives in `shapes.hero`. To take a `Rect`, `area.hero` would need `use shapes`.
   - That makes a cycle, which section 1 forbids: "a module under `internal` cannot use the module that names it." The compiler would refuse it.
   - Declaring a second `Rect` in `area.hero` would make a different type. `total` could not pass a `shapes.Rect` to it, so the compiler would refuse that too.
2. **Named arguments `w:` and `h:`.** Both parameters are `i64`, so section 9 makes names mandatory at the call. Writing `area.of(r.w, r.h)` would be refused by the compiler.
3. **The function's name.** I called it `of`, not `area`.
   - Calling it `area` would be read as `area.area(...)`. That is probably legal: the `use` binds `area` only in `shapes.hero`, and declarations are always reached qualified.
   - But the shadowing rule is unclear inside the module itself, so I avoided the risk. If the compiler did treat it as shadowing, it would refuse the program.
4. **Local names in `total`.** I used `acc` rather than `area` or `sum`.
   - A local named `area` would shadow the `use` binding, and the compiler would refuse it.
   - `sum` is not listed as a built-in, but `acc` avoids any doubt.
5. **Loop or `fold`.** I used a `for` loop with a mutable cell.
   - A `fold` needs a top-level function value whose parameters are named `acc` and `item`. That would mean a second function in `shapes.hero`, which the task says must declare nothing else (or putting it in `area.hero`).
   - The answer would be the same; the loop just avoids the risk.
   - The cell `acc` is re-bound inside the loop, so it does not trigger the "cell nothing re-binds" error.
6. **The `0` in `acc @= 0`.** It has no context, so it defaults to `i64` (section 2). That matches the types of `w * h` and of the return type, so nothing is mixed.
7. **How `total` takes `rs`.** I took it as a plain copy, not `lent`. Either is accepted and gives the same result.
8. **Whether the module is visible.** The specification has no export keyword, so I took every declaration to be visible to a module that `use`s it. I added nothing to mark visibility.

## prediction

- **Compiler:** accepts the program.
- **Output, in order:**
  ```
  26
  ```
- **End:** normally, with no abort. 2·3 + 4·5 = 26, which is far from overflowing `i64`.

## context

Only `brief.md`, `task.md` and `heroes-spec.md` from this directory reached my context. Besides them I had my general system instructions (I am an agent with a file-reading tool) and the working-directory path and date. No other rule, contract, memory or project document reached my context.
~~~~~

### `t1-j-4`, q1-c

~~~~~ markdown
I could not write `report.md` because I have no tool that writes files; I can only read them. The brief says that in this case the whole report goes in my final answer, so here it is.

# report.md

## answer

**Question 1: each file I write, whole, under its path.**

`shapes.hero`:

```
use shapes/internal/area

record Rect
    w: i64
    h: i64

function total(rs: [Rect]) -> i64
    acc @= 0
    for r in rs
        acc @ acc + area.rect_area(w: r.w, h: r.h)
    return acc
```

`shapes/internal/area.hero`:

```
function rect_area(w: i64, h: i64) -> i64
    return w * h
```

`main.hero` (given, unchanged):

```
use shapes

function main()
    rs = [shapes.Rect(w: 2, h: 3), shapes.Rect(w: 4, h: 5)]
    print(shapes.total(rs))
```

**Question 2: does `heroes build main.hero` accept the three files?** Yes.
- **Paths:** `use shapes/internal/area` starts at the folder of `main.hero`, which is also the folder of `shapes.hero`. So it finds `shapes/internal/area.hero`. The part before `internal` is `shapes`, which names `shapes.hero`, so `shapes.hero` is allowed to use that module.
- **No cycle:** `area.hero` uses nothing.
- **Visibility:** `main.hero` names only `shapes`, so it needs no `use` of `area`.
- **Names:** every binding is used, and the cell `acc` is re-bound. Nothing shadows the name `area` that the `use` binds.
- **Labels:** `rect_area` has two `i64` parameters, so the call names both, and it does.
- **Return:** `total` returns on its only path.

**Question 3: every line the program prints:**

```
26
```

**Question 4: how it ends.** It ends normally after that one line, with exit status 0. The sum is 2·3 + 4·5 = 6 + 20 = 26, which does not overflow.

## choice_points

1. **What `rect_area` takes.** I chose two `i64` values (`w`, `h`).
   - The other choice was `rect_area(r: shapes.Rect)`. That needs `use shapes` inside `area.hero`. Since `shapes.hero` uses `area`, that is a cycle, and the spec says a module under `internal` cannot use the module that names it. The compiler refuses it.
   - Declaring `Rect` inside `area.hero` would break the task's rule that `shapes.hero` declares `Rect`.
2. **Named arguments at the call.** I wrote `area.rect_area(w: r.w, h: r.h)`. The two parameters share a type, so labels are required; the positional call `area.rect_area(r.w, r.h)` is refused. Swapping the labels' order is also refused, because labels keep the declared order.
3. **The `use` path in `shapes.hero`.** I wrote `use shapes/internal/area`, starting from the folder of the compiled file.
   - `use internal/area` would look for `./internal/area.hero`, which does not exist, so it is refused.
   - Putting the module at `./internal/area.hero` would make the path before `internal` empty. That breaks the task's layout, and under the `internal` rule I can't be sure `shapes` could use it.
4. **The function's name.** I chose `rect_area`. The name `area` would also work, written `area.area(...)`, because the module name is only bound in `shapes.hero` and the function is always written qualified. The risk is a local binding named `area` in `shapes.hero`: that is a shadowing compile error. `rect_area` avoids the confusion.
5. **How to add up the areas.** I used a loop with a mutable cell.
   - The other choice was `rs.fold(0, f)`. It needs a top-level helper such as `function add_area(acc: i64, item: Rect) -> i64` in `shapes.hero`. That would put a second function in `shapes.hero`, which the task forbids ("declares nothing else"). It would give the same 26.
   - Writing `acc = 0` instead of `acc @= 0` is refused: an `=` binding cannot be re-bound with `@`.
6. **The starting value's type.** `acc @= 0` takes `i64` by default, which matches the return type. Writing `acc: i32 @= 0` would make `return acc` a type mismatch, which is refused.
7. **Where the multiplication happens.** It is in `area.hero`, as the task requires. Doing `r.w * r.h` in `total` would also compile and print 26. But then `area.hero`'s function would go unused (that is not an error, since only unused bindings are errors), and the task would not be met.

## prediction

- **Compiler:** accepts all three files.
- **Output:** one line, `26`.
- **End:** normal, exit status 0, with no abort.

## context

- Besides `brief.md`, `task.md` and `heroes-spec.md`, I read no file and no document.
- My system prompt gave me the working directory path and the date, and said I am an agent built on the Claude Agent SDK. It held no rules about the Heroes language.
- No other instruction, rule, contract or memory about the language reached my context.
~~~~~

### `t1-j-5`, q1-c

~~~~~ markdown
I only have a file-reading tool here, so I couldn't create `report.md`. As the brief allows, the whole report is below.

# report.md

## answer

**Files.** `main.hero` stays as given. The other two files:

`shapes.hero`
```
use shapes/internal/area

record Rect
    w: i64
    h: i64

function total(rs: [Rect]) -> i64
    t @= 0
    for r in rs
        t @ t + area.rect_area(w: r.w, h: r.h)
    return t
```

`shapes/internal/area.hero`
```
function rect_area(w: i64, h: i64) -> i64
    return w * h
```

**Does `heroes build main.hero` accept the three files?** Yes.
- `main.hero` uses `shapes`, which resolves to `shapes.hero`.
- `shapes.hero` uses `shapes/internal/area`. Under §1, a path with `internal` after its first part can only be named by the module the earlier part names (here `shapes`) and modules under it. So `shapes.hero` is allowed to use it.
- `area.hero` uses nothing, so there is no cycle.
- `shapes.hero` declares only `Rect` and `total`. A `use` is not a declaration under the grammar `File = { Use | Declaration }`.

**What it prints:** one line, `26` (2·3 + 4·5 = 6 + 20).

**How it ends:** normally.

## choice_points

1. **What the area function takes.** I gave it two `i64` values (`w`, `h`), not a `Rect`. `Rect` is declared in `shapes.hero`. For `area.hero` to take a `Rect`, it would have to `use shapes`. That makes a cycle, and §1 forbids it ("a module under `internal` cannot use the module that names it"), so the compiler would refuse it. The other option, declaring a second record inside `area.hero`, would need a conversion in `total` and gains nothing.
2. **Named arguments at the call.** Both parameters are `i64`, so §9 makes the names mandatory. I wrote `area.rect_area(w: r.w, h: r.h)`. Writing `area.rect_area(r.w, r.h)` would be refused by the compiler.
3. **How the `use` path in `shapes.hero` is written.** §1 says a `use` starts at the directory of the file you compile. That is `main.hero`'s folder, which is also where `shapes.hero` sits, so I wrote `shapes/internal/area`. Writing `use internal/area` (as if the path were relative to the `shapes/` folder) would fail to resolve, and the compiler would refuse it.
4. **Binding name.** I kept the default binding `area` and wrote no `as` alias. An alias would work the same. Choosing a name already bound in the file would be shadowing, which the compiler refuses.
5. **Function name `rect_area` rather than `area`.** This is only for readability. The call `area.area(...)` is qualified, so naming the function `area` would most likely compile too.
6. **Loop instead of `fold`.** `fold` needs a top-level function value. That would have to live in `shapes.hero`, which would break "declares nothing else", or in `area.hero` with parameters named `acc`/`item` that take a `Rect`, which brings back the cycle from point 1. I used a `for` loop with a cell, `t @= 0`. The cell is re-bound inside the loop, so it doesn't trigger the "cell nothing re-binds" error. Writing `t = 0` (immutable) and then `t @ ...` would be refused.
7. **Type of the `0` literal.** Nothing in the context asks for a type, so it defaults to `i64`, which matches the return type. No choice is needed here.
8. **Overflow.** `w * h` and the sum would abort on overflow. With these inputs they stay small, so nothing changes.

## prediction

- **Compiler:** accepts all three files.
- **Output:**
  ```
  26
  ```
- **Ending:** normal, with no abort.

## context

Only the files in this directory reached me: `brief.md`, `task.md` and `heroes-spec.md`. The only other text in my context was the system prompt for this session (my role as an agent, the working directory and the date). No project rules, memory or other documents about the language reached me.
~~~~~

### `t1-y-1`, now

~~~~~ markdown
I can't write files with the tools available to me, so I'm giving the whole report here as my final answer, as the brief allows.

# report.md

## answer

**`shapes.hero`**
```
use shapes/internal/area

# A rectangle, `w` wide and `h` high.
record Rect
    w: i64
    h: i64

# The sum of the areas of `rs`.
function total(rs: [Rect]) -> i64
    sum @= 0
    for r in rs
        sum @ sum + area.rect_area(w: r.w, h: r.h)
    return sum
```

**`shapes/internal/area.hero`**
```
# The area of a rectangle `w` wide and `h` high.
function rect_area(w: i64, h: i64) -> i64
    return w * h
```

**`main.hero`** (unchanged)
```
use shapes

function main()
    rs = [shapes.Rect(w: 2, h: 3), shapes.Rect(w: 4, h: 5)]
    print(shapes.total(rs))
```

**Does `heroes build main.hero` accept the three files?** Yes, I predict it does.

**What does the program print?** One line: `26` (2·3 + 4·5).

**How does it end?** Normally.

## choice_points

1. **What `rect_area` takes: two `i64`s, not a `Rect`.**
   - **My choice:** `area.hero` uses no other module. This keeps the two modules from using each other.
   - **The other choice:** `rect_area(r: shapes.Rect)` would need `use shapes` inside `shapes/internal/area.hero`. That creates a cycle: `shapes` uses `area`, and `area` uses `shapes`. The spec never says whether modules may use each other in a cycle. It also doesn't clearly say whether `shapes/internal/area` counts as a module "under" `shapes`. If cycles are refused, the compiler refuses the program. If they're allowed, it prints the same `26`.
2. **Named arguments `w:` and `h:` at the call.** Both parameters are `i64`, so §9 makes the names mandatory. Writing `area.rect_area(r.w, r.h)` would be refused by the compiler.
3. **Using `shapes/internal/area` from `shapes.hero`.**
   - **My reading of §1:** the part before `internal` is `shapes`, which names `shapes.hero`. So `shapes.hero` may use this path. `main.hero` doesn't name the path, so the rule doesn't touch it.
   - **Where the path starts:** a `use` starts at the directory of the compiled file (`main.hero`). `shapes.hero` is in the same folder, so the path resolves the same way under either reading.
   - **The other reading:** if `internal` were readable only by modules strictly under `shapes/`, the compiler would refuse the program.
4. **The `use` line in `shapes.hero`.** Under the grammar, a `use` line is not a `Declaration`, so `shapes.hero` still "declares nothing else". If a `use` counted as a declaration, the task couldn't be done at all, because `total` has to reach `area` somehow.
5. **The function's name: `rect_area`, not `area`.** A `use` binds `area` for the whole file, and shadowing is a compile error. `area.area` would probably also compile, because the function's name is never bound in `shapes.hero`. I avoided relying on that.
6. **A loop with a mutable cell, not `fold`.**
   - **My choice:** a `for` loop that updates `sum`. The cell is re-bound, so it isn't the "cell nothing re-binds" error.
   - **The other choice:** `rs.fold(0, f)` needs a top-level helper whose parameters are named `acc` and `item`. That helper would be a third declaration in `shapes.hero`, which the task forbids, so the task would be failed.
7. **Leaving `rs` without the `lent` mark.** Without `lent` the parameter is a copy, and the result is the same. Adding `lent` would also compile and print `26`.
8. **Overflow.** 6 + 20 is well within `i64`, so there is no abort.

## prediction

- **Compiler:** accepts the program.
- **Output:**
```
26
```
- **Ending:** normal; `main` returns and there is no abort.

## context

The only files that reached my context were `brief.md`, `task.md` and `heroes-spec.md` from this directory. The only other context was the system/harness framing: the working-directory path and the date. No other rule, memory or document about Heroes reached me.
~~~~~

### `t1-y-2`, now

~~~~~ markdown
I only have a file-reading tool here, so I couldn't write `report.md`. As the brief allows, the whole report follows.

# report.md

## answer

**1. The two files**

`shapes.hero`:

```
use shapes/internal/area

# A rectangle, `w` wide and `h` high.
record Rect
    w: i64
    h: i64

# The sum of the areas of `rs`.
function total(rs: [Rect]) -> i64
    sum @= 0
    for r in rs
        sum @ sum + area.rect_area(w: r.w, h: r.h)
    return sum
```

`shapes/internal/area.hero`:

```
# The area of a rectangle `w` wide and `h` high.
function rect_area(w: i64, h: i64) -> i64
    return w * h
```

`main.hero` (given, unchanged):

```
use shapes

function main()
    rs = [shapes.Rect(w: 2, h: 3), shapes.Rect(w: 4, h: 5)]
    print(shapes.total(rs))
```

**2. Does `heroes build main.hero` accept the three files?** Yes.
- **The `internal` path is allowed.** In `shapes/internal/area`, the part before `internal` is `shapes`, which names the module `shapes.hero`. That module may name the path, so `shapes.hero`'s `use` is allowed. `main.hero` never names it.
- **Each `use` resolves.** Every `use` starts at the directory of `main.hero`, so the path reaches `./shapes/internal/area.hero` and binds `area`.
- **The call is well formed.** Both parameters of `rect_area` are `i64`, so named arguments are required, and the call gives them.
- **No unused names.** The cell `sum` is re-bound, and every binding and parameter is used.
- **No shadowing.** `area` is bound only once in `shapes.hero`.
- **Nothing else is declared.** A `use` is not a `Declaration` in the grammar, so `shapes.hero` declares only `Rect` and `total`.

**3. What the program prints:** one line, `26` (2·3 + 4·5 = 6 + 20).

**4. How it ends:** normally.

## choice_points

1. **What the area function takes.** I chose `rect_area(w: i64, h: i64)`, called with the fields. The other choice was `rect_area(r: shapes.Rect)`, which needs `use shapes` inside `area.hero`. That makes `shapes` and `area` use each other, and the spec says nothing about whether modules may use each other. If cycles are refused, the compiler refuses the program. If they are allowed, it runs and prints `26` too. I avoided the cycle because the spec doesn't settle it.
2. **Named arguments at the call.** I wrote `area.rect_area(w: r.w, h: r.h)`. Leaving the names out (`rect_area(r.w, r.h)`) breaks the rule that two parameters of one type need named arguments, so the compiler refuses it.
3. **Function name.** I used `rect_area`, not `area`. A function named `area` would still be reached as `area.area(...)`, and the spec doesn't clearly forbid that. But inside `shapes.hero` no other binding may be called `area`, because the `use` takes that name for the whole file (shadowing is a compile error). That is why the sum is called `sum`, not `area`.
4. **Loop or `fold`.** I used a mutable cell and a `for` loop. `rs.fold(0, f)` needs a named top-level function, since there are no anonymous functions. Putting it in `shapes.hero` breaks "declares nothing else", so the program would be wrong by the task's terms. Putting it in `area.hero` instead would work.
5. **The `use` path.** I wrote `use shapes/internal/area`, starting from `main.hero`'s directory. A relative form like `use internal/area` would point at `./internal/area.hero`, which doesn't exist, so the compiler refuses it.
6. **Doc comments.** I added them above the declarations. They are optional, and leaving them out changes nothing.

## prediction

- **Compiler:** accepts the program.
- **Output:**
  ```
  26
  ```
- **Ending:** normally; no abort.

## context

Nothing reached my context except the files in this directory: `brief.md`, `task.md` and `heroes-spec.md`. The only other input was the working-directory path and date given by the environment. No other instruction, rule, contract, memory or document reached it.
~~~~~

### `t1-y-3`, now

~~~~~ markdown
I couldn't write `report.md` because I can only read files here, not create them. Following the brief's fallback, here is the whole report.

# report.md

## answer

**Files.** `main.hero` stays as the task gives it. I wrote the other two files.

`shapes.hero`:
```
use shapes/internal/area

record Rect
    w: i64
    h: i64

function total(rs: [Rect]) -> i64
    sum @= 0
    for r in rs
        sum @ sum + area.rect_area(w: r.w, h: r.h)
    return sum
```

`shapes/internal/area.hero`:
```
function rect_area(w: i64, h: i64) -> i64
    return w * h
```

`main.hero` (unchanged):
```
use shapes

function main()
    rs = [shapes.Rect(w: 2, h: 3), shapes.Rect(w: 4, h: 5)]
    print(shapes.total(rs))
```

**Does `heroes build main.hero` accept the three files?** Yes.
- **Path:** a `use` starts at the directory of the file you compile, so `shapes/internal/area` resolves to `shapes/internal/area.hero` beside `main.hero`.
- **`internal` rule:** the path before `internal` is `shapes`, which names `shapes.hero`. So `shapes.hero` may name the internal module, and `main.hero` doesn't try to.
- **Unused-binding rules:** every binding is used, and the cell `sum` is re-bound.
- **Named arguments:** the two `i64` parameters share a type, so the call names them, in declared order.
- **What `shapes.hero` declares:** only `Rect` and `total`. A `use` is not a declaration.

**What does the program print?** One line: `26` (2·3 + 4·5).

**How does it end?** Normally: `main` returns, nothing aborts, and there are no leases or handles.

## choice_points

1. **What the area function takes.** I gave it two `i64`s (`w`, `h`), not a `Rect`. Taking a `Rect` would need `use shapes` inside `area.hero`, while `shapes.hero` already uses `area`. That makes a cycle between modules, and the spec doesn't say whether cycles are allowed. The other choice would either be refused by the compiler or work; I can't tell which from the spec. Mine has no cycle. The cost is that the "area of one rectangle" is computed from its two fields rather than the record.
2. **Named arguments.** Because the two parameters share type `i64`, I named them at the call: `rect_area(w: r.w, h: r.h)`. A positional call `rect_area(r.w, r.h)` would be refused by the compiler. Writing the labels in the order `h:` then `w:` would also be refused, because labels keep the declared order.
3. **Function name.** I called it `rect_area`, not `area`. `area.area(...)` would most likely also compile, since `area.hero` doesn't bind the name `area` itself. I avoided it to stay clear of the shadowing rule.
4. **Summing.** I used a loop with a mutable cell, not `fold` with a top-level helper function. A `fold` would need an extra function in `shapes.hero`, which breaks "declares nothing else", or in `area.hero`. It would give the same answer.
5. **Local names.** I called the accumulator `sum`, not `total`. Naming it `total` inside the function `total` risks the compiler refusing it as shadowing.
6. **How `total` takes its argument.** `rs: [Rect]` is a plain copy, not `lent`. Either gives the same output; `lent` would only avoid copying the array.

## prediction

- **Compiler:** accepts the program.
- **Output:** one line, `26`.
- **End:** normally, with no abort.

## context

Besides the files in this directory (`brief.md`, `task.md`, `heroes-spec.md`), only the agent-environment system prompt reached my context: the Claude Agent SDK description, the working directory path and the date. No other rule, contract, memory or document did.
~~~~~

### `t1-y-4`, now

~~~~~ markdown
I couldn't write `report.md` because my only tool here reads files and can't write them. As the brief allows, the whole report is below.

# report.md

## answer

**Files.** `main.hero` stays as given. I wrote the other two.

`shapes.hero`:
```
use shapes/internal/area

record Rect
    w: i64
    h: i64

function total(rs: [Rect]) -> i64
    sum @= 0
    for r in rs
        sum @ sum + area.of(w: r.w, h: r.h)
    return sum
```

`shapes/internal/area.hero`:
```
function of(w: i64, h: i64) -> i64
    return w * h
```

**Does `heroes build main.hero` accept the three files?** Yes, I predict it does.
- **The `internal` rule.** Only the module named by the path before `internal` (`shapes`, which is `shapes.hero`) and the modules under it may name `shapes/internal/area`. `shapes.hero` is that module, so its `use` is allowed. `main.hero` names only `shapes`.
- **Path lookup.** A `use` starts at the directory of the file being compiled. That is the folder holding `main.hero` and `shapes.hero`, so `shapes/internal/area` resolves to `shapes/internal/area.hero`.
- **Name binding.** The `use` binds `area` in `shapes.hero`. Nothing else in that file takes that name.
- **Named arguments.** `of` has two `i64` parameters, so its call must name both. It does: `area.of(w: …, h: …)`.
- **Nothing extra declared.** `shapes.hero` declares only `Rect` and `total`. The grammar treats a `use` as something other than a declaration.
- **Nothing unused or unwritten.** Every binding and parameter is used. The cell `sum` is re-bound inside the loop.
- **Literal type.** `0` takes the type `i64`.

**Every line the program prints:** `26`, because 2·3 + 4·5 = 6 + 20.

**How it ends:** normally.

## choice_points

1. **What the helper takes.**
   - **My choice:** `of(w: i64, h: i64)`. This way `area.hero` doesn't need `Rect` and doesn't `use shapes`.
   - **The other choice:** `of(r: shapes.Rect)` together with `use shapes` in `area.hero`. That makes the two modules use each other. The spec only covers declaration order *within* a module, not whether modules may use each other in a cycle. That version might be refused by the compiler. If it were accepted, it would print the same `26`.
2. **Named arguments at the call.**
   - **My choice:** `area.of(w: r.w, h: r.h)`.
   - **The other choice:** the unnamed call `area.of(r.w, r.h)` is refused by the compiler, because two parameters share a type.
3. **The helper's name.**
   - **My choice:** `of`, called as `area.of`.
   - **The other choice:** naming it `area` (called as `area.area`) should also be accepted. The module name `area` is only bound inside `shapes.hero`, not inside `area.hero`.
   - What would be refused: declaring anything named `area` inside `shapes.hero`, because that shadows the `use` binding.
4. **How the sum is computed.**
   - **My choice:** a mutable cell and a `for` loop.
   - **The other choice:** `fold` would need a separate top-level function to pass in, because the language has no anonymous functions. Adding it to `shapes.hero` would break "declares nothing else". Put in `area.hero`, it would give the same `26`.
   - What would be refused: declaring the cell `sum @= 0` and never re-binding it.
5. **The `use` path in `shapes.hero`.**
   - **My choice:** `shapes/internal/area`.
   - **Other choices:** `use internal/area` would look for the module at `./internal/area.hero`, which doesn't exist, so the compiler refuses it. Any module outside `shapes` that names this path is also refused, because of the `internal` rule.

## prediction

- **Compiler:** accepts the program.
- **Output:** one line, `26`.
- **Ending:** normal, with no abort.

## context

No instruction, rule, contract, memory or document from outside this directory reached my context. I read only `brief.md`, `task.md` and `heroes-spec.md`.
~~~~~

### `t1-y-5`, now

~~~~~ markdown
I could not write `report.md` because my only tool reads files and cannot create them. As the brief says for that case, the whole report follows.

# report.md

## answer

**Files**

`shapes.hero`:
```
use shapes/internal/area

record Rect
    w: i64
    h: i64

# The sum of the rectangles' areas.
function total(rs: [Rect]) -> i64
    sum @= 0
    for r in rs
        sum @ sum + area.of(w: r.w, h: r.h)
    return sum
```

`shapes/internal/area.hero`:
```
# The area of one rectangle, given its width and height.
function of(w: i64, h: i64) -> i64
    return w * h
```

`main.hero` (unchanged, as given):
```
use shapes

function main()
    rs = [shapes.Rect(w: 2, h: 3), shapes.Rect(w: 4, h: 5)]
    print(shapes.total(rs))
```

**Questions, in order**

1. **Does `heroes build main.hero` accept the three files?** Yes. Section 1 says a path with `internal` after its first part may be named only by the module named by the path before that part, and by the modules under it. For `shapes/internal/area`, that module is `shapes`, which is `shapes.hero`. So `shapes.hero` may `use` it. `main.hero` never names it. No binding goes unused, nothing is shadowed, and no cell goes un-rebound (`sum` is rebound).
2. **Every line it prints:** one line, `26` (2·3 + 4·5).
3. **How it ends:** normally.

## choice_points

1. **What the internal function takes.** I made it take two `i64`s, `of(w: i64, h: i64)`, not a `Rect`.
   - The other way: `of(r: Rect)` would need `area.hero` to `use shapes` to name `shapes.Rect`. Then `shapes` and `area` would import each other. The spec does not say whether a cycle between modules is allowed. It says only that mutual recursion needs no forward declarations, and that seems to apply within a module.
   - If cycles are refused, the compiler would reject that program. Taking plain numbers avoids the question.
2. **Named arguments at the call.** Both parameters share the type `i64`, so section 9 makes names mandatory. I wrote `area.of(w: r.w, h: r.h)`.
   - The other way: calling with positional arguments, `area.of(r.w, r.h)`, would be refused by the compiler.
3. **The function's name.** I called it `of`, not `area`.
   - `area` would have been legal inside `area.hero`, because no `use` binds that name there. The call would read `area.area(...)`.
   - I chose `of` to keep any doubt about shadowing out of the program. Inside `shapes.hero`, a local declaration named `area` would clash with the `use` and be refused.
4. **Where the `use` path inside `shapes.hero` starts.** The spec says a `use` starts at the directory of the file being compiled.
   - Here that directory and the directory of `shapes.hero` are the same folder, so both readings find `shapes/internal/area.hero`. This choice changes nothing.
5. **How to sum.** I used a mutable cell and a `for` loop.
   - The other way: `fold` with a named top-level helper would give the same answer, 26.
   - The loop needs no extra declaration, which matters because `shapes.hero` must declare nothing besides `Rect` and `total`.
6. **The type of `sum @= 0`.** With nothing else to go on, a literal is `i64` (section 2). That matches `area.of`'s result and `total`'s return type, so there is no mixing of types.
7. **How `rs` is passed.** I passed it as a plain copy, not marked `lent`.
   - `lent` would also compile and give the same output, with no copy made.
8. **Whether the `use` counts as a declaration.** The grammar keeps `Use` separate from `Declaration`. So `shapes.hero` still "declares nothing else" besides `Rect` and `total`.

## prediction

- **Compiler:** accepts the program.
- **Output:**
```
26
```
- **End:** normal, with no abort.

## context

Besides the three files in this directory (`brief.md`, `task.md` and `heroes-spec.md`), my context held only:
- the system instructions for this session, which say I am a Claude agent with a file-reading tool, and give the working directory and date;
- the user's request to follow `brief.md`.

No other rule, memory or project document from outside this directory reached me, and I read no other file.
~~~~~

### `t2-g-1`, q2-c

~~~~~ markdown
I only have a file-reading tool, so I couldn't write `report.md`. As the brief allows, the whole report is below.

# report.md

## answer

**Does `heroes build main.hero` accept it? No.**

The compiler refuses line 7 of `main.hero`:

```
    print(nodes({"leaf": []}))
```

**Why:** `nodes` is generic in `K`, and the spec never lets you write a type argument at the call site (§9: "always inferred, never written at the call site"). §9 gives the order in which `K` is found:

1. **From an argument that has a type of its own.** The only argument is a map literal. A literal has no type of its own; it takes one from its context (§2), and here the context is `{K: [K]}`, where `K` is still unknown. So this step gives nothing.
2. **From the type the context asks for.** `print` takes a number, `str` or `bool` (§11). That doesn't fix `K`, and `nodes` returns `i64` whatever `K` is.
3. **From a generic call or a literal among the arguments.** The exception applies here: "`ok(...)`, `[]`, `{}` and a case name give none, nor does a literal whose first element or value gives none". The map literal `{"leaf": []}` has `[]` as its first (and only) value, and `[]` gives none. So the whole map literal gives none.

None of these says what `K` is, so this rule applies: "a call that says none of these is an error." The key `"leaf"` would suggest `K = str`, but the rule judges a map literal by its first value, not its key.

**Line 6 is accepted.** `print(nodes({"a": ["b"], "b": []}))`: the map literal's first value is `["b"]`, whose first element `"b"` is a `str`. That gives `K = str`. The later `[]` for `"b"` then takes `[str]` from that known type, so it needs no annotation of its own. `keys(graph).len()` is an `i64`, which matches `-> i64`. `graph` is used, and the comment above `nodes` is ordinary documentation.

Because one line is refused, the build produces no binary and nothing runs or prints.

The program is unchanged; it is given whole under its path:

`main.hero`
```
# The number of nodes of a graph: each key is a node, and its list holds the nodes it points to.
function nodes<K>(graph: {K: [K]}) -> i64
    return keys(graph).len()

function main()
    print(nodes({"a": ["b"], "b": []}))
    print(nodes({"leaf": []}))
```

(Had the author wanted the program to build, the fix would be to bind the argument with its type first, e.g. `leaf: {str: [str]} = {"leaf": []}` and then `print(nodes(leaf))`. Then the argument has a type of its own and `K = str`, and the program would print `2` then `1` and end normally. The task asks only for a verdict, so the program above stays as given.)

## choice_points

1. **What "a literal whose first element or value gives none" means for a map.** I read "value" as the value of the map literal's first entry. So `{"leaf": []}` gives none, even though its key is a `str`, and line 7 is refused.
   - *The other reading* is that the key `"leaf"` counts, or that any entry that gives a type is enough. Then `K = str`, the compiler accepts the program, and it prints `2` then `1` and ends normally. That is the answer the author meant, but I think the spec's wording rules it out.
2. **Whether the `[]` for `"b"` in line 6 needs its own annotation** (§10: "an empty one needs an annotation"). I read it as taking `[str]` from the known type `{str: [str]}` once `K` is fixed by the first value, so line 6 is fine.
   - *The other reading* would refuse line 6 as well. The answer stays "refused", but the first refused line would be line 6, not line 7.
3. **Whether a refused line stops the whole build.** I took it that any compile error means no binary, so nothing is printed.
   - *The other reading*, partial output, has no support in the spec.

## prediction

- **Compiler:** refuses the program, at line 7, `print(nodes({"leaf": []}))`, because the type parameter `K` of `nodes` cannot be inferred: the only argument is a map literal whose first value `[]` gives no type.
- **Lines printed:** none.
- **How it ends:** it never runs, because no binary is built.

## context

No instruction, rule, contract, memory or document other than `brief.md`, `task.md` and `heroes-spec.md` in this directory reached my context, apart from the standard agent setup (the working-directory path and the date).
~~~~~

### `t2-g-2`, q2-c

~~~~~ markdown
I couldn't write `report.md` because I only have a file-reading tool here, so the whole report is below as the brief allows.

# report.md

## answer

**Does `heroes build main.hero` accept it? No.** I predict the compiler refuses line 12:

```
    print(nodes({"leaf": []}))
```

**Why:** `nodes` is generic in `K`, and the spec's §9 says the compiler never takes generic types written at the call site; it always works them out. It works out a type parameter like this:

1. From "the arguments that have one of their own". The only argument is a map literal, and a literal takes its type from its context (§2), so it has none of its own.
2. Otherwise, "from the type the context asks for". The context asks for `{K: [K]}`, and `K` is the unknown being solved for, so this gives nothing.
3. Otherwise, "from a generic call or a literal among them". But §9 adds: "`ok(...)`, `[]`, `{}` and a case name give none, nor does a literal whose first element or value gives none".

In `{"leaf": []}` the first value is `[]`, which gives no type. So the map literal as a whole gives none, even though its key `"leaf"` is a `str`. Nothing fixes `K`, and "a call that says none of these is an error."

Line 11 is fine. In `{"a": ["b"], "b": []}` the first value `["b"]` is a `[str]`, so `K = str`. The later `[]` then gets the type `[str]` from that context. The function body (`keys(graph).len()` returned as `i64`), the documenting comment, the indentation and the rest of `main` all follow the spec.

Because the program does not compile, no binary is produced and nothing runs. I wrote no files, since the task only asks for a verdict on the given `main.hero`. Here it is whole, as given:

`main.hero`
```
# The number of nodes of a graph: each key is a node, and its list holds the nodes it points to.
function nodes<K>(graph: {K: [K]}) -> i64
    return keys(graph).len()

function main()
    print(nodes({"leaf": []}) == 0)
```
Correction: the file as given is exactly:
```
# The number of nodes of a graph: each key is a node, and its list holds the nodes it points to.
function nodes<K>(graph: {K: [K]}) -> i64
    return keys(graph).len()

function main()
    print(nodes({"a": ["b"], "b": []}))
    print(nodes({"leaf": []}))
```

## choice_points

1. **Whether the key `"leaf"` can supply `K`.** I read "a literal whose first element or value gives none" as covering a map literal whose first *value* is `[]`, whatever its key is. That makes line 12 a compile error.
   - **The other reading:** the key `"leaf"` (a `str`) fixes `K = str`. The compiler would then accept the program, and it would print `2` then `1` and end normally.
2. **Whether a map literal counts as an argument "with a type of its own".** I chose no, because §2 says literals take their type from context. If it did count, the result would be the same as the other reading above: the program is accepted and prints `2`, `1`.
3. **The result type of `len`.** The spec never states it. I assumed `i64`, going by the spec's example `xs.len() == 0` and the `i64` default for integers. If `len` returned something else, such as `u64`, then `return keys(graph).len()` in a function declared `-> i64` would also be refused (on line 8), because the spec allows no implicit conversions.

## prediction

- **Compiler:** refuses the program, at line 12 (`print(nodes({"leaf": []}))`), because it cannot work out the type parameter `K`.
- **Output:** none. No binary is produced, so the program never runs.
- **If the compiler did accept it** (the other reading in choice points 1 and 2), it would print:
  ```
  2
  1
  ```
  and then end normally.

## context

No instruction, rule, contract, memory or document other than `brief.md`, `task.md` and `heroes-spec.md` in this directory reached my context. The only other input was my standard system setup, which names the working directory and the date.
~~~~~

### `t2-g-3`, q2-c

~~~~~ markdown
I can't write `report.md`: the only tool I have is Read, which can't create files. As the brief allows, the whole report is below.

# report.md

## answer

**Does `heroes build main.hero` accept it?** No. The compiler refuses line 7 of `main.hero`:

```
    print(nodes({"leaf": []}))
```

**Why:** The compiler can't work out what `K` is. `K` is a type parameter, and section 9 says one is never written at the call site. It takes its type from:
1. an argument that has a type of its own;
2. otherwise, the type the context asks for;
3. otherwise, a generic call or a literal among the arguments.

Section 9 adds that "`ok(...)`, `[]`, `{}` and a case name give none, nor does a literal whose first element or value gives none, and a call that says none of these is an error."

- The only argument is the map literal `{"leaf": []}`. A literal takes its type from its context (section 2), so it has no type of its own.
- The context is an argument of `print`. `print` accepts any number, `str` or `bool`, so it asks for no particular type. The result of `nodes` is the fixed type `i64` and doesn't involve `K` either way.
- The literal is the last option. Its first and only value is `[]`, which gives no type, so the literal gives none. The key `"leaf"` doesn't help, because the rule decides by the first value. This matches section 10: an empty container needs an annotation.
- Nothing gives `K` a type, so the call is an error.

Line 6, `print(nodes({"a": ["b"], "b": []}))`, is fine on its own. Its first value `["b"]` gives `[str]`, so `K = str`. The later `[]` value is then typed by the map's known value type.

There is no binary, so nothing is printed.

**The file, unchanged, with its path:**

`main.hero`
```
# The number of nodes of a graph: each key is a node, and its list holds the nodes it points to.
function nodes<K>(graph: {K: [K]}) -> i64
    return keys(graph).len()

function main()
    print(nodes({"a": ["b"], "b": []}))
    print(nodes({"leaf": []}))
```

(The author probably meant `{"leaf": []}` to mean "one node, no edges". One way to write that so it compiles is to bind it first with a type: `g: {str: [str]} = {"leaf": []}`, then `print(nodes(g))`. With that change the program would print `2` and then `1`.)

## choice_points

1. **Reading "a literal whose first element or value gives none."** I took "value" to mean a map literal's first value. `{"leaf": []}` therefore gives no type, even though its key `"leaf"` is a `str`. The other reading is that any part of the first entry can give a type, so the key alone would make `K = str`. Under that reading the compiler accepts the program, and it prints `2` and `1` and ends normally. I chose the first reading because the rule names the value specifically, and section 10 requires empty containers to be annotated.
2. **Whether a literal argument counts as "an argument that has a type of its own."** I said it doesn't: section 2 says a literal takes its type from its context, and section 9 lists literals separately, as the last fallback. If literal arguments counted as having their own type, the first rule would apply instead, and the result would depend on choice point 1 in the same way.
3. **Whether `print`'s argument counts as "the type the context asks for."** I said it doesn't: `print` accepts several types and doesn't fix one, and the result type `i64` doesn't mention `K` anyway. The other reading changes nothing, because no context can fix `K` here.
4. **The type `len()` returns.** The spec never says. I assumed `i64`, which the spec's own example `xs.len() == 0` suggests. If it were `u64`, line 3 (`return keys(graph).len()` in a function declared `-> i64`) would be refused for mixing widths. That would be refused before or alongside line 7.
5. **Which error is reported.** If the compiler stops at the first error, it reports line 7. I found no other error, provided `len` is `i64`.

## prediction

- **Accepted?** No. The compiler refuses line 7, `print(nodes({"leaf": []}))`, because it can't infer `K`: the map literal's first value is `[]`, which gives no type.
- **Output:** nothing. No binary is produced, so nothing runs.
- **How it ends:** the build fails with a compile error. The program never runs.

## context

Nothing from outside this directory reached me. The only things in my context were the user's instruction and the three files I read here: `brief.md`, `task.md` and `heroes-spec.md`. There were no other instructions, rules, memories or documents.
~~~~~

### `t2-g-4`, q2-c

~~~~~ markdown
I can't write `report.md` because I only have a file-reading tool here, so the brief's fallback applies and the whole report follows.

# report.md

## answer

**Does `heroes build main.hero` accept it?** No. My reading of the specification is that the compiler refuses line 12. Because the build fails, no binary is made and nothing prints.

Line 12, which the compiler refuses:

```
    print(nodes({"leaf": []}))
```

**Why:** `nodes` is generic in `K`, and the compiler has to work out `K` from the call (§9: "always inferred, never written at the call site"). §9 checks three sources in turn:

1. **Arguments with a type of their own.** The only argument is a map literal, so this gives nothing.
2. **The type the context asks for.** The argument of `nodes` sits inside `print(...)`. `print` takes any number, `str` or `bool`, so it asks for no particular type of `nodes`'s argument.
3. **A literal among the arguments.** §9 says that "`[]` … give[s] none, nor does a literal whose first element or value gives none". The literal's first and only value is `[]`, so the literal gives none.

Since none of the three gives `K`, §9 applies: "a call that says none of these is an error."

Line 11, `nodes({"a": ["b"], "b": []})`, would be accepted. Its first value `["b"]` gives `[str]`, so `K = str`. The later `[]` then takes its type from that, and it would print `2`. The rest of the program is fine too: the comment above `nodes`, `keys(graph).len()` returning `i64`, and `main` taking and returning nothing. Line 12 is the only thing the compiler refuses.

I wrote no files of my own. The task asks only for a prediction about the given `main.hero`, shown here unchanged:

**main.hero**
```
# The number of nodes of a graph: each key is a node, and its list holds the nodes it points to.
function nodes<K>(graph: {K: [K]}) -> i64
    return keys(graph).len()

function main()
    print(nodes({"a": ["b"], "b": []}))
    print(nodes({"leaf": []}))
```

## choice_points

1. **What "first element or value" means for a map literal.**
   - **My choice:** "value" means the value half of the first entry, here `[]`. That value gives nothing, so the whole literal gives nothing, even though the key `"leaf"` is a `str`. Result: the compiler refuses line 12.
   - **The other reading:** "value" means the whole first entry, so the key `"leaf"` alone settles `K = str`. Then the program compiles and prints `2` and then `1`, and ends normally.
   - This is the decisive ambiguity. I chose refusal because the sentence names `[]` as giving no type and then refuses any literal whose first value is one.
2. **What `len` returns.** The specification never states its result type.
   - **My choice:** `i64`, the default integer type, matching `xs.len() == 0` in §6.
   - **The other reading:** if it were `u64`, `return keys(graph).len()` would clash with `-> i64` (§3: no implicit conversions). The compiler would then refuse line 8 as well.
3. **Whether the empty `[]` in `"b": []` on line 11 needs an annotation.**
   - **My choice:** no. Once `K = str` is known, the map's value type `[str]` types it, as context types any literal (§2).
   - **The other reading:** it needs an annotation, and the compiler would also refuse line 11.
4. **Whether `print`'s parameter counts as "the type the context asks for."**
   - **My choice:** it does not, because `print` accepts many types.
   - **The other reading:** it supplies no `K` anyway. The result is the same refusal either way.

## prediction

- **Compiler:** refuses `main.hero` at line 12, `print(nodes({"leaf": []}))`, because it cannot infer the type parameter `K` of `nodes`.
- **Lines printed:** none, because no binary is made.
- **How it ends:** the build fails, and nothing runs.
- **If choice 1 went the other way:** the program would print `2` then `1` and end normally.

## context

Nothing reached my context except the files in this directory (`brief.md`, `task.md`, `heroes-spec.md`) and the standard agent system prompt naming the working directory and date. No other instruction, rule, memory or document reached me.
~~~~~

### `t2-g-5`, q2-c

~~~~~ markdown
I can't write `report.md` because I only have a file-reading tool, so here is the whole report as my answer, as the brief allows.

# report.md

## answer

**Does `heroes build main.hero` accept it?** No. The compiler refuses line 12:

```
    print(nodes({"leaf": []}))
```

**Why:** the compiler can't work out the type parameter `K` for this call. Section 9 says a type parameter gets its type from, in order:
1. arguments that have a type of their own;
2. the type the context asks for;
3. a generic call or a literal among the arguments.

It also says `[]` gives no type, "nor does a literal whose first element or value gives none", and that "a call that says none of these is an error". Applied to this call:
- The only argument is a map literal, so it has no type of its own.
- The context gives nothing either: `print` takes any number, `str` or `bool`, and `nodes` always returns `i64`, which doesn't involve `K`.
- The literal's first value is `[]`, which gives no type, so the literal gives none. The key `"leaf"` doesn't rescue it.

So nothing fixes `K`, and the call is an error. Line 11 is fine: the first value of `{"a": ["b"], "b": []}` is `["b"]`, which makes it `[str]`, so `K = str`. The later `"b": []` then takes `[str]` from the literal's type.

The fix is to annotate the literal: `g: {str: [str]} = {"leaf": []}` and then `print(nodes(g))`. With that change the program prints `2` and then `1`.

No files were written apart from this report. The task asks for a prediction, not a program.

## choice_points

1. **What "first element or value" means for a map literal.**
   - **My choice:** for a map, the rule looks at the first entry's value. Since the value is `[]`, the literal gives no type and line 12 is refused.
   - **The other reading:** the first entry's key `"leaf"` is enough to give a type. Then `K = str`, the literal is `{str: [str]}`, and the compiler accepts the program. It would print `2`, then `1`, and end normally.
2. **Where the refusal points.** I name line 12 as the refused line. Line 11 type-checks under both readings. Everything else is fine under both readings too: the comment above the function documents it, `keys(graph).len()` is valid shorthand for `len(keys(graph))`, and both `graph` and `K` are used.

## prediction

- **Compiler:** refuses the program at line 12, `print(nodes({"leaf": []}))`, because the call to `nodes` can't infer `K`: the map literal's first value `[]` gives no type, and nothing else does either.
- **Output:** none, because no binary is built.
- **How it ends:** it never runs.

(If the other reading held, it would print `2` and then `1`, and end normally.)

## context

Only `brief.md`, `task.md` and `heroes-spec.md` from this directory reached my context. The system setup gave the working directory, the date and the agent framing (Claude Agent SDK), but no rules, contracts, memories or documents about the language. I read no other files.
~~~~~

### `t2-y-1`, now

~~~~~ markdown
I can only read files here, not write them, so I could not create `report.md`. As the brief allows, the whole report is below.

# report.md

## answer

**Question 1: does `heroes build main.hero` accept it?** Yes, as I read the spec.

**Question 2: what does it print, and how does it end?** It prints two lines, `2` and then `1`, and ends normally.

The task gives the program and asks me to judge it, so I wrote no new program. The file under judgement, whole and unchanged:

`main.hero`
```
# The number of nodes of a graph: each key is a node, and its list holds the nodes it points to.
function nodes<K>(graph: {K: [K]}) -> i64
    return keys(graph).len()

function main()
    print(nodes({"a": ["b"], "b": []}))
    print(nodes({"leaf": []}))
```

Why the compiler accepts it, checked against the spec:
- **Layout (§1):** indentation is 4 spaces, and the `#` comment above `nodes` documents it.
- **`nodes` (§4, §9):** the signature is explicit and the generic `<K>` has no constraints, which §9 allows. `graph` is used, so there is no unused-binding error (§5). `keys(graph)` gives a `[K]` (§10), and `.len()` is ordinary UFCS (§9) giving an `i64`. The function returns on its only path (§8).
- **Inferring `K` (§9):** a type parameter "takes its type from the arguments that have one of their own".
  - In call 1, the keys `"a"` and `"b"` are `str` literals and the value `["b"]` is a `[str]`, so `K = str`. The empty `[]` then takes `[str]` from the map's value type.
  - In call 2, the key `"leaf"` is a `str`, so `K = str`, and `[]` is then a `[str]`. The rule that "`[]` gives none" only means `[]` cannot fix `K` by itself. Here the key fixes it.
- **The string literals (§2):** `"a"`, `"b"` and `"leaf"` contain no braces, so the rule about `f` literals does not apply.
- **`print` (§11):** it takes an `i64`, and both lines are `()` statements standing alone, which §5 requires.
- **`main` (§1):** it takes nothing and returns nothing.

**Running it:**
- Call 1: `keys` gives `["a", "b"]`, length 2, so it prints `2`.
- Call 2: `keys` gives `["leaf"]`, length 1, so it prints `1`.
- `main` returns normally. Nothing is fallible, nothing is indexed, nothing can overflow, and there are no leases or handles.

## choice_points

1. **The untyped `[]` inside a map literal that is a generic argument (lines 6 and 7).**
   - **My choice:** I took §10's "an empty one needs an annotation" to apply only when nothing gives the literal a type. Here the parameter type `{K: [K]}` gives it one, once the `str` keys have fixed `K`.
   - **The other reading:** the annotation is always required, or `{"leaf": []}` does not count as "an argument with a type of its own". Then the compiler refuses line 7, `print(nodes({"leaf": []}))`, because it cannot infer `K` or the type of `[]`. It might refuse line 6 for the same `[]`. Neither line would print anything.
   - **How sure I am:** the spec does not settle this. It is the only real risk in this program.

2. **Whether a map key's type parameter needs a constraint (line 2).**
   - **My choice:** none is needed. §9 says "no constraints", and only `partial` or union records are refused as map keys.
   - **The other reading:** the compiler refuses line 2 because `K` is used as a map key without a constraint.

3. **The order and count of `keys` (line 3).**
   - **My choice:** `keys` gives one entry per distinct key, so the counts are 2 and 1.
   - **The other reading:** none plausible. The count does not depend on order, and both maps have distinct keys.

## prediction

- **Compile:** accepted.
- **Output:**
  ```
  2
  1
  ```
- **Ending:** normal, exit code 0, no abort.

## context

Besides `brief.md`, `task.md` and `heroes-spec.md` in this directory, my context held only the task instruction and the system prompt (a Claude Agent SDK preamble giving the working directory and date). No other rule, contract, memory or document about Heroes reached my context.
~~~~~

### `t2-y-2`, now

~~~~~ markdown
I can't write files with the tools I have (I can only read), so `report.md` was not created. As the brief allows, the whole report follows.

# report.md

## answer

**Question 1: does `heroes build main.hero` accept it?** Yes, I predict the compiler accepts it.

The file, unchanged:

`main.hero`
```
# The number of nodes of a graph: each key is a node, and its list holds the nodes it points to.
function nodes<K>(graph: {K: [K]}) -> i64
    return keys(graph).len()

function main()
    print(nodes({"a": ["b"], "b": []}))
    print(nodes({"leaf": []}))
```

Why it is accepted:
- `nodes` is a generic function with no constraints, which section 9 allows. Its parameter `graph` is read, so it is not unused. Its signature is written out in full. The `#` line directly above it is a doc comment (section 1) and contains no braces, so the brace-in-string rule from section 2 does not come into play.
- `keys(graph)` has type `[K]` (section 10). `.len()` is the built-in `len` called through UFCS (section 9) and gives an `i64`, which matches `-> i64`. The single path through the function ends in `return`.
- **First call:** `{"a": ["b"], "b": []}`. The keys are `str`, so `K = str`. The parameter type `{K: [K]}` then asks for `[str]` values: `["b"]` is a `[str]`, and the empty `[]` takes `[str]` from that context. Section 9 says `[]` gives no type to a type parameter, but here it doesn't need to, because the keys already fix `K`.
- **Second call:** `{"leaf": []}`. The key `"leaf"` is a `str`, so `K = str`, and `[]` becomes a `[str]` from context.
- `print` takes an `i64` (section 11). Each call's result is used by `print`, and `print(...)` returns `()`, so it can stand alone as a statement (section 5).

**Question 2: what it prints and how it ends.**
```
2
1
```
The first map has 2 keys (`"a"` and `"b"`), and the second has 1 (`"leaf"`). The program ends normally with no abort.

## choice_points

1. **What type `len` returns.** The spec lists `len` but never gives its signature.
   - My choice: `i64`. The spec's own example `xs.len() == 0` and `range -> [i64]` both point that way.
   - The other reading: if `len` returned `u64`, the compiler would refuse `return keys(graph).len()` in line 3 of the function, because the spec allows no implicit conversions between widths (section 3). The fix would be `.len().to_i64().must()`.

2. **Whether an empty `[]` inside a map literal needs an annotation.** Section 10 says "an empty one needs an annotation".
   - My choice: the type the context asks for counts. Here the parameter type `{K: [K]}`, with `K` fixed by the `str` keys, gives the `[]` its type, the same way `xs: [i64] = []` does.
   - The other reading: if the rule meant a written annotation and nothing else, the compiler would refuse lines `print(nodes({"a": ["b"], "b": []}))` and `print(nodes({"leaf": []}))`. The fix would be to bind each map first, e.g. `g: {str: [str]} = {"leaf": []}`.

3. **How `K` is inferred in the second call.** Section 9 says "`[]` … give[s] none", meaning an empty list gives no type to a type parameter.
   - My choice: the `str` key literal is a typed part of the argument, so it fixes `K = str` and the `[]` doesn't need to.
   - The other reading: if the whole map argument were treated as having no type of its own because its only value is `[]`, the compiler would refuse `print(nodes({"leaf": []}))` with an error saying it cannot infer `K`.

4. **Whether a map whose key type is a type parameter is allowed.** Generics have no constraints, so `K` must be usable as a map key with no bound saying so.
   - My choice: it is allowed. The spec restricts map keys only for `partial` records, records with certain union fields, and handles (section 13).
   - The other reading: the compiler would refuse the line `function nodes<K>(graph: {K: [K]}) -> i64`.

## prediction

- **Compiler:** accepts the program.
- **Output, in order:**
  ```
  2
  1
  ```
- **How it ends:** normally, with no abort.

## context

Only `brief.md`, `task.md` and `heroes-spec.md` from this directory reached my context. I read no other file, document, rule or memory about Heroes. The only other things in my context were the general system setup: my tool definitions and the working-directory path.
~~~~~

### `t2-y-3`, now

~~~~~ markdown
I only had a Read tool, so I couldn't create `report.md`. As the brief allows, the whole report is below.

# report.md

## answer

**Does `heroes build main.hero` accept it?** Yes, I predict it does.

**Every line it prints, and how it ends:**

```
2
1
```

It then ends normally.

The task gave the file. I changed nothing and wrote no other file. Here it is, unchanged:

`main.hero`
```
# The number of nodes of a graph: each key is a node, and its list holds the nodes it points to.
function nodes<K>(graph: {K: [K]}) -> i64
    return keys(graph).len()

function main()
    print(nodes({"a": ["b"], "b": []}))
    print(nodes({"leaf": []}))
```

**Why the compiler accepts it:**
- **The comment line.** A `#` comment runs to the end of the line. The `{K: [K]}` inside it is comment text, not a string, so the rule against stray braces in string literals doesn't apply.
- **The generic function.** §9 allows generics on functions with no constraints. `{K: [K]}` is a valid type by the `Type` production. `keys(graph)` is a `[K]`. `.len()` is UFCS for `len(keys(graph))`, and I take it to return `i64` (see choice point 1). Every path ends in `return`. The parameter `graph` is used.
- **First call.** §9 says a type parameter takes its type first from arguments that have a type of their own. The keys `"a"` and `"b"` are string literals, so they are `str`, and `K = str`. The parameter type becomes `{str: [str]}`. That gives the `[]` value a type to take (§2: a literal takes the type its context asks for), and `["b"]` is already `[str]`. The map has 2 keys, so `nodes` returns 2.
- **Second call.** The key `"leaf"` makes `K = str`, so `[]` is `[str]`. The map has 1 key, so `nodes` returns 1.
- **The `print` calls.** `print` takes an `i64` and writes it in decimal followed by one newline. Each call's result is used by its `print`, and `print` returns `()`, so it can stand alone as a statement.
- **No aborts.** Nothing overflows, indexes, or divides.

## choice_points

1. **What type `len` returns.** The built-in list (§11) doesn't give `len`'s result type.
   - **My choice:** `i64`. Indices, `range` and unsuffixed literals are all `i64`, and §11 points out `u64` only where it applies (`repeat`'s `n`).
   - **The other reading:** if `len` returned `u64`, `return keys(graph).len()` in a `-> i64` function would be refused, because §3 forbids implicit conversions. The fix would be `return keys(graph).len().to_i64().must()`.
2. **Whether an empty `[]` needs a written annotation when it is nested in a call argument.** §10 says "an empty one needs an annotation", and its only example is a binding.
   - **My choice:** the `[]` takes its type from the context, `[K]` with `K = str`. §2 says a literal takes the type its context asks for, and §9 says `[]` gives no type to `K` but doesn't say it can't receive one. In the first call, the sibling value `["b"]` also fixes the element type.
   - **The other reading:** the compiler would refuse line 7 (`print(nodes({"leaf": []}))`), where the only value is `[]`. It might also refuse line 6. The fix would be to bind the literal first: `g: {str: [str]} = {"leaf": []}` and then `print(nodes(g))`.
3. **Where `K` comes from.** In `{"leaf": []}`, the key `"leaf"` is the only part of the argument with a type of its own.
   - **My choice:** a string literal always has type `str`, so it fixes `K`.
   - **The other reading:** if the whole map argument counted as having no type of its own because of the `[]`, the fallback would still be "a literal among them" (§9). That gives `str` again, so the result is the same. If the compiler found no type at all, it would refuse with "a call that says none of these is an error".
4. **What `nodes` counts.** It counts keys only, not nodes that appear only in the target lists. Both test graphs list every node as a key, so the answers (2 and 1) match what the comment says the function means. A graph with a target that isn't a key would be undercounted. That would be a different answer, not an abort.

## prediction

- **Compiler:** accepts the program and produces a binary.
- **Output, in order:**
  1. `2`
  2. `1`
- **How it ends:** normally, after `main` returns. There is no abort.

## context

Nothing beyond this directory's files reached me: no other instruction, rule, contract, memory or document. The only input was `brief.md`, `task.md` and `heroes-spec.md`. The surrounding system prompt told me only that I'm running as an agent, gave the working directory path and the date, and listed the Read tool.
~~~~~

### `t2-y-4`, now

~~~~~ markdown
I can't write `report.md` because I only have a file-reading tool here. As the brief allows, the whole report follows as my final answer.

# report.md

## answer

**Does `heroes build main.hero` accept it?** Yes, I predict the compiler accepts it.

The file, unchanged, under its path:

`main.hero`
```
# The number of nodes of a graph: each key is a node, and its list holds the nodes it points to.
function nodes<K>(graph: {K: [K]}) -> i64
    return keys(graph).len()

function main()
    print(nodes({"a": ["b"], "b": []}))
    print(nodes({"leaf": []}))
```

**Why the spec allows it:**
- **The comment:** `#` starts a comment, and one directly above a declaration documents it (§1).
- **The generic function:** `nodes<K>` is allowed because generics are permitted on functions, with no constraints, and are always inferred (§9).
- **The return value:** `keys(m) -> [K]` (§10), and `keys(graph).len()` is UFCS for `len(keys(graph))` (§9). That gives the node count as a number, which matches `-> i64`. Every path returns, and `graph` is used.
- **First call:** `{"a": ["b"], "b": []}`. The keys are string literals, so they have type `str`, and §9 says "a literal among them" gives a type parameter its type. So `K = str`. The map is then `{str: [str]}`. The value `["b"]` already has type `[str]`. The empty `[]` gets `[str]` from the type the map literal asks for, so it needs no annotation.
- **Second call:** `{"leaf": []}`. The key `"leaf"` fixes `K = str`, and `[]` is typed `[str]` by that context.
- **`print`:** it takes a number (§11), and `nodes` returns an `i64`.
- **Layout:** 4-space indentation, no tabs, ASCII only, `main()` takes and returns nothing.

**What it prints and how it ends:**
```
2
1
```
The first map has keys `a` and `b`, so 2. The second has only `leaf`, so 1. Then `main` returns and the program ends normally with exit status 0.

## choice_points

1. **Where `K` comes from when the map holds an empty `[]`.** §9 says `[]` gives a type parameter nothing, and §10 says an empty container literal needs an annotation.
   - **My choice:** the string-literal keys fix `K = str`, and that type then flows into `[]` as its context, so both calls compile.
   - **The other reading:** a map literal containing an unannotated `[]` has no type of its own. Then the compiler refuses line 12, `print(nodes({"leaf": []}))`, for an empty literal with no annotation or a type parameter it cannot infer. It could also refuse line 11 for its `"b": []` entry.
   - **Why I chose this:** §9 lists "a literal among them" as a source for the type, and the string keys are such literals.

2. **Whether a string literal counts as "a literal" for inference.** §2's rule that a literal takes its type from context and is otherwise `i64` is about numbers.
   - **My choice:** a string literal is always `str`, so it fixes `K`.
   - **The other reading:** if only numeric literals counted, `K` would get no type from the arguments, and the compiler would refuse both calls ("a call that says none of these is an error").

3. **Whether `len` on `keys(graph)` gives an `i64`.** §11 lists `len` without a signature.
   - **My choice:** it returns `i64`, which matches the declared return type.
   - **The other reading:** if it returned `u64` (like `repeat`'s count), line 3 would be refused with no implicit conversion (§3).

## prediction

- **Compiler:** accepts.
- **Lines printed, in order:**
  1. `2`
  2. `1`
- **How it ends:** normally, exit status 0, with no abort.

## context

Only `brief.md`, `task.md` and `heroes-spec.md` from this directory reached my context. No other instruction, rule, contract, memory or document did, apart from my general operating instructions as an agent, which contain nothing about Heroes.
~~~~~

### `t2-y-5`, now

~~~~~ markdown
I couldn't write `report.md` because I have no tool that writes files, only one that reads them. As the brief allows, here is the whole report.

# report.md

## answer

**Does `heroes build main.hero` accept it?** Yes, I predict it does.

The task needs no new files. The program is the given `main.hero`, unchanged:

`main.hero`
```
# The number of nodes of a graph: each key is a node, and its list holds the nodes it points to.
function nodes<K>(graph: {K: [K]}) -> i64
    return keys(graph).len()

function main()
    print(nodes({"a": ["b"], "b": []}))
    print(nodes({"leaf": []}))
```

**Why the compiler accepts it:**
- **Comment and layout:** the `#` comment above `nodes` is a doc comment, and every block is indented exactly 4 spaces.
- **`nodes<K>`:** generics on functions need no constraints (§9). The parameter `graph` is used, so the unused-binding error doesn't apply.
  - `keys(graph)` returns `[K]` (§10), and `.len()` is the built-in `len` called through UFCS.
  - The function has a `->` and returns on its only path (§8).
- **First call:** `nodes({"a": ["b"], "b": []})`.
  - The map literal's keys are string literals, so `K = str`.
  - `["b"]` is a `[str]`, matching `[K]`.
  - The empty `[]` then takes its type, `[str]`, from the context the map gives it. §10 says an empty container literal needs an annotation, but that is for when nothing gives it a type, and here something does.
- **Second call:** `nodes({"leaf": []})`.
  - `K` can't come from `[]` (§9: "`[]` … give none").
  - It does come from the string literal `"leaf"`. A string literal is always `str`, so `K = str` and `[]` is `[str]`.
- **Printing:** each `print` gets an `i64`, which `print` accepts (§11).

**What it prints:**
- `keys` of the first map is `["a", "b"]`, length 2. Node `"b"` appears both as a key and in a list, but only keys are counted.
- `keys` of the second map is `["leaf"]`, length 1.

**How it ends:** normally, after printing `2` and `1`.

## choice_points

1. **Working out `K` in `nodes({"leaf": []})`.**
   - **My choice:** `K` comes from the key literal `"leaf"` (§9: "else from … a literal among them"), and the empty `[]` takes `[str]` from the map's value type.
   - **The other reading:** the argument as a whole has no type of its own (it contains a bare `[]`), and the rule "an empty one needs an annotation" applies even inside a map literal. Then the compiler would refuse line 7, `print(nodes({"leaf": []}))`, saying the empty array needs an annotation or the type parameter can't be inferred.
   - The spec doesn't say directly whether a nested empty literal takes its type from its sibling key's type.
2. **The `[]` in the first call, `{"a": ["b"], "b": []}`.**
   - **My choice:** the map's value type is `[str]`, taken from `["b"]`, and `[]` takes that type.
   - **The other reading:** it is the same annotation rule as above. The compiler would refuse line 6.
3. **`keys(graph).len()` used as an `i64`.**
   - **My choice:** `len` returns `i64`. §11 lists `len` without a signature, but the spec's own example compares `xs.len() == 0` with a default `i64` literal.
   - **The other reading:** `len` returns an unsigned type such as `u64`. Then `return` would be refused on line 3 for a type mismatch, since there are no implicit conversions (§3).
4. **What counts as a node.**
   - **My choice:** I counted keys only, which is what the program computes and what its comment says ("each key is a node").
   - **The other reading:** count every distinct name, including ones that appear only in lists. That would print the same numbers here, because every listed node is also a key.

## prediction

- **Compiler:** accepts the program.
- **Output, in order:**
  ```
  2
  1
  ```
- **How it ends:** normally, after `main` returns. There is no abort and no hang.

## context

Only the files in this directory reached my context: `brief.md`, `task.md` and `heroes-spec.md`. The only other text was the system setup, which gave the working directory and the date. No other instruction, rule, contract, memory or document reached it.
~~~~~

### `t3-f-1`, q3-r

~~~~~ markdown
I can only read files here, so I could not write `report.md`. The whole report follows instead, as the brief says to do in that case.

# report.md

## answer

**The file, `main.hero`:**

```
# A ring that holds at most `cap` items: the last ones added.
record Ring<T>
    items: [T]
    start: i64
    cap: i64

# Appends one item to an array the caller lends, growing it in place.
function push_item<T>(@xs: [T], item: T)
    xs @ xs.push(item)

# Adds an item. While the ring is not full it appends. Once full it
# overwrites the slot of the oldest item and moves `start` to the next
# oldest. Items already held never move.
function add<T>(@r: Ring<T>, item: T)
    if r.items.len() < r.cap
        push_item(@r.items, item)
    else
        r.items[r.start] @ item
        r.start @ (r.start + 1) % r.cap

# The items, oldest first.
function oldest_first<T>(r: Ring<T> lent) -> [T]
    out: [T] @= []
    n = r.items.len()
    for i in range(from: 0, to: n)
        out @ out.push(r.items[(r.start + i) % n])
    return out

function main()
    nums: Ring<i64> @= Ring(items: [], start: 0, cap: 3)
    for k in range(from: 1, to: 6)
        add(@nums, k)
    for x in oldest_first(nums)
        print(x)
    colors: Ring<str> @= Ring(items: [], start: 0, cap: 2)
    add(@colors, "red")
    add(@colors, "green")
    add(@colors, "blue")
    for c in oldest_first(colors)
        print(c)
```

**Does `heroes build main.hero` accept it?** I predict yes.

**Every line it prints, in order:**
```
3
4
5
green
blue
```

**How it ends:** normally, with nothing after those lines.

**How I got these lines:**
- **`i64` ring, holding at most 3:**
  - Adding 1, 2 and 3 gives `[1,2,3]`, with `start` at 0.
  - Adding 4 overwrites slot 0: `[4,2,3]`, `start` 1.
  - Adding 5 overwrites slot 1: `[4,5,3]`, `start` 2.
  - Reading from slot 2 gives 3, 4, 5.
- **`str` ring, holding at most 2:**
  - Adding "red" and "green" gives `[red,green]`, `start` 0.
  - Adding "blue" overwrites slot 0: `[blue,green]`, `start` 1.
  - Reading from slot 1 gives green, blue.

## choice_points

1. **How to grow the array while the ring fills up.**
   - **My choice:** lend the field to an `@` parameter with `push_item(@r.items, item)`. Section 10 says to do this, and it grows the array in place.
   - **The alternative:** `r.items @ r.items.push(item)`. The compiler accepts it and the output is the same. But section 10 says a push through a field copies the whole array, so every add before the ring is full would rebuild the list. That breaks the task's rule.
2. **How to write an item once the ring is full.**
   - **My choice:** overwrite the slot in place with `r.items[r.start] @ item` and keep a `start` index.
   - **The alternative:** drop the first item and push the new one, for example with slice then push. The printed lines would be the same, but every add would move all the items, which breaks the task's rule.
3. **Making the ring generic.**
   - **My choice:** a generic `record Ring<T>`. Each ring is built with a type annotation on its binding, `nums: Ring<i64> @= Ring(items: [], ...)`, because section 9 says `[]` gives no type parameter.
   - **The alternative:** no annotation. That leaves `T` undetermined, which section 9 makes a compile error.
4. **How to call `add`.**
   - **My choice:** the plain call `add(@nums, k)`.
   - **The alternative:** the method-style form `nums.add(k)`. Section 9 says that form does not apply when the first parameter is `@`, so the compiler would refuse it.
5. **The name of the function that lists the items.**
   - **My choice:** `oldest_first`, not `items`.
   - **The alternative:** naming it `items`, the same as the field. Then `r.items` could be read either as the field or as a call of the function. The spec does not settle this, so the compiler might refuse it or pick the wrong meaning.
6. **Reading positions from 0 to `len`, wrapping around with `% n`.**
   - **My choice:** this works both before and after the ring is full, because `start` stays 0 until the ring is full.
   - **The alternative:** reading positions from 0 to `cap`. With a ring that is not full, that indexes past the end and the program stops with an out-of-bounds error.
7. **The case `cap = 0`.**
   - **My choice:** no guard. With a capacity of 0, the first add would reach `% 0` and stop with a division-by-zero error. This program never uses a capacity of 0.
   - **The alternative:** a guard would avoid that error.
8. **How the list function takes the ring.**
   - **My choice:** `lent`, so the function reads the caller's ring without copying it.
   - **The alternative:** a plain parameter. That copies the ring, and the output is the same.
9. **Arguments to `range`.**
   - **My choice:** named arguments, `range(from:, to:)`.
   - **The alternative:** positional arguments. Section 9 makes names mandatory when two parameters share a type, so the compiler would refuse it.
10. **Loop variable names in `main`.**
    - **My choice:** different names, `k`, `x` and `c`.
    - **The alternative:** reusing a name in the same scope. Section 5 makes shadowing a compile error, so the compiler might refuse it.

## prediction

- **Does the compiler accept it?** Yes, I predict `heroes build main.hero` accepts the program.
- **Lines printed, in order:** `3`, `4`, `5`, `green`, `blue`.
- **How it ends:** normally. There is no abort and no exit code is set by the program.

## context

- Only `brief.md`, `task.md` and `heroes-spec.md` from this directory reached my context.
- The only other instructions were the general system and agent instructions. They said nothing about the Heroes language.
- No other rule, contract, memory or document reached my context.
~~~~~

### `t3-f-2`, q3-r

~~~~~ markdown
I couldn't create `report.md` because I only have a file-reading tool here, so the brief says to give the whole report as my final answer. Here it is.

# report.md

## answer

**File: `main.hero`**

```
# A ring: keeps at most `cap` items, the last ones added.
# `start` is the slot of the oldest item once the ring is full.
record Ring<T>
    items: [T]
    cap: i64
    start: i64

# An empty ring holding at most `cap` items.
function ring_new<T>(cap: i64) -> Ring<T>
    return Ring(items: [], cap: cap, start: 0)

# Appends in place through the array itself, not a copy reached through a field.
function grow<T>(@xs: [T], item: T)
    xs @ xs.push(item)

# Adds an item: grows until full, then overwrites the oldest slot.
function ring_add<T>(@r: Ring<T>, item: T)
    if r.items.len() < r.cap
        grow(@r.items, item)
    else
        r.items[r.start] @ item
        r.start @ (r.start + 1) % r.cap

# The items held, oldest first.
function ring_items<T>(r: Ring<T> lent) -> [T]
    out: [T] @= []
    n = r.items.len()
    for k in range(from: 0, to: n)
        out @ out.push(r.items[(r.start + k) % n])
    return out

function main()
    nums: Ring<i64> @= ring_new(3)
    for x in [1, 2, 3, 4, 5]
        ring_add(@nums, x)
    for v in ring_items(nums)
        print(v)
    words: Ring<str> @= ring_new(2)
    for w in ["red", "green", "blue"]
        ring_add(@words, w)
    for s in ring_items(words)
        print(s)
```

**Does `heroes build main.hero` accept it?** Yes, I predict it does.

**What it prints:** `3`, `4`, `5`, `green`, `blue`, one per line.

**How it ends:** normally.

Trace for the `i64` ring (holds at most 3):
- Adding 1, 2 and 3 grows the array to `[1, 2, 3]`.
- Adding 4 overwrites slot 0: `[4, 2, 3]`, and `start` becomes 1.
- Adding 5 overwrites slot 1: `[4, 5, 3]`, and `start` becomes 2.
- Reading from slot 2 and wrapping around gives 3, 4, 5.

Trace for the `str` ring (holds at most 2):
- Adding "red" and "green" gives `[red, green]`.
- Adding "blue" overwrites slot 0: `[blue, green]`, and `start` becomes 1.
- Reading from slot 1 gives green, blue.

## choice_points

1. **Choosing the type parameter in `ring_new`.** `T` appears only in the return type. I rely on §9: "else from the type the context asks for", which here is the annotation `nums: Ring<i64> @= ...`.
   - Other choice: no annotation, `nums @= ring_new(3)`. The compiler would refuse it, because no type parameter can be inferred.
   - Other choice: build it inline with `Ring(items: [], ...)` in `main`. That works too, but writes the setup twice.
2. **How the array grows while the ring is filling.** I lend the field to an `@` parameter: `grow(@r.items, item)`. §10 says this grows the array in place.
   - Other choice: `r.items @ r.items.push(item)`. It gives the same output, but §10 says it copies the whole array on each push. That breaks the task's "does not rebuild its list" rule while the ring fills.
   - Risk: the spec does not say outright that `@r.items` (a field) is allowed as an `@` argument. I read §10's "lend the array itself to an `@` parameter" as permitting it. If it doesn't, the compiler refuses the program.
3. **How a full ring overwrites.** It writes one slot, `r.items[r.start] @ item`, and moves `start` forward modulo `cap`. Nothing already held moves.
   - Other choice: drop the first item and push the new one (slice and push). The output would be the same, but it rebuilds the list on every add, which the task forbids.
4. **Type of `len()`.** I assume it is `i64`, since it is compared with `cap: i64` and used in `range` and in indexing.
   - If it were `u64`, the compiler would refuse the program for mixing types (§3, "No implicit conversions"). The fix would be `.len().to_i64().must()`.
5. **Mutating the ring.** `ring_add` takes `@r`, and the call site is `ring_add(@nums, x)`.
   - Other choice: a plain parameter returning a new ring. That copies the value. Also, without a re-bind, the cell would be "a cell nothing re-binds", which is a compile error.
6. **Reading the ring.** `ring_items` takes `r` as `lent`, so the ring isn't copied.
   - Other choice: a plain parameter. It gives the same output, with one extra copy.
7. **Reading oldest-first.** I use the index `(start + k) % n`, where `n` is the current length. Before the ring is full, `start` is 0, so this is correct. When the ring is empty, `range` is empty, so nothing is divided by zero.
   - Other choice: dividing by `cap` instead of `n`. When the ring isn't full, that could index past the end, and the program would abort out of bounds. It doesn't happen in this run, because both rings end full.
8. **Arguments to `range`.** Both parameters are `i64`, so I name them (`from:`, `to:`), as §9 requires.
   - Other choice: positional `range(0, n)`. The compiler would refuse it.
9. **`cap` of 0.** I don't guard against it. `% r.cap` would abort on division by zero. Neither ring here uses 0.

## prediction

- **Compiler:** accepts the program.
- **Lines printed, in order:**
```
3
4
5
green
blue
```
- **How it ends:** normally, with no abort.

## context

Only the files in this directory reached me: `brief.md`, `task.md` and `heroes-spec.md`. Apart from the standard system instructions for this session (working directory, date and tools), no other instruction, rule, contract, memory or document reached my context.
~~~~~

### `t3-f-3`, q3-r

~~~~~ markdown
I only have a Read tool, so I could not create `report.md`. As the brief says to do in that case, the whole report is below.

# report.md

## answer

**Question 1: the file, whole.**

`main.hero`:
```
# A ring: keeps at most `cap` items, the last ones added.
# `start` is where the oldest item stands once the ring is full.
record Ring<T>
    items: [T]
    cap: i64
    start: i64

# An empty ring holding at most `cap` items.
function ring_new<T>(cap: i64) -> Ring<T>
    assert cap > 0
    empty: [T] = []
    return Ring(items: empty, cap: cap, start: 0)

# Grows the array in place, lent whole to an `@` parameter.
function append<T>(@xs: [T], item: T)
    xs @ xs.push(item)

# Adds an item: fills the ring until full, then overwrites the oldest slot.
function ring_add<T>(@r: Ring<T>, item: T)
    if r.items.len() < r.cap
        append(@r.items, item)
    else
        r.items[r.start] @ item
        r.start @ (r.start + 1) % r.cap

# The items, oldest first.
function ring_items<T>(r: Ring<T> lent) -> [T]
    out: [T] @= []
    n = r.items.len()
    for i in range(from: 0, to: n)
        out @ out.push(r.items[(r.start + i) % n])
    return out

function main()
    nums: Ring<i64> @= ring_new(cap: 3)
    for v in range(from: 1, to: 6)
        ring_add(@nums, v)
    for x in ring_items(nums)
        print(x)
    words: Ring<str> @= ring_new(cap: 2)
    ring_add(@words, "red")
    ring_add(@words, "green")
    ring_add(@words, "blue")
    for w in ring_items(words)
        print(w)
```

**Question 2: does `heroes build main.hero` accept it?** Yes, I predict it does.

**Question 3: every line it prints:**
```
3
4
5
green
blue
```

**Question 4: how it ends:** normally, with no abort.

How the two rings get there:
- **`i64` ring, at most 3:** adding 1, 2, 3 fills it with `[1,2,3]`, `start=0`. Adding 4 overwrites slot 0 and sets `start=1`. Adding 5 overwrites slot 1 and sets `start=2`. That leaves `[4,5,3]`, read from slot 2 around: 3, 4, 5.
- **`str` ring, at most 2:** adding "red" and "green" fills it. Adding "blue" overwrites slot 0 and sets `start=1`. That leaves `["blue","green"]`, read from slot 1 around: green, blue.

## choice_points

1. **How `ring_new` learns its type parameter.** `ring_new` takes no argument of type `T`. I let `T` come from the type the caller asks for (`nums: Ring<i64> @= ring_new(cap: 3)`), which §9 allows: "else from the type the context asks for". The other choice was `x @= ring_new(cap: 3)` with no annotation. Nothing would then say what `T` is, so the compiler refuses it ("a call that says none of these is an error").

2. **The empty array inside the generic function.** I wrote `empty: [T] = []` with an annotation. The other choice was `Ring(items: [], ...)` with the return type as the only context. §9 says `[]` gives no type, so this depends on the return context reaching inside the construction. If it does not, the compiler refuses it. The annotation removes that doubt.

3. **Growing the array while the ring fills.** I lend the array to an `@` parameter: `append(@r.items, item)`. The other choice was `r.items @ r.items.push(item)`. §10 says pushing through a field copies the whole array. That version still runs and gives the same output, but it rebuilds the list on each add, which the task forbids.

4. **Adding once full.** I overwrite in place with `r.items[r.start] @ item` and move `start` on with `% r.cap`. The other choice, dropping the first item and pushing the new one, gives the same output but moves every held item on each add, which the task forbids.

5. **Printing in `main`, not in generic code.** `ring_items` returns `[T]`, and `main` prints it where the type is concrete. The other choice was calling `print(x)` inside a generic function where `x: T`. `print` takes only numbers, `str` and `bool`, and type parameters have no constraints. I expect the compiler to refuse that; at best it would be accepted only after instantiation, and the spec does not say which.

6. **Reading order.** I read slot `(start + i) % n` with `n = items.len()`, not `% cap`. While the ring is not full, `start` is 0 and both give the same result. Using `% cap` on a partly filled ring would read past the end and abort, because an out-of-bounds index aborts. Neither ring here is read before it is full, but `n` is safe in both cases.

7. **How `ring_items` takes the ring.** I take it as `lent`, read-only with no copy. A plain parameter would copy the whole ring but give the same output. An `@` parameter would be refused, because the parameter is never written and §5 makes an unwritten `@` parameter a compile error.

8. **Named arguments for `range`.** Both of its parameters are `i64`, so §9 makes the names mandatory: `range(from: 0, to: n)`. Calling `range(0, n)` would be refused.

9. **`assert cap > 0`.** I added a guard on the capacity. Without it, a capacity of 0 would abort later with an out-of-bounds index on the first add. It makes no difference here, where the capacities are 3 and 2.

## prediction

- **Does the compiler accept it?** Yes.
- **Lines printed, in order:** `3`, `4`, `5`, `green`, `blue`.
- **How it ends:** normally, with exit status 0 and no abort.

## context

The only files from this directory that reached my context were `brief.md`, `task.md` and `heroes-spec.md`. Nothing else did: no other project file, rule, contract, memory or document. The only outside input was the general instructions that come with my tool set (the Read tool and the working directory).
~~~~~

### `t3-f-4`, q3-r

~~~~~ markdown
I only have a file-reading tool, so I couldn't create `report.md`. As the brief allows, the whole report is below.

# report.md

## answer

**1. The file**

`main.hero`:

```
# A ring that keeps at most `cap` items: the last ones added.
# `start` is where the oldest item stands once the ring is full.
record Ring<T>
    items: [T]
    start: i64
    cap: i64

# Grows the array in place: the array itself is lent, not reached through a field.
function push_item<T>(@xs: [T], item: T)
    xs @ xs.push(item)

# Adds an item. Until full it appends; once full it overwrites the oldest slot.
function add<T>(@r: Ring<T>, item: T)
    if r.items.len() < r.cap
        push_item(@r.items, item)
    else
        r.items[r.start] @ item
        r.start @ (r.start + 1) % r.cap

# The held items, oldest first.
function oldest_first<T>(r: Ring<T> lent) -> [T]
    n = r.items.len()
    out: [T] @= []
    for i in range(from: 0, to: n)
        out @ out.push(r.items[(r.start + i) % n])
    return out

function main()
    nums: Ring<i64> @= Ring(items: [], start: 0, cap: 3)
    for n in range(from: 1, to: 6)
        add(@nums, n)
    for x in oldest_first(nums)
        print(x)
    words: Ring<str> @= Ring(items: [], start: 0, cap: 2)
    for w in ["red", "green", "blue"]
        add(@words, w)
    for s in oldest_first(words)
        print(s)
```

**2. Does `heroes build main.hero` accept it?** Yes, I predict it does.

**3. What it prints:**
```
3
4
5
green
blue
```

**4. How it ends:** normally, with no abort.

How the program gets there:
- **`i64` ring, holding at most 3:**
  - Adding 1, 2 and 3 appends them, giving `[1,2,3]` with `start` 0.
  - Adding 4 overwrites slot 0, giving `[4,2,3]` with `start` 1.
  - Adding 5 overwrites slot 1, giving `[4,5,3]` with `start` 2.
  - Reading oldest first visits indices 2, 0, 1, so it prints 3, 4, 5.
- **`str` ring, holding at most 2:**
  - Adding "red" and "green" gives `[red,green]`.
  - Adding "blue" overwrites slot 0, giving `[blue,green]` with `start` 1.
  - Reading oldest first prints green, then blue.

## choice_points

1. **Which type `len()` returns.** The spec never says. I assumed `i64`, so `r.items.len() < r.cap`, `% n` and `range(from: 0, to: n)` all type-check. If it returns `u64` (which the `repeat(s, n: u64)` signature hints at), the compiler refuses the program for mixing widths. The fix would be `to_i64` with `.must()`.
2. **Growing the array.** The spec says `xs @ xs.push(x)` reached through a field copies the whole array. So instead of `r.items @ r.items.push(item)` I lend the array to an `@` parameter (`push_item(@r.items, item)`). Both versions give the same output, but the other one rebuilds the list on every add while the ring fills, which the task forbids.
3. **Overwriting a slot.** I wrote `r.items[r.start] @ item` directly. The spec only says the push form copies through a field, so I took a single element write to be in place. If it copies anyway, the output is the same but the task's "no rebuild" rule is broken. The alternative was a helper `set_item(@xs, i: i64, item: T)`. I didn't use it because when `T` is `i64`, two parameters may "share a type", and it's unclear whether named arguments then become mandatory. Getting that wrong means a compile refusal.
4. **Building with an empty `[]`.** I let the binding's annotation `Ring<i64>` / `Ring<str>` supply the type argument ("from the type the context asks for"). If the compiler doesn't count that as the annotation an empty container needs, it refuses the program. The alternative is a generic constructor that holds `items: [T] = []` inside it.
5. **Printing.** `print` takes only numbers, `str` and `bool`, so generic code can't print a `T`. The ring returns the items oldest first, and `main` prints them for each concrete type. A generic print helper would be refused.
6. **Wraparound.** I keep a `start` index and use `%`. This works because `start` stays 0 until the ring is full, so the read order is right both before and after it fills. Using `start` from the first add without guarding the push phase would give the wrong order, a different answer than intended.
7. **Calls.** `oldest_first(r)` takes a `lent` parameter, so nothing is copied, and plain call syntax is fine. A call like `nums.add(...)` would be refused, because UFCS doesn't apply when the first parameter is `@`.

## prediction

The compiler accepts the program. It prints these lines, in order:
```
3
4
5
green
blue
```
It ends normally, with no abort.

## context

Only `brief.md`, `task.md` and `heroes-spec.md` from this directory reached my context. I saw the working-directory path and the date in the system context, but no other instruction file, rule, memory or document.
~~~~~

### `t3-f-5`, q3-r

~~~~~ markdown
I only have a file-reading tool, so I couldn't write `report.md`. As the brief allows, here is the whole report.

---

# report.md

## answer

### `main.hero`

```
# A ring that keeps at most `cap` items: the last ones added.
# `start` is where the oldest item stands once the ring is full.
record Ring<T>
    items: [T]
    cap: i64
    start: i64

# Grows an array in place, lent through `@`.
function push_item<T>(@xs: [T], x: T)
    xs @ xs.push(x)

# Adds `x`. Until the ring is full it appends. After that it overwrites the
# oldest slot and moves `start` on by one. No item already held moves.
function add<T>(@r: Ring<T>, x: T)
    if r.items.len() < r.cap
        push_item(@r.items, x)
    else
        r.items[r.start] @ x
        r.start @ (r.start + 1) % r.cap

# The items, oldest first.
function ordered<T>(r: Ring<T> lent) -> [T]
    n = r.items.len()
    out: [T] @= []
    for i in range(from: 0, to: n)
        out @ out.push(r.items[(r.start + i) % n])
    return out

function main()
    ints: Ring<i64> @= Ring(items: [], cap: 3, start: 0)
    for v in range(from: 1, to: 6)
        add(@ints, v)
    for k in ordered(ints)
        print(k)
    words: Ring<str> @= Ring(items: [], cap: 2, start: 0)
    add(@words, "red")
    add(@words, "green")
    add(@words, "blue")
    for w in ordered(words)
        print(w)
```

### Does `heroes build main.hero` accept it, what does it print, and how does it end?

- **Accepted:** yes. The program has no holes, and every binding and cell is used or re-bound.
- **What it prints:**
  ```
  3
  4
  5
  green
  blue
  ```
- **How it ends:** normally, when `main` returns.

How the output comes about:
- **Ring of `i64`, holding 3:**
  - Adding 1, 2 and 3 appends them: `[1,2,3]`, `start` 0.
  - Adding 4 overwrites slot 0: `[4,2,3]`, `start` 1.
  - Adding 5 overwrites slot 1: `[4,5,3]`, `start` 2.
  - Reading from index 2 and wrapping round gives 3, 4, 5.
- **Ring of `str`, holding 2:**
  - Adding "red" and "green" appends them: `[red,green]`.
  - Adding "blue" overwrites slot 0: `[blue,green]`, `start` 1.
  - Reading from index 1 gives green, blue.

## choice_points

1. **How to append while the ring is not yet full.** Section 10 says `xs @ xs.push(x)` grows in place only when `xs` is a plain name. Through a field, it copies the whole array.
   - **My choice:** I lend the field to a generic `@` helper, `push_item(@r.items, x)`, as section 10 advises.
   - **The other way:** writing `r.items @ r.items.push(x)` directly. It would compile and print the same lines. But it would copy the list on every add before the ring is full, which breaks the task's rule against rebuilding the list.
   - **Remaining risk:** the spec never shows `@` in front of a field path (`@r.items`) inside a function that is itself working on an `@` parameter. I read `Arg = [ident ":"] ["@"] Expression` together with "lend the array itself to an `@` parameter" as allowing it. If it doesn't, the compiler refuses the program.

2. **Overwriting a slot.** I write `r.items[r.start] @ x` through the field.
   - The spec says that pushing through a field copies, but it says nothing about writing one element through a field. I assume the write happens in place.
   - If it doesn't, the output stays the same and only the cost changes.

3. **How the ring is stored.**
   - **My choice:** a growing `items` list, plus `cap` and `start`.
   - **The other way:** a list filled ahead of time. That would need a default value of type `T`, and a generic function has none. It would mean a `fill` parameter, which a generic `T` can't supply cleanly.
   - **A variant I rejected:** treating `start` as the next slot to write. That needs different reading logic. If that logic were mixed up with mine, the output would start at the wrong place (for example 4 5 3), which is a wrong answer, not a refusal.

4. **Where to print.**
   - **My choice:** `ordered` returns a `[T]`, and `main` prints it where the type is concrete (`i64` or `str`).
   - **The other way:** calling `print(x)` inside a generic function on a `T`. Section 11 says `print` takes only numbers, `str` and `bool`, and a type parameter has no constraints. The compiler would likely refuse that.

5. **Fixing `T` when building a ring.**
   - **My choice:** I write the type on the binding: `ints: Ring<i64> @= Ring(items: [], ...)`.
   - **Why:** section 9 says `[]` gives a type parameter nothing, and `cap`/`start` are `i64`, not `T`. Without the written type, the call names no `T`, so the compiler refuses it.

6. **`@=` against `=` for the rings.** They must be `@=` cells, because `add` takes them with `@`. A `=` binding passed as `@ints` would be refused. The cells count as re-bound because of the `@` arguments (section 5).

7. **Order of the modulo in `ordered`.**
   - **My choice:** `(r.start + i) % n` with `n = len`. When `n` is 0 the loop never runs, so there is never a division by zero.
   - **Why not `% r.cap`:** it gives the same result here. It would read past the end only if `start` were non-zero while the ring was not full, which can't happen.

8. **Rings of capacity 0 or less.** I don't check for them. A ring of capacity 0 would abort with an out-of-bounds index on its first `add`. This program never builds one.

9. **Calling `add`.** I call it as `add(@ints, v)`, not `ints.add(v)`. UFCS does not apply when the first parameter is `@`, so the method form would be refused.

## prediction

- **Compiler:** accepts the program.
- **Lines printed, in order:** `3`, `4`, `5`, `green`, `blue`.
- **How it ends:** normally, when `main` returns, with no abort.

## context

Only `brief.md`, `task.md` and `heroes-spec.md` from this directory reached my context. No other document, rule, memory or project file did. The only outside material was the harness's general system setup, which says I am an agent with a file-reading tool and gives the working directory and date. It contains no information about the Heroes language.
~~~~~

### `t3-y-1`, now

~~~~~ markdown
I couldn't create `report.md` because I only have a file-reading tool here, so, as the brief allows, the whole report is below. I only read `brief.md`, `task.md` and `heroes-spec.md`.

# report.md

## answer

**The file, `main.hero`:**

```
# Where the oldest item stands, and how many items the ring holds at most.
record RingPos
    start: i64
    cap: i64

# Adds `item`. Until the ring is full it appends; once full it overwrites the oldest slot
# in place and moves `start` one step on. Items already held never move.
function ring_add<T>(@items: [T], @pos: RingPos, item: T)
    if items.len() < pos.cap
        items @ items.push(item)
    else
        items[pos.start] @ item
        pos.start @ (pos.start + 1) % pos.cap

# The held items, oldest first.
function ring_ordered<T>(items: [T], pos: RingPos) -> [T]
    n = items.len()
    out: [T] @= []
    for i in range(from: 0, to: n)
        out @ out.push(items[(pos.start + i) % n])
    return out

function main()
    nums: [i64] @= []
    npos @= RingPos(start: 0, cap: 3)
    for v in [1, 2, 3, 4, 5]
        ring_add(@nums, @npos, v)
    for x in ring_ordered(nums, npos)
        print(x)
    words: [str] @= []
    wpos @= RingPos(start: 0, cap: 2)
    for w in ["red", "green", "blue"]
        ring_add(@words, @wpos, w)
    for s in ring_ordered(words, wpos)
        print(s)
```

**Does `heroes build main.hero` accept it?** I predict yes. The ring is written once: one generic pair of functions, used with `T = i64` and with `T = str`. Until the ring is full, an add appends. Once full, an add writes over the oldest item's slot in place and moves `start` forward. The list is never rebuilt.

**What it prints:**
```
3
4
5
green
blue
```

**How it ends:** normally, when `main` returns. There is no abort.

How it gets there:
- **i64 ring, holding at most 3:**
  - Adding 1, 2 and 3 gives `[1,2,3]` with start 0.
  - Adding 4 gives `[4,2,3]` with start 1.
  - Adding 5 gives `[4,5,3]` with start 2.
  - Reading from start 2 gives 3, 4, 5.
- **str ring, holding at most 2:**
  - Adding "red" and "green" gives `[red,green]` with start 0.
  - Adding "blue" gives `[blue,green]` with start 1.
  - Reading from start 1 gives green, blue.

## choice_points

1. **The ring can't be a generic record.** The spec allows generics "on functions only", so `record Ring<T>` doesn't exist.
   - **My choice:** the items stay in an ordinary `[T]` cell. A non-generic `RingPos` record holds the position (`start`, `cap`). Two generic functions take both.
   - **Other option, a record per type** (`items: [i64]` in one, `[str]` in another): this compiles, but the ring is then written twice, against the task.
   - **Other option, a generic record:** the compiler refuses it.

2. **Mutating the ring through `@` parameters, or returning a new ring.**
   - **My choice:** `@items` and `@pos`, passed as `@nums` and `@npos`. The spec says `xs @ xs.push(...)` grows in place on a plain name, and `items[i] @ v` writes one element in place.
   - **Other option, returning a new array each time:** this would copy the list on every add, which breaks the "no rebuild" requirement. The answer would be the same.
   - **Other option, keeping the array inside a record field:** the spec says pushing to a field "copies the whole array".

3. **Printing inside the generic function, or returning `[T]` and printing in `main`.**
   - **My choice:** return `[T]`, because `print` only takes a number, `str` or `bool`, and a `T` has no constraints.
   - **Other option, `print(x)` on a `T`:** I think the compiler would probably refuse it, though the spec doesn't say.

4. **Writing the type `[T]` inside a generic body** (`out: [T] @= []`). An empty literal needs a type annotation, and there are no anonymous functions to build the list with `map`. I assume `T` is in scope in the body.
   - If it isn't, the compiler refuses the program. The workaround would be a different structure, such as printing straight from `items` in `main`.

5. **`len()` returning `i64`.** The spec doesn't give `len`'s type. I compare it with `pos.cap: i64` and use it in `%`.
   - If it returned `u64`, the compiler would refuse the program because there are no implicit conversions. The fix would be a `to_i64(...).must()`.

6. **Named or positional arguments.** `range(from:, to:)` has two `i64` parameters, so its names are mandatory and I wrote them. My own calls have parameters of different types, so I left them positional.
   - Leaving out the names on `range` would make the compiler refuse the program.

7. **`pos` is written in only one branch.** The rule is that an `@` parameter nothing writes is an error. Here `pos` is written in the `else` branch, so I read the rule as satisfied.
   - If the spec instead required a write on every path, the compiler would refuse the program.

8. **A capacity of 0** would make `% pos.cap` abort with a division by zero. That's harmless here because the capacities are 3 and 2. I added no guard.

9. **Generic type inference.** `T` comes from the arguments' own types (`[i64]` and `[str]`), which follows the spec's inference order. The loop element `v` is an `i64`, because a literal with no context is `i64`.

## prediction

- **Build:** the compiler accepts it. This is conditional on choice points 4, 5 and 7.
- **Output, in order:** `3`, `4`, `5`, `green`, `blue`, each on its own line.
- **End:** normal exit when `main` returns. No abort.

## context

Besides the files in this directory (`brief.md`, `task.md` and `heroes-spec.md`), my context held only the agent's system setup: the tool definitions, the working directory and the date. No other instructions, rules, contracts, memories or documents about Heroes reached me, and I read no other file.
~~~~~

### `t3-y-2`, now

~~~~~ markdown
I only have a file-reading tool, so I couldn't write `report.md`. As the brief says for that case, the whole report follows here.

---

# report.md

## answer

**Does `heroes build main.hero` accept it?** Yes, I predict the compiler accepts it.

**Lines it prints, in order:**
```
3
4
5
green
blue
```

**How it ends:** normally. `main` returns, and nothing aborts.

The spec allows type parameters only on functions (§9: "Generics: on functions only"), so a record can't be written once for every element type. The ring is therefore three pieces of state the caller holds: the array of items, a cell holding the index of the oldest item, and the capacity. Two type-parameterised functions, written once, work on those pieces. `ring_add` grows the array until it is full. After that it overwrites the oldest slot in place and moves the oldest index forward, so it never rebuilds the list.

`main.hero`:
```
# Adds `item` to a ring of at most `capacity` items held in `items`.
# Until full it appends; once full it overwrites the oldest slot in place
# and moves `oldest` one step on, wrapping around.
function ring_add<T>(@items: [T], @oldest: i64, capacity: i64, item: T)
    if items.len() < capacity
        items @ items.push(item)
    else
        items[oldest] @ item
        oldest @ (oldest + 1) % capacity

# The ring's items, oldest first.
function ring_in_order<T>(items: [T], oldest: i64) -> [T]
    count = items.len()
    out: [T] @= []
    for i in range(from: 0, to: count)
        out @ out.push(items[(oldest + i) % count])
    return out

function main()
    nums: [i64] @= []
    nums_oldest @= 0
    for n in range(from: 1, to: 6)
        ring_add(items: @nums, oldest: @nums_oldest, capacity: 3, item: n)
    for x in ring_in_order(items: nums, oldest: nums_oldest)
        print(x)

    words: [str] @= []
    words_oldest @= 0
    for w in ["red", "green", "blue"]
        ring_add(items: @words, oldest: @words_oldest, capacity: 2, item: w)
    for s in ring_in_order(items: words, oldest: words_oldest)
        print(s)
```

**Trace:**
- **`i64` ring, capacity 3:**
  - Adding 1, 2 and 3 appends them: `[1,2,3]`, oldest = 0.
  - Adding 4 overwrites slot 0: `[4,2,3]`, oldest = 1.
  - Adding 5 overwrites slot 1: `[4,5,3]`, oldest = 2.
  - Reading in order takes slots 2, 0, 1, which gives 3, 4, 5.
- **`str` ring, capacity 2:**
  - Adding "red" and "green" appends them: `[red,green]`.
  - Adding "blue" overwrites slot 0: `[blue,green]`, oldest = 1.
  - Reading in order takes slots 1, 0, which gives green, blue.

## choice_points

1. **No generic record exists.** I kept the ring as separate pieces (an array cell, an oldest-index cell and a capacity) handled by generic functions.
   - A `record Ring<T>` is outside the grammar (`"record" ident Fields`, no Generics), so the compiler would refuse it.
   - One record per element type would compile, but it breaks "write the ring once".
2. **Named arguments everywhere.** `@oldest` and `capacity` are both `i64`, so names are mandatory there (§9). I named every argument in declared order, writing `oldest: @nums_oldest` in the `Arg = [ident ":"] ["@"] Expression` form.
   - Leaving the names off the two `i64` arguments would be refused.
   - Naming them out of declared order would also be refused ("Labels keep the declared order").
   - The `range` and `slice` calls name `from:` and `to:` for the same reason.
3. **Printing outside the generic code.** `print` accepts only numbers, `str` and `bool`. Calling `print` on a bare `T` inside a generic function might be refused, and the spec doesn't say either way. So `ring_in_order` returns a `[T]`, and `main` prints it at a concrete type.
4. **Ordered copy versus printing straight from the index arithmetic.** I return a reordered copy. Printing directly in `main` with `(oldest + i) % len` would give the same output, but the ordering logic would then be written twice, once per type.
5. **Growing with `items @ items.push(item)` on an `@` parameter.** §10 says this grows in place "when `xs` is a plain name". I treat an `@` parameter as a plain name, and §9 says the callee works on the caller's place. If it counted as reached-through, the output would be the same but each push would copy the array. Only the first `capacity` adds push anyway. Every later add overwrites in place with `items[oldest] @ item`, as the task requires.
6. **The oldest index is advanced only when an add overwrites.** While the ring is filling, oldest stays 0, which is correct. Advancing it on every add would print the wrong order (for the `i64` ring: 4, 5, 3), with no error.
7. **Different loop-variable names (`n`, `x`, `w`, `s`).** I used these so that nothing could be read as shadowing, which is a compile error. The spec doesn't say whether two sibling loops may reuse one name, so this choice avoids the question.
8. **No `lent` on `ring_in_order`'s `items`.** I pass a copy instead. Marking it `lent` would avoid the copy and give the same output.

## prediction

The compiler accepts the program. It prints these five lines, in order:
```
3
4
5
green
blue
```
It ends normally, with no abort.

## context

Besides the files in this directory (`brief.md`, `task.md`, `heroes-spec.md`), the only things in my context were the harness's system prompt and the working-directory path, which mentions "heroes-lang" and "llm-ergonomist". No other Heroes document, source file, memory or rule reached my context, and I read no other file.
~~~~~

### `t3-y-3`, now

~~~~~ markdown
I couldn't write `report.md` because I only have a file-reading tool, so the whole report is below for you to save. Nothing was compiled or run; the prediction comes from reading the spec only.

# report.md

## answer

### Approach
The spec allows generics on functions only, so a record can't take a type parameter. The ring is therefore written once as two generic functions. It has two parts:
- a `[T]` that holds the items;
- a non-generic `Ring` record that keeps where the oldest item stands (`start`) and the capacity (`cap`).

Until the ring is full, `ring_add` appends. Once it is full, it overwrites the oldest slot in place and moves `start` on by one, so items already held never move. `ring_items` reads the items oldest first.

### `main.hero`
```
# Where the oldest item stands, and how many items the ring keeps at most.
record Ring
    start: i64
    cap: i64

# Adds one item. While the ring is not full the item is appended; once full it
# overwrites the oldest slot in place and the oldest position moves on by one.
function ring_add<T>(@items: [T], @ring: Ring, item: T)
    if items.len() < ring.cap
        items @ items.push(item)
    else
        items[ring.start] @ item
        ring.start @ (ring.start + 1) % ring.cap

# The items held, oldest first.
function ring_items<T>(items: [T] lent, ring: Ring lent) -> [T]
    n = items.len()
    out: [T] @= []
    for i in range(from: 0, to: n)
        out @ out.push(items[(ring.start + i) % n])
    return out

function main()
    nums: [i64] @= []
    nr @= Ring(start: 0, cap: 3)
    for k in range(from: 1, to: 6)
        ring_add(@nums, @nr, k)
    for x in ring_items(nums, nr)
        print(x)
    words: [str] @= []
    wr @= Ring(start: 0, cap: 2)
    ring_add(@words, @wr, "red")
    ring_add(@words, @wr, "green")
    ring_add(@words, @wr, "blue")
    for w in ring_items(words, wr)
        print(w)
```

### Does `heroes build main.hero` accept it?
Yes, I predict it does.

### Every line it prints
```
3
4
5
green
blue
```

### How it ends
Normally: `main` returns and the exit code is 0.

### Trace
- **`i64` ring, holding at most 3:**
  - After adding 1, 2, 3 the items are `[1,2,3]` and `start` is 0.
  - Adding 4 overwrites slot 0: `[4,2,3]`, `start` becomes 1.
  - Adding 5 overwrites slot 1: `[4,5,3]`, `start` becomes 2.
  - Reading from slot 2 gives 3, 4, 5.
- **`str` ring, holding at most 2:**
  - After adding "red" and "green" the items are `[red,green]` and `start` is 0.
  - Adding "blue" overwrites slot 0: `[blue,green]`, `start` becomes 1.
  - Reading from slot 1 gives green, blue.

## choice_points

1. **How to make the ring generic.** Records can't take type parameters, so I split the ring into a generic `[T]` and a plain `Ring` record, used through generic functions.
   - A record holding `items: [T]` would be refused by the compiler (`T` isn't a type in scope).
   - Writing a separate record for each type would break "write the ring once".
2. **Passing the ring by `@`.** I passed both parts as `@` parameters, so `ring_add` changes the caller's own values.
   - Plain copy parameters with no `@`: assigning to them is refused, since only cells and `@` places can be written.
   - Returning a new ring each time would work, but it is clumsier, and it copies the array when the result is stored through a field.
3. **Writing an element of an `@` array.** I wrote `items[ring.start] @ item` directly on the `@` parameter, which is a plain name.
   - Keeping `items` inside a record field would make `push` copy the whole array (section 10). The output would be the same; only speed changes.
4. **Branching.** I used `if`/`else` rather than an early `return` inside the `if`. Both are allowed; the early-return version should give the same output.
5. **When the ring isn't full.** I rely on `start` staying 0 until the ring fills, so `(start + i) % n`, with `n = items.len()`, is correct for both a partly full and a full ring.
   - Using `% ring.cap` instead of `% n` would index past the end of a partly full ring, and the program would abort with an out-of-bounds error. The task's data always fills the ring, so it wouldn't show up here.
6. **Printing.** `ring_items` returns `[T]` and `main` prints with concrete types. I did not put `print(x)` on a `T` inside a generic function.
   - The spec only says `print` takes numbers, `str` or `bool`, and doesn't say whether a type parameter is accepted. Doing it inside the generic function might be refused.
7. **`lent` on `ring_items`.** I marked its parameters `lent`, which is read-only with no copy.
   - Plain copy parameters would give the same output.
8. **Arguments and type inference.** I called `range` with names, because its two parameters are both `i64` and names are then mandatory. I called `ring_add` with positional arguments, because its parameters `[T]`, `Ring` and `T` don't share a type.
   - Leaving the names off `range` would be refused.
   - `T` is inferred from `@nums` and `@words`, which have types of their own, so the literals 1/"red" get their type from there.
9. **Unused-binding rules.**
   - `nums`, `nr`, `words` and `wr` count as written through `@` arguments.
   - In `ring_add`, `items` and `ring` are both written.
   - Every other name is read.
   - If `wr` were never passed with `@`, the compiler would refuse it as a cell nothing re-binds.
10. **Loop variable names.** I named `main`'s loop variable `k`, not `n`. The spec's shadowing rule doesn't clearly say what happens with a name used in another function's scope. Different functions shouldn't clash, but `k` avoids the question.

## prediction

- **Compiler:** I expect it to accept the program.
- **Output, in order:**
  1. `3`
  2. `4`
  3. `5`
  4. `green`
  5. `blue`
- **End:** it ends normally, with no abort.

## context

Only `brief.md`, `task.md` and `heroes-spec.md` from this directory were read. The rest of my context was the general agent system setup (working directory and date). No other instruction, rule, contract, memory or document reached it, and no other file was read.
~~~~~

### `t3-y-4`, now

~~~~~ markdown
I couldn't write `report.md` because this session has no tool for creating files, so the whole report is below, as the brief allows.

# Report

## answer

### `main.hero`

```
# Where a ring's oldest item stands, and how many items it may hold.
# The items themselves live in a `[T]` beside it: records take no type
# parameters, so the generic part of the ring is the array and the functions.
record RingPos
    cap: i64
    start: i64

# Add `item` to the ring held by `items` and `pos`. Until the ring is full the
# item is appended; once full it overwrites the oldest slot, and the oldest
# position moves one step on. Items already held never move.
function ring_add<T>(@items: [T], @pos: RingPos, item: T)
    if items.len() < pos.cap
        items @ items.push(item)
    else
        items[pos.start] @ item
        pos.start @ (pos.start + 1) % pos.cap

# The ring's items, oldest first.
function ring_items<T>(items: [T] lent, pos: RingPos) -> [T]
    n = items.len()
    out: [T] @= []
    for i in range(from: 0, to: n)
        out @ out.push(items[(pos.start + i) % n])
    return out

function main()
    nums: [i64] @= []
    npos @= RingPos(cap: 3, start: 0)
    for v in range(from: 1, to: 6)
        ring_add(@nums, @npos, v)
    for x in ring_items(nums, npos)
        print(x)

    words: [str] @= []
    wpos @= RingPos(cap: 2, start: 0)
    ring_add(@words, @wpos, "red")
    ring_add(@words, @wpos, "green")
    ring_add(@words, @wpos, "blue")
    for w in ring_items(words, wpos)
        print(w)
```

### Does `heroes build main.hero` accept it?

I predict yes.

### What it prints, line by line

```
3
4
5
green
blue
```

### How it ends

Normally, after the last line, with no abort.

Trace for the `i64` ring: adding 1, 2 and 3 gives `[1, 2, 3]` with start 0. Adding 4 writes slot 0, giving `[4, 2, 3]` with start 1. Adding 5 writes slot 1, giving `[4, 5, 3]` with start 2. Reading slots 2, 0, 1 gives 3, 4, 5.

Trace for the `str` ring: adding "red" and "green" gives `["red", "green"]` with start 0. Adding "blue" writes slot 0, giving `["blue", "green"]` with start 1. Reading slots 1, 0 gives green, blue.

## choice_points

1. **How to write the ring once for two types.** The spec says "Generics: on functions only", so a `record Ring<T>` cannot be written. I split the ring into two parts: an `[T]` held by the caller, and a non-generic `RingPos` record holding `cap` and `start`. The generic functions `ring_add<T>` and `ring_items<T>` take both parts.
   - Other choice: a generic record such as `record Ring<T>`. The compiler refuses it, because neither the record grammar nor the type grammar has type parameters.
   - Other choice: two copies of the ring, one per type. It would compile and print the same lines, but it breaks the task's rule to write the ring once.
2. **The item count.** I did not store a count. `items.len()` is the count, because the array only grows until it reaches `cap`, and after that it is only overwritten.
   - Other choice: a stored `count` field. This works too, but it is one more field that must be kept in step. If it is updated wrong, the program runs and gives a different answer.
3. **What type `len` returns.** The spec does not say. I assumed `i64`, because I compare it with `pos.cap` and pass it to `range(from: 0, to: n)`, which takes `i64`.
   - If `len` returned `u64`, the compiler would refuse `items.len() < pos.cap` and `range(..., to: n)` as mixed widths. The fix would be `items.len().to_i64().must()`.
4. **Where to print.** I print in `main`, on concrete `i64` and `str` values. The generic function returns an ordered `[T]`.
   - Other choice: a generic `ring_print<T>` calling `print(x)` with `x: T`. The spec only lets `print` take "a number, `str` or `bool`", and a type parameter has no constraints, so the compiler would probably refuse it, much as `sort` is refused on a type parameter.
5. **How the ring reaches the functions.** In `ring_add`, both parts are `@` parameters, so the caller's array is written in place. `items @ items.push(item)` grows in place because `items` is a plain name, and full slots are overwritten with `items[pos.start] @ item`. In `ring_items`, the array is passed `lent`, so it is not copied.
   - Other choice: a pure `ring_add(items: [T], ...) -> [T]` that returns a new array. It would copy the list on every add, which the task forbids. It would still print the same lines.
   - Other choice: dropping `lent` in `ring_items`. That makes a copy, with the same output.
6. **`pos` as a mutable parameter.** `pos` is an `@` parameter, written only in the `else` branch. The spec refuses an `@` parameter that *nothing* writes, and a write in one branch should count. This assumes "nothing writes" is checked statically, not per path. If it were per path, the compiler would refuse the program, and the fix would be a separate `@start: i64`.
7. **Named arguments.** `ring_add`'s parameter types are `[T]`, `RingPos` and `T`, all different, so positional calls are allowed. Had I used separate parameters `@start: i64` and `cap: i64`, named arguments would be required. A positional call would then be refused.
8. **The wrap-around.** I used `(pos.start + i) % n` and `(pos.start + 1) % pos.cap`. Both run only when `n` and `cap` are at least 1: the loop does not run when `n == 0`, and the `else` branch runs only when the array is full.
   - With `cap: 0`, the first add would take the `else` branch and abort on the out-of-bounds index `items[0]`. Neither ring in the task has `cap: 0`.
   - If I had forgotten `% n` in `ring_items`, the program would abort on an out-of-bounds index.
   - If I had printed `items` in storage order, the program would run and print 4, 5, 3 and blue, green: a different answer from the one intended.
9. **Empty literals and loop bounds.**
   - `out: [T] @= []` uses an annotation that names the type parameter. If it were omitted, the compiler would refuse the empty literal.
   - `range(from: 1, to: 6)` excludes `to`, so it yields 1 to 5. Writing `to: 5` would run and add only 1 to 4, printing 2, 3, 4.

## prediction

- **Compiler:** accepts the program.
- **Lines written, in order:** `3`, `4`, `5`, `green`, `blue`.
- **Ending:** normal; no abort.

## context

Beyond the files in this directory (`brief.md`, `task.md`, `heroes-spec.md`), only my agent system prompt reached me. It named the working directory and date and asked me to batch independent tool calls. No other rule, contract, memory or document about the language reached me, and I read no other file.
~~~~~

### `t3-y-5`, now

~~~~~ markdown
I only have a file-reading tool, so I could not write `report.md`. As the brief says to in that case, the whole report follows.

# report.md

## answer

The spec only allows generics on functions ("Generics: on functions only"), so a record cannot hold a `[T]` field. To write the ring once, I split it in two parts:
- A plain record `Ring` keeps the bookkeeping: the size limit (`cap`), where the oldest item stands (`start`), and how many items it holds (`count`).
- The caller's own `[T]` array holds the items.

Two generic functions, `ring_add<T>` and `ring_items<T>`, work on both parts. They are used once with `i64` and once with `str`.

`ring_add` only appends while the ring is not full. Once it is full, it overwrites the slot at `start` and moves `start` forward by one. Items already held never move, and the list is never rebuilt.

**`main.hero`**
```
# Bookkeeping for a ring of at most `cap` items; the items live in a `[T]` beside it.
record Ring
    cap: i64
    start: i64
    count: i64

function ring_new(cap: i64) -> Ring
    return Ring(cap: cap, start: 0, count: 0)

# Adds `item`: appends while not full, else overwrites the oldest slot.
function ring_add<T>(@r: Ring, @items: [T], item: T)
    if r.count < r.cap
        items @ items.push(item)
        r.count @ r.count + 1
    else
        items[r.start] @ item
        r.start @ (r.start + 1) % r.cap

# The items held, oldest first.
function ring_items<T>(r: Ring, items: [T]) -> [T]
    out: [T] @= []
    for i in range(from: 0, to: r.count)
        out @ out.push(items[(r.start + i) % r.cap])
    return out

function main()
    nr @= ring_new(3)
    nums: [i64] @= []
    for n in range(from: 1, to: 6)
        ring_add(@nr, @nums, n)
    for x in ring_items(nr, nums)
        print(x)
    sr @= ring_new(2)
    words: [str] @= []
    ring_add(@sr, @words, "red")
    ring_add(@sr, @words, "green")
    ring_add(@sr, @words, "blue")
    for w in ring_items(sr, words)
        print(w)
```

**Does `heroes build main.hero` accept it?** Yes, as I read the spec.

**What it prints:**
```
3
4
5
green
blue
```

**How it ends:** normally.

## choice_points

1. **Where the generic ring lives.** Records cannot take type parameters, so I kept a concrete `Ring` record for the bookkeeping and passed a `[T]` array beside it to generic functions.
   - **Alternative:** a `record Ring<T>`, or a record with an `items: [T]` field. The compiler refuses this: the `record` grammar has no `Generics`.
   - **Alternative:** writing one record per type. That is not "written once".

2. **Storing items in a record field.** I keep the array as a plain `@` parameter name, not as a field.
   - **Alternative:** a field such as `items: [i64]`. Per the spec, `@` through a field copies the whole array on every push. The program still runs and prints the same lines, but it copies on each add.

3. **Printing inside a generic function.** I return a `[T]` and print in `main`, where the types are concrete.
   - **Alternative:** `print(x)` with `x: T` inside a generic. The spec does not say whether `print` takes a type parameter, so the compiler might refuse it.

4. **`range(from:, to:)` with named arguments.** I used named arguments, which are required because both parameters are `i64`.
   - **Alternative:** `range(1, 6)` is refused.
   - **Alternative:** `to: 5` runs, but adds only 1 to 4 and prints `2 3 4` instead of `3 4 5`.

5. **Positional arguments to `ring_add`.** No two of its parameters share a type (`Ring`, `[T]`, `T`), so labels are optional. If two did share a type, leaving the labels out would be refused.

6. **Copy parameters instead of `lent` in `ring_items`.** I chose plain copy parameters. `lent` would avoid the copy and give the same output; either is accepted.

7. **Where `start` points while the ring fills.** `start` stays at 0 until the ring is full, and new items are appended at index `count`.
   - **Alternative:** writing `items[count]` before pushing. That index is out of bounds and the run aborts.

8. **The empty arrays carry type annotations** (`nums: [i64] @= []`, `words: [str] @= []`).
   - **Alternative:** a bare `[]` without one. It gives no type, so `T` cannot be inferred and the compiler refuses it.

9. **`%` by `r.cap`.** Both caps are 3 and 2, so this never divides by zero. A cap of 0 would abort at run time; the program does not guard against it.

## prediction

The compiler accepts the program.

Trace of the `i64` ring (limit 3):
- Adding 1, 2 and 3 appends them: `[1,2,3]`, `start` = 0.
- Adding 4 overwrites slot 0: `[4,2,3]`, `start` = 1.
- Adding 5 overwrites slot 1: `[4,5,3]`, `start` = 2.
- Read oldest first from slot 2: 3, 4, 5.

Trace of the `str` ring (limit 2):
- Adding "red" and "green" appends them: `[red,green]`.
- Adding "blue" overwrites slot 0: `[blue,green]`, `start` = 1.
- Read oldest first from slot 1: green, blue.

Lines it writes, in order:
```
3
4
5
green
blue
```

It ends normally (no abort).

## context

Only `brief.md`, `task.md` and `heroes-spec.md` from this directory reached my context. No other instruction, rule, contract, memory or document did, apart from the general system and tool instructions and the working-directory path.
~~~~~

### `t4-w-1`, q4-r

~~~~~ markdown
I only have a file-reading tool, so I could not write `report.md` or `main.hero`. As the brief allows, the whole report is below.

# report.md

## answer

**`main.hero`**
```
# Prints, for each word of a fixed text, its first letter, a space and its length in bytes.
function main()
    text = "use the force luke"
    word @= ""
    for c in text.chars()
        if c == " "
            show(w: word)
            word @ ""
        else
            word @ word + c
    show(w: word)

# Prints the first letter of `w`, a space, and the length of `w` in bytes.
function show(w: str)
    print(w.slice(from: 0, to: 1), " ", w.len())
```

**Does `heroes build main.hero` accept it?** Yes, I predict it does.

**Every line it prints:**
```
u 3
t 3
f 5
l 4
```

**How it ends:** normally, after the fourth line, when `main` returns.

## choice_points

1. **Walking characters vs. indexing bytes.** I used `text.chars()` and built each word with `+`. The other way loops over `range(from: 0, to: text.len() + 1)` and tests `text[i] == ' '`. That depends on `len` returning `i64`, and the spec never says what type `len` returns. If it returns `u64`, then `i == text.len()` mixes `i64` and `u64`, and the compiler refuses the program. My version never compares or adds to the result of `len`. It only prints it, and `print` takes any number type except `u8`.
2. **Printing the last word after the loop.** There is no space after the last word, so the loop never prints it. I call `show` once more after the loop. Without that call, the program runs and prints only `u 3`, `t 3`, `f 5`: one line short.
3. **Comparing with `" "` (a `str`) vs. `' '` (a character literal).** `chars()` yields single-character `str`, so I compare with `" "`. Writing `c == ' '` compares a `str` with an integer, and the compiler refuses it.
4. **Getting the first letter with `slice` vs. `w[0]`.** `w[0]` is a `u8`, and `print` refuses a `u8`, so that would not compile. With `to_str(w[0])` it would compile but print `117 3` instead of `u 3`: a different answer. I used `w.slice(from: 0, to: 1)`, which the spec names as the way to write a character. The literals `0` and `1` take whatever type `slice`'s parameters have.
5. **`print` with several arguments vs. an `f` string.** `print` writes its values with no separator, so `print(a, " ", n)` gives `u 3`. The other way, `print(f"{w.slice(from: 0, to: 1)} {w.len()}")`, would print the same lines. Leaving out the `" "` argument would give `u3`: a different answer.
6. **Helper parameter named `w`, not `word`.** Shadowing is a compile error. Different functions should not shadow each other, but the spec does not say so outright, so I avoided the question. A `word` parameter would at worst be refused.
7. **Passing the argument by name (`show(w: word)`) vs. positionally (`show(word)`).** Names are required only when two parameters share a type, and `show` has one parameter, so both forms should be accepted. Either way, `show` gets its own copy of the string.
8. **The empty string.** `word @= ""` gives the cell the type `str` by inference. `""` holds no braces, so the `f`-literal rule does not apply. The cell is re-bound in both branches, so it is not "a cell nothing re-binds". Using `=` instead of `@=` would make `word @ ...` a compile error.
9. **Using `if`/`else` as a statement.** Neither branch ends in an expression, since each ends in a re-binding, so no value is left over to bind or discard. If a branch ended in a value, the compiler would refuse it until that value was bound or discarded with `_ =`.

## prediction

- **Compiler:** accepts it. There are no holes, no unused bindings, no mixed types, no tabs, and only ASCII.
- **Output, in order:**
  1. `u 3`
  2. `t 3`
  3. `f 5`
  4. `l 4`
- **End:** normal termination, with no abort.

## context

The only files from this directory that reached my context are `brief.md`, `task.md` and `heroes-spec.md`. Apart from those, I had only the standard system prompt: it says I am a Claude agent, gives the working directory and date, and describes the file-reading tool. No other project instruction, rule, contract, memory or document reached me.
~~~~~

### `t4-w-2`, q4-r

~~~~~ markdown
I couldn't write `report.md` because I only have a file-reading tool here, so, as the brief allows, the whole report is below.

# report.md

## answer

**Question 1: give the file whole.**

`main.hero`:
```
function main()
    text = "use the force luke"
    word @= ""
    for c in (text + " ").chars()
        if c == " "
            print(word.slice(from: 0, to: 1), " ", word.len())
            word @ ""
        else
            word @ word + c
```

**Question 2: does `heroes build main.hero` accept it?** Yes. Every name is used: `text` and `c` are read, and `word` is read and re-bound. There is no shadowing and no unused value. Indentation is 4 spaces, and no string literal contains braces.

**Question 3: every line it prints.**
```
u 3
t 3
f 5
l 4
```

**Question 4: how it ends.** Normally, when `main` returns, with no abort.

## choice_points

1. **How to split the text.**
   - **My choice:** go through `(text + " ").chars()`, collecting characters into a cell `word`, and print each word when a space arrives. Adding `" "` at the end means the last word is also ended by a space.
   - **The other way:** loop over byte indices with `range(from: 0, to: text.len() + 1)`, test `text[i] == ' '`, and cut each word out with `slice`. The spec never says what type `len` returns, but `range` takes `i64`. If `len` returns a `u64`, `text.len() + 1` mixes types and the compiler refuses the program. I avoided that risk.
   - **Leaving out the added `" "`:** the program would run but never print `l 4`, which is a different answer than intended.
2. **How to print the length.** I pass `word.len()` straight to `print`, which takes any number type except `u8`. That works whatever integer type `len` returns. Putting it inside an `f"..."` literal would also work, unless `len` returned a `u8`; then the compiler would refuse it.
3. **How to print the first letter.** I use `word.slice(from: 0, to: 1)`, which is a `str`.
   - Printing `word[0]` instead would hand `print` a `u8`, and the compiler refuses that.
   - `to_str(word[0])` would print the byte's number, for example `117 3` instead of `u 3`, which is a different answer.
4. **How to test for a space.** I compare `c == " "`, two `str` values. Comparing with the character literal `' '` would compare a `str` with an integer, and the compiler refuses that.
5. **What to do with an empty word.** With single spaces and no leading or trailing space, `word` is never empty when it is printed, so `slice(from: 0, to: 1)` is always in bounds. If a word were empty, that slice would abort at run time. The task rules this case out, so I added no guard.
6. **How to clear the cell.** After printing I re-bind it with `word @ ""`. Without that reset the words would run together (`t 6` for "usethe"), which is a different answer.

## prediction

The compiler accepts the program. It prints these lines, in order:
```
u 3
t 3
f 5
l 4
```
It then ends normally, when `main` returns, with no abort.

## context

Only `brief.md`, `task.md` and `heroes-spec.md` from this directory reached my context. Apart from those files, the only instructions were the general agent setup (working directory, date, how to use the file-reading tool) and the user's request to follow the brief. No other rules, memories or documents about the language reached me.
~~~~~

### `t4-w-3`, q4-r

~~~~~ markdown
I only have a Read tool, so I couldn't create `report.md`. As the brief allows, the whole report is below.

# report.md

## answer

**The file, `main.hero`:**

```
function show(word: str)
    print(word.slice(from: 0, to: 1), " ", word.len())

function main()
    text = "use the force luke"
    word @= ""
    for c in text.chars()
        if c == " "
            show(word)
            word @ ""
        else
            word @ word + c
    show(word)
```

**Does `heroes build main.hero` accept it?** Yes, I predict it does. Every binding is used, the cell `word` is re-bound, the `print` arguments are a `str`, a `str` and an integer (not a `u8`), and both calls to `show` return `()` and stand alone on their lines.

**What it prints:**

```
u 3
t 3
f 5
l 4
```

**How it ends:** normally, exit status 0, with no abort.

## choice_points

1. **How to split the text.** The spec has no `split` built-in.
   - **My choice:** walk `text.chars()` and build each word with `+`, printing it when a `" "` arrives and once more after the loop.
   - **The other option:** walk the bytes with `range(from: 0, to: text.len() + 1)` and `text[i] == ' '`, and cut with `slice`. This depends on the type `len()` returns, which the spec never states. If it is anything but `i64`, `i == text.len()` and `range(..., to: text.len() + 1)` mix two integer types, and the compiler refuses the program. My version never compares or adds to `len()`. It only prints it, and `print` takes any number except `u8`.
2. **What type `len()` returns.** I left it open on purpose. `print` takes it whether it is `i64` or `u64`. The integer literals given to `slice(from:, to:)` take whatever type the parameters ask for (section 2).
3. **Chars or bytes for the length.** I print `word.len()`, which counts bytes as the task asks. Counting the chars with my own counter would give the same answer here, since the text is ASCII, but a different one for non-ASCII text.
4. **How to get the first letter.**
   - **My choice:** `word.slice(from: 0, to: 1)`, a `str`.
   - **The other option:** `word[0]` is a `u8`. `print` refuses a `u8`, so the compiler refuses the program. `to_str(word[0])` would print `117` instead of `u`, which runs but gives the wrong answer.
5. **The last word.** It has no space after it, so `show(word)` is called once more after the loop. Without that call, `l 4` is never printed, which runs but gives the wrong answer.
6. **Empty words.** The task says single spaces, so no word is ever empty. If one were, `slice(from: 0, to: 1)` on `""` would go out of bounds, and the program would stop with an error when run. I didn't add a guard.
7. **Named arguments for `slice`.** `from:` and `to:` share a type, so the names are required (section 9). Leaving them out makes the compiler refuse the program.
8. **Comparing to `" "`.** `c` is a one-character `str`, so I compare it to the string `" "`. Comparing it to the character `' '`, which is an integer, gives a type mismatch, and the compiler refuses the program.
9. **A helper instead of two `print` lines.** `show` takes a plain copy parameter, not `lent` or `@`. Repeating the `print` inline would behave the same.

## prediction

- **Compiler:** accepts the program.
- **Output, in order:** `u 3`, `t 3`, `f 5`, `l 4`.
- **How it ends:** normally, exit status 0, with no abort.

## context

Besides `brief.md`, `task.md` and `heroes-spec.md` in this directory, my context held only the system prompt. It named the working directory and the date, and said I am an agent on the Claude Agent SDK. No other instruction, rule, contract, memory or document from the project reached me, and I read no other file.
~~~~~

### `t4-w-4`, q4-r

~~~~~ markdown
I could not write `report.md` or `main.hero`: the only tool I have reads files and cannot write them. As `brief.md` allows, the whole report is below.

# report.md

## answer

**Question 1: give the file whole.**

`main.hero`:
```
function main()
    text = "use the force luke"
    start @= 0
    for i in range(from: 0, to: text.len() + 1)
        if i == text.len() || text[i] == ' '
            word = text.slice(from: start, to: i)
            print(word.slice(from: 0, to: 1), " ", word.len())
            start @ i + 1
```

**Question 2: does `heroes build main.hero` accept it?** Yes. It is one module with `function main()` taking nothing and producing nothing. It has no `use`, so it needs no `heroes.toml`. Every binding is used: `text`, `i` and `word` are read, and the program both reads and re-binds the cell `start`. The string literal holds no braces, so nothing in it reads as a hole.

**Question 3: every line it prints, in order:**
```
u 3
t 3
f 5
l 4
```

**Question 4: how it ends.** Normally, when `main` returns, with no abort.

How it runs: `i` goes from 0 to `len` inclusive. At each space, and at the position just past the end, it slices out the word `[start, i)` and prints the word's first character as a one-byte slice, then a space, then its byte length. It then moves `start` past the space. When `i == text.len()`, `||` short-circuits, so the out-of-bounds read `text[i]` is never evaluated.

## choice_points

1. **The type `len()` returns.** The spec never states it. I assumed `i64`, the default integer type and the element type of `range`.
   - If it returns `u64` (or another type), the compiler refuses the program in two places: `range(from: 0, to: text.len() + 1)` and `i == text.len()`. Both mix integer types, and the spec allows no implicit conversions.
   - The alternative was to avoid `len` and count with an `i64` counter over `text.chars()`. That counts characters, not bytes. It would print the same lines for this ASCII text but would not do what the task asks.

2. **Splitting words by byte index versus by `chars()`.** I chose byte indices with `slice`, because the task asks for length in bytes. A `chars()` loop with a counter compiles and prints the same four lines here, but gives a different answer for any non-ASCII word.

3. **How to print the first letter.** I chose `word.slice(from: 0, to: 1)`, a `str`.
   - `print(word[0], ...)`: the compiler refuses it, because `print` does not take a `u8`.
   - `f"{word[0]}"`: refused for the same reason.
   - `to_str(word[0])`: compiles, but prints `117 3` instead of `u 3`.

4. **The order of the condition `i == text.len() || text[i] == ' '`.** I put the end-of-text test first.
   - In the reverse order, `text[i]` is evaluated when `i == len`, and the program aborts with an out-of-bounds index after printing `u 3`, `t 3` and `f 5`.
   - Leaving out the end test and using `range(to: text.len())` compiles and runs, but never prints `l 4`, unless a separate flush after the loop is added.

5. **The type of the character literal `' '`.** It takes its type from the context, so it becomes `u8` to match `text[i]`. Writing `" "` (a `str`) instead would be refused: `==` compares only two values of one type.

6. **Several arguments to `print` versus an `f` literal.** I passed several arguments, which `print` joins with no separator. `print(f"{word.slice(from: 0, to: 1)} {word.len()}")` would print the same lines. Passing a `u8` in either form would be refused.

7. **Using a mutable cell for `start`.** If `start` were never re-bound, the compiler would refuse it, since a cell nothing re-binds is an error. Writing `start = 0` with `=` and then `start @ ...` would also be refused, because `@` re-binds only a cell.

## prediction

The compiler accepts the program, provided `len()` returns `i64` (choice point 1). It prints:
```
u 3
t 3
f 5
l 4
```
It ends normally, when `main` returns. There is no abort.

## context

Besides `brief.md`, `task.md` and `heroes-spec.md`, the only thing that reached my context was the agent environment's system prompt. It said I am a Claude agent on the Claude Agent SDK, and gave the working directory path and the date (2026-11-19). It held no rules about the language. I read no other file, memory or document.
~~~~~

### `t4-w-5`, q4-r

~~~~~ markdown
I only have a file-reading tool here, so I couldn't write `report.md`. As the brief allows, the whole report is below.

# report.md

## answer

**Question 1: the file.**

`main.hero`:
```
function main()
    text = "use the force luke"
    word @= ""
    for c in text.chars()
        if c == " "
            print_word(word)
            word @ ""
        else
            word @ word + c
    print_word(word)

function print_word(w: str)
    print(w.slice(from: 0, to: 1), " ", w.len())
```

**Question 2: does `heroes build main.hero` accept it?** Yes.

**Question 3: every line it prints.**
```
u 3
t 3
f 5
l 4
```

**Question 4: how it ends.** Normally: `main` returns and the exit status is 0.

## choice_points

1. **How to split the text into words.** The spec has no `split`. I went through `text.chars()`, which gives one-character `str`s, and built each word with `+`.
   - **Other choice:** step through byte indices with `text[i]`, which gives a `u8`, and use a `while` loop on `i` and `text.len()`.
   - **What it would produce:** the spec never says what type `len` returns. If it is `u64`, then `i @= 0` (an `i64`) compared with `text.len()` mixes widths, and **the compiler refuses the program**. My version avoids comparing against `len`, so that question never comes up.

2. **Comparing a character with the space.** I compared `c == " "`, a `str` against a `str`.
   - **Other choice:** `c == ' '`. A character literal is an integer, so this compares a `str` with an integer and **the compiler refuses the program**.

3. **Getting the first letter.** I used `w.slice(from: 0, to: 1)`, which is a `str`.
   - **Other choice:** `w[0]` inside `print` or an `f` literal. That is a `u8`, which `print` and `f` literals refuse, so **the compiler refuses the program**.
   - **Another option:** `to_str(w[0])` compiles, but it prints the number, e.g. `117 3`. The program **runs and gives a different answer than intended**.

4. **Separators in `print`.** I passed `" "` as its own argument, because `print` adds no separator.
   - **Other choice:** `print(first, w.len())` prints `u3`. The program **runs and gives a different answer than intended**.

5. **The last word.** I call `print_word(word)` once more after the loop, because the text doesn't end with a space.
   - **Other choice:** leave that call out. The program **runs and gives a different answer**: it prints only three lines and `l 4` is missing.

6. **The length.** I used `w.len()`, which the spec says measures `str` in bytes.
   - **Other choice:** count the characters from `chars()` in an `i64` cell. That gives the same answer here, since the text is all ASCII, but it counts characters, not bytes.

7. **The `print_word` call.** It returns `()`, so its line stands alone.
   - **Other choice:** `_ = print_word(word)`. A `()` line refuses this, so **the compiler refuses the program**.

8. **Where `print_word` is declared.** I put it after `main`. Declaration order doesn't matter, so placing it before `main` gives the same result.

## prediction

The compiler accepts the program. Nothing is left unused: `text`, `word`, `c` and `w` are all read, and the cell `word` is re-bound. Nothing is shadowed or tab-indented, and there are no holes.

It prints these lines, in order:
```
u 3
t 3
f 5
l 4
```
It ends normally, with exit status 0 and no abort.

## context

Only the system prompt reached my context. It says I am a Claude agent built on Anthropic's Claude Agent SDK, and gives the working directory and the date. Apart from that, I read only `brief.md`, `task.md` and `heroes-spec.md` from this directory. No other instruction, rule, contract, memory or document reached my context.
~~~~~

### `t4-y-1`, now

~~~~~ markdown
I couldn't write `report.md` because I only have a file-reading tool here, so the whole report is below as the brief allows.

# report.md

## answer

**The file, `main.hero`:**

```
function main()
    text = "use the force luke"
    word @= ""
    words: [str] @= []
    for c in text.chars()
        if c == " "
            words @ words.push(word)
            word @ ""
        else
            word @ word + c
    words @ words.push(word)
    for w in words
        print(w.slice(from: 0, to: 1), " ", w.len())
```

**Does `heroes build main.hero` accept it?** Yes, I predict it does.
- Every binding is used, and both cells (`word` and `words`) are re-bound.
- The empty array has a type annotation.
- No line computes a value and then leaves it unused: `print` returns `()`, and every other line binds or re-binds a cell.
- No `T?` is dropped.
- Indentation is 4 spaces per level.

**What it prints:**
```
u 3
t 3
f 5
l 4
```

**How it ends:** normally, when `main` returns. Exit status 0 is implied; the spec does not state it.

## choice_points

1. **How to split the text.** I built the words one character at a time from `text.chars()`, comparing each `str` with `" "`.
   - Other choice: index bytes with `text[i]` and compare with `' '`. That gives the same output, but it needs a `range` over `text.len()`. If `len` returns a `u64`, mixing it with `i64` indices would be refused at compile time.
2. **How to get the first letter.** I used `w.slice(from: 0, to: 1)`. Section 10 mentions a slice that splits a character, which suggests `slice` works on `str`.
   - Other choice: `w.chars()[0]`, which gives the same answer.
   - Wrong choice: `w[0]` gives a `u8`, so it would print `117` instead of `u`. The program would run but give a different answer than intended.
3. **How to format the line.** I used `print(a, " ", b)`, relying on "no separator" between values.
   - Other choice: `print(f"{w.slice(from: 0, to: 1)} {w.len()}")` gives the same output.
   - Wrong choice: writing that string without the leading `f` is a compile error (section 2).
4. **How to grow the arrays.** I wrote `words @ words.push(word)`, the form shown in section 10.
   - Wrong choice: calling `words.push(word)` on a line of its own would compute a value and drop it, which the compiler refuses.
5. **The last word.** I push the last word after the loop.
   - If I left that out, the program would run but omit `l 4`, giving a different answer than intended.
6. **The type of `len`.** The spec does not say what type it returns. `print` accepts any number, and the `slice` arguments are literals that take whatever type the context asks for, so either `i64` or `u64` works here.

## prediction

- **Compiler:** accepts the program.
- **Lines written, in order:** `u 3`, `t 3`, `f 5`, `l 4`.
- **How it ends:** normally, with no abort.

## context

Besides the files in this directory (`brief.md`, `task.md`, `heroes-spec.md`), only the user's instruction to follow the brief and the agent's system/environment information (working directory and date) reached my context. No other rule, contract, memory or document did.
~~~~~

### `t4-y-2`, now

~~~~~ markdown
I can't write `report.md` because I only have a tool for reading files, not for writing them. As the brief allows, the whole report is below.

---

# report.md

## answer

**The file.** `main.hero`:

```
# Prints, for each word of a fixed text, its first letter and its length in bytes.
function main()
    text = "use the force luke"
    word @= ""
    for c in text.chars()
        if c == " "
            print(word.slice(from: 0, to: 1), " ", word.len())
            word @ ""
        else
            word @ word + c
    print(word.slice(from: 0, to: 1), " ", word.len())
```

**Does `heroes build main.hero` accept it?** Yes. `main` takes nothing and returns nothing. Indentation is 4 spaces per level. `text` is read, and the cell `word` is both read and re-bound. `c` is a `str` (`chars()` yields single-character `str`), so `c == " "` compares two strings. Both `print` calls stand alone as `()` lines, and `print` accepts a `str` and a number.

**Every line it prints:**
```
u 3
t 3
f 5
l 4
```

**How it ends:** normally, when `main` returns. No abort happens: each word is at least one byte long, so `slice(from: 0, to: 1)` is never out of bounds.

## choice_points

1. **How to split the text.** The specification has no `split`. I went through `text.chars()` and built each word with `word @ word + c`. The other way is to walk byte indices with `range` and `slice` on `text`. That way depends on what type `len()` returns, which the specification doesn't say. If it returns `u64`, then `range(from: 0, to: text.len() + 1)` (which takes `i64`) and the arithmetic mixing the index with the start position would be refused by the compiler. My way never mixes `len()` with another number; it only prints it, and `print` takes any number type.
2. **Comparing with `" "` (a string) or `' '` (a character).** I compared with `" "`. A character literal is an integer, so `c == ' '` would compare a `str` with an integer, and the compiler would refuse it. If I had worked on bytes instead (`text[i]`, which is a `u8`), `' '` would be the right choice and `" "` would be refused.
3. **The last word.** No space follows the last word, so I print it again after the loop. Leaving out that last `print` gives a program that runs but prints only three lines (`u 3`, `t 3`, `f 5`) and loses `l 4`. That's a wrong answer, not an error.
4. **How to get the first letter.** I used `word.slice(from: 0, to: 1)`, which is a `str`. Writing `word[0]` instead gives a `u8`. `print` writes that as a number, so the line would be `117 3`: the program runs, but the answer is wrong. Taking `chars()` of the word and indexing the result would also work, but it costs more.
5. **How to format the line.** I passed several values to `print`, which adds no separator between them, so I put the space in myself. The other way, `print(f"{word.slice(from: 0, to: 1)} {word.len()}")`, prints the same lines. Leaving out the `" "` argument gives `u3`, a wrong answer.
6. **The types of the literals in `slice`.** I wrote `0` and `1` without types and relied on section 2: a literal takes the type its context asks for, here the type of `slice`'s parameters. Writing an explicit type that doesn't match those parameters would be a compile error.
7. **Joining words with `+`.** The specification warns that a loop of concatenations is quadratic. With 4-letter words that cost doesn't matter, and the output is the same either way.

## prediction

- **Compiler:** accepts the program.
- **Output, in order:** `u 3`, `t 3`, `f 5`, `l 4`.
- **Ending:** normal, when `main` returns. There is no abort and the exit code is 0.

## context

Besides `brief.md`, `task.md` and `heroes-spec.md` in this directory, my context held only the system prompt and the user's message. The system prompt identified me as an agent built on the Claude Agent SDK and gave the working directory and the date. The user's message told me to follow `brief.md` and use only these files. No other rules, contracts, memories or documents reached my context, and I read no other file. I also tried to read `report.md` to check whether it already existed; it didn't.
~~~~~

### `t4-y-3`, now

~~~~~ markdown
I only have a file-reading tool, so I couldn't write `report.md` or `main.hero`. As the brief allows, the whole report is below.

# report.md

## answer

**`main.hero`**
```
# Prints each word of a fixed text as its first letter, a space, and its length in bytes.
function main()
    text = "use the force luke"
    first @= ""
    word @= ""
    for c in text.chars()
        if c == " "
            print(first, " ", word.len())
            first @ ""
            word @ ""
        else
            if word == ""
                first @ c
            word @ word + c
    print(first, " ", word.len())
```

**Does `heroes build main.hero` accept it?** Yes, I predict it does. Here is why:
- Every binding is used.
- Both cells (`first`, `word`) are re-bound with `@`.
- Each `print` line produces `()` and stands alone.
- The `if` statements take a `bool`.
- Indentation is 4 spaces, and no string contains braces.

**Lines printed:**
```
u 3
t 3
f 5
l 4
```

**How it ends:** normally, after the fourth line.

## choice_points

1. **How to split the text.** I went through `text.chars()` and built each word as a `str`. The other way was to index bytes with `text[i]` over `range(from: 0, to: text.len() + 1)` and cut words with `slice`.
   - The spec never says what type `len()` returns or what type `slice` and `range` take.
   - If `len()` returned `u64` while `range` takes `i64`, the compiler would refuse the program.
   - My version never does arithmetic on `len()`. It only prints it, and `print` accepts any number.
2. **How to get the first letter.** I saved the first character as a `str` from `chars()`. The other option, `word[0]`, gives a `u8`, so `print` would write `117 3` instead of `u 3`. That runs, but gives the wrong answer. `word.slice(from: 0, to: 1)` would also work, but it has the same unknown index type as point 1.
3. **Comparing with a space.** I compared `c == " "` (`str` with `str`). Comparing a `str` with the character literal `' '`, which is an integer, would be refused as mixing types.
4. **The last word.** It has no space after it, so I print it with the final `print` after the loop. Without that line, the program would run and print only three lines, leaving out `l 4`.
5. **How to print the line.** I passed three arguments to `print`, which writes them with no separator. An interpolated string, `print(f"{first} {word.len()}")`, would print the same thing. Writing braces without the `f` would be refused.
6. **Building the word.** I used `word @ word + c`. The spec says this is quadratic, but it doesn't matter for 18 bytes. Using `join` instead would give the same output.

## prediction

The compiler accepts the program. It prints these lines, in order:
```
u 3
t 3
f 5
l 4
```
It ends normally, with no abort.

## context

Only `brief.md`, `task.md` and `heroes-spec.md` from this directory reached my context. I saw no other instruction, rule, contract, memory or document, apart from the general system setup (the working directory and date).
~~~~~

### `t4-y-4`, now

~~~~~ markdown
I couldn't write `report.md` because I only have a file-reading tool, so the whole report follows here, as the brief allows.

# report.md

## answer

**Program.** The file is `main.hero`:

```
# Prints, for each word of the text, its first letter, a space, and its length in bytes.
function main()
    text = "use the force luke"
    word @= ""
    for c in text.chars()
        if c == " "
            emit(word)
            word @ ""
        else
            word @ word + c
    emit(word)

# Writes one line: the word's first letter, a space, and the word's length in bytes.
function emit(word: str)
    print(word.slice(from: 0, to: 1), " ", word.len())
```

**Does `heroes build main.hero` accept it?** Yes, I predict it does.
- Every name is used, and the cell `word` is re-bound with `@`.
- `emit` returns `()`, so its call is a line on its own, which the spec allows.
- `==` compares a `str` with a `str`, and `+` joins two `str`.
- The literals `0` and `1` take whatever type `slice` asks for.
- No literal holds braces, and nothing shadows another name.
- There are no tabs, and every indent is 4 spaces.

**Lines it prints:**
```
u 3
t 3
f 5
l 4
```

**How it ends:** normally, after `main` returns. There is no abort.

## choice_points

1. **How to split the text: build each word from `chars()` or work with byte indices.** I built each word one character at a time. The other way would be `for i in range(from: 0, to: text.len() + 1)` with `text[i]` and `slice`. That depends on the type `len()` returns, which the spec never states. `range` gives `i64`, so if `len()` returned `u64`, then `i == text.len()` would mix types and the compiler would refuse it. My version only passes `len()` to `print`, which takes any number.
2. **How to get the first letter: `word.slice(from: 0, to: 1)`, `word.chars()[0]`, or `word[0]`.** I chose `slice`. `chars()[0]` would give the same output. `word[0]` is a `u8`, so it would print `117 3` instead of `u 3`, a different answer. Writing `f"{word[0]}"` would have the same problem.
3. **How to compare the character: `c == " "` or `c == ' '`.** I chose the string `" "`. `c` is a `str`, and `' '` is an integer, so the other form compares two different types and the compiler refuses it.
4. **How to build the line: several arguments to `print`, an `f` string, or `+`.** I passed several arguments to `print`, which adds no separator between them. `print(f"{word.slice(from: 0, to: 1)} {word.len()}")` would print the same lines. `first + " " + word.len()` adds a `str` to a number, which the compiler refuses. Passing `" "` as an argument is needed: leaving it out prints `u3`, a different answer.
5. **Whether to print the last word after the loop.** I call `emit(word)` after the loop. Without that call there is no space after `luke`, so it compiles and runs but prints only three lines and drops `l 4`, a different answer.
6. **How to call a function that returns nothing: on its own line or as `_ = emit(word)`.** I wrote it on its own line. The spec says a `()` line "stands alone", so `_ =` is refused by the compiler.
7. **Whether to use a helper function or repeat the `print` line twice.** I used the helper `emit`. Repeating the line would produce the same output; it is only less clean.
8. **Empty words.** The task says words are separated by single spaces, so I added no guard. If the text had a double space or a leading space, `slice(from: 0, to: 1)` on `""` would go out of bounds, and the program would stop with an error when run. With this text that cannot happen.
9. **Building words with `word @ word + c` or by pushing to an array and calling `join`.** I used `+`. The spec says repeated `+` is quadratic, but it changes nothing about the output for words this short.

## prediction

- **Compiler:** accepts the program.
- **Lines printed, in order:**
  1. `u 3`
  2. `t 3`
  3. `f 5`
  4. `l 4`
- **Ending:** normally, when `main` returns. There is no abort and no hang.

## context

Besides `brief.md`, `task.md` and `heroes-spec.md` in this directory, only two other things reached my context: the agent framework's standing instructions (that I am a Claude Agent SDK agent, the working-directory path and the date) and the user's request to follow the brief. I read no other file and received no project rules, contracts, memories or other documents.
~~~~~

### `t4-y-5`, now

~~~~~ markdown
I couldn't write `report.md` or `main.hero` because I only have a file-reading tool here. The brief allows for this, so the whole report follows as my answer.

# report.md

## answer

**Question 1: give the file whole.**

`main.hero`:
```
function main()
    text = "use the force luke"
    words: [str] @= []
    word @= ""
    for c in text.chars()
        if c == " "
            words @ words.push(word)
            word @ ""
        else
            word @ word + c
    words @ words.push(word)
    for w in words
        print(w.slice(from: 0, to: 1), " ", w.len())
```

**Question 2: does `heroes build main.hero` accept it?** Yes, I predict it does.

**Question 3: every line it prints, in order:**
```
u 3
t 3
f 5
l 4
```

**Question 4: how it ends.** It ends normally with no abort.

## choice_points

1. **How to split the text.** The spec has no `split` built-in. I loop over `text.chars()`, which gives one-character `str`s, and build each word with `+`.
   - The other way is to loop over byte indices with `text[i] == ' '`. That needs `range(from: 0, to: text.len())`, and the spec never says what type `len()` returns. If it isn't `i64`, the compiler refuses the program because `range` takes `i64`.
   - Calling a `split` that doesn't exist would also be refused.
   - Building words with `+` takes quadratic time (section 10). That doesn't matter for text this short.
2. **Comparing the character.** `chars()` yields `str`, so I compare with the string `" "`. Comparing a `str` with the character literal `' '` (an integer) would be refused, because mixing types is not allowed.
3. **The last word.** There is no space after `luke`, so I push the last word after the loop. Without that push the program would run and print only three lines, with `l 4` missing.
4. **The empty array.** I wrote `words: [str] @= []` with a type annotation. Section 10 requires one, so leaving it out would be refused.
5. **Updating arrays and strings.** `words @ words.push(word)` follows section 10's `xs @ xs.push(4)`, so I'm assuming `push` returns the new array. If `push` returned nothing (`()`), this line would be refused.
6. **Getting the first letter.** I used `w.slice(from: 0, to: 1)`, which returns a `str` that `print` can take.
   - `w[0]` is a `u8`, so it would run and print `117 3` instead of `u 3`.
   - `w.chars()[0]` would also work.
   - Every word here is non-empty, so the slice cannot go out of bounds. An empty word (from two spaces in a row) would abort.
7. **The separator in `print`.** `print` puts nothing between its values, so I pass `" "` myself. Without it the program would run and print `u3`.
8. **Printing the length.** I pass `w.len()` straight to `print`, which accepts any number type. The program doesn't depend on which integer type `len` returns.
9. **The `if` statement.** I used `if`/`else` as a statement, and each branch only rebinds a cell. Neither branch produces a value, so nothing needs to be discarded with `_ =`.

## prediction

I expect the compiler to accept it: every binding and cell is used, and every cell is rebound. It prints these four lines in order, then ends normally (exit with no abort):
```
u 3
t 3
f 5
l 4
```

## context

Apart from the files in this directory (`brief.md`, `task.md`, `heroes-spec.md`), the only thing in my context was the system setup. That setup gave the working directory and date, and said I'm an agent with a file-reading tool. No other rule, contract, memory or document about the language reached me, and I read no other file.
~~~~~

### `t5-x-1`, q5-c

~~~~~ markdown
I can't write `report.md` because the only tool I have is Read, which can't create files. As `brief.md` allows in that case, the whole report follows.

---

# report.md

## answer

**The file.** `heroes build main.hero` should accept it. It prints `true`, then `false`, and ends normally.

`main.hero`:
```
constant EPSILON: f64
    0.000000001

# Whether `a` and `b` differ by less than `EPSILON`.
function near(a: f64, b: f64) -> bool
    return a - b < EPSILON && b - a < EPSILON

function main()
    print(near(a: 0.1 + 0.2, b: 0.3))
    print(near(a: 1.0, b: 1.001))
```

**Does `heroes build main.hero` accept it?** Yes (see `prediction`).

**What does it print?**
```
true
false
```

**How does it end?** Normally: `main` returns, there is no abort and the exit code is 0.

## choice_points

1. **How to write 1e-9.** Section 2 says a float has "digits on both sides of its point and no exponent". I wrote `0.000000001`. Writing `1e-9` or `1.0e-9` would make the compiler refuse the program.
2. **Absolute value.** Section 11's list of built-ins has no `abs`. I tested both differences, `a - b < EPSILON && b - a < EPSILON`, which together mean |a − b| < ε.
   - Calling `abs(...)` would make the compiler refuse the program (unknown name).
   - Testing only `a - b < EPSILON` would compile, but it gives the wrong answer: `near(1.0, 1.001)` would print `true`, because −0.001 < ε.
   - Using `if d < 0.0` to negate `d` would also work. It needs a named binding `d` and an `if` expression with an `else` on both branches. I chose the shorter form.
3. **Labelled arguments.** Both parameters are `f64`, so section 9 makes named arguments mandatory: `near(a: …, b: …)`. Calling `near(0.1 + 0.2, 0.3)` without labels would make the compiler refuse the program. The UFCS form `(0.1 + 0.2).near(b: 0.3)` might also be refused, since the first argument would have no label. I avoided it.
4. **Strict `<` or `<=`.** The task says "less than", so I used `<`. With `<=` the program would compile and print the same two lines here, but it would answer differently for a difference of exactly 1e-9.
5. **Where ε lives.** I made it a named constant in SCREAMING_CASE, with its body on the indented line below, as section 4 requires. Writing it inline would also work, because the literal would take `f64` from the other side of `<`. A lowercase constant name would make the compiler refuse the program.
6. **Types of the literals.** Section 2 says a literal takes its type from context, "otherwise `i64`", and doesn't say what a float literal does with no context. Every float literal here has an `f64` context: the parameters, the constant's declared type, or the other operand of `+`. So nothing depends on that gap. Mixing in an integer literal, such as `1` in place of `1.0`, would make the compiler refuse the program because there are no implicit conversions.
7. **How to print.** Section 11 says `print` takes a `bool` directly and writes `true` or `false`. I didn't use an f-string. It would give the same output.
8. **Doc comment.** I added a `#` comment above `near`. It is optional and has no effect on the program.
9. **NaN.** Section 7 says ordering a `nan` aborts, so `near` would abort if given one. The task's inputs aren't NaN, so this doesn't come up here.

## prediction

- **Compiler:** accepts it. Every binding and parameter is used, the constant is used, there are no holes, and the indentation is 4 spaces with no tabs. `near` returns on its only path, and `main` takes nothing and returns nothing.
- **Output, in order:**
  ```
  true
  false
  ```
  - In IEEE `f64`, 0.1 + 0.2 = 0.30000000000000004. The difference from 0.3 is about 5.6e-17, well under 1e-9, so the first line is `true`.
  - |1.0 − 1.001| ≈ 0.001, which is not under 1e-9, so the second line is `false`.
- **End:** normal, with no abort.

## context

Besides `brief.md`, `task.md` and `heroes-spec.md` in this directory, the only other things in my context were the harness's own instructions. Those were the system prompt describing me as an agent with a Read tool, the working-directory path and date, and the user's message telling me to follow `brief.md` and write the report in English. No other project files, rules, contracts, memories or documents about Heroes reached my context.
~~~~~

### `t5-x-2`, q5-c

~~~~~ markdown
I couldn't write `report.md` because I only have a file-reading tool, so the whole report is below as the brief allows.

# report.md

## answer

**The file, `main.hero`:**

```
constant EPSILON: f64
    0.000000001

function near(a: f64, b: f64) -> bool
    return a - b < EPSILON && b - a < EPSILON

function main()
    print(near(a: 0.1 + 0.2, b: 0.3))
    print(near(a: 1.0, b: 1.001))
```

**Does `heroes build main.hero` accept it?** Yes, as far as I can tell from the specification.

**Every line it prints:**

```
true
false
```

**How it ends:** normally, when `main` returns. There are no aborts, holes, leases or handles.

Why these answers:
- **First line:** in f64, 0.1 + 0.2 is 0.30000000000000004. It differs from 0.3 by about 5.6e-17. Both `a - b` and `b - a` are below 1e-9, so it prints `true`.
- **Second line:** 1.0 − 1.001 is about −0.001, which is below 1e-9. But 1.001 − 1.0 is about 0.001, which is not, so the `&&` is false.
- **How `print` renders it:** §11 says a `bool` prints as `true` or `false`, and `print` adds exactly one newline.

## choice_points

1. **How to write 1e-9.** Section 2 says a float has "digits on both sides of its point and no exponent". I wrote `0.000000001`.
   - Writing `1e-9` would be a program the compiler refuses: it is not a valid float literal.
2. **Absolute value.** Section 11 lists no `abs` built-in. I tested both differences, `a - b < EPSILON && b - a < EPSILON`.
   - Calling `abs(a - b)` would be refused as an unknown name.
   - Writing my own `abs` with an `if` would work the same, at the cost of one more function.
   - Testing only `a - b < EPSILON` would compile but give a different answer than intended: `near(1.0, 1.001)` would print `true`.
   - The variant `d = a - b`, then `d < EPSILON && -d < EPSILON`, would behave the same.
3. **Named arguments.** Both parameters are `f64`, so §9 requires names at the call site. I wrote `near(a: …, b: …)`.
   - A positional call `near(0.1 + 0.2, 0.3)` would be refused.
4. **Constant or inline tolerance.** I used `constant EPSILON: f64` with a literal body. This is allowed by §4: SCREAMING_CASE, and the body is computed from literals.
   - Writing `0.000000001` inline twice would behave the same.
   - Naming it `epsilon` would break the SCREAMING_CASE rule for constants, which I take to be refused.
5. **`<` or `<=`.** The task says "less than", so I used `<`.
   - `<=` gives the same output here, but differs exactly at a gap of 1e-9.
6. **Float type.** I used `f64` everywhere. The literals take `f64` from their context: the parameter types and the operand type of `+`.
   - Using `f32` for parameters would be refused against the task's `f64`, or would change the precision.
   - Mixing an integer literal in, such as `1` instead of `1.0`, would be refused: §7 allows no mixed types, and a float needs digits on both sides of its point.
7. **What to print.** I passed the `bool` straight to `print`, which writes `true` or `false`.
   - Printing text such as `"yes"` would need an `if` expression, and the output would differ.

## prediction

- **Compiler:** accepts the program.
- **Output, in order:**
  1. `true`
  2. `false`
- **End:** normal exit when `main` returns. No abort.

## context

Only `brief.md`, `task.md` and `heroes-spec.md` from this directory reached my context. The others were the agent-environment setup: the system prompt, the tool definitions, and your instruction to follow the brief. None of it contained any rule, contract, memory or document about the Heroes language.
~~~~~

### `t5-x-3`, q5-c

~~~~~ markdown
I couldn't create `report.md` because I can only read files here, not write them. As the brief allows, the whole report is below.

# report.md

## answer

**The file, `main.hero`:**

```
# The largest difference two numbers may have and still count as near.
constant EPSILON: f64
    0.000000001

# Whether `a` and `b` differ by less than EPSILON.
function near(a: f64, b: f64) -> bool
    return (a - b < EPSILON) && (b - a < EPSILON)

function main()
    print(near(a: 0.1 + 0.2, b: 0.3))
    print(near(a: 1.0, b: 1.001))
```

**Does `heroes build main.hero` accept it?** Yes.

**Every line it prints:**
```
true
false
```

**How it ends:** normally, with no abort.

## choice_points

1. **How to write 1e-9.** The spec says a float has "digits on both sides of its point and no exponent". So I wrote `0.000000001`. Writing `1e-9` or `1.0e-9` would be refused by the compiler.
2. **Constant or inline literal.** I used a constant, named in SCREAMING_CASE as the spec requires, with its value on an indented line below. Writing the literal inline in `near` would give the same output. A lowercase constant name such as `eps` would be refused by the compiler.
3. **Absolute value.** The spec lists no `abs` built-in. I tested both directions instead: `a - b < EPSILON && b - a < EPSILON`.
   - Calling `abs(a - b)` would be refused, because the name is unknown.
   - Testing only `a - b < EPSILON` would compile but give the wrong answer. Every case where `a < b` would come out `true`, so `near(a: 1.0, b: 1.001)` would print `true` instead of `false`.
   - A version with `d = a - b` and `-d` would behave the same as mine.
4. **Named arguments.** Both parameters are `f64`. When two parameters share a type, named arguments are required at the call site, so I wrote `near(a: …, b: …)`. A positional call like `near(0.1 + 0.2, 0.3)` would be refused by the compiler.
5. **`<` or `<=`.** The task says "less than", so I used `<`. With these inputs `<=` would print the same lines. It would only differ when the gap is exactly the threshold.
6. **Type of the float literals.** "A literal takes the type its context asks for", and the parameter `a: f64` gives `0.1 + 0.2` the type `f64`. The spec's fallback "otherwise `i64`" makes no sense for a float literal, but no literal here lacks a context. Adding conversions like `to_f64(0.1)` would not change the result.
7. **Parentheses around the comparisons.** They are optional, because comparisons bind more tightly than `&&` and `-` binds more tightly than `<`. Leaving them out gives the same program.
8. **Breaking lines.** I kept every statement on one line. Breaking the `&&` expression across two lines outside brackets would be refused by the compiler.

## prediction

- **Compiler:** accepts the program. Every binding and parameter is used, there are no shadowed names, `main()` takes nothing and returns nothing, and `near` returns on its only path.
- **Line 1:** `true`. In 64-bit floating point, 0.1 + 0.2 is 0.30000000000000004. Its difference from 0.3 is about 5.55e-17, which is below 1e-9 in both directions.
- **Line 2:** `false`. The difference is about 0.001, so `b - a < EPSILON` fails.
- **Ending:** the program ends normally with exit status 0. There is no abort: nothing is NaN, so the ordering comparisons don't abort.

## context

The only files that reached my context were `brief.md`, `task.md` and `heroes-spec.md` in this directory. The path of the working directory itself was in my context, but no other instruction, rule, contract, memory or document was.
~~~~~

### `t5-x-4`, q5-c

~~~~~ markdown
I couldn't save `report.md` because I can only read files here, not create them. As the brief allows, the full report is below.

---

# report.md

## answer

**Files:** `main.hero`

```
# The largest difference two numbers may have and still be near each other: 1e-9.
constant EPSILON: f64
    0.000000001

# Whether `a` and `b` differ by less than EPSILON.
function near(a: f64, b: f64) -> bool
    d = a - b
    return d < EPSILON && -d < EPSILON

function main()
    print(near(a: 0.1 + 0.2, b: 0.3))
    print(near(a: 1.0, b: 1.001))
```

**Does `heroes build main.hero` accept it?** Yes.

**Every line it prints, in order:**
```
true
false
```

**How it ends:** normally, after the second line.

Why:
- `0.1 + 0.2` is `0.30000000000000004`. It differs from `0.3` by about 5.6e-17, which is less than 1e-9 either way round, so the answer is `true`.
- `1.0 - 1.001` is about `-0.001`. That is less than 1e-9, but `-d`, about `0.001`, is not. So the `&&` is false and the answer is `false`.

## choice_points

1. **How to write 1e-9.** Section 2 says a float has "digits on both sides of its point and no exponent". I wrote `0.000000001`. Writing `1e-9`, or `1.0e-9`, would be **refused by the compiler**.
2. **Absolute value.** Section 11 lists no `abs` built-in. I test `d < EPSILON && -d < EPSILON` instead.
   - Calling `abs(a - b)` would be **refused by the compiler** (unknown name).
   - Testing only `a - b < EPSILON` would be accepted, but for `1.0` and `1.001` the difference is negative. It would print `true` on both lines: **a different answer than intended**.
   - Writing my own `abs` function with `if d < 0.0` would also work and give the same output.
3. **Named arguments.** Both parameters of `near` are `f64`. Section 9 says that when two parameters share a type, named arguments are mandatory at the call. I wrote `near(a: …, b: …)`. A positional call `near(0.1 + 0.2, 0.3)`, or the UFCS form `(0.1 + 0.2).near(0.3)`, would be **refused by the compiler**.
4. **`<` versus `<=`.** The task says "less than", so I used `<`. Using `<=` would give the same output for these inputs. It would only differ for a difference of exactly 1e-9.
5. **Type of the float literals.** Section 2 says an untyped literal defaults to `i64`, but doesn't say what an untyped float literal becomes. Every float literal here has an `f64` context: a parameter, or the constant's declared type. That avoids the question. If a float literal were bound with no context (`x = 0.1`), the spec doesn't settle it. It could be **refused by the compiler** or might become `f64`.
6. **Named constant or inline literal.** I used `constant EPSILON: f64`. Its name is SCREAMING_CASE and its body is a literal, as section 4 requires. Putting `0.000000001` directly in the comparisons would behave the same. A lowercase name such as `eps` would break section 4's naming rule and probably be **refused by the compiler**.
7. **How `-d < EPSILON` groups.** Section 7 ranks unary `-` above comparison, so it reads `(-d) < EPSILON`. No parentheses are needed, and adding them changes nothing.
8. **Printing the answer.** `print` takes a `bool` and writes `true` or `false` (section 11). Printing other text, such as "yes" or "no", would change the output. The task only asks for "whether", so I print the bool as it is.

## prediction

- **Compiler:** accepts the program. Everything is used: both parameters, `d` and `EPSILON`. `near` returns on its only path. `main` takes nothing and returns nothing. Indentation is 4 spaces, with no tabs.
- **Output:**
  ```
  true
  false
  ```
- **End:** normal.

## context

Nothing outside this directory reached my context. I read only `brief.md`, `task.md` and `heroes-spec.md`. The only other thing I received was the general system prompt saying I am an agent with a file-reading tool. It contained no instructions, rules, memories or documents about Heroes.
~~~~~

### `t5-x-5`, q5-c

~~~~~ markdown
I couldn't write `report.md` because the only tool I had could read files, not write them. As the brief allows, the whole report is below.

# report.md

## answer

**Question 1: give the file whole.**

`main.hero`:
```
# Whether two f64 numbers differ by less than 1e-9.
function near(a: f64, b: f64) -> bool
    d = a - b
    return d < 0.000000001 && -d < 0.000000001

function main()
    print(near(a: 0.1 + 0.2, b: 0.3))
    print(near(a: 1.0, b: 1.001))
```

**Question 2: does `heroes build main.hero` accept it?** Yes.
- Every binding is used.
- `near` has two parameters of the same type (`f64`), so the call sites must name the arguments. They do.
- The float literals have digits on both sides of the point and no exponent, as section 2 requires.
- Each float literal gets the type `f64` from the parameter it is passed to.
- The `->` function returns on its only path.
- `print` takes a `bool`.

**Question 3: what does it print?**
```
true
false
```

**Question 4: how does it end?** Normally.

In `f64`, 0.1 + 0.2 is 0.30000000000000004. That is about 5.6e-17 away from 0.3, which is less than 1e-9. 1.0 and 1.001 are 0.001 apart, which is not less than 1e-9.

## choice_points

1. **How to write 1e-9.** Section 2 says a float has no exponent, so I wrote `0.000000001`. Writing `1e-9` would be refused by the compiler.
2. **Absolute value.** Section 11 lists no `abs` built-in, so I test `d < eps && -d < eps`.
   - Calling `abs(...)` would be refused as an unknown name.
   - Writing my own `abs` function would also work and give the same output.
   - Testing only `d < eps` would compile, but it would give the wrong answer for 1.0 vs 1.001: d = -0.001 is less than eps, so it would print `true` when `false` is intended.
3. **Arguments at the call site.** Section 9 says two parameters of one type require named arguments, so I wrote `near(a: ..., b: ...)`.
   - Positional `near(0.1 + 0.2, 0.3)` would be refused.
   - The UFCS form `(0.1 + 0.2).near(b: 0.3)` is unclear under that rule, so I avoided it.
4. **Inline literal or named constant for eps.** I wrote the literal inline. A `constant EPS: f64` with body `0.000000001` would also be valid and print the same output.
5. **Precedence of `-d < ...`.** Unary `-` binds tighter than comparisons, so it reads as `(-d) < eps` and needs no parentheses. Writing `-(d < eps)` would be refused, because unary minus does not apply to a `bool`.
6. **Type of the float literals.** Section 2 only says a literal takes "the type its context asks for, otherwise `i64`". Here every float literal has a context that asks for `f64`: the parameter `a`, the parameter `b`, or the `f64` that `d` is compared against. So the spec's gap about float literals with no context does not affect this program.
7. **Output format.** I print the `bool` directly, which gives `true` / `false` as section 11 says. Printing a string such as `"yes"` would give different text from the intended "whether" answer.

## prediction

- **Compiler:** accepts the program.
- **Output lines, in order:**
  1. `true`
  2. `false`
- **How it ends:** normally, with no abort.

## context

Apart from the files in this directory (`brief.md`, `task.md`, `heroes-spec.md`), what reached my context was the system setup: the working-directory path, the date, and the description of the file-reading tool. No other instruction, rule, contract, memory or document about the language reached me, and I read no other file.
~~~~~

### `t5-y-1`, now

~~~~~ markdown
I can't write `report.md` because I only have a file-reading tool here. As the brief allows, the whole report is below.

# report.md

## answer

**File `main.hero`:**

```
# Answers whether `a` and `b` differ by less than 1e-9.
function near(a: f64, b: f64) -> bool
    d = a - b
    return d < 0.000000001 && -d < 0.000000001

function main()
    print(near(a: 0.1 + 0.2, b: 0.3))
    print(near(a: 1.0, b: 1.001))
```

**Does `heroes build main.hero` accept it?** Yes.

**Lines it prints, in order:**
```
true
false
```

**How it ends:** Normally, with exit status 0.

Why: in f64, `0.1 + 0.2` is 0.30000000000000004. That is about 5.6e-17 away from `0.3`, which is under 1e-9, so the first line is `true`. `1.0` and `1.001` differ by about 0.001, which is not under 1e-9, so the second line is `false`.

## choice_points

1. **Argument labels in calls to `near`.** Both parameters of `near` are `f64`. Section 9 says named arguments are then mandatory, in the order they are declared. I wrote `near(a: ..., b: ...)`.
   - Writing `near(0.1 + 0.2, 0.3)` would be refused by the compiler.
   - Writing the labels in the other order, `near(b: ..., a: ...)`, would also be refused ("Labels keep the declared order").
2. **How to get the absolute difference.** The built-ins in section 11 include no `abs`. I tested `d < eps && -d < eps`.
   - Another option was a helper `abs` written with `if`/`else`. It would give the same answer, but it is more code. If one of its paths had no `return`, the compiler would refuse it (section 8).
   - Testing only `d < eps` would answer `true` whenever `a < b`, however far apart they are. That is the wrong answer when `a` is less than `b`, though both calls in this task would still print the same lines.
3. **How to write the threshold.** I wrote `0.000000001` and not `1e-9`. The spec never gives the grammar of a `float` literal. It only shows that `print` writes exponent forms like `1e-06`, and that what it prints reads back as the same value. So `1e-9` is probably accepted, but the spec doesn't say so.
   - The decimal form is the same f64 value, so the program doesn't depend on that question.
   - If exponent literals were not part of the syntax, writing `1e-9` would make the compiler refuse the program.
4. **Strict or non-strict comparison.** "Less than" means `<`. Using `<=` would only change the answer for a difference of exactly 1e-9, which neither call has. Both versions print the same lines.
5. **One comparison chain or a split test.** The `Compare` rule allows `a < b < c` grammatically, but section 7 says `<` takes numbers only. So `-eps < d < eps` would compare a `bool` with a number, and the compiler would refuse it. I split the test with `&&`.
6. **Types of the literals.** Every float literal sits where the context asks for `f64`: a parameter, or `d` in a comparison. No literal is left to default to `i64` and none is mixed with an integer.
   - Writing `1` and not `1.0` in the second call would give an `i64` literal where the parameter is `f64`. That is either refused, or an integer literal can't take a float type, so I wrote `1.0`.
   - Mixing in an integer, as in `1 + 2.0`, is a compile error (section 3).
7. **Where to use the result.** `print(...)` returns `()`, so it stands alone as a line, which section 5 allows. I didn't add an unused binding, because the compiler would refuse one.

## prediction

- **Compiler:** Accepts the program and produces a binary. It has no holes, no unused bindings, no shadowing, and no mixed types.
- **Output, in order:**
  1. `true`
  2. `false`
- **Ending:** Normal; `main` returns and the program exits with status 0. There's no abort: the program does no integer arithmetic, no indexing and no ordering of `nan`.

## context

Besides the files in this directory (`brief.md`, `task.md`, `heroes-spec.md`), only my standard system context reached me: an agent identity, the working directory and date, and the tool definitions. No other instruction, rule, contract, memory or document about this language reached my context.
~~~~~

### `t5-y-2`, now

~~~~~ markdown
I couldn't create `report.md` because I have no tool that writes files, only one that reads them. As the brief allows, the whole report is below.

# report.md

## answer

**`main.hero`**
```
# Whether two numbers differ by less than 1e-9.
function near(a: f64, b: f64) -> bool
    d = a - b
    return d < 1e-9 && d > -1e-9

function main()
    print(near(a: 0.1 + 0.2, b: 0.3))
    print(near(a: 1.0, b: 1.001))
```

1. **Does `heroes build main.hero` accept it?** Yes.
2. **Every line it prints:**
   ```
   true
   false
   ```
3. **How it ends:** normally. `main` returns, nothing aborts, and no leases or handles are left open.

## choice_points

1. **How to take the absolute difference.** The spec lists no `abs` built-in, so I compare the difference against both bounds: `d < 1e-9 && d > -1e-9`.
   - Alternative: calling `abs(d)`. Since no `abs` exists, the compiler would refuse it.
   - Alternative: `d * d < 1e-18`. This compiles but has a different threshold near the edge. Squaring can also underflow or round, so a few borderline pairs would get a different answer.
2. **Named arguments at the call site.** Both parameters are `f64`, so section 9 requires names: `near(a: …, b: …)`.
   - Alternative: the positional call `near(0.1 + 0.2, 0.3)` would be refused by the compiler.
3. **Spelling of the tolerance literal.** I wrote `1e-9`. Section 11 prints floats as `1e-06`, which suggests exponent form is valid float syntax, but the spec gives no grammar for a `float` token.
   - Risk: if the lexer required a point (`1.0e-9`), the compiler would refuse `1e-9`. I judged that unlikely.
4. **The literals' type.** Section 2 says a literal without context defaults to `i64`. Every float literal here has context: `d` is `f64`, and the parameters are `f64`, so each literal becomes `f64`.
   - Alternative: binding a literal on its own first, e.g. `t = 1e-9`, would rely on the unclear default for a float literal. It could be an `i64`/float mismatch or a different type, and the compiler would likely refuse it. I avoided this by using the literals inline.
5. **Negative bound.** I wrote `-1e-9` as unary minus applied to a literal (allowed by the grammar's `Unary`), on the same line, so the line-break rule about a leading `-` doesn't apply.
   - Alternative: `-d < 1e-9` would be equivalent and give the same answers.
6. **Arithmetic and expected values.** 0.1 + 0.2 is 0.30000000000000004 in IEEE double. Its difference from 0.3 is about 5.6e-17, which is less than 1e-9, so the answer is `true`. The difference between 1.0 and 1.001 is about 1e-3, so the answer is `false`.
   - Alternative: testing `==` instead of `near` would print `false` for the first line, which is not what the task means.
7. **Precedence.** Comparisons bind tighter than `&&`, so no parentheses are needed. If they did not, the expression would mean something else, but the spec's precedence table settles this.
8. **Unused names.** Every binding (`d`, `a`, `b`) is read, so the rule against unused bindings is satisfied. If `d` were left unused, the compiler would refuse the program.

## prediction

- **Compiler:** accepts the program.
- **Output, in order:**
  ```
  true
  false
  ```
- **End:** normal termination with exit status 0 and no abort.

## context

Only `brief.md`, `task.md` and `heroes-spec.md` from this directory reached my context. Apart from the general system and tool instructions of the agent environment, no other instruction, rule, contract, memory or document reached me.
~~~~~

### `t5-y-3`, now

~~~~~ markdown
I couldn't save `report.md` because I can only read files here, not write them. As the brief allows, the whole report is below.

# report.md

## answer

**File `main.hero`:**

```
function near(a: f64, b: f64) -> bool
    d = a - b
    return d < 0.000000001 && d > -0.000000001

function main()
    print(near(a: 0.1 + 0.2, b: 0.3))
    print(near(a: 1.0, b: 1.001))
```

**Does `heroes build main.hero` accept it?** Yes. Both parameters of `near` are `f64`, so the calls must name their arguments (section 9), and they do. The float literals are `f64` because their context (an `f64` parameter, or a comparison with the `f64` value `d`) asks for it (section 2). Nothing mixes types, every binding and parameter is used, `near` returns on its only path, and `main` takes nothing and returns nothing.

**Every line it prints:**
```
true
false
```

**How it ends:** normally, with no abort.

- **Line 1:** in 64-bit floats, 0.1 + 0.2 is 0.30000000000000004. Its difference from 0.3 is about 5.6e-17, which lies between −1e-9 and 1e-9, so it prints `true`.
- **Line 2:** 1.0 − 1.001 is about −0.001, which is not greater than −1e-9, so it prints `false`.
- **Format:** a `bool` prints as `true` or `false`, with exactly one trailing newline (section 11).

## choice_points

1. **How to take the absolute difference.** The spec lists no `abs` built-in. I tested `d < eps && d > -eps` instead.
   - Calling `abs(d)` names something that doesn't exist, so the compiler refuses the program.
   - Testing only `a - b < eps` doesn't fail on these inputs: −0.001 < 1e-9 is true, so the second line would print `true` instead of `false`. The program runs and gives a different answer than intended.
2. **How to write 1e-9.** The grammar never spells out the `float` token. Exponent form only appears in print output (`1e-06`). I wrote `0.000000001`, which uses the same form the task already needs for `0.1`. If `1e-9` is not a valid float literal, writing it would make the compiler refuse the program. If it is valid, the result is the same.
3. **Calling with or without argument names.** Section 9 says two parameters of the same type need named arguments. Writing `near(0.1 + 0.2, 0.3)` or the UFCS form `(0.1 + 0.2).near(0.3)` would make the compiler refuse the program.
4. **Parameter names.** I named them `a` and `b`. Any other names work the same, as long as the calls use them.
5. **Bare literal types.** I passed the literals straight to `near`, so they take `f64` from the parameter. If I had bound them first without a type (`x = 0.1`), the context wouldn't fix the type. Section 2's default is `i64`, which makes no sense for a float literal, so I avoided relying on it. The other choice would probably either be refused or come out as `f64`; the spec doesn't say which.
6. **What a line contains.** "Prints whether" is read as printing the bare bool. Adding labels such as `print("0.1+0.2 near 0.3: ", ...)` would still compile and run, but the lines would differ from the bare `true`/`false` I predict.
7. **`-0.000000001`.** This is either a negative literal or unary minus on a literal. Unary minus binds tighter than comparison (section 7), so both readings give the same `f64` value. There is no line break, so the rule about a `-` that doesn't touch its operand doesn't apply.
8. **Strict versus non-strict comparison.** "Less than" means `<`. Using `<=` would make no difference on these inputs.

## prediction

- **Compiler:** accepts the program.
- **Output, in order:**
  1. `true`
  2. `false`
- **Ending:** normal, exit status 0, no abort.

## context

Besides the files in this directory (`brief.md`, `task.md`, `heroes-spec.md`), my context held only the general system setup: that I am a Claude agent, the working directory path, and the date. No other Heroes documents, rules, contracts or memories reached my context, and I read no other file.
~~~~~

### `t5-y-4`, now

~~~~~ markdown
I couldn't write `report.md` because I can only read files here, so the brief asks for the whole report as my answer instead. Here it is.

# report.md

## answer

**The file, `main.hero`:**

```
function near(a: f64, b: f64) -> bool
    diff = a - b
    return diff < 0.000000001 && diff > -0.000000001

function main()
    print(near(a: 0.1 + 0.2, b: 0.3))
    print(near(a: 1.0, b: 1.001))
```

**Does `heroes build main.hero` accept it?** Yes.
- Both parameters of `near` are `f64`. Two parameters of one type make named arguments mandatory (§9), so both calls name `a:` and `b:`.
- Every float literal gets its type from where it is used (§2). In `near`, `0.000000001` is compared with `diff`, which is an `f64`. In `main`, the literals are arguments to `f64` parameters.
- `-0.000000001` is the unary minus of an `f64`.
- `diff` is used, `a` and `b` are used, and `near` returns on its only path.
- `print` takes a `bool` (§11). Each call to `near` is a `print` argument, so no value is left unused.

**What it prints:**
```
true
false
```

**How it ends:** normally, after `main` returns, with no abort.

## choice_points

1. **How to write the absolute difference.** The spec has no `abs` built-in. I checked both sides at once: `diff < 1e-9 && diff > -1e-9`.
   - If I had checked only `a - b < 1e-9`, the compiler would accept it but the answer would be wrong. `1.0 - 1.001` is about −0.001, which is below 1e-9, so the second line would print `true` instead of `false`.
   - Writing my own `abs` with `if`/`else` would also work and give the same output.
2. **How to write 1e-9.** I wrote `0.000000001`. The spec never gives the lexical form of a `float` literal. It only shows how floats print, such as `1e-06`.
   - If the lexer takes the exponent form, `1e-9` would behave the same.
   - If it doesn't, `1e-9` could be read as `1`, `e`, `-`, `9`. That would be refused, either as an undeclared `e` or as mixing an `i64` with an `f64`.
3. **Named or positional arguments.** I used named ones: `near(a: ..., b: ...)`. A positional call like `near(0.1 + 0.2, 0.3)` would be refused, because the two parameters share a type (§9).
4. **Comparison style.** I wrote two separate comparisons joined with `&&`. The grammar allows chains like `-e < diff < e`, but the spec doesn't say what they mean. Such a chain would probably be refused as comparing a `bool` with an `f64`, or would mean something unintended.
5. **Order of `diff`.** I used `a - b` rather than `b - a`. Since the check is symmetric, both give the same result.
6. **Inlining `diff`.** I could have written `a - b` twice instead of binding `diff`. The output would be the same. I avoided a cell (`@=`), because a cell that nothing re-binds is a compile error (§5).

## prediction

- **The compiler accepts the program.**
- **Line 1: `true`.** `0.1 + 0.2` is 0.30000000000000004. Its difference from 0.3 is about 5.55e-17, which lies strictly between −1e-9 and 1e-9.
- **Line 2: `false`.** `1.0 - 1.001` is about −0.001, which is not greater than −1e-9.
- **Ending:** normal, with no abort.

## context

Nothing from outside this directory reached my context: no instruction, rule, contract, memory or other document. I read only `brief.md`, `task.md` and `heroes-spec.md`. The only other thing I had was the general system setup saying I am an agent with a file-reading tool, plus the working directory and the date.
~~~~~

### `t5-y-5`, now

~~~~~ markdown
I couldn't write `report.md` because my only tool reads files and cannot create them. As the brief allows, the whole report is below.

# report.md

## answer

**Question 1: the file.**

`main.hero`:
```
# Whether two numbers differ by less than one billionth.
function near(a: f64, b: f64) -> bool
    eps: f64 = 0.000000001
    d = a - b
    return d < eps && -d < eps

function main()
    print(near(a: 0.1 + 0.2, b: 0.3))
    print(near(a: 1.0, b: 1.001))
```

**Question 2: does `heroes build main.hero` accept it?** Yes.
- Both parameters of `near` are `f64`, so the spec requires named arguments at each call. Both calls name them.
- Every binding is used, and there are no mutable cells.
- `near` returns on its only path.
- Each `print` line is a `()` call standing alone.
- The float literals get the type `f64` from their context: either the parameter type or the type written on the binding.

**Question 3: what it prints.**
```
true
false
```

**Question 4: how it ends.** Normally, after the second line.

## choice_points

1. **How to write 1e-9.** The spec never gives the grammar for a float literal. It only shows `1e-06` as printed output. I wrote `0.000000001`, which is the safest form.
   - Other choice: `1e-9`. If the lexer accepts exponent notation, the program behaves the same. If it doesn't, the compiler refuses the program.
2. **Absolute value.** The spec lists no `abs` built-in. I used `d < eps && -d < eps`.
   - Other choice: calling `abs(d)`. The compiler would refuse it, because no such function exists.
   - Other choice: `d < eps` alone. The program would run but give a wrong answer: `near(a: 1.0, b: 1.001)` would print `true`, because d = -0.001 is less than eps.
3. **Giving the epsilon a type.** I wrote `eps: f64 = ...` with the type stated.
   - Other choice: a bare literal in `d < 0.000000001`. This relies on the literal getting its type from the other operand. The spec says a literal takes the type its context asks for, otherwise `i64`. If a comparison operand does not count as context, the literal is `i64`, and the compiler refuses the program for mixing types.
   - Other choice: a top-level `constant EPS: f64` with the value on an indented line. This would also compile.
4. **Named arguments.** I wrote `near(a: ..., b: ...)`.
   - Other choice: positional arguments `near(0.1 + 0.2, 0.3)`. The compiler would refuse them, because two parameters share a type.
5. **Printing a bool directly.** I passed the `bool` result straight to `print`, which renders it as `true`/`false`.
   - Other choice: wrapping it in an `if`/`else` that prints text. This would print different words, not wrong answers.

## prediction

- **Compiler:** accepts the program.
- **Output:**
  - Line 1: `true`. 0.1 + 0.2 = 0.30000000000000004, which differs from 0.3 by about 5.6e-17, less than 1e-9.
  - Line 2: `false`. 1.0 − 1.001 = −0.001, so `-d` = 0.001, which is not less than 1e-9.
- **End:** normally, with no abort.

## context

The only documents in my context were `brief.md`, `task.md` and `heroes-spec.md` from this directory, plus the agent's standard system instructions (tool descriptions, working directory and date). No other project instructions, rules, contracts, memories or documents reached my context.
~~~~~

### `verdict`, verdict

~~~~~ markdown
I only have a read tool, so I couldn't write `report.md`. As the brief allows, here is the whole report.

# report.md

## verdict

**approve**

## experiment

Both programs are worked out by hand. I could not run `heroes build`.

### Under spec-A: `main.hero`

```
# Where the oldest item stands, and how many items the ring keeps at most.
record RingPos
    head: i64
    cap: i64

# Adds one item. Until the ring is full it appends; after that it overwrites the oldest slot.
function ring_add<T>(@items: [T], @pos: RingPos, item: T)
    if items.len() < pos.cap
        items @ items.push(item)
    else
        items[pos.head] @ item
        pos.head @ (pos.head + 1) % pos.cap

# The items, oldest first.
function ring_items<T>(items: [T], pos: RingPos) -> [T]
    n = items.len()
    out: [T] @= []
    for k in range(from: 0, to: n)
        out @ out.push(items[(pos.head + k) % n])
    return out

function main()
    nums: [i64] @= []
    nums_pos @= RingPos(head: 0, cap: 3)
    for a in range(from: 1, to: 6)
        ring_add(items: @nums, pos: @nums_pos, item: a)
    for b in ring_items(items: nums, pos: nums_pos)
        print(b)
    words: [str] @= []
    words_pos @= RingPos(head: 0, cap: 2)
    for c in ["red", "green", "blue"]
        ring_add(items: @words, pos: @words_pos, item: c)
    for d in ring_items(items: words, pos: words_pos)
        print(d)
```

### Under spec-B: `main.hero`

```
# Keeps at most `cap` items, the last ones added; `head` is where the oldest stands.
record Ring<T>
    items: [T]
    head: i64
    cap: i64

function new_ring<T>(cap: i64) -> Ring<T>
    empty: [T] = []
    return Ring(items: empty, head: 0, cap: cap)

function append<T>(@xs: [T], x: T)
    xs @ xs.push(x)

# Adds one item. Until the ring is full it appends; after that it overwrites the oldest slot.
function ring_add<T>(@r: Ring<T>, item: T)
    if r.items.len() < r.cap
        append(xs: @r.items, x: item)
    else
        r.items[r.head] @ item
        r.head @ (r.head + 1) % r.cap

# The items, oldest first.
function ring_items<T>(r: Ring<T>) -> [T]
    n = r.items.len()
    out: [T] @= []
    for k in range(from: 0, to: n)
        out @ out.push(r.items[(r.head + k) % n])
    return out

function main()
    nums: Ring<i64> @= new_ring(cap: 3)
    for a in range(from: 1, to: 6)
        ring_add(r: @nums, item: a)
    for b in ring_items(r: nums)
        print(b)
    words: Ring<str> @= new_ring(cap: 2)
    for c in ["red", "green", "blue"]
        ring_add(r: @words, item: c)
    for d in ring_items(r: words)
        print(d)
```

### Expected result under both

I expect `heroes build main.hero` to accept both programs. Each prints these lines and then ends normally with code 0 when `main` returns:

```
3
4
5
green
blue
```

How the output comes about:
- **Ring of 3:** the array ends as `[4,5,3]` with head 2, which reads out as 3, 4, 5.
- **Ring of 2:** the array ends as `[blue,green]` with head 1, which reads out as green, blue.

## choice_points

### spec-A

1. **No generic record.**
   - **My choice:** a generic array plus a non-generic `RingPos` record, both passed as `@` parameters to generic functions.
   - **Other option, `record Ring<T>`:** the compiler refuses it, because the Declaration grammar has no Generics on `record`. That's a win.
   - **Other option, a record with a free `T` field:** refused, because `T` is unknown.
   - **Other option, a record per type (`items: [i64]` and a copy for `str`):** the build accepts it, but it breaks "write the ring once". That's a silent mistake.
2. **Named arguments.**
   - **My choice:** I named every argument.
   - **The ambiguity:** with `T` = `i64`, `item` and `cap` share a type, and the spec doesn't say whether that counts. Leaving names off could be refused, depending on how the compiler reads that rule.
3. **Return type of `len()`.**
   - **My choice:** I assumed `i64`; the spec never states it.
   - **If it is `u64`:** comparing it with `cap` and passing it to `range` is refused at compile time.
4. **Printing.**
   - **My choice:** I print in `main` with concrete types.
   - **Other option, `print` inside a generic function:** the spec doesn't say whether `print` accepts a type parameter (`sort` explicitly refuses one). The likely outcome is a refusal.
5. **Adding.**
   - **My choice:** append until full, then overwrite at `head`.
   - **Other option, slice and push on every add:** the build accepts it and the output is the same, but it breaks the task's "must not move" rule. That's silent, and it's the same under B.

### spec-B

1. **Construction.**
   - **My choice:** `new_ring<T>`, with `T` taken from the annotation `Ring<i64>`.
   - **Other option, `Ring(items: [], ...)` directly:** unclear. `[]` gives no type, and an empty literal "needs an annotation". If it's refused, that's harmless.
   - **Other option, `Ring<i64>(...)`:** refused, because the type is "never written at a construction". That's a win.
2. **Pushing through a field.**
   - **My choice:** `append(xs: @r.items, ...)`, following the spec's §10 advice to lend the array itself to an `@` parameter.
   - **Other option, `r.items @ r.items.push(item)`:** the build accepts it and the output is the same, but it silently copies the whole array on each add until the ring is full.
3. **Element write through a field.**
   - **My choice:** `r.items[r.head] @ item`.
   - **The gap:** the spec doesn't say whether this copies the array. The output is the same either way.
4. **Bare `Ring` without `<T>` in a type.**
   - **My choice:** I always wrote `Ring<T>`.
   - **The gap:** the grammar makes the angle brackets optional, and the spec doesn't say what a bare `Ring` means. Probably the compiler refuses it; it's unclear.
5. **Shared ambiguities.** Choice points 2–5 from spec-A apply here too.
6. **The `u8` and float changes.** They don't affect this program. They turn a silent mistake (`s[i]` printing as a number) into a compile refusal, and they define the `float` literal, which A's grammar names but never defines.

## argument

Both specifications let me write a correct ring. Under A, though, "write once for any type" forces parallel state threaded through `@` parameters. That adds more arguments, more named-argument hazards, and an easy fallback that compiles but breaks the task: one record per type. Under B the natural shape, `record Ring<T>`, is legal. B's new construction rule refuses the obvious mistake (`Ring<i64>(...)`). B adds one silent hazard (`push` through a field copies the array), but §10 already warns about it. The bundled `u8` changes turn a silent wrong answer into a compile error, and the float sentence defines a literal A left undefined. Every construct can be read from its line and signature, so a veto doesn't apply.

## condition

I would object if either of these turned out to be true:
- Under B, `heroes build` accepts a bare `Ring` (no `<T>`) in a type and infers something unintended.
- Under B, `Ring(items: [], ...)` builds a ring of the wrong element type without an error.

I would also object if another trial showed models under B routinely writing `r.items @ r.items.push(...)` while models under A avoided the equivalent copy. I would veto if `Pair<A, B>` in a type could not be told apart from a `<` comparison in some position the grammar allows.

## context

Beyond the files in this directory, only my system setup reached my context: the agent framework identity, the working directory path and the date. No CLAUDE.md, rules, memory or other documents reached it. Of this directory I read `brief.md`, `changes.diff`, `task.md` and `spec-A.md`. I did not read `spec-B.md` itself; I built it as `spec-A.md` plus `changes.diff`, which the brief says is the only difference.
~~~~~

**Added 2026-11-21.** The folders under `.claude/worktrees/` this file names were never part of
the repository, git ignoring that directory, and were removed that day by the author's
instruction, meant as: *delete every worktree that is not active; where one is cited, find a
solution*. What a seat or the coordinator kept under `rebel-scratch/` for this sitting is read
from `docs/panel/252-reports/evidence.tar.xz`, its paths relative to `rebel-scratch/`; `evidence.md`
beside it says what was kept and what was left out.
