# Panel 184, the llm-ergonomist, task 2 (a statement after a jump)

Run by the coordinator as a fresh session from `<scratchpad>/rdr/t2/` (brief `docs/panel/184-briefs/blind/brief-task2.md`), second attempt, 17:26 to 17:35 on 2026-09-30, the same command as task 1's (`llm-ergonomist-task1.md`), the default model; the CLI reported `total_cost_usd` 1.6597065, 12 turns, `is_error` false. **The session could not write into its folder** (the tools named with `--tools` and not pre-approved with `--allowedTools`), so it wrote to its own session scratchpad (`/private/tmp/claude-501/-private-tmp-claude-501--Users-joseph-Temp-heroes-heroes-lang-edfda945-b355-427c-a48e-2113b1040b67-scratchpad-rdr-t2/782f01fb-92a6-47e2-ac5f-184a0981014c/scratchpad/`): `report-steps-1-2.md` at 17:29, before it opened either output, and `report.md` at 17:33, copied here verbatim below. Its `context` answer is in the report.

# report

## verdict

- **P**: approve.
- **Q**: object.
- **R**: object.

P is local, its meaning is readable from the statement above the line, it rejects
nothing I want to write, and it catches the exact mistake `task2.hero` makes. Q is
not untellable, there is nothing to tell, so it is not a veto; it is an object,
because it is the variant the shipped compiler implements and `o2-run.txt` is the
result. R is an object and not a veto: every clause of R is decidable from the
statement that precedes the line and from the body enclosed by the signature, so
its meaning can be told locally, but its list of "always leaves" forms is a closed
enumeration with an arbitrary edge (a call to `exit` leaves, a call to a function
that always exits does not), and its two halves pull against each other, so one
line is an error under R and mandatory under P and Q.

## experiment

### Step 1: the same program written three times

Task: `first_above` (index of the first element greater than a limit, else `-1`,
with a `while` loop and an early `return`), `positive_or_exit` (returns `1` for a
positive argument, otherwise ends the program with `exit`), and a `main` that
prints the first function's result for `[3, 9, 1, 12]` with limit `8`, then walks
the same array with a `for` loop, skipping elements under `5` with `continue`.

#### Under P (a statement after `return`, `break` or `continue` in the same block is a compile error)

```heroes
function first_above(xs: [i64], limit: i64) -> i64
    i: i64 @ 0
    while i < xs.len()
        if xs[i] > limit
            return i
        i @ i + 1
    return -1

function positive_or_exit(n: i64) -> i64
    if n > 0
        return 1
    exit(1)
    return 0

function main()
    xs = [3, 9, 1, 12]
    print(first_above(xs, 8))
    print(positive_or_exit(9))
    for x in xs
        if x < 5
            continue
        print(x)
```

Expected output: `1`, `1`, `9`, `12`.

#### Under Q (no rule)

Byte for byte the same file as under P. This is the first result of the
experiment: P changes nothing about what I write, because nothing I want to write
is dead code. P and Q differ only in what they do to a file that already has a
mistake in it.

```heroes
function first_above(xs: [i64], limit: i64) -> i64
    i: i64 @ 0
    while i < xs.len()
        if xs[i] > limit
            return i
        i @ i + 1
    return -1

function positive_or_exit(n: i64) -> i64
    if n > 0
        return 1
    exit(1)
    return 0

function main()
    xs = [3, 9, 1, 12]
    print(first_above(xs, 8))
    print(positive_or_exit(9))
    for x in xs
        if x < 5
            continue
        print(x)
```

#### Under R (a statement after one that always leaves its block is a compile error; a function none of whose paths reaches its end needs no `return` there)

```heroes
function first_above(xs: [i64], limit: i64) -> i64
    i: i64 @ 0
    while i < xs.len()
        if xs[i] > limit
            return i
        i @ i + 1
    return -1

function positive_or_exit(n: i64) -> i64
    if n > 0
        return 1
    exit(1)

function main()
    xs = [3, 9, 1, 12]
    print(first_above(xs, 8))
    print(positive_or_exit(9))
    for x in xs
        if x < 5
            continue
        print(x)
```

One line differs from P and Q: the trailing `return 0` of `positive_or_exit` is
gone, because under R a statement after a call to `exit` is a compile error, and
under R the function needs no final `return` since no path reaches its end.

That one line is the whole of R's cost here. Under P and Q the same line is either
required, if the compiler checks that a function with a `->` returns on every
path, which the specification never states anywhere, or dead but legal. Under R it
is forbidden. The three variants disagree about one line of a twenty line program,
and they disagree in opposite directions: the file that satisfies P and Q is
rejected by R, and the file that satisfies R is rejected by P and Q if their
fall-off-the-end check exists.

### Step 2: the mistakes in `task2.hero`

