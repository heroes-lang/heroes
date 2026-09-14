# Panel 148 — the mark is written and never inferred, and it names what ends the life

**Sat 2026-09-14**, M-marked-acquisition, full five seats plus the completeness
critic. Convened because the form panel 147 admitted has **surface**, and
`spec/` is a panel trigger.

**Status: `provisional — author ratification pending`.**

---

## The proposal, as the seats received it

Panel 147 had already ruled, and the author ratified it in full the same day: a
form enters, **the obligation is marked where it is CREATED, on the acquiring
call, never on the type**, and **the compiler never picks the release call**.
This sitting decides the **surface** and nothing else.

- **A** — a new contextual word: `@statement: CStmt acquires`,
  `curl_easy_init() -> Curl acquires`. **+64 vendored** as briefed.
- **C** — no new word: the obligation is read off the group's own `consumes`
  function. **+56 vendored**.

A third that nobody listed was explicitly invited, and three seats produced one
each.

---

## The verdict table

| seat | A | C | its veto scope |
|---|---|---|---|
| **compiler-engineer** | approve | **VETO** | unqualified |
| **ffi-pragmatist** | approve | **VETO** — cast outside its stated scope: it measured ABI untouched | ABI breakage |
| **spec-warden** | object *as drafted* | **VETO** — also outside scope: no route breaches the budget | budget breach |
| **llm-ergonomist** | approve, 2 conditions | **VETO** | non-local constructs |
| **historian** *(advisory)* | approve | object | none |

**Unanimous against C on five independent measurements.** Two of the four vetoes
are formal within their seat's stated scope — the engineer's and the
ergonomist's — and the record says which is which rather than counting four.

---

## Why C died, five ways, none of them taste

**The ffi seat opened nineteen real headers** and applied one test: does a
single header declare both a releaser for handle type `T` and a function handing
back a borrowed `T`? **12 of 19 — 8 of the 9 the brief named.** And the pair
that settles it on its own, adjacent lines:

```c
X509 *SSL_get0_peer_certificate(const SSL *s);   /* must NOT free */
X509 *SSL_get1_peer_certificate(const SSL *s);   /* MUST free     */
```

It compiled a probe taking both through one function type: **clang accepts,
zero warnings under `-Weverything`.** Nothing in the header for a rule to key
on. Then it stopped arguing and **reproduced the corruption twice against real
libraries**: `AddressSanitizer: attempting free on address which was not
malloc()-ed` inside `sqlite3VdbeDelete`, and a SEGV inside
`curl_slist_free_all`. **Under A a mistake loses a leak check; under C a
mistake compels a release the program must not make.**

**The critic found 12 of 19 is a LOWER BOUND.** A Heroes group names one header
and clang resolves its includes: `curl.h:3213` includes `easy.h`, which the seat
had filed as safe-alone while it holds both `curl_easy_init` and
`curl_easy_duphandle`.

**The engineer found C is panel 147's R1 in a group-shaped hat** — R1 refused a
rule keyed on the type because the same type comes back owned from one function
and borrowed from another, and `(group, type)` cannot tell them apart either.
`curl_slist_append` is not exotic: N appends and one `curl_slist_free_all` is
how you set an HTTP header, and under C a correct program aborts saying N-1
leaked. **And C has no off switch**: the only way to disarm it for
`sqlite3_db_handle` is to delete `consumes` from `sqlite3_close`, which also
deletes panel 145's use-after-free rule for that type. One word, two meanings a
real binding must separate.

**The ergonomist, reading three label-stripped specs and nothing else, found the
same hole blind** and vetoed on locality: under C the `extern` group is **byte-
identical** to today's, so nothing on any line says which calls acquire. It
looked for four escapes from the borrowing accessor and each fails from the text
alone; the one the language steers you to is a **double free**.

