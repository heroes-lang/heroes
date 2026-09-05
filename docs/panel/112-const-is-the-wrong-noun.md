# Panel 112 — `const` is the wrong noun

**Convened** 2026-09-05, M-c-callbacks step 4, on panel 111 R4's owed item: *the
`c_int`/`const` vocabulary, or a design.md Part 8 wart naming const-qualified
pointee parameters as the known hole with its return condition*. Full five seats:
it changes the language's type vocabulary, it touches `spec/` and `design.md`, and
a diagnostic class is in play.

**Frozen at `aab44f9b`**, clean tree, every seat working from one snapshot copied
per judge (panels 087 and 088's lesson).

## The proposal, verbatim as it went to the seats

1. **THE HOLE.** Since panel 111 R4 a Heroes function may be an `extern`
   parameter. Measured over 8 real callbacks: **3 bind**, **5 do not**, for
   **three different reasons** — `const void *` (`qsort`, `bsearch`),
   `const char *` plus a const record pointee (`nftw`), `char **`
   (`sqlite3_exec`), result position (`signal`, refused on purpose).
2. **WHY IT IS NOT ONE HOLE.** `@result: ptr` against C's `char ***` is
   **accepted at a direct parameter** (`sqlite3_get_table`, exit 0). The same
   `char **` inside a function-pointer type is a hard error. C converts pointer
   types at a call and requires identity inside a function-pointer type.
3. **THE QUESTION.** What does a callback parameter need that a direct one does
   not, and how much of C's pointer vocabulary does that drag in?
4. **OPTIONS.** (a) a `const` pointee spelling; (b) a wider C-pointer
   vocabulary; (c) a design.md Part 8 wart with its return condition, zero
   tokens; (d) something none of these name.
5. **WHAT STANDS IN THE WAY.** Panel 083 R4, ratified 2026-08-16, refused a `Ty`
   variant, a keyword and a spec type name for `const`, and its veto is armed.
   Half its reason has expired: it cited CLAUDE.md §13 closing Part 7 *"until the
   fixpoint"*, and the fixpoint landed 2026-08-18.

**The brief's own framing is the first thing the sitting refuted.** Point 1's
"three reasons" is measurably the wrong axis and its count of what `const` buys
was wrong in the convener's favour. See § What the brief got wrong.

## The verdicts

| seat | verdict | section | measured cost / delta | condition |
|---|---|---|---|---|
| **compiler-engineer** | **veto** (a), (b); approve (c); reject (d) | §1.7, Part 5, §4.19:2146-2154, panel 083 R4 | a new `Ty` case = **173 `non_exhaustive` errors across 49 files**, before one arm gets correct behaviour: checker ~86, IR ~16, emitter ~65. Spec **3830 → 3878, +48, 18% of headroom**. A `quals` run on the existing `function_ty` case = **2 edits, 1 file, compiler clean** | a closure-list or §4.19-ladder binding that needs it · or the `quals` shape with `ty_key` and `synth.content_key` both updated and both tested |
| **ffi-pragmatist** | **object** on (a) as framed; no veto — nothing moves the ABI | §1.11, §4.19, CLAUDE.md §7 flag 13, panel 058 | over the **real `sqlite3.h`, 106 callback signatures**: today **17/106**, `const` **18/106 (+1)**, pointer-to-header-record **82/106 (+64)**, pointer-to-pointer **96/106**. `HERO_RUNTIME_ABI` unmoved at 21; `-O1 -S` **byte-identical** | (a) never lands alone or as a cast · the wart names the measurement · the needle defect repaired in the same commit · §4.19's shim sentence corrected · no emitter-written cast, ever |
| **spec-warden** | **veto** (a), (b) on Principle 0; approve (c) at zero tokens | §1.6, §1.2, CLAUDE.md §2, §12 | ten wordings written and run: **(c) +0**, cheapest spec option **+13**, `const ptr` at the Types table **+16**, full vocabulary **+49**. `selfhost/` binds **0 callbacks** across 4 extern groups / 14 functions | lifts the day a closure-list or ladder binding needs a const pointee · or a blind Part 11 measurement shows the hole raising first-try error rate |
| **llm-ergonomist** | **veto** on the `const` wording; adopt the refusal wording with a condition | the thesis, locality | guesses forced per task: status quo **4 / 5 / 3**; with `const` **8 / 9 / 3**; with the refusal **0 / 0 / 3**. First-try success on `qsort`/`bsearch`: **0% under all three** | the refusal must name the **true** cause, not the qualifier — an unqualified callback hits the identical wall |
| **historian** | **object** (advisory) to the option **list**, not the options | precedent | Haskell added `const` after **~20 years** (base-4.18, 2023); ocaml-ctypes after **11** (0.23.0, 2024); Nim and Go still have none. Swift **SE-0324** names this exact seam. `const` alone predicted **5/8**, an adapter **7/8** | a checked-half language that binds a header-imposed callback with an opaque pointer alone · a documented defect from a generated adapter |

## What was measured, and it is the whole sitting

### `const` is the wrong noun, and the number is 1 of 106

The convener measured 8 callbacks by hand. The ffi seat measured **`sqlite3.h`
itself** — 106 distinct callback signatures, 153 unspellable parameter
occurrences — and sorted them by **cause**:

| cause | occurrences | share |
|---|---|---|
| pointer to a header struct | **102** | 66.7% |
| pointer to a scalar | 26 | 17.0% |
| pointer to pointer | 18 | 11.8% |
| **const pointee, non-`char`** | **4** | **2.6%** |
| opaque typedef | 3 | 2.0% |

Cumulative bindability: today **17/106**; with a `const` spelling **18/106**;
with a pointer-to-header-record spelling **82/106**; with pointer-to-pointer
**96/106**.

**The sitting was convened on 2.6% of its own subject.** That is the finding, and
it is the convener's error rather than any seat's: panel 111 R4's sentence
*"without it 5 of 8 real callbacks do not bind"* was carried into the brief as
though `const` were the cause of all five, and it is the cause of one.

### The convener's count of the 8 was wrong in his own favour

`const void *` buys **2**, not 3. The ffi seat built the matrix in one clang run:
`qsort` and `bsearch` go green; **`nftw` stays red**, because its `const char *`
half is already `cstr` and what blocks it is `const struct stat *` **and**
`struct FTW *` — and the second is not const at all. `sqlite3_exec` is `char **`
and `const` buys it nothing.

### A fourth cause nobody named, and no option reaches it

`(function(i32, @out: i32) -> i32)` is a **parse error** —
`expected_function_type_params_close`. An out-parameter inside a callback is
**26 occurrences** in `sqlite3.h` and it is raylib's `LoadFileDataCallback(const
char *, int *)`. Not a type hole: a grammar one.

### A `const` spelling is not checker-only, and its cheap implementation is a §1.12 hole

This is the measurement that decides the vote. The qualifier **propagates into
the body**. Three routes, all built:

- const in the typedef only → `incompatible function pointer types`;
- const propagated as a real Heroes type → **works**: real `qsort`, real data,
  `1 2 3 4 5 7 8 9`, and panel 058's flag still refuses a write;
- **const, with the emitter casting it away** → compiles **clean under all
  fourteen flags** and prints `42 42 42 42 42 42 42 42`: a Heroes program wrote
  through every byte C had promised was read-only, **exit 0**.

The third is the cheap one, and it is panel 058's guard switched off for every
callback parameter.

### Why the emitter may not cast the function pointer — measured, not quoted

design.md §4.19:2152 forbids it in writing. The engineer and the ffi seat both
reproduced the reason under `selfhost/cli/flags.hero::flags()` verbatim: a
mismatched function-pointer cast gives **zero diagnostics, exit 0**, and an
arbitrary permutation (`7 3 9 1 5 2 8 4`; the engineer's variant printed `0`
where `3` was correct). `-fsanitize=function` does not fire and is not in
`--sanitize`. `-Wcast-function-type-strict` sees it and is not among the
fourteen. The prohibition is earned.

### Can the emitter derive the qualifier from the header? No, and here is what was searched

The ffi seat tried `__typeof__`, `__builtin_types_compatible_p`, `_Generic`,
`__builtin_choose_expr` and `__builtin_classify_type`. **None names a
*parameter* of a header function.** Where a header typedefs the callback,
`static __typeof__(*(TraceLogCallback)0) f;` declares it exactly — but a
*definition* needs a declarator with parameter names, so the route yields a check
and no body, and the check points at generated C where `ffi_callback_type`
already points at the author's line. **The hole is expressibility, not
detection**, and panel 111 R4's diagnostic is already maximal.

A negative claim naming what was searched is the only admissible kind
(CLAUDE.md §1), and this one names it.

### The document's own reader cannot get near the question

The ergonomist, given only `spec/heroes-spec.md`, could not write `qsort` or
`bsearch` under **any** variant, and stopped **before** `const`:

- **nothing turns a `[i64]` into a `ptr`** — the document's producers of `ptr`
  are `nullptr`, a C out-parameter and a group record's field;
- **nothing reads through a `ptr`** — it is "an opaque pointer", and `validated`
  takes a `cstr`.

Verified by the coordinator against the whole document: `ptr` appears **four**
times in `spec/heroes-spec.md` and none of the four produces one from a Heroes
value or reads one. `p: ptr @ xs` over `[i64]` is `type_mismatch`.

**And the proposed sentence would contradict a live rule six lines above it.**
`spec:214` already says a parameter is refused when it disagrees *"except a
parameter C converts exactly … and what a `ptr` points at"*. A qualifier is on
the pointee. The document does not have a silence here; it has a ruling.

The ergonomist's locality veto, in its own words: `const` has no meaning **in
Heroes**, because there is no aliasing and every value is a copy. Its whole
content is *"matches a header in another file"* — a type whose meaning is not on
the line that writes it.

### What no vocabulary fixes

`va_list` is **three different types** on the three platforms this project
ships on, measured by target: `char *` on arm64-apple, `struct __va_list_tag *`
on x86_64-linux, **a struct by value** on aarch64-linux. raylib's
`SetTraceLogCallback` is unbindable by any pointer vocabulary.

Conversely, and the brief did not know it: **a header struct passed BY VALUE into
a callback already works** (`sum 7`, measured), because panel 060 makes a group
`record` the header's own type, so C sees identity.

### `sqlite3_exec` binds today

**28 lines of `static inline` in a header the group names** — no `heroes cc`, no
`compile` clause, the `#include` carries it. Real in-memory database, real rows,
`rc 0`. So the brief's *"`sqlite3_exec` does not bind"* was true only of the
direct declaration, and §4.19's *"how it is compiled is undecided"* is
measurably stale for the `static inline` route.

