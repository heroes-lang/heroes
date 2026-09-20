# Panel 168 — the property is not the base, it is the heap, and route A closes zero of two

Convened 2026-09-20, at M-declared-extents, on **panel 167 clause 1's allocation
half**. The **soundness lane**: two seats, `compiler-engineer` and
`ffi-pragmatist`, because the proposal changes no surface, no diagnostic text and
no spec token. Three briefs and the C model written to disk before either seat
started, a completeness critic after them, and the working tree frozen from the
briefs going out until this file was written.

**The sitting answered the question it was asked and the critic found a larger
one underneath it.** Both are in the resolution.

## The proposal, verbatim

> Panel 167 clause 1 adopted a **trailing** header for route A's lease
> allocation, on the ffi-pragmatist's ground that a leading one traps the
> give-away case. Build route A on the **leading** header that already ships
> instead, and leave design.md §4.19's third reserved case — a buffer C takes
> ownership of — to the feature that builds it, with its own spelling that
> waives `end_lease` and its own allocation question.

## The resolution, in one line

**The proposal is ADOPTED and panel 167 clause 1's allocation half is struck.**
The engineer **vetoes** the trailing header on design.md §1.12, having measured
that it creates a corruption class the leading header does not have; the ffi
seat approves and **withdraws its own panel 167 prediction**, having measured
that the property the give-away case needs is not the allocation base but the
allocation **heap**, which no header layout inside the Heroes runtime can
supply. **And the critic measured that route A closes NEITHER defect 066 nor
068** — on the half of the language that already shipped — so panel 167 clause
8's *"this entry closes when that lands"* is false, panel 167's own spec-warden
prediction is **scored correct**, and **M-declared-extents cannot close on route
A**. Clause 2 is **suspended**, not refused, on three measurements.

## The verdict table

| | compiler-engineer | ffi-pragmatist |
|---|---|---|
| **verdict** | **approve**, and **VETO** the trailing header if it is kept | **approve**, no veto, no ABI break |
| **section** | design.md §1.12 (`design.md:571-593`), §1.1/§1.7 on the ceiling | design.md §4.19:2332-2338, §1.11 via `.claude/rules/c-boundary.md` |
| **cost** | the proposal is **0 lines in every pass**; route A's field lease on it is ~40 lines across 5 files, no ratchet touched | none; it leaves the runtime alone |
| **condition** | a mechanism by which the release decides, without reading the block and without a new IR operand, that the block is still the program's. Searched; the shipping handle set fails it, because C's `free` does not remove the entry | a give-away binding over a `hero_alloc_held` block that survives a non-`malloc` library allocator, or a shipping library whose ownership-taking destructor is contractually `free` |

The completeness critic gives **no verdict**. Its findings are in their own
sections below, and two of them change the resolution.

## THE FINDING THAT MATTERS MOST — route A closes neither defect

The sitting was scoped to the allocation half, both seats accepted that scope,
and **neither measured whether route A does the thing the milestone needs done.**
The critic did.

**The `cstr` side of the language already IS the world route A plus clause 2
would create for `ptr`**: a lease that copies, both type rules in force, the
position rule in force, and defect 067's repair landed at `69853b58`. If a copy
with an owed release closed this defect class, the class would be closed there.

It is not. The coordinator re-ran the critic's finding on the repository's own
shipped example, `examples/gallery/13-lease.hero`, **with the print moved two
lines down, after `end_lease` instead of before it** — the file is committed
beside this sitting as `168-briefs/gallery-example-reordered.hero`:

    ./heroes check   -> exit 0
    ./heroes run     -> exit 0, five of five, prints "C still reads 0 bytes"
                        where the honest answer is 13
    --sanitize       -> heap-use-after-free, 13-lease.h:7 in kept_label_length

**A `cstr` lease has no lifetime rule either.** Defect 066's entry reads *"a
`ptr` lend has no lifetime rule, so C may keep the address past the frame"*. The
lease's sentence is the same with one word changed: **C may keep the address past
`end_lease`.**

What a copy buys is real and smaller than panel 167's resolution says: it moves
the moment the bytes die from *the frame returns* to *`end_lease` runs*. **Both
are Heroes-side events, and the defect is that C's retention is unrelated to
either.** Copying the bytes does not relate them.

**Two sentences in this repository cannot both be true**, and the critic found
them:

- `docs/panel/167-nobody-checks-the-callee-and-the-lease-we-would-copy-is-open.md`
  § Predictions, the spec-warden's: *"routes A, B and C each
  close **zero of two** reproductions when landed alone."*
