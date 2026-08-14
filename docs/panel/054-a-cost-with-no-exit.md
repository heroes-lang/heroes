# 054 — A cost the spec names and leaves no way to avoid

**Status**: `ratified — 2026-08-14, author decision`.
**Convened** 2026-08-14, by author decision in `/decide` (item 7, answer `a`: *add
`repeat`*; the panel decides its shape, not whether it arrives).
**Lane**: full — four judges reporting, one stopped.

**The irregularity, recorded first because it shaped the sitting.** The
compiler-engineer was asked to *"prototype it far enough that the number is real"*
and prototyped **in the repository** rather than in a copy. Its five files were
swept into an unrelated commit and pushed, landing a language feature before its
own panel had ruled (CLAUDE.md §4). The repository's own invariant caught it
within the minute — `every_reserved_builtin_is_named_in_the_spec`, red on
`["repeat"]` — and a first revert was undone by the judge writing again between
the `checkout` and the `add`. The judge was then stopped, and the reason is the
spec-warden's: it was **measuring a tree it was itself mutating**, so the sitting
was *"being asked to price paperwork for a fait accompli"*. Its cost estimate is
therefore absent, and the two judges who priced the feature from outside it —
the warden at **+8** of spec and the ffi-pragmatist at **13 lines** of runtime —
are what the resolution rests on. `/panel` step 3 now tells every judge that
builds to copy the tree first.

## The proposal, verbatim

`docs/measurements/007`: a blind reader avoided the quadratic `+`, naming the
clause that stopped it, and then wrote **`push`-in-a-loop in two of three
programs**. Options: **A** `repeat(s, n)` · **B** the lambda · **C** delete the
`push` half of the cost clause · **D** `repeat` plus a `join` that takes a
separator.

## The verdict table

| judge | verdict | rests on | cost / delta | prediction | condition |
|---|---|---|---|---|---|
| llm-ergonomist | adopt-with-condition **A** · **veto** B · object C | § Strings' cost sentence read against the built-ins list | A = *"one name in an existing list"* | current spec yields `+`-in-a-loop or a hand-counted literal in ≥2/3 of first tries; under A, ~0 | state the signature and what `n == 0` and `n < 0` do; do **not** touch the cost sentence |
| spec-warden | object A (approve under condition) · **refuse** B · approve **C as its own row** · D collapses into A | §1.6, §1.2, Principle 0, CLAUDE.md §13 | **A = +8**, A-honest (with an abort clause) = +17, **C = −8**, and a *new* removal **R-a = −5** | with A + C, 0 of 3 programs accumulate in a loop | negative `n` returns `""` not an abort; **R-a funds it, C does not**; veto if C's −8 appears as A's funding in one ledger row |
| ffi-pragmatist | approve **A** conditionally · object B, C, D | §4.20 and CLAUDE.md §12 | **13 lines** of runtime, no new allocation site, no new UTF-8 hazard | `repeat` earns its place from the **banner**, never from the caret: the port's `render.rs` padding path will contain zero `repeat` calls | the overflow guard is **normative text**; a golden case per abort; ABI 12 → 13 |
| historian | approve **A**, *"with Nim's signature, not Go's"* | precedent | — | a signed count with a runtime abort makes **the compiler's own diagnostic renderer the first caller to abort**, from a `col`/`width` subtraction | — |
| compiler-engineer | *stopped* | — | — | — | — |

## The disagreements, stated plainly

**Two judges independently falsified the proposal's Principle 0 claim.** The
proposal said the port needs `repeat` for its caret line. It does not.
`crates/heroes/src/diagnostics/render.rs:110` builds the padding as a
**tab-preserving per-character map**, not `repeat(" ", col)` — writing it as
`repeat` would re-create sweep 001 audit S5, and the invariant that guards it
(`the_caret_is_measured_in_columns_and_so_is_the_column`) would go red. The
carets themselves are `slice` on a literal band, measured identical output at
**0.032 µs**. The warden reached the same conclusion counting call sites: six
`str::repeat` in the Rust compiler, every one a gutter or an indent of tens of
characters. **`repeat` is not compiler-need**, and the resolution says so.

**What it *is* for is the banner**, and that is the one program measurement 007
showed has no route. The pragmatist checked the other two: `join(xs, sep)` already
takes a runtime separator (`runtime/parts/text.c:100`), so option **D is a
non-proposal**, and the reader's actual want was `map`'s **capture** — which is B,
which CLAUDE.md §13 forbids before the closure list compiles itself.

**The cost claim is true, and by a factor nobody had measured.** 1 MB built by
concatenation: **8.180 s, one million allocations, 500 GB copied**. The same
string by `repeat`: **0.000265 s, one allocation**. 30 800×. And `push`-then-`join`
is *worse* than concatenation at every size — 10.6 s and 80 GB at n = 10⁵. So
option **C is refused**: deleting a true statement misinforms by omission.

**But the clause has a defect the sitting agrees on and panel 043's rule has no
arm for.** The warden put it best: the sentence names `join` as the linear escape
and `push` — the only way to build the array `join` needs — as the trap. **The
escape hatch and the trap are the same clause.** It is grammatically the permitted
kind (it binds the implementation, and `runtime/parts/array.c` can falsify it),
and it steers by juxtaposition toward a route the reader cannot take. The rule it
asks for, and which this sitting adopts: **a cost claim is admissible only where
the same paragraph names a construct the reader can reach for.**