### The precedent, and it is one story told five times

Every language in the **checked half** — where a C compiler validates the
binding, which is where Heroes lives because it emits C and `#include`s the
header — eventually acquired a const spelling, and none had it at the start:

| language | const spelling | when |
|---|---|---|
| Haskell | `Foreign.C.ConstPtr` | base-4.18, GHC 9.6.1, 2023 — **~20 years** after the FFI Addendum |
| ocaml-ctypes | `const`/`volatile` | 0.23.0, 2024 — **11 years**, with `#define const` as the documented workaround throughout |
| Nim | none | never |
| Go / cgo | none | never |

And the sharpest precedent of the sitting: **Swift SE-0324** relaxed pointer
conversions at direct C arguments and left the function-pointer position strict,
saying so outright — *"This solution does not cover C APIs that take function
pointers. However, that case is much less common."* The seam the brief measured
on this Mac is one another language found, named and priced. Go distinguishes
harder still: calling a C function pointer is unsupported and a gateway function
is required.

**Nobody in the checked half spells `char **` with one opaque pointer.** Nim and
Ada each added **one named type for the one recurring shape** — `cstringArray`,
`chars_ptr_array` — rather than widening the vocabulary generically.

The historian also **refuted a claim it found in a search summary** (that
`ConstPtr` was a breaking change forcing `unix` and `bytestring` to update) by
reading `unix`'s changelog directly and finding no mention at all. That is the
seat doing the job it exists for.

