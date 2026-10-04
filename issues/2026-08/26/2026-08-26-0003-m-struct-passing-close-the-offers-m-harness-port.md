- [ ] **M-struct-passing close — the offers** | M-harness-port | `tests/golden/check/x.hero` carries `#~ <code>` comments AND `x.expected` records the same diagnostics. Why is that redundancy the one check a regenerator cannot fake? (2026-08-26)

    **Where to look:** tests/harness/suite_annotations.hero · CLAUDE.md §9
    **Why it matters:** 181 pinned diagnostics rest on it, and it is the reason `UPDATE_GOLDEN` can be forbidden rather than merely discouraged (the fixture as it stood 2026-08-26)
