---
kind: defect
area: runtime
milestone: none
filed: 2026-10-05
commit: none
github: none
---

- [ ] **354 — `[97, 0, 98].validated_bytes()` answers a one-byte `str`, cutting at the zero with no word** | a `[u8]` holding a zero byte answers `validated_bytes()` with the bytes before the zero alone, `len` 1 for three bytes, no failure (panel 192's ffi-pragmatist and coordinator, this Mac, 2026-10-05; `<scratchpad>/192-facts/repair/esc.hero`); the spec gives `validated_bytes()` to *a field of bytes*, read to its first zero, and says nothing of a `[u8]`, which the compiler accepts | `hero_str_try_from_bytes` (`runtime/parts/str.c:379`, its `memchr`) · `spec/heroes-spec.md:384` · panel 192's R1, under which a `str` holds a NUL · **class: blocking**

    **Origin:** panel 192's ffi-pragmatist, 2026-10-05 (its report, *a related defect to file separately*); filed by the synthesis's R12.

    **Class: blocking**, 2026-10-05 (`.claude/rules/verification.md` § Bounded discovery): a wrong value under panel 192's R1, the bytes after the zero dropped in silence.
