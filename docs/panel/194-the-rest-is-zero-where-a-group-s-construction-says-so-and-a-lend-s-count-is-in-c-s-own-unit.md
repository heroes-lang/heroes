# Panel 194: the rest is zero where a group's construction says so, and a lend's count is in C's own unit

Panel 178 sat again on the code of 2026-10-06, by the author's instruction of
that day (*the panel should be regenerated, or at least updated, and then brought
in, so we lose nothing*), with defect 092's question beside it. A full panel:
the compiler-engineer, the ffi-pragmatist, the spec-warden, the historian, the
blind seat as nine fresh `claude -p` sessions, and the completeness critic before
and after. Briefs written 11:32 to 12:37 and repaired on the critic's first pass
(12:19 to 12:35); the tree frozen at **`7a26a0a6`** (batch 12's gated round with
lane b12-ffi13's evidence for 092, `194-evidence/`), its counts re-run there
between 15:33 and 15:35; the seats from 15:36 to 18:07; the blind sessions 15:36
to 15:40; the critic's second pass 18:08 to 18:22; this synthesis from 18:23,
every time read from `date`. Panel 178's sitting stands beside this one, its
verdict marked superseded.

## The question, verbatim from the brief

**How does a program build a C struct whose fields include long arrays, without
writing every element, and without giving up what the language guarantees?**
Row 63 of `docs/ROADMAP.md`, **M-buildable-structs**. And defect 092's: **what
must a binding say to lend a whole group record to a `void *` parameter whose
extent C is told by another argument?**

## The verdict table

| | compiler-engineer | ffi-pragmatist | spec-warden | historian | blind seat |
|---|---|---|---|---|---|
| **R1, `rest: zero`** | approve, built on panel 186's `build`-side check; **veto** built `check`-side | approve, amended: `{0}` never `{}`, a union clause, no padding promise | approve in an honest wording (+55), **only with K** in the same landing | approve, with a union clause and no padding promise | M 2 of 3 correct first try, L 1, C 1 (existence, not a rate) |
| **K, C's `char` is `i8`** (+ K0, the `ffi_field_type` note naming `i8`) | K0 approve (+13/−1, built) | approve K0 | K approve (+10), K0 approve (0 tokens) | approve | 0 of 9 wrote `u8` |
| **Z2 zero as bytes · Z3 a header's initialiser · L defect 091** | approve · approve (094 landed) · closed by 097 | approve all three | approve all three | approve Z2 and Z3 | — |
| **T, A** | wait | wait (A object) | wait | wait | — |
| **R0, Z1, R2, D, nothing** | R0 as `T(rest: zero)` only; object Z1, R2, nothing; **veto D** | object R0 as a spelling, Z1, R2, D, nothing | object R0, Z1, R2; **veto D**; object nothing | object R0, Z1, R2, D | — |
| **092** | approve C (unbuilt) and the interim U (built); object A and B as built, D, E | approve C, D as an addition; object A as built; **veto B**; veto E alone | B with A's refusal +24 on budget, not as closing 092; object A's text, E | A with C's unit, B as A's remedy, D the direction; object C′, E | — |
| **section** | §1.7, Part 5, §1.1, §1.12 | §1.11, §1.12, §4.19 | §1.6, §1.2, Principle 0 | precedent | the spec alone |

## What the sitting measured

- **R1 is sugar on today's code** (compiler-engineer, built): the parser takes
  `rest: zero` off the argument list, the checker records it, the IR keeps the
  construction, and C fills the rest with zeros in the designated initialiser
  panel 186 already emits. +241/−16 in 12 files, two new modules
  (`parse/rest_words.hero`, `check/rest_zero.hero`), no DECIDED row moved but
  `ast.hero` (558 against 550, brought to 517 by moving `depth`, `holes_depth`
  and `max_of` to `expr_kinds.hero`); `check selfhost/main.hero` +0.26%
  instructions, a cold `--emit-c` no higher than its base in three rounds; the
  compiler's own tests 1,299, 27 suites green, 0 of 1,470 programs changing their
  `check` outcome. **Built `check`-side, as panel 178 had it, it zero-fills every
  member of a union and panel 186's own check then refuses the program**
  (`w/union/none.hero`, `ffi_union_field`), the critic reproducing it with that
  build; so the veto.
- **Panel 178's sentence is false under panel 186's union rule** (spec-warden;
  the critic through the R1 compiler: `SA(kind: 1, c: 7, rest: zero)` leaves `d`
  at `3.5e-323`). The honest wording, priced on the real instrument: *End a
  construction with `rest: zero` and every field sharing no byte with one it
  names is zero; only a group's record has it*, and the union sentence amended
  to *is built naming exactly one, or none when it ends with `rest: zero`*:
  9,518 to 9,573, **+55**; K, *`i32` where C says int, `i8` where it says char,
  `u64` where it says `size_t`*, **+10**. Eight `--refresh` runs, every one exit
  0, the first a control on the unchanged base (9,518).
- **What R1 buys**, the ffi-pragmatist's bindings on three legs, `.hero` tokens
  on the vendored table (a lower bound): `utsname` 4,006 to 158; `sockaddr_un`
  836 to 501; `SHA256_CTX` 470 to 268; an EVP buffer 521 to 428; a mutex no
  gain. The emitted C compiles with no diagnostic under the emitter's flags and
  `-Wextra -Werror` on three legs; `{}` gives six errors under `-std=c11
  -pedantic-errors`, so R1 emits `{0}` or a designated field. **A route nobody
  had listed is open today**: 094's repair lets a program's own `{0}` constant
  build any group record at zero (189 tokens for `utsname`).
- **Principle 0 reads 0**: no `.hero` outside `tests/`, `docs/` and `archive/`
  builds a group record holding an array longer than 8 (spec-warden, critic).
  The census on the frozen tree: Darwin 42 public records with an array longer
  than 8, and, recounted typedef-aware by the critic, **Linux 33 and 27**, not
  the 29 and 23 the first count gave (glibc's mutex, cond, rwlock and barrier
  types were credited to the wrong names).
- **K**: panel 178's reader test measured R1 alone raising `u8` for C's `char`
  from 1 of 10 to 18 of 30 (CARRIED); this sitting's nine readers wrote `u8` in
  0 of 9 (Fisher p = 0.020 against 178's 22 of 50), so the effect did not
  reproduce, and nine readers carry existence, never a rate.
- **Zero is bytes**: panel 178's zero-validity table reproduces row for row
  (a zeroed Darwin mutex locks with 22; a spinlock blocks on x86-64 alone; a
  zeroed `SHA256_CTX` returns success with a wrong digest).
- **Whole-union zero is clang's behaviour, not C's promise**: clang zero-fills
  a union under every initialiser shape on three legs (MemorySanitizer reports
  only the control); C11 and C23 promise the rest of a union's bytes under
  neither `{}` nor `{0}`; GCC 15 stopped clearing a union under `{0}`
  (historian); Windows unrun.
- **092's unit has one answer everywhere** (historian): GCC's `access`
  attribute, clang's `__counted_by` against `__sized_by`, Checked C and glibc's
  hardened `poll` count elements of the pointee type, bytes for `void *`. In C
  on three legs (ffi-pragmatist), A's and B's byte guard stops `read` and lets
  `poll` (`nfds: 2`) and `mbstowcs` through; a unit guard stops all three. **B
  lends a 48-byte `addrinfo` as `struct pfd *`, runs at exit 0 and corrupts it**,
  where `@` refuses that mistake at build; **A as built refuses a correct
  `getsockopt(SO_LINGER)`** with no spelling left. The interim U (refuse a
  `counted_by` whose header pointee is wider than a byte) is built, refuses
  `wide` and `mbstowcs`, leaves the 17 tracked `counted_by` programs alone, and
  refuses the correct `wide4`; it is route C′ narrowed, which the ffi-pragmatist
  and the historian object to.

## Disagreements, stated plainly

**R1's place**: the compiler-engineer's veto on the `check`-side build stands on
a run the critic reproduced; every seat that approves R1 approves it with the
union clause. **K**: the spec-warden makes R1 conditional on K from 178's 50
readers; this sitting's nine did not reproduce the effect; robust takes K, which
costs 10 tokens and is true whatever the rate. **092**: the seats agree the unit
is C's and that B as built is unsound; they disagree on the interim, the
compiler-engineer's U against the objection to C′ it narrows. Robust takes C
itself, built, and no interim that refuses correct programs.

## The resolution — ratified by delegation (below)

1. **R1, `rest: zero`, built on panel 186's `build`-side judgement** (the
   compiler-engineer's route): a construction of a group record may end with
   `rest: zero`; every field sharing no byte with one it names is zero; `T(rest:
   zero)` is the whole-record zero, so R0 needs no spelling of its own; it is
   refused anywhere but a group record, `missing_fields` unchanged everywhere
   else. The emission is a designated initialiser or `{0}`, never `{}` and never
   member stores into a bare cell. **A union none of whose members is named is
   zero in all its bytes by a whole-object zero the emitted C itself performs**,
   not by clang's present behaviour, which C does not promise; the landing tests
   it under MemorySanitizer with a control on Linux and on the Windows box. No
   promise about padding. `ast.hero`'s seam as the compiler-engineer built it;
   the flag kept in `Checked`, never on the AST node or the IR (both measured
   costs).
2. **K and K0 land with R1, in one landing**: § 13 says *`i32` where C says int,
   `i8` where it says char, `u64` where it says `size_t`* (+10), and the
   `ffi_field_type` note names `i8` for a C `char` field. R1's sentence is the
   spec-warden's beta wording (+55); together about +65 on the real instrument,
   re-priced on the base the landing lands on.
3. **Z2 and Z3**: zero is bytes, never validity, and § 13 says only what its
   sentence says; a header's initialiser binds as a group constant (defect 094,
   landed). **Defect 392** (a brace list carries no type, so `PTHREAD_COND_
   INITIALIZER` bound as a mutex builds) **is ruled the binding author's, as any
   extern value's meaning is**: C cannot tell which struct a brace list was
   written for; the limit stands in `selfhost/emit/record_constant.hero:39` and
   in 392's ruling, not in the specification.
4. **L**: defect 091 closed by defect 097 (two builds, the critic's ten
   programs).
5. **T and A wait** behind panel 178's measurements; if A returns, Rust 1.89's
   `[x; _]`, a length inferred from the type, answers the author's objection
   that `[x; N]` writes a platform's length into every construction.
6. **Refused**: R2 (omitted fields zero with no mark); D (a zero for every type,
   two vetoes); Z1 (a zero claim on the record); R1 built `check`-side (veto);
   R0 as a second spelling; *nothing*.
7. **Defect 092, route C**: a lend's count is compared in C's own unit, elements
   of the header's pointee type and bytes for `void *`, taken from the header;
   an undeclared whole-record lend to `void *` is refused, naming the
   `counted_by` that would admit it, and `counted_by` accepts a count read
   through an `@` cell (A, amended as the ffi-pragmatist's condition asks); **B is
   refused** (veto); E alone is refused. The landing builds C, which no seat
   built in Heroes; **no interim U**, which refuses correct programs; the same
   route repairs the field lend's unit (filed below).
8. **Filed from the sitting** (each re-run by the critic on the frozen tree):
   the field lend checked in bytes while C counts its own unit (blocking,
   repaired by 7); a single value lent through `@` to a pointer C writes an
   array through, `SHA256_Final(@md: u8)` (systemic: what a lend of one cell
   promises when C writes more is a ruling no rule reaches); `Opaque()`, a handle
   built with no argument, told *this is a compiler bug* (blocking); a function
   with an `@` parameter used as a value, *internal error* (blocking); the
   `fixed_outside_a_group` note pointing to a dead end (adjacent).

**What the vetoes compel**: R1 not built `check`-side; no D; no B. **What
conservative would have been** (CL-040): no `rest: zero`, a program's own `{0}`
header constant through 094's repair (189 tokens for `utsname`), and 092 closed
by A's refusal alone.

**Where the blind seat fell short, the coordinator's own**: task 2's shim
declared `connect_un` without defining it, so every task-2 program stopped at
`ffi_missing_link` until the critic judged it with a defining shim; variant M
carried panel 178's verbatim sentence, which this sitting found false, so no
reader saw the honest wording or R1 with K. The landing's reader test is
registered below with those arms.

## Predictions to score

| seat | prediction | checkable at |
|---|---|---|
| compiler-engineer | R1's landing reads 200 to 330 `selfhost/` insertions, moves no DECIDED row but possibly `ast.hero`, and a cold `--emit-c` stays within 0.5% of its parent | the landing |
| spec-warden | R1 and K together read 9,583 ± 3 on this base; at the landing's reader test (at least 10 fresh readers per arm: L, R1 honest, R1 honest with K; one task holding a union; every program compiled) at least 8 of 10 correct first try and at most 2 of 10 writing `u8` under R1 with K | the landing |
| ffi-pragmatist | under an R1 compiler, its R1 bindings exit 0 with their twins' output on three legs; under a route-C compiler, `wide-units` and `a-typed-pointer-record-count` stop before C runs on three legs | the landings |

## Author's verdict

**RATIFIED by delegation, 2026-10-06**, on the author's answer at 12:49 by the
clock read then, given before this synthesis was written, meant as: *OK,
ratify* (`issues/2026-10/06/2026-10-06-1249-the-author-answers-1-2-3-m-issue-files-to-this-session-github.md`,
point 3). The author may overturn it (CLAUDE.md § 4).
