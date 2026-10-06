# Panel 178 — the rest is zero where it is written, and C says which value is valid

2026-09-24 and 2026-09-25, M-buildable-structs (row 63, `scheduled`), at HEAD
`57679005`. **Full panel**, five seats and a completeness critic. Briefs,
probes and the census: `docs/panel/178-briefs/`. Reports:
`docs/panel/178-reports/`, with each seat's work files beside its report in
`<seat>-work/`. The llm-ergonomist's, the ffi-pragmatist's, the historian's and
the critic's reports were written out by the coordinator from their final
messages, the harness having refused their files; each file's header says so.

**The author's words that bind this sitting**, given in conversation on
2026-09-24 and recorded in English as meant: *bring the most robust solution to
the panel, even if it is less economical*. They do not lift design.md §1.6's
payment rule. The sitting was convened at the end of a conversation in which
the author first asked why Heroes does not give every type a zero default, and
then questioned whether the repository was a fair sample of real C: **it was
not**, and the census that says so is the author's question answered.

**Procedure.** The trunk was not frozen: another session was landing
M-agreed-retention on it, so the sitting ran in its own worktree,
`~/Temp/heroes-lane-panel-178` on branch `lane-panel-178`, and every seat in its
own `git archive` of HEAD with a compiler built from the seed. The blind seat
read a copy of the spec and its brief outside the tree, and wrote at the end of
its report that a `CLAUDE.md` was in its context: panel 175's open question,
not new. **The critic's first run stopped on an API error** (HTTP 403,
`oauth_org_not_allowed`) and was resumed once from its transcript. **Windows is
unrun for every row**: the box was not started.

## Six errors in the coordinator's briefs, found by seats

