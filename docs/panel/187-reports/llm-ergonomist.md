# Panel 187, the llm-ergonomist's reading: five programs, five sessions, each correction built

Run by the coordinator 2026-10-02 from 22:13:59 to 22:15:10 by `date`, after the
critic's first pass and the briefs' repair, on the trunk at `07ccb72a`. Five
fresh sessions, one per program (`docs/panel/187-briefs/llm-ergonomist.md`), each
in its own folder `<scratchpad>/rb1/` to `rb5/`, outside the repository and any
git tree, no `CLAUDE.md` in or above it (the critic's walk to `/`,
`completeness-critic-briefs.md` § 3), each holding `spec.md` (the trunk's spec,
`cmp`-identical), the program as `p.hero`, and `brief.md` instantiated from
`docs/panel/187-briefs/blind/brief.md` with the program, what it should do and
what the sitting's compiler printed for it (the five instantiated briefs are
`docs/panel/187-briefs/blind/rb1-brief.md` to `rb5-brief.md`). Command:
`.claude/skills/panel/SKILL.md` § 2's, `--model claude-opus-5-5`,
`--max-budget-usd 3` each; every session `success`, **0.851 USD in all**.
Each session's `context` names `brief.md` and `spec.md` and the harness's own
system prompt, which carries an account email; no project rule, contract or
memory. The readings stand.

**Each correction built by the coordinator** on the sitting's compiler
(sha256 prefix `60cc49e98b56ddc0`), each `c.hero` copied to
`<scratchpad>/rb-measure/<n>/` and run there, `check` then `run`, its output
compared with what the program should print:

| | stands for | should | ran | turns | USD | the correction, measured | one turn |
|---|---|---|---|---|---|---|---|
| p1 | class (a), the C `for` header (four messages) | print 0, 1 and 2 | 22:13:59 to 22:14:26 | 5 | 0.162 | `check` 0, `run` 0, prints `0, 1, 2` | yes |
| p2 | class (b), its easiest case | print 7 | 22:14:04 to 22:14:38 | 5 | 0.152 | `check` 0, `run` 0, prints `7` | yes |
| p3 | the shape `g2/r09` (a second message at `function main()`) | print 3 | 22:14:09 to 22:14:38 | 5 | 0.158 | `check` 0, `run` 0, prints `3` | yes |
| p4 | class (b), row 130-34a's shape | print 2 | 22:14:14 to 22:14:49 | 7 | 0.193 | `check` 0, `run` 0, prints `2` | yes |
| p5 | a control | print 3 | 22:14:19 to 22:15:10 | 7 | 0.187 | `check` 0, `run` 0, prints `3` | yes |

**Five one-turn repairs of five.** One sample per program and one model: a
rate over many attempts is the sessions' predictions below, not a
measurement. Three readings bear on the questions: p2's session says *the spec
did most of the work* (the message's hint about the body fits a wrong idea of
the mistake, and following it literally deletes the return type); p4's says
*a note like "list opened at 2:9 is not closed" would have made the repair
near certain*, which is Q2's reworded-message route; p3's read the second
message as a cascade of the first and says *a model could misread it and add
a body to `main`*.

## p1, `rb1/report.md`, verbatim

# experiment

```
function main()
    for i in range(from: 0, to: 3)
        print(i)
```

# reading

- `for_missing_in` ("`for` iterates, `for x in xs`") led me to rewrite the header as `for i in ...`. I ignored its `fix (guess): use while` because a counted loop maps directly onto `for x in` over a range. The spec (section 8) says a `for` can loop over a `range`, and section 11 gives `range(from: a, to: b) -> [i64]`, excluding `to`. I added the named arguments because section 9 requires them when two parameters share a type.
- The two `unexpected_character` errors for `;` (and spec section 1, "No braces, no semicolons") led me to remove the C-style header completely, along with `i++`.
- `missing_body` ("an indented body, exactly 4 spaces (found `{`)") led me to drop `{` and `}` and indent `print(i)` 4 spaces below the `for`.

# confidence

