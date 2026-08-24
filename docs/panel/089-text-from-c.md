# 089 — Text from C, and the sitting whose brief was wrong four times

Date: 2026-08-24. Full lane, five judges, differentiated inputs.
**Status** `ratified 2026-08-24` (author instruction, *"ok a tutte le ratifiche"*;
§ Author's verdict says what the yes settles and what it does not).

Convened on author instruction — *"convoca il panel e sistema il problema
scegliendo la soluzione più robusta anche se implica usare più token"* — which
names §12's tie-break and pre-authorises spec tokens. It does not suspend
Principle 0, and no seat treated it as if it did.

## The proposal, verbatim

> Reading text that came from C is a conversion that CAN fail, and `spec:59`
> files it under the sentence saying conversions of that name cannot. Measured:
> `c.to_str()` yields a plain `str`, and on real C bytes it is
> `panic: hero_str_from_bytes: not well-formed UTF-8`, exit 134 — in a program
> that tested `c == nullptr` first exactly as `spec:229` instructs. `args()` has
> the same hole with no channel at all. Asking C whether a pointer is safe is
> impossible, because `hero_cstr_nonnull` fires first.
>
> (A) `cstr.to_str() -> str?` · (B) a separate fallible spelling · (C) status quo
> plus a floor sentence · (D) a bound predicate — *measured impossible*.

## FOUR PREMISES IN THAT BRIEF WERE FALSE, AND THE SEATS FOUND ALL FOUR

This is recorded first because it is the sitting's most reusable output, and
because CLAUDE.md §1 exists for exactly this failure. The convener wrote the
brief; the convener got four things wrong.

**1. "The spec files a failing conversion under a sentence saying it cannot
fail" — a silence read as an open question.** `docs/panel/059`, **ratified
2026-08-15**, put option **C on its ballot, and C was exactly this sitting's
(A)**. It was **refused**, one veto and one unanswerable objection (059:203).
Its condition 1 records that **both** abort paths were measured *in that
sitting*, and that *"converting one aborts"* was chosen deliberately because it
*"names the one and claims nothing about the rest."* The sentence is not sloppy;
it is precise in a way the brief misread as incomplete. Found by the
compiler-engineer. **The convener's own pre-convening grep listed `059-the-read-
direction.md` and the convener did not open it** — CLAUDE.md §1's obligation to
read forward to the end of a record, failed at the one step that would have
caught it.

**2. "(D) is measured impossible" — a failed search written as an
impossibility.** The ffi-pragmatist compiled the whole thing: SQLite, no shim, no
language change, `x'fffe'` and a null column and a good row, **all three
classified, exit 0**, at HEAD. `hero_utf8_valid` has been exported since panel
035 with a comment that says why in the runtime's own words — *"Exported so a
binding can branch instead of dying."* The brief's negative claim rested on one
probe (a possibly-null pointer into a bound predicate) and `spec:229` already
makes the program test null first, after which the predicate is reachable. The
compiler-engineer broke it independently and more cheaply: **one line of
Heroes**, `if c == nullptr`, because what is closed is asking **C** whether a
pointer is NULL, and NULL is the one question the language already answers.

**3. "`hero_str_chars` takes one byte off an invalid sequence"** — the
mechanism is wrong. The loop does take one byte, and then hands it to
`hero_str_from_bytes`, which **panics**: exit 134, compiled (ffi-pragmatist,
experiment 6). The brief's *conclusion* — the abort stays — survives and is
stronger than stated.

**4. The framing itself.** The brief asked which spelling makes the boundary
possible. The boundary is already possible. The measured question is **171
tokens against 51** per binding module — ergonomics, §4.19, not robustness, §12.

## The verdict table

| seat | verdict | section | cost / delta | prediction | condition |
|---|---|---|---|---|---|
| **compiler-engineer** | **VETO on (A)**; approve (B) as a **Tier-2 library function** | §1.7 at design.md:476-480; ratified panel 059 | (A): **78+/9−, 11 files**, spec **+56**, 13–15 call sites, 5/145 emissions re-blessed. (B)-as-library: **32+/0−, 3 files**, **zero checker, zero emitter, zero migration** | at M-separate-compilation a Tier-2 landing touches exactly `str.c`, `heroes_runtime.h`, `library_source.hero`; ≤40 lines; ABI still 15; seed builds before regeneration; exactly 145 re-blessed | lifts on: a program (B)-as-library cannot express · the +1 assert × 145 judged dearer than (A)'s broken fixpoint · a one-commit landing for (A) · author overturning 059 |
| **ffi-pragmatist** | object to the premises; **(B) with `cstr.to_str()` retired**; **not vetoing** | §1.11, §4.19 vs CLAUDE.md §12 | (C) is not death, it is **171 vs 51 tokens**, 3.35× per binding module. Pre-check written *on top of* `from_bytes`: **1.96×**. Written *beside* it: **0.87×** | (B)+retirement compiles §4.19's ladder step 3 with no `extern "heroes_runtime.h"` and no shim, green after exactly **7** shipping edits, named | (1) one validation walk, beside `from_bytes` · (2) `cstr.to_str()` **retired**, not kept · (3) the two compiled riders queued. **Flips to VETO** if any existing signature changes or a `T?` is returned across the C boundary |
| **historian** | approve (B); object (A); **object hard (C)** | precedent | — | (A) → a second name within two milestones · (B) → a lossy third form will be asked for · (C) → a pre-flight validator, the worst outcome | (A) becomes defensible given one language with a UTF-8-invariant string, one name, fallibility keyed to argument type, kept 3+ years |
| **spec-warden** | approve-with-condition on **Fe0** (+3), objects to all four balloted | §1.6, §1.2, §1.4 | baseline **3506**; A **+30**, B **+23** (impossible name), C-validated **+40**, C-to_utf8 **+49**, **Fe0 +3** = +18 − **15** removal | Fe0 → `SPEC_TOKENS` 3509, ≤3 files, `cli_toolchain.hero` gets shorter; any C-family variant → ≥5 renamed sites and a wrong `certain` fix in a golden | the repair lands in **all four homes** of the false sentence · `SPEC_TOKENS` and the ledger row move in the same commit |
| **llm-ergonomist** (blind) | approve alpha; **object** to beta as balloted; no veto | reader locality | confidence 8/10 ceiling in both, and it is *dangling pointers*, not the diff | ≥50% of beta runs emit `to_str` on a `cstr`; ≥85% of alpha runs handle the failure arm against <60% of beta's | **withdraws the objection** if beta's rule paragraph gains ~12 tokens naming the exception — *"the diff as stated is a false choice"* |

## Where the seats disagreed, unsmoothed

**The blind seat approved the shape its four colleagues refused.** It read only
the spec and preferred (A) *because* beta's rule paragraph stopped covering the
hard case: under beta a reader consults the rule, is told `to_str` cannot fail,
writes `c.to_str()`, **and the document authorises it**. That objection is real
and is not answered by (A) winning — it is answered by amending the rule
paragraph, which is that seat's own condition 1 and which (B) can satisfy as
easily as (A). The disagreement is therefore about *the ballot*, not about the
language: four seats refuse (A)'s **shape**, one seat refuses (B)'s **wording**,
and the resolution below takes (B)'s shape with (A)'s wording discipline.

**The warden's Fe0 is (A)-shaped and is caught by the engineer's veto.** It is
the cheapest wording anyone produced (+3 against +23/+30/+40/+49) and it closes
the blind seat's hole inside the rule paragraph. It is not adopted, and the
reason is not its cost: `to_str` may not be the name that carries fallibility —
ratified, panel 059:302-305 — and the warden was not given 059. Its **payment**
survives the refusal of its wording and is taken below.

**The blind seat and the ffi-pragmatist agree from opposite ends** without
either seeing the other: greppability. *"`.text(` is greppable … maintenance work
is auditing that boundary."* The historian searched for that argument as a
**stated** rationale in any language's design record and came back **EMPTY**, and
said so rather than dressing an analogy as precedent. Nearest attested: the Rust
Book on `unsafe` as a marked region. Recorded as an analogy.

## What the field says (all sourced by the historian this session)

Sorted by the only axis that matters: **does the language's own string type carry
a UTF-8 invariant?**

- **No invariant** — Go, Nim, Ada, OCaml: the failure mode does not exist. One
  name, infallible, correct. **Not evidence for (C)**; evidence that Heroes has a
  problem they do not have.
- **Invariant** — Rust, Swift, Python, Erlang, Kotlin, Java, C#: **every one
  ships two or more names or a policy parameter at this boundary, and not one
  aborts the process.** Rust `CStr::to_str -> Result` **and** `to_string_lossy`,
  shipped in the *same* release (1.4.0). Swift ships three. Python puts the
  policy in a parameter.

Heroes has the invariant. The field is unanimous.

**(C) measured in the field is JNI**, the one language that made this conversion
nominally infallible: Android's own documentation tells callers to pre-filter,
and `CheckJNI` **aborts the VM** — crash reports in seven named projects. It has
no stable equilibrium: checking on, it aborts; checking off, it silently
corrupts.

**Two corrections the historian made to the convener, both sourced.** Swift's
`String(cString:)` is **not deprecated and does not trap** — it repairs with
U+FFFD; SE-0405 renamed the *fallible* one, for precondition disclosure. And this
project's standing claim that no language yields a plain value for some argument
types and an optional for others is **literally false** — Zig's `std.mem.span`
does exactly that, in Zig's own stdlib, and C# lifted operators have for twenty
years. **The nuance is what saves the argument against (A)**: both are
*propagation* — the result's optionality equals the argument's, and the caller
already wrote the `?`. (A)'s shape — optional *because this argument type is a C
pointer* — the historian found **nowhere**, and Rust examined and refused it
(RFC 1542, for uniformity).

## The resolution adopted — the most conservative one

**(B) as a Tier-2 library function, with the rule paragraph amended and the false
advice removed.** It is not a new decision: panel 059 refused `to_str` as the
carrier and **reserved the second-name form** as *"the shape any future sitting
starts from, and this ratification does not consume it."* This sitting starts
from it.

1. **Runtime, additive**: `HeroStr hero_str_try_from_cstr(const char *p,
   int64_t *status)` in `runtime/parts/str.c`, **beside** `hero_str_from_bytes`
   over the static `hero_str_alloc` — **one** validation walk, not two
   (ffi-pragmatist condition 1: composed on top it is 1.96×, written beside it
   0.87×). Same contract as `hero_file_read`. The abort at `str.c:244` is
   untouched, so panel 087's well-formedness invariant stands.
2. **ABI held at 15.** Adding a function is self-guarding — an old runtime is a
   link error, verified. Bumping to 16 **breaks the seed**, because
   `selfhost/emit_decls.hero:77` hardcodes 15 (compiler-engineer, measured).
3. **Library, Tier 2**: `validated(c: cstr) -> str?` in
   `selfhost/library_source.hero`. **Zero checker lines, zero emitter lines.**
   The name is the field's only attested one at this exact boundary (Swift's
   `validatingCString:`), and it is measured **free** of collisions where `text`
   would break **2,726** identifiers and `as_str` is refuted by Rust's own API
   guidelines (`as_` promises free and borrowed; this is O(n) and fallible).
4. **Spec**: the rule paragraph names the exception and points at `validated` —
   the blind seat's condition 1, which is what stops the document authorising the
   fatal line. Paid for with the warden's named removal: `spec:229`'s *"test
   `c == nullptr` first, because converting one aborts"*, **−15 tokens**, now
   redundant under §1.4 because the compiler is loud in both directions.
5. **The compiler's own defect is repaired in the same commit.**
   `HEROES_RUNTIME=$'\xff\xfe' heroes doctor` is **exit 134** at HEAD
   (compiler-engineer, reproduced by the coordinator): the shipping compiler can
   be killed by an environment variable, at `selfhost/cli_toolchain.hero:98`.

**Not adopted, and queued rather than done** — each is a surface change or a new
question, and the panel never blocks:

- **Retiring `cstr.to_str()`** (ffi-pragmatist condition 2). Its argument is
  strong — §12 does not let the crashing spelling stay the default — but it is a
  surface removal migrating 7 shipping sites plus goldens, and it is a checker
  change the engineer's "zero checker lines" costing does not cover. **The
  substance of the condition is met at the documentation level in this commit**:
  the spec stops teaching the aborting form. The code removal is the author's.
- **`args()`**, which neither (A) nor (B) reaches, because `hero_args_at`
  converts **eagerly** and no `cstr` ever reaches the program. The
  ffi-pragmatist's compiled shape is `args_checked() -> [str?]` — **not**
  `[str]?`, because one undecodable argument must not cost a program the other
  nine — over a new borrowed `hero_args_raw`, with `args()` keeping its signature
  for the 20+ programs that use it.
- **The two compiled riders**: `char *` struct fields are one line in
  `emit_extern_field.hero:148` (`HERO_RET_CSTR`'s six-way set applied to the
  member), and `char **` out-parameters are one narrowing in
  `emit_ffi_mutable.hero` that `emit_ops.hero:163` **already makes** for the null
  guard. Three of five real C string shapes are closed in *every* option; two of
  them cost one compiled line each.
- **CLAUDE.md §12's SEGV sentence is stale.** It says a null `cstr` handed
  onward is *"a SEGV in libsystem"*. Measured by the warden and reproduced by the
  coordinator: `panic: a null cstr was passed to a C function`, **exit 134**, a
  clean abort — the guard closed it. CLAUDE.md is amended by author instruction
  only, so it is queued, and it is **load-bearing**: it is why the removal in
  item 4 is redundancy §1.4 pays back rather than a warning being deleted.
- **A §11 world-premise, found by the ffi-pragmatist**:
  `runtime/parts/text.c:69` states `join(chars(s), "") == s` *"for every s"*.
  True only because nothing can build an ill-formed `str`; it dies the moment
  something can.

## Predictions to score

| seat | prediction | instrument | scored at |
|---|---|---|---|
| compiler-engineer | Tier-2 landing: exactly `str.c`, `heroes_runtime.h`, `library_source.hero`; ≤40 lines; ABI 15; seed builds pre-regeneration; exactly 145 emissions re-blessed | `git diff --stat`, `grep`, `suite_emission.hero` | M-separate-compilation |
| ffi-pragmatist | ladder step 3 with no `extern "heroes_runtime.h"`, no shim, green after exactly 7 named shipping edits | `suite_corpus.hero`, `suite_golden.hero` | M-separate-compilation |
| spec-warden | `SPEC_TOKENS` = 3509 under Fe0; a C-family variant instead → ≥5 renamed sites and a wrong `certain` fix in a golden | `heroes measure`, `suite_spec.hero` | M-separate-compilation |
| llm-ergonomist | ≥50% of runs on a beta-shaped spec emit `to_str` on a `cstr`; ≥85% of alpha runs handle the failure arm vs <60% of beta's | metric 2 | M-guide-book |
| historian | (B) → a lossy third form is requested within two years | the queue | M-guide-book |

## Process notes — the errors are the coordinator's

Three of the four false premises above were the convener's, and the fourth was
the convener's framing. The one that matters for next time: **the pre-convening
grep found `docs/panel/059` and the convener read the filename instead of the
file.** CLAUDE.md §1 already says to read forward to the end of a record; the
grep was run and its output was not used. A list of hits is not a search.

Two smaller ones, both caught by a contradiction rather than by care:
a name-collision table reported `text` as **0** hits when the true figure is
**2,726**, from a regex that did not match `function eprint(text: str)`; and the
first form of a falsifier for a different item reported 95 hits, all comment
prose. **Both were caught only because a second measurement disagreed with the
first.** Every measurement in this file that a decision rests on was re-run with
a positive and a negative control after that.

## Author's verdict

**2026-08-24: ratified** (author instruction, *"ok a tutte le ratifiche e alla
modifica al CLAUDE.md"*, given the same day the sitting closed and after reading
the synthesis rather than clause by clause).

**What the yes settles.** The second-name form is what this language does at the
C text boundary: `validated(c: cstr) -> str?` as a **Tier-2 library function**,
over a runtime entry point that validates **once**, with the ABI held at 15 and
zero checker and zero emitter lines. It settles that (A) is closed — the
compiler-engineer's veto stands and panel 059's ratified refusal of `to_str` as
the carrier stands with it — and that (C) is closed, on the historian's
unanimous field evidence and JNI's measured lack of a stable equilibrium. It
settles the spec at **3530**, +24 net against the −15 named removal, and it
settles that removal as §1.4 redundancy **paid back** rather than protection
deleted, which the CLAUDE.md correction below makes true in the contract as well.

**What the yes does NOT settle**, and all four remain open in
`docs/debrief/DECIDE.md`: retiring `cstr.to_str()` — the ffi-pragmatist's third
condition, unmet and deliberately so, because it is a surface removal; `args()`,
now the only one of panel 087's four doors still open; the two riders that seat
compiled (`char *` struct fields, `char **` out-parameters); and the shape of a
lossy third form, which the historian predicts will be asked for within two
years.

**Ratified with it, in the same instruction**: CLAUDE.md §12's SEGV sentence,
corrected below in its own commit — the sentence this sitting's payment rested
on.
