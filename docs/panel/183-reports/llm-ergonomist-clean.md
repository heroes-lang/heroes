# Panel 183, the llm-ergonomist's clean reading of Task 1

Run by the coordinator on 2026-09-30 after the seat's first reading answered
`context: yes` (`llm-ergonomist-first-reading-void.md`): a fresh `claude -p`
session started in a folder outside the repository and outside any git tree,
with no `CLAUDE.md` in it or above it and no project memory, given the seat's
method and output structure without the project's names (`brief.md` below),
the spec at `171e8c45`, `task1.hero` and the two outputs, and only the
Read and Write tools. Its own `context` answer is at the end. Copied verbatim.
Its files: `brief.md`, `fix-from-P.hero`, `fix-from-Q.hero`, `p-trap-a.hero`,
`p-trap-c.hero` in the coordinator's scratchpad `183-blind/`; P is `c85bccb8`'s
output, Q is `e5cc73eb`'s.

---

# verdict

Approve the behaviour of Q.

# experiment

## Step 1. Prediction, written and saved before either output was opened

### The program, with columns

```
 1  function total(xs: [i64]) -> i64
 2      t: i64 @ 0
 3
 4      for x in xs
 5          t @ t + x
 6
 7      return t
 8
 9  function main()
10      xs = [1, 2, 3
11      print(total(xs))
12
13  function report(n: i64)
14      print(n +)
```

Columns, 1-based. Line 10 `    xs = [1, 2, 3`: `[` at column 10, `3` at 15.
Line 13: `function` at column 1. Line 14 `    print(n +)`: `print` at 5, `(` at
10, `n` at 11, `+` at 13, `)` at 14.

### Mistake 1: the `[` opened at 10:10 is never closed

The array literal on line 10 has no `]`, so it swallows what follows. The spec
fixes exactly how far:

- "Inside `(` `[` `{` a NEWLINE never ends a statement."
- "A line there keeps its NEWLINE when it ends with a literal, `?`, `???`, a
  closing bracket, or a name or keyword other than `function` and `fail`; that
  NEWLINE may stand only before a closing bracket or a `,`, or where a
  production writes it, and any other line goes on below, at any column."
- `Primary = ... | "[" [ Expression { Sep Expression } ] "]"`, `Sep = "," | NEWLINE`.

Line 10 ends with the literal `3`, so it keeps its NEWLINE, and that NEWLINE is
a legal `Sep` of the array literal. Line 11, `print(total(xs))`, is therefore
read as a fourth element. It ends with a closing bracket, so it keeps its
NEWLINE too, another legal `Sep`. Line 12 is blank. Then line 13 arrives and
`function` is not a `Primary`, so the parser has nowhere to put it.

Predicted position, naive compiler: 13:1, "expected an expression, found
`function`". That is the first token the grammar actually refuses.

Predicted position, compiler that helps: 10:10, the `[` itself, "unclosed `[`",
because the spec hands it an unambiguous resynchronisation point:

- "Every top-level line starts with its kind." (section 4)
- "Indentation is significant and rigid: exactly 4 spaces per level."
- `Declaration = "constant" ... | "function" ... | "record" ... | "variant" ...
  | "test" ... | Extern .`

A `function` at column 1 can only begin a top-level declaration, and no
production puts a declaration inside an array literal. A bracket still open when
such a line arrives is certainly unclosed, so the only fact worth printing is
where the bracket opened.

### Mistake 2: `print(n +)` on line 14

`Sum = Product { ( "+" | "-" ) Product }`. After `+` a `Product` is required,
which reaches `Unary`, `Postfix`, `Primary`, and `)` is not a `Primary`.
Predicted position: 14:14 at the `)`, or 14:13 at the `+` whose right operand is
missing. Either points at the same line and the same repair. Reachable only if
the compiler recovers past mistake 1.

### What the spec says about the kind the first mistake is

The kind is an unclosed opening delimiter. The spec states no recovery rule,
because it is the language and not the compiler. It states everything needed to
bound the damage: brackets suspend the statement rule, which is why the refused
token is far from the real fault; outside brackets "a line ends its statement"
and "Every top-level line starts with its kind", so the lines that can never be
inside a bracket are decidable from one token at column 1; and layout is "rigid:
exactly 4 spaces per level", so column 1 is meaningful and not a matter of
style. The spec does not compel a position, but it makes one position
defensible and one misleading.

### Checked and not a mistake

