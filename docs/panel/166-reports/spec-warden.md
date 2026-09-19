# Panel 166 — spec-warden

**Every number below is REAL**, not a lower bound: `claude-opus-5` through
`POST /v1/messages/count_tokens`, reached by `heroes measure
spec/heroes-spec.md --refresh` on a scratchpad copy of this tree whose
`spec/heroes-spec.md` carried the draft, with the key sourced from the trunk's
`.env`. The compiler was rebuilt from the seed first — `clang -I runtime
seed/heroes.c runtime/runtime.c -o heroes`, 4.09 s user — as the brief asks.
Baseline reproduced exactly: vendored **6089**, real **8106**, digest
`3c065c560426eb07`.

**The budget is clean and I say so before anything else**, because panel 165's
ceiling argument must not be mistaken for the justification argument. Ceiling
10240, FFI floor 60, spendable today **2074**. The most expensive draft on the
table leaves **1984**. **No route on this table breaches the ceiling, and my
budget veto is not engaged.**

## 1. My first job: does § 3's carve-out already cover this case?

**No — and it does not need to, because § 3 is not false.** The brief and panel
165 are wrong on this point, and three programs say so.

The carve-out reads *"two copies reach one foreign thing … may still change, or
free, what C holds."* Both halves name the FOREIGN side. In defect 065 the bytes
are this language's own — ASan named the frame object `'h0_t'` — so the sentence
does not describe it.

But § 3's two claims are about aliasing among OWNED values, and both survive.
Measured, `../t065/alias2.hero`, exit 0:

```
t = slot_make()   b = Box(s: t)   xs = [t]
slot_fill(p: t.nsap.ptr(), n: 8)
t.nsap[0]     -> 65      # the lent binding
b.s.nsap[0]   -> 72      # a record holding a copy
xs[0].nsap[0] -> 72      # an array holding a copy
```

*Every value behaves as an independent copy* — **true**. *No aliasing exists
among the values this language owns* — **true**, and structurally so: a `ptr`
lend stands only as an argument of a call, `k: ptr @ s.b.ptr()` is
`field_lend_escapes` (`selfhost/check/lending.hero:298`,
`tests/golden/check/ffi-a-lent-field-needs-a-place.hero:34`), so no two owned
values can ever reach one storage. C is not a value this language owns.

**So exactly ONE sentence of the specification is false: § 5's *"only a declared
`@` name can be mutated"*.** And the document is not silent about what falsifies
it — § 13's own last sentence licenses it: *"`f.ptr()` lends a binding's field
… and C may write back through it."* The document is **internally
contradictory**, not incomplete, and the rule already lives in the section of
the operation that governs it (`.claude/rules/spec-shape.md`, § Where a rule
lives). **Defect 065 is therefore a compiler defect with a one-word document
consequence, not a document defect.**

**And the falsification is wider than the `=` binding.** An immutable PARAMETER
is written too, and the write is visible inside the callee — measured,
`../t065/eq.hero`: `function via_param(t: Slot)` fills `t.nsap` and reads back
**65**, while the caller's `u` reads **72**. `t` is not a `@` name and its bytes
changed, so § 5 is false there as well. `selfhost/check/lending.hero:304` and
`:309` **assert that shape legal today** (`swept == 0`). A route A written as
*"refuse an immutable binding"* leaves § 5 false in every function that takes a
record by value.

**063 composes with 065**, measured on a `=` binding: `slot_fill(p:
b.nsap.ptr(), n: 64)` corrupts the sibling `id` to `4702111234474983745` =
`0x4141414141414141`, exit 0.

## 2. What each draft costs — measured, real, not estimated

| draft | what it is | vendored | real | Δ real | Δ vendored |
|---|---|---|---|---|---|
| BASE | today | 6089 | **8106** | — | — |
| **D1** | § 13: *lends a **`@`** binding's field* (routes A/B) | 6092 | **8109** | **+3** | +3 |
| **D2d** | route G, aliasing half, ASCII *(section 13)* | 6122 | **8146** | **+40** | +33 |
| D2c | same wording with `§ 13` | 6122 | 8147 | +41 | +33 |
| D2a | same, appended instead of merged | 6133 | 8160 | +54 | +44 |
| **D2b** | route G **whole**: + *"Nothing checks that extent"* | 6152 | **8186** | **+80** | +63 |
| **D3** | route C: `counted_by n` prose + `CParam` production | 6128 | **8157** | **+51** | +39 |
| **D4** | route E: `Type` production + a named length in § 13 | 6115 | **8142** | **+36** | +26 |
| C1 | D1 + D3 | 6131 | 8160 | +54 | +42 |
| C2 | D1 + D3 + D4 | 6157 | 8196 | +90 | +68 |
| **DF** | route F: the `f.ptr()` sentence **removed** | 6056 | **8062** | **−44** | −33 |

Three things that table settles, each of which a sitting would otherwise guess:

- **The vendored delta underprices every draft by 20–30%** (D2d +33 vendored
  against +40 real; D3 +39 against +51). Ranking these on the offline number
  would have put D2a and C1 level at +54 real from +44 and +42 vendored.
- **`§` inside the document costs a whole check.** D2c goes red on
  `spec/inventory` — *"the spec now holds § · — … →"* — and the ASCII pointer
  `.claude/rules/spec-shape.md` permits is **one real token cheaper** (8146 vs
  8147). Any G-family wording uses *(section 13)*.
- **Merging beat appending again**: D2d 8146 against D2a 8160 for the same truth
  content, **−14 real**.
- Every draft reds the four pin checks (`budget`, `spendable`, `real`,
  `ledger`); whatever lands re-pins `SPEC_TOKENS`, `SPEC_REAL_TOKENS`, the
  digest and the ledger row **in one commit**. C2 alone crosses `DELTA_GATE`
  (50 vendored, +68), so its commit body must name what paid.

## 3. Verdict per route

**A — refuse `.ptr()` from an immutable binding. APPROVE, amended.** Document
cost **+3 real** (D1), and by panel 163's precedent it is a **correction that
pays with nothing**: the compiler narrows and the document stops over-promising.
The amendment is not optional — the refusal must read *"a `@` name"*, covering
the immutable PARAMETER measured above, or § 5 stays false inside every
by-value function. Cost of the amendment: two ratified assertions
(`lending.hero:304`, `:309`) turn red and are rewritten with `@`. **Route A
does not close 063**, which is the rank-3 defect.

**B — panel 164's resolution 2 (`@field` at a `@`-marked `ptr` parameter).
APPROVE in principle, UNPRICED, and I will not price it blind.** It is the only
route that puts the mark where § 5 and § 9 already say a mutation is marked —
*at the call site* — so a reader who believes the document writes the right
program. But it either replaces `f.ptr()` in § 13 or stands beside it, and those
are different documents: the first is a rewrite of a sentence bought two days
ago for +66 real, the second is two ways to say one thing, which §1.6 has
refused since *"three ways to write a loop: triple cost, zero gain"*. **Bring
one drafted sentence and I will measure it.** D1's +3 is B's floor, not its
price.

**C — `counted_by n`. APPROVE, and it is the only route that closes 063.**
**+51 real** (D3). CLAUDE.md § Precedence rank 3 puts robustness above token
cost, and design.md §1.12 makes no-corruption a goal of the LANGUAGE rather than
of its implementation, so this is paid for the same way a `cstr` null check is.
Payment under §1.6: a **registered prediction** (below), since the instrument
exists today; a named removal is available if the panel prefers one — DF's −44 —
but it is the same sentence C amends, so it cannot pay for C.

**Where the parameter rule goes, which the brief asked me alone.** § 13 **does**
carry a parameter production: `CParam` at the foot of the section already
enumerates every parameter marker the language has — `@`, `owned`, `consumes`,
`acquires`, `borrows`. Panel 165's finding is true only of the parameter TYPE
list, and **that list must not be added**: § 13 states the parameter rule
without one already (*"A parameter and a field are declared at the header's own
width and sign … and what a `ptr` points at"*). `counted_by` therefore goes in
`CParam` plus one clause merged into the `f.ptr()` sentence, which is what D3
measures. **The document needs no list of what a parameter may be.**

**D — `len()` on a fixed field. OBJECT, it waits.** Unpriced here because it
repairs neither defect: it lowers the cost of writing the honest number and
leaves the dishonest one legal. Under §1.2 that is a token saving against an
unchanged error probability, which the formula calls a net loss whenever it is
not free.

**E — the named extent. OBJECT, stays queued as panel 165's route 12.** **+36
real**, and it is the cheapest addition on the table, which is exactly the trap
§1.6 warns about: *a spec that grows because it can has failed §1.2*. It closes
nothing on its own — the brief's own words are that it *"would make D and C
portable"* — and it widens `Type` for **every fixed array in the language**,
not for extern fields. Principle 0: the compiler does not need it to compile
itself. If C lands and a real header's extent turns out to be unnameable, E is
re-argued with that header in hand.

**F — withdraw the lend. OBJECT, and I record the number because it is the only
removal on the table.** **−44 real**, measured. It closes both defects. I do not
recommend it: design.md §1.12's *"the boundary is complete: any C library must
be bindable"* is the same principle as its *"must not corrupt memory"*, and
`getcwd`, `read` and the 50-of-141 non-`const` pointer parameters panel 164
measured go back to unreachable. Trading one half of §1.12 for the other is not
a repair.

**G — write the hole down. VETO on its second half, on soundness.** D2b's
clause *"Nothing checks that extent: one past the field's own length reads and
writes past it"* costs **+80 real** and makes memory corruption a **documented
property of the language**. design.md §1.12 states that a Heroes program must
not corrupt memory is *"a goal of the language, not a quality of its
implementation"* — and the specification IS the language (§1.6: *that document
is not documentation, it is the prompt*). A sentence that promises the reader a
corruption they can reach is not a narrowing, it is a new guarantee with the
sign flipped. CLAUDE.md § 4: a veto is a refusal rather than a price, so no
token count redeems it.

