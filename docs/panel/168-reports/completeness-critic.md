# Panel 168 — completeness critic

**No verdict.** I am not a sixth judge. What follows is what the two seats and
the shared brief left out, each item run rather than argued.

**Where I ran.** A `cp` of the tree, `.git`/`build`/`target`/`archive` excluded,
at
`/private/tmp/claude-501/-Users-joseph-Temp-heroes-heroes-lang/e64edfa2-e1eb-4a90-b26e-8635f02430e5/scratchpad/critic/tree`,
compiler built there with
`clang -I runtime seed/heroes.c runtime/runtime.c -o heroes` — `/usr/bin/time -p`
reads **real 3.80, user 3.66, sys 0.08**, `./heroes --version` = `heroes 0.2.0`.
Darwin arm64, 2026-09-20. Nothing in the main tree was modified except this
file. Every `.hero` probe below is reproduced **in full** in § Appendix, because
a scratchpad path dies with the session.

**One sentence up front.** Both seats approve a proposal that is right, and
neither of them measured whether it does the thing the milestone needs done.
**Route A closes neither defect 066 nor defect 068**, and I have run the program
that proves it on the half of the language that already shipped.

---

## 1. THE QUESTION THE SITTING SHOULD HAVE ASKED — does route A close 066 and 068?

This is the most important thing in this report, so it is first.

### 1.1 Both defects reproduce on today's compiler

`defect 068`, the record rewritten under C's held address (`alias.hero`):

    ./heroes check alias.hero       -> exit 0
    ./heroes run   alias.hero       -> 72 / 1, exit 0, three runs of three
    ./heroes run   alias.hero --sanitize | grep -c AddressSanitizer  -> 0

`defect 066`, the frame that dies (`escape.hero`):

    ./heroes check escape.hero      -> exit 0
    ./heroes run   escape.hero      -> 120, exit 0, three runs of three   (honest: 72)
    ./heroes run   escape.hero --sanitize -> stack-use-after-scope, ABORTING

Both are as `docs/work/DEFECTS.md` describes them. Nothing here is new.

### 1.2 Route A refuses nothing, so neither reproducer changes

Route A adds a **second use of `.lease()`** on a field. Clause 2 widens two type
rules: *no Heroes function answers a lent pointer type*, *no record outside a
group holds one*. Read against the two reproducers: `escape.hero` and
`alias.hero` each contain **one** `.ptr()` standing as an argument of a call, no
Heroes function answering `ptr`, and no record outside a group holding `ptr`.
Neither clause reaches either program. **This is a reading of the rule text
against the two files, not a measurement**, and I say so — the rule does not
exist yet to be run.

### 1.3 The measurement that settles it, and it is on the half that already shipped

The `cstr` side of the language **is** the world route A plus clause 2 would
create for `ptr`: a lease that copies, both type rules in force, the position
rule in force, and defect 067's repair landed at `69853b58`. If a copy with an
owed release closes this defect class, the class is closed there already.

It is not. `lease_direct.hero` — a lease, handed to a C function that keeps the
pointer, then released by the program that owes the release:

    ./heroes check lease_direct.hero  -> exit 0
    ./heroes run   lease_direct.hero  -> 0, exit 0                (honest: 72)
    ./heroes run   lease_direct.hero --sanitize
      ==26850==ERROR: AddressSanitizer: heap-use-after-free
      SUMMARY: AddressSanitizer: heap-use-after-free lease_direct.hero:10 in h_leasedirect_main

**A `cstr` lease has no lifetime rule either.** Defect 066's entry reads *"a
`ptr` lend has no lifetime rule, so C may keep the address past the frame."* The
lease's own sentence is the same with one word changed: **C may keep the address
past `end_lease`.** Wrong answer, exit 0, no diagnostic, and the sanitizer sees
it only because the bytes happen to be heap.

What the lease buys is real and it is smaller than the resolution says: it moves
the moment the bytes die from *the frame returns* to *`end_lease` runs*. Both
are **Heroes-side events**. The defect is that C's retention is unrelated to
either. Copying the bytes does not relate them.

