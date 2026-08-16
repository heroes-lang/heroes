# Panel 071 — six holes, and the one nobody listed

**Convened** 2026-08-16, M-selfhost-port. **Soundness-adjacent lane**:
ffi-pragmatist + spec-warden. Six queued items were all §4.19 and were going to
cost six sittings; the compiler-engineer was not seated because no item asks what
a form costs to implement until one of them is chosen, and the two seated seats
own the boundary and the budget. That is a deviation and it is recorded.

**The sitting refuted its own convener twice, refused two of the six outright,
and found a seventh hole larger than any of them.**

## What was asked

**A** — three `extern` parameter types are exit 2 when they disagree with the
header · **B** — an `enum` field is unbindable · **C** — a flexible array member
is exit 2 · **D** — two `cstr`s pointing at different bytes compare `true` ·
**E** — `ptr` and `cstr` are not in the spec's § Types table · **F** — `_ = f(x)`
is documented under `match`.

A seventh was **struck before the briefs went out**: it claimed `'A'` as an `i32`
argument is a compile error. Measured: it checks clean at exit 0, because a
character literal takes its context's type (`spec:62`). It had been true when
written and M-sized-integers made it false.

## The convener was wrong twice, and both are worth more than the items

**B was briefed as "35 SDL3 enum fields unbindable". It is 0.** The
ffi-pragmatist bound a real SDL3 enum field and ran it; the coordinator confirmed
with a header of its own — `kind: u32` over a C `enum` prints its value at exit
0. The brief measured *a mechanism panel 064 replaced*: integer fields ask class,
size and sign now rather than type identity, and all three answer correctly for
an enum. `kind: i32` fails because SDL3's enums are **unsigned**, and refusing
`i32` for an unsigned 32-bit member is the check doing its job. Measured over the
real SDL3 AST: **97 enum fields, 95 bindable, 0 unbindable** (2 unreachable
inside anonymous unions).

**A's causal story was half right.** *"The probe already errors on the author's
line"* was measured on `cstr` alone. `cstr`'s zero is a bare `0`, a legal
`int32_t`, so its return assertion compiles and the probe is the only error; for
`ptr` and `str` the zeros are `(void *)0` and `(HeroStr){0}`, so the **return
assertion fails first** and the probe's error is further down the output. The
class still works — but only if the matcher scans **every** stderr line, which
the brief did not say and the implementation must.

That is the fifth and sixth false premise from this seat in six sittings, and
both were caught the same way: a judge compiled the thing instead of reading
about it.

## Verdict table

