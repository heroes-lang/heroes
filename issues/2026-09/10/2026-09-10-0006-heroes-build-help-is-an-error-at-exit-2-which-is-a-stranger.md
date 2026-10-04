- [ ] **M-core-packages** | `heroes build --help` is an error at exit 2, which is a stranger's first minute | `selfhost/cli/table.hero:139-176` · `selfhost/cli/argv.hero:78-81` · `selfhost/cli/help.hero:22-46`

    **Origin:** author decision 2026-09-10, § What production-ready means.

    **Measured**: `./heroes build --help` answers ``error: `build` does not accept
    `--help` — it accepts --dump-ir, --emit-c, -o, --include, --library,
    --sanitize, -O0, -O2`` and exits **2**, the code reserved for *the tool could
    not run*. The message is good and the verdict is wrong: asking a subcommand for
    its help is not a failure, and every tool a production user has ever run
    answers it.

    **Where the answer already lives**: `--help` and `-h` are global words handled
    outside the flag table (`selfhost/cli/argv.hero:78-81`), and
    `selfhost/cli/help.hero:22-46` already prints per-command text from the one
    table that both parses argv and prints help, so the two cannot disagree. What
    is owed is that a subcommand's `--help` reaches that printer instead of the
    unknown-flag arm, and the exit code that goes with it — **0**, since the tool
    did what it was asked.
