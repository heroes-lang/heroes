# Panel 146 — does `ptr` keep its name?

**Convened 2026-09-13**, M-handle-verdict's last open item, on the author's own
order: panel 139 reserved the question, panel 145 put it to the author, and the
author **declined both standing options** — keep `ptr`, or rename to
`unsafe_ptr` or a family — and ordered *a sitting of its own, after the form
lands, so that it argues over how many bare `ptr` remain in the tree rather than
over how many it imagines.*

The form landed at steps 4 to 7. The number existed for the first time. Six
seats sat: the five judges and the completeness critic, which has changed the
resolution in every sitting it has run this milestone and changed this one too.

---

## The verdicts

| seat | verdict | veto |
|---|---|---|
| compiler-engineer | **object** to (B) and (C), support **(A)**, and propose **(D)** | none — *"a rename is erased in the frontend, so §1.7 does not arm me, and I say that rather than reach for a rationale"* |
| llm-ergonomist | **(A)** | **scoped veto on the `opaque` family** |
| spec-warden | **veto the rename**, on Principle 0 | budget breach: **not** reached; the veto is on payment |
| ffi-pragmatist | **approve (A)** | **veto on narrowing `ptr` away**, measured |
| historian | **object**, advisory: precedent argues against (B) and points at (C) spelled `opaque` | none |
| completeness critic | the brief's central claim is **false**, and route **(D)** is the robust one | — |

**Four seats for keeping the name, one advisory objection, and a critic who
moved the sitting off the question it was convened on.**

---

## 1. The resolution, adopted `provisional — author ratification pending`

### 1a. `ptr` KEEPS its name

Not because a rename is expensive — it is, 339 changed lines across 119 files by
the engineer's count, but cost is not the ground. The ground is that **no seat
could produce a program that a rename would make correct**, and CLAUDE.md § 12
holds a refusal to a feature's own standard.

Three independent measurements say the same thing from different directions:

- **The ABI is untouched, proven rather than argued.** The ffi seat compiled the
  full survivor binding, emitted its C, renamed every `ptr` spelling in that C to
  `unsafe_ptr`, and compiled both: `70f7152b018fd60e…` **both times,
  byte-identical**, with the only textual difference a macro name and a
  `_Static_assert` message. The rename was done in the emitted C and not in the
  Heroes source **because it cannot be done in the source** — `heroes check` on
  a binding spelled `unsafe_ptr` is `error[unknown_type]` at the first type
  position, this compiler having no such type. The review made that correction
  and let the conclusion stand: a name is front-end text and reaches nothing.
- **The experiment, not the opinion.** The ergonomist wrote the sqlite binding
  twice and the stdio binding twice from the specification alone, then respelled
  them. *"The diff between program 04 and its B and C respellings is one word per
  type position. The load-bearing line is unchanged in all three."* The wrong-object
  call `sqlite3_close(stmt)` type-checks identically under every spelling. What
  DOES change the program is `record Db tag sqlite3` — the form that landed at
  step 4, not a name.
- **Principle 0 is not reached.** `selfhost/` writes **zero** bare `ptr`, so the
  compiler-need limb is silent, and no measured Part 11 effect exists for the
  thesis limb. §1.7's subtraction is **zero**: a rename moves nothing from core to
  sugar and removes no special case. The warden's veto rests there and not on the
  ceiling, which it measured and cleared: **+10 vendored** for either candidate
  against **326** free after the FFI floor.

**The falsifier, so this refusal expires like any other.** The engineer wrote it
and the sitting adopts it verbatim: *a compiled program in which a bare `ptr`
reaches a position **not inside and not adjacent to an `extern` block***. Panel
140 refused a `raw` module on the measured ground that *"a mark every binding
carries carries no information"*, and this sitting's census says every live
`ptr` in `examples/` sits inside an `extern` group or within three lines of the
parameter it feeds. Produce one that does not, and the warning word has
somewhere to carry information and this row reopens.

