# Panel 184, the llm-ergonomist, task 1 (a forgotten `f`)

Run by the coordinator as a fresh session from `<scratchpad>/rdr/t1/` (brief `docs/panel/184-briefs/blind/brief-task1.md`), second attempt, 17:25 to 17:33 on 2026-09-30: `claude -p "Read brief.md in this directory and follow it exactly. Your inputs are the files in this directory only. Write report.md here. Answer in English." --restricted --safe-mode --strict-mcp-config --tools "Read,Write" --disallowedTools "Bash,WebFetch,WebSearch,Glob,Grep,Edit,Task,Agent,ListAgents,SendMessage" --max-budget-usd 3 --output-format json`, the default model; the CLI reported `total_cost_usd` 1.75286, 11 turns, `is_error` false. **The session could not write into its folder**: the run named its tools with `--tools` and did not pre-approve them with `--allowedTools`, so each `Write` there waited for a permission nobody could give in print mode; it wrote its report to its own session scratchpad instead (`/private/tmp/claude-501/-private-tmp-claude-501--Users-joseph-Temp-heroes-heroes-lang-edfda945-b355-427c-a48e-2113b1040b67-scratchpad-rdr-t1/6b65cee1-769d-4f29-9e50-d5360d425609/scratchpad/`), `report_steps_1_2.md` at 17:29, before it opened `o1-check.txt`, and `report_final.md` at 17:32, copied here verbatim below. Its `context` answer is in the report.

# report

## verdict

- **K: veto.** Whether `"{best} = "` is text or a compile error depends on whether
  the name `best` happens to be bound where the literal stands. That is not
  readable from the line, and not from the enclosing signature either: a binding
  introduced three lines earlier in the same body, or a `use` at the top of the
  file, silently changes the line's meaning. On `task1.hero` this is not
  theoretical: the compiler's own advice for one error (remove the unused
  `count`) deletes the other error. K also leaves undecided what a hole naming a
  built-in (`"{rows.len()}"`) or naming nothing (`"{1 + 2}"`) does.
- **L: object.** Line-local and unambiguous, but it makes the commonest
  interpolation slip, a forgotten `f`, into a program that compiles and prints
  braces. `o1-check.txt` is the demonstration: the only error it found was an
  unused binding, which is luck rather than diagnosis, and taking its advice
  produces a clean compile of a wrong program. L also deletes the only sentence
  that says how a closing brace is written inside an `f` literal.
- **M: approve.** The verdict is read off the literal's own text: does the run
  from `{` to the `}` that closes it parse as an expression. Nothing outside the
  line is consulted. It converts a forgotten `f` from a wrong program into a
  compile error that names the literal and whose repair is one character.

## experiment

**Step 1. Three programs, one per variant.** Target output in every case:

```
score 1 of 3: 12
score 2 of 3: 7
score 3 of 3: 30
best: 30 (score 3)
{best} = 30
```

The first four lines hold no literal brace, so they are written identically
under K, L and M. Only the last line moves.

Under **L**:

```
function main()
    scores = [12, 7, 30]
    n = len(scores)
    best: i64 @ scores[0]
    best_at: i64 @ 1
    for i in range(from: 0, to: n)
        s = scores[i]
        print(f"score {i + 1} of {n}: {s}")
        if s > best
            best @ s
            best_at @ i + 1
    print(f"best: {best} (score {best_at})")
    print("{best} = ", best)
```

Under **M**: the same, with the last line replaced by

```
    print(f"{{best}} = {best}")
```

Under **K**: the same as M, last line `print(f"{{best}} = {best}")`.

K's program equals M's and not L's because `best` is bound where the literal
stands, so under K the text `{best}` in a literal without the `f` is a hole
naming only what is bound there, which is exactly K's compile error. L's shorter
last line is available under K only if the accumulator is called something else:

```
    print("{best} = ", top)     # legal under K, error under M, legal under L
    print("{best} = ", best)    # error under K, error under M, legal under L
```

Same literal, same file, same output, opposite legality, and the deciding fact
is the name of a binding three lines up.

Checked against the spec for all three: `@` declarations carry their type (5);
`range(from:, to:)` must name its arguments, two parameters of one type (9), and
excludes `to` (11); `scores[i]` on an array yields `i64` and aborts out of
bounds, unlike a map index, which is a `V?` (10); `print` takes numbers and
`str` directly, with no separator and one trailing newline (11); every binding
is read, so none is unused (5); `print(...)` returns `()` and so stands alone as
a statement (5).

