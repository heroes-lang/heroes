# Panel 063 — the direction nobody checked

**Lane: soundness** (compiler-engineer, ffi-pragmatist). Convened 2026-08-15.
**Provisional — author ratification pending.**

Both judges **approve**. They disagree on one thing — where Q4 is decided — and
that disagreement is resolved conservatively below, against the seat that raised
it.

## The proposal, verbatim

> `spec/heroes-spec.md:194` promises: *"clang checks every result type, constant
> and record field against that header"*. Since panel 060 a **struct** may be a
> result, and `assert_spelling.rs::return_check` answers `None` for `Ty::Named`.
> Its comment — *"No other type crosses the boundary"* — has been false since that
> sitting. Measured: `ColorAlpha` and `GetSplinePointLinear` in
> `examples/raylib/main.hero` carry **no `heroes-ffi-return` line**; a wrong struct
> result **builds at exit 0** while uncalled, and is **exit 2 with no marker** when
> called.
>
> **Q1** — what mechanism checks a struct result: `sizeof`, `_Generic`, or
> `__builtin_types_compatible_p`? State first what would make each wrong.
> **Q2** — does the assertion survive in an **unevaluated** operand, where the
> parameter check's conversion diagnostic did not?
> **Q3** — is a new diagnostic class needed, or does `ffi_return_type` reach it?
> **Q4** — `()` is admitted as an `extern` **parameter** and nothing rejects it.
> Is that a separate defect?
>
> **Conservative default**: Q1 = `__builtin_types_compatible_p`; Q2 = assume not,
> and use a probe; Q3 = no new class; Q4 = queued as a panel path rather than taken
> here.

## Verdict table

| judge | verdict | section | cost / delta | prediction | condition |
|---|---|---|---|---|---|
| **compiler-engineer** | `approve` | design.md §1.7, Part 5 (in neither); CLAUDE.md §12 via `spec:194` | **+109 / −45**, three files, **zero golden churn**, clippy 0. `assert_spelling.rs` 144→169, `extern_assert.rs` 236→256, `types/ffi_decls.rs` 205→224. 67 code lines, 42 comment; largest file lands at 256, under §11's 300 | At **M-selfhost-probe** close, `grep -c heroes-ffi-return` on raylib's emitted C reads **13**, `cargo test` 0 failures, and `git diff --stat -- tests/golden/emit` shows **0 files changed**. Falsifier: an unconditional macro makes that last number ≥110 lines across 5 files | **object** if (1) the struct-`constant` arm ships without a named falsifiable claim + a test that fires when it dies, (2) Q4 lands in this sitting, (3) the macro ships unconditionally. **veto** only if a new `heroes-ffi-` marker or a fifth `emit/ffi.rs` class is required |
| **ffi-pragmatist** | `approve` | design.md §1.11, §4.19, §1.12 / CLAUDE.md §12; `spec:193`. CLAUDE.md §7's named exception is what the defect defeats | One `#define`, no new diagnostic class, no new toolchain surface. 473 unit + 7 corpus tests pass; 12 of 13 golden groups, the one diff being one `#define` and a `#line` renumber by one | All **113** of raylib's struct-returning entry points bind with no shim and no assertion failure; `examples/raylib/main.hero` gains exactly two lines. **SDL3's `SDL_GetJoystickGUID` needs no shim**. Falsifier: any raylib return type that compiles today and whose new assertion fires | **object** if `_Generic` is chosen over the builtin. **veto** if any proposal reaches the **call site** — a cast, a `memcpy`, or a re-declared prototype to make the result agree |

## What the two produced independently, and agreed on

**Q1 — `__builtin_types_compatible_p`, and `sizeof` is refused for a stated reason.**
Both built the attack battery; the tables agree row for row.

| attack | `types_compatible_p` | `_Generic` | `sizeof` |
|---|---|---|---|
| correct result | accept | accept | accept |
| **identical layout, different tag** (`Vector4` / `Rectangle`) | refuse | refuse | **ACCEPT** |
| **identical size, different field types** (`{float,float}` / `{int32_t,int32_t}`) | refuse | refuse | **ACCEPT** |
| struct vs its own typedef alias (`Vector4` / `Quaternion`) | accept — one type, two names | accept | accept |
| anonymous struct, same members | refuse | refuse | ACCEPT |
| 304-byte hidden-pointer return (`VrStereoConfig`) | accept; refuses `Matrix` | accept | — |
| non-record C result declared as a record (`ColorToInt`) | refuse | refuse | — |
| `void`-returning C function declared as a record (`ClearBackground`) | refuse, cleanly | — | — |

