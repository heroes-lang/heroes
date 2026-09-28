# Defect 120 closed: a discard is never certified on a line that may go on with the one above

2026-09-28, M-agreed-retention step 28, in lane 181 (`5772c830`, the trunk
merged into it at `e3921986`, the spec and design.md at `4c453f5a`), merged
`08581ace`, on panel 181's provisional resolution
(`docs/panel/181-outside-brackets-a-line-ends-its-statement-in-both-directions.md`).

- [x] **120 — `discarded_value`'s `certain` fix on a line that begins with a spaced `-` after a finished line drops a term the author meant to subtract** | `total = base` / `- fee` at one margin is refused with `discarded_value`, and its fix, tagged `certain` in `check --json`, writes `_ = - fee`: applied with `check --apply`, the program prints 100 where the subtraction meant 93; `.claude/rules/diagnostics-and-goldens.md` reads *a `certain` fix repairs the defect the diagnostic names*, and panel 180's `spaced_minus_element` gives the same shape inside a list two `guess` fixes | `selfhost/discard_errors.hero` (`discarded_value`) · `selfhost/parse/list_line.hero` (`spaced_minus_element`) · **closed 2026-09-28**

    **Origin:** panel 181's completeness critic, 2026-09-28; reproduced by the
    coordinator on the trunk's compiler at `0fc98107` the same night
    (`q3_minus.hero`, `check --apply` then `run` printing 100).

    **Why it is a defect.** A machine-applicable fix that compiles and keeps
    the bug is the case the rule was written for: `heroes check --apply`
    would automate the silent reading the refusal exists to stop.

## The repair

`total = base` over `- fee` is refused by defect 116's rule at the `-`, with
the join, `certain`, which gives `total = base - fee` and prints 93; and
`discarded_value` offers `_ = ` only as a `guess` on a line that begins with
`-`, `.`, `(` or `[` (`selfhost/discard_errors.hero`). The audit of the
function's one `certain` site found a second fault there, its insertion placed
by the value's span, so `(5)` alone took `(_ = 5)`, which does not parse; it
now inserts at the line's first byte (the `check/` case
`fixedbugs-a-discard-certified-on-a-line-that-may-go-on`).
`.claude/rules/diagnostics-and-goldens.md`'s reading of 2026-09-08, *none
violates the rule*, carries a dated correction beneath it. The `check/` case
`fixedbugs-a-discard-that-drops-a-term-meant-to-be-subtracted` with its
`.fixed`.

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