**Step 2. `task1.hero`, written before `o1-check.txt` was opened.** Two mistakes,
both the same slip.

Mistake 1, line 13: `print("{count} rows")`. Interpolation meant, `f` left off.

Mistake 2, line 14: `print("total {total} over {rows.len()} rows")`. Same slip,
two holes, one of them a call rather than a bare name.

Nothing else is wrong. `rows.len()` on line 6 is UFCS for `len(rows)` (9);
`"row-" + to_str(at)` is `str` concatenation (7); `total: i64 @ 0` carries the
type its `@` requires; the `f` literal on line 11 is well formed and prints
`row 3: row-3` and so on. The blank line 8 has no production in the grammar; I
read it, as an indentation-sensitive lexer does, as producing no token.

What each variant's words require:

- **Under L.** Both literals are unchanged, so both are text, and then nothing
  ever reads `count`: a write is not a use, but line 10's `total @ total + n`
  does read `total` on its right-hand side, so `total` is used and `count` alone
  is not. One unused-binding error, at line 6, and silence about lines 13 and 14.
  (I first wrote that `total` was unused too; the compiler's output corrected me.
  The correction makes L look better on this file than I had it, and changes no
  verdict.) Had `count` been passed anywhere, L would have accepted the file and
  printed `{count} rows` and `total {total} over {rows.len()} rows`.
- **Under M.** Line 13 is a compile error: `count` between the braces parses as
  an expression, so it would be a hole. Line 14 is a compile error twice, for
  `{total}` and for `{rows.len()}`. Each message names a literal, and the repair
  is the missing `f`.
- **Under K.** Line 13 is an error: `count` is bound on line 6 and in scope on
  line 13, and the hole names nothing else. Line 14's `{total}` is an error,
  `total` being bound on line 7. `{rows.len()}` is where K stops answering: it
  names `rows`, which is bound, and `len`, a built-in that nothing in this file
  binds. If built-ins count as bound where the literal stands, it is an error; if
  only the file's own bindings count, the hole does not name only what is bound
  and K's sentence does not fire, so one literal is half error and half text.
  K's words do not settle it. And the unused-binding error on line 6 fires as
  well, so the file carries two diagnostics whose repairs contradict: removing
  `count`, which the unused-binding message asks for, turns line 13 from a
  compile error into accepted text.

**Step 3. What `o1-check.txt` says, and what I would submit after it alone.**
The compiler reported exactly one error, `unused_binding` for `count` at
line 6:5, exit 1. It said nothing about lines 13 and 14. That is L's behaviour,
and it is what step 2 predicted for L, modulo the `total` correction above.

The message offers two repairs, "remove the binding, or read it". A model that
follows the message and knows nothing else takes the first, which needs no guess
about intent. Submitted program:

```
function label_for(at: i64) -> str
    return "row-" + to_str(at)

function main()
    rows = [3, 5, 8]
    total: i64 @ 0

    for n in rows
        total @ total + n
        print(f"row {n}: {label_for(n)}")

    print("{count} rows")
    print("total {total} over {rows.len()} rows")
```

Run in my head under the specification: it compiles, exit 0, and prints

```
row 3: row-3
row 5: row-5
row 8: row-8
{count} rows
total {total} over {rows.len()} rows
```

Both mistakes survive, the compiler is satisfied, and two of five output lines
are wrong. The second branch, "or read it", is no better: the only natural way to
read `count` is to add the `f` to line 13, which repairs that line by accident
and gives `3 rows`, but line 14 is still never named by anything, so it still
prints `total {total} over {rows.len()} rows`. Under L no repair path reaches
line 14 at all. Under M the same file would have produced three messages naming
three literals; adding both `f`s repairs everything, `count` becomes read, and
the one-turn result is correct: `3 rows` and `total 16 over 3 rows`.

**Step 4. Comparison.** A correct program in one turn is likeliest under **M**:
the only interpolation mistake the language permits is caught at the literal, and
the message's repair is the right one. A silently wrong program is likeliest
under **L**, as `o1-check.txt` and step 3 show. **K** alone makes a program's
meaning depend on something other than the line and its enclosing signature: the
set of names bound at that point, which includes every preceding binding in the
body and every `use` in the file. K is also the only variant under which
obeying one compiler message rewrites another message's verdict.

## choice_points

