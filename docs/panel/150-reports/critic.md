# Panel 150 — report of the completeness critic

No verdict. What follows is what is MISSING: a route nobody listed, a claim with
no command behind it, a contradiction between seats with which one is checkable,
and the question the sitting did not ask.

**Written in two passes.** Everything under § First pass is structural — read off
the five reports and the three cited sittings, no command run. Everything marked
**MEASURED** below it was run by this seat in its own copy of the tree, seed-built,
and says so. A previous critic was killed mid-session and its findings were lost,
so this file was opened before the first command and appended to as it went.

---

# First pass — read off the reports, nothing run yet

## A. The two routes are ONE route, and neither seat saw the other's

**Structural reading, to be checked below.**

- The compiler-engineer's route is `record Blob tag void`. It reports it
  **checks, builds, runs, exit 0** on the shipped compiler and emits `void *`
  byte for byte.
- The ffi-pragmatist's route is letting a `tag` name a C struct tag,
  `tag struct addrinfo`. It reports it compiled **at the C level only**, by
  hand-editing `--emit-c` output. It did not compile in Heroes.

**These are the same route with two different tag spellings**, and the thing that
makes one free and the other not is a fact about C, not about Heroes: the emitter
writes the tag verbatim into a type position, `void` happens to be a legal C type
name and `addrinfo` alone is not. So:

> **The composed resolution is one sentence: a `tag` names a C TYPE NAME.**
> `void` already satisfies it. `struct addrinfo` does not, only because the tag
> position holds one identifier.

If that reading is right, the compiler-engineer's Q1-B is **not a route the
ffi-pragmatist's route competes with — it is the half of it that already
shipped**, and the ffi seat's route is the missing half that covers
`struct X *` where the CE's covers `void *`. **Nobody composed them because
neither seat read the other's report**: reports were written independently and
in parallel.

*To check: (1) grep each report for the other's construct; (2) run
`record X tag void` and `record X tag addrinfo` in a copy and read the emitted C;
(3) price the surface of the missing half.*

## B. The warden's second falsehood is not a separate sitting — it is load-bearing for THIS one

The shared brief measures Q1's blast radius as **one file**, and it computes that
number under GROUP semantics: *"catches a producer only where the same group also
declares `ptr consumes`"*. The spec-warden's row 2 says the compiler keys on the
**TYPE, program-wide**, and the compiler-engineer independently says the same and
compiled a two-module nuisance to show it.

**If the compiler is type-keyed program-wide, the shared brief's blast-radius
table is a measurement of a rule nobody implements.** And one seat's approval
rests on that table directly:

> historian, condition 2: *"Key the demand on the opened region, never on `ptr`.
> … the blast-radius-of-one-file is what it looks like when it works."*
> And earlier: *"that is not a coincidence, it is what an audited region buys."*

**It is a coincidence.** The one-file number is an artifact of the coordinator
counting a conjunction the checker does not evaluate. The historian's Q1 approval
therefore rests on a property of the implementation that the compiler-engineer
measured to be absent. **That is the sharpest cross-seat contradiction in the
sitting, and it is checkable in one program.**

So: **it is this sitting's business**, because Q1's own cost was priced with it.

*On whether D1 at +7 real is the right unit: D1 rewrites `group` to `any extern`.
That is only true if `consumes` cannot appear outside an `extern`. To check.*

## C. The ergonomist is not refuted by the other two — it refuted itself and scored it wrong

Its report already contains the class the other two measured:

> *"`acquires` where `borrows` is true makes the language REQUIRE a release of a
> pointer the library kept, and the program then double-frees. Worse, **the
> demand itself biases toward `acquires`**, because the mark is demanded
> precisely when a consuming function is visible in the group."*

And then:

> *"It approves anyway, because that double-free is not a class the rule
> creates."*

**Those two sentences cannot both stand.** A demand issued exactly where a
consumer is visible, whose wrong answer is a double free, and which biases toward
the wrong answer, is a rule that creates the class. The compiler-engineer's ASan
bad free and the ffi seat's SEGV in `libsystem_info` are not new information
against the ergonomist — **they are the measurement of the ergonomist's own
paragraph**, which it wrote and then discounted.

So it is not panel 149's licenses-versus-causes. There the seat had no way to
see the cause. Here the seat named the cause and mis-scored it, and its own
premise is the thing that settles it.

**And its condition 3 does not rescue the approval.** Condition 3 asks whether a
`ptr` from a `borrows` call may reach a `consumes` parameter. The
compiler-engineer's bad free has **no `borrows` call in it**: `fopen` is marked
`acquires free`, and `free` is the wrong releaser. Condition 3 is silent on the
measured program.

