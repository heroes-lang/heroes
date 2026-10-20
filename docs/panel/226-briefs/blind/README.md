# The blind seat's folder, as it was handed to the llm-ergonomist

A copy of `<scratchpad>/226-llm-ergonomist/` on 2026-10-20, minus `heroes-spec.md`, which is
`spec/heroes-spec.md` at `a0a8d1249` (sha256 `212264059c985c2e…`). The seat ran as a fresh
`claude -p` session from that folder, outside every git tree, with `--restricted --safe-mode
--strict-mcp-config`, the tools `Read` and `Write` alone, and `--max-budget-usd 4` (`/panel` § 2).
The two programs under `feedback/` are `tests/golden/full/never-rebound-near-miss-says-how-a-cell-is-declared.hero`
and `tests/golden/full/inferred-cell-width-class-names-its-birth.hero` with their header comments and
`#~` marks removed and their paths renamed `main.hero`; `full.txt` is each case's `.expected`,
`brief.txt` what `heroes check --brief` printed on 2026-10-20. The programs are kept as `.txt` so
that no suite walks them as Heroes sources.
