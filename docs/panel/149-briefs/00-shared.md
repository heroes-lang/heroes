# Panel 149 — shared brief

**The question.** How deep does `unmarked_handle_producer` look for a handle,
and can the mark that answers it be written for a record carrying more than one?

**Where it comes from.** `docs/work/DEFECTS.md` 033, filed 2026-09-14 at the
M-marked-acquisition close by the full net before the push, then widened by
attacking the shapes next to it. Panel 148 R5 landed the rule in
`selfhost/check/acquiring.hero` (145 lines): where any `extern` consumes a handle
type `T`, every `extern` handing a `T` back says `acquires <releaser>` or
`borrows`. The rule reads the RESULT and the `@` out-parameters and asks
`handle_behind` of each; a group record whose FIELD is a handle answers no.

## What was measured this session, and the command that settles each

Compiler built from the seed: `clang -I runtime seed/heroes.c runtime/runtime.c
-o heroes`. Every program below is in the session scratchpad and each was run
with `heroes check` and then `heroes build` + execute.

**Four shapes reach the runtime with no diagnostic, and all four abort 134:**

| shape | the binding | what happens |
|---|---|---|
| handle in a group-record field | `pair_make(n: i64) -> Pair`, `record Pair` holds `s: Slot` | checks clean, prints, aborts 134 |
| handle two records deep | `outer_make(n: i64) -> Outer`, `Outer` holds `Pair` holds `Slot` | checks clean, aborts 134 |
| handle in a fixed array field | `four_make(n: i64) -> Four`, `Four` holds `a: Slot[4]` | checks clean, aborts 134 |
| a record through an `@` out-parameter | `pair_out(n: i64, @out: Pair)` | checks clean, aborts 134 |

**Three shapes the FFI vocabulary ALREADY refuses**, so the defect's own wording
— *"a handle behind an optional, behind a list"* — names shapes that do not
exist:

- `-> Slot?` is `error[ffi_type]`
- `-> [Slot]` is `error[ffi_type]`
- a result record declared OUTSIDE the group is `error[ffi_type]`

because `crosses_the_boundary` (`selfhost/check/ffi.hero:89`) returns false for
`.array`, `.fixed`, `.map` and `.fallible`, and `ffi_field` (`ffi.hero:188`)
admits only a number, `bool`, `ptr`, `cstr`, `error`, a fixed array whose element
is header-owned, or a nested header-owned record.

**So the reachability graph is closed and finite**: an extern's result or `@`
parameter, then group-record fields and fixed-array elements, then a handle. No
other route exists.

**The surface ALREADY works for the single-handle case, and this is the finding
that shrinks the repair.** `function pair_make(n: i64) -> Pair acquires
slot_close` parses, checks, emits, counts, and the program exits **0**.
`-> Pair borrows` likewise exits **0**. Nothing in the parser, the emitter or the
runtime is missing. What is missing is only that the RULE does not demand the
mark.

**The surface CANNOT express a record carrying two handle types.** `record Two`
holding `s: Slot` and `c: Conn`, both consumed by some `extern`, marked
`two_make(n: i64) -> Two acquires slot_close`: the program compiles, runs, and
aborts **134** with the count at -1, because one mark produced one increment and
two `consumes` ran. The counter in `runtime/parts/alloc.c:103` is **one global
`_Atomic int64_t`**, not a per-type table.

**A recursion over the declaration graph must carry a visited set.**
`sized.order_and_cycles` refuses a record cycle at `selfhost/checker.hero:46`,
but it does NOT stop `acquiring` at `checker.hero:74`: a program holding both a
record cycle and an unmarked producer emits `error[no_size]` **and**
`error[unmarked_handle_producer]`, measured. So the pass runs on a cyclic
declaration graph that a user can write, and an unguarded walk would not
terminate. `crosses_the_boundary` recurses safely only because it walks the
interned TYPE table, which is acyclic by construction.

**A fixed array's length is written**, so the number of handles a returned type
reaches is a compile-time constant: `Slot[4]` reaches four.

## The three resolutions the sitting must rule on

- **R1 — what the rule SEES.** Reachability to any depth through group-record
  fields and fixed-array elements, with a visited set, versus stopping at one
  level. One level is a premise about the world in the sense
  `.claude/rules/module-shape.md` refuses.
- **R2 — what the diagnostic SAYS** when the handle is two fields down: the
  path to it (`Outer.p.s`), or only the producer's name.
- **R3 — the multi-handle record**, which the current surface cannot express.
  Candidates: refuse a returned type reaching more than one handle type (a
  refusal owes the program fact that would make it wrong, CLAUDE.md § 12);
  allow the mark to repeat; or name the field in the mark.

## Rules that bind you

Build in a COPY — `cp -r` the tree to your own scratch directory and
`rm -rf target build` after copying. Never `archive/bootstrap-rs/`, which
nothing builds. The seed builds in about 3.4 s; rebuilding from `selfhost/` is
about 20 minutes and will kill you on the watchdog. A claim enters your report
only after the command that settles it has been run; where you could not run it,
write *unrun* in your own words.
