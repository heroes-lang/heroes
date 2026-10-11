---
kind: decision
area: check
milestone: none
filed: 2026-10-06
commit: self
github: none
---

- [x] **panel 194** | ratify, amend or overturn R1 to R8 (`rest: zero` on a group record built on panel 186's `build`-side judgement, a union none of whose members is named zeroed by the emitted C itself, no padding promise; C's `char` is `i8` in § 13 and in the `ffi_field_type` note, landing with R1; zero as bytes and a header's initialiser as a constant, defect 392 ruled the binding author's; 091 closed by 097; T and A waiting; R2, D, Z1, a `check`-side R1, R0's second spelling and *nothing* refused; 092's count in C's own unit, an undeclared whole-record lend to `void *` refused, route B refused, no interim; five defects filed) | `docs/panel/194-the-rest-is-zero-where-a-group-s-construction-says-so-and-a-lend-s-count-is-in-c-s-own-unit.md` § The resolution

    **Origin:** panel 194's synthesis, 2026-10-06 from 18:23, on the tree frozen at `7a26a0a6`, convened by the author's instruction of that day to sit panel 178 again on the code of the day (`issues/2026-10/06/2026-10-06-1118-panel-178-reaches-the-trunk-sat-again-on-the-trunk-of-today.md`).

    **Recommendation: ratify R1 to R8**, the robust route at each disagreement, on what was built and run: R1 built by the compiler-engineer on 186's `build` side, its `check`-side twin vetoed on a run the critic reproduced; the honest wording priced at +55 and K at +10 on the real instrument; 092's unit taken from C, the answer every precedent the historian read gives, the only guard that stops `read`, `poll` and `mbstowcs` on three legs.

    **The conservative alternative, the author's to choose instead**: no `rest: zero`, a program's own `{0}` header constant through defect 094's repair (189 tokens for `utsname`), and 092 closed by A's refusal alone.

    **Ratified by delegation, 2026-10-06**, on the author's answer at 12:49 by the clock read then, given before the synthesis existed, meant as: *OK, ratify* (`issues/2026-10/06/2026-10-06-1249-the-author-answers-1-2-3-m-issue-files-to-this-session-github.md`, point 3). **What it settles**: R1 to R8 as the synthesis states them. **What it does not settle**: what only the landings measure, the reader test the spec-warden registered above all.

## Prior art

Panels 178 and 194 (`docs/panel/178-the-rest-is-zero-where-it-is-written-and-c-says-which-value-is-valid.md`, `docs/panel/178-reports/historian.md`, `docs/panel/194-the-rest-is-zero-where-a-group-s-construction-says-so-and-a-lend-s-count-is-in-c-s-own-unit.md`, `docs/panel/194-reports/historian.md`): each line says what the cited page itself states, the sitting's own file says what was taken from it, and every link was re-read 2026-10-11.

- **Taken from C11**: [ISO C11 (N1570) 6.2.6.1p6](https://www.open-std.org/jtc1/sc22/wg14/www/docs/n1570.pdf): the bytes of the object representation that correspond to padding take unspecified values after a store (6.2.6.1p6)
- **Taken from GCC**: [GCC 15, {0} no longer clears a whole union](https://gcc.gnu.org/gcc-15/changes.html): {0} in C or C++ no longer guarantees clearing the whole of a union
- **Taken from GCC**: [the access attribute, size in elements of the pointee type or bytes for void *](https://gcc.gnu.org/onlinedocs/gcc-14.2.0/gcc/Common-Function-Attributes.html): the access attribute's size is the number of elements of the referenced type, or of bytes when the pointer is void *
