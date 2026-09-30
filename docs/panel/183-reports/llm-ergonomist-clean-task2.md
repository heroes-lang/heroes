# Panel 183, the llm-ergonomist's clean reading of Task 2

Run by the coordinator on 2026-09-30 the same way as `llm-ergonomist-clean.md`:
a fresh `claude -p` session in a folder outside the repository and any git
tree, with the spec at `171e8c45`, `task2.hero`, and two outputs, R and S,
both run: R is what `c85bccb8` and `e5cc73eb` both print, S is what the
compiler-engineer's prototype of question (b) printed
(`compiler-engineer-task2.txt`, byte-identical to the prediction
`00-shared.md` gave). Only the Read and Write tools. Copied verbatim; its
`predict.md`, `fix-R.hero` and `fix-S.hero` are in the coordinator's
scratchpad `183-blind-2/`.

---

# verdict

Approve the behaviour of S.

The kind of mistake at issue is an unclosed opening bracket. S reports it at the
opener (5:19), closes it at the end of that line, and goes on parsing the rest
of the function as ordinary statements, which lets it also report the two later
mistakes that are independent of it. R leaves the bracket open, consumes the
stray `)` on line 10 to balance it, reports the failure at the following token
instead of at the opener, and never reports line 10 at all.

# experiment

## Step 1: prediction, written before either output was opened

Written to `predict.md` in this directory before reading `task2-output-R.txt` or
`task2-output-S.txt`. Summary of what it claimed:

Three mistakes in `task2.hero`:

- M1, unclosed `(` at 5:19 in `total = double(3`.
  Spec words relied on: "Inside `(` `[` `{` a NEWLINE never ends a statement. A
  line there keeps its NEWLINE when it ends with a literal, `?`, `???`, a
  closing bracket, or a name or keyword other than `function` and `fail`; that
  NEWLINE may stand only before a closing bracket or a `,`, or where a
  production writes it, and any other line goes on below, at any column."
  Line 5 ends with the literal `3`, so it keeps its NEWLINE; that NEWLINE must
  stand before `)` or `,`; the next token is `if` at 7:5. Grammar:
  `Args = "(" [ Arg { "," Arg } ] ")" .`
  Predicted report site: 7:5 or 5:21, and in a good compiler with the opener at
  5:19 named.
- M2, one `)` too many at 10:25 in `print(double(total)))`. `(` at 10:10, `(` at
  10:17, `)` at 10:23 closes `double`, `)` at 10:24 closes `print`, `)` at 10:25
  closes nothing. Predicted report site: 10:25.
- M3, missing right operand at 11:17, reported at the `)` at 11:18, in
  `print(total +)`. Grammar: `Sum = Product { ( "+" | "-" ) Product } .` and
  `Primary = integer | float | string | character | Interp | ident | "true" |
  "false" | "nullptr" | "???" | "fail" | "(" Expression ")" | ...`, in which `)`
  is not a Primary. Predicted report site: 11:18.

Nothing else. Once the syntax closes, `total` is an `i64` read four times so it
is not an unused binding, `total > 2` is a `bool`, `print` takes a number,
`main()` takes nothing and returns nothing, indentation is four spaces per level
throughout.

The prediction also recorded the trap, before seeing any output: over the whole
file the brackets BALANCE. Line 5 leaves one `(` open and line 10 leaves one `)`
spare, so a compiler that lets the bracket stack run across statements pairs
5:19 with 10:25 and reports neither at its true place.

What the spec says about the kind M1 is: it fixes exactly where the program
stops being legal, at the first token after a kept NEWLINE that is neither `)`
nor `,`, and says nothing about recovery, error count, or how to resume. It also
removes the cue a reader would resynchronise on, since inside brackets a
continuation line sits "at any column" and the otherwise rigid four-space
indentation is inert there. So the compiler has a free choice: close the bracket
at the end of the offending line and keep parsing normally, or keep it open and
read lines 7 to 11 as argument text. Only the first choice can reach M2 and M3
in the same run.

