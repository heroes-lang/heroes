---
kind: defect
area: compiler
milestone: none
filed: 2026-09-28
commit: 687c54f34fea0c8393828dd444279ec43b61fc59
github: none
---

# Defect 128 closed: an arm an earlier arm covers is refused, a literal's value and a fallible's case included

2026-09-28, M-agreed-retention step 30, in lane 123 (`d5e088d9`, `492a7ee4`,
`1e2c3506`, `17e3c5a8`, `5c0a9aac` and `c6d62c7e`, the trunk merged into it at
`6a84ce36`), merged `eed23b19`. The six defects were found at 181's landing and
beside each other's repairs; the two found after the author's waiver of
16:40, 129 and 130, are carried and not repaired here.

- [x] **128 — two literal arms with the same value compile, while two arms naming the same case are refused** | `match n` with `0 => 10` and a later `0 => 30` is `check` 0 and prints 10, the second arm never reachable, and `-0` beside `0` likewise; the same shape over a variant, `.dot` twice, is `duplicate_arm` (*`.dot` is already covered by an earlier arm*, `selfhost/data_errors.hero:254`) | `selfhost/check/walk.hero` (the arm walk that calls `duplicate_arm` for a case and not for a literal) · **closed 2026-09-28**

    **Origin:** lane 123's agent, 2026-09-28, as a question beside defect 124
    (`shapes/v07`, `v14`); the coordinator found the existing refusal for
    cases and reproduced both shapes on the trunk's compiler at `5fdd0edd`
    (`dup_literal.hero`, `dup_case.hero` in
    `/Users/joseph/Temp/heroes-recovery-2026-09-26/defect127/`).

    **Why it is a defect.** The compiler already names an arm an earlier arm
    covers as a mistake, for cases; for literals it keeps the unreachable arm
    in silence, the plausible slip of a copied line whose value was not
    changed.

    **Widened 2026-09-28, by lane 123's agent, reproduced by the coordinator
    on the trunk's compiler at `d0f24496` at 18:50** (`w01` to `w06` in
    `/Users/joseph/Temp/heroes-recovery-2026-09-26/defect127/`): the class is
    an arm an earlier arm covers, and it compiles in silence in four more
    shapes: an arm after `_` (`_ => 20` over `0 => 10` prints 20); `"a"`
    twice on a `str`; `97`, `'a'` and `0x61 | 1`, one value in three
    spellings; and two `.ok` arms on an `i64?`, where `check/walk.case_pattern`'s
    fallible branch marks the case covered without asking whether it already
    was, while its variant branch refuses `.a` twice and `.a | .a`. The
    coordinator reads the fallible shape as this defect's class and not a
    new defect under the waiver of 16:40: the same rule, `duplicate_arm`, in
    the neighbouring branch of the same function, landing in this defect's
    commit.

## The repair

`check/walk.case_pattern` counted a variant's cases against the ones already
covered, and its fallible branch pushed onto the same list without asking it;
nothing counted a literal's value, and nothing counted what a `_` had already
taken. `check/reach.after`, called for every pattern over an integer or `str`
subject, refuses a literal whose value an earlier literal of the same `match`
took, read as the checker reads it for its width, the sign folded in and `-0`
as `0`, a string by its decoded text, and any literal or `_` after a `_`; a
fallible's `.ok` and `.err` named twice are `duplicate_arm` as a variant's case
is. One code for one mistake, a pattern no value can reach, and the message
names the earlier spelling where it differs (*`'a'` is already covered by an
earlier arm, written `97`, the same value*). No fix: deleting a covered arm
keeps the behaviour, but which arm the author meant is not in the program.
`case_pattern`, `wildcard_pattern` and `exhaustive` moved into
`check/reach.hero` along a seam, none of them calling back into the walk, so
`check/walk.hero` goes 1867 to 1773 lines as `layout` counts them against its
1870, where the repair in place would have made it 1875. Over the tree's 1202
`.hero` files no `check` exit moved: no program held such an arm
(`c6d62c7e`).

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
