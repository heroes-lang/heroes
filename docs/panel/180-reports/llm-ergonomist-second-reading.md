# Panel 180, llm-ergonomist, second reading

(Written to disk by the coordinator from the seat's final message; its brief
is `/Users/joseph/Temp/heroes-recovery-2026-09-26/panel-180/llm-ergonomist-2/brief.md`,
copied beside the sitting's briefs as `180-briefs/llm-ergonomist-second-reading.md`.
The seat noted that the harness put `CLAUDE.md`, `.claude/rules/spec-shape.md`
and `.claude/rules/verification.md` into its context unasked and that it did
not use them.)

## verdict

**approve.** I recommend the rewording in part 3 of the experiment, but it is not a condition.

## experiment

### Part 1: the twelve fragments

Two terms below. A line **carries** a NEWLINE when its end becomes a NEWLINE token. A line **goes on** when its end becomes no token and the expression continues on the next line.

| # | Proposed sentence | Words relied on | Current sentence (lines 11-12) |
|---|---|---|---|
| 1 | accepted, a call with 2 arguments | the line ends in literal `1`, so it carries a NEWLINE, and a NEWLINE "stands only before ... a `,`" | accepted ("may fall between any two tokens") |
| 2 | accepted, **2 elements** | same words as 1. Also "separates without a `,`", which implies that a NEWLINE with a `,` does not separate | two readings: as `Sep` it puts `,` where an Expression must be, which is an error; between tokens it is accepted with 2 |
| 3 | accepted | literal `0` carries; the NEWLINE stands "before a closing bracket" | accepted |
| 4 | **refused** | word `a` carries; `Sep` means "where a production writes it"; `- b` is "a `-` set apart from its operand" | **two readings that both type-check**: 1 element (2) or 2 elements (5, -3) |
| 5 | accepted, **2 elements** (5, -3) | as 4, except `-b` touches its operand | two readings that both type-check, 1 or 2 elements |
| 6 | accepted, **1 element** (2) | a line ending in `-` is "a line ending otherwise", so it "goes on below" | accepted, 1 element (the only parse) |
| 7 | **refused** | word `a` carries; the next token is `+`; `"(" Expression ")"` writes no NEWLINE | accepted, `a + b` |
| 8 | accepted, **2 elements** (1, -1) | the line ending in `[` goes on; `1` carries and acts as `Sep`; `-1` touches its operand; `-1` carries before `]` | **two readings that both type-check**: `[0]` or `[1, -1]` |
| 9 | accepted, **2 elements** (5, -3) | the line ending in `,` goes on, so no NEWLINE exists; `- b` is a unary minus; the last sentence does not apply | accepted, 2 elements |
| 10 | **refused** | literal `10` carries; the map literal writes `Sep`; `- 2` is set apart from its operand | two readings: `{1: 10, -2: 20}`, or a parse error |
| 11 | line break **refused** | `counted_by` is "a word other than `function` or `fail`", so it carries; the next token is `n`; `CParam` writes no NEWLINE | accepted |
| 12 | line break accepted | a line ending in `->` goes on | accepted |

### Part 2: four long expressions

Under the proposed sentence, first try:
```
    yearly = [
        rent_per_month * months_occupied + parking_per_month * months_occupied
        gross_salary * marginal_rate_percent / 100 -
            pension_contribution * marginal_rate_percent / 100
        electricity_units * price_per_unit + standing_charge_per_day * days_billed
        insurance_premium * policy_count + broker_fee_per_policy * policy_count
    ]
```
The derivation:
- The line ending in `-` goes on, and the deeper indent of the next line is cosmetic ("at any column").
- `100` carries a NEWLINE, which is a `Sep`, so the next line starts a new element.
- The last line carries a NEWLINE that stands before `]`.

The comma form is also allowed, because a line ending in `,` goes on:
```
    yearly = [rent_per_month * months_occupied + parking_per_month * months_occupied,
        gross_salary * marginal_rate_percent / 100 -
            pension_contribution * marginal_rate_percent / 100,
        electricity_units * price_per_unit + standing_charge_per_day * days_billed,
        insurance_premium * policy_count + broker_fee_per_policy * policy_count]
```

