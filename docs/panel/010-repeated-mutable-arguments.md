# Panel 010 — the same variable passed to two `@` parameters

Date: 2026-08-04. Trigger: the llm-ergonomist found it while judging panel
009 and it went on the watch list — `shift(a @ n, b @ n)` compiles under
both readings of §4.8 and yields different answers, with nothing in
design.md deciding which. Three judges convened (compiler-engineer,
llm-ergonomist, historian); the ffi-pragmatist and spec-warden have no
standing on a checker rule that costs zero spec tokens and touches no ABI.

## Proposal (as put to the judges)

- **E** — passing the same variable to two `@` parameters of one call is a
  compile error, detected at M3c.
- **F** — legal; copy-out specified left-to-right, last write wins.
- **G** — legal; copy-out specified in reverse argument order.

## Verdicts

| Judge | Verdict | Cost/delta | Prediction | Condition |
|---|---|---|---|---|
| compiler-engineer | **approve E**, object to F and G | E: ~30–40 lines in `types/` at M3c, inside the routine that already walks arguments for the same-typed-argument rule; **0 lines** in desugar/ir/ownership/descriptors/backend; 0 spec tokens. F/G: few lines but they make copy-out order *observable semantics* — the ownership pass must guarantee it on all six exit edges, and it enters the M8c fixpoint | at tag `m3c`: ≤40 added lines under `resolve/`+`types/`, exactly 0 under the back half; baseline shows the form in <1% of completions, so E's false-error cost ≈ 0 | rule stated **root-based** (place expressions sharing a root binding), E stays out of spec v1, DESIGN-LOG records the emitter invariant E buys |
| llm-ergonomist | **approve E**, **veto F and G** | silent wrong-output count over three realistic programs: F = 3, G = 3, **E = 0** | belief probe: ≥60% of models answer the *reference* reading unprompted, <15% answer F's; under an E spec ≥80% answer "compile error". Task level: F/G produce ≥1 silent wrong-output per 10 tasks in that bucket; E converts them to compile errors with ≥90% retry-with-diagnostic success | E stated over **places**, not variables: static error where decidable, **abort** where the index is dynamic |
| historian (advisory) | **approve E** | n/a | a same-*name* check holds for the simple case and fails on the first path pair (`arr[i]`/`arr[j]`, `v.a`/`v.b`) that is only sometimes the same object — void if `@` arguments are restricted to simple names | would withdraw only on a sourced language that specifies copy-out order and has field evidence it is not a bug source; searched, found none |

## Findings that changed the proposal

**The motivating example was degenerate.** `shift(a @ n, b @ n)` swapping
two copies of `n` prints 5 under *all three* readings — it discriminates
nothing. The discriminating shape is an **asymmetric per-parameter update**:
`a @ a + 1` / `b @ b + 10` gives 16 (reference) / 15 (F) / 6 (G). Corollary
worth keeping: a *swap* helper — the first thing anyone reaches for — is the
one case that can never expose the bug. Any document arguing this point must
use the asymmetric example.

**The spec already contains the sentence E makes true.** The ergonomist,
reading only the spec, reports that the question never occurred to it: "no
aliasing exists anywhere" is a universal sitting beside `b = a` and it reads
as a promise that interference is not a category in this language — so the
reader stops checking. "Copy in, copy out" is four words inside a bullet
about marking, and installs only the mechanics. **Under F/G the universal is
simply false**: two copy-outs to one place *is* aliasing, of the destination
rather than the source. E therefore does not add a restriction; it is the
precondition that makes an existing sentence true.

**F is not even well-defined.** Same-typed parameters are exactly the ones
for which §4.9 makes named arguments mandatory — so call order is free, and
"left-to-right" is ambiguous between call order and signature order.
`count_line(lines @ total, words @ total, s)` and the same call with its
named arguments reordered would be *different programs*. Reordering named
arguments is the edit every programmer believes is safe.

