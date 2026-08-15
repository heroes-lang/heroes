# 059 — The read direction

**Status**: `RATIFIED 2026-08-15` (was `provisional — author ratification pending`).
**Convened** 2026-08-15 by author instruction, from panel 058's blind seat — which
could not write `getcwd` in **any** of three spec variants and reported the read
direction as the larger hole, having been asked about the write one.
**Lane**: full, five judges, four of five building in copies.

**The ballot's cheapest option turned out to be worse than doing nothing, its most
principled option collected a veto, and the two seats that compile disagreed on a
fact.** All three are measurements rather than opinions.

## The gap

`spec/heroes-spec.md` documents one direction: `s.cstr()` lends a `str` to C. It
never says how a `cstr` coming back becomes a `str`. The operation **exists and
works**:

```
home = getenv(name: "HOME".cstr())
print(home == nullptr)     # false
print(home.to_str())       # /Users/joseph
```

On a null `cstr` it is a clean abort: `panic: hero_str_from_cstr: NULL pointer from
C`. This is **panel 048's own finding with half unrepaired** — 048 recorded that the
language is wider than its specification in two places at this boundary, its item
(c) went to panel 053, and 053 repaired `nullptr` and `==` and left `to_str`
undocumented.

## The proposal, verbatim

> **A** name the conversion (+11 measured). **B** name it and say a null aborts
> (+28). **C** make it fallible — `str?`, absent when C returned null (+26 in spec,
> plus a compiler change). **No change.**

## The verdict table

| judge | A | B | C | no change |
|---|---|---|---|---|
| llm-ergonomist | **refuse** — worse than nothing | adopt-with-condition | **adopt-with-condition** (first choice) | refuse |
| spec-warden | adopt-with-condition (dominated) | refuse *as worded* | **VETO** | refuse |
| ffi-pragmatist | **adopt** | adopt-with-condition ×2 | refuse | refuse |
| compiler-engineer | approve-with-condition | object *on shape, not content* | object — **veto declined, and it says why** | object |
| historian *(advisory)* | refuse | **adopt** | adopt-with-condition, **under a second name** | refuse |

**No seat defends the status quo. A alone dies on the seat that measured it. C
carries a veto.**

## A is worse than nothing, and the spec's own law is why

The llm-ergonomist, blind, wrote this **first and unhesitatingly** under variant A:

```
v = getenv("NOT_SET_ANYWHERE".cstr()).to_str()
if len(v) == 0
    print("not set")
```

It compiles. It is wrong. And it was written **with confidence, backed by a rule** —
spec line 58: *"the name says whether it can fail: `to_str` and `to_f64` cannot, so
they give a value"*. Naming the conversion inside that law **positively asserts**
that converting a null `cstr` yields a `str`.

The mechanism is the finding. Under the current spec the judge did not know whether
`to_str` applied to a `cstr` at all — *"that ignorance is what sent me hunting, and
the hunt is what turned up 'a `ptr` or `cstr` compares as an address' and
`nullptr`."* A answers the question being asked and stays silent on the one that
should be asked, so the hunt never starts.

**Registered as a falsifiable number**: unguarded-conversion rate ≈45% with no
clause and ≈70% under A — *A above no-change by ≥15 points*. If a harness shows
otherwise the whole objection falls.

## C collects a veto, and three independent reasons

**The spec-warden's veto** is on price and on internal contradiction: C is **+37
honest**, not +26, because it must also repair spec line 58 — and *"any C wording
that leaves spec:58 standing is a veto on sight, self-contradiction inside one
3000-token prompt."*

**The ffi-pragmatist's refusal is the one that would survive any wording**: C
advertises a guarantee the boundary cannot honour. ncurses `tigetstr` returns
`(char *)-1` for *"not a string capability"* — not NULL:

```
p == NULL ? no        -- the guard PASSES
EXIT=139                 (SIGSEGV inside strlen)
```

And it counted the tax: **all four `to_str(cstr)` sites in this repository are on
functions that cannot return NULL** (`curl_version`, `curl_easy_strerror` ×2,
`sqlite3_libversion`). 4 of 4 would pay `.must()` for nothing.

**The compiler-engineer implemented C end-to-end and ran the suite green** — then
objected anyway, on a section it does not own, and **declined a veto it could not
ground**: design.md Part 5 lists `T?` as *sugar*, so C adds no core construct and
§1.7 does not bite. Recording that refusal to invent a veto is worth as much as the
verdict.

