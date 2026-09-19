# Panel 167 — nobody checks the callee, and the lease we would copy is open

Convened 2026-09-20, at M-declared-extents, on **defect 066**: a `ptr` lend has
no lifetime rule, so C may keep the address past the frame. Panel 166 filed it
and declined to price it — *"its two routes are the shape three seats vetoed for
the write direction, and pricing them is its own sitting."* This is that sitting.

Five seats, a completeness critic, six briefs written to disk before any seat
started, and the working tree frozen from the briefs going out until this file
was written.

## The resolution, in one line

**Route A is ADOPTED — a field lease that COPIES, spelled as a second use of
`.lease()` so it costs no new surface, allocated so the pointer C receives is
the allocation base, and emitted `const` so it does not re-open what route H
closed. Two type rules that `cstr` has and `ptr` does not are widened, because
the critic measured that without them the lease is not sound. Route C is REFUSED
on two vetoes, route D on one, route B is not adopted on a veto whose stated
ground turned out to be false and whose real ground is a ceiling. And the
sitting found that the `cstr` lease we were about to copy is itself open at exit
0 — filed as defect 067 — and that defect 066 is two defects, the second of
which no sanitizer can ever see — filed as defect 068.**

## The verdict table

| route | compiler-engineer | ffi-pragmatist | spec-warden | llm-ergonomist | historian |
|---|---|---|---|---|---|
| **A** the field lease | object, **approve on one condition** | **approve**, with a flaw found | object — refuted by its own twin | object — unwritable as drafted | **approve**, best-attested in the survey |
| **B** a `keeps` mark | **VETO**, on the ceiling | object — design.md already ruled | **VETO**, 0 of 71 | **approve** | approve **as a companion only** |
| **C** write the hole down | object | **VETO** — leaves no instrument | approve, +13 absorbing | **VETO**, non-locality | object — the survey's one clean negative |
| **D** withdraw the lend | object | **VETO** — 28 + 7 unbindable | object, −80 | — | object — no precedent |

**No budget veto.** The warden said so before anything else: ceiling 10240,
baseline re-measured twice at **8154 real**, FFI floor 60, **2026 net free**, and
the dearest route on the ballot lands 1966 short.

**Principle 0, from the tree**: `.ptr()` is not on the closure list. The engineer
opened all 30 hits in `selfhost/` — every one is diagnostic text, a comment or a
test fixture. **The compiler never lends a field.** So every route here is judged
on design.md §1.12 and Part 11, never on compiler-need.

## The finding that decides the sitting, and it is the historian's

