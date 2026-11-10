# Panel 243, the spec-warden's report

Sat 2026-11-10 in `.claude/worktrees/243-spec-warden`, detached at `89db84347`. Compiler built there
in two stages from the seed: `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes` (13.96 s
real), then `./heroes build selfhost/main.hero -o heroes-next` (329.98 s real against 149.98 user,
while the load average stood near 200 with the other seats building: a duration nobody should
quote). `heroes-next --version` prints `heroes 0.6.0` and `sources 14e9e5ce168a2336`, the base's
key. Probes are under `.claude/worktrees/p243-scratch/spec-warden/`: `sentences/`, `q1-2421/`,
`q1-today/`, `q1-wformat/`, `q2-2428/`, `q3/`. No paid run, no `--refresh`, no GitHub write (two
read-only `gh issue list` searches), no push. My worktree's tree is clean (`git diff --quiet`
after the one temporary edit, § 6).

**Every real-token figure below is an inference.** The vendored rows are measured. The real row
is the pin of 2026-11-09, and every delta on it is a ratio read from the ledger. That makes my
verdict **provisional on the binding instrument**.

## The verdict, in the seat's shape

- `verdict`: **veto** of the format exception in routes (R) and (E), on Principle 0. **Approve**
  (R0) with **no** § 13 sentence. **Object** to (C) as a resolution. **Approve** Q2 and Q3 on
  spec grounds, at zero spec tokens. **Provisional**: the real row was not refreshed.
- `section`: design.md §1.6 (the ceiling, **16384**, `claude-opus-5` through `count_tokens`,
  read today at `docs/design.md:253-256`); §1.2; Principle 0 (CLAUDE.md § 2, and § Precedence's
  ranks 3 and 4 read together, panel 215: *a refusal lands at once and a new form still needs
  § 2's ticket*). Also design.md Part 7 row 10 (`docs/design.md:3869`, the correction at
  3900-3910) and §4.19 (`docs/design.md:3156-3163`).
- `spec_token_delta`: base **8167 legacy / 8314 cl100k / 10898 real** (pin of 2026-11-09).
  (R0) with no sentence: **+0**. If a sentence is wanted: V0 **+19**, V1 **+34**, V2 **+47**.
  (R) needs V3, **+52**. (E) needs V4, **+52** (+53 legacy). An honest (C) needs V5, **+18**.
  All are vendored maximums, each measured in place in a whole copy of the spec. V3 and V4 cross
  `DELTA_GATE`'s 50, so a `--refresh` is owed. Real deltas are about ×1.25 of these, an
  inference. Q2 and Q3: **+0** spec tokens.
- `removal`: (R0) at zero tokens owes none. For any § 13 sentence: **nothing, and that is a
  problem**. No sentence of § 13 becomes redundant under any route (I read 373-490). So a
  sentence would have to be paid by prediction P3 below, which is a paid run.
- `needed_for_self_hosting`: **no**. Measured: `git grep -h '^extern "' -- 'selfhost/*.hero'`
  names `hero_os.h` 17 times, `hero_compiler.h` 6 times and `stdlib.h` once (`atof`).
  `git grep -E '\.\.\.\s*\)'` over `runtime/hero_os.h` and `runtime/hero_compiler.h` prints
  nothing. The compiler passes nothing past a `...`.
- `argument` (120 words): (R0) is the one Q1 route under which § 13's existing sentence stays
  the whole rule. Past `...` the header declares no width, so nothing is declared at it. That is
  the reading defects 2102, 2103 and 2107 landed at zero spec tokens, the note carrying the
  wrapper. (R) and (E) add a second yardstick, a format's conversion. The spec must state it
  (+52 vendored, past `DELTA_GATE`) or leave per-leg verdicts unpredictable. Under (R), `%ld` has
  no spelling every leg shares. Under (E), no literal spelling of an `i64` passes on both Mac and
  Linux. Their exception is new code the compiler does not need, unmeasured on the thesis:
  Principle 0 unmet. (C) pins a wrong value under a sentence promising refusal.
