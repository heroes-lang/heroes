- [ ] **190 — `records/lists` reads an item's class token and not the rest of its first line, so an item that lost its title and its *where* field reads as whole** | item 130's first line as `8349d264` left it, with no `**` closing its title and no ` | ` at all, before its class token: `records` 24 passed, 0 failed, after that commit and after every one until the item was restored at `50644159` | `tests/harness/suite_records.hero` (`list_offences`; the class rule, about `:4448-4610`) · **class: improvement**

    **Origin:** the coordinator, 2026-10-02, on the damage panel 187's completeness critic found (`docs/records/log/2026-10-02-2141-item-130-cut-in-8349d264-and-restored-what-cut-it-is-unknown-the-commit-did-not-read-its-diff.md`). It would have caught the cut in the item's line, not the one in its body.

    **Class: improvement**, 2026-10-02 (`.claude/rules/verification.md` § Bounded discovery): hardening of an instrument; nothing a program does moves.
