# M-marked-acquisition — the mark goes where the obligation is created

**Scheduled 2026-09-14 by panel 147 R4**, at M-cleanup-verdict's close, and it
is the form that milestone's verdict admitted. The sitting refused a releaser
keyed on the handle TYPE and named the axis instead of the spelling: **the
obligation is created by a CALL, so the mark goes on the acquiring call, and the
compiler never picks the release call.**

**What it delivers**: a compiler rule by which a C handle acquired in a scope
and not released on some path out of it stops being silent. Whether *stops being
silent* means a compile error or a loud exit is the first thing this milestone
measures, not something it inherits.

**Why the sitting would not choose the instrument here.** Two are on the ladder
and both have shipped relatives in this tree, so the choice is a price and not
an argument — and panel 147 had no price, because Route A did not even parse
(`error[expected_extern_signature]`), so nothing at that sitting was built.

- **A `consumes`-shaped mark on the acquiring call.** Nearest shipped
  neighbour: `selfhost/check/consuming.hero`, **114 code lines**, landed
  2026-09-13 at panel 145. It already expresses *this call ends that value's
  life*; the mirror is *this call begins one*.
- **Escape refusal.** Its machinery ships **twice already** — `cstr_escapes`
  (`selfhost/check/lending.hero:242`) and `lease_escapes`
  (`selfhost/check/leasing.hero:109`) — and panel 147's completeness critic
  found that **panel 122 refused an INFERRED release and not escape refusal**,
  so the precedent usually cited against it is for another form. Five shipped
  relatives measure **93 to 241 code lines** (`ffi_sweep` 93, `consuming` 114,
  `leasing` 195, `freer` 221, `lending` 241), two of them under the
  compiler-engineer's own 120-line bar. **Blast radius on this tree: five
  sites** — three handle-typed fields outside an `extern` group and two
  wrapper-returning functions.

**What this milestone may not do.** Re-open panel 147's R1: a releaser keyed on
the type is refused on its axis, it now has a design.md Part 6 row with a
falsifier, and no spelling repairs it. Adopt a scope-bound statement without
answering the `@`-cell defect two seats found from opposite ends — the deferred
call's arguments are read where the statement is written, so the natural program
registers the release of a value the acquiring call has not yet filled, and it
compiles.

**The urgency it inherits is smaller than the sitting was told, and that is
written here rather than discovered later.** `docs/measurements/030`'s
correction of 2026-09-14: live exposure is **22 paths in one file**, not 23 in
two, and **all 22 end in `exit(1)` or `abort`** — `main` closes and exits on
`is_err()`, the six test blocks use `.must()`. **No shipped program leaks a
handle and then goes on running.** The class is real, the reduction proves the
mechanism, and no program in this corpus has yet been written that handles an
error instead of leaving. A milestone that opens by re-measuring that will know
whether its own warrant has grown.

**All three items closed 2026-09-14 at this milestone's own close**, and their
record is
`docs/records/done/2026-09-14-1900-the-form-is-closed-at-three-words-and-two-of-the-three-items-were-decisions.md`.
The highlighter class was not closed but MOVED, to
`docs/work/milestones/M-vscode-extension.md`, which is the milestone whose
subject it is; closing it here would have been ticking somebody else's work.

*******************************************************************************
**OPEN: 0**

*******************************************************************************
