---
kind: defect
area: compiler
milestone: none
filed: 2026-09-28
commit: aee8b01ed0a4f427fcaf31b0aadf55ba4962fa4d
github: none
---

# Defect 119 closed: a suffix never reaches across a block that just closed

2026-09-28, M-agreed-retention step 28, in lane 181 (`5772c830`, the trunk
merged into it at `e3921986`, the spec and design.md at `4c453f5a`), merged
`08581ace`, on panel 181's provisional resolution
(`docs/panel/181-outside-brackets-a-line-ends-its-statement-in-both-directions.md`).

- [x] **119 — a line at a statement's margin that begins with `(`, `[` or `?` after an `if` or `match` block is applied to the block's value** | `f = if c` / `    double` / `else` / `    triple` / `(5)` at the statement's margin is `check` 0 and prints 10, the `(5)` read as a call of the `if`'s value; with `[0]` after a block of arrays it prints 7; `heroes fmt` exits 2 on both, *this is a compiler bug*; the reader sees a block and then a line of its own | `selfhost/grammar_expr.hero` (`ends_the_expression`, the postfix loop after a block-valued expression) · **closed 2026-09-28**

    **Origin:** panel 181's completeness critic, 2026-09-28, asking whether
    defect 116 is the whole class or one entry of it; reproduced by the
    coordinator on the trunk's compiler at `0fc98107` the same night
    (`q1_postfix_call.hero` and `q2_postfix_index.hero` in
    `/Users/joseph/Temp/heroes-recovery-2026-09-26/panel-181/coordinator-key/`).

    **Why it is a defect.** Defect 005's neighbour: its repair taught
    `ends_the_expression` to stop a binary operator at a line end after a
    block, and a postfix was left reading on. A program that reads as two
    statements runs as one, which is design.md §4.15's one accepted silent
    case widened by the compiler rather than by the language.

## The repair

`selfhost/grammar_expr.hero`'s suffix loop stops at every suffix after a
closed block, not only `.`, which is panel 035's own rule made true. A line
that begins with `(` or `[` after a block is a statement of its own: `(5)` and
`[0]` are refused as values nothing receives, and `(5).print()` after a block
runs as the second statement it reads as. The narrowing from the resolution's
list, which named `(` and `[` among the tokens that can only go on with the
line above, is the lane's, measured (`comments101/parenplace.hero:10` is a
legal statement that begins with `(`), approved at 03:59 and recorded beneath
the sitting. The `check/` case
`fixedbugs-a-suffix-after-a-block-applied-to-its-value`, and a `surface` row
running the `(5).print()` shape.

## The gate

In the lane, before each of its three commits, one suite at a time with the
compiler built from the regenerated seed, the fixpoint by `cmp`; the last run
on the merged tree: the compiler's 815 tests and the net's own 174; check 161,
annotations 210, fixes 30, surface 332, canonical 2, grammar 9, spec 20,
special 10, layout 4, order 3, records 24, run 210, emission 636, determinism
240, corpus 55, warnings 271, lines 211, ir 24, emit 8 and probe 24, each 0
failed. Over the 1164 `.hero` files of `0fc98107`, exactly one `check` exit
moved, `surface-fixtures/comments107/margin.hero`, 0 to 1 (635 zero and 529 one
before, 634 and 530 after), the spec-warden's prediction held. The cost, on a
still machine at 06:58 (load 1.80, no run waiting): lexing the 272 `selfhost/`
files concatenated, 74,955 lines, median 1.44 against 1.49 s user, +3.5%;
`check selfhost/main.hero` 4.17 against 4.19, +0.5%; a first form that read the
next line ahead cost +5.6% and was replaced by one that decides at the next
line's first token, identical on 1898 files by `lex --dump-tokens` and `check
--json`.

Linux arm64 on the lane's tree (`4c453f5a`, the `heroes-linux-arm64` image):
the compiler's 815 tests and surface 332, canonical 2, annotations 210, check
161, fixes 30, layout 4, order 3, lines 207, run 206, emission 624, grammar 9,
spec 20 and probe 24, each 0 failed. The Windows box on the same tree, one
archive whose sha256 matched on both sides (`eaa9f74f8d3c5b31`), seed
`3c6915bf0be5bf2a`: the compiler's 815 tests and surface 325, canonical 2,
annotations 210, check 161, fixes 30, layout 4, order 3, lines 205, run 204,
emission 602, grammar 9, spec 20 and probe 24, each 0 failed; the last four
were run again at 09:44 after the Mac's sleep cut the connection during
`emission`, from the same extracted tree with `build/` emptied.
