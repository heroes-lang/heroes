# Panel 158 — the sitting produced a resolution and left it off its own ballot

2026-09-16, at M-check-completeness. Five seats and the completeness critic.
**Resolution provisional — ratified the same day by the author's standing
delegation, recorded at the foot.**

Convened on a doubly-fallible value: `i64??` is tracked by the checker, printed
by the compiler, and refused by the parser. Two vetoes were cast, on two
different options. The critic then found that a combination of the seats' own
amendments carries **no veto at all**, and that a route nobody named dissolves
the harder veto rather than paying it.

## The four repairs as put, and the verdict table

1. refuse `{K: V?}` at the declaration · 2. flatten `m[k]` · 3. make written
`T??` legal · 4. leave it and qualify the spec.

| seat | R1 | veto |
|---|---|---|
| **compiler-engineer** | option 4 plus a booked defect | **CAST**, on option 3 *as scoped* |
| **llm-ergonomist** | 3 > 4 > 1 > 2 | **CAST**, on option 2 |
| **spec-warden** | a fifth option at +0 tokens | not cast |
| **ffi-pragmatist** | option 3 plus a checker move | not cast |
| **historian** | precedent refuses the flatten | advisory |

## What was measured, and it moved every question

**The footing is not safety, and the ffi seat proved it rather than assuming
it.** Twelve shapes across two platforms — `str??`, `str???` with allocating
payloads, `Res??`/`Res???` released and not, `find` over `[Res?]`, real SQLite
with a `Stmt??` finalized and dropped — 0 where 0 was owed and 134 where 134 was
owed, with a deliberate 512-byte leak proving LeakSanitizer awake on the Linux
leg. **§1.12 was tested and NOT reached**, said plainly rather than manufactured.

**And a fallible has no C ABI at any depth.** `error[ffi_type]` refuses it as an
extern parameter, as a result and as an extern record field. So `T??` is a
representation C never sees and **every repair costs bindings zero** — the whole
question is internal to the language.

**Option 1 is measured near-useless.** Map-of-fallible declarations across
`examples/`, `selfhost/` and `tests/` number **four, all four inside
`tests/golden/check/`** — zero in the corpus, zero in the compiler. It breaks no
program *and* protects none, while leaving the generic and `find` doors open,
both of which were run at exit 0.

## THE HINGE: two seats read the same document oppositely, and both lose half

The llm-ergonomist, reading only the specification, found § 10 (`m[k]` is a
`V?`) and § 3's production (`Type = Prefix { "[" integer "]" } [ "?" ]`, at most
one `?`) **contradictory, with the document declining to choose**. The
spec-warden, grepping the same text, found them **consistent — value yes,
spelling no — and said so is stated four times** over § 10, § 9, § 5 and § 3.

**On the formal point the warden is right.** § 10 is a rule about the type an
expression *has*; § 3 is a production over the types a program *writes*. The
historian's Rust `{integer}` item is the shipped precedent for exactly that
distinction.

**On *four times* the warden is wrong, measured.** `grep -niE "cannot be
written|no spelling|unwritable|not writable|cannot write|no written"` over the
specification returns **0**. `??` appears nowhere in it — the two hits are `???`,
the hole. The warden's rows state the **value** half three times, none of them
mentioning a spelling, and the **spelling** half once, not mentioning that a
computed type may exceed it. **The relation between them is stated zero times**,
and a reader reaches it only by the inference the warden itself performed, whose
connective is its own *"so"* (CL-018).

**The consequence is that the warden's +0 resolution does not exist.** It refuses
option 4 on the ground that a qualification "has nothing to correct" and cites a
ledger row that refused a draft *for saying it twice*. The document says it
**zero** times, so a sentence here is a **first** statement, not a repetition.
The warden's own **+9** merge is what its argument actually supports.

## THE ROUTE NOBODY NAMED, and it dissolves the harder veto

The compiler seat's veto rests on one measured collision, and it built the patch
to find it: under option 3, `heroes fmt` on `v: i64?? ?` prints **`error: this is
a compiler bug`** at exit 2, because `selfhost/print/types.hero:25` renders three
levels as `i64???` and the lexer max-munches that as the **hole** token. On the
seed the same file is a clean parse error, so **the patch creates the breakage** —
§4.15, and the formatter is the tool whose failure makes every diff in the
project untrustworthy.

