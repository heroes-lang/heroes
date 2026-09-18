# Panel 164 — the read had a door, and the write had none

Convened 2026-09-18, between milestones: M-readable-bytes closed the day before
and M-core-packages has not opened. Five seats, a completeness critic, and the
working tree frozen from the briefs going out until this file was written.

The question: **a header record's `char[N]` field can be READ as text since panel
162. It cannot be PASSED to any C function. How should it cross?** That is defect
061, filed by panel 162's ffi-pragmatist beside a question about reading.

## The proposal, verbatim as it went to the seats

1. **Automatic decay.** A fixed byte field is accepted wherever a `cstr` or `ptr`
   parameter is declared — C's own rule, applied at the checker.
2. **An explicit lend to `cstr`**: `f.cstr()` widened from `str` to a fixed byte
   field, under the same position rule the `str` lend already obeys.
3. **An explicit lend to `ptr` only**: refusing `cstr`, because a `cstr` is a
   promise of a terminator that a `char[N]` field does not make.
4. **Refuse.** A field is read with `validated_bytes` and never passed; a C
   function that wants the field's bytes gets them through a `static inline` the
   header declares.

## The resolution, in one line

**Routes 1 and 2 are refused on veto. The language gains ONE type rule — a fixed
byte field's ADDRESS reaches a `ptr` parameter through an explicit lend, in both
directions — and the `cstr` direction stays refused, because it is already served
by a copy.**

## The premise this sitting was convened on is false, and three seats measured it

Defect 061's body says, in bold in the record: **"No Heroes-side shim can route
around it."** The shared brief quoted it as the sitting's premise. The
spec-warden, the ffi-pragmatist and the critic each ran it independently, and
each got the same answer:

```
slot_len(t.name.validated_bytes().must().cstr())
  terminated i8[8] ("Hi\0…")                     ->  2    exit 0
  UNTERMINATED i8[8] ("fullest!"), 8 'A's after   ->  8    exit 0
```

Eight, not seventeen. The same struct in C, `slot_len(full.name)`, under
`clang -std=c11 -Weverything -fsanitize=address,undefined`, prints **17** and
reads into the next field with every diagnostic silent (ffi-pragmatist,
`decay.c`). So the overread routes 1 and 2 were convened to legislate against was
**already closed** the day before, by `validated_bytes` stopping at the field's
end and `str.cstr()` supplying the zero.

**The precise true sentence, from the emitted C** (critic):

```c
HeroStr hero_vb_text = hero_str_try_from_bytes((const char *)(t21.name), INT64_C(8), &hero_vb_status);
t33 = slot_len(hero_cstr_nonnull(t32));
```

C receives the runtime `str`'s pointer, **never the field's address**. So *no
Heroes-side shim lends the field's address* is true; *no Heroes-side shim routes
around it* is false. The whole sitting rested on the second wording, and the
correction is written under defect 061.

**And the emblem was drawn from the safe class.** The historian fetched POSIX:
*"The character arrays are of unspecified size, but the data stored in them shall
be terminated by a null byte"* (`sys/utsname.h`), repeated in Linux's `uname(2)`.
`strlen(u.sysname)` — defect 061's own headline program — is the case where
route 1 would have been sound, and it works today. The sitting should have argued
from the 37, not the 13.

## What the sitting found instead, and it is bigger than what it was convened on

**The write direction has no door at all**, and it was on defect 061's own unrun
list the whole time. The ffi-pragmatist's classification of 16 real headers says
**50 of 141** pointer parameters are non-`const` — C writes into them — and marks
them *"no route in this sitting reaches these"*. Nobody followed it up until the
critic did:

- `function getcwd(@buf: ptr, size: u64) -> cstr` **binds**; `heroes check` passes.
- `getcwd(buf: @b.name, size: 8)` is `type_mismatch: expected ptr, found i8[8]` —
  **one** error, so the grammar and the `@`-marker machinery already accept a
  field there. Only the type rule is missing.
- The working read route cannot serve it, twice over: it hands C a `cstr` over a
  **copy**, and a `cstr` cannot reach a `ptr` parameter (spec-warden, `p7`).

And the spec already reaches for the rule, at line 121: *"`@` declares a mutable
cell and re-binds it, **or a field or element inside one**."*

**The read direction's remaining hole is the binary one.** `arpa/inet.h` declares
`inet_nsap_ntoa(int __binlen, const unsigned char *_LIBC_COUNT(__binlen), char *)`
— the header itself says the extent is the sibling, never a terminator. Today the
only door the language has gives a **silently wrong answer at exit 0**: the
program compiles, runs, and prints `0x47` where the C prints
`0x47.0005.80FF.0000.01`, because `validated_bytes` stopped at the first zero
(ffi-pragmatist, `p8b`). Not a refusal — a wrong answer.

## The verdict table

