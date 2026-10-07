---
kind: defect
area: cli
milestone: none
filed: 2026-10-04
commit: c1d23efebdcd7c84afa8e4b852729244566de92f
github: none
---

- [ ] **296 — three test helpers slice a text at every byte, so they panic on a character above ASCII when the needle is absent or comes after it** | `holds` in `selfhost/cli/process.hero:309` and `selfhost/cli/publish.hero:271`, and `has_text` in `selfhost/emit/externs.hero:257`, slice the text at each byte: on a text holding a character above ASCII they panic *string slice splits a character* when the needle is absent or after it; batch 9's emit lane met it extending defect 240's test with `é`, `€` and an emoji, and wrote that test its own byte comparison (lane b9-emit, 2026-10-04) | the three helpers · the non-test searches, which the lane read as byte- or character-safe · **class: improvement**

    **Origin:** lane b9-emit, 2026-10-04 (its reply's *found beside*), measured on its own test; the three places read by the coordinator in batch 9's round tree.

    **Class: improvement**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): a test's own helper that dies on a text a test may hold; no program moves.

    Repaired at `c1d23efe`, 2026-10-07 (lane b14-cli), gated by its cases and the compiler's own tests; the net is owed at the batch's close. One predicate, `cli/strings.holds`, asks each offset of its bytes (`bytes.bytes_at`), and the four slicing copies (`process.holds`, `publish.holds`, `syntax_cmds.strings_has`, `emit/externs.has_text`) and `compiling.holds_bytes` are gone, their 37 calls asking it; its case asks a needle absent, before and after `é`, `€` and an emoji, and its first call, made of the base's helper copied into a program, aborted at exit 134.
