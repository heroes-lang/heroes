# Panel 175 — llm-ergonomist

Your input is **`spec/heroes-spec.md` and this file, and nothing else**: not
the repository, not design.md, not the other briefs. Your verdict is an
experiment. You hold a veto on a non-local construct. Write your report to
`docs/panel/175-reports/llm-ergonomist.md`.

## The two variants, label-stripped

The specification as it stands is variant **X**. Variant **Y** is the same
document with two changes in section 13:

- the sentence beginning *`acquires sqlite3_finalize` after a result* ends
  *… names the one that ends it, which the program owes it, and ending it with
  another aborts.* instead of *… which the program owes it.*;
- after *Bytes C owns come from its own allocator, with their disposer.* comes
  *A pointer C hands out for the program to give back is a handle; a `ptr`
  given back twice is a double free nothing catches.*

## The tasks

Do each task twice, once reading X and once reading Y, and write down what you
produced each time before comparing.

1. **Two ways to open a stream.** C's `stdio.h` has `FILE *popen(const char
   *command, const char *mode)` ended by `int pclose(FILE *)`, and `FILE
   *fopen(const char *path, const char *mode)` ended by `int fclose(FILE *)`;
   on this platform `FILE` is `struct __sFILE`. Write the `extern` group, and a
   program that opens one stream each way and passes both to ONE Heroes
   function `finish` that closes whatever it is given. Then say what happens
   when that program runs, from the document alone.
2. **A pointer to give back.** A C library has `void *make(void)` and `void
   release(void *p)`, and `release` frees. Write the binding and a program that
   makes two values and releases each once. Which spelling did you choose for
   what `make` returns, and why? Then say what the document says happens if a
   later edit releases the first value twice.
3. **One question.** Under each variant, name the one sentence you would point
   to if asked *"who ends the life of a `popen` stream in Heroes?"*

Report, per task and per variant: what you wrote, whether it is correct, and
which sentence decided it. Give a verdict on Y against X, one falsifiable
prediction with the milestone at which it is checkable, and the condition under
which you would change your mind.
