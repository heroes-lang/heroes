# Task t1: seven programs, does the callback run?

You have the specification of the language Heroes (`spec.md`). Nothing else is yours: no other file of this
folder, no tool but Read and Write.

Each program below is a Heroes `main` and the C header it extends. `prelude.h` is ordinary C, shared by all
and compiled as it stands; `pN.h` is the program's own C. Every program is correct C on the platform it runs
on, and every one builds. The question is only what happens when it RUNS, on a Mac and on Linux alike.

Every program's `main` is the same:

```
function answer(n: i64) -> i64
    print("answer ran for ", n)
    return n * 10

function main()
    print("result ", run_case(cb: answer))
```

For each of `p1` to `p7` (the files `programs/prelude.h` and `programs/pN.h` in this folder), write:

- **verdict**: `RUNS` if it prints `answer ran for 7` then `result 70`, or `ABORTS` if the program ends
  before that, saying why;
- **if it aborts**, the one sentence the abort says, as the specification lets you say it, and what you
  would change in the C so that the program runs, or `nothing can` if nothing;
- **confidence**: 1 (a guess) to 5 (the specification says so).

Then, once, in the structure `brief.md` gives: every place where the specification left you a choice, the
choice you made, and what the other would produce.
