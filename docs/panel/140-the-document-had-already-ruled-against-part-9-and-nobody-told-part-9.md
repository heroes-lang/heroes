# Panel 140 — the `raw` module: the document had already ruled against Part 9, in Part 9's own document, and nobody told Part 9

**Sat** 2026-09-13 · **milestone** M-deferral-ledger, step 5 · **status**
`provisional — author ratification pending`

**Lane: full five seats plus a completeness critic.** The gate CLAUDE.md § 4 asks
once per milestone was given at step 1.

## The item, and the Part it points at

`design.md:2948` is one line: *"**A `raw` module for low-level access** — see
Part 9."* Part 9 (`:3287-3316`) is the substance: Oberon put every unsafe
operation in a pseudo-module `SYSTEM`, Modula-3 in `UNSAFE`, Rust in `unsafe`,
Zig in `@ptrCast`, and *"the principle: **low-level access is a marked, bounded
region, not a scattering of features**"*. It shows `use raw` then
`raw.load_u32(b, off)`, and says **"Everything that breaks the guarantees goes in
`raw`: pointers, load/store at offsets, casts. Outside it, the language is
unchanged."** It closes: *"Note this requires modules (deferred item 4) and, for
anything serious, sized integers (deferred item 10)."*

## The measurement that ends the sitting, and it is a sentence in the same document

**design.md already ruled against Part 9's premise, and Part 9 was never told.**
The FFI seat found it:

- `design.md:1547`, §4.10: *"Two exceptions, named rather than discovered later:
  **§4.19's `ptr`/`cstr` sits outside the guarantee**"*.
- `design.md:3306`, Part 9: *"Everything that breaks the guarantees goes in
  `raw`: **pointers**, load/store at offsets, casts."*

Same document, opposite rulings on the same type, and §1.11 and §4.19 say which
one won: **`ptr` is a § 3 first-class type usable in any file**, because a
language whose founding constraint is that everything comes from C cannot put
pointers behind an import. Part 9 describes a design this language could not
take, and has said so elsewhere in its own pages since §4.10 was written.

## The other three facts, measured before the briefs went out and re-run by the seats

**1. Both stated preconditions landed on 2026-08-12 and nothing woke.** Modules
at M-module-namespace, sized integers at M-sized-integers. **The fourth item of
this ledger whose condition expired in silence** — the same shape panel 136 struck
on doctests.

**2. The escapes are eleven, in two files** — `ptr` in a real type position
outside any `extern` group, in programs, both of them SQLite bindings — against
**56** extern declarations mentioning `ptr` across **37** files. Reproduced
independently by two seats.

**3. The name is already a binding** in `selfhost/main.hero:79` and
`selfhost/ir/mono.hero:108`. The engineer priced it and it is the weakest of the
four: `use` is per-file, neither file says `use raw`, so **zero renames are owed**,
and the compiler already answers the collision with `error[shadowed_binding]` and
a note offering `use raw as <other>`. The sitting records that it should carry no
weight.

## Verdicts

| seat | verdict | what it measured |
|---|---|---|
| `compiler-engineer` | **veto against ENTERS**; stands for **REFUSED** with a Part 6 row | **`use X` resolves to a file on disk**, and the tree has exactly **one** synthesised module, the library — *unqualified and unconditional*, where a `raw` must be qualified and conditional on a use line, a third kind the resolver has never had: ~100 lines across seven sites, **five of them at or over their stop-on-growth ceiling**. And the existing gate bites: a qualified extern across a module boundary is refused (panel 033 R5), so `raw`'s operations **cannot be externs — they must be builtins, and a builtin needs no module**, at **3 files** measured on `chars`. **Move and addition are conflated by the item**: `ptr` exists (a move), load/store at offsets and casts **do not exist at all** (an addition, core by §1.7's own test). Subtraction: **zero** |
| `ffi-pragmatist` | **refuse** (Part 6 row), with a **standing veto** on any form that removes `ptr`/`cstr` from an `extern` signature | the §4.10-against-Part-9 contradiction above. And it compiled Part 9 at its word: since Part 9 offers load/store on byte arrays rather than an opaque handle type, handles become `i64` slots through a hand-written shim — which builds at exit 0 and, with the handle swapped, **exits 139**, *"now unfixable: both are i64"*. Part 9's own spelling would make defect 029 **worse** |
| `llm-ergonomist` | **veto** | reading only the specification: the decisive pair is two `read_u16_le`, identical in intent, where the `[u8]` version **aborts** at § 10 and `raw.load_u8` would return a number — so the region does not add danger to a safe language, it **removes a guarantee that exists**. Its honest counter-question found the one real gap and says the proposal does not fix it: a call to an `extern` with scalar parameters is **textually identical** to a call to a Heroes function. And, unprompted, it reproduced defect 030 from the text alone: *"I wrote raw code and caught myself assuming `b = a` copies a pointee"* |
| `spec-warden` | **veto on ENTERS**; *"REFUSED is the honest verdict"* | §1.0's burden unmet and measured; and it asks for what the sitting adopts below — **a dated correction beneath Part 9**, since *"leaving it uncorrected while deferring the item is the worst of both: the document keeps promising a boundary the compiler does not have"* |
| `historian` | **object** (advisory), to the item as written and not to low-level access as a goal | — |

## What the critic found, and it changed the resolution twice

**A route nobody listed, and it is already in the compiler.** Heroes bounds a
dangerous type today **by a per-declaration diagnostic rather than by an import
line**: `error[cstr_in_a_record]` (`selfhost/ffi_errors.hero:281`) confines
`cstr`, and `unsupported[pointer_element]` (`selfhost/emit/gate.hero:255`)
confines `ptr` inside containers — with no module, no `use` line, no spec section,
and **unrenameable**, where `use raw as x` would let a file rename the mark away.
Generalising that gives the bounded region Part 9 asks for, **derived by the
checker rather than declared by the author**.

