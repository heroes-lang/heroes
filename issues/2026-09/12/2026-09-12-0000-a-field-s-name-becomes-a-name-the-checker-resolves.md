2026-09-12 · **A field's name becomes a name the checker resolves, and the
measurement that justifies it was run rather than predicted** (panel 132's
adopted shape E, ratified 2026-09-11; M-reflection-verdict step 3). `Point::x`
is the `str` `"x"` and `Point::witdh` does not compile, while the program still
writes every byte of its own output — quoting, escapes, order, `sort` — which was
the clause the corrected question said was not optional. **The number, run on one
program in two spellings**: with bare string keys, `heroes mutate --operator
typo-key` plants four one-character slips and the compiler kills **0 of 4**; with
`m[Room::width]`, `typo-ident` plants five and the compiler kills **5 of 5**,
because the slip lands on a name the checker resolves. Two seats predicted
approximately that and neither had run it. **The cost is not the prototype's, and
the reason is the author's ratified choice.** The compiler-engineer's branch
measured **+59 code lines in three files with no ceiling moved**, reached by
reusing the existing field node; `::` has to be in the TREE for the formatter to
re-print it, so it is a node of its own, and the language charged for that
correctly — **68 exhaustive `match`es over `ExprKind` went red**, every one
listed, none forgettable. Four `DECIDED` ceilings move with their reasoning:
`ast.hero` 491 → 505, `check/walk.hero` 1842 → 1870, `grammar_expr.hero`
1044 → 1085, `ir/flatten.hero` 1123 → 1150. **Three new diagnostics and one old
one repaired**: `no_such_field` reused so the fields are listed without opening
another file; `not_a_record_name` for `::` on a value; **`dot_on_a_record_name`
for the near miss** — `Room.name` where `name` really is a field — which used to
be a message about how records are constructed and is now the repair that was
wanted, with a **certain** fix whose `.fixed` checks clean; and
`record_name_alone` unchanged where no such field exists, **with a golden for the
first time**, `grep -rln record_name_alone tests/` having been empty. **CL-036's
walk was performed and not read**: the formatter round-trips byte-identical,
`--dump-ast` prints `::`, and both things that COLOUR a program learned the form,
`site/src/lib/highlight.ts` and the TextMate grammar — without which a field
called `str` or `record` would have been painted as a type or a keyword.
**Spec 5662 → 5716 real, +54**, already net of panel 117's restoration clause
coming out, digest `35968b435ed519d7`, 428 free and 368 net of the FFI floor,
ledger row 68. **What paid is the registered prediction**: `typo-key` is `heroes
mutate`'s **fifteenth** operator and the first that typos a string literal.
**A defect in the net's own instrument, found and repaired on the way**: the
annotation reader trimmed before testing for the `v` of `#~v`, so
`#~ variant_in_value_position` read as a next-line mark plus the code
`ariant_in_value_position` — **a whole class of codes that could not be
annotated, silently, for as long as that reader has existed**. Two codes are in
the class and neither had ever been annotated, which is why nothing found it
until a golden needed one; the rule now lives in its own function so a test can
reach it rather than imitate it. **And the honest cost**: the new compiler is
**~1.0% slower** on `check selfhost/main.hero`, 14.05 s → 14.19 s, three runs each
on a still machine with a 0.08 s spread inside each set — outside the noise,
barely, and reported rather than rounded. Full net **1733 passed, 0 failed** |
design.md §1.6, §1.7, §4.17; CLAUDE.md §8, §9, §11, § Verification | — (117, 131,
132)
