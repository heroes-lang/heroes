# Panel 052 — the C scalar vocabulary an `extern` cannot spell

**Convened** 2026-08-14, on the author's instruction to close panel 051's three
queued gaps: *"implement f32, size_t and varargs and whatever else it needs — the
FFI is the most important thing."* **Lane**: full, five judges.
**Status**: `ratified — 2026-08-14, author decision`.

**One irregularity, recorded rather than smoothed:** the compiler-engineer's first
run was cut off mid-flight by a machine sleep and produced no verdict. It was
re-convened on a narrowed brief — the code that by then existed, rather than the
proposal — and its second run is the one below. Its measurements therefore had
sight of four other judges' findings, which the panel's own design normally
prevents. The verdict it returned was a **veto**, so the asymmetry did not soften
it; but a judge that reads late is not the same instrument as a judge that reads
first, and this line is here so a later reader knows which one spoke.

## The proposal, verbatim

**A** — `f32`, which does not exist, so a C `float` parameter is bound `f64`.
**B** — a variadic spelling, which does not exist, so `examples/curl/main.hero`
declares `curl_easy_setopt`'s third argument as fixed.
**C** — `size_t` and signedness, which `-Wshorten-64-to-32` is blind to.
**D** — the parameter check itself, panel 051's queued item, gated on the file
clang reports.

## The verdict table

| judge | verdict | measured | prediction | condition |
|---|---|---|---|---|
| **llm-ergonomist** | adopt-with-condition: `f32` at the boundary with an explicit `.f32()`; **veto `...`** | Given only the spec it bound `SetMasterVolume(volume: f64)` *"in about four seconds and felt fine"* — and named why that is the defect: **the width rule is unsatisfiable for `float` and nothing says so**. On variadics: *"`...` suspends the spec's strongest FFI protection exactly where C punishes hardest, and every mistake it permits compiles"* | ≥8/10 fresh models write `volume: f64` under the current spec; silent-error rate on float parameters 100% → 0% under a boundary type | the boundary conversion must be **written**, symmetric with `s.cstr()`; state the three edges (return position, `@`, `[f32]`); variadics need a rename clause or the alternative does not exist |
| **spec-warden** | **approve D**; A, B, C **wait** | Every option measured: A1 +39 · A2 +34 · B +72 · C +16 · **D −1**. And it falsified A's own justification: `void set_volume(float)` bound `f64` and given `0.1` delivers `0.10000000149011612`, **byte-identical to a C caller**. Found a new removal, **R2 −12**: `to_i64` is listed twice | with D landed and no spec addition, **0** of 45 programs change output for want of `f32` | A1 returns the day a program needs a `float` **value** Heroes must hold |
| **ffi-pragmatist** | approve C and D; **object to A**; approve B's rule and **reject its premise** | The sitting's central measurement: a variadic through the **real prototype** returns `66` with the URL intact; through a **fixed** prototype declaring the extra argument it returns `8393946705` with the URL empty **on arm64** — and is **correct on x86-64**, with zero warnings on both. But `heroes build --emit-c` shows the call going through curl's own prototype. **So the binding is not accidental: §4.19's refusal to re-declare is load-bearing ABI machinery.** `-Wsign-conversion`: 180 units, delta 6, all FFI, runtime clean. The remaining wall: **356 of 1159 entry points (31%) pass or return a struct by value** | `M-binding-fidelity` will not raise raylib past 254 of 600 bindable entry points, because 346 are struct-blocked | **standing veto on any variadic implementation that emits a re-declared prototype**, because it is silently correct on x86-64 and garbage on arm64 — the exact defect class this language exists to kill, and the Linux CI leg would not see it |
| **historian** (advisory) | approve A2, approve B with a framing correction, approve D with a narrowing | C 6.7.6.3p15 makes the fixed declaration formally UB; Apple's own ABI page states the stack rule; **three shipped projects bled on exactly this target** — Erlang/OTP PR #2708, Terra #508, CPython #92892. cgo, GHC's `ccall` and Swift all **refuse** `...` rather than approximate it. **Dart's `dart:ffi` `Float` is A2 exactly, shipped**: *"not constructible in Dart code and serve purely as a marker in type signatures"*. And on D: `cmd/cgo/gcc.go` already made this argument — *"we used to look at specific warning or error messages here, but that tied the behavior too closely to specific versions of the compilers"* | the fixed-arity curl binding fails on arm64 | **narrow the gate**: *"a `.hero` path"* is a premise about the world, since this emitter lowers the author's own call sites under their `#line` too |
| **compiler-engineer** | **veto** the mechanism as built and `f32` at `@`; object on the gate and the seam | Object files **byte-identical** with and without the probe at `-O0` and `-O2`; +3 ms per unit. `heroes_spelling` proposed `u32` for an `unsigned long`, **64 bits on two of three CI legs**. `location()` matched `<heroes library>`, whose own module doc forbids exactly that. `emit/ffi.rs` reached **550** lines. And, outside its brief: `-Werror=sign-conversion` made a **three-line program with no FFI** exit 2 | at M-ffi-ladder close, sqlite emits 12 clang warnings where its own comment claims four | emit the probe **only for an extern with no call site**; `f32` at `@` is core, because `getf(&h0_v)` passes the author's own slot |