### 1.4 Panel 167 already predicted this, in its own file, and DEFECTS.md says the opposite

Two sentences in the repository that cannot both be true:

- `docs/panel/167-…md` § Predictions, the spec-warden's, registered for scoring
  at this milestone's close: *"routes A, B and C each close **zero of two**
  reproductions when landed alone."*
- `docs/work/DEFECTS.md:70-72`, defect 066: *"**Panel 167 adopts route A** … this
  entry closes when that lands."*

Panel 167's own completeness critic wrote the third version, at §5.2 of
`docs/panel/167-reports/completeness-critic.md`: *"Routes A and B close it (a
copy, or a refusal)."* A copy that the program may use is not a refusal of the
lend, and only a refusal closes a defect. The measurement in §1.3 says the
warden's prediction is the one to keep.

### 1.5 What is still missing, named

CLAUDE.md § Verification: *a milestone is tagged only over a clean list*, zero
open items in `docs/work/DEFECTS.md`. `DEFECTS.md` reads `**OPEN: 2**` and both
are 066 and 068. So M-declared-extents cannot close on route A alone, and the
sitting in front of the author does not say so.

Three things are missing and each is nameable:

1. **Nothing refuses the lend.** 066 closes only if `.ptr()`/`.cstr()` at a
   parameter C may retain becomes an error, or disappears. That is panel 167's
   route D, explicitly deferred by its clause 6 to *"the next sitting's
   question"*. There is no next sitting; there is this one, and it did not take
   it. And route D is measured against `.ptr()` only: the corpus half is
   `.cstr()`, 126 lines / 136 occurrences against `.ptr()`'s 65 / 67 over
   `examples tests selfhost` (`grep -rn --include='*.hero'` and `grep -rho …|wc -l`).
2. **Nothing refuses retention past `end_lease`** (§1.3). Route A inherits this
   hole from the mechanism it extends, and no clause of 167 or 168 mentions it.
3. **068 has no route at all on the ballot.** See §3.2: its corrupting action is
   a Heroes statement, which makes it a different problem from 066 and nobody
   has treated it as one.

---

## 2. THE TWO SEATS DISAGREE ABOUT `ffi_writable_parameter` — settled by running it

Three versions were in play. The shared brief (measurement 3): *"refused before
any of this is reachable … exit 1"*, stage unnamed. The compiler-engineer (M3):
*"`./heroes check` on that file is exit 0. `error[ffi_writable_parameter]` fires
at `./heroes build`, exit 1."* The ffi-pragmatist (correction 1): *"the
**declaration alone** is `heroes check` exit 0 and `heroes build` **exit 0**.
`ffi_writable_parameter` fires only when the function is **called**."*

Three files, each `free(p: cstr)` against `stdlib.h`:

| file | what is in it | `heroes check` | `heroes build` |
|---|---|---|---|
| `decl_only.hero` | the declaration, **no call anywhere** | **0** | **1** |
| `decl_and_call.hero` | declaration + `free(p: x.cstr())` | 0 | 1 |
| `decl_and_lease_call.hero` | declaration + a lease passed to it | 0 | 1 |

`decl_only.hero` has no call. Its build output:

    error[ffi_writable_parameter]: `p` of `free` is declared `cstr`, and the header
      says `void *` — C does not promise to leave it alone
      at decl_only.hero:2:5
      2 |     function free(p: cstr)

**The caret is on the declaration and the exit is 1 with no call in the file.**
So: the **compiler-engineer is right**, the **ffi-pragmatist's correction 1 is
false**, and the shared brief is true but silent about the stage. The
ffi-pragmatist's own experiment-4 row (*"a lease into `free(p: cstr)` →
`ffi_writable_parameter` — build exit 1"*) is right; its correction contradicts
its own table.