**On whether route (A) shrinks the model further — and this is the finding
nobody in the sitting could have had.** The ergonomist's three conditions are:
(1) `_` must not drop an owed `ptr`; (2) say what happens across `extern` groups;
(3) say whether a borrowed `ptr` may be consumed. **Under Q1-B — refuse the mark
on a `ptr` — all three dissolve by construction**: there is no owed `ptr` for `_`
to drop, no cross-group demand on `ptr` to specify, and no owned `ptr` to pass.
The route the ergonomist was never shown satisfies every condition it attached to
the route it was shown. On its own stated criterion — *"that is less to
remember, not more"* — Q1-B is the shorter sentence: `ptr` is never owned, a named
handle always is.

**The cost of Q1-B that nobody stated, and the ergonomist is the seat that would
have caught it**: Q1-B deletes the mark position but **does not close the leak**.
The ergonomist's central finding — *"`ptr` is the only pointer-shaped thing in the
language with no diagnostic at either end"* — is **untouched** by Q1-B. An
unmarked `ptr` producer stays legal and stays silent. The compiler-engineer
reports Q1-B as *"breaks nothing"* and never says that what it also does is
nothing, for the defect as filed. It closes 037 by making the defect's program
unwritable-as-briefed, not by diagnosing it.

## D. The narrowed refusal on Q2 was offered as a withdrawal condition and priced by nobody

Five seats, one narrowed refusal, **zero line counts**. The compiler-engineer
priced *refuse the element release* (route 5a) at ~25 lines naive, refusing four
shipped call sites, and the sound version as needing provenance the checker does
not have. **Nobody priced *elements AND composite in one program*, which is the
shape the ffi seat measured as the only one that corrupts** (exit 133, zero bytes
on both streams).

Structurally it is **strictly harder than 5a, not narrower**, in the dimension
that matters: 5a is a local syntactic test on one argument; the narrowed one is a
**join over two call sites that may be in different functions and different
modules**. The compiler-engineer's objection to 5a's sound version applies to it
with more force, and the ffi seat filed it without a line.

*To check: whether the four shipped call sites CE measured would survive the
narrowed rule — i.e. whether any of them also releases the composite.*

## E. The unfiled defect, restated as a question of WIDTH

The ffi seat filed it as `ptr`-specific-in-appearance and the coordinator
confirmed it on a handle type. The sitting never asked how far it goes. Three
open edges, none of them run by any seat:

1. does a `borrows` call have the same problem?
2. does an `@` out-parameter have it?
3. how many shipped producers are exposed?

*All three below, measured.*

---
# Second pass — MEASURED

Copy at `04661466`, the tip, verified with `git log --oneline -1` before the
first command — the ffi seat's own lesson, applied. Seed built in one step,
`HEROES_RUNTIME` set to the copy's `runtime/`. `selfhost/` never rebuilt.

## A — MEASURED. The two routes do not compose as alternatives. One SUBSUMES the other, and the sitting adopted the subsumed half

**Neither seat read the other's.** `grep -niE "addrinfo|struct tag|netdb"` over
`compiler-engineer.md`: **zero hits**. `grep -niE "tag void|Blob"` over
`ffi-pragmatist.md`: **zero hits**. They are parallel independent findings, and
nobody put them on one table.

### A1. The compiler-engineer's route reproduces exactly as reported

```
extern "stdlib.h"
    record Blob tag void
    function malloc(size: u64) -> Blob acquires free
    function free(p: Blob consumes)
```
`check` **exit 0**, `build` **wrote blob**, run **exit 0**, and the emitted C is
`void *` throughout — `_Static_assert(HERO_RET_RECORD(malloc((uint64_t)0), void *),
"heroes-ffi-return malloc Blob")`. **Confirmed.**

### A2. The ffi seat's route reproduces too, and its obstacle is the ONE identifier

```
extern "netdb.h"
    record AI tag addrinfo
```
**`heroes check` exit 0. `heroes build` fails, in clang, with 18 errors**, all of
one kind:

```
ai.c:124:31: error: must use 'struct' tag to refer to type 'addrinfo'
```

So the seat's hand-edit was not a workaround for a missing feature — it was a
one-token substitution in a type position, and the whole gap between the two
routes is that the `Member` production spells the tag `[ "tag" ident ]`, one
identifier, and C's name for this type is two.

### A3. THE FINDING THE SITTING DOES NOT HAVE: `tag void` is capped at ONE per program

```
extern "netdb.h"
    record AI tag void
    record HE tag void
```
```
error[duplicate_tag]: `HE` and `AI` both name the C type `void` — one tag, one type
  note: two Heroes types over one C type is a hole nothing can close … Give the
        two types two tags, or give the one type one name and use it twice
```
**exit 1.**

