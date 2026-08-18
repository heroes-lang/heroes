# The blessed emissions

One file per program: the C that **both** compilers emitted for it, byte for
byte, on 2026-08-18 — the last day the bootstrap and the self-hosted compiler
both existed and `crates/heroes-cli/tests/differential.rs` was green over all
142 of them.

Nothing here is hand-written and nothing here is edited. The instrument that
reads these bytes, the argument for storing the whole C rather than a hash, and
the four guards that keep the oracle from quietly stopping are all in
`tests/harness/suite_emission.hero`'s module doc — its only home.

    heroes run tests/harness/main.hero -- <compiler> emission

To re-bless, deliberately, after an emitter change that is *meant* to change
these bytes:

    UPDATE_EMISSION=1 heroes run tests/harness/main.hero -- <compiler> emission

CLAUDE.md §9 follows the bytes: a regeneration that turns a red suite green has
its diff read and quoted in the commit body. What changed here is what changed
for every Heroes program in the world, so it is worth the minute.

The name of each file is its program's path, flattened — `run-loop.c` is
`tests/golden/run/loop.hero`, and `examples-json-main.c` is
`examples/json/main.hero`. A file no program claims is a failure, not a leftover.
