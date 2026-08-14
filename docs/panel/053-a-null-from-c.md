# 053 — An inbound `cstr` cannot be checked and read

**Status**: `ratified — 2026-08-14, author decision`.
**Convened** 2026-08-14, by author decision in `/decide` (item 2, answer `d`).
**Lane**: full, five judges.

**A criterion arrived mid-sitting and re-ranked it.** After four of the five had
reported, the author stated a goal of the language — *"a Heroes program must not
segfault and must not corrupt memory; choices in that direction win over other
criteria"* — now CLAUDE.md §12. The compiler-engineer, still working, was given it
along with the ffi-pragmatist's measurements and asked one added question. Its
amendment is the sitting's answer, and it is recorded as an amendment rather than
folded in, so a later reader can see which verdicts were formed before the
criterion existed.

## The proposal, verbatim

Verified on the repository the day it was convened:

```
extern "stdlib.h"
    function getenv(name: cstr) -> cstr      # to_str(v) prints /Users/joseph
                                             # v == nullptr → error[type_mismatch]:
                                             #                expected `cstr`, found `ptr`
extern "stdlib.h"
    function getenv(name: cstr) -> ptr       # v == nullptr → prints "unset"
                                             # to_str(v) → cannot be written
```

You must choose between checking for null and reading the string. Four options:
**A** `nullptr` a literal of both types · **B** an inbound `cstr` is `cstr?` ·
**C** `to_str` on a `cstr` is fallible · **D** a nullable `char *` is `ptr`, plus a
checked `ptr → cstr` conversion.

## The verdict table

| judge | verdict | rests on | cost / delta | prediction | condition |
|---|---|---|---|---|---|
| llm-ergonomist | adopt-with-condition — **C**, then B | § FFI's one-way paragraph read against § Types' *"the name says whether it can fail"* | — | under the current spec ≥60% of first tries read a nullable C string with no check; under C, 0% | **hard**: withdraws the adopt if `print` accepts a `T?`; the amendment must appear in § Types, not only § FFI |
| spec-warden | **veto B, veto C**, object A, object D | §1.0, §1.6, §12 | A **+24** · B **+48** (+75 with the example) · C **+44** · D **+31** · the `==` repair **+15** · R2 removal **−12** | with none of A–D and only the `==` repair, `SPEC_TOKENS` reads 3144 | veto hardens to the whole sitting if any option lands while the `==` sentence stays false |
| ffi-pragmatist | approve **A**, object B, object C, object D | §4.19, §1.11, §4.20 | 108 of 1161 entry points return `char *` (9.3%); ~45 of the 72 determinable are nullable (62%) | binding raylib's 15 `const char *` returns under D emits 15 discards-qualifiers warnings; under A, zero | flips to approve B **only** as a declaration-site opt-in; vetoes B if inferred |
| compiler-engineer | adopt **A**, **veto B**, adopt-with-condition **B′**, reject C, object D, and **adopt E** | §1.7's three-place test; Part 5's sugar table | **A = +19, two files** · B ≈ 110–130 and core · B′ ≈ 45–55 + ABI bump · C ≈ 70–90 + ABI bump · D ≈ 20 · **E = +33, four files** | if any option puts `Ty::Fallible` in an extern's result without an arm in `return_check`, the assertion count drops 15 → 13 **and the test named for it stays green** | A lands with a golden for `nullptr` in an argument position |
| historian | approve D, object to A, B and C | precedent | — | if blanket-B lands, an escape hatch is requested within two milestones, and ≥50% of the `.must()`s it forces sit on non-nullable functions | withdraws the objection to blanket-B on a shipped statically-typed counter-example |

## The disagreements, stated plainly

**The proposal's own premise was false, and two judges falsified it
independently.** `to_str` on a null `cstr` does **not** dereference:
`runtime/parts/str.c:215` guards NULL before `strlen` and panics
`hero_str_from_cstr: NULL pointer from C`, exit 134, byte-identical at `-O0`,
`-O2` and `--sanitize`. That path is a **diagnostic** gap — the message names
neither the `extern` nor the `.hero` line — and not a safety one. Option C's whole
value proposition rested on it.