**No language in the survey enforces foreign retention statically. Not one.** Ten
ecosystems. The static enforcement that exists — Ada's accessibility levels,
Rust's lifetimes, Swift's `@escaping` and `~Escapable`, Hylo's second-class
references, Cyclone's regions — is always on the **caller's** side and stops at
the foreign boundary by construction, because the callee is not in the type
system. cgo's own designer wrote it down in 2015: *"It is still possible to
violate the invariant on the C side. There is little we can do about this."* Java
reached the same conclusion in 2024 and is restricting JNI. The two attempts to
push further are dead (Cyclone) and seven years in preview with its design
document retired for drift (D's DIP 1000).

**What survived instead is a triple**: a call-length lend, a lease with an owed
release, and a run-time instrument. **Nine lease pairs, 1997 to 2023, across
eight ecosystems, zero withdrawn.** Route A is the single most replicated
construct in the survey.

## Four things the sitting measured that no document in it knew

### 1. There is no instrument at all — and the sanitizer actively lies

The shared brief said `--sanitize` catches the escape. **It catches it only
because the coordinator's retainer was a `static inline` in a header.** The
ffi-pragmatist bound `sqlite3_bind_blob` against the real `sqlite3.h` and linked
the real `libsqlite3.dylib`:

```
plain build     -> AAAAAAAAAAAAAAAA   (honest: 0A141E28323C4650), exit 0, five runs of five
--sanitize      -> grep -c AddressSanitizer = 0, exit 0, and it prints the CORRECT value
```

The critic re-ran it and put it more sharply: **the sanitizer does not merely
stay silent, it masks the bug.** So route C's *the program is on its own* leaves
the program with nothing — which is the ground of that seat's veto, and it is
the ground the coordinator's own brief got wrong.

### 2. Retention is decided per CALL, and no route can reach it

One declaration, one dead frame, argument five the only difference:
`SQLITE_TRANSIENT` gives the honest bytes, `SQLITE_STATIC` gives garbage. Thirteen
destructor-sentinel APIs against ten unconditional retainers in one header, and
`curl_easy_setopt` is variadic so a mark has nowhere to land.

**And the argument-site route two seats reached independently — the warden's
`retained_by d`, the historian's MPI placement — dies on one program the critic
wrote**: the discriminator can be a local assigned in a branch, so one call site
prints 10 or 72 by a run-time `bool`, both check-clean. A mark naming the
deciding argument cannot decide anything the argument decides at run time.

**Retention is invisible to the toolchain too.** Four clang attribute families
checked across 3120 SDK headers: `lifetimebound` in 0 files, `noescape` in 11 and
every one on a block parameter, `__counted_by` expands to nothing. `sqlite3.h`
and `raylib.h`: zero of all four. Route H worked because the header carried
`const`; there is no counterpart here.

### 3. THE LEASE WE WERE ABOUT TO COPY IS ITSELF OPEN — defect 067

Three seats proposed replicating the `cstr` lease for fields. The critic asked
whether the thing being replicated is sound, which no seat did, and it is not:

```
a lease pointer copied into an `extern` group's record survives `end_lease`,
is handed to C, and reads freed bytes
  check exit 0 · run prints 0 where 72 is honest · --sanitize: heap-use-after-free
```

Clause 1 permits a lease name *"as an argument of a call"*, and **a record
constructor is a call**. The other four escape shapes are correctly refused; this
one door is open, and `cstr_in_a_record`'s own note blesses it — *"a group's
`record` may hold a `cstr`, because there the fields are the header's and C owns
the bytes"* — which is true of a header's field and **false of a lease**.

### 4. WHAT ACTUALLY HOLDS THE `cstr` LEASE SOUND IS TWO RULES `ptr` DOES NOT HAVE

The critic found the axis nobody measured, and it decides the shape of route A.
The `cstr` lease is held by the position rule **plus** two type rules from panel
122: no Heroes function may answer `cstr`, and no record outside a group may hold
one. For `ptr` neither exists — `record Box { p: ptr }` and `function give() ->
ptr` **both check clean today**.

So *"spell the field lease as a second use of `.lease()` and it costs no
surface"* and *"the field lease is sound because the `cstr` one is"* are two
different claims, and only the first is true. **The cheap spelling and the sound
spelling are not the same spelling**, and the difference is those two clauses.

## The disagreements, stated plainly

**On route B the two compiling seats disagreed with the ergonomist, and the
critic falsified the engineer's stated ground.** The engineer vetoed B partly
because *"nothing reaches a `ptr` parameter today except `nullptr` and a lend, so
`keeps` is a refusal with no route out"* — §1.12's completeness clause. The
ergonomist, reading only the spec, wrote the route out and could not compile it.
**The critic compiled it**: `malloc` + `memcpy` + the lend reaches a `ptr`
parameter, checks clean, prints the honest 72, zero ASan reports. Neither seat
had read the other's report. **The veto's completeness half is false; its ceiling
half stands**, and that is what B is not adopted on.

**On route C the warden stood alone.** It approved an absorbing form at **+13
real**, cheaper than the +20 joined and +32 standalone because the free-standing
position clause becomes a consequence of the lifetime rule. The critic confirmed
+13 to the digest, `8154 → 8167`, `44fa44bb11d45b03`. It is refused anyway: two
vetoes, one on non-locality (under C, whether a line is correct or a
use-after-free is decided by a comment in a C header, and the `extern`
declaration is byte-identical either way) and one on the instrument (there is
none). **A veto is a refusal rather than a price**, so +13 does not redeem it.

**On route A the seats split four ways and were describing one construct on
different axes**: the engineer's spelling axis (no new surface), the
pragmatist's allocation axis (the leading header traps the give-away case,
SIGABRT exit 134, fixed for free by a trailing header since a field's length is
a compile-time constant), the ergonomist's escape axis (the pin's name cannot
leave the function that made it), and the warden's efficacy axis (the `cstr`
lease shipped and its lend still corrupts). All four are true at once.

