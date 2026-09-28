*Written out by the coordinator from the seat's final message on 2026-09-28 at
03:46: the seat was told to return its report as its final message. The text
below is the seat's, unchanged.*

# Panel 181, llm-ergonomist, second reading (blind)

I read two files: the brief (`/Users/joseph/Temp/heroes/heroes-lang/docs/panel/181-briefs/llm-ergonomist-second-reading.md`) and the whole candidate (`/Users/joseph/Temp/heroes-recovery-2026-09-26/panel-181/llm-ergonomist-second-reading/heroes-spec-candidate.md`). I did not use the project instructions that were loaded into my context, and I read nothing else. I have no compiler at this seat, so every outcome below is a prediction and has not been run.

## verdict

**approve**

## experiment

**Columns I counted.** The base is the column of each fragment's first token.
- 1: +0
- 2: +4
- 3: +0
- 4: +0, then +4
- 5 to 9: +0
- 10: `double` +4, `else` +0, `triple` +4, `(5)` +0
- 11: +0
- 12: `b)` +8, `print(1)` +4
- 13: +0
- 14: arms and `1` at +4
- 15: +0, then +4

None of my answers depends on these counts. A +4 after a line that takes no block is an unexpected indentation, which is also refused. If `(5)` in fragment 10 sat at +4, it would join the else block and leave `triple` as an unused value, which is refused too.

**Task 1, predictions under the candidate.** "Break rule" means the added last sentence of the opening paragraph: *"Outside brackets a line ends its statement: it may not end where the statement cannot, and the next line may not go on with it"*.

| # | outcome | effect | words relied on |
|---|---|---|---|
| 1 | refused | none | break rule: "may not end where the statement cannot" |
| 2 | refused | none | same; the +4 does not help: "the next line may not go on with it" |
| 3 | accepted | `y` = 6 | "Inside `(` `[` `{` a NEWLINE never ends a statement"; a line ending in `+` is not on the keep list, so it "goes on below, at any column" |
| 4 | refused | none | "a long expression, a condition included, breaks inside parentheses" |
| 5 | refused | none | "may not end where the statement cannot" (it ends on `.`) |
| 6 | refused | none | "the next line may not go on with it"; `+ 1` alone does not parse, because Unary has only `-` `!` `~` |
| 7 | refused, never `total` = 13 | none | "the next line may not go on with it"; `- 2` is then a line whose value is unused (§5 "A line that computes a value must use it"), and possibly also the "`-` that does not touch its operand" rule |
| 8 | refused | none | "may not end where the statement cannot" |
| 9 | accepted | `ys` = `[1, 2, 3]` | a line ending in `,` is not on the keep list, so it goes on; `Sep = "," \| NEWLINE` |
| 10 | refused, `g` never becomes `double(5)` | none | "a statement whose last part is a block ends with that block instead"; `(5)` is its own line, an unused value (§5) |
| 11 | refused | none | `v = xs` is complete; `[0]` is an unused array literal (§5) |
| 12 | accepted | prints nothing (`a > 0` is true, `b` is false) | inside `(`, a line ending in `&&` goes on "at any column"; §1 "a condition needs no parentheses" allows them; `print(1)` at +4 is the Block |
| 13 | refused | none | the comment is not a token, so the line ends on `+`: "may not end where the statement cannot" |
| 14 | refused | none | the arm `Inline = ( Expression ... ) NEWLINE` cannot end on `+`; the `1` below is then a pattern with no `=>` |
| 15 | refused | none | the For cannot end after `in`: "may not end where the statement cannot" |

I also read the fifteen against the candidate with its last opening sentence removed, which is my reconstruction of today's text. I get the same fifteen outcomes, because "NEWLINE ends a statement" already refuses every one of the twelve. What changes is my confidence on 1, 2, 4, 13 and 15. Without the sentence, "so a long expression breaks after an operator" reads as a general, Go-style rule unless "there" (inside brackets) is noticed. I cannot reconstruct the §1 clause as it was before, so for §1 I judged only the candidate's wording.

**Task 2 under the candidate.** I added one-parameter `-> i64` or `-> bool` stubs.

```
    total = (weight_of_the_left_branch(i) + weight_of_the_right_branch(i) +
        weight_of_the_middle_branch(i) + weight_of_the_spare_branch(i))
    if (index_is_inside_the_table(i) && entry_is_not_a_tombstone(i) &&
            entry_holds_the_wanted_key(i))
        print(total)
```

