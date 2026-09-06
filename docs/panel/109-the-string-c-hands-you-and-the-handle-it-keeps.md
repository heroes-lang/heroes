# 109 — The string C hands you, and the handle it keeps

Date: 2026-09-04 · **full panel, five judges** · convened by author instruction
(*"write the sentence yourself, then convene the sitting"*) on the question
panel 108 queued the same day: the ownership fact at the C boundary that no
unread-cell rule can see.

Status: **RATIFIED 2026-09-04** (author instruction, *"I ratify"*). See
§ Author's verdict.

Lane: **full**, and every seat earned its place. The two that do not compile
found the two facts the resolution rests on: the ergonomist that **the correct
program cannot be written under today's spec** (a caller-owned C string can be
freed or read, never both — which the FFI seat then measured on the compiler),
and the historian that **no system in forty years names the freeing function
per slot on an out-parameter**, so the `@error: cstr owned f` position is a
genuine composition and not a catch-up. The three that compile then split the
proposal in two along a line all five drew independently.

## The proposal, as put

> A group's function may mark a `cstr` or `ptr` it hands the program as the
> program's to release, naming the C function that does it: `-> cstr owned
> sqlite3_free`, `@error: cstr owned sqlite3_free`, `-> ptr owned
> curl_easy_cleanup`. A `cstr` so marked arrives as a `str?`, copied and
> released at once. A `ptr` so marked is a counted value like a `str`: copies
> share it and the last to go releases it. An unmarked pointer stays opaque and
> is never released.

Two wordings were measured: A (above) **+129** tokens, B (terser) **+80**.
`design.md:2120-2125` reserved this years ago: *"A binding annotation vocabulary
will eventually be needed … a pointer you must free (and with which function), a
borrowed pointer you must not touch, and a buffer that C takes ownership of. Do
not use `@` … Reserve a keyword."*

## Why it was asked

`examples/ledger/db/sqlite.hero` leaked `sqlite3_exec`'s `errmsg` — 40 bytes per
refused statement, caller's to `sqlite3_free` — invisible on the development Mac
in all three configurations and found by LeakSanitizer on the Linux CI leg.
Panel 108 measured that no rule about an unread cell can catch a leak (`_ =
error` satisfies it while the bytes still go), that the word "free" does not
occur in the spec and cannot, and that no static analyser catches this class
without an annotation. It queued this sitting as the right question.

## The verdicts

| seat | `cstr owned f` | `ptr owned f` | what it measured |
|---|---|---|---|
| compiler-engineer | **object**, three conditions | **veto** | parse + printers built (+63 lines, fixpoint holds); the `cstr` half is ~270 lines through three files at zero headroom; the `ptr` half is a new `Ty` case touching **179 exhaustive arms in 49 files**; the errmsg lowering hand-emitted and run under LeakSanitizer: **0 bytes** where the control leaks 40 |
| ffi-pragmatist | **approve**, four riders | (same riders; `sqlite3_close` as a freer leaks 62,464 B under the emitter's drop order) | 12/12 freers compile **by name**, 3/7 refused through a generic pointer; `fclose(NULL)` segfaults on glibc; a borrowed pointer marked owned aborts in **20 of 20** runs; `HeroOwned` prototyped against the runtime on both platforms |
| llm-ergonomist | **approve** A + one sentence | — | 3 tasks × 3 texts from the spec alone: under the base text the correct program is a guess or unwritable; two silent hazards remain and one is the feature's own |
| spec-warden | **object** A and B; **approve W10** (+59) with R2a (−28) | costs ~0 tokens more | floor for the three facts **+49**; panel 108's registered ≤ +25 **falsified**; the corpus leaks statements on every `?` error path **today**, `sqlite3_close` → 5 at exit 0 |
| historian | **approve**, precedent-backed | approve as departure, one advisory objection | GCC 11 `malloc(deallocator)` names the freer (return values only); GI `(transfer full)` covers the out slot (freer by type); .NET's hard-coded `CoTaskMemFree` is the recorded crash class the proposal's design fixes |

One veto, on one half. Per CLAUDE.md §4 a veto is a refusal and not a price,
and it stands with its return conditions written below.

### What could not be written today — the fact under everything

The ergonomist, from the spec alone: to free `errmsg` you need `void
sqlite3_free(void *)`, whose parameter is a `ptr`; you hold a `cstr`; *"no
implicit conversions"* and no `cstr`→`ptr` conversion exist, so the only program
that surely compiles is the one that leaks. The FFI seat, on the compiler:
`@error: cstr` against `char **` is **`error[ffi_writable_parameter]`** (panel
058's rule), and `@error: ptr` followed by `.validated()` is **`type_mismatch`**.
**A caller-owned C string can be freed or read, never both.** The `nullptr`
repair of `fa01ab20` was not the cheap fix; it was the only legal program. The
compiler seat states the same as an inference: the leak alone is closable today
with `function sqlite3_free(p: ptr)`; what the language cannot do is *read* the
message and release it.

### The `cstr` half — what the five seats built, and where they converged

**The freer is called by name.** Twelve real freers (`sqlite3_free`, `free`,
`sqlite3_close`, `sqlite3_close_v2`, `sqlite3_finalize`, `fclose`,
`curl_easy_cleanup`, `curl_free`, `curl_slist_free_all`, `freeaddrinfo`, and
Win32 stand-ins `LocalFree`, `CoTaskMemFree`) compile by name on a `void *`
under all fourteen project flags; **three of seven are refused through `void
(*)(void*)`** (`sqlite3_close`, `fclose`, `LocalFree` — their signatures differ),
and calling through a cast is UB clang cannot see. The emitter already emits the
by-name shape for every probe. The probe verifies the freer: arity, one pointer
parameter, a typo answered with clang's own correction.

**NULL is the emitter's.** On success `sqlite3_exec` leaves `errmsg` NULL; ten of
ten freers accept NULL on macOS, **nine of ten on glibc — `fclose(NULL)`
segfaults**. The guard is emitted, never assumed of the library.

**A borrowed pointer marked owned is loud, measured.** `sqlite3_errmsg`,
`curl_version`, `getenv`, `CURLINFO_EFFECTIVE_URL` each freed with the wrong
function: five shapes × two platforms × plain and `--sanitize` = **20 runs, 0
silent** — abort 134 or 139 plain, `bad-free`/`double-free` under ASan. The
ergonomist's hunted hazard (marking SQLite's own memory) is therefore a crash,
not a silent corruption; Vala documents the identical mistake in its binding
guide.

**The feature's own hazard is silent, and four seats wrote the same sentence
against it.** A marked value is still a `cstr`/`ptr`, so a program that calls the
named freer itself and then lets the compiler release it double-frees — **exit 0,
no message, on both platforms** (FFI seat, row 7). The ergonomist reached the
tempting shape from the spec's own sqlite example (`@out: ptr owned
sqlite3_close`, then `rc = sqlite3_close(db)` to read the status); the warden
priced the guard at +9 tokens; the historian found Racket's `(deallocator)`
cancelling the finalizer for exactly this reason; the FFI seat made it a rider
without which its verdict is *object*. **The sentence: a call of the declared
freer with an owned value is a compile error.** It needs only the argument's type
and the callee's name.

**The `@` position is out-only.** Both texts said the value *"arrives as a
`str?`"* and neither said what the caller declares; the ergonomist guessed
`errmsg: str? @ fail(...)`, and the compiler seat's condition is that the
position be specified out-only — no copy-in value the callee could observe. That
is panel 108's out-only mode, unnamed there, with a reason to exist here: the
cell is **written by the call and never read into it**, which is also what GNAT's
`out` means and why its `-gnatw.o` rule is exact for that mode (panel 108's
historian).

**The `char **` carve-out is a spec fact.** Panel 058's `ffi_writable_parameter`
refuses `@x: cstr` against `char **` today because C could write through the
lent string; a *marked* `@cstr` is the one case where C writes a pointer the
program will own, so the rule steps aside there and the spec must say so — the
compiler seat's third condition.

**The lowering exists and is twelve lines of C.** The compiler seat hand-emitted
the errmsg call under the proposed lowering — hidden cell, `validated` copy,
store, NULL branch, freer call — and ran it in the Linux image with the
compiler's fourteen flags plus `-fsanitize=address,undefined`: **exit 0, stderr
empty**; the unpatched control: `Direct leak of 40 byte(s)`. Second call after a
success: `validated(NULL)` → `null_cstr`, freer skipped.

**What it costs in the compiler**: ~270 code lines, and it lands in three files
with no room — `ir/lower.hero` at **1511/1511**, `parse/decl.hero` 638/638,
`ast.hero` 453/455 (the parse prototype alone took the last two over). The
compiler seat's second condition: split `ir/lower.hero` and `parse/decl.hero`
**first**, along seams, with their `DECIDED` rows lowered rather than raised.
`owned` is a contextual word, never a keyword: it is an identifier at 22 sites in
`selfhost/` today. Compile time not measured for the unbuilt half.

**What it costs in the spec, all measured this sitting**:

| wording | tokens | gross | net with R2a (−28) |
|---|---|---|---|
| base | 3750 | | |
| floor for the three facts (warden's W8) | 3799 | +49 | +21 |
| warden's W10 (`cstr` + `ptr` + guard) | 3809 | +59 | +31 |
| **Wt2 — `cstr` only, guard, `@` out-only, `char **`** | **3820** | **+70** | **+42** |
| coordinator's A (`cstr` + `ptr`, no guard) | 3879 | +129 | +101 |

Wt2, the wording this resolution proposes:

> `owned sqlite3_free` after a `cstr` result or a `char **` out-parameter: the
> compiler frees that string with that function, hands it over as a `str?` (the
> `@` cell is only written), and refuses your own call of it. Unmarked pointers
> are never freed.

The removal that pays: **R2a**, `spec:225-227`'s *"— frameworks on macOS, `-lGL
-lX11` on Linux — in one spelling that is the same everywhere"*, platform
commentary whose wrong guess is harmless, the class panel 090 removed elsewhere
(−28, measured). Net **+42** against the ledger's mean of **+31**: above the
mean by eleven tokens, and eleven is what the `@` cell and the `char **` clause
cost, which two seats made conditions. The warden's stated condition — *object
if the `@` cell costs more than +15 over W10* — is met: Wt2 is +11 over W10.
Panel 108's registered prediction for this sitting, *"≤ +25 spec tokens"*, is
**falsified by every wording**; the floor is +49, and that score is appended to
panel 108's table.

### The `ptr` half — why the veto stands, and what brings it back

A counted `ptr` is a new case in `Ty` with `is_refcounted` true: a box
representation, a boxing op after every extern call that returns one, unboxing
at every extern argument, a per-freer drop descriptor row, rules for `==`, hash
and map keys, and the `pointer_element` gate. **179 exhaustive `Ty` arms in 49
files**, five more files at or within seven lines of their ceilings
(`emit/ctype.hero` 364/365, `emit/gate.hero` 341/341, `emit/inst.hero` 343/350,
`ir.hero` 305/310, `ir/print.hero` 465/470), 350–450 lines estimated, and either
`HERO_RUNTIME_ABI` moves or the array descriptor is reused as a one-element
array. Checker *and* lowering *and* backend is design.md §1.7's definition of
core, and §4.19:2124's explicit-release route (`sqlite3_close`,
`curl_easy_cleanup`) works in both corpus examples today. The veto is on
core-vs-sugar and cost, which is that seat's mandate.

**The FFI seat found two hazards inside the half, both real.** The emitter drops
locals in declaration order, so a `db` declared before its `statement` is
released first; with `sqlite3_close` as the freer SQLite refuses (`SQLITE_BUSY`)
and the handle stays alive: **62,464 bytes, 105 allocations leaked on Linux,
silent on macOS**. With `sqlite3_close_v2` (zombie mode) the same program is
clean. And a handle nobody drops becomes `panic: 1 heap blocks still live`, exit
134 — **the first leak detector for C handles Darwin would ever have**, which is
the half's strongest argument.

**The warden found the live witness.** `examples/ledger/main.hero` has five
prepared statements with a `?` between `prepared` and `finalized` (lines 67-70,
76-81, 151-155, 169-171, 186-188): a failed `bind` returns through the `?` and
the statement is never finalized. Forced on this Mac: `sqlite3_close` answers
**5** where the happy path answers 0, exit 0 both times. **The corpus leaks
statements on its error paths today.** It can release them by the explicit route
— `match` instead of `?`, finalize in the error arm — at a cost in shape, so it
does not yet meet the veto's return condition; it is the program to point that
condition at.

**The historian's advisory objection stands with the veto**: every system that
refcounts an opaque C handle (PyCapsule, `SafeHandle`, `Unmanaged`, Lua
userdata, Perl `T_PTROBJ`) leaves the case where **C keeps the raw pointer** to
the programmer, and the proposal said nothing about it. Its prediction: within
the first ten corpus programs using `ptr owned`, one passes the counted pointer
to an extern that retains it (`sqlite3_bind_*` with `SQLITE_STATIC`,
`CURLOPT_WRITEDATA`, a raylib callback `userdata`) and the last-copy release is a
use-after-free `--sanitize` reports.

**Return conditions, the compiler seat's, verbatim in substance**: a compiled
prototype that gives an owned `ptr` shared release **without a new `Ty` case**
(the array descriptor with a compiler-emitted per-freer drop row is the shape it
names) in **≤ 120 lines and no ABI move**, **plus** one corpus program that
cannot release its handle by the explicit route. And, from the FFI seat, the
spec example names `sqlite3_close_v2` and the emitter's drop order is stated.

### The third case — C keeps a buffer

`sqlite3_bind_text(stmt, 1, s.cstr(), -1, SQLITE_STATIC)` with the `str` dead
before `sqlite3_step`: macOS prints `length: 0` at exit 0, **Linux prints
`length: 53`** — a wrong program with the right answer by luck — and
`--sanitize` reports heap-use-after-free on both. The C-side answer is spellable
today: `constant SQLITE_TRANSIENT: ptr` compiles and gives 53. A Heroes-side
refusal needs the lifetime of a lent `str` across two calls, which a language
with no references (§4.10) cannot state; the warden priced a keyword for it at
+18 and found the compiler could refuse nothing with it. **A separate sitting,
if ever; Principle 0 does not ask for it.** Owed now, measured at +3 tokens:
`spec:239`'s *"lends a `str` to C to read"* becomes *"to read for that call"*.

### Precedent, and two corrections to the brief

GCC 11's `__attribute__((malloc(deallocator)))` is the one compiler-level mark
that names the freeing function in a declaration — return values only, with
`-Wmismatched-dealloc` on by default. GObject-Introspection's `(out) (transfer
full)` covers the out-parameter slot, freer by type. Racket's `(allocator f)`
wraps the binding and names the freer; `(deallocator)` cancels the finalizer on
an explicit call. Vala's `free_function` is per type. .NET marshals returned
strings by copy-then-free with a **hard-coded** `CoTaskMemFree`, and its recorded
failure — memory from another allocator, silent until a Visual Studio update
made it crash — is fixed, in Microsoft's own troubleshooting table, by *"expose
the specific free function of the allocator"*: the proposal's design. cppcheck's
`sqlite3.cfg` has `<alloc arg="2">` for `sqlite3_open` and **no block for
`sqlite3_exec`**, which is how catalogues miss `errmsg`. Deterministic decref
spares Heroes the finalizer-ordering class that Java deprecated for removal
(JEP 421) and Go documents as unguaranteed.

Corrections: the brief called clang's `ownership_returns` the strongest
precedent for naming the freer — it names an allocation *kind* and pairs by kind,
return values only; SAL's `_Ret_` carries no ownership and `__drv_freesMem`'s
kind is documented as unused; .NET frees with `CoTaskMemFree`, not
`Marshal.FreeHGlobal`.

## Where the seats disagreed

**On the `ptr` half**: the compiler seat vetoes as core; the FFI seat approves
it under the same four riders and built it against the runtime on both
platforms; the historian approves as a departure with an objection; the warden
finds it costs ~0 tokens and has a live witness; the ergonomist finds its
locality equal to a `str`'s. A veto is a refusal and stands. The synthesis does
not smooth it: the half is **wanted by four seats and unaffordable by the one
whose mandate is cost**, and the record keeps both, with the return conditions
that turn the one into the other.

**On the price**: the warden's approve is at W10 (+59, +31 net); the complete
wording the compiling seats require is Wt2 (+70, +42 net). The eleven tokens are
the `@` cell and the `char **` clause. The synthesis takes the complete wording
and says the mean is not met and why.

**On what the compiler cannot do today**: the compiler seat's inference (free
by `ptr` is possible, reading is not) and the FFI seat's measurement (`cstr` is
refused for `char **`, `ptr` cannot reach `validated`) agree; the ergonomist
reached the same wall from the spec alone.

## The resolution — provisional, and the robust one

**Adopted: the `cstr owned <function>` mark**, in result position and in the
`@` position where the header spells `char **`, landing at the §4.19 step of
M-ffi-ladder with every condition the seats set, none of them a compromise:

1. The freer is called **by name** in a generated per-freer release, verified
   by the probe (arity, one pointer parameter); never through `void (*)(void*)`,
   never cast.
2. **A call of the declared freer with an owned value is a compile error** —
   four seats, one sentence.
3. The **NULL guard is emitted**, never assumed of the library.
4. The `@` position is **out-only**: the cell is written by the call and never
   read into it; the spec says so in the same sentence.
5. The spec names the **`char **` carve-out** from `ffi_writable_parameter`.
6. Wording **Wt2** (+70), paid by **R2a** (−28): +42 net, measured; and
   `spec:239` gains *"for that call"* (+3), the third case's one owed word.
7. **Before the feature**: `ir/lower.hero` (1511/1511) and `parse/decl.hero`
   (638/638) are split along seams that name a concern, their `DECIDED` rows
   lowered and not raised.
8. **After it**: `examples/ledger/db/sqlite.hero` returns to `@error: cstr owned
   sqlite3_free` — the program that could not be written — and the Linux
   `--sanitize` leg is green with it.

**Refused, veto standing: the `ptr owned <function>` mark**, with its return
conditions written above and the warden's witness named as the program those
conditions point at. What the conservative resolution would have been: refuse
both halves and leave `design.md:2124`'s shim route as the answer. The synthesis
does not take it, because four seats measured that the `cstr` program is
unwritable today and the fifth built its lowering leak-free in twelve lines of C.

**Queued, not decided**: the third case, C keeping a lent buffer, is a sitting
of its own when a program needs it; the C-side spelling exists today.

## Predictions to score

| origin | prediction | instrument | scored at |
|---|---|---|---|
| compiler-engineer | at the landing step, `layout` is red at exactly `ast.hero`, `parse/decl.hero` and `ir/lower.hero` before any re-baseline, and the `selfhost/` diff measures 180–300 code lines | the layout suite + `code_lines` | the `owned` step |
| ffi-pragmatist | `examples/ledger/db/sqlite.hero` with `@error: cstr owned sqlite3_free` reports 0 leaks on the Linux `--sanitize` leg; `examples/curl/main.hero` with line 75's `curl_easy_cleanup` deleted exits 0 today and 134 at `hero_runtime_check_leaks` if `ptr owned` ever lands | the Linux CI leg; this Mac | the `owned` step; the `ptr` return |
| llm-ergonomist | on caller-frees tasks outside the spec's examples, first-try rate (no leak, no double release, compiles) ≤ 35% under base, ≥ 70% under a wording with examples; silent double-release ≤ 5% with the guard sentence | a Part 11 reader harness | first Part 11 run |
| spec-warden | `heroes measure` at the landing commit: gross ≥ +49 whatever the wording, net with R2a within ±3 of the figure the step records; `examples/ledger`'s forced-failure path prints `close … answered 0` only if the `ptr` half lands | `heroes measure`; the example | the `owned` step |
| historian | within the first ten corpus programs using `ptr owned`, ≥ 1 passes the counted pointer to a retaining extern and `--sanitize` reports use-after-free | the corpus under `--sanitize` | the `ptr` return, if any |
| coordinator | the splits of `ir/lower.hero` and `parse/decl.hero` each land at ≤ 900 and ≤ 400 code lines respectively with `DECIDED` rows lowered, before the `owned` step opens | the layout suite | M-ffi-ladder |

Scored from panel 108: the coordinator's *"≤ +25 spec tokens"* for this
sitting is **FALSE** — the floor is +49.

## Author's verdict

**RATIFIED 2026-09-04** (author instruction, *"I ratify"*, the third
ratification of the day and the first that is not a refusal).

**What the yes settles is a form entering the language**, which panels 107 and
108 did not do: both of those refused, and their value was the falsifier they
named. This one adds `owned <C function>` to the FFI, and the burden Principle 0
puts on an addition is discharged not by elegance but by a measurement three
seats reached independently — **the correct program cannot be written today**.
A caller-owned C string is freeable or readable and never both, so §1.11's
founding constraint (everything comes from C) had a hole in it that no program
could route around, and `fa01ab20`'s `nullptr` was not the cheap repair but the
only legal one.

**What it authorises, in the order the work must happen**: the two splits first
(`ir/lower.hero` at 1511 of 1511 and `parse/decl.hero` at 638 of 638, along
seams that name a concern, `DECIDED` rows lowered and not raised), then the mark
with all eight conditions, then `examples/ledger/db/sqlite.hero` returning to
`@error: cstr owned sqlite3_free` with the Linux `--sanitize` leg green on it.
None of the eight is negotiable at implementation time: each is a seat's
condition, and the one that four seats wrote independently — **your own call of
the declared freer on an owned value is a compile error** — is the one that
turns a silent double release at exit 0 into a diagnostic.

**What the yes does NOT settle**: the `ptr owned` half stays refused under a
standing veto. The author's ratification does not overturn a seat's veto and is
not being asked to: the half returns through its own conditions, measured, and
`examples/ledger/main.hero`'s five leaking error paths are the program those
conditions point at. The third case — C keeping a lent buffer — stays queued
with its C-side spelling working today.

**And the record keeps the day's shape.** Three sittings, three ratifications:
107 refused a number that cannot be uniform, 108 refused a rule that cannot see
what it was convened for, and 109 admits a form because a program was
unwritable. The first two were convened by `/decide` items whose recommendations
the sittings overturned; this one was convened by the author on a question a
sitting had queued, and it is the one that changes the language.

## What the lane gave up

Nothing — this was the full panel. What it cost: five seats, three compiling on
two platforms, one building a runtime prototype and one a parser prototype, the
historian fetching forty sources. What it bought is the fact under the whole
file: the leak that convened panel 108 was not carelessness but a program the
language cannot express, and now the record says which half of the fix is sugar
and which half is core.

## The coordinator's prediction, scored at M-declared-freer step 1 (2026-09-06)

| judge | prediction | result |
|---|---|---|
| coordinator | the splits of `ir/lower.hero` and `parse/decl.hero` each land at ≤ 900 and ≤ 400 code lines respectively with `DECIDED` rows lowered, before the `owned` step opens | **Right on `parse/decl.hero`; on the other half it is the LETTER that saves it and not the intent.** Measured with `tests/harness/suite_layout.hero`'s own `code_lines`: `parse/decl.hero` **638 → 217**, its siblings `parse/tails.hero` **246** and `parse/group.hero` **235** — under 400 and under §11's own 300, so that row was not lowered, it was **removed**. `ir/lower.hero` **1511 → 174**, under 900, which makes the sentence true as written; but what the split produced beside it, `ir/flatten.hero`, is **1104**, which makes it false under the reading every seat plainly meant — *no piece of the lowering above 900*. Scored **FALSE** on that reading, with the number given rather than argued around. |

**Why no arrangement of files takes that piece under 900, measured rather than
assumed.** The knot inside the old `ir/lower.hero` is **25 functions and 990
code lines**, every one of them reachable from every other through `expr`: a
statement lowers its expressions, an if-expression lowers its blocks, a block
lowers its statements. Heroes refuses module cycles, so a knot is one module or
nothing (CLAUDE.md §11's knot clause), and 990 is a floor no cut gets under. The
one route out of a mutually recursive walk is to hand the recursion over as a
**function value**, and this walk threads its builder as `@b`:
`(function(@Box) -> ())` is `error[expected_type]: expected a type, found `@``,
run on this compiler, so there is no such value to hand over. That is a command
and not a search (CLAUDE.md §1: a failed search is not an impossibility).

**What the split bought, since it was not the number.** A reader who opens
`ir/lower.hero` now meets **174** lines saying how a declaration becomes a
function — its frame, its parameters, §4.8's copy-out — and a pointer to where
the body walk lives; the knot is in a file named for what it does, carrying the
map §11 owes. The **21** modules that call `lower.lower(…)` did not move a
character, because the part that only ever calls INWARDS is the part that stayed
behind and kept the name. And what left did so by a **mechanical** rule rather
than a line count: `ir/questions.hero` (**149**) is everything with no `@b`
parameter, `ir/emissions.hero` (**185**) everything that emits one fixed shape
and never walks back into the tree. Both are far under §11's 300 and neither is
in `DECIDED`, which is where the `owned` step has room to spend.

**And the second split refutes a claim that was in the compiler's own source.**
`parse/decl.hero`'s module doc said it was *"one module because the language
leaves no choice"*, naming the ring `file → extern_group → members →
constant_tail`. The ring is real; the conclusion did not follow. Measured, that
file's call graph has **no cycle at all** — the ring closes only while the shared
tails sit in the same module as the dispatcher. Moved into `parse/tails.hero`,
which neither of the other two is used by, every edge runs one way and
`module_cycle` never fires. The answer had been printing itself in that
diagnostic's own note the whole time: *"move what both modules need into a third
one that neither uses"*. **A knot is a property of a PARTITION, not of a
grammar**, and the two files this step was told to cut turn out to be one of
each kind — which is the finding worth more than either number.

## The spec-warden's prediction, scored at M-declared-freer step 2 (2026-09-06)

| judge | prediction | result |
|---|---|---|
| spec-warden | `heroes measure` at the landing commit: gross ≥ +49 whatever the wording, net with R2a within ±3 of the figure the step records | **TRUE on both halves, and every delta was re-measured rather than carried.** Base **3824**; Wt2 alone **3895** (+71, where this sitting measured +70); the third case's three words **3898** (+3, exactly); R2a removed **3871** (−27, where this sitting measured −28). **Gross +74**, comfortably over the +49 floor. **Net +47**, and the pair the prediction names (Wt2 with R2a) is **+44** against the sitting's arithmetic of +42 — inside ±3. Headroom **272 → 225**. |

**Why two of the three deltas moved, and it is not an error in either
measurement.** The sitting measured against a base of 3750; the landing commit's
base is 3824, because `docs/panel/115` added a sentence four days later and
`docs/panel/111` one before that. A BPE tokenizer's counts are **not additive
across an edit boundary** — a sentence costs what it costs *in the text around
it* — so a ledger that adds up a sitting's columns is doing arithmetic the
instrument does not support. This is why `docs/measurements/010`'s row re-runs
`heroes measure` for each delta instead, and why the row records four numbers
where the sitting recorded three.