**A consequence neither seat drew, and it is worth more than the correction.**
`check` is what an editor runs, and it is exit 0 on this. It is also exit 0 on
`tests/golden/unsupported/fixedbugs-pointer-reached-through-a-field.hero` — I
ran it, `check` **0**, `build` **1** with
`unsupported[pointer_element]`. So at least two of the pointer/FFI refusals this
sitting reasons about are **build-time, not check-time**. Any sentence of the
form *"the checker refuses X"* about FFI needs the stage measured before it is
written, and the synthesis should carry that as a rule rather than as a
correction to one line.

**And it revises the ffi seat's defect B.** Its ground is that
`ffi_writable_parameter`'s note (*"`s.cstr()` lends the string's own bytes …"*)
is *"false for a lease"*. The diagnostic is sited on the **declaration**, where
no lease and no call exists yet — `decl_only.hero` proves it fires with neither.
The note is not false for a lease; it is a note that cannot know what will be
passed, which is a different defect with a different repair.

---

## 3. A ROUTE NOBODY LISTED

Both seats searched and both concluded there is none. The engineer: *"no third
route exists at the allocation layer, and that is provable rather than
unsearched."* That sentence is probably true **and it is about the allocation
layer**, which the ffi seat had already shown is the wrong layer. The ffi seat
reframed once — *the property is not the allocation base, it is the allocation
heap*. Asked what would have to be true for a third route to exist, I reframed
again and went looking at the layers neither seat was assigned.

### 3.1 The language already owns a retention vocabulary, and both sittings priced it as if it did not

`spec/heroes-spec.md:375-386` and the grammar at `:394-398`:

    Member = "function" ident "(" [ CParam {"," CParam} ] ")"
               [ "->" Type [ "owned" ident ] [ "acquires" ident | "borrows" ] ] NEWLINE
    CParam = [ "@" ] ident ":" Type [ "counted_by" ident ] [ "owned" ident ]
             [ "consumes" | "acquires" ident | "borrows" ] .

**Four marks already parse on an FFI parameter** — `counted_by`, `owned <freer>`,
`consumes`, `acquires <ender>` / `borrows`. Three facts follow and no seat in
either sitting states one of them:

- **`borrows` is a retention statement that already ships.** The spec's own
  words: *"`borrows` says the call hands back one it keeps."* Panel 167 concluded
  *"no declaration-site mark can express retention at all"*. The language has one
  for handles. Whether it can be made to reach a `ptr`/`cstr` parameter is a
  question nobody asked; it is not *"nothing can decide retention"*, it is *"the
  thing that decides it was built for handles"*.
- **The run-time instrument the engineer says cannot exist already exists.**
  `spec:381-383`: *"The live handles are a **set**, so giving one back twice
  aborts on its own."* Panel 168's engineer dismissed the pointer-keyed side
  table (*"`hero_handle_slot` … would answer yes, mine"*). That is a correct
  objection to the table answering *is this block still the program's* and no
  objection at all to the table this project already ships, which answers *is
  this pointer's life over*. The sitting says there is **no instrument** for 066;
  there is a live-set-with-abort in the tree, wired to `acquires`, and nobody
  checked whether a lend or a lease can enter it.
- **§4.19's third reserved case is `owned` read backwards, and the allocator
  question answers itself.** `docs/design/design.md:2332-2337` reserves *"a
  buffer that C takes ownership of"*; `owned sqlite3_free` already spells *"C
  made this, you free it, with that function"*. The mirror spells *"Heroes made
  this, C frees it, with that function"* — and the moment the group names the
  freer, the emitter knows **whose heap** to allocate from, which is exactly the
  ffi seat's experiment 2. Both sittings framed the third case as an **allocation
  layout** question. It is a **declaration** question whose allocation is a
  consequence. The ffi seat's prediction 3 says the buffer *"will come from the
  library's allocator"*; the mark is how the compiler is told which one.

### 3.2 Defect 068's corrupting action is a Heroes statement, and nobody treated it as one

This is the reframe that matters, and it is measurable in the reproducer.

