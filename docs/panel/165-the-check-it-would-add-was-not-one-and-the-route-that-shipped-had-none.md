# Panel 165 — the check it would add was not one, and the route that shipped had none

Convened 2026-09-19, at M-declared-extents step 1, on the one row of the chain
that was scheduled **behind a measurement rather than behind a decision**. Five
seats, a completeness critic, six briefs written to disk before any seat started,
and the working tree frozen from the briefs going out until this file was
written.

The question: **panel 164 queued route 6 — the extent declared on an `extern`
parameter, `function arr_len(s: i8[8])`, with the compiler CHECKING it. The
measurement is run. Adopt, refuse, or leave queued?**

## The proposal, verbatim as panel 164 wrote it

> **Route 6 — declare the extent on the PARAMETER**, `function arr_len(s: i8[8])`,
> which is how C spells it. Refused today at `error[ffi_type]`. It needs **no new
> expression, no lend, no built-in and no position rule** — one widening of
> §4.19's parameter list, with ordinary type identity doing the matching. **It is
> the only route of the seven where the compiler CHECKS the extent** instead of
> trusting the author or the callee. Its whole value is a number nobody has
> measured: how many real headers spell the parameter as an array. **Queued, not
> adopted**, because adopting an unmeasured route is the cheap move.

## The resolution, in one line

**Route 6 as spelled is REFUSED on two vetoes. The sentence that justified it is
false and was falsified three times with three different instruments. What the
sitting adopts instead is the repair of what is already broken: three defects,
filed as 063, 064 and 065, one of which corrupts memory at exit 0 — and the
pricing of ROUTE 12, a header `constant` as the extent, which nobody listed and
which is the only form that is both portable and checked against the header.**

## The sentence route 6 rested on is false, and three instruments say so

Panel 164: *"the only route of the seven where the compiler CHECKS the extent."*

| seat | the instrument it used | what it got |
|---|---|---|
| ffi-pragmatist | `_Generic` on the function's address | `long (*)(const char *)` **passes**; `long (*)(const char (*)[8])` **fails** |
| compiler-engineer | `__builtin_types_compatible_p` | `void f(int8_t[8])` and `void g(int8_t*)` are **one type** under `-std=gnu11` |
| coordinator | clang's own AST, both dump forms | `f_ptr` and `f_arr8` are both `{"qualType": "char *"}` |

C adjusts `T a[N]` to `T *a` (C11 §6.7.6.3p7), so the header's declaration of that
parameter **contains no extent to check against**. Route 6's check compares the
author's declaration to the author's argument. **It is an author-against-author
check wearing the sign of a header check.**

**And the inversion is complete**: route 4 already checks a field's extent
*against the header*, measured on the live tree —
`error[ffi_field_type]: Slot3.name is not i8[20] in wrong.h — clang read the
header's struct and the field disagrees`. Route 6 would **remove** a
clang-verified extent and put two author-written numbers agreeing with each other
in its place, then write through it.

## The verdict table

| seat | verdict | rests on | cost / delta | prediction | condition |
|---|---|---|---|---|---|
| **compiler-engineer** | **veto** | design.md §1.7, Part 5, §1.12, §4.19 | ~120–190 selfhost lines, 6–8 modules, ≥1 `DECIDED` raise; `emit/gate.hero` at 360 against a `DECIDED` 365 | `git diff --numstat` over `selfhost/` exceeds **300** insertions and `suite_layout.hero` changes | a build-time probe that reads the extent back out of the header would drop the §1.12 objection |
| **spec-warden** | **veto** | §1.6 payment, Principle 0 | five drafts priced REAL on `claude-opus-5`: merge **+28**, standalone **+55**, with caveat **+75**, truthful about the boundary **+80**. **No budget breach** — worst case 8246 of 10240 | if route 6 lands, `extern` array-spelled parameters in this repo are **0** at that close and **≤2** at the next | — (Principle 0 unmet, not a price) |
| **ffi-pragmatist** | object | §1.11, §4.20 | no ABI change: three call forms diffed **byte-identical** at `-O0` | of the eight both-platform fixed functions, **zero** bind through route 6 without a shim | widen `f.ptr()` to any fixed array with a C spelling instead |
| **llm-ergonomist** | object, **veto** on one form | spec § 3, § 5, § 9, § 13 | — | as proposed, ≥9 of 12 write `copy_name(dst: t.name)` with no `@` and read C's bytes back | one spec sentence: a fixed-array parameter C writes through is `@`, at declaration and call site |
| **historian** (advisory) | approve | Ada RM B.3(70), 4.6(37); bindgen; D bug 8887 | — | within two milestones, **zero** array-parameter declarations whose extent is an `extern constant` rather than a literal | a precedent that checks an extent it does not know at check time |