**A program may declare exactly one `tag void` record, ever.** The
compiler-engineer's Q1-B — *refuse the mark on a `ptr` and point the author at
the named form* — therefore points every binding author at a form that **works
once**. The second opaque `void *` library in the same program has no route at
all: not `ptr` (refused by Q1-B), not a second `tag void` (refused by
`duplicate_tag`), not `tag struct X` (refused by clang, A2).

And the case defect 037 was briefed on needs **two**: the ffi seat's own
counterexample declares `freeaddrinfo` and `freehostent` over two different
opaque types in one group. Under Q1-B that program is **unwritable**.

**The ffi seat measured the other half of this without knowing it**:
*"`netdb.h` declares 19 `struct X *`-returning entry points and zero `void *`
ones."* Q1-B's route covers **zero of the nineteen** entry points in the header
this defect is about.

### A4. `tag void` gives up exactly the check §4.19 exists to perform

```
extern "stdlib.h"
    record Blob tag void
    function malloc(size: u64) -> Blob acquires freeaddrinfo
extern "netdb.h"
    function freeaddrinfo(ai: Blob consumes)
```
An eight-byte `malloc` handed to `freeaddrinfo`. **`heroes check` exit 0.
`heroes build` succeeds with ZERO clang diagnostics** — the probe emits
`static void hero_ffi_probe_h_crosswire_freeaddrinfo(void * a0)` and C converts
`void *` to anything in silence. **Run: exit 134.**

The ffi seat measured the same cross-wire under `struct addrinfo` as
`error: incompatible pointer types … [-Werror,-Wincompatible-pointer-types]`.

> **So the two routes are not two options. `tag struct X` is the one that keeps
> the clang check and scales past one library; `tag void` is the one that
> compiles today, loses the check, and holds one type per program. The sitting
> has one seat advocating each, neither aware of the other, and the compiler
> seat adopted the weaker one as its Q1 resolution and priced +51 lines of
> refusal to steer authors into it.**

**The composed resolution nobody wrote**: a `tag` names a C **type name**, which
today it already does — `void` is one and `addrinfo` is not. The whole delta is
the tag position holding what C actually writes. Cost: one grammar production,
`[ "tag" ident ]` → a tag that may carry `struct`. Unpriced by anybody.

## B — MEASURED. The warden's second falsehood is this sitting's business, and D1 is HALF the repair

### B1. The grammar settles the unit

`consumes` occurs in exactly one production, `CParam`, reachable only from
`Extern` (`spec/heroes-spec.md:393-394`). **So *any `extern`* is a true
statement where *a group* is false**, and D1's wording is sound as far as it
goes. The repair text also already exists in the repository:
`tests/golden/check/unmarked-handle-producer.hero` says in its own header
comment *"Where any `extern` function consumes a handle type"* and *"The rule is
keyed on the TYPE and not on the group"*. **The spec is the only document still
saying `group`.**

### B2. Reproduced: two groups, one file

Warden row 2 reruns exactly — `slot_close` in `extern "a.h"`, `slot_open` in
`extern "b.h"`, **`error[unmarked_handle_producer]`, exit 1**, on a program
§ 13 permits.

### B3. AND IT CROSSES A MODULE BOUNDARY, which D1 does not say

`slots.hero` declares the record and `slot_open`. `main.hero` does
`use slots` and declares `slot_close(s: slots.Slot consumes)`. Result:

```
error[unmarked_handle_producer]: `slot_open` hands back a `Slot` …
  at slots.hero:3:14
```

**The refused line is in a file that imports nothing and has never heard of
`slot_close`.** *Any `extern`* reads, to any reader, as *any `extern` in this
file*. The measured scope is **the whole program, across modules, in the
direction the import does not run**. D1 at +7 real repairs the group axis and
leaves the module axis unwritten, so it converts one falsehood into one
half-truth. **The right unit is D1 plus a clause naming the program as the
scope**, and nobody priced that.

### B4. A DEFECT NOBODY FILED, falling out of B3

**The diagnostic names `slot_close` and never says where it is.** In the
cross-module run above the caret sits in `slots.hero` and the only mention of
the arming declaration is the bare identifier `slot_close` in the message and
the note. design.md §4.17 asks that a diagnostic carry everything needed to fix
the program without opening another file; here the reader cannot learn **why**
the demand fired without grepping a tree. On `selfhost/`'s own 209 files that is
a grep, not a read.

### B5. And this is what dissolves the historian's Q1 approval

The historian's condition 2 is *"key the demand on the opened region, never on
`ptr`"*, and its evidence is the shared brief's blast radius: *"the
blast-radius-of-one-file is what it looks like when it works … that is not a
coincidence, it is what an audited region buys."*

