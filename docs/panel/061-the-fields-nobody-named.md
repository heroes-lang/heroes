# 061 — The fields nobody named

**Status**: `RATIFIED 2026-08-15` (was `provisional — author ratification pending`).
The author's yes is a **work order**, and its order is the ratified part: the five
swept sites, the completeness probe and the false sentences land *before* `partial`.
**Convened** 2026-08-15, hours after `M-struct-passing` closed, by author instruction
(*"ok rendili legabili"*) adopting the partial field list panel 060 had queued.
**Lane**: full, five judges. All five compiled or ran programs.

**The sitting was called to choose a spelling and found that the feature it was
spelling already ships, unmarked and unchecked — and that the milestone which
closed this morning emits a silently wrong answer.** Three judges object, one
threatens a veto on the ballot's own conservative default, and the warden vetoes
one option outright. Every one of those is a measurement.

## The proposal, verbatim

> **Q1** the spelling: **A** a trailing `...` line (+57) · **B** `record Font
> partial` (+36) · **E** Nim's shape, partial is the default and `complete` is the
> marker (+39). **Q2** is panel 060's refusal set complete — construction, `==`/
> `hash`, map key? **Q3** what does a nested partial record do to its container?
> **Q4** the diagnostic. **Q5** may a partial record be `@`-passed?
>
> Conservative default: Q1 = B · Q2 = the three · Q3 = transitive · Q4 = one code
> · Q5 = permitted.

## The verdict table

| judge | verdict | Q1 | Q2 construction | Q3 | rests on |
|---|---|---|---|---|---|
| **ffi-pragmatist** | object, **veto threatened** | wrong question | **permit** | emitter, not ABI | §1.11, §1.12, §4.19 |
| **compiler-engineer** | object | **B**, measured | refuse | **split** | §1.7, Part 5, §4.17 |
| **spec-warden** | object, **veto on A** | all three false | — | contagious | §1.6, §1.2, §1.0, §12 |
| **llm-ergonomist** | approve-with-condition | **E**'s polarity | — | — | the documents alone |
| **historian** *(advisory)* | approve, **object to E** | `partial`, not E | — | transitive | Nim, cgo, bindgen, Dart, ctypes, C |

## The finding that reframes the ballot

**Partial already ships.** `spec/heroes-spec.md:211` says a group's `record` is
*"the same name, all its fields"* and **nothing enforces it**. The ffi-pragmatist
bound `SDL_Event` with **1 member of 41** and ran it at exit 0 on today's compiler.
So panel 060's Q2 resolution — *"Q2 = complete"* — ratified a rule that was never
implemented, and this sitting's Q1 does not *enable* partial. Its real job is the
inverse: making **complete** checkable.

The cost of that gap is not theoretical. Three of raylib's four `Color` fields,
every `_Static_assert` green, zero warnings, **exit 0**:

```
record Color            Heroes prints  -16777216   (0xFF000000, alpha 0)
    r: u8               C prints       -16776961   (0xFF0000FF, alpha 255)
    g: u8
    b: u8
```

A transparent colour where the program asked for an opaque one, with nothing said
anywhere. That is a **silent wrong answer shipped by the milestone that closed this
morning**, and it is the class this language exists to make impossible.

## Five shipped defects, found by three judges independently

Panel 060's condition 3 asked for `Names::of()` (the C type) to be split from
`Names::satellite()` (the generated-function prefix). The split landed at the
function-definition sites and **five call sites were missed**, all of them reading
the type table where they need the prefix:

| site | what it produces today |
|---|---|
| `emit/aggregate.rs:92` (`aggregate_name`) | `error: call to undeclared function 'Color_eq'`, exit 2 |
| `emit/aggregate.rs:240`, `:251` | the same, for `==` and `retain`/`release` |
| `emit/descriptors.rs:229`, `:262` | `static const HeroDesc Color_desc` — a **global unmangled name beside the library**, CLAUDE.md §7 |

Measured on today's tree, on a **complete** group record: `a == b`, `[Color]`,
`{Color: i64}` and `Color?` all fail at exit 2 with `internal error`.