The sitting classifies 066 and 068 together as *foreign retention*, where panel
167's historian proved static enforcement does not exist in any of ten
ecosystems. That is true of 066. **It is not true of 068.** In `alias.hero` the
wrong answer is produced by line 13:

    12    k_register(p: s.name.ptr(), n: 8)
    13    print(k_read_later())          # 72
    14    s @ KSlot(name: [1, 2, …], id: 1)    <-- the write that corrupts
    15    print(k_read_later())          # 1

Line 14 is a **Heroes assignment, two lines below the lend, in the same
function, to the very binding whose field's address was lent.** No C code
participates in the corruption; C only observes it. That is caller-side, which
is precisely where the historian found that static enforcement *does* exist
everywhere.

The checker already has the machinery to see this shape: `field_lend_written`
lives at `selfhost/emit/ffi_lend.hero:74`, and `field_lend_escapes` /
`field_lend_needs_a_place` at `selfhost/check/lending.hero:218-228` already
relate a lend to its root binding. A conservative rule — *a binding whose field's
address has been lent in this function is not re-assigned in this function* —
needs no retention knowledge, no flow analysis beyond the lend's own parent map,
and refuses `alias.hero` exactly. It over-approximates, which is the safe
direction, and `.claude/rules/module-shape.md`'s *ask the value, never the world*
is satisfied: it asks about a binding in hand.

I am not proposing it as a resolution — I give no verdict. I am naming that
**068 was never searched at the layer where its defect actually happens**, and
that the sitting's ground for believing it unsolvable (*"nothing decides this one
from a declaration, a header or an argument"*, DEFECTS.md:95-97) is an argument
about the C side of a line the Heroes side writes.

### 3.3 A group-level mark is not route B, and it is priced here for the first time

Panel 167 refused route B partly on a ceiling: a second contextual word cannot
be parsed without touching `selfhost/parse/members.hero`, which I re-measure at
**298** in `suite_layout.hero`'s own unit against a ceiling of 300.

A mark on the **`extern` group head line** — *this library may retain what it is
given*, over-approximating, refusing the lend for the whole group and admitting
only a lease or a C-owned buffer — is a different route, is not refuted by panel
167's run-time-`bool` program (it decides nothing per call), and **lands in
different files**. Measured with an awk replica of `suite_layout.hero:526-545`,
validated against the engineer's four anchors exactly
(`selfhost/ast.hero` 524, `selfhost/print/fmt.hero` 1174,
`selfhost/parse/members.hero` 298, `selfhost/check/builtins.hero` 378):

| file a group mark enters | code lines | ceiling | headroom |
|---|---|---|---|
| `selfhost/parse/group.hero` | 235 | 300 | **65** |
| `selfhost/keywords.hero` | 231 | 300 | 69 |
| `selfhost/check/lending.hero` | 293 | 300 | 7 |
| `selfhost/lend_errors.hero` | 277 | 300 | 23 |
| `selfhost/ast.hero` | 524 | **525** `DECIDED` | **1** |
| `selfhost/print/fmt.hero` | 1174 | **1175** `DECIDED` | **1** |

**It dodges the file route B died on and hits two others at one line each.**
Honestly stated: a bool on the group node is at least a declaration and a
constructor argument in `ast.hero`, so it does not fit in 1. The route exists, it
is not the one that was vetoed, and its price is two ceilings rather than one.

### 3.4 And a ceiling veto against a robustness route inverts § Precedence

Named because it decides how §3.3 and route B should be read, and it comes from
the contract rather than from me. CLAUDE.md § Precedence:

> 3. **Robustness** (design.md §1.12) … a goal of the language, so it beats
>    elegance, token cost, ergonomics, **compiler size** and **speed** (CL-012).
> …
> 6. **Conventions** (§ 11).

The ~300-line module ceiling is § 11, rank 6. A route that closes a memory-
corruption class is rank 3. **Panel 167 refused route B on rank 6 against rank
3**, and CL-012 names *compiler size* in the list robustness beats. A split is
what `.claude/rules/module-shape.md` prescribes and nobody priced one. I take no
position on whether B should have been adopted; I name that the ground it was
refused on is the ground the contract says loses.