**Ada arrived at E after thirty-three years of the alternative** (historian,
verified at the source, correcting this panel's brief). Ada 83–2005 defined
copy-back but left its order arbitrary (RM 6.4.1(17)); **Ada 2012 made the
overlapping call illegal** for elementary types (RM 6.4.1(6.15/3–6.16/3),
"known to denote the same object"). The stated reason, AARM 6.4.1(6.z/3):
*"Such dependence is usually a bug, and in any case, is not portable to
another implementation (or even another optimization setting)."* That is
§1.1 in period costume. Fortran is the counter-example that shows the cost
of not deciding: the standard makes aliasing the programmer's error, and
gfortran picks by-reference or copy-in/copy-out *per argument shape* — the
divergence is not cross-vendor, it is cross-expression.

**No precedent exists for F or G.** The historian could not source a single
shipping language that specifies copy-out order; Ada explicitly declines to.
Swift — whose `inout` is specified as copy-in/copy-out with by-reference as
a permitted optimisation, i.e. exactly Heroes' position — forbids overlap
instead (SE-0176: "passing the same variable twice means performing two
overlapping write accesses… which therefore conflict"). Hylo, already in
Heroes' ancestry, does the same and ships the repair in the diagnostic:
*"pass `v.copy()` as the second argument instead."*

**E buys the backend an invariant.** The engineer's correction: two
`int64_t*` to one object is *not* a strict-aliasing violation, so
`-fno-strict-aliasing` is irrelevant here. The real hazards are that the
natural lowering of `@` on an aggregate is a pointer into the caller's slot
— under which the generated C silently implements *reference* semantics and
§4.8's "copy in, copy out" becomes a lie — and that `restrict` on `@`
parameters (the obvious emitter move, and what Swift does in SIL) would make
an aliased call genuine UB. E makes non-aliasing of `@` parameters a
guarantee the emitter may rely on. F/G ban `restrict` and mandate
temporaries forever.

## Resolution — adopted, `provisional — author ratification pending`

Per CLAUDE.md § Panel (asynchronous): the conservative resolution is
adopted and the ratification is queued.

1. **Amendment E, stated over places.** Two `@` arguments of one call whose
   place expressions share a **root binding** are a compile error. Because
   Heroes has no references and no aliasing (§4.10), every place has exactly
   one root, so a root comparison is a *complete* may-alias test — no
   dataflow, no borrow checker. `shift(a @ p.x, b @ p.x)`,
   `shift(a @ p, b @ p.x)` and `shift(a @ xs[0], b @ xs[0])` all fall out.
2. **It over-rejects `f(a @ xs[i], b @ xs[j])`** with `i ≠ j`, and that is
   accepted: distinct-index mutation is not on the closure list (a `sort`
   takes `(@xs: [int], i: int, j: int)`, not two element cells), and the
   over-rejection is relaxable later without breaking any existing program.
   Rust is conservative in exactly the same way (E0499 rejects
   `mem::swap(&mut arr[0], &mut arr[1])`).
   The ergonomist's dynamic-index **abort** is therefore *not* adopted now:
   with the static rule rejecting the whole family, there is nothing left to
   trap at runtime. It returns if 2 is ever relaxed.
3. **The diagnostic carries a `certain` fix** — pass a copy for the second
   argument — following Hylo's wording.
4. **Lands as an error, not a warning.** Swift needed a warning phase
   because code already existed; at M1 no Heroes program does.
5. **Zero spec tokens.** Panel 009's allocation rule binds: tokens that
   prevent a *loud* compile error are not the purchase to make at 2000. The
   rule lives in design.md §4.8 and in §4.17's diagnostic.
6. **Implementation at M3c** (`docs/ROADMAP.md`), in the routine that
   already builds the label→argument map for the same-typed-argument rule.

## Predictions to score

- engineer: at tag `m3c`, ≤40 lines under `resolve/`+`types/` and exactly 0
  under `desugar/`/`ir/`/`ownership/`/`descriptors/`/`backend/`.
- ergonomist: under E, ≥80% of models answer "compile error" on the belief
  probe; the silent-wrong-output bucket for this class goes to 0.
- historian: the first real friction will be a *path pair* that is only
  sometimes the same object — i.e. pressure to relax resolution point 2.
