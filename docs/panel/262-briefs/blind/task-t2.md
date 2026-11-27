# Task t2: four programs whose C switches stacks or threads; does the callback run?

You have the specification of the language Heroes (`spec.md`). Nothing else is yours: no other file of this folder
but those this task names, no tool but Read and Write.

Each program is a Heroes file `nN.hero` in `programs/` and the C header it names there: `co.h` (coroutines switched
by the C library's `makecontext`, `swapcontext` and `setcontext`), `esc.h` (the coroutines of `sw.h`, switched by that
header's own assembly, with a save point and a jump out of a coroutine by `setjmp` and `longjmp`) and `th.h` (a thread
started by `pthread_create`). Every header is correct C on the platform it runs on, and every program builds. The
question is only what happens when it RUNS on a 64-bit Arm machine, a Mac or Linux. Where you hold that the two can
differ for a program, give each its own verdict, the Mac's first; otherwise one verdict stands for both.

For each of `n1` to `n4`, write:

- **verdict**, one of three words: `RUNS` and the lines it prints, if it runs to its end; `ABORTS` if it ends before
  its end, saying why, and the lines it prints before; or `NOT PROMISED` if the specification does not let you say
  which of the two;
- **if it aborts**, the one sentence the abort says, as the specification lets you say it, and what you would change
  in the C or in the Heroes so that the program runs, or `nothing can` if nothing;
- **confidence**: 1 (a guess) to 5 (the specification says so).

Then, once, in the structure `brief.md` gives: every place where the specification left you a choice, the choice you
made, and what the other would produce.
