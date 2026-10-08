---
kind: defect
area: cli
milestone: none
filed: 2026-10-04
commit: c1d23efebdcd7c84afa8e4b852729244566de92f
github: none
---

- [x] **296 — three test helpers slice a text at every byte, so they panic on a character above ASCII when the needle is absent or comes after it** | `holds` in `selfhost/cli/process.hero:309` and `selfhost/cli/publish.hero:271`, and `has_text` in `selfhost/emit/externs.hero:257`, slice the text at each byte: on a text holding a character above ASCII they panic *string slice splits a character* when the needle is absent or after it; batch 9's emit lane met it extending defect 240's test with `é`, `€` and an emoji, and wrote that test its own byte comparison (lane b9-emit, 2026-10-04) | the three helpers · the non-test searches, which the lane read as byte- or character-safe · **class: improvement**

    **Origin:** lane b9-emit, 2026-10-04 (its reply's *found beside*), measured on its own test; the three places read by the coordinator in batch 9's round tree.

    **Class: improvement**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): a test's own helper that dies on a text a test may hold; no program moves.

    Repaired at `c1d23efe`, 2026-10-07 (lane b14-cli), gated by its cases and the compiler's own tests; the net is owed at the batch's close. One predicate, `cli/strings.holds`, asks each offset of its bytes (`bytes.bytes_at`), and the four slicing copies (`process.holds`, `publish.holds`, `syntax_cmds.strings_has`, `emit/externs.has_text`) and `compiling.holds_bytes` are gone, their 37 calls asking it; its case asks a needle absent, before and after `é`, `€` and an emoji, and its first call, made of the base's helper copied into a program, aborted at exit 134.

## The repair

Repaired at `c1d23efe`, 2026-10-07 (lane b14-cli), gated by its cases and the compiler's own tests; the net is owed at the batch's close. One predicate, `cli/strings.holds`, asks each offset of its bytes (`bytes.bytes_at`), and the four slicing copies (`process.holds`, `publish.holds`, `syntax_cmds.strings_has`, `emit/externs.has_text`) and `compiling.holds_bytes` are gone, their 37 calls asking it; its case asks a needle absent, before and after `é`, `€` and an emoji, and its first call, made of the base's helper copied into a program, aborted at exit 134.

**Closed 2026-10-08**, after the push's platform legs, this defect being at the C boundary (`.claude/rules/verification.md` § The batch): batch 14 closed on this Mac alone and the CI's legs ran its cases afterwards. The CI's four legs on `ee95a6f0` (run 37735987684, created at 08:08 and its Windows leg finished at 10:22 on 2026-10-08) are all green: Darwin arm64 with the net at 6,809 passed and 0 failed, Linux arm64 and Linux x86-64 at 6,790 each, Windows x86-64 at 6,648, and on every leg the compiler's own tests 1,430, the module's 260 and the net's own tests 308, all passed. A case bound to one platform ran where it is bound, read from the legs' logs: the SDL3 event case of defect 213 and the `sys/prctl.h` case of defect 437 are not among the SKIP lines of either Linux leg (they are, as they must be, on Darwin and on Windows), and the Linux legs built SDL3 from source and checked that `pkg-config` answers 3.2.10 before the net started. The leg that had read red on `02256c1e`, Windows, did so on defect 505's test of the order of legs, repaired at `ee95a6f0`.