| item | verdict | the finding that decides it |
|---|---|---|
| **A** parameter class | **approve** (ffi) | Seven shapes measured under the project's own flags, all stable, all containing `" to parameter of "`. `spelling()`'s last-quoted-name rule holds; `argument_columns` reproduced clang's caret **exactly** — 88/95/89 computed against 88/95/89 reported. **The condition is the design**: the *diagnostic* must fire unconditionally on the family and only the **`Fix`** may consult `spelling()`. `parameter_width` declines wholesale when the spelling is unknown, which is right for a width and catastrophic here: `Vec2`'s repair is *declare `record Vec2` in the group*, a sentence rather than a type name, and inheriting the decline reintroduces exit 2 on struct parameters — **356 of 1159** entry points by panel 052's count |
| **B** enum field | **VETO** (ffi) | It binds today. The change would loosen a sign check that is **correct on 95 of 95** real fields in exchange for nothing. The veto lifts for a **diagnostic-only** repair: `ffi_field_type` says *"is not `i32`"* and never says `u32`, which is the actual gap and is A's `Fix` machinery. And a false premise is exposed at `emit/extern_record.rs:202` — it says an enum classifies as **3**; measured, `__builtin_classify_type` applies the default argument promotions and answers **1**, so a comment reads as a refusal that never happens |
| **C** flexible array member | **approve the repair, object to the method** (ffi) | Route 1, `__builtin_types_compatible_p`, **fails**: clang says `char[4]` and `char[]` are compatible. Route 2 works and was compiled: `TAIL(T,m) = sizeof(T) − offsetof(T,m)` is **0** for a flexible array member and **≥ N·sizeof(elem)** for every sized one, including the padded and over-aligned cases, with no `sizeof` of an incomplete type anywhere. Replacing the array branch's `sizeof` conjunct with `TAIL` makes the assertion **fail instead of failing to compile**, so it carries `heroes-ffi-field` and §7's named exception reaches it **unchanged** — no new marker, no prose matching, which is the cgo lesson `ffi_narrowed.rs` already quotes |
| **D** `cstr` equality | **object**, with a conditional **veto** (ffi) | Reproduced against **real POSIX** rather than a model: `inet_ntoa` returns the same pointer twice, and after the second call both records read `192.168.1.1`. **`a == b` is `true` CORRECTLY** — both hold the same address, which is what `spec:71` says — and the defect is that `a.name` no longer holds what the author put in it. **Refusing `==` leaves that untouched**: `a.name.to_str()` still returns the wrong string silently, which is worse than a surprising `true` because nothing signals it. Cost of the refusal, measured over real headers: **43 structs** lose `==` (raylib 2/35, SDL3 19/136, sqlite3 9/23, curl 13/49) |
| **E** `ptr`/`cstr` rows | **object to the rows, approve a relocation** (warden) | **§ Types is a glossary, not a predicate** — grepped: nothing in the spec reads off it, and `spec:214`'s field list names `ptr`/`cstr` itself. Two rows close no hole and take the table from 8 to 10 of 13 writable type classes, still omitting `()`, nominal types, function types and `i32[4]` — which is panel 069's instrument. **What is real is placement**: `p: ptr @ nullptr` and `record Handle { p: ptr }` compile at **exit 0 with no `extern` group in the file**, so ordinary types are filed under FFI. **Two ballot options measured FALSE** — both "one sentence pointing at § FFI" wordings, at +31 and +15 — and pricing them is what found it; that would have been the ninth silent-direction sentence in this document |
| **F** `_ = f(x)` | **object to the framing, approve a §12 repair** (warden) | The question was where the rule is documented. **The spec never states the rule.** Measured: `f(1)`, a bare `5`, a bare `x`, an `if` and a `match` yielding `i64`, a discarded `i64?` — all exit 1 `discarded_value`, and grep for *"discard"* returns only `spec:121`'s escape hatch. `design.md:1718` carries it normatively and calls it *"the highest-frequency LLM habit error under value semantics"*. **Panel 068's row exactly**: design.md carries it, the compiler is the decision, the spec never caught up |

## The two live defects the sitting found in the compiler

**Neither is one of the six, and one of them inverts the thesis.**

1. **The `certain` fix turns a caught bug into a silent wrong answer.** Verified
   by the coordinator: `xs.push(4)` is `discarded_value`, and the diagnostic's
   machine-applicable repair is `_ = …`. But `push` **returns** the new array —
   so `_ = xs.push(4)` compiles at exit 0 and `len(xs)` prints **3**, where
   `xs @ xs.push(4)` prints 4. CLAUDE.md §8's CI rule is *"the applied fix
   compiles"*, and it does. A language whose thesis is that every plausible
   mistake is a compile error is here producing one **through its own repair**.
2. **`--in-place` does not modify the file.** Verified: `heroes check f.hero
   --apply --in-place` prints the fixed text to stdout, leaves the file byte-identical,
   and exits **0**. CLAUDE.md §10 names `--in-place` as *the* mutating flag.

## The seventh hole, which is larger than the six

**A tag-only `struct` is unbindable, at exit 2** (ffi-pragmatist; reproduced by
the coordinator). `struct Plain { int32_t a; int32_t b; };` with no `typedef`:

```
internal error: compiling the generated C failed:
  error: use of undeclared identifier 'Plain'
```

The emitter writes the bare record name where C requires `struct Plain`. Measured
tag-only definitions in real headers: **POSIX 35 of 46**, **curl 35 of 49**,
sqlite3 3 of 23, SDL3 5 of 134. That set contains `struct stat`, `struct
timeval`, `struct timespec`, `struct sockaddr_in` and `struct in_addr` — the
backbone of the platform §1.11 says everything comes from. It cost the judge a
shim header to run item D's experiment at all: a one-line `typedef` whose entire
content is a name, which is exactly the shim §1.11 exists to abolish.

## The resolution — provisional, author ratification pending

1. **B is refused on a veto and the item is struck**, not deferred: the field
   binds. What survives is a **diagnostic** repair — `ffi_field_type` must be
   able to say `u32` — and the deletion of `extern_record.rs:202`'s false `(3)`
   premise, or the test CLAUDE.md §11 requires for it.
