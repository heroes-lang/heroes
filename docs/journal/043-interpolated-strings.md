# 043 — M-interpolated-strings

Closed 2026-09-09, tagged `m-interpolated-strings`. One step, no sitting of its
own, and the form panel 121 ruled in that morning landed the same evening.

## Goal

Implement the ruling of `M-interpolation-verdict`: `f"line {n}: {word}"`, the
brace active only behind an `f`, a hole admitting any expression with the scan
stated, one AST node carrying the whole literal's span and desugared in
lowering, `{{` writing one brace. With it, the two silences panel 121 R6
accepted: one definition of *abort* covering the eight places the spec said the
word and defined it nowhere, and the printed float's round-trip guarantee.

The milestone opened with four constraints measured at the verdict's close: a
new node touches 85 sites in 27 files that enumerate `.str_lit`;
`grammar_expr.hero` is at 1002 of a decided 1002 and a helper module that calls
`parse_expr` closes a `use` cycle (R5); `check/walk.hero` 1700 of 1700,
`ir/flatten.hero` 1102 of 1110, `print/fmt.hero` 1136 of 1150.

## What surprised

**The skipping scan is free, because the lexer already counts brackets.** R1
asks that a hole end at the `}` that closes it, nested brackets and nested
literals skipped, and the sitting priced that as a scan to write. It is not:
the pieces of the literal are tokens, the hole's own tokens stand between them
lexed by the ordinary scanner, and `state.emit` has counted `( [ {` since
panel 007. A hole records the bracket depth when it opens, and a `}` closes it
only when the depth is back to that number. A nested literal is a `str_lit`
because `"` starts one. The whole lexer half is 110 lines, most of them the
module doc.

**One token kind, and the text says which piece it is.** `f"a {n} b"` is three
tokens, `f"a {`, `n`, `} b"`. The head starts with `f"`, every later piece
starts with the `}` that closed a hole, and a piece ending in `"` closes the
literal. So the parser's hook is a loop: parse an expression, expect a piece,
stop when the piece closes. Three new kinds would have cost three arms in every
match over token kinds; one costs one.

**`{{1: 2}` at a hole's edge is text, for Python's reason.** A map literal as
the first thing inside a hole needs a space after the opener, because `{{` is
one brace of text. The coordinator's lexer test asserted the opposite and the
lexer was right; the rule is Python's and so is the workaround.

**The compiler was right both times the coordinator's probe programs were
wrong.** `ok = true` is a reserved name. `f"{m["k"]}"` is a hole holding an
`i64?`, because a map index answers `V?` (§4.9), and the new diagnostic sent it
to `.must()` in its own note — the third line of
`tests/golden/check/hole-not-renderable.hero` is that case, kept because it is
the one a reader meets first.

**The blast radius the sitting predicted was paid, and the compiler enumerated
it.** 68 exhaustive matches in 22 files gained an arm; seven are real (the
resolver walks the holes, the checker types them, the lowering lowers them, the
tree's depth counts them, the dump re-prints them, the token has a description
and a line-ender rule) and the rest join a literal group. `--dump-ast` and
`heroes fmt` print the literal verbatim, and `fmt` is a fixpoint on it.
`heroes mutate` finds the sites inside the holes and kills all four.

**Four decided ceilings moved, by measured lines, and the first was a
registered prediction.** `grammar_expr.hero` 1002 to **1041** — the hook with
its comment, and the test section's fully-parenthesised re-print, which calls
`render` and so cannot leave the file either; the compiler seat at panel 121
predicted *strictly greater than 1002*. `check/walk.hero` 1700 to **1708**,
`ir/flatten.hero` 1110 to **1114**, `ast.hero` 475 to **484**. Each because a
helper that calls back into a knot is a `use` cycle, and each row in
`suite_layout.hero` names its lines. The three helpers that needed no call back
— `lex_interp.hero`, `check/interp.hero`, `ir/interp.hero` — are modules of
their own.

## What broke and why