| # | Choice the spec left open | Taken | The other choice produces |
|---|---|---|---|
| 1 | Loop shape: index loop over `range(from:, to:)`, or `for s in scores` with a mutable counter | index loop | Compiles. The counter form is correct only if the increment is the last statement of the body; incrementing first prints `score 2 of 3: 12`. Silently wrong, all three variants. |
| 2 | Display numbers from 1, array indices from 0 (10) | `i + 1` | Compiles, prints `score 0 of 3: 12` and `best: 30 (score 2)`. Silently wrong, all three variants. |
| 3 | Initial `best` and `best_at` | `scores[0]` and `1` | `0` and `0` compiles and prints the same for this input, since 12 > 0 sets both on the first pass; it differs on all-negative scores, and `best_at: i64 @ 0` alone prints `(score 0)`. |
| 4 | Tie-break in the comparison | `>`, first maximum wins | `>=` compiles and prints the same here; on `[12, 30, 30]` it prints `(score 3)` instead of `(score 2)`. The task names no winner. |
| 5 | `n = len(scores)` bound, or `len(scores)` written inside the hole | bound once, read twice | Both legal. Binding it and then reading it only inside a literal without the `f` is an unused-binding compile error under L (5). That is precisely the trap `task1.hero` fell into, and the only reason L rejected it. |
| 6 | `len(scores)` or `scores.len()` | `len(scores)` | Identical; UFCS makes them one call (9). |
| 7 | **The literal-brace line**: `f` with `{{` and `}}`, a literal without the `f`, a concatenation, or several `print` arguments | `f"{{best}} = {best}"` under K and M, `print("{best} = ", best)` under L | Under M the literal without the `f` is a compile error. Under K it is a compile error if and only if `best` is bound there, so the same line is accepted or rejected according to a name chosen elsewhere. Under L both compile, but see 8. |
| 8 | How a closing brace is written inside an `f` literal | `}}`, which is K's and M's word and only theirs | Under L nothing defines `}}`: the surviving sentence names only `{{`. Reading a `}` that closes no hole as itself, `f"{{best}} = {best}"` prints `{best}} = 30` under L. Silently wrong, and the reason L's program avoids the `f` form on that line. |
| 9 | Under M and K, the concatenation escape hatch `print("{" + "best} = ", best)` | not used | It compiles under M and under K: `"{"` has no closing `}`, and `"best} = "` has no `{`, so neither literal holds anything that would be a hole. Both rules add friction to literal braces; neither forbids them, and neither leaves the author stuck. |
| 10 | One `f` literal per line, or several `print` arguments | one literal for the first four lines | Identical output; `print` writes its values with no separator (11). |
| 11 | `scores` bound with `=` or declared `: [i64] @` | `=` | Both legal, both read. `@` would need the written type (5). |
| 12 | Under K only: the accumulator's name | `best` | Naming it `top` makes `"{best} = "` legal text under K with unchanged output. One line's legality flips on another line's name. |
| 13 | Under M and K: what "would be a hole" covers | the run from `{` to its closing `}` parses as an Expression | `"{}"`, `"{ }"` and `"{a b}"` hold no expression, so they stay text under both. This is decidable from the literal alone under M; under K it needs the environment as well. |

## argument

M is line-local: the verdict comes from the literal's own text, and a forgotten
`f` becomes an error naming the literal. L is line-local too, but silent. o1
rejected `task1.hero` only because `count` was never read, which is luck, not
diagnosis; take the message's own advice, remove the binding, and the file
compiles and prints `{count} rows`. Nothing in L ever names line 14, so that
mistake survives every repair L can suggest. K is not line-local: the same
literal is text or an error depending on whether a name is bound nearby, so
obeying the unused-binding message rewrites the brace rule's verdict, and holes
naming built-ins are left undecided. Approve M, object to L, veto K.

## prediction

Over 100 one-turn tasks whose expected output interpolates a value, measured on
programs a model writes from this spec:

- **L**: a forgotten `f` in 15 to 25 of them. The unused-binding rule catches
  only those where the literal held the binding's sole read, which I put at
  about a third, so 10 to 17 compile and print braces. One-turn repair after the
  compiler speaks: below 10, because no message names a literal, and the message
  that does speak offers a repair (remove the binding) that keeps the bug.
  Additionally, any program writing a literal `}` inside an `f` literal is
  undefined under L; I expect implementations to disagree on `f"{{x}}"`.
- **M**: forgotten `f` surviving to a wrong program, 0 to 2, and those only
  through the concatenation hatch. Cost: tasks wanting a literal brace, which I
  put at 3 to 8 per 100, hit one compile error; one-turn repair from that
  message above 90, since the fix is `f` plus doubling the braces.
