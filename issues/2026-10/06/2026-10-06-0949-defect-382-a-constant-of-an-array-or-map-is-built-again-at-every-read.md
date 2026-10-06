---
kind: defect
area: ir
milestone: none
filed: 2026-10-06
commit: none
github: none
---

- [ ] **382 — a constant of an array or map is built again at every read** | a `constant K: [i64]` of 32 elements read 1,000,000 times in a loop, `K[at % 32]`, retires 56,768,740,722 instructions; the same loop reading a name bound once to `K` above it, 116,805,017, 486 times fewer. `--dump-ir` shows `call heroes K()` inside the loop's body, and the emitted `h_k_K(void)` builds the array from its literal at each call (the trunk's compiler at `0f48f9f9`, measured by the coordinator at 09:46 on 2026-10-06). 66 constants of an array or map type stand in `selfhost/` and `tests/harness/` (`grep -E '^constant [A-Z_0-9]+: (\[|\{)'`), among them `shown_char.hero`'s `UNSEEN`, `DEFAULT_IGNORABLE` and `BIDI_CONTROL` and `rename_fit.hero`'s `RENAMES`; how many of them are read in a loop is unrun | `selfhost/ir/lower.hero:77`, a constant lowered as a function of no arguments, and every read of it a call · the emitter's constant function · **class: adjacent**

    **Origin:** lane b12-cli12, 2026-10-06 (its final report, *adjacent or improvement*); reproduced and filed by the coordinator at 09:49.

    **Class: adjacent**, 2026-10-06 (`.claude/rules/verification.md` § Bounded discovery): a cost found beside the work, no value wrong; the specification states no cost for a constant, so no sentence of it is false, and defect 372 was classed the same for a cost of the same kind. A constant's body holds only literals and other constants (the author's option a of 2026-08-12), so building it once cannot change what a program computes.

    Its no-state route landed at `f7576a01`, 2026-10-06 (lane b12-ir12): an array literal is built in the one block `hero_array_new` sizes to it, each element copied in where it lies, where it was one copying push per element, n+1 blocks and n(n+1)/2 copies; the reproducer reads 4,598,758,203 instructions at -O0 where it read 56,849,933,897, its hoisted twin 117,634,662. The read still builds the array, so the item stays open: the two routes that stop that, a static block no count moves for a constant whose body is literals (`HERO_STR_STATIC`'s shape for an array) and a value built once per thread, written by hand in the reproducer's C at -O0 at 142,668,198 and 195,594,982 instructions, each need the runtime, and are with the coordinator for the panel's soundness lane. Of the 66 constants of an array or map in `selfhost/` and `tests/harness/`, every body is literals, 61 `[str]` and 5 `[i64]`, and 19 are read lexically inside a loop, one of them in `selfhost/` (`clang_told.hero`'s `LEVELS`).
