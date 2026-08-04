# 006 — M5a: scalars run

Milestone M5a · 2026-08-04 · panel 020.

## 1. Goal

The compiler becomes a compiler. `int`/`bool`/`if`/`while`/functions/`print` →
C11 → clang → a native binary, from the M4 IR. `heroes build` compiles,
`heroes run` executes, `--emit-c` stops at the C, and the double-emit determinism
diff lands and stays green forever (CLAUDE.md §7).

Module born: `emit/`, eight files, **1166 non-test lines**, none over 300.
`cli.rs` split three ways. Two golden directories born (`emit/`, `unsupported/`)
and one filled (`run/`). **363 tests** (was 331): 326 crate, 13 golden harnesses
over 55 cases, 24 surface.

Three outcomes:

1. **`heroes run examples/gallery/00-first.hero` prints `20`.** Five milestones of
   frontend, and the thing that took the longest afterwards was not code
   generation — it was deciding what the compiler should *say* about the eleven
   gallery programs it cannot compile yet.
2. **Zero clang warnings across the whole corpus at both `-O0` and `-O2`**, with
   warnings forwarded on success so that they could fire. That number is what
   promoted `-Wconditional-uninitialized` to `-Werror=` in the same step that
   measured it — panel 020's falsifiable disposition, resolved by a run rather
   than by a discussion.
3. **Panel 020 arrived with two vetoes and both were lifted by a change**: one
   table row (`extern` waits for M7, because §4.19's guarantee is the `#include`
   and an emitter-invented prototype is verified by nothing) and one exit code
   (an unsupported form is **1**, not 2).

## 2. What surprised

**The exit code was the most consequential character in the milestone.** A
correct program that this compiler cannot yet compile is not a wrong program, so
the tempting answer is exit 2 — "the tool could not do its job", which is
literally true. Two judges, given different inputs, converged against it. The
llm-ergonomist was handed the message and asked what it would do next: its first
action was `heroes --version && heroes doctor`, its second `grep -rn "M5b" .`, and
its third a sentence to its user saying the toolchain looked broken — which it
reported, verbatim, as false. The historian went looking for precedent and found
GCC's `sorry()`, which has printed `sorry, unimplemented:` since version 2.5.8,
and then read to the end: `if (sorrycount) exit (FATAL_EXIT_CODE)`, and
`FATAL_EXIT_CODE` is `EXIT_FAILURE`, which is **1**. The same source that looked
like an argument for 2 settles on 1.

**A milestone identifier in a message is a locality violation.** `(M5b)` was in
the proposal, written by the same hand that wrote the locality rule. It resolves
only in `docs/ROADMAP.md`, which the reader does not have — and §4.17's standard
is everything needed to act without opening another file. Both the ergonomist and
the warden struck it independently, from different sections. What ships names the
*capability*: `text (`str`) is not emitted yet`, with a note saying what the
backend does emit and one saying that no change to this file will help. The
milestone stays in the gate table, in source, where the author reads it.

**"`#line` on source-line change" is not implementable as written.** `#line N`
anchors the *next* line and C then auto-increments, so a Heroes line that lowers
to K lines of C drifts by K−1 — and the further into a function you read, the
further off the map is. The comparison has to be against what clang currently
believes, which makes a line-counting writer a structural requirement rather than
a detail. The frozen target had the bug: `tools/spike/01-first.c` advertises
`00-first.hero:2` in its own header, and clang reports `:6`, which is blank, with
the *C* text printed under the `.hero` name.

**Two vetoes, two changes, no arguments.** The ffi-pragmatist compiled nineteen C
files and found that `extern function labs(x: int) -> int` lowers today with no
header anywhere: `abs(-2147483649)` returned `2147483647` at exit 0 with one
non-fatal warning, and `sqlite3_open` was accepted in **total silence** under
`-Weverything -pedantic`. With the real header present both are `error:
conflicting types`. §4.19's mechanism is the `#include`, not the declaration —
which means `extern` had to wait for M7, and the veto cost exactly one row in a
table that was being written anyway.

