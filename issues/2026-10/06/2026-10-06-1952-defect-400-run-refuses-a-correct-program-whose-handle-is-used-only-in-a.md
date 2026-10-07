---
kind: defect
area: cli
milestone: none
filed: 2026-10-06
commit: 9be5f8b3c8d23575dc4b87d85208ca3d730d76d0
github: none
---

- [x] **400 — `run` refuses a correct program whose handle is used only in a test** | `fwd.h` holding `struct opaque;`, `extern "fwd.h"` / `record Opaque tag opaque`, `main` printing 1, and `test "t"` binding `h: Opaque = nullptr`: `heroes run` exits 1 with *`fwd.h` declares no `opaque` — clang read the header and could not find it*, while `heroes test` passes (1 test, all passed); without the test block the program runs (the trunk's compiler at `6a03c488`, built from its seed at 19:52 and run by the coordinator at 19:52 on 2026-10-06) | the header's question asked of a record a `run` build does not reach, `selfhost/cli/` (the pointee and layout checks) · **class: blocking**

    **Origin:** lane b13-bs, 2026-10-06 (its report, *found beside* 1), met while landing panel 194; reproduced by the coordinator on the trunk's compiler.

    **Class: blocking**, 2026-10-06 (`.claude/rules/verification.md` § Bounded discovery): a correct program refused, with a false message: the header does declare `struct opaque`. Into batch 13.

    Repaired at `9be5f8b3`, 2026-10-06 (lane b13-run400), gated by its cases and the compiler's own tests; the net is owed at the batch's close.

## The repair

Repaired at `9be5f8b3`, 2026-10-06 (lane b13-run400), gated by its cases and the compiler's own tests; the net is owed at the batch's close.

**Gated 2026-10-07** with batch 13 (lanes b13-c382, b13-bs, b13-unit, b13-front, b13-run400, b13-zero401, b13-gen402, b13-fixed405, b13-tmpl407, b13-w411, b13-land-addr and b13-land-buf, and panel 196's sitting, merged into one round tree made from the trunk at `74bc6308`), its closing gate run on the round's head `06cf8cd4`: the seed regenerated over two generations from the trunk's compiler, the runtime's ABI moving from 27 to 29, 40,044,821 bytes, SHA-256 beginning `2ba1ae96d684aa2c`, its fixpoint by `cmp`; the compiler's own tests 1,357, all passed; the net's own tests 289, all passed; the full net 6,581 passed over 29 suites, 0 failed but `records` 27 and 1 on the ROADMAP's count, defect 442 being filed during the gate and recounted in the closing commit; the census of `check` over 2,892 tracked files moving 180 verdicts and of `--emit-c` over 1,305 moving 213 refusals, every move attributed (123 and 85 to `rest: zero`, the rest to panel 196's R5, to the fixed array's wording of 399, 405, 408, 423 and 432, to the reproducers of 397 and 398 now refused, to defect 092's route C refusing the 14 probes of its own fault, and to a cache path in 19 warnings); panel 187's R2 replay of 13,594 mutants and 16,041 pairs against the trunk's run, 0 findings, 23 mutants moving from several messages to one (defect 391's); Linux arm64 on the same tree, the compiler's own tests 1,357 and 20 suites, every one 0 failed.

**Closed 2026-10-07** after the push's platform legs, a defect at the C boundary closing only after them (`.claude/rules/verification.md` § The batch), the last Windows leg sent to the CI by the author at 10:19 (`issues/2026-10/07/2026-10-07-1019-the-author-sends-batch-13-s-last-windows-leg-to-the-ci.md`): the CI's run 37603092655 on the pushed `34f95b71`, every job a success, read at 13:43; Linux x86-64 the compiler's own tests 1,357, the full net 6,536 passed and the net's own tests 289, 0 failed; Linux arm64 1,357, 6,536 and 289; Darwin arm64 1,357, 6,571 and 289; Windows x86-64 1,357, 6,422 and 289. The cases whose header Windows lacks were skipped there by name and run where the header is: defect 092's `getsockopt`, 396's POSIX and SHA-256 buffers and 436's stopped child on both Linux legs and Darwin, 413's zlib case on the three and its libuv case on Darwin alone, `pkg-config` on both Linux legs not knowing `libuv`.
