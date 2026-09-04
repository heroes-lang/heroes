# 108 — The unread cell is not the leak

Date: 2026-09-04 · **full panel, five judges** · convened from `/decide` on the
author's question the same morning (*"shall we put a rule in the compiler to
avoid these leaks?"*), after CI's Linux leg found `examples/ledger/` leaking.

Status: **RATIFIED 2026-09-04** (author instruction, *"ratify 108"*). See
§ Author's verdict.

Lane: **full**, and it had to be. The two seats that do not compile settled
two of the sitting's three questions: the ergonomist showed the habit is real
(3 of 3 first-try programs from the spec alone), and the historian showed that
every language with one in-out parameter mode counts the pass as a read, so the
proposal is a departure and not a catch-up. The three that compile then agreed
on the number that decides it.

## The proposal, as put

> `spec/heroes-spec.md:95-96` today: *"An unused binding or parameter is a
> compile error; a read is a use and a write is not, except through an `@`
> parameter."* Proposed: a cell passed as an `@` argument and never read by the
> program itself is `error[unused_binding]`; `_ = x` is the escape. Three
> wordings were measured (+13, +16, +20 tokens), and one narrower variant:
> the rule for `@` arguments of `extern` calls only.

Mechanism: `selfhost/resolve/writes.hero:72-79` (`inout_root`) records an `@`
argument as a **read** (copy-in) plus a **write** (copy-out). The proposal drops
the read.

## Why it was asked

`examples/ledger/db/sqlite.hero` declared `error: ptr @ nullptr`, passed
`@error` to `sqlite3_exec`, never read it and never freed it. SQLite allocates
that message for the caller. The Linux leg of CI reported `LeakSanitizer: 40
byte(s) leaked` and `main` went red; the development Mac cannot see it, because
LeakSanitizer does not exist on Darwin arm64 (CLAUDE.md §7). `examples/sqlite/`
carried the identical binding. Both were repaired the same morning by declaring
the parameter a plain `ptr` and passing `nullptr` (`fa01ab20`).

Three facts were established before the briefs went out. **No C warning can
catch the class**: both forms compiled with `clang -Wall -Wextra
-Wunused-but-set-variable` give zero warnings, because the slot is
address-taken. **The escape already works**: `x @ 0; x @ 1; _ = x` is accepted
today (exit 0), so the rule would arrive with its escape for free. **Panel 015
ruled the callee side only** (DESIGN-LOG:53, ratified :56): *"a write through
an `@` parameter is a use, because the copy-out always happens"* is about the
parameter inside `function f(@n)`; the caller side — that `f(@x)` reads `x` —
was never ruled and falls out of `inout_root`'s implementation, whose own
comment gives the reason (*"resolved the argument as a READ first (copy in)"*).

## The verdicts

| seat | full rule | `extern`-only | what it measured |
|---|---|---|---|
| compiler-engineer | **object** | approve on three conditions | a real prototype, rebuilt and run: **54 fires, 0 defects** |
| spec-warden | **object** | object | an independent scan: **55 fires, 0 defects**; the escape hides the leak |
| llm-ergonomist | **approve**, text B | — | 3 of 3 first-try programs from the spec alone pass an `@` cell they never read |
| ffi-pragmatist | **approve**, conditional on the message | **object** | `nullptr` is a crash in 5 of 9 real out-parameter shapes; `_ = x` is safe in all 13 |
| historian | approve as a *deliberate departure* (advisory) | — | GNAT `-gnatw.o` is the exact precedent and covers Ada `out` only; every in-out language counts the pass as a read |

No seat vetoed. Both objections said so explicitly: the compiler-engineer's
mandate does not reach a resolver counter rule, and the FFI seat found no ABI
ground (emitted C byte-identical, `HERO_RUNTIME_ABI` 20 unchanged, verified).

### What the compiler seat built

Variant A (the full rule): **+13 −2 lines in `writes.hero`**, nothing else.
Rebuilt via `--emit-c` (37.6 s) + clang (5.2 s), then `check` over
`selfhost/main.hero`, 44 corpus programs and the harness. **54 `unused_binding`,
54 distinct sites, 19 files.** Every site read:

| class | count | what they are |
|---|---|---|
| real defect | **0** | (the two `@error` leaks were repaired before the snapshot) |
| harmless, `nullptr` correct | 2 | the two `@tail` of `sqlite3_prepare_v2` |
| legitimate, needs `_ = x` | **52** | 44 in `test` blocks — `parse_expr(@c, @a, …)` then `assert` on the tree; the cursor is driven and never inspected — and 8 in functions: the `memo` caches of `cli/pointee`, `cli/toolchain`, `cli/produce`; the `seen` sets and counters of `ir/mono`, `ir/own`, `ir/inout`; a reader threaded through a loop in `examples/interpreter/` and `examples/pipeline/` |

So the 52 are **the language's own idiom** — §4.8 exists so a cursor can be
threaded by `@` — and the compiler goes from exit 0 to **48 errors in 15
files**; it would need 48 `_ = x` lines (it holds 26 today) to compile itself.
`_ = x` costs +5 tokens per site (`heroes measure`), +260 across the 52.

**A `fixedbugs` fixture gets worse.** `tests/golden/check/fixedbugs-at-marker-on-a-builtin.hero`
records a mistake a judge made writing `push(@lines, x)` from the spec alone
(2026-08-13, `docs/measurements/007`). Today it receives four
`marker_mismatch` lines with a `certain` fix; under the rule a resolver error
stops the checker and two `unused_binding` lines replace them, pointing the
reader at the wrong repair — a regression on design.md §4.17. Two more
`fixedbugs` fixtures move (`ffi-out-parameter-guard.hero:44-45`,
`ffi-unknown-name.hero:34`, whose `#~ ffi_unknown_name` is never reached).

**Variant C (`extern` only) fires 2** — the two `tail`s — and 0 in `selfhost/`.
It has a hole the seat measured: `"42".cstr().strtol(@end, 10)` is exit 0 under
C and `unused_binding` under A, because `names.method_name` resolves the callee
**after** the arguments (`walk.hero:145`). Closing it reorders a knot arm.

**`_ = error` compiles the leak clean** (`leak_escaped.hero`, exit 0). The rule
detects "never looked at", not "never freed".

The analysis is *read anywhere in the function*, not liveness; a cell read in a
loop condition and mutated via `@` in the body is exit 0 under both prototypes.
No unknown-name or shadowing diagnostic is lost. Compile time: no measurable
change over 15 interleaved runs.

### What the warden measured

Spec **3750** today (re-measured, both tokenisers, spread 81), headroom 346;
the three wordings +13/+16/+20; **W1** — *"…parameter; `f(@c)` does not read
`c`."* — **+11**, the shortest true wording found. Two removals nobody has
bought: `spec:15` (`##` is a heading, redundant with `:14`) **−8**, and
`spec:46` (mutual recursion needs no forward declarations, implied by
*"Declaration order never matters"*) **−9**, so W1 would land the spec at
**3744**, below today. Budget is therefore not the objection.

The objection is precision. Its own scan: **458** cells passed as `@` across
`selfhost/`, `examples/` and `tests/golden/run/`, **55 (12.0%)** never read
otherwise, hand-verified as correct programs. Four true fires in the history of
this repository against 55 false ones today — about 7%, the shape
`design.md:201` calls *"almost always a net loss"*, at 500–2000 tokens a
round-trip. And §1.12 does not carry it: a 40-byte leak is neither a segfault
nor a corruption.

Its condition — lift to approve W1 *"if a blind seat writing an FFI out-param
binding passes `@x` and never reads it in ≥ 2 of 5 attempts"* — **was met by
the ergonomist's experiment, 3 of 3**, before either seat knew of the other.
That convergence is recorded as it is: the habit is real (the ergonomist) and
it costs 55 false fires (the warden), and both have the measurement.

### What the ergonomist wrote from the spec alone

Three tasks, first try, identically under the base text and under wording A:
`strtol` without wanting `end`, `stat` as an existence test, `sqlite3_prepare_v2`
without wanting `tail`. In every one: declare an `@` cell, pass it, never
mention it again. The line that led there is `spec:213`, *"A C out-parameter is
an `@` parameter"*, which reads as a rule rather than one of two options. And
the confirming absence: **the word "free" does not occur in the spec**; nothing
says a `ptr` C wrote through may be the caller's, and no sentence at `:95-97`
could, because C does not distinguish `errmsg` from `pzTail`.

