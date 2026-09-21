# Panel 172 — brief for the llm-ergonomist

You receive ONLY `spec/heroes-spec.md` and this brief. Do not open design.md and
do not open the repository. Your verdict is an experiment: what a reader of the
document alone writes and predicts.

## The situation, in the document's own words

`spec § 13` says today: *a parameter is taken to keep what it is handed unless
declared `lent`, and a lend reaches only one so declared. `x: cstr @ s.lease()`
is a COPY of the bytes that C may read for as long as the program says, and
`end_lease(@x)` frees it and empties the cell.* And of handles: *`consumes`
after a parameter says the call ends that value's life, so passing one the
function borrowed is an error.*

Nothing in the document says what happens when a C function FREES the bytes it
is handed. A program can write

    extern "giveaway.h"
        function eat(s: cstr)            # C: free((void *)s);

    function main()
        x = "payload"
        c: cstr @ x.lease()
        eat(s: c)
        end_lease(@c)

and the compiler accepts it; the program dies inside `eat` with nothing on
stderr.

## Two variants, blind

**Variant P.** § 13 gains, merged into the `consumes` sentence: *on a `cstr` or
`ptr` parameter, `consumes` says C frees what it is handed, so no lend and no
lease reaches it.* An unmarked parameter keeps what it is handed, as today: a
lend is refused there, a lease is admitted.

**Variant Q.** § 13's lend sentence becomes a rule with three words on a
pointer parameter: `lent`, read while the call runs and let go; `borrows`,
kept past the call and never freed; `consumes`, freed. A lend reaches only a
`lent` parameter; a lease reaches a `lent` or a `borrows` parameter; neither
reaches a `consumes` parameter, and neither reaches a parameter that says
nothing. (This is the rule and not the wording: write the sentence yourself,
as you would want to read it.)

## Tasks, and write what you actually produce

1. **Cold word test.** With the document open at § 13 as it is TODAY (before
   either variant), you are handed three C declarations and their one-line
   documentation, and asked to write the Heroes `extern` line for each:
   - `void free(void *p);` — *frees the block.*
   - `int putenv(char *string);` — POSIX: *the string pointed to by `string`
     shall become part of the environment*; it is kept, never copied, never
     freed by the library.
   - `char *strdup(const char *s);` — *returns a copy; `s` is only read.*
   Write the three lines under variant P and under variant Q. Then say which
   word you reached for on `putenv` and why: it is the trap (keeps, not frees,
   not lends).
2. **Prediction test.** Under each variant, what does the compiler say for the
   program above, and for the same program with `eat` declared `consumes`, and
   for `putenv(s: c)` where `c` is a lease? Predict the code's NAME and the
   first sentence of the message as you imagine it; the sitting will compare.
3. **Word candidates for the freeing case**, cold, one line each: `consumes`,
   `frees`, `takes`, `owns`, `given`. Which one would you write on `free` and
   NOT write on `putenv` and NOT write on `strdup`, first try?
4. **Locality veto.** Under variant Q a program's legality depends on a word
   on a declaration in the same `extern` group — as it already does for `lent`
   since the flip. Is anything in Q non-local in a way P is not? Say yes or no
   and why.
5. **Cost of reading**: which variant makes § 13 easier to hold in mind: one
   word with a meaning per case (Q's three), or two rules (P: unmarked keeps,
   `consumes` frees)? A sentence, and your reason.

## Report

`docs/panel/172-reports/llm-ergonomist.md`: verdict · the lines you wrote,
verbatim · your predictions, verbatim · one falsifiable prediction the sitting
can score with the compiler · veto if any. No design.md, no repository.