**It is a coincidence.** B2 and B3 measure the mechanism as type-keyed and
program-wide; the one-file number is the coordinator counting a conjunction —
*same group declares `ptr consumes`* — that the checker never evaluates. The
compiler-engineer says the same of `ptr` and compiled a three-module nuisance;
this seat has now measured it for **handles**, which is the half nobody ran.

> **The one seat that approved Q1's scoping approved a property the compiler
> does not have, and the one number that persuaded it was produced by the
> coordinator's own brief.** That is the same failure as panel 149's uncited
> 147, one level up: the brief measured the proposal instead of the mechanism.

## E — MEASURED. The unfiled defect is wider than the ffi seat filed it, and one of its two halves is a LEAK the counter is supposed to close

Probes built against a header written for this report (`np.h`), a pool with a
producer that fails the way C fails: it returns NULL.

| probe | the program does | exit |
|---|---|---|
| **E1** handle result, `acquires`, `if s == nullptr` handled | prints, handles it | **134**, *"1 C handle(s) never given back"* |
| **E2** `@` out-parameter, `acquires`, call returns an error code | prints, handles it | **134**, same message |
| **E3** `borrows` result, NULL | prints, handles it | **0** |
| **E4** E1 plus `slot_close(s)` on the NULL path | releases the null | **0** |
| **E4b** E1 plus `slot_close(nullptr)` literal | releases a literal null | **0** |
| **E6** a REAL handle acquired, never released, one `slot_close(nullptr)` | **leaks** | **0**, silent |

### E-a. `borrows` does NOT have it

Measured, E3: no increment, no abort. The class is `acquires` only, on both of
its two positions.

### E-b. The `@` out-parameter DOES have it, and that is the worse half

Measured, E2. And it is worse because **the out-parameter is the shape C uses
precisely when a call can fail** — the two shipped examples both bind
`sqlite3_open` and `sqlite3_prepare_v2` that way, and both are functions whose
return code is the thing you are meant to check. The `acquires` fires on the
call, not on the success.

### E-c. The only correct program is one that releases a null — and the SAME construct hides a leak

E4 is the only shape of E1 that exits 0: **call the releaser on the null
handle**. That is the ergonomist's sentence, which it wrote about a hypothetical
array and which is in fact the **only** way to write a failure-handling program
today:

> *"The document would have me close a null handle to balance a counter."*

**And it does not have to be the null that came back.** E4b: a bare
`slot_close(nullptr)` discharges the obligation. E6 then closes the circle: a
program that acquires a **real** handle, never releases it, and writes one
`slot_close(nullptr)` **exits 0 in silence and leaks**.

> **So the escape from the false abort and the hole in the leak check are the
> same construct.** Telling authors *release the null* — which is the only advice
> available today — is telling them to write the line that makes the counter
> unable to see a leak. That is § 13's own admission generalised: *"the owing is
> counted, so a handle consumed twice hides one never consumed"* becomes **a null
> consumed hides one never consumed**, and the null is free to write.

Whether a C releaser even accepts NULL is the library's business and not the
language's: `sqlite3_finalize(NULL)` is a documented no-op, `fclose(NULL)` is
not. **The counter compels a call the header may refuse.** That half is unrun
here — this seat did not test a releaser that rejects NULL.

### E-d. How wide: MEASURED ON A SHIPPED EXAMPLE, which aborts

Real acquiring producers outside `tests/`: **five call sites in three files**.

| site | handled how | exposed |
|---|---|---|
| `examples/curl/main.hero:57` `curl_easy_init()` | **no NULL check at all** | yes |
| `examples/sqlite/main.hero:79` `sqlite3_prepare_v2` | `if rc != SQLITE_OK: return 0 - 1` | **yes** |
| `examples/ledger/db/sqlite.hero:286` `sqlite3_prepare_v2` | `return fail(code: "cannot_prepare"…)` | **yes** |
| `examples/sqlite/main.hero:99` `sqlite3_open` | closes the handle on the failure path | no |
| `examples/ledger/db/sqlite.hero:226` `sqlite3_open` | closes the handle on the failure path | no |

**Three of five, and the third is measured rather than argued.** Copy of
`examples/sqlite/main.hero` into a scratch directory, **one SQL string changed**
so sqlite refuses to prepare it, nothing else touched:

```
rows: 3
longest: -1
panic: 1 C handle(s) never given back — every call marked `acquires` owes one
       marked `consumes`, and this program is missing that many
run exit: 134
```

**The program handled the error exactly as it is written to, printed its
fallback, and was then aborted for it.**

### E-e. And the repository already wrote this down, in the file it happens in

`examples/sqlite/main.hero:59-61`, shipped, about the sibling `char **errmsg`
repair:

> *"`examples/ledger/` shipped the identical binding and leaked 40 bytes per
> refused statement; **this program never leaked because no statement here is
> ever refused, which is a fact about its SQL and not a defence.**"*

That sentence is the explanation of why `corpus` is green, it was written into
the corpus by this project, and **no brief and no seat cited it.** The ffi seat
reached the same conclusion from `examples/curl/main.hero:57` without knowing
the repository had already said it about a different call in the same class.

**The defect to file is therefore not *`acquires` counts a NULL*. It is two:**

1. **`acquires` counts a failed call**, on both positions, result and `@`
   out-parameter; `borrows` does not.
2. **`consumes` on a null discharges an obligation the null never created**,
   which makes the balance check blind to a real leak.

**Fixing (1) alone by telling authors to release the null makes (2) universal.**

## C — MEASURED where it could be. The ergonomist's own conditions, tested on the type where the rule already ships

The structural answer is above and this seat did not re-run the
compiler-engineer's bad free or the ffi seat's SEGV: both are already twice-run,
by the seat and by the coordinator, and a third run adds nothing. What was
**not** run by anybody is the ergonomist's conditions against the compiler, on
handles — the type where the rule the ergonomist is reasoning about **already
exists**. Two of the three come back with an answer the sitting does not have.

**Condition 1, *`_` must not drop an owed value*, is ALREADY UNMET for handles.**

```
function main()
    _ = slot_open(n: 1)     # marked `acquires slot_close`
    print(1)
```
**`heroes check` exit 0.** Run: prints `1`, then
*"panic: 1 C handle(s) never given back"*, **exit 134**.

So `_` does not discharge — the obligation survives, and the program dies at
runtime rather than at check. The ergonomist asked for the static refusal *"on
the model of `A _ never drops a T?`"*. **It does not exist on handles today**,
which means condition 1 is not a `ptr` condition at all: it is an open question
about the shipped rule that this sitting inherited and did not notice.

**Condition 2 is answered by § B3 above, and the answer is worse than the
question.** The ergonomist asked *"say what happens when the consuming function
is in another `extern` group"*, and declined to veto on the non-locality because
*"a later consuming member turns unmarked siblings into a loud compile error
naming the group"*. Measured: the demand crosses **modules**, the error is
raised in a file that imports nothing, and **the diagnostic names no group and
no location for the consuming declaration** — only the bare identifier. The
ground on which the ergonomist declined to veto is not there.

**Condition 3 is silent on the measured programs**, as § C of the first pass
says: the compiler-engineer's bad free contains no `borrows` call.

**So of the three conditions the ergonomist attached to its approval, one is
already unmet on the shipped rule, one rests on behaviour the compiler does not
have, and one does not reach the programs that vetoed it.** It is not refuted
by the other seats — **it is refuted by its own conditions, once anybody runs
them.** And the sitting is not panel 149's licenses-versus-causes: there the
seat could not see the cause; here the seat wrote the cause down, in its own
report, and scored it as somebody else's.

## D — MEASURED. The narrowed refusal is checkable, nobody priced it, AND the premise it rests on is FALSE

### D1. The four shipped call sites reproduce

Enumerated by script over `examples/`, `tests/`, `selfhost/` — every call of a
function this tree declares with a `consumes` parameter, whose argument is not a
bare name. **Exactly four**, one false positive discarded
(`selfhost/print/fmt.hero:547`, a Heroes function named `consumes_suffix`):

- `examples/ledger/db/sqlite.hero:246` `sqlite3_close(db.handle)`
- `examples/ledger/db/sqlite.hero:291` `sqlite3_finalize(statement.handle)`
- `tests/golden/check/ffi-consumes-a-borrowed-handle.hero:22` `free(h.cell)`
- `tests/golden/check/ffi-consumes-a-borrowed-handle.hero:33` `free(h.cell)`

**The compiler-engineer's count and its two-in-ledger split are confirmed.**

### D2. The narrowed refusal IS checkable without flow analysis, and breaks none of the four

*Elements AND composite in one program* is a **type-level** test: does some
`consumes` argument have type `T`, and some other `consumes` argument have a type
reached by projecting out of `T`. Both halves are facts the checker already
holds — it resolves the type of `f` and of `f.a[0]`, and panel 149's walk
already computes what a type REACHES. **No provenance and no flow are needed.**

And none of the four shipped sites is at risk: `Db`, `Statement` and `Holder` are
Heroes records that **no `consumes` parameter ever takes whole**, so the
conjunction never closes. Measured by reading all three declarations.

**Nobody priced it.** The ffi seat offered it as its veto's withdrawal condition
with no line count; the compiler-engineer priced route 5a and route 5b and was
never shown this one. That is the unpriced option in the set, and the set is a
measurement (CL-057).

