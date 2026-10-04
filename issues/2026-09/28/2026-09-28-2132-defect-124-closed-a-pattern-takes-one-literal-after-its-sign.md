---
kind: defect
area: compiler
milestone: none
filed: 2026-09-28
commit: 687c54f34fea0c8393828dd444279ec43b61fc59
github: none
---

# Defect 124 closed: a pattern takes one literal after its sign, as spec § 8 writes it

2026-09-28, M-agreed-retention step 30, in lane 123 (`d5e088d9`, `492a7ee4`,
`1e2c3506`, `17e3c5a8`, `5c0a9aac` and `c6d62c7e`, the trunk merged into it at
`6a84ce36`), merged `eed23b19`. The six defects were found at 181's landing and
beside each other's repairs; the two found after the author's waiver of
16:40, 129 and 130, are carried and not repaired here.

- [x] **124 — after a `-`, a `match` pattern reads a whole expression, so a pattern can name a variable or call a function** | `grammar_expr.pattern` reads `pattern_operand`, which is `unary`, so after a `-` any postfix expression parses, and `check/walk.literal_pattern` compares only its type: `-m => 10` with `m: i64 = 1` and `n = -1` is `check` 0 and prints 10, the pattern compared against a runtime name, and `-one() => 10` runs `one` inside the match, printing its 99 and then 10; also `-(1)`, `-xs[0]`, `--1`, `- -1`, and `-1.5` on an `f64` where `1.5` is `expected_pattern`; spec § 8 reads `Pattern = ... | [ "-" ] ( integer | string | character )` | `selfhost/grammar_expr.hero` (`pattern`, `pattern_operand`) · `selfhost/check/walk.hero` (`literal_pattern`) · **closed 2026-09-28**

    **Origin:** lane 123's agent, 2026-09-28, attacking the shapes beside
    defect 123 (reproducers in `/Users/joseph/Temp/heroes-lane-123-scratch/shapes/`);
    reproduced by the coordinator on the trunk's compiler at `aee8b01e` the
    same morning (`name_pattern.hero`, `call_pattern.hero` in
    `/Users/joseph/Temp/heroes-recovery-2026-09-26/defect124/`).

    **Why it is a defect.** A pattern is a constant the spec names, and the
    compiler admits a name, which silently reads a value at run time, and a
    call, which runs code with its effects inside a match, both at exit 0; and
    a float after a `-` where a float is not a pattern at all.

    **Widened 2026-09-28, by lane 123's agent:** it is wider than the `-`.
    With no sign at all a pattern reads a literal's suffixes, `"ab".len() =>
    10` compiles and prints 10, `'a'.to_i64().must() =>` compiles, and so does
    `0 | -m`: a pattern's literal is one token, optionally after one `-`.

## The repair

`grammar_expr.pattern` read a literal through `unary`, chosen so that `|`
could never be read as an operator, and `unary` reads any operand after a `-`
and every suffix after a literal: `-m => 10` compared the scrutinee with a
runtime name, `-one()` called a function inside a pattern, and `-1.5` was a
pattern where `1.5` is not. `parse/arm_pattern.read`, out of the expression
knot, reads ONE token, an integer, a string or a character, after at most one
`-`, as spec § 8's production writes it, and builds the same tree node, so the
checker and the lowering read what they always read. Anything else after the
`-`, and any suffix after the literal, is `expected_pattern`, whose message
now says what a pattern may be; it has no fix, since no repair of `-m` can be
derived from the program. The knot shrinks, `grammar_expr.hero` 1075 to 1013
lines as `layout` counts them, and the probe's eight known failures for this
defect are gone with their row (`17e3c5a8`).

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
