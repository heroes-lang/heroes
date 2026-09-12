# 042 — M-interpolation-verdict

Closed 2026-09-09, tagged `m-interpolation-verdict`. Four steps, one sitting,
three measurements, and a ruling that was named so it could be a refusal and
turned out to be an acceptance whose shape changed twice on the way.

## Goal

Rule on design.md Part 7 item 7, string interpolation, which had said
*deferred; `print` takes multiple arguments* since the first draft. The
milestone's deliverable is a **verdict** and not a feature, which is why its id
may not claim the implementation (panel 121 R10) and why the implementation takes
an id of its own, `M-interpolated-strings`.

The milestone owed a count before it sat, on the same rule that bound
`M-closures-verdict`: a sitting that rules on a form nobody counted rules on a
feeling.

## What surprised

**The count that was supposed to settle the hole rule was wrong in three ways,
and the second count did not trust the first.** `docs/measurements/021` swept
1461 concatenation chains and 2536 holes across 194 of 308 files, and found that
a bare name would fill 56.9% of the holes and 48.7% of the chains, a postfix run
99.7%. The coordinator's first instrument counted parentheses inside string
literals, missed left-nested `+` chains and did not split call arguments; each
was found by attacking the repair at the shape next to it and re-running.

**The narrow rule costs MORE spec tokens than the wide one.** `022` priced four
wordings against each other with the example held constant: *any expression* at
+58, *a bare name* at +59, *a name or a field run* at +64, *a postfix run* at
+67. Enumerating what is admitted costs more than admitting everything, and the
narrow rules refuse seven holes of 2536, one of them a `???` the spec promises is
valid anywhere. So the rule that wins on tokens, on coverage and on the spec's
own promise is the same rule, and the count that recommended it is the one that
filed the `???` question against itself.

**The brace was already taken.** 312 string literals in 52 files hold a `{`, 211
of them in 39 of the compiler's own modules, and 24 are hole-shaped and all in
one program, `examples/template/main.hero`. An ungated brace would have stopped
the compiler compiling itself and made a two-stage bootstrap of a syntax change.
That is what put the gate on the literal and made the letter before the quote the
whole question of spelling.

**The spelling went to the author and the author's ground was the ergonomist's
axis.** Three spellings were live at once and each was cheapest on a different
scale: the coordinator recommended the author's own `\{e}` at -25 real tokens,
the compiler seat offered `\(e)`, and the sitting adopted `f"…"`. The author
chose `f` because it is the standard one, which is the shape a reader has met
most, and that is the only warrant the thesis rests on. Neither rival added an
escape, so panel 008's rule on escape sequences was not triggered and no second
sitting was owed.

**And every ceiling the sitting computed was arithmetic on the wrong scale.**
R7 fixed a ceiling of 4253 against a spec measured at 3995 with 40 free. `023`
found the binding instrument was `cl100k_base`, which is OpenAI's, and that the
spec was **5094** real tokens against a stated 4096 — **998 over** a line it was
told it was 40 under. The 4224 the author had confirmed, the 4237 the coordinator
recommended and R7's 4253 are void as arithmetic. The ceiling is **6144** by
author decision, with its instrument named, and the clause's delta re-measured on
the real instrument was +151 against the `\{e}` spelling's +126: the ranking
survived and every level did not.

## What broke and why

**The coordinator read a grep line where a file had to be opened, and a seat
caught it each time.** The census instrument's three defects above; a ceiling
argued to the token on a number that was 27.5% low; and, at the sitting after
this one, a floor presented to the author as a fact that was a vendor's price
list about a different kind of request.

**Two defects were filed out of the sitting and both were repaired in the next
two milestones.** The ffi seat's 16-line program — a computed `str` lent to C and
returned — built at exit 0 and was a use-after-free, and became defect 022 and
`M-cstr-lifetime`. The spec-warden's steelman found that the fix for
`mixed_arithmetic` on a `str` sent a reader back to the same error, defect 023,
closed the same day.

**The parser has no legal home for the form, and that was known before the
form was adopted.** `selfhost/grammar_expr.hero` is at 1002 of a decided 1002. R5
measured that a helper module which calls `parse_expr` closes a `use` cycle, and
that the escape hatch is shut because a function type cannot declare an `@`
parameter. So the hook that reads the holes stays in the knot and the DECIDED
number rises by its lines with the reason written — which `.claude/rules/module-shape.md`
already licenses for a recursive-descent grammar, a knot the language forbids
splitting.

## Predictions scored

| prediction | verdict |
|---|---|
| ffi-pragmatist, panel 121: `heroes build --sanitize` reports heap-use-after-free on the 16-line `cstr` case and the plain build prints an empty line at exit 0, on every seed up to this milestone's close, **unless the escaping `cstr` is refused** | **CONFIRMED, by its own exception**: the case is defect 022, refused since `M-cstr-lifetime` as `cstr_escapes`, so at this close it does not build at all |
| compiler-engineer, panel 121: `DECIDED` holds a number strictly greater than 1002 for `grammar_expr.hero` when the form ships | **CARRIED to `M-interpolated-strings`**, the milestone that ships the form, which this one was named not to |
| spec-warden, panel 121: at close `heroes measure spec/heroes-spec.md` reads <= 4047 and no second sentence about holes is needed | **FALSIFIED on its first half, and the reason is recorded rather than adjusted**: the vendored figure is **4098** today, because the lend clause and the lease clause landed first, at +25 and +78 vendored; the prediction assumed the interpolation clause would be the next spec amendment and two milestones intervened. Its second half is carried to the shipping milestone |
| historian, panel 121: `"{{{n}}}"` and a hole containing a `}` inside a nested literal land in `DEFECTS.md` within the shipping milestone unless the spec states the brace-run resolution and the skipping scan | **CARRIED to `M-interpolated-strings`**, and it binds the clause's wording: R1 states the skipping scan, and the brace-run case must be pinned by a golden |
| llm-ergonomist, panel 121: >=6 of 10 fresh models emit undoubled lone braces under the ungated proposal | **LAPSES to M-thesis-harness**, metric 2, which has never run |

## What landed, and what carried forward

A ruling: string interpolation **enters**, as `f"line {n}: {word}"` — the brace
active only behind an `f`, a hole admitting any expression with the scan
stated, one AST node carrying the whole literal's span and desugared in lowering,
`{{` writing one brace. Three measurements, `021`, `022` and `023`, the third of
which moved the language's central number and is why the spec now carries the
name of its tokeniser. Two defects filed and both closed since. The author's
ratification of panel 121 on the day the verdict was given, including the
override of the warden's Principle 0 veto, because no reader has been put in
front of either form and the metric that would settle it has never run.

**Carried forward to `M-interpolated-strings`**: the two spec sentences R6
accepted, one definition of *abort* at +12 and the printed float's round-trip at
+9, which land with the clause; the clause itself, re-measured on today's spec
before it is written; three predictions; and the constraints measured at this
close — 85 sites in 27 files enumerate `.str_lit`, and four decided ceilings
stand at or near their numbers: `grammar_expr.hero` 1002 of 1002,
`check/walk.hero` 1700 of 1700, `ir/flatten.hero` 1102 of 1110,
`print/fmt.hero` 1136 of 1150.
