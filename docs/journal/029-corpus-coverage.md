# 029 — M-corpus-coverage: the first program that reached the library broke the compiler

## Goal

Twenty more programs for `examples/`, by author instruction of 2026-09-02:
*"aggiungere altri esempi (almeno altri 20) che facciano uso di tutte le
feature del linguaggio compresa quest'ultima"*. Ordered by a **measured**
coverage gap rather than by invention, which is the scheduling item's own rule
and the thing that made the milestone worth more than its count.

Delivered: **20 programs, 21 modules, 3762 lines, 156 `test` blocks.**
`examples/` goes 15 programs to 35, 55 modules to 78, 6426 lines to 10,286.

## What surprised

**The first program that reached the library found a defect in the compiler,
on its first run.** The scheduling item had written the reason for this
milestone in one line — *"a corpus that does not reach the library cannot find
a defect in the library"* — and `examples/readings/`, the program written to
close that exact gap, produced `heroes check` exit 0 and `heroes run` exit 2 on
its first invocation. Twelve lines reproduce it:

```
function main()
    xs: [R] = [R(n: 1), R(n: 2)]
    print(xs.map(to_n).fold(0, add))
```

The class was narrowed by three variants that all worked: `xs.map(to_n).len()`
(one generic, nothing to overwrite), the same pair through a binding, and the
same chain without UFCS. So: **one generic call chained directly into another,
through UFCS** — which is the spelling `spec:113` uses to teach the feature.

**The cause was a premise that never held, asserted in two files.**
`check/state.hero` keyed each generic call's type arguments by the call's span
START, justified in its own comment as *"a span is a byte range and two calls
cannot share one"*, and `ir/containers.hero` repeated it as *"at check time a
span occurs once"*. Both false at the parser: `grammar_expr.hero:322` gives a
method-call node its span start from its **receiver**, so a UFCS chain's inner
and outer calls begin on the same byte. `fold`'s bindings overwrote `map`'s.

Two documents asserting one premise are not two witnesses. The second copy is
what made the first look checked.

**And it was invisible for as long as it existed, for a reason worth keeping.**
When both calls instantiate at the same types the overwrite changes nothing —
`[i64]` mapped by an `i64 -> i64` and folded prints the right answer either
way. Measured: that program works on the unfixed compiler. 536 of the
compiler's own tests never caught it because none of them wrote the chain at
two different type parameters.

**The measurement in the scheduling item was wrong twice, and re-measuring is
what found the real gaps.** It said 39 of 46 forms exercised and 7 not. Over 56
forms probed on the day: **12 unexercised**, and two the item called "thin at
one use" were at **zero** — it had counted occurrences inside comments. The
sharpest number of the whole milestone came out of that re-measurement: the six
library functions the spec's own § Built-ins calls *"written in Heroes"* were
called by **one** program in the repository, `all` by **nothing anywhere**, and
the UFCS form by **nothing at all** — zero uses of `xs.map(f)` in every `.hero`
file, while `spec:113` illustrates the library with exactly that spelling.

**THE LANGUAGE CORRECTED THE AUTHOR OF THESE PROGRAMS SEVEN TIMES**, and every
correction is now written in the file that met it. None of them would have come
out of a checklist of features; all of them came out of writing programs a
person would actually write.

1. **`record Dealt<T>` does not exist.** `spec:116` puts generics on functions
   only, so a generic operation with two answers is two functions or an `@`
   parameter. `error[empty_record]` refused it at the `<T>`, the parser reading
   `<` where a field list belongs.
2. **Two parameters of one type make the labels mandatory** at the call site
   (`spec:104`). Five of the twenty learned this from `error[needs_label]`.
3. **`heroes fmt` deleted the C parentheses** around `mode & MASK == 0`,
   because `spec:166` binds `&` **tighter** than a comparison. C is the other
   way round, where `x & MASK == 0` parses as `x & (MASK == 0)` — the oldest
   bug in that language. The canonical form says so by refusing to keep the
   armour, and `examples/palette/` keeps the lines as the formatter produced
   them with the reason above.
4. **`if c a else b` on one line does not exist.** `error[missing_body]: an if
   needs an indented body`. So `spec:167`'s *"there is no ternary; `if` is an
   expression"* says something stronger than it looks: there is no inline
   conditional either, and a program written against the block form grows small
   named helpers rather than long lines.
5. **A multi-line `match` cannot sit inside `ok(...)`** — its arms want one
   level below itself and inside a call there is no such level.