**And it catches the shape every file-level mark misses.**
`tests/golden/surface-fixtures/qualifyptr/window.hero:9` is
`function destroy(handle: ptr) -> i64` in a module with **no `extern` group** —
verified by the coordinator — which falsifies the FFI seat's *"the `extern` group
already covers that set"* exactly there.

**A contradiction between two seats, checkable.** The warden wrote that all three
open defects occur *"inside a file that already carries an `extern` group and
would therefore already carry `use raw`"*; the engineer compiled 031 and found
its dangerous line is `b: Holder @ a` **in `main`**, with the record type escaping
the moment anyone writes it elsewhere. Same verdict, opposite premises, and the
warden's *100% redundancy* figure collapses if the engineer is right.

**A cost nobody priced, and it ties this sitting to panel 139.** `raw` was the
only proposed design under which spec § 3's *"No aliasing exists anywhere"* could
have stayed **unconditionally true**, by quarantining the exception. Refusing it
**commits the specification to an in-text exception** — which is exactly the
sentence panel 139 queued the same night.

**And a process finding about the coordinator, not the seats.** Two of the four
framing facts *"are quoted by nobody and no brief exists on disk"*, so they are
unverifiable from the sitting's own artifacts. The briefs went out as prompts and
were never written down where a later reader could check what the seats were
told. **That is a defect in how this panel is convened**, and it is recorded here
rather than in a list, because the repair is one line in `/panel`: write the brief
to the sitting's directory before the seats start.

## The resolution adopted, provisionally

**Item 11 is REFUSED to Part 6, Part 9 is corrected at the Part rather than at one
sentence, and the form that returns is the one already in the compiler.**

1. **Item 11 leaves Part 7 for Part 6**, with the falsifier the engineer and the
   FFI seat converged on: *a program on the §1.0 closure list, or a §4.19 ladder
   binding, needing a load, a store or a cast that **cannot** be written as a
   declaration-level builtin — so that the import line does work no annotation can
   do.* Three vetoes stand against ENTERS and no seat supports it.
2. **Part 9 gets a dated correction beneath it, and it is owed at the Part.** Its
   thesis — *"low-level access is a marked, bounded region"* — is what all four
   seats falsified for this language; its preconditions expired on 2026-08-12; and
   its *"everything that breaks the guarantees goes in `raw`: pointers"*
   contradicts §4.10's *"`ptr`/`cstr` sits outside the guarantee"* in the same
   document. **design.md has no token budget, so this half is free**, and leaving
   it is *"the worst of both"*.
3. **The form that returns is named and it is not a module**: the per-declaration
   confinement the compiler already performs twice, generalised — a mark the
   checker derives, that no `use ... as` can rename away, and that reaches a
   `ptr` in a file with no `extern` group. **Its home is M-handle-verdict**, whose
   three defects are the three things `ptr` does not carry, and this sitting hands
   that milestone a **narrowed option set** rather than a clean bill: refusing the
   file-level region leaves it the declaration-level and type-level routes only.
4. **What this sitting does not decide, and says so**: whether load/store at
   offsets or casts should ever exist — the tree contains **zero** of either, so
   there is nothing to bound; and anything belonging to M-handle-verdict.

**What conservative would have been**: defer with the builtin route as the return
condition. The sitting refuses because three vetoes are engaged, because Part 9's
premise is contradicted inside its own document, and because a Part 6 row carries
a falsifier a reader can check.

## Author's verdict

**Ratified 2026-09-13 as adopted, nothing changed** — by the author, together with the other seven sittings of M-deferral-ledger the evening the milestone closed, after each was verified against the repository: design.md carries the amendment, `records` is green, every citation resolves. The record is `docs/records/done/2026-09-13-11NN-decided-2026-09-13-author-…` for this sitting; what the yes settles and what it leaves to a later sitting is written there and in the sitting's own resolution above. The return conditions stand exactly as written.

## Predictions to score

| prediction | instrument | scored at |
|---|---|---|
| engineer: the repair closing 029 and 031 touches **zero** lines of the five module-resolution files, and its diagnostic is keyed on a type or a declaration, never on a use line | `git diff --stat` against the tag | M-handle-verdict |
| engineer: any Part 9 operation ever wanted lands as a Tier-1 builtin touching exactly 3 files, as `chars` does | `git diff --stat` | whichever milestone adds one |
| critic: splitting 031's reproducer so the record and the misuse live in a module with **no** `extern` group still builds clean — which makes the warden's *100% redundancy* false | `heroes build` | any later sitting on the family |
| the falsifier's own clock: a closure-list program needing a load, a store or a cast no declaration-level builtin can express | a sitting that proposes one | every later sitting of this ledger |

## What the seats could not source or could not run

The engineer's resolver cost is an estimate with no spike behind it, and its
*"three for three"* on the defects rests on one compiled case, the other two being
inherited and argued. The FFI seat's denominator for its headline ratio is
unverifiable — the critic reproduced 121 files declaring an `extern` and measured
**725** `.hero` files rather than the 683 quoted — and its experiment B measures a
shim it wrote itself. The warden's drafts are vendored-only with the `real` row
**unrun**, and its collision probe used a proxy name rather than `raw`. The reader
ran nothing, as its seat requires, and the critic ran the half that could be run,
confirming it. Two of the coordinator's four framing facts are unverifiable from
the sitting's artifacts. And no seat ran `heroes mutate` or `heroes check
--permissive`, the two Part 11 instruments that ship in the binary.
