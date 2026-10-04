# The number C keeps, and the one it throws away

2026-09-19. M-declared-extents step 6, landing panel 166's route C for defect
063, with one clause the resolution left open and the shapes beside it closed.

## The decision

| | |
|---|---|
| date | 2026-09-19 |
| decision | **`counted_by <sibling>` on a group's `ptr` parameter**, and a lend's extent judged three ways: a `_Static_assert` against C's own `sizeof` of the field where the call states a constant, **one compare before the call where it does not**, and a refusal at `check` where the parameter carries no mark at all |
| reason | the relation has to be declared, because `f(p: ptr, n: i64)` is two independent parameters and there is no *extent argument* to find — the correction defect 063's own entry carries. The check is C's because the number is C's: a `partial` record's size and a group `constant`'s value are both things Heroes does not hold, and both were measured refusing correctly. The runtime half lands because the constant half alone left a 4096-byte read at exit 0, which is the engineer's veto-lifting condition and §1.12 at rank 3 |
| design.md § | §4.19, §1.12, §1.6 |
| panel | 166, ratified 2026-09-19 by the author, reading; the runtime half is this implementation's own choice inside that resolution, and what conservative would have been is recorded with it |

## What the spec bought, and what paid

§ 13's `f.ptr()` sentence is rewritten rather than appended to, and `CParam`
gains `[ "counted_by" ident ]`. **+37 vendored and +48 on the reader's own
instrument** — 6089 to 6126, 8106 to 8154 on `claude-opus-5`, digest re-pinned
to `249990ca1b6b2f66`, under `DELTA_GATE`'s 50. The merge is what makes it
cheaper than the two drafts panel 166's spec-warden priced apart, +3 and +51
against 48 for both halves, which is that seat's own finding about merging
arriving again.

**The payment is a registered falsifiable prediction**, instruments that exist
today, scored at the M-declared-extents close:

> A `.ptr()` lend at a parameter declaring `counted_by n` with an extent the
> call states as a constant emits exactly one `_Static_assert` per lend and
> **no runtime compare**; one whose extent is read at run time emits **no
> assertion and exactly one compare**. Instrument: `./heroes build <program>
> --emit-c` over the three goldens this step ships, counting
> `heroes-ffi-extent` and `hero_panic(` lines. The falsifier is a lend that
> emits both or neither.

## The production and the parser land in one commit

Panel 166's spec-warden measured that the `grammar` suite reads **7 passed, 0
failed with a production no parser accepts**, and filed it as a footnote. This
change is the first to owe that warning something: the `CParam` row and
`counted_by_marker` are in this commit together, and the goldens compile the
form. The hole itself stays what it was — nothing compares a production to the
parser — and the warden's note is the record of it.

## What the two colourers owe, measured rather than assumed

**Nothing, and that is a fact rather than a shortcut.** The site derives its
keyword set from `selfhost/keywords.hero` at build time, so it cannot go stale;
`counted_by` is not a keyword but a contextual word, like `owned`, `consumes`,
`acquires` and `borrows`, and the TextMate grammar colours **none** of those —
grepped, all five absent. Adding only the new one would make it the single
coloured mark of six, which is worse than the uniform silence. The gap is the
one `.claude/rules/diagnostics-and-goldens.md` already records for that file
in 2026-09-10, and it is unchanged by this step.

## And a diagnostic downgraded before it shipped

`field_lend_extent`'s fix — write the field's own length — was drafted
`certain` and is `guess`. Writing the shorter number ends the corruption and
CHANGES what the program asks C to do, and the author may have meant the longer
number and the wrong field; the note carries both repairs, and a fix `--apply`
may take is one with no second candidate (design.md §4.17's inverse case, panel
071).