### 3.5 One route I looked for and refuted

**Copy-in / copy-out at the lend site** — the emitter allocates a temporary,
copies the field in, passes the temporary, copies back after the call. It keeps
the write direction that `tests/golden/run/ffi-a-byte-field-crosses-to-c.hero`
depends on, and it needs no surface at all. It does **not** close 066: the
temporary dies with the same frame the field did. This is reasoning, not a
measurement, and I mark it as such.

---

## 4. WHAT A SEAT SWALLOWED FROM THE SHARED BRIEF

The brief is unusually honest — it corrects itself twice in its own text — and
both seats re-ran its three measurements. What got through is a **scope
sentence**, not a number.

### 4.1 The scope sentence, swallowed by both

> *"Clause 2 …, clause 3 … and the refusals of routes B, C and D are **not** in
> front of this sitting."*

Both seats accepted it. The engineer then found clause 2's blast radius anyway
and filed it under *"What I could not settle"* with the words *"it belongs to
clause 2, which is not in front of this sitting"*. The ffi seat went further: its
resolution point 5 **votes** — *"Clause 2 stays"* — on a re-run of its premise
alone (that the two rules do not exist for `ptr`), without measuring anything the
widening would refuse. That is a vote cast on a question the same report accepts
it was not given.

The cost of the sentence is §1: the scoping excluded the defects the milestone
must close, so two seats spent a full sitting on the give-away case — which the
ffi seat then proved **the language cannot express at all** (its experiment 4,
five gates, all run) — and zero measurements on 066 and 068.

### 4.2 Numbers: the brief's three greps are line counts presented as use counts

Brief: *"`grep -rn` over `examples/ tests/ selfhost/`, `*.hero` only: `.lease()`
**28**, `.cstr()` **126**, `.ptr()` **65**"*. The ffi seat reports *"The
whole-tree counts reproduce exactly."* They do — `grep -rn … | wc -l` gives
28/126/65 on my copy. But `grep -rn` counts **lines**, and the occurrence counts
are **29 / 136 / 67** (`grep -rho --include='*.hero' -- '<pat>' examples tests
selfhost | wc -l`). The `.cstr()` gap is ten. Nothing in either verdict turns on
it; the corpus-size argument in §1.5 above does, so I state both rulers.

### 4.3 What I checked and found sound

Re-read rather than trusted, because the sitting rests on them:
`runtime/heroes_runtime.h:158-160` (the header and `HERO_HELD_MAGIC`);
`runtime/parts/str.c:419-423` (`hero_str_held`, `sizeof(HeroHeldHeader) + s.len + 1`);
`:459-466` (the subtraction and the compiler-blaming panic);
`runtime/parts/alloc.c:349-358` (`hero_live_held`);
`selfhost/check/leasing.hero:25-30` (the module doc's *no flow analysis*, quoted
correctly) and `is_lease_cell` at `:62`. `wc -l` on the family reproduces the
brief's six numbers exactly. **All sound.**

**And the engineer's veto ground reproduces on a probe I wrote myself**, three
runs of three, `clang -std=gnu11 -Wall -Wextra -Wpedantic -Werror`: with a
trailing header, after C frees the block, the magic word survives at **n=65536**
and **n=1048576** and does not below, so the release's check would pass and it
would free a block it no longer owns. Independently confirmed.

---

## 5. IS CLAUSE 2 STILL WHAT MAKES ROUTE A SOUND?

Panel 167's ground: *"Without them route A is not sound, and that is measured
rather than argued."* I ran the four programs that ground implies.

### 5.1 The unsafe shapes clause 2 aims at are already refused, by a different rule

| program | today |
|---|---|
| `function give(s: KSlot) -> ptr` / `return s.name.ptr()` | **check exit 1**, `error[field_lend_escapes]` |
| a lend parked in a variant case payload, `.live(p: s.name.ptr())` | **check exit 1**, `field_lend_escapes` |
| a lease parked in `[cstr]` | **check exit 1**, `error[lease_escapes]` |
| a lease parked in `{i64: cstr}` | **check exit 1**, `lease_escapes` |
| a lease parked in a variant case payload | **check exit 1**, `lease_escapes` |

