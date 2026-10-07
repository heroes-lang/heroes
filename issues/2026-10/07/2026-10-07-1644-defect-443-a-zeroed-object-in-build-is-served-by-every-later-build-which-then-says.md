---
kind: defect
area: cli
milestone: none
filed: 2026-10-07
commit: 667d9039f962b54ee4b0ed0661e9ea43b7b3a9e6
github: none
---

- [ ] **443 — a zeroed object in `build/` is served by every later build, which then says clang refused the generated C** | zero `build/runtime-*.o` or a `build/tu-*/x-*.o` at its length and build again: every later build exits 2 with `ld: unknown file type` and then *error: clang refused the generated C*, which is false, until `build/` is deleted by hand; the recovery half of defect 357's object row, which was repaired only by flushing before the rename (lane b14-cli's measurement, reproducer in `<scratchpad>/batch14/cli/p435/shapes`, not re-run by the coordinator) | `selfhost/cli/`, the build cache's reuse of an object · defect 357 · **class: blocking**

    **Origin:** filed by the coordinator at 16:44 on 2026-10-07 from lane b14-cli's final report (*found beside* 435).

    **Class: blocking**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): a false message at exit 2, lasting until the cache is deleted by hand.

    Repaired at `667d9039`, 2026-10-07 (lane b14-cli), gated by its cases and the compiler's own tests; the net is owed at the batch's close. Each cached object, the runtime's and a module's, carries a seal beside it, the key of the bytes clang wrote (`cli/served.hero`), and is held to it before it is served: one zeroed at its length, emptied, cut at half or another unit's object at its name, each exit 2 and *clang refused the generated C* on the base, is compiled again and said, exit 0, and a block of 4 KiB zeroed in the middle, which the base linked and ran, the same; a link the linker refuses ends *the linker refused the objects clang compiled*; the check costs +13.4% instructions on a warm `print(1)`, +14.6% on `examples/interpreter` and +4.0% on this compiler's own build.
