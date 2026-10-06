# Panel 194, spec-warden

Started 2026-10-06 15:36:48 (`date`). Written as I go; each section carries
the clock read before it was written.

## Setup (15:36:54 to 15:37:07)

- Copy: `<scratchpad>/194-spec-warden/`, made by
  `git -C /Users/joseph/Temp/heroes/heroes-lang archive 7a26a0a6 | tar -x`.
  Seed SHA-256 prefix `fc9751a29a1ecb8a` (equal to the shared brief).
  `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`: binary
  8,364,712 bytes, `heroes 0.2.0`.
- design.md §1.6, reached by grep (`docs/design.md:253`), reads today
  (line 255-256): *must fit in 10240 tokens, measured by `claude-opus-5`
  through `POST /v1/messages/count_tokens`*. §1.2 at line 190: real cost =
  program tokens x (1 + rewrite rate). The payment rule (lines 311-314) is
  unconditional: a named removal or a registered prediction naming an
  instrument that exists today.
- `./heroes measure spec/heroes-spec.md` (offline, unpaid): real **9518**
  (claude-opus-5, pinned 2026-10-06), claude-legacy 7080, cl100k 7212,
  headroom 722, FFI floor 60, judged 9578. Equal to the shared brief.
- Read in full: the shared brief, my brief, the critic's first pass, panel 178's
  sitting and its spec-warden report, `spec/heroes-spec.md` (445 lines).

(continued below)

## Facts run before any paid call (15:39:24 to 15:39:55)

- **094 is repaired on this tree.** 178's `cinit/` reproducer copied to
  `194-spec-warden/w/z3/`: `heroes check cinit.hero` 0, `heroes run` 0,
  prints `2`.