`report` is declared and never called. Section 5 makes "an unused binding or
parameter" a compile error and says nothing about a top-level function, and
"Declaration order never matters". So I predicted no error for it. `n` is used
inside `report`, so the parameter rule holds even though the expression using it
is malformed. I also assumed blank lines inside a block are dropped by the
lexer, since `Block = INDENT { Statement } DEDENT` has no empty alternative.

### Prediction against outcome

Both outputs report 10:10 for mistake 1 and 14:14 for mistake 2, with the same
wording and the same caret. My "compiler that helps" position was right, and it
was right for both. Both also recover far enough past the unclosed bracket to
parse `report`'s body and find the second mistake. The recovery is identical.
The entire difference is one message: P additionally raises
`error[expected_expression]` at 13:1, the resynchronisation point. My "naive
compiler" prediction turned out to describe not a whole compiler but one extra
line inside P.

## Step 2. Fix from each output

### From Q alone

Q gives two errors: `unclosed_bracket` at 10:10 with the caret on `[`, and
`expected_expression` at 14:14 with the caret on `)`. Two messages, two lines,
two edits. Written to `fix-from-Q.hero`:

```
function total(xs: [i64]) -> i64
    t: i64 @ 0

    for x in xs
        t @ t + x

    return t

function main()
    xs = [1, 2, 3]
    print(total(xs))

function report(n: i64)
    print(n)
```