High. Every construct is in the grammar: `For = "for" ident "in" Expression Block`. `range` is a built-in that returns `[i64]` and takes the named arguments `from:` and `to:`. `print` takes an `i64`, and `i` is read, so it is used. `main` takes nothing and returns nothing. The one uncertainty is the exact spelling of `range`'s parameters. Section 11 writes `range(from: a, to: b)`, so I followed that.

# argument

The messages made a one-turn repair likely. Each one pointed at a real defect: the semicolons, the missing `in`, and the braces where an indented block belongs. The `for x in xs` hint shows the right loop shape. They fell short in two places. First, the guessed fix `use while` points away from the simplest repair. A model that follows it must also invent `i: i64 @ 0` and `i @ i + 1`, which no message mentions, and could easily write `i = 0` (immutable) or `i++` again. Second, no message names `range` or its mandatory named arguments `from:`/`to:`. A model writing `range(0, 3)` or `range(3)` would fail, and only the spec saves it.

# prediction

Over 100 models given this directory, 75 to 90 produce a program that compiles and prints 0, 1, 2 in one turn. Below 65 or above 95 would falsify this.

# context

Only `brief.md` and `spec.md` from this directory reached my context, plus the harness's system context (environment details and an account email, which I did not use). I read no other files and ran no compiler.

## p2, `rb2/report.md`, verbatim

# experiment

```
function seven() -> i64
    return 7

function main()
    print(seven())
```

# reading

The message points at the `:` at 1:17 and says the line should have ended there, with the body on the lines below. Taken literally, that edit would delete `: i64 )` and leave `function seven()`, a function that returns `()`. Then `return 7` would fail. I did not take it literally. The program has to return a value, so the head needs a return type, and the spec's grammar (section 4: `"function" ident [ Generics ] Params [ "->" Type ] Block`) and the `dist2` example write it as `-> Type`. So I replaced `: i64` with `-> i64`. The message never mentioned the stray `)`. I removed it because nothing in the `Declaration` production allows it.

# confidence

High. The fixed head matches the production exactly. `return 7` gives an `i64` literal to an `i64` result, and `print` takes an `i64`. `main` takes nothing and returns nothing. There are no unused bindings, no tabs, and the indentation is 4 spaces. The only doubt is whether something outside the spec's text matters, and I see nothing like that.

# argument

The message points at the right place: the `:` is where the head stops being valid. Its hint is about the body going on the next line, though, and suggests nothing about the return type, so it fits a wrong idea of what went wrong. A model that follows it literally deletes the type and gets a second error at `return 7`. The repair works because the spec shows `->` clearly and because the program has to return 7, not because the message explains it. The message also says nothing about the stray `)`, but a model rewriting the head would probably drop it anyway. The message did not make a one-turn repair likely by itself. The spec did most of the work.

# prediction

85 of 100 models write a `c.hero` that compiles and prints 7 on the first try. The rest mostly delete the return type as the hint suggests, or keep the stray `)`.

# context

Nothing reached my context except `brief.md` and `spec.md` from this directory, plus the system and harness instructions for this session (including an attached account email, which I did not use). I read no other files and used no tool other than reading and writing files here.

## p3, `rb3/report.md`, verbatim

# experiment

```
record Point
    x: i64
    y: i64

function main()
    p = Point(x: 1, y: 2)
    print(p.x + p.y)
```

# reading

The first message, `expected_field` at 4:5, said that a `record` holds only `name: type` lines and that a function belongs at the top level of the file, taking the record as a parameter. Nothing calls `sum`, and `main` already computes `p.x + p.y` itself, so I deleted the `function sum() -> i64` line instead of moving it. That leaves `Point` with only its two fields, which matches `Fields = INDENT { ident ":" Type NEWLINE } DEDENT`. I read the second message, `missing_body` at 6:1, as a cascade from the first: the parser took the bodiless `function sum` line as a declaration and then reached the dedent before `main`. Deleting that line fixes it too, so I made no separate edit for it.

# confidence

High. What remains matches spec examples exactly: the record is the `Point` from section 4, the construction uses named fields (section 9), and `print` takes an `i64` and returns `()`, so it stands alone as a line (section 5). There are no unused bindings, since `p` is read. The output is 3.

# argument