**That collision is not intrinsic to option 3. It is intrinsic to rendering `?`
adjacent to `?`.** The llm-ergonomist recorded the missing piece without seeing
its use: `Prefix` has **no `"(" Type ")"` alternative**. Add one and `(i64?)?`
and `((i64?)?)?` are writable at any depth, with **no lexer change at all** and a
formatter that round-trips. The unpriced cost is that `(i64)?` becomes writable
too, so `fmt` acquires a canonicalisation duty. **UNRUN**, and it is the first
thing the repair must measure.

**And the sitting had a zero-veto resolution on the table without noticing.** The
ffi seat's amendment (move `pointer_element` from the emitter into the checker)
and the compiler seat's condition (price and land the `???` half) are not the
same option in two hats: they are **two different missing halves of option 3, and
they are additive**. Option 3 with both carries **no veto and one objection**.
Nobody scored that version.

## THE FRAMING WAS WRONG, and the critic ran the program that shows it

*Consistency, not safety* has two boxes and the thesis's own failure mode is in
neither. Run, `./heroes check`, **exit 0 on both**:

```
m: {str: i64?} @ {}   m["a"] @ fail(…)   if m["b"].is_err()     → exit 0
xs: [i64?]            hit = find(xs, is_ok)   if hit.is_err()   → exit 0
```

Both interrogate the **outer** layer while reading as a test of the stored
failure. **That is a plausible mistake which compiles today, before any repair**,
and it is §1.1's subject rather than §1.12's. Two consequences no seat drew:
**option 1 is the only listed repair that makes it a compile error**, and
**option 3 does not close it at all.**

## The resolution adopted

**R1. The spelling gap is closed by the parenthesis route, not by deleting the
refusal.** `Prefix` gains `"(" Type ")"`. `(i64?)?` and deeper become writable at
any depth; `i64??` written bare stays refused, so `nested_fallible` keeps its
job and its golden. No lexer surgery, no `???` collision, and the formatter round
trips — which is what both halves of the compiler seat's veto asked for. **What
it owes before landing**: the `fmt` canonicalisation duty for `(i64)?`, measured,
and the walk of `.claude/rules/diagnostics-and-goldens.md` § *a new surface form
lands in every tool*, the two colourers included.

**R2. Flattening stays vetoed**, and the historian's precedent is why it is
refused rather than merely declined. Swift's SE-0230 flattened the **operator**
`try?` and said in as many words *"we are not proposing to eliminate nested
Optionals from the language entirely"*; its `Dictionary` subscript still yields
`Value??` and keeps absent distinct from present-and-empty. And Swift bought its
collapse with an **enumeration** — 613 sites, *"zero cases … distinguish between
the error case and the nil-as-a-value case"* — not with an argument. **The
equivalent count here is empty, not zero**: the producer half returns two files,
both written to probe this refusal. An empty corpus licenses nothing, which the
critic says plainly and which is the difference between Swift's evidence and
ours.

**R3. The specification owes a FIRST sentence, not a qualification.** The
relation between the value § 10 promises and the spelling § 3 permits is stated
zero times. The warden's **+9** merge is the price its own argument supports, and
the sentence must say what a reader cannot otherwise derive: that a `T?` may
itself be fallible, and how many levels an operation peels. The ergonomist's
draft — merged into § 6's opening, ~26 tokens — and the warden's +9 are the two
candidates, and the cheaper honest one wins on §1.6.

**R4. Two defects are booked, and one of them compiles today.**

- **The false invariant.** `selfhost/check/table.hero:66-67` asserts *"`T?` —
  never nested … this node's argument is never itself a fallible"*, measured
  false three ways at exit 0 by two seats independently. `docs/design/design.md:2836-2837`
  cites those lines as a live invariant inside the `alias` costing, and nobody
  booked that correction. **The critic corrected the compiler seat here**:
  `check/builtins.hero:17` is **true** and claims nothing about what the checker
  can build, so it is one comment and not two. And the enumeration nobody ran:
  is any `.fallible` **consumer** non-recursive? Three of twenty-odd are proved
  recursive; the rest are unchecked.