Under the current sentence, Rust and black habit made me write this first:
```
        gross_salary * marginal_rate_percent / 100
            - pension_contribution * marginal_rate_percent / 100
```
That NEWLINE can be a `Sep`, giving five elements, or it can fall between tokens, giving four. Both type-check as `[i64]`. The only safe spelling was to wrap the element in `( )`.

Under the proposed sentence:
- The spaced leading `-` is refused.
- The wrapped `( )` version is refused too, because `100` carries a NEWLINE that cannot stand before `-`.
- Both refusals are loud.
- One spelling still compiles to a different program without any error: `-pension_contribution` with the `-` touching its name, which gives five elements.

### Part 3: the judgement, as a sentence

**What a reader must hold.** Three facts, all visible where the line breaks:
1. Which line endings carry a NEWLINE.
2. The three places a carried NEWLINE may stand.
3. The rule for `-`.

**Locality.** Nothing is non-local. The inputs are the last token of the line, the first token of the next line, and the innermost open bracket's production. All three sit inside the same statement. No veto.

**Rewrite.** Same twelve answers, checked row by row:

> Inside `(` `[` `{` a NEWLINE never ends a statement. A line there keeps its NEWLINE when it ends with a literal, `?`, `???`, a closing bracket, or a name or keyword other than `function` and `fail`; that NEWLINE may stand only before a closing bracket or a `,`, or where a production writes it, and any other line goes on below, at any column, so a long expression breaks after an operator. Where a NEWLINE separates without a `,`, the next line may not begin with a `-` that does not touch its operand.

The changes:
- **The "word" item moves to the end of the list.** In the original, the phrase "other than `function` or `fail`, a literal, ..." can be parsed so that the exception list swallows everything after it.
- **"carries one" becomes "keeps its NEWLINE"**, so the antecedent is explicit.
- **"stands only" becomes "may stand only"**, so the refusal is explicit.
- **"word" becomes "name or keyword"**, which makes fragment 11 derivable.
- **"set apart" becomes "does not touch".** Unlike "followed by a space", it still covers `-` at the end of a line.
- **"so a long expression breaks after an operator"** is new. It states the idiom a model needs and changes no answer.

"without a `,`" also changes none of the twelve answers, since fragment 9 has no NEWLINE at all. I would keep it anyway, because it is the phrase that tells a reader fragment 9 is safe.

## hesitation_points

- **H1, the nested "or" in the list of endings that carry.** The wrong parse says that literals and `)` do not carry. I recovered because "a word other than `?`" makes no sense. When writing code, a wrong guess is loud or harmless. When reading, it mispredicts fragment 8 as `[0]`.
- **H2, "carries one".** The antecedent could be NEWLINE or "statement". I resolved it to NEWLINE; no program changes.
- **H3, "stands only".** Elsewhere in this spec, "stand" means "may appear" (section 13: "stand only as an argument of a call"; section 2: "Any expression may stand there"), so I read it as a refusal. Reading it as "counts only there" brings back the old sentence: fragments 7 and 11 become accepted, and a model writes `(a\n + b)`. The compiler then refuses it, which is loud but costs a first-try compile.
- **H4, "word" and keywords (fragment 11).** Because the exceptions `function` and `fail` are keywords, "word" must include keywords. A reader who takes "word" to mean identifier predicts 11 is accepted, and the compiler refuses it (loud). The same edge applies to `owned`, `acquires`, `retains` and `transfers`: each always needs a following word, yet none is exempt. This case is loud and rare.
- **H5, a NEWLINE before `,` inside a list (fragment 2).** It could be read as the `Sep`, which would be an error, or absorbed by the before-`,` rule, which gives 2 elements. I chose absorbed. Either wrong guess is loud.
- **H6, the column after a carried NEWLINE (fragment 3).** "at any column" is attached only to lines that go on. I assumed that no line inside brackets produces INDENT or DEDENT. A wrong guess only adds harmless indentation.
- **H7, the one remaining silent path (fragments 5, 8, part 2).** A `-` touching its operand after a separating NEWLINE starts a new element. That is right for a negative element and wrong for a continued subtraction. What keeps this small is that models almost always put a space after a binary `-`.
- **H8, section 10's "by newline across lines and by comma on one".** It reads as a prescription, and I briefly doubted that fragment 9 is legal. A wrong guess is harmless.
- **H9, chains that start the next line with `.`.** `print(words\n .filter(f))` is now refused; the current sentence accepts it. Inside a list, `.filter(f)` becomes a variant-case element, which the types refuse. Both are loud, and both cost first-try compiles for code written with Rust or JavaScript habits.

