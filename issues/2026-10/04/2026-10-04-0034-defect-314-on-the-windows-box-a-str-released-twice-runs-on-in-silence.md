---
kind: defect
area: runtime
milestone: none
filed: 2026-10-04
commit: 8eb70d0bca8deb2c2ad23d9af1e2d3850241e132
github: none
---

- [ ] **314 — on the Windows box a `str` released twice runs on in silence unless ASan is on: the runtime's mark reads a freed block intact there, and the second release writes into it** | panel 190's ffi-pragmatist's control `ctl.doubled.c` (the trunk's C of a two-return function, one `hero_str_decref(h4_s1)` written twice): exit 134 with the runtime's *a str's block has lost its mark* at `-O0` and `-O2` on this Mac (Apple clang 21) and on Linux arm64 (clang 22.1.8 and 18.1.8), exit 0 with the right output on the Windows box (clang 23.1.1), where only ASan names it, `heap-use-after-free ... runtime\parts\str.c:77 in hero_str_hdr_checked` (`docs/panel/190-reports/ffi-pragmatist.md` § 5, 2026-10-04) | `runtime/parts/str.c` (`hero_str_hdr_checked` and `hero_str_decref`: the mark is read in a block already freed, and a count found at 0 is taken to -1 in it) · **class: improvement**

    **Origin:** panel 190's ffi-pragmatist, 2026-10-04, the controls of its sections 1, 3 and 5, run on all three platforms before the Windows box stopped answering at about 09:03. The mechanism is read from the code, not run: the mark sees a freed block only where the allocator wrote over the block's first bytes, which Darwin's and glibc's do and the box's did not; there a second `hero_str_decref` finds the count at 0, writes -1 into the freed block, and returns.

    **Class: improvement**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): hardening of an instrument. A correct program never releases a string twice; a doubled release is a compiler's fault, which the mark exists to name, and it names it on two platforms of three. No route of panel 190 moves it.

    Repaired at `8eb70d0b`, 2026-10-07 (lane b14-runtime), gated by its cases and the compiler's own tests; the net is owed at the batch's close. A str's block has its mark overwritten by the runtime before it is freed and a count of 0 or below is refused by name; an array's element descriptor and a map's key descriptor are set to none before the block goes, and a map's release asks its count as an array's did, where it asked nothing and freed twice: panel 190's control, rebuilt from this tree's emitted C with one `hero_str_decref` doubled, ran on at exit 0 3 runs of 3 on the Windows box at `dad2da47` (`-O0` and `-O2`, through `cmd`) and stopped with the runtime's mark panic 3 of 3 after; this Mac at `dad2da47` missed a str of 20,000 bytes or more, an array of 1 to 32 elements and a map of 1 entry released twice, all named after, as are arrays of 4 to 1,000 and maps of 1 to 1,000 on the box where the heap stopped them with no word; about 2% more instructions a round on string, array and map traffic, 1.0 to 1.6% on the compiler's own `check`; cases `run/fixedbugs-314-*`, three, each red on this Mac's base. The Linux leg of the control is the round's.
