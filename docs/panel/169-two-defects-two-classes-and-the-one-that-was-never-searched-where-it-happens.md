# Panel 169 — two defects, two classes, and the one that was never searched where it happens

Convened 2026-09-20, at M-declared-extents, on **what closes defects 066 and
068**. Full panel: five seats and a completeness critic, six briefs written to
disk before any seat started, and the working tree frozen from the briefs going
out until this file was written.

**Panel 168 measured that route A closes zero of two. This sitting found that
066 and 068 are not one problem, and that one of them has excellent precedent,
costs this repository nothing, and was never searched at the layer where it
happens.**

## The question, verbatim

> What closes defect 066 and what closes defect 068? Not *which route is
> cheapest*, and not *is route A sound*: panel 168 settled that route A is sound
> and closes zero of two.

Four routes were on the ballot — **R1** the handle route, **R2** the caller-side
rule for 068, **R3** a group-level mark, **R4** withdrawing the field lend. A
fifth was registered by a seat during the sitting.

## The resolution, in one line

**The lend has no stated extent, and every route on the ballot presupposes one.**
Two seats with isolated inputs found that independently, and it is what this
sitting adopts first: **R0, the sentence that says how long a lend is readable.**
**R2 is not adopted as worded** — the completeness critic measured a third hole
in it, a re-declaration, after the engineer had already widened it once — and
whatever rule lands is **stated over the IR (R6)** rather than over the AST,
where the set of ways to write a binding went from one to two to three in this
one sitting. **R4 is refused on the ffi seat's veto**; **R1 is a spelling and not
an answer**, measured to close zero of 066 because `acquires` counts and does not
order; **R3 is not adopted**. **R5, give the ownership away, is the route for
066**, blocked by **defect 069 alone**, whose repair the engineer measured at six
lines.

**A procedural note this file owes its reader.** The completeness critic ran
**after** a first draft of this synthesis rather than before it, which is the
order `/panel` § 3b forbids. The coordinator did that and says so here. What it
cost is visible: the critic's eight asks changed the one-line resolution, the
verdict table and five of the eight resolution items, and two of its findings —
the third hole and the omitted R0 — would have changed the draft before anybody
read it.

## The verdict table

| | compiler-engineer | ffi-pragmatist | spec-warden | llm-ergonomist | historian |
|---|---|---|---|---|---|
| **R0** state the lend's extent | — | — | **priced, +27 real merged** | **prefers it to both candidates** | — |
| **R1** handle | the live set is an **exit** check and not a **use** check: `leak.hero` printed its answer first and aborted after | **object** as the answer to 066; welcome as a spelling | **object**, +69 real | object (V1) — two of three clauses restate § 13 | approve the run-time half; its **static** half unprecedented |
| **R2** caller-side | **object to the WORDING**, approve widened | **approve**, on the BALLOT's narrower wording | **approve**, +16 real merged | object (V2), **unwithdrawn** — its trigger has two readings that compile to two programs | **approve, best precedent in the sitting** |
| **R3** group mark | — | — | **object**, +60 real | — | precedented as a **marker**, never as a proof |
| **R4** withdraw the lend | — | **VETO** | approve, **−89 real** | — | **no precedent at all** |
| **R5** give ownership away | 069's repair is 6 lines | **registered, and compiled** | — | — | — |
| **R6** state it over the IR | — | — | — | — | — (found by the completeness critic, after the seats) |

**No budget veto.** Baseline live today: **real 8154**, digest
`249990ca1b6b2f66`, effective 8214 against a ceiling of 10240. The widest route
on the ballot leaves 1941 free, and the warden declined to manufacture a veto it
did not have.

## THE FINDING THAT SPLITS THE SITTING — 066 and 068 are two classes

Three seats reached it from three inputs and none of them could see the others.

