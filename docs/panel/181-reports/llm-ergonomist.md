*Written out by the coordinator from the seat's final message on 2026-09-28 at
01:50: the harness refused the seat's own Write to both paths ("Subagents
should return findings as text, not write report files"). The text below is
the seat's, unchanged, from its heading down; the summary the seat put above
it is kept first.*

**Summary, the seat's.** My predictions per variant assume each bound name is
read afterwards; as written, every fragment is refused anyway for unused
bindings.
- **X and Y** give the same answer on all ten fragments. They refuse 1, 2, 4,
  5, 6, 7, 8 and 10, and accept 3 (y = 6) and 9 (ys = [1, 2, 3]).
- **Z** differs only on 2 and 8, which it accepts (y = 6, and y = 2 * f(5)).
- Z's refusals of 1, 4 and 5 are my reading: Z never says what a line at the
  same column is.

**My judgement.** I approve Y, subject to two wording fixes. I object to X and
to Z, without a veto.
- None of the three produces a program that compiles and means something
  different from how it reads. What prevents it is section 5's rule that a line
  computing a value must use it, plus the absence of a unary `+`, not the
  layout rule.
- X answers fragments 1, 2 and 10 only through the word "there". Its closing
  clause, "so a long expression breaks after an operator", reads like a licence
  to break anywhere.
- Z leaves three cases unstated: chains (flat or staircase), the body's column
  after a continued condition, and same-column lines. Both writing tasks need
  chains.
- The one program in the experiment that compiles and does something different
  comes up under X and Y. Section 1 says "no parentheses around conditions". A
  reader who takes that as a ban moves the parts of a long condition into
  separate names, and loses `&&` short-circuiting.
- The two Y fixes: say what "unless a block opens below it" means, and say
  whether a long condition may be broken inside parentheses.

**Registered prediction.** Today's compiler refuses 1, 2, 4, 5, 6, 7, 8 and 10
and accepts 3 (y = 6) and 9 (ys = [1, 2, 3]). That scores my X and Y columns
10 of 10 and my Z column 8 of 10. For the next harness run: Y at most halves
X's first-attempt refusals from line breaks, and silent errors stay at 0 under
all three.

---

# Panel 181: llm-ergonomist report

**Inputs.** I used the brief `docs/panel/181-briefs/llm-ergonomist.md` and `spec/heroes-spec.md`, both read in full, and nothing else. The session attached `CLAUDE.md` and three `.claude/rules/` files to my context without my asking. Nothing below uses them.

**Columns, counted in the brief's fence.** The fence content starts at column 3, and each fragment's first token sits at column 7.
- The second line of fragments 1, 3, 4, 5, 6, 7, 9 and 10 sits at the first line's column.
- The second line of 2 and 8, and the third line of 4, sit 4 spaces (one level) deeper.

**Assumption behind every answer.** Each fragment is followed by a line that reads what it binds, and `b` and `xs` are read somewhere. Taken literally, every fragment is refused under all three variants by section 5's "An unused binding or parameter is a compile error", before layout comes into it. I take `f(n: a)` to be legal, because the grammar makes an argument's name optional (`Arg = [ ident ":" ] ...`).

## verdict

- **Variant Y: approve**, with two wording amendments (see condition).
- **Variant X: object.** Its answers are right, but outside brackets they rest on the single word "there". Its closing clause reads as a general licence.
- **Variant Z: object, no veto.** The rule is local. As written it settles only a single break, and both writing tasks need several breaks in a row.

## experiment

### Task 1: thirty predictions

The spec wording I relied on, keyed:

- **N1**: "NEWLINE ends a statement, and a statement whose last part is a block ends with that block instead."
- **N2**: "Inside `(` `[` `{` a NEWLINE never ends a statement. A line there keeps its NEWLINE when it ends with a literal, ... ; that NEWLINE may stand only before a closing bracket or a `,`, or where a production writes it, and any other line goes on below, at any column, so a long expression breaks after an operator." Two words limit it to brackets: "Inside" and "there".
- **N3**: "Where a NEWLINE separates without a `,`, the next line may not begin with a `-` that does not touch its operand."
- **G5**: `Statement = ident "=" Expression NEWLINE ...` (section 5).
- **G7**: section 7's `And`, `Sum`, `Product`, `Unary` and `Postfix` productions. Every binary operator needs an operand after it, and `.` needs an `ident` after it. No Expression begins with `+`, because `Unary` has only `-`, `!` and `~`.
- **U**: "A line that computes a value must use it: bind it, or discard it on purpose with `_ = f(x)`" (section 5).
- **S10**: "A container literal separates elements by newline across lines and by comma on one" (section 10).
- **OP**: the section 7 operator table. `=` and `@` are section 5 binding forms, not operators.
- **Y** and **Z**: the sentence each variant adds.

