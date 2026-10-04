- [x] **M-check-completeness** | the two written-type rules, in the one pass that closes both | **CLOSED 2026-09-16** — one landed and the other is a position taken, not work left | `docs/panel/155-the-hole-was-never-made-by-the-generic.md` R2 and R3

    **This item is ticked without the pass it asks for**, and that is the
    finding rather than a shortcut. Panel 082 R3 ruled in 2026-08-16 that one
    pass in the checker would close both rules, keyed by the call-site span.
    Panel 155 priced that pass at **284 code lines** against the sitting's own
    estimate of 90-110, refused it on **soundness** — it needs a second
    span-keyed table against the standing rule at `selfhost/check/state.hero:78-85`,
    has no node-to-declaration map to build on, and reproduces defect 035's
    shape in its own walk — and found the answer already written in
    `docs/design/design.md:1721`: *"No constraints. No `where`, no bounds. If an
    operation on `T` is needed, pass it as a parameter."*

    **`ffi_partial_operation`: LANDED**, as a body rule. `==` on two values of a
    bare type parameter is `error[generic_equality]`, wired at
    `selfhost/check/ops.hero`, with
    `tests/golden/check/equality-on-a-type-parameter.hero` and its three
    controls. Measured: of the **50** generic functions in this tree, two use
    `==` in a body and **neither compares two bare type parameters**, so the
    rule deletes nothing. `spec § 13` is true as written at **+0** spec tokens —
    the compiler had the bug (CLAUDE.md § 12).

    **`float_map_key`: A POSITION, not unfinished work.** Panel 155 R3 decided
    it **waits** under Principle 0, with its reasons measured: `selfhost/` has
    zero generics so the closure list does not need it; the specification
    constrains `K` **nowhere**, which the llm-ergonomist found from the document
    alone and the spec-warden confirmed from the tree; and the body rule that
    closes the other half would delete
    `tests/golden/run/abort-map-key-nan.hero` and
    `tests/golden/run/fixedbugs-a-map-key-that-is-not-itself.hero`, the second
    of which had **already been rewritten once** to route through the generic
    this would close.

    **What the item asked for that a later session still owes**, so the tick is
    not read as more than it is: `spec § 10` still has no sentence saying which
    types may key a map. That is panel 155 R5, owed whichever way R1 went, and
    it is the llm-ergonomist's standing condition — without it the compiler
    refuses `m: {f64: i64}` on a rule the language never states. It is not this
    item's, because this item is about the pass and R5 is about the document.

    **And the third rule this item used to claim closed itself.** The line read
    *the sortable obligation and the two written-type rules* until 2026-09-16,
    when the milestone's opening measurement found `sort` on a type parameter
    already refused by panel 084 R1 — on the very day panel 082 R3 wrote the
    sentence claiming it was not.
