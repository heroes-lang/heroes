---
kind: defect
area: print
milestone: none
filed: 2026-09-28
commit: 687c54f34fea0c8393828dd444279ec43b61fc59
github: none
---

# Defect 125 closed: fmt prints a negative pattern with its sign against its literal

2026-09-28, M-agreed-retention step 30, in lane 123 (`d5e088d9`, `492a7ee4`,
`1e2c3506`, `17e3c5a8`, `5c0a9aac` and `c6d62c7e`, the trunk merged into it at
`6a84ce36`), merged `eed23b19`. The six defects were found at 181's landing and
beside each other's repairs; the two found after the author's waiver of
16:40, 129 and 130, are carried and not repaired here.

- [x] **125 — `fmt` accuses itself on every negative literal pattern** | `k = match n` with the arm `-1 => 10` is `check` 0, and `heroes fmt` exits 2 with *`fmt` produced source that does not parse* (`expected_pattern`, found `(`) and *this is a compiler bug*: `print/bodies.render_pattern` renders a literal pattern through `render_expr`, whose unary case prints `(-1)`, and a parenthesis opens no pattern; also on `return match n` and on a statement `match` | `selfhost/print/bodies.hero` (`render_pattern`) · **closed 2026-09-28**

    **Origin:** lane 123's agent, 2026-09-28, attacking the shapes beside
    defect 123 (reproducers `shapes/u01` to `u03` in
    `/Users/joseph/Temp/heroes-lane-123-scratch/`); reproduced by the
    coordinator on the trunk's compiler at `62425393` the same morning
    (`neg_pattern.hero` in `/Users/joseph/Temp/heroes-recovery-2026-09-26/defect125/`).

    **Why it is a defect.** A verb that says *this is a compiler bug* is one;
    and no suite saw it because no fixture and none of the probe's families
    holds a negative pattern.

## The repair

`print/fmt.arm_heads` rendered each arm's pattern with the `--dump-ast`
printer, which writes every unary in parentheses to prove the precedence
table, so `-1` came out as `(-1)`, and `(` begins no pattern. `print/parens.pattern`
renders an arm's pattern as `fmt` prints an expression, the sign against its
literal (`-1`, `-'a'`, `-0x1F` lowered to `-0x1f`); the dump keeps its
parentheses, which are its purpose. A round-trip test in `print/fmt.hero`, a
fixture that is its own canonical form
(`tests/golden/surface-fixtures/patterns125/negative.hero`) with two `surface`
rows, and the fixture made a seed of `heroes probe` (`cli/probe.hero`'s
FIXTURES 10 to 11): no seed held a negative pattern before, and the probe's
families only vary what a seed holds, which is why no suite caught this
(`d5e088d9`).

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