**And the critic already found the shape of the counter-example**, so it is
written here rather than waiting to be discovered:
`tests/golden/surface-fixtures/qualifyptr/main.hero:4` holds `window: ptr @ nullptr`
in a **six-line** program (five non-blank) with **no `extern` group at all** —
the sitting said four and the file has been six since 2026-09-03, so the number
was never true. It is a fixture rather
than a program anybody runs, so it does not fire the falsifier today — but it
proves `ptr` is not an FFI-only type, which every argument in the brief's § 4
assumed, and the sitting records that rather than letting the assumption stand.

### 1b. The `opaque` family is REFUSED BY NAME, on two independent grounds

The historian's survey converges on it — Zig `anyopaque`, Terra `&opaque`, Odin
`rawptr`, Swift's `Raw` — and the warden priced it at **+0 on the binding
vendored maximum**, cheaper than every other candidate. It is refused anyway,
and both reasons were measured at this sitting:

- **The ergonomist's scoped veto.** § 13 already spends that exact word on the
  handle: *"C's pointer to what the header leaves opaque"*, and § 3 line 58 reads
  *"an opaque pointer"*. Naming `ptr` `opaque` makes the wrong construct and the
  right construct homonyms, so `db: opaque` reads as though it already **is** the
  handle — raising the silent-error rate on the one decision the document already
  gets wrong.
- **The ffi seat falsified the premise.** A name meaning *unnameable pointee* is
  false at **6 of the 9** surviving positions, where the header names the type
  perfectly well (`sqlite3_callback`, `sqlite3_destructor_type`, `const char **`,
  `char **`) and only Heroes cannot spell it. And it is wrong in kind at a
  function pointer, which has no pointee at all.

Priced and refused here so no later sitting goes shopping for it without the
refusal attached.

### 1c. (D) NARROWING is adopted, at exactly one position, because that is
### where it is measured

The critic's finding is that **three of the four survivor kinds are nameable
today**, and it compiled all three. The sitting ran every one of them again
rather than accept them, and they do not all survive:

| position | the critic's claim | re-run at this sitting | verdict |
|---|---|---|---|
| `@tail: ptr` → `@tail: cstr` | builds, exit 0 | **builds, runs, `rows: 3` / `longest: 6` byte-identical** | **ADOPTED** |
| `recs: ptr` → `record Recs tag Rectangle` | builds, exit 0 | **builds, and exposed an emitter defect** | adopted as a defect, below |
| `destructor: ptr` → `(function(p: ptr) -> ())` | builds, exit 0 | **the DECLARATION builds; the program does not** | refused, with the reason |
| `error: ptr` → `error: cstr` | not claimed | **refused: `error[ffi_parameter_type]`, `char **` has no name here** | stays `ptr` |

**`@tail` is adopted and it is not a tidying.** The emitted C is the argument:

```c
/* @tail: ptr  */  sqlite3_stmt ** a3, void ** a4)        { … (a0,a1,a2,a3,(void *)a4); }
/* @tail: cstr */  sqlite3_stmt ** a3, const char ** a4)  { … (a0,a1,a2,a3,a4); }
```

The `ptr` spelling makes this compiler **insert a cast**; the named spelling
hands clang an exact type and lets it check. That is a robustness difference,
**§ Precedence rank 3**, and it outranks every naming argument at this sitting
by the contract's own order. Two sites, `examples/sqlite/main.hero:56` and
`examples/ledger/db/sqlite.hero:90`.

**And `cstr` is the STRICTER type, which is the opposite of what a reader would
guess and was measured only after the sitting closed.** The adversarial review
put the sharpest possible objection to this narrowing — *it accepts a
heap-use-after-free that `@tail: ptr` refused at compile time* — and the
refutation ran three ways. The use-after-free is **byte-identical under both
spellings**, same address, same 37-byte region, same free site: a local `str`
dying while a pointer into it escapes, which neither type sees. The compile-time
refusal the objection rested on was a **missing method**, `.validated()` being
`cstr`-only, caretted at the read rather than at the escape; replace it with a
`ptr`-legal read and the identical program runs at exit 0. And the direction is
inverted by grep: **four compile-time diagnostics are keyed on `cstr`** —
`cstr_escapes`, `cstr_in_a_record`, `cstr_out_of_heroes`, `owned_needs_cstr` —
and **zero on `ptr`**. `record Holder { tail: cstr }` is refused;
`record Holder { tail: ptr }` is exit 0.