`task2.hero`, blank lines removed:

```heroes
function first_negative(xs: [i64]) -> i64
    i: i64 @ 0
    while i < xs.len()
        if xs[i] < 0
            return i
            i @ i + 1
    return -1

function main()
    xs = [4, 0, -2, 7]
    for x in xs
        if x == 0
            break
            print("found a zero")
    print(first_negative(xs))
```

Two mistakes, both of one kind.

**Mistake 1 (line 7).** `i @ i + 1` stands after `return i`, inside the `if`
block. The increment was meant to be the last statement of the `while` body, one
indentation level out. As written it is unreachable, and the `while` body then
holds nothing that advances `i`. For `xs = [4, 0, -2, 7]`: `i = 0`, `xs[0] = 4`,
not negative, the body ends, `i` is still `0`, and the loop spins forever. The
`-2` at index 2 is never reached and the function never returns.

**Mistake 2 (line 17).** `print("found a zero")` stands after `break`, inside the
`if` block, and is unreachable. The message the author wanted is never printed; it
would have had to come before the `break`.

Behaviour of the program as written, if it is accepted: the `for` loop runs,
`x = 4` passes, `x = 0` breaks, nothing is printed, then
`print(first_negative(xs))` hangs. The program prints nothing at all and never
terminates.

What each variant must do, by its own words:

| | Mistake 1 (`i @ i + 1` after `return i`) | Mistake 2 (`print(...)` after `break`) |
|---|---|---|
| **P** | Compile error. The statement follows a `return` in the same block, which is exactly what P names. | Compile error. The statement follows a `break` in the same block. |
| **Q** | Accepted. No rule in the specification forbids an unreachable statement; `Place "@" Expression` is a legal statement and nothing else bites. | Accepted. Same reason: a call standing as a statement is legal, and its being unreachable is named nowhere. |
| **R** | Compile error. `return` is in R's list of statements that always leave the block. | Compile error. `break` is in R's list. |

On this file P and R are indistinguishable: both must reject, both must name the
two statements, and the repair is one reindentation plus one deletion, or a move
of the `print` above the `break`. Q must accept and produce a binary that hangs
with no output.

### Step 3: what `o2-check.txt` and `o2-run.txt` show, and what a model reading only those does next

`o2-check.txt` is one line: `exit 0`. No diagnostic, not even a warning.
`o2-run.txt`: the program printed nothing and was still running after 5 seconds,
killed with exit 142. The shipped compiler implements **Q**, and the run is the
hang derived above from the source, confirming both the analysis and Q's cost.

A model that reads only these two files knows: it compiled clean, it printed
nothing, it did not stop. It has no line number, no construct, no word from the
compiler, because the compiler said nothing. Its next move is a guess, and the
likeliest guess is the right family, since "hangs, prints nothing" points at a
loop that does not advance. So it will open the source, find the `while`, and see
the increment nested under the `return`. One turn, probably, for mistake 1.

Mistake 2 it will not find, and this is the part that matters. Nothing in either
file points at it: the `print` was unreachable, so its absence from the output
looks the same as the loop never reaching it, and the hang masks the whole
question of what `main` should have shown. Once the increment is reindented, the
program prints `2` and terminates, the symptom in `o2-run.txt` is gone, and the
model stops. The dead `print("found a zero")` survives the repair and ships. Q
does not merely fail to catch the second mistake, it hides it behind the first,
and it hides it again behind the first one's fix.

Under P or R the compiler would have named both statements in the same run, and
one turn would have fixed both.

## choice_points

Every place the specification left me a choice while writing step 1, the choice I
made, and what the other choice produces.