- `docs/work/DEFECTS.md`, defect 066: *"**Panel 167 adopts route A** … this entry
  closes when that lands."*

The measurement says the warden's is the one to keep. **It is scored here rather
than at the close**, because it was checkable the day it was written and nothing
read it.

## The finding the sitting was convened for, and it is the ffi seat's

**The property the give-away case needs is not the allocation BASE. It is the
allocation HEAP.**

The seat gave the trailing header its best case first — the give-away with
`end_lease` waived, which is what a future third-case spelling would do — against
the SDK's real `sqlite3.h` 3.51.0 and the real `/usr/lib/libsqlite3.dylib`,
current version 382.0.0. It worked: `0 0 0 0 0`, correct bytes, zero ASan
reports.

Then it asked what panel 167 did not: **is `sqlite3_free` `free`?** The header
documents the answer at `:1879-1891` — the allocator is replaceable through
`sqlite3_config(SQLITE_CONFIG_MALLOC, ...)`. The seat registered a private
`sqlite3_mem_methods` whose `xFree` keeps its own 16-byte size word, which is what
memsys3, memsys5 and every arena allocator in the wild do. The system library
accepted it, `rc=0`:

| | five runs |
|---|---|
| **replaced allocator + trailing header** | **`134 134 134 133 133`**, dying inside `sqlite3_finalize` |
| replaced allocator + `sqlite3_malloc` block | `0 0 0 0 0` |

The failing runs print the seat's own allocator's words: *"my_free: size word
reads 45550143952 — this is not my block"*. **So the trailing header's success is
a fact about this build of libsqlite3, where `sqlite3_free` happens to wrap
`free`, and not about the C ABI.** The evidence is committed as
`168-briefs/allocator.c`, because the critic pointed out that a request in a
seat's report is not a resolution and prose about an unrunnable file is not a
measurement.

**A seat withdrawing its own prediction on its own measurement is recorded as the
seat's act**, not as anybody's correction of it.

## The veto ground, and it is the engineer's

The brief's model varied the layout and nothing else. The engineer varied the
**size** — CLAUDE.md § RUN IT's *a repair is attacked at the shapes next to the
one that provoked it* — and found where the trailing header stops being merely
useless. Three runs, identical, and independently reproduced by the critic on its
own probe, three of three:

    n=4096     magic survives the free: no   -> release would panic
    n=65536    magic survives the free: YES  -> release would DOUBLE FREE
    n=1048576  magic survives the free: YES  -> release would DOUBLE FREE

Run for real at 64 KiB, five runs: **no `panic:` line is printed**, the magic
check passes, and the process dies inside the **second** `free`, exit 134, with
nothing on stderr.

`runtime/parts/str.c:447-450` already states the rule this breaks — *"freed memory
owes nobody its contents"* — written when an earlier draft of this same feature
made the same mistake. Under the leading header the runtime never reads the block
after C frees it, because C's `free` on `base + 16` is rejected by the allocator
at the fault, and ASan names the program's own C line.

**Unrun and named**: the 64 KiB survival is allocator behaviour on Darwin arm64.
glibc's `mmap` threshold is 128 KiB by default, so the 1 MiB case is likely the
same on Linux and the 64 KiB one may not be. It does not change the verdict — one
platform where the check silently passes is enough for §1.12 — but this file does
not say *at 64 KiB everywhere*.

## Clause 2 is not what makes route A sound, and it refuses safe code

Panel 167 clause 2: *"the two type rules are widened from `cstr` to `ptr`.
Without them route A is not sound, and that is measured rather than argued."* The
critic ran the programs that ground implies, and all three legs fail.

**The unsafe shapes clause 2 aims at are already refused, by a different rule.**
Five programs, every one `check` **exit 1** today on the position rule:
`function give(s: KSlot) -> ptr` returning a lend (`field_lend_escapes`), a lend
in a variant payload (`field_lend_escapes`), a lease in `[cstr]`, in
`{i64: cstr}`, and in a variant payload (`lease_escapes` each). Clause 2's two
type rules are a backstop behind a rule that is already closed.

