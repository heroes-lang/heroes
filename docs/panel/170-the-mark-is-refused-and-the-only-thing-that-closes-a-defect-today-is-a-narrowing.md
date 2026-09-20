# Panel 170 — the mark is refused, and the only thing that closes a defect today is a narrowing

Convened 2026-09-20, at M-declared-extents, on **the mark that says a C
parameter retains what it is handed**. Full panel: five seats and a completeness
critic, six briefs written to disk before any seat started, **the critic run
before the synthesis** (which is `/panel` § 3b's order and which the previous
sitting got wrong), and the working tree frozen throughout.

**Four open defects converged on one missing thing, and the sitting measured
that the thing they converged on cannot be one thing.**

## The proposal, verbatim

> Four open defects converge on one missing thing. **Nothing lets a declaration
> say a parameter RETAINS, and nothing refuses a lend that reaches one.** What is
> the mark, what does it refuse, what does an unmarked retaining parameter cost,
> and does it also answer 072?

## The resolution, in one line

**The mark as briefed is REFUSED on two vetoes.** What survives is smaller and
sharper: **narrowing `one_tag_one_type` at `tag void` is adopted and closes
defect 072** — the only thing in this sitting that closes an open defect at
`check` with the language it already has. **The default flip is the only
candidate that would close 066 and 068, its cost is measured at eight refusals
of eight correct programs, and it is queued for the author rather than taken.**
**070 is a separate fact that no candidate touches**, and its route is named.
Three defects are filed, and an emission divergence one seat reported as a
question is **resolved: it was a stale binary**.

## The verdict table

| | compiler-engineer | ffi-pragmatist | spec-warden | llm-ergonomist | historian |
|---|---|---|---|---|---|
| **the mark as briefed** | **VETO** | **object**, and **veto** any mark naming an ending call | **object** | **object** (parameter form), **VETO** (group form, locality) | approve **with two corrections** |
| **section** | design.md §1.7, §4.17 | §4.19's own retention paragraph | §1.6, §1.2, CL-005 | — | — |
| **cost** | **69 code lines, 7 files**, spec +62 real; four ceiling breaches all priced with their splits, and *"the ceiling is not my objection"* | none; ABI diff is 10 lines and the call is byte-identical | +79 to +99 real depending on spelling; **+3** for the own-slot fix | — | — |

**No budget veto.** Baseline 8201 real, ceiling 10240, worst draft 8360. The
warden declined a veto it did not have.

## THE VETO, and it is one sentence

**The compiler-engineer:**

> `keeps`'s only honest repair is a lease. On a parameter that **frees**, that
> repair **is** defect 070. Retention must **admit** a lease; give-away must
> **refuse** one. **Opposite admissions, so not one word.**

It ran both halves: exit 133, stderr 0 bytes. A single word would hand the author
a `certain`-shaped fix that is the next crash, against design.md §4.17.

## THE EXPERIMENT THAT SETTLES THE SPELLING, and it is the ffi seat's

`keeps sqlite3_finalize` on `sqlite3_bind_blob`, written in C against the real
`sqlite3.h` and linked to the real `libsqlite3` 3.51.0, **in both possible
readings of *ended by*, against all three modes of argument five**. Clang
accepted every cell:

| | `SQLITE_TRANSIENT` | `SQLITE_STATIC` | a real destructor |
|---|---|---|---|
| **A** the compiler releases at `end_fn` | 0 | 0 | **ASan double-free, exit 134** |
| **B** `end_fn` releases | **leak**, `live_blocks=1` | **leak**, `live_blocks=1` | 0 |

**Neither reading is right for more than two of three, and the three are one
declaration.**

**And the named call is the wrong call.** On `examples/ledger/main.hero:67-72`'s
exact shape with 1000 rows: `frees_before_finalize=999`. **SQLite disposes at the
next bind, not at the finalize**, and reading A's cost on the mode the ledger
passes is `peak_live_blocks` **1 → 1000**.

**The census, with clang's own AST as the ruler**: 977 functions, **542 pointer
parameters, 25 disposer parameters, 0 classifiable as retaining**. raylib 6.0 and
curl 8.7.1 declare **zero** disposer parameters across 693 functions. And
`curl_easy_setopt` is variadic, so in the library that most needs this **there is
no parameter to mark at all**.

## THE FINDING THAT WOULD HAVE MADE IT CLOSE NOTHING, and it is the warden's

`selfhost/check/marks.hero:64-84` sweeps `consumes|acquires|borrows` and
`refuse_unread` fires **unless a handle is behind the type**. Defects 066, 068
and 070 all sit on `cstr` and `ptr`. Run: `atoi(s: cstr consumes)` is
`error[unread_mark]`, exit 1.