- `prediction`: three, in § 7.
  - **P1**: V1 landed as worded reads exactly 8201 / 8348 vendored and 10,925 to 10,950 real.
  - **P2**: under (R0) with integer scope and no sentence, gate B turns exactly the four
    tracked programs the critic named from building to refused.
  - **P3** (paid, the author's yes): with no sentence, at least 7 of 10 blind readers put a
    number or an `@` past `...` on the libcurl task; with V2, at most 3 of 10.
- `condition`: four things would change this verdict, set out in § 8.
  - A program the closure list or §4.19's ladder needs that hands a number to a format
    function and cannot go through a wrapper.
  - A blind-seat measurement that gives the exception a thesis effect.
  - P3's first half holding, which makes V2 owed.
  - (R0) landing without Q2's route: then I object to (R0) alone.

## 1. The ceiling, read today, and what binds below it

`grep -n '^### 1\.6' docs/design.md` gives line 253. Lines 255-256 say: *"must fit in 16384
tokens, measured by `claude-opus-5` through `POST /v1/messages/count_tokens`"*.

The consolidation block's 1,500 applies only "before the thesis harness has run". That harness
has run: design.md §1.6 says *"Metric 2 first ran on 2026-10-24 (measurement 052)"*, and the
ledger records the M-thesis-harness close of 2026-10-24. So that cap no longer binds this
sitting. What binds is the ceiling, the FFI floor of 60, `DELTA_GATE` of 50 vendored
(defined at `selfhost/measure/judged.hero:162-163`, read at `selfhost/cli/measure.hero:146`), and the payment rule, which is unconditional.

## 2. The base, measured

From the worktree's root, `./heroes-next measure spec/heroes-spec.md` printed claude-legacy
**8167**, cl100k_base **8314**, spread 147, `real 10898 claude-opus-5, 2026-11-09`. Headroom is
5486 against 16384; with the FFI floor, 10958 is what is measured against the ceiling. This
matches the coordinator's numbers and the critic's item 1. The largest candidate below leaves
about 5,360 real free, so **no route is near the ceiling, and my veto ground is Principle 0, not
the budget.**

## 3. Q1: what § 13 says past `...`, and what each route would have to add

### 3.1 The grep

`grep -n -i -E 'variadic|\.\.\.|va_arg|printf|format|varargs' spec/heroes-spec.md` hits lines
289, 304 and 338 (*no user variadics*, `ok(...)`, `print(...)`). None of them is about C.
**§ 13 says nothing past a header's `...`.**

What a reader would apply is § 13's sentence at 379-383:

> A **parameter** and a **field** are declared at the header's own width and sign — `i32` where
> C says int, `i8` where it says char, `u64` where it says `size_t` — and one that disagrees is
> refused, except a parameter C converts exactly (`i16` against int) and what a `ptr` points at.

Past `...` the header gives no width, and C converts nothing; only the default promotions
apply. Read to the letter, the sentence leaves no declaration that agrees with it.

That is the reading the compiler already takes for four of the five refusals past `...`:
- 2102: a `cstr lent`;
- 2103: a counted lend;
- 2106 and 2107: an `@`;
- panel 217's R3(c): a record lent read-only.

All four landed with **no spec row**. Their notes cite § 13; `selfhost/emit/ffi_past_prototype.hero:205,233`
says *"past a header's `...` the header gives none"*. A by-value number is the fifth case, and
the only one still accepted.

Grep of § 13 for a program's own header (`own`, `your own`, `of its own`): nothing. **The spec
never says a group's header may be a file the program writes.** Every refusal route's repair
("a header of your own") lives in the diagnostic's note alone, which is what §4.17 promises a
note can carry.

### 3.2 The sentences, each measured in place

Each sentence goes after the unique anchor `and what a `ptr` points at.` (spec line 383).
`sentences/insert.sh` inserts it into a copy of the spec, and the whole copy is measured from the
worktree's root. Each was also measured alone. Base: 8167 / 8314.

| id | route | sentence | legacy | cl100k | Δ vendored max |
|---|---|---|---|---|---|
| (none) | R0 | (the note carries the route) | 8167 | 8314 | **+0** |
| V0 | R0 | *Past a header's `...` it declares no width, so a number there is refused.* | 8186 | 8333 | **+19** |
| V0i | R0, integers only | same, *an integer* for *a number* | 8186 | 8333 | +19 |
| V1 | R0 | V0, then *: bind instead a function of a header of your own that hands it on.* | 8201 | 8348 | **+34** |
| V1i | R0, integers only | V1, *an integer* | 8201 | 8348 | +34 |
| V2 | R0, the whole class | *Past a header's `...` it declares no type, so a call hands C there only a lease, a `ptr` or a handle: bind anything else through a function of a header of your own that hands it on.* | 8214 | 8361 | **+47** |
| V3 | R | *Past a header's `...` it declares no width, so a number there is refused unless the call's format is a literal whose conversion reads that width on the machine building it: elsewhere bind a function of a header of your own that hands it on.* | 8219 | 8366 | **+52**, past `DELTA_GATE` |
| V4 | E | *Past a header's `...` it declares no width, so a number there is refused unless the call's format is a literal, which clang checks against the number's C type: elsewhere bind a function of a header of your own that hands it on.* | 8220 | 8366 | **+52** (+53 legacy), past `DELTA_GATE` |
| V5 | C, told honestly | *Past a header's `...` nothing holds a number to the width C reads there.* | 8185 | 8332 | **+18** |

**Real tokens: inference, unrun.** The landed rows of `docs/measurements/010-spec-budget-ledger.md`
read +48 vendored to +60 real (row 7703), +143 to +178 (7846) and +308 to +390 (8327), about
×1.25. That puts V0 near +24, V1 near +43, V2 near +59, V3 and V4 near +65 and V5 near +23.
Row 8314 read −6 vendored and **+4** real, so at this size even the sign can turn. This is why
my verdict is provisional.

**The word checks pass.** I wrote V1 then V2 into my worktree's spec, ran
`./heroes-next run tests/harness/main.hero -- ./heroes-next spec` and restored the file from a
byte copy (`git diff --quiet` then printed nothing). The suite read **31 passed, 4 failed**. The
four are the count's own: `spec/budget` (*"8395 and the record says 8314 — a delta of 81"*,
which is 34 + 47), `spec/spendable`, `spec/real` and `spec/ledger`. The word, anchor and
colouring checks all passed on both sentences. V0 adds no word that V1 and V2 lack. The
others do, by a vocabulary diff against V1 and V2 (`comm -23`): V0i adds *an* and *integer*; V3
adds *building*, *call's*, *conversion*, *elsewhere*, *format*, *literal*, *machine*, *reads*,
*the*, *unless* and *whose*; V4 adds *against*, *call's*, *checks*, *clang*, *elsewhere*, *format*,
*literal*, *number's*, *the*, *unless* and *which*; V5 adds *holds*, *nothing*, *reads*, *the* and
*to*. None of V0i or V3 to V5 went through the suite, so whether its word checks pass on them is
unrun.

