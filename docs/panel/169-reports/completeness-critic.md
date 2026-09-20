# Panel 169 — completeness critic

**No verdict.** This seat names what is missing.

Every number below came from a command run in this session, 2026-09-20, Darwin
25.6.0 arm64, in a copy of the tree at
`/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/e64edfa2-e1eb-4a90-b26e-8635f02430e5/scratchpad/critic169/tree`,
compiler built from the seed (`clang -I runtime seed/heroes.c runtime/runtime.c
-o heroes`, exit 0; **not timed**, so no duration claim is made). Nothing in
`/Users/joseph/Temp/heroes/heroes-lang` was modified but this file
(`git status --porcelain` before writing: the three untracked panel-169 paths and
nothing else). `archive/bootstrap-rs/` was never opened. `selfhost/` was never
rebuilt.

**What made this seat able to RUN the adopted rule rather than read it.** The
compiler-engineer's built R2 survives in the shared scratchpad at
`…/scratchpad/tree`: `selfhost/check/lend_alias.hero` (new),
`selfhost/checker.hero`, `selfhost/lend_errors.hero`, `selfhost/r2scan.hero`
(`diff -rq` against the main tree names exactly those four). I copied those four
into my own clean copy and exercised the rule through its own `swept()` entry
point. So every verdict below marked *R2-widened: N* is the built rule's own
diagnostic count, not a reading of the ballot's English.

- `./heroes test selfhost/checker.hero` → **397 tests, all passed** — the
  engineer's number, reproduced.
- `./heroes run selfhost/r2scan.hero` → **files scanned: 754, files refused: 0**.
  754 is 752 plus the two probe modules I added; the engineer's **0 of 752** is
  reproduced exactly, on a second build, by a second seat.

---

## 1. THE NEXT HOLE, AND IT IS NOT A `@` AT ALL

**The adopted wording is *"not WRITTEN in this function — by a `@` statement or
as a `@` argument"*. A `declare` is neither, and a `declare` on the second turn
of a loop is a write to the same storage.**

`w/redecl_pure.hero`, and there is no `@` anywhere near the lent root:

```
extern "k.h"
    record KSlot tag KSlot
        name: u8[8]
        id: i64
    function k_register(p: ptr counted_by n, n: i64)
    function k_read_later() -> i64

function main()
    i: i64 @ 0

    while i < 2
        b: u8 @ 72

        if i == 1
            b @ 1
        s: KSlot @ KSlot(name: [b, 2, 3, 4, 5, 6, 7, 8], id: 1)

        if i == 0
            k_register(p: s.name.ptr(), n: 8)
        print(k_read_later().to_str())
        i @ i + 1
```

| | |
|---|---|
| `./heroes check redecl_pure.hero` | **exit 0** |
| `./heroes run redecl_pure.hero`, three times | **`72` then `1`**, exit 0, three of three |
| `./heroes run redecl_pure.hero --sanitize` | **`72` then `1`, exit 0, zero sanitizer lines** |
| **R2-widened, the built module, on this exact file** | **0 diagnostics** |

That is defect 068's entry sentence word for word — *C holds a field's address,
the program writes the record, and C reads bytes the program never meant it to —
a wrong answer at exit 0 with zero AddressSanitizer reports* — and the adopted
rule is silent on it.

**The control, so the instrument is not being credited with blindness it does not
have.** The same file with an explicit `s @ KSlot(…)` added in the `i == 1`
branch (`w/redecl.hero`) is **R2-widened: 1**, and runs `72` then `1` likewise.
So the rule sees this file shape; what it does not see is the **statement kind**.

**Why, in the module's own text.** `judge_statement`
(`selfhost/check/lend_alias.hero:129-133`) reads:

    .mutate m                                                                     => judge_write(…)
    .bind | .declare | .return_stmt | .break_stmt | .continue_stmt | .assert_stmt => return

`.declare` returns. And `lent_roots` keys by local index, so the re-declaration
is the **same** local as the lend's root — the rule has the root in its map and
walks past the statement that overwrites it.

**And the compiler itself says the two are one instruction.**
`./heroes build redecl_pure.hero --dump-ir`:

    slots  i: i64 · b: u8 · s: KSlot · $own3: str
    bb4  if: join
        $t20: KSlot = construct KSlot($t18, $t19)
        store s <- $t20

One slot for the whole function, and `store s <- $t20` **inside the loop body**.
The emitted C agrees: `rp.c:122` declares one function-scope
`struct KSlot h2_s;` and the loop body executes `h2_s = t20;` — byte for byte the
instruction defect 068's own reproducer emits. **The AST distinguishes `declare`
from `mutate`; the IR does not, and the machine does not.**

