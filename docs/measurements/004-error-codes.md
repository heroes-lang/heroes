# 004 — the error code, measured: one operator at zero

Date: 2026-08-12 · after M8a · **the first operator this project has written to
find a hole rather than to confirm a defence.**

## Provenance

| what | value |
|---|---|
| compiler | `347a704` plus this commit (the operator is new here) |
| spec | sha256 `b9155671b852279d…` — measured max **2434**, unchanged by this run |
| corpus | `examples/` — **17** programs (the gallery, `calculator.hero`, and `calculator/`'s four modules) |
| operators | `harness/mutations/operators.md`, now **eleven** — `typo-code` added |
| arms | `check` and `check --permissive` — unchanged |
| command | `heroes mutate` |

## The question

§4.6 fixes the error's payload at two `str`s and tells you which one to test
against: *"The code is snake_case and stable; assert on `e.code`."* So the
contract between the function that fails and the code that handles the failure is
**a pair of string literals, usually in different files**. Nothing checks that
they agree.

`heroes mutate` had ten operators and none of them touched a string literal, so
the thesis's own instrument had never asked about this. `typo-code` asks: drop
the middle character of the code in `fail("unknown_char", …)`, or of the code in
the `e.code == "unknown_char"` that reads it back, and see what the compiler says.

## Result

| operator | mutants | excluded | killed (check) | killed (--permissive) |
|---|---|---|---|---|
| swap-args | 114 | 0 | 83 (73%) | 62 (54%) |
| drop-case | 28 | 0 | 28 (100%) | 28 (100%) |
| forget-at-decl | 52 | 0 | 46 (88%) | 46 (88%) |
| mutate-undeclared | 57 | 6 | 51 (100%) | 51 (100%) |
| typo-ident | 570 | 3 | 567 (100%) | 567 (100%) |
| **typo-code** | **25** | **0** | **0 (0%)** | **0 (0%)** |
| wildcard-variant | 88 | 0 | 88 (100%) | 47 (53%) |
| positional-named | 84 | 1 | 69 (83%) | 0 (0%) |
| mix-int-float | 82 | 0 | 82 (100%) | 82 (100%) |
| shadow | 53 | 0 | 53 (100%) | 0 (0%) |
| drop-question | 20 | 0 | 20 (100%) | 20 (100%) |
| **total** | 1173 | 10 | 1087 (93%) | 903 (78%) |

**Twenty-five mutants, none excluded, none killed.** Every other row in the table
is between 73% and 100% in the strict arm. This one is the only zero, and it is a
zero in *both* arms — which means it is not a thesis rule doing the work
somewhere else, it is nothing at all.

## What moved in the corpus-wide pair, and why it is not a regression

| | mutants | counted | killed (check) | killed (--permissive) |
|---|---|---|---|---|
| ten operators (the figure the ROADMAP records) | 1148 | 1138 | 1087 (**96%**) | 903 (**79%**) |
| eleven operators (this run) | 1173 | 1163 | 1087 (**93%**) | 903 (**78%**) |

The killed counts are **identical**: 1087 and 903 in both rows. Nothing got
worse. The rate fell three points because twenty-five mistakes that the harness
could not previously see entered the denominator, and every one of them survives.

That is the honest direction for this number to move. A mutation rate rises when
the corpus stops asking hard questions, and this project has already corrected
one such number once — M8a found `heroes mutate` scoring a frontend with no
library attached and lowered 97%/81% to 96%/79% (`docs/ROADMAP.md`). The same
rule applies here: **an instrument whose every row can only report success is
confirming, not measuring.**

## What a mutant looks like

From `examples/calculator/lex.hero` and the test in the same module:

```
    return fail("unknow_char", "the lexer met a byte it does not know")
```

with the handler in `examples/calculator/main.hero` still reading

```
        .err e => print(c, " -> ", e.code, ": ", e.msg)
```

— which prints, so the mutation is invisible there. The sharper one is the
comparison, where a program that means to branch on a failure silently takes the
other branch for ever:

```
        .err e => assert e.code == "unknow_char"
```

Exit 0 from `heroes check`. Under `heroes test` this particular one fails, which
is worth stating plainly: **`test` blocks catch some of these and the compiler
catches none.** A test is written by the same author, at the same moment, from
the same wrong belief about the code's spelling — and the twenty-five sites here
include seven where the only reader of the code is a `print`, which cannot fail.

## What this does and does not license

It **does** establish that the class exists and is not hypothetical: 25 sites in
17 programs, in the acceptance program this project has carried since M0.

It does **not** by itself license a cure. Under Principle 0 the burden is a
measured Part 11 effect *or* a §1-derived argument, and a measured effect is a
number that moves when the rule lands — this file is the *before*. What it
licenses is the panel: the question of whether §4.6's code should be checked, and
at what price, is now a question with a number attached rather than an intuition.
That is panel 034.

**The cheapest candidate cure, for the record, so the panel prices it rather than
invents it**: a comparison of `e.code` against a `str` literal that no `fail` in
the program constructs is a compile error with a did-you-mean. Zero spec tokens —
the precedent is panel 031, where cross-module UFCS became a diagnostic rather
than a spec sentence. It is sound because compilation is whole-program, and it
must not fire at all when any `fail` in the program builds its code from a
non-literal, since the constructible set is then open.

If that lands, this table's `typo-code` row moves off zero and the movement is
the measurement. If it does not, this file is why the refusal was informed.

## Two things this run does not answer

- **The other end.** `typo-code` mutates one end of the contract at a time. A
  program where *both* the `fail` and the comparison are typo'd identically is
  correct, and the operator never builds one — correctly, since that is not a
  plausible slip.
- **Which sites are `print`-only.** The harness reports rates, not mutants, so
  the seven-versus-eighteen split above was counted by hand from the corpus. This
  is the second measurement in a row to want `heroes mutate` to print surviving
  mutants for one operator (002 § *What this suggests for the harness*). Still
  queued.