1. **The shared brief said the third kind of array, bytes the program writes,
   had *no working route today at all, except a literal that spells every byte
   as a number*.** False, and a negative sentence written unrun. The critic ran
   the route that exists: `strlcpy` or `memcpy` into `a.sun_path.ptr()`,
   declared `ptr counted_by <n> lent` (spec § 13's field lend), writes a UTF-8
   path and `bind` and `connect` return 0 on all three legs, with no 091 repair
   and no new form.
2. **The proposal's padding clause, *and so is every byte between fields*, has
   no support in C** (the historian, from N1570 §6.2.6.1p6 and footnote 51: a
   store to a struct or a member leaves padding unspecified, and a struct
   assignment need not copy it), **is false for trailing padding** (the
   spec-warden: `struct timeval` has 4 trailing bytes, which are not *between*
   fields), **and is false through the pipeline on Linux x86-64** (the critic,
   under MemorySanitizer with a positive control: a `{char c; double d}` record
   passed by value to a Heroes function and handed to `write()` carries
   uninitialised padding at `-O0` to `-O3`; clean on Linux arm64; today's
   `partial` construction loses it the same way, because the loss is in the
   by-value call).
3. **Z1's placement, *after its name*, is refused by the parser** as
   `empty_record` (`record Utsname zero tag utsname`); the prototype put the
   word after `tag` or `partial`, where every reader of the variant put it
   (critic).
4. **The coordinator's note to the critic said the net ran with neither 091
   lowering.** False for the compiler-engineer's: its `heroes-r1` carries it,
   and `suites-r1.log` reads run 146/0, corpus 55/0, check 135/0 (critic).
5. **The brief left `cond` and `rwlock` zero-validity on Linux unrun.** The
   critic ran them: both initialisers are all zeros on both Linux legs and not
   on Darwin; `PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP` is non-zero on Linux.
6. **In conversation before the sitting the coordinator told the author that
   `zero_of` "writes the zero of a type in C"**; `/panel`'s own text calls it a
   C11 type probe never evaluated. Corrected before the briefs, which say so,
   and recorded here because the author was told the false version.

## The question

**How does a program build a C struct whose fields include long arrays, without
writing every element, and without giving up what the language guarantees?**
Today `utsname` needs 1280 literal zeros on Darwin (905 real tokens with
`partial`, 4016 with every field declared, the spec-warden's replica of the real
instrument); one construction emits 5555 lines of C (ffi-pragmatist).

## The verdict table

| | compiler-engineer | ffi-pragmatist | spec-warden | llm-ergonomist | historian |
|---|---|---|---|---|---|
| **verdict** | object to R1+Z1+T as composed; approve L and A; **veto D** | approve R1 amended, Z2, L, T on conditions; object A, Z1, R2, D; no veto as written | approve R1 (+36, merged), L, Z2, S as fallback; object R0, R2, A, Z1, T; **veto D** (Principle 0) | approve variant III (R1 + T), IV second; object II (Z1); no veto | approve R1 (not its padding clause), R0, A, Z1, T, L; object R2, Z2, D |
| **section** | §1.1, §1.7, Part 5 | §1.11, §1.12, §4.19 | §1.6, §1.2, Principle 0 | the spec alone | precedent |
| **cost** | R1 +435/−27, 351 layout lines, 3 DECIDED rows over; A +96/−22; Z1 +46/−9; L +27/−6 | `utsname` 4067 → 198 tokens (vendored); L +18/−1 | R1 +36 real; Z1 +55; T +59 to +75; D +66 | — | — |
| **prediction** | an R1 landing is at least 380 insertions and moves those 3 rows | a Z1 prototype leaves 3 silent zero failures at `check` 0 | R1 reads 8397; `rest: zero` in 3 or more `.hero` files outside `tests/` and `docs/` at M-core-packages' close | at least 7 of 20 readers write a false `zero` on the mutex | no seat cites a C paragraph keeping padding zero after a store |
| **condition** | R1 approve if the landing names its 3 rows first and moves the construction check out of `walk.hero` | Z1 approve if it also gates `partial` and is checked per platform; veto T without its terminating zero | R1 lapses if fewer than 3 bindings construct such a struct | to II if its zero-built-without-init count is 3 in 20 below III's | lift the padding objection if C guarantees it |

The completeness critic gives no verdict. It ran a 50-reader test of the
variants, the padding question under MemorySanitizer, the unlisted routes, and
the defect-094 repair.

## What the sitting measured

**R1, `rest: zero`, is sound and it is sugar.** The compiler-engineer built it:
`ir/zeros.hero` gives each field left out a zero of its own type and the IR
then holds only Part 5's existing construction, `t2 = (struct
utsname){.sysname = {0}};`. Every field kind a group record may hold reads zero
on three legs; the refusals (`rest_outside_a_group`, `rest_not_last`,
`rest_not_a_construction`, `rest_is_a_field`) exit 1; `missing_fields` still
fires without the words; `heroes fmt` is a fixpoint; none of 520 programs
changes its `check` outcome; run 146/0, corpus 55/0, own tests 676/0. It costs
+435/−27 `selfhost/` lines and moves three DECIDED layout rows (`ast.hero`
531/527, `check/walk.hero` 1921/1870, `print/fmt.hero` 1188/1175). The critic
read `suite_layout.hero`'s history: those rows have moved 6, 6 and 2 times since
2026-09-02, so naming them first is how landings go here. **And the guard R1
must not weaken has no test**: `missing_fields` appears in no golden, annotation
or unit test (compiler-engineer, confirmed by the critic).

**R1 without the padding clause is the cheapest honest text**: *End a
construction with `rest: zero` and every field it does not name is zero; only a
group's record has it.* **8393, +32 real**, digest `36a1c34f7eaceaba` (critic,
`--refresh` exit 0; the spec-warden's `padding too` version re-measured at 8397
as a check on the instrument). **The base moved after the sitting priced it**: the landing of panels 176 and 177 took the spec from 8361 to **8805** real (`heroes measure spec/heroes-spec.md` in the lane at `517b8e25`, 1435 free), and the record paragraph the sentence joins is unchanged by that diff, so the spec-warden's own rule predicts **+32 ± 2 above the new base**; unrun there, and the landing re-prices. Padding is visible only to C: Heroes' `==` and
`hash` compare fields (`selfhost/emit/structural.hero:9-14`).

**Z1 does not guard what it claims to.** Four seats and the reader test:

- the state it guards is reachable today with no mark, through `partial`: a
  zeroed Darwin mutex locks with 22 at exit 0, `check` 0, measured by three
  seats independently;
- the claim is never checked: a false `zero` on the mutex compiles
  (compiler-engineer's prototype) and no binding author can know it holds
  everywhere, because zero-validity differs between Darwin and Linux, and
  between two Linux legs on one glibc (the ffi-pragmatist's table: the
  spinlock is free at zero on arm64 and LOCKED on x86-64; a zeroed
  `pthread_barrier_t` blocks forever on one and raises SIGFPE on the other);
- **50 blind readers** (critic, every program compiled and run on Darwin):
  under variant II **0 of 10** wrote a false `zero` on the mutex, against the
  ergonomist's predicted 7 in 20, and **10 of 10** paid Z1's cost by writing the
  mutex's 56 zeros by hand; **no reader in 50** used a zeroed mutex without
  `pthread_mutex_init`.

**What zero means for a group record is Z2, measured**: all-zero bytes are a
defined value of every field type a group record holds, NULL, +0.0 and false,
on three legs (ffi-pragmatist). Whether that value is valid **for the library**
is the library's rule and clang cannot see it: a zeroed `SHA256_CTX` returns
success and a wrong digest, a zeroed `struct flock` takes a read lock on Linux.
That is the binding module's job, a constructor that calls the library's init.

**C can say which value is valid, and today it cannot be bound.** A header's
own initialiser, `PTHREAD_MUTEX_INITIALIZER`, bound as a group `constant`, is
`check` 0 and build exit 2, `internal error` (defect 094, the
compiler-engineer's find, reproduced on three legs). The critic prototyped the
repair, 19 lines: the accessor becomes `T hero_v = MACRO; return hero_v;`, own
tests 676/0, and the Darwin mutex then locks and unlocks with 0. **Its limit**:
a brace list fits any struct of a compatible shape, and `constant
PTHREAD_COND_INITIALIZER: Mutex` compiles and runs with the condition
variable's signature.

**T, a `str` into a fixed field, is not needed to write one today.** Its value
form needs `T[N]` storage inside `T[N]?`, reversing `selfhost/emit/ctype.hero:380`
(*C has no assignable array*), in three files at their ceilings
(compiler-engineer); 11 of 50 readers wrote a fixed-array local the language
refuses (`fixed_outside_a_group`), which is the same structure seen from the
reader. The place form, `s.copy_into(@a.sun_path)`, is +61 real, unbuilt and
untested. What does the job now: defect 091 repaired (L), or C writing through
the field's lend with a counted extent, which is refused at `check` when the
literal extent is overstated and aborts 134 when the run-time extent is.

**A, `[x; N]`, is sound and is a second spelling of the zero case.** Built, +96
lines, one golden moves (`;` becomes a token). The length argument does not
separate it from R1 today: both are per-platform bindings refused loudly on the
wrong platform (critic, `ffi_field_type`), and `[x; _]` would restate no N at
+32. Its 30-point raw first-try lead over R1 in the reader test **is the `char`
sign**: normalised, both are 10 of 10. The ffi seat's *GNU range needed* does
not apply, since the emitter compiles `-std=gnu11` and A emits N copies.

**The largest first-try failure is not in the question**: **22 of 50** readers
wrote C's `char` as `u8`, which is refused (`ffi_field_type`) because the emitter
passes `-fsigned-char` on every leg (`selfhost/cli/flags.hero:109`). Unpriced.

**D, a zero for every type**, is refused twice over: an all-zero `HeroStr` is
the runtime's non-value and `hero_str_len` aborts 134 on it (compiler-engineer);
a function value has no zero (§4.13); and no Heroes record appears in the
census (spec-warden). **R2**, omitted fields zero with no mark: a forgotten
`ai_socktype` returns 2 results where 1 is right on Darwin and 6 where 2 is
right on Linux, at exit 0 (ffi-pragmatist); the historian's precedents all
grew a completeness check afterwards.

## The resolution — `provisional — author ratification pending`

1. **R1 lands: `rest: zero` ends a construction of a group record**, and every
   field it does not name is zero. § 13 gains the +32 sentence above, with no
   padding clause. `T(rest: zero)` with no field named is the whole-record zero,
   so R0 needs no second spelling. The refusals are the prototype's four, plus
   `missing_fields` unchanged everywhere else. **The landing's order is a
   condition, not a preference**: a golden for `missing_fields` on a Heroes
   record and on a group record lands **first**; the three DECIDED rows are
   named before the build, and the construction check leaves `check/walk.hero`
   for its own module (the compiler-engineer's condition, which lifts its
   objection); the emission is the compound literal, never member stores into a
   bare cell (the ffi-pragmatist's veto 2, which the vetoes compel).
2. **Zero is admitted on every group record (Z2), and it is bytes, not a
   promise of validity.** § 13 says only what the sentence says. **Z1 is
   refused**: it gates a spelling while `partial` reaches the same state, its
   claim is unverifiable and platform-dependent, and it taxes the careful
   reader (0 of 10 false claims, 10 of 10 hand literals).
3. **C says which value is valid (Z3): defect 094 is repaired as a lowering**,
   so a header's initialiser binds as a group constant of the record's type.
   Its limit is written down with the repair: a brace list is untyped, so a
   compatible wrong initialiser compiles, and the binding's author names the
   right one.
4. **Defect 091 is repaired as a lowering (L)**, owed whatever else lands: the
   element store, the element lend `@o.pts[i]`, a fixed field inside a nested
   record and inside a shared `[Box]`, index-checked as the read is. The two
   independent lowerings agree on every shape the critic ran; the
   compiler-engineer's, which ran the net, is the base.
5. **C's `char` is `i8`, said once in § 13**, because it is the largest measured
   first-try failure of the sitting. Priced on the real instrument at the
   landing, and the `ffi_field_type` note for `u8` against `char` names `i8`
   if it does not already (the coordinator has not read the note's text).
6. **T waits behind its measurement**, as panel 164 queued route 6: its job is
   done at 0 tokens by L and by C writing through a counted lend. It returns,
   in the place form `s.copy_into(@field)` (+61), if either is measured: a
   reader test on the landed R1 + L in which writing a path fails first try at
   least half the time, or a run in which the loop or the lend hands C an
   unterminated field at exit 0 on some leg.
7. **A waits too**: with R1, its only case of its own is a non-zero repetition,
   which nobody measured a need for. What would move it: that need, or a reader
   test where it beats R1 by 20 points after the `char` normalisation.
8. **R2 is refused; D is refused on two vetoes** (the spec-warden's on
   Principle 0, the compiler-engineer's on §1.12); R0 as a separate spelling is
   refused.
9. **Padding is not promised.** A record passed by value loses defined padding
   on x86-64, and C can write it out; whether records should cross calls by
   address or by `memcpy` so C never sees uninitialised bytes is the
   milestone's open question, measured and not decided here.

**What the vetoes compel**: D out; an emission that never fills a bare cell by
member stores alone. **What conservative would have been** (CL-040): L alone,
no new form, 0 spec tokens. **Refused**: `utsname` stays 905 to 4016 real
tokens per construction and 5555 lines of C, the census's 23 to 42 structs a
platform stay unbuildable in practice, and the one route for text into a field
is a loop a reader must get right.

## Found in what ships

Filed with this sitting, numbers agreed with the session holding
`docs/work/DEFECTS.md` that night, each reproduced by the coordinator on all
three legs before filing:

- **091**: writing an element of a fixed-array field is `check` 0 and dies at
  run time saying it is a compiler bug (coordinator, measuring for the brief).
- **092**: a whole group record lent to a `void *` parameter with a C count has
  no bound: `read(fd, buf: @h, n: 4096)` into a 48-byte record dies at 138, 135
  and 139, *stack-buffer-overflow* under `--sanitize` (ffi-pragmatist).
- **093**: a `str` holding a zero byte, from `read_file`, reaches C through
  `.cstr()` cut at that byte, at exit 0 (ffi-pragmatist).
- **094**: a group constant whose header value is a struct initialiser stops the
  build with `internal error` (compiler-engineer).

And, to the milestone's file rather than the defect list: the ffi-pragmatist's
*glibc's `pthread_mutex_t` cannot be held by value* is a spelling, not a gap
(the critic: untagged, `record pthread_mutex_t partial`, it locks with 0 on
Linux arm64), but `ffi_unknown_tag`'s note sends the author to a handle, which
aborts; `m2 = m` copies a mutex, which POSIX forbids, and nothing refuses it
(unrun); and the spec-warden's instrument gap, `spec/rejected` passing a code
span that holds a character the lexer refuses, which matters only if A returns.

The work files kept beside the reports include shell and Python scripts:
`census.sh`, `initializers.sh`, the spec-warden's replica of the real
instrument. They are the record of how a number was taken, as
`docs/panel/173-briefs/linux_run.sh` is, and not a capability of the project
(CLAUDE.md § 10).

## Predictions to score

| seat | prediction | checkable at |
|---|---|---|
| compiler-engineer | an R1 landing is at least 380 `selfhost/` insertions and moves the DECIDED rows of `ast.hero`, `check/walk.hero` and `print/fmt.hero` | the landing |
| ffi-pragmatist | a Z1 prototype leaves the zero `SHA256_CTX`, the zero `flock` and the zero Darwin mutex at `check` 0 — **not scored**: Z1 is refused, and no seat ran the three under a Z1 compiler | — |
| spec-warden | R1 with `padding too` reads 8397 — **void as stated**, the adopted text drops the clause and the critic measured it at **8393**; the registered payment: at M-core-packages' close, `rest: zero` in 3 or more `.hero` files outside `tests/` and `docs/`, and no literal of more than eight zeros | the landing; M-core-packages |
| llm-ergonomist | at least 7 of 20 fresh readers write a false `zero` on the mutex under II — **FALSIFIED in the sitting**, 0 of 10 (critic); task 1 under I at most half of III or IV — **FALSIFIED**, 8 against 4 and 7 | scored here |
| historian | no seat cites a C11 or C23 paragraph keeping padding zero after a member store — **HELD** in the sitting; the first `sockaddr_un` binding of M-core-packages is zero-then-store; Z2 reproduces measurement 7 on the first Darwin program that zero-builds a pthread record | scored here; M-core-packages; the landing |

## Author's verdict

**SUPERSEDED 2026-10-06.** This sitting stood only on its branch,
`lane-panel-178`, from 2026-09-25 until the branch was merged on 2026-10-06,
and its ratification was in no list of the trunk. The author's instruction of
2026-10-06, meant as: *I agree with your proposal, but there have been a
thousand changes since that sitting, so the panel should perhaps be
regenerated, or at least updated, and then you can bring it in, so we lose
nothing.* So it is sat again on the trunk of that day as panel 194, and its
`panel 178` item is closed as superseded
(`issues/2026-09/25/2026-09-25-1106-panel-178-ratify-the-rest-is-zero-where-it-is-written.md`).
What this sitting measured stays its record, and so does what this section
read until 2026-10-06:

*Pending: `docs/work/DECIDE.md` carries this sitting as `panel 178`. Work
proceeds on the provisional resolution: defects 091 to 094 are filed, and the
landing is M-buildable-structs' when its row opens.*
