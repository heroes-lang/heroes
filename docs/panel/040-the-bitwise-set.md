# Panel 040 — the bitwise set becomes real

**Status** `retro-record — the decision was the author's, taken in /decide on
2026-08-12`.

**Lane** none. No judges sat. This file exists because CLAUDE.md §4 admits no
design change to `spec/**` without `docs/panel/NNN` and a DESIGN-LOG line, and
because `/panel`'s own procedure provides for exactly this case: *"If the decision
is already taken (author's call), mark the session `retro-record` — record real
objections, do not stage dissent."* Nothing below is invented disagreement. Every
objection is one the record already held, and the ones the implementation
discovered are marked as such.

---

## The change

`&` `|` `^` `~` `<<` `>>` become real operators on `int`, from reserved-and-
unimplemented. Spec: one row in the operator fence, five levels in the precedence
chain, **+48 → 2675**.

    boolean      && || !            (bool only; && and || short-circuit)
    +bitwise      & | ^ ~ << >>      (int only; shift count 0..63 or it aborts)

    -Precedence, strongest first: call and `.` → unary `-` `!` → `* / %` → `+ -`
    -→ comparisons → `&&` → `||`.
    +Precedence, strongest first: call and `.` → unary `-` `!` `~` → `* / %` → `+ -`
    +→ `<<` `>>` → `&` → `^` → `|` → comparisons → `&&` → `||`.

---

## Why it was asked for, and the evidence is panel 039's

The warrant is a **silent wrong answer**, which is the one class this language
spends tokens to prevent. With no `|`, a flag union had to be spelled
`FLAG_A + FLAG_B`, and that is correct exactly while the bits are disjoint. Panel
039 found it twice in one sitting, from opposite directions:

- the **llm-ergonomist**, writing from the spec alone, produced
  `FLAG_READ + FLAG_WRITE + FLAG_EXEC` and flagged its own premise unprompted:
  *"sum == or, only because bits are disjoint — the spec gives me no help noticing
  that"*;
- the **ffi-pragmatist** compiled the overlapping case: `O_RDWR|O_ACCMODE` is **3**
  where `+` gives **5**, and `S_IRWXU|S_IRUSR` is **448** where `+` gives **704**.

design.md §4.14 had already argued the substance, and only the timing was
deferred: *"bitwise operators are exactly what is needed both for low-level work
and for the web: binary format parsing, network protocols, masks, flags, UTF-8
encoding, hashing."*

---

## The objections, all of them real

**1. §4.14's own sentence said "not implemented in v1", and this overrides it.**
That is the primary objection and it has no answer except the author's decision.
It is recorded here and in §4.14's amendment rather than quietly overwritten: the
deferral was not wrong when it was made, and what changed is that the FFI arrived
and made the workaround silently wrong.

**2. Principle 0 is not satisfied by compiler-need.** CLAUDE.md §2 admits a form
if the compiler needs it or it provably serves the thesis. The closure list
(§1.0) has **no bitwise row** — and the one place that looked like it needed one,
the mangler's FNV hash, was closed the same afternoon by changing the hash
instead. So this entered on the thesis limb, on a measured silent-error class,
which is the weaker of the two and should be read as such.

**3. The spec budget, at +48 with no removal available.** Panel 012's rule above
the soft 2000 asks for a named removal or a registered prediction. There is no
removal to name: panel 036 already deleted the spec's own sentence listing the six
reserved spellings, so the debt was spent. This is therefore the registered-
prediction branch, and the prediction is below.

**4. It does not close the gap it was argued from.** `CURLAUTH_ANY` =
`(~((unsigned long)1<<4))` and `SIZE_MAX` = `((size_t)-1)` remain unexpressible,
because `0xFFFFFFFFFFFFFFFF` is `int_out_of_range` whatever the operators are.
That is a **type-vocabulary** gap and it now has its own panel owed
(`docs/debrief/DECIDE.md`, the author's own question about `u64`). Anyone reading
this sitting as having fixed the FFI's flag problem is reading it wrong.

**5. It is not free.** Five `BinOp`s, one `UnOp`, five tokens, and edits across
fourteen files — every one of them found by an exhaustive `match` rather than by
searching, which is the property that made the change safe rather than cheap.
Against it, one genuine subtraction: `reserved_operator` is **deleted**, because
nothing was left for it to fire on, and §9 admits no check without a test that
makes it fire.

---

## The four decisions the implementation forced

Each is a rule, not a detail, and each is pinned by `tests/golden/run/bitwise.hero`.

1. **`int` only.** A float's bits are not its value, and `bool` is excluded for
   §4.14's own reason: one of `&&`/`&` short-circuits and the other does not, so
   admitting both on `bool` would invite the reader to believe they are one
   operator.
2. **`&` binds tighter than `==`.** This is where C's table is on record as wrong —
   `x & 1 == 0` means `x & (1 == 0)` in C — and Heroes does not inherit it. Each of
   the six gets its own level, so `a | b ^ c & d` groups as in every language that
   copied C without copying that mistake.
3. **`1 << 63` is `INT64_MIN`, not an abort.** The shift is performed on the
   unsigned bit pattern and cast back: defined for every input, where C leaves a
   signed left shift into the sign bit undefined. Without the sign bit reachable a
   mask set cannot name its own top flag, which is the whole use.
4. **A shift count outside `0..63` aborts**, because C6.5.7p3 makes it undefined
   and CLAUDE.md §7 admits no C UB. `>>` is arithmetic, written explicitly rather
   than relying on C's implementation-defined choice.

---

## What the implementation discovered, and it is the sitting's real finding

**`|` is now dual-use, and that cost one silent defect on the day it arrived.**

A literal pattern was parsed with the full expression parser, so

    1 | 2  => "small"
    _      => "big"

silently became the *expression* `1 | 2` = 3. `name(1)` matched nothing, fell
through to `_`, and printed `big`. The compiler was right about every rule it knew
and wrong about the program — which is precisely the class this change was made to
remove, reintroduced by the change itself, for about an hour.

The premise that had made the old code correct — *no binary operator can follow a
literal in pattern position* — **was never written down**. It was true from
M-token-stream until this commit and expired silently, which is CLAUDE.md §11's
named failure mode, and the third instance this month. The repair is
`syntax::expr::pattern_operand`, which parses a pattern's operand at the unary
level so no binary operator is reachable from a pattern whatever the table grows,
and the test that fires if it is ever widened back is
`a_pattern_join_is_not_bitwise_or`.

This also **vindicates the llm-ergonomist's narrow veto at panel 039** — no
construct whose meaning depends on position rather than on its own line — from a
direction nobody predicted: it was a hazard for the *compiler*, not only for a
reader.

---

## Predictions to score

| # | prediction | checkable at |
|---|---|---|
| 1 | At the next FFI rung, **≥1 binding uses a bitwise operator** and **zero** programs in the corpus spell a flag union with `+`. Falsified if a rung lands with no bitwise operator in it, which would mean the warrant was theoretical | next FFI rung |
| 2 | **Panel 012's registered prediction for the +48**: the six operators produce **no new surviving-mutant class** in metric 3 — no `heroes mutate` operator's survivor list gains an entry whose mistake is a bitwise operator misread as its boolean twin (`&` for `&&`, `|` for `||`). Falsified by any such survivor, which would say the two families are confusable and the +48 bought a new silent class while closing another | first `mutate --survivors` run over a corpus containing bitwise code |
| 3 | `SIZE_MAX` and `CURLAUTH_ANY` are **still** unexpressible at the next rung, and the unsigned panel is what closes them | next FFI rung |

Prediction 2 is the one to read: it is the falsifier for this whole change. The
argument was that `+`-for-`|` is a silent wrong answer; if `&`-for-`&&` turns out
to be another one, the set traded a measured silent class for an unmeasured one.

---

## Ratification — 2026-08-12, by author decision in `/decide`

The set landed as judged: `& | ^ ~ << >>` on `int`, spec **+48 → 2675**, taken
over the narrower option of a diagnostic that only names the mistake. The
DESIGN-LOG line is the same date; the ledger row is `2675` in
`crates/heroes/src/measure/gate.rs`. This section is appended on **2026-08-13**,
because the decision was recorded everywhere except in the file that was decided
— readable, but only by someone who already knew where to look.

**And the appending found the defect panel 046 was convened about.** Predictions
1 and 3 above are scored *"at the next FFI rung"*, and M-ffi-ladder closed on
**2026-08-12** — the same day this sitting registered them — with neither one
looked at. Prediction 2 names `mutate --survivors`, an instrument that exists,
and is the only one of the three that panel 046's R1 would admit as payment
today. Under R2 the `2675` row is **re-decided at M-program-corpus**, and that
date is written once, in the gate's module doc beside the rows it governs.

---

## Predictions scored — 2026-08-13, at M-program-corpus (panel 046 R2)

**Prediction 2 — held, and vacuously.** `heroes mutate --survivors` over 5798
mutants produced 892 survivor lines and not one is a bitwise operator misread as
its boolean twin. The corpus contains bitwise code for the first time
(`examples/logs/mask.hero`: `|` to add a flag, `&` to test, `~` to take away, `^`
to toggle, `>>` to count, `<<` to declare the bits — the flag union this set was
argued from).

It holds because **none of the twelve operators makes that substitution**. The
prediction named a live instrument, which is what panel 046 R1 asks; the
instrument has no arm for the question. That is recorded as the score rather than
banked as evidence, and a `boolean-twin` operator is queued in `DECIDE.md`.

**Predictions 1 and 3 — lapsed, and they are the reason the ledger's date moved.**
Both named *"the next FFI rung"*. M-ffi-ladder closed 2026-08-12, the day after
this row was written, and neither was looked at. Panel 046 cites exactly this as
the evidence that a milestone-named expiry belongs beside the ledger row rather
than in a panel file, and `/step`'s close checklist now greps for it.

A by-product worth keeping: `typo-digit` produced **0** mutants over the corpus
when the §11 sweep looked at it. With bitwise constants in the tree it produces
**12**, of which 0 are killed — `constant BIT_TRACE: i64` with body `1` mutated
to `2` survives, as it must. The operator now measures something, and what it
measures is a hole.