6. **`template`'s escape rule was asymmetric** and a **test** found it, not the
   compiler and not a re-reading: `{{` escaped and `}}` did not, so `{{name}}`
   — what a writer types for a literal `{name}` — came back as
   `error[stray_brace]`. Python's and Rust's format strings, which that file's
   own comment cites as its model, double both.
7. **An accumulator bound with `=` cannot be mutated.** A cell needs `@` and
   its type, which `error[not_mutable]` says with the repair pre-written.

**A stale claim in `SCHEDULED.md` died to a measurement before it cost
anything.** The item said all 301 blessed emissions in `tests/emission/` would
change because the C component is the sanitised whole path. **Zero** of them
hold a symbol from a `selfhost/` module — they are emissions of `tests/golden/`
programs. That removed a chunk of predicted work in the first ten minutes.

## What broke and why

**Nine key sites, one word each, and the fix rests on the other half of the
asymmetry that caused it.** The parser anchors a call's span START at its
receiver and its END at its own closing parenthesis, so an outer call always
ends strictly after the inner one it contains. Both tables are keyed by the
END now. CLAUDE.md §11 asks two things in exchange for an unavoidable premise
and both are paid: the claim is rewritten as a falsifiable one in the field's
own comment, and the test that fires when it dies is
`tests/golden/run/fixedbugs-a-ufcs-chain-gave-two-calls-one-key.hero`, verified
to **pass** on the fixed compiler and **fail** on the unfixed one. Its blessed
emission records **three** distinct `map` instances where the unfixed compiler
emitted one, byte for byte.

**The adjacent shapes were swept and the sweep produced the general rule**
(CLAUDE.md §1). The compiler's other two span-keyed tables — `locals_by_span`
and `ir/build.hero`'s `by_span` — key on a `name.start` and are sound. The
start of a **token** is a fact: one identifier occurs at one place. The start
of an **expression** is not, because a composite expression inherits it from a
child. That line is now in the code.

**Twenty-one call sites needed a label added after the fact**, across five
programs, all of them `error[needs_label]` on two parameters sharing a type.
That is the language's rule working, and it is also the single most repeated
correction of the milestone — worth noting because it says where a writer's
instinct and this language differ most often.

**Two instrument numbers moved, and one was stale before the milestone touched
it.** `suite_corpus.hero`'s floor went 15 → 35 in six steps, re-measured each
time; its module doc had said *"fourteen programs"* for two milestones after
the number stopped being fourteen, and the count now lives only in `LEAST`,
which is measured rather than written twice.

## Predictions, scored

| origin | prediction | scored |
|---|---|---|
| ledger row 3541 | `args_checked`'s clause was paid by a prediction scored FALSE at M-separate-compilation close, on the ground that no file used the built-in and *"`suite_run` has no way to pass argv to a golden"* | **The first half is closed and the second was half wrong.** `examples/argv/` uses it, and the corpus suite has had a way all along: `main.args`, one argument per line, read by `program_arguments`. The debt was closable with the existing mechanism and nobody had looked |
| ledger row 3685 | at M-corpus-coverage close the reservation from panel 102 has refused **at most two** locals across the whole corpus beyond `examples/json/parse.hero`'s `value` | **HOLDS at zero.** Twenty new programs, 21 modules, and not one needed a local renamed for it |

## What landed, and what carried forward

**The count the author asked for is met**: twenty programs, and the first ten
were chosen by the coverage measurement while the rest were chosen for the
shape of what they can get wrong. That split is the method, and it is why the
corpus now has a program whose answers **somebody else already wrote down** —
`examples/sieve/`, checked against 25 primes below 100, 168 below 1000 and
1229 below 10,000, with a second algorithm sharing no line with the first.
Almost everything else in `examples/` asserts what its own functions produce,
which catches a change and not a mistake.

**The cost was measured before it was paid and again after, and the
projection held within six seconds.** At the start: **1:20.28 for 16
programs**, which is 5.1 s each and projected 2:57 for thirty-five. Measured
at the close: **2:51.19 for 35 programs, 0 failed**. That is the author's
standing rule honoured in the only way it can be — the number in hand before
the work, and the same number checked after — and it makes the corpus leg the
slowest part of the net, which the next milestone to touch it should know. The
net as a whole runs to **1309 checks, 0 failed**.

**What is still owed and is filed rather than done**: `heroes mutate`'s
fourteenth operator, the one that steals a module's name, held for a milestone
that can run it against a corpus this size rather than the 15-program one —
because adding an operator moves the denominator of the thesis's own score and
that discontinuity should be spent once. It is in `docs/work/SCHEDULED.md` with
the reason.