**The judges disagreed about a negative count, and the disagreement is the
sitting's sharpest.** The warden wants `""`, for consistency with
`range(from: 0, to: -1)`, which returns an empty array — verified. The pragmatist
wants an abort. The historian brought the evidence that settles it: Rust's
`str::repeat` shipped **CVE-2018-1000810**, an unchecked `len * count` overflow
giving an out-of-bounds write, live from 1.16.0 to 1.29.0; Go's `strings.Repeat`
panics on a negative count and **oh-my-posh #4135 is a shipped crash from exactly
this panel's motivating example** — `strings.Repeat` with `0xffffffe0`, from a
renderer's padding subtraction, which is what `repeat(" ", col)` is. Nim answers
it in the **type**: `repeat(s: string; n: Natural)`.

## What the overflow actually does here, measured

| case | guarded | unguarded |
|---|---|---|
| `n = 0`, or `s = ""` | `""`, zero allocations | same |
| `2 × (2⁶³/2+1)` wraps **negative** | `string length overflow` | caught by the existing `len < 0` |
| `4 × (2⁶²+2)` wraps **positive to 8** | `string length overflow` | **exit 138, SIGBUS, silent** — ASan: `heap-buffer-overflow, WRITE of size 4, 0 bytes after a 25-byte region` |
| `1 × 2⁵⁰` | `out of memory`, exit 134 | same |

Eight bytes allocated and 2⁶⁴ copied, with no message and no output. `repeat`
unguarded would be **the only primitive in this runtime whose length can wrap
positive** — `hero_str_concat` guards its add, `hero_array_new` guards its
product. This is the class design.md §1.12 was written for, three hours earlier
and independently.

## Resolution — provisional, author ratification pending

**A lands, with a `u64` count.** Neither judge's answer to the negative-count
question is adopted, and the third option is the historian's precedent written in
this language's own types: **`repeat(s: str, n: u64) -> str`**. A negative count
cannot be written, so the class does not arise; `repeat("-", 40)` still reads as
one line, because a literal takes the type its context asks for; and a *computed*
count is `repeat(" ", col.to_u64().must())`, which aborts with
`does_not_fit: the value is outside the target's range` **at the subtraction that
went negative** rather than inside `repeat`. That is design.md §1.12's own test:
a defensive check must surface a defect, not hide it — and returning `""` for a
negative count is the hiding kind, which is why the warden's consistency argument
loses to it despite `range` behaving that way.

**The multiply is guarded and the guard is normative.** §4.20 gains one sentence:
a runtime primitive that computes a length guards it before allocating. Three
aborts, one golden case each.

**B is refused** (CLAUDE.md §13, and no judge supported it). **C is refused as a
deletion and adopted as a repair**: the clause keeps both true halves and gains
the construct a reader can reach for, which is the rule above. **D is a
non-proposal.**

**Funding**: the warden's **R-a, −5** — `spec:144`'s *"integer overflow aborts"*
duplicates `spec:62`'s *"Overflow aborts at every width"*. C's −8 is **not**
spent here; putting it in the same ledger row would make the row read *"a built-in
for +4"*, which is the laundering panel 041 refused and panel 048's row `3191`
deliberately avoided.

**Two findings kept out of the resolution and put in `DECIDE.md`**, both the
warden's: `spec:63`'s *"Character literals are `i64`"* is redundant with the
context rule **and false in the compiler's favour** — `s[0] == 'a'` type-checks
today against a `u8` — which is a §12 discrepancy of panel 048's class; and
`examples/gallery/04-loops.hero:46` contains `out @ out + "-"`, the quadratic the
clause names, in the repository's own gallery.

## Predictions to score

| # | judge | prediction | scored at |
|---|---|---|---|
| 1 | ffi-pragmatist | the Heroes port of `diagnostics/render.rs` contains **zero** `repeat` calls on the padding path, and removing `repeat` does not stop it compiling | M-selfhost-probe |
| 2 | ffi-pragmatist | with the guard, `heroes mutate` under `--sanitize` reports no heap-buffer-overflow from `hero_str_repeat`; without it, one program reading `n` from `args()` reproduces exit 138 | next mutate run |
| 3 | spec-warden | with `repeat` and the repaired clause, **0 of 3** of measurement 007's programs accumulate in a loop | next spec-only writing experiment |
| 4 | llm-ergonomist | on tasks needing k copies of a string, the current spec gives a quadratic loop or a hand-counted literal in ≥2/3 of first tries, ≥1-in-5 of the literals off by one and silent; under `repeat`, ~0 | next harness run |
| 5 | historian | a signed count with a runtime abort would make the compiler's own renderer the first caller to abort — **untestable under the adopted `u64` shape, which is the point** | — |

## Ratification — 2026-08-14, by author decision

**RATIFIED as it stands.** `repeat(s: str, n: u64)` stays landed with its guarded
multiply, the cost clause stays repaired rather than deleted, B stays refused, and
the record that `repeat` is **not** compiler-need stays in the ledger.

What the yes settles, and it is a rule rather than a feature: **a cost claim is
admissible only where the same paragraph names a construct the reader can reach
for.** The clause was true, was read, was named by the reader as the thing that
stopped it — and then that reader wrote the other quadratic in two programs of
three, because `join` needs an array and `push` is the only way to build one. The
escape hatch and the trap were the same sentence. Panel 043's rule had no arm for
that shape; it has one now.

It settles the `u64` too, and by the same test design.md §1.12 states: **a
defensive check must surface a defect, not hide it.** Returning `""` for a
negative count hides the subtraction that went negative; aborting names `repeat`
rather than the subtraction; the type puts the diagnostic on the value that is
wrong. That §1.12 decided a question three hours after being written is the
strongest evidence the principle earns its place.

The irregularity stays in the file: a judge prototyped in the repository, its
half-built feature was pushed, and the repository's own invariant caught it twice.
`/panel` now tells every judge that builds to build in a copy.
