# Panel 162 — the conversion already existed, and what was missing was the extent

**2026-09-18. M-readable-bytes. Full panel, five seats plus the completeness
critic.** Briefs at `docs/panel/162-briefs/`, reports at
`docs/panel/162-reports/`, all committed with this file.

## The proposal, verbatim as it went to the seats

> **What turns a run of bytes into a `str`, and what lets a program build a
> fixed-array field without writing every element?**
>
> Both halves block one program, which is why they are one sitting.

The program was `uname()`: bind `struct utsname`, print the machine's name.

## What the sitting found before it found anything else

**Three of the shared brief's premises were measured false, by three different
seats.** They are listed first because they shrink the question:

- **"`spec § 13` forbids a record holding a `cstr`."** False. The
  compiler-engineer measured it is a *checker* rule binding records **outside**
  a group (`selfhost/check/lending.hero:200`), and `lending.hero:282` asserts a
  `cstr` field **inside** a group gives zero diagnostics. Nothing had to move.
- **"A `char *` field cannot be read."** False. The spec-warden measured
  `p.pw_name.validated()` at exit 0. **The one-way door is `char[N]` alone.**
- **"The program cannot be written."** False. The compiler-engineer generated
  the 256-element literal, ran it, and printed `Darwin`.

**So the gap is smaller and sharper than the milestone file claimed**: not *bytes
cannot become text*, but **a fixed array cannot say where it ends**.

## The measurement that decided the failure mode

`read_file` on invalid UTF-8 answers **`.err` with `code == "not_text"`** —
measured, `printf 'abc\xff\xfe\x80def'`, verified with `xxd`, three files through
`./heroes run`. No abort, no replacement character, no lossy decode, no
truncation. Decided at `runtime/parts/os.c:259-263`, whose own comment states the
doctrine: *"the repair is a pre-check at the caller, never a weaker
conversion."*

**So the rule was already chosen and the sitting had only to obey it.** That
settles the historian's condition 1, which warned that if Heroes' existing
answer were Go's unchecked one, consistency would argue the opposite way. It is
not Go's.

And **the conversion is already built**: `hero_str_try_from_cstr`,
`runtime/parts/str.c:299-316`, panel 089. Line 304 is `size_t n = strlen(p);` —
**that one line is the entire difference between what exists and what this
sitting wanted.** A `str` already carries a length as well as a NUL
(`runtime/heroes_runtime.h:73-76`), so a byte run of known extent is already the
right shape.

## The verdict table

| seat | verdict | section | cost / delta, measured | prediction | condition or veto |
|---|---|---|---|---|---|
| **llm-ergonomist** | **approve** route 1, **veto** inside it on route 2 | spec § 13, with § 3, § 10, § 11 | not its unit | today first-try correct ≈ 0%, over half invent a name absent from § 11, 10-30% write the decimal loop; with `validated()` ≥70% correct on the printing half | veto on a `cstr` view of a field: the line sends a reader to three places, one of them a memory-safety question |
| **compiler-engineer** | **object** | design.md §1.7, Part 5 | **~75-110 lines, ZERO lowering lines, no new type** — a built-in dispatches generically off `inventory.table()` (`ir/flatten.hero:523-544`) | no new IR node kind and `emit/descriptors.hero` untouched, under 150 lines | **veto on a slice TYPE**; approve if the resolution names a **built-in** rather than a type, **states the inbound byte's sign**, and says in writing that the question is the **extent** of a fixed-array field |
| **ffi-pragmatist** | **object**, with a veto | design.md §1.11, §4.19 | 16 headers, 712 fields, **50** `char[N]`: 21 not text, 14 fixed-width with no terminator, 2 length-in-a-sibling, **13 reliably NUL-terminated — 26%** | rung 3 of §4.19's ladder needs no C shim under a slice-shaped rule | **veto on any resolution that changes how a `char[N]` field is DECLARED**; six conditions for approval |
| **spec-warden** | **approve**, and only merged into § 13 | design.md §1.6, §1.2, §4.9 | baseline **7984**; one § 11 name **+5**; route 1 as § 11 text **+99**; read merged into § 13 **+45**; build via `repeat` **+18**; both plus a named removal **+57**. **Merging beats appending by 54 real tokens** | `--refresh` reads ≤ 8060 at the close; `examples/` gains a reading program ≤ 300 bytes against 4234 today | object if the build half lands as a default value (§4.9); **object to any wording asserting `char[N]` is NUL-terminated** |
| **historian** (advisory) | **approve**, with one correction | precedent | — | — | the sitting is choosing **two** failure modes, not one |
| **completeness critic** | — | — | — | — | — |

## The disagreements, and the counting the critic did

**1. The convergence did not carry the sitting, and it is countable.** The
ffi-pragmatist listed **six** conditions for moving from `object` to `approve`;
the resolution four seats were converging on met **three**. Unmet: the caller
cannot supply the extent, the element sign is unstated, and the field is still
**unpassable to C**.