**The repository had already blessed this spelling for this exact position, and
nobody at the sitting cited it.** `tests/golden/run/lease-tail-points-into-the-bytes.hero`
was filed by panel 124's ffi seat against `sqlite3_prepare_v2`'s `*pzTail` shape,
names `sqlite3.h:4472` in its own header, and says of itself: *"The tail cell
itself is a plain `cstr` cell, not a lease, which is right."* So the narrowing is
not a new idea and it is not this sitting's invention — it is the shipped
bindings catching up with a golden this repository had held for weeks.

**And that falsifies a seat, not only the coordinator.** The ffi-pragmatist told
this sitting that `@tail` could not be `cstr` — *you cannot hold a `cstr` in a
variable (`error[cstr_escapes]`), so there is nothing to take `@` of*. Run:
`tail: cstr @ nullptr` compiles, the program runs, and the output is
byte-identical. A seat with a veto and a compiler in front of it reported a
refusal that does not happen, and the sitting adopted the narrowing anyway
because the critic had compiled the opposite. **The record says so rather than
quietly keeping the seat's other findings and dropping this one.**

**`destructor` is refused, and the reason is the critic's own fourth category.**
The parameter type is declarable. The program still does not compile, because
the only value for it is `SQLITE_TRANSIENT`, declared `constant SQLITE_TRANSIENT: ptr`,
and `selfhost/check/ffi.hero:175` refuses an `extern constant` of function type
in so many words. So this is **not a type the language cannot spell. It is a
value the language cannot hold** — a distinct reason the brief's three-item list
did not have, living in the constant position the census never opened.

**And the ffi seat's veto stands over all of it**: narrowing `ptr` *away*
entirely is refused, measured. `@tail: cstr owned sqlite3_free` is rejected by
clang — *"passing `char **` to parameter of type `const char **` discards
qualifiers in nested pointer types"* — and would free a pointer into the
caller's own buffer, which §1.12 forbids whatever clang says.

### 1d. Three repairs fall out of the sitting and land with it

1. **An emitter defect, one day old, found by testing the census.** A handle used
   as a **record field** builds at exit 0 and makes clang say *"braces around
   scalar initializer"*, twice, on a correct program. A handle is declared with
   `record` so its type is `.named` and `selfhost/emit/extern_record.hero` braced
   it, but a handle emits as `<tag> *`, a scalar. Measured: **not one
   `record X tag Y` anywhere in the tree was used as a record field**, so the form
   landed at step 4 without ever meeting the position that breaks it. Fixed by
   asking `handles.is_handle` about the field's own declaration, with
   `tests/golden/run/fixedbugs-a-handle-in-a-record-field.hero` — which warns
   under the old compiler and is silent under the new one — and a unit test where
   a handle and a nested record sit in one probe so the two answers are asserted
   against each other.

2. **design.md §4.19's worked examples are stale**, found by the ffi seat. They
   still teach `sqlite3_open(path: cstr, out: ptr)`, `sqlite3_close(db: ptr)` and
   `curl_easy_setopt(handle: ptr, …)` — the exact shapes this milestone replaced,
   in the document that teaches them. Corrected with today's date.

3. **The Part 6 `raw` row's measurement**, and the correction is smaller than the
   brief ordered. The row's *"11 times in 2 files"* reproduces **exactly** at
   `101ab191` and is **2** today, both `tail: ptr @ nullptr` — and after 1c both
   of those go too, so it will be **0**. The row's other half, *"56 extern
   declarations across 37 files"*, the critic could not reproduce under any
   reading and neither could the coordinator: 58/24 by one instrument, 69/98 by
   another. **It is corrected as a question, not as a number**, because a figure
   whose unit was never written down cannot be re-measured — which is CL-017's
   shape and panel 144's own finding one sitting earlier.

---

## 2. The question nobody asked, and this sitting's answer to it

The critic put it, and it is the sharpest thing the sitting produced:

> **Who writes `ptr` now?**

