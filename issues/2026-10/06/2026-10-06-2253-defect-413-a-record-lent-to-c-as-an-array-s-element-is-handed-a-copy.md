---
kind: defect
area: ir
milestone: none
filed: 2026-10-06
commit: c1f87af5c071a6d6bd52d893c02b1a6812f450a8
github: none
---

- [x] **413 — a record lent to C as an array's element is handed a copy, and a library that knows it by its address refuses it** | zlib's `z_stream` bound `record ZStream tag z_stream_s partial` and lent `@zs[0]`, `zs: [ZStream]`, to `deflateInit_`, `deflateReset` and `deflateEnd`: `check` 0, `run` 0, printing `0 -2 -2`, `Z_STREAM_ERROR` from the second call on, where the same calls on a local `@zs` print `0 0 0`; each call hands C a temporary copied in and out, and zlib keeps its stream's address (the coordinator's re-run of panel 196's ffi-pragmatist's `z_elem.hero` and `z_local.hero` on round b13's compiler at `3710c5a4`, 22:53) | the lowering of an `@` element argument, `selfhost/ir/inout.hero`; the ruling is panel 196's · **class: blocking**

    **Origin:** panel 196's ffi-pragmatist, 2026-10-06 (`docs/panel/196-reports/ffi-pragmatist.md`, *Found beside* 1), reproduced by the coordinator; the evidence is `docs/panel/196-evidence/r413/`. Measured by the seat, not re-run by the coordinator: on Linux arm64 `--sanitize` exits 1 with LeakSanitizer reporting 268,096 bytes in 5 allocations, and `@b.m[0]` of a `u8[32]` field hands C one byte.

    **Class: blocking**, 2026-10-06 (`.claude/rules/verification.md` § Bounded discovery): a wrong value at exit 0, `check` and `run` both silent. Spec § 9 says an `@` argument is *copy in, copy out*, which promises no address; what a lend to C promises about its address is the ruling panel 196 is sitting on (the ffi-pragmatist's veto: a record lent to C is never copied), so the repair waits on its synthesis.

    Repaired at `c1f87af5`, 2026-10-07 (lane b13-land-addr, panel 196's R1), gated by its cases and the compiler's own tests; the net is owed at the batch's close, and as a C-boundary defect it closes after the push's platform legs.

## The repair

Repaired at `c1f87af5`, 2026-10-07 (lane b13-land-addr, panel 196's R1), gated by its cases and the compiler's own tests; the net is owed at the batch's close, and as a C-boundary defect it closes after the push's platform legs.

**Gated 2026-10-07** with batch 13 (lanes b13-c382, b13-bs, b13-unit, b13-front, b13-run400, b13-zero401, b13-gen402, b13-fixed405, b13-tmpl407, b13-w411, b13-land-addr and b13-land-buf, and panel 196's sitting, merged into one round tree made from the trunk at `74bc6308`), its closing gate run on the round's head `06cf8cd4`: the seed regenerated over two generations from the trunk's compiler, the runtime's ABI moving from 27 to 29, 40,044,821 bytes, SHA-256 beginning `2ba1ae96d684aa2c`, its fixpoint by `cmp`; the compiler's own tests 1,357, all passed; the net's own tests 289, all passed; the full net 6,581 passed over 29 suites, 0 failed but `records` 27 and 1 on the ROADMAP's count, defect 442 being filed during the gate and recounted in the closing commit; the census of `check` over 2,892 tracked files moving 180 verdicts and of `--emit-c` over 1,305 moving 213 refusals, every move attributed (123 and 85 to `rest: zero`, the rest to panel 196's R5, to the fixed array's wording of 399, 405, 408, 423 and 432, to the reproducers of 397 and 398 now refused, to defect 092's route C refusing the 14 probes of its own fault, and to a cache path in 19 warnings); panel 187's R2 replay of 13,594 mutants and 16,041 pairs against the trunk's run, 0 findings, 23 mutants moving from several messages to one (defect 391's); Linux arm64 on the same tree, the compiler's own tests 1,357 and 20 suites, every one 0 failed.

**Closed 2026-10-07** after the push's platform legs, a defect at the C boundary closing only after them (`.claude/rules/verification.md` § The batch), the last Windows leg sent to the CI by the author at 10:19 (`issues/2026-10/07/2026-10-07-1019-the-author-sends-batch-13-s-last-windows-leg-to-the-ci.md`): the CI's run 37603092655 on the pushed `34f95b71`, every job a success, read at 13:43; Linux x86-64 the compiler's own tests 1,357, the full net 6,536 passed and the net's own tests 289, 0 failed; Linux arm64 1,357, 6,536 and 289; Darwin arm64 1,357, 6,571 and 289; Windows x86-64 1,357, 6,422 and 289. The cases whose header Windows lacks were skipped there by name and run where the header is: defect 092's `getsockopt`, 396's POSIX and SHA-256 buffers and 436's stopped child on both Linux legs and Darwin, 413's zlib case on the three and its libuv case on Darwin alone, `pkg-config` on both Linux legs not knowing `libuv`.
