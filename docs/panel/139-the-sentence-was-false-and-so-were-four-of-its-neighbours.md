# Panel 139 — the specification's promise about copies was false, and so were four of its neighbours

**Sat** 2026-09-13 · **milestone** M-deferral-ledger, beside step 4 · **status**
`provisional — author ratification pending`

**Lane: full five seats plus a completeness critic.** This sitting was convened
on a **defect**, not a proposal: `docs/work/DEFECTS.md` item 030, filed at panel
137, says § 3's *"No aliasing exists anywhere"* is false through a `ptr` field.
CLAUDE.md § 12 makes the false sentence the defect rather than the compiler's
behaviour, so the repair is words — which is what the sitting was asked to
choose. **It came back with more than a sentence.**

## The defect as filed

> Every value behaves as an independent copy: after `b = a`, mutating `b` never
> changes `a`. **No aliasing exists anywhere.**

Two copies of a `record Statement { handle: ptr }` advance the same C cursor:
`a` sees row 1, `b` sees row 2, `a.handle == b.handle` true, with `b` copied
before `a` advanced. `examples/ledger/db/sqlite.hero:287` **ships** a function
taking that wrapper by value and mutating the cursor through it — a signature
that by § 9 promises it changes nothing the caller passed.

## What the sitting found, in the order it matters

**1. The defect is six times wider than its witness** (FFI seat, twelve shapes
compiled, eleven run). Aliasing and nothing noticing, in: a `ptr` in a record
field; a `ptr` in a **variant case payload**; a record-in-an-array; a
record-in-a-map; a record inside a record; and — the one that breaks every
candidate wording — **a bare `ptr` as a plain parameter, with no record in sight**.
The compiler looks in exactly one place, panel 068's map-key gate, and it is
looking for a hash rather than for aliasing. **A sentence scoped to fields would
have been false on its first day.**

**2. Both candidate sentences merge `ptr` and `cstr` wrongly.** A Heroes record
**may not hold a `cstr` at all** — `error[cstr_in_a_record]`, compiled — so every
draft saying *"a `ptr` and a `cstr`"* names a field the reader cannot write and
leads them somewhere that does not compile.

**3. The mutable marker is not inert, and is worse than inert: it is a
no-op that costs.** Marking `stepped` `@statement` **does** change the emitted C
— a pointer parameter, one copy-in, three copy-outs — and the FFI seat then ran
it end to end against real SQLite: **29 lines changed, identical output**. The
copy-out writes the same address back. The marker buys an indirection and says
nothing true. Its cost was measured rather than guessed: one function's marker
broke **6 call sites**, and fixing those broke **6 bindings**, **13 lines across
2 files for one of 13 by-value wrappers**.

**4. The compiler repair is refused, and for a better reason than the brief
gave.** The coordinator asserted that refusing this would refuse the FFI. The FFI
seat tested it instead and found the real reason: **the header carries no signal
the rule could key on.** `sqlite3_column_count` is **not** const-qualified —
compiled, the falsifier fired — so the mutating step and the pure reader have the
identical C signature. A rule keyed on *reaches C through a `ptr`* would refuse
**13 of 17** functions in the shipped binding, four of which mutate nothing, and
CLAUDE.md § 12's requirement that a refusal name the program fact that would make
it wrong **cannot be satisfied**. The critic narrowed it further: `string.h`'s
readers *are* const-qualified, so the true claim is that **an opaque-handle API
does not const-qualify its readers**, not that no header does.

**5. The false sentence has descendants, and two of them the compiler prints at a
programmer** (engineer). `grep -rn "aliasing exists anywhere"` over `selfhost/`
and `runtime/` returns four hits and `"independent copy"` adds a fifth;
`selfhost/resolve/writes.hero:307` and `selfhost/size_errors.hero:37` are
**diagnostic text**. Repair § 3 alone and the compiler goes on telling users a
sentence the specification no longer contains, **and no suite links them**: the
`spec` suite reads the document, `check` reads goldens, neither reads the other.
That is what makes this a step with a gate rather than a documentation edit.