| seat | R1 decay | R2 lend to `cstr` | R3 lend to `ptr` | R4 refuse | rests on |
|---|---|---|---|---|---|
| compiler-engineer | **veto** | **veto** | object (cost) | **approve** | design.md §4.3, Part 6 `Subtyping`, §1.12 |
| ffi-pragmatist | **veto** | **veto** | **approve** | object | design.md §1.11:547, §4.20, §4.10:1567 |
| spec-warden | **veto** | object | object (Principle 0) | **approve** | design.md §1.6, §1.2, Part 6 |
| llm-ergonomist | **veto** (non-local) | object | approve, only beside R4 | **approve** + a clause | spec § 13, § 3, § 9 |
| historian (advisory) | object | only with a declared terminator | **approve**, the spine | object as written | Cyclone, Zig, Rust, Go, D, Ada, Swift, Nim |

**Costs and deltas, each measured by the seat that carries that instrument.**

| | spec, real (spec-warden) | compiler (compiler-engineer) |
|---|---|---|
| baseline | **8040**, digest `15919ac16c8ddbb8` | — |
| R1 | 8097 (**+57**) | ~15-25 lines, **three in files with zero headroom** |
| R2 merged | 8070 (+30) | ~35-50 lines, two in zero-headroom files |
| R3 merged, with the removal | 8067 (**+27**) | ~90-150 lines, **none in a zero-headroom file** |
| R4, tightest wording | 8058 (**+18**) | zero compiler lines |
| the § 3 removal alone | **8034** (−6), unspent since panel 162 | — |

Ceiling 10240. **No budget veto**; the worst route leaves 2137 free.

## What every seat agrees on

**A `cstr` reaching C without a NUL is out.** Four seats refuse route 1 and two
refuse route 2, and the ffi-pragmatist puts it as a refusal rather than a price:
design.md §1.11 lists NUL-termination among the decisions *"that should not be
revisited"*, and it is load-bearing three times — `hero_str_cstr` is zero-copy
**because of** the NUL, `c.validated()` reads **to** the NUL, and
`guard_arguments` checks only null **because** the NUL is assumed. One `cstr`
without one falsifies all three at once.

The historian's survey is the same finding from outside: of nine languages that
bind C, **eight refuse the decay** — Rust, Zig, Go, D, Odin, Swift, Ada, Cyclone
— each making the crossing visible at the site or in the type. The ninth is Nim,
and its own manual says the implicit conversion *"will be removed in future
releases"*. CLAUDE.md §6 already rules on that one: copy the surface, never the
implementation.

## The disagreements, stated plainly

**R3 versus R4 is the whole sitting, and it splits on what counts as served.**
The compiler-engineer and the spec-warden approve R4 because the `cstr` class
works today and Principle 0 is unmet for anything more: zero corpus programs, no
Part 11 measurement. The ffi-pragmatist and the historian approve R3 because the
binary class is **answered wrongly**, not refused. The ergonomist approves R4
with a clause and R3 only beside it.

**The compiler-engineer is wrong on one load-bearing fact**, and it is the one
the critic settled: it reaffirmed *"no Heroes-side shim can route around it"*
from the brief, and three independent runs refute it. Its second finding stands
untouched — a C-side `static inline` closes the owned-header case, which §4.19
already blesses.

**The two corpus numbers are not a sample of each other** (critic). **80 / 58 /
5** is this repository — the right denominator for a Principle 0 argument. **141
/ 91 / 8** is 16 real system headers — the right denominator for a soundness
argument, and the ffi seat itself calls 8/91 a floor. Both are carried, each
labelled. The compiler seat's *"22 of 80 call sites"* is `31 − 9` and they are
declarations, not call sites.

## Three routes nobody listed, and the critic found them

**Route 5 — the terminator declared on the field**, `name: i8[16] terminated`.
The compiler-engineer and the historian converged on it independently; Cyclone's
`@zeroterm` and Zig's `[N:0]u8` are the precedent. **Refused, and priced.** It
adds a **fourth** notion of the field's length to a language that already has
three that agree — construction demands exactly N, the `_Static_assert` marker
pins `(*)[N]`, the index bounds-check aborts at N, `validated_bytes` passes N to
the runtime. Cyclone's own rule is that the size **includes** the terminator, so
`terminated` makes the usable run N−1 while all four sites say N. Nothing
reconciles them and nothing can check the reconciliation. Plus the whole
new-surface-form walk. The critic also corrected the compiler seat's framing:
unverifiable clauses are **not** novel here — `consumes`, `acquires`, `borrows`
and `owned <freer>` are already author claims clang cannot test.

**Route 6 — declare the extent on the PARAMETER**, `function arr_len(s: i8[8])`,
which is how C spells it. Refused today at `error[ffi_type]`. It needs **no new
expression, no lend, no built-in and no position rule** — one widening of
§4.19's parameter list, with ordinary type identity doing the matching. **It is
the only route of the seven where the compiler CHECKS the extent** instead of
trusting the author or the callee. Its whole value is a number nobody has
measured: how many real headers spell the parameter as an array. **Queued, not
adopted**, because adopting an unmeasured route is the cheap move.

**Route 7 — the write direction**, above. **Adopted, folded into route 3**,
because it needs the same type rule.

## The resolution — `provisional — author ratification pending`

Per CLAUDE.md § 4, the most robust and complete resolution, not the cheapest and
not a compromise.