### D3-D5. And the premise under the withdrawal condition is REFUTED

Three programs, one header each, `heroes check` exit 0 on all three.

| program | the C behind the array | exit | what the user sees |
|---|---|---|---|
| **D3** elements only, four independent `malloc`s | safe | **134** | the `+3` message, **two causes, neither is this** |
| **D4** elements **and** composite | double free | **133** | **0 bytes stdout, 0 bytes stderr** |
| **D5** elements only, **four interior pointers into one block** | **corrupt** | **133** | **0 bytes stdout, 0 bytes stderr** |

**D3 and D4 reproduce the ffi seat exactly. D5 falsifies its premise.**

> The ffi seat's veto withdraws *"if the refusal is narrowed to elements AND
> composite in one program — **the only shape the seat could make corrupt**"*.
> **D5 is elements only and it corrupts**, because the four elements are interior
> pointers into one allocation. That is not a contrivance: it is
> `jpeglib.h`'s `quant_tbl_ptrs`, one of the two headers this sitting's own
> shared brief cites as the reason a mark cannot carry the number. Its tables
> come from the decompressor's pool and `jpeg_destroy` frees the pool.

**What the seat measured was a property of the header it wrote — four separate
`malloc`s — not a property of the shape.** Whether an element release corrupts
is decided by the C allocator behind the array, which no mark on either
declaration can see. **That is panel 147's axis again, and it cuts the narrowed
refusal as cleanly as it cut the broad one.**

### D6. And the repair all five seats agree on is only ever READ on the safe program

D4 and D5 both die in the allocator at **133 with zero bytes on both streams**,
before the exit counter runs. **The repaired runtime message can only ever
appear on D3 — the program that corrupts nothing.** The compiler-engineer wrote
*"what it fails to catch: everything"*; the measurement is sharper than that.
**The message is improved for the one program that is memory-safe and is silent
for both programs that are not.** That is worth landing on §4.17 grounds and it
should be landed knowing this, rather than as a partial answer to defect 038.

---

# Claims asserted without the command that settles them

The instrument check first, because one seat's whole table depends on it.
**The spec-warden's instrument reproduces exactly**, independently, in this
seat's own copy: `heroes measure spec/heroes-spec.md --refresh` with `.env`
sourced prints `7974`, `claude-opus-5`, digest `7066ff27485400b0` — **the number
and the digest both**. Its refutation of the coordinator's *no key* premise is
confirmed.

**And the citation failure panel 149 paid for did not recur.** `grep` over the
five reports: panel 147 cited by three seats, 148 by three, 149 by four. Zero
became ten. One structural note: the **llm-ergonomist read neither the shared
brief nor any sitting**, by its own charter — *"Input read:
`150-briefs/llm-ergonomist.md` and `spec/heroes-spec.md`. Nothing else."* So the
shared brief's binding instruction *"read these three sittings before you
answer"* is unsatisfiable for one seat in five, and the brief does not say so.

## 1. ffi-pragmatist — *"the only shape the seat could make corrupt"*

**The load-bearing one, and it is false.** It is a negative claim about an option
set, and CLAUDE.md § RUN IT asks such a claim to name what was searched for. The
report names no shapes tried. **D5 above corrupts and is elements-only**, on the
header this sitting's own brief cites. The seat's Q2 veto-withdrawal condition
is therefore built on a premise that does not hold, and the narrowed refusal it
offers would let D5 through.

## 2. historian — *"that is not a coincidence, it is what an audited region buys"*

A **causal** claim about the Heroes implementation, made by the one seat with no
way to run one, resting entirely on the coordinator's brief. Its own preamble
says *"no Heroes number here is the seat's own measurement and each is
attributed"* — the number is attributed, but a conclusion about the mechanism is
not a number, and the attribution does not cover it. **B2 and B3 falsify it**:
the mechanism is not a region. The precedent the historian brought is sound and
well-sourced; the bridge from clang's nullability pragma to Heroes is the
unrun step, and it is the step its condition 2 stands on.

## 3. compiler-engineer — Q1-B generalised from one program

*"the binding author pays one line"* was run **once, for one type**. **A3: a
second `tag void` in the same program is `error[duplicate_tag]`.** The route
Q1-B sends every `ptr` author to works once per program, and the defect's own
header needs it twice. **This is the shape-next-door test CLAUDE.md § RUN IT
asks for — *one field, none, padded, nested, tagged, generic, empty* — applied
to the route rather than to the repair**, and it was not done.

