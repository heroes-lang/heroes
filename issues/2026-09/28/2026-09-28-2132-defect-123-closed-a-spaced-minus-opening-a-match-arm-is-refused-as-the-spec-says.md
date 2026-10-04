# Defect 123 closed: a spaced minus opening a match arm is refused, as the spec says

2026-09-28, M-agreed-retention step 30, in lane 123 (`d5e088d9`, `492a7ee4`,
`1e2c3506`, `17e3c5a8`, `5c0a9aac` and `c6d62c7e`, the trunk merged into it at
`6a84ce36`), merged `eed23b19`. The six defects were found at 181's landing and
beside each other's repairs; the two found after the author's waiver of
16:40, 129 and 130, are carried and not repaired here.

- [x] **123 — a `match` arm whose pattern begins with a `-` set apart from its operand compiles, where the spec refuses it** | `k = match n` with the arm `- 1 => 10` is `check` 0 and prints 10, as `-1 => 10` does; spec § 0 reads *where a NEWLINE separates without a `,`, the next line may not begin with a `-` that does not touch its operand*, and a `match`'s arms are separated by NEWLINE; panel 180's `spaced_minus_element` refuses the shape in a literal and panel 181's refusal at depth zero, and an arm is the one NEWLINE-separated context left | `selfhost/parse/list_line.hero` (`spaced_minus_element`) · `selfhost/grammar_expr.hero` (`arm`) · **closed 2026-09-28**

    **Origin:** lane 181's agent, 2026-09-28, beside panel 181's landing,
    left unchanged and reported; reproduced by the coordinator on the trunk's
    compiler at `28d7129c` the same morning (`arm_minus.hero` in
    `/Users/joseph/Temp/heroes-recovery-2026-09-26/defect123/`).

    **Why it is a defect.** The spec states the rule without the bracket
    limit design.md's panel 180 bullet gives it, and CLAUDE.md § 12 reads the
    disagreement as the compiler's: a reader of the spec predicts a refusal
    the compiler does not make. The shape has one reading, a negative
    pattern, so the refusal loses no program and its fix, the `-` touching
    its operand, is the spelling `fmt` prints.

    **Corrected 2026-09-28, by lane 123's agent:** `fmt` prints no spelling
    of a negative pattern, it exits 2 on every one, the unspaced `-1 =>`
    included (defect 125), so the last clause above was false when written.

## The repair

`parse/arm_line.spaced_sign`, called at an arm's first pattern, refuses a `-`
that a space sets apart from its operand, with the list's code,
`spaced_minus_element`: one sentence in the spec, one code for it, where a new
code would be a new diagnostic class and a panel's. Its fix is `certain`, since
in a pattern a `-` can only be its literal's sign; nothing is consumed, so the
parse reads the pattern it always read. A comment or a line end after the `-`
is left to the lexer's `continuation_outside_brackets`, so one break costs one
diagnostic. And the join after a `-` that begins its line writes no space
(`open_line.refuse_the_end`), so the certain join on `match n` over `-` over
`1 => 10` writes `-1 => 10`, which this refusal admits, never `- 1`, and never
before `>`. Sixteen refused shapes, each applied with `check --apply`, check
clean and print what the trunk's compiler printed for the original
(`492a7ee4`).

## The gate

Each commit in the lane was gated, one suite at a time, by the map's suites for
its change, the compiler built from the regenerated seed and the fixpoint by
`cmp`, with the compiler's own tests and the net's own; each commit's body
lists them. A `check` census over the tree's `.hero` files, the trunk's
compiler against each commit's, moved one file in the six commits: 127's own
new `run` golden, 1 to 0. The merge of the trunk at `0338b598` (`6a84ce36`) had
one conflict, `seed/heroes.c`, regenerated and not resolved by hand, and moved
one blessed emission, 127's new file, by 36 lines each way, every one a
` = {0}` dropped on a temporary, 23 `HeroStr` and 13 `HeroArrayHeader *`: panel
182's rule arriving with the trunk, the file's 12 named slots keeping theirs
(counted by the coordinator on the merge). After it: the fixpoint by `cmp`, the
compiler's 828 tests, the net's own 179, and the whole net, 3,144 passed and 0
failed.

Linux arm64 on the merged tree (`6a84ce36`, the `heroes-linux-arm64` image):
the compiler's 828 tests and surface 334, canonical 2, annotations 216, check 167, fixes 32, layout 4, order 3, lines 208, run 207, emission 626, determinism 237, wholes 303, descriptors 303, cache 6, grammar 9, spec 20, probe 24, corpus 53 and warnings 266, each 0 failed. The Windows box on the same
tree, one archive whose sha256 matched on both sides (`190dffa99b896aac`), seed
`0bef1dfe91c51dcd`: the compiler's 828 tests and surface 327, canonical 2, annotations 216, check 167, fixes 32, layout 4, order 3, lines 206, run 205, emission 604, determinism 235, wholes 303, descriptors 303, cache 6, grammar 9, spec 20, probe 24, corpus 50 and warnings 261, each 0 failed.
