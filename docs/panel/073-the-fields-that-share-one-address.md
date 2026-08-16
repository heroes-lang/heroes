# Panel 073 — the fields that share one address

**Convened** 2026-08-16, M-selfhost-port, from `docs/debrief/DECIDE.md:345` —
panel 072's ffi-pragmatist rider 2, the one that produces **a wrong value at exit
0**. Full lane, five seats, every one of them with a different input.

## What was asked

A C `union` bound as an `extern record` compiles, runs and prints a number that
is not the one the author stored. Reproduced by the coordinator before the briefs
went out, on a **typedef'd** union, so tags have nothing to do with it:

```
extern "u.h"                      | typedef union { int32_t i; float f; } UDef;
    record UDef
        i: i32
        f: f32

a = UDef(i: 1, f: 1.0)   ->   a.i = 1065353216      exit 0
```

`1065353216` is the bit pattern of `1.0f`. The emitter writes
`(UDef){.i = t1, .f = t2}` and C's last initialiser wins. clang emits
`-Wexcess-initializers` **and** `-Winitializer-overrides` on the author's own
`.hero` line, and the compiler throws both away.

**Three directions are unsound, not one**: construction; reading a member that
was not written; and the generated `eq`/`hash`, which walk fields and so read the
same bytes twice under two types.