**The consequence is sharper than the bug.** The two operations this ballot spends
28–57 spec tokens forbidding **cannot be performed on any group record at all** —
so the refusal's control arm (*"a complete record CAN be compared"*) is unwritable,
and a golden asserting the new refusal would pass **vacuously**. That is gate row
`2675`'s finding for the fourth time in this project.

And a sixth, from the ffi-pragmatist: a group whose members are only `record`s
emits **no `#include`**, so its field assertions reference an undeclared type.

## Q1 — B, and the split that decided it was resolved by evidence

The ergonomist, blind, wanted **E**: make partial the default so the *unmarked*
record is the restricted one. Its reasoning is the strongest thing in the sitting —
under A and B, *say nothing and you have claimed `SDL_Event` is four bytes, the
construction is accepted, and SDL writes 128 bytes into it*. It fell into that trap
under both A and E, and reported the difference: **under E every wrong line it wrote
was a compile error.** Its sentence is worth keeping: *"Doc 2 reads better, Doc 3
writes safer, and I take the writer's side, because the reader can be wrong at no
cost and the writer cannot."*

**Two judges then falsified E's foundation from two directions, and neither could
have seen what the ergonomist saw.**

The **historian** read Nim's compiler source. `compiler/sizealignoffsetimpl.nim`
sets `typ.size = szUnknownSize` when `completeStruct` is absent — and **everything**
PR #13926 changed is `typ.size`. Nothing in that path refuses **construction**. So
Nim, the language E copies, ships exactly the hazard panel 060 measured at exit 139.
*"E claims Nim's authority for a refusal Nim does not make."* And E's cost is on the
record with a date: Nim issue **#19040, open since 2021-10-22** — five years of a
diagnostic naming a repair that does not work, *"despite what the error message
suggests"*. That is what happens when the restricted form is the **unwritten**
default: the message must explain a class the reader never typed.

The **compiler-engineer** found the one that settles it. Under E, `extern_record.rs`
asserts every field the author names — so **the default program gets *fewer*
`_Static_assert`s and the author opts in to being checked.** For a robustness rule
that is the wrong direction, and it is invisible from the spec alone, which is why
the seat that wanted E could not weigh it.

**A is vetoed** (spec-warden) and separately refused by the engineer's measurement.
Its sentence reads *"`Point(...)`, `==` and a map key are compile errors"* — and
`Point` is **this document's own record**, constructed at `spec:182`. It tells the
reader the spec's only worked construction is an error, re-using `...`, the token it
defined nine words earlier. Panel 060's failure mode, repeated. The engineer priced
its parse at **+47 lines over B, five of them in the lexer** (a new
`TokenKind::Ellipsis`), plus a forced arm in an exhaustive match and a signature
change that makes `case_block` carry a refusal it does not need. B is 30 parse lines
in two files and costs **nothing in the identifier namespace** — `record partial`
with a field named `partial` still runs, measured.

**Q1 = B.** The ergonomist's problem is real and is answered below by a mechanism no
option on the ballot contained.

## The mechanism nobody put on the ballot, found twice independently

The spec-warden and the ffi-pragmatist, working separately, compiled the same thing:
a never-called **positional** probe.

```c
Font f = {0,0};   /* -Wmissing-field-initializers: missing field 'fileName' initializer */
```

The **designated** form the emitter actually writes — `(Font){.baseSize=…}` — warns
**nothing**, even under `-Wextra`, and that is precisely why the `Color` bug above is
reachable. A positional probe names the missing field, by name, on Apple clang today,
at **zero spec tokens**. Unions correctly give no signal, because naming one member of
a union *is* complete.

This is the answer to the ergonomist's condition (b) — *"`complete` must be checked
against the header; if it is unchecked, all three are equally unsafe and Doc 3 has
only added a word"*. It is checkable, positionally, and it protects the **unmarked**
record, which `partial` does nothing for. It should land whether or not partial does.

## Q2 — the sitting's one irreconcilable split, and it is about construction

**The ffi-pragmatist threatens a veto if construction is refused**, on three
measurements:

- The zero-fill is **deterministic** — 0 of 124 unnamed bytes survive, identically at
  `-O0` and `-O2`. It is not undefined behaviour; it is a defined wrong value.