Given the refusal, its first reach is `_ = end`, correct in all three tasks. Its
second reach — a plain `ptr` and `nullptr` — is correct for two and **silently
wrong for `stat`**: `stat(path, NULL)` is −1 for every path, `exists` false for
every file, exit 0. Hence its condition: the message must name `_ = x` and must
never suggest `nullptr`.

On the wordings: **A can be parsed as its own negation** — *"…except through an
`@` parameter; passing a cell as `@` is a write"*: the pass goes through the
`@` parameter, so the exception applies, so it is a use. Two readers, two
conclusions. **B** (*"Passing a cell as an `@` argument does not read it"*) was
the only text of the three that changed its programs before compiling.

Debt found beside the question: the spec never shows a **named** `@` argument
(`out: @stmt`), which two same-typed parameters make mandatory, so its spelling
is a guess under every text.

### What the FFI seat compiled

Thirteen out-parameter shapes on Darwin and in the Linux image, each run with
the cell dropped both ways:

| function | `nullptr` accepted | with `nullptr` | which message leads right |
|---|---|---|---|
| `stat` as existence test | no | Darwin −1 `EFAULT` every path (`false false`, exit 0); **Linux segfault, exit 139** | `_ = x` only |
| `clock_gettime` | no | Darwin SIGSEGV 139; Linux −1 `EFAULT` | `_ = x` only |
| `getsockname`, `accept` with addr kept | no | −1 `EFAULT` | `_ = x` |
| `getline` `n`, `getsockopt` `optlen` (true in-out) | no | −1 `EINVAL` / `EFAULT` | `_ = x` only |
| `sigaction` old action, `strtol` end, `sqlite3_prepare_v2` tail, `sqlite3_exec` errmsg | yes | correct | either |

`nullptr` is safe in 4 of 9 real shapes and a fault or a crash in the other 5;
`_ = x` is safe in all 13 and leaves the emitted C unchanged. **The
compiler-engineer's proposed message arm — *"pass `nullptr` if the value is not
wanted"* — is refuted by this table and must not ship.**