Remaining mistakes: none I can find. `xs` is `[i64]` by section 2 ("otherwise
`i64`") and matches `total`'s parameter; `total` has one parameter so no named
argument is required; `print(total(xs))` returns `()` and stands alone as
section 5 requires; `t`, `x`, `xs`, `n` are all read. Expected turns to a
program that compiles: 1.

### From P alone

P gives three errors: 10:10, then 13:1, then 14:14. P's first message already
names the cause, and the spec says a column-1 `function` cannot be inside an
array literal, so I read 13:1 as a consequence of 10:10 and repaired only lines
10 and 14. Written to `fix-from-P.hero`, byte-identical to `fix-from-Q.hero`.

Remaining mistakes: none. Expected turns: 1.

So my own turn count is 1 under both. The difference is not what I did with P
but what P invited, which I also wrote out and traced.

### The edits P's second message invites

P's message at 13:1 says an expression was expected and lists what would satisfy
the parser there: a value, a name, a call, `[`, `{`, `.case`, `if`, `match`, or
`???`. Nine suggestions, and typing any of the nine at 13:1 makes the program
worse. Two concrete edits follow from taking the message at its word.

`p-trap-a.hero`, close the bracket at the place the message points to:

```
function main()
    xs = [1, 2, 3
    print(total(xs))
    ]
```

Trace: line 10 ends with a literal so it keeps its NEWLINE, a `Sep`; line 11
ends with a closing bracket so it keeps its NEWLINE, and that NEWLINE now stands
before a closing bracket, which the spec permits. The literal closes. The
binding is `xs = [1, 2, 3, print(total(xs))]`. Two new errors: `xs` is read
inside its own initialiser, and the elements mix `i64` with the `()` that
`print` returns, against "No implicit conversions". Expected turns: 2 to 3, and
the next round's messages are again about line 13.

`p-trap-c.hero`, three errors so three repairs, the third being to remove the
declaration that "was not expected" there:

```
function total(xs: [i64]) -> i64
    t: i64 @ 0

    for x in xs
        t @ t + x

    return t

function main()
    xs = [1, 2, 3]
    print(total(xs))
```

This compiles. `report` was never called, so the binary behaves identically and
nothing at runtime reveals the loss. Expected turns: 1, with a declaration the
author wrote silently deleted. This is the expensive outcome, because no later
turn will surface it.

A third variant, deleting lines 13 and 14 from the original while leaving the
bracket open, leaves the `[` running to end of file and returns
`unclosed_bracket` at 10:10 again. Expected turns: 2 or more.

## Step 3. Compare

Turns, by me: 1 under P and 1 under Q. I did not pay a turn for P, because P's
first message is correct and I applied the cascade rule myself.

The message that sends you to the wrong place is P's second, at 13:1. It is
wrong in three separate ways. It points at a line with nothing wrong with it. It
carries the same severity and the same `error[...]` shape as the two genuine
errors, so anyone counting errors counts three mistakes in a file that has two.
And its list of suggestions is advice that is actively harmful at that position,
where the correct number of edits is zero. The identical template at 14:14 is
useful, because there an expression really does belong. Same message, one good
use in Q, one good and one bad use in P.

Q suppresses nothing real. Line 13 is correct Heroes, and Q still reports both
genuine mistakes, so its two errors map one-to-one onto the two required edits.

# hesitation_points

1. Line 14, what `print(n +)` was meant to be. I chose `print(n)` as the
   smallest repair. `print(n + 1)` and any other operand are equally
   well-formed. A wrong guess here produces a program that compiles and prints a
   different number: silent, not an error. Neither output helps, and neither
   could; both give the same message at the same column, so this guess does not
   discriminate between P and Q and I exclude it from the rate below.
2. Whether P's error at 13:1 is a cascade or a genuine second mistake. I judged
   cascade, from "Every top-level line starts with its kind". A wrong guess here
   is the costly one: it produces either an error and two to three turns
   (`p-trap-a.hero`) or a program that compiles with `report` deleted
   (`p-trap-c.hero`). Q removes this guess entirely.
3. Whether an unused top-level function is a compile error. I judged not, from
   section 5 naming only a "binding or parameter". A wrong guess would be an
   error message I never saw, costing a turn. Both outputs agree with me by
   silence, since both parsed past line 13 and neither complained about
   `report`.
4. Whether blank lines inside a block are legal, since `Block = INDENT
   { Statement } DEDENT` has no empty-statement alternative. I judged the lexer
   drops them. A wrong guess would be an error on lines 3 or 6. Both outputs are
   silent there, which confirms it.
5. Whether the NEWLINE ending line 11 may stand before the `]` in
   `p-trap-a.hero`. I judged yes, from "that NEWLINE may stand only before a
   closing bracket or a `,`". If I am wrong, `p-trap-a.hero` fails with a
   different message instead, which changes the wording of that trap but not its
   cost.
6. Which of P and Q is the compiler of today. Nothing in these files says, and
   my verdict does not depend on it.

# argument

Both compilers recover the same way and both find the two real mistakes, at
10:10 and 14:14. They differ in one message: P also reports 13:1, where the
bracket recovery fired. Line 13 is correct Heroes. Section 4 says every
top-level line starts with its kind, so a column-1 `function` cannot sit inside
an array literal; the compiler proved this when it blamed the `[`, then
contradicted itself by raising an error at 13:1. Worse, the message lists nine
things that would satisfy the parser there, and every one is the wrong edit.
Three errors for two mistakes invites a third repair. Approve Q.

# prediction

Take this file, hand a competent model one output and one turn, and count the
resubmitted programs. Excluding the line-14 intent guess, which is identical
under both:

- Under Q: at least 18 of 20 resubmissions compile on the first try, and at most
  1 of 20 touches line 13.
- Under P: 12 to 15 of 20 compile on the first try; 5 to 8 of 20 contain an edit
  to line 13, a line with nothing wrong with it; and 2 to 4 of 20 are silent
  wrong edits, meaning they compile with `report` deleted or relocated.

The sharp claim: P's rate of edits to line 13 is at least four times Q's. If the
two rates come out within a factor of two of each other, this prediction is
false.

# condition

Three results would move me.

1. A file where the text between the unclosed bracket and the next column-1
   declaration contains its own independent mistake, and Q reports only the
   bracket. That would show Q is suppressing genuine errors and not just the
   cascade, costing a turn per hidden mistake. Verdict would become neither.
2. P demoting 13:1 from `error[expected_expression]` to a note attached to the
   `unclosed_bracket` error, for instance "gave up here", with no error code and
   no suggestion list. The information is real and worth having; only its
   severity and its advice are wrong. That change makes P and Q equivalent and I
   would approve both.
3. Measured resubmission rates showing models touch line 13 about as often under
   Q as under P. That would mean the extra message is not what drives the wrong
   edit, and my argument would not hold. Verdict would become neither.

# context

Nothing about Heroes, this program, or these compilers reached my context from
outside this directory. I read exactly four files, all here: `brief.md`,
`spec.md`, `task1.hero`, `task1-output-P.txt`, `task1-output-Q.txt`, in that
order, with the prediction in step 1 written to `report.md` before either output
was opened. I ran no compiler; I have none, so every claim about what a program
does is a trace against `spec.md`, and I have said where each trace rests on a
reading that could be wrong.

I wrote five files here: this report, `fix-from-Q.hero`, `fix-from-P.hero`,
`p-trap-a.hero`, `p-trap-c.hero`. I used no tool but reading and writing files
in this directory.

Other material did reach my context, none of it about this task: a system prompt
describing the harness, the working directory and the platform; a listing of
tools and skills available in this session, including document and artifact
tools I did not use; instructions from a connected documents server; a note
carrying the account's email address; and an instruction to answer in a
particular language, which the brief overrides with English. None of it
mentions Heroes, compilers, or error recovery, and none of it informed the
verdict.