**A directory called `runtime/` is a security-shaped hazard.** The same judge put
a decoy one in the working directory: the generated C compiled with zero warnings
under `-Weverything` and printed `0x1e` where `30` was expected. The fix is one
`_Static_assert` on `HERO_RUNTIME_ABI` per translation unit, which turns silence
into `error: use of undeclared identifier`.

**The panel's own prediction scored on both halves.** The compiler-engineer
predicted `emit/` would close at ≥700 non-test lines in ≥5 files and that
`cli.rs` would exceed 340 lines or be split. It is 1166 in 8, and `cli.rs` was
split. The spec-warden predicted the amended spec would measure exactly 2155 /
2096 / binding 2155 / spread 59 / headroom 845. It does.

**Panel 006's amendment had been sitting unlanded for four milestones**, and the
milestone where `print`'s bytes first become observable is the milestone where
that stops being harmless: a model authoring `run/*.expected` from the spec cannot
tell `ab\n` from `a b\n`. Landed at **+16 measured** — against panel 006's own
"+15 est" for a text that measures **+39**. The word heuristic was ~2× low for the
third time on the record.

## 3. What broke and why

**Symptom** · `-Wunused-label`, on a correct program. **Cause** · an `if` whose
both arms `return` leaves a join block with no predecessors; the emitter walked
the block list and printed every label. **Fix** · a block is emitted only where
some edge targets it, and unreachable blocks are omitted *entirely* rather than
merely unlabelled — C is physical, and an unlabelled block would fall through
from the one above. The warning stays on, which is the point: it is now a live
check that the emitter's `goto` edges agree with the IR's `preds`.

**Symptom** · `error: variable has incomplete type 'void'`. **Cause** · `$t0` is
the unit value of every function and `Function::values` is dense, so the obvious
prologue walk writes `void t0;`. **Fix** · a unit-typed temporary is never
declared and never named, and a `Return(Some(v))` whose value is unit is
`return;`.

**Symptom** · `error[needs_label]` on two `run/` goldens the author wrote by hand.
**Cause** · not a defect: §4.9's same-typed-argument rule doing its job on
`count_down(@n, @seen)` and `nested(true, false)`, where the two arguments are
interchangeable at a glance. **Fix** · label them, and say so in the case's own
comment — the rule earned its place in a test written to exercise something else.

**Symptom** · `missing_return` fired on design.md's appendix. **Cause** · not a
defect either: `function simplify(e: Expr) -> Expr` has `???` for a body, and a
hole falls off its end by construction. **Fix** · §4.16's file-wide hole
exemption, the same one the unused rule takes. The new class had found the one
program in the repository designed to exercise exactly this.

**Symptom** · the dominance invariant rejected `assert 1 + 1 == 2`. **Cause** ·
the invariant was written as "a temporary is read only in the block that defines
it", which the `assert` lowering correctly falsifies — it computes both sides in
the test block and reads them in the abort block. **Fix** · the true statement,
dominance, by the textbook fixpoint. The wrong version would have passed every
test in the suite until the day somebody wrote an `assert`.

**Symptom** · the mutated-corpus invariant over the backend emitted **zero**
mutants. **Cause** · eleven of the gallery's twelve programs are outside the M5a
subset, so the emitter half of the invariant was asserting nothing while looking
thorough. **Fix** · mutate `tests/golden/run/` as well, and record the hole in the
module doc — a coverage gap a test cannot see is the same shape as the verifier
that had 257 lines and no evidence any of them could speak.

**Symptom** · four spikes silently changed their output. **Cause** · `print`'s
newline moved from `hero_print_int` to `hero_print_end`, and the spikes' expected
output lived in a **C comment**, where nothing could check it — spike 04's three
distinguishable results collapsed to `10-42`. **Fix** · each spike gains a checked
`.expected` and a test that compiles it against the current runtime, which also
makes spike 04's frozen ABI a thing that is verified rather than remembered.
