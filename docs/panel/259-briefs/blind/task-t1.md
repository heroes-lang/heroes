# Task t1: seven programs, what does each one print?

You have the specification of the language Heroes (`spec.md`). Nothing else is yours: no other file of this folder
but those this task names, no tool but Read and Write.

Each program is a Heroes file `qN.hero` and the C header it names: `co.h` (coroutines switched by the C library's
`makecontext`, `swapcontext` and `setcontext`), `sw.h` (coroutines switched by the header's own assembly) and
`jump.h` (a try and a throw by `setjmp` and `longjmp`), all in `programs/`. Every program opens with the same records
`E0` to `E5` and the functions `e4` and `e5` (`e2` in q7) that build them. Every header is correct C on the
platform it runs on, and every program builds. The question is only what happens when it RUNS on a 64-bit Arm
machine, a Mac or Linux. Where you hold that the two can differ for a program, give each its own verdict, the Mac's
first; otherwise one verdict stands for both.

For each of q1 to q7, write:

- **verdict**, one of four words: `WRONG` if it prints any line other than a correct reading of its own code would,
  whatever happens after; else `ABORTS` if it ends before its end, saying why; else `PRINTS` and the lines it prints,
  if it runs to its end; or `NOT PROMISED` if the specification does not let you say which of the three;
- **if it aborts**, the one sentence the abort says, as the specification lets you say it, and what you would change
  in the C or in the Heroes so that the program runs, or `nothing can` if nothing;
- **confidence**: 1 (a guess) to 5 (the specification says so).

Then, once, in the structure `brief.md` gives: every place where the specification left you a choice, the choice you
made, and what the other would produce.
