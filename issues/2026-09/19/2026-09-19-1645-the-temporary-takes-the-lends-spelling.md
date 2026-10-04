# The temporary takes the lend's spelling

2026-09-19. M-declared-extents step 5, landing panel 166's route H for defect
065, after its first landing (`6823cca7`) stopped on one question.

## The decision

| | |
|---|---|
| date | 2026-09-19 |
| decision | **a lend's temporary is declared with the lend's own C spelling**: `const void *` where the lend is rooted at a name the program may not write, `void *` where the root is a `@` cell. `emit/field_lend.hero` answers which values are const lends and `emit/body.hero`'s prologue reads the answer; the IR carries a local slot's mutability for it. And the diagnostic reads **clang's own qualifier line at the call**, never a marker |
| reason | the prologue declares temporaries by Heroes type and `ptr` has one spelling, so the `const` died on the assignment one line before the call, refusing the honest read instead of the write — the failure `6823cca7` measured. Rendering the cast at the argument instead would put a lend's C text in `emit/ops.hero`, a module that knows nothing about places; a second spelling for one temporary is the smaller change and keeps the cast where the place is rendered. For the diagnostic, a marker needs the header's parameter text, which only the `__typeof__` dump gives and no macro-named function survives; clang's line reaches every case, and `ffi_mutable` has gated on the same phrase since panel 058 |
| design.md § | §4.4, §4.19, §1.12 |
| panel | 166, ratified 2026-09-19 by the author, reading; this entry is the implementation's own choice inside that resolution |

## What was measured before it was written

Eleven shapes beside the defect, all on this Mac with the compiler built from
`selfhost/`: three legal roots print their sums at exit 0 (`=` to a reader,
by-value parameter to a reader, `@` cell and `@` parameter to a writer, 16 and
36), and every immutable root handed to a writer is refused at the lend with
`field_lend_written` — the `=` binding, the by-value parameter, a nested field, a
`match` payload, a `for` variable, and two lends on one line. `--emit-c` on the
refused program writes nothing at exit 1. The record is
`docs/records/done/2026-09-19-1645-defect-065-closes-and-the-header-decides-per-call.md`.

## The clause the shapes beside it forced

A lend handed to a Heroes function taking `ptr`, or through a function value,
went from exit 0 to exit 2 under the cast alone — clang refusing `const void *`
into the wrapper's `void *` at a line no reader owns. **So a `ptr` lend stands
only as an argument of an `extern` call**, refused at `check` as
`field_lend_needs_a_header`. It anticipates by one step what route C requires
anyway — the extent is declared on the group's parameter, and only an `extern`
has one — and it refuses one shape that was legal this morning: a `@` cell lent
through a `ptr`-taking wrapper. That shape reached C with nothing asked of the
header; the wrapper §4.19 prescribes takes the record and lends inside itself,
and stays legal. § 13's sentence is amended with route C in the next step, and
the two steps travel in one push.

## What was priced and not taken

A `_Static_assert` on `__builtin_types_compatible_p(<header type>, const
<pointee> *)` in `cli/pointee.hero`'s check unit, carrying a marker with the
lend's span. It would state the verdict in C and name the parameter from the
header, and it was not taken because the check unit's text comes from the dump,
and a function the header spells as a macro gives the dump nothing — the case
would fall through to exit 2. It is recorded here so it is not rediscovered as
new: if clang's wording for the qualifier error ever changes shape,
`ffi_lookup.DISCARDS` is the one string to move, and the dump route is the
alternative with its cost already known.
