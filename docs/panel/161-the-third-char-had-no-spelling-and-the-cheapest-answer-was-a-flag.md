# Panel 161 — C's third `char` had no spelling, and the answer was not a type

**2026-09-17/18. M-arm-platform. Full panel, five seats plus the completeness
critic.** Briefs at `docs/panel/161-briefs/`, reports at
`docs/panel/161-reports/`, all committed with this file.

## The proposal, verbatim as it went to the seats

> `spec § 13` says a field and a parameter are declared at **the header's own
> width and sign**. C has three distinct `char` types, and the sign of the plain
> one is chosen by the platform's ABI. Heroes has eight integer types and every
> one of them carries a fixed sign.
>
> **So: what does a Heroes program write to bind a header's plain `char`, such
> that ONE source file compiles on all four legs?**

Four routes were named in the brief, with the standing note that naming four is
not a claim that four is the set (CL-057): **R1** a ninth integer type whose
sign is the target's; **R2** a spelling legal only inside an `extern` group;
**R3** accept either `i8` or `u8` against a plain `char` everywhere, the written
spelling governing the value; **R4** refuse plain `char` and say so.

**Three more were found during the sitting**, which is the whole return on
naming the four as provisional: **R5**, plain `char` binds as `u8` on every
target (spec-warden and compiler-engineer, independently); **R5b/R6**, pin the
sign at the clang invocation with `-fsigned-char` (historian and
compiler-engineer, independently); **R7**, split the rule by position, arrays
one way and scalars/parameters another (the critic).

## What provoked it

Defect 058, measured on the day M-arm-platform opened. The fourth leg's full net
read **1825 passed, 3 failed**, and all three failures were one program,
`tests/golden/run/ffi-a-char-array-member.hero`, in `run`, `determinism` and
`emission`. That program has shipped since 2026-08-16 and its own comment carries
the premise the leg falsified: *"`char` is signed here"*, measured on one machine
and true on three legs of four.

## The verdict table

| seat | verdict | section | cost / delta, measured | prediction | condition or veto |
|---|---|---|---|---|---|
| **llm-ergonomist** | **veto R1; veto R2 as stated; approve R2 pinned to `u8`**; object R3, R4 | spec § 13, with § 3, § 7, § 10 | not its unit | route 3 takes first-try compile to ~100% and drops cross-sample behavioural agreement to 50-60%; route 2-unstated or R1 drops cross-**machine** agreement below 50% at a 100% compile rate — the one cell a single machine's green suite cannot see | veto lifts on R2 if the read type is named, machine-independent and written into § 13 |
| **spec-warden** | **veto R1, R2** (Principle 0 unmet); object R3 (§1.2), R4 (§1.12); **approve R5** | design.md §1.12, §1.6 reporting no breach | baseline **7984** real; R1 min **+82**, R1 full **+104**, R2 **+49**, R3 **+21**, R4 **+56**, R5 full **+47**, **R5-min +21** | with R5 landed, arm64 net 1825/3 → **1828/0**, `corpus` stays 53/0, `--refresh` reads 8005 | objection to R3 lifts if a field's correct spelling is genuinely `i8` against a plain `char`; *"none of the 39 measured fields is one"* |
| **compiler-engineer** | **veto R1, R2**; object R3, R4; **recommend R5**, record **R5b** | design.md §1.7, §1.12 deciding, Part 5 the instrument | R1: **11 `non_exhaustive` in 6 files** plus runtime rows, `int_signed` not representable without target-parameterising 25 call sites in 12 files. R3 priced as one line: **measured false, 6 clang errors, sign asserted in 8 places**. R5: **~60-90 lines across 5 emit modules**. **R5b: one string** in `cli/flags.hero` | if the resolution relaxes only the assertion and adds no cast, arm64 fails with **five** `implicit conversion changes signedness` at **exit 2**, not `ffi_field_type`; if 5b is taken the arm64 net goes 1825/3 → 1828/0 and no suite but `run`, `determinism`, `emission` moves | veto lifts on a measured program where a target-signed type makes output *more* stable across four legs |
| **ffi-pragmatist** | **approve R3; veto R4**; object R1, R2 | design.md §1.11, §4.19, `c-boundary.md` | three machines, compiled and run | SQLite ladder step 3 needs no shim under R3; arm64 net → 1828/0 with the golden **unchanged** at `i8[4]` | approval holds only if four sites move together; veto on R4 lifts on a `dirent.d_name` workaround that is neither a C shim nor the out-of-bounds `cstr` |
| **historian** (advisory, no veto) | **approve**, with one correction to the brief | precedent | — | the Windows leg will measure `char` **signed**, so the split is 2-2 by platform and never by architecture | R5b inadmissible until somebody runs AAPCS64's parameter-extension question |
| **completeness critic** (no verdict) | — | — | — | — | — |