Its substantive objection: C makes `to_str` **the first built-in in this language
that is fallible for one argument type and total for four** — one name, one
`Callee::Builtin` index, five C entry points chosen by the first argument's type.
That is the shape panel 043's historian found in no language and the author ratified
against on 2026-08-13.

**And C does not subsume B.** The null check lives on the runtime side
(`str.c:266-269`) and has a **second caller inside the runtime** — `hero_args_at`,
which implements `args()`. C cannot delete it, adds a second null test to generated
C, and leaves the abort reachable.

**The historian's position is the reconciling one and is recorded rather than
adopted**: C belongs under a **second name**, which is Swift SE-0405's shipped
lesson — Swift kept the trapping `String(cString:)` and *added*
`String(validatingCString:)`. Two names, two contracts, no law broken.

## The two compiling seats disagreed about a fact, and the disagreement resolves

**ffi-pragmatist**: *"Correction to the record, against my own position: panel 053
priced its option C at ≈70–90 + ABI bump. **There is no ABI bump.**"* It hand-wrote
the C in the emitter's shape, compiled it under the project's exact flags against the
real `runtime-*.o`: clean, exit 0, leak-free.

**compiler-engineer**: *"The **ABI bump is not pre-paid**, and I hit it in the first
run: `_Static_assert(HERO_RUNTIME_ABI == 13)` failed at exit 2."* Its measured
landing is **+76/−18 across 18 files**, including 13→14 and five
`tests/golden/emit/*.expected`.

**They priced different implementations and both are right.** The pragmatist's C
needs no runtime function, so nothing crosses the ABI; the engineer's adds
`hero_failure_null_from_c` to `failure.c`, and a new runtime entry point is exactly
what the stamp exists to catch. The lesson is the one §1.11 states: the composition
arm is **pre-paid** (`emit/convert.rs` already builds a `T?` inline for
`to_<width>`), and the ABI bump is not — *"§1.11's argument applies at half
strength, not zero and not full."*

## What every seat agreed on, and it was not on the ballot

**`to_str` copies**, and that fact — not nullability — is what makes the read
direction sound. Proven by the ffi-pragmatist with the control that settles it:

```
first, before the second call: Unknown error: 9999
second:                        Unknown error: 8888
first, AFTER the second call:  Unknown error: 9999      <- unmoved
```

with a C control showing `ctime` returning **the same pointer** twice with different
content. The spec-warden located it at `runtime/parts/str.c:241`; design.md §4.20
already says it — *"make an owning copy of a borrowed C pointer"* — **and that
sentence is nowhere in the spec.**

**The copy deletes a defect class rather than a surprise.** `getenv` then `setenv` in
C is `AddressSanitizer: heap-use-after-free`, exit 134. The identical Heroes program
is ASan-clean at exit 0.

And the historian found that cgo has an issue open **since 2019** asking for exactly
this sentence (golang/go#32734, and #12427 for the copy) — against `C.CString`
documented in five lines with its ownership rules and `C.GoString` in one line with
nothing. *"A half-documented FFI boundary does not self-heal; it accretes
folklore."* That is the answer to no-change.

## The abort is reachable on §4.19's own ladder

The ffi-pragmatist wrote rung 3 for a TEXT column — which the shipped
`examples/sqlite` never does, reading only `sqlite3_column_int`:

```
first body:  hello
panic: hero_str_from_cstr: NULL pointer from C     EXIT=134
```

on `values ('hello'), (NULL)`. **The guard costs four Heroes lines and no shim.**
That is what makes the abort survivable and B admissible.

## Resolution — provisional, author ratification pending

**The clause is adopted at +25 measured**, folded into the sentence that is already
there rather than added beside it:

> `ptr` is an opaque pointer whose only literal is `nullptr`, `cstr` a C string,
> `s.cstr()` lends a `str` to C to read and `c.to_str()` copies one back — **test
> `c == nullptr` first, because converting one aborts.** A C out-parameter is an `@`
> parameter.

**It is dearer than the spec-warden's W14 (+17 measured here, +16 at its own
wording), and the eight tokens are the difference between a clause every seat
adopts and one that fails three stated conditions.** W14 states the copy and the
abort; it does not state the **escape**, and the escape is a condition of the
ffi-pragmatist's adopt and of the llm-ergonomist's. Spelling `c == nullptr` also
discharges the historian's live worry — that the abort might not be *guardable* —
and answers the llm-ergonomist's finding that `nullptr` is documented as **`ptr`'s**
literal one section away from where a reader needs it at `cstr`.

