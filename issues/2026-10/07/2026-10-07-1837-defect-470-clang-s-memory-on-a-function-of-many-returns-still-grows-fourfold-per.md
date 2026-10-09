---
kind: defect
area: cli
milestone: none
filed: 2026-10-07
commit: ac79639f6e0065139b9cd9b61aab8f9ec51b0a85
github: none
---

- [ ] **470 — clang's memory on a function of many returns still grows fourfold per doubling** | under `-g` and under line tables alike, 3.7 to 4.0 times per doubling of the returns at `-O2` (panel 197's compiler-engineer, 400 and 800 returns); panel 197's (C) lowers the constant only, so defect 322 closing does not record the growth | the emitted C of a function of many returns · defect 322 · panel 197's R7 · **class: improvement**

    **Origin:** filed by the coordinator at 18:37 on 2026-10-07 from panel 197's R7.

    **Class: improvement**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): a growth on extreme shapes, no program wrong.

    Repaired at `ac79639f6e0065139b9cd9b61aab8f9ec51b0a85`, 2026-10-09, gated by its cases and the compiler's own tests; the net is owed at the batch's close. Panel 200's route C: the exit sweep hands a string, array or map slot to the runtime by its address, `hero_{str,array,map}_release_at(&slot)`, and a record, option or case slot to the unit's own release through `hero_slot_escape(&slot)`, the runtime's identity, so the escape survives inlining (`emit/release.hero`, split from `emit/inst.hero`); `HERO_RUNTIME_ABI` 30 at both stamp sites; `guarded_names` and the runtime's guard files take four names, the fourth being the records' route; the premise (the runtime a separate unit, LTO or a unity build would undo it) written beside it with a test that no compile or link word asks for LTO. At `-O2` on this Mac, clang's instructions and peak footprint before and after: strings of 200, 400 and 800 returns 101.9e9 and 260 MB, 507.9e9 and 891 MB, 2,486.9e9 and 3.46 GB, to 7.38e9 and 39 MB, 17.9e9 and 55 MB, 37.6e9 and 96 MB; records 112.9e9 and 254 MB, 587.9e9 and 889 MB, 3,090.5e9 and 3.40 GB, to 11.1e9 and 72 MB, 27.6e9 and 161 MB, 65.6e9 and 512 MB; arrays of 400 returns 98.3e9 and 262 MB to 6.54e9 and 46 MB. The compiler checking itself, built from this source both ways, two runs each: at `-O0` 73.05e9 and 73.00e9 to 74.97e9 and 75.00e9 instructions (+2.6 to +2.7%), at `-O2` 21.19e9 and 21.27e9 to 22.01e9 and 21.99e9 (+3.4 to +3.9%), under the sitting's 5% condition; which share the records' escape costs is unrun. Every blessed emission moved (522, by five kinds of line, read by kind) and the ten `emit` expectations; emission 1,082 and 0, emit 11 and 0, wholes 524 and 0, descriptors 524 and 0, runtime 8 and 0, determinism 462 and 0, run whole 424 and 0, cache alone 7 and 0, the compiler's own tests 1,534 passed. The seed stamps ABI 29 and does not compile against this runtime (measured), so the batch gate builds the round's compiler first against the base's runtime.