**So a `keeps` landed inside that alternation is refused on exactly the types the
defects live on.** Its own grammar slot, beside `counted_by`, costs **+3 real**
and is the cheapest correctness in the warden's table. A sitting that had adopted
the mark without this would have shipped a word that closes zero and looks like
it closes four.

## AND THE ANSWER TO THE SYMMETRY QUESTION, which two seats reached separately

The brief asked whether `keeps` is symmetric with `owned`, `consumes` and
`acquires`, which are already the author's words.

**The spec-warden**: *the three existing marks fail **safe** when omitted and
`keeps` fails **unsafe***.

**The llm-ergonomist**, reading nothing but the spec, reached the same place by
counting: **six of the seven optional marks in § 13 have a compiler-enforced
absence**, so the document teaches, row by row, that *absence is the checked
default*. `keeps` would be the first whose absence is an unbacked claim about a C
library's run-time behaviour, written in the same slot. Its sentence:

> **The proposals do not add a check to the unmarked case, they subtract the
> warning from it.**

Three lines apart the document would read *nothing checks it* and *a lend is
refused there*, and the cheap resolution a reader reaches for is *the mark is the
check*.

## THE HISTORIAN'S CORRECTION: the lesson is the DEFAULT, not the mark

Swift is the precedent and it is exact: SE-0103 made non-escaping the **native**
default **and in the same proposal made the foreign default the opposite** — the
Clang importer marks every imported C and Objective-C block or function-pointer
parameter `@escaping` **unless the header carries `noescape`**. Optimistic where
Swift can read the callee's body; **pessimistic where it cannot**.

**This sitting proposed the optimistic default in the one region Swift refused to
apply it to.**

And the definitional trap, from Clang's own `AttrDocs.td`: *"Users are
responsible for making sure parameters annotated with `noescape` do not actually
escape. **Calling `free()` on such a parameter does not constitute an escape.**"*
So an escape-shaped mark leaves **defect 070 outside its own definition**, which
is the engineer's veto arriving from the library side.

**The asymmetry itself is acceptable and universally admitted.** Rust RFC 3484:
*"we are asserting to the compiler that these declarations are correct. **The
compiler cannot itself verify these assertions.**"* Rust's answer is to make
declaring the foreign signature the attributable unsafe act. D: *"For other
functions scope semantics must be manually enforced."* Java FFM: *"the linker
runtime cannot validate linkage requests."*

**Positive polarity is analyser-only everywhere.** *This parameter is retained*
ships as Clang's `ownership_holds` and SAL's `__drv_aliasesMem`, and **neither is
a compiler error anywhere**. Heroes would be first, and first is a cost to price
rather than discover.

## WHAT THE SITTING ACTUALLY CLOSES — the critic's measurement

The critic rebuilt all four defects from scratch on a current copy and then
judged each surviving candidate against them.

| candidate | what it closes |
|---|---|
| the ffi seat's weak mark, one word, no ending call | **0 of 4** — every reproducer is unmarked |
| the ergonomist's W0, eight tokens of prose | **0 of 4** |
| **the historian's default flip** | **066, 068 and the lend shape, with no author action** |
| nothing | leaves `**OPEN: 4**`, and a fourth sitting running |

**And the default flip's cost is measured, by the seat nobody assigned it to:**
**eight lend call sites in `examples/`, in four files, and all eight are correct
today** — `sqlite3_open` twice, `exec` twice, `prepare_v2`, `bind_text` with
`SQLITE_TRANSIENT`, `curl_easy_setopt` with a URL. **Eight refusals of eight
correct programs.**

**No candidate touches defect 070 as filed**, because it hands a **lease**, and
every candidate either admits a lease or speaks only about lends.

## THE ONE THING THAT CLOSES A DEFECT TODAY — narrowing `one_tag_one_type` at `tag void`

Found by the critic as a route nobody listed, and measured.

`one_tag_one_type` lives at `selfhost/check/decls.hero:313` — **not at
`selfhost/handles.hero`, where defect 072's entry points**; `grep -c` there is
zero, and the entry is corrected.

**The rule's own stated reason is written about `struct s *`**, where clang is a
second judge of the spelling. **At `tag void` no second judge can exist**, so the
rule buys nothing there and costs defect 072 — two C allocator families forced
onto one Heroes type, each freeing the other's blocks at `check` 0 / `build` 0 /
`run` 0 with **zero ASan lines**.

**And the rule's own claim that the mutant survives every instrument is falsified
by a run**: with two distinct tags the crossed program is `error[type_mismatch]`
at **check, exit 1** — an annotatable refusal.

**The two seats that claimed 072 landable each had half of it**, and the critic
settled the matrix:

- both families as `tag void` → `duplicate_tag` at check, which is the defect;
- invented distinct tags, frees **crossed** → **`type_mismatch` at check, exit 1**
  — the engineer's half is true;
