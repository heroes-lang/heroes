---
kind: defect
area: compiler
milestone: none
filed: 2026-10-03
commit: f57047318351e2e024d55d1d5647a65d437dd9de
github: none
---

- [ ] **252 — what lld-link and a Windows `pkg-config` do with a `>`, a quote or a backslash in a `link` or `package` name, and with a `/` in a `link`, is unmeasured, and the rules admit them** | batch 8 refuses `*`, `<`, `?` and `|` in a group head's name for Windows, and the names NTFS stores as another file's (234, 235), judging all three strings, and admits `>`, `"` and `\` in a `link` or a `package` and `/` in a `link` (batch 8's FFI lane, 2026-10-03, its report's finding 4); `selfhost/head_windows.hero:21` says *What lld-link and a Windows `pkg-config` do with such names is unrun* | `selfhost/head_windows.hero` · `selfhost/head_names.hero` · the Windows box · **class: improvement**

    **Origin:** batch 8's FFI lane, 2026-10-03 (its report's finding 4).

    **Class: improvement**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): a measurement owed on the box; no program measured wrong.

    **2026-10-07, lane b14-box**: Repaired at `f5704731`, gated by its cases and the compiler's own tests; the net is owed at the batch's close. Measured on the Windows box (clang 23.1.1, lld-link, NTFS): `>` and `"` stand in no file's name (Win32 error 123, as `*`) and lld-link could not open `g>h.lib` or `g"h.lib` by `-l`, `/defaultlib:` or a path; a `\` is a directory to lld-link and a byte of one file's name to ld64 and GNU ld 2.44; a `link`'s `/` opened `sub/m252.lib` there and `libsub/m252.a` on this Mac and in the Linux container; the box carries no `pkg-config`, so a Windows `pkg-config` stays unrun. Each is now `unwritable_name` in a `link` and the first three in a `package`, a rooted name keeping panel 055's message; its case `fixedbugs-252-a-library-name-no-windows-linker-reads-as-one-file` holds seven refusals and seven other lines, `fixedbugs-235` and `fixedbugs-216`'s package case read `zlib>=1` as `unwritable_name`, and the compiler's 1,359 tests passed.