The prediction's turn counts: a compiler naming the opener and reaching all
three, one turn; a compiler that pairs 5:19 with 10:25 and stays silent about
line 10, three or more turns.

## Step 1 against the outputs

S matched the "good compiler" branch exactly: `unclosed_bracket` at 5:19,
`expected_end_of_line` at 10:25, `expected_expression` at 11:18. Three errors,
three true mistakes, every caret on the character to edit, no false error.

R matched the "keeps the bracket open" branch exactly: `expected_args_close` at
7:5 found `if`, then `expected_expression` at 11:18. The predicted trap fired.
R is silent about 10:25 for the reason predicted, because it used that `)` to
close line 5's `(`. Neither R error is false; the defect is one displaced error
and one missing one.

## Step 2: fix from R alone

`fix-R.hero`, written from `task2-output-R.txt` and `task2.hero` only:

```
function double(n: i64) -> i64
    return n * 2

function main()
    total = double(3)

    if total > 2
        print(total)

    print(double(total)))
    print(total)
```

Reasoning available from R alone: error 1 says the parser wanted `)` or `,` at
7:5, so an argument list is open at line 7; line 7 is a plain `if` at the
statement indentation of `main`, so the open bracket is earlier, and scanning up
gives line 5. Add `)`. Error 2 at 11:18 removes the dangling `+`. R reported an
error at line 11, which is past line 10, so R read line 10 through to the end;
nothing in R's output suggests line 10 needs anything.

Mistakes still in `fix-R.hero`: one, M2 at 10:25. It is not a mistake R
introduced; it is a mistake R concealed, and closing line 5 is what exposes it.
Expected turns to a program that compiles: 2.

## Step 2: fix from S alone

`fix-S.hero`, written from `task2-output-S.txt` and `task2.hero` only:

```
function double(n: i64) -> i64
    return n * 2

function main()
    total = double(3)

    if total > 2
        print(total)

    print(double(total))
    print(total)
```

Each of S's three carets is the character to change: add `)` after `3` at 5:20,
delete the `)` at 10:25, resolve the operand at 11:17 to 11:18. Mistakes still
in `fix-S.hero`: zero. Expected turns: 1.

## Step 3: compare

S in one turn, R in two. The message that sent me to the wrong place is R's
`expected_args_close` at 7:5: the caret sits on an `if` that is correct as
written, the edit belongs at 5:20, and the message never names 5:19. Recovering
line 5 from it takes an inference the message does not supply, namely that an
"args close" complaint at a statement-level `if` means an opener several lines
up. The message that sent me to the wrong fix is R's silence about line 10,
which is worse than a wrong caret because it reads as a clean bill of health for
a line R had in fact mis-parsed.

S's second message is not perfectly framed. `expected_end_of_line`, glossed "one
statement per line, no semicolons", blames statement separation for what is an
unmatched closer. The caret is on the right character and the repair is
unambiguous, so this costs nothing here, but `unmatched_bracket` would be the
honest code.

# hesitation_points

- Line 11, under both outputs. `print(total +)` admits two repairs that both
  compile: delete the `+`, giving `print(total)`, or supply an operand, giving
  something like `print(total + 1)`. Nothing in the file or the spec says which
  was meant. A wrong guess produces a program that compiles and does something
  else, printing 6 instead of 7 on the last line. This risk belongs to the
  source, not to R or S; both outputs describe the mistake identically and
  neither disambiguates. I took the minimal edit, deleting the `+`, because
  inventing an operand invents semantics.
- Line 10, under R only. I had to decide whether R's silence about line 10 is
  evidence or an artefact. A wrong guess here produces an error, not a silent
  change: leaving the stray `)` fails to compile and costs a turn. I recorded
  the fix that trusts R's silence, because trusting a compiler's silence about
  lines it claims to have parsed is the default behaviour worth measuring.
