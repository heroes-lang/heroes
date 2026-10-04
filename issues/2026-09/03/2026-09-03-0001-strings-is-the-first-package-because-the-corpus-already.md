---
kind: feature
area: examples
milestone: M-core-packages
filed: 2026-09-03
commit: none
github: none
---

- [ ] **M-core-packages** step 1 | `strings` is the first package because the corpus already wrote it | `examples/` · `tests/harness/strings.hero` · `docs/panel/057`, `097`

    **Origin:** measured 2026-09-03.

    **Four helpers copied by hand across `examples/`, sharable by nothing**:
    `function split_lines(` in 8 files, `is_space` in 8, `trimmed` in 7,
    `index_of` in 4, and `tests/harness/strings.hero` carries `trimmed`,
    `is_space`, `lines`, `contains`, `starts_with` and `ends_with` once more.
    `use` cannot reach a module outside the program's root
    (`selfhost/modules.hero:125`), no search path exists by ruling (panel 028
    R3), and the two doors into the language are shut: panel 097 condition 5
    closes `selfhost/library_source.hero`, panel 057 refused `path_join` as a
    built-in on Principle 0. A package is the only home, and the copies are the
    measurement of what it must hold before anyone invents it.

    **Where to look also:** `grep -rl '^function split_lines(' examples` ·
    `spec` § Built-ins.
    **Why it matters:** the corpus has already written the package, seven or
    eight times, without a name.

    **Re-verified 2026-09-10: STILL OPEN, and every count is UNDERSTATED.** With
    the item's own command: `split_lines` **9** files (said 8), `is_space` **9**
    (said 8), `trimmed` **7** and `index_of` **4** (both exact). And
    `tests/harness/strings.hero` carries **14** functions, not the six listed —
    `ends_with, without_suffix, split_on, lines, base_name, trimmed, is_space,
    contains, starts_with, without_prefix, to_number, index_of, split_text, words`.
    **`strconv` joined this step's package table on 2026-09-10** and `to_number`
    above is the copy that proves why.
