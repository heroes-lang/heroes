# 021 — What would stand inside a hole

**The count M-interpolation-verdict is owed before it sits.** `docs/ROADMAP.md`
§ M-interpolation-verdict lists three questions and says the first one bites
hardest: *"What may stand inside a hole: a name, an expression, a call? A bare
name is the cheapest rule to write and to lex, and it is the one that reads
worst on the day somebody wants `{count + 1}`."* That sentence had no number
under it. This is the number, measured 2026-09-08 over `selfhost/` (**190**
modules, **55,414** lines) and `examples/` (**118** files, **18,622** lines).

It also re-measures the four counts that section took on 2026-09-02, because
they were taken against a smaller tree: **51,788** lines in **178** modules
then, and the milestone had no step yet. And it adds `tests/harness/`, which
those counts never read.

## The instrument, and the disagreement that validated it

**`heroes parse <file> --dump-ast` is the instrument**, because the parser
re-prints every binary expression fully parenthesized and left-nested:
`"a" + b + c` comes back as `(("a" + b) + c)`. So precedence, line breaks and
the difference between a `+` in a chain and a `+` inside a call's arguments are
settled by the compiler rather than by a regex.

**It reads the whole corpus, which the closures instrument could not.** `021`'s
predecessor, `020-four-sites-in-fifty-five-thousand-lines.md`, had to pair
`--dump-ir` with a text census because `--dump-ir` builds the module it names
and a nested module does not build standalone. `parse` resolves no `use`, so it
answers for every file: run 2026-09-08, **308 of 308** parse, zero failures.

A **chain** here is a maximal `+` expression holding at least one string
literal and at least one operand that is not one. Each non-literal operand is
one **hole**: what an interpolation form would have to admit in that position.
An operand written `x.to_str()` is counted as `x`, because the form absorbs the
rendering (`docs/ROADMAP.md` § M-interpolation-verdict question 2: every type
already has a canonical `to_str` and a second rendering rule is the expensive
answer).

**The second instrument knows nothing about parentheses.** `heroes lex <file>
--dump-tokens` gives a flat stream, and a `str` token with a `plus` token
immediately beside it is one hit. Counting the same quantity, string literals
that are an operand of a `+`, the two disagreed:

| | literals beside a `+` |
|---|---|
| the token stream | **2985** |
| the AST | **2982** |

**Three files, one each, and the same shape in all three** — the AST is right
and the tokens are coarse. A string literal can be the RECEIVER of a UFCS call,
and then the operand of the `+` is the call and not the literal:

```
selfhost/diag_render.hero:110    padding.join("") + "^".repeat(width.to_u64().must())
selfhost/emit/literal.hero:292   hex_float("1" + "0".repeat(400) + ".0")
selfhost/print/fmt.hero:89       f.out + " ".repeat(indent.to_u64().must()) + text
```

2982 plus those 3 is 2985, exactly. Two of the three sit in chains with no
literal operand at all, so the census excludes them by design: there is nothing
there to interpolate into. The shape is worth naming for the sitting on its own,
because `" ".repeat(n)` is padding built from a literal, and no spelling of a
hole makes it shorter.

## What is there

| | selfhost/ | examples/ | both |
|---|---|---|---|
| chains | 1247 | 214 | **1461** |
| holes | 2217 | 319 | **2536** |
| files holding one | 126 | 68 | **194** of 308 |

Of the 2536 holes, **273** (10.8%) are written `.to_str()` today. The other
89.2% are already `str`, so what the hand assembly mostly spends is syntax and
not conversion.

**A third body of Heroes was measured because leaving it out would have been a
choice nobody could see.** `tests/harness/` is **28** files and **12,878**
lines, and § M-interpolation-verdict's counts never looked at it:

| | chains | holes | per 100 lines |
|---|---|---|---|
| `selfhost/` | 1247 | 2217 | 2.25 |
| `examples/` | 214 | 319 | 1.15 |
| `tests/harness/` | **639** | **915** | **4.96** |

**The densest hand assembly in the repository is the net's own failure
messages**, at four times the examples' rate, and its ladder is the same shape:
a bare name admits 59.1% of its holes and 54.5% of its chains, a postfix run
98.6% and 98.0%. Three corpora, three sizes, one answer, which is the closest
thing to a replication this measurement can offer.

**Holes per chain**, and the tail is the argument that a chain is hard to read:

| holes | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 12 |
|---|---|---|---|---|---|---|---|---|---|---|
| chains | 792 | 438 | 144 | 46 | 15 | 15 | 6 | 3 | 1 | **1** |

The four re-measured counts from § M-interpolation-verdict, 2026-09-02 against
2026-09-08:

| | then | now |
|---|---|---|
| `selfhost/` lines holding `" + ` | 945 | **1017** |
| `selfhost/` lines holding a `to_str()` call | 146 | **159** |
| `print(` calls in `examples/` carrying a comma | 118 | **174** |
| `examples/template/main.hero` | 235 lines | **235** lines |

## The ladder, which is the answer to question 1

Every hole classified by its OUTERMOST shape, and the candidate rules read as
rungs: what fraction of holes each one admits, and what fraction of CHAINS it
lets you rewrite whole. The chain is the unit that matters, because a chain with
one inadmissible hole stays a `+` chain.

