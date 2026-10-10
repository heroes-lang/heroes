---
kind: defect
area: cli
milestone: none
filed: 2026-10-10
commit: none
github: none
---

- [ ] **593 — on macOS two links of the same objects from one line give two binaries that differ** | measured 2026-10-10 on this Mac (Apple clang 21.0.0, `ld-27037.1`): the compiler's 580 objects linked twice by hand from one response file into one path gave two binaries that differ from byte 1449, the `LC_UUID`, on; the base compiler's per-module build of itself, built twice warm, gave two that differ in 206,113 bytes, and two generations of the repaired compiler differ in 231,207 with an identical symbol table and debug map (`nm -ap`) and the C they emit `cmp`-equal; in the Linux arm64 container (Debian clang 22.1.8) the two generations are `cmp`-equal; what in the link varies is not known, unrun | `selfhost/cli/link.hero`'s `link_line` and the flags it carries (`flags.level_words`, the link's own flags); the linker's options for a reproducible output · **class: improvement**

    **Origin:** found by lane b19-link at 16:04 on 2026-10-10, checking that defect 583's repair left a build's binary reproducible: it was not on this Mac before the repair either, the base compiler's two links differing as the repaired one's do.

    **Class: improvement**, 2026-10-10 (`.claude/rules/verification.md` § Bounded discovery): no rule here promises a binary's bytes, the fixpoint is the emitted C (`.claude/rules/generated-c.md` § The instrument), and every binary runs; it is a build that could be made reproducible.
