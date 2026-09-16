# Panel 158 — spec-warden report

- **verdict**: **object** (not a veto). Every count below is measured; the REAL
  deltas of the drafts are **estimated** and marked, so treat those rows as
  provisional.
- **section**: design.md §1.6 (budget, payment rule), §1.2 (cost formula), §1.7
  (is it a simplification), §4.6 (`T??` under monomorphisation), §4.17
  (diagnostics); spec § 3, § 5, § 9, § 10; CLAUDE.md § 2 (Principle 0), § 12.
- **spec_token_delta**: baseline **measured** 2026-09-16, `./heroes measure
  spec/heroes-spec.md`, exit 0 — claude-legacy 5863, cl100k_base **5989**, real
  **7974** (`claude-opus-5`, record dated 2026-09-15), ceiling **10240**
  (grepped, design.md:255), headroom 2266 / 2206 net of the FFI floor. My
  recommended resolution is **+0 / +0**.
- **removal**: none needed, because nothing enters the document.
- **needed_for_self_hosting**: **no** — measured: `grep -rnE "^function [a-z_]+<"
  selfhost/` reads **0**.
- **prediction**: with the resolution below, `heroes measure
  spec/heroes-spec.md --refresh` reads **7974** real / **5989** vendored and
  digest `21a9dc541cfa2fa8` at the milestone close, unchanged.
- **condition**: `selfhost/` acquiring its first generic, or a measured Part 11
  first-try-rate delta, or a measured corruption on the nested path.

## 1. The central question: the specification is ALREADY RIGHT, on both halves

Grepped, not taken from the brief.

| what the document says | where |
|---|---|
| ``m[k]` is a `V?` with code `missing_key`` — unqualified, so `V = i64?` promises the VALUE `i64??` | § 10:287 |
| generics, **no constraints**, so `find<A>(…) -> A?` with `A = i64?` promises it too | § 9:258-262 |
| *"Both read the OUTERMOST type, so a `[T?]`, a record holding one, or **a type parameter that arrived fallible** is still dropped"* | § 5:128-131 |
| `Type = Prefix { "[" integer "]" } [ "?" ] .` — `[ ]` is **optional**, so at most one `?`: **the written form `i64??` is underivable** | § 3:71 |
| `[ ]` encloses what is optional, `{ }` what repeats — the metasyntax is defined in the document | opening paragraph, lines 5-7 |

So the document says, deliberately and in four places: **the value exists, the
spelling does not.** Those are consistent. There is no false sentence, so R1
option 4's premise fails — "qualify the spec" has nothing to correct, and any
qualification restates the production. Ledger row 5863 refused a draft *"for
saying it twice"* and `.claude/rules/spec-shape.md` gives every rule one home.

**design.md agrees.** §4.6: *"No implicit `T` → `T?` promotion exists, ever …
it creates the `T??` level ambiguity under monomorphisation with `A := Expr?`,
the exact trap Swift had to patch with SE-0230."* The document foresaw this
exact value and closed the trap that makes it ambiguous. Nothing is left to
repair in the language.

## 2. What is wrong is in the compiler, and it costs zero spec tokens

`selfhost/check/table.hero:66-67`, above the `fallible` node:

    # `T?` — never nested: `T??` is rejected by the parser, so this
    # node's argument is never itself a fallible.

**That invariant is false**, and this sitting's own `--dump-ir` measurement
(`$t8: i64?? = call heroes find(…)`) falsifies it. A false premise written into
the checker is the defect. CLAUDE.md § 12: the spec beats the compiler, so the
compiler has the bug — **+0 spec tokens**.

## 3. The four repairs, priced. Instrument: `./heroes measure`, baseline 5989

Real deltas are ESTIMATED by the ledger's own measured band (×1.20 to ×1.45,
rows 5945/5988); `--refresh` is UNRUN here because `ANTHROPIC_API_KEY` is unset
in this seat's environment.

| # | draft | vendored Δ | real Δ (est.) |
|---|---|---|---|
| 1 | refuse `{K: V?}`, merged into § 10's sentence | **+20** | +24 to +29 |
| 1b | …and close the OTHER two doors (a type parameter never binds a fallible) | **+32** | +38 to +46 |
| 2 | flatten `m[k]` (VETOED on soundness; priced so the refusal has a number) | **+22** | +26 to +32 |
| 3a | `[ "?" ]` → `{ "?" }` in § 3's production, nothing else | **+0** | ~0 |
| 3b | 3a plus the one sentence a reader needs to peel two levels | **+28** | +34 to +41 |
| 4 | qualify, as a free-standing sentence after the production | **+42** | +50 to +61 |
| 5 | the cheapest honest merge, § 3's type row **or** § 10's map sentence | **+9** | +11 to +13 |

Drafts are in the scratchpad, `…/scratchpad/copy/drafts/`. All sit under
`DELTA_GATE` (50, VENDORED, `selfhost/measure/judged.hero:156`).