**G's first half (D2d, +40 real) I OBJECT to rather than veto.** It is
admissible as a stopgap and it is honest. But it spends 40 real tokens to make
the document agree with a compiler defect, and when A/B lands those 40 tokens
come back out. **Order decides the price**: compiler first, and the document
owes **+3**; document first, and it owes **+40** and then a removal.

## 4. The ledger row I would write

Assuming the sitting adopts A/B-amended with C (C1, real 8160, vendored 6131,
+42 vendored — under `DELTA_GATE`):

> | 6131 | panel 166, 2026-09-19, at M-declared-extents: the lend had a door and
> no lock | **a `ptr` lend is marked at the binding and its extent is declared
> on the parameter** — § 13's `f.ptr()` sentence gains *`@`* before *binding's
> field* and a clause naming `counted_by n`; `CParam` gains `[ "counted_by"
> ident ]`. **+42** vendored and **+54** on the reader's instrument (6089 to
> **6131**; 8106 to **8160** on `claude-opus-5`; digest re-pinned). **The `@`
> word is a CORRECTION and pays with nothing** (panel 163's precedent, in the
> same direction: the document over-promised where the compiler is about to
> narrow). **`counted_by` is an addition and pays with a registered
> prediction** naming `./heroes check` and the M-declared-extents close, an
> instrument that exists today. **What the sitting measured that its own briefs
> had wrong: § 3 is NOT false** — a copy, a record field and an array element
> all read 72 while the lent binding read 65 — and a lend can stand only as a
> call argument, so no two owned values ever alias; **one** sentence was false,
> § 5's *only a declared `@` name can be mutated*, and it is false for an
> immutable PARAMETER as well as for a `=` binding, a shape two ratified
> assertions bless today. **What was refused**: writing the unchecked extent
> into the document (+80 real, vetoed on design.md §1.12), the named extent
> (+36, queued, closes nothing alone), and withdrawing the lend (−44, trades
> §1.12's completeness for §1.12's safety). **A `§` in the wording costs a
> check**: `spec/inventory` reds, and the ASCII *(section 13)* is one token
> cheaper. **1984 free**, and 1924 net of the FFI floor. |

## 5. One falsifiable prediction

**Registered, instrument exists today, scored at the M-declared-extents close.**

> Route A/B landed with D1's one word moves the document to **8109 ± 2 real**
> (`heroes measure spec/heroes-spec.md --refresh`), and turns exactly **two** of
> this repository's eighteen `.ptr()` sites red — `selfhost/check/lending.hero:304`
> and `:309`, both immutable parameters — with **zero** red sites in
> `examples/`, in `tests/golden/run/` and in `selfhost/` outside that file.
> Instrument: `./heroes check` over the sites enumerated by
> `grep -rn '\.ptr()' --include='*.hero' selfhost/ examples/ tests/`.

If a third site reds, the refusal reaches further than the defect and route A/B
is re-argued at the parameter position. If the real count lands above 8111, my
wording was not the one that was measured.

## 6. What would change my verdict

- **On C**: a drafted wording that closes 063 for **under +51 real**. I priced
  one shape; a cheaper true one beats it, and the panel should ask for it.
- **On B**: one drafted § 13 sentence. I refuse to rank B against C on a price
  nobody has measured.
- **On the G veto**: nothing. A specification sentence that promises a reachable
  corruption is refused at any price.
- **On E**: a real header whose field extent cannot be written as an integer
  literal, compiled. Then it is compiler-need rather than portability.

## 7. Commands, so every number above can be re-run

```sh
cp -r <trunk>/. <scratch>/work/ && cd <scratch>/work && rm -rf target build archive/bootstrap-rs
clang -I runtime seed/heroes.c runtime/runtime.c -o heroes     # 4.09 s user
set -a && . ./.env && set +a
./heroes measure spec/heroes-spec.md                            # vendored
./heroes measure spec/heroes-spec.md --refresh                  # real, claude-opus-5
./heroes run tests/harness/main.hero -- ./heroes spec           # 16 passed, 4 failed on any draft
./heroes run tests/harness/main.hero -- ./heroes grammar        # 7 passed, 0 failed — see below
./heroes run ../t065/eq.hero      # 72 / 65 / 65 / 72   exit 0
./heroes run ../t065/alias2.hero  # 65 / 72 / 72        exit 0
./heroes run ../t065/shapes.hero  # 65 / 65 / 4702111234474983745 (0x4141414141414141) exit 0
```

**One warning for whoever implements C or E.** The `grammar` suite read **7
passed, 0 failed** with D3 applied and with D4 applied — a production no parser
accepts is green. `.claude/rules/spec-shape.md` says why (*"it does not compare
a production to the parser"*), and this is the measurement: the document can
state `counted_by` or `i8[SL_NAME_LEN]` and no instrument will object. The
production lands **in the same commit as the parser**, or it lands as a lie the
net cannot see.