**The safety gap is elsewhere, and no option on the ballot was written for it.**
A null `cstr` handed **straight to another C function**:

```
strstr(haystack: getenv(UNSET), needle: "y")
  -O0         exit 134
  --sanitize  AddressSanitizer: SEGV on unknown address 0x0 in libsystem_platform
```

`to_str` is never called on that path. C is structurally blind to it; so are A and
D; B closes it only where the author opted in.

**The ergonomist and the warden are irreconcilable on B and C, and the
disagreement is about what each was allowed to see.** The ergonomist, holding only
the spec, ranked C first because it makes the failing program unwritable. The
warden, holding the repository, measured that the corpus has **three** `-> cstr`
bindings and **none is nullable** — a 100% false-positive rate — and that
`runtime/hero_os.h`, the closure list's only `extern` group, has no `cstr`-
returning entry point at all, so **Principle 0 funds none of the four**. Both are
right about what they were given. The panel does not smooth this: it is the
clearest instance yet of the blind brief producing a verdict the repository
contradicts, and of the repository brief missing the reader the language is for.

**The compiler-engineer measured a defect inside option B that no one predicted.**
Admitting `Fallible(Cstr)` at the boundary **silently deletes** §4.19's return
assertion — `return_check`'s `_ => None` arm swallows it — and
`every_extern_carries_a_return_type_assertion` stays green, because it tests one
`i64` extern. A premise expiring in silence inside the test named for the
guarantee it guards (CLAUDE.md §11).

**The historian brought a fifth shape nobody proposed**: OCaml ctypes'
`string`/`string_opt`, twelve years stable with no annotations and no `unsafe` —
B's type at D's opt-in point, `-> cstr?` written by the **author**, with plain
`-> cstr` still meaning non-null. It also reported a negative finding, marked
unverified: no statically-typed language makes every C `char *` return nullable by
default without header annotations to derive it from. Swift, which *has* the
annotations, chose implicitly-unwrapped optionals for un-annotated headers
instead — and SE-0054's later retreat was from the escape hatch, not from
nullability.

## Amendment — the compiler-engineer, after the criterion arrived

**Option E, which was not on the ballot: a call-site null guard.** Built,
compiled, run.

```c
t3 = getenv(hero_cstr_nonnull(t2));
t7 = strstr(hero_cstr_nonnull(t4), hero_cstr_nonnull(t6));
```

The same program that gave a SEGV under the sanitiser now gives
`panic: a null \`cstr\` was passed to a C function`, exit 134. **+33 lines across
four files**, two failing tests and both are the ABI stamp; nothing in
`tests/golden/check/` or `tests/golden/ir/` moves, which is the sugar test
passing. Panel 052's standing veto is untouched and verifiably so: both return
assertions and both probes are byte-identical to baseline, and the call still goes
through the header's own prototype — only the *argument expression* changed.

**And the argument is the strongest in the sitting, because it is not a language
change at all.** It is a backend obligation of exactly the class CLAUDE.md §7
already mandates: *"Arithmetic aborts via `__builtin_*_overflow` — never C UB —
and `%` is guarded like `/`."* `hero_cstr_nonnull` is to a null `cstr` what
`__builtin_mul_overflow` is to signed overflow. Zero spec tokens, no new type, no
new surface. The narrowing is `Ty::Cstr` **on the callee's declared parameter
type** — a fact about the declaration in hand, never about where the value came
from.

The engineer also **vetoed** the shape the coordinator proposed first: a checker
rule refusing an un-narrowed inbound `cstr` needs *provenance*, which is a premise
about where a value came from, and this compiler has no flow-sensitive narrowing
at all (`types/tests/flow.rs` is jump typing, not value narrowing).