Yes, the message made a one-turn repair likely. The first error names the exact rule (a record holds only `name: type` lines) and the fix (a top-level function taking the record). That is enough to either delete the line or move it out. The weak point is the second error. It describes `missing_body` at `main`, line 6, which is a cascade, and it does not say so. A model could misread it and add a body to `main`, or indent something, instead of seeing that it disappears with the first fix. A model that takes the "move it to the top level" advice literally must also write a body for `sum`, and use it, which adds some risk.

# prediction

90 of 100 models produce a `c.hero` that compiles and prints 3 in one turn. Most failures would come from moving `sum` out with a faulty signature, or leaving it with no body or a mistake in its body.

# context

Only `brief.md` and `spec.md` from this directory, plus the system context of the session, which gave the environment and the user's email address. No other files or outside sources reached my context.

## p4, `rb4/report.md`, verbatim

# experiment

```
function main()
    x = [1, 2]
    print(x.len())
```

# reading

The message `expected_separator` at 3:20 says the parser was still inside a list, reading elements, when it met the stray `)`. That told me two things. First, the `)` at the end of line 3 is extra, so I deleted it. Second, the parser treated `print(x.len())` as a list element. That can only happen if the `[` on line 2 was never closed. Under the spec, line 2 ends with the literal `2` inside a bracket, so it keeps its NEWLINE as a separator, and line 3 becomes the next element. So I added the missing `]` after `2`. The message does not name the second edit. I worked it out from the spec's bracket and NEWLINE rule and from the program's stated purpose.

# confidence

High. `c.hero` has balanced brackets and one statement per line at 4-space indentation. `x` is an immutable binding that gets used, `len` is a built-in reached by UFCS, and `print` takes an `i64`. I see no other rule it breaks. It should compile at once and print `2`.

# argument

Only partly. The caret points correctly at the stray `)`, and deleting it is half the fix. But the most useful fact is hidden: the parser is inside a list opened on line 2. The message never mentions the unclosed `[` or line 2. A model that follows the caret literally will delete the `)` and resubmit, leaving `[1, 2` unclosed, which fails again. The repair is still likely: the program is three lines long, the stated purpose ("the length of the list") points to the list, and "between one element and the next" hints that a list is still open. A note like "list opened at 2:9 is not closed" would have made the repair near certain.

# prediction

85 of 100 models will repair the program in one turn, so that it compiles and prints 2. Most failures will delete only the `)` and leave the `[` unclosed.

# context

Only this directory's `brief.md` and `spec.md` reached my context, plus the harness system prompt and an automatically attached user email address, which played no part in the task. I read no other files and used no tools other than reading and writing files here.

## p5, `rb5/report.md`, verbatim

# experiment

```
function main()
    total = 1 + 2
    print(total)
```

# reading

The single message, `error[unknown_name]` at p.hero:3:11, said `totl` is not in scope and suggested `total`, with `fix (certain): rename to `total``. I made exactly that edit: `print(totl)` became `print(total)`. Nothing else changed. The edit also fixes a second problem the message did not have to name: with `totl` read instead, `total` was a binding with no read, which section 5 makes a compile error. Renaming the read makes it used.

# confidence

High. After the edit, `main` takes nothing and returns nothing (section 1); `total = 1 + 2` is an immutable binding inferred as `i64` (sections 2 and 5); it is read once, so it is not unused; `print` takes a number and writes `3` with one trailing newline (section 11). Indentation is 4 spaces, ASCII only. I see no rule in the spec the corrected program breaks.

# argument

Yes. The message names the error kind, the exact location with a caret, the likely intended name, and a concrete edit labelled certain. The edit is a one-token substitution and the result is the whole correct program. Nothing in the spec complicates it: the only latent second error (unused `total`) disappears with the same edit, so no follow-up error hides behind this one. A model could still fail only by going beyond the suggested rename, for example rewriting the binding as mutable without the required type, or adding a type annotation with the wrong syntax. No part of the message misled.

# prediction

At least 97 of 100 models will produce a one-turn repair that compiles and prints 3.

# context

Only this directory's files reached my context: `brief.md` and `spec.md`. The system prompt also carried the harness's general instructions and the user's account email, which played no part in the judgment. I read no other file and ran nothing.
