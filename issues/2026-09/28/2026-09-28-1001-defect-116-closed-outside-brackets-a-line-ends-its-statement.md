---
kind: defect
area: compiler
milestone: none
filed: 2026-09-27
commit: aee8b01ed0a4f427fcaf31b0aadf55ba4962fa4d
github: none
---

# Defect 116 closed: outside brackets a line ends its statement, and one that cannot is refused at the break

2026-09-28, M-agreed-retention step 28, in lane 181 (`5772c830`, the trunk
merged into it at `e3921986`, the spec and design.md at `4c453f5a`), merged
`08581ace`, on panel 181's provisional resolution
(`docs/panel/181-outside-brackets-a-line-ends-its-statement-in-both-directions.md`).

- [x] **116 — a statement continued at its own margin after a trailing operator compiles, where design.md says a long expression at depth zero is broken inside parentheses or not at all** | `y = a +` over `1` at the statement's own indentation is `check` 0 and prints 6: the lexer plants no terminator after `+` (Go's rule) and the same margin plants no indent, so the statement goes on; one level deeper it is refused; design.md §4.15 reads *at bracket depth zero every line's indentation is structural: a long expression is broken inside parentheses or not at all*, and *trailing-operator continuation at depth zero (Nim's rule) was considered and deferred* | `selfhost/layout.hero` (`is_line_ender`, `maybe_terminator`) · `docs/design/design.md` §4.15 · **closed 2026-09-28**

    **Origin:** lane B's agent, 2026-09-27, beside defect 107 (a comment on
    its own line in such a continuation is refused by `fmt` at exit 2, since
    the owner rule reads the continued line as a new logical line); searched
    `docs/work/DEFECTS.md`, `docs/records/` and `docs/panel/` for *same
    indentation*, *depth zero* and *Nim's rule* and found only panel 007's
    deferral and panel 180's reports quoting it. Re-run by the coordinator on
    the trunk at `f08b192d`: `y = a +` / `1` and `y = xs.` / `len()` at the
    same margin, `check` 0 and `run` printing 6 and 2; the continuation one
    level deeper, `check` 1.

    **Why it is a defect, and why it needs a sitting.** The compiler admits a
    form the source of truth says was deferred; the spec's own last-token
    rule reads as admitting it. Whether the repair refuses it, as design.md
    says, or design.md admits it, is what the lexer does: a panel path
    (CLAUDE.md § 4).

## The repair

`selfhost/open_line.hero` (new) refuses, with one diagnostic,
`continuation_outside_brackets`, at the break, a depth-zero line whose last
token cannot end a statement, at the same, a deeper or a shallower margin, and
a line that begins with a token that can only go on with the line above (a
binary operator, a spaced `-`, `?`, `::`) at the statement's margin or after a
closed block; a line that ends with a value owes its terminator, planted when
the next line's first token is read, so nothing is read twice and nothing
planted is taken back. The parser is handed the joined line. The fix joins the
lines, `certain` where the next line cannot stand alone and a `guess` where it
could; the parser adds the parenthesised form as a `guess`
(`selfhost/parse/wrap_break.hero`). An `error` token ends a depth-zero line, so
a malformed token no longer hides the next line's mistake. The code is on the
thesis list. Against the compiler-engineer's map of 340 programs: every
same-margin, shallower, blank-line, comment-line and CRLF break refused with
exactly one diagnostic; every `certain` fix applied compiles, 145 of 145. The
spec gains *"Outside brackets a line ends its statement: it may not end where
the statement cannot, and the next line may not go on with it, so a long
expression, a condition included, breaks inside parentheses."*, and § 1 reads
*"a condition needs no parentheses"* (9060 real, +61). The ratified
`check/depth-zero-continuation.hero` carries a dated correction beneath its
2026-08-04 header.

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