### 3.3 Does a route need a § 13 sentence?

- **(R0): no sentence is needed for truth.**
  - Without one, § 13's sentence stays true and complete: nothing can be declared at a width the
    header does not give.
  - A spec-only reader who binds a number past `...` gets one compile error at turn 1. Its note
    carries the wrapper, as 2107's does.
  - A sentence (V1, V2) buys only **predictability**. Under §1.2 that is the rewrite rate's
    factor. Its size is a question for the blind seat, so V2 is P3's to earn, and P3 is a paid
    run that the synthesis queues for the author's yes.
  - V2 is the only wording that would let a reader bind the ladder's rung 4,
    `curl_easy_setopt` plus `curl_easy_getinfo`, without a refusal: it states the whole class,
    five refusals in one sentence. V0 and V1 state one refusal of five.
- **(R): yes, V3 or an equivalent, +52 vendored, past `DELTA_GATE`.**
  - Without it a reader cannot know three things:
    - `printf("%d".cstr(), n)` builds while a format held in a name does not;
    - `%ld` with an `i32` is refused on three legs and accepted on Windows;
    - which functions count as format functions.
  - The spec has no other rule that turns on whether an argument is a literal at a call. Only
    constants use "known values" (spec line 133).
- **(E): yes, V4, +52, past `DELTA_GATE`.** Even V4 cannot tell a reader the spelling to write,
  which differs by platform (§ 3.6).
- **(C): yes, V5, +18, if the spec is to stay honest.** Today § 13 tells a reader *"one that
  disagrees is refused"*. Under (C) a disagreeing number past `...` builds and prints
  `4294967295` at exit 0. Pinning that in design.md alone would leave the spec implying a
  guarantee that design.md records as broken. V5 would be the first spec sentence that
  advertises a silent wrong value.

**So, plainly:** (R) and (E) each need a § 13 sentence. If either is chosen, the synthesis
queues the blind seat's paid run and a `--refresh` for the author. (R0) needs none, and P3 is
the paid run that would say whether V2 should still land.

### 3.4 R10's exception, priced (design.md, which carries no token budget)

