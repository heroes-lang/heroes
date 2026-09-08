# 022 — The narrow rule costs more than the wide one

**The three prices M-interpolation-verdict's sitting is owed before it sits, not
during.** Two are `docs/work/SCHEDULED.md` items that name this milestone and say
so in those words; the third is the one `docs/ROADMAP.md`
§ M-interpolation-verdict demands of anybody who proposes the form, *"whoever
proposes it owes the other side of that trade in spec tokens and says what
`print` becomes afterwards"*. Measured 2026-09-08, every figure from
`./heroes measure` on a full copy of the spec with one draft applied. A copy of
the spec measures **3995**, the same as the spec, so the method adds nothing of
its own.

**The number to spend against is 40.** `tests/harness/suite_spec.hero:204` reads
`SPEC_TOKENS + FFI_FLOOR >= CEILING`, so with `FFI_FLOOR` 60 and `CEILING` 4096
the spec's own green ceiling is 4035 and the free budget is 4035 less 3995. Every
`free after` column below is `4096 - 60 - 1 - tokens`.

## The `.must()` silence, and its own hypothesis was right

`docs/work/SCHEDULED.md` filed this with two routes and one open question:
route (b) *"is dearer than (a) and may be cheaper than (a) written eight
times. Nobody has priced (b)."* Priced:

| draft | tokens | delta | free after |
|---|---|---|---|
| (a) the local clause, `.must()` alone | 4000 | **+5** | 35 |
| (a) written at all **eight** abort sites | 4024 | **+29** | 11 |
| (b) one definition of *abort*, tightly worded | 4007 | **+12** | 28 |
| (b) the same, with the message and the no-catch clause | 4017 | **+22** | 18 |

**Route (b) covers all eight sites for less than half of what route (a) costs at
those same eight**, +12 against +29, and it is the only one of the four that
makes `spec:162`'s *recursion too deep* and `spec:71`'s *overflow aborts at
every width* legible. The tight wording is *"To abort is to end the program at
once."*; the +22 one adds *"with a message; nothing catches one"*, and what the
extra 10 tokens buy is the fact that an abort is not an error and no `T?`
catches it, which is the confusion the ergonomist's appetite was reported from.

Both routes are placed at the **first** occurrence, `spec:71`, and not beside
`.must()`: a reader meets *aborts* in § Types, 85 lines before § Failure, and a
definition after its first use is a definition a reader has already had to guess
past. Placement costs nothing either way, which is why it is stated rather than
priced.

## The printed float's guarantee

`docs/work/SCHEDULED.md` asks for a sentence saying what a printed float
guarantees, *"it reads back as the same value"*, or a design.md Part 6 row with
its falsifier.

| draft | tokens | delta | free after |
|---|---|---|---|
| `, and reads back as the same value` at `spec:193` | 4004 | **+9** | 31 |

**The two scheduled items together are +21**, exactly the sum of +12 and +9 with
no interaction, leaving **19** free. So both silences the milestone inherited can
be closed inside the budget, and neither needs a removal or a prediction.

## Interpolation, and the finding is the opposite of what the entry assumed

§ M-interpolation-verdict says *"a bare name is the cheapest rule to write and
to lex"*, and question 1's whole framing follows from it. **Cheapest to lex,
yes. Dearest to specify.** Every draft below carries the same opening sentence,
the same example and the same escape sentence, so the only thing that moves is
the sentence that states the rule:

| what may stand in a hole | tokens | delta | free after |
|---|---|---|---|
| any expression | 4053 | **+58** | -18 |
| only a bare name | 4054 | **+59** | -19 |
| a name or a `.field` run | 4059 | **+64** | -24 |
| any postfix run: a name, a field, a call or an index | 4062 | **+67** | -27 |

