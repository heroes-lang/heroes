# Two sittings ratified, and the author reads them

2026-09-19, M-declared-extents. The author's instruction, in one line and given
in Italian: *ratify the decisions*. Written here for what it meant, as
CLAUDE.md §11 asks; the original is in this commit's own history.

- [x] **panel 165** | route 6 is refused on two vetoes and the sitting adopts the repair of three defects instead — ratify, or take the conservative resolution the file records | `docs/panel/165-the-check-it-would-add-was-not-one-and-the-route-that-shipped-had-none.md`
- [x] **panel 166** | the `ptr` lend gains a `const` it already had and throws away, and the extent is checked by a `_Static_assert` the probe can write — ratify, or take the conservative resolution the file records | `docs/panel/166-clang-already-knew-and-the-lend-was-throwing-it-away.md`

## What was ratified

**Panel 165.** Route 6 — the extent declared on an `extern` parameter, with the
compiler checking it — is **refused** on the compiler-engineer's and the
spec-warden's vetoes. The sentence it rested on was falsified three times with
three instruments: C erases a parameter's extent, so route 6's check compares the
author's declaration to the author's argument while route 4's field check is
against the header. Routes 12, 13 and 14 are recorded with their measurements.
**Resolution 3's route 14 clause is superseded by panel 166.**

**Panel 166.** **Route H** is adopted for defect 065 — the lend emits
`const void *` and clang refuses the write with a flag this project already
ships. **Route C in its C-emitted form** is adopted for defect 063 — a plain C11
`_Static_assert` the probe writes. **Route G is refused on three vetoes**, each
from a different ground. Routes A, B, D, E and F are not adopted.

## The coordinator recorded it as a delegation and the author corrected that

**This entry first said the author had not read either file**, on the shape five
previous ratifications had taken. The author corrected it in the same session, in
their own words: *that is not true, I read it* — and then, as a standing
instruction: *write down that I always read.*

**The claim was a supposition and not a measurement.** CL-058's rule is that the
record be true about who read what; it refuses a reading credited where none
happened, and it refuses a delegation recorded where a reading did. The
coordinator had turned a rule about accuracy into a habit of assuming the humbler
answer, which is the same failure facing the other way.

The standing instruction is now in `CLAUDE.md` § 4, with its date, because
CLAUDE.md § Hard stops says what the author wants kept goes there and not into
the assistant's memory. **The five earlier ratifications are not retro-corrected**
— each stands at its date, and a record is not rewritten — but a session reading
them should know that the phrase *by delegation* in them was the coordinator's
assumption and was never put to the author.

## What the ratification makes due, and what it does not

**Due:** routes H and C stop being a provisional default and become the
milestone's work. Two halves were marked unpriced in the sitting and are owed
with it — the checker side of H, and the `cli/pointee.hero` diagnostic row, so a
reader gets a Heroes sentence rather than a clang one.

**Not due:** **defect 066** is not covered. The lend's missing lifetime rule has
two routes and both are the shape three seats just vetoed for the write
direction — a hole documented rather than checked, or a rule invented without a
precedent. Pricing them is its own sitting, and the defect stays open until it
sits.

**Also not due:** the conservative resolutions. Both files record what
conservative would have been — for 165, leave route 6 queued and file only the
defects; for 166, route A alone at +3 real — and the author may take either at
any time. Neither was taken by this delegation, because CLAUDE.md § 4 makes the
robust resolution the default and the author's instruction named no exception.
