# Panel 096 — Three defects at a variadic `extern` argument

Date: 2026-08-28. **Soundness lane** (compiler-engineer + ffi-pragmatist), by
author instruction (`/decide` answers `2a` and `3a`). Convened as one sitting
because all three questions are one subject: an `extern` argument in **variadic
position**, which is the one place where C's own type checking stops and the
author's declaration is all there is.

**Status: `provisional — author ratification pending`.**

**The two seats disagree on Q1 and one of them holds a veto, so Q1 does not
land.** They also propose two *different* mechanisms for Q2 and reach opposite
answers on whether Q3 belongs in this lane. Every disagreement below is recorded
rather than smoothed, and each is grounded in something one of them compiled.

## The three questions, verbatim from the brief

**Q1.** `printf(format: "%d\n".cstr(), value: 4294967303)` with `value: i64`
prints **`7`** at exit 0. Proposal: where an argument to an `extern` is `.cstr()`
applied directly to a string **literal**, emit the C literal instead of the
runtime conversion, so `-Werror=format` can see it.

**Q2.** `function printf(format: cstr, value: f32)` is refused
`ffi_parameter_type` — *"declared narrower than the header's `double`"* — and the
header says `...`, not `double`. The equivalent C compiles at exit 0 and prints
`0.500000`.

**Q3.** `function printf(format: cstr)` is **exit 2**, `internal error`, from
`-Wformat-security` on the emitted probe: the compiler blaming itself for the
author's binding.

## Resolution — provisional

| | resolution | why |
|---|---|---|
| **Q1** | **does not land** | the compiler-engineer vetoed the mechanism; the ffi-pragmatist would take it only with the emission changed. **Both seats refuse the form that was briefed**, for different reasons. |
| **Q2** | **lands, REPAIRED not removed**, by the ffi-pragmatist's mechanism | both seats independently refused removal; the ffi seat's construction is strictly cheaper (no ABI bump) and separates the **position** rather than the type. |
| **Q3** | **goes to the full panel** | the compiler-engineer's call, and it is the conservative one: closing it mints a 21st `ffi_*` code, which is a diagnostic *class* and CLAUDE.md §4 puts that outside this lane. |

**Principle 0's first door is shut on all three**, measured by the engineer:
`selfhost/` declares 3 extern groups and 5 functions — `hero_write_err`,
`hero_word_bits`, `system`, `getenv`, `atof` — **none variadic, none a format
function, none taking a promoting type**, and no real `.cstr()` on a literal. The
compiler does not need any of this; it enters on §1.11/§1.12 or not at all.

## THE FINDING OF THE SITTING, and it is a memory-safety regression the brief would have shipped

The ffi-pragmatist made one condition and called it load-bearing rather than
cosmetic. **It is.** Reproduced by the coordinator independently, under the
project's own thirteen flags:

```
char *t = strtok("a,b", ",");                     /* the proposal as briefed */
  → compiles CLEAN, exit 0
  → runs:  exit 138  (SIGBUS)

char *t = strtok((const char *)"a,b", ",");       /* the condition */
  → error: passing 'const char *' to parameter of type 'char *' discards
    qualifiers [-Werror,-Wincompatible-pointer-types-discards-qualifiers]
```

**In C a string literal has type `char[N]`, not `const char[N]`** (C11 6.4.5p6),
so the bare literal reaches a `char *` parameter with nothing complaining, and
`strtok` writes into read-only memory. The thirteenth of this project's flags
exists for exactly that — `selfhost/cli_flags.hero:21-22` calls it *"panel 058's
cstr-write refusal at the C level"* — and only the cast lets it fire.

**So Q1 as briefed would have traded one silent-wrong-answer class for a SIGBUS
class.** The brief was written by the coordinator and did not see it.

And the case it protects is **already a fixture in the tree**, one level of
quoting down: `selfhost/emit_ffi_mutable.hero:111` holds
`strtok(target: "a,b".cstr(), sep: ",".cstr())` as a quoted Heroes program inside
a Heroes program. Nothing tests it as an emission.

## Q1 — the disagreement, and it is a real one

**compiler-engineer: VETO.** Three grounds, and the coordinator confirmed the
first and third independently:

1. **The rule is defeated by one line.** Prototyped, built, run (+53 lines over
   `emit_decls.hero` and `emit_ops.hero`, 62 s, working on the exact case). Then:
   `f = "%d\n"` followed by `f.cstr()` **still prints `7` at exit 0**. Re-measured
   by the coordinator: it does.
