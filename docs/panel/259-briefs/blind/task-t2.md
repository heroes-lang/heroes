# Task t2: a binding for a coroutine library

You have the specification of the language Heroes (`spec.md`) and the headers `programs/sw.h` and `programs/co.h`.
Nothing else is yours.

You are writing the Heroes binding for a C coroutine library whose switch is written in assembly, as `sw.h`'s is,
and whose `start` takes the memory for each coroutine's stack from its caller. A Heroes function runs as each
coroutine's body: it builds a record of 262,144 bytes, yields, and after the yield reads the record it built. Three
designs for where the binding's C takes each coroutine's stack from:

- **A**: a part of an array in the frame of a C function that runs for as long as the coroutines do (`sw_host`'s
  way);
- **B**: memory from `malloc`, freed after the coroutine ended;
- **C**: memory from `mmap`, with an inaccessible page below it.

1. Under each design, does the record keep its value across the yield: **yes**, **the program aborts** (saying
   what), **a wrong value**, or **not promised**? Confidence 1 to 5 for each.
2. Which design do you write, and why, in at most three sentences?
3. The library's maintainers offer to call, at every switch, a function your binding names, handing it the new
   stack's lowest address and size. Does the specification give you a function to name? If yes, which, and what
   does it change in your answer to 1; if no, say so.

Then, once, in the structure `brief.md` gives: every place where the specification left you a choice, the choice you
made, and what the other would produce.