**The historian named the class.** 068 is not escape analysis. **It is
exclusivity**: no write to a location while a reference into it is live. That
rule ships **statically and caller-side** in Rust (`E0506`), Swift (SE-0176,
Swift 4.0), Ada (limited types, since 83), Austral and Nim. **None was ever
withdrawn** — Rust relaxed its duration with NLL, Swift strengthened its warning
to an error in 4.2, GCC added an escape hatch in 14, Nim kept it experimental.
**Panel 167's finding, that nobody enforces retention statically, was about the
CALLEE.** Applying it to 068 is a category error, and the seat that wrote it says
so itself.

**Java's FFM draws the line in the same place, and it is the best-resourced FFI
ever built.** Ten JEPs across roughly five years, the lifetime model redesigned
twice inside the preview window. It enforces the **caller-side** half —
`IllegalStateException` on access after `Arena.close()` — and refuses the
**foreign** half outright, in its own javadoc: the runtime *"has no insight into
the lifetime intended for said region of memory by the foreign function that
allocated it."*

> **Defect 068 is FFM's `IllegalStateException` case. Defect 066 is FFM's "no
> insight" case.** Filing 068 with 066 as unsolvable was filing an enforced class
> with an unenforceable one.

**And the spec already forbids 068.** The warden reproduced it — `check` 0,
`run` 0, prints 8 then 72, `--sanitize` exit 0 and **zero sanitizer lines** —
and read `spec § 3` against it: *"No aliasing exists among the values this
language owns"*, whose own exemption reads *"two copies reach one foreign
thing"*. C holds an alias into a record field, which is **not** a foreign thing.
**§ 3 is false as measured and, read strictly, already forbids the program.**
068 is therefore a compiler defect against text already written, which is the
cheapest outcome available and nobody had looked for it.

## R2 WAS BUILT, AND THE BALLOT'S WORDING HAD A HOLE ONE SYNTAX WIDE

The engineer did not price R2, it **built** R2, and then attacked the shape
beside it. The ballot said *"not **re-assigned** in this function"*. `fill(@t)`
is a call, not a `@` statement, so the stated rule walks straight past it:

    ./heroes check hole.hero   -> exit 0
    ./heroes run   hole.hero   -> "72" "1", exit 0

**Byte for byte defect 068, one syntax over.** Widening to *"not **written***,
by a `@` statement **or** as a `@` argument, asking the argument's root so
`fill(@t.name)` counts" is **22 lines** and catches **both of those**. It does
not catch the third, which the completeness critic found afterwards: see § The
third hole.

**The built rule, measured rather than estimated:**

| | |
|---|---|
| `.hero` files refused, across `tests/golden`, `examples`, `selfhost`, `spec`, `tests/harness`, `site`, `docs` | **0 of 752** |
| planted positive controls, so the instrument is not watching itself | **3 of 3 found** |
| files holding a `ptr` lend root at all | **7 of 752** |
| the `cstr` widening, which the ballot did not have | also **0 of 752**, own control found |
| `./heroes check selfhost/main.hero` | exit 0 |
| `./heroes test selfhost/checker.hero` | **397 passed**, ten of them this rule's own |

**Cost, in `suite_layout.hero`'s own unit**: a new `check/lend_alias.hero` at
**145**, `lend_errors.hero` 277 to **298**, `checker.hero` 103 to **108**,
`check/lending.hero` **293 unchanged**. **+171 lines, zero ceilings breached,
zero `DECIDED` entries moved.** No lexer, grammar, AST, type rule, descriptor,
ownership, IR or emitter change: design.md Part 5's core is untouched.

**The engineer's zero is the load-bearing one and it stands alone.** The critic
reproduced it on a second build — `files scanned: 754, files refused: 0`, 754
being 752 plus its own two probes — and then corrected this synthesis's first
draft, which had merged it with the ffi seat's. **They do not measure the same
thing**: the ffi seat counted *pure re-assignments*, which is the **ballot's**
wording that this sitting refuses, over the **8** files holding a `.ptr()`
outside `selfhost/`, with a grep. It corroborates that the corpus holds no
`s @ …` after a lend and says nothing about the `@`-argument clause or about the
744 files it did not open. A second drift the critic grepped: this file's first
draft said the lends are *all in `tests/golden/`*, and `tests/harness/suite_surface.hero:231`
is not — 34 sites in 9 files by a plain grep, one of them inside a test
expectation string.