**The historian found the purest shipped instance of C and what it cost.** ARC
infers ownership from the method name — and *never removed the annotation*: it
relocated it to the exceptions and then paid four attributes, a fifth for family
control, a namespace prohibition (*"you cannot give an accessor a name that
begins with `new`"*), and a warning class for when the name is unknown. Apple's
own `CFBase.h` comment begs you to rename your functions instead.

**And Heroes has already made this choice once, correctly.**
`examples/ledger/db/sqlite.hero`'s `@error: cstr owned sqlite3_free` is an
explicit mark on the acquiring parameter that names its releaser — exactly GCC's
`__attribute__((malloc (fclose, 1)))`.

---

## What the critic changed, and it changed the resolution

**1. The three "third options" are two axes, and one composite nobody proposed.**

- the ergonomist's *diagnostic* and the ffi seat's *`borrows`* are **the same
  axis**, and `borrows` **dominates**: two producers of `CURL` in one group
  (`curl_easy_init`, `curl_easy_duphandle`) make a per-type completeness check
  satisfiable while an unmarked producer leaks in silence;
- the historian's `acquires <releaser>` is **orthogonal** — it does not close a
  forgotten mark, it enriches the mark's payload.

**So the maximal resolution is both, and no seat proposed it.**

**2. The hinge cost the engineer measured is not the cost, and the same sentence
would have killed the other two options.** The engineer priced a group-keyed
rule at **+90 lines over a key the language does not define** — true of a
*group*, and the critic verified every line of it, and then found the compiler
had solved the problem already: **`selfhost/check/decls.hero`'s
`one_tag_one_type` is 17 lines, program-wide, and its own comment is the answer
to the hinge**, written before the sitting convened —

> The walk is over the declarations rather than the groups because a group is
> flattened in the parser and no later pass knows the word.

**One tag is one handle type across the whole program**, proven by probe:
`error[duplicate_tag]` fires across two groups. So a completeness question keyed
on the **TYPE** needs no group identity and no new kind of table. **This does
not save C**, which dies on correctness independent of the key — but the
synthesis must not carry "a new table over a destroyed entity" as C's cost,
because it is wrong for all three options.

**3. The historian's option is the cheapest correct text on the table, and no
seat could see it.** The coordinator measured it after four of five reports were
in: **+66 vendored**, against the warden's repaired A at **+70**. It already
carries the warden's merge and amends **both** grammar positions.

**4. Two defects in the coordinator's own brief, beyond the two already known.**
The warden found A's grammar does not derive A's own example (`Member` was never
amended, so `-> Curl acquires` is underivable — the same shape panel 133 caught
three days earlier) and that A duplicates a clause the spec already carries for
`lease`. The critic found **the grammar is also too WIDE**: `@` is optional in
`CParam`, so `f(db: CDb acquires)` on a plain input parameter is derivable and
the prose does not define it. `consumes` carries the same looseness today.

**5. Nobody judged the budget A actually breaches.** *"`ast.hero` 505 → 508"*
reads as before-and-after and is not: 505 and 1156 are the **DECIDED ceilings**;
today's measures are 503 and 1153. **Landing the mark moves two ceilings in
`suite_layout.hero`'s table**, which that file treats as an act owing a written
reason. The warden judged the spec budget; the layout budget went unexamined.

**6. The timing number is discarded.** The engineer reported `+0.17 s, +1.2%`
and declared the machine still. The five seats ran **concurrently** — the ffi
seat was linking `libsqlite3` under ASan and the warden was running
`heroes measure` inside that window. CL-025 is unambiguous: parallel work is
free on correctness and forbidden on duration. **A panel that runs its seats in
parallel cannot produce a valid timing from any of them**, and the procedure
must serialise a seat that times or ask for no timing at all.

**7. The blind is not enforced, and the critic proved it of its own prompt.**
The ergonomist disclosed, unprompted, that a commit subject in its process
environment told it which variant had been refused. The critic confirmed the
harness injects the same block into **every** subagent — and that it also
carries **the whole of `CLAUDE.md`**, which the ergonomist did not disclose and
which its own agent definition forbids it. **The blind cannot be asserted, only
hoped for.** Its findings survive, because the critic re-checked each against
the compiler the seat never opened and each holds; what does not survive is
counting its agreement with two other seats as independent. **It is counted
once.**

---

## Resolution — `provisional — author ratification pending`

**R1. Option C is REFUSED**, on measurement and not on cost: 12 of 19 real
headers make it wrong, and the wrongness is a compelled free, reproduced twice
under ASan. It goes to design.md Part 6 as a row of its own, with the falsifier
the ffi seat wrote: *a rule, derivable from header text alone, separating
`SSL_get0_peer_certificate` from `SSL_get1_peer_certificate`, with a measured
false-acquire rate of zero across the twelve.*

**R2. The mark is WRITTEN and it NAMES what ends the life.** The text that lands
is the historian's, measured at **+66 vendored**, which already carries the
warden's merge and both grammar positions:

> `acquires sqlite3_finalize` after a handle result or `@` out-parameter says
> the call begins that handle's life and names the one that ends it, which the
> program owes it.

It is chosen over the bare word on § 4's own rule — the most robust resolution,
never the cheapest — and it is **also four tokens cheaper** than the corrected
bare word, which is the sitting's one free lunch. Its parser exists:
`owned_marker` reads that exact shape in both positions today.

**R3. The grammar is repaired in BOTH directions**, the narrow the warden found
and the wide the critic found: the result position gains the mark, and the mark
is admitted only where the prose admits it.

**R4. The spec sentence gains the balance clause.** The counter is a balance,
not a matching: acquire two handles and consume one of them twice and the net is
zero — one leak and one double free, at exit 0. The engineer said it belongs in
the sentence now rather than in a defect later, and no draft on the table
contains it.

**R5. The completeness diagnostic ENTERS, keyed on the TYPE and not the group,
and takes its own step.** A handle type with a `consumes`-marked releaser and an
unmarked producer is a compile error naming both lines. This is the ffi seat's
`borrows` axis, restated on the key the critic measured — `one_tag_one_type`'s
17 shipped lines rather than 90 over an undefined key. It closes the one silent
failure the written mark leaves.

**R6. This sitting does NOT settle loud-exit versus compile-error, and the brief
settled it by accident.** Both drafts ended *"aborts when `main` returns"*, so a
compile-error instrument was never on any seat's page — including the seat
holding the comprehension veto. **That question returns to the milestone's own
second item, undecided**, and the critic's § 5 is what it must answer first:
every draft on the table amends `Member` and `CParam`, so the mark is
**extern-only**, while this corpus acquires inside **Heroes wrappers** one module
away (`opened`, 7 call sites; `prepared`, 6). A runtime counter does not care.
**A compile error would need somewhere to write the fact on an ordinary
function, and no text on this table has room for it.**

**What conservative would have been** (CL-040): the bare `acquires`, the
warden's merged text at +58, with no named releaser and no completeness
diagnostic. It is eight tokens cheaper than what was adopted and it leaves both
the mismatched-deallocator class and the forgotten-mark class open. Recorded so
the author can take it.

---

## Predictions to score

| # | whose | prediction | checkable at |
|---|---|---|---|
| 1 | historian | the next handle-returning SQLite function the corpus binds is a **borrowing** one | the next binding that grows |
| 2 | ffi-pragmatist | under C, `sqlite3_finalize consumes` + `sqlite3_next_stmt` cannot be written; under A both compile | M-marked-acquisition |
| 3 | compiler-engineer | the unbuilt half lands at **120–280** code lines, and `heroes check selfhost/main.hero` moves under 1% | the milestone close |
| 4 | spec-warden | the landing text's **real** count lands in **7864–7896**, leaving ≥236 free | the commit that lands the spec |
| 5 | spec-warden | the shipped tree needs **exactly three** `acquires` marks and no fourth | the milestone close |
| 6 | ergonomist | under the written mark, the dominant failure becomes the loud abort, not the silent leak | a Part 11 run |
| 7 | historian | its own option costs *"more than A's +64"* | **SCORED — HELD, and beaten.** Measured +66: right in sign, and four tokens under the corrected A it was compared against |

---

## What this sitting did not do

It did not re-open panel 147's R1. It did not choose the instrument (R6). It did
not price the completeness diagnostic beyond the critic's 17-line precedent.
And **it produced no valid timing**, which is recorded as a fact about the
procedure rather than omitted.

## Author's verdict

**Pending.** The item is open in `docs/work/DECIDE.md` as `panel 148`.

**What a yes settles**: R1, C refused to Part 6 with its falsifier; R2, the mark
is written and names its releaser, at +66; R3, both grammar repairs; R4, the
balance clause; R5, the completeness diagnostic keyed on the type, in its own
step.

**What a yes does not settle**: R6 — whether a missed release is a loud exit or
a compile error — which returns to the milestone undecided, and which the
extern-only placement of the mark must be answered against first.

**The conservative resolution is at R5's end** and the author may take it
instead: the bare `acquires` at +58, no named releaser, no diagnostic.
