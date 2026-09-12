# 051 — M-reflection-verdict: the ruling on reflection

## Goal

Write down three rulings the language had never made, and that nobody could
cite: run-time reflection, compile-time derivation over a record's fields, and
the general annotation mechanism the author asked about on 2026-09-06 after
noticing that `owned fclose` already looks like one.

The milestone was scheduled on 2026-09-03 as **a decision, not a feature**, on
the author's own example of something never considered. What warranted it turned
out to be sharper than what scheduled it: measured at the opening, **the word
`reflection` appears 0 times in design.md's 3660 lines.** The 2026-09-03
scheduling had measured *three mentions and no ruling* over a wider set; by the
opening the count across design.md, the log and the sittings was 22 and **none of
them was in design.md**, because every new mention is the record talking about
the absence. So the refusal was citable from no ruling at all — the shape
CLAUDE.md §1 exists to refuse, and the reason the author reopened it.

## What surprised

**Three refusals were sound and three of their reasons were false.** That is the
milestone in one line, and each falsification came from a seat that went and
looked rather than from an argument.

*"Reflection requires runtime dispatch, i.e. an interpreter"* is design.md's own
ground and it does not carry: Go is compiled and has `reflect`. The real cost was
sourced from the language that shipped run-time reflection and then removed it —
Rust RFC 0379, 2014 — whose three stated drawbacks are per-type metadata
inflating compile time and binary size, slow formatting, and recompilation
complexity, and which says reflection became obsolete **because `deriving` was
better**.

*"A user-defined tag set cannot be closed by construction"* was the hinge the
brief handed the sitting, and it is false. The distinguishing property is not
user-defined against built-in; it is **whether the tag is a name that resolves or
a string**. Java's annotations are type declarations and an undeclared one does
not compile; Go's struct tags are strings whose `govet` check, added in the same
2011-06-29 commit that introduced the convention, still does not notice `jsn` for
`json` fifteen years later.

*"The recursion cliff dissolves — the walk is `eq`'s walk"* was panel 117's own
load-bearing sentence, written four days earlier and ratified the same day. It is
false at `runtime/parts/array.c:185-190`, where the worklist carries
`{a, b, elem, len}` and no path at all.

**And a fourth thing was falsified: the sitting's own brief.** Panel 131 framed
compile-time derivation as *rendering Heroes' own construction call*, which is
display, while the precedent and the measured pressure are about a per-type walk
aimed at a **foreign notation**, which is derivation. Three seats answered the
question and two answered the brief, and reading them together they were not
disagreeing. The sitting refused to rule, said so in as many words, and was re-sat
on the corrected question.

**The corrected sitting produced two independent convergences, and that is the
strongest evidence a process like this can make.** The seat holding only the
specification, with no repository, and the seat holding the whole compiler
invented the same construct without seeing each other: a field's name becomes a
name the checker resolves. Separately, the seat at the C boundary and the seat
reading precedent reached the same different answer, one by compiling it and one
by reading Ada's thirty-year-old version of it.

**The negative claim the brief asked for is false, and it is thirty years old.**
Ada shipped compiler-derived per-component serialisation with no macro, no
compile-time execution, no trait and no separate generator — `T'Write` in ISO
1995 and `T'Put_Image` in ISO 2022 — and carries a warning with it: GNAT
documents that the derived format is *"deliberately not documented"*.

**What the corpus said, twice, from two seats who did not compare notes.** A
construction-call derive would delete **0 of 56** hand-written renderers by one
count and **0 of 31** by the other, because every one targets a foreign notation:
a date zero-pads, an amount converts cents, a JSON writer sorts keys and escapes
strings. Both counts were fatal to the drafts and silent about derivation, and
separating those two things is what the second sitting was for.

## What broke and why