**The seed compiler's `fmt` refused the new goldens, correctly.** A loop that
formats every changed `.hero` file ran the OLD `./heroes` over programs
containing `f"…"`, and it reported `expected )` and `expected the end of the
line` — the seed does not know the form yet. `fmt --in-place` refuses a file it
cannot parse, so nothing was rewritten, and the lesson is the one every syntax
change teaches: until the seed is regenerated, the compiler under test is
`heroes-next`, for the formatter as much as for the suites.

**The layout suite's self-test pins a witness ceiling by number.** Moving
`check/walk.hero`'s decided number to 1708 turned
`ceiling_for("selfhost/check/walk.hero") == 1700` red inside
`suite_layout.hero`'s own tests — the check is a pair with the table, and the
pair moved together.

**The spec's clause cost 37 tokens more than the sitting priced**, +114 against
+77, and every one of them is R1's scan stated in the document: *the hole ends
at the `}` that closes it, nested brackets and literals skipped*. The
historian's registered prediction said the brace-run and the nested-`}` cases
would land in `DEFECTS.md` unless the spec stated the scan; the spec states it,
and two goldens pin the cases instead.

## Predictions scored

| prediction | verdict |
|---|---|
| compiler-engineer, panel 121: `DECIDED` holds a number strictly greater than 1002 for `grammar_expr.hero` when the form ships | **CONFIRMED**: 1041 |
| spec-warden, panel 121: no second sentence about holes is needed | **FALSIFIED in letter, and the reason is the historian's**: the clause carries a second sentence stating the skipping scan, at +37, because a stated scan is what keeps the brace-run case out of `DEFECTS.md` |
| historian, panel 121: `"{{{n}}}"` and a hole containing a `}` inside a nested literal land in `DEFECTS.md` within the shipping milestone unless the spec states the brace-run resolution and the skipping scan | **CONFIRMED by its exception**: the spec states both, and `tests/golden/run/interpolation-holes-and-braces.hero` and `tests/golden/check/unterminated-hole.hero` pin the cases; `DEFECTS.md` holds nothing |
| llm-ergonomist, panel 121: fresh models and lone braces | **LAPSES to M-thesis-harness** |

## What landed, and what carried forward

`f"…"` as one token kind, one AST node, three helper modules, four raised
ceilings with their reasons, and a diagnostic that refuses a hole `to_str`
cannot render in `to_str`'s own words. The spec at **+138** real tokens (5231
to 5369 on `claude-opus-5`, 715 free net of the FFI floor): the clause with the
scan, *an abort ends the program, saying why*, and *reads back as the same
value*. Five goldens — the form as a reader writes it, its desugar in the IR,
the unrenderable hole, the hole a line does not close — and a gallery example,
`examples/gallery/12-interpolation.hero`, which is what the site will embed.
`heroes mutate` reaches the holes. design.md Part 7 item 7 says ENTERS.

**Carried forward**: the site, all of it at the end by author instruction, with
this example on it; the Linux and Windows legs before the push; a `--dump-ast`
that shows a hole's structure rather than the literal verbatim, which the
test-section printer already does and the production printer could; and panel
125's ratification.

**Appended 2026-09-09, the two other platforms run before the push.**
`interpolation-holes-and-braces` and `examples/gallery/12-interpolation.hero`
are clean under `--sanitize` in the Linux container and print their expected
output on the Windows box; the compiler's 618 tests and the harness's 125 pass on
both. Nothing carried forward from the platform list.

**Corrected 2026-09-09, the same evening.** *The harness's 125 pass on both*
was true of the Windows box and unrun on Linux: the Linux run whose output this
appendix rests on printed the compiler's 618 and the goldens, and no harness
count. Run later that evening, the harness's own tests on the Linux image read
**126 tests, 1 failed**, the failing case being the citation test that asks
`git check-ignore`, and the image had no `git` at all, so that case had been red
there since 2026-09-07 with nobody reading the count. `git` is in the
`Dockerfile` now, and over a copy that carries `.git` the Linux leg reads **126
passed** and `records` **15 passed**, which it could never run before. The 126th
test joined this evening with `records/c`'s claim clause.