**What clause 2 would newly refuse is safe.** `function mk_block() -> ptr`
returning a `malloc` is `check` 0 / `run` 0 today; `record Arena { base: ptr, n:
i64 }` holding a `malloc`'d block is `check` 0 / `run` 0. Neither has a lend, a
lease, a frame or a borrowed byte in it. **A `cstr` is a pointer into somebody
else's bytes; a `ptr` from `malloc` is a block the program owns**, and widening a
rule written for the first onto the second refuses the ordinary way to bind any
opaque-handle library. CLAUDE.md §12, CL-005: a refusal is held to the same
standard as a feature and owes a design.md Part 6 row naming the program fact
that would make it wrong. No such row is drafted.

**And the rule is bypassed by one line, which the compiler itself recommends.** A
tagged handle is a pointer, and `record Box { h: CThing }` outside any group is
`check` 0 / `run` 0 — the line that
`unsupported[pointer_element]`'s own note tells the author to write, quoted in
`examples/ledger/db/sqlite.hero:201-203`. Three Heroes records outside a group
hold tagged handles today, two of them in that shipped example.

**The blast radius, enumerated from the world rather than grepped.** An
indentation-aware walk of every `.hero` file under
`examples tests selfhost spec docs site tools harness editors`: **0** Heroes
functions answering `ptr`, **1** record field and **1** variant payload outside a
group (both in `tests/golden/unsupported/`, both resting today on a **build-time**
diagnostic), 3 raw hits exempt inside groups, and **3** records holding tagged
handles. Both fixtures it hits would move their `#~` annotations and their
`.expected` files, and per `.claude/rules/verification.md` a change to what the
checker refuses is judged by **every** golden tree.

**And the rules it copies are not transitive.** `record Box { c: cstr }` is
`check` **1**, but a **variant** case payload holding `cstr` is `check` **0**, and
so are `record Box { cs: [cstr] }` and `record Box { c: cstr? }`. Widened
faithfully the `ptr` versions inherit three holes; widened transitively they stop
being the same rule.

## The routes nobody listed, and the critic found four

Both seats searched at the layer they were assigned. The engineer's *"no third
route exists at the allocation layer, and that is provable rather than
unsearched"* is probably true **and it is about the allocation layer**, which the
ffi seat had already shown is the wrong one.

**1. The language already owns a retention vocabulary, and both sittings priced
it as if it did not.** `spec § 13`'s grammar already parses `counted_by`,
`owned <freer>`, `consumes` and `acquires <ender>` / `borrows` on an FFI
parameter. Three consequences, none stated by any seat in either sitting:

- **`borrows` is a retention statement that already ships** — the spec's own
  words, *"`borrows` says the call hands back one it keeps"*. Panel 167 concluded
  *no declaration-site mark can express retention at all*; the language has one,
  built for handles, and whether it can reach a `ptr` or `cstr` parameter is a
  question nobody asked.
- **The run-time instrument this sitting says does not exist already exists.**
  `spec § 13`: *"The live handles are a set, so giving one back twice aborts on
  its own."* The engineer's objection to a pointer-keyed table is correct about
  *is this block still the program's* and no objection at all to the table the
  project ships, which answers *is this pointer's life over*.
- **§4.19's third reserved case is `owned` read backwards.** `owned sqlite3_free`
  already spells *C made this, you free it, with that function*. The mirror spells
  *Heroes made this, C frees it, with that function* — and the moment the group
  names the freer, the emitter knows **whose heap** to allocate from, which is
  exactly the ffi seat's experiment 2. Both sittings framed the third case as an
  allocation-layout question. **It is a declaration question whose allocation is a
  consequence.**

**2. Defect 068's corrupting action is a Heroes statement, and nobody treated it
as one.** In the reproducer the wrong answer is produced by a Heroes assignment
two lines below the lend, in the same function, to the very binding whose field's
address was lent. **No C code participates in the corruption; C only observes
it.** That is caller-side, which is precisely where panel 167's historian found
that static enforcement *does* exist in every ecosystem it surveyed. The
machinery is already in the tree: `field_lend_escapes` and
`field_lend_needs_a_place` (`selfhost/check/lending.hero:218-228`) already relate
a lend to its root binding. So 068 was classified with 066 as *foreign retention*,
filed as unsolvable on that ground, and **never searched at the layer where its
defect actually happens**.

**3. A group-level mark is not route B**, and it is priced here for the first
time. A mark on the `extern` group head line — *this library may retain what it is
given* — decides nothing per call, so panel 167's run-time-`bool` program does not
refute it, and it lands in **different files**: `parse/group.hero` at 235 of 300
rather than `parse/members.hero` at 298 of 300. It does hit `ast.hero` 524 of 525
and `print/fmt.hero` 1174 of 1175, so its price is two ceilings rather than one.

