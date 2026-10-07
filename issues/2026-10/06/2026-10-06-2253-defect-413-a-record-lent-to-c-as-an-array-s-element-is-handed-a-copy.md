---
kind: defect
area: ir
milestone: none
filed: 2026-10-06
commit: c1f87af5c071a6d6bd52d893c02b1a6812f450a8
github: none
---

- [ ] **413 — a record lent to C as an array's element is handed a copy, and a library that knows it by its address refuses it** | zlib's `z_stream` bound `record ZStream tag z_stream_s partial` and lent `@zs[0]`, `zs: [ZStream]`, to `deflateInit_`, `deflateReset` and `deflateEnd`: `check` 0, `run` 0, printing `0 -2 -2`, `Z_STREAM_ERROR` from the second call on, where the same calls on a local `@zs` print `0 0 0`; each call hands C a temporary copied in and out, and zlib keeps its stream's address (the coordinator's re-run of panel 196's ffi-pragmatist's `z_elem.hero` and `z_local.hero` on round b13's compiler at `3710c5a4`, 22:53) | the lowering of an `@` element argument, `selfhost/ir/inout.hero`; the ruling is panel 196's · **class: blocking**

    **Origin:** panel 196's ffi-pragmatist, 2026-10-06 (`docs/panel/196-reports/ffi-pragmatist.md`, *Found beside* 1), reproduced by the coordinator; the evidence is `docs/panel/196-evidence/r413/`. Measured by the seat, not re-run by the coordinator: on Linux arm64 `--sanitize` exits 1 with LeakSanitizer reporting 268,096 bytes in 5 allocations, and `@b.m[0]` of a `u8[32]` field hands C one byte.

    **Class: blocking**, 2026-10-06 (`.claude/rules/verification.md` § Bounded discovery): a wrong value at exit 0, `check` and `run` both silent. Spec § 9 says an `@` argument is *copy in, copy out*, which promises no address; what a lend to C promises about its address is the ruling panel 196 is sitting on (the ffi-pragmatist's veto: a record lent to C is never copied), so the repair waits on its synthesis.

    Repaired at `c1f87af5`, 2026-10-07 (lane b13-land-addr, panel 196's R1), gated by its cases and the compiler's own tests; the net is owed at the batch's close, and as a C-boundary defect it closes after the push's platform legs.