- invented distinct tags, frees **matched** → check 0, **`build` 1
  `ffi_unknown_name`** — the row the engineer had no entry for, and where a
  shim-free route dies.

So two tags are declarable today **only with a shim header**, which is the ffi
seat's twelve lines, and the engineer's relaxation of the rule is **a real third
mechanism, unbuilt and priced at a condition in a 17-line function in a file at
219 of 300**.

**GCC shipped the precedent for the other half.** GCC 11.1, 2021-04-27:
`__attribute__((malloc(deallocator)))` plus `-Wmismatched-dealloc`, **enabled by
default**, a declaration-site allocator/deallocator pairing diagnosed at the call
site. **Two mechanisms, not one**, which is the historian's answer to the
sitting's fourth question.

## THE EMISSION DIVERGENCE IS RESOLVED, AND IT IS A STALE BINARY

The ffi seat reported, as a **question rather than a defect**, that once the
compiler emitted `hero_unreachable()` for `d: free` where minutes later the same
binary emitted `t6 = free;`, and that it could not reproduce it in twenty
cold-cache emissions. It preserved both files, which is what made the answer
findable.

The critic found it. **A step-10 and a step-11 compiler emit the gate line;
step-12 — this milestone's own *"a gate that refuses a form and then emits it is
not a gate"* — and HEAD emit `t6 = free;`.** Stripping `#line` paths, the
critic's step-10 emission diffs **empty** against the seat's preserved first
file, and its HEAD emission **empty** against the second. Determinism holds:
twenty cold-cache emissions give one hash each.

**The mechanism is procedural and it is worth more than the finding.** The two
seats shared one scratchpad, and the compiler in it hashes identical to the
critic's HEAD build: **one seat rebuilt the other's compiler underneath it.** The
compiler-engineer independently reported the same trap against itself — its first
copy was six commits behind and it re-ran everything. **Twice in one sitting**,
and it is `.claude/rules/verification.md` § *The compiler that judges is a build
artifact* arriving inside a panel. **A seat's copy is its own, and `/panel`'s
working rules should say so.**

## Three defects this sitting found, and one correction

- **`owned <fn>` on a plain input parameter is grammar-admitted, prose-undefined,
  and crashes the backend.** Found by the spec-warden and reproduced by the
  critic: `check` **0**, `build` **2**, `internal error: compiling the generated C
  failed`, caret on the caller's line. **And that slot is what two candidate
  routes wanted to build on.**
- **One Heroes declaration cannot reach all three of `sqlite3_bind_text`'s
  modes.** `d: ptr` takes `nullptr` and refuses a function name; `d: (function(ptr)
  -> ())` takes the function and refuses `nullptr`. **Measurement 037's give-away
  and the shipped ledger are two different declarations of the same C function,
  each forbidding the other's arguments.**
- **A fourth shape of the give-away, unfiled**: a plain `.cstr()` lend into a
  freeing callee, `check` 0, exit 134, stderr 0 bytes — and `keeps` would never
  fire on it.
- **Correction**: defect 072's entry points at `selfhost/handles.hero` and the
  rule is at `selfhost/check/decls.hero:313`.

## Two routes named for the next sitting rather than left to be rediscovered

- **Defect 070's answer is a MEANING for a word that already parses.** `borrows`
  and `consumes` both parse on a `cstr` today and are thrown away by the
  handle-only sweep. Giving one of them a meaning there is smaller than a new
  word and is the shape the engineer's veto points at: give-away **refuses** a
  lease.
- **The fact is chosen per CALL, and Heroes already passes arguments per call.**
  Nobody in four sittings proposed a **call-site** mark. Panel 167 refuted a
  parameter mark with a run-time `bool`; that refutation does not reach a mark
  the caller writes.

## The resolution — `provisional — author ratification pending`

Per CLAUDE.md § 4, the most robust and complete resolution, never the cheapest.

1. **The mark as briefed is REFUSED**, on the compiler-engineer's veto (one word,
   two opposite admissions) and the ffi seat's veto on any mark that schedules a
   release from a named C call. The ergonomist's veto stands against the
   group-level form on locality.
2. **`one_tag_one_type` is NARROWED at `tag void`, and this closes defect 072.**
   The rule's own reason is about `struct s *` where clang is a second judge; at
   `void *` no second judge can exist. It is the only thing in this sitting that
   closes an open defect at `check` with the language it already has, and it is a
   **relaxation of a refusal rather than a new one**, so CL-005's burden runs the
   other way.
3. **The default flip is NOT taken, and it is queued with its number.** It is the
   only candidate that closes 066 and 068, it needs no author action on any
   correct program, and it **refuses eight correct programs in `examples/`
   today**. Eight of eight is a decision the author owns, and the historian's
   `[[carries_dependency]]` finding is what it is weighed against: an annotation
   that pays off only when written on every link loses its guarantee at the first
   unannotated boundary, and then nobody writes it. **Fifteen years from standard
   to removal, and Clang removed it nineteen days before this sitting.**