## Where the seats disagreed, left standing

**The engineer and the historian split on option (d).** The historian holds that
the option list is wrong because it omits the generated adapter — what cgo has
shipped since 2009 and what ctypes' inverted bindings emit — and predicts it
reaches **7/8** where `const` reaches 5/8. The engineer built it and rejected it
on two measurements: the **cast** variant is lethal and forbidden by name, and
the **adapter** variant needs a C declarator parser fed by platform-varying text
(`int (*)(const char *, const struct stat *, int, struct FTW *)` is not a comma
split), which is the precise thing `emit/c_spellings.hero:11-21` refuses to
table. The ffi seat's `static inline` finding sits between them: the adapter
already exists as a route the **author** writes, in 28 lines, with no build rule.

**The engineer and the warden split on the `ptr` escape hatch.** The engineer
holds that `qsort(compar: ptr)` at exit 0 is design.md §4.19:2148's documented
route; the warden holds it is a live §1.12 defect. **The coordinator measured
both and they are the same fact seen from two sides**: the declaration is
accepted, and what may then be passed is only `nullptr` (compiles, **exit 139**)
or a `ptr` C itself produced (works). A Heroes function value is refused at the
call site. So the route carries exactly one legitimate value and one segfault.

## Resolution — provisional, author ratification pending

