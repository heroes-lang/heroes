---
kind: defect
area: cli
milestone: none
filed: 2026-10-05
commit: none
github: none
---

- [ ] **357 — a cache file a power cut left as zeros is read back as clang's refusal, an internal error at exit 2** | the Windows box's crash on 2026-10-04 (its System log: Kernel-Power 41, and EventLog 6008 *the previous system shutdown at 6:46:31 PM*) left 11 files of `/c/w/b10-4c3524fb-23776/build/`, written from 18:47:05 to 18:47:07, at their length and every byte zero, a pointee check's 3-byte `verdict-702a199d28974598-736ae57c2d490824.txt` among them; from then on `heroes.exe build tests/golden/unsupported/fixedbugs-157-a-record-over-an-enum-s-tag.hero` in that folder says `internal error: ` and three NUL bytes, then *clang refused the generated C*, at exit 2, where the same `heroes.exe` in a fresh folder says `ffi_tag_is_a_union` at exit 1 (measured on the box, 2026-10-05) | `publish.write_whole` and `publish.publish` (`selfhost/cli/publish.hero:95-116`: a write and a rename, no flush, where `runtime/parts/replace.c`'s `hero_stage_flush` flushes before its rename) · their 21 calls in `selfhost/cli/` (`compiling` 1, `layout` 8, `link` 1, `pointee` 5, `toolchain` 2, `units` 4) · `pointee.hero:146-153` and `layout.judged`, which read a kept verdict that is not `ok\n` as clang's words whatever it holds · **class: blocking**

    **Origin:** batch 11's coordinator, 2026-10-05, reading batch 10's Windows leg: `unsupported` 127 passed and 2 failed at `4c3524fb`, where batch 9's leg read 129 and 0, the two `fixedbugs-157-*` cases that bind a tag of another kind. Found by the files, not by bisecting the commits: of the 762 cached verdicts in that folder one held NUL bytes, written 34 s after the last heartbeat the System log records before the crash.

    **Class: blocking**, 2026-10-05 (`.claude/rules/verification.md` § Bounded discovery): an exit 2 where the author can be told, and a false message, *internal error* over three NUL bytes for the author's own extern disagreement; and robustness (design.md §1.12), a build cache that a power cut corrupts in silence serving every later build on it.

    **Measured beside it, 2026-10-05**: the 11 files are 1 verdict, 4 dump units, 3 screen units and 3 screen texts of the pointee and layout probes. The scan covered the `.txt`, `.json` and `.c` files written since 2026-10-04 12:00 under every `/c/w/*/build` on the box and was cut by its own 300 s bound before its last line, so 11 is a floor; objects and listings were not scanned. **Unrun**: what a zeroed object, listing or screen text does to a later build, and a power cut on this Mac or Linux, which no machine here can be asked for (`replace.c`'s own *NOT RUN*). Two routes beside each other, neither chosen here: the publisher flushing before its rename, as `replace.c` does, at a cost per published file to be measured; and every reader of a kept file treating one it cannot read whole as absent and asking again.
