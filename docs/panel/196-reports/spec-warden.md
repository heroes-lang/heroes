# Panel 196, spec-warden: report (written as the sitting runs)

**In one paragraph.** The frozen spec reads **9583 real** (`claude-opus-5`,
`--refresh`, run 1), 7266 vendored; design.md §1.6's 10240 less the FFI floor
of 60 leaves **596 spendable**, and no route or composition comes near it, so
the budget vetoes nothing and Principle 0 decides. Priced on the real
instrument: **S7 +79, S2 +17, S6 +67**; the rest are vendored lower bounds.
The blind seat's nine programs, which I built and ran on the frozen compiler,
contain **the fault 0 times in 9**, and **C written by hand in 5 of the 6 buffer
programs**: what readers pay today is design.md §1.12's cost, not 396's.
Verdicts: **U approve; S7 approve, provisional on its prototype and guard; S2
approve (one rule with S7); S5 approve, provisional; S6 object beyond U; S4
object standalone; nothing object; S1 veto; S3 widened veto, narrow object.**
Panel 194's registered prediction for lane bs (9583 ± 3) scored a hit at 9583.

Copy: `<scratchpad>/196-spec-warden/`, from `git archive 39935f7c`, compiler built
from its seed at 20:56 by `date`. Nothing in the repository or any worktree edited.

## 0. The ceiling, grepped today (20:56)

design.md §1.6, line 255 of the frozen tree: *must fit in 10240 tokens, measured
by `claude-opus-5` through `POST /v1/messages/count_tokens`*. The payment rule is
unconditional (§1.6, lines 305-314): every addition owes a named removal or a
registered prediction naming an instrument that exists today.

## 1. The frozen spec's count

**Paid run 1 of 4, 20:57:35 to 20:57:37 by `date`**: `.env` sourced from the
trunk, `${#ANTHROPIC_API_KEY}` printed 108; `heroes measure spec/heroes-spec.md
--refresh` in my copy, exit 0:

| instrument | count | source |
|---|---|---|
| **real, `claude-opus-5` count_tokens** | **9583** | run 1, digest `3bb126c03750e011` |
| vendored `cl100k_base` | 7266 | `heroes measure`, offline, same file |
| vendored `claude-legacy` | 7135 | same |
| recorded real on the trunk's pin | 9518 | `selfhost/measure/pinned.hero:54`, digest `7a1ea85e443d182b`, vendored 7212 (`suite_spec.hero:93`) |

