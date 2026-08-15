# Panel 041 — the notation gap and the unsigned boundary

**Convened** 2026-08-12, on the author's instruction (*"chiama il panel su 0x e
unsigned"*). **Lane: full panel** — the charge touches surface syntax, the type
vocabulary, a diagnostic class and the spec, so the soundness lane was never
available. **Status: RATIFIED 2026-08-12 — the author overturned both refusals;
the verdict is the last section of this file, and the objections above stand as
the judges wrote them.**

The sitting carries **two** questions by the author's own decision 1a
(`docs/debrief/DECIDE.md`, third `/decide` session): the notation gap and the
type-vocabulary gap are one reading of a C header apart, and a judge asked only
about `u64` writes `0x` in its own example programme. That pairing paid: the
llm-ergonomist, reading only a spec, found a live silent defect neither question
was about.

---

## The proposal, verbatim as the judges received it

### Q1 — hexadecimal integer literals (notation; no new type)

Spec diff, § Types, one clause:

```
+ - An `int` may be written in hexadecimal, lowercase, as exactly the 64 bits:
+   `0xff` is 255 and `0xffffffffffffffff` is `-1`. Decimal stays a value, so
+   `18446744073709551615` is still out of range.
```

**The sub-decision that matters is not the notation, it is the semantics.** A hex
literal can denote a *value* (bounded by the signed range, as decimal is — Go,
Rust, Zig) or a *bit pattern* (all 64 bits, so `0xffffffffffffffff` is `-1` —
Java JLS 3.10.1, C# `unchecked`). The bit-pattern reading closes the all-ones-mask
case **with no unsigned type at all**, and it is the same decision panel 040
already took for `1 << 63` ("performed on the unsigned bit pattern and cast back").
The value reading leaves `0xffffffffffffffff` as `int_out_of_range`, i.e. Q1 buys
readability for small masks and closes nothing else.

Sub-questions: lowercase digits only (§4.15's "exactly one spelling"), and whether
`_` separators come too or are refused where they are written, as panel 035
refused exponents.

### Q2 — unsigned integers in the type vocabulary

- **(a) nothing.** Part 7 item 10 stands ("never in v1"); `strlen` stays unbindable.
- **(b) `u64` as an FFI-boundary type only** — the shape `cstr` and `ptr` already
  have. Only in an `extern` group's signatures and constants: no literal, no
  arithmetic, no comparison. `to_int(x)` aborts above the signed range,
  `to_u64(n)` aborts below zero.
- **(c) full sized integers** (`i8 u8 i32 u64` …) with conversion rules.
- **(d) one first-class `u64`** with arithmetic and its own overflow rules.

### Measured before the sitting, on this tree

1. `x = 0xFFFF` → `error[expected_end_of_line] … found a name (xFFFF)` at col 10.
2. `x = 18446744073709551615` → `int_out_of_range`, note naming the signed range.
3. `extern constant SIZE_MAX: int` → `ffi_constant_type`, note *"correct the
   declared type"* — **and no correct type exists to write.** Same `ULONG_MAX`.
4. `extern function strlen(s: cstr) -> int` → `ffi_return_type`. **`strlen` cannot
   be bound in Heroes today**, and §1.11 makes the FFI the standard library.
5. `UINT_MAX` **is** bindable and prints `4294967295`. The boundary is exactly
   2^63, not "unsigned".
6. The four-rung ladder binds only `int`, `cstr`, `ptr`. It has no `size_t`
   anywhere — it passed by binding the functions whose signatures have none.

### Spec budget, `heroes measure`, not estimated

| variant | tokens | delta |
|---|---|---|
| today | **2675** | — |
| A: hex clause only | **2740** | **+65** |
| B: hex + `u64` boundary clause | **2820** | **+145** (the `u64` half is +80) |

---

## The verdict table

| judge | Q1 bit-pattern | Q1 value | Q2(a) | Q2(b) | Q2(c)/(d) |
|---|---|---|---|---|---|
| compiler-engineer | **object** | approve | approve | **VETO** | **VETO** |
| llm-ergonomist | adopt-cond | adopt-cond | object | adopt-cond | — |
| spec-warden | **VETO** | adopt-cond | adopt | adopt-cond | **VETO** |
| ffi-pragmatist | adopt-cond | — | — | **object** | (d) veto |
| historian (advisory) | approve, Java-only | approve | — | **object** | — |

| judge | section | cost / delta | condition |
|---|---|---|---|
| compiler-engineer | §1.7, §1.1, §4.15; Part 5 | Q1 ~120 net Rust lines, **0 in `emit/`, 0 in `printer/`**; Q2(b) ~180–240 across 12+ files, a new confinement pass, and `HERO_RUNTIME_ABI` 10→11 | bit-pattern: a diagnostic that keeps the over-`i64::MAX` class catchable. (b): make `u64` **genuinely opaque like `ptr`** — no `to_int`, no `to_u64` — which collapses it to ~35 lines |
| llm-ergonomist | — (spec only) | — | hex: **`0700` must be a compile error**. u64: say whether it may bind to a local; `to_int` on an `int` must be an error; reconcile `to_u64` with "no overloading" |
| spec-warden | §1.0, §1.6, §4.15, Part 6, Part 7 preamble | tightened wordings **+42** (Q1) and **+50** (u64), against +65/+80 as proposed; **a −39 removal exists and is named** | Q1 adopts at ≤+42 **if** the reading is value **and** the −39 lands in the same commit. (b) adopts if M-selfhost-probe reports it as a *blockage* |
| ffi-pragmatist | **§4.19** (two sentences, both against), §1.11, Part 7 item 10 | 13 C files compiled, 5 run | (b): either a **written LP64-only premise with the test that fires when it dies**, plus a `sizeof(...)==8` companion assertion; or a real binding where `int64_t`'s *bit pattern* is observably wrong |
| historian | precedent | — | one sourced instance of a compiled AOT language with a single signed int that shipped FFI-only unsigned, no arithmetic, aborting conversion, and kept it 3+ years unchallenged |

---

## What the judges found that the proposal did not know

**1. The `strlen` case and the `SIZE_MAX` case are not the same case, and no
option closes both** (compiler-engineer). `strlen(s: cstr) -> u64` then `to_int`
works — a real string length is under 2^63. `SIZE_MAX: u64` then `to_int`
**always aborts**. Today `heroes` gives a compile error with a note; under (b) it
gives a binary that traps. That is the thesis running backwards, and it is why
(b) is vetoed rather than merely doubted.

**2. The ABI was never the problem; the type check was** (ffi-pragmatist,
`e4-abi.c`, run on arm64 *and* x86-64). `int64_t`↔`size_t` round-trips
**losslessly** at all four edges in both directions, plus real `strlen`. So `u64`
buys type-checking only — and buys it badly:

- its `_Generic` **cannot be spelled with typedefs at all**: `uintptr_t` and
  `size_t` in one generic association is a *hard clang error*, not a failed
  assertion;
- `-> u64` is a **compile error on i386 and armv7**, where `size_t` is
  `unsigned int`, and Heroes has no conditional compilation. That would be the
  first `.hero` declaration whose correctness depends on host word size;
- on wasm32 `_Generic` **accepts a 32-bit `unsigned long` as `u64`** — the
  assertion cannot verify its own name, so (b) needs a second `sizeof` assert
  per extern.

**3. There is a fix at one-tenth the cost, and it is compiled** (ffi-pragmatist,
`e6-cheapest.c`, linked against the real `runtime/runtime.c`). Widening the
existing `_Generic` return assertion by **one pair of generic associations**
(`unsigned long`, `unsigned long long`, in the fundamental-type shape
`emit/externs.rs:130` already uses) makes `strlen`, `SIZE_MAX` and `CURLAUTH_ANY`
bind today: no new type, no conversions, no spec token, no ABI move, the mangled
accessor unchanged — and **6/6 wrong signatures stay compile errors**
(`e7-thesis.c`).

**4. …and the historian pre-refuted it without knowing it existed.** The two
sourced instances of option (b) are Python `ctypes` and **`dart:ffi`** — and Dart
is Heroes' exact integer model (one 64-bit signed `int`, a real C FFI, unsigned
refused). Dart shipped the marker-type shape ~7 years ago. The recorded
complaints are not "we want arithmetic", they are ***"nobody told us the value
went negative"*** ([dart-lang/sdk#46214](https://github.com/dart-lang/sdk/issues/46214),
#46498, #35757). `e6-cheapest.c` has no marker at all, so it inherits that
failure **more** exposed, not less: `print(SIZE_MAX)` would print `-1`.

**5. Panel 035's R6 refusal applies unchanged, and the proposal's escape from it
is false** (spec-warden). R6 refused exponent literals for "a new syntactic form
for a type not on the closure list, with no Part 11 evidence"; the proposal
negated the parenthetical by noting `int` *is* on the list. But **the closure list
is a list of forms, not of types** — it enumerates `record`, `variant` +
exhaustive `match`, `T?`, `@` parameters, UFCS. "`int` exists" no more carries hex
notation than "`f64` exists" carried exponents. Both operative conjuncts of R6
hold of Q1 unchanged, **and so does R6's repair**: today's
`expected_end_of_line … found a name (xFFFF)` is a trap, and a `hex_literal`
diagnostic that teaches costs **zero spec tokens**.

**6. `fmt` has no canonical answer, and §4.15 is the section that hurts**
(compiler-engineer). Both render paths echo the source slice
(`printer/fmt_expr.rs:53`, `printer/bodies.rs:169`), so `255`, `0xff`, `0x00ff`
and `0xFF` would be **four `fmt`-stable spellings of one program**, against
§4.15's "any textual difference between two versions is semantic" and Part 6's
own "two spellings for one thing". Chars already breach this (`'a'` vs `97`), so
hex is not a new class — but hex adds *case* and *leading-zero* variation **within
one notation**, which chars do not. The cheapest honest fix is to make the
notation canonical **in the lexer** — a fact about the characters in hand, CLAUDE.md
§11 — never to make `fmt` rewrite.

**7. No language enforces hex digit case, and the only precedent is a formatter**
(historian; searched Java, Go, Rust, Zig, C#, C, Julia — negative result).
rustfmt's `hex_literal_case` is the instance, and it is `rustfmt`.

**8. Two claims in the proposal were wrong and are corrected here.** C# is **not**
a bit-pattern language: `0xFF_FF_FF_FF` is `uint` 4294967295 by C's own
first-that-fits rule, and `-1` requires `unchecked((int)…)`. So the bit-pattern
reading is **Java-only** among sourced languages — though Java is the one whose
situation matches Heroes exactly. And C itself is a *third* answer (C11 6.4.4.1:
hex and decimal get **different** type lists), which is the rule that bit the
world: C90→C99 silently changed the signedness of `2147483648`.

**9. The removal exists, and it is priced** (spec-warden). Spec lines 152–153 —
*"Build a long string with `join`, not repeated `+`… each `+` copies both sides"* —
are **−39 measured**, and they are the only paragraph in 191 lines that is neither
syntax, semantics nor built-in library: they are **performance advice**, which
CLAUDE.md §13 makes "a non-goal, never a justification". −39 funds a tightened Q1
at +42 to a **net +3**. It does not fund Q2(b).

**10. `heroes mutate` measures Q1 backwards** (compiler-engineer + spec-warden,
independently). `neighbouring_digit` (`mutate/edits.rs:276-286`) returns `None` on
a trailing `a`–`f`, so `0xff` gets **no** typo-digit mutant while `0x10` does —
character-dependent, silent coverage loss in the twelfth operator. And
`mix_int_float` appends `.0`, turning `0xff` into a **lexer** error, so an operator
named for an implicit-conversion prior would silently measure the lexer instead.

**11. Part 7 item 10's reason is mis-filed, and repairing it is free and owed**
(spec-warden, and CLAUDE.md §12 now compels it). The item defers sized integers as
*"needed for any binary format"*; the live case is the **FFI**, inside v1. That is
exactly the defect panel 039 repaired in the `Macros` row. **Correcting a wrong
reason is not granting an exception.** The falsifier to name is `strlen` — and
per finding 1, `SIZE_MAX` is *not* a falsifier, because no option makes it
readable.

---

## The two live defects this sitting found, which are not its question

Both were found by the llm-ergonomist reading only a spec, and both were then
**reproduced on this tree**:

**D1 — `print(0700)` prints `700`.** A leading zero is decimal in Heroes. Every
file-permission example a model has ever read writes `0700`, and here it silently
means seven hundred: a wrong program that compiles, runs, and tests green against
itself. This exists **today, with or without hex** — but Q1 makes it worse by
raising the salience of number bases while closing the one that is dangerous.
The judge predicts ≥20% of first tries at a permissions task contain a
leading-zero literal, **in all three spec variants**.

**D2 — a `constant`'s body may compute, so `~0` is already the all-bits mask.**
`constant ALL_ONES: int` / `~0` prints `-1` today. The spec shows only `64` as a
body and never says a body may compute, so the judge did not know it was legal —
and this **weakens part of Q1's own warrant**: the all-ones mask has had a
spelling since panel 040 landed `~`.

---

## Disagreements, stated plainly

**The bit-pattern reading splits the panel, and the split is real.** The
ffi-pragmatist adopts it because it matches panel 040's `1 << 63` decision exactly
— the same bit pattern, the same cast back — and consistency inside one language
is worth more than consistency with Go. The spec-warden **vetoes** it because it
converts a compile error into a silent 16× wrong value: sixteen `f`s is `-1`,
fifteen is 1152921504606846975, and both are legal. The compiler-engineer objects
on the same ground and adds the one that decides it: `0xFFFFFFFF00000000` is a
diagnostic today and would silently become `-4294967296`. **The veto stands and
the conservative default takes the value reading** — but the ffi-pragmatist's
consistency argument is not answered by that, it is only outweighed, and the
author should know the language now has two rules for the sign bit: reachable by
`<<`, unreachable by a literal.

**The llm-ergonomist is the only judge who wanted `u64`, and that is information,
not noise.** It reads only the spec — and from the spec, the sentence *"clang
checks every signature and constant against that header, so a wrong FFI type is a
compile error"* is **false at the integer boundary**, silently. It wrote
`strlen(s: cstr) -> int` in two variants knowing it was wrong, because nothing
else could be written and nothing would complain. The four judges who can see the
repository all rejected (b) — but they rejected the *mechanism*, and none of them
disputed the gap the ergonomist measured. **The spec's own promise is the thing
that is wrong**, and no option on the menu repairs it.

**The spec-warden and the ffi-pragmatist disagree about what Part 7 item 10 owes.**
The warden says (a) plus a repaired reason. The pragmatist says design.md §4.19
already named the missing thing and it is **`c_int` and `const`** — a *C-width*
vocabulary — so `u64` is "a Heroes-width type wearing a C-width name", and
adopting (b) would spend item 10 on the wrong vocabulary. That is the sharpest
thing said about (b) and it is not a cost objection at all.

---

## Provisional resolution — the most conservative available

Adopted while the author's verdict is pending. Nothing here changes the language.

1. **Q1 bit-pattern reading: refused.** One veto (spec-warden), one objection
   (compiler-engineer), and the sourced precedent is Java alone.
2. **Q1 value reading: held, not adopted.** Every judge would take it, but the
   spec-warden's R6 consistency argument is unanswered and its adoption is
   conditional on the −39 removal landing in the same commit. Holding costs a
   mask staying decimal; adopting on a 4–1 that has not answered its own strongest
   objection costs more.
3. **What ships instead, at zero spec tokens and with no judge against it: the
   `hex_literal` diagnostic.** `x = 0xFFFF` must stop saying *"expected the end of
   the line, found a name (`xFFFF`)"* — a message about a name, for a number. This
   is panel 035 R6's shape exactly: refuse where it is written, and teach. It does
   not decide Q1; it makes the current answer honest either way.
4. **Q2 (b), (c) and (d): refused.** (b) carries a veto whose ground is measured —
   a compile error becoming a runtime trap — and three independent failures on
   non-LP64 targets. (c) and (d) carry two vetoes and Part 7 item 10's own words.
5. **Q2 (a) adopted, with the repair CLAUDE.md §12 now compels**: Part 7 item 10
   must name the program that would make it wrong, and that program is `strlen`.
   Not `SIZE_MAX` — per finding 1, no option makes `SIZE_MAX` readable.
6. **D1 (`0700`) is separated out and goes to the work list, not to the author.**
   It is a defect, not a design question, and it does not wait on Q1.
7. **`e6-cheapest.c` is recorded, not adopted.** It closes `strlen` for one pair of
   generic associations — and per finding 4 it inherits Dart's exact reported
   failure with no marker to soften it. It is the cheapest *mechanism* and the
   panel does not know that it is the right *answer*; it goes to the author with
   its counter-evidence attached.

**What a ratification would compel.** If the author takes Q1's value reading: the
−39 removal in the same commit, the lexer made canonical (lowercase, no leading
zeros) rather than `fmt`, the two decoders in `types/exprs.rs:42` and
`ir/exprs.rs:208` unified first, and `mutate/edits.rs` taught about hex digits. If
the author takes the bit-pattern reading over the veto: additionally a diagnostic
that keeps the over-`i64::MAX` class catchable, which is the compiler-engineer's
stated price for withdrawing. If the author takes `e6-cheapest.c`: a written
LP64-only premise **with the test that fires when it dies** — `e8-portable.c`
under `-target i386-linux-gnu` is that test — and design.md §4.19's two sentences
amended, because they currently book the refusal as an achievement.

---

## Predictions to score

| # | judge | prediction | checkable at |
|---|---|---|---|
| 1 | compiler-engineer | If hex lands with the value reading, its commit touches **zero** lines in `crates/heroes/src/emit/` and **zero** in `crates/heroes/src/printer/`, ≤160 net lines total. Falsified if either directory changes at all | the hex commit |
| 2 | compiler-engineer | If `u64` lands as (b), `grep -rc "Ty::U64" crates/heroes/src` exceeds **11** (today's measured `Ty::Cstr` count) and at least one new module exists for the confinement rule | M-selfhost-probe |
| 3 | spec-warden | `docs/measurements/` at M-selfhost-probe lists **≥1** blockage shaped as `f64 → bits` / hex-digit *output* (`emit/ops.rs::hex_float`) and **exactly 0** naming a hex integer literal or `u64` | M-selfhost-probe |
| 4 | spec-warden | If bit-pattern hex lands, a `typo-hex-digit` operator over `examples/` kills **0%** on both arms, and the corpus's copied-constant site count goes **0 → ≥1**, reversing measurement 005's 5→0 | first mutate run after |
| 5 | llm-ergonomist | On a task binding a libc function whose C signature mentions `size_t`: today's spec produces `int` in **≥90%** of first tries and **0%** are diagnosed. With `u64`, **≥70%** produce `u64`, and **100%** of the residual fails loudly | harness run |
| 6 | llm-ergonomist | On a permissions task, **≥20%** of first tries contain a leading-zero literal, **in every variant**. Since `0700` lexes as 700, every one is a silent wrong program | harness run |
| 7 | llm-ergonomist | Given hex, the silent-wrong-value rate on an all-bits/transcribed-constant task drops from **≥15%** to **≤2%** — and **≥25%** of today's attempts already reach for hex unprompted. **If that last figure is under 10%, the hex case weakens sharply** | harness run |
| 8 | ffi-pragmatist | Adopting (b) leaves `examples/sqlite/main.hero` at exactly **8 warnings, all `-Wincompatible-pointer-types` on `void **`, zero mentioning `size_t`** — so (b) removes none of the ladder's real warnings | next FFI rung |
| 9 | ffi-pragmatist | `extern function strlen(s: cstr) -> u64` under `-target i386-linux-gnu` raises `ffi_return_type`, while `-> int` under the widened assertion raises nothing on any of six targets | next FFI rung |
| 10 | historian | If (b) ships as written, within the next two FFI milestones a binding requires **comparing or equality-testing** a `u64` (a `*_MAX` sentinel or a flag mask) and the panel is asked to add comparison. Falsified if `u64` is used only in signatures and never inspected | +2 FFI milestones |

Prediction **7** is the one to read: it is the falsifier for Q1 as a whole. If a
model does not reach for hex unprompted, the notation is a road nobody takes and
Principle 0 says it waits.

---

## Method note

Full panel, five judges, differentiated inputs. The llm-ergonomist received three
**label-stripped** spec variants (`spec-x/y/z`) and did the tasks before being
told which was the status quo; it identified the ordering correctly *after* the
experiment, which is the sequence that makes its numbers admissible. The
spec-warden re-ran `heroes measure` on every file rather than accepting the
proposal's table, confirmed all three numbers exactly, then produced cheaper
wordings and measured those. The ffi-pragmatist compiled 13 C files and ran 5,
across six targets. The historian returned **three** corrections to the proposal's
own precedent claims and dropped four claims it could not source, which is the
role working as intended.

**The spec-warden also corrected its own standing brief on the record**: it
believed the spec ceiling was 3000; design.md §1.6 says **4096** by author
decision 2026-08-10 (panel 024). No budget veto was available on either variant
and it did not reach for one.

---

## The author's verdict, 2026-08-12 — both refusals overturned

Given the same day the panel sat, in conversation, after the synthesis was read.
**Two vetoes are overruled and one provisional hold is lifted.** CLAUDE.md §4
makes this the panel's closing act, not a new sitting: the verdict is appended,
the record is not rewritten, and the objections above stand exactly as the judges
wrote them.

**On notation — adopted in full, and widened.** *"Se sono interi sono interi,
quindi gli interi possono essere espressi sia con la notazione decimale sia con
la notazione ottale sia con la notazione binaria sia con la notazione
esadecimale, come in tutti i grossi linguaggi. Su questo io non transigerei."*

So the deliverable is not `0x` alone: it is **all four bases and the `_`
separator**. The spec-warden's R6 consistency argument is answered rather than
ignored, and the answer is the disanalogy the panel accepted too fast — **an
exponent creates values that do not otherwise exist; a base creates none.** Every
number a hexadecimal literal can write, decimal can write too. R6's two conjuncts
were "a new syntactic form" and "for a type not on the closure list"; a base is a
form for writing an existing value of an existing type, which is not what
exponents were. The historian's search stands behind it: nine languages have this
and none regrets it.

The **value** reading is kept, so the bit-pattern veto survives the ratification
— the one judge's finding that changed nothing is the one that was right.

**And octal earns its place twice**, which is the thing the panel did not see:
the sitting's own D1 defect (`print(0700)` printing `700`) has two possible
repairs, and the panel offered only the narrow one. Giving octal a real spelling
closes it *and* delivers the notation: `0o700` is 448, and the bare leading zero
becomes an error. That is Python 3's answer, Rust's, and Go's `0o` — the modern
consensus, arrived at because the bare leading zero is a trap.

**On unsigned — the veto is overruled, and the ask is bigger than the menu.**
*"Vorrei che procedessi anche con la parte di unsigned di tutti i vari tipi …
poi un sistema di tipi fatto bene, almeno quelli base … molto simile a Rust
sostanzialmente."*

That is **not option (b), and not option (c) as the panel priced it**. The panel
judged a menu whose expensive end it described in one line and refused in one
line. What is asked for is the Rust shape: `i8 i16 i32 i64` and `u8 u16 u32 u64`,
first-class, with explicit conversions and no implicit ones — which Heroes
already forbids, so half the conversion-rule cost design.md calls "the most
expensive spec item that exists" is already paid by §4.3.

The judges' objections are **not** thereby answered, and three of them survive
into the implementation as work rather than as doubt:

- the compiler-engineer's measured point that `Ty::Cstr` appears 11 times across
  9 files with **9 of them silent `_` arms** — every new `Ty` variant inherits
  that, and CLAUDE.md §11 makes each silent arm a defect;
- the ffi-pragmatist's compiled finding that `_Generic` **cannot be spelled with
  typedefs** (`size_t` and `uintptr_t` in one association is a hard clang error),
  so the FFI half must use fundamental types, and that a width-named type needs a
  `sizeof` companion assertion or it cannot verify its own name;
- the historian's Kotlin precedent, now pointing the other way: the KEEP's
  motivation **#1** for unsigned types was *hexadecimal literals that overflow
  the signed type* — which is this ratification's two halves, in the order they
  are being built.

**Consequences recorded now, so they are not rediscovered as surprises.** Part 7
item 10 says *"never in v1"* and is **retired**, not reinterpreted — this
overrides it, exactly as panel 040 overrode §4.14's own sentence, and the
milestone that retires it is the one that must say so. Panel 041's finding 1
still holds and is not fixed by this: `SIZE_MAX` becomes **readable** under a
first-class `u64`, which is the thing no option on the menu could do — so the
falsifier design.md Part 7 item 10 was given this morning (`strlen`, never
`SIZE_MAX`) was the right one for the menu and is now superseded by both.

**Two milestones, in this order** (`docs/ROADMAP.md` § The order, rows 1 and 2):
`M-literal-bases` then `M-sized-integers`. They are deliberately **not** one
milestone: decision 1a made the cheap question hostage to the expensive one this
morning and said so in its own words, and folding the notation into the type
system would be the same trap a third time.

**Predictions 1, 4, 6 and 7 become checkable early** and are scored at
`M-literal-bases` close rather than at the milestones written above. Prediction 3
is unaffected. Predictions 2, 8, 9 and 10 were written about option (b), which is
not what is being built; they are **retired unscored**, and the reason is recorded
rather than the numbers quietly dropped.


---

## Predictions scored at `M-literal-bases` close, 2026-08-12

| # | judge | outcome |
|---|---|---|
| 1 | compiler-engineer | **FALSIFIED, on one of its two halves — and it was right about the other.** *"Zero lines in `emit/`, zero in `printer/`, ≤160 net."* Measured on `944ee94`: `emit/` **0**, exactly as predicted and for the reason given — `emit/ops.rs` prints a decoded `i64`, never the source text, so §7's `INT64_C(n)` rule was satisfied for free by four new bases. `printer/` **12 lines**, so the prediction falls. What it did not foresee is what the judge itself had flagged as the problem: it wrote that four `fmt`-stable spellings of one value would breach §4.15 and that the fix should be *"canonical in the lexer, never `fmt`"*. The ratification took the other half of that advice — lexer-canonical for the **prefix**, `fmt`-canonical for the **digits** — because refusing uppercase digits would refuse the paste from a C header, which is the reason the notation exists. So the twelve lines are the judge's own objection being answered rather than ignored. Net **453**, against ≤160, and the overrun is not the notation: 296 of it is the new scanner file and 167 the decoder, the second of which exists because the judge's own precondition — two decoders had to become one — was adopted |
| 4 | spec-warden | **VACUOUS, and recorded rather than dropped.** Its antecedent is *"if bit-pattern hex lands"*, and the value reading landed instead. The `typo-hex-digit` half is superseded by what actually happened to that operator: `neighbouring_digit` was repaired at this milestone because it would otherwise have skipped any mask ending in `a`–`f`, and the corpus's copied-constant site count is unchanged at **0**, so the reversal of measurement 005's 5→0 that the judge feared did not occur |
| 6 | llm-ergonomist | **NOT SCORABLE, and the reason is the instrument.** *"≥20% of first tries at a permissions task carry a leading-zero literal."* Metric 2's harness has never run (spec-warden, this sitting), so the sample is the judge itself: **1 of 1**. That single instance is what produced D1 and it is why `0700` is now an error — but one draw does not score a rate, and pretending it does would be the panel scoring its own evidence twice |
| 7 | llm-ergonomist | **NOT SCORABLE**, same instrument. Its second clause is the falsifier for the whole notation case — *"≥25% of today's attempts reach for hex unprompted; under 10% and the case weakens sharply"* — and it is now **moot for the decision** and live only as a measurement: the author ratified the notation on a §1-derived argument (a base creates no value that did not exist, so it is not the new syntactic form panel 035 refused), not on a frequency. It stays here because a measurement that would have changed the decision is worth taking even after the decision |
| 2, 8, 9, 10 | compiler-engineer, ffi-pragmatist ×2, historian | **RETIRED UNSCORED.** All four are about option (b), the FFI-boundary `u64` that no judge who could see the repository accepted and that the ratification did not build. Retiring them is recorded here rather than done silently, because a prediction dropped without a reason is how a judge's track record gets flattered |
| 3, 5 | spec-warden, llm-ergonomist | still open, checkable at **M-selfhost-probe** and at the first harness run respectively |

## Scored at M-selfhost-probe close (2026-08-15)

- **Prediction 3 (spec-warden): CONFIRMED.** Measurement 009 lists two
  blockages of the predicted shape: the two diagnostic builders of
  `digits.rs` stay unported because rendering a boundary **in the reader's
  own base** (hex-digit output) has no spelling yet, and gap 5 records the
  missing byte-of-string conversion. Neither needed `f64 → bits`.
