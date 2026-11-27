# Task t1: thirteen programs whose C handles a signal; does the callback run?

You have the specification of the language Heroes (`spec.md`). Nothing else is yours: no other file of this folder
but those this task names, no tool but Read and Write.

Each program is a Heroes file `hN.hero` and the C header it names, `programs/hN.h`. Most headers include
`programs/prelude.h`, ordinary C shared by all; `h12.h` also includes `programs/sw.h`, coroutines switched by that
header's own assembly. Every header is correct C on the platform it runs on, and every program builds. The question
is only what happens when it RUNS on a 64-bit Arm machine, a Mac or Linux. Where you hold that the two can differ for
a program, give each its own verdict, the Mac's first; otherwise one verdict stands for both.

Every program's Heroes code but `h9`'s and `h13`'s is the same but for the header it names:

```
extern "h1.h"
    function run_case(cb: (function(i64) -> i64)) -> i64

function answer(n: i64) -> i64
    print("answer ran for ", n)
    return n * 10

function main()
    print("result ", run_case(cb: answer))
```

`h9.hero` and `h13.hero` are in `programs/`: each has the same `answer` and calls C to call it, and each prints a line
of its own before (and `h13` one after).

For each of `h1` to `h13`, write:

- **verdict**, one of three words: `RUNS` if it prints `answer ran for 7` then `result 70` (with `h9`'s and `h13`'s
  own lines where they print them); `ABORTS` if the program ends before that, saying why; or `NOT PROMISED` if the
  specification does not let you say which of the two;
- **if it aborts**, the one sentence the abort says, as the specification lets you say it, and what you would change
  in the C so that the program runs, or `nothing can` if nothing;
- **confidence**: 1 (a guess) to 5 (the specification says so).

Then, once, in the structure `brief.md` gives: every place where the specification left you a choice, the choice you
made, and what the other would produce.
