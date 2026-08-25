# Panel 092 — the check that a macro was hiding

**Convened** 2026-08-25 by author instruction (*"per la spec fai la cosa più
robusta a costo di prendere token di spec"* — do the most robust thing, and spec
tokens may be spent). **Trigger** a diagnostic *class* and a sentence of
`spec/**` (CLAUDE.md §4). **Lane** full panel: five seats, differentiated inputs.
**Status** `provisional — author ratification pending`.

**The repair is two production lines, and the sitting's real work was finding out
that almost everything the convener believed was false.** Four of the five seats
overturned a premise of the brief; two overturned each other; and the convener's
own diagnosis of the cause was wrong twice in a row.

## The proposal, verbatim

> `spec:204` says *"A **parameter** and a **field** are declared at the header's
> own width and sign — `i32` where C says int."* **The compiler enforces exactly
> half of that sentence.** A field at the wrong width, sign or kind is exit 1,
> all three; a parameter at the wrong width or sign is **exit 0** — `memset(n:
> i32)` against `size_t` builds and runs, `pow(x: f32, y: f64)` against `double
> pow(double, double)` builds, runs and prints the right answer.
>
> **P0** nothing · **P1** a clang warning the author's own `extern` caused
> becomes exit 1 on the `.hero` line · **P2** P1 plus `-Werror` on those warning
> families · **P3** the sitting's own.
>
> Spec: **S0** nothing · **S1** narrow it to what is checked · **S2** keep the
> rule and state the hole · **S3** the sitting's own.

## The resolution

**P3 + S2, and the convener's framing discarded.**

1. **Parenthesise the probe's callee** — `(void)(f)(a0, …)` rather than
   `(void)f(a0, …)`, two production lines at `selfhost/emit_extern_probe.hero:82`
   and `:90`. Built and run by the compiler-engineer: `heroes test
   selfhost/main.hero` **492 passed, 0 failed**, and `memset(dst: ptr, value:
   i32, n: i32)` goes **exit 0 → exit 1** with the full diagnostic and its fix.
2. **`(void)`-prefix the discarded extern result** at `selfhost/emit_ops.hero:99-108`
   — the compiler's own warning on a correct program, today routed to the
   author's line.
3. **Spec S2**, not S1: the parameter promise is **kept by value**; what the
   mechanism cannot see is a **pointee**. Plus the spec-warden's one measured
   row, `size_t` → `u64`, at **+11 tokens**.

**P1 and P2 are vetoed. The disjunction is falsified by compiling it.**

## The verdict table

| judge | verdict | section | its own finding |
|---|---|---|---|
| compiler-engineer | **veto on P1, P2 and the disjunction** · P3 only | §4.19, §1.1 | **it is not builtins, it is `_FORTIFY_SOURCE`** — and the disjunction rejects **16 of 25** real bindings, including the compiler's own `getenv` |
| ffi-pragmatist | **veto on P2** · object to P1 | Part 6 row 10, §4.19, §1.11 | P2 stops `spec:207`'s **own example** from building; and `(&f)(...)` finds a **live defect in `examples/curl`** |
| llm-ergonomist | **veto on S0 and S1** | the thesis, locality | answered the brief's first question **wrong at 8/10 confidence**, having *noticed* the truth and talked itself out of it |
| spec-warden | **object** — S1 and S2 refused as briefed, **S3e at +11** | §1.4, §1.12 | **the sentence is not false**: `parameter` is absent from the checked list, and the document already draws the measured line |
| historian (advisory) | **accept-with-conditions**: P1 + S2 | precedent | **nobody checks a hand-written parameter width inside C**; cffi documents Heroes' exact behaviour verbatim as intended semantics |

## What the sentence actually says, and why five reads of it disagreed

The spec-warden read the words rather than the claim:

> clang checks every **result type**, **constant** and **record field** against
> that header … A **parameter** and a **field** are declared at the header's own
> width and sign