**13 lines across 7 files** of `selfhost/` tell an author to write it, or name
it as the repair — `ffi_errors.hero` ×3, `emit/ffi_pointee.hero` ×2,
`emit/ffi_mutable.hero` ×2, `check/ffi.hero` ×4, `emit/ffi_tag.hero`,
`check/freer.hero` — against **one** site that admits it as a type,
`resolve/vocab.hero:53`. And `selfhost/`, sixty thousand lines written by the
people who know the language best, contains **zero**.

**So `ptr` is no longer a type a programmer reaches for. It is a type the
compiler recommends.** That inverts the sitting: a type nobody chooses does not
need a warning in its name, because the place to put the warning is the sentence
that hands it over.

**Two corrections, both from the review, both to this section's own numbers.**
The count was written as *five diagnostic sites*, and it is a line count taken
with no unit written down. Re-measured: **13 lines across 7 files** in
`selfhost/` whose diagnostic text tells an author to write `ptr` or names it as
the repair — `ffi_errors.hero` ×3, `emit/ffi_pointee.hero` ×2,
`emit/ffi_mutable.hero` ×2, `check/ffi.hero` ×4, `emit/ffi_tag.hero`,
`check/freer.hero`. And the site said to *admit* `ptr` was cited as
`resolve/types.hero:203`, which is inside `type_candidates` — a **did-you-mean
list for an unknown-type diagnostic**, so the citation named a fourteenth
diagnostic rather than the grammar. The admitting site is
**`resolve/vocab.hero:53`**, `if name == "ptr" return ok(.ptr_prim)`, and the
sitting never cited it.

**The shape of the finding survives both corrections and is sharper for them**:
thirteen places where the compiler hands `ptr` over, one place where it admits
it, and zero places in sixty thousand lines where its own authors chose it.

**And a third correction, this one withdrawing a claim rather than repairing
it.** This section said *one of those recommendations was measured wrong today*.
It was not measured at all. Run at the close: for the `@tail` position the
compiler recommends **`@tail: cstr`**, as a `guess` fix, which is the right
answer — and each of the other twelve names a position no handle can reach, a
function pointer three times over, a freer's signature, a C-owned buffer. **The
measured fact that replaces it is better than the claim was**: nobody was told
to write `ptr` at `@tail`. The binding author wrote it against the compiler's
own advice.

**That is the item this sitting queues**, and it is not a naming question:
*are the five recommendations still correct after the handle form?* It is filed
rather than answered, because answering it is a diagnostics change and this
sitting was convened on a name.

---

## 3. The ergonomist's finding, which is the defect the sitting was really about

Read only the specification, and this is what happens:

> *"I wrote `01` before `02` unprompted — I reached for `ptr` for `FILE *` on the
> first pass, from § 3's table, and only found the handle form when I got to § 13
> for the width rules."*

§ 3 is where a writer **chooses** a type. § 13 is three hundred lines later, and
the handle is one clause inside the densest paragraph in the document. Both are
glossed with the same word, *opaque*. There is no sentence routing between them.

The rule is derivable and never written: **a pointer whose pointee the header
names gets a handle; a pointer the header leaves untyped gets `ptr`.**

The ergonomist's proposal is one clause in § 3 — *a pointer the header names
gets a handle (section 13)* — and § 8 already writes *"(section 11)"*, so the
shape exists and is priced like any word. Its registered prediction is that the
clause moves correct-handle usage by **15 points or more** where the rename moves
it by **less than 3 in either direction**.

**Queued, not adopted.** A spec amendment owes §1.6's payment and this sitting
was convened on a name; smuggling a § 3 change through it would be the sitting
deciding something nobody asked. It is filed with the measurement it needs.

---

## 4. What the coordinator got wrong, counted so the arithmetic can be checked

**Seventeen, and the tally itself was wrong three ways until the review caught
it** — this section said *the fourth, fifth and sixth* while enumerating six
bullets, the log entry said *the fourth through ninth*, and the journal said
*nine* over components that add to eleven. So it is written here as a sum a
reader can add up, and it is the last thing in this milestone to be measured:

| where | how many |
|---|---|
| panel 145's brief — the census, wrong four ways | 4 |
| a brief for the scouts — a clause the coordinator reported to the author as the sitting's ratified text, and was not | 1 |
| panel 146's brief — the six below | 6 |
| **panel 146's own file**, after the sitting closed, all six found by the adversarial review | 6 |
| **total** | **17** |