2. **The one real instance in the tree is out of reach.**
   `tests/golden/surface-fixtures/twoarity/as_int.hero:8` *is* the wrong binding,
   and `spec:200-233` forbids calling an `extern` across a module, so every real
   binding routes through a Heroes wrapper whose format arrives as a
   **parameter**. The rule cannot see it.
3. **design.md already refused this mechanism by name**, in the paragraph about
   `cstr` at the boundary. Quoted verbatim from `design.md:563-567`:

   > a `cstr` from `s.cstr()` can never be null and is null-checked anyway,
   > because the alternative is a rule that has to reason about **where a value
   > came from**, and **provenance is a premise about the world** (CLAUDE.md §11).

**ffi-pragmatist: approve-with-conditions**, and it answers ground 3 head-on: its
condition 3 states the narrowing as *"a string literal with `.cstr()` applied
directly to it, **in this expression**, passed to an `extern` declared in this
module"* — a fact about the value in hand rather than about where a `cstr` came
from. Its case for the value: the change turns a silent `7` into
`error: format specifies type 'int' but the argument has type 'int64_t'`, naming
`%lld`, which is §1.12's whole point.

**The crux, stated so the author can settle it**: is *"this expression is
`.cstr()` applied to a literal"* a fact about the value, or a premise about
provenance? The engineer says provenance and design.md forbids it. The ffi seat
says value and CLAUDE.md §11 permits it. **Both seats are reading the same rule
and reaching opposite answers, and neither is careless about it.**

**And a second condition that decides whether Q1 works at all** (ffi, measured):

```
(void)printf((const char *)"%d\n", v);                    → exit 1, -Wformat fires
(void)printf(hero_cstr_nonnull((const char *)"%d\n"), v); → exit 0, -Wformat BLIND
```

`guard_cstr_arguments` (`emit_ops.hero:254`) wraps every `cstr` argument, so
**keeping the guard makes the proposal buy nothing.** Dropping it on that one
argument has a precedent the author already set — `emit_ops.hero:218-221` excludes
an `@cstr` because *"the argument is the address of a local, never null — a guard
that cannot fail is read as protection that is there (author decision
2026-08-15)"*.

**Lifetime: NEUTRAL, and the brief's own hypothesis was false.** A Heroes literal
is *already* static storage duration — `HERO_STR_STATIC` at file scope, refcount
−1, `(__TEXT,__const)` under `nm -m`. Verified by the coordinator at
`runtime/heroes_runtime.h:88-94`. And the brief's `putenv` guess does not hold:
`putenv` is already refused `ffi_writable_parameter`, and its note already says
*"and on a literal it aborts"*. There was no pre-existing defect for the
substitution to fix.

Named bindings the ffi seat checked against the docs on this machine:
`CURLOPT_URL` copies (*"The application does not have to keep the string around"*),
so `examples/curl/main.hero:62` is safe either way; `CURLOPT_POSTFIELDS` does
**not** copy and `sqlite3_bind_text(…, SQLITE_STATIC)` requires the pointer to
outlive the statement — both would bite, and **neither is in the tree**.

## Q2 — both seats refuse removal, and propose different repairs

Removal is off the table and the reason is one measurement: **clang's variadic and
fixed messages are byte-identical.** `-Wdouble-promotion` with the same text
catches `pow(x: f32, y: f32) -> f64`, where the header really does say `double`
and the precision loss is real. Removing the flag trades a false refusal for a
true one and breaks §4.19's central promise. The ffi seat vetoes removal
explicitly; the engineer reaches the same place.

**compiler-engineer's mechanism**: `HERO_VPROMOTE` in `heroes_runtime.h`, wrapping
the six promoting declared types (`f32 i8 i16 u8 u16 bool`) with C's own promotion
target. +3 runtime lines, +25-35 in `emit_extern_probe.hero`, **ABI 16 → 17**, a
6-entry table. Zero in the checker, lowering, IR, spec or diagnostic codes; zero
golden churn (36 probe lines in 7 blessed files, none carrying a promoting type).
Adjacent shapes attacked 7/7 at slot 1 and slot 2 plus a non-variadic control.

**ffi-pragmatist's mechanism**: separate the **position**, not the type.

```c
#define HERO_VARIADIC(call, f) \
    (!__builtin_types_compatible_p(__typeof__(f), __typeof__(call) ()))
```

It spells **no parameter and no return type** — it compares the callee against an
*unprototyped* function type of the callee's own return type, taken with
`__typeof__` from the probe's own unevaluated call, because C11 6.7.6.3p15 makes
a type with an ellipsis never compatible with `T()`. Measured against real headers
with Heroes' own spellings: **9 of 9 correct, 0 false fires on the exact five that
panel 094 R6 measured false** (`strlen`, `abs`, `sqlite3_open`, `sqlite3_close`,
`sqlite3_column_int`). Usable in one compile via `__builtin_choose_expr`, whose
unselected arm clang does not type-check — so the variadic arm makes the promotion
explicit and the fixed arm keeps the check. **Zero ABI effect**: the probe is
`__attribute__((unused)) static`, never called, never emitted. Promotion table:
**one entry**, `f32`→`double`, because nothing else fires.