**6. Four of the sentence's neighbours are suspect, and the sitting ran two of
them.** The warden enumerated from a grep of every absolute in the document; the
critic added five more from its own read. **The one that matters is § 13's
*"Unmarked pointers are never freed"*** — true of the compiler, read as a fact
about the program, while `examples/ledger/db/sqlite.hero:225` and `:270` ship
`closed(db: Db)` and `finalized(statement: Statement)` **by value**, handing to C
a pointer that frees what **every copy** of that record holds. The warden
proposed it as a test; **the coordinator ran it** (below). Also suspect and
unrun: § 9's copy-out being silent about the pointee, § 4's *"There are no mutable
globals"* (true of names, false of reachable state — an open database handle is
process-global mutable state every copy reaches), § 5's *"All bindings are
initialised"* (a field holding a freed address is initialised), and § 3's own
*by-value* bullet, since the value is a tree and the reachable data is a graph.

**7. And design.md carries the same falsehood in a refusal.** `design.md:2588`:
*"Borrow checker — **unnecessary**, value semantics removes aliasing, so there is
nothing to check."* CLAUDE.md § 12 holds a refusal to a feature's standard, and
**the fact that would make that row wrong is now in the tree, measured**. The
critic found it, no seat did, and design.md has no token budget — so this half of
the repair costs nothing and was nobody's to skip.

## Verdicts

| seat | verdict | what it would ship | cost / delta | condition |
|---|---|---|---|---|
| `llm-ergonomist` | **veto on the current text**; adopt B, adopt-with-condition A | **B** — *"the only text that answers Task 2 in advance"* | under the current text the bookmark pattern **compiles and lies**: a reader writes `b = a` as a private copy and gets the second row. The falsehood is **not detectable from inside the document** — § 7's *compares as an address* is consistent with it — so no reader catches it by reading. Counted: the current sentence 28 characters, A 123, B 202, and its own proposal **117**, shorter than A and covering more | both A and B omit that the exception travels **through a record field**, left to adjacency; its 117-character sentence covers parameters and transitivity |
| `compiler-engineer` | **adopt-with-condition. Ship D2, not D1** | **D2**, because *"a repair that does not cover the shipped counterexample is not a repair"* | **zero lines** in lexer, checker, descriptors, ownership and emitter. Non-zero in text: the sentence plus **2 diagnostic strings and 7 comments** across six files. Touching the two diagnostics moves the `check` and `annotations` suites, so it is a step with a gate | **the sentence alone is not enough** — the descendants must go with it, or the compiler quotes a retracted sentence and a green net proves nothing |
| `ffi-pragmatist` | **adopt-with-condition**, no veto (nothing touches the ABI) | **D3**, its own redraft, **+75**: *"A `ptr` is a copied ADDRESS, **wherever it sits** … A `cstr` copies an address too, and only a group's `record` may hold one"* | the twelve compiled shapes above; the marked-versus-unmarked emission diff; the const falsifier under `-Wall -Wextra`. *"`wherever it sits` covers six shapes without enumerating them; the `cstr` clause stops the reader walking into a form that does not compile"* | D1 is **insufficient** (silent on the marker), D2 **misleads** on `cstr` |
| `spec-warden` | **object to the shape of the fix**, not the budget | **W2**, +33 vendored, paid by § 4's kinds sentence at **−54** | *"W1 at +12 buys 21 tokens by leaving § 9's promise false, and § 9's promise is the half `sqlite.hero:287` already violates in shipped code."* And the finding that outlives the sentence: **§ 7:177 already contradicts § 3:63** — the document was inconsistent with itself before C was involved | moves to approve when the wording carries the § 9 clause, the removal lands in the same commit with its ledger row, and the real row measures ≤ 7531 after the pair; **moves to veto if a wording is adopted that leaves the more expensive error live** |
| `historian` | (no file; advisory) | — | the seat wrote no report file and its absence cost the sitting a route the critic then supplied: **the type's NAME** — Rust's `*mut`, Go's `unsafe.Pointer`, Swift's `UnsafeMutablePointer` all put the warning in the spelling, so every use site carries it and § 3's absolute stays true of what the language owns | — |

## The critic's findings, which changed the resolution

**Four routes nobody listed**, of which two are adopted below: design.md's Part 6
row, where there is **no budget** (point 7 above); a draft sited at **§ 9**, where
the reader actually makes the mistake, which nobody priced; **an executable claim
rather than a sentence**, since the document already carries one a suite runs; and
**the type's name**, above.