Candidates: **1** a `variant` over the union with a declared discriminant (the
author's proposal) · **2** refuse mechanically, by asserting that no two declared
fields share an address · **3** a marker, construction refused and reading
permitted · **4** a Part 6 row.

## The author's premise, corrected before the sitting

The proposal was *"a variant whose discriminant is a named sibling field, as
SDL_Event does, which has `type` next to the union"*. Measured in
`/opt/homebrew/include/SDL3/SDL_events.h:1016`: **`type` is a member OF the
union**, at offset 0, overlapping every arm's own first field — C11 6.5.2.3p6's
common initial sequence, not a sibling. And the arm↔value mapping is **not in the
header in any machine-readable form**: **124** `SDL_EVENT_*` enumerators against
**38** arms, stated only in doc comments (`SDL_KeyboardEvent`'s reads
`/**< SDL_EVENT_KEY_DOWN or SDL_EVENT_KEY_UP */`).

## Verdict table

| judge | verdict | the finding that decides it |
|---|---|---|
| **compiler-engineer** | **veto 1** · approve **2** with a mandatory file split · object 3 · object 4 | Prototyped 2 end to end in a clone: **+95 lines, 3 files, all in `emit/`**, every golden green and byte-identical, `#line` untouched because the assertion is gated on `fields.len() >= 2`. **One test failed and it is §11's ceiling** — `extern_record.rs` 281 → 329. Found the better predicate and did not have time to build it: **`sizeof(T) >= Σ sizeof(field)` is O(n) where all-pairs `offsetof` is O(n²)** — a 16-field record emits **120** conjuncts under the form it built. Measured today's `eq`: two unions written **differently** compare `true`, and `URef(f: 0.0, i: 2143289344)` gives **`a == a` false** — reflexivity gone |
| **ffi-pragmatist** | object 1 · approve **2 as a floor**, object as the whole answer · object 3 (close to veto) · **veto 4** | Bound **real SDL3** and ran it. **Three programs that work TODAY**: a read-only two-field binding (exit 0, correct), a **one-member** binding (exit 0, **zero warnings**), and a **four-arm** event loop (exit 0, correct on all four arms). So candidate 4 is refuted by a program that already runs, and candidate 2 as briefed **breaks the four-arm loop**. Attacked the `offsetof` predicate on **11 shapes**: right on 10 — typedef'd, tagged, anonymous inner union, common-initial-sequence, `#pragma pack`, disjoint pairs — and **one false positive**: a struct of two GNU **zero-sized** members. Bit-fields and incomplete types fail with *clang's* text, not ours, so §7 would report the wrong thing |
| **llm-ergonomist** (spec-only, blind) | **veto 1** · object 2 alone (approve as the detector) · **approve 3** with two conditions · approve 4 as fallback | Wrote Task A and Task C — the union and the struct — and reported: *"I wrote C by copying A and changing four letters… same confidence."* The `union`/`struct` distinction reaches the specification **nowhere**. Its sharpest sentence is the locality case: **`i: i32` and `f: f32` are each individually right, and their conjunction is wrong**, and nothing on either line says so. On candidate 1 it vetoed on §4.7's own promise: *"Exhaustive or compile error"* trains a reader that a compiling `match` covers every value — with 124 constants over 38 arms most values land nowhere, so **the reader's confidence is transferred intact onto a construct that does not earn it** |
| **spec-warden** | **veto 1** · **approve 2 at +0 spec tokens** · object 3 · object 4 | The finding that reframes the sitting: **`spec:217` already says a group's `record` *is the header's struct*, and a union is not a struct** — so under CLAUDE.md §12 the spec wins and **the compiler has the bug**. Candidate 2 adds nothing to the language and carries no §1.0 burden: **3440 → 3440**. Candidate 1 measured at **+131…+186**, *"31% of the real remaining budget for a construct that binds 2 types across 6 header sets"*, with **no admissible payment**: `selfhost/` has **0** `extern` groups, and `heroes mutate`'s 13 operators have no arm for a discriminant mapping. Ran `record UDef partial` with one member: **binds a union, gives the right value, and `partial` already refuses `==`** |
| **historian** (advisory) | approve the detector · **object to candidate 2's type-level arity rule** · warns hardest against 1 | Eleven systems, sourced. **Five independent designs restrict the CONSTRUCTION EXPRESSION, not the type** — Rust ("it must specify exactly one field", RFC 1444, stable 1.19 **2017-07-20**, no reversal in 9 years), D ("only one member initializer is allowed"), Zig, Swift (one initializer per member), and **Nim's C++ backend, which caught Heroes' exact bug** (`nim-lang/Nim#22708`, the identical *excess elements in union initializer*, open 2023-09-15 → 2024-03-03) while **its C backend did not**. The two that restricted the **type** — Go (union → byte array) and Fortran — both had users rebuild unions by hand, and **Go's rebuild defeated its own runtime pointer check** (`golang/go#15942`); Fortran's refusal was **partly reversed** in GCC 7.1. On candidate 1: it is **Modula-2's variant record**, which **Wirth deleted from Oberon** — *"mostly being misused to breach the typing concept… The true sin was that this tag could be omitted"* (HOPL-3, p.13 §5.1, read directly) — and **Ada standardised it only by making the tag size-zero, the checks suppressed, punning erroneous and `=` a `Program_Error`** (RM B.3.3). rust-bindgen **refused** the same mapping in 2021: *"there's no way to guarantee that some C object with a particular tag will have actually initialized the corresponding union variant"* |

## The disagreement, and it is the useful one

Every seat kills candidate 1. **The live disagreement is what candidate 2 should
restrict**, and the historian stated it against the brief: *"Candidate 2 restricts
the wrong thing… No language in the record does that."* The ffi-pragmatist
reached the same place from a compiled program rather than from precedent — its
four-arm SDL loop runs today and candidate 2 as briefed makes it exit 1 — and the
ergonomist reached it from the spec alone, refusing candidate 2 as a complete
answer because it leaves Task B unwritable.

**Three seats, three inputs, one correction to the brief.**

## What the coordinator measured after the verdicts came in

- **The sum-of-sizes predicate has no false positive on the shape that broke the
  other one.** The pragmatist's `S5` — `struct { struct Empty e1; struct Empty
  e2; int32_t x; }` with GNU zero-sized members — fires the `offsetof` assertion
  (measured: *"assertion fired: 1"*) and **passes** `sizeof(T) >= Σ sizeof(field)`
  (measured: exit 0). It also catches every union shape tested, including the
  anonymous union promoted into a struct (`8 >= 12`, fires). So the engineer's
  cheaper predicate is also the **more correct** one, and the two findings — one
  from a seat that compiled, one from a seat that prototyped — repair each other.
- **A `partial` record carries the header's size, not the field list's**: a
  one-`i32` `partial` view of a 128-byte struct, passed as `@b`, comes back with
  all 124 bytes written and a correct checksum (**21204 = 124 × 0xAB**, exit 0).
  The ergonomist could not know this and objected on the opposite assumption; the
  ffi-pragmatist confirmed it independently on a **real `SDL_Event`** — the
  emitter writes `SDL_Event h2_e;`, the header's own type.

## The resolution — provisional, author ratification pending

1. **Candidate 1 is refused, on three vetoes and a fourth objection**, and it does
   not return without: a mechanism by which clang refutes a wrong arm↔value
   pairing, a `heroes mutate` operator that can produce the mistake, and an answer
   to §4.7's exhaustiveness promise. The record against it is the strongest this
   project has assembled — Wirth removed this construct from a language, Ada could
   only standardise it by suppressing its own checks, and bindgen refused it for
   the reason this sitting's brief states.
2. **Candidate 4 is refused**, on the ffi-pragmatist's veto: a permanent refusal
   is falsified by two programs that run today, and CLAUDE.md §12 requires a Part
   6 row to name the fact that would make it wrong — that fact exists and was run.
3. **The detector lands, in the sum-of-sizes form**, gated on a record with two or
   more declared fields, with bit-fields and incomplete types filtered **before**
   the assertion so that clang's own text never reaches the author.
4. **What it gates is the wrong-value paths, not the declaration.** Construction
   naming two or more overlapping fields is exit 1; a generated `eq`/`hash` over
   such a record is exit 1; **reading is untouched**, because reading works today
   and three programs depend on it. This is the historian's correction and the
   pragmatist's measurement, and it keeps every program that runs today running.
5. **It is a fifth CLAUDE.md §7 class** — `ffi_union_field`, exit 1 on the `.hero`
   line, both field names recovered through `declaration()`, one `#~` golden case.
   The spec-warden made this a condition (panel 020's rule: a refusal that ships
   outside `Diagnostic` is a parallel channel), and it is what turns an assertion
   into a diagnostic.