**And the over-refusal is real, just absent from a seven-file corpus.** The
critic wrote two sound programs of nine lines each, both `check` exit 0 and
correct today, both **refused by the built rule**, and **neither has any escape**:
RFC 2094's Problem case #1 with an explicit *C has let go* call, and **the
in-and-out buffer refill loop, which is the idiom the R4 veto is built on**. So
the historian's prediction is **falsified on this corpus and unfalsified as a
claim about the rule**, its condition 4's proposal to drop the caution must not
be acted on, and its own named instrument — `check` over the corpus plus the
`corpus`, `emission` and `run` suites — **was run by nobody**.

## THE ROUTE NOBODY LISTED — R5, give the ownership away

Registered by the ffi-pragmatist during the sitting, with the C compiled against
the real header and the real library.

A plain `malloc` buffer with plain `free` as `sqlite3_bind_text`'s fifth
argument: `clang -Wall -Wextra -Werror -fsanitize=address,undefined` **exit 0**,
run **0 0 0**, correct answer, no shim.

**And it survives the measurement that killed the trailing header.** Under
`sqlite3_config(SQLITE_CONFIG_MALLOC)` with a private allocator, `rc=0`, it still
reads back correctly and reports `sqlite private blocks still live: 0` —
**because the author's destructor is paired with the author's allocator**. Panel
168's killer does not reach it. Three sittings asked how Heroes can hand C bytes
that survive; **the answer is that it should not**.

`sqlite3.h`, measured with `clang -E -P` and split on `;`: **33** `const void *`,
of which 12 are results, 17 parameters and 4 callback-only, and **8 of the 17
carry a destructor argument and take R5**.

**R5 is blocked by defect 069 alone.** No spelling hands C a destructor: `d:
(function(Block) -> ())` is `check` 0, `build` 0, **run 134 ten of ten** with the
temporary never assigned, and `d: ptr` is `type_mismatch` at check. **069 was
filed three hours earlier as a diagnostics defect; it is the milestone's blocking
item.**

## Why R1 is a spelling and not an answer, measured twice

The ffi seat **wrote** R1: `sqlite3_bind_text` with `SQLITE_STATIC` over a
`record Block tag void` from `malloc`, `check` 0, `build` 0, run **0 ten of
ten**, correct, no shim. Cost against `.lease()`: **56 lines against 39**, two
extra `extern` groups, three declarations.

**Then it moved the `free` above `sqlite3_step`**: `check` 0, `build` 0, ten runs
`0 0 0 0 134 134 0 134 0 0` — **seven silent empty answers** — and
`heap-use-after-free` under the sanitizer. The sentence that explains it:
**`acquires` counts, it does not order.**

The engineer reached the same place from the other side: the live set is an
**exit** check and not a **use** check. R1 turns 066 from *a silent wrong answer
at exit 0* into *a wrong answer, then exit 134 with an address*. That is a real
§1.12 gain and **it is not a closure**, and any later sitting claiming otherwise
owes a run showing the abort landing before the read.

Three more caps the ffi seat measured on R1, each run: `one_tag_one_type` makes
two `void *` families **unbindable** (`malloc`/`free` beside `dlopen`/`dlclose`
is `error[duplicate_tag]`); the only legal shape then **reintroduces defect 029**
— one `Block` over two allocator pairs, each freed by the other's deallocator,
`check` 0 / `build` 0 / run `0 0 0`, live set balanced and silent; and R1 has
**no way home**, because `Block.validated()` is `type_mismatch` and no
`load_u8`/`store_u8` exists.

## The veto, and it is new ground