**2. That third one is the sitting's largest single measurement and only one
report carries it.** `function strlen(s: cstr) -> u64` called as
`strlen(u.sysname)` is `error[type_mismatch]: expected cstr, found i8[256]`.
**Heroes gives a fixed array no array-to-pointer decay, so the field is not only
unreadable — it is unpassable**, and no Heroes-side shim can route around it.

**3. The historian corrected the brief's framing, and it holds.** The sitting is
choosing **two** things, not one: **termination** (where does the run stop) and
**validity** (are the bytes text). Precedent agrees on validity — Rust and Python
fallible from the start, Swift shipped lossy and **added** a failable initializer
in 6.0, and no case was found of the reverse move — and **disagrees on
termination**: Ada raises, Go and Zig truncate silently, Rust errors.

**4. A seat carrying a burden closed off a payment route on a false premise.**
The spec-warden reported that no first-try-rate or mutation instrument exists, so
the ergonomist's predictions name instruments that do not. The critic ran it:
**`heroes mutate` exists**, and `design.md:319-321` lists it and Part 11 metric 3
**by name** among the instruments a registered prediction may be paid with, metric
3 having run four times. The registered-prediction payment was available and was
declined for a −6 removal.

**5. Two predictions are unscoreable as written.** Rung 3 of §4.19's ladder is
**already expressible** — `sqlite3_column_text -> cstr` against
`const unsigned char *` builds, measured — because it is a pointer and was never
on this wall. And the ffi-pragmatist's sharpest prediction, that a terminator-only
rule over-reads `utmpx.ut_id[4]`, is **falsified by the adopted wording**, which
is not terminator-only.

**6. The ergonomist's hinge was discharged by measurement.** Its objection
depended on whether the silently-wrong program compiles. It does:
`out @ out + u.sysname[i].to_str()` builds at exit 0 and prints
`6897114119105110` for `Darwin`. **So refusing is defeated by design.md Part 6's
own standard** — a refusal must name the program fact that would make it wrong,
and the fact is that a wrong program already compiles and a refusal cannot reach
it, because `to_str` on a byte is legitimate.

## The resolution — `provisional — author ratification pending`

**Adopted: three existing built-ins are widened, and no name, type or spec
section is added.**

1. **`slice(from:, to:)` reaches a fixed-array field of bytes.** It already
   exists (`spec § 11`, `selfhost/inventory.hero:75`), already carries the
   `from:`/`to:` labels, and is refused on a fixed field today with
   `bad_operand`. **This is the critic's finding and it is what makes the
   resolution work**: it is how the caller supplies the extent — `d_namlen`,
   `sun_len`, `sqlite3_column_bytes` — which is the ffi-pragmatist's unmet
   condition, and **it adds no type**, so the compiler-engineer's veto, which is
   against a slice *type*, does not reach it.

2. **`validated()` reaches a byte run**, answering **`str?`** and failing with
   **`not_text`** — the code `read_file` already returns, measured. It reads
   **to its first zero, or whole**: that is the spec-warden's phrasing and it is
   load-bearing, because the ffi-pragmatist measured that only 26% of `char[N]`
   fields are reliably NUL-terminated, so a terminator-only rule would over-read
   three-quarters of the population.

   **Corrected at implementation, 2026-09-18, and the correction is written here
   rather than quietly applied.** This resolution said *three existing built-ins
   widen*. `validated` is **not** a built-in: it is a library function taking a
   `cstr` (`selfhost/library_source.hero:192`), and `spec § 9` gives the language
   **no overloading**, so `validated(cstr)` and `validated(i8[N])` cannot both
   exist as written. The sitting's own framing was wrong about one of its three
   names and no seat caught it, because no seat was asked to implement.

   **What the correction does not change is the name**, which is the
   llm-ergonomist's finding and the reason to keep it: `validated` is the name a
   reader reaches for. So `validated` **moves from the library into the built-in
   table** and answers both a `cstr` and a fixed byte array. Measured before
   choosing it: `spec/offered` is one-directional and `spec/named` asks only that
   a reserved name appear in some code span, which `c.validated()` in § 13 already
   does — so the move adds nothing to § 11's `Built-ins:` sentence and the
   spec-warden's +45 merge stands.

3. **`repeat(x, n)` builds a fixed array**, which closes the build wall while
   keeping design.md §4.9's *no default values* intact — the spec-warden
   measured that a zero default contradicts §4.9 and that widening `repeat`
   does not, at **+18 real** against a larger bill.

4. **Both `i8[N]` and `u8[N]` are accepted, and the resolution says so**, which
   is the compiler-engineer's condition. The header decides the sign and the
   binding author cannot, which M-arm-platform measured the hard way one day
   earlier; and the historian found Go paying for exactly this, its own toolchain
   still reaching for `unsafe.Pointer` in 2026 because the frozen half of its
   standard library could not change an element type.

**Why this and not the four routes the brief named.** A ninth name in § 11 costs
+99 real against +45 merged into § 13, and the sitting measured that **merging
beats appending by 54 tokens**. A `cstr` view of a field is vetoed twice over —
by the ergonomist for sending a reader to three places, and by the
ffi-pragmatist because 37 of 50 fields have no terminator, so it turns correct
bindings into out-of-bounds reads. Refusing is defeated by Part 6's own standard,
above. And a slice *type* is vetoed by the compiler-engineer against 24 fields in
six headers and zero in `examples/`.