6. **Spec cost: +0, measured.** `spec:217` already says a group's `record` **is
   the header's struct**. Under §12 this is a compiler bug, not a language change,
   and `SPEC_TOKENS` stays **3440**.
7. **The escape route is panel 074's `tag` marker, and it composes exactly**: one
   Heroes record per arm, each naming **one** member of the same C type. The
   ffi-pragmatist measured this shape today — one member, exit 0, **zero
   warnings**, correct output out of a buffer SDL filled — and it is that seat's
   registered prediction for the ladder.
8. **The construction-arity form is filed, not refused**: *a record over a union
   may declare every member and be constructed naming exactly one*, which is what
   Rust, D, Zig and Swift all converged on. It needs the compiler to know the type
   is a union **before** clang runs, so it waits for a marker; the historian's
   five-language precedent is its warrant when it returns.

**What a veto at ratification would compel**: the detector is ~60 lines in
`emit/`, one diagnostic and one golden; reverting is a revert. The spec is
untouched either way.

## Predictions to score

| judge | prediction | at |
|---|---|---|
| compiler-engineer | in the sum-of-sizes form the detector lands under **60 added lines** in `emit/` and keeps `extern_record.rs` under 300; `cargo test` at 567 with zero golden C diffs. The all-pairs form cannot, and `layout.rs` will say so | M-ffi-ladder close |
| ffi-pragmatist | with the detector and panel 074's marker, `sdl_four.hero`'s four arms bind as **four Heroes records over the one C type**, each naming one member, **no shim and no `ptr`**, and the assertion fires on none of them. If any of the four needs a hand-written C function, the detector is a ceiling and not a floor | M-ffi-ladder next rung |
| spec-warden | `SPEC_TOKENS` still reads exactly **3440** in the commit that lands `ffi_union_field`, and `tests/golden/check/` gains ≥1 case whose `.hero` carries `#~ ffi_union_field` and whose `.expected` names **both** colliding fields. If it moves by one token the "already loud" claim was wrong and the approval is void | the closing commit |
| llm-ergonomist | today ≥8/10 fresh readers produce a program that compiles, exits 0 and prints the wrong number, **0/10 produce a diagnostic**; under the detector the silent-error rate is **0/10** with the error naming `i` and `f` | M-program-corpus |
| historian | if the type-level arity rule ships, **at least one ladder binding will reach a union through a hand-written C accessor or an opaque `ptr`** — Go's documented outcome; under the construction-arity form that count is **zero** | M-ffi-ladder close |

## Conditions on the record

- **compiler-engineer**: flips candidate 2 to `object` if the sum-of-sizes form is
  refused and the O(n²) form kept — *"the file split then buys nothing the cheaper
  predicate did not already give"*. Withdraws the candidate-1 veto only for a
  closure-list program that needs it.
- **ffi-pragmatist**: **its panel-072 veto on extending the c-name marker to
  `union` is LIFTED, conditionally** — the marker renames, it does not construct —
  on condition that the marker reaches `union` only in the **same landing** as the
  detector, never the widening alone.
- **spec-warden**: approval is void if the assertion ships unclassified (exit 2)
  or without a `#~` golden. **Corrects the brief twice**: the spendable headroom
  is **595, not 656** (`measure/gate.rs:206-211` reserves 60 for the mortgaged
  closure rows, and it is an assertion that goes red), and the brief's own
  candidate-1 sketch spells `case`, which the lexer refuses today.
- **llm-ergonomist**: candidate 3 becomes an `object` if no zero-value
  construction and no single-field write exist — *"a rule that refuses the only
  expressible form is candidate 4 wearing a longer sentence"*.
- **historian**: reverses if a language is found that made a C union single-member
  **at the type level** and whose users did not route around it after ≥3 years.

## Two holes recorded rather than closed

1. **A non-`partial` record naming ONE field of a union compiles silently** — zero
   warnings, exit 0 (spec-warden, measured). It claims completeness falsely and
   keeps `==`/`hash`. The assertion needs two fields and cannot see it. Filed as
   its own item rather than discovered later, at that seat's insistence.
2. **`partial`'s size is unstated in the spec.** The behaviour is right (measured
   twice today) and the document is silent, so a reader binding an out-parameter
   must guess whether their buffer is 4 bytes or 128. Filed.

## Author's verdict

*Pending.*
