---
kind: defect
area: compiler
milestone: none
filed: 2026-10-04
commit: 36c6944730cd51e5838080878cfc9f596b321217
github: none
---

- [ ] **283 — bidirectional control characters and U+2028 are accepted in comments and strings, so a line can show one order and mean another** | `function main()` over a comment holding U+202E (RIGHT-TO-LEFT OVERRIDE, Trojan Source's character) and `print("ab<U+202E>cd")`: `check` exit 0, and the program prints the override's three bytes, `e2 80 ae`, between `ab` and `cd` (batch 8's round compiler, this Mac, 2026-10-04, `<scratchpad>/filings-b8/probe/c281/bidi.hero`); the spec-warden measured the same for the other bidirectional controls and U+2028 (`docs/panel/189-reports/spec-warden.md`, *Byproducts*) | the lexer's comment and string readers (`selfhost/lexer.hero`) · spec § 1 (*comments and strings are UTF-8*) · panel 188, which refused these characters in a header's string only · **class: systemic**

    **Origin:** panel 189's spec-warden (*not this sitting's question*), carried by the critic's second pass as a shape no sitting has ruled on; filed by the sitting's R12 and run by the coordinator.

    **Class: systemic**, 2026-10-04 (`.claude/rules/verification.md` § Bounded discovery): a program whose text shows one order and compiles another, a truth `blocking` names, whose remedy (a refusal in comments, in strings, or a word the reader sees) is a ruling no rule reaches: a sitting's, since spec § 1 admits every character there.

    Repaired at `36c69447`, 2026-10-05, on panel 192's R2 and R3 (the refusal of `462b4a2d` widened, the escape of `aa150c8c` its fix's spelling), gated by its cases and the compiler's own tests; the net is owed at the batch's close.