R10 (`docs/design.md:3900-3910`) rests on *"a parameter has a spelling every leg shares, a
narrower width C converts exactly: `i32` for a C `long`"*. That is true through a prototype and
false past `...`, as panel 242's ffi-pragmatist § 1.4 says (`docs/panel/242-reports/ffi-pragmatist.md:99-107`:
*`sizeof(long)` printed 4 on the box and 8 on this Mac*).

My first draft said *"`i64` is right on three legs"*. My own `-Wformat` probe (§ 3.6) printed
`int64_t (aka 'long long')` on this Mac. By C11 §7.16.1.1p2, `va_arg(ap, long)` over a
`long long` is an incompatible type, so it is undefined here too, at the same width. I rewrote
the sentence so that it holds under **either yardstick**, the C types or the machine's widths:

- **(R0), appended after 3910**, 216 legacy / 221 cl100k:
  > **Corrected 2026-11-10** (panel 243, defect 2421): a parameter's shared spelling holds through
  > a prototype, and past a header's `...` there is none. C converts nothing there and `va_arg`
  > reads the type the callee asks for, so a C `long` read there has no spelling every leg
  > shares: `i32` reads wrong on three legs (`-1` printed `4294967295` on this Mac), and `i64` is
  > `long` only on Linux, `long long` on this Mac, the same width and by C11 §7.16.1.1p2 another
  > type, and four bytes wider than `long` on Windows. A number past `...` is therefore refused,
  > and crosses through a header of the program's own, a `static inline` function of a new name
  > whose prototype says `long` and calls the variadic through its own, bound with `i32` as
  > above; never a re-declaration of the variadic (panel 052).
- **(R) adds**, 76 / 75:
  > The one exception is a format function called with a literal format, whose conversions the
  > compiler reads at the width each names on the machine building it: `%ld` takes an `i64` on
  > three legs and an `i32` on Windows, so no spelling of it is shared, and `%lld` is the one a
  > program meaning to build on all four writes.
- **(E) adds**, 59 / 57:
  > A format function called with a literal format is judged by clang's `-Wformat`, which compares
  > C types and not widths: an `i64` passes as `%lld` on this Mac and Windows and as `%ld` on
  > Linux, so no literal spelling of it is shared.

The (E) sentence's Linux and Windows halves are the lane's and the C standard's, not my runs.
Docker is off, and the box was not used.

### 3.5 Route (C)'s design.md wording, priced the same way

159 legacy / 165 cl100k:

