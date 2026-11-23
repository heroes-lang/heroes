# Task t2: three designs for one library, and one abort to read

You have the specification of the language Heroes (`spec.md`). Nothing else is yours.

A C library `watchdog.h` offers `int64_t run_with_deadline(cb_t cb)`: it starts a timer, runs a long
computation, and **when the timer's signal arrives the library's handler abandons the computation**; then
`run_with_deadline` calls `cb(7)` and returns what it returned. The library's author has three designs for
the handler, and a Heroes program binds `run_with_deadline` and passes its own function as `cb`
(`programs/prelude.h` holds the helpers; the three handlers are:)

```
/* A */ static void handler(int sig) { (void)sig; siglongjmp(env, 1); }          /* env: a static sigjmp_buf saved by run_with_deadline */
/* B */ static void handler(int sig) { (void)sig; swapcontext(&handler_ctx, &scheduler); }   /* scheduler: a context saved by run_with_deadline before the timer started, never resumed */
/* C */ static void handler(int sig) { (void)sig; deadline_hit = 1; }                /* the computation polls the flag and returns */
```

Questions, each answered for A, B and C:

1. **Does the Heroes program's function run** after the deadline fired, or does the program abort?
2. If you may ask the library's author to change the design, **which design do you ask for**, and why in
   one sentence?
3. **If a program aborts, here are the words it prints, SHORTENED** (the real message also names an inline
   `__builtin_longjmp`, a C++ exception and a jump inside a system library as examples, and ends with a sentence
   telling the program no function of it is a signal handler; they are left out, the same in every arm, and the
   key says why in `answers.md`):
   *"`answer` was called while a signal handler of C's that this runtime saw begin had not been seen to end:
   either C called it from inside that handler, or C left the handler in a way this runtime does not follow.
   A handler not seen to end is held to be running, where neither this runtime's memory nor the C library's
   is safe to use."* Say, for the design you think aborts: **whose mistake it is** (the library's C, the
   Heroes program, or neither: the language cannot follow it), and what you would tell the library's author.

Then, once, in the structure `brief.md` gives: every place where the specification left you a choice, the
choice you made, and what the other would produce.