## The three ratchets, and what this milestone's own work cost the next one

The engineer's veto ground, reproduced exactly by the critic in the unit
`suite_layout.hero` counts:

| file | code lines | ceiling |
|---|---|---|
| `selfhost/parse/members.hero` | 298 | 300 |
| `selfhost/ast.hero` | 524 | 525 |
| `selfhost/print/fmt.hero` | 1174 | 1175 |

Two code lines turns two of them red. **Route C of panel 166 — `counted_by`,
landed this afternoon — touched all three**, and no other route on this ballot
touches a ratcheted file. A second contextual word cannot be parsed without a
split or three raised ceilings, which nobody priced.

And the number the engineer gives the project rather than the sitting: the
lend/lease family went **689 → 2255 lines** in one milestone while all of
`selfhost/` grew 1675. **93.5% of the compiler's growth bought this one
feature.**

## Defect 066 is two defects

The critic separated them and only the first is what the brief described:

- **the frame dies and C reads it later** — exit 0, wrong answer, and ASan sees
  it only when the retainer is a header inline;
- **the record is rewritten while C still holds its field's address** — a wrong
  answer at exit 0 with **zero** ASan reports, and **no sanitizer can ever see
  it**, because nothing is freed and no frame dies. Filed as **defect 068**.

**And routes H and C of panel 166 narrowed 066 by zero**, run rather than
argued.

## The resolution — `provisional — author ratification pending`

Per CLAUDE.md § 4, the most robust and complete resolution, never the cheapest
and never a compromise.

1. **Route A is ADOPTED**, in the one shape that survives all four axes: spelled
   as a second use of `.lease()`/`end_lease` so it costs **no parser, no AST, no
   formatter, no IR and no ownership pass**; allocated with a **trailing** header
   so the pointer C receives is the allocation base, which also closes
   design.md §4.19's third reserved case — *a buffer that C takes ownership of*,
   unbuilt since it was reserved; and emitted **`const` from a non-`@` root**,
   reusing `field_lend.const_lends`, so it does not re-open what route H closed
   nineteen hours before this sitting.
2. **The two type rules are widened from `cstr` to `ptr`** — no Heroes function
   answers a lent pointer type, no record outside a group holds one. **Without
   them route A is not sound**, and that is measured rather than argued.
3. **Defect 067 is filed and repaired FIRST**: a lease name reaching a group
   record's constructor escapes its own release. Route A rests on this
   mechanism, so the mechanism is repaired before it is extended. The repair is
   in `cstr_in_a_record`'s exemption, whose note states a premise that is true of
   a header's field and false of a lease.
4. **Route B is NOT adopted.** Two vetoes. Its completeness ground is false and
   its ceiling ground stands. **What would lift it**: a split of the three
   ratcheted files, priced, plus a rule deciding retention that survives the
   critic's run-time-`bool` program. Neither exists today.
5. **Route C is REFUSED on two vetoes**, on locality and on the absence of any
   instrument. Not recorded in design.md Part 6; what is recorded is the
   historian's reason — **every ecosystem that wrote this rule as prose alone
   came back later to add a mechanism**, four for four.
6. **Route D is REFUSED on the ffi seat's veto**, 28 `const void *` declarations
   in `sqlite3.h` and 7 in `raylib.h` unbindable without shims. **But the veto
   was measured on a tree without route A**, where nothing but a lend reaches
   such a parameter. Once A lands, a lease reaches it too, and D's ground is
   untested. **That is the next sitting's question and it is named here rather
   than left to be rediscovered**: with the lease shipped, does withdrawing the
   field lend close 066 by construction at a cost the corpus can pay?
7. **Defect 068 is filed**: the live record rewritten under C's held address,
   which no sanitizer can see.
8. **Defect 066 stays open** until A lands with clauses 2 and 3, and it is
   corrected in place to say what the sitting measured: it is a **lend** defect
   rather than a field-lend defect — the `cstr` lend has the identical hole, 59
   occurrences against 33, ten of them in four shipped examples against zero.

**What a veto compels.** C and D stay refused whatever the author decides among
the rest, and B stays unadopted until its ceiling ground is paid.