So the document moved **+65 real** (+54 on cl100k) since the pin, and the pin is
STALE on the frozen tree as the shared brief says. One commit moved it: `git log
7001dfb3..39935f7c -- spec/heroes-spec.md` names only lane bs's `54c5f618`, so
lane unit added no sentence (the critic's item 1, re-run). The
frozen file's SHA-256 begins `d368659c51940305`.

## 4 (answered first, it frames the rest). The headroom

The harness goes red at `REAL_TOKENS + FFI_FLOOR >= CEILING`
(`tests/harness/suite_spec.hero:405`), `FFI_FLOOR` 60 (`:258`, panel 030 R3),
`CEILING` 10240 (`selfhost/measure/judged.hero:56`). On the frozen tree:
10240 - 60 - 9583 = **597**, so the document may grow by at most **596 real
tokens** before the budget check is red. Every route below is priced against
that, and against the payment rule, which no headroom relaxes.

**A registered prediction scored by run 1.** Panel 194's spec-warden registered,
for lane bs's composition (R1 beta + K), *9583 ± 3 on the base's 9518*
(commit `54c5f618`'s body). Run 1 reads **9583**: a hit, at zero error. The two
disjoint § 13 edits 194 priced apart on the real instrument (beta +55, k-char
+10) summed to the landed count (+65) exactly, which is the evidence the
composition estimates below lean on.

## 2. The drafts (21:05)

Exact text: `<scratchpad>/196-spec-warden/w/drafts.py` (`--show <name>` prints
each diff; anchors asserted to match once). Pristine copy `w/pristine-spec.md`,
SHA-256 `d368659c...`. **U** is the ruling every route carries, merged into the
sentence that already defines the out-parameter (panel 122: merging beats
appending): *"... and what it points at is ONE element, held to the same width
and sign ..."*. Every route's draft is U plus its own text, so a route's own
cost is its draft minus U.

| draft | what it adds | legacy (vendored) | cl100k (vendored) |
|---|---|---|---|
| U | *ONE element*, merged | +4 | +3 |
| S6 | U, then after the fence: *C writing more than one through an `@` parameter overwrites what lies beside it, so a buffer C fills is a group record holding it, lent whole to a function your own header declares taking its struct.* | +50 | +49 |
| S1 | U, then: *Where C writes several and no argument says how many, declare the fixed array it fills, `@md: u8[32]`, and lend a group record's field of exactly that type: `@d.b`.* | +53 | +51 |
| S2 | U, and the `f.ptr()` sentence's *naming the sibling that gives the extent* becomes *naming the sibling C reads the extent from or a constant of the group holding it* (no grammar change: a constant is an `ident`) | +14 | +13 |
| S2L | S2 with *a constant of the group or a number*, and `CParam`'s `[ "counted_by" ident ]` becomes `[ "counted_by" ( ident \| integer ) ]` | +19 | +18 |
| S7 | U, then: *Where C writes several and no argument says how many, `@md: [u8] counted_by N`, `N` a constant of the group, hands C `N` elements, the array's own then zeros, and leaves the array exactly those `N`.* | +63 | +62 |
| S3 (widened) | the out-parameter sentence says *`@n: u64 counted_by 1` where it says `size_t *`; a number or record lent to a pointer without its count is refused*, plus S2L's sentence and grammar | +34 | +34 |
| S5 | U, and *and refused where the header says C writes more* | +14 | +13 |
| S4 | U, then: *C writing past an `@` cell aborts when the call returns, where it wrote within the cell's guard; further than that is not seen.* | +37 | +36 |
| vA | the coordinator's blind variant A, verbatim | +58 | +58 |
| vB | the coordinator's blind variant B, verbatim | +37 | +38 |

These are the vendored detector, **not prices** (`.claude/rules/spec-shape.md`
§ How a change to the document is made). Runs 2 to 4 price S7, S2 and S6 on the
real instrument; the rest stay lower bounds, marked so wherever they appear.

## 3. The real prices (paid runs 2 to 4, 21:04:57 to 21:05:03 by `date`)

Each draft copied over `spec/heroes-spec.md` in my copy, `heroes measure
spec/heroes-spec.md --refresh`, then the pristine file restored and its SHA-256
read back as `d368659c51940305` after every run. Raw output:
`<scratchpad>/196-spec-warden/w/refresh-<n>-<draft>.txt`. **Four paid runs, the
bound, all exit 0. No other paid call was made.**

| run | draft | real (`claude-opus-5`) | delta real | digest | vendored cl100k / legacy delta | real / cl100k |
|---|---|---|---|---|---|---|
| 1 | frozen | **9583** | 0 | `3bb126c03750e011` | 0 / 0 | |
| 2 | **S7** (U + its sentence) | 9662 | **+79** | `570bc6dc3a991bc7` | +62 / +63 | 1.27 |
| 3 | **S2** (U + the `ptr()` sentence) | 9600 | **+17** | `3247787e6c59fee5` | +13 / +14 | 1.31 |
| 4 | **S6** (U + its sentence) | 9650 | **+67** | `78c1d7de1200f8ce` | +49 / +50 | 1.37 |

**Unpriced on the real instrument, so lower bounds** (vendored maximum), with
an estimate at the three measured ratios (1.27 to 1.37), marked *estimated*
wherever it is used: U +4 (est. +4 to +6); S1 +53 (est. +65 to +73); S2L +19
(est. +23 to +26); S3 widened +34 (est. +43 to +47); S5 +14 (est. +17 to +19);
S4 +37 (est. +46 to +51).

**Additivity, checked where it can be checked free**: S7 and S2 composed
(`S7S2`, both sentences and U once) reads cl100k +72 and legacy +73; the sum of
their deltas less U's reads 62 + 13 - 3 = 72 and 63 + 14 - 4 = 73. Exact on both
vendored tables, as 194's two drafts were exact on the real one (run 1, above).

**Against the headroom (597 to the line, 596 spendable)**: the dearest measured
route, S7, leaves 517; the dearest composition drafted, `Rec` (§ 6; vendored
cl100k +106, legacy +107) is **estimated** at +137 ± 8 real, leaving about 460. **No route and no composition
on the table reaches the ceiling, so no budget veto is available**, and the
budget decides nothing here: the payment rule and Principle 0 do.

## 3b. The blind seat's nine programs, built and run by me on the frozen compiler (21:01 to 21:03)

The nine readers had finished at 20:58:51 (`<scratchpad>/p196/blind-run.log`).
I copied each program and the headers it wrote out of `/tmp/b196-t*-*/` (read
only) into `<scratchpad>/196-spec-warden/w/blind/` and ran `heroes run
main.hero` in each, frozen compiler, this Mac:

| task | C (frozen spec) | A (S1's sentence) | B (S2's sentence) |
|---|---|---|---|
| 1, SHA-256 of `abc` | **exit 0, correct digest**; own header `sha256_abc.h` with `struct sha256_digest` and a `static inline` wrapper taking `md_len`, bound `md: ptr counted_by md_len lent` | **exit 1**, four errors: `lent_shape` (`lent` beside `@md`), `ffi_type` (`u8[32]`, the unbuilt form), `fixed_outside_a_group` twice (`u8[3]` and `u8[32]` in plain records) | **exit 1**: `expected_extent` on `counted_by 32` (the unbuilt literal); own header of two structs, no function |
| 2, `frexp` (control) | exit 0, prints 4, `@exponent: i32` | exit 0, 4, `@exp: i32` | exit 0, 4, `@exp: i32` |
| 3, host name | exit 0, `venus`; own header struct, `ptr counted_by size lent` | exit 0, `venus`; same shape | exit 0, `venus`; same shape |

What this measures, at **one reader per cell, which is thin**:

- **The fault itself was written 0 times in 9**, and 0 in the 2 buffer
  programs read off the frozen spec. Every buffer program but t1-A's (no
  header, the S1 form) went to a header struct and `ptr counted_by`. The frozen spec already leads a reader to a
  correct binding of both buffers; what it costs that reader is **C written by
  hand** (t1-C's 22-line header with a function in it), which is design.md
  §1.12's *a library Heroes cannot reach is C code the author has to keep
  writing by hand*, not a rewrite.
- **A's three other refusals survive S1 as worded.** `fixed_outside_a_group`
  (twice) and `lent_shape` are not the unbuilt form; they are the reader taking *lend a
  field of exactly that type* to mean any record's field. Under S1 the task-1
  cell goes from correct (C) to refused (A). One reader, so a direction and
  not a rate.
- **B's reader found the S2 form unprompted by any example but the sentence's
  own**, and its program is one grammar change from plausible; it still wrote
  a header of structs, as S2 requires.
- **The control holds under every variant**: 3 of 3 bind `frexp`'s `int *` as
  an unmarked `@exp: i32`. That is the program S3 widened would refuse.

## 3c. Principle 0, measured where it can be (21:09)

- **Compiler need: no route.** The compiler's own scalar lends, grepped in my
  copy: 7 signatures, 12 lends (`selfhost/main.hero:78,81`,
  `selfhost/cli/process.hero:47,60,61`, `selfhost/cli/runtime_places.hero:17`,
  `selfhost/module/reading.hero:30`); the runtime writes through them 44 times
  as `*x = ...` and indexes them 0 times (`grep` of `runtime/`). Each is ONE
  element, so U describes them and no route serves them. S3 widened would cost
  them 12 rewrites and move the seed.
- **Thesis, on the blind seat's nine programs (3b)**: the fault 396 names was
  written 0 times in 9; **C written by hand** appeared in 5 of the 6 buffer
  programs and in both correct ones read off the frozen spec (t1-C's header:
  22 lines, 211 tokens on cl100k, a vendored count of output a reader spent).
  So the effect a route can be measured to have is on design.md §1.11 and
  §1.12's axis (*a library Heroes cannot reach is C code the author has to keep
  writing by hand*), not on the fault's frequency in readers. One reader per
  cell: a direction, never a rate, and every prediction below asks n = 10.
- **S5's mechanism, re-run by me** (Apple clang on this Mac, the critic's
  redeclaration under `-Werror=array-parameter`, `w/probe/s5.c`): `pipe(int
  fds[1])` exit 1 *mismatched bound*; `uuid_generate(unsigned char out[1])`
  exit 1; `frexp(double, int e[1])` exit 0; `SHA256_Final(unsigned char md[1],
  ...)` exit 0. The headers: `math.h:435` `frexp(double, int *)`,
  `unistd.h:482` `pipe(int [2])`.
