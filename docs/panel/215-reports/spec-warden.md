# Panel 215, the spec-warden's report

Written 2026-10-14 from 08:31 in my copy `.claude/worktrees/scratch-panel-215/spec-warden` at
`7e5c02048`, its compiler built from the seed (`clang -I runtime seed/heroes.c runtime/runtime.c -o
heroes`, real 8.66 s, `heroes 0.4.0`). Offline only: no `--refresh`, no paid run, no GitHub write
(two `gh issue view` reads). Every number is followed by its command. What I kept is in
`docs/panel/215-reports/spec-warden-work/`: `drafts.py` (every candidate sentence as an exact edit,
each anchor asserted to occur once), `measure.txt` (its counts and digests), `at.hero` (a probe, built against the critic's `critic-work/taskB/counter.h` copied beside it).
The copy is removed at the end.

## In short

- **No budget veto on any route.** The dearest composition I could draft, a Q2 mark with its place
  clause plus a padding field for Q1, measures **+88** vendored (both tables), so **+88 to +141 real**
  on the ledger's own ratio band, against **132** spendable. It fits at the band's floor and centre and
  crosses the line at its top: a real count before landing is owed by whoever may `--refresh`.
- **Q1: a toolchain fact, 0 spec tokens.** No padding sentence changes a program a reader writes.
  The rule belongs in `.claude/rules/generated-c.md` § Arithmetic and memory, beside *fields, never
  bytes*. I object to every padding sentence (+7, +23, +28 vendored).
- **Q2: a refusal decided from the header, 0 spec tokens** (route C without its sentence), on panel
  009's allocation rule. I object to B (+49 or +60 vendored), because it protects only the bindings
  whose author already knew the danger. I object to D, which C dominates, and to A. Two facts beside
  it, both measured: § 9 still says `@` is *copy in, copy out*, and the emitter copies a whole record
  to read one field of it (`t2 = (*ph0_c); t3 = t2.n;`).
- **Q3: panel 178's condition is void. R1 stands on panel 194's conditions, and its reader-test
  payment is overdue.** Panel 194 registered that test as *checkable at the landing*. The landing was
  2026-10-06, and ledger row 7467 records as payment only the token prediction. Under §1.6 the test
  is scored or marked `lapsed`, never renewed, so route F, re-registering it, is refused. A withdrawal
  of R1 and K returns **51 vendored**, and the landing measured **+65 real**.

## 0. The ceiling, read today

`grep -n "1\.6" docs/design.md` puts §1.6 at line 253, and line 255 reads *"must fit in 10240
tokens, measured by `claude-opus-5` through `POST /v1/messages/count_tokens`"*. The payment rule is
unconditional: *"Every amendment owes a named removal or a registered prediction naming an
instrument that exists today ... at every level"* (the soft-line paragraph, same section).

`./heroes measure spec/heroes-spec.md`: legacy **7519**, cl100k **7655**, real **10048**
(claude-opus-5, pinned 2026-10-13), headroom **192**, the FFI floor **60**. That leaves **132** real
spendable, because the tool's own words are *"what is measured against the ceiling is 10108"*. A
copy under `build/` reads the same two vendored numbers. Its sha256 equals the spec's
(`af7fc4d6a85fc786`).

**Real cost from a vendored delta.** No seat may `--refresh`, so every real number below is a band
and not a reading. The ratios the tree carries:

- `docs/measurements/010-spec-budget-ledger.md` says *"the ratios run 1.00 to 1.60 across
  seventy-nine rows"*. One row has the two instruments disagreeing on the sign. Found by a Python
  scan for `ratio is|ratio of|ratios run`.
- The document as a whole is 10048 / 7655 = **1.31**.
- Panel 194's warden measured edits to this same § 13 paragraph at 1.00 (K, +10/+10) to 1.25 (R1
  beta, +55/+44) (`docs/panel/194-reports/spec-warden.md:133-151`).

So the **lower bound is the vendored delta × 1.00**, the centre × 1.31 and the top × 1.60. For a
removal the same band is what it probably returns, with the ledger's one sign reversal as the
exception the band does not cover.

## 1. Every candidate sentence, measured

Every count was made with `./heroes measure build/sw/out/<name>.md` from the copy's root, on drafts
`drafts.py` wrote from the spec at `7e5c02048`. Exact texts are in `drafts.py`. "Δ" is against
7519 / 7655.

