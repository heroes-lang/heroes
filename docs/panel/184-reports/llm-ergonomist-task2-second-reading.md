# Panel 184, the blind seat, task 2's second reading (the critic's § E, item 7)

Run by the coordinator on 2026-10-01 from `<scratchpad>/rdr/t5/`, on the author's
authorization of the same evening (`docs/records/log/2026-10-01-2224-six-answers-panel-184-ratified-panel-185-convened-the-openers-message-reworded-the-blind-seats-command.md`),
one fresh session outside the repository with the panel skill's command,
`--model claude-opus-5-5`, `claude` 2.1.285: 8 turns, 0.40 USD by the CLI's
JSON report, `is_error` false. The report below is the session's own file,
copied unchanged.

# report

## verdict

- **P**: object
- **Q**: veto
- **R**: approve
- **S**: approve

## experiment

Steps 1 and 2 below were written before I opened `o2-check.txt` or
`o2-run.txt`.

The function bodies come out the same under all four variants. Only the
optional trailing `return` after `exit` differs, so I show the shared program
once and then say what each variant does with it.

Shared program (my choice under every variant):

```
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
    exit(code: 1)

function main()
    xs = [3, 9, 1, 12]
    print(first_above(xs, 8))
    for x in xs
        if x < 5
            continue
        print(x)
```

Expected output: `1`, `9`, `12`, one per line.

- **P**: I wrote the program as above. P does not say whether a `-> i64` function
  may end on `exit(code: 1)` with no `return`, so an author who wants to be safe
  appends `return 0` after `exit`. P allows that, because the dead statement
  follows `exit` and not `return`/`break`/`continue`.
- **Q**: The same as P. Neither unreachable code nor a missing `return` is
  specified, so the defensive `return 0` is just as tempting and also allowed.
- **R**: As above, with no trailing `return`. R says so in its own words: no path
  of `positive_or_exit` reaches its end, so it needs no `return`. A defensive
  `return 0` after `exit` would be a compile error.
- **S**: As above, with no trailing `return`. `exit` ends the path, so the
  `->` obligation is met. A defensive `return 0` after `exit` is permitted, since
  S's unreachability clause names only `return`, `break` and `continue`.

### task2 mistakes (step 2)

- **M1, line 7:** `i @ i + 1` is indented under `return i`, inside the `if`. It
  is unreachable, and the loop never advances when `xs[i] >= 0`. With
  `xs[0] = 4`, `first_negative` loops forever.
- **M2, line 17:** `print("found a zero")` follows `break` in the same block, so
  it is unreachable. The author presumably meant to print before breaking.
- No other mistakes: bindings are used, types agree, and `-> i64` returns on
  every path.

What each variant requires the compiler to do with them:

- **P**: M1 and M2 are both compile errors: each is a statement after a
  `return` or `break` in the same block. No binary is produced.
- **Q**: Nothing forbids either one. The program compiles. At run time, the
  `for` loop breaks at `0` with nothing printed, then `first_negative` hangs.
  The output is empty and the program never terminates.
- **R**: Both are compile errors, the same as under P (these are the base cases
  of "always leaves").
- **S**: Both are compile errors under S's first sentence. Its second sentence
  does not bear on task2.

### step 3: outputs

`o2-check.txt` contains only `exit 0`: the program compiled, with no
diagnostic. `o2-run.txt` says the built program printed nothing and was killed
after 5 seconds (exit 142). This is exactly what Q predicts, and what P, R and S
forbid, so this compiler implements Q.

A model that reads only these two files learns that the program hangs and that
nothing was printed. It has to search for the cause with no line number. The
likely next turn:

1. It looks for an infinite loop, finds `while i < xs.len()` in
   `first_negative`, sees that the increment sits under `return i`, and dedents
   it. That is one more turn.
2. The rerun then prints `2` and exits 0. Nothing points at M2. A model that
   does not know the author wanted "found a zero" printed takes the empty
   output of the `for` loop as correct.

So M2 most likely survives as a silently wrong program. Fixing M1 takes at
least one blind repair turn instead of a pointed one.

## choice_points

1. **Where the increment `i @ i + 1` sits.** I put it at the `while` body's
   level, after the `if`. The other choice is to indent it under `return i`
   (the mistake in task2). Under P, R and S that is a compile error. Under Q it
   compiles, and the loop never advances on an element `<= limit`. Here
   `xs[0] = 3`, so the program hangs.
