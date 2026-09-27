# Panel 180, llm-ergonomist

(Written to disk by the coordinator from the seat's final message, verbatim;
the harness refused the seat's own write. The seat noted: *"I read the brief
and `spec/heroes-spec.md` and nothing else. The session harness also put
`CLAUDE.md` and three `.claude/rules/` files into my context without my
asking. I did not use them, and none of them says what the compiler does on
these fragments. P is the text now at spec lines 11-12, which I could not
avoid seeing."*)

## verdict

**Approve R**, preferably in the shorter wording S below. **Object to P.** Q is acceptable but ranks below R. **No veto**: none of the three is non-local.

## experiment

`NL` marks the line break in a fragment. A means the parser accepts; X means it refuses, loudly.

Short names for the words I relied on:
- **P-sep:** "where it separates, a production writes it"
- **P-else:** "elsewhere it may fall between any two tokens"
- **Q-cont / R-cont:** "may end after an operator, `.`, `,`, `:` or an opening bracket and go on below"
- **Q-close:** "stands only before a closing bracket other than an index's `]`"
- **Q-comma:** "before a `,` between a call's arguments"
- **R-close:** "stands only before a closing bracket"
- **Q-prod / R-prod:** "where a production writes it"

Grammar facts I used:
- Only `Sep = "," | NEWLINE` (section 7) writes a NEWLINE inside a bracket, and only between the elements of an array or map literal.
- `Args` (section 9), `Entry`, the index `"[" Expression "]"` and `"(" Expression ")"` write none.

### Task 1: thirty predictions

| # | break | P | Q | R |
|---|---|---|---|---|
| 1 | `2` NL `)` | A, P-else | A, Q-close | A, R-close |
| 2 | `1` NL `+` | A, P-else | X: the line ends with a literal, and `+` is none of Q's three places | X, same test against R's two places |
| 3 | `+` NL `2` | A, P-else | A, Q-cont | A, R-cont |
| 4 | `0` NL `]` (index) | A, P-else | X, "other than an index's `]`" | A, R-close (no index exception) |
| 5 | `1` NL `,` (call) | A, P-else | A, Q-comma | X: `,` is not a closing bracket, and `Args` writes no NEWLINE |
| 6 | `1` NL `,` (array) | A, **a guess** (hesitation 1) | X: this `,` separates array elements, not a call's arguments, so the NEWLINE can only be the Sep, and a `,` after a Sep cannot start an Expression | X, same through R-prod |
| 7 | `xs` NL `.len()` | A, P-else | X: the line ends with a name, and `.` is none of the places | X |
| 8 | `.` NL `len()` | A, P-else | A, Q-cont | A, R-cont |
| 9 | `"a"` NL `:` | A, P-else (`Entry` writes none, and an unfinished entry cannot be followed by a Sep) | X: `:` is none of the places | X |
| 10 | `[` NL `1` NL `2` NL `]` | A: first and third breaks are P-else, the second is P-sep | A: Q-cont after `[`; the NEWLINE after `1` is the Sep; the NEWLINE after `2` stands before a literal's `]` (Q-close) | A, same through R |

What the columns say:
- **P** accepts all ten; fragment 6 is a guess.
- **Q** accepts 1, 3, 5, 8 and 10.
- **R** accepts 1, 3, 4, 8 and 10.
- Q and R differ only on fragments 4 and 5.
- Under Q and R every answer can be read straight off the sentence.

Two caveats that apply to all three sentences:
- Each fragment leaves `x`, `n`, `m` or `ys` unused, so the checker (section 5) would refuse all ten as written. My answers are about the parser only, as the brief asks.
- No sentence says how a continuation line is indented. The fragments indent by 0 or 4 spaces, so I assumed indentation is free inside brackets.

### The silent case that is not among the ten

```
    deltas = [
        1
        -1
    ]
    print(deltas.len())
```

- **Under P, the answer is undetermined.** The break after `1` is a place where the Sep may be a NEWLINE (P-sep). It is also a place where a NEWLINE "may fall between any two tokens" (P-else), and there `1 - 1` is a `Sum`. So `[1, -1]` and `[0]` both parse and both type-check, and P does not say which one wins. The program prints 2 or 1.
- **Under Q, R and S, it prints 2.** The line ends with a literal, so its NEWLINE can only be the Sep, and the array is `[1, -1]`. A writer who wants the subtraction puts `1 -` at the end of the line.
- **Only unary `-` causes this.** `+` cannot start an element, since section 7's `Unary` is only `-` `!` `~`. An element that starts a line with `(`, `[` or `.` gives a type clash that is refused loudly in nearly every program.

### Task 2: writing under each sentence

This part is identical under P, Q, R and S. It breaks after `(` and after each `,`, and the last line ends with a name before `)`.

```
constant MAXIMUM_ATTEMPTS: i64
    5

function open_connection(
    remote_host_name: str,
    remote_port_number: i64,
    connect_timeout_millis: i64,
    maximum_retry_attempts: i64
) -> i64
    return remote_port_number + connect_timeout_millis + maximum_retry_attempts + remote_host_name.len()

function main()
    handle = open_connection(
        remote_host_name: "db.internal.example",
        remote_port_number: 5432,
        connect_timeout_millis: 250,
        maximum_retry_attempts: MAXIMUM_ATTEMPTS
    )
    attempts = 2
    shutdown_requested = false
```

The condition is where the sentences produced different code. Under P, my first draft was:

```
    keep_trying = (attempts < MAXIMUM_ATTEMPTS && !shutdown_requested
        && handle > 0)
    if keep_trying
        print("retrying")
```

Under Q, R and S I wrote:

```
    keep_trying = (attempts < MAXIMUM_ATTEMPTS && !shutdown_requested &&
        handle > 0)
    if keep_trying
        print("retrying")
```

Why I broke where I did:
- **The call.** Breaking after `(` and every `,`, with the closing bracket alone on its line, is legal under all four wordings. It is also what I would write from Rust, Go and Python habit, so no sentence changed it.
- **The leading comma.** Q would also allow `, remote_port_number: 5432` at the start of a line. I did not choose it, and R refuses it.
- **The condition under P.** My first draft put `&&` at the start of the continuation line, as rustfmt and Black both do, because P says any two tokens will do.
- **The condition under Q and R.** I moved `&&` to the end of the line. Their first clause lists where a line may end, and a name is not on the list. This is the only place where my program differed between sentences.
- **Binding `keep_trying`.** I gave the condition a name instead of writing `if (...)`, because section 1 says "no parentheses around conditions". I cannot tell from the spec whether that describes the syntax or forbids the form.

### Task 3: a shorter sentence

S gives the same ten answers as R in 37 words instead of R's 60. I counted words by hand, not tokens.

> **S.** Inside `(` `[` `{` a NEWLINE never ends a statement, and a line there may break only after an operator, `.`, `,`, `:` or an opening bracket, before a closing bracket, or where a production writes NEWLINE.

Checked against the ten fragments:

| # | break | what S allows there | result |
|---|---|---|---|
| 1 | before `)` | before a closing bracket | A |
| 2 | `1` NL `+` | nothing | X |
| 3 | after `+` | after an operator | A |
| 4 | before `]` | before a closing bracket | A |
| 5 | before a call's `,` | nothing (`Args` writes no NEWLINE) | X |
| 6 | `1` NL `,` | only the Sep, so the `,` follows a Sep | X |
| 7 | `xs` NL `.` | nothing | X |
| 8 | after `.` | after `.` | A |
| 9 | `"a"` NL `:` | nothing (`Entry` writes no NEWLINE) | X |
| 10 | three breaks | after an opening bracket; the Sep; before a closing bracket | A |
| extra | `[1` NL `-1]` | only the Sep | `[1, -1]` |

S also closes a gap in R. R's two lists do not cover a line that ends in `true`, `nullptr`, `lent`, `consumes`, `borrows`, `->` or `::`. S needs no list of tokens that end an operand: anything not on its "after" list may break only before a closing bracket or at a Sep.

If the sitting keeps Q's behaviour, the same shortening gives: *"... before a closing bracket other than an index's `]`, before a `,` between a call's arguments, or where a production writes NEWLINE."*

P has no shorter repair with the same answers, because P does not settle fragment 6 or the `deltas` case. Any repair has to add a tie-break, and the cheapest tie-break is a rule about how the line ends, which is Q or R.

## hesitation_points

1. **P, fragment 6.** Is a NEWLINE directly before a container's `,` "where it separates"? The comma does the separating, so I read the NEWLINE as falling between two tokens and said accept. A reader who treats any NEWLINE after an element as the Sep says refuse. Section 10's "by newline across lines and by comma on one" fits neither reading. **A wrong guess is loud either way.**
2. **P, an element that starts with `-` on a new line.** The array is either `[1, -1]` or `[0]`. **A wrong guess is silent**: a different array, of a different length, that compiles. This point decides my verdict.
3. **Q, fragment 4.** To tell whether a `]` closes an index or a literal, the reader has to find its `[`. In `table[slot_of(key: k,` NL `seed: 3)` NL `]` that `[` is two lines up. Loud.
4. **Q, fragments 5 and 6.** A leading `,` is legal in a call and in a record construction (section 9 calls it "a call with field names"). It is not legal in an array, a map, a `function` parameter list, a function type's `TypeArgs` or an extern's `CParam`s. A reader who learns the leading comma from a call and uses it in a declaration gets a loud error.
5. **Q and R, unclassified line endings.** Neither list covers a line ending in `true`, `false`, `nullptr`, `lent`, `consumes`, `borrows`, `->` or `::`. I read `true` and `false` as literals. Before `)` both readings accept, and anywhere else both are loud.
6. **All three, indentation.** None says how a continuation line is indented, while section 1 says "exactly 4 spaces per level". Python-style alignment under the opening parenthesis (column 29 in my call) might be refused. Loud if so; I avoided it.
7. **All three, task 2.** Section 1's "no parentheses around conditions" clashes with the only way to break a condition across lines. I bound the condition to a name. Loud if the checker refuses `if (...)`.
8. **Q and R, writing.** The rustfmt and Black habit of starting a line with the operator is refused. The error is loud and costs first-try rate, and its fix is certain: move the operator up a line.
9. **Q and R, vocabulary.** "Carries a NEWLINE, which stands" is lexer language. A writer's question is "may I break here?", and S answers that directly.
10. **All three, trailing commas.** A trailing comma before `)` or `]`, a Rust and Python habit, is refused by `Args` and by `{ Sep Expression }`. Q and R's "may end after `,`" could be misread as allowing `x,` NL `)`. Loud.

## argument

P leaves `[1` NL `-1]` undetermined: `[1, -1]` and `[0]` both parse and type-check, and P picks neither. That is the only silent class in this question, and it comes from the plainest vertical table a model writes. Q and R close it the way Go does: how a line ends decides whether it continues. A reader applies that from the two lines around the break, so neither is non-local, and every misprediction left under Q or R is loud. R has three facts to hold. Q has five, and its two extras (an index's `]`, a call's `,`) cover shapes a model almost never writes and require knowing what the bracket is for. S states R in 37 words and closes R's keyword gap.

## prediction

1. **Scorable now against the coordinator's record.** Whichever sentence the compiler implements, my column for it matches the compiler on 10 of 10 with no guesses. My P column (accept all ten) matches a compiler that behaves as Q, or as R, on exactly 5 of 10. I expect the compiler to match my Q column exactly: accept 1, 3, 5, 8 and 10, refuse 2, 4, 6, 7 and 9.
2. **Runnable now.** The `deltas` program above, built by today's `heroes`, prints `2`.
3. **At the next harness run, under Q, R or S.** Generated programs that break a line inside brackets show zero silent misparses. More than half of the loud refusals at those breaks come from a leading `&&`, `||` or `+`, or from a trailing comma.

## condition

- **If the `deltas` program prints `1`:** the compiler joins lines across that break. Q and R then both misdescribe it on the one silent case, and my approval becomes an objection to all three until the compiler's rule is decided.
- **If the compiler implements Q and the compiler seats will not change it:** adopt Q in its short form. The difference from R is two rare shapes, both loud, and a sentence that disagrees with the compiler costs more than two facts do.
- **If a harness run shows operator-first breaks inside parentheses in more than a third of multi-line conditions:** I would ask the sitting to measure a route none of the three candidates describes. This is unrun and a question, not a finding. A NEWLINE would be a plain space inside `(` and inside an index's `[`, and the line-end rule would apply only inside container literals. That keeps `[1` NL `-1]` settled and accepts fragments 2, 5 and 7.

## Scored by the coordinator, 2026-09-27, before the synthesis

- Prediction 1: today's compiler (`heroes parse` at `29ed5601`) accepts 1, 3, 5, 8 and 10 and refuses 2, 4, 6, 7 and 9, the seat's Q column exactly: **10 of 10, held**. The P column matches it on 5 of 10: **held**.
- Prediction 2: the `deltas` program, `heroes check` exit 0 and `heroes run` prints `2`: **held**.
- Prediction 3: scored at the next harness run, not yet.
