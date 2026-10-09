---
kind: defect
area: cli
milestone: none
filed: 2026-10-07
commit: 5b60a02977d689a5c580afc7459f7fd69a32eaad
github: none
---

- [x] **484 — the build cache hashes every object's bytes in Heroes compiled at `-O0`** | defect 443's seal check costs about 1,158 instructions per byte of object, +13.4% on a warm `print(1)`, +14.6% on `examples/interpreter`, +4.0% on the compiler's own build; a runtime C function keying a file's bytes would cut it several times (lane b14-cli's inference, unmeasured) | `selfhost/cli/served.hero` (batch 14's round, unmerged on 2026-10-07) · defect 443 · **class: improvement**

    **Origin:** filed by the coordinator at 18:37 on 2026-10-07 from lane b14-cli's final report; the lane's measurement, not re-run by the coordinator.

    **Class: improvement**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): a cost the robust check pays; robustness kept, the price measurable.

    Repaired at `5b60a029`, 2026-10-09 (lane b15-runtime, batch 16), gated by its cases and the compiler's own tests; the net is owed at the batch's close. `key_of` asks `hero_file_key` (`runtime/parts/key.c`, declared in `runtime/hero_compiler.h`), which makes the same sixteen characters over the file's bytes where they lie, and the shown text the Heroes route built and hashed at `-O0` is not built. Measured on this Mac, instructions retired by a warm build: `print(1)` 2.53e9 to 0.55e9, `examples/interpreter` 8.76e9 to 5.99e9, the compiler 418e9 to 379e9, so the inference the item was filed on (*several times*) holds for a small program and not for a large one, where the key was never most of the build. Case `run/fixedbugs-484`, red on the base and green here and on the Windows box at every level the run suite builds; a test holds `key_of` to `keyed` over ten files of every shape.

## The repair

Repaired at `5b60a029`, 2026-10-09 (lane b15-runtime, batch 16), gated by its cases and the compiler's own tests; the net is owed at the batch's close. `key_of` asks `hero_file_key` (`runtime/parts/key.c`, declared in `runtime/hero_compiler.h`), which makes the same sixteen characters over the file's bytes where they lie, and the shown text the Heroes route built and hashed at `-O0` is not built. Measured on this Mac, instructions retired by a warm build: `print(1)` 2.53e9 to 0.55e9, `examples/interpreter` 8.76e9 to 5.99e9, the compiler 418e9 to 379e9, so the inference the item was filed on (*several times*) holds for a small program and not for a large one, where the key was never most of the build. Case `run/fixedbugs-484`, red on the base and green here and on the Windows box at every level the run suite builds; a test holds `key_of` to `keyed` over ten files of every shape.

**Closed 2026-10-09** with batch 16 (lanes b16-emit, b16-runtime, b16-land199, b15-parse, b16-tools, b16-compiler, b16-land198 and b16-misc, merged into one round tree made from the trunk), its closing gate run on the round at `164e699a`: the seed regenerated over two generations, the runtime's ABI staying at 29, 46,097,621 bytes, SHA-256 beginning `d3bf3451eae3f3e8`, its fixpoint by `cmp`; the compiler's own tests 1,531, all passed; the net's own tests 322, all passed; the full net 7,237 passed over 29 suites, 0 failed, `run` taken one harness process per case and its one red the skip ratio of a one-case run (defect 437's case, which binds `sys/prctl.h`, skipped on this Mac as in every whole run). It is the first batch under the optimistic chain (author instruction 2026-10-09, `.claude/rules/verification.md` § The optimistic chain): the census and panel 187's R2 run after the push beside the CI, no platform leg ran before it, and a defect at the C boundary closes here, a CI leg red on its case filing a new `blocking` defect naming it. A trial of the same gate on the round at `2dbbf7e6`, before lane b16-misc's four repairs, read the full net 7,226 and 0, the census's every move attributed and R2's one finding read as defect 457's rule speaking.
