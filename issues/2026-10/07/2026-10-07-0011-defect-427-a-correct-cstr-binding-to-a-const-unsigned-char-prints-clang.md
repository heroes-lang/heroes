---
kind: defect
area: emit
milestone: none
filed: 2026-10-07
commit: 7a4090f2f1286c66462196a2954268c23773386f
github: none
---

- [x] **427 — a correct `cstr` binding to a `const unsigned char *` prints clang warnings** | every `cstr` lent to `const unsigned char *`, `const signed char *` or `const uint8_t *` builds and runs and prints `-Wpointer-sign` warnings, 14 for the w411 run case's first draft, on the base too | the cast the emitter writes at a `cstr` lend, `selfhost/emit/` · **class: blocking**

    **Origin:** filed by the coordinator at 00:11 on 2026-10-07, from lane b13-w411's report of the evening before (*found beside*); the lane's measurement, not re-run by the coordinator.

    **Class: blocking**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): a clang warning on a correct program is `blocking` by the class's own list.

    Repaired at `7a4090f2`, 2026-10-07 (lane b13-w411): a `cstr` passed to a `const` pointer spelled other than `const char *` or `const void *` is cast to the header's own type at the call and in the probe, 411's refusal unchanged since the pointee check runs before any unit compiles; a non-byte pointee told `ffi_parameter_type`; cases `run/fixedbugs-427-*` and two refused shapes; gated by its cases, the compiler's own tests, every form whole on this Mac and its cases on Linux arm64 (the lane's); a C-boundary defect, so it closes after the batch's platform legs; the card filled by the coordinator.

## The repair

Repaired at `7a4090f2`, 2026-10-07 (lane b13-w411): a `cstr` passed to a `const` pointer spelled other than `const char *` or `const void *` is cast to the header's own type at the call and in the probe, 411's refusal unchanged since the pointee check runs before any unit compiles; a non-byte pointee told `ffi_parameter_type`; cases `run/fixedbugs-427-*` and two refused shapes; gated by its cases, the compiler's own tests, every form whole on this Mac and its cases on Linux arm64 (the lane's); a C-boundary defect, so it closes after the batch's platform legs; the card filled by the coordinator.

**Gated 2026-10-07** with batch 13 (lanes b13-c382, b13-bs, b13-unit, b13-front, b13-run400, b13-zero401, b13-gen402, b13-fixed405, b13-tmpl407, b13-w411, b13-land-addr and b13-land-buf, and panel 196's sitting, merged into one round tree made from the trunk at `74bc6308`), its closing gate run on the round's head `06cf8cd4`: the seed regenerated over two generations from the trunk's compiler, the runtime's ABI moving from 27 to 29, 40,044,821 bytes, SHA-256 beginning `2ba1ae96d684aa2c`, its fixpoint by `cmp`; the compiler's own tests 1,357, all passed; the net's own tests 289, all passed; the full net 6,581 passed over 29 suites, 0 failed but `records` 27 and 1 on the ROADMAP's count, defect 442 being filed during the gate and recounted in the closing commit; the census of `check` over 2,892 tracked files moving 180 verdicts and of `--emit-c` over 1,305 moving 213 refusals, every move attributed (123 and 85 to `rest: zero`, the rest to panel 196's R5, to the fixed array's wording of 399, 405, 408, 423 and 432, to the reproducers of 397 and 398 now refused, to defect 092's route C refusing the 14 probes of its own fault, and to a cache path in 19 warnings); panel 187's R2 replay of 13,594 mutants and 16,041 pairs against the trunk's run, 0 findings, 23 mutants moving from several messages to one (defect 391's); Linux arm64 on the same tree, the compiler's own tests 1,357 and 20 suites, every one 0 failed.

**Closed 2026-10-07** after the push's platform legs, a defect at the C boundary closing only after them (`.claude/rules/verification.md` § The batch), the last Windows leg sent to the CI by the author at 10:19 (`issues/2026-10/07/2026-10-07-1019-the-author-sends-batch-13-s-last-windows-leg-to-the-ci.md`): the CI's run 37603092655 on the pushed `34f95b71`, every job a success, read at 13:43; Linux x86-64 the compiler's own tests 1,357, the full net 6,536 passed and the net's own tests 289, 0 failed; Linux arm64 1,357, 6,536 and 289; Darwin arm64 1,357, 6,571 and 289; Windows x86-64 1,357, 6,422 and 289. The cases whose header Windows lacks were skipped there by name and run where the header is: defect 092's `getsockopt`, 396's POSIX and SHA-256 buffers and 436's stopped child on both Linux legs and Darwin, 413's zlib case on the three and its libuv case on Darwin alone, `pkg-config` on both Linux legs not knowing `libuv`.
