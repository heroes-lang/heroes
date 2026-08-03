# Open panel sessions — awaiting the author's decision

Each has a recommended resolution from the panel-000 review. The full session
(five judges, differentiated inputs, per `/panel`) runs when the author picks
it up; the decision is then applied to design.md and spec/ directly, with this
file and DESIGN-LOG updated. design.md carries an inline `OPEN QUESTION`
marker at each affected section.

## 002 — `ok(x)` constructor and `fail`'s typing

**Problem.** design.md's acceptance program needs `T`-typed expressions where
`T?` is expected at six sites (§Appendix L1838, L1852, L1864, L1865, L1878,
L1886), but §4.3 says "no implicit conversions" and Part 6 rejects them
permanently. Root cause: `fail(code, msg)` and plain values both need
return-type-directed typing that §4.12's inference cannot provide.

**Recommended resolution.** Add `ok(x)` as `fail`'s symmetric twin; both `ok`
and `fail` are checked *against the expected type* (bidirectional ⇐ mode).
~+8 spec tokens, zero new vocabulary (`ok`/`err` arm names already exist).
Avoids the `T??` ambiguity implicit promotion would create under
monomorphisation. The acceptance program changes at the six sites.

## 003 — statement-position rule (replaces the `discard` idea)

**Problem.** `xs.push(4)` as a statement compiles and silently does nothing
useful (`push` is pure under value semantics, §4.10) — a plausible silent
error in exactly the class the language exists to kill.

**Recommended resolution.** A non-`()` expression in statement position is a
compile error; the diagnostic dictates the fix (`_ = expr`, or use the value).
Zero keywords, ~+8 spec tokens; subsumes Nim's `discard`.

## 005 — Principle 0 formal adoption (§1.0)

**Problem.** The stopping rule ("v1 is done when the language compiles
itself") and the burden of proof (compiler-need OR measured thesis effect)
were adopted in the approved plan, and §1.0 now states them in design.md. The
open half is the **closure-list audit**: confirming the list in §1.0 is the
real v1 surface, and re-reading Part 7 under it.

**Recommended resolution.** Ratify §1.0 as written; audit the closure list at
the M6 checkpoint.

## 006 — map iteration order and `print`'s contract

**Problem.** (a) `for k in m` needs a specified order — determinism is a
fixpoint requirement, and design.md never specifies it. (b) `print(c, " = ",
v)` mixes `str` and `int` (§Appendix L1984): the document's one implicit
conversion, unspecified.

**Recommended resolution.** (a) Insertion order, fixed hash seed. (b) `print`
is a compiler-known form (not a user-callable variadic function) accepting a
comma-separated list of `str`/`int`/`f64`/`bool` values, each rendered by its
canonical `.str()`; no other variadics exist (§4.9 stays true).