This is CL-078 arriving for the second time in one sitting. The engineer widened
*re-assigned* to *written* after finding `fill(@t)`. The class is wider than
*written* too.

## 2. DOES R2-WIDENED CLOSE DEFECT 068 AS FILED?

**It closes the entry's reproducer. It does not close the entry's sentence.** The
entry (`docs/work/DEFECTS.md:120`) states a class — *"C holds a field's address,
the program writes the record, and C reads bytes the program never meant it to"*
— and names no program of its own; its reproducer is panel 168's critic's, which
the engineer retyped and which R2-widened refuses (**R2-widened: 1**, and I
reproduced the refusal). `w/redecl_pure.hero` satisfies the entry's sentence and
is **R2-widened: 0**.

The entry's own later paragraph is the one that should have been read as the
test: *"in the reproducer the wrong answer is produced by a **Heroes
assignment**"*. That sentence describes the **witness**. The sitting adopted a
rule shaped to it, widened it once, and the class is still one statement kind
wider.

## 3. THE SHAPES I RAN THAT ARE CLOSED — the honest negatives

Each was run through the built rule; each quiet one was then written as a program
and run. Naming them is the point: this is the enumeration, and a later sitting
should not have to redo it.

| shape | R2-widened | what settles it |
|---|---|---|
| `s @ KSlot(…)`, defect 068 itself | **1** | refused |
| `s.name[0] @ 9`, an index write straight into the lent array | **1** | `leasing.place_root` walks `.index`, so the root is found |
| `s.a.b @ x` / lend of a field of a field | **1** | same walk, `.field` recursive |
| write through a **second** `@` parameter, `both(@u, @s)` | **1** | `judge_arguments` asks every mutable arg's root |
| lend in one branch, write in the other | **1** | flow insensitivity, the stated over-refusal |
| array of records, `rows[0].name.ptr()` then `rows[0] @ …` | **1** | root is `rows` |
| `@` argument on an index of the lent root, `bump(@s.name[0])` | **1** | root is `s` |
| **UFCS mutating method, `t.fill()`** | **0** | **closed elsewhere**: `./heroes check` exit 1, `error[ufcs_on_mutable]` — *"`fill` changes its first argument, so it is called `fill(@x, …)` — the dotted form would hide the `@`"*. The language already refuses the spelling. |
| **a sibling declared after the lent block ends** | **0** | **does not corrupt**: `w/sibling.hero`, `check` 0, run `72 2 72` three of three. The slot is not reused. |
| **re-declaration of the lent root on a second loop turn** | **0** | **§ 1 above — the hole** |

`for x in …` is not a write: `ast.hero:91-94`'s `for_in` carries a `name` and
binds a fresh local, so there is nothing for the rule to miss there. **I did not
find a mutable-receiver spelling**; the vocabulary searched was `ast.hero`'s
`method` node (`receiver: i64`, no `mutable` field, `:162-165`) and the
`ufcs_on_mutable` refusal above.

## 4. THE OVER-REFUSAL IS NOT HYPOTHETICAL — two sound programs, refused, with no escape

The synthesis says the historian's caution is *"already falsified by two seats"*.
What the two seats measured is that **this corpus contains no instance**. The
rule over-refuses by construction, and the programs are nine lines each.

**(a) RFC 2094 Problem case #1, in Heroes.** `w/rfc2094.hero` — lend, let C read,
then **tell C to let go**, then write the root:

    k_register(p: s.name.ptr(), n: 8)
    print(k_read_later().to_str())
    k_forget()                              # C drops the pointer
    s @ KSlot(name: [1, …], id: 2)

`check` **exit 0** today, run prints `72` then `1`, both honest. **R2-widened:
1.** There is no spelling by which the author can say *C has let go*, so this
program has no repair but restructuring.

**(b) The in-and-out buffer, which is the idiom the R4 veto is built on.**
`w/refill.hero` — C fills the record's field each round, the program resets the
record between rounds:

    while i < 3
        k_fill(p: s.name.ptr(), n: 8)
        print(s.name[0].to_str())
        s @ KSlot(name: [0, …], id: 0)
        i @ i + 1

`check` **exit 0** today, run prints `1 1 1`, correct. **R2-widened: 1.**

The ffi seat's veto on R4 is that *"the field lend is C's only write path into
Heroes memory"*. R2-widened refuses the most ordinary loop over that write path
in the language, and the corpus does not contain it only because `examples/`
contains **zero** `.ptr()` at all (grepped: 0 in `examples/`, 34 sites in 9 files
across `tests/`, `spec/`, `site/`, `docs/`, of which two are a comment and a test
expectation string).

