---
kind: feature
area: cli
milestone: M-core-packages
filed: 2026-09-10
commit: none
github: none
---

- [ ] **M-core-packages** | `heroes test` cannot run one test, and the binary already can | `selfhost/cli/verbs.hero:104-163` · `selfhost/cli/table.hero` · `CLAUDE.md` §10

    **Origin:** author decision 2026-09-10, § What production-ready means. Homed
    here because this is the milestone that multiplies both the packages and their
    `test` blocks; it is small enough to ride any commit that touches the CLI
    table.

    **What is true today**: there are **545** `test` blocks in the corpus and no
    way to run one. `run_test` (`selfhost/cli/verbs.hero:104-163`) compiles once
    and then loops over every title, handing the binary an **index** — so the
    binary can already run test *N* and no flag exposes it. Tests are collected
    across every `use`d module, so a run cannot even be scoped to one file.

    **What it owes**: §10's stopping-rule argument, like any other flag. The
    argument available is that the harness itself will need it once a package's
    tests are a corpus of their own, which is the same shape that admitted
    `--operator` for `heroes mutate`. **A flag, never a verb.**