The engineer states the reason `sizeof` loses, and it is not a worse measurement:
it is **a different question**. `sizeof` and offsets ask about **layout**; the
builtin asks about **type identity**, which is strictly stronger than layout
identity and therefore strictly stronger than register class. Panel 060's veto
case — `{float,float}` against `{int32_t,int32_t}` — is one size, one set of
offsets, **two register classes**, and the builtin separates them where `sizeof`
cannot.

**`_Generic` works and is still refused, on a defect this project has already paid
for twice.** A `_Generic` association type must be **complete**, and an incomplete
one is a C2y extension:

```
error: incomplete type 'struct HeroOpaque' in a '_Generic' association
       is a C2y extension [-Werror,-Wc2y-extensions]
```

Apple clang at this project's own flags accepts it **silently**; `-pedantic-errors`
catches it. That is the exact shape of the `HERO_RET_UNIT` `void`-association
defect, which survived two milestones on this laptop and failed on the first CI run
that compared two machines. `__builtin_types_compatible_p` has no completeness
constraint, and it is **already** the mechanism `HERO_RET_UNIT` uses
(`extern_assert.rs:164`) — so the family gains **no new dependency**. Cross-compiled
clean for `x86_64-linux-gnu`, `i386-linux-gnu` and `wasm32`.

**Q2 — the unevaluated operand survives, and the reason names why the parameter
side could not.** The parameter check needed a **conversion diagnostic**, and a
conversion inside an unevaluated operand is silent. This needs a **type-identity
fact**, and `__typeof__` gives the type of an unevaluated expression exactly. So
**no probe is needed**:

```c
#define HERO_RET_RECORD(c, T) __builtin_types_compatible_p(__typeof__(c), T)
_Static_assert(HERO_RET_RECORD(ColorAlpha((Color){0}, (float)0), Color),
               "heroes-ffi-return ColorAlpha Color");
```

Verified from both sides: `nm -u` on a unit whose only mention of the function is
the assertion shows **no undefined symbol** — the call is never emitted. Completeness
holds by construction: the `#include` is written at `emit/decls.rs:70`, before
`extern_assertions` at `:81`, and a function cannot return an incomplete struct by
value.

**Q3 — no new diagnostic class.** `ffi.rs::ASSERTION` is unchanged,
`ffi_declared.rs::wrong_type`'s `split(ASSERTION)` still yields `(name, type)`, and
`FAILED` still rejects clang's echoed source line. The existing message reaches the
new type verbatim:

```
error[ffi_return_type]: `ColorAlpha` does not return `Vector2`
                        — that is what `raylib.h` says, and clang read it
```

exit **1**, on the author's `.hero` line — for the uncalled case that was exit 0
**and** the called case that was exit 2. Free bonus: clang's own note reads
`__builtin_types_compatible_p(struct Color, struct Vector2)`, naming both structs.

## The correction to the proposal's premise, and it is the worse diagnosis

The proposal asked whether a wrong struct result could be **silently wrong**. The
pragmatist looked for the pair and **there is none**: C struct assignment is
**nominal**, not structural (C11 6.2.7p1), so two distinct tags are incompatible
inside one translation unit no matter what they contain. `ColorNormalize` returning
`Vector4` (`{float×4}`) declared as `Rectangle` (also `{float×4}`, same size, same
alignment, `_Static_assert`ed) is refused by clang. The only accepted pair is
`typedef Vector4 Quaternion;` — one type with two names, which is not a wrong answer.

So the class is **misattributed exit 2, never silent-wrong-answer** — and that is
**worse for the thesis, not better**. A silent wrong answer is a bug in a program.
An exit 2 saying *"internal error: compiling the generated C failed"* is the
compiler asserting **it** is broken when the author's `.hero` line is, which is
precisely what CLAUDE.md §7's named exception exists to prevent. The escape hatch is
closed by Heroes itself: discarding the result fires `unused_binding` at exit 1, and
there is no statement-position call.

## The number

Both judges counted, by different routes, and agree.

- **raylib: 113 of 600** entry points return a struct by value — 18.8%, 26 distinct
  types (`Image`×23, `Vector2`×20, `Color`×12 …), largest `VrStereoConfig` at 304
  bytes, smallest `Color` at 4. The milestone's **349 is the union**: 294 take one,
  113 return one, **58 do both**. So 236 are param-only and already covered; **113
  are the unchecked direction**.
- **SDL3: 4 of 1248** — `SDL_GUID` only. SDL3 returns `bool` 477 times and a pointer
  or an ID everywhere else.

Two libraries, two orders of magnitude apart. The fix is worth 18.8% of raylib and
0.3% of SDL3, and that spread is itself the finding: *how much this matters is a
property of the library, not of C*.

## The ABI, measured — the gap is type identity, not layout

The pragmatist ran Heroes against a C reference at every AAPCS64 return class,
byte-for-byte identical output, all clean under `--sanitize`:

| class | struct | size | Heroes = C |
|---|---|---|---|
| integer register (w0) | `Color` | 4 | ✓ |
| HFA s0–s1 | `Vector2` | 8 | ✓ |
| HFA s0–s2 | `Vector3` | 12 | ✓ |
| HFA s0–s3 | `Vector4` | 16 | ✓ |
| indirect x8 (+24-byte indirect arg) | `Matrix` | 64 | ✓ |
| indirect x8 | `VrStereoConfig` | **304** | ✓ |

**What the assertion would guard is already sound.** This is a pure type-identity
gap. The pragmatist withheld its veto on that basis, and its standing veto is
directed at the one repair that would change it: nothing may reach the **call
site** — no cast, no `memcpy`, no re-declared prototype. The layout stays the
header's.

## The residual hole the fix does not close — and the fix makes it *look* closed

The engineer found this and made it a condition. `__builtin_constant_p` returns **0
for every struct**, including a fully-constant compound literal. So the naive repair
— letting `Ty::Named` through the `else { continue }` gate — newly refuses
`constant WHITE: Color`, one of raylib's **26** `CLITERAL(Color)` macros that binds
and runs at exit 0 today. Guarding it (`Check::asks_for_a_value()`) keeps those
working and leaves this:

| | scalar `g_count: i32` | struct `g_pair: Pair2f` |
|---|---|---|
| type assertion | emitted | emitted |
| `__builtin_constant_p` | emitted | **not emitted** |
| verdict | `error[ffi_not_constant]` | **reaches the linker** |

§4.2's back door — a zero-argument accessor over a mutable global that returns two
answers — **stays open for struct constants**, and after this change it looks
closed: an auditor doing what panel 062's did now sees an assertion line where there
were none. That inversion is why the engineer requires the claim to be written as a
**falsifiable claim with a test that fires when it dies** (CLAUDE.md §11), and not
absorbed into a comment. The shape is
`a_struct_constant_cannot_be_asked_for_a_value`.

## The golden trap, and why the macro is conditional

Emitted unconditionally, the `#define` adds one preamble line — which **renumbers
every `#line N` restore beneath it**. Measured across `tests/golden/emit/`: **56
restore directives, 112 changed lines in 5 files**, in a directory where
`crates/heroes-cli/tests/golden.rs:517` forbids `UPDATE_GOLDEN` — a third forbidden
directory beyond CLAUDE.md §9's two. Emitted only when a record result exists:
**zero golden files touched**, full suite green.

The pragmatist, who did not make it conditional, measured the same thing from the
other side and said so: its one golden failure was exactly this and nothing else.
Two judges, two prototypes, one number.

## The disagreement, stated plainly

**Q4 — `()` as an `extern` parameter. Both agree it is a defect and both agree on
the repair. They disagree on who decides it.**

The pragmatist argues it belongs **in this sitting**: the refused set is **empty**,
so nothing is lost. No C signature makes it meaningful — ISO C has no zero-sized
object type, `void` is not an object type, and `f(void)` means *no parameters*,
which Heroes already spells `function f()`. `abs(n: ())` is already unreachable:
`()` has no literal, so it is `expected_expression`, and `abs(n: nothing())` dies at
the assertion, exit 2. A `()` parameter is **a declaration the language provides no
way to call**. And `ffi_decls.rs:91`'s `ffi_constant` already refuses `()` with the
same argument in the record: *"a `constant` names a value, and `()` is a type rather
than a value"*. So: no class created, four to six lines, take it here.

The engineer agrees on every one of those facts and refuses the conclusion, on
**CLAUDE.md §4's trigger being path-based, not class-based**: *"`crates/heroes/src/{lexer,syntax,types}/` behaviour"*. The repair changes what the **checker accepts**, and it lives in `types/ffi_decls.rs`. Its words: *"The soundness lane may hand the author the evidence and the 6-line patch; it must not land it."* It flips to **object** if Q4 lands here.

Both also report, independently, that **design.md and the spec are silent** on
whether `()` is a legal `extern` parameter — `spec:195` says only that a parameter
is declared *"at the header's own width and sign"*, and `()` has neither. Neither
judge invented a rationale from that silence; both reported it.

**Resolution: the engineer's.** Not because its argument is better — the
pragmatist's "the refused set is empty" is the stronger *design* argument — but
because the panel's rule is to adopt the most conservative resolution, and one seat
flips to `object` on this and the other does not flip on the reverse. The evidence,
the measurement and the 6-line patch are carried to the queue intact so the full
panel starts from a finished answer rather than a question.

## Resolution — `ratified 2026-08-24` (author instruction, *"ok ratifica anche quelli"*; § Author's verdict below)