- **A brace list binds silently as the wrong struct (Z3's limit, re-run).**
  `w/z3/wrong.hero`: `#define SZ_INIT {7, 9}` written for `struct sz`, bound
  as `constant SZ_INIT: Pt` (`struct pt`): `check` 0, `run` 0, prints `9`.
  `w/z3/mutexcond.hero`: Darwin's `PTHREAD_COND_INITIALIZER` bound as a
  `Mutex` constant: `check` 0, `run` 0, prints `1018212795` then lock `22`.
  Spec § 13 line 348 says *clang checks every result type, constant and record
  field against that header*. That holds for a brace list in the same sense it
  holds for a scalar constant (an inference, no scalar case run): clang
  checks that the value fits the declared type, never which one was meant. So Z3 needs **no spec
  sentence**: the limit is the same as every constant's. Its home is the repair's
  record, not the prompt. Priced at **0**, with no paid run.
- **The `ffi_field_type` note does not name `i8`** (K's zero-token route,
  178's point 5 second half, unlanded). `w/k/u8.hero`: `sysname: u8[256]`
  against Darwin's `char[256]`: `check` 0, `run` 1, note *every field is at
  the header's own width and sign: correct `sysname`, or name the header that
  spells `Utsname` this way*. The note names neither `char` nor `i8`. With
  `i8[256]` (`w/k/i8.hero`): `run` 0, prints `0`.
- **Why `i8` is right on every leg**: `-fsigned-char` is in `flags()`
  (`selfhost/cli/flags.hero:108`, panel 161). Its comment says *§ 13's sentence
  stays true to the letter ... No spec token is spent*. So a K sentence adds
  no rule. It spells out one consequence of the width-and-sign rule, and pays
  only if it changes what readers write first time.

## Principle 0, counted (15:40:18 to 15:42:51)

- `git -C /Users/joseph/Temp/heroes/heroes-lang ls-tree -r --name-only 7a26a0a6`
  (read-only) to `w/tree.txt`: 9,329 paths, 2,684 `.hero`. Outside `tests/`,
  `docs/` and `archive/`: 562 `.hero` (120 `examples/`, 442 `selfhost/`).
- A fixed-array field there (`grep -nE '^\s+ident:\s*Type(\[N\])+'`, then a
  looser `:\s*[a-z][a-z0-9]*\[[0-9_]+\]`): **one**, `examples/raylib/main.hero:101`
  `params: i32[4]`. **Longer than 8: 0.** So no construction of such a record
  exists outside the three excluded trees, and 178's lapse condition
  (*fewer than 3*) reads **0**.
- Under `tests/` (1,388 `.hero`): 61 fixed-array field declarations, three
  longer than 8, all under `tests/golden/`: `u8[16]`
  (`run/fixedbugs-151-...`), `i8[18]` twice (`unsupported/fixedbugs-156-...`).
  This equals the critic's 3.
- M-core-packages (row 64, `scheduled`): 16 items carry its milestone
  (`grep -l 'milestone: M-core-packages' issues/*/*/*.md`). Two would build a
  census struct: the loopback HTTP server (`issues/2026-09/05/...-0000-...`,
  *`sockaddr` differs between Darwin and glibc*), and the clean-stop service
  (`issues/2026-09/10/...-0002-...`, signals). **Neither item names a struct
  with an array longer than 8.** `sockaddr_in`'s `sin_zero` is 8. Whether the
  server binds `sockaddr_un` (104 or 108) or `sigaction`'s `sigset_t` (glibc
  `unsigned long[16]`) is not written in either item. The milestone's own item
  for the count, `issues/2026-09/25/...-0005-count-which-census-structs-...`,
  is open and unrun.

### The census's record names are wrong on Linux (a find, from the script itself)

`docs/panel/178-briefs/census.sh` sets `rec` only on a **named**
`RecordDecl ... struct|union NAME definition`. An anonymous record's array fields
are therefore written under the previous named record. Lines from the
committed tsv files:

- `census-linux-arm64.tsv`: `pthread_attr_t __size char[32]`, `[48]`, `[56]`,
  `[64]`, `[8]` are five rows under one name. glibc's mutex, cond, rwlock and
  barrier types are typedef'd anonymous unions, and here they collapse into
  `pthread_attr_t`. That they are anonymous is my reading of glibc, not shown
  in the tsv.
- `timespec __fds_bits __fd_mask[16]` is glibc's anonymous `fd_set`, credited
  to `timespec`.
- `_IO_cookie_io_functions_t __val unsigned long[16]` is the anonymous
  `__sigset_t`, credited to the cookie-functions record.

The *public* columns for Linux (29 and 23) count names, so several real records
can collapse into one. Typedef-only names (`fd_set`, `pthread_mutex_t`,
`sigset_t`) are never counted as public. **The likely direction is an undercount**,
an inference from the awk and **not a recount**. An overcount is also
possible: an internal anonymous record that follows a public named one is
counted as public. So the column can move either way, and the sign is
unmeasured. It is the census seat's
to recount (the typedef name from the AST's `TypedefDecl`). I did not touch the
Linux legs.

## The drafts and the paid runs (from 15:43, clock read at each run)

What a union looks like when the construction names none of it, checked before
any text was written (15:43). `w/un/u.c` is a struct holding
`union { char a; long b; }` and `char name[16]`, built as
`(struct s){.tag = t}` and `(struct s){.tag = t, .u.a = 1}`. Apple clang 21
lowers both, at `-O0` and `-O2` (`-S -emit-llvm`), to
`llvm.memset(..., i8 0, i64 32)` followed by the named stores. They print
`0 1`: the unnamed union is zero in every byte, and a narrow named member
leaves the union's other bytes zero. This is clang's lowering, **not a C11
promise**: C11 §6.7.9p10 says the first named member, and the historian is the
one to cite it. It was measured on this Mac only.

**What it means for the text.** 178's sentence, *every field it does not name
is zero*, is **false under panel 186's union rule**. A construction that names
`u.a` leaves out `u.b`, and `u.b` holds `a`'s byte, not zero. Spec beats
compiler (CLAUDE.md § 12), so landing that sentence would make the compiler
wrong by definition, with no possible repair. The honest drafts below say
*every field sharing no byte with one it names*, which is the 186 diagnostic's
own wording (*`y`, which shares no byte with the fields it names, so C would
build it with `y` zero*). They also say a construction may then name **none**
of a union.

Drafts: `194-spec-warden/w/drafts/drafts.py` (each exact text; `--show <name>`
prints its diff). Pristine `w/pristine-spec.md`, sha256 `c7ff862e41ea8e3c`.
Offline vendored maxima (cl100k), a detector and **not a price**: base 7212,
r1-178 7241 (+29), r1-beta 7256 (+44), r1-gamma 7246 (+34), r1-delta 7247
(+35), k-char 7222 (+10), 092-b 7233 (+21), 092-a-unit 7267 (+55). Restored
after: sha256 `c7ff862e41ea8e3c`.

| # | draft | `--refresh` exit | real | Δ real | digest | clock |
|---|---|---|---|---|---|---|
| 1 | **base, the control** | 0 | **9518** | 0 | `7a1ea85e443d182b` | 15:45:14 to 15:45:15 |

Run 1 equals the pin in `selfhost/measure/pinned.hero:54-64` (9518,
`7a1ea85e443d182b`), so the instrument has not moved since it was pinned
today. `.env` was sourced from the repository; key length printed (108),
value never printed.
| 2 | r1-178: 178's sentence verbatim after *as many elements as the type says.* | 0 | 9551 | **+33** | `31fe438012275bf6` | 15:45:41 to 15:45:42 |
| 3 | r1-beta: R1 there, *every field sharing no byte with one it names is zero*, and the union sentence *exactly one, or none when it ends with `rest: zero`* | 0 | 9573 | **+55** | `40f608280e808f70` | 15:45:43 to 15:45:44 |
| 4 | r1-gamma: one sentence merged at the end of the union sentence | 0 | 9560 | **+42** | `1efb20345a20f664` | 15:45:44 to 15:45:45 |
| 5 | r1-delta: a sentence of its own after the union sentence (*it may name none*) | 0 | 9562 | **+44** | `3c6089a823b0b0fe` | 15:45:46 to 15:45:47 |
| 6 | k-char: *`i8` where it says char*, merged into the width-and-sign list | 0 | 9528 | **+10** | `036248e843d6617c` | 15:46:43 to 15:46:45 |
| 7 | 092-b: `f.ptr()` lends *a binding's field or group record*, plus *A record lent whole through `@` to C's `void *` is refused.* | 0 | 9542 | **+24** | `33ef9fb06b4e67ae` | 15:46:45 to 15:46:46 |
| 8 | 092-a-unit: lane ffi13's route-A-with-unit sentence, verbatim, after the `ptr()` sentence | 0 | 9583 | **+65** | `181626ae25d4cb3c` | 15:46:46 to 15:46:48 |

**Eight runs, the bound, all exit 0, none a lower bound.** After every
revert, `spec/heroes-spec.md` read sha256 `c7ff862e41ea...`, the pristine
digest. The raw outputs are `w/drafts/refresh-<n>-<draft>.txt`. No other paid
call was made.

- **The dearest composition stays under the ceiling.** beta + K + 092-a-unit
  is +130, so 9648 real, plus the 60 FFI floor gives 9708 against 10240.
  **No budget veto is available on any route.** That figure is a sum I did not
  measure. Additivity held in 178's table within a few tokens (R1 +55, T +61,
  R1+T merged +116), and the composition is a prediction for the landing, not
  a reading.
- **178's honest R1 costs 9 to 22 more than its verbatim sentence.** gamma is
  the cheapest honest wording (+42). beta is the dearest (+55).

### 178's predictions, scored today (each with its command)

| 178's prediction | today | verdict |
|---|---|---|
| R1's sentence reads *+32 ± 2 above the new base* (178 § What the sitting measured, *unless § 13's field sentence itself changed*) | The field sentence is unchanged since `57679005` and `517b8e25` (`git show <sha>:spec/heroes-spec.md`, `grep -A9`; 186 added the bit-field and union sentences after it). Run 2: **+33** | **HELD** |
| spec-warden's: R1 *padding too* reads 8397 | void in 178, the clause dropped | void |
| spec-warden's payment (1): no `.hero` outside `docs/` and `archive/` has a literal of more than eight zeros | `find . -name '*.hero' -not -path './docs/*' -not -path './archive/*' \| xargs grep -lE '\[(0, ){8,}0\]' \| wc -l` = **0** | holds today; scored at M-core-packages' close |
| spec-warden's payment (2): `rest: zero` in 3+ `.hero` outside `tests/` and `docs/` | **0** (all 188 files holding it are under `docs/panel/178-reports/`) | not scorable until M-core-packages closes, and R1 has not landed |
| spec-warden's condition: R1 lapses if fewer than 3 bindings construct such a struct | **0** constructions outside `tests/`, `docs/`, `archive/`; **0** of 16 M-core-packages items names a census struct (`xargs grep -l -i -E 'utsname\|sockaddr_un\|...\|flock' < w/mcp.txt`, 0 files); ROADMAP row 64 names none | **the demand is unregistered** (below) |
| compiler-engineer's: R1 at least 380 insertions, moves 3 DECIDED rows | no landing; 178's diff fails 14 of 36 hunks (critic) | not checkable today: the compiler-engineer's rebuild is the new number |
| historian's three | padding: no seat yet; the other two at M-core-packages or the landing | not checkable today |
| ergonomist's two | falsified in 178 | scored |

Also re-run on this copy: **091 repaired** (`w/l/elem_min.hero`: `check` 0,
`run` 0, prints `72`), so L costs 0 tokens and spec § 5 already rules it.

### §1.2, from 178's reader test (CARRIED, 2026-09-24, its text, not today's)

178's critic § 6 table (N = 10 per variant, every program compiled):
`u8` for C's `char` was written by **1 of 10** readers with no sentence (I) and
**6 of 10** under each R1 variant (II, III, V), **18 of 30** in all. First try on
task 3 (mutex) was **9 of 10** under I and **3 of 10** under V (R1 alone). On
task 1 it was 8 and 4. Normalised (`u8[` read as `i8[`), R1 is 10 of 10 on
task 1. Two-sided Fisher exact, computed here: `u8` I against II+III+V
**p = 0.0094**; task 3 I against V **p = 0.020**; task 1 I against V p = 0.17.

**So R1 alone, as 178 measured it, worsens the first try on the very tasks it
serves, and all of the loss is the `char` sign.** Under §1.2's own round-trip
figure (500 to 2000 tokens per correction), CARRIED program costs for a
one-field `partial` `utsname` (905 under today's language, 117 under R1, 178's
replica of the real instrument):

- no sentence: 905 + 0.2 x (500..2000) = **1005 to 1305**
- R1 alone: 117 + 0.6 x (500..2000) = **417 to 1317**, a wash at the top of
  the range
- R1 with the `u8` rate back to I's 0.1: 117 + 0.1 x (500..2000) = **167 to
  317**

That last row assumes K moves the rate, and is unmeasured. So K is what turns
R1 from a wash into a §1.2 win. It is the cheapest text this sitting priced
(+10), and it is exactly where design.md §1.4 says redundancy is spent: *where
errors actually occur*.

## Principle 0, the ruling the brief asks for (15:50)

- **The compiler's branch: no.** `selfhost/` declares no fixed-array field. The
  longest anywhere outside `tests/`, `docs/` and `archive/` is `i32[4]`
  (`examples/raylib/main.hero:101`).
- **A scheduled milestone's need counts only where an item names it, measured.**
  M-buildable-structs **cannot count as its own witness**: its row exists
  because of the form, so counting it would be circular. M-core-packages
  **names none**: 0 of its 16 items and its ROADMAP row 64. So today's demand
  is **unregistered**, not merely unmet. 178's lapse condition (*fewer than 3
  bindings*) reads 0, but 178's own warden withdrew the corpus count as the
  wrong instrument (*an empty corpus is what the status quo predicts whether or
  not the need exists*), and I hold that reading: when one construction is 905
  to 4016 tokens, absence does not measure need.
- **The thesis branch: yes, as a measured argument from Part 1 (§1.11,
  §1.12), the one 178 accepted.** It is the census plus the program-side cost.
  - The census rows this argument uses carry correct names, because they are
    **named** `RecordDecl`s: `utsname` (`char[256]` x5 on Darwin, `char[65]`
    x6 on Linux), `sockaddr_un` (104 and 108) and `sockaddr_storage` (118).
    So the census's misattribution (above) does not touch it.
  - The program-side numbers are CARRIED: 905 to 117 and 4016 to 154 real
    tokens per `utsname` construction (178's replica, 2026-09-24). This
    sitting had no paid run for programs.
  - **It admits the cheapest honest single form, and only with the
    composition §1.2 needs**: R1 with K.

## Verdict per route

| route | verdict | section | real-token cost (`claude-opus-5`, `--refresh`, this sitting unless marked) | prediction | condition |
|---|---|---|---|---|---|
| **R1**, beta wording | **approve, only composed with K in one landing** | §1.6 (payment), §1.2, Principle 0 (Part 1 argument), CLAUDE.md § 12 | **+55** (9573) | R1+K landed on this base read **9583 ± 3**, or +65 ± 3 above a moved base (sum of runs 3 and 6, unmeasured as a composition). At the landing's reader test (≥10 fresh readers per arm, every program compiled, 178's Darwin `utsname` task): under R1+K **≥ 8 of 10 correct first try and ≤ 2 of 10 write `u8`** | to **object** if that test reads `u8` ≥ 3 of 10 under R1+K, or if any leg shows a union none named, or a field sharing no byte with a named one, nonzero after `rest: zero` (MSan on the Linux legs, the critic's control) |
| R1, gamma (cheapest honest) | the conservative alternative, recorded (CL-040) | §1.6 | **+42** (9560) | as beta, −13 | to recommended if the reader test at gamma's untested position equals beta's |
| R1, delta | object (dearer than gamma, no gain I can name) | §1.6 | +44 (9562) | — | — |
| R1, 178's sentence verbatim | **object** | CLAUDE.md § 12 (spec beats compiler), §1.6 (a draft must be honest) | +33 (9551) | — | it is **false under panel 186's union rule**: naming `u.a` leaves `u.b` holding `a`'s byte. No wording that says *every field it does not name* passes |
| R1 without K | **object** | §1.2 | +55 | 178's test: `u8` 18 of 30 under R1 variants against 1 of 10 without (p = 0.0094); task 3 first try 9 to 3 of 10 (p = 0.020) | lift if a reader test shows R1 alone does not raise the `u8` rate |
| **K**: *`i8` where it says char* | **approve** | §1.4 (redundancy where errors occur), §1.2 | **+10** (9528) | as R1's row; **K alone** (the blind seat's variant C at N ≥ 10): `u8` ≤ 2 of 10 on task 1 | withdraw if K alone leaves `u8` at ≥ 3 of 10 |
| K0: the `ffi_field_type` note names `i8` when clang says `char` | **approve, lands with K** | §1.2 (the repair turn), design.md §4.17 | **0** | the note's text on `w/k/u8.hero` contains `i8` after the landing (today it names neither `char` nor `i8`) | none; it costs the prompt nothing |
| R0 (`T.zero()`) | object | §1.6, §1.2 | CARRIED +50 honest at 8361; not re-priced | `T(rest: zero)` with no field named is the whole-record zero under beta at 0 more | — |
| Z1 | object | Principle 0, §1.2 | CARRIED +55 over R1; not re-priced | — | 178's: a census struct whose zero value corrupts memory, AND Z1 gating `partial` |
| Z2 | approve | §1.6 | **0** over R1 (beta's text already says zero, never validity) | — | object to any validity warning sentence (178: +36) |
| Z3 (094 as a lowering) | **approve, 0 tokens** | §1.6; § 13 line 384 already covers it | **0** | — | 094 repaired here (`check` 0, `run` 0, prints 2). Its limit, re-run here (a brace list binds silently as the wrong struct: `check` 0, `run` 0; Darwin's cond initialiser as a mutex locks with 22), goes in the repair's record, not the prompt |
| L (091 as a lowering) | approve | spec § 5 already rules; CLAUDE.md § 12 | **0** | — | re-run here: `check` 0, `run` 0, prints 72 |
| T | wait | Principle 0, §1.2 | CARRIED +59 to +75 at 8361 | — | 178's: a reader test where writing a path fails first try ≥ half the time, or an unterminated field at exit 0 on some leg |
| A (`[x; N]`) | wait | §1.6 (a second spelling of the zero case) | CARRIED +36 honest | — | a reader test where A beats R1+K by ≥ 20 points |
| R2 | object | §1.2, §4.9 | CARRIED +41 | — | — |
| D | **veto** | Principle 0 | CARRIED +66 | — | lifted if a Heroes record outside any group is shown to need a zero default by a program in `examples/` or `selfhost/` (count today: none) |
| **092, route B with A's refusal** | **approve on budget** | §1.6; §1.12 outranks my ground (CLAUDE.md § Precedence 3) | **+24** (9542) | 092-b alone landed on this base reads 9542 ± 2 | **object** if the synthesis lets this sentence stand as closing 092: it leaves `today-typed-pointer-*` (133 on Darwin, **0, silent** on Linux) and `poll`'s record count open, and those need an open defect, not a silence |
| 092, route A with the unit (lane's text) | **object to the text** | CLAUDE.md § 12, §1.6 (the draft must be honest) | **+65** (9583) | — | its first clause, *declares `counted_by n` where C is told its extent*, is an obligation the compiler checks only where the pointee is `void` (lane: route C unbuilt, typed pointers unasked). Approve once scoped to what a prototype enforces, re-priced |
| 092, E (refusal alone) | object | §1.2 | lower bound 26 (lane, vendored) | — | it refuses the correct `today-void-count-fits` with no remedy |
| 092, D (`n: u64 sizes buf`) | no verdict: unbuilt, unpriced | Principle 0 | lower bound 31 + a `CParam` production (lane, vendored) | — | price it once built; it is the one route where the call cannot be wrong |
| **nothing** (R1) | object | Principle 0's Part 1 argument | 0 | — | becomes my verdict if the landing's reader test fails R1+K's prediction |
| nothing (092) | not mine to veto; the compiler-engineer's §1.12 | §1.12 | 0 | — | memory written past a record at exit 0 is not a price |

**Removal: nothing, and that is a problem.** I read § 13's record paragraph
again under beta and K. Every sentence still carries weight:
- *as many elements as the type says* still governs a literal that is not
  ended by `rest: zero`;
- *`partial`* still declares fewer fields;
- the union sentence is amended, not made redundant.

So the whole addition, **+65 for R1 and K** (+89 with 092-b), is paid by
registered predictions. Each names an instrument that exists today and a
milestone:
1. **The reader test at M-buildable-structs' landing** (fresh `claude -p`
   sessions, every program compiled, as this sitting's blind seat runs them).
2. **178's grep at M-core-packages' close**, re-registered. It is at risk,
   because no item of that milestone names such a struct, and that is why the
   first leg is the one that pays first.

## Findings beside the question

- **178's R1 sentence is false under panel 186** (above). Any landing that
  copies 178's text makes the compiler wrong by definition.
- **The census script names records wrong on Linux** (above). The sign of the
  error is unmeasured. It is the census seat's to recount.
- **A wrong brace list binds silently** (Z3's limit, re-run on this tree).

Finished 15:50 (`date`).