**4. And a ceiling veto against a robustness route inverts § Precedence.** The
~300-line module ceiling is CLAUDE.md § 11, rank 6. A route that closes a
memory-corruption class is rank 3, and CL-012 names **compiler size** in the list
robustness beats. Panel 167 refused route B partly on rank 6 against rank 3. The
critic takes no position on whether B should have been adopted and neither does
this resolution; what is recorded is that **the ground it was refused on is the
ground the contract says loses**, and that a split is what
`.claude/rules/module-shape.md` prescribes and nobody priced.

## The disagreements, stated plainly, and one is settled by running it

**On when `ffi_writable_parameter` fires, three parties gave three answers.** The
coordinator's brief said the declaration is refused *"before any of this is
reachable … exit 1"*, stage unnamed. The engineer measured `check` exit 0 and the
refusal at `build`. The ffi seat measured that the declaration **alone** is
`check` 0 **and** `build` 0. The critic settled it on a file with **no call in
it**: `check` **0**, `build` **1**, caret on the declaration line. **The engineer
is right, the ffi seat's correction 1 is false and contradicts its own
experiment-4 table, and the brief was true but silent about the stage.**

**The consequence is worth more than the correction, and it becomes a rule
here.** `check` is what an editor runs, and it is exit 0 on this and on
`unsupported[pointer_element]`. **Any sentence of the form *"the checker refuses
X"* about FFI names its stage or it is unrun.**

**It also revises the ffi seat's defect B.** Its ground was that the note is
*"false for a lease"*. The diagnostic is sited on the declaration, where no lease
and no call exists yet. The note is not false for a lease; it is a note that
cannot know what will be passed, which is a different defect with a different
repair.

**On `const void *` in `sqlite3.h` the two rulers disagree**: the ffi seat reads
37 where panel 167 recorded 28, and `raylib.h` reads 7, which matches. The seat
names its ruler and does not claim the other wrong. Nothing rests on it.

**On the exit code of the leading header's give-away, it is not stable**, which
the brief got wrong by measuring one build: 133 five of five unbuffered,
`133 134 133 134 133` buffered, `133 ×8, 134 ×2` in the engineer's ten Heroes
runs.

**And both seats corrected the sitting on where the process dies.** Under the
leading header it dies **inside `sqlite3_finalize`**, before `end_lease` is
reached. The brief and panel 167 both put it at the release.

## What a seat swallowed, and it is a scope sentence rather than a number

The brief's *"clause 2, clause 3 and the refusals of routes B, C and D are not in
front of this sitting"*. Both seats accepted it; the ffi seat then **voted**
*"clause 2 stays"* on a re-run of its premise alone, without measuring anything
the widening would refuse. **That scoping is what produced a full sitting on a
case the same report proves the language cannot express, and zero measurements on
066 and 068.** The coordinator wrote the sentence and it is the coordinator's
error, not a seat's.

One number: the brief's `.lease()` 28, `.cstr()` 126, `.ptr()` 65 are `grep -rn`
**line** counts. The occurrence counts are **29 / 136 / 67**. Nothing in a verdict
turns on it; the corpus-size argument does, so both rulers are named.

## Route A on the leading header is sound, measured rather than argued

The ffi seat built route A's own program: `struct dirent`'s `char d_name[1024]`,
`sizeof(struct dirent)` = 1048, copied out and handed to libsqlite3 under
`SQLITE_STATIC`, stepped, reset, `sqlite3_clear_bindings`, then `end_lease`.
**Both layouts `0 0 0 0 0`**, three rows stored, **zero AddressSanitizer lines on
both**. The engineer walked the four things that could need the allocation base
and none does: `sizeof(HeroHeldHeader)` = 16, `_Alignof` = 8,
`_Alignof(max_align_t)` = 8, six `malloc` returns all `base % 16 == 0`, and
**nothing reads `h->len`** — it is written at `str.c:423` and never consulted.

This is why the resolution keeps route A's soundness finding while suspending its
value: **the construct is sound on the leading header and it closes nothing.**

## The resolution — `provisional — author ratification pending`

Per CLAUDE.md § 4, the most robust and complete resolution, never the cheapest
and never a compromise.

1. **The proposal is ADOPTED. Panel 167 clause 1's allocation half is struck.**
   Route A, if and when it is built, is built on the **leading** header that
   ships. No `hero_held_release_n`, no side table, no allocation change. The
   engineer's veto stands against any return to the trailing header.