CLAUDE.md §4: the most robust and complete resolution, never the cheapest and
never a compromise, with the conservative one recorded.

**R1 — Option (a), a `const` pointee spelling, is refused, and (b) with it.**
Four seats against on four independent grounds, and independence is what makes
the agreement worth anything: 173 errors across 49 files; +48 tokens against 266
of headroom with **zero** closure-list need; a contradiction with a live spec rule
and no meaning in a language without aliasing; and **1 real callback of 106**.
Panel 083 R4's veto stands. **It is stronger than in August, not weaker**: in
August it rested on a citation, half of which has since expired with the
fixpoint; today it rests on `heroes check` printing 173 and `heroes measure`
printing 3878.

**R2 — Option (c) lands, and its TEXT is the ffi seat's measurement rather than
this sitting's framing.** A Part 8 wart, zero spec tokens, naming the causes **in
measured order**: pointer to a header struct (102 occurrences, +64 callbacks),
pointer to a scalar (26), pointer to pointer (18, +14), const pointee (4, +1),
and the **`@`-inside-a-callback parse refusal** that no option on the table
reaches. A wart that names the smallest cause funds the wrong milestone, and
CLAUDE.md §12 holds a refusal to a feature's burden of proof.

**R3 — Three defects this session shipped are collected; two are repaired in
this commit and the third is filed with its price.** All three are the same
shape — a claim written without being run — and all three were found by seats
rather than by the convener:
- `emit/ffi_narrowed.hero`'s note **cites design.md Part 8 for a wart that does
  not exist**. Part 8 has 18 warts and none is this one, counted.
- The same note's needle is `has(text: c_type, needle: "const ")`, and
  **`const char *` matches** — which `cstr` spells exactly. Reproduced by the
  coordinator: a callback whose only error is `i64` for `i32` is told *"this one
  cannot be written at all today"*, and the one-character repair **builds and
  runs at exit 0**. A diagnostic that stops a correct repair is §4.17's own
  failure.
- `atexit(f: ptr)` handed `nullptr` **compiles and exits 139**. This is panel
  111's ergonomist's registered prediction, live: R4 made the right spelling
  legal without making the wrong one illegal. **This third one is FILED RATHER
  THAN REPAIRED, and the reason is a measurement the sitting did not have.** The
  warden's repair — `-Wpedantic` in `extern_probe.hero`'s `PROBE_ERRORS` — was
  measured against `curl`, `ctime`, `nbody` and `spectral` and reported zero
  false fires. The coordinator ran it against the rest and it fires **twice on
  `examples/sqlite/main.hero`**, which declares `sqlite3_exec`'s callback as
  `ptr` and passes `nullptr` — and that program is **correct**: SQLite documents
  a null callback as *do not call back*. So the flag does not separate the
  segfault from the deliberate NULL, and neither does anything else available:
  `nullptr` does not inhabit a function type, so the example cannot be rewritten
  at the typed spelling without losing what it demonstrates. The two routes out
  are (i) land the flag and rewrite a correct example, or (ii) let `nullptr`
  inhabit a function type — a spec change, and one that leaves `atexit(nullptr)`
  compiling and segfaulting anyway, because the difference is whether the C
  function tolerates NULL and no compiler on either side of the boundary knows
  that. It goes to `docs/work/DEFECTS.md` with its reproducer and this
  measurement, and to the author.