**What conservative would have been, so the author can choose it:** route C's
absorbing sentence alone, **+13 real**, which documents the hole, closes nothing,
and leaves a corruption reachable from unmarked ordinary-looking code with no
instrument anywhere. The warden's own draft, and the cheapest thing on the
table.

## Predictions to score, at the M-declared-extents close

- **compiler-engineer**: route A spelled as a `.lease()` overload lands with
  **zero lines** under `selfhost/parse/`, `selfhost/ast.hero` and
  `selfhost/print/`, and `heroes fmt` returns the new program byte-identical.
- **ffi-pragmatist**: under the trailing-header allocation, `sqlite3_bind_blob`
  needs no shim for all three retention modes — `SQLITE_TRANSIENT`,
  `SQLITE_STATIC` and a real destructor — and each prints `0A141E28323C4650`
  under `-Wall -Wextra -Wpedantic -Werror`, plain and under ASan. The
  counter-prediction is already run: under a leading header the third mode
  aborts, exit 134.
- **spec-warden**: the absorbing form of C ratifies at **8167 real**; above +32
  it owes a fresh removal. And routes A, B and C each close **zero of two**
  reproductions when landed alone.
- **llm-ergonomist**: under a `keeps` mark, **0 of 10** first tries that lend
  into a keeping function compile, and at least 6 of 10 take the C-owned-buffer
  route or get a diagnostic naming the mark; under route C, the silent-error rate
  moves **fewer than 2 of 10**.
- **historian**: if Heroes ships route C alone, within four milestones a
  mechanism for foreign retention is proposed again, by a seat other than the
  historian, because a program needed it rather than because a panel was
  reviewing the spec.

## Also recorded, so it is not rediscovered

- **The copy costs nothing anybody will notice.** Lend 0.0 ns; lease 21.4 ns at
  8 bytes and 25.9 ns at 4096 — the 512× costs 4.5 ns, so it is `malloc` and not
  `memcpy`. The denominator that settles it: `SQLITE_TRANSIENT`, **SQLite's own
  copy of the same bytes, 15.1 ns.** The library already charges for a copy and
  expects you to take it.
- **`examples/ledger/db/sqlite.hero:19-23` prevents this exact hazard with an
  eleven-line comment** — prose doing a compile error's job, in the project whose
  thesis is that it never should.
- **A precedent for aborting at exit with a count, in a shipping language's FFI,
  the historian could not find**, having searched JNI, .NET, Go, Haskell, OCaml,
  Erlang and Nim. Heroes' lease already does it, so route A extends a local
  precedent rather than importing one.
- **The coordinator's own brief was wrong twice**, both caught by seats: the
  sanitizer claim above, and a file-size table that ranked the files the lend
  already lives in rather than the files a mark would enter — which is why one
  seat found the only ceiling veto in the sitting and four did not.
- **Unrun, and named**: whether the trailing header's alignment padding differs
  off darwin arm64 (Linux and Windows); and whether any route other than a lend
  or a lease reaches a `ptr` parameter, which one seat did not search
  exhaustively.

## Author's verdict

**Pending.** Queued as `panel 167` in `docs/work/DECIDE.md`, and the adopted
resolution is the default while it stands there: the field lease is built,
defect 067 is repaired first because the lease rests on its mechanism, and
defects 066 and 068 stay open until it lands.

**What a yes settles**: that a lend gains a sibling that COPIES rather than a
rule that decides, since the sitting measured that nothing can decide retention
— not the header (0 of 71, and zero of four clang attribute families across 3120
SDK headers), not the declaration (retention is per call, proven on one
signature), and not the argument (a run-time `bool` picks it, proven on one
program). It also settles that the two type rules holding the `cstr` lease sound
are widened to `ptr`, which is what makes route A sound rather than merely
cheap.

**What it does not settle, and both are named in the resolution rather than left
to be rediscovered**: whether withdrawing the field lend once the lease exists
closes 066 by construction — the ffi seat's veto on that was measured on a tree
where nothing but a lend reaches a `const void *` parameter, which route A
changes — and what answers defect 068, which no sanitizer can see and no
declaration can reach.

**What a no leaves standing**: the conservative resolution is route C's
absorbing sentence alone, +13 real, recorded above in full. Taking it overrides
two vetoes rather than choosing a cheaper price, and three seats independently
found that every ecosystem which wrote this rule as prose alone came back later
for a mechanism.