- **The same SEGV is reachable from a *complete* binding.** `record Image` with all
  five fields named, `data: ptr` set to `nullptr` — `ptr`'s only literal, which the
  spec's own FFI section offers — gives `AddressSanitizer: SEGV on unknown address
  0x24`, exit 134. So refusing construction does not buy §1.12; it buys one route to
  a crash that has others.
- Refusing it **deletes the only expression in the language that produces an
  `SDL_Event`**, so `SDL_PollEvent(@e)` becomes unwritable — and that binding is the
  reason partial was adopted. Its acceptance test runs today at exit 0, clean under
  `--sanitize`, with **zero lines of shim C**; under the ballot's default it needs a
  hand-written `sdl_shim.c` forever, for the one library the feature was bought for.

**The compiler-engineer refuses construction**, and its ground is the one fact
nobody disputes: construction is the only operation that *writes* the unnamed bytes.
Everything else copies. It walked `descriptors.rs` and `counted.rs` and split the
paths by touch:

*Touches → refuse*: construction, `equality_body`, `hash_body` — and everything
reached through a `HeroDesc` (`array.c:117,165`; `map.c:85,86,165,190,207`;
`map-write.c:47,48,172`) sits behind those two.
*Copies only → permit, each measured*: `desc_copy`, `Op::CopyOut`, the prologue
copy-in, array element `memcpy`, a field read.
*Already refused*: `sort` (`unsupported[builtin]`), `print` (`bad_operand`).

**The resolution adopted**: `==`, `hash` and a map key are **refused,
transitively**; **construction is permitted**, and the completeness of the *unmarked*
record is enforced by the positional probe instead. That takes the pragmatist's
veto seriously on the ground it was raised — a refusal that deletes the feature's
own acceptance test is not conservatism — and answers the engineer's hazard with a
check rather than a prohibition. The engineer's own alternative is taken for the
pair: `_eq`/`_hash` for a partial record are **still emitted**, as `hero_panic`
bodies, so `descriptors.rs` never holds a null and a hole in the containment walk
costs a named abort rather than a wrong answer (CLAUDE.md §11's loud direction;
panel 022's `pc 0x0` stays impossible).

**And the engineer retracted its own panel-060 price, in three places.** *"An arm in
`perfn::every_generated_type`"* was **backwards** — skipping a partial record there is
what *creates* the null. *"An entry decision in `descriptors::generated`"* was wrong;
that file is unchanged. *"A refusal at every container site"* was half right: one
transitive predicate asked at three sites. Its measured total is **298 lines across
11 files, zero under `ir/`, zero lexer, zero runtime, no new `Op` or `Ty`** — 0.85%
of the compiler, and **not core**: it restricts construct 3 rather than adding one.

## Q3 — the proposal's default was wrong, and it costs the milestone its own use case

The engineer split it and both halves are measured:

- **Construction: per-declaration, NOT transitive.** `Outer(f: a)` copies a whole
  `Font` that came from C — measured safe under `--sanitize`, the unnamed `texture`
  intact. The proposal's transitive default would make `record Outer { f: Font }`
  unbuildable, and raylib's `RenderTexture { Texture texture; Texture depth; }` **is
  exactly that shape**. The default costs the milestone its own use case for no
  safety.
- **`==` / hash / map key: transitive, and it must be.** `[Font] == [Font]` reaches
  `Font_eq` through `hero_array_eq`; `{i64: Font}` through `map.c`; `Outer == Outer`
  through the generated `Outer_eq`; and a **variant case field** through
  `variant_equality_body`. All four measured firing.

The spec-warden reached the same rule from the budget seat and wrote it in six
tokens — *"for it and for any value holding it"* — a rule rather than a list, so
arrays, maps, `T?`, variant payloads and a future `sort` are covered without being
named. The historian confirms transitivity is unanimous in precedent: C forbids an
incomplete type *"to declare a variable or field"*, ctypes forbids it as a struct
member, and Dart's stated motive for removing empty structs included the nested case.

## Q4 and Q5

**Q4 — one code**, and the engineer found its message currently false for the
transitive cases: it prints ``` `[Font]` names only some of its C struct's members ```
and `[Font]` is an array with no C struct. §4.17 requires it to name the **partial
record reached** and the path to it.

**Q5 — permitted**, and both compiling judges compiled it. §4.8's copy-out is
`*ph0_f = h0_f`, a whole-struct assignment of a size clang computed, not a field
walk; 124 of 124 unnamed bytes survive a full `@` round trip. The ffi-pragmatist
asked to be told plainly rather than have it forbidden out of caution, so: it is
fine.