**R4, withdrawing the field lend, is refused on the ffi seat's veto.** Panel 167
refused it on bindability and panel 168 noted that ground was measured on a tree
route A would change. **The ground this sitting refuses it on is different and
larger**: `xs.ptr()` on a `[u8]` local is already `bad_operand`, and a handle's
bytes cannot come back into Heroes. **The field lend is C's only write path into
Heroes memory**, so R4 costs every in-and-out buffer a shim, which is design.md
§1.11 failing by its own definition.

That the warden priced R4 at **−89 real** — where panel 167 recorded −80, the
vendored table understating it by 29% — does not redeem it. A veto is a refusal
rather than a price.

## THE THIRD HOLE, and it is not a `@` at all

The completeness critic did not read the adopted rule, it **ran** it: the
engineer's built module survived in the shared scratchpad, and the critic copied
its four files into its own clean copy and exercised them through their own
`swept()` entry point. So every count below is the built rule's own diagnostic
count.

**A lent root RE-DECLARED on the second turn of a loop.** There is no `@`
anywhere near it:

| | |
|---|---|
| `./heroes check redecl_pure.hero` | **exit 0** |
| `./heroes run`, three times | **`72` then `1`**, exit 0, three of three |
| `--sanitize` | identical, exit 0, **zero sanitizer lines** |
| **the built R2-widened, on this exact file** | **0 diagnostics** |
| the control, the same file with an explicit `s @ …` added | **1** |

The cause is one line of the new module: `judge_statement` returns on
`.declare`. And **the compiler itself says the two are one instruction**:
`--dump-ir` shows one slot `s: KSlot` for the whole function and
`store s <- $t20` **inside the loop body** — byte for byte what defect 068's own
reproducer emits, and the emitted C agrees with one function-scope
`struct KSlot h2_s;` assigned each turn.

> **The AST distinguishes `declare` from `mutate`. The IR does not, and the
> machine does not.**

**So R2-widened closes defect 068's REPRODUCER and not defect 068's SENTENCE.**
The critic's program satisfies the entry word for word and is unrefused. This is
CL-078 arriving for the second time in one sitting, and the third repair of a
witness in one milestone.

The critic also enumerated the shapes that ARE closed, so no later sitting
redoes it: an index write into the lent array, a nested field, a field of a
field, a write through a **second** `@` parameter, a lend in one branch with the
write in the other, an array of records, and an `@` argument on an index of the
lent root — **all refused**. A UFCS mutating method is quiet under the rule and
closed elsewhere by `error[ufcs_on_mutable]`. A sibling declared after the lent
block ends is quiet and **does not corrupt**, measured.

## R6 — THE ROUTE NOBODY LISTED, and it is about which LAYER the rule is stated at

The five seats enumerated routes along two axes: **what is marked** — a
parameter, a group head, a handle, ownership — and **which side**, caller or
callee. **Nobody enumerated along the third: which layer of the compiler the
rule is stated at.** The sitting's own subject is a defect never searched where
it happens, and the search went one layer rather than all of them.

Stated over the **AST**, the ways to write a binding are an **open set the rule
must enumerate by hand**, and in this one sitting that set went from one (`@`
statement) to two (`+ @` argument) to three (`+` re-declaration). Every future
sugar adds a fourth.

**Stated over the IR the set is closed, and it is two**, measured from
`--dump-ir` on the three shapes:

| Heroes source | IR |
|---|---|
| `s @ KSlot(…)` | `store s <- $tN` |
| `s: KSlot @ KSlot(…)` **on a later loop turn** | `store s <- $tN`, the **same** instruction |
| `fill(@t)` | `call heroes fill(@t)` |

This is `.claude/rules/module-shape.md`'s rule arriving one level up: **a fact
about the value cannot expire, a premise about the world expires silently.** The
AST spelling set is a premise about the world, and it expired three times in one
sitting. The IR's instruction set is a fact about the lowering.