Its companion claim, *"it is type-separated: handing a `ptr` where a `Blob`
belongs is `error[type_mismatch]`"*, is true of Heroes and **was never run on the
C side. A4: the cross-wire is invisible to clang under `tag void`**, because
`void *` converts to anything. The seat's own §4.19 argument runs against its own
route and it did not test it.

And *"Q1-B breaks nothing"* was run against the tree. It was **not** run against
the one thing Q1-B has to do, which is bind `netdb.h`. Unrun.

## 4. spec-warden — the right measurement, the wrong text priced

Row 2 of its own table says the compiler keys on the type **program-wide**. The
draft it prices at **+7 real** rewrites *group* to **any `extern`**, which does
not say program-wide and does not say **across modules** — and **B3 measures a
refusal raised in a file that imports nothing and never names the arming
declaration**. The number is honest and the text under it is short of what the
same report measured. **D1 is the right shape and not yet the right sentence**,
and the delta for the missing clause is unpriced.

## 5. llm-ergonomist — one inference wearing a reason's clothes

*"It approves anyway, because that double-free is not a class the rule creates."*
The connective is the tell CL-018 names. The seat cannot run anything, which is
its charter and not a fault — but its own preceding sentence establishes the
opposite, and the honest form was *a question rather than a premise*. **It is the
one place in five reports where a seat had the finding and wrote it down as a
reason to discount itself.**

**Credit where the discipline held**: the historian's § *What could not be
sourced, said plainly* declines a second retraction it could not find and marks
the clang `Optimistic` reading **unverified** with the fact named as
load-bearing. The compiler-engineer labels every claim about a patched rule
unrun in its first paragraph and again in its last. The ffi-pragmatist opens by
retracting a whole round of experiments run against a stale copy. Those three
paragraphs are why the failures above are findable at all.

---

# The question the sitting did not ask

Q1 asks *when must a `ptr` producer be marked*. Q2 asks *what does a whole-value
mark owe*. **Both are questions about the MARK.** Panel 149's lesson made the
seats ask about the consumer as well, and they did. **Nobody asked about the
INSTRUMENT.**

Line up what this sitting has now measured, and it is one fact seven times:

| | |
|---|---|
| E1, E2 | a call that **failed** and returned NULL is counted as an acquisition |
| E4b, E6 | `slot_close(nullptr)` **discharges** an obligation the null never created, and hides a real leak |
| D3 | a program that **corrupts nothing** is aborted, against a message naming two causes that did not happen |
| D4, D5 | the two programs that **do** corrupt die in the allocator at 133 with **zero bytes on both streams** |
| § 13 | *"The owing is counted, so a handle consumed twice hides one never consumed"* |

**Every one of them is the runtime holding a NUMBER where it needs a SET.** The
llm-ergonomist is the seat that named it — *"that is the document admitting its
count is a number and not an identity"* — and its report calls it the sharpest
thing it found. **No brief and no other seat followed it.**

**And the repository already wrote the answer down, in the file every seat
patched.** `runtime/parts/alloc.c:174-178`:

> *"this is a counter and not a set. Telling a double release from an unmarked
> producer needs the IDENTITY of each handle, so the runtime would hold every
> live pointer and every call site would pay a lookup — **a different
> instrument, not a better sentence**."*

**That is a cost argument with no cost in it.** No number, no benchmark, no
`/usr/bin/time -p`. CLAUDE.md § Precedence is explicit about this exact shape:

> *"Never slow the compiler down sits at rank 5, so a guard that closes a
> corruption class lands, **with its cost measured and reported rather than
> argued**."*

A handle set closes D4 and D5 — a double release is a `consumes` of an address
not in the set, catchable **before** the allocator traps, where today the user
gets zero bytes. It closes E4b and E6 — a null was never in the set, so
releasing it discharges nothing. It closes E1 and E2 — a producer that returns
NULL puts nothing in the set. It closes D3 — four addresses released, one
registered, and the message can say **which**. Robustness is rank 3 and speed is
rank 5, and the one place the trade is written down decided it at rank 5 without
measuring rank 5.

**The brief foreclosed the question in its own words**: *"Nothing has to be
built; something has to be refused."* That sentence is what kept five seats
arguing about refusals over two days while the instrument under both defects
went unexamined.

**And the cost of not asking is visible in the git log.** The message landed
`7e3bb986` on 2026-09-14 and has been rewritten twice since — `50859c5f`
2026-09-14, `bd03f8d9` 2026-09-15. **Route 6, which all five seats converge on,
is the third rewrite in two days and the fourth version of the sentence.** The
spec-warden saw the pattern and named it — *"a closed list, short for the third
time in two days"*, with CL-057 and panel 087's veto of a closed abort list —
and then the sitting adopted another entry on the list. **D6 above says what
that entry buys: a better sentence for the one program that is memory-safe, and
silence for both programs that are not.**