1. **Q1 = `__builtin_types_compatible_p`.** `_Generic` is refused on the
   completeness constraint, `sizeof` on the two attacks it accepts.
2. **Q2 = no probe.** The assertion goes directly in the unevaluated operand.
3. **Q3 = no new class.** `ffi_return_type`, `ASSERTION`, `FAILED` unchanged.
4. **Q4 = queued to a full panel**, with both judges' evidence and the patch.
5. **The macro is emitted conditionally**, only where a record result exists.
6. **The struct-`constant` hole is written into the record as a falsifiable claim
   with a test that fires when it dies** — never absorbed into a comment.
7. §9's cases are owed and are not in either prototype: a `fixedbugs/` case for the
   struct result, one for the layout twins, one for `()`.

**What a veto would compel.** The pragmatist's veto is reserved for any repair that
reaches the **call site**; if that is ever needed, the whole approach is withdrawn
and the 6-row ABI table above is what would be at stake. The engineer's veto is
reserved for a new `heroes-ffi-` marker or a fifth `emit/ffi.rs` class — either
would make this a diagnostic-class decision, which **this lane cannot decide**, and
the sitting would be re-run full.

## What the soundness lane gave up

Three seats. The spec-warden would have priced the `spec:194` sentence — the repair
is expected to be **0 tokens**, since the sentence already promises the check and
only the compiler is short, but that is an assumption this sitting did not measure.
The llm-ergonomist would have tested whether an author reading `spec:193-195`
predicts that a **result** is checked and a **parameter** is converted silently —
which is exactly the asymmetry panel 064's ergonomist, sitting the same day, found
rescued three of its four `long`s by accident. The historian would have looked for
precedent on nominal-vs-structural result checking at an FFI boundary.

The lane was chosen because the proposal changes **no surface, no diagnostic class
and no spec token**. Q4 is the part that did not fit that description, and it is
exactly the part being handed on.

## Predictions to score

| judge | prediction | scored at |
|---|---|---|
| compiler-engineer | `heroes build examples/raylib/main.hero --emit-c \| grep -c heroes-ffi-return` = **13**; `cargo test` 0 failures; `git diff --stat -- tests/golden/emit` = **0 files changed** | **M-selfhost-probe** close |
| compiler-engineer | second falsifier: `emit/extern_assert.rs` exceeds **260** lines, meaning the mechanism grew past one row | **M-selfhost-probe** close |
| ffi-pragmatist | all **113** raylib struct-returning entry points bind with no shim and no assertion failure; raylib's example gains exactly **2** lines; SDL3's `SDL_GetJoystickGUID` needs no shim | **M-selfhost-probe** close |
| ffi-pragmatist | **`ldiv_t` cannot be bound on Darwin arm64 and this panel does not fix it** — `__builtin_types_compatible_p(int64_t, long) == 0`, same width, same sign, distinct types. Falsifier: a spelling that binds `ldiv_t` on this machine without `partial` | **panel 064's resolution**, whichever way it lands |

That last one is not this sitting's business and is recorded because the judge
produced it while measuring something else: it is a live §1.11 hole with a name, it
arrived from the **field** side, and it is the concrete program panel 064 — sitting
the same day on exactly that question — was asking for.

## Author's verdict

_Pending._

## Author's verdict

**2026-08-24: ratified** (author instruction, *"ok ratifica anche quelli"*).

**Verified in the port rather than in the Rust the sitting measured**, which
matters because the tree it ran against is now `archive/bootstrap-rs/`:
`selfhost/emit_extern_assert.hero:95` emits
`#define HERO_RET_RECORD(c, T) __builtin_types_compatible_p(__typeof__(c), T)`,
and `HERO_RET_UNIT` uses the same builtin at :78 — Q1 as resolved, `_Generic`
refused, and R5's *"emitted conditionally, only where a record result exists"*
held: `tests/emission/run-ffi-libm.c` carries `HERO_RET_UNIT` and not
`HERO_RET_RECORD`.

**What the yes settles**: Q1 `__builtin_types_compatible_p`; Q2 no probe, the
assertion in the unevaluated operand; Q3 no new diagnostic class; R5's
conditional emission. Both reserved vetoes stayed unspent — nothing reached a
call site, and no fifth assertion class was added — so the soundness lane was
the right lane and this ratification does not owe a re-run.

**What the yes does not settle**: Q4, which the sitting itself queued to a **full**
panel and which stays queued; R6's struct-`constant` hole, which is owed a
falsifiable claim with a test that fires when it dies rather than a comment; and
R7's three §9 cases. The lane's own stated cost also stands unpaid: no
spec-warden priced `spec:194` (expected 0 tokens, an assumption the sitting did
not measure), and no blind seat tested whether a reader predicts that a *result*
is checked while a *parameter* is converted silently.