**The abort is stated as a floor, not an identity**, which is the spec-warden's
condition 1: `to_str` has **two** abort paths, `hero_str_from_cstr: NULL pointer
from C` and `hero_str_from_bytes: not well-formed UTF-8`, both measured. *"Aborts if
C returned `nullptr`"* would read as an identity and be the fourth
false-in-the-silent-direction sentence this spec has produced. *"Converting one
aborts"* names the one and claims nothing about the rest.

**A alone is refused**, on the measurement of the seat that wrote the trap program.
**C is refused**, one veto and one unanswerable objection — and its reconciling form,
a *second name*, is recorded as the shape any future attempt takes rather than
struck. **No-change is refused by every seat.**

**Payment**: none owed. This is a **§12 repair**, not an entry: the operation already
exists, design.md §4.19:1989 already says *"declare it `cstr` and convert with
`to_str`"*, and `types/decls.rs:237` is a **shipped diagnostic** naming `to_str` as
the cure for a `str` constant — the compiler documents an operation the spec does
not. Precedents are ratified and explicit: ledger rows `2983`, `3141`, `3158` all
landed §12 repairs with no removal and no prediction. Panel 051's −8 stays refused
and unspent, and the spec-warden's newly found **−42** (compressing the eight width
names) is priced and **deliberately unspent** — banking it against a §12 repair would
be §1.6's *"headroom is not an entitlement"* in action, and it deserves its own
sitting.

**Owed with the clause, and both are conditions rather than decoration:**

1. **A run-golden for the inbound abort and one for the guarded
   `sqlite3_column_text` on a SQL NULL.** `tests/golden/run/` has **zero** coverage
   of the read direction today — `abort-null-cstr-into-c.hero` is the *outbound*
   null and never calls `to_str` (ffi-pragmatist).
2. **`bad_operand` on a `cstr` must offer a route.** Measured: `print(v)` and `"x" +
   v` both stop with no `Fix` naming `to_str`, and `spec/reserved-words.md` mentions
   neither `to_str` nor `cstr`. *"A hard stop with no route is what produced the one
   genuinely unsafe artifact in this whole record"* — panel 058's blind seat reaching
   for a `static char[4096]`. Zero spec tokens (spec-warden).

## Findings that are not about the ballot

- **A `cstr` C malloc'd cannot be freed in any spelling, and panel 058 closed half
  the door.** `free(p: cstr)` is now `ffi_writable_parameter`; `free(p: ptr)` is
  `type_mismatch: expected ptr, found cstr`. Both reproduced. So `readline`,
  `strdup` and `sqlite3_expanded_sql` leak on every call — and `--sanitize` **and**
  the leak gate report nothing, exit 0. §4.19:2062 names the ownership vocabulary as
  future work; it is reachable today. **Queued.**
- **`ffi_writable_parameter`'s message over-claims on `void *`.** It says *"C would
  write through it"* for `free`, which deallocates rather than writes. The refusal
  may still be right; the sentence is not. Queued with the above.
- **`print` on a `cstr` is `bad_operand` at exit 1**, not an address printed at exit
  0. This corrects a claim in `DECIDE.md`'s own queued item, written by the
  coordinator and never measured (spec-warden).
- **The option nobody put on the ballot**: nullability belongs to the **declaration**,
  not to the conversion — Clang's `_Nonnull`/`_Nullable` and audited regions (2015),
  Swift SE-0055 mapping `_Nullable` to `Optional` (Swift 3), Zig's `?[:0]const u8`
  return type, Ada's `Null_Ptr` as a value of the pointer type since 1995. Recorded
  so a future sitting starts there rather than rediscovering it.
- **`tigetstr`'s `(char *)-1` is a sentinel no null test can see**, and it is the
  general shape: a boundary can hand back a non-null pointer that is not a string.
  No option on this ballot addresses it.

## Predictions to score