2. **D is refused as scoped.** The `==` refusal treats the visible half of a
   borrowed-pointer defect while the silent half ships, and costs 43 real
   structs. **The conditional veto is recorded as binding**: if it is ever
   implemented as `partial::reaches`, that function answers `Some` at depth 0, so
   `cstr == nullptr` — `spec:223`'s documented null test, 9 sites in this tree —
   becomes a compile error. Any future rule must be *a `cstr` reached **through a
   field***, never *a type that reaches a `cstr`*.
3. **A lands**, with the design its condition names: the diagnostic fires on the
   family, only the `Fix` consults `spelling()`, and the matcher scans every
   stderr line rather than the first.
4. **C lands** by the `TAIL` form rather than by phrase-matching, with a message
   that names the flexible array member and gives the route — `ptr` and an
   accessor.
5. **E lands as R1**: the two rows *plus* deleting `spec:222`'s definition
   clause, **+4 net** against a **−23 named removal**. It repairs a second thing
   for free — `spec:222` gives `nullptr` to `ptr` alone while `c: cstr @ nullptr`
   compiles. **And the standing prohibition is adopted at zero tokens**: no
   sentence in this spec may refer to *"this document's types"*. That phrasing is
   what nearly cost panel 060 130 of 192 raylib entry points, and it is
   grep-enforceable.
6. **F lands as W1 + V1**: state the rule (+19 against a −9 removal) and fold
   `push`'s shape into the existing cost sentence (+10, a rewrite rather than an
   addition). Full package **3405 → 3438**, headroom 658, not within 10 of any
   ceiling so **not provisional on panel 019's tokeniser condition**.
7. **The two compiler defects and the seventh hole are filed as their own
   items.** The `certain`-fix inversion is the most serious thing in this
   sitting and does not belong to any of the six.

**What a veto at ratification would compel**: (3) and (4) are emitter-local; (5)
and (6) are spec text with named removals and revert cleanly.

## Predictions to score

| judge | prediction | at |
|---|---|---|
| ffi-pragmatist | a matcher on `" to parameter of "` gated by the existing `extern_at_line` turns all **seven** shapes into `ffi_parameter_type` at exit 1 with **no change** to `extern_probe.rs`, `location()` or `argument_columns` | M-binding-fidelity |
| ffi-pragmatist | binding SDL3's `SDL_Surface`, `SDL_DisplayMode`, `SDL_CameraSpec` and `SDL_AsyncIOOutcome` with every enum member `u32` needs **no shim and no compiler change** | M-complete-structs |
| ffi-pragmatist | `TAIL` replacing the `sizeof` conjunct turns a flexible array member into exit 1 naming the field, and **every existing array-field golden stays green** | M-complete-structs |
| ffi-pragmatist | a curl binding of `struct curl_fileinfo` will want `==` on the record; under D's refusal it needs a hand-written field walk, which is the shim §1.11 abolishes | M-package-manager |
| spec-warden | R1 lands at exactly **3338 / 3409 / max 3409** | the landing commit |
| spec-warden | with V1 landed, non-golden occurrences of `_ = <expr>.push(` stay **0** through M-selfhost-port close — today there is **1**, a deliberate `marker_mismatch` case, against **204** lines of the correct `@ … push(` form | M-selfhost-port close |

**Scored in this sitting**: the convener's *"35 SDL3 enum fields unbindable"* is
**falsified** — 95 of 95 bind. And its *"the probe already errors on the author's
line"* is **half falsified**: true of `cstr`, false of `ptr` and `str`, because
their zero values make the return assertion fail first.

## Conditions on the record

- **A**: the diagnostic must not inherit `parameter_width`'s decline-when-unspelled.
- **B**: the veto lifts only for a diagnostic-only change.
- **D**: the `partial::reaches` shape is vetoed outright; and the objection
  withdraws only if the **read** side is addressed with it.
- **F**: flips to veto if `_ = f(x)` is promoted **without** the rule and
  `push`'s shape — which is the defect above, made more discoverable.
- **The `()` watch-list item is dead as payment and dormant as observation**
  (warden). It is conditioned on metric 2, which `SCHEDULED.md` defers to v1, so
  *"owes a row if the baseline shows confusion"* cannot authorise a row for the
  whole life of v0. `docs/panel/OPEN-QUESTIONS.md` should say so.
- **No live instrument can score E or F**, so panel 012's prediction branch is
  closed for this sitting: everything is paid with a removal or stands as §12.

## Author's verdict

*Pending.*