The **position rule** catches every one of them, at check, including the shape
beside defect 067's — so 067's repair holds at the shapes next door (CL-061), and
I ran that rather than assuming it. Clause 2's two type rules are a backstop
behind a rule that is already closed. **The claim that route A is unsound without
them is not demonstrated by any program I could write.**

### 5.2 What clause 2 would newly refuse is safe

| program | today | under clause 2 |
|---|---|---|
| `function mk_block() -> ptr` / `return malloc(size: 8)` | **check 0, run 0** | refused |
| `record Arena { base: ptr, n: i64 }` holding a `malloc`'d block | **check 0, run 0** | refused |

Neither has a lend, a lease, a frame or a borrowed byte in it. A `cstr` is a
pointer **into somebody else's bytes**; a `ptr` from `malloc` is a block **the
program owns**. Widening a rule written for the first onto the second refuses the
ordinary way to bind any opaque-handle library. CLAUDE.md §12: *a refusal is held
to the same standard as a feature — a design.md Part 6 row must name the program
or compiler fact that would make it wrong* (CL-005). No such row is drafted.

### 5.3 And the rule is bypassed by one line, which the compiler itself recommends

    record CThing tag thing        # inside the group
    record Box                     # outside any group
        h: CThing
    ./heroes check handle_wrap.hero  -> 0 ; ./heroes run -> 0

A tagged handle **is** a pointer, and a Heroes record may hold one today. Under
the literal reading of clause 2 (*a field spelled `ptr`*) it still may. So the
rule stops a spelling, not a pointer — and the line that bypasses it is the line
`unsupported[pointer_element]`'s own note tells the author to write: *"hold the
pointer in a `record`"*, quoted in `examples/ledger/db/sqlite.hero:201-203`.

### 5.4 The blast radius, enumerated from the world with its command

The engineer's grep (5 record fields typed `ptr`, 23 `-> ptr`) counts hits
**inside** `extern` groups, which clause 2 exempts. Classified by an
indentation-aware walk of every `.hero` file under
`examples tests selfhost spec docs site tools harness editors`
(`python3 clause2.py`, source in § Appendix), cross-checked against
`grep -rn --include='*.hero' -E '^[[:space:]]+@?[a-z_][a-z0-9_]*:[[:space:]]*ptr[[:space:]]*$'`:

| clause 2's two rules, across the whole tree | count | where |
|---|---|---|
| Heroes function answering `ptr` (outside a group) | **0** | — |
| record **outside** a group with a `ptr` field | **1** | `tests/golden/unsupported/fixedbugs-pointer-reached-through-a-field.hero:33` |
| variant case payload outside a group with a `ptr` field | **1** | `tests/golden/unsupported/fixedbugs-pointer-reached-through-a-payload.hero:17` |
| the other three raw `ptr` field hits | 3 | all **inside** `extern` groups — `tests/golden/run/fixedbugs-a-nested-record-and-a-typed-pointer.hero:46,47`, `tests/golden/check/ffi-record-field.hero:23` — exempt |
| Heroes records **outside** a group holding a **tagged handle** | **3** | `examples/ledger/db/sqlite.hero:215,218`, `tests/golden/check/ffi-handle-refusals.hero:43` |

Four things the sitting owes that nobody has named:

1. **Whether a variant case payload counts.** I ran the `cstr` twins: `record Box
   { c: cstr }` is `check` **1** (`cstr_in_a_record`), but a **variant** case
   payload holding `cstr` is `check` **0**, and so are `record Box { cs: [cstr] }`
   and `record Box { c: cstr? }`. **The two rules clause 2 widens are not
   transitive.** Widened faithfully, the `ptr` versions inherit three holes;
   widened transitively, they refuse a second golden and stop being the same rule.
