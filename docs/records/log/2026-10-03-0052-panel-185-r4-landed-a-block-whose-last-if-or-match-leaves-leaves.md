# Panel 185's R4 landed: a block whose last `if` or `match` leaves on every branch leaves

2026-10-03 at 00:52 by the clock, lane flow4, defect 175, at `937ae12f`.
Panel 185's R4
(`docs/panel/185-a-macro-is-named-as-a-macro-an-arm-takes-a-statement-a-leaving-block-leaves-and-a-spaced-sign-has-two-readings.md`,
ratified 2026-10-02), built as the sitting's (3x).

**What landed.** An `if` or a `match` where a value is wanted, a value
block's last line or an arm's one statement, every branch of which leaves,
leaves with them, so its block or its arm leaves as one ending on a `return`
does: its own `no_value`, the last thing its value check says, is withdrawn
(`check/path_end.withdrawn`), failing safe wherever anything else was said,
and no field is added to the checker, so no frame widens. A *binding* of such
a construct is still told, in the words true of how its arms left; and R4's
refusal reads the jump words alone, so a statement after a leaving `if` or
`match` stays legal (the sitting's c5). Lane flow's a55, a69, a73, b1, b2,
b3, b5, b6 and b8 build and run (b4 a parse error and b7 an `if` with no
`else`, as the sitting said), with the engineer's d2, d4, d5, d6, d9 and d10
(`tests/golden/run/fixedbugs-175-a-value-block-whose-last-if-or-match-leaves.hero`);
d1 and d8 stay refused, at the true place
(`tests/golden/check/fixedbugs-175-what-still-has-no-value.hero`).

**Where it is written.** Spec § 8, the sentence the sitting left unpriced,
written at the landing: *A block or an arm leaves, and owes no value, once a
statement in it ends a path or is an `if` or a `match` every branch of which
leaves; only a jump refuses the statement after it.* (priced with 184's R4,
its own entry); design.md §4.7, as R8 names.

**Measured**: the value tails the base refused, nested, now check to 98 deep
and build to 97 (the base refused them up to 91 before its stack gave out);
the sitting's prediction that the census changes exit for exactly the files
R4 alone changes is owed at the round's gate.