| judge | prediction | scored at |
|---|---|---|
| llm-ergonomist | Twenty first-try T2 generations per variant, scoring *converts a `cstr` with no null handling*: no-clause ≈45%, **A ≈70%**, B ≈20%, C ≈5%. The clause I most want checked: **A > no-change by ≥15 points**. Second: 100% of C's failures are compile errors, and ≥90% of B's are aborts with a Heroes-visible message and 0 SEGV | first Part 11 run with a model harness |
| ffi-pragmatist | At the next milestone touching `examples/sqlite`, extending it to a nullable TEXT column reaches exit 0 with **one `if cell == nullptr` and zero C shim, in ≤4 added Heroes lines**, ASan and leak-gate clean — and **no `to_str(cstr)` site in `examples/` needs `.must()`**. Falsified by a shim, by >4 lines, or by any `.must()` appearing | the next milestone touching `examples/sqlite` |
| compiler-engineer | If C is ever adopted, its landing commit shows **≥70 added lines across ≥13 files**, including a `HERO_RUNTIME_ABI` bump touching ≥5 `tests/golden/emit/*.expected`, and the null check at `str.c:267` is **still present** because `os.c:62` calls it. Falsified by any green implementation under 70 added lines, or one that deletes that check | M-ffi-ladder close |
| spec-warden | With the clause in the spec, a blind reader given `getenv`, `curl_version` and `strerror` writes `to_str` unprompted **3 of 3**, reaches for a C helper with a static buffer **0 of 3**, and writes a `nullptr` test before conversion in **≥1 of 3**. If `to_str` appears in ≤2 of 3, the wording failed rather than the ceiling. Instrument: `docs/measurements/007`'s blind-reader protocol, run twice already | the next FFI rung |
| historian | If the clause lands without a **testable** null `cstr`, the first binding of a C function whose NULL return is ordinary data is unwritable without a shim. **Pre-falsified during the sitting**: `v == nullptr` on a `cstr` branches correctly without calling `to_str`, measured — the historian's own condition 1 is met and its objection to B-alone is discharged | settled 2026-08-15 |

## Process notes

- **Four of five judges built in copies with `target/` removed**; the fifth read no
  repository file, which is its brief. The working tree was frozen from the briefs
  going out to this synthesis — the third observance of the `/panel` amendment.
- **The coordinator's ballot was wrong in three places**, all corrected by seats: A
  was tabled as the cheap safe option and is the dangerous one; B's wording was an
  identity where only a floor is true; and the queued item's claim that `print` on a
  `cstr` *"may print an address"* is false.
- **A judge declined a veto it could not ground and said so in the verdict.** That is
  the instrument working in the direction that is hardest to observe.

## Ratification — 2026-08-15

**RATIFIED as it stands** (author instruction: *"ratifico tutto e finisci tutto prima
del prossimo step, accetto le decisioni degli esperti senza chiedere a me"* — a
blanket yes given after reading the synthesis, and the fifth of the day, recorded as
that rather than as five reviews). The clause is in the spec at **+25**, A is refused
on the measurement that condemned it, C stays vetoed with its second-name form
recorded, and no-change stays refused by every seat.

**The instruction has a second half this file must carry**: the author has delegated
the judges' own recommendations, so the two things owed with the clause are **not
open questions** — they are work, and they land without coming back:

1. a run-golden for the inbound abort and one for a guarded `sqlite3_column_text` on
   a SQL NULL, because `tests/golden/run/` has zero coverage of the read direction;
2. a route on `bad_operand` for a `cstr`, because a hard stop with no route is what
   produced panel 058's `static char[4096]`.

**What a blanket yes still cannot settle.** Four of the five predictions name a model
harness or a milestone that has not happened, and panel 046's R2 governs them
unchanged. Two are checkable and both are worth watching: the llm-ergonomist's — *the
+11 clause is **worse** than no clause by ≥15 points* — because it is the sitting's
most counter-intuitive claim and its author offered it as the falsifier of its own
refusal; and the ffi-pragmatist's, that extending `examples/sqlite` to a nullable TEXT
column needs **≤4 added lines and no shim**, which the run-golden above is the first
half of.

**On the veto of C**: ratifying it does not rule that a fallible conversion is wrong.
It rules that `to_str` may not be the name that carries it — spec:58's law, the
`(char *)-1` sentinel, and four call sites paying `.must()` for nothing. The
historian's second-name form is the shape a future sitting starts from, and this
ratification does not consume it.