**So: the historian's prediction 1 is falsified as a statement about the corpus
and unfalsified as a statement about the rule**, and its condition 4's proposal
— *"the historical caution should be dropped from the record rather than carried
forward"* — must **not** be acted on. The mechanism it warned about is
demonstrable in nine lines; what is absent is an instance in a corpus of seven
root-bearing files. The synthesis should say that, and its prediction line should
read *falsified on this corpus* rather than *already falsified*.

Two further things make the falsification narrower than the synthesis states.
The historian named its instrument: *"`./heroes check` over the corpus plus the
`corpus`, `emission` and `run` suites, run after the rule lands"*. **None of
those was run** — the engineer lists the selfhost rebuild and the suites under
*What I did not run*, and calls the 752-file sweep a substitute it itself calls
*narrower*. And the second seat the synthesis credits did not measure the adopted
rule at all: see § 5.

## 5. TWO MEASUREMENTS THE SYNTHESIS TREATS AS ONE

> *"Two seats measured zero, on different instruments, over different file sets."*

**They do not cover the same thing, and only one of them is about the adopted
rule.**

| | compiler-engineer | ffi-pragmatist |
|---|---|---|
| rule measured | **R2-widened** — `@` statement **and** `@` argument | *"pure **re-assignments** of a lent root"* — the **ballot's** wording, the one the sitting refused |
| file set | 752 `.hero` files: `tests/golden`, `examples`, `selfhost`, `spec`, `tests/harness`, `site`, `docs` | the **8 files** holding a `.ptr()` **outside `selfhost/`** |
| instrument | the compiled sweep | a grep |

The ffi seat's zero is a count of the narrower rule over about 1% of the
engineer's file set, explicitly excluding `selfhost/`. It corroborates that the
corpus has no `s @ …` after a lend. It says nothing about the `@`-argument clause
the sitting adopted, and nothing about the 744 files it did not look at. The
engineer's zero is the load-bearing one and it stands alone; I reproduced it and
it should be cited alone.

Two smaller drifts in the same paragraph, grepped in the main tree today: the
synthesis writes *"33 `.ptr()` lends in 8 files, **all in `tests/golden/`**"*. A
plain `grep -rn '\.ptr()' --include='*.hero' tests examples spec site docs` reads
**34 sites in 9 files**, and one of the nine is
`tests/harness/suite_surface.hero:231` — not `tests/golden/`. It is inside a test
expectation string, so the ffi seat's filtered count is defensible and the
synthesis's **"all in tests/golden/"** is not.

## 6. THE `cstr` WIDENING IS ADOPTED, UNTESTED, AND MEASURED TO CLOSE NOTHING

The resolution says *"The `cstr` widening lands with it, measured free."* Three
things about it, each run.

1. **The built module does not contain it.** `lent_roots`
   (`lend_alias.hero:79`) reads `is_builtin_named(…, name: "ptr")`. The ten tests
   beside it are all `ptr`.
2. **No test distinguishes the two.** I made the one-line swap to
   `lending.is_lend` (which is `cstr` **or** `ptr`, `check/lending.hero:76-77`)
   and `./heroes test selfhost/checker.hero` read **397 tests, all passed** —
   the identical number. A widening that no assertion can tell apart from its
   absence is a widening with no golden.
3. **Free of over-refusal, confirmed; empty of closure, also measured.** With the
   swap in, `r2scan` reads **files scanned: 754, files refused: 0** — the
   engineer's claim, reproduced. But the shape it newly refuses has now been run
   **four times across two seats without once reproducing 068**: the warden's
   `r68b` (literal, prints 13) and `r68c` (heap `f"…"`, prints 34), and mine —
   `w/cstrlend.hero` (`check` 0, run `114` then `114`, `--sanitize` identical,
   exit 0) and `w/cstrheap.hero` (`check` 0, run `114` then `114`, three of
   three, `--sanitize` identical).

None of that argues against landing it — loud is the prescribed direction. It
argues that the synthesis presents it as *a measurement the ballot did not have*
on the **closure** side, where the only measurements available say the shape does
not corrupt. Say what it is: over-refusal bought cheaply, with a golden owed.

## 7. A ROUTE NOBODY LISTED — R6, state the rule where the sugar is gone