**A fifth, half-listed and the cheapest of all**: mark the shipped counterexample
`@statement`. The FFI seat measured it as the *cost of a compiler rule* and never
as a repair on its own — **13 lines, zero spec tokens, zero compiler lines**, and
the shipped code stops demonstrating the falsehood.

**D2's price, settled.** Three seats said +50 and two said +48; the critic
re-measured the file they were re-typing and got **+48**, the +50s being
re-wrapped transcriptions. **Four seats brought four different texts** to a
sitting framed as a choice between two, and no two priced the same candidate set.

**Every number in the sitting is Darwin arm64**, and 029 changed class between
Darwin and Linux the same night.

## Found beside the sitting, and run rather than proposed

**The use-after-free is real, and it is defect 031.** The warden named
`closed(db: Db)` and `finalized(statement: Statement)` as a suspect and proposed a
test; the coordinator wrote and ran it. A `record Holder { cell: ptr }`, a copy,
`free` through the copy, then a write through the original's own field:

```
build exit 0, zero diagnostics
"the copy was closed; now the original writes through its own field"
"wrote through a freed pointer, and nothing said a word"
run exit 0
```

and under `--sanitize`:

```
ERROR: AddressSanitizer: heap-use-after-free on address 0x6020000000f0
WRITE of size 8 at 0x6020000000f0 thread T0
    #1 h_uaf_main uaf.hero:22
```

**This is design.md §1.12** — a Heroes program must not corrupt memory — which
CLAUDE.md § Precedence ranks above cost, ergonomics and token count. It is the
sharper twin of 030: not *two copies reach one thing* but **one copy can free what
the other holds**, at exit 0, from code that looks ordinary. Filed as **031**.

**And one of the critic's own measurements did not reproduce.** It reported that a
record holding a freed pointer compares `==` **true** to a fresh unrelated record,
because the allocator reused the address. The coordinator ran the same shape and
got **false**. So the class is real — a `ptr` compares as an address and an address
can be recycled — but **the reproducer is not deterministic**, and it is recorded
here as a hazard rather than filed as a defect, because `docs/work/DEFECTS.md`
holds measured failures with reproducers. It is handed to **M-handle-verdict**,
whose subject is what `ptr` carries.

## The resolution adopted, provisionally

**The sentence is chosen, and the repair is four acts rather than one.** Three
seats said in three ways that a sentence alone is not the repair, and the sitting
takes them.

1. **The wording is the FFI seat's `wherever it sits` merged with the warden's § 9
   clause**, because those two seats each covered what the other missed and no
   draft on the table carried both:

   > Every value behaves as an independent copy: after `b = a`, mutating `b` never
   > changes `a`. No aliasing exists among the values this language owns. A `ptr`
   > is a copied ADDRESS, wherever it sits: two copies reach one foreign thing, so
   > a function taking one without `@` may still change, **or free**, what C holds.
   > A `cstr` copies an address too, and only a group's `record` may hold one.

   *"Wherever it sits"* covers the six compiled shapes without enumerating them;
   the `cstr` clause stops the reader walking into a form that does not compile;
   *"or free"* is added by the sitting on defect 031, measured after every seat had
   reported. Measured by the coordinator in a copy: **5728 vendored, +73**, and
   the **real row goes STALE the moment the file changes**, `measure` refusing a
   verdict against it.
2. **The descendants go with it**: two diagnostic strings and seven comments
   across `selfhost/` and `runtime/` that quote the retracted sentence, which
   moves the `check` and `annotations` suites.
3. **design.md's borrow-checker row is corrected**, at no token cost, because
   CLAUDE.md § 12 holds a refusal to a feature's standard and the fact that would
   make it wrong is now in the tree, measured twice.
4. **The shipped counterexample is marked**, 13 lines, so `examples/ledger/` stops
   demonstrating the falsehood — the cheapest act on the table and the one nobody
   proposed as a repair.

**What this sitting does NOT decide, and says so**: the four suspect neighbours
beyond § 13's freeing sentence stay open questions with their commands written
down; the type's name — `unsafe_ptr` or a family — is **M-handle-verdict's**, and
this sitting must not decide it by accident; and defects 029 and 030 are **not one
defect**, the critic having separated them cleanly — **029 is the missing pointee
type, 030 the missing identity** — so giving `ptr` distinct pointee types would
leave 030 untouched and any sentence here leaves 029's swapped handle building.