1. **Must a `-> T` function return on every path?** The specification never says.
   I wrote `return 0` after `exit(1)` under P and Q, and omitted it under R.
   Other choice: under R, writing it is a compile error, since it follows a call
   to `exit`. Under P and Q, omitting it is either a compile error ("missing
   return") or an accepted program that behaves identically, since the line can
   never run. So the risk here is always a compile error and never a wrong
   program, but P and Q leave a model with no way to know which spelling compiles.
2. **Does `exit` end a block?** Only R says so. I relied on it under R. Under P
   and Q a statement after `exit(1)` is accepted and silently never runs: a
   program that compiles and does something else, namely nothing, at that line.
3. **`len`'s result type.** I assumed `i64`, so `i < xs.len()` type-checks. If it
   is `u64`, section 3's ban on implicit conversions makes that comparison a
   compile error, and the shape becomes `n = to_i64(xs.len()).must()`. Identical
   under all three variants, so it does not disturb the comparison.
4. **Is an unused top-level function a compile error?** Section 5 bans an unused
   binding or parameter; a `function` declaration is strictly neither. I call
   `positive_or_exit(9)` from `main` so the question cannot bite, which adds one
   line of output (`1`) beyond the three the brief's `main` asks for. If unused
   functions are legal, dropping that call changes only the output; if they are an
   error, dropping it fails to compile. Identical under all three variants.
5. **Named or positional arguments.** `first_above(xs: [i64], limit: i64)` has two
   parameters of different types, so positional `first_above(xs, 8)` is legal. Had
   I typed the limit as a second index, section 9 would make names mandatory at
   every call site and `first_above(xs, 8)` would be a compile error. Same for
   `exit(1)` against `exit(code: 1)`: one parameter, so positional is fine.
   Identical under all three variants.
6. **The loop's shape.** I wrote `while i < xs.len()` with a trailing `return -1`.
   The other natural shape is `while true` with the bounds test and its own
   `return -1` inside, and then a trailing `return -1` after the loop. Under R
   that trailing return is a compile error, since a `while true` with no `break`
   always leaves its block, and R's second half makes it unnecessary. Under P and
   Q it is legal and, if the fall-off-the-end check exists, required. This is the
   second place where R and P/Q demand opposite text; I avoided it by picking the
   bounded shape, which is legal under all three.
7. **Where `i @ i + 1` goes.** After the `if`, not inside it. Inside it, which is
   `task2.hero`'s mistake, is a compile error under P and under R, and under Q a
   program that compiles and hangs forever.
8. **`i` mutable.** `i: i64 @ 0` and `i @ i + 1`. The other choice, `i = 0`, binds
   once forever and makes `i @ i + 1` a compile error ("only a declared `@` name
   can be mutated"). Identical under all three variants.
9. **`continue` versus an inverted test.** I wrote `if x < 5` / `continue` and
   then `print(x)` after the `if`, as the brief asks. The inverted shape,
   `if x >= 5` / `print(x)`, needs no `continue` and is legal everywhere.
   Identical under all three variants.
10. **Blank lines inside a block.** `Block = INDENT { Statement } DEDENT` and
    `Statement` has no empty alternative, so the grammar produces none, yet
    `task2.hero` has four. I assumed the lexer discards them and wrote none. If
    they were an error, `task2.hero` would fail to compile for that reason under
    every variant, which `o2-check.txt`'s `exit 0` refutes.
11. **One `print` per element.** `print` writes its values with no separator and
    one trailing newline, so `print(x)` per element gives one number per line. The
    other choice, accumulating and printing once, changes the output format only.
    Identical under all three variants.

Of the eleven, only 1, 2 and 6 are touched by `[RULE 2]`, and all three are the
same question: may a statement stand where control cannot reach it, and does the
end of a function count as such a place. P answers half of it, R answers all of
it, Q answers none of it.

## argument

Step 1 produced one file for P and one for Q that are byte identical, and a file
for R one line shorter: the rule does not change correct code. Step 2: both of
`task2.hero`'s mistakes are a statement after a jump, so P and R must reject both
and name the lines, and Q must accept. `o2-check.txt` is `exit 0` with no output
and `o2-run.txt` is a five second hang printing nothing, so the shipped compiler
is Q and this is what Q buys. A model reading only those two files sees a clean
compile and a timeout with no line to look at. It will likely find the loop, and
it will then ship the dead `print`, because fixing the hang removes the only
symptom. P costs nothing for that catch. R catches more and forbids a trailing
`return` after `exit`.

## prediction

Falsifiable, over 100 one turn generations of short Heroes programs that contain a
loop with an early `return`, `break` or `continue` (the `task2.hero` family):

- **Q**: 8 to 12 of 100 contain a statement stranded after a jump. All 100 compile
  clean, so 8 to 12 are silently wrong programs: a hang, a skipped update or a
  message never printed. 0 of them draw a diagnostic. Of those that a model then
  debugs from run output alone, at most half of the stranded statements are ever
  found, because a hang or a wrong number points at one of them and hides the
  rest. Confirmed in miniature: `o2-check.txt` is `exit 0` and `o2-run.txt` is a
  hang, on a file with two such statements.
- **P**: the same 8 to 12 misplacements occur, 100 percent become compile errors
  naming the line, and 90 or more percent are repaired in one further turn, since
  the fix is a reindentation. Silent wrong programs from this cause: 0. New first
  turn compile failures introduced by P itself: 0 to 1 of 100, because P forbids
  nothing a model writes deliberately.
- **R**: silent wrong programs from this cause: 0. New first turn compile failures
  introduced by R's extra clauses, chiefly a defensive `return` after `exit`,
  after a `while true`, or after an exhaustive `if` or `match`: 4 to 8 of 100
  overall, and 15 to 25 percent of those programs that contain such a tail. All
  are loud, and 95 or more percent are repaired in one further turn by deleting a
  line. Net one turn success: R below P by roughly 4 points, both far above Q.
- Both P and R leave one residue that neither names: a stranded statement that
  changes nothing, deleted by the repair without the model ever seeing it run.

## condition

- **P loses my approval** if a corpus shows P rejecting a statement a model
  reasonably wanted: chiefly if a `match` arm's inline `return` is read as opening
  a block, so the next arm counts as a statement after a `return`. If that parse
  exists, P is a trap and I would object.
- **R gains my approval over P** if the P/Q compiler turns out to require a
  `return` on every path of a `-> T` function. Then under P and Q the exit
  function has no legal spelling except one ending in a dead `return 0`, the
  specification would be forcing the very code P forbids elsewhere, and R's second
  half stops being a convenience and becomes the only coherent answer. Test:
  compile `positive_or_exit` without its trailing `return 0` against the same
  compiler and read the exit code.
- **R falls to a veto** if its list turns out to be semantic rather than
  syntactic: if `assert FLAG` with a constant false `FLAG` counts as
  `assert false`, or if a `while true` whose only `break` sits under a statically
  false `if` counts as having no `break`. Then whether a line is an error depends
  on constant folding that is not visible in the line or the signature, and no
  model can tell from the text.
- **Q's objection softens to a note** if the compiler emits a warning on dead code
  while exiting 0, since a model that reads compiler output would then see it.
  `o2-check.txt` is one line and holds no warning, so the objection stands as
  written.
- **The whole comparison changes** if a stranded statement is more often a
  deliberate scaffold than a mistake. If, in a corpus of accepted Heroes programs
  under Q, unreachable statements are mostly debug prints left after a `return`
  and mostly harmless, then P and R buy less than I claim.

## context

Files read: `brief.md`, `spec.md`, `task2.hero`, `o2-check.txt` and `o2-run.txt`,
all in this directory, and nothing else. The two output files were opened only
after steps 1 and 2 were written to disk, as the brief requires; because writing
`report.md` needed a permission that had not been granted at that moment, the step
1 and 2 draft was committed to the session scratchpad (`report-steps-1-2.md`)
first, and this file was written afterwards. Nothing was read from outside this
directory. No tool other than reading and writing files was used, and no compiler
was run: every claim about what the three variants must do is derived from their
own words and from `spec.md`.

Other things that reached my context, none of them about this language: the agent
system prompt describing the environment (platform, working directory, today's
date, the scratchpad path, the model I am), a system reminder holding the user's
email address, which played no part in this report, and my own prior knowledge of
unreachable code rules in other languages (C, Go, Rust, Java, TypeScript), which
shaped the expectations behind the prediction section and the remark that R
forbids a line every mainstream language permits. No web access, no other
repository, no other file of this project.


---

## Appendix: the session's steps 1 and 2, as written at 17:29 before it opened the outputs

# report (draft: steps 1 and 2 only, written before o2-check.txt or o2-run.txt was opened)

## experiment

### Step 1: the same program written three times

Task: `first_above` (index of the first element greater than a limit, else `-1`,
with a `while` loop and an early `return`), `positive_or_exit` (returns `1` for a
positive argument, otherwise ends the program with `exit`), and a `main` that
prints the first function's result for `[3, 9, 1, 12]` with limit `8`, then walks
the same array with a `for` loop, skipping elements under `5` with `continue`.

#### Under P

```heroes
function first_above(xs: [i64], limit: i64) -> i64
    i: i64 @ 0
    while i < xs.len()
        if xs[i] > limit
            return i
        i @ i + 1
    return -1

function positive_or_exit(n: i64) -> i64
    if n > 0
        return 1
    exit(1)
    return 0

function main()
    xs = [3, 9, 1, 12]
    print(first_above(xs, 8))
    print(positive_or_exit(9))
    for x in xs
        if x < 5
            continue
        print(x)
```

Expected output: `1`, `1`, `9`, `12`.

#### Under Q

Byte for byte the same file as under P.

#### Under R

Same, except `positive_or_exit` loses its trailing `return 0`:

```heroes
function positive_or_exit(n: i64) -> i64
    if n > 0
        return 1
    exit(1)
```

### Step 2: the mistakes in task2.hero

Mistake 1 (line 7): `i @ i + 1` after `return i` inside the `if`. Unreachable,
and the `while` body no longer advances `i`, so `first_negative([4, 0, -2, 7])`
spins forever.

Mistake 2 (line 17): `print("found a zero")` after `break` inside the `if`.
Unreachable; the message never prints.

Program as written, if accepted: prints nothing, hangs forever.

P: both are compile errors (statement after `return`, statement after `break`).
Q: both accepted, no rule touches them.
R: both are compile errors (`return` and `break` are in R's list).