4. **Defect 070 is a separate fact and stays open**, with its route named: a
   meaning for `borrows` or `consumes` on a `cstr`, where give-away **refuses** a
   lease.
5. **Three defects are filed** and defect 072's file pointer is corrected.
6. **The emission divergence is closed as resolved, not as a defect**: a stale
   binary, twice in one sitting, from two seats sharing one scratchpad. **`/panel`
   gains the rule that a seat's copy is its own.**
7. **If any mark is ever adopted it lives in its own grammar slot**, beside
   `counted_by`, never inside the `consumes|acquires|borrows` alternation, which
   is swept by `check/marks.hero` and refuses on exactly the types the defects
   live on. **+3 real**, and without it a mark closes zero while looking like it
   closes four.
8. **`spec § 13` owes the handle-only rule regardless**, +26 real: the document
   has never stated the rule the compiler has enforced since 2026-09-15.

**What a veto compels.** No mark naming an ending call, ever, whatever the author
decides among the rest.

**What conservative would have been, so the author can choose it**: adopt the ffi
seat's weak mark now. It costs +82 real in its own slot, it breaks nothing, and
**the critic measured that it closes zero of four**, because every reproducer is
unmarked. It would look like progress on the list and move no number on it.

## Predictions to score, at the M-declared-extents close

- **compiler-engineer**: if one mark is adopted for both facts, `DEFECTS.md`
  still carries an open give-away defect and `heroes check` on 070's reproducer
  exits **0**; with two marks it exits **1**.
- **ffi-pragmatist**: `examples/sqlite/main.hero` needs no shim, no mark and no
  edit; `examples/ledger` pays, `heroes check` going 0 → 1 at
  `db/sqlite.hero:342`, with four call sites hoisting a lease into two loops.
- **spec-warden**: a `keeps` inside the swept alternation reports
  `error[unread_mark]` at `check` exit 1 on all three reproducers, **zero defects
  closed**. And any adopted draft lands within ±6 real of its table figure.
- **llm-ergonomist**: handed an unmarked keeping binding, a reader chooses the
  lend in **≥8 of 10** under either mark against **≤6 of 10** today — **+2 of 10
  toward the silent bug** — and **≤4 of 10** under the prose-only wording.
- **historian**: no foreign retention mark has ever been shipped and withdrawn,
  and none will be found; the nearest real withdrawal stays
  `[[carries_dependency]]`.
- **completeness critic**: narrowing `one_tag_one_type` at `tag void` closes 072
  with **zero** other programs refused across `tests/golden/` and `examples/`.

## Author's verdict

**RATIFIED 2026-09-20, AND THE DEFAULT IS NOT FLIPPED**

**Ratified by the author on 2026-09-20**, in conversation, in these words: *"I am
following your recommendations."* Recorded as a ratification given on the
coordinator's summary and price in `docs/work/DECIDE.md`, not as a reading of
this file, and **not `by delegation`**.

**The adopted resolution stands in full**, and the one question this sitting said
no sitting could spend for the author is answered the way it recommended: **the
default is not flipped.** Not because eight refusals are many, but because no
measurement exists of what the ninth costs — the corpus is `examples/` and
`tests/golden/`, and the author's own programs are not in it.

**What that leaves standing, written plainly so a later reader does not have to
infer it.** Defects **066 and 068 have no route in flight**. Every candidate this
sitting weighed either closes zero of them or closes them at a price the author
has now declined for a stated reason. They stay open, and what would move them is
named rather than left to be rediscovered: **one binding the author cares about,
written both ways** — with the lend the language admits today, and with the
pessimistic default that would refuse it. That is a measurement, not a sitting,
and it is the next thing that can change this answer.

**Defect 070 keeps the route this sitting named and nothing else changes for it.**
The coordinator measured after the ratification that `consumes` on a `cstr` is
`error[unread_mark]` today, so giving that word a meaning there is a real change
and would meet this sitting's own two findings head on: the warden's *the three
existing marks fail safe when omitted and this one fails unsafe*, and the
ergonomist's *a mark does not add a check to the unmarked case, it subtracts the
warning from it*. **A fifth sitting would re-derive them**, which is why none was
convened.

**What this section said while the sitting was open**, kept because a
record is not rewritten:

**Pending.** Queued as `panel 170` in `docs/work/DECIDE.md`.

**What a yes settles**: that no one word carries both retention and give-away,
that defect 072 closes by relaxing the refusal that created it, that any future
mark lives in its own grammar slot, and that the default flip is yours to weigh
against eight correct programs.

**What it does not settle**: whether the default flips. That is the only route
measured to close 066 and 068, its price is eight refusals of eight correct
programs in `examples/` alone, and no sitting can spend that for you.
