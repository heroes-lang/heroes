# Defect 118 closed: `fmt` refuses an arm continued at its own margin at exit 1, as the compiler does

2026-09-28, M-agreed-retention step 28, in lane 181 (`5772c830`, the trunk
merged into it at `e3921986`, the spec and design.md at `4c453f5a`), merged
`08581ace`, on panel 181's provisional resolution
(`docs/panel/181-outside-brackets-a-line-ends-its-statement-in-both-directions.md`).

- [x] **118 — `fmt` accuses itself on a `match` arm continued at its own margin** | `.dot => a +` over `1` at the arm's own indentation, inside `y = match s`, is `check` 0 and prints 6 (defect 116's shape), and `heroes fmt` on it exits 2 with *`fmt` is not a fixpoint on its own output* and *this is a compiler bug*; the same shape after a `.` at a statement's margin, `y = xs.` over `len()`, formats at exit 0 to `y = xs.len(` over a `)` of its own | `selfhost/print/fmt.hero` · `selfhost/layout.hero` (`maybe_terminator`) · **closed 2026-09-28**

    **Origin:** the coordinator, 2026-09-28, writing panel 181's brief on
    defect 116: the seventeen shapes of that brief run through `fmt` with the
    trunk's compiler built from the seed at `dfcac362`; five format at exit 0,
    the arm exits 2.

    **Why it is a defect, and why it has a number of its own.** A verb that
    says *this is a compiler bug* is one, whatever the input's standing. If
    panel 181 refuses the shape, the input stops parsing and this closes with
    116; if it admits it, the formatter is owed a repair of its own, and a
    defect hidden inside another's body would be forgotten by the route that
    does not close it.

## The repair

Defect 116's: the arm continued at its own margin no longer parses, so `fmt`
refuses it at exit 1 with the compiler's diagnostic where it accused itself at
exit 2; `.dot => .` over `sq` likewise. Over the compiler-engineer's 340
programs `fmt` exits 2 on none (the trunk before the landing, on 5). The
`check/` case `fixedbugs-fmt-on-an-arm-continued-at-its-own-margin` with its
`.fixed`, and two `surface` rows over `continuation181/`.

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
