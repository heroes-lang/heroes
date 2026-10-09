---
kind: defect
area: emit
milestone: none
filed: 2026-10-09
commit: ce2bf93181fcfcdd873b4f23fe422b4ac6efc0e9
github: none
---

- [x] **517 — `ffi_field_type` names the type the program wrote and never the header's width or sign** | *is not `u32`* and nothing of what the header has: the gap an author met when SDL's `type` and `scancode` were `int` on Windows and `unsigned int` on macOS and Debian (lane b15-box) | the field-type diagnostic of `selfhost/emit/` · defect 447 · **class: adjacent**

    **Origin:** filed by the coordinator at 08:39 on 2026-10-09 from lane b15-box's final report; the lane's measurement, not re-run by the coordinator.

    **Class: adjacent**, 2026-10-09 (`.claude/rules/verification.md` § Bounded discovery): a true message less exact than it could be.

    Repaired at `ce2bf931`, 2026-10-09 (lane b16-compiler), gated by its cases and the compiler's own tests; the net is owed at the batch's close, and as a C-boundary item its case is owed the push's platform legs. A refused round asks clang each group field's type and, through an array sized by the field, its width and sign (`emit/field_asked.hero`), and the message says them beside the type written, an enumeration's with a note that the answer is this machine's, and offers the type at the header's own width and sign as a guess (`emit/ffi_field_told.hero`). Run by hand: `enum { 1, 2 }` is unsigned 32 on this Mac and Linux arm64 and signed 32 on the Windows box, the SDL gap itself, and the new case's questions answered alike on all three. One new `unsupported` case, red on the base; two expectations moved, each read; the floor rises to 188.

## The repair

Repaired at `ce2bf931`, 2026-10-09 (lane b16-compiler), gated by its cases and the compiler's own tests; the net is owed at the batch's close, and as a C-boundary item its case is owed the push's platform legs. A refused round asks clang each group field's type and, through an array sized by the field, its width and sign (`emit/field_asked.hero`), and the message says them beside the type written, an enumeration's with a note that the answer is this machine's, and offers the type at the header's own width and sign as a guess (`emit/ffi_field_told.hero`). Run by hand: `enum { 1, 2 }` is unsigned 32 on this Mac and Linux arm64 and signed 32 on the Windows box, the SDL gap itself, and the new case's questions answered alike on all three. One new `unsupported` case, red on the base; two expectations moved, each read; the floor rises to 188.

**Closed 2026-10-09** with batch 16 (lanes b16-emit, b16-runtime, b16-land199, b15-parse, b16-tools, b16-compiler, b16-land198 and b16-misc, merged into one round tree made from the trunk), its closing gate run on the round at `164e699a`: the seed regenerated over two generations, the runtime's ABI staying at 29, 46,097,621 bytes, SHA-256 beginning `d3bf3451eae3f3e8`, its fixpoint by `cmp`; the compiler's own tests 1,531, all passed; the net's own tests 322, all passed; the full net 7,237 passed over 29 suites, 0 failed, `run` taken one harness process per case and its one red the skip ratio of a one-case run (defect 437's case, which binds `sys/prctl.h`, skipped on this Mac as in every whole run). It is the first batch under the optimistic chain (author instruction 2026-10-09, `.claude/rules/verification.md` § The optimistic chain): the census and panel 187's R2 run after the push beside the CI, no platform leg ran before it, and a defect at the C boundary closes here, a CI leg red on its case filing a new `blocking` defect naming it. A trial of the same gate on the round at `2dbbf7e6`, before lane b16-misc's four repairs, read the full net 7,226 and 0, the census's every move attributed and R2's one finding read as defect 457's rule speaking.