## argument

The current sentence gives fragments 4, 5, 8 and 10 two readings each, and in 4, 5 and 8 both readings type-check. That makes the commonest vertical list, one with a negative element, a coin-flip that fails silently. The proposal is Go's line-end rule, which models already know, with a gentler edge: a NEWLINE before `)` `]` `}` `,` is fine. Every answer follows from the two lines and the innermost bracket, so it is local. Each refusal it adds is loud: a spaced leading `-`, a leading `+`, a leading `.` inside a call, and the break after `counted_by`. One silent residue remains, a touching `-b` meant as subtraction. Four phrases invite misreading; the part 3 rewrite keeps all twelve answers.

## prediction

- **P1, which you can score today against the prototype.**
  - **Twelve of twelve:** the prototype agrees with every row of part 1, both accept or refuse and the element counts for 2, 5, 6, 8 and 9 (2, 2, 1, 2, 2).
  - **Where a miss can fall:** any disagreement will be on fragment 2 or 11 only.
  - **What a miss elsewhere means:** a disagreement on 4, 5, 7, 8 or 10 means the sentence does not describe the compiler.
- **P2, for the next harness run.**
  - **Scope:** tasks that need a multi-line container literal, or a long expression inside brackets.
  - **Silent errors:** under the proposal, zero programs are silently wrong because of a line break.
  - **First-try rate:** it moves by at most 5 points against the current sentence.
  - **Loudness:** every failure caused by a line break is a compile error at the continuation line.

## condition

- **The prototype and the sentence disagree:** on any fragment other than 2, I object until one of them moves.
- **A fresh reader answers 2, 7 or 11 differently:** that reader has only the spec and the proposed sentence. If this happens, the H3 and H4 wording is misread in practice, and I object until the rewrite lands.
- **The silent path shows up in the harness:** if a touching `-x` meant as subtraction adds elements in at least 1% of multi-line containers, I object. I would then ask for the stricter rule: refuse any `-` at the start of an element separated by a NEWLINE, and write a negative element as `(-1)`.
- **The first-try cost is too high:** if the rate drops more than 5 points and the refusal diagnostic does not offer a certain Fix that moves the operator up a line, I object.

## Scored by the coordinator, 2026-09-27, before the synthesis

- P1: the twelve fragments, each with every binding used, run with `heroes
  parse` and `heroes run` on the critic's rebuild of the compiler-engineer's
  (b)+(ii) prototype
  (`/Users/joseph/Temp/heroes-recovery-2026-09-26/panel-180/critic/bii/heroes-bii`):
  1 accepted, 2 accepted with 2 elements, 3 accepted, 4 refused
  (`spaced_minus_element`), 5 accepted with 2, 6 accepted with 1, 7 refused
  (`expected_group_close`), 8 accepted with 2, 9 accepted with 2, 10 refused
  (`spaced_minus_element`), 11 refused (`expected_extent`), 12 accepted:
  **12 of 12, held**.
- P2: scored at the next harness run that generates such programs.
- The part 3 rewrite, priced by the coordinator with the string sentence of
  R4: vendored maximum 6794, `real` 8999 (+138 against today's 8861).