**The limit, stated rather than oversold**: the `@` argument is not a store at
the call site, so the IR does not erase that clause. It makes the clause set
finite and checkable against the lowering instead of against the grammar.
**The cost**: `selfhost/ir.hero` is at 310 of a `DECIDED` 310, so a pass there
needs the split `.claude/rules/module-shape.md` prescribes — and a ceiling is
rank 6 while a corruption class is rank 3. The real objection is `ast.hero`'s own
rule that errors speak in the syntax written, and the IR carries the spans, so
that is a cost and not a refusal.

## R0 — THE SENTENCE TWO SEATS FOUND INDEPENDENTLY, AND THE FIRST DRAFT OF THIS FILE RECORDED NEITHER

This is the sitting's completeness failure, and the critic named it as such.

- The **spec-warden** priced a row it called **R0** — *the lifetime stated for
  BOTH lends, merged* — at **+27 real, 8181**, and found where the rule actually
  lives today: *"`examples/gallery/13-lease.hero:3` tells the reader `s.cstr()`
  lends a string to C **for one call**. That rule is in the teaching material and
  absent from the document that IS the prompt."*
- The **llm-ergonomist**, which read only the spec, reached the same place: *"the
  document contains no sentence about how long a lend is readable, and both
  candidates presuppose one"* — **"this is the whole sitting"** — and proposed a
  third sentence, then said of its first clause alone: *"I would take that over
  either candidate as written."*

**And the missing extent is not a decoration, it decides whether R2 does
anything at all.** The ergonomist wrote V2 both ways and could not tell which
compiles: *under "readable until the callee returns" the write is legal and V2
closes nothing; under "readable for the rest of the binding's life" it closes
it.* The second reading is also the one that refuses the critic's two sound
programs. **So the same rule closes defect 068 or closes nothing, depending on a
sentence the document does not contain**, and no seat could have settled that
from the ballot.

**The ergonomist's objection is therefore unwithdrawn**, on its own stated
condition: it withdraws if the rule *states the extent in the same sentence*. The
adopted wording states a **scope**, *in this function*, and not an **extent**.

## The disagreements, stated plainly

**On what a mark can buy, the warden and the ffi seat agree and the ergonomist
adds the reason.** The warden: R1 and R3 *"buy a mark the header cannot supply"*,
and in all three of 066's reproducers **the mark is absent**, so they close the
case where the author already knew. The ergonomist, reading only the spec, found
that V1's one new prescription *"instructs whoever wrote the C header, not the
Heroes programmer"*. Same finding, two inputs, neither seat aware of the other.

**On the historian's precedent there is one correction it makes against
itself**: every surveyed language **exempts raw pointers by name**. SE-0176
verbatim: *"Unsafe pointers will not use any active enforcement."* Rust's borrow
checker does not track `*mut`. **So R2 is a deliberate departure, not a port**,
and it is justified rather than accidental: in Rust and Swift the raw pointer
*is* the escape hatch, and in Heroes `.ptr()` is the only road to C. The sitting
records it as a departure.

**On whether the spec's lease sentence is false, the coordinator's brief was
wrong and the warden corrected it.** The brief said *"C may read past
`end_lease`, so the sentence is false"*. The warden ran the shipped example —
**13 bytes, three times, correct** — and the reordered one — **0** — and read the
sentence as *a permission with an end date*, true of the order the document's own
example models, **merely unenforced**. So no route needs to spend a token saying
when the bytes die. **The missing sentence is about the LEND**, which the
ergonomist found independently: *the document never states how long a lend is
readable.*

**And the ergonomist found that the language's own instrument creates the bug.**
*"A lease nobody ends aborts when `main` returns"* tells the reader they must end
it, and **the nearest safe-looking place is right after the call**. That is why
the shipped gallery example puts the read before `end_lease`: whoever wrote it
knew, and the document does not say.

## The resolution — `provisional — author ratification pending`

Per CLAUDE.md § 4, the most robust and complete resolution, never the cheapest
and never a compromise.