> **The question the sitting should have asked, and which no seat was briefed
> on: should the runtime hold handle identities rather than a count, and what
> does that actually cost?** The compiler-engineer is the seat that can price
> it. It was asked about `+51` lines of refusal instead.

---

# Summary for the coordinator

- **A.** The two routes are one route and they do **not** compose as peers. `tag
  struct X` **subsumes** `tag void`. `tag void` is **capped at one per program**
  (`duplicate_tag`, measured) and **loses the clang cross-wire check** (measured,
  clean build on a `malloc` handed to `freeaddrinfo`). The compiler-engineer
  adopted the subsumed half. The composed sentence — *a `tag` names a C type
  name* — is unpriced by anybody.
- **B.** The warden's second falsehood is **this sitting's business**: it is what
  the historian's Q1 approval rests on, and it is **false in a stronger way than
  the warden measured** — the demand crosses **module** boundaries and the
  diagnostic names no location for the arming declaration. D1 at +7 is the right
  shape and the wrong sentence.
- **C.** The ergonomist is refuted **by its own conditions**, not by the other
  seats: condition 1 is already unmet on handles (`_ =` an acquiring call checks
  at 0, aborts at 134), condition 2's stated ground does not exist, condition 3
  does not reach the vetoing programs. Route (A) satisfies all three by
  construction, and the ergonomist was never shown it.
- **D.** The narrowed refusal **is** checkable without flow analysis and breaks
  **none** of the four shipped sites — and **the premise under it is false**:
  elements-only corrupts too, on `jpeglib.h`'s own shape. Nobody priced it, and
  pricing it is now beside the point.
- **E.** Two defects, not one. `acquires` counts a failed call on **both**
  positions (`borrows` does not); and `consumes` on a null discharges an
  obligation, which makes the balance check **blind to a real leak**. Measured
  on a **shipped example**, unmodified but for one SQL string: it handles the
  error as written and is aborted at 134 for it. Three of five real producer call
  sites are exposed, and `examples/sqlite/main.hero:60` already says in writing
  why the corpus is silent.
- **The question**: is the counter the right instrument? The one place that was
  decided is a runtime comment that argues the cost and does not measure it,
  against a precedence rule that says a guard closing a corruption class lands
  with its cost **measured**.

---

# Addendum, run after the summary: the two headers checked on this machine

Both cited by the ffi-pragmatist, one on each side of its Q2 veto. Read from
the installed headers, not from documentation.

**`jpeglib.h` confirms D5 and goes further than D5.** `/opt/homebrew/include/jpeglib.h:392`
and `:638` carry `JQUANT_TBL *quant_tbl_ptrs[NUM_QUANT_TBLS]`. Line 1016
declares `jpeg_alloc_quant_table(j_common_ptr cinfo)` — allocated **from the
object**, not from `malloc` — and **`grep` for a per-element releaser finds
none**: there is `jpeg_add_quant_table`, and after that only
`jpeg_destroy` / `jpeg_destroy_decompress`, whose own comment at :1140 says
*"You can just call jpeg_destroy_(de)compress"*. **On the header this sitting
cites as its motivating case, the element release is not merely wrong — it is
unwritable**, which is the ffi seat's *zero per-element releasers* and the
historian's GObject `transfer container` arriving at the same place.

**`raylib.h` refutes one of the ffi seat's two veto examples.**
`/opt/homebrew/include/raylib.h:1497`:

```c
RLAPI void UnloadFontData(GlyphInfo *glyphs, int glyphCount);
```

**That takes the ARRAY and a count. It is a whole-value release in ONE call, not
a per-element release.** The seat's veto reads *"releasing elements IS sometimes
correct … raylib ships `UnloadFontData(glyphs, glyphCount)` beside `UnloadFont`.
Both correct, on different objects"* — and the seat itself notices the count
without drawing the consequence: *"`UnloadFontData` carries an explicit count."*
`LoadFontData` at :1495 returns one `GlyphInfo *` with `int *glyphCount`, so it
is **one allocation, one obligation, one releasing call** — which is panel 149's
*one mark is one obligation on the whole value*, and the historian's
`g_list_free_full` shape. **The seat cited as a counterexample the very rule it
exemplifies.**

**`av_buffer_unref` is UNRUN here**: `libavutil/buffer.h` and `libavutil/frame.h`
are not installed on this machine and this seat did not fetch them. That half of
the veto stands unexamined, and it is now the **only** shipped example left
supporting *releasing elements is sometimes correct*. **Somebody should open it
before the veto is weighed**, because if it goes the way `UnloadFontData` went,
the ffi seat's Q2 veto has no shipped example under it at all.