| draft | question, route | what it says | Δ legacy | Δ cl100k | real (×1.00 / ×1.31 / ×1.60) |
|---|---|---|---|---|---|
| `q1-p-short` | Q1, a promise | *Its padding reaches C zero.* after R1's sentence | +7 | +7 | ≥7 / 9 / 11 |
| `q1-p-long` | Q1, a promise | *The bytes between and after a group's record's fields reach C zero, however the record got there.* | +23 | +23 | ≥23 / 30 / 37 |
| `q1-e-field` | Q1, E | *A field named `_` is C's padding, as wide as the header leaves it: zero, and never read or written.* (its grammar change unpriced) | +28 | +28 | ≥28 / 37 / 45 |
| `q2-b-mark` | Q2, B | ``record Lock tag lock pinned` is a place and never a value: `=`, a return, an argument without `@`, and holding it in an array or another record are compile errors.`` + `[ "pinned" ]` in `Member` | +49 | +49 | ≥49 / 64 / 78 |
| `q2-b-mark-place` | Q2, B, honest | the same + *and `@` hands a function the place itself* | +60 | +60 | ≥60 / 79 / 96 |
| `q2-c-header` | Q2, C with a sentence | *A record whose C struct holds a lock or an atomic, `pthread_mutex_t` or `_Atomic`, is a place and never a value: ...* (same refusal list, no word) | +60 | +60 | ≥60 / 79 / 96 |
| (none) | Q2, C without a sentence | the refusal lives in design.md and its diagnostic | 0 | 0 | 0 |
| `q2-d-runtime` | Q2, D | *Copying a record whose C struct holds a lock or an atomic aborts.* | +16 | +16 | ≥16 / 21 / 26 |
| `q2-at-place` | beside Q2, a removal | § 9's *Semantics: copy in, copy out (copy-out always happens, including on early return and `?`).* becomes *Semantics: the callee works on the caller's place itself.* | −9 | −10 | returns ≈10 / 13 / 16 |
| `q3-r1-short-a` | Q3, C | *End a group's construction with `rest: zero` and every field sharing no byte with one it names is zero.* | −6 | −6 | returns ≈6 / 8 / 10 |
| `q3-r1-short-b` | Q3, C | the same with *; only a group's record has it* dropped and no "group's" | −8 | −8 | returns ≈8 / 10 / 13 |
| `q3-r1-short-c` | Q3, C | *`rest: zero` ends a group's construction, zeroing every field sharing no byte with one it names.* | −7 | −7 | returns ≈7 / 9 / 11 |
| `q3-r1-out` | Q3, B | R1's sentence (line 383) and its union clause (389) out | −42 | −42 | **+55 real measured** at its pricing (194, run 3) |
| `q3-k-out` | Q3, B | K (line 355) out | −9 | −9 | **+10 real measured** at its pricing (194, run 6) |
| `q3-r1k-out` | Q3, B | both out | −51 | −51 | **+65 real measured** at the landing (ledger row 7467: 9,583 against 9,518) |
| `combo-dearest` | budget | `q2-b-mark-place` + `q1-e-field` | +88 | +88 | ≥88 / 115 / 141 |
| `combo-c-plong` | budget | `q2-c-header` + `q1-p-long` | +83 | +83 | ≥83 / 109 / 133 |

The compositions are additive to the token on the vendored tables: +88 is 60 + 28 and +83 is 60 + 23.
**R1 and K are byte for byte what landed**: `git show 54c5f6187 -- spec/heroes-spec.md` shows the
three hunks, and the text at lines 355, 383-384 and 389 is unchanged. So the landing's **+65 real**
is the withdrawal's price on its own base. On today's base it is a band, ≥51 by the floor.

**The budget, verdict.** No route alone comes near the line. The worst single sentence is +60
vendored, at most 96 real against 132. The dearest pair, a Q2 sentence with any Q1 sentence, is +83
to +88 vendored. Its real cost is 109 to 115 at the centre and **133 to 141 at the top ratio, over the
132** that remain. That is not a measured breach, so **no veto**. But no pair of a Q2 sentence and a
Q1 sentence may land without a `--refresh` reading, and §1.6's payment is owed for each part either way.

## 2. Q1: padding across a call by value (issue 1334)

**A toolchain fact, not a language rule, and its home is `.claude/rules/generated-c.md` § Arithmetic
and memory** (line 56 already holds *"A generated `eq` or `hash` walks **fields, never bytes**,
because padding makes two equal records hash differently"*). design.md §1.12 carries the argument
for a defended boundary. The facts this rests on:

- **Nothing a reader writes changes with the sentence.** Under every route the program text is the
  same: `rest: zero` does not help, which the critic measured (§ 4 of `critic-briefs.md`: 3 reports at
  `small_rz.hero:29` on x86-64 at `-O0`, the same line as without it). No Heroes construct reads
  padding (`==` and `hash` walk fields). So no sentence moves §1.2's rewrite term, and §1.2 pays
  nothing for `q1-p-short`'s +7 or `q1-p-long`'s +23.
- **The 3 reports are padding, not a field.** This is read off the struct, not inferred from
  MemorySanitizer: `small.h` declares `struct cd { char c; double d; }`, and the probe names both
  fields (`Cd(c: 1, d: 2.0, rest: zero)`). So every uninitialised byte is one of the 7 bytes of
  padding. **Panel 194's second condition** (*a union none named, or a field sharing no byte with a
  named one, nonzero after `rest: zero`*) **does not fire on them.**
- **Route E is G with syntax, and dearer.** A field the C struct does not declare cannot be added
  to the header's struct. So `_` padding fields would have to be written by Heroes at every
  boundary, which is G's mechanism. It also asks the reader to compute C's padding by hand, a new
  plausible mistake per record. That is +28 vendored plus a `Fields` production change, a §1.2 loss.
- **What a sentence would cost if the sitting wants the promise in the prompt anyway**:
  `q1-p-short`, ≥7 real. It is honest only once F or G is built and the critic's probe reads 0 at
  `-O0` with a positive control on every level. Under CLAUDE.md § 12 (spec beats compiler), a
  promise the emitter does not keep makes the compiler wrong by definition.
- **Not §1.12 strictly** (a question, not a premise): C11 §6.2.6.1p6 leaves padding *unspecified*,
  and handing those bytes to `write()` is not undefined behaviour. The class is CERT DCL39-C's
  information leak. So the run-time cost of F or G is the compiler-engineer's to weigh under rank 5,
  not mine to waive.

**Adopt** A in the spec (0 tokens), with the remedy, F or G, as the emitter's rule. **Refuse** B as
closing 1334 (the critic's table refutes it), and **refuse** E.

## 3. Q2: a record C forbids to copy (issue 1335)

### Principle 0 (design.md §1.0; CLAUDE.md § 2)

- **The closure list: no.** design.md line 136 lists `record` through *spawning a process* and no
  mark. `git grep -l -i -E 'pthread|mutex' 7e5c02048 -- 'selfhost/*.hero'` finds 9 files, every hit
  a link flag (`-pthread`) or a comment. The compiler binds no lock.
- **The thesis branch: unmeasured for a word, admissible for a refusal.** design.md §1.12 says in
  its own words *"It does not suspend Principle 0 ... 'it would be safer' is not an entry ticket"*.
  So robustness cannot admit a new word on its own. What is measured:
  - the silent wrong program, issue 1335's run of 2026-10-14: two locks, exit 0, no message;
  - the reader's attention, panel 178: *"19 of 50 readers raised copy-in/copy-out on their own"*
    (`docs/panel/178-reports/completeness-critic.md:99`). That reading was taken on a spec before
    panel 196's R1, the `@`-is-its-place text, landed.

  **Not measured: how often a reader writes the copy.** That is Task B's number, and only if its
  scoring sees a copy (the critic's § 3 item 4). A refusal adds no form, and panel 010 approved
  one at **0 spec tokens** under the same thesis (`docs/panel/010-repeated-mutable-arguments.md:107`).
  A word (`pinned` or another) is a form, and it waits on that number.
- **The population a mark or a refusal would meet today**: `git grep -l -E '^[[:space:]]+record
  [A-Za-z_0-9]+.*(pthread_(mutex|cond|rwlock|spin|barrier)|atomic|os_unfair_lock|CRITICAL_SECTION|SRWLOCK)'
  7e5c02048 -- '*.hero'` finds **21 files, all 21 under `docs/panel/`**: 0 elsewhere, `examples/`
  included (21 files there carry `^extern`, by the quoted `grep -rl '^extern' examples
  --include='*.hero' | wc -l`; one names `pthread_t` and none a mutex). A `partial` record hiding a
  mutex in an undeclared field (Task B's shape) is invisible to a grep, so 0 is a floor, not a
  count.

### §1.2 and panel 009's allocation rule

*"Tokens that close a silent corner are worth buying ...; tokens that prevent a loud compile error
are not"* (`docs/panel/009-spec-budget-2000.md:50-52`).

**The copy is silent today, and that corner is closed by the refusal, not by the sentence.** Once
the copy is refused, a `q2-c-header` sentence (≥60 vendored, about 79 real) only prevents a loud
error. That error is a round trip of 500 to 2000 tokens (§1.2), and it fires in programs that bind
a lock record and copy it. The sentence would pay only if that share of **all** programs exceeded
about 79 / 1000 ≈ 8%. The tree's share is 0 of the 905 files with an `extern` outside `docs/`.
So the sentence does not pay, and the refusal's diagnostic carries the fix (*pass it with `@`*,
design.md §4.17).

**Why not B, at any price that fits.** B protects only bindings whose author wrote the word. That
author is the reader who already saw the danger, and so the least likely to write the copy. B is
the less robust route at a higher token price: ≥49 vendored, ≥60 with the place clause it needs (below).
CLAUDE.md's instruction of 2026-09-12 (*the most robust ... never the cheapest*) and the allocation
rule point the same way. **Object.** D runs at run time, needs the same type list as C and reaches
the program later. C dominates it, so **object**. For A, the measured program is silent and wrong,
so **object**. A2 is not a route: the critic measured that the copy still copies.

### Two facts beside Q2, measured in my copy

1. **§ 9 still says `@` is *copy in, copy out*** (spec line 272). Panel 196's R1 made it *"its place,
   never a copy"* in the emitter (`selfhost/ir/inout.hero:5`, `selfhost/emit/prologue.hero:38-50`),
   and `inout.hero:11-15` says the two readings are one meaning inside Heroes. Under the spec's own
   words, though, route C's fix (*write `@`*) is a copy of the lock. My probe `at.hero` (kept) lends
   a lock record as `@p.left` and as `@xs[0]` through a Heroes `@` helper. `heroes build --emit-c`
   shows `h_at_bump(&h0_p.f_left)` and the element's own slot, by address, and the binary prints
   `2`, exit 0. **`q2-at-place` makes § 9 say what the emitter does, and returns 9 to 10
   vendored.** It is the one named removal this sitting can offer. It changes the sentence every
   reader reads about `@`, so it needs the compiler-engineer's yes (every `@` lowered by address: I
   ran three shapes, not all) and the ergonomist's. It is not owed by Q2 at 0 tokens; it is owed by
   B at once, which is why B honest is `q2-b-mark-place`.
2. **The emitter copies the whole record to read one field.** In the critic's *correct* `b.hero`,
   `heroes build b.hero --emit-c` lowers `c.n @ c.n + 1` inside `bump(@c)` to `t2 = (*ph0_c); t3 =
   t2.n;` (`b.c:179-181`). That copies the whole `pthread_mutex_t` (64 bytes on this Mac, by a C `sizeof` compiled here;
   Linux unmeasured), while the program holds the lock, into a temporary. The copy is never used as a mutex. Under a second thread contending
   for the lock it is a plain read racing an atomic write: a question, unrun. Any "never a value"
   promise, in the spec or in design.md, is false of this emitter until a field read reads the field.
   It is the compiler-engineer's.

**Adopt** C without a sentence: a refusal of `=` from a name, a by-value parameter, a return, an
element of a `[T]` and a field of a Heroes record, for a group record whose C struct holds such a
type; construction into its cell allowed; the diagnostic carrying `@` as its fix; its home design.md
§4.19 and §4.17. Whether C can decide from the header without a list of type names it cannot keep
is the ffi-pragmatist's. If it cannot, my verdict on B stays *object*, never *approve*: B then
waits on Task B's copy rate, by Principle 0.

## 4. Q3: R1's payment

**What each sitting registered, read in its file:**

- **Panel 178** is superseded. `sed -n 255,286p docs/panel/178-the-rest-is-zero-*.md` reads
  *SUPERSEDED 2026-10-06*. Its warden's turn-to-object condition (*"M-core-packages' plan puts every
  long-array construction inside 2 or fewer bindings"*, `178-reports/spec-warden.md:42`) is the one
  measurement 044 reads as met (*one binding or two*). It is **void**, on three grounds:
  - the sitting is superseded;
  - panel 194's warden declined it in so many words (*"I hold that reading: when one construction
    is 905 to 4016 tokens, absence does not measure need"*, `194-reports/spec-warden.md:219-223`);
  - measurement 044 says of itself that it *"answers a question the payment still rests on, not one
    that gates the landing"*.

  **Route D: approve**, recorded as a superseded sitting's condition, not as a condition met.
- **Panel 194** registered two payments (`194-reports/spec-warden.md:270-277`) and one
  turn-to-object (`:240`):
  1. **The reader test**, *"at least 10 fresh readers per arm: L, R1 honest, R1 honest with K; one
     task holding a union; every program compiled ... at least 8 of 10 correct first try and at most
     2 of 10 writing `u8` under R1 with K"*, **checkable at: the landing**
     (`194-the-rest-*.md:179`). The landing was 2026-10-06. The milestone page itself calls issue
     1332 *"the landing that already happened"*. **It is unscored.** Ledger row 7467 names as
     payment *"panel 194's registered prediction for bs's composition (9,583 ± 3, scored a hit)"*.
     That is the token prediction, a forecast of the price, not of the rewrite rate §1.2 asks to
     be paid. **The ledger records as paid what was not paid.** Under the append-only rule the
     correction goes underneath, dated.
  2. **178's grep at M-core-packages' close**: `rest: zero` in 3 or more `.hero` files outside
     `tests/` and `docs/`. `git grep -l 'rest: zero' 7e5c02048 -- '*.hero' ':!tests' ':!docs' | wc
     -l` reads **19** today, every one under `selfhost/`. With `':!selfhost' ':!archive'` it reads
     **0**. As written it is met vacuously by the compiler's own source. Read as meant, programs
     that construct with it, it is 0. **Not due until M-core-packages closes.** It must be read
     with `:!selfhost` then, or it pays nothing.
  3. **The MSan condition** on unions and unnamed fields: run 2026-10-14 on arm64, clean at `-O0`
     to `-O3` with a positive control (the milestone page). **x86-64 and the Windows box are unrun
     for the union case.** The critic's x86-64 reports are padding (§ 2), so they do not score it.

**What §1.6 makes of the overdue test** (design.md, the paragraph after the payment rule): *"An
outstanding prediction is re-decided, never renewed. At the milestone it names it is scored, or it
is marked `lapsed` in the ledger row that spent it and the clause it bought is re-argued on its
merits under the removal branch."* So:

- **Route F** (R1 stands, its payment re-registered on a working instrument) **is a renewal**, and
  §1.6 refuses it in those words. **Refuse.**
- **This sitting's paid run scores it only if it is 194's test.** My drafts give the three arms on
  today's spec exactly: `q3-r1k-out` is L (sha256 `0050060a5d421f2d`), `q3-k-out` is *R1 honest*
  (`ef45d6c419c1259a`), and today's spec is *R1 honest with K* (`af7fc4d6a85fc786`), each from
  `python3 -I drafts.py <spec> <dir>`. It also needs a union task (194's warden named 178's Darwin
  `utsname` task). The critic shows Task A is not that task (`critic-briefs.md` § 3). **If the run
  goes as designed, it does not score R1, and the report must say so.**
- **If the test is not scored before M-buildable-structs closes**, row 7467's reader-test payment
  is marked `lapsed`, and R1 with K is re-argued under the removal branch. That costs **65 real** to
  keep (measured at the landing), and what is on offer is `q2-at-place` (≈10) and `q3-r1-short-b`
  (≈8), about 18 of the 65. The rest would have to be withdrawn or paid by a new prediction scored
  at a named milestone.
- **Route C (shortened), not now.** `q3-r1-short-b` drops *only a group's record has it*, which
  prevents a loud error. My check: `rest: zero` on a plain record exits 1 with
  `error[rest_outside_a_group]`. So it is a legitimate removal under the allocation rule, −8
  vendored. **Landed before the reader test, it changes the arm the test scores**, so it waits for
  the score.
- **Route B (withdrawal), not now**: the instrument exists, so the payment can still be collected.
  The withdrawal returns 51 vendored and about 65 real (above).

**Adopt** A + D: R1 stands on panel 194's conditions, and panel 178's is recorded void. The
ledger's row 7467 is corrected underneath. The reader test is scored with 194's arms before the
milestone closes, or the row is marked `lapsed` and R1 re-argued.

## 5. The verdict, in the seat's structure

- `verdict`: **approve** Q1 route A in the spec (0 tokens, the remedy as an emitter rule) and Q2
  route C **without a sentence** (0 tokens), and Q3 routes A + D. **Object** to every Q1 sentence
  (+7 / +23 / +28), to Q2 B (+49, +60 honest), to C-with-its-sentence (+60), to D (+16) and to A,
  and to Q3's B and C *before the reader test* and to F. **No veto.**
  **Provisional** on every real figure: they are vendored deltas times the tree's ratio band.
- `section`: design.md §1.6 (the ceiling, line 255; the payment rule; *re-decided, never renewed*),
  §1.2, §1.0 and §1.12 (*does not suspend Principle 0*); panel 009's allocation rule; CLAUDE.md § 12.
- `spec_token_delta`: recommended composition **0** (7519 / 7655 / 10048 unchanged), or **−9 / −10
  vendored** with `q2-at-place`. Dearest drafted: **+88 vendored**, ≥88 and up to 141 real against
  132 spendable. Every row is in § 1.
- `removal`: `q2-at-place` (−9 / −10 vendored, ≈10 to 16 real), and after the reader test
  `q3-r1-short-b` (−8). For a Q2 or Q1 sentence the removal is **nothing, and that is a problem**:
  neither removal covers +49.
- `needed_for_self_hosting`: **no** (closure list, line 136; `selfhost/` binds no lock).
- `argument`: A padding sentence changes no program a reader writes, so §1.2 pays it nothing; only
  an emitter rule can close 1334. A copied lock is a silent corner. The refusal closes it, and a
  sentence then only prevents a loud error, which panel 009's rule declines to buy. B's word
  protects only an author who already knew, at ≥49 tokens. R1 stands on panel 194's conditions, not
  178's, which is void. Its reader test was due at the landing and was never scored. §1.6 says
  score it or lapse it, never renew it. (≤120 words)
- `prediction`:
  1. Tokens, offline. If the sitting adopts my recommended composition, `./heroes measure
     spec/heroes-spec.md` at its landing reads 7519 / 7655, or 7510 / 7645 with `q2-at-place`, and
     the real count falls by 9 to 16.
  2. A sentence, if adopted. If any Q2 sentence lands with any Q1 sentence, `--refresh` reads real
     between **10131 and 10189** (10048 + 83 at the ratio floor, 10048 + 141 at its top). The check
     goes red above 10180, 10240 less the floor's 60, so the top of that range crosses it.
  3. Readers. In this sitting's Task B (today's spec, at least 10 readers, every program read and
     not only run), **at most 2 of 10** copy the record after `counter_init`, counting a by-value
     parameter, a return, `=` or `@=` from an existing value, or a field or growing element that
     holds it. A Heroes `@` does not count.
  4. R1. Scored by 194's own arms if the run has them; else `lapsed` at M-buildable-structs' close.
- `condition`:
  - Turn to **approve B** if Task B reads **3 or more of 10** copying, **and** the ffi-pragmatist
    shows C cannot decide from the header without a type list the code cannot keep. A word then has
    its measured thesis effect and the only robust route is gone.
  - Turn to **approve a Q1 sentence** if a reader test shows a program choosing differently because
    of it.
  - Turn to **object to R1 standing** on 194's own condition: `u8` 3 or more of 10 under R1 with K,
    or the union case nonzero under MSan on x86-64 or Windows. Turn to **withdrawal** if
    M-buildable-structs closes with the test unscored and no payment named for the 65.
  - Turn to **veto** if a `--refresh` of any adopted composition reads above 10180 (10240 with the
    floor) with no named removal.

## 6. What I could not run

- **No real count.** Every real figure is a band from the vendored delta. `--refresh` was
  forbidden to this seat.
- **Task B's copy rate**, the number Q2's word waits on: a paid run, the coordinator's.
- **Whether route C can decide from the header**, and with which list: the ffi-pragmatist's.
- **Whether every `@` lowers by address.** I ran a local, a field and an array element through a
  Heroes helper, not generics, UFCS or a callback.
- **The field-read copy under a second thread** (§ 3, fact 2): unrun.
- **The union case under MSan on x86-64 and the Windows box**, panel 194's third condition: unrun
  by me, and by the milestone page.
- **Measurement 044's headers**: not re-run, quoted as the critic verified them.
- I did not read issue 1332, nor the ledger rows after 7467.