> **Corrected 2026-11-10** (panel 243's conservative route, defect 2421): the shared spelling holds
> through a prototype only. Past a header's `...` C converts nothing, so `i32` for a C `long` the
> callee's `va_arg` reads hands it four bytes where it reads eight on three legs, and
> `slot_read(count: 1, value: -1)` prints `4294967295` at exit 0; `i64` reads the right bytes on
> those legs, defined by C11 §7.16.1.1p2 only on Linux, and is undefined on Windows. No check
> reaches it, the compiler seeing no callee's `va_arg`: a known cost, pinned by a case.

(C) cannot be a resolution, for three reasons:
- It pins a wrong value at exit 0, which is `blocking`, *"never deferred"*
  (`.claude/rules/verification.md` § Bounded discovery), and CLAUDE.md § Precedence puts
  robustness at rank 3, above Principle 0.
- Panel 242's R5 already refused the compiler's own libcurl binding without a header *because
  of* 2421, by the ffi-pragmatist's veto (`docs/panel/242-the-compiler-*.md:169-171`).
- It needs V5 in the spec to stay honest.

It stands only as the record of what the conservative route would have been. My veto does not
reach it, because it adds no form; my objection is the sentence it leaves untrue.

### 3.6 The yardstick (critic item 19), from the spec's side

§ 13's own yardstick is **width and sign**, and it holds through a prototype, where C converts.
Past `...` nothing converts, so behaviour is defined by C's **type** rule (§7.16.1.1p2). Only
route (R0) needs no yardstick at all: nothing numeric past `...` is judged, and the wrapper's
prototype brings back § 13's existing rule unchanged. (R) and (E) must each choose one, and the
spec would have to say which.

`sentences/../q1-wformat/fmt.c` under Apple clang 21.0.0 with `-Wformat` measures the cost:
- An `int64_t` passes only as `%lld`. `%ld` and `%jd` are refused (*"'intmax_t' (aka 'long')
  but the argument has type 'int64_t' (aka 'long long')"*).
- A `uint64_t` passes only as `%llu`. `%lu` and `%zu` are refused.
- An `int32_t` under `%ld` is refused, and under `%d` passes.
- The lane measured `%lld` with `int64_t` refused on Linux arm64; I did not rerun it.

So under (E) **no literal spelling of an `i64` passes on both legs**, which is R10's falsifier
word for word.

## 4. Q2 and Q3: the spec says nothing, so the routes cost the spec nothing

`grep -n -i -E 'shadow|stand(s|ing)? in|pkg-?config|sibling|quoted|quotes|#include' spec/heroes-spec.md`
hits:
- 7, *a quoted word* of the notation;
- 164, a Heroes `use` shadowing;
- 397 and 437, *the sibling*, a record's count field;
- 479 and 482, `pkgconfig` *asks the system where its headers and libraries are*.

**Nothing in the spec mentions a header standing in, `shadowed_header`, a folder `pkg-config`
names, or an include in quotes.** The refusal is design.md §4.19's alone, so a reader cannot
predict it today. A route that **narrows** it owes the spec nothing, and a route that keeps its
danger side refused owes nothing either. **Q2 and Q3: +0 spec tokens on every route.**

### Q2, run on my compiler

The coordinator's reproducer was copied to `q2-2428/`. Built with
`PKG_CONFIG_PATH=/opt/homebrew/opt/curl/lib/pkgconfig`, it exits **1**:
*"`shadowed_header`: `/opt/homebrew/Cellar/curl/8.23.0/include/curl/curl.h` stands in for the
C library's `<curl/curl.h>`, which `net/curl_own.h` includes"*. Without the variable it exits
**0**.

§4.19's phrase today (28 / 28 alone):

> a named folder's file is refused where a platform or runtime header includes it, or where a
> platform folder holds a file of its name;

- **(a), group-scoped, held to the `pkg-config` arm as (d) asks**, 68 / 68, so **+40**:
  > a named folder's file is refused where a platform or runtime header includes it, or where a
  > platform folder holds a file of its name, except a file a group's own header includes from a
  > folder `pkg-config` answers for that group's library, which is that library's, its header
  > and its library read from one answer;
- **(c), the second arm narrowed**, 43 / 43, so **+15**:
  > a named folder's file is refused where a platform or runtime header includes it, or where a
  > platform folder holds a file of its name and the folder is not one `pkg-config` answers for
  > a library;

Of the two, the spec-warden prefers (a). It names the one fact that makes the keg's header the
library's own: header and library come from one answer. (c) also excuses a header that a
*different* group's answer names. Which route is correct is the compiler-engineer's and the
ffi-pragmatist's question, not mine; on tokens they differ by 25 design.md tokens and 0 spec
tokens.

### Q3, run on my compiler

The lane's shapes were copied to `q3/`:
- **d** (`lib/clock.h` including `"../sys/timeb.h"`): exit **1** `shadowed_header`;
- **g** (root `clock.h` including `"sys/timeb.h"`): exit **1**;
- **e** (`sys/clock.h` including `"sys/timeb.h"`): exit **0**, printing `42`.

§4.19's phrase today, 13 / 13: *and a header's own sibling included in quotes is its own,*

- **The lane's sentence**, 21 / 21, so **+8**: *and a file a header includes in quotes, found
  from that header's own folder, is its own,*. **Read as a replacement of "sibling", it no longer
  excuses `e`**, which builds today and is found through `-I .`, not from the includer's folder
  (critic item 9). Landed as written, it risks refusing a program accepted today.
- **Covering d, g and e, with the danger refused in both orders**, 60 / 60, so **+47**:
  > and a file under the program's own folder that one of its headers includes in quotes is its
  > own, however clang finds it, unless a platform or runtime header includes it as well, which
  > the listing shows even where a guard skips that second open (`-fshow-skipped-includes`),

  My first draft said *"wherever clang finds it"* with no scope (55 / 55). That would also have
  decided Q2 by the back door, for a keg's header included in quotes. So the scope *"under the
  program's own folder"* is in the sentence, and costs 5 tokens.

## 5. The plausible mistakes of a spec-only reader, and what each route answers

"Today" is route (C), and every cell of that column was run on my compiler unless it is marked.
The R0, R and E columns are **unbuilt by my seat**. I expect `ffi_parameter_type`, by 2106's
precedent; the compiler-engineer builds them.

| # | the reader writes | today = (C), run | (R0) | (R) | (E) |
|---|---|---|---|---|---|
| M1 | `printf(format: cstr lent, value: i64)` called with `"%d"` and `4294967338` (`q1-today/m1`) | builds; prints **`n = 42`**, exit 0 | refused | refused (`%d` reads an `int`) | refused by `-Wformat` (int vs `int64_t`), if (E) makes the call's warning an error |
| M2 | an `i32` under `"%ld"` | builds; wrong value (the lane; 2421's class) | refused | refused on three legs, **accepted on Windows** | refused (probe line H) |
| M3 | `curl_easy_setopt`'s `long` option as `value: i32`, which R10's own `i32 for a C long` invites (`q1-2421`, the slot shape) | builds; prints **`4294967295`**, exit 0 | refused; the note names the wrapper | refused (no format) | refused (no format, so R0's rule) |
| M4 | the same as `value: i64` | builds; right bytes here, undefined by the letter here and on Windows | refused | refused | refused |
| M5 | `value: cstr lent` past `...` (`q1-today/m5`) | refused `ffi_writable_parameter`, exit 1 | same | same | same |
| M6 | `@out: i64` past `...` (`q1-today/m6`) | refused `ffi_parameter_type`, exit 1 | same | same | same |
| M7 | `open(path: cstr lent, flags: i32, mode: i32)` with `O_CREAT` (`q1-today/m7`) | builds; prints `true`; `made.txt` is created `-rw-r--r--`: **a correct program** | **refused, a correct program**, wrapper needed | refused (no format) | refused |
| M8 | `printf("%lld", n: i64)`, the portable spelling | builds; correct | refused | accepted on every leg | accepted on Mac and Windows, **refused on Linux** (lane) |
| M0 | `open(..., flags, mode)` or `printf(format, value: cstr)` without labels | `needs_label`, spec § 9 | same | same | same |

M0 is my own: two of my four probes first met § 9's label rule, before any rule about `...`. A
reader binding a variadic with two parameters of one type pays that round trip on every route.

Under (R0), M7 and `printf("%d", i32)` are correct programs refused. The synthesis has to write
them down as the **ruling**, with 2107's refusal of `sscanf("%d", @n)` as precedent. Otherwise
`records/tagged` reads them as `blocking` (*"a correct program refused"*).

**Q2:**
- **N1**, a wrapper over the keg through `pkgconfig`: refused today (run). Builds under (a), (c)
  and (d). Builds under (b) only where another group names `<curl/curl.h>` directly.
- **N2**, a group naming `<curl/curl.h>` directly over the keg: builds today (critic, not rerun),
  and on every route.
- **N3**, a wrapper with `CPATH` pointing at the keg and `link "curl"`: refused today (critic).
  Still refused under (a), (c) and (d), rightly: its header is 8.23.0 and its library the SDK's
  8.7.1.
- **N4**, the reader's own `util.h` or `time.h` beside `main.hero`: refused on every route. This
  is the danger the rule exists for.

**Q3:**
- **d** and **g**: refused today (run). Accepted under the lane's sentence and under mine.
- **e**: builds today (run). Risks refusal under the lane's sentence as a replacement; accepted
  under mine.
- **The critic's danger shape**: refused in one order of the groups and accepted in the other
  (critic, not rerun). It stays order-dependent under the lane's sentence and is refused in both
  orders under mine.

## 6. Commands (from the worktree's root unless named)

- `grep -n '^### 1\.6' docs/design.md`, and `awk 'NR>=253 && NR<=440'` over it.
- `clang -I runtime seed/heroes.c runtime/runtime.c -o heroes`, then
  `./heroes build selfhost/main.hero -o heroes-next` and `./heroes-next --version`.
- `./heroes-next measure spec/heroes-spec.md`.
- For each `sentences/q1-*.txt`: `sentences/insert.sh spec/heroes-spec.md <s> <copy>`, then
  `./heroes-next measure <copy>` and `./heroes-next measure <s>`.
- For each `sentences/d-*.txt`: `./heroes-next measure <s>`.
- The temporary edit: spec copied out, V1+V2 written in, then
  `./heroes-next run tests/harness/main.hero -- ./heroes-next spec` (31 and 4), the spec copied
  back, and `git diff --quiet` checked.
- `clang -std=c11 -fsyntax-only -Wformat q1-wformat/fmt.c`.
- `heroes-next build main.hero -o <prog>`, then `timeout 10 ./<prog>`, in `q1-2421`,
  `q1-today/m1`, `m5`, `m6`, `m7`, `q2-2428` (with and without `PKG_CONFIG_PATH`), and `q3/d`,
  `g`, `e`.
- The compiler's bindings: `git grep -h '^extern "' -- 'selfhost/*.hero'` and
  `git grep -E '\.\.\.\s*\)' -- runtime/hero_os.h runtime/hero_compiler.h`.
- What the tree binds today: `git grep -n -E 'function (printf|...|ioctl)\('`.
- The spec greps of § 3.1 and § 4.

## 7. Registered predictions, each naming an instrument that exists today

- **P1, tokens.** If V1 lands as worded at § 13's anchor, `heroes measure` reads **8201 legacy
  and 8348 cl100k exactly**, and `--refresh` reads **10,925 to 10,950 real**. A real reading
  outside that range falsifies P1, and the row then owes a named removal. Scored at the landing
  by `heroes measure --refresh`, a paid run, on the author's yes.
- **P2, how far (R0) reaches.** Under (R0) with integer scope and no § 13 sentence, batch 30's
  gate B turns **exactly four** tracked programs from building to refused:
  - `run/ffi-variadic-at-a-fixed-arity`;
  - `run/fixedbugs-584-sprintf-with-fortification-off-builds-silent`;
  - `surface-fixtures/twoarity/as_int`;
  - `surface-fixtures/twoarity-lent/as_int`.

  A fifth falsifies P2. Scored by the full net at batch 30's gate.
- **P3, what V2 buys** (a paid run, queued for the author; scored at M-core-packages' close, or
  `lapsed`). The task: bind `curl_easy_setopt` for `CURLOPT_TIMEOUT` (a `long`) and
  `curl_easy_getinfo` for `CURLINFO_RESPONSE_CODE`.
  - Given the spec with no V2, at least **7 of 10** blind readers put a number or an `@` past
    `...` in their turn-1 program.
  - Given the spec with V2, at most **3 of 10** do.
  - The instrument is panel 217's launcher, `docs/panel/217-reports/llm-ergonomist-work/armR2/run_one.sh`
    (it exists; `ls` printed it).
  - If the first half fails, V2 does not land.

## 8. What would change the verdict

- **On the veto of (R)'s and (E)'s exception.** Either of these lifts it:
  - a program the closure list or §4.19's ladder needs that hands a number to a format function
    and cannot go through a wrapper;
  - a blind-seat measurement that readers' format calls with numbers cost more round trips
    under (R0) than V3's +52 vendored on every reading of the spec.

  Either one gives the exception § 2's ticket. Until then it waits, whatever its elegance.
- **On (R0) without a sentence.** P3's first half holding makes V2 owed, and I would approve it
  at +47 vendored on P3's payment.
- **On (R0) landing at all.** It must land in the same batch as a Q2 route under which N1 builds.
  Otherwise its note sends a reader with a keg-built libcurl to a header that is refused
  `shadowed_header` (run, § 4). That breaks §4.17's promise that a note carries a repair that
  works, so I would object to (R0) alone. The same holds for 2107's existing note over the keg,
  today.
- **On Q3.** I object to the lane's sentence unless it says `e` stays building. I object to any
  route that accepts the danger shape in either order.

## 9. Found beside the work, for the coordinator to read (none filed; I have no GitHub write)

- **The `shadowed_header` message for shapes d and g names the angle form**, *"stands in for the
  C library's `<sys/timeb.h>`, which `./lib/clock.h` includes"*. `clock.h` includes
  `"../sys/timeb.h"` in quotes. The 2428 message calls libcurl's header *"the C library's
  `<curl/curl.h>`"*. Searched with four phrasings (`shadowed_header "the C library" curl`,
  `stands in for the C library quoted`, `shadowed_header message curl.h C library`,
  `shadowed_header quotes angle message`): only 2428 (open) and 2420 (closed) came up. So when
  the synthesis files 2420's two rows (critic item 10), the message's wording belongs in them,
  whose class is the coordinator's to read: a false message is `blocking`.

## 10. What I did not do

- No `--refresh`, so every real figure is an inference.
- No implementation of any route in the compiler.
- No Linux or Windows run.
- No rerun of the critic's danger shape or its N2 and N3 builds.
- No `spec` suite run on V0 or V3 to V5.
