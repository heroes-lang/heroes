---
kind: decision
area: records
milestone: none
filed: 2026-09-25
commit: self
github: none
---

- [x] **panel 178** | ratify panel 178: a group record's construction may end with `rest: zero`, with no promise about padding; zero is admitted as bytes and never as a claim of validity; a header's initialiser binds as a group constant; C's `char` is `i8` in § 13; a string into a fixed field and `[x; N]` wait behind their measurements; a zero default for every type is refused | `docs/panel/178-the-rest-is-zero-where-it-is-written-and-c-says-which-value-is-valid.md`

    **Origin:** panel 178, 2026-09-25, M-buildable-structs, convened on the
    author's instruction of 2026-09-24 to bring the most robust solution even
    if less economical. **The default the compiler runs on meanwhile** is
    today's: nothing of it has landed, the row stays `scheduled`, and defects
    091 to 094 are filed and open. **What conservative would have been**: the
    element-write repair alone, no new form and no spec token, refused because
    `utsname` stays 905 to 4016 real tokens a construction and the census's 23
    to 42 structs a platform stay unbuildable in practice. **What the robust
    reading would add, for the author to choose**: route T's place form now
    rather than behind its measurement, and a sentence saying zero is not a
    valid value of every C type (+36 real, priced by the spec-warden and
    unneeded in fifty readers).

    **Superseded 2026-10-06**, ticked rather than left open. This item stood only on `lane-panel-178` from 2026-09-25, in no list of the trunk, until the branch was merged that day. The author's instruction of 2026-10-06, on the coordinator's proposal to bring the sitting in, meant as: *I agree with your proposal, but there have been a thousand changes since that sitting, so the panel should perhaps be regenerated, or at least updated, and then you can bring it in, so we lose nothing.* So the sitting is sat again on the trunk of that day, panel 194, every premise and measurement of 178 re-run there, and the ratification is that sitting's own decision issue (`issues/2026-10/06/2026-10-06-1118-panel-178-reaches-the-trunk-sat-again-on-the-trunk-of-today.md`). This one keeps the question 178 put. Its defects stand as their own issues, each re-run on `bef739dd`: 091 closed by defect 097's repair, 092 open, 093 closed into defect 245, 094 open.