**Two vetoes, and neither is a price.** The compiler-engineer's rests on §1.12:
route 6 substitutes a weaker check for a stronger one and then writes through it.
The spec-warden's rests on Principle 0: nothing in the tree needs it, and the
warden said the budget was clean **before** giving the veto, so a ceiling argument
could not stand in for a justification argument.

## What the census measured, and what it turned out not to decide

Four legs, one instrument, coverage reported rather than assumed:

| | Darwin | Linux arm64 | Linux x86-64 | Windows UCRT + shared |
|---|---|---|---|---|
| `.h` walked | 3120 | 5404 | 5411 | 345 |
| parsed / failed | 127/0 | 333/0 | 334/0 | 7/0 |
| parameters spelled as an array | **82** | **182** | **182**, the same SET | **0** |
| with a fixed extent | 31 | 59 | 59 | 0 |
| fixed **and** byte-typed | 12 | 31 | 31 | 0 |
| byte-typed and fixed **on both** Darwin and Linux | — | **0** | **0** | — |

**The Windows column covers the C library and `shared`, and NOT the Win32 API.**
`um` is 1516 headers and its census was still running when this file was written:
**unrun**, in those words, and not assumed to agree with the other two. What it
cannot change is the UCRT result, because the UCRT is the C library a Heroes
program meets first.

**Windows spells none at all**, and the reason is structural rather than small:
the UCRT assembles declarations from macros that take the type and the name as
separate arguments, so an array spelling is not a form that occurs. A second,
independent text instrument over all 66 UCRT headers agrees: zero.

**And the same constant is a different number on each of the three platforms**,
each compiled and run on a real machine:

```
L_tmpnam = 1024  Darwin        20  glibc        260  Windows (the box, 2026-09-19)
```

Odin's shipped hand-written binding says **15** for Windows. It is stale, and the
historian had described that exact failure mode — *"family 1 needs a whole second
instrument to stay true"* — a few paragraphs before its own example proved it.

## The three routes nobody listed, and the critic found two of them