**What conservative would have been, recorded so the author can choose it**
(CL-040): `validated()` alone, without widening `slice`. It is conservative
because it closes the motivating program with one widening instead of three. It
was not taken because it leaves the ffi-pragmatist's measured population
unserved — the 2 length-in-a-sibling fields and every future one — and the seat
that measured that population is the seat CLAUDE.md § 12 says the C boundary
belongs to.

**What this resolution does NOT close, named with its trigger.** A fixed-array
field is still **unpassable to C**: `strlen(u.sysname)` stays `type_mismatch`,
because Heroes gives a fixed array no array-to-pointer decay. That is a separate
deliverable — handing a field TO C rather than reading it — and it is filed as
**defect 061** rather than absorbed here, because no route in this sitting was
priced against it.

## Predictions to score

| # | seat | prediction | checkable at |
|---|---|---|---|
| 1 | compiler-engineer | no new IR node kind, `emit/descriptors.hero` untouched, under 150 lines total | M-readable-bytes close, by `git diff --stat` |
| 2 | spec-warden | `--refresh` reads **≤ 8060** real at the close | M-readable-bytes close |
| 3 | spec-warden | a `char[N]`-reading program in `examples/` is **≤ 300 bytes** against the 4234 of today's literal route | M-readable-bytes close |
| 4 | llm-ergonomist | with `validated()`, ≥70% of samples correct on the printing half | **payable on `heroes mutate` and Part 11 metric 3, which exist** — the critic measured that, correcting the seat that said they do not |
| 5 | ffi-pragmatist | rung 3 of §4.19's ladder needs no C shim | **lapsed by measurement**: already expressible, never on this wall |
| 6 | ffi-pragmatist | a terminator-only rule over-reads `utmpx.ut_id[4]` | **lapsed by resolution**: the adopted wording is not terminator-only |
| 7 | historian | no language moves from fallible to lossy at this boundary | standing; falsifiable by one counterexample |

## What this sitting found and did not resolve

- **Defect 061**, filed: a fixed-array field is unpassable to C, `strlen(u.sysname)`
  → `type_mismatch`, and no Heroes-side shim can route around it.
- **Defect 062**, filed: `ffi_incomplete_record` names a field that IS present —
  a single `cstr` field in `extern record Utsname` reports *does not name
  nodename* with `nodename` on the line above.
- **One rule would have had two homes.** `repeat` is a § 11 built-in and the
  resolution writes its widened meaning into § 13. `.claude/rules/spec-shape.md`
  says every rule has exactly one home; the implementation puts the `repeat`
  sentence in § 11 with a pointer from § 13, and the critic is why.
- **The input discipline was breached again.** The llm-ergonomist reported, for
  the second sitting running, that `.claude/rules/*.md` files arrived in its
  context unrequested — including `spec-shape.md`, which states the very token
  counts that seat exists not to hold. It disclosed the breach and discarded the
  material. **Twice is a pattern and not an accident**, and it belongs to the
  harness rather than to the coordinator.

## Author's verdict

**Ratified 2026-09-18 BY DELEGATION AND NOT BY READING**, and what the
delegation rests on is named exactly rather than stretched: the author's
instruction of this date was **resolve it** — *there is no way to turn a C
`char[65]` field into text, so whoever binds `struct utsname` to print the
machine's name cannot do it. Resolve it.* They did not pronounce on the route,
because the sitting had not yet sat. The preceding instruction of 2026-09-17,
under which panel 161 was ratified, asked for the step finished *with no open
defects and no open decisions, ratifying the panels*, and this sitting is the
continuation of that work.

**So the yes is inferred from an instruction to resolve, and this paragraph says
so in order that the author can overturn it cheaply.** Crediting them with a
reading that did not happen would be the same falsehood wearing the flattering
sign (CL-058).

**What the yes settles.** Three existing built-ins widen — `slice(from:, to:)`
reaches a fixed-array field of bytes, `validated()` answers a `str?` failing
`not_text`, `repeat(x, n)` builds a fixed array. No name, no type and no section
is added. Both `i8[N]` and `u8[N]` are accepted and the text says so.

**What it does NOT settle, each with its trigger:**

- **Defect 061**, filed: a fixed-array field is **unpassable to C** in the other
  direction, `strlen(u.sysname)` → `type_mismatch`. That is handing a field TO
  C rather than reading it, and no route here was priced against it.
- **Defect 062**, filed: `ffi_incomplete_record` names a field the author has
  already written.
- **The conservative route stays available**: `validated()` alone, without
  widening `slice`. It closes the motivating program with one widening instead
  of three, and it leaves the length-in-a-sibling fields unserved.
- **Two predictions lapsed rather than scored**, and the sitting says which:
  the ffi-pragmatist's rung-3 claim, already expressible and never on this wall;
  and its `ut_id[4]` over-read, falsified by the adopted wording rather than by
  a run.

## Queued

`docs/work/DECIDE.md` carried **panel 162** as an open item until this verdict.
