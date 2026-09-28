# Defect 127 closed: a literal arm takes the width of what it matches

2026-09-28, M-agreed-retention step 30, in lane 123 (`d5e088d9`, `492a7ee4`,
`1e2c3506`, `17e3c5a8`, `5c0a9aac` and `c6d62c7e`, the trunk merged into it at
`6a84ce36`), merged `eed23b19`. The six defects were found at 181's landing and
beside each other's repairs; the two found after the author's waiver of
16:40, 129 and 130, are carried and not repaired here.

- [x] **127 — a literal pattern on an integer type other than `i64` is refused, so a `match` over a `u8` can only be `_`** | `255 =>` in a `match` over a `u8` is `type_mismatch: expected u8, found i64`, and `1 =>` over an `i32` likewise: `check/walk.literal_pattern` synthesises the literal's type, `i64`, instead of checking the literal against the scrutinee, where spec § 2 reads *a literal takes the type its context asks for*; and `x: i64 = -9223372036854775808` compiles while the same spelling as a pattern is `int_out_of_range` | `selfhost/check/walk.hero` (`literal_pattern`) · **closed 2026-09-28**

    **Origin:** lane 123's agent, 2026-09-28, attacking the shapes beside
    defect 124 (reproducers `shapes/v05`, `v06`, `v12`, `v15`, `v17` in
    `/Users/joseph/Temp/heroes-lane-123-scratch/`); reproduced by the
    coordinator on the trunk's compiler at `5fdd0edd` the same afternoon
    (`u8_pattern.hero` in `/Users/joseph/Temp/heroes-recovery-2026-09-26/defect127/`).

    **Why it is a defect.** A program the spec admits, a `match` over a byte
    by its values, is refused, and the only spelling left is `_`; the
    refusal's message names a type the author never wrote.

## The repair

`check/walk.literal_pattern` synthesized the pattern's literal, which with no
context is an `i64`, and then compared. It checks the literal against the
subject's type with the checker's own `check`, the path a binding's literal
takes, so the sign folds into the literal and the range is the width's at
each edge; the lowering reads the type the checker records, so a `u8` arm
compares a `uint8_t` in the emitted C. Now compiling: every integer width's
edges, `'a'` on a byte and on `s[0]`, and `-9223372036854775808` on an `i64`;
refused with `int_out_of_range` naming the width: one past each edge, where the
trunk's message named `9223372036854775809`, a number nobody wrote. A `run`
golden, `a-literal-arm-takes-the-width-it-matches`, its `.expected` written by
hand, and its emission blessed by the suite's own procedure, one new file and
no other blessed file changed (`5c0a9aac`).

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
