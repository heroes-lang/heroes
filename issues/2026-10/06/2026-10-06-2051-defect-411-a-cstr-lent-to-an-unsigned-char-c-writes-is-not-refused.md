---
kind: defect
area: emit
milestone: none
filed: 2026-10-06
commit: a3cf55ebb927f5f500257f49d5ee4ebba294b0e0
github: none
---

- [ ] **411 — a `cstr` lent to an `unsigned char *` C writes is not refused** | `SHA256_Final(md: cstr lent, @c: Sha256Ctx)` lent `s.cstr()`, `s` a 32-byte `str` and `t = s` copied before the call: `check` 0, `run` 0, and `t[0]` prints 186, SHA-256("abc")'s first byte, where it held `0` (48): C wrote through a `str` the program shares, and two values § 3 keeps apart moved together; the only sign is two `-Wpointer-sign` warnings in the build's output (the coordinator's re-run of panel 196's critic's `lend_str.hero` on the frozen tree `39935f7c` at 20:51) | `ffi_writable_parameter`, `selfhost/emit/ffi_mutable.hero:119`, which reads clang's *discards qualifiers* and is never shown this case, since clang tells a `const char *` handed to an `unsigned char *` as `-Wpointer-sign` · **class: blocking**

    **Origin:** panel 196's completeness critic, 2026-10-06, first pass over the briefs (`docs/panel/196-reports/completeness-critic-briefs.md` once the sitting is committed), *the neighbouring shape*; reproduced by the coordinator.

    **Class: blocking**, 2026-10-06 (`.claude/rules/verification.md` § Bounded discovery): memory written behind the program's back at exit 0 (design.md §1.12), the shape `ffi_writable_parameter` exists to refuse against `char *`. Measured by the critic beside it, not re-run by the coordinator: `"a".repeat(1)` lent the same way, `run` 0 and `--sanitize -O0` 0 while C writes 32 bytes into a two-byte heap string; the literal `"a"`, `run` 138 (SIGBUS); a one-byte lease, silent.

    Repaired at `a3cf55eb`, 2026-10-06 (lane b13-w411), gated by its cases and the compiler's own tests; the net is owed at the batch's close. Every `cstr` parameter that is not `@` now asks the pointee check whether the header's pointer points at `const` bytes, `__builtin_types_compatible_p(P, __typeof__(*(P)0) const *)` on the header's own type, and `emit/ffi_mutable.hero` reads its marker into `ffi_writable_parameter`, whatever clang calls the call. The shapes beside it held the same cause and stay in the item: `signed char *`, `uint8_t *`, a typedef of `unsigned char *`, `unsigned char md[32]`, `unsigned char *restrict`, `unsigned char *const` and `unsigned char *_Atomic` built at exit 0 and wrote the shared `str` on the base, on this Mac, Linux arm64 and x86-64; a literal was 138 here and 139 on Linux; `char *`, `char *const`, `void *` keep their message byte for byte by the new route. Its cases are twelve `unsupported/fixedbugs-411-*` and one `run/fixedbugs-411-*`, and the `const unsigned char *` family is held by a test in `cli/pointee.hero`.
