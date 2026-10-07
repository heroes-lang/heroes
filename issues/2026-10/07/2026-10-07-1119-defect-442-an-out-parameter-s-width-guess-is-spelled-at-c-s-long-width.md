---
kind: defect
area: emit
milestone: none
filed: 2026-10-07
commit: 06cf8cd4f4d5e1da46d20ac3b7d7678a83b4cc6d
github: none
---

- [ ] **442 — an out-parameter's width guess is spelled at C's `long` width, false on Windows** | an `@n: i32` against a header's `size_t *`, refused `ffi_parameter_type` correctly, told on the Windows box *`size_t` is the platform's pointer width: 32 bits on this machine* with the guess `@n: u32`, where `size_t` is 64 bits there: the reader handed C's `long` width to the table that spells a type, and `long` is 32 bits on Windows (the box's pre-leg of batch 13 at `0dd1cfd8`, case `unsupported/fixedbugs-411-a-function-pointer-s-out-parameter-is-held-to-its-width`, read by the coordinator at 10:43) | `selfhost/emit/ffi_pointee.hero`, the width the guess is spelled at · **class: blocking**

    **Origin:** filed by the coordinator at 11:19 on 2026-10-07, from the Windows box's pre-leg of batch 13's round; traced by lane b13-w411, which found it predates defect 411's function pointer case, the first case to bind a `size_t *` out-parameter on the box.

    **Class: blocking**, 2026-10-07 (`.claude/rules/verification.md` § Bounded discovery): a false message, and a guess fix that writes the wrong width.

    Repaired at `06cf8cd4`, 2026-10-07 (lane b13-w411): the width question is five assertions, one per size (1, 2, 4, 8 and 16 bytes), each failing only where the binding is wrong and the header's type has that size, its message naming the width in bits, so the header's actual width travels with the refusal and the guess is spelled at it (`emit/width_line.hero`, new); on the Windows box the case matches its expectation byte for byte, `unsupported fixedbugs-411` 15 and 0, `fixedbugs-163` 1 and 0; on this Mac `unsupported` 188, `run` 372, the compiler's own tests 1,357, all passed (the lane's); a C-boundary defect, so it closes after the push's CI Windows leg; filed and its card filled by the coordinator.
