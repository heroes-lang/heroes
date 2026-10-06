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
