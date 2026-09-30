# Panel 184, the completeness critic's second pass: what is missing

Written 2026-09-30 from 21:21 to 21:42, after every seat reported, by the
completeness critic, per `docs/panel/184-briefs/completeness-critic.md`'s last
paragraph. It gives no verdict. It names what is MISSING: a route nobody
listed, a claim asserted and not measured (with the command that would settle
it), a contradiction between seats (and which side is checkable, and how), a
framing fact a seat took on trust, and the question the sitting should have
asked.

`<c>` is my own copy of a294a6ff,
`/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/edfda945-b355-427c-a48e-2113b1040b67/scratchpad/184-critic/`,
with `<c>/heroes` built from its seed (plain `clang`, identical to the
coordinator's `<p184>/heroes-a294a6ff` but for the link UUID, first pass).
Every measurement below was run there, by me, in this pass, unless it says
otherwise. Read: the repaired briefs (00-shared.md as of 17:00,
`llm-ergonomist.md` as of 17:38), and all eight reports in
`docs/panel/184-reports/`. Not read: any seat's directory, including the blind
sessions' scratchpads the coordinator copied from.

**Status: complete, 21:42.**

## What the synthesis must not get wrong

1. **The blind seat's "M: approve" is an approval of (1a) WITH a symmetric
   `}}`, not of (1a) as the compiler would build it.** The task 1 brief
   defines K and M as writing text braces `{{` and `}}`
   (`docs/panel/184-briefs/blind/brief-task1.md:17`, `:22`), which the
   language does not do. The seat's own M program, extracted from its report
   and built here, prints `{best}} = 30` (A1).
2. **Route labels collide.** The ffi-pragmatist's (1e) (a symmetric escape)
   is not the historian's (1e) (a hole spelling illegal in a plain literal).
   The engineer's (2d) (the (2b) refusal plus the (2a) widening) is not the
   historian's (2d) (no jump in mid-block). The two (3e)s agree. A tally that
   adds verdicts by label adds different routes (A5, B5).
3. **A narrow (2a) is not free.** The spec-warden approves the narrow (2a)
   without a census of its refusals. The engineer's 133 statements that are a
   `return` after a `match` whose every arm returns are all narrow-(2a)
   refusals, most of them in `selfhost/` (read at
   `selfhost/check/consuming.hero:100`). The warden made (1a) wait over 12 of
   the compiler's literals. Under the same measure, the narrow (2a) waits over
   about 134 statements, until someone measures it (B2).
4. **By its own stated condition, the blind task 2 reading prefers R over P.**
   It wrote: *R gains my approval over P if the compiler requires a `return`
   on every path*, and it named the test. I ran the test on its own program:
   `missing_return`, exit 1. It never saw (2d), which keeps both spellings
   legal (B3).
5. **A thread alone is not (3b).** With a 256 MiB stack alone, parens and
   the `else if` chain hold to 10,000. But eight shapes still exit 134 at
   10,000, and `g(g(...))` from 5,000. The spec-warden's (3b) prediction
   (*the table's programs at 10,000 exit 0 or 1*) fails, and so does the
   blind seat's condition for approving X. The real choice is a number, or a
   residual abort at a machine-dependent depth told better (C1).
6. **The compiler need the engineer measured is met by the thread, not by
   the limit.** At `ulimit -s 512` the compiler cannot check itself today,
   and that is closed *by the thread and by nothing else* (the engineer's
   words). So it does not carry (3c)'s number past Principle 0 (C2).
7. **Two (3c)s are in play.** The engineer's built (3c) counts chains: a
   256-term `+` chain is refused, and so is anything past about 128 nested
   `if`s, so the blind task's own 300-term sum would be refused. The blind Y, which the blind seat
   approved, and the historian's (3c) hold chains at any length. The
   warden's (3c) sentence counts an `if` as one level, where the prototype
   counts two nodes (C4).
8. **The `match` defect is wider than filed.** A `match` inside one branch of
   an `if` also silences `missing_return`: check 0, build exit 2, measured.
   The class is "any block holding a `match` statement counts as leaving",
   not "at the body's top level" (B1).
9. **The function a stack panic names depends on where the stack starts.**
   The same binary names `cursor.pass_comments`, `grammarexpr.unary` or
   `cursor.current_kind` for one file, depending only on the size of its
   environment (measured). The engineer attributes the brief's observation to
   frames, which cannot differ between two identical builds. No golden may
   pin that name (C6).
10. **clang's bracket depth is not one number.** Apple clang 21 stops at 2048
    (reproduced here). Upstream's source says 256 (the historian's reading),
    which makes *the same number wherever clang runs* false as measured. The
    ffi-pragmatist's conclusion stands anyway, since the emitted C nests 4
    deep (C7).
11. **The instrument's (1a) and (1b) counts are true by construction**, as
    the warden says of (1a). The same holds for the engineer's (1b) *0
    SILENT*. None of the seats measured how often a model forgets the `f` or
    writes brace text. In the blind transcripts: 0 forgotten `f`, 1
    hole-shaped text literal, induced by the task (A6, A7).
12. **(3e) is unbuilt and unpriced**, and it is the backstop two seats lean
    on. Naming the source position at stack exhaustion needs the compiler to
    know it; no seat measured how (C9).

## A. Question 1

**A1. The blind M carries F1 inside it, and its program shows the trap.**
`brief-task1.md` (the version that ran, 17:27) defines K and M with *such
braces, meant as text, are written `{{` and `}}` in an `f` literal* (`:17`,
`:22`). The session took that as M's own definition: its choice point 8 says
*`}}`, which is K's and M's word and only theirs* and *under L nothing defines
`}}`*. It then wrote its M program with `print(f"{{best}} = {best}")`.
Extracted verbatim from its report into `<c>/blindprog/t1-M.hero` and built:
`check` exit 0, and the run prints `{best}} = 30`, where its L program
(`t1-L.hero`, `print("{best} = ", best)`) prints the right `{best} = 30`. So
the blind reading approves (1a) together with a symmetric escape, and it adds
one data point to what four seats measured: a model told the escape is `{{`
and `}}` writes `{{best}}`, and today's language prints a stray brace.
Reproduced once more here (`<c>/brace/braces.hero`, check 0): `f"{{x}}"`
prints `{x}}`, `f"{{x}"` prints `{x}`, `f"a}}b"` prints `a}}b`.

**A2. (1b): the compiler-engineer approves with amendments, the blind seat
vetoes. What can be checked:**

- *The blind seat judged a different (1b).* K had neither of the engineer's
  amendments: *at least one name* (which spares the emitter's six `{0}`) and
  *the hole's names counted as read* (which removes `unused_binding` beside
  the literal). The second amendment removes one of the blind seat's two
  examples: *obeying the unused-binding message rewrites the brace rule's
  verdict* does not happen when `count` counts as read. The other example
  survives it: renaming `best` to `top` flips `"{best} = "` from refused to
  accepted (by the route's words; I did not run the engineer's prototype,
  which is in its directory). The FFI seat's FTS5 case is the same shape: a parameter named
  `title` refuses `"...MATCH '{title} : "`. Settling whether the veto
  survives the amended wording takes one re-run of task 1 with the amended
  sentence. That is a paid run (one session, 3 USD cap); the coordinator
  decides.
- *What the veto criterion says.* The blind brief's criterion is *a rule
  whose meaning cannot be told from the line and its enclosing signature*,
  design.md §1.3's test (*the meaning of a line*). What (1b) makes depend on
  bindings is a literal's LEGALITY, and its meaning when legal is its text.
  The spec already has a rule of that kind: § 5 makes an unused binding an
  error, so whether line 6 of `task1.hero` compiles depends on lines 13 and
  14. The synthesis has to say whether §1.3 covers legality. If it does,
  `unused_binding` fails it too. If it does not, the veto rests on something
  §1.3 does not say. No seat raised this.
- *The only local (1b) reaches nothing silent.* The blind seat's condition
  for lifting the veto is a (1b) whose names are *only that signature's
  parameters*. I estimated its reach on the 25 `forget-f` sites: a regex
  over each hole's names, checked against the parameters of the nearest
  preceding `function` line, in the c85bccb8 corpus. **4 of 25 sites hold
  only parameter names, and 0 of the 17 SILENT do**: every silent site is in
  `main()`, which has no parameters. This is an approximation, not the
  prototype. So the (1b) the blind seat would accept catches none of the
  measured silent class, and the (1b) that catches them is the one it
  vetoes.

**A3. The historian's own condition turns its (1a) approval.** It approves
(1a) *as a deliberate departure, on the census's answer*. Its condition
reads: *the census finding many literals of the template kind, whose braces a
program fills later. Then ... (1a) should wait for (1e) or settle for (1c)*.
The engineer's census found 20 of the 34 in `examples/template/main.hero`,
template data, plus 6 C `{0}` that the C compiler reads later. Its
prediction (*mostly literals whose braces something else reads later, or text
that shows Heroes source*) is borne out: 20 template, 6 C, 7 Heroes source in
tests, 1 golden. By its own words, the historian's (1a) becomes *wait*.

**A4. 34 or 32: resolved.** The engineer counts 34 literals (1a) refuses,
using the real lexer; the warden counts 32, using `heroes parse` on
`_ = f"..."`. The two only the engineer counts are
`tests/harness/suite_surface.hero:367`, a test string full of escaped quotes
and holes, and
`tests/golden/check/fixedbugs-135-a-backslash-before-a-bracketed-expression.hero:33`,
`print("{x} \(n)")`, a file the lexer refuses at `\(`. Both printed here with
`awk`. Cite 34 with its instrument.

**A5. Two (1e)s and two brace variants.** The ffi-pragmatist's (1e) is *`}}`
writes one `}`, and a lone `}` is an error*; the warden prices that as
F1-strict, +15 vendored. The warden approves F1-lang instead, *`{{` and `}}`
write one brace*, +4. The two move different files:

- F1-lang changes the output of 3 literals in 2 goldens.
- F1-strict also refuses `examples/gallery/12-interpolation.hero:29`
  (`{{like this}`, a lone `}`, printed here), which is the FFI seat's third
  file.

The historian's (1e) is a third thing: a hole spelling illegal in every plain
literal (`\(`), which reopens panel 121 R3. The synthesis names which one it
adopts.

**A6. By construction, and dominated on what.** Under (1a) and the
engineer's (1b), the instrument's 25 mutants are exactly the class the rule
refuses, so *0 SILENT* is true by construction for both. The warden says so
of (1a) only. The engineer's *(1c) is dominated* holds on reach: (1b) tells
the 6 at the literal and 17 more. It does not hold on false alarms: (1c)
changes no program's legality, and (1b) has the FTS5 and rename cases of A2.
The prediction that pays (the warden's § 2, limit 1) has to name committed
goldens, since `recovery.py` lives in the scratchpad.

**A7. The question the sitting should have asked for Q1: how often, in fresh
writing.** Both sides of §1.2 are rates: forgotten `f`s against hole-shaped
text. No seat measured either. The blind seat estimates *15 to 25 of 100* and
says so. Only the blind transcripts hold fresh writing, and they show 0
forgotten `f`s and 1 hole-shaped plain literal meant as text (task 1's
`"{best} = "` under L). The task asked for literal braces, so that one was
induced. The committed tree shows 0 forgotten `f`s among the 34 (engineer)
and 50 `f` literals (FFI), but it is filtered by review, not a first try.
The run that measures it: N fresh sessions writing interpolating programs
from the spec with L's sentence, counting both. Paid, for the coordinator to
size. The FFI seat's 10-session `}}` run is the same kind.

## B. Question 2

**B1. The `match` defect, reproduced and wider.** Files in `<c>/q2m/`,
`<c>/heroes check`, then `build`:

```
a-match-then-falls.hero     (the engineer's: match with printing arms, then print(3), in an i64 function)  check 0, build 2 (non-void function ... should return a value)
b-match-in-if-branch.hero   (if c: a match with printing arms; else: return 2)                            check 0, build 2 (the same)
e-one-arm-returns.hero      (match n: 0 => return 1, _ => print(2), as the whole body)                   check 0, build 2 (the same)
c-match-in-value-block.hero (x = if c: a match statement, then 5; else 6)                                check 0, build 0, prints 0, 5, 6: correct
```

So a block holding a `match` statement counts as leaving wherever that block
sits (b), not only *at its top level* (the engineer's report, § The one
finding). A value block is not affected (c). **My first pass missed this**:
its B2 measured only a `match` whose arms all return and called `match`
consistent with today's predicate. It did not attack the shape beside it,
which is CL-061's failure, and the engineer found the defect.

**B2. The narrow (2a) has a migration nobody counted.** The warden's census
(its 0.9) scans a statement after a jump word and the three widenings
(`exit`, `while true`, `assert false`). It does not scan a statement after an
`if`/`else` or `match` all of whose branches leave. That is exactly what the
narrow (2a) adds, and what the engineer's (2a) census found in bulk: *133 a
`return` after a `match` every arm of which returns*, plus some of the *4
fallback values* and the IR golden. Read here at
`selfhost/check/consuming.hero:88-100`: a `match` whose `.local` arm ends in
an inner `match` whose arms all return, then `return fail(code: "none", msg:
"unreached")` at `:100`. So the narrow (2a) refuses on the order of 134
statements, most in `selfhost/`, and the compiler stops compiling itself until
they are deleted. The warden's § 4 table gives (1a)'s 12 literals as the
reason it *waits*, and the narrow (2a) *enters*. The command that settles it:
the engineer's prototype with `leaves.ROUTE` set to the narrow predicate (jump,
`if`/`else`, `match`; no widening), then its census over the 1,389 files.

**B3. The blind seat's own condition, run.** Task 2's condition section says
*R gains my approval over P if the P/Q compiler turns out to require a
`return` on every path of a `-> T` function ... Test: compile
`positive_or_exit` without its trailing `return 0` against the same
compiler*. Its R program, extracted verbatim into `<c>/blindprog/t2-R.hero`:
`error[missing_return]: positive_or_exit must return i64, and one path through
it returns nothing`, exit 1. Its P program (`t2-P.hero`) builds and prints
`1`, `1`, `9`, `12`, its expected output. **So by its own condition the blind
reading prefers R to P.** Its objection to R was that R's two halves pull
against each other, one line refused under R and demanded under P. That
objection does not reach (2d), under which both spellings compile (the
engineer's table: `exit-then-return` exit 0). (2d) was never shown to a blind
session; one more session with (2d) as a variant would give its reading, a
paid run for the coordinator.

**B4. (2d)'s spec sentence, priced.** The warden's table predates (2d).
Written in my copy after § 8's `Loops:` line (`<c>/drafts/2d.md`): *A
statement after a jump, in its block, is a compile error. A function with a
`->` must `return` on every path that reaches its end, and `exit(code:)`,
`assert false` and a `while true` with no `break` of its own end a path.*
`<c>/heroes measure drafts/2d.md` gives claude-legacy 6780, cl100k 6902,
against 6716 and 6838 for `drafts/base.md`: **+64 vendored**. By the warden's
band that is 9126 to 9162 real, an inference; the real count needs
`--refresh`, a paid call. My method reproduces the warden's (2b): its sentence
alone is 6732 and 6854, +16, the warden's number. (2d) at +16 is possible
only by leaving the return rule unstated, as the spec does today.

**B5. Two (2d)s.** The engineer's (2d) is measured (0 files moved in both
arms, `over-indent` 68 to 49 SILENT on 2,000 mutants). The historian's (2d)
is Wirth's *no jump in mid-block*, named and not recommended. They share a
label only.

**B6. `assert` cannot be switched off, as far as a search reaches.** The
historian keeps `assert false` in the predicate *only if a Heroes assertion
can never be switched off*, a Heroes fact it did not read.
`grep -rn -i -E 'NDEBUG|no.?assert|disable.*assert|assert.*(off|disable)|--release'`
over `selfhost/cli/*.hero`, `runtime/*.h` and `runtime/parts/*.c` prints
nothing. The spec's assertion lines (`:148`, `:243`, `:324`, `:327`) name no
switch. The engineer's built `assert-false-last` aborts 134, *assert failed:
false*. So the historian's condition for excluding it is not met, by that
search.

**B7. The historian's case against (2b), read against (2d).** The C# cost it
cites is *a user writing a redundant `throw` to satisfy the one that does
not*, a dead statement a missing-return check demands. (2d) removes exactly
that. Its remaining point is that one fact has two predicates: a statement
after an `if`/`else` that returns on both branches stays legal. On the
engineer's 1,532 shared mutants that difference changes no class. So the
objection to (2d) is one of consistency, and no measurement in the sitting
prices it.

**B8. Under every route, the spec states no return rule.** 00-shared.md
says so (*`missing_return` lives in the compiler alone*). The blind task 2
session hit it (*The specification never says*, its choice point 1).
CLAUDE.md § 12 says *spec beats compiler: the compiler has the bug*, so today
`missing_return` refuses programs the spec permits. The warden prices the
sentence only for (2a). Under (2b) or (2d) the hole stays, unless the
synthesis states the rule (B4's +64 for (2d)).

**B9. What the blind task 2 condition list still holds.** *P loses my
approval if a `match` arm's inline `return` is read as opening a block*. That
is settled by the engineer's (2b) census moving 0 of 1,389 files, while the
tree has many inline `=> return` arms (`consuming.hero:97-98`). *R falls to a
veto if its list turns out to be semantic*, for example `assert FLAG` with a
constant `FLAG`, or a `while true` whose only `break` sits under a statically
false `if`. Nobody ran that against the prototypes. The command: a program
with `constant OFF: bool` bodied `false` and `assert OFF`, through
`<ce>/heroes-2a2` and `<ce>/heroes-2d`, in the engineer's directory.

## C. Question 3

**C1. A 256 MiB stack alone still aborts.** I linked the seed with
`-Wl,-stack_size,0x10000000` (`<c>/heroes-256m`; `otool -l` reads `stacksize
268435456`). That is the ffi-pragmatist's method, at the same size as the
engineer's thread, and stands in for it: a main thread, not a created one.
Then `check` over `depth.py`'s shapes, plus the `&&` and method chains, at
1,000, 2,000, 5,000 and 10,000 (`<c>/big/`):

| shape | 1000 | 2000 | 5000 | 10000 |
|---|---|---|---|---|
| `((1))`, closed | 0 | 0 | 0 | 0 |
| never closed | 1 | 1 | 1 | 1 |
| `[[1]]` | 0 | 0 | 0 | **134** `checkwalk.synth` |
| `g(g(1))` | 0 | 0 | **134** `checktable.ty_key` | 134 |
| `- - 1`, `!!true` | 0 | 0 | 0 | **134** `checkwalk.synth` |
| `+` chain, `&&` chain, method chain | 0 | 0 | 0 | **134** `checkwalk.synth` |
| `if` nested | 0 | 0 | 0 | **134** `grammarexpr.if_expr` |
| nested `f"{...}"` | 0 | 0 | 0 | **134** `checkwalk.synth` |
| `else if` chain | 0 | 0 | 0 | 0 |

So a stack the compiler chooses moves every abort from 100-250 to between
2,000 and 10,000, and removes none. The engineer's frames predict it: 256 MiB
over `synth`'s 37,104 bytes is about 7,200 levels. Consequences:

- The warden's (3b) prediction, *the brief's table programs at 10,000 exit 0
  or 1 under `check`*, fails for a 256 MiB stack, by this stand-in.
- The blind task 3 condition for approving X (*`check` and `build` of a
  10000-deep nest and a 10000-term chain returning 0, 1 or 2 and never 134*)
  is not met by a thread.
- (3b) as worded, *every walk made to hold any depth*, is reached only by
  the explicit stack the engineer refuses on cost.

The real choice is a number, as in (3c), or a residual abort at a
machine-dependent depth, told as exit 2 under (3e) or left as 134.

**C2. The compiler need is the thread's.** The engineer's
`needed_for_self_hosting` is *yes under a small one*: `ulimit -s 512 ./heroes
check selfhost/main.hero` is 134 today and 0 under the prototype, *closed by
the thread and by nothing else* (its words). The warden measured the same hole
at 1024 and 896 KB. So the measured compiler need carries the thread past
Principle 0, and the limit still needs its own ticket. The warden's *13 at
most* argument (an N true at 1280 KB refuses the compiler's own 14) holds only
without the thread. Under the thread it no longer holds (the engineer, `ulimit -s` 512 and
1024 against 8176). The synthesis must not carry "13" into a route that has
the thread.

**C3. What §1.12 says, and which way the precedence went.** The engineer
rests (3c) on *design.md §1.12 (the tool must not die on its input: a
diagnostic, not an abort)*. Read here, §1.12 (`design.md:577`) is *a Heroes
program must not segfault and must not corrupt memory*, and `:590-593` is
*It does not suspend Principle 0 ... What §1.12 decides is the shape of a form
already admitted*. Today's abort is a named panic through the guard, not a
segfault and not a corruption. What it breaks is the exit contract
(`.claude/rules/cli-surface.md:40-42`) and §1.12's falsifier (`:605-607`, an
abort that names no place is *in the wrong place, not unwanted*). Both (3c)
and (3e) answer those two. Only (3c) enters the language. So CLAUDE.md §
Precedence's rank 3 (robustness) is not what decides here, and the synthesis
writes down which rule did.

**C4. Which (3c), and how it counts.** The engineer's prototype refuses, at
`LIMIT` 256, a `+` chain of 256 terms (depth.py's n = 255: exit 1), `[[` and
`g(` at 255, `((` at 256, and 200 nested `if`s, since an `if` costs two nodes
(its table in § The prototype). So:

- The blind task 3's own program A, a 300-term sum, is refused under the
  engineer's (3c). The blind seat approved Y *because program A is a chain,
  exempt by Y's own words*.
- The historian approves (3c) with *chains built without recursion* (Lua's
  shape), which the engineer did not build.
- The warden's (3c) sentence (*each bracket, block, hole, operator and call
  is one level*) counts an `if` as one level. The prototype counts two, so a
  reader of that sentence and the compiler disagree at 128 nested `if`s.
- The blind seat's Y condition (*more than 1 disagreement in 20 means the
  author's count no longer predicts the answer*) is broken by the prototype
  for `if`, and for `[[` against `((` at 255.

The unbuilt option is 00-shared.md's own *(3c) ... with the chains built so
that no walk recurses per link*. Its cost in lines was asked of the engineer
and not priced. With the thread, a flat chain holds to about 5,000 (C1).

**C5. A route nobody listed: a floor, not a ceiling.** The blind task 3
reading lifts its veto on Z *if it names a floor, for example "at least 256
on every platform, more where the stack allows, and the compiler says
which"*. With the thread, that is a route: the spec promises that every
source nested up to N compiles on every platform, refuses nothing, and past
the thread's stack the compiler exits 2 naming the place, which is (3e). C1
bounds N on this Mac: every shape passes at 2,000 with 256 MiB, and
`g(g(...))` fails at 5,000.

- It meets the blind seat's lift condition.
- It keeps panel 107's refusal of a refusing number.
- It meets the warden's condition (*the sentence states only what is uniform
  ... with the compiler choosing its own stack so the acceptance half holds on
  every platform*).
- It leaves 256-term chains legal.

It is unbuilt, its sentence is unpriced, and its N on x86-64 and Windows is
unrun. The engineer's x86-64 frames, 23,272 bytes for `synth`, are smaller
than arm64's.

**C6. The panic's function name, measured on one binary.** `env -i PAD=<n
bytes> <c>/heroes check`, the same binary and the same file, with only the
environment's size changed:

```
pad 0      unary-minus-10000 -> cursor.pass_comments   fstring-nested-600 -> grammarexpr.postfix
pad 1000   unary-minus-10000 -> grammarexpr.unary      fstring-nested-600 -> grammarexpr.unary
pad 4000   unary-minus-10000 -> cursor.current_kind    fstring-nested-600 -> grammarexpr.binary
pad 16000  unary-minus-10000 -> cursor.pass_comments   fstring-nested-600 -> grammarexpr.postfix
pad 64000  unary-minus-10000 -> grammarexpr.unary      fstring-nested-600 -> grammarexpr.unary
```

The environment sits at the top of the main stack, so its size moves where
the guard page falls among the frames. That explains 00-shared.md's
*two compilers ... differing only in their link UUID, named different
functions*. The engineer's *the frames change with the optimisation level,
the clang and the architecture, ... which is why 00-shared.md found the abort
moving between builds* does not: the two builds had identical frames.

- My own first-pass function names (`cursor.pass_comments` from 600 nested
  `f`) were only true of my environment.
- A golden, a test or a spec sentence that pins a function name, or a depth
  one level from the boundary, would be flaky on one machine.

**C7. clang's bracket depth.** The historian reads upstream's
`LangOptions.def`, `LANGOPT(BracketDepth, 32, 256, ...)`, and writes *it is
the same number wherever clang runs (a reading of the definition, not a
run)*. The ffi-pragmatist measured Apple clang 21 stopping at 2048.
Reproduced here, `<c>/clangdepth/`, `clang -std=c11 -fsyntax-only`: 300
nested blocks exit 0, 2,047 exit 0, 2,048 give *fatal error: bracket nesting
level exceeded maximum of 2048*. So the number differs between clang builds,
and the Linux (Debian clang 22.1.8) and Windows (LLVM clang 22 and 23) legs
are unrun. The command that settles those two: the same 300-block file with
`clang -fsyntax-only` in the Linux container and on the Windows box. Nothing
in the sitting turns on it: the emitted C nests 4 deep (FFI Q3.2).

**C8. `fmt` is a walk too.** `<c>/heroes fmt` without `--in-place`, stdout
discarded: a 600-term `+` chain exits 134 in `printparens.render`, while
`parse` holds it at 0. 600 parentheses: `fmt` and `parse` both 134 in
`grammarexpr.binary`. The shapes I ran at 200 to 500 (`+` at 200 and 250,
`- -` at 250, `if` at 200, `g(` at 200, `((` at 500) format at 0; the others
were not run with `fmt`. So the list of
walks that fail per level is at least nine: the engineer's eight (my first
pass's C4 and its own table) and the formatter's `printparens.render`. Under
(3c) the parser's refusal comes first, if `fmt` stops at a parse
diagnostic; nobody ran `fmt` against a prototype.

**C9. (3e) is a backstop nobody built.** The warden and the historian both
lean on *exit 2, with the source position of the opener or link where the
stack ran out*. The guard is the runtime's (`runtime/parts/stack.c:21-25`),
shared by every program, and it knows a function, not a source position.
Naming the position needs the compiler to record where it is before it
descends, and to hook the guard's report. Neither is built or priced. The
historian's *what changes is what it prints and which code it returns* is an
inference. The command that settles feasibility and cost: a prototype that
stores the current node's span in a global before each recursive call in the
nine walks, and prints it from a compiler-registered handler, then timed
`check selfhost/main.hero` before and after on a still machine.

**C10. Claims measured on one platform.** The engineer's *it holds under any
`ulimit -s`* was measured at 8176, 1024 and 512 KB on Darwin. Its *no exit
134 ... on Linux x86-64 and on the Windows box* is a prediction (it says so).
The 256 MiB reservation is unrun on Linux (overcommit, `ulimit -v`) and on
Windows (`STACK_SIZE_PARAM_IS_A_RESERVATION`). Its own condition (*a platform
where the 256 MiB reservation is refused or delivers less*) is open.

**C11. The type depth is outside every counter.** The engineer measured
`emitsynth.collect` exit 134 at 1,000 nested records, and clang's own crash,
*Illegal instruction: 4*, reported as exit 2 internal error at 10,000. Its
flat declarations open nothing, so neither (3a) nor (3c) sees them. Only the
engineer proposed a bound; no seat priced its sentence or cost. It belongs to
the synthesis or to a defect.

## D. The blind seat's run

**D1. The path still names the checkout.** The repair renamed the leaf
folders (`rdr/t1` to `t3`). The probe's report (`llm-ergonomist-probe.md:11`)
shows the session told
`/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/.../rdr/p0`,
and each task header shows a scratchpad path holding `heroes-heroes-lang`
too. The spec the seat reads is titled *Report on the Programming Language
Heroes* (`spec/heroes-spec.md:1`), so the name adds nothing the input does not
carry. It is recorded because *with neutral names* (`llm-ergonomist.md:14`)
can be read as more.

**D2. Program B does not parse.** The blind task 3 session wrote program B by
hand and argued Y from counting (*102 in all, under 256*). Extracted verbatim
into `<c>/blindprog/t3-B.hero`, it has 101 openers and 100 closers
(`str.count`), and `check` says `error[unclosed_bracket]: ( opened here is never
closed` at `print(`, exit 1. One hand count, off by one, from the seat whose
approval of Y rests on *the author can count*.

**D3. Not verified by me**: that the three reports are the sessions' files
verbatim (they were copied from the sessions' scratchpads, which I did not
read), and the CLI's cost figures (7.20 USD in all; the arithmetic of
`llm-ergonomist.md:92-99` adds up: 0.34 + 1.23 + 1.75 + 1.66 + 2.22 = 7.20).

## E. The commands that would settle what is still open

Free, local:

1. The narrow (2a)'s census (B2): the engineer's `<ce>/tree2` with the narrow
   `ROUTE`, `<ce>/census.py` over `<ce>/corpus`.
2. The semantic-list test (B9): `constant OFF: bool` bodied `false`,
   `assert OFF`, and a `while true` whose only `break` sits under `if false`,
   through `<ce>/heroes-2a2` and `<ce>/heroes-2d`.
3. `fmt` under the (3c) prototype on a refused file (C8).
4. A (3c) with flat chains, priced in lines, and the floor route of C5 with
   its N measured at 1,000 and 2,000 on this Mac.
5. The Linux container and the Windows box: `ulimit -s` (Linux's default
   stack is unmeasured by every seat), the 256 MiB thread, and clang's bracket
   depth (C7, C10).

Paid, each for the coordinator to size and decide:

6. Task 1 re-run with the engineer's amended (1b) wording and F1 stated, to
   see whether the veto survives (A2). One session, 3 USD cap.
7. Task 2 re-run with (2d) as a fourth variant (B3). One session.
8. The frequency runs of A7: forgotten `f` and brace text in fresh writing,
   and the FFI seat's `}}` run.
9. `heroes measure spec/heroes-spec.md --refresh` on the adopted text, in the
   landing's tree (the warden's § 5).

## Verdict

None: the completeness critic gives no verdict (its brief).