The six in this sitting's brief:

- **§ 6 was false in three parts, and it was the section claiming to bind the
  verdict.** It said a machine-applicable `certain` fix ships the string `ptr`.
  `emit/ffi_pointee.hero:227` is a **note**, not a fix; `:237` and
  `emit/ffi_mutable.hero:126` are both `certainty: .guess`.
  `grep -rn 'certainty: .certain' selfhost/ | grep -i ptr` returns **nothing**.
  Found independently by the engineer and the critic.
- **Two occurrence counts were line counts.** The spec holds **5** occurrences of
  the word, not 7; design.md holds **43**, not 37. Both figures counted lines
  containing the substring, and `nullptr` contains `ptr` — the identical error the
  brief's own § 2c had already confessed and named three lines earlier.
- **The central claim of § 2c was false.** *"A bare `ptr` now means, in every
  place it survives, a pointer whose type this language cannot name."* Three of
  four survivor kinds are nameable, and the critic compiled them. The honest
  statement is the one the sitting adopts: **`ptr` is what is left where nobody
  has migrated, plus two genuine residues** — the callback protocol and the
  sentinel constant.
- **The census glued two denominators.** *"`tests/golden/` — 3 parameters and 6
  record fields"*: the 3 is `run/` only, the 6 is all of `tests/golden/`, and the
  real parameter count across that tree is about 25. CL-057's shape.
- **The warden was handed a generous budget.** 386 free is the pre-mortgage
  figure; `heroes measure` says the FFI floor mortgages 60 of it, so the number is
  **326**. A seat told to veto on budget breach was given an input 60 tokens
  loose.
- **Three positions the census never opened**, all found by the critic: a
  **return type** (`-> ptr`, missed inside the very file the brief cites), a
  **variant payload field**, and a **`ptr` in a file with no `extern` group**.

And the six in this file, found after the sitting closed by an adversarial review
of the whole step, every one of them corrected in place above with what was
measured instead:

- *one of those recommendations was measured wrong today* — **withdrawn**; it was
  never measured, and running all thirteen shows each names a position no handle
  can reach, while the `@tail` position was already being told to write `cstr`;
- *five diagnostic sites* — **13 lines across 7 files**, a line count with no unit
  written down, which is CL-017's shape for the third time in this one document;
- *`resolve/types.hero:203`* as the site that admits `ptr` — it is inside
  `type_candidates`, a **did-you-mean list for a diagnostic**, so the sitting's
  central contrast cited a fourteenth diagnostic against thirteen others. The
  admitting site is `resolve/vocab.hero:53` and was never cited;
- *a four-line program* — **six**, and it has been six since 2026-09-03;
- *compiled the binding twice, once spelled `unsafe_ptr`* — the rename was done in
  the **emitted C**, because `heroes check` on a Heroes binding spelled
  `unsafe_ptr` is `error[unknown_type]`. The conclusion survives, the description
  did not;
- and in the step's own golden, *not one `record X tag Y` anywhere in the tree was
  used as a record field* — **three were**, and the true property is narrower: none
  was a field of a record declared INSIDE an `extern` group, which is the only
  position the completeness probe reaches.

**The pattern across all seventeen is one pattern.** Not one of them is a wrong
verdict; every one is a wrong **number, unit, or citation underneath a verdict
that held**. That is the same finding M-deferral-ledger arrived at nine times one
milestone ago — *an item's stated reason is measured false before its verdict can
be written* — and it has now happened to the coordinator's own prose seventeen
times in two sittings. The instrument that catches it is never rereading. It is
somebody running the command.

---

## 5. Predictions to score