**The seed, twice, for the same reason, once by this lane and once by another.**
The rule is in `seed/README.md` in a sentence about exactly this case: if the
diff touches `selfhost/`, the seed is regenerated in the same commit. This
milestone verified everything against the freshly built compiler and never against
the seed-built one — which is the binary the gate uses — so every suite was green
locally and `main` went red on all three platforms. The repair took twenty
minutes; it was public for all of them. That file also records that `main` once
sat red for a day for the same reason, and says the two loud conditions exist
because you should not rely on them.

**A defect in the net's own instrument, found because a golden needed it.** The
annotation reader trimmed before testing for the `v` of `#~v`, so
`#~ variant_in_value_position` read as a next-line mark plus the code
`ariant_in_value_position`. **A whole class of codes could not be annotated,
silently, for as long as that reader has existed.** Two codes are in the class and
neither had ever been annotated, which is why nothing found it until this
milestone wrote the first golden that needed one. The rule now lives in its own
function so a test can reach it rather than imitate it.

**Two compiler defects, and one of them was a rule already decided.** Defect 028
was one arm that returned instead of walking: a function type reachable only
through a record's field reached no typedef, and an assertion whose comment
states the premise in its own words found it false and aborted at exit 134 after
`check` had passed the program at exit 0. It survived only while nothing *called*
through the field. Defect 027 is the one worth re-reading: `cstr` emits
`const char *` and a header's member is often plain `char *`, and **panel 089 had
already widened the per-field assertion to accept both** and written down why —
const restricts writes and the language offers none through a `cstr`. The
declaration was legal, the assertion passed, and the compound literal was then
refused by this project's own `-Werror`. A binding the compiler had deliberately
admitted could be declared and not built.

**And the form cost more than its prototype, because the author chose the other
spelling.** The compiler seat's branch spelled it `Point.x`, measured **+59 code
lines in three files with no ceiling moved**, and was green — and it reached that
by reusing the existing field node and deciding in the checker. `::` has to be in
the tree for the formatter to re-print it, so it is a node of its own, and a new
node is a new arm in every exhaustive match over the expression kind: **68 of
them**, which the compiler listed one by one and would not let a session forget.
Four ceilings moved. The price was taken knowingly and is on the record beside
the reason: `.` already qualifies a module and already reads a field from a
value, so a third meaning would make the line legible only to a reader who knows
whether the base names a type or a value, which is not on that line.

## What landed, and what carried forward

**Landed.** Two design.md Part 6 rows with their falsifiers — run-time reflection
refused on layout rather than on interpretation, user-defined tags refused on
inertness rather than on impossibility. The Ruby row's reason corrected, and
panel 039's C4 row corrected underneath itself. Panel 117's sixth option
withdrawn on the three measurements it had itself demanded, and its restoration
clause removed from the specification because no shape adopted or scheduled here
is a renderer `assert` could use. `Point::x`, the field-name operator, with four
diagnostics, one of them carrying a `certain` fix and one of them —
`record_name_alone` — getting a golden for the first time in its life. The walk a
new surface form owes every tool that re-prints or colours a program, performed
and not read. `typo-key`, the fifteenth mutation operator and the first that
typos a string literal, which is what pays for the spec addition under §1.6.

**The number that justifies it, run and not predicted**: one program, two
spellings, the same four keys. Written with bare strings, four one-character
slips and the compiler kills **0 of 4**. Written `m[Room::width]`, five slips and
the compiler kills **5 of 5**, because the slip lands on a name the checker
resolves.

**Carried forward, and named rather than implied.** `Point::x` kills the typo and
**does not kill the rot**: add a field to a record and a marshaller written this
way still compiles and silently never mentions it. Only a walk that enumerates
fields closes that, and that shape is rehomed to **M-core-packages**, whose step 4
is the package that would consume it. It is scheduled and not adopted because what
was compiled for it is a hand-written simulation of what the compiler would
generate and not the generation — the distinction that cost panel 117 five days
earlier, and the one thing this milestone was determined not to repeat.