2. **A trailing `return` after `exit` in `positive_or_exit`.** I wrote none.
   - Under P and Q, leaving it out has unspecified meaning: the spec never says
     what a `-> i64` function that falls off its end does. It could be a compile
     error, an abort, or a garbage value. Writing `return 0` compiles under P
     and Q.
   - Under R, writing `return 0` is a compile error (a statement after a call to
     `exit`).
   - Under S, writing it compiles and is dead code.
3. **`if`/`else` or a bare `if` before `exit`.** I used a bare `if`. Writing
   `if n > 0 / return 1 / else / exit(code: 1)` is equivalent under every
   variant. Under R, a `return` after that `if` would be an error, because
   every branch leaves. Under S, that `if`/`else` form needs no `return`
   either, because both of its paths end.
4. **`exit(code: 1)` or `exit(1)`.** Named arguments are mandatory only when two
   parameters share a type, and `exit` has one parameter. So both should
   compile; I named it. The exit code `1` is my choice. Any nonzero code
   compiles and only changes the observed status.
5. **Positional or named arguments to `first_above`.** I used positional
   arguments: `[i64]` and `i64` are different types, so naming is optional.
   Both compile.
6. **The type of `xs.len()`.** The spec does not state it. I assumed `i64`. If
   it is `u64`, then `i < xs.len()` is a compile error under every variant (no
   implicit conversions).
7. **Whether to call `positive_or_exit` from `main`.** The brief does not ask
   for it, so I did not call it. If an unused top-level function counted as an
   unused binding, that would be a compile error under every variant. The spec
   names only bindings and parameters, so I expect it to compile. Calling it
   with `print(positive_or_exit(1))` would add an extra `1` line to the output.
8. **`continue` in an `if` with no `else`, versus `if x >= 5 / print(x)`.** Both
   are correct. Under R, a `print` after an `if` with no `else` is legal,
   because not every branch leaves.
9. **`>` or `>=` against the limit.** The brief says "greater than", so I used
   `>`. With `>=` the program still compiles, and with limit 8 the answer is
   still `1`. The wrong choice is silent here, under every variant alike.

## argument

Under Q, task2 compiled cleanly and then hung. The fix for the hang is found
blind, and the dead `print` survives as a silently wrong program. Q also leaves
undefined what a `-> i64` function ending in `exit` means, so neither the line
nor the signature can tell you. That is a veto. P catches both task2 mistakes
but leaves the same undefined end. R and S both catch the mistakes and define
the end. R also rejects dead code after `exit` or after an `if` where every
branch leaves. That forbids a defensive `return 0` a model is likely to write,
but the repair is one turn with a pointed message. S tolerates that harmless
dead code, so it forbids nothing I would want to write. A correct program in
one turn is most likely under S, then R.

## prediction

Take 100 model-written programs that contain a statement placed after a jump
by an indentation slip. I predict the share that compiles and runs wrong will
be:

| Variant | Silently wrong | Repaired in one turn |
|---|---|---|
| Q | about 70% | under 20% |
| P | under 5% | over 90% |
| R | under 5% | over 90% |
| S | under 5% | over 90% |

The share of programs that fail to compile only because of a defensive
`return` after `exit` or `assert false`:

| Variant | Fails to compile |
|---|---|
| R | 10 to 30% |
| S | 0% |
| P | 0% |
| Q | 0% |

On top of that, P and Q will show inconsistent behavior on functions that fall
off their end, depending on the compiler.

## condition

- If R's compile message for a statement after `exit` does not lead models to a
  fix in one turn more than 90% of the time, R drops to object.
- If S's tolerated dead code after `exit` turns out to hide real mistakes at a
  rate above 5%, S drops to object.
- If the spec stated elsewhere what falling off a `-> T` function does, P would
  rise to approve and Q to object.
- If Q's hang reproduced with a pointed runtime message, Q would rise to object.

## context

Nothing outside this directory reached my context except the harness's system
prompt and an automatic note giving the user's account email, which I did not
use. I read only `brief.md`, `spec.md`, `task2.hero`, `o2-check.txt` and
`o2-run.txt`, and I wrote only `report.md`. I opened the two output files only
after writing steps 1 and 2 into this file.
