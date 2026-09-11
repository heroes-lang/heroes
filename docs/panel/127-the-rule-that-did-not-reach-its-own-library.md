# 127 — the rule that did not reach its own library

Convened and closed 2026-09-11 · M-labelled-builtins · **full lane, five seats**
· status: **provisional — author ratification pending**
(`docs/work/DECIDE.md`)

Defect 025, filed the same evening out of panel 126's ergonomist seat, and the
author asked for the repair in one word.

## The proposal

`spec § 9` says that when two parameters of a signature share a type, the
arguments are named at the call site. The rule did not reach two of the
compiler's own built-ins. Measured before the sitting:

| what was measured | the answer |
|---|---|
| `fail("code_here", "message here")`, `xs.slice(1, 3)` | accepted |
| a user function with two `str` parameters, positional | refused, `needs_label`, twice |
| `range(1, 4)`, a built-in written in Heroes | refused |
| `memcmp(p, p, n)` through an `extern`, two `ptr` | refused |
| where the check reads a label | `selfhost/check/walk.hero:1541`, a span into a **declaration**; a `.runtime` built-in has none |
| built-ins whose parameters share a type | four: `fail`, `slice`, `range`, `write_file`; the last two are written in Heroes and were already checked |
| positional `fail(` call sites in the tree | **840** by the first count, **1022** once the calls that span lines were counted too |
| a swapped `fail` | compiles, exits **0**, puts the message in `e.code` |
| `heroes mutate --operator swap-args --survivors` | 1910 mutants, 1441 killed (80%), **367 survivors, 159 of them a swapped `fail(`** |

Route **A**: the rule reaches the two built-ins. Route **B**: the document names
the exception. Route **C**: something nobody listed.

## What the seats found that the proposal had wrong

- **The hole was not that the label is optional. It is that the label is
  unverified** (compiler seat, with running programs). `fail(nonsense: "a",
  garbage: "b")` was accepted and the labels ignored; `xs.slice(to: 3, from: 1)`
  **panicked**, because the values were used in the order written while the
  labels said otherwise. A declared function refuses both shapes.
- **`needs_label` alone would have killed none of the 159** (compiler seat).
  `selfhost/mutate/edits.hero:29` says the operator moves *label and value
  together*, so after the repair a swap is a **labelled** call in the wrong
  order, and the order half of the rule is what kills it. A route A shipping
  only the missing-label half would have moved 1022 call sites, regenerated a
  22 MB seed, and left the survivor count where it was.
- **Route B was a false sentence** (spec-warden, measured, and the warden vetoed
  it). *The built-ins the compiler provides are the one exception* is wrong
  about two of the four built-ins with two same-typed parameters: `range(1, 4)`
  and `write_file("out.txt", "hello")` are refused today. The truthful wording,
  naming `fail` and `slice` rather than the class, costs **+32 real** against
  route A's **+8**.
- **Route A as balloted was half-written** (spec-warden). § 6's shape line said
  `fail(code, msg)` without colons, the very form the repair makes an error, so
  the document would have carried two conventions for one rule.
- **The certain fix on a wrong label kept the defect it named** (compiler seat,
  with a running program). `copy_text(to: "b", from: "a")` was refused
  `wrong_label` with a fix marked `certain`, and applying it renamed the label
  to give `copy_text(from: "b", to: "a")`: it compiled and printed the other
  answer. `.claude/rules/diagnostics-and-goldens.md` calls that a `guess`.
- **Two silent-swap forms remain at the boundary and neither route touches
  them** (ffi seat, with running programs). A call through a function-typed
  value, `f: (function(str, str) -> str) @ report; f("msg", "code")`, compiles
  and swaps, and an invented label there is accepted too, because a function
  *type* names no parameters. And a C callback's own parameter order is
  unchecked: the seat wrote a header, bound it, labelled every call site, and
  measured the program printing `-7` where the names promise `7`. Filed as
  defects 026 and 027 rather than closed here.
- **The precedent runs one way** (historian, verified). Objective-C cannot
  express the exemption, because the label *is* the selector. Swift exempts by
  semantics and never by library membership, *omit all labels when arguments
  can't be usefully distinguished*, `min(number1, number2)`, and SE-0006
  rewrote the standard library rather than exempt it. **PEP 570 is the closest
  precedent and it ran the opposite way**: CPython's built-ins were *stricter*
  than user code, refusing keywords, and the language changed to let user code
  declare the same strictness, never to let the built-ins escape a rule.
  `gofix`, `cargo fix --edition` and the Swift migrator are the precedent that
  a thousand mechanical call sites is routine, and `2to3` is the precedent for
  what rots: a rewriter that is not the compiler's own parser.
  **CVE-2006-7049** is this class shipped and remotely exploitable, `strstr` and
  `strrpos` called with their arguments swapped, and **CWE-683** is its name.
  Google's own study of 200 million lines found a swapped-argument defect in the
  ASM library that had stood **thirteen years**.
- **One of the 1022 sites was malformed**, which is the historian's prediction
  scored by grep: `fail("compiler bug", …)` in `selfhost/emit/ctype.hero`, where
  § 6 says a code is a stable snake_case string. Nothing matched it; it is
  `compiler_bug` now.