**Route 12 — a header `constant` as the extent, `i8[SL_NAME_LEN]`, checked by
clang against the header.** Every seat and the coordinator treated this as a wall,
because `error[expected_array_length]: expected the array's length after `[`,
found a name` refuses it today. **The wall is the Heroes grammar and nothing
else.** Run:

```c
_Static_assert(sizeof(((struct sl2 *)0)->name) == SL_NAME_LEN, "...");  /* passes  */
_Static_assert(sizeof(((struct sl2 *)0)->name) == 20, "...");           /* fails:
                                             expression evaluates to '8 == 20' */
```

The macro expands in the emitted C **against the real header**, so a named extent
is portable *and* header-checked at once. **This dissolves the historian's
Thread A** — *"no language in this survey does both"* — and it dissolves the
coordinator's own headline finding, which was that a declared extent cannot be
portable. It cannot, **as a literal**. As a name it can, and the instrument is
already in the compiler.

**Route 13 — declare which argument carries the extent and derive it from the
field**, Heroes *writing* what `__counted_by` says rather than reading it. The
field's extent is header-checked, so the derived number is header-checked: the
exact property three seats proved route 6 cannot have. Unpriced, and the critic
says so.

**Route 14 — tighten route 3's own extent check**, proposed independently by the
spec-warden and the ffi-pragmatist. It costs about zero spec tokens and it closes
defect 063. Route 6 does not close it: route 6 parks a tighter form beside a
looser one that stays legal, and `n: 4096` compiles the day after.

## Three defects, all verified twice, and the register said zero

| | what it does | filed |
|---|---|---|
| **063** | `f.ptr()`'s call-stated extent is unchecked: `n: 64` over an `i8[8]` field **corrupts the sibling field**, `id` 7 → `1094795585` (`0x41414141`), exit 0; ASan reports a stack-buffer-overflow READ and WRITE | §1.12, rank 3 |
| **064** | `t.name == u.name` on two fixed fields: `check` exits 0, `build` exits with `internal error: use of undeclared identifier` | the class is every `.binary`; `.call` is the unguarded sibling |
| **065** | C writes into a binding declared `=`, with no `@` at the declaration, the parameter or the call site: `72` → `65`, exit 0 | falsifies `spec § 3` and `spec § 5` |

**063 was shipped eight commits ago**, at `ef7b013b`, as defect 061's repair.
**065 is demonstrated as correct behaviour** by a golden this repository ships —
on a `@` binding, which is why it never showed. **064 is a prerequisite of route
6's own first act**, because its unguarded sibling `.call` arm is held shut only
by the two refusals route 6 exists to remove.

## What the briefs got wrong, and the coordinator wrote every one of them

Recorded because a brief is the one document in a sitting nobody is assigned to
check — the seats check the world against the brief, and nothing checks the brief
against the world.

- **`check/ffi.hero` is 275 `code_lines`, not 384.** 384 is `wc -l`;
  `.claude/rules/module-shape.md` names that mistake verbatim, and `layout` is
  green. The engineer caught it.
- **Route 3 did not ship at M-readable-bytes.** It shipped after the close, at
  `ef7b013b`.
- **The warden was sent to merge into "the existing sentence listing what a
  parameter may be." It does not exist** — § 13 enumerates fields, never
  parameters; that list lives in the diagnostic and in design.md §4.19.
- **The llm-ergonomist's brief quoted that same non-existent sentence as if it
  were the specification**, contaminating the one seat whose input is controlled.
- **"172 extern functions" is wrong twice**: five come from
  `archive/bootstrap-rs/`, which every brief forbids, and `sort -u` counts unique
  names rather than declarations. Properly counted: **316 declarations, 0
  array-spelled.** The conclusion is stronger and the denominator was never
  checked.
- **"You are pricing a difference, not a capability" is false.** `pipe(int [2])`
  does not cross — `error[bad_operand]: ptr takes a fixed run of bytes — i8[N] or
  u8[N], found i32[2]`. The framing sent five seats to price the wrong thing.

## The disagreements, stated plainly

**The historian approves and everyone who compiled something does not.** Its
precedent is real — Ada has shipped a constrained array subtype as a Convention-C
formal for decades — but the critic settled what that precedent is: Ada checks a
view conversion to the **Ada** formal's subtype and never sees the C header. It
is the author-against-author check. **So the strongest precedent for route 6
supports a strictly weaker claim than the one route 6 was queued on.**

**The ergonomist approves the read direction and vetoes the write.** Its blind
comparison flipped on direction, which no brief anticipated: for a read-only
array, the array spelling wins decisively, because the pointer form forces the
reader into the header to learn the number. For a buffer C fills, the pointer form
wins, because `.ptr()` is written at the call site and that is where the write is
marked. Defect 065 is that veto arriving as a measured fact rather than a
prediction.

**Panel 164's resolution 2 was ratified by delegation and never implemented.**
The critic ran it: `@field` reaching a `@`-marked `ptr` parameter still fails with
`type_mismatch: expected ptr, found i8[8]` — the identical error panel 164 quoted
when it wrote *"only the type rule is missing."*

## The resolution — `provisional — author ratification pending`

Per CLAUDE.md § 4, the most robust and complete resolution, never the cheapest and
never a compromise.

1. **Route 6 as panel 164 spelled it is REFUSED**, on the compiler-engineer's and
   the spec-warden's vetoes. Not recorded in design.md Part 6: Principle 0 says a
   form that has not earned its way in *waits*, and a permanent refusal row is the
   cheap move. What is recorded is the **sentence** that was false, so it is not
   proposed again as new.
2. **Defects 063, 064 and 065 are the milestone's work, and they outrank the
   route.** 063 is rank 3 — design.md §1.12, memory corruption — and CLAUDE.md
   § Precedence puts that above elegance, cost, ergonomics and speed. 064 is a
   prerequisite. 065 is a language question and owes its own sitting.
3. **Route 14 is ADOPTED as the repair of 063**: the extent argument of a lend is
   checked against the field's own extent wherever it is a compile-time constant,
   and `t.name.len()` is priced as the form that removes the literal from the call
   entirely. It closes the hole route 6 was the expensive way of not closing.

   **CORRECTION, 2026-09-19, the same day, by the coordinator who wrote it.**
   The first half of that sentence is **not implementable as written**, and it
   was written without being tried. **Nothing declares which argument is the
   extent**: `sum_n(p: ptr, n: i64)` is two independent parameters, and the
   compiler has no relation between them to check. Measured by attempting the
   repair in `selfhost/check/lend_types.hero`, where `f.ptr()` is typed and the
   extent never appears.

   What survives of route 14 is the ffi-pragmatist's half — `t.name.len()` as a
   compile-time constant, so the honest call needs no literal — and that is a
   **widening of a built-in**, `len` today answering *"`len` takes `str`, `[T]`
   or `{K: V}`, found `i8[8]`"*, which `spec § 11` states and CLAUDE.md § 4
   therefore sends to a sitting. **So defect 063 is not closed by this
   resolution.** What closes it is route 13 — declaring on the parameter which
   sibling carries the extent — and that is a new form at the boundary.

   This correction is the sitting's own rule applied to the sitting: a sentence
   that was not run is a guess, and four of the six brief errors above were the
   same shape.
4. **Route 12 is QUEUED WITH ITS MEASUREMENT ALREADY RUN**, which is the
   difference between this queue and panel 164's. A header `constant` as the
   extent is portable and header-checked, proven in C; what is unpriced is the
   grammar change (`Type = Prefix { "[" integer "]" }` admits an `integer` only)
   and what it costs the spec. **It is the form route 6 should have been.**
5. **Route 13 is recorded, unpriced**, so it is not rediscovered as new.

**What a veto compels.** The refusal of route 6 as spelled stands whatever the
author decides about routes 12, 13 and 14, because two seats with veto power
refuse it on soundness rather than on price.

**What conservative would have been, so the author can choose it:** leave route 6
queued and file only the three defects. That is cheaper by the whole of route 14
and it leaves 063 open — a memory corruption at exit 0 — which is why robust and
conservative disagree here, and why rank 3 decides it.

## Predictions to score

- **compiler-engineer**: if route 6 lands, `git diff --numstat` over `selfhost/`
  exceeds 300 insertions and `tests/harness/suite_layout.hero` changes. Falsified
  under 300 with that file byte-identical.
- **spec-warden**: if route 6 lands, `extern` parameters spelled as a fixed array
  in this repo are **0** at that milestone's close and **≤2** at the next.
- **ffi-pragmatist**: of `erand48, futimens, jrand48, lcong48, nrand48, pipe,
  seed48, utimensat`, **exactly zero** bind through route 6 without a shim; and a
  route-6 `if_indextoname` declaring `i8[8]` compiles clean on both platforms and
  writes 16 bytes into 8.
- **llm-ergonomist**: as proposed, ≥9 of 12 spec-only runs write
  `copy_name(dst: t.name)` with no `@` at either site and read C's bytes back;
  amended to `@dst: i8[8]`, silent-wrong-answer rate 0 of 12.
- **historian**: within two milestones of route 6 being adopted, zero `extern`
  array-parameter declarations state their extent as an `extern constant` rather
  than an integer literal. **Falsified by a single `function f(s: i8[L_tmpnam])`
  that compiles** — which route 12 is exactly the proposal to make possible.

Each becomes checkable at the milestone that lands route 12 or route 14.

## Also recorded, so it is not rediscovered

- **Panel 164's resolution 2 is unimplemented.** Verified by running it.
- **The fixed-field mechanism is platform-locked today**: extents must be
  literals, a disagreeing literal is refused at build, and real extents are
  platform-varying macros. **Panel 164's own emblem program cannot build on
  Linux.** That is route 12's warrant, arriving from the opposite direction.
- **`_LIBC_COUNT` expands to nothing** on this Mac: Apple clang does not define
  `__LIBC_STAGED_BOUNDS_SAFETY_ATTRIBUTES`, and `clang -E -P` over
  `#include <stdio.h>` yields `char * tmpnam(char *);`. The annotation route is a
  direction, not an available source of extents.
- **Every run in this sitting is `arm64-apple-darwin`**, except the Linux census
  (both legs, in containers) and the three Windows measurements, which were taken
  on the box on 2026-09-19.

## Author's verdict

*Pending.* Queued as `panel 165` in `docs/work/DECIDE.md`.
