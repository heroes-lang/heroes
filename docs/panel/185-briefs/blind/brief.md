# Your brief

You judge one thing: whether a rule of a programming language makes it more or
less likely that a language model produces a correct program in one turn. Your
input is the files of this directory and nothing else: `spec.md`, the
specification of a small programming language (read it in full first), and
three programs, `task-a.hero`, `task-b.hero` and `task-c.hero`. Read no file
outside this directory and use no tool but reading and writing files here.

In `spec.md`, three places hold a marker. Candidates for each, in no
particular order:

**[RULE A]**, the production `Inline` in section 8 (what may follow `=>` on
an arm's own line):
- **A1**: `Inline = Place "@" Expression NEWLINE | "return" [ Expression ]
  NEWLINE | "break" NEWLINE | "continue" NEWLINE | "assert" Expression NEWLINE
  | While | For | Expression NEWLINE .` (every `Statement` of section 5 but the
  two that bind a name).
- **A2**: `Inline = ( Expression | "return" [ Expression ] | "break" |
  "continue" | "assert" Expression ) NEWLINE .`, and anything else after `=>`
  on the arm's line is a compile error.
- **A3**: `Inline = ( Expression | Place "@" Expression | "return" [
  Expression ] | "break" | "continue" | "assert" Expression ) NEWLINE .`, and a
  `while` or a `for` after `=>` on the arm's line is a compile error: a loop is
  written as the arm's block.

**[RULE B]**, a sentence of section 8 after the one about jumps:
- **B1**: *A block whose last statement leaves on every path (a jump, or an
  `if` or a `match` whose every branch leaves) leaves as well, and counts as a
  jump.*
- **B2**: *An `if` or a `match` whose every branch leaves gives no value: as
  the last line of a block whose value is used, it is a compile error; write
  the jump on the arm itself.*

**[RULE C]**, a sentence of section 2 after the one about `{{` and `}}`:
- **C1**: *A literal without the `f` holding a `{` whose text up to the `}`
  that closes it would be a hole is a compile error; such braces, meant as
  text, are written `{{` and `}}` in an `f` literal.*
- **C2**: *A literal without the `f` holding a `{`, then a name, or names
  joined by `.` and calls on them, then a `}`, is a compile error; such
  braces, meant as text, are written `{{` and `}}` in an `f` literal.*
- **C3**: *A literal without the `f` is unchanged.*

Method: write the code, do not opine. For each marker in turn:

1. **Write** the program its task asks for, once under each candidate:
   - for A: a function that takes an array of a three-case variant and, in one
     `match` per element, adds 1 to a counter for the first case, does nothing
     useful but legal for the second, and for the third adds 2 to another
     counter until it reaches 6, written as briefly as each candidate allows;
   - for B: a function `rank(k: i64, j: i64) -> i64` that, when `k` is 0,
     returns 1 if `j` is 0 and 2 otherwise, and otherwise gives 20 through a
     `match` on `k` whose value is bound to a name and returned;
   - for C: a `main` that sums `[4, 5, 6]` into `total`, prints `total 15`,
     then prints the text `{"id": 7, "ok": true}` exactly, then prints `done:
     true` from an `f` literal.
   List every place where the specification left you a choice, the choice
   you made, and what the other choice would produce under that candidate: a
   compile error, or a program that compiles and does something else.
2. **Read the task's program** (`task-a.hero` for A, `task-b.hero` for B,
   `task-c.hero` for C): say what each candidate makes the compiler do with
   each of its lines that a candidate treats differently, and what the
   program prints where it compiles.
3. **Compare** the candidates: under which is a correct program most likely
   in one turn, under which is a silently wrong one most likely, and does any
   candidate make a program's meaning, or whether it compiles, depend on
   something other than the line it is on and its enclosing function's
   signature? Say which of the two it is, meaning or compiling, for each
   candidate you judge that way.

Write `report.md` in this directory as you go, with one section per marker
(`A`, `B`, `C`), each with exactly these headings: `verdict` (for each
candidate: approve, object or veto; a veto is for a rule whose meaning cannot
be told from the line and its enclosing signature), `experiment` (the code
you wrote), `choice_points` (every place the specification left a choice,
and what the wrong choice produces), `argument` (at most 120 words),
`prediction` (a falsifiable rate of silent wrong programs or one-turn
repairs under each candidate), and `condition` (what result would change your
verdict); and at the end one `context` heading (whether anything other than
this directory's files reached your context, and what). English, no em
dashes.