- **The silent `.is_err()`.** The two programs above, exit 0 today. A test that
  reads as asking about the stored failure and asks about the outer layer.

**R5. `--permissive` is NOT a defect**, and three seats reached that by three
routes. The warden: adding `nested_fallible` to `is_thesis_rule` would make
`--permissive` compile `x: i64??` **as `i64?`**, because the parser swallows the
extra `?` — a program meaning something other than what it says. The compiler
seat: the fifteen codes there are all rules about programs the checker could
otherwise compile, and this is a parser refusal that would leave no type node.
The ffi seat, measured: `check --permissive` on `i64??` **still exits 1**, so the
question was moot — and no `ffi_*` code is in that list either, which must stay
so, because Part 11's control arm is §1 switched off and not the C boundary
switched off.

## What the conservative resolution would have been

Option 4 alone — leave the language, add a sentence — which the compiler seat
recommended. It is refused here because it leaves the compiler **printing a type
it will not read back**, which the historian found no precedent for: printing an
unspellable type is shipped and defensible (Rust's `{integer}`), and *refusing to
parse a type you fully print* is not. The difference from Rust matters and the
historian drew it: `{integer}` denotes a type **not yet decided**, and `i64??` is
one the checker fully knows.

## Predictions to score

| seat | prediction | scored by |
|---|---|---|
| compiler-engineer | under option 3 *as scoped*, `fmt` over a three-level fallible exits non-zero with *this is a compiler bug*, and `annotations` reports ≥3 orphaned `#~` | already measured; the parenthesis route must falsify it |
| spec-warden | with a +0 resolution, `measure --refresh` reads 7974 / 5989, digest unchanged | at the repair's close |
| spec-warden | landing option 3 needs exactly **five** `nested_fallible` sites deleted; a sixth falsifies it | `grep -c` at the close |
| llm-ergonomist | flattening scores the **best** first-try compile rate of all four while behavioural mismatch goes to ~100% | any harness scoring only compilation; M-thesis-harness |
| ffi-pragmatist | `examples/sqlite/`, `examples/curl/` and `examples/ledger/db/sqlite.hero` need no shim and their emitted C is unchanged | `--emit-c` diff at the close |
| historian | if a sentence ships, the confusion arrives at the **write** side — `m["a"] @ fail(…)` versus a key never set | unmeasured; one command |

## Process, recorded against this sitting

**No seat was killed by the watchdog**, the third sitting in a row under the
write-your-report-first rule.

**Seven claims in the coordinator's briefs were corrected by seats**, after six
at panels 155-157. The two new ones: the shared brief said the diagnostic *cannot
be acted on* — it carries `fix (guess): .must()`, falsified by running it — and
its library row cited a grep that reads **0**, because the library is Heroes
source embedded as string literals. The content of that row survives; the
citation does not. **The warden's own `needed_for_self_hosting` rests on the same
grep shape**, so that instrument is not what it claims, and the conclusion
survives only through the compiler seat's independent count.

**A procedural knot the synthesis records rather than settles.** The
llm-ergonomist pre-declared corpus evidence inadmissible for its own verdict,
which fences off the one instrument the historian's precedent runs on. CLAUDE.md
§ 12 says measurement beats opinion, including the panel's — so the count stays
admissible for the sitting even where a seat declines to use it.

## Author's verdict

**RATIFIED 2026-09-16, as adopted — BY DELEGATION AND NOT BY READING**, under the
author's standing instruction of that day. The yes is the assistant's judgement
under an authority the author handed over, on a sitting the author has not read;
recording it otherwise would credit them with a reading that did not happen.

**What the yes settles.** That the spelling gap closes by widening `Prefix`
rather than by deleting a refusal or cutting the lexer. That flattening stays
refused, and that the corpus count which might have licensed it is empty rather
than zero. That the specification owes a first sentence and not a correction.
That the false invariant and the silent `.is_err()` are booked.

**What it does not settle.** The parenthesis route is **UNRUN** — its
canonicalisation duty and its tool walk are the first things its implementation
owes, and if `fmt` cannot round-trip `(i64)?` the route fails and the sitting
returns to a vetoed ballot. Nothing here is landed.