2. **Panel 167 clause 8 is corrected: route A does NOT close defect 066**, and
   defect 066's entry is corrected in place to say so. **Panel 167's spec-warden
   prediction is scored CORRECT**, here rather than at the close, and the sitting
   records that it was checkable the day it was written.
3. **Defect 066's class is wider than its entry says, for the second time.** It
   was corrected from *field lend* to *lend* on 2026-09-20; it is corrected again
   to **lend AND lease**, measured on the repository's own shipped gallery
   example with one line moved. The class is: **nothing in the language relates
   C's retention to the moment the bytes die**, and a copy does not relate them.
4. **Clause 2 is SUSPENDED, not refused**, and re-opened as a question. Three
   measurements: the position rule already refuses every unsafe shape at check;
   what clause 2 would newly refuse is safe; and it is bypassed in one line by
   the idiom the compiler's own note recommends. **It is not built until that is
   settled**, because landing a refusal that refuses safe code is a regression
   and CL-005 asks a refusal to name the program fact that would make it wrong.
5. **M-declared-extents cannot close on route A**, and this sitting says so
   rather than leaving it to the close checklist to discover.
6. **Four routes are carried to the next sitting, named so they are not
   rediscovered**: the language's existing retention vocabulary and §4.19's third
   case as `owned` read backwards; defect 068 as a **caller-side** problem, where
   static enforcement exists in every ecosystem panel 167's historian surveyed; a
   group-level mark, which is not route B and lands in different files; and the
   § Precedence finding, that a ceiling veto against a robustness route is rank 6
   against rank 3.
7. **Three defects are filed** — the lease read past `end_lease` (folded into 066
   as clause 3 says, because a repair is owed at the class), the give-away that
   dies with an empty stderr, and the C function name passed as a callback, which
   is `check` 0, `build` 0, `run` 134 with a panic that blames the compiler. The
   third is **load-bearing rather than incidental**: a destructor callback is the
   one mechanism every real C library offers for exactly the retention problem
   066 and 068 are about, and Heroes cannot pass one.
8. **The seats' evidence is committed**, `168-briefs/allocator.c`,
   `bind_blob.c`, `failpath.c`, `fieldlease.c` and the reordered gallery example
   — because a request in a seat's report is not a resolution, and prose about a
   file nobody can re-run is not a measurement.

**What a veto compels.** The trailing header stays refused whatever the author
decides among the rest.

**What conservative would have been, so the author can choose it**: adopt the
leading header and say nothing about route A's efficacy, leaving defect 066's
entry reading *"this entry closes when that lands"*. That is the cheapest thing on
the table and it is false, measured; it would have the milestone build a feature
for two more steps and discover at the close that the list is not clean.

## Predictions to score, at the M-declared-extents close

- **compiler-engineer**: `layout` green with no new `DECIDED` row, and
  `selfhost/ir.hero` still **310**, `emit/inst.hero` **350**, `emit/ctype.hero`
  **395** in `code_lines`; zero lines in `selfhost/parse/`, `ast.hero`, `print/`,
  `ir/`. **Counter-prediction: keep the trailing header and `ir.hero` exceeds 310
  or `DECIDED` gains a raised ceiling — there is no third outcome.**
- **ffi-pragmatist**: step 3 of §4.19's ladder needs **no shim** under the
  leading header for the two retention modes Heroes can express, exit 0 with zero
  `--sanitize` reports; the third stays `error[type_mismatch]` exit 1 until a
  feature waives `end_lease`; and **when §4.19's third reserved case is built its
  buffer comes from the LIBRARY's allocator, not from `hero_alloc_held` at any
  offset** — falsifier run today at `134 134 134 133 133`.

**Scored here rather than at the close**, because it was checkable when written:
**panel 167's spec-warden prediction — *"routes A, B and C each close zero of two
reproductions when landed alone"* — is CORRECT**, measured on the `cstr` half
that already ships.

## Author's verdict

**Pending.** Queued as `panel 168` in `docs/work/DECIDE.md`, and the adopted
resolution is the default while it stands there.

**What a yes settles**: that the runtime's lease allocation does not change, that
route A is sound but closes nothing, that clause 2 is not built until it is
re-argued, and that this milestone's remaining question is 066 and 068 rather
than route A.

**What it does not settle, and both are named rather than left to be
rediscovered**: what closes 066, and what closes 068 — which the critic argues is
a different problem from 066 and has never been searched as one.