1. **R0 IS ADOPTED AND COMES FIRST: `spec § 13` states how long a lend is
   readable.** It is the sentence two seats found independently with isolated
   inputs, it is **+27 real merged** on the warden's `--refresh`, and it is not
   optional: until it exists, R2 closes defect 068 under one reading and nothing
   under the other, and **the ergonomist wrote both and could not tell which
   compiles**. The rule today lives in `examples/gallery/13-lease.hero:3`, the
   teaching material, and is absent from the document that IS the prompt.
2. **R2 IS NOT ADOPTED AS WORDED.** The critic ran the built rule and it leaves
   defect 068 open **one statement kind over**, a re-declaration on a second loop
   turn: `check` 0, run `72` then `1`, zero sanitizer lines, **0 diagnostics**.
   That is the third wording in one sitting and the third repair of a witness in
   one milestone. **Nothing lands until the rule is stated at a layer where the
   write set is closed.**
3. **R6 IS ADOPTED AS THE LAYER.** Whatever rule closes 068 is stated over the
   **IR**, where `declare` and `mutate` are already one instruction and the clause
   set is two and enumerable from the instruction set, rather than over the AST,
   where it is an open set that expired three times today. `selfhost/ir.hero` is
   at 310 of 310, so the split `.claude/rules/module-shape.md` prescribes is
   **priced and taken**: a ceiling is rank 6 and a corruption class is rank 3, and
   CL-012 names compiler size among what robustness beats.
4. **THE OVER-REFUSAL IS OWED AN ANSWER BEFORE ANY RULE LANDS.** Two sound
   nine-line programs are refused with no escape, and one of them is the
   in-and-out buffer refill loop — **the very idiom the R4 veto is built on**. The
   historian's three precedents each ended in flow analysis (Rust, three years) or
   a named escape (GCC, one release). Heroes' answer is R0: once the extent is
   stated, a write after the lend's extent has ended is legal by construction, and
   the escape needs no new word. **That is the claim, and it is unrun.**
5. **R5, give the ownership away, is the ROUTE FOR defect 066** — not its
   closure, which the critic corrected in this file's first draft. The author
   allocates, C frees with the function the author names, and the author's
   destructor is paired with the author's allocator so panel 168's
   replaced-allocator measurement does not reach it. It is design.md §4.19's third
   reserved case and this sitting compiled it in C. The ffi seat's own prediction
   is that 066's reproducer becomes **unwritable**, which is a different claim
   from closed.
6. **Defect 069 is the milestone's blocking item**, promoted from the diagnostics
   defect it was filed as three hours ago, and **it is the one thing here that can
   land now**. Its repair is emission and not refusal: the engineer hand-patched
   the emitted C to `t4 = cb_free_it;` and clang exits 0 and the program exits 0,
   while a mismatched shape is clang exit 1 with `incompatible function pointer
   types` — **so the C compiler is already the judge**. About six lines in
   `emit/inst.hero`'s `.func_ref` arm, into a file with zero headroom, so the
   18-line arm moves out and frees roughly 12 net.
7. **R4 is REFUSED on a veto**, R1 is **not adopted as an answer to 066** and
   stays welcome as a spelling, and **R3 is not adopted**: precedented as a marker
   and never as a proof (Oberon's `SYSTEM`, Fortran's `TARGET`), at +60 real for a
   mark the header cannot supply.
8. **The `cstr` widening is NOT adopted with the rule.** The critic made the
   one-line swap and `./heroes test selfhost/checker.hero` read **the identical
   397 tests, all passed** — *a widening no assertion can tell apart from its
   absence is a widening with no golden*. And the shape it would newly refuse has
   been run **four times across two seats without once reproducing 068**. It
   returns when it has a golden.