- The exact report site for M1, in the prediction only. I hedged between 5:21,
  the kept NEWLINE, and 7:5, the offending token, because the spec licenses
  either reading of "that NEWLINE may stand only before a closing bracket or a
  `,`". A wrong guess here costs nothing in the program, only in the sharpness
  of the prediction.
- Whether M2 and M3 are separate mistakes or consequences of M1. I treated them
  as independent, since deleting line 5 entirely leaves both intact. If they
  were consequences, R's reticence would be correct rather than lossy.

# argument

Both compilers see the same three mistakes; only S reports all three. R spends
line 5's open bracket on line 10's stray closer, so its parse is coherent only
under a reading the author did not write, and line 10 is reported nowhere. R's
one located error sits at 7:5, on an `if` that is not wrong, and never names
5:19, so the caret points away from the edit. Fixing from R therefore closes
line 5 and thereby creates the line 10 error that R had hidden: a second turn,
guaranteed by construction. S's virtual close at end of line costs nothing here
and buys two correct extra reports.

# prediction

On single files holding one unclosed opener inside a function body plus at least
one independent later mistake whose token is a closer of the same shape, given
one turn and the compiler output alone:

- One-turn repairs, meaning the next compile succeeds: S about 8.5 in 10, R
  about 1.5 in 10. R's residual is almost entirely the second turn spent on the
  closer it concealed.
- Carets landing on a line that needs no edit: R about 10 in 10 for the first
  error, S about 0 in 10.
- Mistakes present in the source and absent from the output: R about 1 per such
  file, S about 0.
- Silent wrong edits, meaning the program compiles and behaves differently from
  what the author meant: about 5 in 10 under both, concentrated on the truncated
  binary operator, where neither compiler can disambiguate. R does not raise
  this rate. R's extra failure mode is loud, a leftover syntax error, not quiet.

Falsified if measured one-turn repair rates under R and S come within 2 in 10 of
each other, or if R's rate is the higher.

# condition

Two results would change the verdict.

First, false errors from S's recovery. S closes the bracket at the end of the
line, which is a guess about what the author meant. On a file where the author
genuinely intended a multi-line call and merely forgot the final `)`, so that
the later stray closer really does belong to the opener, S's virtual close would
turn the remaining argument lines into garbage statements and emit a cascade of
errors that are not mistakes. If S produced such false errors on more than about
1 in 5 files of that shape, while R parsed them correctly, I would switch to
neither, since a model cannot tell a false error from a true one and will edit
correct code to silence it.

Second, an R that names the opener. If R's `expected_args_close` at 7:5 also
pointed at 5:19 as the unclosed `(`, the displaced caret would stop costing
anything, and the case against R would rest only on the concealed line 10. If
measurement then showed the concealed error costing under about 2 in 10 extra
turns, approving both would be defensible.

What would not change it: R reporting fewer errors is not in itself a virtue
here. R's two errors are not a filtered subset of S's three. They are a
different parse, and the one R omits is the one its own recovery created the
illusion of having checked.

# context

Only this directory's files reached my reading: `brief.md`, `spec.md`,
`task2.hero`, `task2-output-R.txt`, `task2-output-S.txt`. I read no file outside
this directory, used no tool but reading and writing files here, and ran no
compiler, so every claim about what R and S do is read off their two outputs and
the spec, not observed.

Beyond those files, the session's own harness text was in my context before I
started: the environment description, a standing instruction about which
language to answer in, a listing of available skills and tool names, and
instructions from a connected documents server. None of it concerns Heroes, this
program, or either compiler, and none of it told me which of R or S is the
compiler of today. I did not learn, and still do not know, which is which.

Files I wrote in this directory: `predict.md`, the step 1 prediction, written
and closed before either output was opened; `fix-R.hero` and `fix-S.hero`, the
step 2 programs; and this report.