## The disagreements, stated plainly rather than smoothed

**1. Two seats priced two different quantities and only one said so.** The
spec-warden's **+21 real** is the price of the *sentence*; the
compiler-engineer's **60-90 lines** is the price of the *change*. The critic
settled which is which by compiling the emitted C under `-funsigned-char` with
the compiler's own fifteen flags: **six errors — four `int8_t`-to-`char` at the
construction, one `char`-to-`int8_t` at the read, one static assertion.** The
one-line price is false for R3 *and for R5*, because the emitter types the array
literal's elements and the field read as `int8_t` independently of the member's C
type. **The ffi-pragmatist's scoping was falsified by its own method**: it built
its R3 evidence by hand-retyping those four temporaries, which *is* the emitter
change, and then listed four sites to move, none of which is the code that types
them.

**2. The spec-warden's own condition fired, and the population could not have
contained the counterexample.** Its objection to R3 lifts *"if a program's
correct spelling is genuinely `i8` against a plain `char`"*, and the
ffi-pragmatist measured one: `struct elf_prpsinfo.pr_nice`, a nice value of
**−20**, which reads −20 under R3 on every leg and **236** under R5's byte rule.
The 39 fields the warden tested that condition against are **39 arrays** — the
brief's own table reports 0 scalars — so it was a set with no scalars in it.
**And R5 has no way back**: `to_i8(236)` fails by spec § 11, *because the number
may not fit*, so the signed value returns only through hand-written two's
complement. R5-min at +21 is priced for text buffers and does not answer the one
real field this sitting measured.

**3. The brief's "0 plain-`char` scalar fields" was a nine-header walk wearing a
heading about the world.** The ffi-pragmatist's wider walk found four scalars in
`elf_prpsinfo` and a second parameter. The count was not wrong; the sentence over
it was, and **three seats rested on it without re-measuring** — the
compiler-engineer's scope for R5, the historian's central steer, and the
warden's condition above. The brief's own § had warned that its first walk
returned a wrong answer. It was right to warn.

**4. Two seats predicted the same net from two different trees.** Both the
warden and the ffi-pragmatist predict **1828/0**; the warden with the golden's
field changed to `u8[4]`, the ffi-pragmatist with it **unchanged** at `i8[4]`.
`determinism` and `emission` compare bytes, so they cannot both be the
resolution.

## What the critic measured that no seat had

The historian handed one probe to *"whichever seat owns the C boundary"* and
called it the single highest-value thing another seat could measure: **is the
sign of a one-byte argument observable across a translation-unit boundary?** The
ffi-pragmatist answered two neighbouring questions and not that one. The critic
ran it, `clang -O2 -S`, two triples:

| leg | the callee of `int callee(char c)` | sign observable across TUs? |
|---|---|---|
| `aarch64-unknown-linux-gnu` | `and w0, w0, #0xff` / `sxtb w0, w0` — it re-narrows | **no** |
| `arm64-apple-macos11` | `ret` alone — it trusts the caller | **yes** |

and executed it on Darwin: one callee object, two callers compiled differently,
**200 or −56 from the same machine code**.

