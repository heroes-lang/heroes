# Panel 155 — spec-warden

**verdict**: object (not provisional on the ceiling; the spec delta's *real*
component is estimated and cannot flip the verdict, reasoning below)

**section**: design.md §1.6 (budget and the unconditional payment rule), §1.2
(cost = tokens x (1 + rewrite rate)), §1.12 via CLAUDE.md § Precedence rank 3
(robustness), §4.12:1721 ("No constraints. No `where`, no bounds"), Part 6's
traits row (design.md:2650), CLAUDE.md §12 (spec beats compiler; a refusal is
held to the same standard as a feature), Principle 0 (CLAUDE.md §2).

## The ceiling, re-reached by grep this sitting

`grep -n` on `docs/design/design.md`: §1.6 is line 253, the ceiling is **10240**
(line 255), raised from 8192 by author decision 2026-09-14 (line 278). Not taken
from the brief.

## The level, measured this sitting

`./heroes measure spec/heroes-spec.md`, run 2026-09-16:

```
claude-legacy 5863 · cl100k_base 5989 · maximum 5989 · spread 126
real 7974   claude-opus-5, 2026-09-15 — the binding number
```

The tool printed **no STALE line**, and for the spec the staleness test is a
**digest** (`selfhost/cli/measure.hero:106`, `here != pinned.SPEC_DIGEST`), not a
drift window. So 7974 is certified against today's exact bytes. Headroom **2266**,
**2206** net of §4.19's FFI floor.

## spec_token_delta, both directions, measured

Variants built in the scratchpad from a byte-identical copy (it measures
5863/5989, confirming fidelity) and counted with `./heroes measure`. Vendored
counts are exact; the *real* delta is estimated at 1.0x to 1.35x the vendored
delta (the document-wide ratio is 7974/5989 = 1.33).

| direction | edit | cl100k | legacy | vendored Δ | real Δ (est.) |
|---|---|---|---|---|---|
| base | — | 5989 | 5863 | 0 | 0 |
| **ADOPT** | no spec change at all | 5989 | 5863 | **+0** | **+0** |
| ADOPT | "A binding the body cannot use is an error at the call." | 6004 | 5877 | +15 | ~+20 |
| ADOPT | "A call binding a type parameter to a type the body cannot use is an error there." | 6009 | 5882 | +20 | ~+27 |
| **REFUSE** | "…or an abort if a generic hid it." | 5999 | 5873 | **+10** | ~+13 |
| REFUSE | "…though reached through a type parameter the program aborts instead." | 6003 | 5876 | +14 | ~+19 |
| REFUSE | one general sentence in §9 covering float, partial and handle | 6009 | 5882 | +20 | ~+27 |

**No breach in any direction.** Worst case 7974 + 27 = **8001** against 10240.
A 2239-token gap cannot be closed by a one-sentence edit under any tokeniser, so
the estimated half of the delta cannot flip the verdict and the vote is not
provisional. **I do not cast the budget veto.**

## The coordinator's inversion is real, and it reverses when both sides are held to one standard

The coordinator is arithmetically right that **refusing costs spec tokens and
adopting can cost none**, for the `partial` half. spec:353-354 says comparing a
`partial` record and using it as a map key are compile errors "for it and for any
value holding it", with no qualifier. Adopting makes that true at **+0**;
refusing makes it false and costs **+10 to +14** vendored to qualify.

But the zero is only a zero if the spec is allowed to stay silent on the other
side. spec:258-259 says generics have "**no constraints**", and design.md
§4.12:1721 says "No `where`, no bounds." The proposal computes obligations from a
generic's body and enforces them at call sites. That is inferred constraints
without the word. Held to CLAUDE.md §12's standard ("a refusal is held to the
same standard as a feature"), adopting honestly costs **+15 to +20** vendored,
which is **more than refusing**. The coordinator priced refusing honestly and
adopting silently. **Same standard, refusing is the cheaper direction.**

Either way the sums are trivial against 2206. §1.6 does not decide this sitting.
Say so plainly rather than dressing a 13-token payment as an argument.

## What the spec actually promises, and where it is silent

