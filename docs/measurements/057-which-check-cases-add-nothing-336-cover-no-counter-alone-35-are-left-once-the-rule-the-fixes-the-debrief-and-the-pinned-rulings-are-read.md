# 057: which `check` cases add nothing: 336 cover no counter alone, and 35 are left once the rule, the fixes, the debrief and the pinned rulings are read

Measured 2026-10-28 at M-generated-programs step 4, for task 2214
(https://github.com/heroes-lang/heroes/issues/2214), on the trunk at `10f0ea9c7` with the
compiler of `sources 5e0ca4c634086a7c`, by panel 231's R8 (decision 2243): *a
`docs/measurements/` file, the criterion written before any case is touched*. **No case is
removed by this file**: removing a case deletes files, which CLAUDE.md § Hard stops makes the
author's to ask for, and the review below is the condition for asking, not the asking.

## 1. The criterion, written before anything is touched

A `check` case is **load-bearing** where some regression of the compiler would fail it and no
other case of its kind; the task's words, after Goose's audit (coverage counted only toward
fixtures *that would notice the same regression*). Measured as four witnesses, in order, each a
reason to keep a case:

1. **coverage**: the case executes a counter of the compiler no other `check` case executes;
2. **the rule**: a case named `fixedbugs-` keeps its place whatever it covers, a fixed defect
   getting a case named after it (`.claude/rules/diagnostics-and-goldens.md`);
3. **the text**: the case pins a diagnostic code, or a message template (its text with every code
   span and every number made a placeholder), that no other case pins;
4. **what the counters cannot see, as measured**: the case pins a `certain` fix's output (a
   `.fixed` or an `.applied` beside it), carries the marker `UNVERIFIED — pending debrief`
   (an adversarial case the author has not yet read), or is a recovery row panel 187's R1 pinned as
   a known cost, its header naming the ruling it rests on.

A case no witness reaches is a **candidate**. A candidate is removed only after a reading that tries
to refute its removal (the caret's column, a note's words, an excerpt's line, a shape a future
regression of its own would meet), written beside it, and then only on the author's word.

## 2. What was measured

- **Coverage**, by panel 231's compiler-engineer
  (`docs/panel/231-reports/compiler-engineer.md`, Q8): the compiler instrumented with clang's
  `-fprofile-instr-generate -fcoverage-mapping` over its own emitted C, the 709 `check` cases run
  one profile each; **373** cover a counter no other case covers, **336** cover none, the list
  `cov/no-unique.txt` in the sitting's evidence (`docs/panel/231-reports/evidence.tar.xz`).
- **The rule**: of the 336, **257** are named `fixedbugs-`; **79** are not.
- **The text**, over `tests/golden/check/*.expected` (709 files, 204 codes and 662 templates
  pinned in all, each line `... (error|warning|note)[<code>]: <message>`): of the 79, **1** pins a
  code no other case pins and **1** a template no other case pins (panel 231's critic, finding 10,
  measured the codes' half: 2 of the 336); **77** pin nothing alone.
- **What the counters cannot see**, over the 77: **14** carry a `.fixed`, **9** an `.applied`,
  **23** one or the other; **12** carry the `UNVERIFIED` marker, 5 of them with a fix too; **12**
  are `panel-187-*` recovery rows whose first lines name the sitting or its R1, every one of them.

## 3. The candidates

**35** cases no witness reaches: `arm-pattern`, `braces-as-block`, `builtin-name-taken`, `cell-redeclared-over-a-binding-is-shadowing`, `declaration-in-arm`, `ffi-a-group-head-names-not-locates`, `ffi-a-lend-parked-in-a-group-record`, `ffi-a-lend-reaches-only-a-lent-parameter`, `ffi-consumes-a-borrowed-handle`, `ffi-parameter-width`, `ffi-transfers-a-borrowed-handle`, `handle-ended-on-every-path-is-refused-at-the-read`, `indentation`, `inout-on-a-by-value-parameter`, `inout-on-a-generic-by-value`, `inout-on-a-loop-variable`, `inout-on-a-plain-binding`, `inout-through-a-field-of-an-immutable`, `inout-through-a-nested-record-of-an-immutable`, `inout-through-an-index-of-an-immutable`, `int-out-of-range`, `leading-zero`, `line-end-before-continuation-in-a-call`, `missing-body`, `missing-return`, `nested-fixed-array-tiebreak`, `never-rebound-beside-a-refused-marker`, `never-rebound-never-read-is-unused`, `no-size-self`, `not-a-place`, `repeat-count-is-unsigned`, `rest-zero-ends-only-a-construction-of-a-groups-record`, `shadowing`, `stray-carriage-return`, `unterminated-hole`.

They cluster by shape: the seven `inout-*` refusals of an `@` argument that is not a place, five
`ffi-*` refusals at the C boundary, and the lexer's and the layout's one-message cases
(`indentation`, `leading-zero`, `stray-carriage-return`, `unterminated-hole`). A cluster of
cases each pinning the same template is the shape the criterion exists for, and also the shape in
which one case is often the only one a future regression of that refusal's OWN branch would meet,
which a counter shared across the cluster cannot tell apart: that is what the reading of § 1 must
look at, case by case.

## 4. What this measurement does not settle

The reading of the 35 is unwritten. Message templates were compared with code spans and numbers
made placeholders, so two messages differing only in a noun outside a code span count as two; the
carets, notes and excerpts were not compared. Coverage was read on the compiler of one commit, and
a counter one case alone reached there may be shared on the next.