The checked list is *result type, constant, record field*. **`parameter` is not
in it.** The next sentence states a **declaration obligation on the author**, not
a promise of a check. So the convener's *"the compiler enforces exactly half of
that sentence"* was **false**: document and compiler agree.

**And the llm-ergonomist proved the sentence is the problem anyway.** Given only
the spec, asked what happens if a parameter's width is wrong, it answered *"a
compile error, exit 1"* at **8/10 confidence** — and reported that it had
**noticed** the two lists differ and talked itself out of it, *"because the next
clause reads as one continuous rule and because nothing in the document
acknowledges the gap."* Its sentence for the record: **a fact present only by
omission is a fact a model reading at speed will not have.**

That is the whole case for S2 over S0: the sentence is *true* and *misleading*,
and the two are not in tension.

## The cause, wrong twice before it was right

**The convener's first diagnosis**: parameters are unchecked. **False** — the
ffi-pragmatist measured `sqlite3_sleep(ms: i64)` → exit 1 *"wider than the
header's `int`"*, `(ms: u32)` → exit 1 *"a different sign"*, `(ms: i32)` → exit
0. `spec:204`'s parameter half **is enforced, width and sign**, by
`-Werror=shorten-64-to-32` and `-Werror=sign-conversion`, which
`selfhost/cli_flags.hero` has shipped all along. The convener had measured with
`-Wall -Wextra` instead of the compiler's own flags.

**The convener's second diagnosis**: clang folds its builtins. **Also false**,
and the compiler-engineer killed it with a table:

| caught today — all clang builtins | silent |
|---|---|
| `strncmp` `malloc` `calloc` `abs` `fwrite` | `memset` `memcpy` `strcpy` — **fortify-rewritten** |
| | `pow(f32,f64)` `sqrt(f32)` — **lossless widening** |

`clang -E` names it, and the coordinator reproduced it:

```c
(void)__builtin___memset_chk (a, b, c, __builtin_object_size (a, 0));
```

**A `_FORTIFY_SOURCE` macro, not a builtin.** `-fno-builtin` cannot help,
because a macro is doing the redirect — which is exactly why the coordinator's
`-fno-builtin` experiment came back empty and was misread as proof that warnings
do not work. `-D_FORTIFY_SOURCE=0` restores all three; **so does parenthesising
the callee**, because `(memset)(…)` is not a function-like macro invocation.
Verified independently by the coordinator: `strncmp` and `malloc` error,
`memset` is silent, `(memset)` errors.

## Why P2 is vetoed — the wall is the language's one pointer type

Heroes has **one** pointer spelling. `widths.hero:90-99` runs `int8_t` to
`uint64_t` and has no `size_t`; `@` on a `ptr` emits `void **`. So under
`-Werror=incompatible-pointer-types`:

- `examples/sqlite/main.hero` produces **12 errors** — and that program is
  `spec:207-211`'s own worked example;
- `getline(@lineptr: ptr, @n: u64, stream: ptr)` becomes unbindable: `u64` is the
  **only** spelling available and it meets `size_t *`;
- the ffi-pragmatist counted **40 functions with a `T**` parameter** across seven
  stock headers.

Its sentence: *"that is not a strict compiler, it is an unbindable library."*
And the compiler-engineer's independent form: `spec:206`'s own `sqlite3_open`
**warns on a correct binding today**, so P1 routes that warning to the author and
P2 turns it into an error.

**design.md Part 6 row 10 already ruled this**, and it named its own falsifier:
*"a program the closure list or §4.19's ladder needs, binding a C `long`,
`size_t` or `unsigned long` **parameter** … Produce one and this row is wrong."*
The ffi-pragmatist's `getline` **is** that program, and it says so — which turns
a refusal into a dated, falsifiable claim rather than a preference.

## The disjunction, proposed by one seat and falsified by another