**1. Routes 1 and 2 are REFUSED.** This is not the synthesis's choice: two seats
with veto power refuse route 2 and four refuse route 1, on soundness. Neither is
recorded in design.md Part 6, because Principle 0 says a form that has not earned
its way in *waits* — a permanent row is the cheap move.

**2. Route 3 is ADOPTED and widened to the write direction.** One type rule, one
explicit lend: a fixed byte field's address reaches a `ptr` parameter, and
`@field` reaches a `@`-marked `ptr` parameter so C can fill a field. The lend is
registered in `is_lend`, so `check/lending.hero`'s existing position rule binds it
for free — **only as an argument of a call, never into a local** — which is the
ffi-pragmatist's condition 2 and closes the use-after-free it names.

**3. Route 4 is ADOPTED as the `cstr` answer.** Nothing lends a field to `cstr`.
The text route stays `validated_bytes`, and § 13 gains the clause the ergonomist
asked for: what a field **cannot** do and why. The `type_mismatch` a reader
actually gets today carries no `note`, no `fix`, and does not contain the word
`validated_bytes` — measured by the critic, `grep -c` returns 0 — so the
diagnostic gains one. That is design.md §4.17, not a nicety.

**4. Route 5 is REFUSED**, priced above. **Route 6 is QUEUED** behind its
measurement.

**What conservative would have been, so the author can choose it:** route 4
alone — the compiler-engineer's and the spec-warden's verdict. Zero compiler
lines, **+12 real net** after the −6 removal, and the `cstr` class already works.
It leaves `inet_nsap_ntoa` answered wrongly at exit 0 and `getcwd` unwritable,
and that is why robust and conservative disagree here.

**What a veto compels:** the refusal of routes 1 and 2 stands whatever the author
decides between 3 and 4. The ffi-pragmatist's words: *"there is no ergonomic gain
I will trade for it."*

## What the resolution does NOT settle, each with its trigger

- **Route 4 is a copy at every crossing** — one runtime allocation per call,
  `hero_str_try_from_bytes` building a `HeroStr` (critic, Q3). Rank 3 puts
  robustness above speed so no verdict moves, but the resolution says it rather
  than leaving a reader to find it in the emitted C.
- **A fixed-array field IS indexable and `len` is not** (critic). `t.name[0]`
  gives `72`; a hand-written scan compiles and gives the right answer on a
  terminated field and **aborts** on an unterminated one, `panic: index out of
  range`, exit 134. That is the status quo all four routes were compared
  against, and no brief contained it. The ergonomist predicted it and could not
  run it.
- **`validated_bytes` accepts both signs** — `i8[N]` and `u8[N]`, measured — and
  the spec does not say so. It is also absent from § 11's `Built-ins:` sentence,
  appearing once in the whole document.
- **The construction wall is not a defect and is already recorded.** A real
  five-field `utsname` needs **1280 literal zeros**, 4312 bytes for a nine-line
  program. The critic **built it and ran it**: `Darwin arm64`, exit 0. It blocks
  nothing, `docs/work/milestones/M-readable-bytes.md:39` already carries it, and
  it is scheduled rather than filed, because `docs/work/DEFECTS.md` holds what is
  **broken**.
- **Defect 061's body carries a false sentence about `slice`** — *"That sitting
  widens `slice`, `validated` and `repeat` so a field can be READ"*. Measured:
  `slice` takes `str` or `[T]`, not `i8[8]`. Corrected underneath, per § 14.
- **The `mangle.value` trap is a question, not a claim.** For a **field**
  receiver it does not bite — `storageless.fixed_text`'s `.field` arm renders
  `base.field`, and the emitted C shows `t21.name` with no temporary. For a fixed
  value that is not a field, nobody ran it.
- **Every run in this sitting is `arm64-apple-darwin`.**

## Predictions to score

- **spec-warden**: a golden program passing a fixed byte field's text to a C
  `cstr` function needs **zero** new compiler lines and **zero** new spec forms.
- **ffi-pragmatist**: under route 3, `sqlite3_bind_blob` round-trips a `u8[16]`
  UUID with no C shim. Wrong if it round-trips today without one.
- **compiler-engineer**: route 3 lands green on `layout` with no `DECIDED` row
  raised, at 90-150 selfhost lines by `git show --stat`.
- **llm-ergonomist**: with route 4's clause, compile-on-first-try above 2 in 3,
  silent-overrun rate 0.
- **historian**: no language in the nine surveyed accepts a bare fixed byte array
  where a NUL-terminated parameter is declared, without a site-level act or a
  type-level termination claim.

## Author's verdict

**Ratified by delegation, 2026-09-18.** The author delegated this sitting's
ratification in the session that convened it, in those words: *ratify the
panels*. This is a delegated ratification and not a reading — the author has not
read this file, and crediting them with a reading that did not happen is the
falsehood CL-058 names wearing the flattering sign.

The resolution stands as written: routes 1 and 2 refused on veto, route 3 adopted
and widened to the write direction, route 4 adopted as the `cstr` answer, route 5
refused, route 6 queued behind its measurement. Conservative — route 4 alone — is
recorded above and the author may take it at any time.