9. **Two defects the sitting found are filed**, each re-run by the coordinator
   before filing: **the consume guard is emitted AFTER the call**, so it can never
   prevent, and a double consume of a local is `check` 0 / `build` 0 / run **133
   three of three with no message at all**; and **two allocator families collapsed
   onto one handle type** by the `one_tag_one_type` refusal, each block freed by
   the other's deallocator at `check` 0 / `build` 0 / run 0 in silence.
   **A third the ffi seat proposed is NOT filed**: the `const void *` parameter
   position is not a hole in §4.19's promise, because `spec § 13` already exempts
   *"what a `ptr` points at"* in those words.
10. **A fourth ratcheted file is recorded**, which nobody had named:
    `selfhost/emit/gate.hero` at **365 of 365**, beside `emit/inst.hero` 350/350,
    `emit/ctype.hero` 395/395 and `ir.hero` 310/310.
11. **Two interactions are owed a run before any rule lands**, both named by the
    critic: `error[field_lend_written]` (panel 166, defect 065) is the closest
    existing ruling to R2, it is **emit-stage** rather than check-stage, and **the
    two rules push opposite ways on the same line** — panel 166 tells an author
    whose C parameter is non-`const` to declare the lent root `@`, and R2 then
    forbids writing that `@` root in that function. The composite an author must
    hold is *the root must be declared mutable and must never be mutated here*,
    which is exactly the refill loop of item 4.

**What a veto compels.** R4 stays refused whatever the author decides among the
rest.

**What conservative would have been, so the author can choose it**: adopt
R2-widened now, as this file's own first draft did. It is built, it is measured
at 0 of 752, and it is **measured to leave defect 068 open one statement kind
over** — the critic ran the program. Taking it would close the witness and ship
the class for the third time in one milestone, and would land a compile error
with no sentence in the document that is the prompt, which is precisely what the
only seat that read the prompt said not to do.

## Predictions to score, at the M-declared-extents close

- **compiler-engineer**: R2-widened refuses **0 of 752** `.hero` files and
  `layout` stays green with no new `DECIDED` row; `emit/inst.hero`, `ctype.hero`,
  `ir.hero` and `gate.hero` all stay at their ceilings.
- **ffi-pragmatist**: once 069 emits a C function's address, **§4.19 ladder step
  3 needs no shim for a retaining bind** — the ledger's `bind_text` as a
  `malloc`'d block with `destructor: free` is check 0 / build 0 / run 0 — and
  **066's reordering becomes unwritable, because there is no `free` line left to
  move**. R5 needs exactly one further rule: `consumes` must refuse a second
  consume of a local. R2 refuses 0 goldens and 0 examples.
- **spec-warden**: R2M lands at **8170 ± 12** real; any adopted draft off its
  table figure by more than 40 owes a re-price. 068 stays unreproducible through
  `s.cstr()`. Any draft reading above **10180** real is vetoed.
- **llm-ergonomist**: asked *how long may C read what `f.ptr()` handed it?*,
  **8 or more of 20** models answer something other than *until the call
  returns*. Under 4 of 20 and the seat is wrong about the gap.
- **historian**: if a caller-side rule lands without a named escape, one is
  requested within four milestones **by a program rather than by a panel**. Its
  first prediction — that at least one green program would be refused — is
  **falsified on this corpus and unfalsified as a claim about the rule**: the
  critic wrote two such programs in nine lines each. Its condition 4's proposal
  to drop the caution is **not acted on**, and its own named instrument, `check`
  over the corpus plus the `corpus`, `emission` and `run` suites, **remains unrun
  by anybody.**
- **completeness critic**, which gives no verdict but registers this: a rule
  stated over the AST needs a **fourth** widening within two milestones, and one
  stated over the IR does not. Falsified if the AST wording survives two
  milestones untouched.

## Author's verdict

**RATIFIED 2026-09-20, INCLUDING THE EXTENT**

**Ratified by the author on 2026-09-20**, in conversation, in these words: *"I am
following your recommendations."* Recorded as a ratification given on the
coordinator's summary and price in `docs/work/DECIDE.md`, not as a reading of
this file, and **not `by delegation`**.