**Why the resolution takes the ffi seat's**: no ABI bump, one table entry instead
of six, and it isolates the property that is actually wrong — the **position** —
rather than enumerating types that happen to promote.

**This falsifies panel 094 R6 as applied here**, and both seats say so
independently. R6 refused *"the variadicity `_Static_assert`"* because §4.19
mandates the Heroes spelling differ from the header's, so a whole-type comparison
cannot isolate the ellipsis (4 false fires of 5). `HERO_VARIADIC` spells no type
at all. The distinction must be in the commit body or the next convener reads a
re-proposal.

## Q3 — the lane splits, and the conservative reading wins

**compiler-engineer: REJECT for this lane — it is a class, not an attribution.**
Measured: no matcher among the 17 in `selfhost/emit_ffi.hero:48-122` claims a
`-Wformat` line, and 20 `ffi_*` codes exist. Closing Q3 mints the 21st, which
CLAUDE.md §4 makes a full-panel path. A second cost the brief missed: the second
clang error sits at the **call** line, where `extern_at_line`
(`emit_ffi_narrowed.hero:156-164`) fails — machinery none of the four existing
exit-1 classes has.

**And Q1's repair reproduces Q3's defect**: with the literal emitted, the
`-Wformat` error has no matcher either, so it comes out as `internal error` at
exit 2. So Q1 and Q3 cannot be separated in shipping order.

**ffi-pragmatist: approve-with-conditions**, with the measurement that matters for
the instrument: the same probe, same TU, **with and without**
`#pragma clang diagnostic ignored "-Wformat-security"` gives **byte-identical
output**, including the `size_t *` vs `int32_t *` fire. So silencing it inside the
probe block costs the memory-safety instrument **nothing** — the probe's `a0` is a
parameter by construction — and the meaning is kept at the **call site** by Q1.

The resolution takes the engineer's, because a new diagnostic code is a class and
§4 does not let this lane mint one.

## Corrections to the brief, which the coordinator wrote

**Three of its numbers did not reproduce, and one of the corrections is itself
wrong.** All re-measured by the coordinator this session:

| the brief said | the seat said | measured now |
|---|---|---|
| 18 blessed emissions carry `hero_str_cstr` | **zero**, so Q1 has no blessed-C churn | **both half-right**: `tests/golden/*/*.expected` has **0**, and `tests/emission/` has **18 files**. `tests/emission/` *is* blessed C. **Q1 does have churn: 18 files.** |
| 19 call sites | 23 total, **17 reachable** | 27 by a `"…".cstr()` grep; the reachable figure is the one that matters and it is the seat's |
| zero `.cstr()` on a literal in `selfhost/` | zero | **2 hits, both inside ONE string literal** at `emit_ffi_mutable.hero:111` — a Heroes program quoted inside a Heroes program. The seats are right; the grep is not. |

The brief also claimed the probe *"is the only thing that sees a `size_t`
out-parameter declared `i32`"*. The ffi seat measured that it sees it **and the
build does not stop**: `getline(@n: i32)` against `size_t *` is a warning at
**exit 0**, because `-Wincompatible-pointer-types` is not among the thirteen as
`-Werror`. It could not demonstrate corruption and says so — the 8-byte write
landed in stack padding — so the claim is only what was measured: accepted at exit
0, out of bounds by C's rules, corruption at the allocator's discretion.

## Two live holes found on the way, to be filed separately

1. **`getline(@line: ptr, @n: i32, stream: ptr) -> i64` is accepted at exit 0**
   with an `-Wincompatible-pointer-types` warning, writing 8 bytes into a 4-byte
   slot by C's rules.
2. **`extern function strtok(target: cstr, sep: cstr, pad: str)`** — an arity
   mismatch against the header — is **exit 2**,
   `too many arguments to function call, expected 2, have 3`, from the return
   `_Static_assert`. The compiler blaming itself for the author's declaration:
   Q3's family, a different member.

Neither is folded into this sitting. The ffi seat's condition 8 asked for exactly
that and it is honoured.

## Author's verdict

**Pending.** Queued as an open item in `docs/work/DECIDE.md` naming `panel 096`.