| the rule admits | holes | of all | chains rewritable | of all |
|---|---|---|---|---|
| a bare name | 1444 | 56.9% | 712 | 48.7% |
| a name and `.field` steps | 1746 | 68.8% | 869 | 59.5% |
| any postfix run (a call, an index) | 2529 | 99.7% | 1454 | **99.5%** |
| any expression at all | 2536 | 100.0% | 1461 | 100.0% |

**The cheapest rule leaves half the corpus where it is, and the postfix rule
costs seven holes.** Those seven are the whole distance between rung three and
rung four, so they are listed rather than summarised:

```
examples/todo/list.hero          (i + 1).to_str()
selfhost/check/holes.hero        (shown.len() - SUGGESTIONS).to_str()
selfhost/cli/measure.hero        (CONTRACT_CEILING - highest).to_str()
selfhost/cli/measure.hero        (highest + FFI_FLOOR).to_str()
selfhost/ffi_errors.hero         (length - 1).to_str()
selfhost/main.hero               (at + 1).to_str()
examples/gallery/09-holes.hero   ???
```

Six of the seven are arithmetic rendered as text, and five of those six are an
off-by-one or a subtraction. **The seventh is a question about the spec rather
than about a corpus**: `spec/heroes-spec.md` § Tests and holes says *"`???` is a
valid expression anywhere"*, and `examples/gallery/09-holes.hero:28` takes it at
its word inside a concatenation. Any rule below rung four makes *anywhere* false
in a place the spec does not except, which is a §12 shape: the spec would have
the bug, and it costs tokens to say so.

## Where the pressure is, and it is not where the record says

§ M-interpolation-verdict reads the 945 lines as *"this compiler's own
diagnostics being assembled by hand, and it is the largest single body of
evidence in the repository."* **Measured, the diagnostics are the small half.**

| | holes | share | of them, a bare name |
|---|---|---|---|
| the nine modules that carry diagnostics | 286 | 11.3% | **252 (88.1%)** |
| `selfhost/emit`, `selfhost/print`, `selfhost/ir` | 1241 | 48.9% | 704 (56.7%) |

By directory, the ten heaviest: `selfhost/emit` **411**, `selfhost/cli` **252**,
`selfhost/` itself **179**, `selfhost/print` **134**, `selfhost/ir` **111**,
`selfhost/check` **55**, `selfhost/resolve` **39**, `examples/interpreter`
**34**, `selfhost/module` **28**, `selfhost/parse` **25**. The heaviest single
files are `selfhost/ir/print.hero` (**69** chains), `selfhost/print/fmt.hero`
(**58**) and `selfhost/print/bodies.hero` (**37**).

**So the largest body of hand assembly in this repository is the code generator
and the two program printers**, not the error messages. A proxy count agrees and
is named as a proxy: only **397** chains of 1461 (27.2%) quote program text in
backticks, which is how a diagnostic in this project cites a name.

Two consequences the sitting inherits rather than derives. **The place the cheap
rule would serve best is the small half**: 88.1% of the diagnostics' holes are
bare names, against 56.7% in the emitter and the printers. And the thesis is the
only warrant available for this item (`docs/ROADMAP.md`, Principle 0's second
branch), so an argument about a reader's comprehension now has to be made about
`selfhost/emit/`, which is the one body of code in the tree whose whole job is
to print another language.

## What the absence costs in characters

The 1461 chains of `selfhost/` and `examples/`, as written against the single
literal each would become, holes included at one brace each:

| | characters |
|---|---|
| as written today | 101,383 |
| as one interpolated literal each | 83,581 |
| saved | **17,802 (17.6%)** |

That number is deliberately in characters and not in tokens. `heroes measure`
prices the SPEC, and what a form costs there is the spec-warden's input at the
sitting; this is the corpus, where the unit a reader meets is the line.

## What this does not measure

- **The spec price of any spelling.** Nothing is drafted or measured here. That
  is the next step of this milestone, against the **40** free tokens
  `./heroes measure spec/heroes-spec.md` reported this session (3995, with the
  green ceiling at 4035 because `tests/harness/suite_spec.hero` reads
  `SPEC_TOKENS + FFI_FLOOR >= CEILING`).
- **The escape rule.** § M-interpolation-verdict question 3 stands untouched:
  `spec/heroes-spec.md` says six escapes and no others, and
  `docs/panel/008-escape-sequences.md` R3 says a new escape reconvenes that
  panel.
- **Whether any of this is worth buying.** A count says what is there. Principle
  0 asks for a measured design.md Part 11 effect, and no reader has been put in
  front of either form.
- **The ruling that does not exist.** § M-interpolation-verdict's *"zero hits"*
  over `docs/panel/`, `docs/journal/` and `docs/measurements/` was re-run
  2026-09-08 and is **2**: `docs/panel/118-the-missing-word-was-a-dead-variable.md:451`
  and `docs/journal/039-closures-verdict.md:171`, both of them scheduling
  references to this milestone. Neither is a ruling, so the substantive claim
  holds and there is still nothing to read forward from.