**What would have to be true for a sixth route to exist?** The five seats
enumerated routes along two axes: **what is marked** (a parameter, a group head,
a handle, ownership) and **which side** (caller or callee). Nobody enumerated
along the third: **which layer of the compiler the rule is stated at.** The
sitting's own title says the defect was never searched where it happens; the
search went one layer, not all of them.

R2 is stated over the **AST**, where the ways to write a binding are an **open
set** the rule must enumerate by hand. In one sitting that set went from one
(`@` statement) to two (`+ @` argument) and § 1 shows it is at least three
(`+ declare`). Every future sugar adds a fourth.

**Over the IR the set is closed and it is two.** Measured, from
`--dump-ir` on the three shapes:

| Heroes source | IR |
|---|---|
| `s @ KSlot(…)` | `store s <- $tN` |
| `s: KSlot @ KSlot(…)` **on a later loop turn** | `store s <- $tN` — the **same** instruction |
| `fill(@t)` | `call heroes fill(@t)` |

So a rule stated over the IR needs **two clauses, both of them enumerable from
the IR's own instruction set**, and it collapses `declare` and `mutate` into one
by construction rather than by a seat noticing. I state the limit rather than
overselling it: the `@` argument is **not** a store at the call site, so the IR
does not erase that clause — it makes the clause set finite and checkable against
the lowering instead of against the grammar.

**What it costs, named rather than waved at.** `selfhost/ir.hero` is at 310 of a
`DECIDED` 310 (the engineer re-measured it this sitting), so a pass there needs
the split `.claude/rules/module-shape.md` prescribes — and a ceiling is rank 6
while this is rank 3. The real objection is `ast.hero:161`'s own rule, *errors
must speak in the syntax written*: an IR-stated rule owes a span back to the
source line. The IR carries them — the emitted C is full of
`#line 16 "redecl_pure.hero"` — so the objection is a cost, not a refusal.

**This is not a proposal to delay R2.** It is the question the sitting did not
ask, and the answer decides whether the third widening is the last one.

## 8. PRIOR ART INSIDE THIS COMPILER THAT NO SEAT AND NO BRIEF CITED

`error[field_lend_written]` — `selfhost/lend_errors.hero:283`, raised at
`selfhost/emit/ffi_lend.hero:74`, from **panel 166 / defect 065**. Its message:

> *"`t.nsap.ptr()` lends a field of `t` to `slot_fill`, and the header declares
> that parameter `void *` — C may write through it, and `t` is not a `@` name"*
> … *"§4.4: only a declared `@` name can be mutated, and a C write is a
> mutation."*

This is the **closest existing ruling to R2** — a rule relating a field lend to
the mutability of its root — and it appears in no brief and no report. Two things
follow that the sitting should have run.

- **It is an EMIT-stage diagnostic**, so it is `build` and not `check`. The
  shared brief's own item 1 (*"`check` is not `build`"*) applies to the rule
  R2 sits next to, and nobody checked whether the two belong at the same stage.
- **The two rules push opposite ways on the same line.** Panel 166 tells an
  author whose C parameter is non-`const` to declare the lent root `@`
  ("a C write is a mutation"). R2-widened then forbids writing that `@` root in
  that function. The composite rule an author must hold is *the root must be
  declared mutable and must never be mutated here* — which is exactly
  `w/refill.hero` in § 4(b), refused. This interaction is unexamined.

## 9. WHAT THE SYNTHESIS MISREPRESENTS OR OMITS

**(a) The biggest one: two seats independently produced the same missing
sentence, and the synthesis records neither.** `grep -c "R0"` on the synthesis
returns **0**; `grep` for *"third sentence"*, *"keeps end_fn"* and *"+27"*
returns nothing.

- The **spec-warden** priced a row it called **R0** — *"lifetime stated for BOTH
  lends, merged"* — at **+27 real, 8181**, and wrote: *"What IS missing from the
  prompt is in the gallery instead of the spec: `examples/gallery/13-lease.hero:3`
  tells the reader `s.cstr()` lends a string to C **for one call**. That rule is
  in the teaching material and absent from the document that IS the prompt."*
- The **llm-ergonomist**, which read only the spec, reached the same place: *"the
  document contains no sentence about how long a lend is readable, and both
  candidates presuppose one"* — *"**This is the whole sitting**"* — and proposed
  sentence **T**, then: *"**The cheap half, if T is too much.** The first clause
  alone … **I would take that over either candidate as written.**"*

Two seats, two inputs, neither aware of the other, converging on one sentence,
one of them with a `--refresh` price on it. CLAUDE.md § 4 asks for the most
robust and complete resolution, never the cheapest. The synthesis adopts R2
alone and never records that the option existed, what it cost, or why it was not
taken. **That is the completeness failure of this sitting.**

