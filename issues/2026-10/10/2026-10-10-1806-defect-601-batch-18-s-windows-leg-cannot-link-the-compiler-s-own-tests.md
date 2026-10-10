---
kind: defect
area: cli
milestone: none
filed: 2026-10-10
commit: bf1eb16bc7f0d456689864bf7f7954c5cb585dff
github: none
---

- [x] **601 — batch 18's Windows leg cannot link the compiler's own tests, OS error 87** | batch 18's push (`03bb5cb0`), run 38064729342: the Windows x86-64 leg's step *The compiler's own tests* exits 2 with *error: clang could not be started, so linking did not run; the operating system's own reason is 87.* (read through `gh run view --log-failed` at 18:05); every step before it green, the other three legs cancelled by the next push's run; defect 583's shape (the link's command line past Windows' limit), reached now by `heroes test selfhost/main.hero`, which builds by module since defect 560's repair | `selfhost/cli/link.hero`; defect 583's repair in batch 19 (`bf1eb16b`, the link's words through a response file) · **class: blocking**

    **Origin:** filed by the coordinator at 18:06 on 2026-10-10 from the CI's Windows leg of batch 18's push, as `.claude/rules/verification.md` § The optimistic chain item 5 asks: a red CI leg is a new `blocking` defect.

    **Class: blocking**, 2026-10-10 (`.claude/rules/verification.md` § Bounded discovery): a red CI.

    Repaired at `bf1eb16b`, 2026-10-10 (lane b19-link, defect 583's repair), gated by its cases and the compiler's own tests; the net is owed at the batch's close. `heroes test` reaches the link by one path in batch 19's round, `cli/rounds.hero` to `assemble.per_module` to `link.link_objects`, which hands every word but the binary's name to clang through `response_file.for_link` (read in the round's code by the coordinator at 18:08, not run on Windows: the box did not answer lane b19-link at 15:05). The compiler's own self-build links 580 objects through a 35,165-byte file on this Mac and Linux arm64 (lane b19-link's measurement). The CI's Windows leg after batch 19's push judges it; red there files a new `blocking` defect.

## The repair

Repaired at `bf1eb16b`, 2026-10-10 (lane b19-link, defect 583's repair), gated by its cases and the compiler's own tests; the net is owed at the batch's close. `heroes test` reaches the link by one path in batch 19's round, `cli/rounds.hero` to `assemble.per_module` to `link.link_objects`, which hands every word but the binary's name to clang through `response_file.for_link` (read in the round's code by the coordinator at 18:08, not run on Windows: the box did not answer lane b19-link at 15:05). The compiler's own self-build links 580 objects through a 35,165-byte file on this Mac and Linux arm64 (lane b19-link's measurement). The CI's Windows leg after batch 19's push judges it; red there files a new `blocking` defect.

**Closed 2026-10-10** with batch 19 (lanes b19-link, b19-dep and b19-pack, merged into the round `lane-round-b19` made from batch 18's closed round, with the trunk), its closing gate run on the round at `0edc5085`: the seed regenerated over two generations, the runtime's ABI at 30, 50,887,297 bytes, SHA-256 beginning `f091d8e3ca299237`, its fixpoint by `cmp`; the compiler's own tests 1,593, all passed; the net's own tests 332, all passed; the full net 8,036 passed over 29 suites, 0 failed, `run` in four shards and `cache` alone after, its one red `spec/anchors` on a module doc of defect 588's repair whose words read as a spec citation, reworded on the same two lines (the fixpoint re-checked by `cmp`) and `spec` 23 and 0 after; six floors told outgrown and raised in the closing commit, `canonical`, `emission`, `lines`, `probe`, `records` and the net's own tests 2, 1,221, 497, 27, 28 and 332, all 0 failed, after it. Lane b19-dep's merge of lanes b19-dep and b19-pack found defect 585's true headline rewritten by defect 563's reader and repaired it in the round (`0071bdac`). A defect at the C boundary closes here under the optimistic chain; the platform its case needs judges it at the CI's legs after the push, a red leg filing a new `blocking` defect naming it.
