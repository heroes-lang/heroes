# Your brief

You judge one thing: whether the specification of a small programming
language lets a language model produce a correct program in one turn. Your
input is the files of this directory and nothing else: `spec.md`, the
specification (read it in full first), and `sa.h`, a C header a program in
that language binds. Read no file outside this directory and use no tool but
reading and writing files here.

`sa.h` declares `SA`, a C struct holding an anonymous union, and three
functions:

```c
typedef struct { int32_t kind; union { int32_t i; float f; }; int32_t x; } SA;
SA make_sa(void);          /* kind 1, i 12, x 3 */
float sa_f(SA s);          /* returns s.f */
int32_t sa_x(SA s);        /* returns s.x */
```

Method: write the code, do not opine.

1. **Write ONE program**, as you would submit it, that: prints the `i` of
   the `SA` that `make_sa()` returns (`12`); builds an `SA` with `kind` 2,
   `f` 1.5 and `x` 4 and prints what `sa_f` and `sa_x` return for it (`1.5`
   and `4`); and prints whether two values returned by `make_sa()` are equal.
   Where the specification makes a part impossible, write the closest program
   it allows and say which part it cannot express. Save the program, exactly
   as you would submit it, as `program.hero` in this directory.
2. List every place where the specification left you a choice, the choice
   you made, and what the other choice would produce: a compile error, or a
   program that compiles and prints something else.
3. Say, line by line, what `program.hero` prints, reading only the
   specification and C's own rules for a union.

Write `report.md` in this directory as you go, with exactly these headings:
`experiment` (the program, as in `program.hero`), `choice_points`,
`prints` (step 3), `argument` (at most 120 words: is a correct program likely
in one turn under this specification, and where is a silently wrong one
possible), `prediction` (a falsifiable rate of one-turn successes and of
silent wrong programs over 100 attempts at this task), and `context`
(whether anything other than this directory's files reached your context,
and what). English, no em dashes.
