# 090 — A spelling that aborts, aborts nobody names, and a no-op that jumps

Date: 2026-08-24/25. Full lane, five judges, differentiated inputs.
**Status** `ratified 2026-08-25` (author instruction, *"ratifica 090"*;
§ Author's verdict says what the yes settles and what it does not).

Convened on author instruction — *"panel su tutte"*, with *"il linguaggio deve
essere super solido"* as the deciding criterion. §12's tie-break; spec tokens
pre-authorised where they buy safety. Principle 0 was not suspended and two seats
used it.

## THE FILTER THAT RAN BEFORE THE BRIEF, AND WHAT IT REMOVED

The author asked for a sitting on **five** queued questions. Two were struck
before any judge was briefed, because the record already held their answers —
which is the check panel 088 did not run and paid three hours for:

- **A bytes reader.** Panel 087, ratified: *"Principle 0 has no compiler-need for
  it today, measured"*, and its prediction names the trigger that brings it back
  (the first milestone whose harness must read a non-`.hero` artifact byte for
  byte). A settled deferral, not an open question.
- **The not-text `e.code`.** Panel 087 states a **reversal condition** — a named
  reader in the same commit plus a golden that fires the arm. That is
  constructible work, not a question to re-argue.

Three questions reached the judges. All three came back changed.

## The proposal, verbatim

> **Q1** — retire `cstr.to_str()`, or rule that it stays beside `validated`?
> Measured: it compiles and aborts at exit 134 on a null pointer or non-UTF-8
> bytes; `validated` handles both at exit 0. The spec no longer teaches it — the
> compiler still does, in three `check_ffi.hero` notes.
> **Q2** — should the spec name the ~12 reachable aborts it is silent about?
> Measured: 67 `hero_panic` sites / 51 distinct messages against 6 spec lines
> saying "abort". Two wordings costed: +29 and +40.
> **Q3** — `continue` as an arm's no-op is a loop jump that compiles at exit 0
> and hangs. Two wordings costed: a `pass` keyword at +25, a warning at +24.

## The verdict table

| seat | Q1 | Q2 | Q3 | its own strongest measurement |
|---|---|---|---|---|
| **ffi-pragmatist** | **approve retirement** | (item 4 only) | — | C has **one** way to promise non-null, `returns_nonnull`: **0 in sqlite3.h, curl.h, raylib.h, SDL2, SDL3, string.h, stdlib.h — 2 of 3120 SDK headers.** No real library gives a program anything to know with |
| **spec-warden** | **approve, +0 tokens** | **object to both**; adopt `q2-args2` (+16) | **VETO `pass`** (Principle 0); adopt `w-jump4` (+24) | ran the retirement in a worktree: `check selfhost/main.hero` **clean, 2m39s** — no compiler-need, proved not assumed |
| **compiler-engineer** | **object** — to the priority, not the cost (~25 lines, 5 files) | **VETO the enumeration** | **VETO `pass`** (§1.7) · **VETO the diagnostic** | the proposed checker rule fires on **3 of 3** `while`-enclosed `continue` arms in the compiler and **all three are correct — 100% false positives** |
| **llm-ergonomist** (blind) | — | four of five silences are one class | **approve `pass`**, object to warning-only | **it wrote the hanging program itself**, rated its own confidence **7/10**, then 0/10 on re-reading |
| **historian** | no precedent for removal anywhere | no spec promises completeness | PHP withdrew its removal RFC and shipped a warning **naming the alternative** | C tried the enumeration and had to mark Annex J.2 **non-normative and incomplete** |

## Q1 — `cstr.to_str()` is retired

**Two approvals, one objection to the ORDER rather than the act, one advisory
against on a precedent two seats rebutted.**

The ffi-pragmatist went looking for the case where retirement makes a program
worse and **found the opposite**. `blob_tostr.hero`: real SQLite, a real
`x'fffe4142'` blob through `sqlite3_column_text`, **guarding `cell == nullptr`
exactly as `spec:229` used to instruct** — `panic: hero_str_from_bytes: not
well-formed UTF-8`, exit 134. The same program with `validated`: exit 0. *"The
null guard does not save it, because the failure is not nullness."*

And the mechanism question is closed by a sweep rather than an argument: C's only
way to promise a non-null return is `__attribute__((returns_nonnull))`, and it
occurs **2 times in 3120 SDK headers**, one of them C++ and one the macro's own
definition. Every *"I know this one is safe"* is therefore a §11 premise about
the world, not a fact about the value.

Cost, measured by two seats independently: **~25 lines across 5 files**, **11 call sites, migrated and counted**, **0 spec tokens** (the spec already says `to_str` cannot fail and
routes a `cstr` to `validated`), **ABI unmoved at 15**, **+15% per conversion**
(0.20 s → 0.23 s over 2,000,000), which §13 does not reach. The warden proved no
compiler-need by *doing it* — retirement applied in a worktree, `check
selfhost/main.hero` clean in 2m39s.

**What retirement buys beyond safety, and it is not obvious:** `?` propagation.
`propagate.hero` carries a SQLite error through two stack frames and three
distinct outcomes at exit 0. *"A value that aborts has no error to propagate."*

**The historian's precedent, and why it does not bind.** No language in seven
surveyed has ever *removed* an aborting FFI conversion — Rust cannot (stability
policy), Java has not in thirty years, Swift abolished the IUO type and kept the
trap. The warden answered it and the answer is accepted: Swift kept
`String(cString:)` **for source compatibility with a shipped ecosystem**. Heroes
has **eleven call sites and no users** — the seats estimated 10 and 13, and eleven
is what the migration actually touched. The precedent's *reason* does not transfer;
§1.12's tie-break does.

**The engineer's objection is honoured rather than overruled.** It does not
dispute the cost or the safety; it says the crashing default a program actually
meets is **`args()`**, not `to_str`, and *"a spec that ratifies a defect is worse
than a silent one"*. So the order is its order: **`args()` is answered first, in
this same sitting's Q2, and the retirement follows.**

## Q2 — both ballot options are refused, and the abort that mattered was not on the ballot

**The convener's own wording asserted something false, and the warden caught it by
running it.** `q2-full` said a float too large to render aborts. Reached by
squaring, `inf` **prints at exit 0** — verified independently by the coordinator.
A false sentence in the prompt is what panel 035 called the worst possible spend,
and 035 said it about this document.

The warden enumerated all 51 distinct messages: **24 say "compiler bug"** and are
unreachable; the nan map key is a **compile error** with a teaching note; OOM and
too-large are real but **change no program text**, because there is no
allocation-failure construct to write instead — pure §1.2 cost at zero rewrite-rate
benefit. Panel 035 R4 covers all of it and does not stop short.

**What R4 does not cover is an abort that is data-dependent and decides which
built-in you call** — and both compiling seats found the same one independently:

```
./prog $'\xff\xfe'
panic: hero_str_from_bytes: not well-formed UTF-8      exit 134
```

`args()` is a closure-list built-in, listed with no warning, sitting beside
`args_checked() -> [str?]` **to which the spec gave no reason to exist**. A
correct program, killed by an argument the *user* typed, passing every test the
author runs because the author's argv is UTF-8. The engineer reached it from the
runtime — `os.c:156` guards the file reader with `hero_utf8_valid` and `os.c:62`
does not, one file apart — and filed it as `docs/defects/002` shipped without its
adjacent shape.

**Adopted:** `args() -> [str]` (…; **one that is not UTF-8 aborts**) ·
`args_checked() -> [str?]` (**which does not**). **+16**, and it repairs a second
defect for free: it gives `args_checked` the reason §1.6 says it needs.

**Refused:** the full enumeration (+40 as costed here; the engineer measured a
real 34-line enumeration at **+364**, which is 67% of all remaining headroom) and
the minimal blanket. The historian's survey says the same thing from outside:
**no specification promises a complete list.** C tried and marked Annex J.2
non-normative and incomplete; the JVMS put *"may be thrown at any time"* into
normative text; Ada covers everything in one sentence; Go refuses to name the
values.

## Q3 — `pass` is vetoed twice; the sentence that sets the trap is what changes

The blind seat is the finding. Given only the spec, its honest first draft was

```
            .space => continue
        i @ i + 1
```

which compiles and never terminates — and it explains the funnel: `_` is
forbidden as a variant catch-all, a computed value must be used, and *"a jump is
a valid arm body: it yields no value"*. **Doors close until `continue` is the
only blessed spelling, and `continue` skips the counter.** It rated its own
confidence **7/10** at the time and **0/10** on re-reading. The warden measured
the damage as worse than a hang: `n=202` where a no-op gives `302`, **exit 0, no
diagnostic** — a silent wrong answer.

**`pass` is vetoed by both compiling seats**, on different grounds and both
measured. §1.7: a no-op keyword adds a construct to lexer, parser, resolver,
checker, lowering and formatter — **27 exhaustive-match sites** — and reduces to
nothing, so Part 5's *"Sugar | Reduces to"* table has no row for it. Principle 0:
the compiler writes the `x @ x` no-op **296 times** and compiles today, so it
does not *need* the form; and `pass` removes neither `x @ x` nor `continue`,
growing the surface to four no-ops.

**The checker diagnostic is vetoed on a measurement.** All **3** `while`-enclosed
`continue` arms in the compiler satisfy the proposed rule and **all 3 are
correct** — 100% false positives. To fire truly it must prove the loop makes no
progress, which is a termination analysis. And the hazard is narrower than the
brief said: **124 of 127 `=> continue` arms are in a `for`, where the same shape
terminates at exit 0** — measured.

**Adopted:** the sentence that sets the trap is **replaced**, not appended to:

> A jump (`return`, `break`, `continue`) is a valid arm body: it yields no value
> and does not constrain the `match`'s type. **An arm that does nothing is a
> block holding `_ = 0` — `continue` is not one.**

**+24**, and it answers all three seats: it names a route (the blind seat's
objection to warning-only was the *vacuum*, and this fills it), it negates the
trap in the same breath (the historian's PHP precedent — the withdrawn removal
RFC, and a warning whose text names the alternative), and it adds no construct
(both vetoes respected). The route was run before it was written down: `seen 2`,
exit 0, where the `continue` version hangs.

## The package, measured as one document

| | |
|---|---|
| baseline | **3553** |
| `w-jump4` — the no-op route | +24 |
| `q2-args2` — argv's abort, and `args_checked`'s reason | +16 |
| **`cut-header`** — the named removal | **−45** |
| **landed, composed not summed** | **3548 — five tokens BELOW where it started** |

The removal is `spec:227-228`'s *"A header shows more than ISO C's names — `M_PI`,
`strdup` and `fileno` are usually there"*. The warden is explicit that this is
**not** panel 081's or 084's shape: the sentence is true and non-redundant,
merely low-value — the only passage that is platform commentary rather than a
language rule, and the only one whose wrong guess is harmless (a reader who does
not know `M_PI` is bindable writes the digits and gets a correct program). It is
a design act the author ratifies, not a free sweep.

**Two removals the warden found and refused to spend**, which is the seat doing
more than counting: `Point(3, 4)` (−12) is the sentence that stops the single
likeliest error in this language from being *written*, and the NaN idiom (−12) is
§1.4's canonical silent bug.

## Where the seats disagreed, unsmoothed

**The blind seat wants a keyword; both compiling seats veto it.** That is not
resolved by ranking the seats — it is resolved by noticing that the blind seat's
*objection to the alternative* was that a warning leaves a vacuum, and the
adopted wording does not: it names the route. Its own stated condition for
raising the warning to approval was that readers converge on one spelling; the
sentence gives them one.

**The engineer and the ffi-pragmatist agree on Q1's substance and disagree on its
order**, and the order is the engineer's. Its own argument is turned on it once,
for the record: it calls `args()`'s abort a defect while `to_str`'s is a
"spelling", and the two are the same shape — a crashing default with a safe
sibling. The sitting treats them the same way, which is why both are answered
here.

**A brief error the engineer reported that was itself wrong**: it recorded that
the convener's Q2 costs were *lines* and re-measured them as tokens. They were
tokens, measured, and the warden confirmed all five to the digit. What the
engineer actually measured was a much longer wording of its own — 13 and 34 lines
against the convener's 1 and 2 — so both numbers are right about different text,
and the difference is the finding: **a one-sentence blanket costs +29; a real
enumeration costs +364.**

## What the sitting found that it was not convened for

- **`./heroes` in the repository root was nine hours stale** — dated 07:11,
  before panel 089 landed at 16:13, and it does not know `validated`. The warden
  nearly filed that as a finding; the coordinator reproduced it. **Any seat, or
  session, briefed with `./heroes` is testing yesterday's language.**
- **A diagnostic names a fix it then refuses.** `.b => 0` says *"write `_ = …` to
  discard it on purpose"*; `.b => _ = 0` is `declaration_in_arm`. Its second
  route — open a block — does work, so this is a wording defect and not a loop,
  and the warden says so rather than reporting the loop it first hypothesised.
- **A real C name declared under the wrong header is exit 2**, found by the
  coordinator's own sloppy test: `strstr` under `stdlib.h` is `internal error`,
  while a name no header declares is `ffi_unknown_name` at exit 1. Same author
  mistake, two verdicts. clang has **three** words for it and the mapper knew
  two.

## Predictions to score

| seat | prediction | instrument | scored at |
|---|---|---|---|
| ffi-pragmatist | retirement touches exactly 3 selfhost files and 10 `.hero` sites; ABI stays 15; `heroes measure` unchanged; ladder rungs 3–5 rebuild byte-identical with zero shims; `str.c:308` becomes unreachable and no golden reddens | net, `heroes measure`, `nm`, `grep` | M-separate-compilation |
| spec-warden | the package lands at 3561 ± spread and the spec is ≤3600 at M-generics-library | `heroes measure` + ledger | M-generics-library |
| spec-warden | with `q2-args2`, a blind reader picks `args_checked` ≥2 of 3 when the task mentions arbitrary filenames; today 0 of 3 | `docs/measurements/007` | M-ffi-ladder |
| compiler-engineer | if the checker diagnostic ships, `check selfhost/main.hero` emits ≥3 false positives at named lines | seed build + check | M-separate-compilation |
| llm-ergonomist | ≥30% of first-try programs under the old wording hang; <5% under a document that names the route | metric 2 | M-guide-book |
| historian | keeping both spellings → a call whose argument did not originate inside the compiler appears within three milestones | `grep` | M-separate-compilation |

## Author's verdict

**2026-08-25: ratified** (author instruction, *"ratifica 090"*, given the same
night the sitting closed and after reading the synthesis).

**What the yes settles.** `cstr.to_str()` is **gone**, not deprecated — and that
is the clause with the least precedent behind it, so it is named first: no
language in seven surveyed has ever removed an aborting FFI conversion. The
warden's answer is what the yes adopts — Swift kept its trap **for source
compatibility with a shipped ecosystem**, and Heroes had eleven call sites and no
users, so the precedent's *reason* does not transfer while §1.12's tie-break
does. It settles that the spec names **one** abort and not twelve, because a real
enumeration measures **+364** and because the field is unanimous that no
specification promises a complete list. It settles that **no keyword enters**:
`pass` carried two vetoes on independent grounds, and the checker diagnostic
proposed in its place fired **3 of 3 false positives** on the compiler's own
source. And it settles the package at **3548**, five tokens below where it
started.

**What the yes does not settle**, and each is named so it can be reopened
cheaply: the **`cut-header` removal** is a design act rather than a free sweep —
the warden was explicit that the deleted sentence was *true and non-redundant,
merely low-value*, and it is the one clause a reader might want back. The two
removals the warden **found and refused to spend** stay unspent. And the
predictions in the table below are unscored.

**What the sitting is worth reading for is not the resolution.** The convener
brought three proposals; two were refused outright, one of the refused wordings
**asserted something false** — a float too large to render does not abort, `inf`
prints at exit 0 — and the abort that mattered most was not on the ballot at all.
Two of the five queued questions never reached a judge, because the record
already held their answers. That filter, and the four corrections, are the
transferable part.