- **How I broke them:** I opened a parenthesis and broke after an operator.
- **The words:** "a long expression, a condition included, breaks inside parentheses", "goes on below, at any column, so a long expression breaks after an operator", and §1 "a condition needs no parentheses" (they are allowed, just not required).
- **Continuation column:** I put the condition's continuation at +8 so it cannot be mistaken for the block. That is my choice; the spec allows any column.

**Task 2 without the added sentence.** My first draft of (i) was the Go-style break outside brackets, which is a compile error. For (ii) I hesitated, and my fallback was:

```
    inside = index_is_inside_the_table(i)
    live = entry_is_not_a_tombstone(i)
    wanted = entry_holds_the_wanted_key(i)
    if inside && live && wanted
```

This compiles, but all three calls always run. If the later ones index with `i`, the program aborts in cases where `&&` would have skipped them.

## hesitation_points

1. **Fragment 9.** The opening paragraph (a line ending in `,` goes on) and §10 ("separates elements by newline across lines and by comma on one") pull in different directions. I put about 75% on accepted. Either wrong guess is a compile error or a harmless restyle, never a different program, and the added sentence is not the cause.
2. **Fragment 12.** The block's column counts from the `if` line, not from the continuation line. I inferred that from "at any column". A wrong guess would be an indentation error, which is loud.
3. **Fragments 7, 10, 11.** I can tell they are refused but not which rule refuses them. In Python, 7 and 11 would compile and silently drop the value. Here §5 makes them loud.
4. **"a line ends its statement" is literally false for a block header** (`if c`, `k = match s`, `for x in xs`) and for the line before `else`. The sentence just before it ("ends with that block instead") and the phrase "a condition included" resolve this. If a model took it literally, it would write one-line `if` or `match` expressions: longer code, still correct.
5. **"at any column" against §1 "rigid: exactly 4 spaces per level".** A cautious model uses multiples of 4, which is harmless.
6. **Probed next to the proposal.** A leading `.` or leading `&&` inside parentheses is refused, because the NEWLINE may stand only before a closing bracket or a `,`, so it fails loudly. One edge exists in today's text already and is out of scope: `[a` / `-b` is silently two elements.

**Task 3, the judgement.**
- **Is every answer readable off the candidate?** Yes, except fragment 9 (see point 1).
- **Is the sentence local?** Yes. It is read off the break's last token, the next line's first token and the bracket depth within the same statement. That is the same class as the inside-brackets rule, so it passes the locality rule and I do not veto it.
- **Permissions:** None granted and taken back. "breaks inside parentheses" agrees with §1's "needs no parentheses". I found the reverse in only one place: the tension in point 4, which does no harm.
- **Rewording:** I would not reword it. I considered "Outside brackets no line goes on below: ...". It changes no answer, but it drops the explicit next-line direction, and that direction is what catches leading-operator habits (fragments 6 and 7).

## argument

By my reading of the candidate without its last opening sentence, no fragment's outcome moves. The sentence changes how a writer gets there. It names the one remedy, parentheses, right next to the rule that says where to break inside them. Its two "so" clauses combine into one recipe: open a parenthesis, break after an operator. Without it, the cheap fallback for a long condition is to hoist the clauses into bindings. That compiles and quietly gives up `&&` short-circuiting. Every wrong guess I found at a break outside brackets is a compile error. The rule is read off the break's last token, the next line's first token and the bracket depth, so it is local.

## condition

- **Object (or veto, if the meaning differs silently):** if the landed compiler accepts any of 1, 2, 4 to 8, 10, 11, 13, 14 or 15. For example, `(5)` becoming a call so that `g` is 10.
- **Object:** if it refuses 3, 12 or either Task 2 program, because then the compiler takes back the route the sentence grants.
- **Ask for the rewording:** if a harness run shows models avoiding multi-line `if` or `match`.
- **Fragment 9 refused:** this would not change my verdict. It would open a separate question about §10.

## prediction

The compiler that lands this sitting's resolution, run on the fifteen fragments wrapped in `function main()` with the stated names, will accept exactly 3, 9 and 12 and refuse the other twelve: 15 of 15 as I answered. If it differs, it differs on fragment 9 alone (14 of 15).
- 3 binds `y` = 6.
- 9 binds `[1, 2, 3]`.
- 12 prints nothing.
- No refused fragment is accepted with some other meaning (zero silent divergences).
- Both Task 2 programs, with one-parameter stubs, compile on the first try.
