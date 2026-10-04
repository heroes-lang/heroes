---
kind: defect
area: runtime
milestone: none
filed: 2026-10-04
commit: none
github: none
---

- [ ] **314 — on the Windows box a `str` released twice runs on in silence unless ASan is on: the runtime's mark reads a freed block intact there, and the second release writes into it** | panel 190's ffi-pragmatist's control `ctl.doubled.c` (the trunk's C of a two-return function, one `hero_str_decref(h4_s1)` written twice): exit 134 with the runtime's *a str's block has lost its mark* at `-O0` and `-O2` on this Mac (Apple clang 21) and on Linux arm64 (clang 22.1.8 and 18.1.8), exit 0 with the right output on the Windows box (clang 23.1.1), where only ASan names it, `heap-use-after-free ... runtime\parts\str.c:77 in hero_str_hdr_checked` (`docs/panel/190-reports/ffi-pragmatist.md` § 5, 2026-10-04) | `runtime/parts/str.c` (`hero_str_hdr_checked` and `hero_str_decref`: the mark is read in a block already freed, and a count found at 0 is taken to -1 in it) · **class: improvement**

    **Origin:** panel 190's ffi-pragmatist, 2026-10-04, the controls of its sections 1, 3 and 5, run on all three platforms before the Windows box stopped answering at about 09:03. The mechanism is read from the code, not run: the mark sees a freed block only where the allocator wrote over the block's first bytes, which Darwin's and glibc's do and the box's did not; there a second `hero_str_decref` finds the count at 0, writes -1 into the freed block, and returns.

    **Class: improvement**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): hardening of an instrument. A correct program never releases a string twice; a doubled release is a compiler's fault, which the mark exists to name, and it names it on two platforms of three. No route of panel 190 moves it.