**That resolves R5b at the position the objection was about.** The only leg where
`-fsigned-char` disagrees with the platform default is Debian arm64 — and that is
exactly the leg where the receiver re-imposes its own type in both directions. On
Darwin, x86-64 and (documented, unmeasured) Windows the flag *agrees* with the
default, so nothing diverges.

Two further facts the critic read that no report carried: **`unsignedness_of` at
`extern_field.hero:261` is evaluated inside the emitted translation unit**, so
under the flag it answers consistently by construction; and **defect 059 closes
with zero edits under R5b**, because `c_spellings.hero:59` is not wrong code but
a sentence that is false on one leg, which the flag makes true on four.

## The resolution — `provisional — author ratification pending`

**Adopted: R5b. Every clang invocation carries `-fsigned-char`, so plain `char`
is signed in every translation unit Heroes emits, on every leg.**

CLAUDE.md § 4 asks for the most robust and complete resolution and never the
cheapest, and this one happens to be both; the reasoning is robustness and the
cheapness is a consequence, not the argument.

- **It makes the existing rule TRUE rather than weakening it.** § 13's *"declared
  at the header's own width and sign"* stays exactly as written — the header's
  own sign, as the compiling translation unit sees it, becomes signed on all four
  legs. R3 would have made that sentence false; R5 would have made it false for
  one type; R1 and R2 add vocabulary to describe a divergence instead of removing
  it. **No spec token is spent, and no sentence becomes a lie.**
- **It answers the measured counterexample.** `pr_nice` reads **−20** on every
  leg, which R5 could not deliver without a bit-preserving reread the language
  does not have.
- **It keeps every guard.** `signed char`, `unsigned char`, wrong width and wrong
  length stay refused, because the assertion machinery is untouched — the flag
  changes what clang thinks `char` is, not what the compiler asks about it.
- **It closes 058 and 059 together**, which is what the milestone's gate needs,
  and 059 closes by becoming true rather than by being edited.
- **The precedent is the Linux kernel**, which took the mirror flag
  (`-funsigned-char`, 6.2-rc1, 2022-12-05) for the reason Torvalds gave:
  *"making the language rules stricter to avoid differences is a good thing."*

**What conservative would have been, recorded so the author can choose it**
(CL-040): R3, the ffi-pragmatist's approval — accept either spelling and let the
author's written type govern. It is conservative because it changes no clang
invocation and therefore cannot perturb a bound library. It was not taken because
it costs 60-90 emitter lines against one string, makes § 13's sentence false, and
carries the historian's precedent against it — proposed in Rust in 2018 and
**withdrawn by its own proposer in 25 hours**, taken by Swift anyway, whose
`CChar` doc comment has been publicly false since 2016.

**What a veto would compel.** The ergonomist's veto is on R1 and R2 and does not
reach R5b, which introduces no construct at all. The compiler-engineer's veto is
on the same two. The ffi-pragmatist's veto is on R4. **No seat vetoed R5b**, and
the historian's stated condition for admitting it has now been run. Were a veto
raised on the flag, the fallback is R3 with the compiler-engineer's full
mechanism — five emit modules, not four assertion sites.