- **S7's form today** (`w/probe/s7.hero`, `@md: [u8] counted_by
  SHA256_DIGEST_LENGTH`): `check` 1, `ffi_type` on `[u8]` and
  `counted_by_shape`, whose text says *`counted_by` says how far C reads or
  writes through a `ptr`, or through a group's record lent whole with `@`*.
  That second shape, panel 194's route C, is accepted by the compiler and
  stated nowhere in the spec (the critic's item 1, confirmed by the
  diagnostic's own words): a sentence S2 or S7 writes about `counted_by`
  should name it in the same place. Drafted as `S2C` (S2 plus *a group's
  record lent whole with `@` may say it too*): vendored cl100k +28, legacy +29,
  so +15 over S2 on both; unpriced on the real instrument.

- **Does a permanent refusal stand in S7's way?** Searched design.md Part 6
  (lines 2895 to 3063) and Part 7 (3063 to 3545) for *marshal*, `[T]`, *array*,
  *FFI*, *extern*, *copy*, *fixed array*, *out-param* and `counted_by`: no row
  refuses an array copied across the boundary. A negative over that
  vocabulary, so a question rather than a premise for the historian.

## 5. Verdicts, one per route

Every real figure is `claude-opus-5` through `--refresh`, this sitting. Every
figure marked *vendored* is a lower bound and every *est.* is that lower bound
times the three measured ratios (1.27 to 1.37), never a price. None reaches the
headroom of 596, so every veto below stands on Principle 0, never the budget.

### U, the ruling: an unmarked `@` is ONE element

- `verdict`: **approve**
- `section`: design.md §1.6 (payment rule); CLAUDE.md § 12 (spec beats compiler, which needs a spec that says something)
- `spec_token_delta`: vendored cl100k +3, legacy +4; not priced alone on the real instrument, est. +4 to +6; it is inside runs 2 to 4
- `removal`: nothing; paid by the prediction below
- `needed_for_self_hosting`: no (it describes the compiler's 12 lends; it does not serve them)
- `argument`: Defect 396 is *a ruling no rule reaches*: § 13 says what width an `@` cell has and never how many. U states the promise the compiler already makes and every live lend keeps, merged into the sentence that defines the out-parameter. It adds no form, so Principle 0 is not engaged. The strongest case against is that nothing enforces it; but § 13 already states C's unenforced obligations (*No word says C frees what it is handed*), and silence leaves a reader to infer a count. Tonight's readers wrote the fault 0 of 9 times, so U is not bought for its effect on readers: it is the minimum true sentence.
- `prediction`: at the landing's reader test (at least 10 fresh `claude -p` readers per arm, `blind-run.sh`'s flags, every program built and run), the `frexp` control reads at least 9 of 10 correct first try under U (3 of 3 tonight without it), and task 1's one-cell `@md: u8` appears at most 1 of 10. Landed alone on this base: 9588 ± 2 real (estimated: vendored +3 times the measured ratios).
- `condition`: object if the control falls below 9 of 10 under U.

### S7, an emitter-owned buffer, copy in and copy out

- `verdict`: **approve, provisional on its prototype**
- `section`: design.md §1.0 (the Part 1 argument: §1.11 and §1.12), §1.6, §1.2
- `spec_token_delta`: **real +79** (9583 to 9662, run 2), vendored +62 / +63; the sibling-count form `S7s` vendored +64 / +65 (est. +81 to +83); with a guard clause `S7g` vendored +69 / +70 (est. +88 to +89)
- `removal`: nothing; paid by the prediction below
- `needed_for_self_hosting`: no
- `argument`: The readers measured tonight did not write the fault; they wrote C: 5 of 6 buffer programs carry a header, the correct `SHA256` one a function. §1.12 names that failure in its own words. S7 is the one route on the table, listed or not, after which `SHA256_Final` needs no C: the extent is owned by the code that allocates, and § 9's *copy in, copy out* already says what `@` means. Strongest case against: N is still the binding's word (SHA-512 into 32 is the same lie), and a temporary the emitter owns is safer than the program's frame only if it is guarded; unguarded, on the stack, it corrupts the same frame.
- `prediction`: at n = 10 on an S7 prototype, task 1: at least 8 of 10 readers build and print the digest with no C file written, against at least 8 of 10 writing a C file on the frozen arm. Landed verbatim on this base: 9662 real.
- `condition`: object if the temporary is not guarded (an overrun past N must abort naming the call), if the guard clause claims more than the guard does, or if the reader test reads below 8 of 10; **veto** if the sentence lands before the emitter does it (today `check` 1, `ffi_type` and `counted_by_shape`). Taking the sibling count too (`S7s`, +2 vendored) closes task 3's header as well, and I would approve that wording over the constant-only one.

### S2, `counted_by` a constant of the group

- `verdict`: **approve**, best stated once with S7 as one rule for `counted_by`
- `section`: design.md §1.3 (locality), §1.6, §1.2
- `spec_token_delta`: **real +17** (9583 to 9600, run 3), vendored +13 / +14; the literal spelling `S2L` with `CParam` moved, vendored +18 / +19 (est. +23 to +26); with route C's clause `S2C`, vendored +28 / +29 (unpriced)
- `removal`: nothing; paid by the prediction below
- `needed_for_self_hosting`: no
- `argument`: S2 turns a silent extent into a stated one where the reader looks, checked against the field lent, and saves the C function t1-C wrote only to carry a count; not the struct, so alone it moves C from code to data. The B reader reached the form from the sentence alone. My wording also closes the critic's twin in the only way a sentence can: *the sibling C reads the extent from* makes `EVP_DigestFinal_ex`'s out-count visibly the binding's lie. Strongest case against: a second meaning for `counted_by`, and beside S7 a second way to bind one buffer; stated as one rule it costs one clause.
- `prediction`: under S2 alone at n = 10, task 1: at least 8 of 10 build and print the digest with no C function written (a struct only). Landed verbatim on this base: 9600 real.
- `condition`: object to the literal spelling unless a reader test on the constant-only text shows at least 3 of 10 writing a number; object to S2 landing while S7 and S0 are both undecided, since it then closes 396 only for a reader who writes a header.

### S6, the sentence alone

- `verdict`: **object** to its second sentence (U is approved apart)
- `section`: design.md §1.6 (payment), §1.12
- `spec_token_delta`: **real +67** (9583 to 9650, run 4), vendored +49 / +50; the second sentence alone est. +63
- `removal`: nothing, and that is a problem
- `needed_for_self_hosting`: no
- `argument`: The second sentence teaches what the frozen-spec reader derived unaided: t1-C wrote exactly the header struct and function S6 describes and printed the right digest on the frozen compiler. That is 63 real tokens for no measured change in a reader, and what they buy is the spec writing down, as the language's answer, that a C library needs C written by hand, which §1.12 calls §1.11 failing. Its true half, that C writing more overwrites what lies beside it, is worth a clause beside a route that gives the reader a way out, not a sentence instead of one.
- `prediction`: at n = 10, task 1 under S6 and under the frozen spec write a C function in shares within one reader of each other, both at least 8 of 10.
- `condition`: approve if the frozen arm at n = 10 writes the one-cell fault at least 3 of 10 times and S6 brings it to at most 1.

### S1, a fixed-array out-parameter

- `verdict`: **veto**
- `section`: design.md §1.0 (Principle 0's burden unmet), §1.2
- `spec_token_delta`: unpriced on the real instrument; vendored +51 / +53 (a lower bound), est. +65 to +73
- `removal`: nothing
- `needed_for_self_hosting`: no
- `argument`: A new crossing type the compiler does not need, and the one measurement of its sentence points the wrong way: the A reader of task 1 wrote a program the frozen compiler refuses with four errors, three of them (`lent_shape`, and `fixed_outside_a_group` twice) surviving S1 as worded, where the frozen-spec reader wrote a correct one. S1's place must still be a header's struct (the critic: `ffi_unknown_tag`), so it moves C from a function to a struct, which S2 does for about a fifth of the tokens and S7 removes outright. Dominated on cost and on §1.12.
- `prediction`: at n = 10 on an S1 prototype without S0, variant A's task 1 builds and prints in at most 5 of 10.
- `condition`: lifted if an S1 plus S0 prototype (a local `u8[32]` lent) reads at least 8 of 10 task-1 readers correct with no C file, and beats S7's arm on the same tasks.

### S3, a refusal at the boundary (widened, and narrow)

- `verdict`: **veto** (widened); **object** (narrow)
- `section`: design.md §1.0, §1.2; `.claude/rules/verification.md` § Bounded discovery (*a correct program refused* is `blocking`)
- `spec_token_delta`: widened, vendored +34 / +34 (a lower bound), est. +43 to +47, plus 34 live lends rewritten, 12 of them the compiler's
- `removal`: nothing
- `needed_for_self_hosting`: no; it costs the compiler 12 rewrites and the seed
- `argument`: S3 widened makes the commonest correct binding a compile error to catch a mistake nobody made tonight: 3 of 3 control readers bound `frexp`'s `int *` as an unmarked `@exp: i32` (exit 0, prints 4, my runs), each a refused first try, a round trip §1.2 prices at 500 to 2000 tokens; the fault it catches appeared 0 of 9. It adds a spelling, `counted_by 1`, to 12 compiler lends that each write one value. §1.12 says robustness does not suspend Principle 0, and S5 catches the header-declared half with no false positive on the control. Narrow, it refuses 163's correct `char *` and misses `pipe`.
- `prediction`: under S3 widened at n = 10, the control reads at most 2 of 10 correct first try.
- `condition`: lifted if the frozen arm at n = 10 writes the one-cell fault at least 3 of 10 times on tasks 1 or 3 and S5 catches none of them.

### S5, the header's own extent

- `verdict`: **approve, provisional** (priced only on the vendored detector)
- `section`: design.md §4.19 (*clang checks every result type, constant and record field against that header*), §1.12, §1.6
- `spec_token_delta`: vendored +13 / +14 (a lower bound), est. +17 to +19; its clause over U alone vendored +10 / +10
- `removal`: nothing; paid by the prediction below
- `needed_for_self_hosting`: no
- `argument`: No new form: a refusal read off the header, which §4.19 already promises for types, extended to the extent a header states. It is the one refusal that costs the control nothing (`frexp`'s `int *` states none: exit 0 in my probe) and catches the fault where no byte pointee is involved (`pipe`'s `int [2]`, `uuid_generate`'s `unsigned char[16]`: exit 1 each). It misses `SHA256_Final`, whose header says nothing, so it complements S7 and replaces nothing. Strongest case against: a sentence wider than its check is false; *the header says* covers `_LIBC_COUNT` and `__attr_access`, which the redeclaration does not read.
- `prediction`: a prototype refuses 0 of the frozen tree's 34 live scalar lends, 1 of 1 on `critic/pipe_cell.hero`, and 0 on `md_scalar.hero` (the miss, stated).
- `condition`: object if any of the 34 is refused, or if the wording claims an annotation the probe does not read: it narrows to *declares it an array of more* unless the prototype reads both.

### S4, a run-time guard

- `verdict`: **object** standalone; approve only as S7's guard
- `section`: design.md §1.6 (a prediction must name an instrument that can score it), §1.12
- `spec_token_delta`: vendored +36 / +37 (a lower bound), est. +46 to +51; as S7's clause, vendored +7 over S7
- `removal`: nothing
- `needed_for_self_hosting`: no
- `argument`: The honest sentence promises a detection whose limit the reader cannot use: it aborts where C wrote within the guard and says nothing of further. A program cannot be written against *sometimes*. On the program's own cell it also adds a temporary to every scalar lend, which is S7's mechanism without S7's benefit. On S7's own temporary it is §1.12's *check rather than assume* on memory the compiler owns, and there it is worth its clause.
- `prediction`: none admissible as payment for the standalone sentence.
- `condition`: approve as S7's guard, worded to what the prototype guarantees: a guard page makes *C writing past them aborts* true of any overrun, a canary only within its bytes.

### Nothing

- `verdict`: **object**
- `argument`: it leaves 396's ruling unreached for the sake of about four tokens (U), and tonight's readers show the forms that compose today need C by hand.

## 6. What I recommend the synthesis price, and the registered figure

**U with S5's clause, S7 taking the sibling count and a guard clause worded to
the guard, and S2's `counted_by` constant stated once with route C's record
lend.** Drafted exactly as `Rec` (`w/drafts.py`, applied file `w/d/Rec.md`):
vendored cl100k +106, legacy +107. Additivity on the vendored tables holds to
the token for every pair checked (S7 + S2 - U = S7S2; and Rec's +106 is 62 +
10 + 10 + 7 + 2 + 15, its parts' deltas). On the real instrument the measured
parts are S7 +79 and S2 +17; the clauses not priced (U, S5's, the guard, the
sibling, route C's) are estimated at the measured ratios. **Registered
prediction: `Rec` landed verbatim on this base reads 9720 ± 8 real (+137),
leaving about 460 of the 596.** Instrument: `heroes measure
spec/heroes-spec.md --refresh`, which exists; scored at the commit that lands
it in M-buildable-structs. It is **unmeasured as a composition** and says so.
The payment for the tokens is the reader-test predictions above (instrument:
`claude -p` under `blind-run.sh`'s flags, then `heroes build` and `run`, both
run tonight), scored at the landing's reader test.

**What I could not do and say so**: S1, S3, S4, S5 and U alone are priced only
on the vendored detector, the four paid runs being spent on the frozen spec,
S7, S2 and S6. No prototype of any route exists in my copy, so every reader
prediction is about a sentence and a compiler that are not yet built.
