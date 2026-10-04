- [x] **072 — two allocator families collapse onto one handle type, and each frees the other's blocks in silence** | **closed 2026-09-20**, M-declared-extents step 18, on panel 170's narrowing | **The repair is one condition and it names `void` and no other word.** `one_tag_one_type` is stood down at `tag void`, so two C allocator families that both hand back `void *` may be two Heroes types. The crossed free is then `error[type_mismatch]` at **check**, twice, one per crossed call; the matched program — the one the author meant and could not write — runs at exit 0 | `selfhost/check/decls.hero:313` · `tests/golden/run/fixedbugs-two-allocator-families-over-one-void.hero` · `tests/golden/check/fixedbugs-a-crossed-free-between-two-void-families.hero` | 170

    **Origin:** panel 169's ffi-pragmatist, 2026-09-20, while measuring what caps
    the handle route; the repair is panel 170's completeness critic's, as a route
    nobody listed.

    ## The repair, and it is the rule ceasing to contradict itself

    `one_tag_one_type` exists because two Heroes types over one tag would put the
    checker and clang on opposite sides — defect 029, panel 145, in
    `selfhost/handles.hero`'s own words: *"two handles of one tag are two Heroes
    types and ONE C type … the mutant survives every instrument the language has,
    permanently."*

    **At `tag void` the rule PRODUCES that shape instead of preventing it.** Two
    families that both hand back `void *` have no tag to differ by, so the rule
    forces them onto one Heroes type and the swap it exists to kill becomes an
    ordinary call. Admitting a second record over `tag void` gives them two
    types, both still `void *` in C, and Heroes refuses the crossing itself.

    **`void` and no other word**, because every other tag is a name the author
    can vary: `record Arena tag arena_t` beside `record Heap tag heap_t` already
    works where the header declares both, and the census that opened this found
    the pair only where the header declares neither — `sqlite3_malloc`/
    `sqlite3_free` beside `malloc`/`free`, which is SQLite's own shape.

    ## The measurements

    | | before | after |
    |---|---|---|
    | the crossed program | `check` 0, `build` 0, **`run` 0**, arena destroyed and heap block leaked, **0 ASan lines** | **`check` 1**, two `error[type_mismatch]`, one per crossed call |
    | the matched program | **unwritable** — `error[duplicate_tag]` | `run` **exit 0** |
    | two handles over a **real** tag, defect 029's own shape | `error[duplicate_tag]` | **`error[duplicate_tag]`, unchanged** |

    The third row is the one that says the narrowing is narrow:
    `tests/golden/check/ffi-handle-refusals.hero` still asserts `duplicate_tag`
    over `sqlite3` and over `tm`, and it passes.

    ## Why the refusal is Heroes' and can never be clang's

    The check case ships **no header**, and that is the point rather than a
    convenience: `void *` and `void *` are one C type, so clang could not have
    refused either crossed line. This is the compiler doing what §1.12 asks of it
    where no second judge exists.

    ## The gates

    `check` **130**, `annotations` **169**, `run`, `emission`, `determinism`,
    `corpus`, `unsupported`, `canonical`, `layout`, `order`, `records`, `fixes`,
    the compiler's own and the net's own — all 0 failed, with the whole golden
    tree run because a change to what the checker refuses is judged by every one
    of them (`.claude/rules/verification.md`). **Zero other programs changed
    verdict**, which is panel 170's completeness critic's own prediction, scored
    here.

    The seed was regenerated in the same commit and the fixpoint verified
    byte-identical.
