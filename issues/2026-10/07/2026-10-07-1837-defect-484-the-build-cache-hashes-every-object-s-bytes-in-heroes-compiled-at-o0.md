---
kind: defect
area: cli
milestone: none
filed: 2026-10-07
commit: 5b60a02977d689a5c580afc7459f7fd69a32eaad
github: none
---

- [ ] **484 — the build cache hashes every object's bytes in Heroes compiled at `-O0`** | defect 443's seal check costs about 1,158 instructions per byte of object, +13.4% on a warm `print(1)`, +14.6% on `examples/interpreter`, +4.0% on the compiler's own build; a runtime C function keying a file's bytes would cut it several times (lane b14-cli's inference, unmeasured) | `selfhost/cli/served.hero` (batch 14's round, unmerged on 2026-10-07) · defect 443 · **class: improvement**

    **Origin:** filed by the coordinator at 18:37 on 2026-10-07 from lane b14-cli's final report; the lane's measurement, not re-run by the coordinator.

    **Class: improvement**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): a cost the robust check pays; robustness kept, the price measurable.

    Repaired at `5b60a029`, 2026-10-09 (lane b15-runtime, batch 16), gated by its cases and the compiler's own tests; the net is owed at the batch's close. `key_of` asks `hero_file_key` (`runtime/parts/key.c`, declared in `runtime/hero_compiler.h`), which makes the same sixteen characters over the file's bytes where they lie, and the shown text the Heroes route built and hashed at `-O0` is not built. Measured on this Mac, instructions retired by a warm build: `print(1)` 2.53e9 to 0.55e9, `examples/interpreter` 8.76e9 to 5.99e9, the compiler 418e9 to 379e9, so the inference the item was filed on (*several times*) holds for a small program and not for a large one, where the key was never most of the build. Case `run/fixedbugs-484`, red on the base and green here and on the Windows box at every level the run suite builds; a test holds `key_of` to `keyed` over ten files of every shape.