The historian, having explained why exact comparison failed — on Darwin ARM
`__uint64_t` is `unsigned long long` while `__darwin_size_t` is `unsigned long`,
same width, same sign, **different canonical spelling, and there are only two** —
proposed asserting a **disjunction** over the small substitution set, and said
plainly that it had not compiled it.

The compiler-engineer compiled it. `memset(n: i32)` fails ✓, `pow(f32,f64)`
fails ✓, and **16 of 25 correct bindings are rejected** — all seven `sqlite3_*`,
`getenv`, `fopen`, `fclose`, `fputc`, `fseek`, `strstr`, `strerror`,
`curl_version`, `curl_easy_setopt`. Two causes, neither fixable by adding
spellings: typed pointers and enums (`struct sqlite3 **` against `void **`), and
**§4.19's own widened result** — an `int` header with an `i64` declaration is
legal by the spec's own sentence, and whole-type identity refuses it. The
cross-product explosion the historian feared is **not** what fired: at most 2
terms across those 25.

**It rejects the compiler's own `getenv`**, so it fails Principle 0 outright.

## What is still not checked, stated plainly

- **A by-value integer widening** — `i32` where the header says `long`, `i16`
  where it says `int` — is silent under **`-Weverything`**, measured. This is the
  sentence S2 must be written around.
- **The float half is NOT unreachable**: `-Wdouble-promotion` catches
  `pow(x: f32)`. The compiler-engineer's live candidate is that warning **scoped
  by `#pragma clang diagnostic error` around the probe block**, and it measured
  that ordinary generated float code outside the pragma stays silent. It approves
  that today given one more measurement (below).
- **A pointee's width** — `int32_t *` against `size_t *` — carries the same bare
  `-Wincompatible-pointer-types` as the benign `void **` → `sqlite3 **`, so no
  rule can separate the corrupting case from the correct one until Heroes has a
  typed pointer.

## Precedent: this is the fork's price, not a defect

**Every system that catches a parameter's width reads the header** — Go's cgo
parses DWARF out of a probe, Swift embeds Clang, Zig's `translate-c`, Nim's
futhark and Rust's bindgen use libclang, and the Linux kernel's `#[export]`
generates with bindgen and then compares. **None does it inside C.**

**Python cffi's API mode documents Heroes' measured behaviour verbatim**, as
intended semantics:

> *"functions taking or returning integer or float-point arguments can be
> misdeclared: if e.g. a function is declared by `cdef()` as taking a `int`, but
> actually takes a `long`, then the C compiler handles the difference."*
> *"other arguments are checked: you get a compilation warning or error if you
> pass a `int *` argument to a function expecting a `long *`."*

Cython documents the same hole as a feature. So Heroes is already at the frontier
of what checking-without-parsing buys.

**And the wordings that aged well named the hole.** Haskell 2010: *"Generally, no
check for consistency with the C type of the imported label is performed"* —
GHC 6.10.1 removing header-based checking falsified not one word of it. Rust RFC
3484: *"The compiler cannot itself verify these assertions."* The wording that
aged worst claimed a check the compiler could not make statically — C++'s dynamic
exception specifications, deprecated in C++11 and **removed in C++17**,
*"embarrassing to explain to new developers."*

**A real shipped instance of this exact mistake**: `rust-lang/libc#1036` —
`ioctl`'s `request` declared `c_int` on Android aarch64 where it is `c_ulong`;
and `#301`, where the wrong width was invisible on x86 and visible elsewhere.
Correct by accident under one ABI, which is Heroes' shape.

## The coordinator's errors, recorded

**Three, and the second is the same shape as panel 091's.**

1. **"The compiler enforces half the sentence."** False. The spec's checked list
   omits `parameter`; document and compiler agree. Found by the spec-warden.
2. **"Warnings are out as the mechanism."** False, and it is CLAUDE.md §1's named
   shape again: measured with `-Wall -Wextra` instead of the compiler's own
   thirteen flags, then generalised from that sample to a class. The real flags
   catch width and sign today.