**What a yes settles**: Q2 lands by `HERO_VARIADIC`, with `-Wdouble-promotion`
kept in `PROBE_ERRORS` and the panel-094-R6 distinction written into the commit;
Q3 goes to the full panel with the 20 existing codes on the page; Q1 does not
land in the form that was briefed.

**What a yes does NOT settle, and it is the sitting's one open question**: whether
*"this expression is `.cstr()` applied to a literal"* is a fact about the value
(CLAUDE.md §11 permits) or a premise about provenance (design.md §1.12 forbids).
Two seats read the same two rules and answered opposite. If the author reads it
the ffi seat's way, Q1 returns **with the cast and without the guard** — and with
the engineer's measured ceiling stated: **17 of 22 sites, and one rename defeats
it**, so it buys `-Wformat` inside a declaring module and must not be sold as
closing the class.

## Predictions to score

| seat | prediction | checkable at |
|---|---|---|
| compiler-engineer | `HERO_VPROMOTE` leaves all **36** blessed probe lines byte-identical, `printf(format: cstr, value: f32)` is exit 0, and `sqrt(x: f32)` is still exit 1 `ffi_parameter_type` | M-ffi-ladder close |
| compiler-engineer | if Q1 lands **as briefed** at any milestone, that close shows **either** a 21st `ffi_*` code in `selfhost/` **or** `grep -c "unused-but-set-variable" > 0` over a build of the 18 `hero_str_cstr` sources. Both are one command | that milestone's close |
| ffi-pragmatist | with all three landed under its conditions, `examples/curl/main.hero:62` compiles with **zero** clang warnings under the thirteen, needs no shim, no cast in the `.hero`, no change to its `extern` group — and all four rungs of §4.19's ladder run. Already compiled standalone: exit 0, zero warnings | M-ffi-ladder close |
| ffi-pragmatist | `pow(x: f32, y: f32) -> f64` stays exit 1 **and** `printf(format: cstr, value: f32) -> i32` becomes exit 0 printing `0.500000`, in the same commit, with `-Wdouble-promotion` still in `PROBE_ERRORS` | the landing commit |

**Scored now, against its own author**: the ffi-pragmatist's prediction at
`docs/panel/094:198-200` named `-Wformat-nonliteral` as *"a lever nobody has
pulled"*. The brief measured it at **100% false positives** — it fires on every
Heroes call to a format function, because the format is never a literal by
construction. **FALSIFIED**, and the seat recorded it against itself before being
asked.

## Consolidated conditions, if Q2 lands and Q1 ever returns

1. **The literal is emitted `(const char *)"…"`, never bare, never `&"…"[0]`.**
   Bare is `strtok` exit 1 → exit 0 → run **exit 138**.
2. **`hero_cstr_nonnull` is dropped on that argument and only that argument**,
   citing `emit_ops.hero:218-221`'s existing `@cstr` exclusion. Without it the
   change is inert.
3. **The narrowing is stated as a fact about the value in hand**, and the commit
   body states the ceiling: 17 of 22 sites, and `f = "%d\n"` defeats it.
4. **`-Wdouble-promotion` stays in `PROBE_ERRORS`**; the position is separated by
   `HERO_VARIADIC` + `__builtin_choose_expr`; the promotion table has exactly one
   entry; `argument_columns` is rebuilt against the **fixed** arm (measured:
   columns 26 and 30).
5. **The commit body cites `docs/panel/094` R6** and says what `HERO_VARIADIC`
   does differently, carrying the 9-of-9 measurement.
6. **`ffi_parameter_type` stops asserting *"the header's `double`"* for a variadic
   callee.** The header says `...`. A false premise inside a diagnostic is §8's
   own subject.
7. **Four `fixedbugs/` cases named after their defects** (panel 094 R5, §9): the
   `%d`-fed-`i64`, the variadic `f32`, arity-1 `printf`, and the one that will be
   forgotten — **`strtok` with a literal at a `char *` fixed parameter, asserted
   as an emission**, which is what condition 1 exists for and which no golden
   covers today.
8. **The two holes above are filed as their own items, not folded in.**

## Process notes

- Both seats compiled. The engineer prototyped Q1 end to end and ran it; the ffi
  seat built 18 numbered experiments under the project's own thirteen flags and
  rebuilt its compiler from the live seed mid-session because the tree had moved
  three commits and all four of its subject files were in that diff.
- The lane was the right gear for Q1 and Q2 and the wrong one for Q3, and it said
  so itself rather than deciding out of scope.
- The coordinator's brief carried three wrong numbers, one wrong hypothesis
  (`putenv`), one wrong reading of a defect (Q2 offered as a message fix), and one
  proposal that would have shipped a SIGBUS. Every one was caught by a seat, which
  is what the differentiated inputs are for.
