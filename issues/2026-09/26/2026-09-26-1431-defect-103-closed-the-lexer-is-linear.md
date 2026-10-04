---
kind: defect
area: compiler
milestone: none
filed: 2026-09-26
commit: fa813325a4ba437fd93a50107ce8299568ddb92b
github: none
---

# Defect 103 closed: the lexer is linear in the size of a file, and the compiler checks itself a quarter faster

2026-09-26, M-agreed-retention step 19, in lane `e64b93ca`, merged `eaf7e88f`.
Found by lane g's agent timing its own round; measured and profiled by the
coordinator before filing.

- [x] **103 — the lexer is quadratic in the size of a file: it copies the token array at every line end and the rest of the file at every escape** | `layout.hero` appends the line-end, indent and dedent tokens as `l.tokens @ l.tokens.push(…)`, a field place the in-place store never fires on (design.md Part 8 wart 8), so every line copies the whole token array and resets its capacity, and the next in-place push reallocates it; and `literals.escape_step` reads one character as `text.slice(from: …, to: text.len()).chars()[0]`, a copy of the rest of the file per escape, as the char-literal loop and the unexpected-character path do per character | `selfhost/layout.hero:112,119,135` · `selfhost/literals.hero:30,159` · `selfhost/scan.hero:98,215` · `selfhost/state.hero:97,105` · `selfhost/lexer.hero:123,125` · **closed 2026-09-26**

    **Origin:** lane g's agent, 2026-09-26, timing its own round (*parsing
    walk.hero four times over takes 6.3 s against 0.16 s for one copy*);
    measured by the coordinator the same day on the trunk's compiler at
    `bdf430f1`, and profiled with `sample`, which put the time in
    `hero_array_push` under `layout.maybe_terminator` and `layout.line_start`
    and in the Token copies inside `hero_array_push_owned` under `state.emit`.

    **The measurements**, `heroes lex` on a file of 500 functions and its 2, 4
    and 8 copies, `user` seconds: with a string holding escapes in each, 1.32,
    5.13, 20.67, 91.05; with a plain string, 0.03, 0.12, 0.48, 2.58; with no
    string at all, 0.13, 0.49, 1.89 at 2, 4 and 8. Four times the time for
    twice the file, every row; 210 KB of source takes a minute and a half to
    lex.

    **Why it is a defect and not the wart.** Design.md Part 8 wart 8 stays a
    wart on panel 144's ruling that the cost of a push through a field is in
    the spelling, and names the cheap one: lend the field to an `@`
    parameter. `state.push_token` is exactly that, written for this lexer on
    2026-08-26 with a comment saying so, and the lexer calls it from one path
    of the nine that append a token (eight as filed, corrected at the close by counting the calls). The repair is the spelling the ruling
    names at every append, a character read by its own width, and a test
    that fails if the slow spelling comes back.

## The repair

- **Every append to the lexer's growing arrays is the spelling panel 144's
  ruling names.** `state.push_token(@l.tokens, t)` lends the array to an `@`
  parameter, so inside it the place is bare and the in-place store fires;
  it existed since 2026-08-26 and served one path of the nine that append a
  token. The line end, indent and dedent (`layout.hero`), comments
  (`scan.hero`), error tokens (`state.hero`) and the end of file
  (`lexer.hero`) use it now, and `state.push_diagnostic` does the same for the
  diagnostics, twelve appends in six modules.
- **A character is read by its own width.** `bytes.char_at(text, at)` slices
  the one character the lead byte announces, where three places sliced the
  rest of the file and split it into characters to take the first:
  `literals.escape_step`, the character-literal loop, and the refused
  character in `scan.hero`.
- **The spelling cannot come back in silence.** `suite_layout`'s
  `layout/appends` reads every module of the compiler, outside comments, for
  `l.tokens @ l.tokens.push(` and `l.diagnostics @ l.diagnostics.push(`, and
  names the line; it reads every module rather than a list of lexer files,
  because a list would expire the day a new one appends.

## What it does not change

Design.md Part 8 wart 8 stays a wart, on panel 144's ruling: a push through a
field copies the array in every Heroes program, and the cost is in the
spelling. Measured tonight on the trunk before this repair, `b.xs @
b.xs.push(i)` 0.16, 0.61 and 2.50 s user at 10,000, 20,000 and 40,000, the
same through a field of a local record, an array element, a map value and a
nested field; a bare local 0.00 at all three. This defect is the compiler's
own code using the spelling the ruling calls slow, not the wart.

## The measurements

`heroes lex` over a file of 500 functions and its 2, 4 and 8 copies, `user`
seconds, the trunk at `bdf430f1` against the lane at `e64b93ca`:

| the file holds | ×1 | ×2 | ×4 | ×8 |
|---|---|---|---|---|
| a string with escapes in each function, before | 1.32 | 5.13 | 20.67 | 91.05 |
| the same, after | 0.02 | 0.04 | 0.08 | 0.18 |
| a plain string in each, before | 0.03 | 0.12 | 0.48 | 2.58 |
| the same, after | 0.01 | 0.03 | 0.07 | 0.14 |
| no string, before | | 0.13 | 0.49 | 1.89 |
| the same, after | | 0.03 | 0.08 | 0.17 |

The token dump of the three ×8 files is byte-identical between the two
compilers, 48,015 tokens each. `heroes check selfhost/main.hero`, alternating
the two compilers: 7.09, 7.14, 7.13 s user before and 5.47, 5.51, 5.44 after,
real within 0.4 s of user plus sys in all six runs.

Gate in the lane: the compiler's 727 tests; the full net 2519 passed and 0
failed, `layout` at 3 with `layout/appends`; the net's own 168; the seed
regenerated and the fixpoint held. On Linux x86-64, Linux arm64 and the
Windows box: surface 121, 121 and 114, annotations 191, check 150, layout 3
and lines 203, 203 and 201, every one 0 failed, the smaller counts the
programs that need a library the machine does not have. On the trunk after
the merge: the index equal to the lane's tree, the compiler's 727 tests.