- **K**: silent wrong programs from a forgotten `f` about as rare as M's, but
  add a class neither other variant has. On files where an unused-binding error
  and a brace error name the same binding, as `task1.hero` does, a model
  following the first message reintroduces the second bug in 30 to 50 percent of
  attempts. And on a hole naming a built-in or naming nothing, two independent
  implementations of K disagree; I predict disagreement on `"{rows.len()}"` and
  on `"{1 + 2}"` specifically.

## condition

- I lift the **veto on K** if K's "bound where the literal stands" is shown to
  mean a set fixed by the line and the enclosing signature alone, for instance
  only that signature's parameters, or if an amended sentence settles the
  built-in and no-name cases. Line-local K would still be worse than M, but it
  would be an objection, not a veto.
- I downgrade **M to object** if literal braces turn out to be common in real
  code in this language, say above 10 percent of literals without an `f`, so
  that M's friction is paid on more programs than its error saves, or if the
  "would be a hole" test proves undecidable in practice on some literal I have
  not thought of, which would cost M its line-locality.
- I upgrade **L to approve** if measurement shows the unused-binding rule already
  catches above 90 percent of forgotten-`f` cases, which would make L's silence
  mostly theoretical. `task1.hero` is weak evidence the other way: it caught one
  of the two mistakes, and the caught one was repaired by a message that left the
  bug in place. Any run of a real corpus, or a single file where an M compiler
  rejects a literal a competent author meant as text, moves these verdicts.

## context

Only this directory's files reached my context as inputs: `brief.md`, `spec.md`,
`task1.hero` and `o1-check.txt`, read in that order, with `o1-check.txt` opened
only after steps 1 and 2 were written to a file. I read nothing outside this
directory and used no tool other than reading and writing files.

Two disclosures. First, my write of `report.md` was not permitted at the moment
steps 1 and 2 were finished, so I wrote them instead to this session's scratchpad
directory, outside this directory, at `report_steps_1_2.md`, then opened
`o1-check.txt`, then wrote this file. That is a departure from "writing files
here", forced by the permission, and it preserved the ordering the brief asked
for; the contents of that scratchpad file are reproduced in steps 1 and 2 above,
with the `total` correction added and marked. Second, the harness supplied its
usual environment preamble before the brief: working directory, platform, OS
version, today's date, my model identity, and, in a system reminder, the user's
email address. None of it bears on the judgement and none of it was used.


---

## Appendix: the session's steps 1 and 2, as written at 17:29 before it opened the compiler output

# report (steps 1 and 2, written before o1-check.txt was opened)

## verdict

Provisional, written before `o1-check.txt` was opened.

- **K: veto.** Whether `"{best} = "` is text or a compile error depends on whether
  the name `best` happens to be bound at that point. That is not readable from
  the line, and not from the enclosing signature either: a binding introduced
  three lines earlier in the same body, or a `use` at the top of the file,
  silently changes the line's meaning. K also leaves undecided what a hole that
  names nothing (`"{1 + 2}"`) or a hole that names a built-in (`"{rows.len()}"`)
  does.
- **L: object.** Line-local and unambiguous for non-f literals, but it makes the
  commonest interpolation slip, a forgotten `f`, a program that compiles and
  prints braces. L also deletes the only sentence that says how a closing brace
  is written inside an `f` literal, so `f"{{best}} = {best}"` has no defined
  output under L.
- **M: approve.** The decision is made from the literal's own text: does the run
  from `{` to its closing `}` parse as an expression. Nothing outside the line
  is consulted. It converts the forgotten `f` from a wrong program into a
  compile error that names the literal.

## experiment

**Step 1. The three programs.** Target output, for all three:

```
score 1 of 3: 12
score 2 of 3: 7
score 3 of 3: 30
best: 30 (score 3)
{best} = 30
```

The first four lines are written identically under all three variants, because
they contain no literal brace. Only the last line moves.

Under **L**:

```
function main()
    scores = [12, 7, 30]
    n = len(scores)
    best: i64 @ scores[0]
    best_at: i64 @ 1
    for i in range(from: 0, to: n)
        s = scores[i]
        print(f"score {i + 1} of {n}: {s}")
        if s > best
            best @ s
            best_at @ i + 1
    print(f"best: {best} (score {best_at})")
    print("{best} = ", best)
```

Under **M**: the same, with the last line replaced by