**The enumeration is what costs.** *"Any expression may stand in a hole"* is
four words; naming the admitted forms is a list, and a list is tokens. So the
rule that `021` measured at 100% coverage is also the cheapest of the four to
write down, and the rule that covers 48.7% of chains costs one token more. The
cheap rule is dominated on both axes at once, which is a conclusion neither
measurement could have reached alone.

**The tightest honest clause is +46**, and it reaches that by stating no rule at
all: *"A string literal may hold `{e}`, which writes that value as `to_str`
does: `"line {n}: {words[i]}"`. `{{` and `}}` write one brace."* The rule is
left to the example. That is a silence of exactly the kind this project has
twice bought prose to close, most recently at panel 120, where a signature that
could not say which half `filter` keeps cost +30 of prose. It is recorded as the
floor of the range rather than recommended.

**So the honest range is +46 to +67 against 40 free**, and no draft fits.

## What the trade returns, and it is three tokens

design.md:1422 records `print`'s variadic-looking form as *"compiler-known, not
a function value"* and *"bought by trading away string interpolation (Part 7
item 7)"*, panel 006. The ROADMAP therefore asks what `print` becomes
afterwards. Giving it back:

| draft | tokens | delta | free after |
|---|---|---|---|
| `print` writes **one** value | 3992 | **-3** | 43 |

**Three tokens.** The credit side of a trade worth +46 at its cheapest is -3, so
the trade cannot be bought back out of its own proceeds, and the corpus cost of
that -3 is not in the spec at all: `print(` carries a comma at **174** sites in
`examples/` alone (`021`). The two payment branches panel 012 leaves, as amended
by panel 046, are a **named removal** or a **registered falsifiable prediction
naming an instrument that exists today and the milestone that scores it**. A
removal worth 46 is not lying around: the newest one priced, panel 120's -35, was
**refused rather than merely unspent** on 2026-09-08, and that ratification says
it is not available to the next sitting that goes shopping.

## What the brace spelling costs the corpus that exists

Question 3 of the entry asks for the escape rule the brace spelling owes.
Measured over every string literal in `selfhost/`, `examples/` and
`tests/harness/`, read from `heroes lex --dump-tokens` rather than from the
source:

| | literals |
|---|---|
| string literals in the three trees | **17,707** |
| holding a `{` or a `}` | **312** (1.76%), in **52** files |
| of those, a bare name between braces | **24** |
| an unpaired brace, so a doubling rule would rewrite it | **173** |

**All 24 are in one file, and it is the one program whose subject is
interpolation**: `examples/template/main.hero`, which implements `{key}`
substitution by hand and defends its escape choices in its own module doc. A
corpus program found a defect in exactly that escape rule at
`docs/journal/029-corpus-coverage.md`.

**And all 24 would fail loudly rather than silently.** The names inside them are
`name`, `role`, `nobody` and `anything`, and they are **map keys**, not
bindings: the file declares `render`, `render_or`, `needed`, `sample_values` and
`main`, and holds no binding, parameter or loop binder of any of the four names.
The language has no mutable globals and forbids shadowing, so there is no other
route by which one could be in scope. Every one of the 24 is therefore
`unknown_name` at compile time, which is the good half of a bad number: the
migration is 312 literals to review and 24 to repair, and none of it changes what
a program does while still compiling.

## What is still unmeasured

- **The lexer and the checker cost.** Nothing here prices the implementation:
  `021` and `022` are the spec and the corpus. A hole admitting any expression
  is a recursive call from the lexer into the parser, and that is the
  compiler-engineer seat's input rather than a number this file can produce.
- **Whether a reader is better off.** Principle 0 asks for a measured design.md
  Part 11 effect, and no reader has been put in front of either form. Both
  measurements are about what exists, not about what a model does with it.
- **The backslash spellings.** `\(name)` and `${name}` are unpriced here,
  because `spec:75` fixes six escapes and `docs/panel/008-escape-sequences.md`
  R3 says a new escape reconvenes that panel. The spelling question therefore
  has a procedural answer before it has a token one.