## What the chair measured, and where a judge was answered rather than followed

**The compiler-engineer's veto is declined, and the ground is a measurement.** Its
condition — emit a probe only for an *uncalled* `extern`, since *"the call site
already warns"* — is true about the warning and false about the diagnostic. With
the probes stripped, the same defect is:

```
internal error: compiling the generated C failed:
  wrong.hero:5:18: error: implicit conversion loses integer precision …
      5 |     t2 = putchar(t1);
```

Exit **2**, the compiler blamed for the author's declaration, with generated C on
screen — the three things §4.17 exists to prevent. The probe's line is the only
report that carries a *declaration*, and `extern_at_line`, the gate that makes the
class safe at all, has nothing to match without it. Under CLAUDE.md §12 the
measurement decides.

**Its other four findings are folded in as fixes, not queued**, because every one
was right:

- `heroes_spelling` no longer answers for `long` or `unsigned long`. The first
  draft mapped them to 32-bit spellings, marked the fix `guess`, and *named the
  platform variance in the justification* — CLAUDE.md §11's failure mode written
  out longhand. A C type absent from the table now makes the class decline.
- `location()` refuses `<heroes library>`, whose own module doc says a clang error
  against it *"must not look like an error in a file the author can edit"*.
- The struct-return exit 2 is closed by one line, and the judge's reasoning was
  right while its constant was not: `__builtin_classify_type` returns **12** for a
  record and **5** for a pointer, so the positive test `== 5` replaces the negative
  list entirely. Verified six ways.
- `-Werror=sign-conversion` on a three-line non-FFI program was a **real
  pre-existing defect**, and the best thing the sitting found.

## The defect the flag found, which nobody was looking for

`print(n)` and `print(n.to_str())` printed **different numbers for the same
`u64`** — `18446744073709551615` and `-1`. Two readers of one decision:
`emit/abort.rs` chose the runtime entry point with an exhaustive match over
`IntKind` *and the comment explaining that `u64` needs `hero_uint_to_str` because
2^64−1 read as signed is −1*; `emit/builtins.rs` chose it again with `_ =>
"hero_int_to_str"`. The fix panel 042 records as landed had landed on one path
only, and stayed half-landed for two milestones behind a catch-all — CLAUDE.md
§11's *"put the fallback in the loud direction"*, read backwards.

## Disagreements, unsmoothed

**Two judges falsified two of the sitting's own headline claims**, and both were
mine, carried forward from panel 051:

- *"19 raylib entry points are bindable-but-silently-wrong for want of `f32`"* is
  **false**. A C `float` parameter bound `f64` is bit-exact, because clang converts
  against the real prototype; `0.10000000149011612` is simply what a `float` holds.
  Measured independently by the warden and the pragmatist. The genuine float hole
  is `@f64` against a `float *` out-parameter — **2** entry points — and it is a
  *pointer* problem, which no float type fixes.
- *"variadics work by accident"* is **false**, and the correction matters more than
  the claim: they work because §4.19 refuses to re-declare a signature, so the call
  compiles through the header's own variadic prototype. The pragmatist's standing
  veto exists to stop a future implementer "fixing" this non-bug — which would be
  silently correct on x86-64 and garbage on arm64.