| origin | prediction | instrument | scored at |
|---|---|---|---|
| engineer | if (B) or (C) is ever adopted, the rename lands at ≥ 300 changed lines across ≥ 100 files, and at least one of the **seven unsnapshotted diagnostic strings** ships naming the old spelling with the full net green | `git show --stat`, the net | whenever a rename is attempted |
| engineer | under (A), `swap-ptr`'s kill rate over `examples/` is identical under every candidate spelling, because `mutate/handles.hero:135` matches the WRITTEN type and `score.fate` judges the RESOLVED one | `heroes mutate` | M-handle-verdict close |
| ergonomist | a rename moves correct-handle-usage by **< 3 points**; one § 3 routing clause moves it by **≥ 15**; `opaque` specifically **lowers** it | any harness counting handle-vs-`ptr` choice against a real header. **If no such counter exists, that absence is the panel's answer: the naming question cannot be scored today** | when such a harness exists |
| warden | `unsafe_ptr` or `raw_ptr` across all five spec occurrences reads **exactly 5873** vendored, and the real row lands in **[7816, 7824]** | `heroes measure`, `--refresh` | whenever a rename is attempted |
| warden | §1.2's break-even: at ~13 real tokens per prompt against 500–2000 per correction round trip, a rename pays only if it cuts the `ptr`-related error rate by **0.65 to 2.6 percentage points**. Below that it is a net loss by the formula | the formula | any future naming sitting |
| ffi | on 2026-10-13, `@tail` in the ledger still has no alternative beyond the one adopted today, and `heroes build` on `@tail: cstr owned sqlite3_free` still exits non-zero on clang's nested-qualifier error | `heroes build` | 2026-10-13 |
| historian | if `ptr` keeps its name, the territory the handle form cleared **stays cleared**: at the next `m-*` tag, bare `ptr` in type positions in `examples/` is **≤ 12** and escaping an `extern` group is **≤ 2**. A rise means `ptr` is absorbing cases the handle form should take, and panel 140's *"every binding carries it"* is re-established | the occurrence count, not a line count | the next `m-*` tag |
| critic | the handle-as-record-field position, having had **no golden anywhere in the tree**, is not the last such hole: another position where the handle form meets a shape it has never met will be found before the milestone after next | `docs/work/DEFECTS.md` | the milestone after next |

---

## 6. What this sitting does NOT settle

- **Whether the five diagnostics that recommend `ptr` are still correct.** § 2's
  queued item. Thirteen lines across seven files hand `ptr` over; each one
  measured at this close names a position no handle can reach, so the question
  is not whether any is wrong but whether a **fourteenth** place should now
  recommend the handle where it says nothing.
- **The § 3 routing clause.** Queued with its price unmeasured and its prediction
  registered.
- **Whether `ptr` is an FFI type or a core type.** One four-line fixture says it
  is not FFI-only, and every argument here assumed otherwise.
- **`nullptr`.** The engineer's third condition: a coherent rename would have to
  move the literal too, which is a **lexer and `heroes grammar`** change — the one
  thing §1.7 actually counts — and nobody priced it. If a future sitting reopens
  the name, it reopens as a lexer question and the engineer's veto is live.

## Author's verdict

**Ratified 2026-09-14, in full.** `ptr` keeps its name with the engineer's
falsifier attached; the `opaque` family is refused by name on both measured
grounds; and the `@tail` narrowing stands at both sites, on the emitted C rather
than on anybody's taste.

**What the yes settles**: the name, the two refusals, and the one narrowing —
together with the three repairs that fell out of the sitting and landed with it,
the emitter's handle-field brace, design.md §4.19's two stale fences, and the
Part 6 `raw` row's measurement.

**What it does not settle**, and all three stay queued because a sitting
convened on a name may not decide them: whether the **thirteen lines that
recommend `ptr`** are still right after the handle form, and whether a
fourteenth place should now recommend the handle; the **§ 3 routing clause**,
which is the defect the sitting turned out to be about; and whether **`ptr` is
an FFI type or a core type**.

**The sitting's own lesson, which outlasts its subject.** It was convened to
decide a word and it found a **cast** — `@tail: ptr` making this compiler write
`(void *)a4` where `@tail: cstr` hands clang the exact type. The completeness
critic is why: it refused the brief's central sentence and compiled three
counter-examples, two of which then failed re-running here. **A seat that
attacks the brief rather than the question has now changed the resolution in
every sitting of this milestone**, and the brief it attacked was the
coordinator's own, carrying six errors into a sitting it claimed to bind.