```
    print(f"{{best}} = {best}")
```

Under **K**: the same as M, last line

```
    print(f"{{best}} = {best}")
```

Why K's program equals M's and not L's: `best` is bound at the point the
literal stands, so under K the text `{best}` in a non-f literal is a hole
naming only what is bound there, which is exactly K's compile error. The
L-shaped last line is available under K only if the accumulator is called
something else:

```
    print("{best} = ", top)          # legal under K, error under M, legal under L
    print("{best} = ", best)         # error under K, error under M, legal under L
```

Checks against the spec: `@` declarations carry their type (5); `range(from:,
to:)` needs its argument names, two parameters of one type (9), and excludes
`to` (11); `scores[i]` on an array yields `i64` and aborts out of bounds, it is
not a `V?` like a map index (10); `print` takes numbers and `str` directly, no
separator, one trailing newline (11); every binding is read, so none is unused
(5); `print(...)` returns `()` and stands alone as a statement (5).

**Step 2. task1.hero.** Two mistakes.

Mistake 1, line 13: `print("{count} rows")`, interpolation meant, `f` left off.
Mistake 2, line 14: `print("total {total} over {rows.len()} rows")`, same slip,
two holes, one a call.

Nothing else is wrong: `rows.len()` is UFCS for `len(rows)` (9), `"row-" +
to_str(at)` is `str` concatenation (7), `total: i64 @ 0` carries its type, the
`f` literal on line 11 is well formed. The blank line 8 has no production in the
grammar; read as producing no token.

- **Under L.** Both literals are text. Then nothing reads `count` and nothing
  reads `total`, and a write is not a use (5). The compiler must reject with two
  unused-binding errors, lines 6 and 7, saying nothing about lines 13 and 14.
  Loud failure by accident, pointing away from the mistake. Had `count` been
  passed somewhere and `total` returned, L would accept and print
  `{count} rows` and `total {total} over {rows.len()} rows`.
- **Under M.** Line 13 a compile error, `count` parses as an expression. Line 14
  a compile error twice, `{total}` and `{rows.len()}`. The repair is the `f`.
- **Under K.** Line 13 an error, `count` bound on line 6. Line 14: `{total}` an
  error, `total` bound on line 7. `{rows.len()}` is where K stops answering: it
  names `rows`, bound, and `len`, a built-in bound by nothing in this file. If
  built-ins count, an error; if only the file's bindings count, the hole names
  something not bound and K does not fire. K's words do not settle it. Also,
  under K, deleting the unused binding on line 6 turns line 13 from a compile
  error into accepted text.

## choice_points

1. Loop shape: index loop over `range(from:, to:)` vs value loop with a mutable
   counter. Taken: index loop. Other: compiles; the counter form is right only
   if the increment is last in the body, else `score 2 of 3: 12`. Silent, all
   variants.
2. 1-based display vs 0-based indices (10). Taken `i + 1`. Other: compiles,
   prints `score 0 of 3: 12`, `best: 30 (score 2)`. Silent, all variants.
3. Initial `best`/`best_at`: `scores[0]`/`1`. Other: `0`/`0` compiles and prints
   the same here (12 > 0 on the first pass), differs on all-negative input;
   `best_at @ 0` alone prints `(score 0)`.
4. Tie-break `>` vs `>=`. Same output here; `[12, 30, 30]` would differ.
5. `n = len(scores)` bound vs inline in the hole. Both legal; bound and not read
   is an unused-binding compile error (5). This is L's trap in task1.hero.
6. `len(scores)` vs `scores.len()`. Identical under UFCS (9).
7. The literal-brace line: `f` with `{{`/`}}`, non-f literal, concatenation, or
   several `print` arguments. Under M the non-f literal is an error; under K an
   error iff `best` is bound; under L both compile.
8. How a closing brace is written in an `f` literal: `}}`, K's and M's word.
   Under L nothing defines `}}`, the surviving sentence names only `{{`, so
   `f"{{best}} = {best}"` prints `{best}} = 30`. Silent.
9. Under M and K the concatenation escape hatch `print("{" + "best} = ", best)`
   compiles: `"{"` has no closing `}` and `"best} = "` has no `{`. Friction, not
   a ban.
10. One `f` literal per line vs several `print` arguments: identical (11).
11. `scores` with `=` vs `: [i64] @`: both legal and read.
12. Under K only: the accumulator's name. `top` instead of `best` makes
    `"{best} = "` legal text. One line's legality flips on another line's name.
