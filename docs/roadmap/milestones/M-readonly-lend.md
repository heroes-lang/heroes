# M-readonly-lend — a function reads the caller's place without claiming to change it

**Scheduled 2026-10-18, at row 68, by panel 217's R6**, ratified by the
author on 2026-10-17 (https://github.com/heroes-lang/heroes/issues/2105), the
sitting's one question answered with its recommendation: the form lands on a
row of its own, next in the chain, before M-witnessed-seed. R6 owed the row at
M-buildable-structs' close, the sitting then unratified; asked whether to
enter it that night or in the next session, the author chose the next session
(https://github.com/heroes-lang/heroes/issues/2132). The sitting is
`docs/panel/217-a-read-only-lend-is-lent-on-a-parameter-the-callers-place-read-and-never-written-refused-beside-an-at-of-that-place-and-past-a-variadic-prototype.md`;
its § Resolution, R1 to R7, is this page's reasoning and is not restated here.

**Opened 2026-10-18**, by the author's `/step` naming no step, the next row of
the chain, in one lane, `lane-readonly-lend`, the steps in this order: the form
and its refusals (R1, R2), from the compiler-engineer's final prototype; the C
half (R3); the read-only mode in the IR with a verifier rule, and the messages
(R5, R4); the walk; the specification's text and design.md's §4.8 and §4.19;
then defect 2128, which counts `blocking` at this milestone's tag, being
`adjacent` and filed between the two tags of 2026-10-17. **What the seats'
reports leave to the landing, read on 2026-10-18**, each beside the prototype
`f1f` the compiler-engineer approved on:

- **(a)**: `f1f`'s probe takes `struct counter *` and puts the `const` question
  in an assertion of its own; the ffi-pragmatist's condition is a probe whose
  parameter carries the `const` (`docs/panel/217-reports/critic-reports.md`
  § 2, `ffi-pragmatist.md` § 7);
- **(c) for a record**: `f1f` builds a record lent read-only past a writing
  variadic header, `103` at exit 0, the critic's `va_record_lent`; the `cstr`
  half is defects 2102 and 2103's repair;
- **(d) at a C call**: the overlap of an `@` lend and a read-only one handed to
  C, where `restrict` makes `-O0` and `-O2` print two answers (`ffi-pragmatist.md`
  § 4); whether `f1f`'s sweep reaches an `extern` call is to be run;
- **the words**: the draft's *no other `@` argument of the call* is narrower
  than the refusal, which reaches an `@` nested inside another argument, and the
  sentence on C writing through an address it was let keep is owed and
  unpriced, so the landing's count may differ from the predicted 10,246, and is
  scored as measured;
- **a message**: an `@` argument rooted at a lent parameter and handed to an
  `extern`, `counter_value(@c)`, is told the generic `not_mutable` and not the
  route that works, a `lent` C declaration (`compiler-engineer.md`, r01).

**The fact that schedules it.** A parameter is a value or an `@` place the
function may change, and C's `const struct counter *` is neither: a function
that only reads a record holding a lock can take it by copying it, which panel
215's R2 refuses, or by `@`, which says it changes it. The sitting's three
blind arms measured what that costs a reader (5.0202 USD): with the language
as it stands, ten readers of ten were correct only on their second try, all
through `doubled(@c: Counter)`, a signature nine of them had first refused as
a claim of a change; with the form, nine of ten were correct on the first
try, ten of ten wrote `doubled(c: Counter lent)` with an unmarked call, and
none copied a lock. Principle 0's second door, a measured argument the panel
accepted.

**What it delivers**, each part the sitting's:

1. **R1, the form**: `lent` on a parameter of a Heroes function, the caller's
   place read and never written, the call unmarked; `@p: T lent` refused; a
   function with a `lent` parameter not a function value.
2. **R2, the refusals that are part of it**: a `lent` argument whose place an
   `@` argument of the same call reaches, nested in another argument included;
   a lock-holding record lent through a generic `T lent`; a `lent` parameter
   written, `@`-lent, returned or copied; a function with one used as a value.
3. **R3, the C half**: a record lent read-only crosses as `const T *` through
   a probe that carries the `const`, never `void *`; the read-only question
   `selfhost/emit/ffi_read_only.hero` asks of a `cstr` (defect 411) asked of a
   record, refused by the compiler's own diagnostic; a read-only record lend
   past a variadic prototype refused. **R3(c) for `cstr` has landed already**,
   defects 2102 and 2103 in M-buildable-structs' batch (38b1146f7, 5711bcefa).
4. **R4, the messages**: `ffi_lock_copied`'s note and `never_rebound`'s fix
   name `lent` where they name `@c`; a fresh construction stored into a
   lock-holding place gains a note.
5. **R5, the IR carries the read-only mode**, with a verifier rule.
6. **The specification's text** the spec-warden priced for the landing,
   § 9's function-value clause included (`docs/panel/217-reports/spec-warden.md`
   § 2 and § 3).
7. **The walk a new surface form owes** every tool that reads the language
   (`.claude/rules/diagnostics-and-goldens.md`): the formatter and its
   self-check, the dumps, `heroes grammar`, `heroes mutate`, the probe's
   reader, the fixes that quote a program, `heroes measure`, the two
   highlighters.

**Where it starts.** The compiler-engineer's prototype, about +524 lines over
20 files, half in the checker and none in code generation, ownership or the
runtime, and the spec-warden's landing draft are kept in
`docs/panel/217-reports/evidence.tar.xz` (its `evidence.md` says how to put
them back): `217-ce-probes/f1f-prototype.patch` is the final one, on which the
compiler-engineer approved and which the critic built again and found failing
condition (c), and `217-spec-warden-work/drafts/f1-landing.md` the text.

**Predictions to score at the landing** (the sitting's table): the
specification measures exactly 10,246 real, 10,108 being its count on
2026-10-17 (the spec-warden); the landing measures 450 to 700 lines, leaves
emit, ownership and the runtime untouched, and moves `parse/`'s budget by +2
and `ir/flatten.hero` by +13 (the compiler-engineer); `counter_init(c:
Counter lent)` is refused at exit 1, and `fixedbugs-2062` rewritten with
`lent` builds on both platforms (the ffi-pragmatist). And the
compiler-engineer's condition, R5's: timed on a still machine, a cold
`--emit-c` of the compiler more than 2% slower turns that seat to object.

**What it must not do.** Land the form without R2's refusals or R3's
conditions: the ffi-pragmatist's veto stands without (a) or (c), and the
final prototype, on which the compiler-engineer approved, built a read-only
lend past a writing variadic header and printed `103` at exit 0, found only
because the critic built it again.

**What stays open, recorded by the sitting**: C writing through an address a
binding let it keep, which no compiler check reaches (a specification sentence
owed at the landing, unpriced); a pointer C hands back with its `const`
stripped (issue 1292 records it for strings as left open; for records nothing
does); the Windows box, unrun at the sitting.

**Named for the deliverable**, the read-only lend. Not `M-lent-parameter`,
the spelling, which the sitting weighed against a second word and a later
sitting may change; not `M-const-lend`, C's word, for half of what it delivers;
not `M-borrowed-places`, since `borrows` is already a mark of spec § 13 with
another meaning. The coordinator's default, the author having named none; a
rename before the first tag moves this page, the chain's row and the issues'
milestone.