3. **"Clang folds its builtins."** False. `strncmp`, `malloc`, `calloc`, `abs`
   and `fwrite` are builtins and are all caught; the silent set is
   `_FORTIFY_SOURCE` macro rewriting plus lossless widening. Found by the
   compiler-engineer after the coordinator had already relayed the wrong cause to
   two seats mid-sitting.
4. **A relayed claim that was not measured**: the coordinator passed on the
   historian's note that `-std=gnu11` supplies `__builtin_types_compatible_p`.
   **Refuted** — it and `__typeof__` work under `-std=c11`, `c17` and `c23`; they
   are clang extensions, not gnu-mode features. `-std=gnu11` is load-bearing for
   `M_PI`/`strdup`/`fileno` exactly as panel 047 said, and for nothing else here.

## What this sitting found that it was not convened for

- **`CLAUDE.md:182`'s citation is dead, confirmed by two seats and the
  coordinator.** It names `commands/flags.rs::FLAGS`; that file exists only at
  `archive/bootstrap-rs/heroes-cli/src/commands/flags.rs`. The live file is
  `selfhost/cli_flags.hero`. **This is the third failure of that one sentence** —
  and the paragraph that says *"the copy that used to stand here is deleted
  rather than corrected a third time"* has now had its **replacement citation**
  rot. CLAUDE.md is the author's (panel 086 R6), so it is queued rather than
  edited.
- **An emitter defect that routes the compiler's warning to the author's line.**
  `selfhost/emit_ops.hero:99-108` prefixes an extern call's result with
  `(void *)` or `(const char *)` — sound for an **assignment**, and the reason is
  recorded there — but when the result is discarded the line becomes a cast
  expression whose value is unused. Two shapes, measured by the ffi-pragmatist:
  `(void *)memset(...)` → *"should this cast be to `void`?"*, `(const char
  *)getenv(...)` → *"expression result unused"*; an integer result is exempt.
  Both land on the author's own `.hero` line, **indistinguishable from a binding
  error**. It must be repaired before any option that routes warnings to the
  author.
- **A live `spec:204` violation in this repository's own corpus.**
  `examples/curl/main.hero:44` declares `option: i32` where `CURLoption` is
  unsigned. Today's probe misses it; the parenthesised probe catches it; `u32`
  compiles clean.

## Where the seats disagreed

**`(&f)(…)` or `(f)(…)`.** The ffi-pragmatist proposed the first, the
compiler-engineer built the second. Both defeat the macro and both break a
**macro-only** C function identically — `(htonl)(a0)` is `use of undeclared
identifier`, and so is `(&htonl)`. §4.19 promises *"Macros and `inline` functions
are reachable"*, and the harness has **no case** for a macro-only binding: 44
distinct C functions are probed in the corpus and none is macro-only, so nothing
fires today. **The engineer's `(f)` is adopted** as the smaller change, and the
missing golden case is owed with it.

**P1's cost.** The engineer priced P1's plumbing at 30 lines and found that no
attribution work is needed — `grep -c 'needle: "error'` is **0** across all nine
`emit_ffi_*` modules, and `emit_ffi_mutable.hero:109` already feeds a `warning:`
line to a matcher and asserts success. P1 is therefore cheap and still vetoed,
on what it would route rather than on what it costs.

## Predictions to score

At **M-separate-compilation close** unless stated.