## The numbers, corrected — and panel 060's were wrong

| | crossings | entry points |
|---|---|---|
| raylib, complete list, **as actually shipped** | **226** of 349 | 478 |
| raylib, complete + a `ptr` that binds any object pointer | 337 of 349 | 589 |
| raylib, **partial** | **346** of 349 | **598** |

**Panel 060's "337 of 349" is the middle row and it is not what shipped.**
`_Generic(&((Font*)0)->recs, void **: …)` **fails**, because `Rectangle *` is not
`void *` — so `Font.recs: ptr` is `error[ffi_field_type]` with no type to correct it
to. 21 of raylib's 40 unbindable fields are typed pointers, blocking `Font`, `Mesh`,
`Shader`, `Model`, `Sound`, `Music`, `AudioStream`, `FilePathList` and
`ModelAnimation`.

**SDL3 is worse than 060 said and better where it matters.** 8 by-value crossings,
not 15 — and **7 of the 8 need `SDL_GUID` (`Uint8 data[16]`, zero bindable fields),
so partial buys 0 of them.** What it buys is the **addressable local**: 245 entry
points touch a declared struct, **178 complete → 235 partial**. `SDL_Event` fails a
complete list on exactly one field, `Uint8 padding[128]`.

**The array residue does not dissolve, it concentrates**: 3 under partial, all
`VrStereoConfig`, which has eight array fields and zero bindable ones, so it hits
`empty_record` instead.

## Principle 0 — the proposal argued the wrong ground, and the warden said so

*"raylib 337 → 349"* is 96.6% already bindable — a margin, not a failure — and
SDL3's by-value surface is 8, of which partial buys none. §1.12 **explicitly
forecloses** the ground the proposal leaned on: *"it does not suspend Principle 0 …
'it would be safer' is not an entry ticket."*

The form enters on two grounds the proposal did not state, both the warden's:

1. **The rule is unsatisfiable.** *"All its fields"* cannot be written for a header
   with an array member or an anonymous union — `SDL_Event`, `sockaddr`, `struct
   stat` cannot be declared completely **at any price**, while the compiler silently
   accepts the short list the spec forbids. Under CLAUDE.md §12 one of the two is
   wrong today.
2. **The thesis branch, §1.0's second limb.** A program passes `heroes check` at
   exit 0 and dies at exit 134 with no diagnostic, because a field the author never
   named was zero-filled. *A plausible model mistake that is not a compile error is
   the thesis failing.*

## Three false sentences, one of them about the next milestone

**The seventh**, measured by the warden and reproduced here: `spec:64` says
*"Character literals are `i64`"* and they are **context-typed like every other
literal** — `b: u8 @ 'a'` prints 97, `c: i32 @ 'z'` prints 122. The direction is
what makes it urgent: a reader who believes the spec thinks `s[i] == 'a'` is a type
error, which is **the most common idiom in a lexer, and the lexer is the next
milestone**. Repair costs **0 tokens**.

**The eighth** is *"all its fields"* itself, above.

**A candidate ninth, reported and not spent**: `add == add` runs and prints `true`,
`add == sub` prints `false`. Functions are values (`spec:103`) and appear in neither
list at `spec:71`. Panel 053's `cstr` class, one type later.

## The provisional resolution

Adopted as the most conservative reading every measurement supports. **The order is
part of the resolution and is the sitting's main instruction.**

**First, and before partial lands** — because without these the new refusal has no
control arm and a golden for it passes vacuously:

1. The **five `names.of` → `satellite()` sites** (`aggregate.rs:92,240,251`,
   `descriptors.rs:229,262`) and the **records-only-group `#include`**, each with a
   `#~`-annotated golden. Both compiling judges made this a condition.
2. The **positional completeness probe**, at zero spec tokens. It is what turns the
   `Color` bug from a wrong colour into `missing field 'a' initializer`, and it
   protects the unmarked record that `partial` does not reach.
3. The **three false sentences**: `spec:64` (0 tokens), *"all its fields"*, and the
   `==` list's missing function row reported rather than spent.