`f(@x, x)` (the historian's question from roslyn #58564): emitted C is
`t2 = h0_x; f(&h0_x, t2);` — the plain argument is copied before the call, so
the rule cannot misjudge it. Ownership: `selfhost/keywords.hero` has no
`owned`/`borrow`/`frees`, and `design.md:2120-2125` says the vocabulary *"will
eventually be needed … reserve a keyword"*. **No `extern` can say today that a
`ptr` written through `@` is the caller's to free.**

Two things beside the question. (1) **A compiler defect**: `@value: i32`
declared against C's `void *optval` makes the pointee check emit
`_Static_assert(sizeof(void) == sizeof(int32_t) …)`, clang refuses it, and the
compiler answers `internal error … clang refused the generated C`, **exit 2** —
CLAUDE.md §7 says a failure the author's own `extern` caused is exit 1 on the
`.hero` line. Filed in `docs/work/DEFECTS.md`. (2) The `nullptr` route is a
**declaration** change, sometimes a module: two modules may hold both shapes of
one C function, one module may not (`declared_twice`).

### What the historian found

**GNAT `-gnatw.o`** is the exact diagnostic: since GCC 4.5 (2010) it warns when
a variable passed to an Ada `out`-mode formal is never read afterwards — and by
its own text **does not cover `in out`**. Heroes' `@` is copy-in copy-out, which
is `in out`. Every language with a single such mode counts the pass as a read:
Swift's checker marks `inout` `RK_Read|RK_Written` explicitly; Go's `go/types`
marks `&x` used by construction and no proposal to change it was found in three
searches; Zig's `_ = &x` is the blessed escape for a never-mutated local. C# gave
the unwanted out-parameter **syntax** (`out _`, C# 7.0), and its analyser's one
recorded failure is the `f(@x, x)` shape.

On the leak class: `sqlite3_exec`'s `errmsg` is an **open bug today** in colord
(#110) and darktable (#6051), both found by reading; and **no static analyser
catches it without ownership annotations** — clang's `ownership_returns` marks
return values only, MSVC's C6014 needs SAL, cppcheck's own sqlite model has the
fifth argument as `direction="out"` with no allocation metadata.

Two corrections to the brief, kept here because a brief's errors are part of the
record: the sentence attributed to golang/go#49214 was a paraphrase (the issue
says *"There is no use of `p` in this program but until now the compiler didn't
report an error"*), and *"in Rust `&mut x` is a use"* was asserted without a
source and is UNVERIFIED.

## Where the seats disagreed

**On the rule itself, two against two with the historian advising.** The two who
object are the two who **counted**: 54 and 55 fires on correct code, zero
defects, independently. The two who approve measured something else and both
say so: the ergonomist that the habit is real and its escape discoverable, the
FFI seat that the escape is safe at the boundary where `nullptr` is not. The FFI
seat's own argument concedes the point that decides the sitting: *"what it does
not do is what the brief sells: the leak was an ownership fact, `_ = error`
compiles it clean."*

**On the message, the compiler seat against the ergonomist, and the FFI seat
settled it.** *"Pass `nullptr`"* is a segfault on the Linux CI leg for `stat`.
The compiler seat's condition cannot be met as written.

**On the `extern`-only narrowing, the compiler seat against the warden and the
FFI seat.** It fires twice, both harmless; it has a measured UFCS hole; it gives
`@` a second meaning at the boundary; and the warden's locality objection stands
— *`extern`* is not on the line the reader is looking at.

## The resolution — provisional, and the robust one

**The rule is refused, in both shapes, and the refusal names its falsifier as
design.md Part 6's preamble requires.** Robust means leaving the fewest ways to
be wrong. The rule adds ways: 54 fires on the idiom §4.8 exists for, a
`certain`-fixable `fixedbugs` diagnostic degraded to a worse one, a spec
sentence that says a copy-in is not a read against §4.8's own *"copy in, copy
out"*, and — under the message one seat asked for — a reader steered into
`stat(path, NULL)`. It removes none: **`_ = error` satisfies it and the 40 bytes
still leak.** Here the conservative resolution and the robust one are the same
resolution, and this file says so rather than reaching for a change because a
sitting was held.

**What the question actually asked for is an ownership fact, and the sitting
names where it lives.** Every seat converged on it from a different direction:
the ergonomist (the word "free" is not in the spec and cannot be), the warden
(the escape hides the leak), the compiler seat (`leak_escaped.hero`, exit 0),
the FFI seat (no `extern` can state it; §4.19 owes a keyword), the historian
(every analyser that catches this needs an annotation). **The direction with a
Principle 0 argument is an `extern` declaration that states what C hands the
caller to free**, which `design.md:2120-2125` already reserves a keyword for.
That is a §4.19 sitting of its own, with its own falsifier and its own token
count; this file queues it and does not pretend to have held it.

**And the direction the historian points at is recorded for that sitting**:
Heroes has one `@` for C's two pointer roles. An out-only mode on `extern`
parameters — the callee does not read the incoming value — would make GNAT's
rule exact precedent and would be the natural carrier of an ownership mark. It
is new surface, so it enters only through Principle 0's thesis door with a
measurement, and not here.

**The instrument that catches the leak today is named, because the record
should say what works.** LeakSanitizer on the Linux leg found this defect, runs
on every corpus program under `--sanitize`, and is blind to nothing the rule
would have seen. The development machine cannot run it (CLAUDE.md §7). So an FFI
program's Linux run before its commit is the one that must include
`--sanitize` — CLAUDE.md § Commands' three-platform rule, sharpened by one flag,
and proposed to the author as an amendment rather than written by this sitting.

**What the refusal would be wrong about** — the falsifier, in two shapes: a
measured instance in `examples/` or `selfhost/` where an `@`-passed cell that is
never read is a **real defect** the rule catches and neither LeakSanitizer nor
the goldens do (0 of 54 today; the compiler seat's prediction is the standing
check); or an `extern` parameter mode that declares out-only, after which the
rule is GNAT's and right for that mode.

**Two corrections and one defect land with this file.**
`selfhost/emit/body.hero:141-143` says `_ = v` is *the only* way to reach a
written-never-read slot; `@error` reaches it without one, so the premise is
corrected (CLAUDE.md §11) — and the attribute it guards silences nothing,
measured: clang warns about neither form. The spec's missing named-`@`-argument
example is queued. The `void *` pointee exit-2 defect is open in
`docs/work/DEFECTS.md` with the FFI seat's reproducer.

## Predictions to score

| origin | prediction | instrument | scored at |
|---|---|---|---|
| compiler-engineer | if the full rule lands, `grep -rhE '^\s*_ = [A-Za-z_][A-Za-z0-9_]*\s*$' selfhost \| wc -l` reads ≥ 74 (26 today + 48), and `fixedbugs-at-marker-on-a-builtin.expected` loses its four `marker_mismatch` lines | grep + the golden harness | if it ever lands; otherwise stands as the refusal's cost |
| spec-warden | under W1 the rule fires on **55 ± 5** cells that compile today and the Linux `--sanitize` leg reports **0** leaks among them; the rule's marginal catch is 0 | `heroes check` + golden harness + Linux CI | M-corpus-depth close (falsified by ≥ 1 real defect among the 55) |
| ffi-pragmatist | with `@error` **plus `_ = error`** reinstated in `examples/ledger/`, the Linux `--sanitize` leg reports **40 bytes leaked** and `unused_binding` is silent — the rule sees nothing the leak detector sees | the Linux CI leg | the next step of M-ffi-ladder |
| llm-ergonomist | under the base text ≥ 80% of first-try `strtol` programs from the spec alone declare `@end` and never read it, and 0% write `_ = end` | a Part 11 reader harness | first Part 11 run |
| historian | run the check over every accepted program and classify: (a) C out-only never read, (b) true in-out consumed by the callee, (c) read (must be 0). (a) ≥ 1 and (b) ≥ 1; if (b) > (a) the robust fix is a declared out mode | `heroes check` in a copy | the §4.19 ownership sitting |
| coordinator | the §4.19 ownership sitting, when held, prices its keyword at **≤ +25 spec tokens** and its falsifier is a bound library whose caller-owned out-parameter the compiler then refuses to drop | `heroes measure` + the sitting's own compiled binding | that sitting |

**Scored 2026-09-04, at panel 109**: the coordinator's row above — *the §4.19 ownership sitting prices its keyword at ≤ +25 spec tokens* — is **FALSE**. Panel 109's spec-warden measured the floor for the three facts the mark must state at **+49** (`heroes measure`, cl100k binding), the wording the sitting proposes at +70 gross and +42 net of a named removal. The instrument was the one named; the number was a hope.

## Author's verdict

**RATIFIED 2026-09-04** (author instruction, *"ratify 108"*, given while the
CI run for the synthesis commit was still in flight — the author asked the
same morning for the sitting's question and accepted its answer the same day).

**What the yes settles is a refusal with a named falsifier**, the second in one
day after panel 107, and the same shape: a `/decide` item recommended a change
from the cases that provoked it, a sitting measured the population the change
would judge, and the recommendation did not survive the count. The rule that a
cell passed as `@` and never read is `unused_binding` is refused in both shapes.
It comes back only through its falsifier — a measured real defect among the 54
sites it fires on today, or an out-only `extern` parameter mode, after which the
rule is GNAT's and correct for that mode.

**What it authorises**: the §4.19 ownership sitting, when someone convenes it,
carries the author's word that the question is the right one — an `extern`
declaration stating what C hands the caller to free, with the historian's
out-only mode recorded as its likely carrier and `design.md:2120-2125`'s
reserved keyword as its budget line. And defect 010 is the author's to see
repaired, not a finding to argue about.

**What it leaves to the author's own hand**: the CLAUDE.md § Commands
sharpening — an FFI program's pre-commit Linux run includes `--sanitize`. The
contract is amended by author instruction (CLAUDE.md §4), so the sitting
proposed the sentence and the author writes it; it is recorded in the same
`/decide` exchange as this ratification.

**And the record keeps what the ratification says about the process.** Two
sittings in one day each overturned the recommendation that convened them, and
each recommendation was honest and measured — on a sample of one machine, or of
four sites. Neither would have been caught by a commit. That is the cost of a
sitting stated as what it bought, twice.

## What the lane gave up

Nothing — this was the full panel. What it cost: five seats, three of which
compiled, one relaunched after an API limit killed it at its first step. What
the two non-compiling seats bought is at the top of this file.
