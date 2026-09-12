# 008 — Aggregates: records, variants, arrays, and the only indirection there is

M5c. Tag `m5c`. Panels 022 and 023.

## Goal

Make `record`, `variant` and `[T]` reach C, which means making the memory model
real rather than described. Three things had to become true at once: an
aggregate is **by value**, the per-type functions C cannot write are
**generated**, and a mutation **unshares per step of the place it mutates**.

The measure of success was fixed before the work started, and it is one program:
`examples/gallery/11-trees.hero`, design.md §4.10's own recursive variant, `Expr`
holding `[Expr]`. It compiles, runs, prints 14, and exits with the leak counter
at zero. A tree containing itself, in a language with no `Box`, no `ref` and no
pointer.

**Delivered:** the `no_size` diagnostic class and the topological type order;
which types own a counted reference; records by value; variants as tagged unions;
arrays with the descriptor ABI at `HERO_RUNTIME_ABI 3`; copy-on-write per array
step with write-back. The gallery goes from 3 programs building to 5, and nothing
in it exits 2.

**Not delivered, deliberately:** the read-only map and `T?`'s representation.
Panel 022 struck the map's *mutation* half for having zero reachable call sites,
and its own R4 leaves a larger question open — `{K: V}` may be **deleted from the
language** at M6's closure audit, priced at −57 spec tokens against +29 to fund
`set` plus `for k in m`. Building ~180 lines of C for a container that may not
survive is what `cow_check` was struck for at M5b, and the same argument applies
with more force. The step is named in the ROADMAP with its precondition: the
closure-list answer first.

## What surprised

**Every defect in this milestone was a missing row, never wrong logic.** Six of
them, and the shape repeated so exactly that it is worth stating as a rule: the
tables in this compiler are exhaustive matches, so adding a capability is a
compile error — but adding a capability to a *type* while forgetting it in a
*walk over fields* is not, because the walk already has an arm for "anything
else". Three landed there.

**The catch-all's spelling decided whether a bug was loud or silent.** The field
walk's last arm emits `hero_unreachable()` rather than nothing, so a variant with
a `[Expr]` payload **aborted** on its first run instead of leaking. Had that arm
been `_ => {}`, the same defect would have been a silent leak in the one place no
test looks. The rule "list every arm" is usually justified as making additions
loud; this milestone showed the second half — what the *last* arm does decides
the failure mode of everything the list forgot.

**A question can be right for two milestones and still be the wrong question.**
The ownership pass asked whether the *root slot's* type was counted. For a
whole-slot store that is the same question as the stored value's type, so it was
correct from M5b through step 5. `p.cells @ [7, 8]` is the first program where
the two differ: the slot is a `Row`, counted because it *contains* an array,
while the place written is the `[int]`. One leaked block, and the leak counter
named it in a line.

**Twice, a counting bug printed every correct answer first.** The double-free
after `Pair(one: p, two: q)` and the leak after `p.cells @ [7, 8]` were both
programs that were right for their whole visible life and wrong once, at the
sweep. Nothing about the output could have shown it. That is the argument for an
instrument whose job is the balance rather than the answer.

**One decision looked like performance and was correctness.** `push` copies,
always. `xs = [1,2,3]` leaves `xs` observable and its slot holds one reference —
so a refcount of 1 means "only the slot has it", and appending in place would
change what the slot sees. There is no reading of value semantics in which the
argument may be consumed, so O(n²) construction is not a trade-off here; it is
the only available answer, and §4.10 had already declared the same bill for
`s + t`.

**A rule with an instrument caught the file that wrote the rule.** Panel 022
required that a descriptor's `hash` never be null, because a call through a null
one is `SEGV at pc 0x0` with no type name. `hero_array_new` refuses such a
descriptor — and the first thing it refused was `tools/spike/04-variant.c`, the
spike that had frozen the descriptor ABI in the first place. A check with an
instrument beats a check in a comment, demonstrated on the comment's own author.

**The panel's cheapest option was also its only true one.** Panel 023 priced six
spec wordings for the `no_size` rule. The submitted two-line bullet cost +40 and
was *false* about `{K: V}`; corrected it cost +49 and was still false. What
landed was +6 in the existing `[T]` row — and the asymmetry that decided it had
not been named before: a diagnostic can only teach you at the moment you are
wrong, and can never tell you something is **allowed**, because a legal program
produces no diagnostic. The spec pays for the licence; the diagnostic covers the
prohibition, free.

## What broke and why