## The verdict table

| seat | verdict | cost / delta, measured | prediction | condition |
|---|---|---|---|---|
| spec-warden | **veto on route B**, approve route C | route C **+1 real** net: +8 for § 6's two edits, −7 for § 11's duplicate removed; route B +20, truthful B +32 | swapped-`fail(` survivors fall 159 → 0; total 367 → 208 ± 10; kill rate 79.7% → 88.5% ± 0.6 | the veto lifts only if `range(1, 4)` and `write_file(…)` are shown to compile, which they do not |
| compiler-engineer | **object** to route A as written, approve with both halves | **+86 code lines**, all frontend: `inventory.hero` +34, new `check/labels.hero` 47, `walk.hero` +5; emitted C byte-identical, ABI 22 | both halves shipped → ≤ 215 survivors and zero `fail(`; `walk.hero` ≥ 1713 lines and a DECIDED amendment of ≥ 6 rows raised and ≥ 3 added | the six probes pass, the order half lands, `wrong_label`'s fix demoted |
| ffi-pragmatist | **approve** route A | 0 at the boundary: positional and labelled emit the same unit, sha1 `d407b4ed…`, IR identical, ABI 22 | of 441 emission goldens, 2 move and only in their `#line` numbers | `fail` and `slice` stay `.runtime`; the label text is a literal; holes 1 and 2 get their own defects |
| llm-ergonomist | **veto** on route B's form, keep the names | 11 characters a site, 99 over the nine failure sites of its program | ≥ 15% of generations swap under the positional form, ≤ 5% under the named one; first-try compile within 5 points, first-try **correct** better by ≥ 5 | a compiler that refuses a non-literal code would make the short form safe |
| historian | **approve** route A (advisory) | 0 | at least one of the 1022 sites is already malformed or transposed | a mandatory-label language that deliberately exempts its own library and is glad it did |

## Resolution adopted, provisional

**Route C, with both halves of the rule.** The two `.runtime` built-ins whose
parameters share a type carry their parameter names as literals in
`selfhost/inventory.hero`, beside the table that already enumerates every
built-in; `selfhost/check/labels.hero` enforces both halves at the two call
paths, a missing label as `needs_label` and a label in the wrong position as
`wrong_label`; `wrong_label`'s fix is a `guess` when the written label names
another position of the same callee, which is a fact about the call in hand and
not a premise about the world; the document takes § 6's two edits and pays for
them with § 11's duplicate; and the 1022 call sites are relabelled.

**What conservative would have been**, recorded so the author can choose it:
route B at +20 real, which the warden vetoed as false and the ergonomist vetoed
as a withdrawal of the locality guarantee from a class of calls.

## Predictions to score, and four are scored here

`heroes mutate --operator swap-args --survivors`, before and after, same corpus:

| | before | after |
|---|---|---|
| mutants | 1910 | 1910 |
| killed by `heroes check` | 1441, **80%** | **1733, 96%** |
| survivors | 367 | **75** |
| survivors that are a swapped `fail(` | **159** | **0** |
| killed by `--permissive` | 1052 | 1052 |

| seat | prediction | state |
|---|---|---|
| spec-warden | swapped-`fail(` survivors 159 → 0 | **HELD**, exactly |
| spec-warden | total survivors 367 → 208 ± 10, kill rate → 88.5% ± 0.6 | **FALSIFIED, in the good direction**: 75 and 96%. The estimate counted the `fail(` class alone and the rule also reaches every positional `slice`, so 292 mutants died where 159 were predicted |
| compiler-engineer | both halves → ≤ 215 survivors, zero `fail(` | **HELD** |
| compiler-engineer | `walk.hero` ≥ 1713 lines; the commit amends ≥ 6 DECIDED rows and adds ≥ 3 | **HALF HELD**: `walk.hero` reads **1746**, and the amendment is **five rows raised and none added**. The wraps landed in fewer files than the estimate |
| historian | one of the 1022 sites is already malformed or transposed | **HELD**: one, `fail("compiler bug", …)`, malformed rather than transposed. No site was transposed, which is what made the mechanical repair safe |
| ffi-pragmatist | 2 of 441 emission goldens move, `#line` only | open, scored by the emission suite at the landing |
| llm-ergonomist | the generation rates | open, and metric 2 does not exist; registered as an observation |

## Author's verdict

**Pending.** The queue item is `panel 127` in `docs/work/DECIDE.md`. What a
ratification settles is whether the rule stays where it now reaches, since 1022
call sites in the repository grew eleven characters each to pay for it.

## What this sitting did not do

It did not close the class. A swap through a function-typed value and a
callback's own parameter order are both silent today, measured by the ffi seat,
and they are defects 026 and 027. It did not make the spec say that labels are
checked **in order**: `range(to: 4, from: 1)` is refused `wrong_label` and no
sentence of the document says it will be, which the warden priced at +7 and
recommended filing rather than buying. And it did not measure a reader: the
ergonomist's rates need Part 11's metric 2, which has never run.
