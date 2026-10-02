# Your brief

You judge one thing: whether a rule of a programming language makes it more or
less likely that a language model produces a correct program in one turn. Your
input is the files of this directory and nothing else: `spec.md`, the
specification of a small programming language (read it in full first), and
`sa.h`, a C header a program in that language binds. Read no file outside
this directory and use no tool but reading and writing files here.

In `spec.md`, section 13, one place holds the marker `[RULE 1]`, after the
sentence that ends *as many elements as the type says.* Three candidate texts
for it, in no particular order:

- **A**: *`record Font partial` names only some, and its size stays C's, not
  the field list's. A field in a C union, named or anonymous, shares its bytes
  with the union's other members: a record names one or more of each union,
  reads any, and is built naming exactly one, an anonymous struct's fields
  counting as one. Comparing a `partial` record, or one holding a union's
  field that is not an integer, pointer or array of them as wide as the
  union, and using it as a map key are compile errors, for it and for any
  value holding it.*
- **B**: *`record Font partial` names only some, and then comparing it and
  using it as a map key are compile errors — for it and for any value holding
  it. Its size stays C's, not the field list's.*
- **C**: *`record Font partial` names only some, and its size stays C's, not
  the field list's. A field in a C union, named or anonymous, shares its bytes
  with the union's other members: a record names one or more of each union
  and reads any, and one naming two of a union, an anonymous struct's fields
  counting as one, cannot be built. Comparing a `partial` record, or one
  holding a union's field that is not an integer, pointer or array of them as
  wide as the union, and using it as a map key are compile errors, for it and
  for any value holding it.*

`sa.h` declares `SA`, a C struct holding an anonymous union, and three
functions:

```c
typedef struct { int32_t kind; union { int32_t i; float f; }; int32_t x; } SA;
SA make_sa(void);          /* kind 1, i 12, x 3 */
float sa_f(SA s);          /* returns s.f */
int32_t sa_x(SA s);        /* returns s.x */
```

Method: write the code, do not opine. **Do steps 1 and 2 and write them into
`report.md` before step 3.**

1. **Write**, three times, once under each of A, B and C, ONE program that:
   prints the `i` of the `SA` that `make_sa()` returns (`12`); builds an `SA`
   with `kind` 2, `f` 1.5 and `x` 4 and prints what `sa_f` and `sa_x` return
   for it (`1.5` and `4`); and prints whether two values returned by
   `make_sa()` are equal. Where a candidate makes a part impossible, write
   the closest program the candidate allows and say which part it cannot
   express. List every place where the specification left you a choice, the
   choice you made, and what the other choice would produce under that
   candidate: a compile error, or a program that compiles and prints
   something else.
2. **Under B, say what the program you wrote prints**, line by line, reading
   only the specification and C's own rules for a union (C keeps the member
   written last; reading another member reads the same bytes). If the
   specification does not settle a line's output, say so and give each
   possible output.
3. **Compare** the three: under which is a correct program most likely in
   one turn, under which is a silently wrong one most likely (a program that
   compiles and prints something other than `12`, `1.5`, `4` and the
   equality's true answer), and does any candidate make a program's meaning,
   or whether it compiles, depend on something other than the line it is on,
   its enclosing function's signature and the header the group names? Say
   which of the two it is, meaning or compiling, for each candidate you judge
   that way.

Write `report.md` in this directory as you go, with exactly these headings:
`verdict` (for each of A, B and C: approve, object or veto; a veto is for a
rule whose meaning cannot be told from the line, its enclosing signature and
the named header), `experiment` (the code you wrote), `choice_points` (every
place the specification left a choice, and what the wrong choice produces),
`argument` (at most 120 words), `prediction` (a falsifiable rate of silent
wrong programs or one-turn successes under each candidate), `condition`
(what result would change your verdict), and `context` (whether anything
other than this directory's files reached your context, and what). English,
no em dashes.