**Then partial**, at the spec-warden's wording (**+28**, cheaper than all three
ballot options and the only one that is true), with:

- **Q1 = B**, `record Font partial`.
- **Q2** = `==`, `hash` and map key refused **transitively**; **construction
  permitted**; `_eq`/`_hash` still emitted, as `hero_panic` bodies.
- **Q3** = construction **per-declaration**, comparison **transitive**.
- **Q4** = one code, naming the partial record *reached* and the path to it.
- **Q5** = permitted.
- **`heroes fmt` and `printer/dump.rs` grow their arm in the same commit** — see
  below; it is non-negotiable and it is the sitting's most alarming finding.

## The finding nobody asked for, and it is panel 060's again

The compiler-engineer, unprompted, checked what `heroes fmt` does to the new word:

```
$ heroes fmt fmt2.hero
extern "p61.h"
    record P61Font              # `partial` is GONE. It parses. It checks clean.
        baseSize: i32           # And it is now constructible.
```

**And the guard that exists to catch exactly this is blind in the same way.**
`printer/tests/mod.rs::assert_canonical` asserts `dump(text) == dump(fmt(text))` —
*"fmt changed the tree"* — but `printer/dump.rs:69` drops the word too, so **two
blind spots cancel into a green test.** The engineer proved it both ways: unchanged,
the probe passes; with one arm added to `dump.rs` it fails with
`left: "record Font partial" right: "record Font"`.

This is panel 060's `fmt_extern.rs` hoisting, one milestone later, in the same file
family, with the round-trip test complicit. A formatter that silently deletes a
safety marker is the worst shape this defect class takes, and it is now the second
time it has been found by a judge rather than by a test.

## Predictions to score

| judge | prediction | milestone |
|---|---|---|
| compiler-engineer | spelling B lands in **≤320 added lines** across **≤14 files**, with **zero** lines under `crates/heroes/src/ir/` and **zero** new `TokenKind`. If any `ir/` file changes, `partial` is a core construct and §1.7 priced it wrong. Instrument: `git diff --stat` | M-selfhost-probe close |
| compiler-engineer | fixing the two `names.of` sites turns `Color?`, `[Color]`, `{Color: i64}` and `c == c` from exit 2 into exit 0 or a named exit 1, and **no third site** needs changing | the commit that sweeps them |
| ffi-pragmatist | under the ballot's Q2, `examples/sdl3/main.hero` binding `SDL_Init`/`SDL_PollEvent(@e)`/`SDL_Quit` **cannot be written with zero lines of `.c`**. Falsified the moment that file exists and prints a non-zero event type with no hand-written C. Instrument: `heroes run` over the corpus | whichever lands partial |
| ffi-pragmatist | bindgen's extra refusal (`Copy`/`Clone`, PR #3110) does **not** transfer, because its cause is unknown layout and Heroes' layout is clang's — zero ASan reports traceable to a copy, pass or return of a partial record | M-ffi-ladder close |
| spec-warden | with the +28 clause and the sites fixed, the `Color` shape moves from **exit 0 with a wrong colour** to **exit 1 with one diagnostic** on 3 of 3 refusal sites, and a `run/` case comparing two *complete* group records runs at exit 0 where today it is exit 2 | M-selfhost-probe close |
| llm-ergonomist | n=20 per document: under A and B **≥8/20** declare `SDL_Event` with one field and no marker and build it; under E **≤1/20**. Under E **≥6/20** omit `complete` on `SDL_Rect` — so E's first-try-clean rate is predicted **lower** and its silent-error rate near zero. **Score as two numbers, not one** | when metric 2 exists |
| historian *(advisory)* | no shipped language refuses *reading*, *passing* or *returning* a partially-described type whose layout the C compiler supplies. A dated counterexample retires the permission set | standing |

## What each seat gave up, and what the lane earned

The full lane earned itself for the second sitting running, and this time in a way
that should be recorded: **the seat that could see the spec and nothing else wanted
the option the seat that could see the compiler proved less safe.** The ergonomist
was right that the unmarked case is the danger and wrong that E fixes it; the
engineer could see that E *reduces* the assertions a default program carries, and
could not have known how badly the unmarked case reads. Neither verdict is
complete alone, and the resolution — B, plus a probe neither of them proposed —
belongs to the disagreement rather than to either seat.