**R4 — Two false sentences in `design.md` are corrected**, both §11's expiring
premise and both about this sitting's own subject:
- `:2150`'s *"`qsort`, `sqlite3_exec`, `sqlite3_busy_handler` and all six raylib
  callback typedefs fail, 9 out of 9"* — **`sqlite3_busy_handler` binds at exit
  0**, as do `atexit` and `pthread_create`. Panel 111 R4 made it false and
  nothing updated it.
- §4.19's *"how it is compiled is undecided"* for a shim — **`static inline` in a
  group's header compiles with no build rule**, measured, and it is the route
  that binds `sqlite3_exec` today.

**R5 — The historian's option (d) is recorded, not adopted, with its return
condition.** The **cast** form is refused permanently: measured lethal under all
fourteen flags, unpoliced by `--sanitize`, and forbidden by name at
design.md §4.19:2152. The **adapter** form survives on soundness and dies on
cost. It returns the day a measured C declarator parse over `stdlib.h`, `ftw.h`,
`sqlite3.h` and `SDL.h` on all three platforms shows a stable recovered spelling
under ~60 lines — the engineer's condition 3, adopted verbatim.

**R6 — The `quals` shape is recorded as the ONLY admissible route if the const
half is ever closed**, and the record says why: it adds no `Ty` variant, so panel
083's veto does not fire on it, and it measured **2 edits in 1 file** against 173
errors in 49. It carries two silent sites that are owed tests when it lands —
`check/table.hero`'s `ty_key` and `emit/synth.content_key`, where two callback
types differing only in `const` would intern to one TyId with no diagnostic.

**What the conservative resolution would have been**, recorded so the author can
take it: adopt (c) with the brief's own wording — const-qualified pointee
parameters as the known hole — and stop. It changes least. It also writes the
2.6% cause into the design document as though it were the subject, leaves three
shipped defects live, and leaves two false sentences in `design.md` about the
exact thing the wart describes. §4's *robust over conservative* is why R2 to R4
are in this resolution and not in a follow-up.

## Predictions to score

| seat | prediction | scored at |
|---|---|---|
| compiler-engineer | if the wart lands, `heroes measure spec/heroes-spec.md` reads **3830** and `variant Ty` in `check/table.hero` still has **17 cases**. Either moving means a language change went in under a wart's name | M-c-callbacks close |
| compiler-engineer | falsifier for its own veto: if a `const`/`c_int` kind ever lands, the commit touches **≥40 files** under `selfhost/` and `heroes measure` reads **≥3870**. If it lands under 20 files and ≤3840, the veto was overpriced | M-c-callbacks close |
| ffi-pragmatist | with (a) landed alone and flag 13 unchanged, a `qsort` binding whose comparator passes either parameter to any other `extern` taking `ptr` — which every real comparator must, `ptr` having no dereference — **cannot reach exit 0** | M-c-callbacks close |
| ffi-pragmatist | `sqlite3_exec` needs no shim under (b) and still needs one under (a): §4.19 ladder step 3 is untouched by `const` | M-c-callbacks close |
| spec-warden | with the SEGV repair landed: `heroes measure` reads **exactly 3830**; every `extern` declaring a function-pointer parameter as `ptr` is exit 1 on the author's line; **zero** programs in `examples/` or `tests/golden/` need editing to keep building; `atexit(f: ptr)` and `qsort(… compar: ptr)` are exit 1, not 139 | **partly scored NOW, and the third clause is FALSE**: the flag fires twice on `examples/sqlite/main.hero`, a correct program passing a deliberate NULL callback. The seat measured four programs and the corpus has more. The rest is unscored until a repair lands |
| llm-ergonomist | re-running its experiment on N fresh models: under the status quo and under a `const` sentence, **≥95%** of `qsort`/`bsearch` attempts fail to build and the modal failure is at the **call site or comparator body**, not in the `extern` group; under a refusal sentence **≥80%** decline at zero `extern` lines. And under a `const` sentence only, **≥30%** will change or flag `spec:221`'s `path: cstr` | M-thesis-harness metric 2 |
| historian | the `ptr`-against-`char ***` acceptance is **not** on a compiler clock: GCC 14 exempts `void *` verbatim, so the same reproducer is exit 0 under clang 22.1 **and** gcc 14, while `ptr` against `const void *` and `const char *` fails on both | M-c-callbacks close, Linux container |
| historian | (a) alone moves 3/8 to **exactly 5/8**; an adapter reaches **7/8**, with only `signal` refused | the next callback measurement |