**Fragment 1** (`y = a +` over `1`, same column)
- **X: refused.** By N1, the NEWLINE after `+` stands outside brackets. N2's joining applies only "Inside" brackets. By G7, `Sum` is left without its `Product`.
- **Y: refused.** Y: "a line that ends with an operator ... is an error unless a block opens below it", and no block opens here.
- **Z: refused, by my reading.** Z joins a line only onto one "indented one level deeper", and `1` is not deeper, so X's rule applies. Z never says a same-column line is an error. If Z's clause is only descriptive, the fragment is accepted with y = 6.

**Fragment 2** (`y = a +` over a line one level deeper, `1`)
- **X: refused.** Same as 1. The deeper line would also be an INDENT that no statement production writes (G5, and section 1's "Indentation is significant and rigid").
- **Y: refused**, with one doubt. Y's words are "unless a block opens below it", and I read "block" as the grammar's `Block`. No production puts one after `+`.
  - On a layout reading (the next line is deeper), Y exempts this line and says nothing about what it then means.
  - Section 8's "a block's value is its last expression" would then suggest y = 6.
- **Z: accepted, y = 6.**

**Fragment 3** (`y = (a +` over `1)`, same column)
- **All three accept it, y = 6.** By N2, a line inside `(` ending in `+` is not in the keep list, so it "goes on below, at any column". Y and Z both open with "Outside brackets", so they do not apply.

**Fragment 4** (`if a > 0 &&` over `b`, same column, then `print(1)` one level deeper)
- **X: refused.** By G7, `And` is left without its `Compare`.
- **Y: refused.** The line ends with an operator, and the line directly below it opens no block. My doubt: the `if`'s block does open two lines down. I read "below it" as the next line.
- **Z: refused, by my reading**, as in fragment 1.
  - On the descriptive reading it is accepted: the condition is false and nothing prints.
  - With `b` indented one level, Z accepts it and nothing prints.

**Fragment 5** (`n = xs.` over `len()`, same column)
- **X: refused.** By G7, `.` must be followed by an `ident`.
- **Y: refused.** Y lists `.`.
- **Z: refused, by my reading**, as in fragment 1. On the descriptive reading, n = 2.

**Fragment 6** (`y = a` over `+ 1`, same column)
- **All three refuse it.** Line 1 is a whole statement (N1, G5). By G7, nothing can begin with `+`. Y and Z do not apply, because line 1 ends with a name.

**Fragment 7** (`total = a * 3` over `- 2`, same column)
- **All three refuse it.** Line 1 is a whole statement. Line 2 is `-2` standing alone, which U refuses.
- N3 also refuses it if a statement's NEWLINE counts as "separates without a `,`". N3 does not limit itself to brackets, so I read that it does.

**Fragment 8** (`y = f(n: a) *` over a line one level deeper, `2`)
- **X: refused.** The `)` closes the only bracket, so `*` ends a line outside brackets.
- **Y: refused**, with the same doubt as in fragment 2.
- **Z: accepted, y = 2 * f(5).** The value depends on the body of `f`, which the brief does not give.

**Fragment 9** (`ys = [1, 2,` over `3]`, same column)
- **All three accept it, ys = [1, 2, 3].** By N2, a line inside `[` ending in `,` goes on. Y's `,` applies only outside brackets.
- Doubt: S10 could be read as refusing a comma at the end of a line. I read S10 as a description of the two separators, not as a refusal.

**Fragment 10** (`z =` over `a`, same column)
- **X: refused.** By G5, the NEWLINE (N1) arrives where the Expression should be. A second error follows on `a` alone, by U.
- **Y: refused.** Y lists `=`.
- **Z: refused.** By OP, `=` is not a binary operator, so X's rule applies, and `a` is not deeper either.
  - A reader who counts `=` as "the assignment operator" still refuses this fragment because of the column.
  - The same reader would predict that `z =` over a deeper `a` is accepted, which Z does not say.

| # | X | Y | Z |
|---|---|---|---|
| 1 | refused | refused | refused (reading) |
| 2 | refused | refused | accepted, y = 6 |
| 3 | accepted, y = 6 | accepted, y = 6 | accepted, y = 6 |
| 4 | refused | refused | refused (reading) |
| 5 | refused | refused | refused (reading) |
| 6 | refused | refused | refused |
| 7 | refused | refused | refused |
| 8 | refused | refused | accepted, y = 2 * f(5) |
| 9 | accepted, [1, 2, 3] | accepted, [1, 2, 3] | accepted, [1, 2, 3] |
| 10 | refused | refused | refused |

### Task 2: writing under each variant

**(i) Under X and Y**
```
    total_cost = (compute_base_price_for_order(order) +
        compute_shipping_surcharge_for_order(order) +
        compute_regional_tax_amount_for_order(order) +
        compute_loyalty_bonus_credit_for_order(order))
```
- I put parentheses around the whole right side and broke after each `+`.
- N2 is the only rule that joins lines, and it works only inside brackets.
- A leading `+` (the PEP 8 and Black style) is refused in all three variants, and the refusal is loud. The reason is that a line ending in `)` keeps its NEWLINE, and that NEWLINE "may stand only before a closing bracket or a `,`".
- Y states the rule outright: "a long expression is broken inside parentheses".

**(i) Under Z**
```
    total_cost = compute_base_price_for_order(order) +
        compute_shipping_surcharge_for_order(order) +
        compute_regional_tax_amount_for_order(order) +
        compute_loyalty_bonus_credit_for_order(order)
```
- The guess is in lines 3 and 4. Z measures the column from "the line it continues", so read literally the lines form a staircase at +8 and +12.
- I wrote the flat form by convention. The spec does not choose between flat and staircase.
- The X program is still legal under Z.

**(ii) Under X and Y**
```
    if (index_is_within_bounds(index: i, count: n) &&
            element_matches_the_target(items: xs, index: i) &&
            no_earlier_duplicate_before(items: xs, index: i))
        print(i)
```
- Section 1 says "No braces, no semicolons, no parentheses around conditions". I cannot tell whether that refuses `if (...)` or only says none are needed.
- If it is a ban, a model has two ways out:
  1. Parentheses around part of the condition. This is legal and keeps short-circuiting:
     ```
         if index_is_within_bounds(index: i, count: n) && (
                 element_matches_the_target(items: xs, index: i) &&
                 no_earlier_duplicate_before(items: xs, index: i))
             print(i)
     ```
  2. Moving each clause into a name first. This **compiles and is a different program**:
     ```
         in_bounds = index_is_within_bounds(index: i, count: n)
         matches = element_matches_the_target(items: xs, index: i)
         fresh = no_earlier_duplicate_before(items: xs, index: i)
         if in_bounds && matches && fresh
             print(i)
     ```
     `matches` now runs even when `in_bounds` is false, so an index inside it aborts where the original would not.
- Y sharpens the clash, because it recommends parentheses where section 1 seems to forbid them.

**(ii) Under Z**
```
    if index_is_within_bounds(index: i, count: n) &&
        element_matches_the_target(items: xs, index: i) &&
        no_earlier_duplicate_before(items: xs, index: i)
        print(i)
```
- Flat or staircase is unstated, as in (i).
- "Opens no block" puts the body at the same column as the condition's last line.
- The two-level continuation I used under X is refused by Z's "one level", and a PEP 8 habit writes exactly that.

## hesitation_points

**Under X**
- **How far N2 reaches.** Whether N2 is scoped by the word "there" or read globally flips six fragments (1, 2, 4, 5, 8 and 10). A wrong guess by a writer is a **compile error**, never silent.
- **`if (...)` against section 1.** Guessing wrong one way gives a **compile error**. Guessing wrong the other way leads to the hoisting route, which is **silently different**.
- **S10 on fragment 9.** A wrong guess is a **compile error**.

**Under Y**
- **"Unless a block opens below it".** No production puts a `Block` after any token Y lists. Only an arm's `=>` ends a line with a `Block` below it, and Y does not name `=>`, nor does section 7 call it an operator.
  - On the layout reading, the clause exempts fragments 2 and 8 and the Z-shaped `if`.
  - The result is still a **compile error**, but the clause points to the wrong program.
- **Parentheses against section 1.** The same as under X, and sharper.
- **"Every line ends its statement" against block statements.** The spec's first sentence reconciles the two, so the risk is low.

**Under Z**
- **Same-column lines** (fragments 1, 4 and 5).
  - If the compiler refuses them, that is a **compile error**.
  - If it accepts them, fragment 4's `b` reads as a line of its own and runs as part of the condition.
- **The column in a chain.** Flat or staircase is a coin flip, and both tasks need a chain. The wrong side is a **compile error**.
- **The body's column after a continued condition**, and the refused two-level habit. A **compile error**.
- **What Z covers.** `=`, `,`, `@`, `->` and `:` are not covered. A **compile error**.
- **Two column rules to hold.** Brackets allow "any column" while outside brackets it is exactly one level. A **compile error**.

**Silently different programs found:** one, the hoisting route under X and Y. None under Z.

### The four judge questions

1. **Least to hold: Y.** It gives one way to join lines and one prohibition. Z adds a second way to join lines, with its own column rule and three cases left unstated.
2. **Two statements that run as one, or the reverse: none that compiles, in any variant.**
   - What prevents it is section 5's U plus the absence of a unary `+`, not the layout rule.
   - Fragment 7 is refused by U and N3. N3 already closes `[a` over `- 1]`.
   - Z's continued `if` puts the condition's last line at the body's column. The program behind that misreading is itself refused, so it costs reading time, not correctness.
3. **Non-locality: none rises to a veto.** Whether a line continues is decided by the last token of the line above it and by the bracket depth.
   - On the flat reading, Z checks a chain's column from the statement's first line. That reaches past the two lines around the break, but it checks validity, not meaning.
   - On the staircase reading the check is fully local.
4. **Does X already answer fragments 1, 2 and 10? Yes: all three are refused.**
   - The words are N1, N2's scope ("Inside `(` `[` `{`" and "A line there"), G5 for fragment 10, and `Sum = Product { ( "+" | "-" ) Product }` for fragments 1 and 2.
   - X answers them only through the word "there" and by saying nothing outside brackets. No sentence states it.
   - "So a long expression breaks after an operator" reads to a skimming reader as a general licence.

## argument

All three variants are silent-safe at the break: every mis-split leaves an operator-final line or a bare value line, and section 5 refuses both. The choice is therefore first-try rate, which is what the reader must infer. X is correct but reaches fragments 1, 2 and 10 only through the word "there", and its closing clause reads as a licence. Y states X's rule where the reader looks and adds no mechanism. Z adds a second continuation rule with its own column rule, and leaves same-column lines, chains and the body column after a continued condition unstated; both writing tasks need chains. Y's defects are wording: its block exemption, and parentheses against "no parentheses around conditions".

## prediction

**Registered, and scorable today with the compiler.** Put each fragment inside `function main()` with the four names in scope and the bound name printed afterwards.
- I predict the compiler **refuses 1, 2, 4, 5, 6, 7, 8 and 10**, and **accepts 3 (y = 6) and 9 (ys = [1, 2, 3])**.
- That scores X and Y 10 of 10, and Z 8 of 10 (it misses 2 and 8). Any other result falsifies the prediction.
- If the compiler accepts only 2 and 8, it behaves as Z describes, and a careful reader of X mispredicts it.
- If it accepts any of 1, 4, 5 or 10, it joins lines outside brackets at any column. Only the loose reading of X describes that.

**Standing prediction for the next harness run over the variant prompts.**
- First attempts refused for a line break outside brackets: Y at most half of X's rate.
- Z at least Y's rate.
- Silent errors from line breaks: 0 under all three.

## condition

- **A parenthesised condition.** Run `if (a > 0 &&`, then `        b)`, then `    print(1)` on today's compiler. If it is refused, I object to Y unless Y names a way to break a condition, and I prefer Z with a stated chain rule.
- **The harness.** If a run shows Z's first-try rate on long-expression tasks at least 5 points above Y's, with silent errors still at 0, I approve Z.
- **A reworded Z.** My objection to Z drops to a preference for Y if Z is reworded to state three things:
  - a same-column line is an error;
  - every continuation line sits one level below the statement's first line;
  - the block after a continued header sits one level below the header.
- **A silent difference.** If any variant compiles a fragment or a task program above to a different meaning from the one written here, I veto that variant.
- **The two amendments that my approval of Y depends on:**
  - (a) Replace "unless a block opens below it" with "unless it is an arm's `=>` whose body is a block below it", or delete it.
  - (b) Say whether a condition may be put in parentheses to break it. If the compiler accepts `if (...)`, the wording could be "so a long expression, a condition included, is broken inside parentheses". If it does not, name the route.
  - Lower priority: "every line ends its statement or opens its block".
