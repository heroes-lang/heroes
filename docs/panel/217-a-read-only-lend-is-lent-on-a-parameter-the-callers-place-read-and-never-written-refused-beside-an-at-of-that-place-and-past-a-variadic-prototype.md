# Panel 217: a read-only lend is `lent` on a parameter, the caller's place read and never written, refused beside an `@` of that place and past a variadic prototype

**Convened and written 2026-10-16, from 22:00**, on issue 2098 (task of M-buildable-structs), the
sitting panel 215's question 2 recommended and the author ratified (*3a*, issue 2070); the author,
leaving the session that evening, answered *convene it now* and gave the evening's sittings up to 10
USD of paid runs. **Full panel**: compiler-engineer, ffi-pragmatist, spec-warden, historian,
llm-ergonomist (three blind arms run by the coordinator), and a completeness critic twice. The seats
copied lane `lane-land215` at `f57af53da` (R2 landed); the readers read `m-g-at`, panel 215's R3.
**Resolution: provisional, author ratification pending.** Briefs: `docs/panel/217-briefs/` (the
shared brief repaired at 22:20 after the critic's first pass, which it records). Reports:
`docs/panel/217-reports/` (the five seats, `critic-briefs.md`, `critic-reports.md`, the arms'
outputs under `llm-ergonomist-work/`).

## The proposal, verbatim (the shared brief's question)

> Heroes has two kinds of parameter (`Param = [ "@" ] ident ":" Type`): a place the function may
> change, `@c`, written `@` at the call too, and a value. C's `const struct counter *` reads a place
> and changes nothing, and there is no way to write a Heroes function that takes such a record's
> place without declaring that it changes it. **Should the language have a read-only lend, and in
> which form?** F0 nothing; F1 `lent` on a parameter of a Heroes function too, and on a C
> function's parameter with no `@`; F2 no word, a by-value parameter never written passed by place;
> F3 `@` admitted on a parameter only read (the critic's correction of the brief's first F3); and the
> critic's U1 a new word, U2 a mark at the call, U3 decided by the type, U4 a lend method to C, U6
> R2's message alone.

## What was measured

- **Arm R2, the second try under panel 215's R2** (1.4139 USD): the ten readers of panel 215's
  re-run, each handed its own refused program and the compiler's messages: **10 of 10 correct**, all
  through `doubled(@c: Counter)`, the signature nine of ten had refused as a claim of a change. The
  notes: *the spec has no read-only place lend, so `@` was the only option*.
- **Arm F1, the first try under the form** (1.7700 USD; the spec-warden's draft, 10,227 real): **9 of
  10 correct**, **10 of 10 `doubled(c: Counter lent)`** with an unmarked call, **0 of 10 copying a
  lock**; r03 refused by R2 at a set-up helper storing `Counter(n: 0, rest: zero)` into the caller's
  place.
- **Arm C, the control without *without changing the counter*** (1.8363 USD): **9 of 10 correct, 10 of
  10 `lent`, unmarked**, 0 copying; r06 refused at the same set-up shape. **The three arms: 5.0202 USD
  of the author's 10.**
- **The compiler-engineer's prototype**, F1 in about +524 lines over 20 files, half in the checker,
  none in code generation, ownership or the runtime, the compiler's own emitted C byte-identical; its
  own build found and closed three holes the brief's outline kept: a nested `@` in the same call
  (`both(p, b: bump(@p))` printing 1 by value and 2 with `lent`), a writable C header lent read-only,
  a lock-holding record lent through a generic `T lent`.
- **The ffi-pragmatist**: task C's C under F1 compiles and prints `6 4 5` on Darwin and glibc; a
  header writing through the pointer is refused by clang (`discards qualifiers`) **unless the lend
  crosses as `void *`**, when it prints `206 204 5` at exit 0; **today's `cstr lent` at a variadic
  position is written by C** (defect 2102, filed and reproduced), and a counted lend there stops at
  exit 2 (defect 2103); 11 of the 188 `@` extern parameters of the buildable corpus point at `const`;
  262 parameters of ten real headers point at a `const` record.
- **The critic's second pass**: the final prototype `f1f` **fails the ffi-pragmatist's condition
  (c)**: a record lent read-only at a `...` of a header that writes builds, an immutable binding
  printing `103` at exit 0. A private copy at `...` in place of a refusal stops neither a write past
  the end (ASan sees a five-byte write into a three-byte copy, the normal build exits 0) nor applies
  to a lock-holding record, whose copy R2 forbids.
- **The spec-warden**: F1 at 10,227 real for the arm, **10,246 at landing** (+138 against 10,108),
  the function-value clause added; p15, a writable C header bound read-only, builds under the first
  prototype: a wrong value at exit 0, the check defect 411 asks of `cstr` not asked of records.
- **The historian**: thirteen languages verified; every one that passes a read-only parameter by
  place and does not refuse its alias with a mutable one has a documented failure (D's output from 0
  to 1, 2020; Nim's `s.add(s)` memory corruption, issue #14983), Rust and Hylo, which refuse it,
  report none.

## Verdicts

| seat | verdict | section | cost or delta | prediction | condition |
|---|---|---|---|---|---|
| compiler-engineer | F1 **approve** with its three refusals; F0, F2, U5 object | design.md §1.7, §4.10, §4.8, §4.17 | +524 lines over 20 files; `parse/` budget +2, `ir/flatten.hero` 1150 → 1163; timing unsettled | a landing measures 450–700 lines, leaves emit, ownership and the runtime untouched, moves those two limits | object if a refusal is dropped, the IR mode stays erased with no verifier rule, or a still machine reads > 2% slower |
| ffi-pragmatist | F1 **object**, approve on (a) to (d), **veto without (a) or (c)** | §1.11, §4.19, §1.12 | 11 of 188 corpus `@` parameters; 262 real `const` record parameters | without (c) the prototype prints `103` at exit 0 over a writing variadic header | (a) typed `const T *`, never `void *`; (b) the read-only question asked of records; (c) a read-only lend past a variadic prototype refused; (d) aliasing with an `@` refused |
| spec-warden | F1 **approve, provisional**; U6 with whichever lands; U1, U2, U3, U4 object | §1.6, §1.2, §1.3, Principle 0, §1.12 | 10,108 → 10,227 (arm), **10,246** at landing | arm F1 ≥ 8 build, ≥ 8 `lent`, ≤ 1 copy; landing exactly 10,246 | veto on a miss of the first two or on landing without p15's check |
| historian (advisory) | F1 **approve** on the alias refusal and the C half settled; U1 equal to F1; F0, U5 object | precedent | — | arm F1 ≥ 7 unmarked `lent`, ≤ 2 copy | — |
| llm-ergonomist (blind, three arms) | **approve F1**: arm F1 and the control 9 of 10 first try, 10 of 10 honest signature; F0 measured: correct only on the second try, through a false signature | the thesis | 5.0202 USD | (scored below) | — |

## Disagreements, stated plainly

- **Whether F1 as prototyped is sound.** The compiler-engineer approved on `f1f`; the critic built
  `f1f` and found condition (c) failing. **Measurement decides it**: F1 lands with (c), and the
  ffi-pragmatist's veto line is the resolution's own text.
- **(c) or a private copy at `...`.** (c) refuses with a message, keeps `printf`-style bindings of a
  string buildable through a lease (three lines a call), breaks no buildable binding of the corpus,
  and applies to records; the copy keeps the shorter spelling but stops no write past the end and
  cannot apply to a record holding a lock. **(c).**

  **Corrected 2026-10-17**, at R3(c)'s landing as defects 2102 and 2103's repair: *breaks no
  buildable binding of the corpus* is false. The critic's pass wrote it as a reading of the
  ffi-pragmatist's census, which it did not re-run, and that census held the corpus's programs:
  `tests/golden/surface-fixtures/twoarity/as_text.hero` bound `printf(format: cstr lent, value:
  cstr lent)`, built and printed `s = forty-two`, and `suite_surface` went red on it under the
  repair. One fixture, moved to a lease (`value: cstr`, three lines in its wrapper), the program as
  it stood kept refused as `twoarity-lent/`. The coordinator's census of the tracked bindings of
  twenty common variadic C functions that night found no other `cstr lent` past a `...`, and, past
  the read-only lend this sitting ruled, one `@` lend, `run/fixedbugs-437`'s `prctl`, which defect
  2106's decision refuses (issue 2107).
- **One word or two.** F1 gives `lent` a second meaning beside *C does not keep it*. Arm F1 saw three
  readers drop `lent` from the writing C functions (21 of 30 declarations), arm C none (30 of 30): a
  reading of one arm, not a measured shift. The spec-warden objects to a new word on its price, the
  historian rates them equal. **One word**, recorded with what a second would cost below.
- **Whether a read-only lend *behaves as a copy*.** The compiler-engineer and the spec-warden said
  so; the critic measured three ways it does not (C writing through an address it kept, `11` against
  `10`; a variadic write, `3103`; a pointer C hands back with its `const` stripped, `42`). The first
  two are refused or stated by the resolution; the third is the shape issue 1292 (panel 171) wrote
  down for strings as *what the flip leaves, not promised away*, and is recorded, not closed.

## Prior art

Panel 217 (this file, `docs/panel/217-reports/historian.md`), every link below re-read 2026-10-16.

- **Reached first by Hylo**: [Functions and methods](https://docs.hylo-lang.org/language-tour/functions-and-methods): *"`let` is the default convention"*; *"`let` parameters are (notionally) passed by value, and are truly immutable"*, the compiler ensuring nothing modifies them during the call, which *"allows arguments to be passed without making any copies"*; arguments to `inout` parameters are *"marked with an ampersand (`&`) at the call site"*; and `offset_inout(&v, by: v)` is *"error: overlapping `inout` access to `v`; pass `v.copy()` as the second argument instead."*

## Resolution (provisional: author ratification pending)

**R1, the form: F1, `lent` on a parameter of a Heroes function, the caller's place read and never
written.** `function doubled(c: Counter lent)`, called `doubled(s.first)` with no mark, since nothing
changes; the callee reads `c` in place and hands it on only to another `lent` parameter or to a C
parameter declared `c: T lent` with no `@`, which a header's `const T *` gives. `Param = "@" ident ":"
Type | ident ":" Type [ "lent" ]`, so `@p: T lent` on a Heroes function is refused (the spec-warden's
p1). The spec-warden's landing draft, 10,246 real, with its function-value clause: a function with a
`lent` parameter is not a function value.

**R2, the refusals that are part of the form**, each built and measured by a seat: a `lent` argument
whose place an `@` argument of the same call reaches, nested in another argument included (Hylo's
rule, the compiler-engineer's `both(p, b: bump(@p))`); a lock-holding record lent through a generic
`T lent`; a `lent` parameter written, `@`-lent, returned or copied; and a function with a `lent`
parameter used as a value.

**R3, the C half, the ffi-pragmatist's four conditions as conditions of R1**: (a) a record lent
read-only crosses typed, `const T *`, through a probe that carries the `const`, never `void *`; (b)
the read-only question defect 411 asks of `cstr` is asked of records, its refusal the compiler's own
diagnostic on the parameter, never clang's words; **(c) a read-only lend past a header's variadic
prototype is refused**, records and `cstr` alike, the message naming the lease for a string, which
**repairs defect 2102 on its own**, and a counted lend there is told on the binding, which repairs
defect 2103; (d) as R2's aliasing refusal.

**R4, the messages**: R2's `ffi_lock_copied` note and `never_rebound`'s fix name `lent` where they
name `@c` today (U6); the store of a fresh construction into a lock-holding place, which two readers
of twenty wrote, gains a note saying to build the record where it lives.

**R5, the IR carries the read-only mode**, about ten lines with a verifier rule (the
compiler-engineer's condition), and the timing condition is measured on a still machine at the
landing: more than 2% on a cold `--emit-c` of the compiler turns the compiler-engineer to object.

**R6, where it lands.** R3(c) for `cstr` lands now, as defects 2102 and 2103's repair, in the batch
that closes M-buildable-structs. **The form, R1 to R5, is a new surface form** (the walk
`.claude/rules/diagnostics-and-goldens.md` owes it: formatter, dumps, highlighters, `heroes grammar`,
mutate, probe), and it lands on a row of its own the coordinator enters at M-buildable-structs'
close, next in the chain, its feature issue carrying this file; issue 2098, the sitting, closes with
this file. The coordinator's reason, put to the author: the milestone's deliverable is a struct a
program can build, which R2 already keeps safe, and a new form's walk is a milestone's work, not a
closing step's.

**What stays open, recorded**: C writing through an address a binding let it keep, which no compiler
check reaches (a spec sentence is owed at the landing, unpriced); a pointer C hands back with its
`const` stripped (issue 1292 records it for strings as left open; for records nothing does); the Windows box (unrun, `ssh
win` timed out twice).

**R7, the routes refused**, each with its measured reason: F0 (correct only on a second try and
through a false signature, arm R2); F2 (reaches no C parameter, so task C's copy or `@c` stays); F3
and U5 (reverse panel 210's refusal of an `@` nothing writes); U2 (a mark at the call: no reader of
twenty wrote one unprompted, and only Rust has it); U3 (by type: the binding's text stops saying
struct or pointer, §1.3); U4 (a lend method reaches C calls and not task C's helper); a private copy
at `...` (stops no write past the end, and cannot apply to a lock).

### The question to the author

1. **Where the form lands**: on a row of its own next in the chain (the coordinator's
   recommendation, R6), or inside M-buildable-structs before it closes. *Default while unanswered*:
   its own row.

### What the conservative resolution would have been, so the author can choose it

F0 with U6: no form, R2's and `never_rebound`'s messages made to agree (the spec-warden's veto stands
against any § 9 sentence saying `@` also serves a reader, which `never_rebound` contradicts). It
leaves arm R2's measurement standing: every reader correct only on its second try, through a
signature saying the function changes the counter.

## Predictions to score

| seat | prediction | checkable at |
|---|---|---|
| llm-ergonomist (panel 215) | with R2's message shown, ≥ 8 of 10 correct on task C's second try | **scored: 10 of 10, held** (arm R2) |
| llm-ergonomist (panel 215) | with a read-only lend, ≤ 2 of 10 copy the lock first try | **scored: 0 of 10, held** (arm F1; arm C 0 of 10) |
| spec-warden | arm F1: ≥ 8 build and print `6 4 5`, ≥ 8 write `doubled(c: Counter lent)`, ≤ 1 copies a lock | **scored: 9, 10, 0; held** (8 if r09's correction is not taken) |
| spec-warden | the landing measures exactly 10,246 real | the form's landing |
| historian | arm F1: ≥ 7 write the read-only lend with an unmarked call, ≤ 2 copy | **scored: 10 and 0, held** |
| compiler-engineer | a landing measures 450–700 lines, emit/ownership/runtime untouched, `parse/` +2 and `flatten.hero` +13 | the form's landing |
| ffi-pragmatist | with (c), the variadic writer is refused at exit 1, never 0 or 2; `fixedbugs-2062` rewritten with `lent` builds on both platforms; `counter_init(c: Counter lent)` refused at exit 1 | defect 2102's repair and the form's landing |

## Process notes

- **The first launch of each arm's sessions spent nothing and wrote nothing**: in `--bare` mode the
  CLI reads no stored login (*Not logged in*); the readers ran once `ANTHROPIC_API_KEY` was in the
  environment, as panel 215's had.
- **The prototype the approving seats judged failed a seat's condition**, found only because the
  critic built it again for its second pass: the condition was in one report and the build in
  another, and no seat ran the one against the other.
- **The readers' working directory names the project and the seat**, which one reader noted; panel
  215's readers ran inside the repository, whose path named it too.
- **Found beside the sitting and filed**: defects 2102 and 2103 (the variadic lends), from the
  ffi-pragmatist.
- **A stale number in a seat's own definition**: `.claude/agents/spec-warden.md` still says the
  ceiling is 10,240 (the spec-warden's report); amended with this sitting.

## Author's verdict

Pending: https://github.com/heroes-lang/heroes/issues/2105 asks for it, naming the sitting as `panel 217`.
A yes settles R1 to R7 as written and the one question above the way the author answers it.

**RATIFIED, 2026-10-17**, R1 to R7 as written above, the author answering in conversation that evening, meant as *I ratify everything*, on the coordinator's list of the five open decisions, each with its recommendation. So **the question to the author is
answered with the recommendation: the form lands on a row of its own**, M-buildable-structs having
closed that afternoon; R3(c) for `cstr` landed in its batch (defects 2102 and 2103). The row, its page
and its feature issue are owed by R6 and were not entered at M-buildable-structs' close, the sitting
then unratified; the coordinator says where they stand in the commit that records this reading.
Recorded as a reading (CLAUDE.md § 4). The author may overturn it.