- **`partial`**: promised, unconditionally, at spec:353-354. Panel 084 already
  filed the gap ("`spec:221` promises a compile error … through a generic it is a
  run-time abort"). Under CLAUDE.md §12 **the compiler has the bug.**
- **float map keys**: the spec constrains `K` **nowhere**. Grep of the spec for
  `key` returns exactly two restrictions, both FFI (`partial` at 354, handles at
  357). `tests/golden/run/fixedbugs-a-map-key-that-is-not-itself.hero:15` says it
  in the repository's own voice: *"The spec constrains `K` nowhere."* The silence
  is not an oversight: `selfhost/check/map_keys.hero:20-23` states the carve-out
  as design, and **the diagnostic's own note ships it to the user** — "Reached
  through a type parameter this cannot be checked, and the runtime aborts
  instead" (run this sitting).

## The blast radius is twice what the shared brief states

The brief names one golden. Measured this sitting, it is **two**, and the second
is not an abort case:

| file | build | run | keyed at |
|---|---|---|---|
| `tests/golden/run/abort-map-key-nan.hero` | 0 | **134**, prints `2` first | f64 |
| `tests/golden/run/fixedbugs-a-map-key-that-is-not-itself.hero` | 0 | **0**, prints `3/2/1/true/true` | f64, in **four** generics |

The second is a green, exit-0 regression case for a fixed defect, and its own
comment records that it was **already rewritten once** (panel 069 R4, 2026-08-16)
to route through a generic because the direct form had just become illegal. The
proposal closes the route that rewrite moved to. That is a second rewrite of one
file for one reason, which is §1.2's subject matter, and this time there is no
third route to move to: the generic **was** the route.

## The robustness objection, which outranks the token one

`selfhost/check/map_keys.hero:111-118`: the float walk's `depth > 16` give-up is
sound **only because the runtime guard stands behind it** — "a missed float
refusal costs a named abort because the runtime guard is still behind it. There
is no such guard behind the handle rule … which is what made the same line
correct here and a hole there." Defect 035 is that hole, in the twin walk.

Close every static route and delete the two goldens, and `hero_map_slot_of`'s
`eq(key, key)` guard has **no Heroes-source test left**, while the `depth > 16`
bound still depends on it. That converts a justified bound into defect 035's
shape. CLAUDE.md § Precedence rank 3: robustness beats token cost, ergonomics and
consistency. Both rules were measured at panel 084 as **consistency, not safety**;
consistency does not buy a regression in the guard that safety rests on.

## R4 is where this sitting turns, and the answer is yes

Three measured asymmetries, not one judgement call:

1. **Spec ground.** `partial` is promised unconditionally; `K` is constrained
   nowhere.
2. **Deleted programs.** The set of corpus files declaring a generic and the set
   declaring a `partial` record are **disjoint** (`comm -12`, run this sitting:
   empty). The `partial` half deletes **zero** working programs. The float half
   deletes **two** green goldens.
3. **Guard dependency.** Float has one and its bound leans on it. `partial` has
   panel 061's deliberate `hero_panic` and nothing leans on it.

## Principle 0

`grep -rnE '^function [a-z_]+<' selfhost/` is **0**, verified. Neither half is
compiler-need.

- The **`partial` half does not need Principle 0**: it is a **bug fix** against a
  ratified spec sentence, not an addition, so §1.0's burden and the payment rule
  do not attach. Panel 084 R1 used exactly this route ("B lands as a bug fix, not
  a feature"). Nothing is added, so nothing must be removed.
- The **float half fails Principle 0**: no compiler-need, no spec sentence, no
  measured Part 11 effect, and the brief supplies no measured argument. The
  argument it would need is a corpus rate — programs that reach the runtime abort
  where a call-site refusal would have caught them — and none is offered. **It
  waits.**

## One documentation duty the proposal does not name

design.md:2650, Part 6's traits refusal (panel 137, 2026-09-13, thirteen days
old), rests on eight measured cases, among them "**a `{A: i64}` map key works**"
and "`==` on a type parameter over a user record works". Adopting narrows the
first. CLAUDE.md §12 requires a Part 6 row to name the fact that would make it
wrong; this proposal moves that fact and the row would go stale in silence. Not a
§1.6 cost (design.md is unbudgeted), but it is owed.

## Verdicts per resolution

- **R1 — object to it as one rule.** Split it. A generic call whose bindings make
  the body illegal is a compile error **for `ffi_partial_operation`** (spec:353
  already says so; CLAUDE.md §12 makes it a bug fix). It is **not** one for
  `float_map_key`, where the generic body is a place the rule deliberately does
  not reach, said in three places in the tree: the golden's comment,
  `map_keys.hero:20-23`, and the shipped diagnostic note.
- **R2 — moot under the split, and the brief undercounts it.** Neither golden is
  rewritten and neither is deleted. `abort-map-key-nan.hero` stays red-by-design
  at 134 and `fixedbugs-a-map-key-that-is-not-itself.hero` stays green at 0. If
  the panel adopts the float half anyway, it owes a replacement test for
  `eq(key, key)` **before** the goldens move, and a repair of `reaches_float`'s
  `depth > 16` bound, which loses its stated justification the moment the guard
  goes untested.
- **R3 — the compiler is wrong, and the spec is not qualified.** CLAUDE.md §12.
  Cost **+0 spec tokens**, measured. Qualifying instead costs +10 to +14 vendored
  to turn a true sentence into a hedged one, and also invalidates
  `pinned.SPEC_DIGEST`, forcing a `--refresh` network round trip for a new real
  count. The zero-token direction is also the only one that leaves the digest
  valid.
- **R4 — yes, decisively.** Three measured asymmetries above. Treating the two
  rules as one is the error this sitting exists to catch.

## removal

**Nothing, and that is correct here, because nothing is added.** The `partial`
half is a bug fix at a measured +0 tokens, so §1.6's unconditional payment rule
has nothing to attach to. The float half's payment is not a sentence: **the
feature comes out.** If the panel insists on adopting the float half, then the
named removal I will accept is `float_map_key`'s note sentence "Reached through a
type parameter this cannot be checked, and the runtime aborts instead", which
becomes false the day it lands, plus a written replacement for the guard test.

## needed_for_self_hosting

**no.** Zero generics in `selfhost/`, verified this sitting.

## argument (<=120 words)

Priced both ways. Adopting the `partial` half costs **+0** measured spec tokens
and makes spec:353 true; refusing costs +10 to +14 vendored to hedge it. The
coordinator's inversion holds there, and I approve that half as a bug fix under
CLAUDE.md §12. It deletes zero working programs: generic files and `partial`
files are disjoint, measured. The float half is different on all three counts.
The spec constrains `K` nowhere, it deletes **two** green goldens rather than
one, and `map_keys.hero:111-118` says the `depth > 16` bound is sound only
because the runtime guard stands behind it. Adopting leaves that guard untested
and the bound unjustified: defect 035's shape, in the twin walk. Robustness
outranks consistency.

## prediction

Falsifiable, instrument named, both exist today.

1. **If the float half lands**, `./heroes run tests/harness/main.hero -- ./heroes`
   goes red on **exactly two** source files, `tests/golden/run/abort-map-key-nan.hero`
   and `tests/golden/run/fixedbugs-a-map-key-that-is-not-itself.hero`, across the
   `run`, `check`, `canonical`, `determinism` and `lines` suites. A **third** file
   moving falsifies my enumeration and I owe a correction; **zero** moving
   falsifies the proposal, which would then not do what it claims.
2. **If the `partial` half lands alone**, `./heroes measure spec/heroes-spec.md`
   reads **5863 / 5989 unchanged** and `pinned.SPEC_DIGEST` still matches, so the
   `spec` suite stays green with no `--refresh`. Any non-zero vendored delta
   falsifies my +0.
3. **If the panel refuses both**, the spec must be qualified and the delta is
   **+10 to +14 cl100k**, scored by the same command.

## condition

I change my vote on the float half when both of these exist, measured:

- a test that keeps `hero_map_slot_of`'s `eq(key, key)` guard exercised without a
  Heroes source route to a float key (a `tests/harness/` runtime case), **and**
- either a spec sentence stating that floats cannot be keys, which this document
  has never had and which costs tokens I have priced at +10 to +20 vendored, or
  `reaches_float`'s `depth > 16` bound replaced by a total walk, which is defect
  035's repair applied to the float twin.

I change my vote on the `partial` half if someone shows a corpus program that
compares a `partial` record through a generic and **works**. The disjointness
measurement says there is none; produce one and the half becomes a deletion like
the other.

I would **veto** the float half if it were put as part of one undifferentiated
R1, because it trades a tested runtime guard for consistency and CLAUDE.md
§ Precedence puts robustness above consistency. Put as a split, it is a price and
not a refusal, so it is an **object**.