| seat | prediction | falsifier |
|---|---|---|
| compiler-engineer | the 2-line parenthesised callee alone keeps `heroes test selfhost/main.hero` at **492/492** and requires exactly **146 files re-blessed / 922 changed lines**, every one `(void)f(` → `(void)(f)(`, with no other line moving | any other line moves |
| compiler-engineer | with `-Werror=incompatible-pointer-types`, `heroes run examples/sqlite/main.hero` exits **1** and the emission suite reports **≥2** failures | it does not |
| ffi-pragmatist | under P0 + the parenthesised probe, all 14 `examples/` programs still build and the **single** new diagnostic in the whole corpus is `examples/curl/main.hero:44` | a fifteenth unit where the two forms disagree |
| ffi-pragmatist | under P2, §4.19's ladder rung 3 needs a shim and `getline` cannot be bound at all | one `.hero` line that builds `sqlite3_open` clean under `-Werror=incompatible-pointer-types` |
| spec-warden | S3e costs **11 cl100k tokens and is rewrite-free** — after a repair lands it reads identically, where S2-as-briefed's +24 must be deleted | at M-ffi-ladder close, any FFI golden still declaring an integer parameter narrower or differently signed than its header typedef |
| llm-ergonomist | with the mapping named, generated callers **convert at a size argument** (`to_u64`/`to_u32`) in a majority of samples, against near zero today | the count does not move — then the tokens bought nothing |
| historian | no system exists that checks a hand-written parameter width against a real header without parsing it | one counter-example retires the finding |

**The ergonomist's trap, recorded**: any option that leaves the mismatch silent
scores *better* on a naive "does the program run and print the right thing"
first-try rate, because the wrong declaration is not on the path. **Measure the
binding, not the run.**

## Process notes

**The freeze held.** The tree was untouched from the first brief until this file.
Four addenda went out mid-sitting — two of them corrections of the convener's own
briefs, sent the moment the error was found rather than at the synthesis, and
both told the seats their measurements win over the correction.

**The blind A/B was abandoned before it was tried.** Panel 091's ergonomist
identified the current wording on sight because the spec contains it verbatim;
this sitting told the seat which was current and asked it to answer the first
question before reading further. That worked: the wrong answer at 8/10 is the
sitting's most useful single data point, and it would not have survived a
comparison the seat could see through.

**Two seats corrected a third.** The historian proposed the disjunction and said
it had not compiled it; the compiler-engineer compiled it and it failed 16 ways.
That is the lane working, and it is the argument for keeping an advisory seat
that cannot run code beside two that can.

## Author's verdict

**Ratified 2026-08-25**, the day it closed (author instruction, *"ok ratifichiamo
e chiudiamo tutto"*). The resolution stands: the probe's callee is parenthesised,
the discarded-result cast becomes `(void)`, and the spec gains one row at **+12**.

**What the yes settles.**

- **The mechanism is not replaced, it is unblocked.** Nothing new was built: the
  check, the diagnostic and its fix all existed, and three production lines let
  clang reach them. That is the shape the author's *"most robust"* instruction
  bought — the robust answer turned out to be the small one, and it was small
  because four seats refused the large ones.
- **`spec:204` keeps its promise and is no longer only derivable.** `size_t` had
  no spelling anywhere in the document; it has one now. The two rows that were
  refused stay refused: a rule is written in exactly one place.
- **Reachability, `-Werror` on pointer families, and whole-type identity are all
  closed** — the first by panel 091, the second and third here, each with a
  compiled counter-example rather than an argument. A future sitting that wants
  any of the three must overturn a measurement, not a preference.

**What the yes does not settle**, and both are queued rather than implied:

- **The float half.** `-Wdouble-promotion` under a `#pragma clang diagnostic
  error` is approved by the compiler-engineer **today**, given three named
  measurements it did not run. Until then `pow(x: f32, y: f64)` still builds and
  prints a fifth-digit-wrong answer.
- **The sentence naming what is unchecked** waits for that, deliberately. It is
  bought once, at its final size — a **wider** by-value integer and a **pointer's
  target** — rather than twice at two sizes.

**And the sitting's process finding is ratified with its result**: four of five
seats overturned a premise of the convener's brief, and the answer got smaller
and truer at every correction. The brief's two errors are recorded in this file
under their own heading rather than smoothed away, because the shape they share —
generalising from a sample to a class, then relaying it before checking — is
CLAUDE.md §1's, committed by the seat whose job is to police it.
