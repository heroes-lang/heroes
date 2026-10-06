---
kind: defect
area: runtime
milestone: none
filed: 2026-10-04
commit: 8763d6be547f53e4b27928346b202d851c552d20
github: none
---

- [ ] **245 — a `str` holding a NUL is lent to C by `.cstr()` as it is, so C reads a shorter string than the program holds** | `s = "a<NUL>b"`, a raw NUL in the literal (`check` 0, `build` 0, `s.len()` 3), or `s = read_file(path: "nul.txt").must()` over the bytes `61 00 62`: `strlen(s: s.cstr())` through `extern "string.h"` prints 1 (batch 8's round compiler at `1eb854c3`, this Mac, 2026-10-04, `<scratchpad>/filings-b8/probe/nulc.hero` and `rf.hero`); design.md §4.3 freezes `\0`, `\xNN`, `\u{...}` and octal escapes *because they can produce an interior NUL, which silently truncates every C call and voids §4.20's guarantee that `.cstr()` is free*, and a raw byte and a file each produce one with no escape | `hero_str_cstr` (`runtime/heroes_runtime.h:214`, *free because of the NUL*) · the lexer's string literal (`selfhost/lexer.hero`), which refuses a raw CR (`raw_carriage_return`, panel 066) and no other control byte (defect 251) · `read_file`, `args_checked()` and every other door a `str` comes through · design.md §4.3's escape freeze · **class: systemic**

    **Origin:** the coordinator, 2026-10-04, at the shape beside batch 8's NUL refusal in a group head (`unwritable_name`'s *a NUL byte*), which refuses the NUL there and nowhere else.

    **Class: systemic**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): a wrong value at the C boundary, `blocking`'s, whose remedy needs a ruling no rule reaches: §4.3's freeze rests on no `str` holding a NUL, two doors measured make one, and what `.cstr()` does then (a refusal where a literal holds one, a check at run time, a fallible lending) is a sitting's.

    Repaired at `6b33db23`, 2026-10-05, gated by its cases and the compiler's own tests; the net is owed at the batch's close, and the platform legs before it closes (panel 192's R13).

    Repaired at `8763d6be` too, 2026-10-05: three `check` cases that named the prelude's old file doors (`extern-across-modules-library` and two of defect 138's) follow what it binds now; `6b33db23`'s gate had not run `check`.
