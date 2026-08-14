# Panel 051 — the assertion asks what an `extern` returns, and never what it accepts

**Convened** 2026-08-14, by the third CI leg. **Lane**: full, five judges.
**Status**: `provisional — author ratification pending`.

## The proposal, verbatim

Under the project's exact compile flags, a C function declared `int narrow(int n)`
called with an `int64_t` of `4294967301` prints **`5`**. Silently. No warning of
any kind. `spec/heroes-spec.md` § FFI said *"clang checks every signature and
constant against that header, so a wrong FFI type is a compile error"* — true
about return types, false about parameters. CLAUDE.md §12 says spec beats
compiler: the compiler has the bug.

Options put: **A** `-Werror=shorten-64-to-32`, mapped to a diagnostic on the
author's line. **B** a `_Static_assert` per whole signature via
`__builtin_types_compatible_p`. **C** declare a parameter at the header's own
width (`i32` for a C int). **D** repair the sentence to claim only what is true.

A fifth option, **E**, was raised by the historian mid-sitting and measured by the
chair: a function-pointer probe, `static R (*const p)(P…) = f;`, which clang ≥16
rejects as an **error with no flag at all**.

## The verdict table

| judge | verdict | rests on | measured | prediction | condition |
|---|---|---|---|---|---|
| **compiler-engineer** | adopt C · **veto A-as-mapped, veto B** | §1.1, §1.7; CLAUDE.md §7, §11 | C is **zero lines of Rust** — it already works; `abs(n: i32)` runs, emits `HERO_RET_I32`, and the frontend refuses the narrowing with `type_mismatch`. A costs ~60–75 lines, ~50 of them in `emit/ffi.rs`, which was **221** lines at the §11 sweep and is **398** after panels 048–050 | if A's mapped class lands, `emit/ffi.rs` passes **440** lines and at least one golden cannot carry its `#~` annotation, because clang reports a column and `#line` carries file and line only. Checkable at M-ffi-ladder close | withdraw the veto on A if clang emits the callee's name in the message line itself, so `declaration()` has something to gate on; or if A lands as a plain warning through the zero-warning check with no class in `ffi.rs`, and `emit/ffi.rs` is split below 300 first |
| **llm-ergonomist** | adopt-with-condition (C's prose) | §Types line 56, §FFI | wrote `abs(n: i64) -> i64` from the spec alone, at **high confidence**, and named why: the section's worked example binds a C `int` to `i64` **twice**. Given the parameter sentence it wrote `i32` unambiguously | on 5 fresh FFI tasks binding a header containing a C int: correct-width rate **≤20%** under the old text, **≥80%** under the new one with the example corrected. The gap between "prose added" and "example corrected" is the falsifiable half | the section must not state a rule its own example violates |
| **spec-warden** | adopt **D**; **object to C** | §12, §1.0, §1.6; design.md Part 7 item 10 | every option measured, not estimated: **A 0 · B 0 · D 0 or −1 · C +19 to +36**. Demonstrated the false sentence with a running program (`putchar(c: i64)` given `4294967361` prints `A`, exit 0, zero diagnostics). Found a real removal: **R1, −8**, backed by a *scored* prediction | `SPEC_TOKENS` stays where the sitting leaves it; if C is adopted the ledger row must name which CI platform the `i32` spelling is wrong on | D's wording must claim no more than `HERO_RET_*` gives. **§12/panel 039**: what would make its refusal of C wrong is a ladder program binding a `short`, `float` or `unsigned` parameter, getting a wrong number A cannot catch, writable in one spelling on all three legs |
| **ffi-pragmatist** | adopt **A + C**; **veto B** | §4.19, §1.11 | counted clang's own AST over three real headers: **267 of 643 bindable entry points (42%)** take at least one non-64-bit parameter. A catches **251 (94%)**. Blast radius over **501** generated units: **89** trip A, every one an FFI example, **zero** non-FFI. B is **unsatisfiable for `ptr`** — `void *` is not compatible with `struct sqlite3 *`, so §4.19's own two-line example fails and every SQLite binding in the repository becomes uncompilable | rung 4 is the rung A does not reach: after one edit `examples/curl/` compiles clean under A, **and `curl_easy_setopt`'s variadic argument still takes a 64-bit value with zero diagnostics under any clang flag**. sqlite needs exactly two edits and no shim | A must not ship alone (it would point at no correct program); the sentence must name what stays uncovered; **A must be gated per target or measured on Windows first** — the same binding is clean on LP64 and an error on LLP64 |
| **historian** (advisory) | approve, with a correction to the framing | — | **cffi API mode has exactly this hole and has never fixed it in over a decade**: *"functions taking or returning integer or floating-point arguments can be misdeclared … the C compiler handles the difference"*, while a pointer mismatch is an error. **GHC had the check and removed it** (≤6.8.3) for three reasons — native code generator, FFI-spec conformance, cross-module inlining — **none of which apply to Heroes**. Rust's `improper_ctypes` does **not** fire on a wrong width; RFC 3484's own words: *"this proof cannot be checked by the compiler"*. rust-lang/libc ships the exact whole-signature check as a **separate CI suite**, not per-compilation. Found the LLP64 bug class in the wild: Linux commit `25e11700b54c`, bugs.python.org #30286, node-ffi #402 | the function-pointer probe rejects `abs(n: i64)` as an error on clang ≥16 with no flag | would change its reading if shown a shipped language that emits a per-compilation whole-signature assertion **and kept it** |

## What the chair measured, and it decided the sitting

**Option E is refused, and it is refused for a reason this project has already
paid for once.** The probe is precise: six of six synthetic cases pass, variadic
and enum *returns* included, and against the real headers it finds the same four
true positives `-Wshorten-64-to-32` finds. Then, at the header's own widths, two
of the four still fail:

```
error: incompatible function pointer types
   … 'CURLcode (CURL *, CURLoption, ...)'
   … 'const char *(CURLcode)'
```

An **enum parameter** cannot satisfy a function-pointer type at any width,
because C's pointer compatibility does not accept the enum's compatible integer
type where `_Generic` does. That is panel 042's defect exactly — *"the first
version of this macro refused every enum-returning C function in existence, which
is most of libcurl, OpenSSL and raylib"* — arriving on the parameter side. The
option that looked best on paper, and is the only one with a shipped precedent,
is the one that repeats a defect already in this record.

**And the asymmetry the spec now states is principled, not a convenience.** It was
found by moving the two examples rather than by reasoning:

- `examples/sqlite/` moved to the header's own widths in **two edits and no
  shim** — `length: i32`, `column: i32` — plus one call-site spelling, because
  `-1` is contextually typed and `0 - 1` is not.
- `examples/curl/` moved its **parameters** and could **not** move its
  **results**. `curl_easy_setopt` returns `CURLcode`, whose compatible type is
  unsigned; `HERO_RET_INT` accepts every unsigned type narrower than 64 bits
  because they all fit an `i64`, and `HERO_RET_I32` correctly refuses, since an
  `unsigned int` does *not* fit an `i32`. The compiler is right and the diagnostic
  is the one it should give.

So: **a result may be wider than C's; a parameter may not.** That is the sentence,
and it is true because the direction of the conversion is what decides it.

## Disagreements, unsmoothed

**The warden refuses C on a row whose own falsifier a second judge measured as
already satisfied.** design.md Part 7 item 10 says the program that would make it
wrong is `strlen`, and the ffi-pragmatist ran it: `extern function strlen(s: cstr)
-> u64` **compiles and prints 5**. Panel 041's *"no result type exists to correct
it to"* and that row's falsifier were both closed by panel 042's widths, and the
text has not caught up. Under §12 — measurement beats opinion, and a refusal is
held to a feature's burden of proof — the row is now unsupported and is queued for
the author.

**The warden also priced C as buying "no compile error at all".** The
compiler-engineer measured otherwise: `abs(n: i32)` given an `i64` is
`type_mismatch` in the frontend, before any C exists. Both statements are true of
different things — C compels nothing (nobody must write `i32`), and C *enforces*
everything once written. The sitting's resolution takes the second half and
concedes the first: **the spec now says which one to write**, which is the only
lever a document has.

**The compiler-engineer and the ffi-pragmatist split on A.** The pragmatist
measured A landing on the author's line already, through `#line`, with zero
false positives over 501 units. The engineer's objection is not about the line
but about §7: a clang failure is exit 2 *"the compiler is wrong"* unless
`emit/ffi.rs` recognises it, and `-Wshorten-64-to-32`'s message carries **no C
name** to gate on. Both are right; A is therefore queued rather than landed, and
what it needs is a gate — the obvious candidate, which no judge named, is the
**file** clang reports: a `.hero` path means the author, a generated `.c` path
means the compiler. That is a fact about the value in hand rather than a premise
about messages (CLAUDE.md §11), and it is the decision the author is asked for.

## Resolution — provisional, author ratification pending

**Landed.** **D**, at **+34** measured: § FFI now says *"clang checks every result
type and constant against that header, and a result may be wider than C's. A
**parameter** is declared at the header's own width — `i32` where C says int —
because clang narrows a wide one in silence."* Paid as §1.0 compiler-need under
§12, with the registered-prediction branch of panel 012 (the ergonomist's
first-try width rates, scored at the next ladder rung). The warden's **−8**
removal is deliberately **not** spent: it belongs to panel 037's live clause, and
funding this sitting from another's unscored purchase is what panel 041 refused.

`examples/sqlite/` and `examples/curl/` moved to the new rule, and
`tests/golden/check/ffi-parameter-width.hero` is the case that makes it fire.

**Refused.** **B**, two independent vetoes with two different measurements. **E**,
by the chair's measurement above.

**Queued for the author** (`docs/debrief/DECIDE.md`): **A**, with the file-based
gate as the proposed answer to the engineer's veto and the per-target measurement
the pragmatist requires; the **stale Part 7 item 10 row**; and the three holes no
option on the table closes — **variadic arguments** (reachable today, in
`examples/curl/`), **`float` parameters** (`f32` does not exist; 19 raylib entry
points are bindable-but-silently-wrong), and **`size_t` signedness** (A blind, C
fixes it).

**A fourth, found while landing this:** `n.to_i64()` on an `i32` yields `i64?`.
The widening cannot fail, and every narrow-width binding therefore composes
through a `.must()` that asserts nothing. That is the ergonomic cost the
pragmatist booked — *"251 entry points' worth of call sites"* — and half of it is
recoverable.

## Predictions to score

| judge | prediction | checkable at |
|---|---|---|
| compiler-engineer | A's mapped class puts `emit/ffi.rs` over **440** lines and breaks one `#~` annotation on a column | M-ffi-ladder close |
| llm-ergonomist | first-try correct-width rate **≤20%** before, **≥80%** after, with the example correction worth more than the prose | next first-try harness run |
| spec-warden | with A landed, exactly **2** of 113 units exit 1, **111** exit 0, zero failures on a generated-C line | if A lands |
| ffi-pragmatist | rung 4's variadic argument still takes a 64-bit value under **any** clang flag | M-ffi-ladder close |
| historian | the function-pointer probe is an error on clang ≥16 with no flag | **scored now: true**, and it is why E was measurable at all — and E was refused anyway, on the enum parameter |