**(b) The ergonomist's objection is recorded as decoration and is in fact
unwithdrawn.** The verdict table reads *"object (V2) — the better half, but its
trigger is undefined"*. Its stated condition is: *"I withdraw the objection to V2
if it is amended to state the extent in the same sentence."* The adopted wording
states a **scope** (*in this function*) and not an **extent** (*how long the lend
is readable*), and resolution item 2 then makes the spec sentence **optional** —
*"what it costs **if the sitting chooses to say it out loud**"*. So the sitting
may land a new compile error with no sentence in the document that is the prompt,
which is the precise thing the only seat that read the prompt said not to do.
Resolution item 2 should either carry the warden's **+16 merged** as owed rather
than optional, or say why the objection is overridden.

**(c) The one-line resolution outruns the evidence on R5.** The one-liner reads
*"the route nobody listed, R5 … **is what closes 066**"*. Item 3 then says it is
*"registered rather than adopted because it is not built"*, and the ffi seat's own
prediction 2 says R5 needs one more rule that is measured broken today
(`consumes` does not refuse a second consume of a local: `check` 0, `build` 0,
run `133 133 133`). And what the ffi seat actually predicted is that 066's
reproducer becomes **unwritable**, which is a different claim from closed and is
a prediction, not a measurement. The one-liner should read *is the route for 066*,
which is what item 3 says.

**(d) The verdict table's `—` for the compiler-engineer on R1 understates it.**
The engineer's appendix B is a measurement on R1 that the body of the synthesis
relies on and the table hides: `leak.hero` printed its answer **first** and
aborted **after**, so *"the live set is an **exit** check, not a **use** check"*,
and *"a seat arguing R1 closes 066 owes a run that shows the abort landing before
the read."* That belongs in the row.

**(e) A claim in the R2 section that is not a measurement.** *"Widening to `not
written` … is **22 lines**, catches both, and **still refuses nothing**."*
*Catches both* is true of the two shapes looked at and false as stated: it is
measured to catch two of the three that now exist (§ 1). Write *catches both of
those*.

## 10. WHAT I DID NOT RUN, NAMED

- **`./heroes build selfhost/main.hero` with R2 in, and the golden suites.** The
  brief forbids the selfhost rebuild. So no `check`/`run`/`emission`/
  `determinism`/`corpus` verdict in this report, and the historian's named
  instrument remains unrun by anybody.
- **Any timing.** No `/usr/bin/time -p` figure appears here and none should be
  quoted from it.
- **`--refresh` on any spec draft.** `.env` is not in a scratchpad copy; the
  warden's figures stand and I did not re-derive them.
- **A search for a mutable-receiver spelling beyond `ufcs_on_mutable`.** What I
  searched is named in § 3; *there is no other* is a question, not a claim.
- **Whether the re-declaration hole has an instance in the corpus.** `r2scan`
  measures refusals, not misses. A sweep for *a lend whose root is re-declared*
  does not exist and I did not write one, so **how many corpus files hold § 1's
  shape is unrun**.
- **raylib and curl.** Not opened; the ffi seat's header counts are its own.

## 11. WHAT I ASK THE SYNTHESIS TO CHANGE

1. **Widen again, or state the hole in the resolution.** The adopted rule's own
   wording is measured to leave defect 068 open one **statement kind** over. Take
   the wording to *a binding whose field's address has been lent in this function
   is not WRITTEN in this function — by a `@` statement, as a `@` argument, or by
   a re-declaration of the same binding* — or record § 1 as a filed defect
   against the resolution the day it lands, with `w/redecl_pure.hero` as its
   golden. Silence is the third repair of a witness in one milestone.
2. **Put R6 on the next ballot**: state the rule over the IR, where `declare` and
   `mutate` are already one instruction and the clause set is closed.
3. **Correct the historian's line** to *falsified on this corpus*, keep the
   caution, and record §4's two sound programs as the over-refusal the corpus
   does not happen to contain.
4. **Un-merge the two corpus measurements** (§ 5) and cite the engineer's alone.
5. **Record R0 / the ergonomist's cheap half**, with the warden's **+27**, and
   say why it is not adopted beside R2 — or adopt it.
6. **Make the spec sentence owed, not optional**, or record the ergonomist's
   objection as overridden and on what ground.
7. **Owe a golden for the `cstr` widening**, since 397 tests cannot tell whether
   it is there.
8. **Run the `field_lend_written` interaction** (§ 8) before R2 lands: the two
   rules meet on the same line and at two different stages.
