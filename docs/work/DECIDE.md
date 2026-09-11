# DECIDE — the decisions the compiler is waiting on

Read by **`/decide`**. Every item asks **what should be true**, and until it is
answered the compiler goes on behaving some way by default — so the item names
that default, because it is the cost of leaving the item open.

**Only open items live here.** The moment one is answered it is ticked with the
verdict written into it and moved to `docs/work/DONE.md`, the record. Rank by
what an item blocks, never by age, and verify it against the repository before
putting it to the author: asking a settled question is the one cost this list
cannot pay.

**The shape.** One line per item, and an optional body indented four spaces
under it. The first field is the item's **origin**, and where that origin is a
sitting it is spelled `panel NNN`, padded — because
`tests/harness/suite_records.hero`'s `queued` check reads the `- [ ] ` lines
alone and scans them for exactly that, so a citation that slides into the body
makes every pending sitting report as unqueued, and it fails silently. Nothing
lives outside the two banners: `records/lists` is the executor of that.

Format: `- [ ] **<origin>** | <the question, in one line> | <where to look>`

*******************************************************************************
**OPEN: 2**

- [ ] **panel 131** | Two refusals, a corrected reason, a ratified route withdrawn on its own measurements, and one question deliberately not ruled because the brief put it wrongly | `docs/panel/131-the-refusals-were-sound-and-their-reasons-were-not.md`

    **Origin:** the opening sitting of M-reflection-verdict, 2026-09-11, full
    five seats.

    **What your yes covers, in five parts.** (1) **Run-time reflection is
    refused** — unanimous, two vetoes engaged, and its ground is the measured
    one: a value cannot reach its own type, because a descriptor is emitted per
    TU and only for a type used as a container element, and putting the type in
    the value changes `sizeof`, every `offsetof` and the **register class** of
    every call across the C boundary (raylib's `Vector2`: 8 bytes to 16,
    `offsetof(y)` 4 to 12, floats moving from `s0`/`s1` into `x1`). (2)
    **User-defined tags are refused** — unanimous, and the ground is that such a
    tag in Heroes is **necessarily inert**: no macros, no compile-time
    execution, no closures, no run-time reader, and §10 forbids a plugin. (3)
    **design.md's Ruby row has its reason corrected**, because its clause about
    *runtime dispatch* is false of reflection and Go falsifies it; the
    replacement is emitted per-type metadata and the loss of dead-code
    elimination, which Rust's RFC 0379 gives as its own reason for removing
    reflection in 2014. That clause is true of dispatch by name and untrue of
    reading a type's fields. (4) **Panel 117's sixth option is withdrawn on the three
    measurements that sitting itself asked for** — see the separate item below.
    (5) **Compile-time derivation is NOT ruled**, and is re-sat as step 2 on a
    corrected question.

    **The one part that is a judgement call and not a measurement is (5), and it
    is the one to read first.** Three seats adopted derivation and two objected,
    and reading them together they are answering different questions: the brief's
    drafts render **Heroes' own construction call**, which is display, while the
    precedent and the pressure are about a per-type walk aimed at a **foreign
    notation**. The warden found that a construction-call derive would delete
    **0 of 56** hand-written renderers and the compiler seat **0 of 31**, both
    correct and both fatal to the drafts rather than to derivation. The FFI seat
    wrote `encoding/json` by hand (120 lines, three records), changed
    `f["width"]` to `f["witdh"]`, and got **exit 0, zero diagnostics, and the
    wrong key in the output** — caught only because that program round-trips.
    **Recommended: ratify all five**, and the reason for (5) in one line: ruling
    now would write a Part 6 row about derivation on evidence about display, and
    a refusal whose reason the next reader can disprove is what produced this
    milestone in the first place.

    **What the conservative resolution would have been**, so you can take it
    instead: rule all three today and refuse derivation as well, on the compiler
    seat's Principle 0 measurement — 0 of 31, not needed for self-hosting,
    350-450 code lines across six modules. The milestone then closes in one step
    instead of two, and panel 117's spec clause *"until this language renders
    one"* goes false-by-standing and owes its rewrite immediately.

- [ ] **panel 131** | Panel 117's sixth option, which you ratified on 2026-09-07, does not survive the three measurements it asked for. Withdraw it, or take the records-and-variants-only variant | `docs/panel/131-the-refusals-were-sound-and-their-reasons-were-not.md` · `docs/panel/117-both-sides-of-an-assert-when-a-side-is-an-aggregate.md`

    **Origin:** panel 131, 2026-09-11. It is a separate item from the one above
    because it asks you to reverse something you have already said yes to, and
    that should not ride inside a longer yes.

    **The three owed things were run and two of the four claims failed.** The
    prototype exists and it was built in a scratchpad copy, so `selfhost/emit/differ.hero` is a path this tree has never had (2026-09-11): **160 code lines**, type-
    checking clean in the real module graph — but 160 is a floor: the realistic
    cost is **350-450 code lines across six modules and the runtime**. **The map
    case**: for the yes-or-no answer the walk is order-independent, but two
    `==`-equal left operands against one right operand produce **two different
    messages**, so the leaf it names is a function of insertion history rather
    than of the value — panel 117's own refusal ground for map rendering,
    reproduced inside the thing it adopted instead. **The depth case**: the walk
    reports a leaf to depth 3600 with a path 28,813 characters long, and at 3700
    it is `panic: stack exhausted`, **exit 134** — where today's thin
    `assert failed: a == b` is correct at exit 0 at depth 100,000. **And the
    premise is false at the source**: panel 117 adopted it because *"the walk is
    `eq`'s walk"*, and `HeroEqWork` (`runtime/parts/array.c:185-190`) is
    `{a, b, elem, len}` with no path, no parent and no index base, draining LIFO.

    **Recommended: withdraw it.** The conservative variant is on the record and
    is not recommended: records and variants only, no recursion through `[T]` or
    `{K: V}`, about 120 of the 160 lines, no cliff and no order dependence — but
    it buys nothing for an array, and the motivating case in this sitting's own
    ergonomics experiment is two `[i64]`, as are the compiler's own ASTs.

*******************************************************************************