**The second question this item asked, which was invisible when it was written
and is named in the correction above, is settled with it: the extent is *a lend
lives for its call and no longer*.** It landed in `spec § 13` at step 15, before
the ratification, under CLAUDE.md § 3's rule that a whole milestone asked for in
one `/step` decides its delegated questions with the recommended resolution as
the default and says once which way it went. The ratification confirms that
choice rather than authorising it after the fact, and the distinction is written
here because the record should not read as permission that was asked for.

**And the consequence the correction names is now the standing position**:
defects 066 and 068 are **one class**, the caller-side rule closes neither, and
the sitting's split of them does not survive the extent it adopted.

**What this section said while the sitting was open**, kept because a
record is not rewritten:

**Pending.** Queued as `panel 169` in `docs/work/DECIDE.md`, and the adopted
resolution is the default while it stands there.

**What a yes settles**: that the lend's extent is stated in the document before
any rule enforces it, that the rule is stated over the IR rather than the AST,
that 066 and 068 are two classes, that the foreign one is answered by giving the
ownership away rather than by any pointer Heroes can hand out, and that defect
069 is what stands in front of it and is six lines.

**What it does not settle, and all three are named rather than left to be
rediscovered**: which extent the sentence states, since the ergonomist measured
that two readings compile to two different programs; whether R0 alone answers the
over-refusal the critic demonstrated in nine lines, which is claimed in item 4
and **unrun**; and whether R5's spelling is `owned` read backwards, a new word, or
something no sitting has seen.

**And M-DECLARED-EXTENTS CANNOT CLOSE ON THIS SITTING.** Six defects are open,
each now has a route, and one of the six — 069 — can land immediately. The rest
wait on the sentence this sitting adopted and on the layer it moved the rule to.

## THE SPLIT DEPENDS ON THE EXTENT, AND THIS SITTING DID NOT NOTICE — written underneath, 2026-09-20, after the synthesis was committed

Written under rather than into the text above, which stands.

**The sitting's headline is that 066 and 068 are two classes.** It rests on the
historian's finding that 068 is *exclusivity*, which ships caller-side in five
languages, and on Java FFM drawing the same line. Read again with this sitting's
own adopted question in hand — *how long is a lend readable?* — **the split is
not independent of the answer**:

- under **a lend lives for the call and no longer**, defect 068's write happens
  after the lend's extent has ended, so **the program is right and C is the one
  retaining**. 068 is then the same class as 066, the caller-side rule closes
  nothing, and Rust's `E0506` and Swift's SE-0176 do **not** apply, because in
  both of those the reference's extent is known to the compiler and here it is
  the thing being defined;
- under **a lend lives as long as the binding**, 068 is exactly exclusivity, the
  precedent applies in full, **and the rule refuses the two sound programs the
  completeness critic wrote in nine lines each.**

So the resolution's items 1 and 3 are more entangled than it says: **the layer
question (R6) is independent of the extent, and the split is not.**

**And one precedent is overstated, which is the coordinator's error rather than
the historian's.** The historian wrote that *defect 068 is FFM's
`IllegalStateException` case*. FFM raises that exception when Java accesses a
segment **after its own arena has closed** — the program touching something whose
lifetime it controls and has ended. Defect 068 closes nothing and ends nothing:
the program rewrites its own record while native code holds a pointer into it,
and **FFM does not catch that shape**, because a `MemorySegment` is off-heap and
has no Heroes-style reassignment. The sentence should have gone out as a
question. What survives of it is the weaker and still useful half: FFM enforces a
caller-side temporal rule at run time and refuses the foreign half outright, in
its own javadoc.

**What this changes for the author's decision.** The queued item asks for a yes
on the extent's being stated first. It should be read as asking for two things,
and the second was invisible: **which extent**, because the answer decides
whether 066 and 068 are one defect or two, and therefore whether the caller-side
rule is worth building at all. The coordinator's recommendation, with its reason
and its cost, is in `docs/work/DECIDE.md` under `panel 169`.