**What blocks the landing, and it is a hard stop rather than a judgement.**
Amending `spec/heroes-spec.md` trips the staleness detector — measured in a copy:
*"STALE: the recorded count is for 0b96e29b3ca14666 and this file is
297ecc45149a4137"* — and `measure` gives **no verdict against a stale count**. The
only repair is `heroes measure spec/heroes-spec.md --refresh`, which
`selfhost/cli/refresh.hero:4` calls **the only place in the compiler that reaches
the network**. CLAUDE.md § Hard stops asks for every outward-facing act, every
time, and sending the document to an external service is one — even though the
document is already public. **So the sentence, the pins, the ledger row and the
descendants are queued for the author's yes, and everything that does not touch
the specification lands now.**

## Author's verdict

**Ratified 2026-09-13, by the yes that authorised the network call.** The author
was asked, the evening M-deferral-ledger closed, whether to run
`heroes measure spec/heroes-spec.md --refresh` — the one act this sitting named
as blocking its landing — in a question that listed the three specification
corrections it would unblock, this sitting's § 3 sentence first. The answer was
yes, it ran that hour, and the four acts landed in one commit as M-handle-verdict
step 1 (`docs/records/log/2026-09-13-1000-…`; the DECIDE item is
`docs/records/done/2026-09-13-1001-…`).

**What the yes settles**: the merged wording, measured at **+75 vendored** alone;
the § 4 removal, measured at **−43** against the −54 priced here, because the
rule sentence stays and only the half the grammar repeats goes; the eight
descendants, the goldens corrected underneath with the date; and
`stepped(@statement)` at thirteen lines across two files, the compiler catching
two call sites the first pass missed with a `fix (certain)`.

**What it does not settle**: the type's name, reserved above for M-handle-verdict
and untouched by this yes; and defects 029 and 031, which the sentence now
describes and no rule refuses.

**Scored at the landing, as § Predictions asked**: the spec-warden's *≤ +45 real,
lands ≤ 7525* is **falsified** — measured **+79** and **7610** — for two causes the
record names: this sitting adopted the merged wording at +75 over the warden's
own W2 at +33, and the landing carried panel 144's § 10 correction beside it.
The real-to-vendored ratio of the pair is 1.18 against the document's 1.33.

**And one thing the landing found that the sitting did not**: the harness held a
single `REAL_TAKEN` for two independently refreshed documents, and went red the
moment the spec was re-taken and the contract was not. `pinned.hero` had two
dates all along; the suite now has two.

## Predictions to score

| prediction | instrument | scored at |
|---|---|---|
| engineer: if the sentence ships with no `selfhost/`/`runtime/` change, `grep -rn "no aliasing exists anywhere"` still returns ≥ 4 hits **and** `spec` and `check` are both green — a green net over a compiler quoting a retracted sentence | `grep`, the two suites | the landing commit |
| warden: the real delta measured with `--refresh` is ≤ +45 and the spec lands ≤ 7525 real after the −54 removal | `heroes measure --refresh` | the landing commit |
| warden: a program copying a `Db`, closing the copy and using the original builds with zero diagnostics | `heroes build` | **scored at the sitting: CONFIRMED**, and filed as defect 031 |
| FFI: a reader following D1 alone still writes the shipped binding but keeps believing § 9's promise | metric 2's runner | M-thesis-harness |
| reader: under the current text, a fresh model writing the bookmark pattern produces a compiling program that prints the wrong row | metric 2's runner | M-thesis-harness |

## What the seats could not source or could not run

The historian wrote no file, and the sitting lost the precedent survey on how
other languages word this; the critic supplied the one route that survey would
have found. Every measurement is Darwin arm64. The warden's −54 removal is priced
and was never run against the `shape`, `named` and `offered` checks that read the
sentence it removes. The engineer read every caller rather than running one, for
its claim that an emitted `hero_unreachable()` is genuinely unreachable. The FFI
seat's *no header-verified rule can separate reader from writer* is measured on
sqlite alone and the critic narrowed it. The reader has no shell and priced in
characters where the instrument counts tokens. And the critic's own `==` finding
did not reproduce for the coordinator.