| symptom | cause | fix |
|---|---|---|
| `Pair(one: p, two: q)` printed seven correct lines, then double-freed at exit; ASan named `hero_str_decref` with the magic word clobbered | an aggregate constructor captures its fields **by value**, including the `str` handle inside them, so the new value is a second owner — and nothing in the pass added that reference | ownership rule 6: a constructor retains every counted field. `ok(x)` deliberately excluded: wrapping consumes its argument |
| `variant Token { word { text: str } }` copied with `=` and never increfed | `Ty::Named(d)` is the type of a whole declaration and a variant is one of the things it names; the field walk read only `DeclKind::Record` | both kinds, and a variant's cases are alternatives, so any counted case counts |
| whether a type owned a reference depended on **whether the program read the field** | record and variant field types were never interned by the declaration walk — they were lowered lazily, at construction or access. `.word _` ignores the payload, and the `str` inside it disappeared | `types/decls.rs` interns every field type eagerly |
| `variant Expr` with a `[Expr]` payload aborted at `hero_unreachable` on its first run | `Ty::Array` absent from the field walk in `perfn.rs` | the two array arms — and the abort rather than a leak is the catch-all's spelling working |
| `Undefined symbols: _h_11trees_Expr_hash`, at link | a variant's `hash` was prototyped and referenced by its descriptor and never defined | `variant_hash_body`, plus one per case payload |
| three `error: use of undeclared identifier 'h_m_Point_desc'` | descriptors were emitted with the other bodies, after the functions that name them | emitted straight after the prototypes |
| `[[int]]` was `-Wincompatible-pointer-types-discards-qualifiers` | `const {spelling} *` reads as pointer-to-pointer-to-const when the element is itself a pointer | `{spelling} const *`, which says what is meant whatever the element is |
| `p.cells @ [7, 8]` printed the right answer, then `1 heap blocks still live at exit` | rule 3 read the root slot's type instead of the stored value's | the value's type; an index path stays copy-on-write's business |
| `xs[0] @ 7` reached clang as `incompatible integer to pointer conversion`, reported as an internal error with a path to generated C | arrays emitted before element *writes* did, and no row refused the gap | the `array_write` row, which lived exactly one step and retired with the primitive |

The last one is the milestone's own lesson about the gate: a row can be owed
because a capability **arrived**, not only because one is missing. Emitting
arrays created a form the emitter could reach and could not spell, and the
failure mode was the exact one the gate exists to prevent — the compiler telling
the author that the compiler is broken.

## The instruments, ranked by what they caught

- **the leak counter** — two defects, both after every correct answer had printed
- **`hero_unreachable()` in a catch-all** — one, as an abort instead of a leak
- **clang and the linker** — three, all of them ordering or spelling
- **a test written one line after the code** — two (the partial order for a cyclic
  file, and the variant that was never counted)
- **panel judges compiling rather than reasoning** — the whole shape of the COW
  rule, and the `T?` row two of them found independently from opposite ends
- **ASan** — agreed with the leak counter twice and found nothing on its own,
  which is the honest record: on this platform it is the use-after-free
  instrument and the counter is the leak one

## What landed, and what carried forward

Moved verbatim from `docs/ROADMAP.md` on 2026-08-12, when the ROADMAP became a
file about what is next (CLAUDE.md §14). The identifiers are the ones this
milestone was built under.

- **M5c — Aggregates ✅** (2026-08-10, tag `m5c`, panels 022 and 023)**:** records
  and variants **by value** (confirming spike 04), `[T]` through the descriptor pass at
  `HERO_RUNTIME_ABI 3`, structural `==` as a direct `h_T_eq`, and **COW as one unshare
  per array step of the place path** with write-back — the primitives take
  `HeroArrayHeader **`, because a caller cannot forget to store a result that does not
  exist. `Op::CowCheck` did **not** enter the IR: inside the primitive, C's
  argument-evaluation rule makes the hoisted order inexpressible, and the hoistable form
  is what a judge built a three-block cycle from.
  **The acceptance is one program:** `examples/gallery/11-trees.hero`, design.md §4.10's
  own recursive variant — `Expr` holding `[Expr]` — compiles, runs, prints 14, leak
  counter zero. A tree containing itself in a language with no pointer. The gallery goes
  from 3 building to 5, and nothing in it exits 2.
  **The veto ran**: `h = g` then `g.rows[0].cells[0] @ 7` prints 7 and 0, where the
  single-unshare version printed 7 and 7 with ASan clean, the leak counter at zero and
  exit 0 — a green harness on a program violating spec line 60. All three owed `run/`
  cases exist (`adversarial-cow-per-step.hero`).
  New in the front end: the **`no_size`** class (panel 023) with the topological type
  order it shares, and `Checked::counted` answering "does this type own a reference"
  where the AST is.
  **Runnable:** `heroes run examples/gallery/11-trees.hero` · `heroes run
  tests/golden/run/adversarial-cow-per-step.hero` · `heroes build
  tests/golden/emit/aggregates.hero --emit-c` (the descriptors and the tag switch, as
  text) · `heroes check tests/golden/check/no-size-mutual.hero`.

