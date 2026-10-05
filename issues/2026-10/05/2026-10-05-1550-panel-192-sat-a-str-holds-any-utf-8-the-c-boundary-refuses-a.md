---
kind: decision
area: none
milestone: none
filed: 2026-10-05
commit: self
github: none
---

# Panel 192 sat: a str holds any UTF-8, the C boundary refuses a NUL, and a string refuses what its reader cannot see

2026-10-05, written at 15:50 by the clock (`date`). Panel 192 sat as a full panel on defects 245, 251 and 283 and panel 191's Q-c and Q-i: the compiler-engineer, the ffi-pragmatist, the spec-warden and the historian, the llm-ergonomist as a blind experiment of 20 sessions (3.9035 USD of the author's 5), and the completeness critic over the briefs and over the reports. Convened by the author's *3a* of 2026-10-04.

**The synthesis**: `docs/panel/192-a-str-holds-any-utf-8-the-c-boundary-refuses-a-nul-and-a-string-refuses-what-its-reader-cannot-see.md`.

**Its resolution, `provisional — author ratification pending`, R1 to R13**:
- **R1, the invariant**: a `str` holds any UTF-8, NUL included; one bit per `str` lets `.cstr()` and `.lease()` abort on a NUL, and `read_file` and `write_file` answer a failure; `HERO_RUNTIME_ABI` 26 to 27.
- **R2 and R3**: a string refuses raw every control but the line end (the tab with a `certain` fix to `\t`), the twelve bidirectional controls, U+2028 and U+2029; a comment the same but the tab; ZWJ, ZWNJ, the variation selectors and the tag characters stay legal.
- **R4**: robustness for the person reading, kept under `check --permissive`.
- **R5**: `\u{hex}` only for what a string refuses raw, never 0 or a surrogate, in one spelling.
- **R6**: `args()` on Windows by a lossless WTF-8 conversion, owed a run on the box.
- **R7**: one runtime printer writing a message by its length and its controls by their code.
- **R8 and R9**: the spec's rec4-r1 amended, paid by a removal; design.md's freeze opened for R5's escape.
- **R10 to R13**: what is refused, the costs restated over the composite, four filings, the landing.

Queued as the decision issue `panel 192`. **Filed from the sitting**: 353 (`args()` on Windows, `blocking`), 354 (`validated_bytes()` on a `[u8]` cut at a zero, `blocking`), 355 (the runtime's printer and `heroes test` writing controls raw, `adjacent`), 356 (no instrument scans the documents for raw invisible characters, `improvement`).