**The one condition, and it is measured rather than promised.** The historian
cited Microsoft's own `/J` page, where pinning the sign is documented to break
ATL/MFC, and the compiler-engineer named the residual: a bound header's `static
inline` code whose behaviour depends on plain `char`'s sign, compiled into the
Heroes translation unit under the flag while the shipped library's copy was
compiled the other way. **That is run at this milestone, on the leg where the
flag disagrees with the default**, and the resolution stands or falls on it.

## Predictions to score

| # | seat | prediction | checkable at |
|---|---|---|---|
| 1 | compiler-engineer | with R5b, the arm64 net goes 1825/3 → **1828/0** and **no suite other than `run`, `determinism`, `emission` moves** | M-arm-platform close |
| 2 | compiler-engineer | a resolution that relaxes only the assertion and adds no cast fails on arm64 with **five** `implicit conversion changes signedness` at **exit 2** | M-arm-platform close, if R5b is ever reverted for R5 |
| 3 | historian | the Windows x86-64 leg measures plain `char` **SIGNED**, so the split is by platform, never by architecture | next time the author starts the Windows box |
| 4 | historian | if Heroes ever ships R1, a golden that compiles on one leg and fails on another is filed within one milestone | whichever milestone lands such a type |
| 5 | historian | if the sign is pinned at the invocation, the first breakage is in a third-party header/library pair, not in the compiler's own tests | M-arm-platform, by the corpus probe |
| 6 | spec-warden | `--refresh` reads **8005 ± 0** real if R5 lands | superseded: R5b spends no spec token, so this is **lapsed by resolution** rather than scored |
| 7 | llm-ergonomist | route 3 drops cross-sample behavioural agreement to 50-60% | M-thesis-harness, which runs Part 11's metrics |
| 8 | llm-ergonomist | **≥80% of samples asked to print `u.sysname` as text fail to produce any program** | M-thesis-harness |

## What this sitting found and did not resolve

Three things, each filed rather than absorbed:

- **There is no route from a `char[N]` field to `str`, and it is a language gap
  rather than an unwritten sentence.** The ergonomist reached it from the spec
  alone; the critic confirmed it against the compiler — `check/builtins.hero`
  refuses `to_str` of anything but an integer, a float, `bool` or `str`, and no
  `from_bytes` exists. **So the motivating program — bind `struct utsname`, print
  the name — stays blocked whichever route wins**, twice over: by this, and by
  panel 081's already-recorded 65-element construction literal. No seat had cited
  either.
- **Defect 060**, filed this sitting and reproduced by the coordinator before
  filing: a header record with a `const` member makes the compiler answer
  `internal error` at exit 2 for the author's own `extern`.
- **R7 and R9 are unpriced.** R7 splits the rule by position; R9 lets the
  `extern` group state its sign assumption once. Both are admissible and neither
  was costed, because R5b makes the question they answer not arise. If the flag
  is ever withdrawn they are where to start.

## Author's verdict

**Ratified 2026-09-18, BY DELEGATION AND NOT BY READING**, and that is written
in those words rather than crediting a reading that did not happen (CL-058,
and the same form the five sittings of 2026-09-16 used). The author's
instruction, given as part of a `/loop` on 2026-09-17: *finish the step in the
most robust way, with no open defects and no open decisions, ratifying the
panels.*

**What the yes settles.** `-fsigned-char` is in `flags()` and stays there. C's
plain `char` is signed in every translation unit this compiler emits, on every
leg. Defects 058 and 059 are closed by it and `docs/work/DECIDE.md` returns to
zero.

**What it does NOT settle, named with its trigger so nobody reads the yes as
wider than it is:**

- **R7 and R9 are unpriced**, not refused. Splitting the rule by position, and
  letting an `extern` group state its sign assumption once, are both admissible
  and neither was costed — because the flag makes the question they answer not
  arise. If the flag is ever withdrawn, they are where to start rather than the
  four routes the brief opened with.
- **The `char[N]` → `str` gap is untouched.** Whichever route had won, a program
  cannot render a bound `char` array as text: `check/builtins.hero` refuses
  `to_str` of anything but an integer, a float, `bool` or `str`, and no
  `from_bytes` exists. The motivating program — bind `struct utsname`, print the
  name — is still blocked, and by panel 081's 65-element construction literal as
  well. **This is the largest thing the sitting found and the one it did not
  resolve**, and it belongs to whichever milestone takes the array-to-text
  question.
- **Windows is unrun.** Every route here was priced against three measured legs
  and one document. The historian's prediction 3 — that Windows reads `char`
  signed, so the split is 2-2 by platform and never by architecture — is
  checkable only when the author starts the box, and the flag AGREES with
  MSVC's documented default, so a divergence there would be a finding about the
  document rather than about the flag.
- **Defect 060 is filed and not resolved by this sitting.** A header record with
  a `const` member makes the compiler answer `internal error` at exit 2 for the
  author's own `extern`. It was found beside this question, not by it.