**The ergonomist and the warden disagree about `f32`'s shape and are answered by
neither**: the ergonomist wants a written `.f32()` at the call, the warden priced
A1 and A2 at the same +19 and refused both, and the compiler-engineer's
measurement makes the question moot for now — `@f32` needs an `f32` **local slot**,
because an `@` argument is the address of the author's own variable, so the
boundary-only shape does not reach the only case that needs it.

## Resolution — provisional, author ratification pending

**Landed.** **D**, built as `emit/extern_probe.rs` + `ffi_parameter_type`, with
`-Werror=shorten-64-to-32` and **C**, `-Werror=sign-conversion`. Spec: **+1**, *"at
the header's own width and sign"* — `examples/curl/` produced the second half the
day the first landed, `CURLoption` being signed and `CURLcode` unsigned in one
header. Three pre-existing defects fixed with cases named after them.

**Waiting.** **A** — no shape survives all five briefs, and the case that motivated
it turned out not to need it. **B** — nothing to build: the ABI is already correct,
and what is missing is a *rename* so one C name can carry two arities, which is a
different feature.

**Queued** (`docs/debrief/DECIDE.md`): the rename clause; the variadic slot's
unchecked width; `emit/ffi.rs` at 550 lines against §11's 300, with the seam the
engineer mapped; the warden's **R2 −12**; and struct-by-value, which is now the
whole remaining wall at **31%** of every entry point in three real headers.

**design.md Part 7 item 10** is amended rather than deleted: the falsifier it names
has been produced (`strlen -> u64` prints 5), and the row survives on its closing
sentence — a Heroes width is a number, a C width is a question about the machine —
which panel 052 confirmed from the other side. It now carries a **new** falsifier,
stated so it can expire in its turn.

## Predictions to score

| judge | prediction | checkable at |
|---|---|---|
| llm-ergonomist | ≥8/10 fresh models bind a C `float` as `f64`; 100% → 0% silent-error rate under a boundary type | next first-try harness run |
| spec-warden | 0 of 45 programs change output for want of `f32` | M-binding-fidelity close |
| ffi-pragmatist | raylib does not pass 254 of 600 bindable entry points; struct-by-value blocks 346 | M-ffi-ladder rung 5 |
| compiler-engineer | sqlite emits 12 clang warnings where its own comment claims four | M-ffi-ladder close |
| historian | the fixed-arity curl binding fails on arm64 | **scored now: falsified** — it succeeds, and the reason is the one the pragmatist measured: the call goes through the header's prototype, not the Heroes declaration |
| chair (panel 051) | `emit/ffi.rs` passes 440 lines if the mapped class lands | **scored now: true**, and low — it reached 550 |

**Appended 2026-08-14, the same day.** The file finished at **573** and has been
split four ways, on a seam that is what a verdict is *about* rather than how long
the file got: `ffi.rs` keeps the markers, the dispatcher and the span finders
(160); `ffi_declared.rs` the classes where the header contradicts the declaration
(157); `ffi_narrowed.rs` the one where a parameter would narrow in silence (188);
`ffi_build.rs` the ones where the machine lacks what the group named (131). The
prediction did its job twice — it was right about the number, and the number was
the argument for the split.

## Ratification — 2026-08-14, by author decision

**RATIFIED as it stands.** D and C stay landed, A and B stay waiting, the Part 7
amendment stands, and everything queued stays queued.

What the yes settles: **two of the three things this sitting was convened to add
were removed by measurement, and that is the sitting's result, not its failure.**
The instruction was to implement `f32`, `size_t` and varargs. A C `float`
parameter bound `f64` is bit-exact — two judges got identical bits — so the case
that motivated `f32` does not need it. A variadic bound with a fixed arity is
correct, because §4.19 refuses to re-declare and the call goes through the
header's own prototype; the Heroes declaration never reaches C. What was left
after both were struck is smaller and true, and it is what landed.

The ffi-pragmatist's **standing veto is ratified with the rest**: no variadic
implementation may emit a re-declared prototype. The version that does is silently
correct on x86-64 and returns garbage on arm64, and the Linux CI leg would not see
it — which is the exact defect class §1.0 says this language exists to kill.