2. **Whether a tagged handle counts** (§5.3). If yes, `examples/ledger/db/sqlite.hero`
   stops compiling — a shipped example whose own comment says the language forced
   that idiom on it. If no, the rule is one line from useless.
3. **The two fixtures it does hit are both in `tests/golden/unsupported/`, and
   both rest today on `pointer_element` at BUILD** (measured in §2). Clause 2
   would refuse them earlier, with a different code, so both `#~` annotations and
   both `.expected` files move. Per `.claude/rules/verification.md`, *a change to
   what the checker REFUSES is judged by every golden tree*: `check` `run`
   `emission` `determinism` `corpus`, plus `unsupported` `annotations`
   `canonical`. Unpriced.
4. **`selfhost/lend_errors.hero` is 277 of 300** (my replica; the engineer's
   number, reproduced). Two `ptr` diagnostics with message, notes and fix are the
   ~20-30 lines he estimates. He is the only one who has said this out loud and it
   belongs in the synthesis, not in a seat's appendix.

---

## 6. Two smaller things

- **The ffi seat asks the coordinator to copy four scratchpad C files into
  `docs/panel/168-briefs/`.** That request is in a report, not in the resolution,
  and a resolution is what gets executed. If the sitting wants `allocator.c` — the
  file carrying the strongest measurement in either report, the replaced SQLite
  allocator — it has to say so in the resolution. Otherwise experiment 2 survives
  only as prose about a file nobody can re-run.
- **The ffi seat's defect A, independently reproduced here** and with its weight
  re-read. A C function name passed to a function-pointer parameter:
  `./heroes check cb.hero` **0**, `./heroes build` **0**,
  `./heroes run` **134**, `panic: entered unreachable code — this is a compiler
  bug, please report it`. In the emitted C, `hero_unreachable(); /* the gate
  refuses this form */` at line 124 and then `(void)cb_take(t3, t4, t5);` at 128
  with `t4` never assigned. The seat files it as a **diagnostics** defect. It is
  also the reason §3.1's route is not available today: **a destructor callback is
  the one mechanism every real C library offers for exactly the retention problem
  066 and 068 are about** — `SQLITE_TRANSIENT`, `sqlite3_free`,
  `curl_easy_setopt`'s write callbacks — and Heroes cannot pass one. That makes it
  load-bearing for this milestone rather than incidental to it.

---

## 7. What I could not settle, and why

- **Darwin arm64 only.** The magic-survival sizes, the exit codes and the ASan
  lines. `.claude/rules/platforms.md`: a platform fact run on one platform is an
  inference about the other two. Unrun on Linux and Windows.
- **Clause 2 is not built**, so §1.2 and §5.2 are readings of the rule text
  against programs I ran, not runs of the rule. Said in place.
- **I did not run any suite.** Every number here is from `heroes check`,
  `heroes build`, `heroes run`, `clang`, `grep`, `wc` and an awk replica validated
  against four independent anchors. The blast radius in §5.4 is therefore an
  enumeration, not a suite result.
- **Whether `borrows` or `consumes` can be made to reach a `ptr`/`cstr`
  parameter** (§3.1). I read the grammar and the spec text; I did not try to
  compile a binding that uses one on a non-handle type. It goes out as a question,
  and what I searched was `spec/heroes-spec.md:360-398` and
  `docs/design/design.md:2328-2345`.
- **Whether the conservative 068 rule in §3.2 breaks anything in the corpus.**
  Unrun. `.ptr()` has 0 occurrences in `examples/` (panel 167's critic, re-read
  not re-run), so the risk is in `tests/golden/`, and I did not enumerate the
  lend-then-assign shape.

---

## Appendix — every probe, in full, because a scratchpad path dies

`k.h`

    #include <stdint.h>
    typedef struct { unsigned char name[8]; int64_t id; } KSlot;
    static const unsigned char *g_kept = 0;
    static int64_t g_n = 0;
    static inline void k_register(const void *p, int64_t n) { g_kept = (const unsigned char *)p; g_n = n; }
    static inline int64_t k_read_later(void) { return g_kept ? (int64_t)g_kept[0] : -1; }