## What the brief got wrong, collected

Five, and every one was caught by a seat. This is the fifth sitting in a row
where that has happened, and the count is kept because CLAUDE.md §1 exists for
exactly it.

1. **"Three different reasons"** is the wrong axis and the wrong scale. Measured
   over a real header there are **five** causes and `const` is the **fourth** by
   frequency, at 2.6%.
2. **"`const` buys 3 of the 8"** — it buys **2**. `nftw` stays red on a
   non-const `struct FTW *` the brief did not look at.
3. **"`sqlite3_exec` does not bind"** — it binds today, in 28 lines of
   `static inline`, with no build rule.
4. **"C converts pointer types at a call and requires identity inside a function
   pointer"** is true and the *cause* named was wrong: the `@` route works
   because **the emitter writes a cast** (`(void *)&h3_table`, read out of the
   emitted C), not because C is lenient. The direct side is *looser*, not
   sounder, and the callback is the one place clang tells the truth.
5. **The option list omitted the adapter**, which is what the two languages
   closest to Heroes' architecture actually ship.

And one the convener should not have needed a seat for: the brief cited
`design.md Part 8` as naming the hole, in a diagnostic shipped hours earlier,
when Part 8 names 18 warts and not this one.

## Process notes

- Every seat worked from one frozen snapshot at `aab44f9b`, copied per judge,
  with `build/` and `target/` removed. No seat touched the repository, and the
  working tree stayed clean from the briefs going out to this file being written.
- The seed route was given in every brief: **2.8 s**, against ~28 s from
  `selfhost/`. No seat died on a watchdog.
- The ergonomist was given `spec/heroes-spec.md` and three tasks and nothing
  else, and its finding — that the document cannot reach the question at all —
  is the one the soundness lane would have lost.
- **Panel 083's compiler-engineer pinned its prediction at "M-ffi-ladder
  close", and that milestone closed 2026-08-12, four days before the sitting.**
  It was unscoreable the moment it was written. Both of this sitting's engineer
  predictions name an open row.

## Author's verdict

**Ratified 2026-09-05 by the author, in full** — *"ratify everything and commit"*,
given the same evening the sitting closed, as a blanket yes recorded as one.

**What the yes settles.** R1 through R6 stand as adopted: a `const` pointee
spelling and the wider C-pointer vocabulary are refused, and panel 083 R4's veto
carries forward stronger than it was written; Part 8 wart 19 lands at zero spec
tokens with the ffi seat's measured ordering rather than the brief's; the two
diagnostics the coordinator shipped that day are repaired here; design.md's two
expired sentences are corrected in place; the emitter-written **cast** is refused
permanently and the **adapter** is recorded with its return condition; and the
`quals` field on the existing `function_ty` case is the only admissible route if
the const half is ever closed.

**What the yes does NOT settle, and it is said here rather than assumed.** R3's
third item — defect 013, `atexit(f: ptr)` handed `nullptr` at exit 139 — was
**filed rather than repaired**, and the sitting put no recommendation with it
because both routes cost something a measurement cannot choose between: land
`-Wpedantic` and rewrite `examples/sqlite/main.hero`, which is a **correct**
program passing a deliberate NULL callback, or let `nullptr` inhabit a function
type, which is a spec change that does not close the defect anyway. A blanket yes
ratifies the *filing*, which is what the sitting decided. The route stays the
author's, `docs/work/DEFECTS.md` holds it with both prices, and the coordinator's
recommendation is recorded there rather than defaulted into the compiler.

**The process fact worth keeping.** This is the fifth sitting in a row where
every false premise in the brief was caught by a seat rather than by the
convener, and the first where the sitting's own subject turned out to be 2.6% of
the thing it was convened about. What produced that was one seat refusing the
sample and measuring the header.