**What the criterion changed, in the engineer's own words:** the guard moves from
unlisted to first, because it is the only option that is unconditionally safe —
an answer that works only when the author asks for it is a lesser answer under a
rule that says a program must not segfault. C moves from object to reject. And
**A moves from cheapest to necessary**: without it there is no way to write the
null test at all, so the guard would fire on programs whose authors had no means
to prevent it. A makes the check writable; the guard makes its omission
survivable. Fifty-two measured lines together, neither core.

## Resolution — provisional, author ratification pending

**Landed: E and A.** The call-site guard at +33, and `nullptr` context-typed at
+19 — measured, two files, `contextual()` + `adopts()` + a `check` arm, the same
three points the integer literals already use, with `ptr` kept as the `synth`
default exactly as a number literal defaults to `i64`. **Zero spec tokens for E.**

**Refused: C**, on a falsified premise. **Vetoed: B as inferred**, on the deleted
assertion and on 3-of-3 false positives. **Waiting: B′ and D**, both queued.

**The warden's `==` repair is owed and is not part of this.** It measured that
`==` on `cstr` is address comparison while the spec says *"structural equality on
everything"* — panel 048's exact class, in the silent direction: `a == b` true and
`a.cstr() == b.cstr()` false for the same strings. Under §12 the compiler has the
bug or the spec does, and the warden's veto hardens to the whole sitting if a
nullability form lands while the sentence that describes the operator you check
nullability *with* stays false. It is queued as its own item, at a measured +15
minimum, and it is the first thing the next FFI sitting should take.

## Predictions to score

| # | judge | prediction | scored at |
|---|---|---|---|
| 1 | compiler-engineer | if any option puts `Ty::Fallible` in an extern's `Function::result` without an arm in `return_check`, `grep -c heroes-ffi-return` over `examples/curl/main.hero` reads **13**, not 15, and `every_extern_carries_a_return_type_assertion` stays **green** | M-ffi-ladder close |
| 2 | ffi-pragmatist | binding raylib's 15 `const char *` returns under **D** emits 15 `-Wincompatible-pointer-types-discards-qualifiers` on the author's own `.hero` lines; under A or today's `cstr`, zero. And `GetMonitorName` binds under A with no shim | M-ffi-ladder rung 5 |
| 3 | spec-warden | with A–D waiting and only the `==` repair landed, `SPEC_TOKENS` reads **3144**; at M-binding-fidelity close, 0 of the 3 existing `-> cstr` bindings will have needed a nullability form | next FFI sitting |
| 4 | llm-ergonomist | on ≥12 first-try tasks binding a null-returning C string function, the current spec yields ≥60% unchecked reads; under C, 0% | next harness run |
| 5 | historian | if blanket-B lands, an escape hatch is requested within two milestones, and ≥50% of the `.must()`s it forces sit on functions that cannot return NULL | M-ffi-ladder close |

## Ratification — 2026-08-14, by author decision

**RATIFIED as it stands.** E and A stay landed, B stays vetoed as inferred, C
stays refused, B′ and D stay queued.

What the yes settles: **the sitting's premise was false and its answer is better
than the question.** It was convened because a nullable inbound `cstr` looked like
a hole in the type system, and the measurement said otherwise — `to_str` on a null
`cstr` was already a clean abort with its own name, and the real crash was a null
`cstr` handed **straight back to C**, which three of the four options could not
see. The option that landed was on nobody's ballot.

It also settles the shape of that answer, which is the part worth keeping: **the
guard is not a language change**. CLAUDE.md §7 already obliges the backend to
abort rather than reach C's undefined behaviour — `__builtin_*_overflow` for
arithmetic, `%` guarded like `/` — and `hero_cstr_nonnull` is that same obligation
at the same boundary, for zero spec tokens and no new type. A boundary that needs
one next time does not have to be argued from first principles.

And `nullptr` moved from *cheapest* to *necessary* for a reason that is now
precedent: a guard whose omission the author had no way to prevent is a worse
guard. A makes the check writable; E makes its omission survivable.