`k2.h`

    #include <stdint.h>
    static const char *g_s = 0;
    static inline void k_keep(const char *s) { g_s = s; }
    static inline int64_t k_first(void) { return g_s ? (int64_t)(unsigned char)g_s[0] : -1; }

`alias.hero` — defect 068

    extern "k.h"
        record KSlot
            name: u8[8]
            id: i64
        function k_register(p: ptr counted_by n, n: i64)
        function k_read_later() -> i64

    function main()
        s: KSlot @ KSlot(name: [72, 2, 3, 4, 5, 6, 7, 8], id: 1)
        k_register(p: s.name.ptr(), n: 8)
        print(k_read_later())
        s @ KSlot(name: [1, 2, 3, 4, 5, 6, 7, 8], id: 1)
        print(k_read_later())

`escape.hero` — defect 066: same group, then

    function lend_and_return()
        s: KSlot @ KSlot(name: [72, 2, 3, 4, 5, 6, 7, 8], id: 1)
        k_register(p: s.name.ptr(), n: 8)

    function main()
        lend_and_return()
        print(k_read_later())

`lease_direct.hero` — §1.3, the one that decides this report

    extern "k2.h"
        function k_keep(s: cstr)
        function k_first() -> i64

    function main()
        x: str @ f"Hello-payload-{7}"
        c: cstr @ x.lease()
        k_keep(s: c)
        end_lease(@c)
        print(k_first())

`decl_only.hero` — §2

    extern "stdlib.h"
        function free(p: cstr)

    function main()
        print(1)

`ret_malloc.hero` / `rec_malloc.hero` — §5.2

    extern "stdlib.h"
        function malloc(size: u64) -> ptr
        function free(p: ptr)

    function mk_block() -> ptr
        return malloc(size: 8)

    function main()
        b: ptr @ mk_block()
        free(p: b)
        print(1)

    # and, second file:
    record Arena
        base: ptr
        n: i64

    function main()
        a: Arena @ Arena(base: malloc(size: 8), n: 8)
        free(p: a.base)
        print(a.n)

`handle_wrap.hero` + `h.h` — §5.3

    /* h.h */ typedef struct thing thing;
    static inline thing *thing_open(void) { return (thing *)malloc(8); }
    static inline void thing_close(thing *t) { free(t); }

    extern "h.h"
        record CThing tag thing
        function thing_open() -> CThing
        function thing_close(t: CThing)

    record Box
        h: CThing

    function main()
        b: Box @ Box(h: thing_open())
        thing_close(t: b.h)
        print(1)

`cb.hero` + `cb.h` — §6

    /* cb.h */ typedef void (*cb_t)(void *);
    static void cb_free_it(void *p) { printf("C called the destructor\n"); (void)p; }
    static inline void cb_take(const void *p, cb_t d, int64_t n) { (void)n; d((void *)(uintptr_t)p); }

    extern "cb.h"
        function cb_free_it(p: ptr)
        function cb_take(p: ptr counted_by n, d: (function(ptr) -> ()), n: i64)

    extern "stdlib.h"
        function malloc(size: u64) -> ptr

    function main()
        b: ptr @ malloc(size: 8)
        cb_take(p: b, d: cb_free_it, n: 8)
        print(1)

`clause2.py` — §5.4, the enumerator. Walks every `.hero` file; a line at column 0
opens or closes an `extern` group; inside a `record` block it matches
`<name>: ptr` / `: cstr` and attributes it to the group or to the file; a
`function … -> ptr` line is attributed the same way.

`sizes.c` — §4.3, the trailing-header magic-survival probe: `trail_hold(n)` with
the brief's `pad()`, then `free(p)`, then read the word at `p + pad(n)` through
`memcpy` and report whether it still equals `HEROHELD`.

The `code_lines` replica — §3.3, awk over `suite_layout.hero:526-545`: count
non-blank lines, skipping from a line beginning `test "` until the next non-blank
unindented non-comment line.
