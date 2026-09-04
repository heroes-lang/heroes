# Panel 039 — compile-time evaluation, and what Part 6 is for

**Convened** 2026-08-12, by author decision in `/decide` (the item offered
*convene now* / *convene at M-selfhost-probe* / *do not convene*; the author took
the first).

**Status** `RATIFIED 2026-08-15` (was `provisional — author ratification pending`).

**Lane** full panel, five judges. The soundness lane was considered and refused:
the change costs zero spec tokens and emits no code, which is the lane's
criterion — but the question is *why* a mechanism is refused, and the two seats
that decided the outcome were the blind one and the sourced one. The lane would
have lost both.

**Origin** `docs/reasoning/003-comptime-and-macros.md` § What stayed open, which
named the number 039 and the panel path (CLAUDE.md §4: design.md Parts 1–11).

---

## The proposal, verbatim as put to the judges

design.md **Part 6 is "Rejected permanently"** (preamble, design.md:2126: *"Do
not add these. Each violates locality, and each is also expensive to
implement"*). **Part 7 is "Deferred, in order"** (preamble, design.md:2159:
*"These lose only on the *simplicity* vertex, which means they are postponed
rather than refused. The distinction matters and depends on which vertex said
no."*).

Part 6 carries one row, at design.md:2136:

    | Macros | the code you read is not the code that runs |

The words `comptime`, `const-eval`, `constexpr` and `CTFE` appear in the entire
2500-line design.md exactly **once**, at design.md:1833, and there only as a
passing remark that Zig got an answer right by accident. So compile-time
*evaluation* is named nowhere as rejected, deferred, or accepted.

- **A** — extend the Macros row to name compile-time evaluation explicitly,
  keeping it in Part 6 (permanent rejection). Zero spec tokens.
- **B** — a new Part 7 entry instead: comptime is *deferred*, not rejected,
  because it loses on the simplicity vertex rather than the locality one.
- **C** — leave the silence. The row's stated reason already covers it.

---

## The verdicts

| judge | verdict | veto | cost / delta | the one sentence that carries it |
|---|---|---|---|---|
| **compiler-engineer** | **A-amended** — its own row, its own reason | **VETO on B** | floor **~1,500 new lines** across ≥8 modules, realistic 2,000+, plus edits in the 33 files that match `Op::`; against 533 for generics | *"Comptime deletes nothing"* — §1.7's test is subtraction, and generics passed it (533 lines bought the deletion of seven checker cases into 149 lines of Heroes) |
| **llm-ergonomist** | **object** to any added construct · **approve** one restrictive sentence about a `constant`'s body | **VETO, narrow**: any construct whose evaluation time depends on its call site — *including a `constant` body permitted to call an ordinary function* | first-try 2/3 under the current spec, 2/3 under its own remedy — *"B's gain is in silent-error rate, not first-try rate"* | *"I never wanted compile-time evaluation. I wanted to know what a constant's body may contain."* |
| **spec-warden** | **object**, veto-grade against **A**; ranks B-in-item-14's-shape ≈ C+greppability > C > A | veto-grade on A | measured **before = after = 2627**, Δ **0** for all three; refuses three candidate spec sentences at +13, +19, +37 | *"§1.6 prices the prompt, §1.2 prices programs; a design.md row is in neither, so both instruments read zero and neither can license this"* |
| **ffi-pragmatist** | **A** · objects to **C** | none — nothing touches the ABI (`HERO_RUNTIME_ABI` stays 10) | four canonical header shapes compiled against the real macOS SDK; **0** symbols across `curl.h`, `sqlite3.h`, `fcntl.h`, `sys/stat.h`, `arpa/inet.h`, `sys/mman.h`, `stdint.h` need evaluation | *"M-header-constants answered the strongest comptime case by computing nothing"* |
| **historian** | **B** + narrow the Macros row | none (advisory seat) | D's CTFE ate **>16GB on one `std.regex` pattern**; its ~12k-line replacement never merged | Kelley built comptime so macros would be *"unnecessary"* — so the Macros row's reason is **false** of comptime and true of `mixin` |

**No option survives.** A is refused by the warden at veto grade and objected to
on wording by the engineer; B is vetoed by the engineer; C is objected to by the
ffi-pragmatist and the historian. This synthesis therefore adopts none of the
three as put — see § The resolution.

---

## What all five agree on

Four seats reached this by four different methods, which is the only kind of
agreement this panel is built to produce:

1. **Nothing needs it.** The closure list (design.md:134-147) has no
   metaprogramming row. The ffi-pragmatist compiled four canonical header shapes
   and found zero blocked. Measurement 005's digit class went 5 sites → 0 via
   panel 038's `extern constant`, with no evaluator. The ergonomist, writing
   three programs that a comptime language would have used it for, never reached
   for it — and named the reason: spec line 3, *"This document is the whole
   language"*, closes the world, so there was no keyword to guess.
2. **The Macros row's reason does not describe comptime.** Engineer, historian and
   warden, independently. In `constant MAX: int` / `10000 + 2` the code you read
   **is** the code that runs; it merely runs earlier. The historian sources the
   fusion claim to its author and finds it runs the other way: Zig has comptime
   *so that* macros are unnecessary, and matklad enumerates three things comptime
   provably cannot do — generate source, take custom syntax, add declarations.
3. **A quota is the wrong answer to termination, and there is a right one.** The
   historian's contribution, and nobody else had it: a CTFE restricted to
   structurally-recursive functions terminates **without** a quota (Agda, Coq),
   hence without Zig's defect class — an error whose fix belongs in a scope other
   than the loop (ziglang/zig#1767, #12624), which generic types bypass into a
   compiler segfault (#21324, #22842). Rust shipped a bound and **removed it** in
   1.72 (2023-08-24). The engineer's fork closes it: structural termination ⇒
   straight-line folding only ⇒ zero measured benefit, and CLAUDE.md §13 forbids
   speed as a reason; any useful power ⇒ a quota ⇒ contradicts `ir/mono.rs:24-33`,
   which refuses exactly that answer to exactly this question.
4. **Part 6's preamble is already false of part of its own table.** The engineer
   found three cost-only rows; the count is **four** — `Subtyping`,
   `Higher-kinded types, dependent types`, `Coroutines`, `Metatables / dynamic
   dispatch` — all sitting under *"Each violates locality"*.

---

## The disagreements, unsmoothed

**The warden against the engineer and the ffi-pragmatist, on A.** The warden's
objection is not that A is unproven but that it is **refuted**, and it produced a
compiler fact to say so: `ir/mod.rs:96` reserves constant folding as *"the
emitter's optimisation, not the IR's concern"*, and every emitted unit — verified
on `function main()` / `print(1)`, with no `extern` anywhere — carries two
`_Static_assert(__builtin_constant_p(…))` from the library's own group. A row
forbidding compile-time evaluation would be false of the compiler's own output on
the day it was written. The engineer and the ffi-pragmatist read the row as being
about *Heroes-level* evaluation of *Heroes* code, where the claim is true. **Both
readings are correct about different sentences, which is the warden's point**: the
one-line form Part 6 admits cannot distinguish them, and the reasoning note needed
a seven-row table to.

**The engineer against the historian, on B.** The historian wants Part 7 because
Go's hedged FAQ wording aged well and Python's flat rejection did not — PEP 3103
still reads `Rejected` about a language that shipped the feature under another
name. The engineer vetoes Part 7 because its preamble *asserts* the item loses
only on simplicity, and that is false here: comptime also contradicts a decision
this compiler already enforces with a diagnostic, and duplicates 1,754 lines of
runtime semantics with no `_Static_assert` between the two copies. Those are
self-consistency failures, not schedule costs — **they do not get cheaper after
the fixpoint**, which is what Part 7 membership would imply.

**The warden against the item that convened this panel.** The DECIDE entry said
the cheapest outcome was *"one Part 6 row at zero spec tokens"*. The warden
refuses the premise: *"'costs nothing to write' is not 'costs nothing'"* — A
spends optionality permanently at a price no instrument in this project can show,
and Part 7 item 5's reversibility argument is the record's own template for why
that is the expensive currency.

---

## Two findings the question did not ask for, and they outrank it

### 1. A `constant` in Heroes is not constant — DEFECT, open

The ergonomist's veto is not hypothetical. It named a construct whose evaluation
time cannot be read off the line, and gave `build_powers()` in a `constant` body
as the case it could not resolve from the spec. Run against the compiler, the
answer is neither of the two it considered:

    constant ARGC: int
        len(args())

    function main()
        print(ARGC)

    $ heroes run varying.hero -- a b c
    3
    $ heroes run varying.hero
    0

Same binary, two values. And the body is re-evaluated **on every read** — a
`constant` whose body calls a function that prints, prints once per read.
`ir/mod.rs:90-97` is explicit that a `constant` lowers to a zero-argument
function and a read to a call, so this is the documented design working as
built; what nothing checked is that the body may then reach `args()`,
`read_file`, or `print`.

Three rules this crosses at once. §4.2: *"There are no mutable globals"* — a
value that varies with argv is one, wearing SCREAMING_CASE. §4.2 again:
*"Constants use SCREAMING_CASE"*, which promises the reader something the
compiler does not enforce. And the spec never says what a body may contain: its
only example is a bare literal, and the **one** sentence in the document using
the phrase is the FFI's *"A group's `constant` has no body"*.

The ergonomist approves one restrictive sentence; the warden priced it at **+19**
and refuses it *now*, returning it to the table only on a Part 11 run showing
≥10% of models write an illegal body. That disagreement is about the **spec**.
The defect is about the **compiler**, and no seat defended the current
behaviour.

### 2. `+` where C means `|` is a silent wrong answer — two seats, opposite methods

The ergonomist, writing blind, spelled a flag union `FLAG_READ + FLAG_WRITE +
FLAG_EXEC` and flagged its own premise: *"sum == or, only because bits are
disjoint — the spec gives me no help noticing that"*. The ffi-pragmatist compiled
the case where they are not disjoint and measured it: `O_RDWR|O_ACCMODE` is **3**
and `+` gives **5**; `S_IRWXU|S_IRUSR` is **448** and `+` gives **704**. §4.14
reserves `& | ^ << >> ~` and v1 implements none, so `+` is the only reachable
spelling and it is wrong on every mask whose bits overlap. Comptime does not
unblock this; an operator does. The ffi-pragmatist's prediction turns on it.

---

## The resolution — provisional, author ratification pending

**None of A, B or C is adopted.** Each carries an objection this synthesis has no
standing to overrule, and the conservative resolution is the one no seat vetoes:

1. **Repair the Macros row's reason** — narrow design.md:2136 to name what it
   actually rejects: user code that expands into other code, or that extends the
   syntax. This is a **correction, not a decision**: three seats independently
   found the current reason false of comptime, and the historian sourced it. No
   seat objects to a repair.
2. **Write no rejection and no deferral.** A is refused (warden, veto-grade) and B
   is vetoed (engineer). Comptime is therefore **examined and unplaced**, which is
   the honest state and is what the record will say.
3. **Make the word greppable**, which is the warden's own condition for C and the
   historian's answer to silence: one line in Part 6's neighbourhood pointing at
   this sitting and at reasoning 003, saying the question was examined, that
   nothing on the closure list needs it, and naming the three conditions under
   which it returns (a measured Part 11 effect · a structural termination
   argument of `mono.rs:24-33`'s standard · a named deletion). Under CLAUDE.md §1
   an answer that lives only in `docs/reasoning/` is uncitable, so the next
   session greps `comptime`, hits design.md:1833's Zig remark, and correctly
   concludes nothing is settled — which is how this panel got convened.
4. **The `constant`-body defect is queued as a defect**, with its reproducer, and
   the spec sentence stays unbought pending the warden's Part 11 condition.
5. **The bitwise gap is queued**, with the two measured wrong answers.
6. **The warden's proposed rule is queued for the author, not adopted here**: *a
   Part 6 row must be falsifiable by the compiler* — it must name the program or
   the compiler fact that would make it wrong. It is a real gap (Principle 0 binds
   only what *enters*, so a permanent rejection is the one design act under no
   stated burden of proof), but amending CLAUDE.md or design.md Part 0 is author
   instruction, and this panel was not convened on it.

### What a veto would compel

- **Overturning to A** compels the engineer's amendment — its own row, its own
  reason — and compels answering the warden's refutation: the row must be written
  so that it is not falsified by the two `__builtin_constant_p` assertions in
  every unit this compiler emits.
- **Overturning to B** compels amending Part 7's preamble, because the engineer's
  veto is against the preamble's claim and not against the entry. Part 7 currently
  asserts its items lose *only* on simplicity; a comptime entry would need the
  preamble to admit a second reason, or a stated exception.
- **Overturning to C** compels nothing mechanical and accepts the historian's
  prediction as the cost: a second compile-time-evaluated value form arrives
  before the fixpoint as a flag, a subcommand, or an extension to `extern
  constant`, because §10 governs tool surface and Part 6 governs semantics and
  neither names it.

---

## Predictions to score

| # | judge | prediction | checkable at |
|---|---|---|---|
| 1 | compiler-engineer | the probe's blockage list contains **zero** items a compile-time evaluator would close, and every `constant` in the ported lexer has a body that is a literal or scalar arithmetic over literals — zero requiring a call to be evaluated | **M-selfhost-probe** |
| 2 | llm-ergonomist | under the current spec, **≥60%** of Task-1 first attempts write the values as literals with no compile-time construct invented, and the dominant failure is array-literal spelling, not evaluation time (falsified if ≥25% invent a `comptime`/`const fn`/macro spelling) | first Part 11 run |
| 3 | llm-ergonomist | **≥30%** of Task-2 attempts avoid a derived constant — hard-coding `10002` or routing through a function — producing **correct stdout with the wrong structure** | first Part 11 run |
| 4 | llm-ergonomist | adding a `comptime` construct instead: **≥40%** of Task-1 attempts use it, Task-1 first-try does **not** improve, and Task-2/3 first-try **drops** | only if a construct is ever proposed |
| 5 | spec-warden | `SPEC_TOKENS` moves **+0** on account of comptime or macros, **and** the record shows **≥1 further reopening** whichever of A, B or C lands | **M-selfhost-fixpoint** |
| 6 | ffi-pragmatist | at M-ffi-ladder's next rungs, constants needing compile-time evaluation = **0**, needing a C shim = **0**, needing a bitwise operator = **≥1**; the first genuinely blocked binding is a flag *composition*, `sqlite3_open_v2(…, SQLITE_OPEN_READWRITE\|SQLITE_OPEN_CREATE, …)` | next FFI rung |
| 7 | historian | if C is adopted, a second compile-time-evaluated value form is proposed before the fixpoint and arrives as a **flag, subcommand, or `extern constant` extension** — not as a language feature | M-selfhost-fixpoint or 2027-02-12 |

Prediction 5's control arm is already on the record and the warden named it
against itself: macros are refused in Part 6, in CLAUDE.md §13 and in CLAUDE.md
§6 *by implementation*, and were reopened anyway on 2026-08-12 — in the same
sentence as comptime. **Observed effect of the proposed cure on the one observed
event: zero.**

---

## Corrections owed, found by judges

- **The warden corrected its own brief**: §1.6's ceiling has been **4096** since
  panel 024, not 3000. A warden carrying a remembered budget number is the exact
  failure §1.6 documents, and it said so rather than let it ride.
- **The ffi-pragmatist reported two results against its own prediction**: it
  expected `__builtin_constant_p(SQLITE_TRANSIENT)` to return 0 for a cast of
  `-1` to a function pointer, and it returns 1; and it expected the `void *`
  accessor over a function pointer to be an error, where under §7's flag set it is
  clean (`-Wpedantic` only — a second reason `-pedantic-errors` waits).
- **The brief's own premise was false** and the ffi-pragmatist said so: a
  function-like macro is *not* the case `extern constant` cannot reach. §4.19
  never re-declares the signature, so the preprocessor expands the macro and
  `_Generic` type-checks the expansion — `CURL_VERSION_BITS` and Darwin's `htonl`
  both bind as `extern function` and ran.
- **`docs/reasoning/003` § What stayed open** still lists the mutually-recursive
  `constant` as undiagnosed. It was closed by `7ca621d` the same day, after the
  note was written. CLAUDE.md §14 makes that an **append**, never a rewrite.
- **The historian marked its own unverified claims** rather than dropping them
  silently: whether Jai has macros distinct from `#run`; Zig's exact langref
  sentence for `@setEvalBranchQuota` (the number 1000 stands on the error string
  in ziglang/zig#11996); N3065's paper number; and Nim's `importc`-constant
  precedent, which it read from CLAUDE.md §6 rather than searching.

## Ratification — 2026-08-15, blanket author instruction

**RATIFIED as it stands** (author instruction, `/decide`: *"le ratifiche ratifica
tutto per me"*). All three parts of the resolution are the decision: the Macros
row's reason is narrowed to what it actually rejects, **no rejection and no
deferral is written** — comptime is *examined and unplaced* — and the word is
greppable, so the next reader who has the idea finds the sitting instead of
re-litigating it.

**What this ratification is worth beyond its own subject**: panel 056 reached the
same shape of answer for a project file eleven days later and cited *this* sitting
as its precedent (*"examined and deliberately unplaced — neither Part 6 nor Part
7"*). Ratifying 039 therefore ratifies the precedent 056 stands on, and the two are
now ratified in the same instruction, which is the honest order — the precedent
should not be provisional under a decision that leans on it.

The limit is the 2026-08-12 blanket's limit: a yes settles that the resolution
**is** the decision, not anything keyed to a measurement not yet taken.

## Scored at M-selfhost-probe close (2026-08-15)

- **Prediction 1 (compiler-engineer): CONFIRMED.** The probe's blockage list
  (docs/measurements/009, twelve entries) contains **zero** items a
  compile-time evaluator would close: the gaps are a decoder's width, an
  overflow guard, Option/tuples/no-ops as shapes, and two missing
  spellings (`\r`, byte-to-str) — none is a constant a comptime could fold.

## Appended 2026-09-04 — the seven capabilities, moved here from the retired reasoning note

The note this sitting was convened from was retired on 2026-09-04 by author
instruction, with the rest of `docs/reasoning/`. Its text stands in the git
history at `git show 8d68fa55:docs/reasoning/003-comptime-and-macros.md`. **Two
blocks move here rather than into the record**, because two living documents cite
them by line — `docs/ROADMAP.md` and `docs/work/SCHEDULED.md` both send a reader
to the C4 row for the only place reflection's refusal is written down — and
because `design.md`'s Part 6 paragraph calls this file's long form the answer to
*"an answer reachable only from a reasoning note is uncitable under CLAUDE.md
§1"*. Leaving them in a deleted file would have made that sentence false about
itself. The line numbers the citations used are `003:89` for the C4 row and
`003:194` for nothing in this file; the wording is unchanged.

### Seven capabilities under one word

| | capability | what Heroes has | what actually refuses it |
|---|---|---|---|
| C1 | fold a constant expression | the surface, unrestricted; no evaluator | `CLAUDE.md` §13 + panel 037 — an optimisation with no measured need. **Part 6 does not touch it** |
| C2 | run a function at compile time | nothing | Principle 0. **Part 6 does not touch it** |
| C3 | types as compile-time values | §4.12 generics, spent the other way | §4.12 + panel 029. **Part 6 does not touch it** |
| C4 | reflection over types | field-walking `eq`/`hash`, generated, at zero surface cost | Part 6 — but the **Ruby row**, not the Macros row |
| C5 | conditional compilation | `runtime/hero_os.h` absorbs target differences | Part 6's Macros row, literally |
| C6 | macros proper | nothing, refused three deep | Part 6's Macros row. **Zig has no macros either** |
| C7 | read a header at compile time | delegation to clang: panel 036 for signatures, panel 038 for values | already answered, at +28 spec tokens |

The column that matters is the last: **for four of seven, the thing that refuses
it is not Part 6.** Answering the author's question with "Part 6 says no" would be
right about C5 and C6, wrong about C4's reason, and simply unresponsive about C1,
C2, C3 and C7. That is the taxonomy this sitting's § What all five agree on
records as needing seven rows where a one-line Part 6 form admits one.

**The C4 row is the citation two living documents depend on**: reflection over
types is refused by Part 6's Ruby row and not by the Macros row, which is why
`M-reflection-verdict` was scheduled as a milestone whose deliverable is a
ruling. The Ruby row's own line number is deliberately not repeated here — it has
moved twice since the note was written, and `grep` reaches it.

### And the measured pain is disjoint from C1 and C2

Against `docs/measurements/005`'s target — the `typo-digit` operator, **5 mutants
found and 0 killed**, all five of them C header values hand-copied into Heroes —
the alternatives are:

| answer | sites | spec Δ | new pass | new evaluator |
|---|---|---|---|---|
| write `10000 + 2` in today's body | 5 → 5 | 0 | no | no |
| panel 038's `extern constant` | **5 → 0** | +28 | no | no |
| a restricted (`const fn`-shaped) evaluator | 5 → 5 | > 0 | yes | yes |
| do nothing | 5 → 5 | 0 | no | no |

The third row is the finding. **A restricted evaluator closes none of the five
sites**, because the number is still hand-copied: the authority a header holds is
not something an evaluator can consult. So C1 and C2 and the measured pain are
**disjoint**, which is the sentence `design.md`'s Part 6 paragraph compresses into
*"§4.19's `extern constant` answers that by computing nothing"*. Re-measured at
`2ac0403` after the merge: `typo-digit` **0 sites**, corpus 1252 mutants, 93% /
78% — sites 5 → 0, the number `docs/measurements/005` asked to be reported rather
than the rate.