**Option 3's +0 is an illusion**, and this is the warden's job to say. A
production that legalises a form the prose never teaches is a trap in the one
document a reader is told to trust; the honest price is 3b, **+28**.

## 4. And option 3 does not do what the brief says it does. MEASURED

The brief calls option 3 *"the only option that closes all three doors"*. Run:

    function f(x: i64???) -> i64
    error[expected_params_close]: expected `)`, or `,` and another parameter, found `???`

`???` is the **hole** token (§ 12; the `Primary` production lists `"???"`), and
the lexer takes it by maximal munch. The brief's own fourth fact says a user
generic reaches `i64???` via `wrap<T>` twice. So option 3 legalises two levels,
leaves the third unspellable, and replaces a precise refusal with a message
about a hole. It closes two doors of three and makes the third worse.

## 5. Principle 0, and it is panel 155 R3's ground exactly

- compiler need: **none, measured** — 0 generic declarations in `selfhost/`;
- a spec sentence behind it: **none** — § 1 above shows the document consistent;
- a measured Part 11 effect: **none** — the footing is CONSISTENCY, and the
  library surface is `find<A>` alone of six (`selfhost/library_source.hero:76-122`,
  read).

Nothing may ENTER on that footing. Panel 155 R3 made `float_map_key` WAIT on it.

## 6. R1-R4

- **R1: a FIFTH option, and it is +0 spec tokens.** The document stands. Repair
  the compiler: delete the false invariant at `check/table.hero:66-67`, and let
  the `type_mismatch` message say the found form has no spelling. Object to 1
  (breaks the document's own symmetry — § 5 blesses `[T?]` and a record holding
  one, so singling out the map is an inconsistency sold as a consistency
  repair, and it closes one door of three), to 2 (soundness), to 3 (§ 4 above,
  plus §1.2: it removes a compile error and buys an error class), to 4 (§ 1
  above). If the sitting insists on text, take option 5 at **+9**, never 4 at +42.
- **R2: not a defect.** `selfhost/parse/type.hero:51-63` raises
  `nested_fallible` while **swallowing** the extra `?` and returning a SINGLE
  `.fallible` node. Add the code to `is_thesis_rule` unchanged and
  `--permissive` compiles `x: i64??` as `i64?` — a program that means something
  other than what it says. design.md:3727 names five control-arm checks and all
  five permit the SAME program; this one would change it. Correctly absent.
- **R3: printing `i64??` is acceptable; hiding it would be worse.** Measured:
  the message already carries `fix (guess): `.must()` — abort on the error case,
  giving `i64?``, which is correct and actionable, so the brief's *"a message
  that cannot be acted on"* is **falsified by the run**. What is owed is one
  clause saying the form cannot be written. +0 spec tokens.
- **R4: yes, decisively.** Consistency with no safety claim, no compiler need
  and no measured thesis effect is the WAIT case.

## 7. UNRUN

- **Real (`claude-opus-5`) deltas for all seven drafts.** `ANTHROPIC_API_KEY` is
  unset here. Settled by `heroes measure spec/heroes-spec.md --refresh` from a
  tree whose spec carries the draft.
- **The baseline's own `real` 7974 is a RECORD dated 2026-09-15**, not a live
  count; the spec file has not moved since (`git log -1 -- spec/heroes-spec.md`
  = `559af725`, 2026-09-15). Settled by the same `--refresh`.
- **That `--permissive` would mean `i64?`** is read from the parser, not run:
  the run needs `nested_fallible` added to `is_thesis_rule` and a 59 s rebuild,
  which this sitting forbids.
- Line counts for any compiler-side repair: not prototyped, for the same reason.

## 8. The prediction, and what it pays

Payment for my resolution is a **named removal**: the false comment at
`check/table.hero:66-67` goes, and nothing enters the document, so §1.6's rule
is met at +0.

**Falsifiable, instrument existing today (`grep` + `heroes check`):** landing
option 3 requires deleting exactly **five** `nested_fallible` sites —
`selfhost/parse/type.hero:55` and its test at `:403`, the golden pair
`tests/golden/check/a-fallible-type-is-never-written-fallible-twice.{hero,expected}`
(3 annotations, 3 expected lines) — and **zero** programs elsewhere in the tree
change. A sixth site, or any program in `examples/` or `tests/golden/` that
moves, falsifies it.

**Registered as an observation and paying nothing** (§1.6's rule: the instrument
must exist today, and metric 2 does not): under option 3 the accidental `??` —
a doubled keystroke, the most plausible LLM mistake here — stops being a compile
error and becomes a silent type change, which §1.2 prices at one 500-2000 token
round trip each time it is not caught.

**And the standard clause**: if a document option lands, its landed text reads
exactly the vendored number in § 3's table for that row — 6009, 6021, 6011,
5989, 6017, 6031, 5998. Any other number means the landed text is not the text
priced.
