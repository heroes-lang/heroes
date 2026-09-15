- [x] **044 — a record naming one field type TWICE registers only the first handle behind it, so a correct program aborts and a leaking one passes** | `check/reaches.hero`'s `gather` threaded one `seen` map across sibling fields, so the second field whose type was already walked enumerated nothing | **repaired 2026-09-15** | `selfhost/check/reaches.hero`'s `gather`, `fields_of` and `on_route`

    **Origin:** found 2026-09-15 by a **six-agent adversarial sweep** over the
    shapes next to defect 037's repair, and put to an independent skeptic told to
    refute it, which could not. Diagnosed and repaired by an independent judge
    that reproduced both arms before touching anything.

    **THE CAUSE, in one sentence: a prefix-dependent result was memoised under a
    prefix-independent key.** `gather` accumulates the dotted suffix of every
    handle a type reaches, and its answer for a declaration depends on the prefix
    it arrived under. The `seen` map was keyed on the declaration alone and never
    cleared, so a sibling field of the same record type returned having pushed
    nothing. The module's own doc states the intended invariant — *"the guard is
    a set of the declarations already on the route"* — and the code implemented
    *a set of declarations visited anywhere in the walk*. Those are two different
    sets, and the difference is exactly one sibling.

    **Why the sibling function was right to keep its whole-walk memo, which is
    the half that makes this a repair rather than a swap.** `walk` asks an
    EXISTENTIAL question and unwinds on the first hit, so control only returns to
    a later sibling with an entry still in `seen` if that subtree FAILED — and by
    induction every declaration in `seen` at that moment reaches no handle.
    Re-asking it yields fail again. Its own comment says so: *"either way the
    answer for this declaration is settled."* True of `walk`, false of `gather`,
    and the walk was copied with the sentence attached.

    **THE REPAIR is mark, descend, unmark**, and two shapes in it are deliberate.
    `fields_of` exists only so those three are consecutive statements: the arm
    that ends a non-record declaration used to `return` out of `gather` itself,
    which under a route-scoped mark would leave the declaration marked forever —
    the bug's own mirror image. And `on_route` asks the VALUE rather than the
    presence, because the language has no map removal (searched for `.remove(`,
    `.delete(` and `.erase(` across `selfhost/`: zero hits), so leaving a route
    writes `false`. **Net +8 lines of repair in one module**, nothing in the
    lowering, nothing in the backend, nothing in the runtime.

    **The cycle guard still terminates, proved rather than argued.** `gather`
    recurses through exactly two constructs, a record field by value and a fixed
    array, and `check/sized.hero` refuses all three by-value cycle shapes before
    emission — measured: a record holding itself, a record holding a fixed array
    of itself, and two records holding each other are each `error[no_size]`. So
    no program that reaches emission can drive `gather` into a cycle today, which
    is a premise about the world rather than a fact about the value, and the
    guard therefore stays. Two unit tests call the walk on cyclic types directly
    and both pass.

    **The exception hunt, sixteen shapes, each a built program.** Unchanged: one
    field, padded, nested to depth four, the tagged handle itself, a fixed array
    of one, the same handle declaration at two depths — that last was never
    broken, because the handle branch fires before the guard, so the bug keyed on
    the intermediate record. Refused as before: none, empty, generic, a variant
    in a group, a fixed array of length zero. **Repaired**: a fixed array of 2, 3
    and 17 went from one acquire and exit 134 to the right count and exit 0; a
    diamond went from 1 to 2; seventeen distinct fields of one record type went
    from 1 to 17. A DAG of depth fourteen yields 16,384 suffixes in 0.16 s of
    user time, and that is not a blow-up to defend against: each suffix is a
    distinct C lvalue and therefore a distinct address, so the answer is
    genuinely that large.

    **The two golden cases are verified in both directions**, green on the
    repaired compiler and red on a compiler with only the guard hunk reverted.
    `tests/golden/run/fixedbugs-sibling-fields-of-one-record-type.hero` releases
    both handles and must exit 0; under the old compiler it exited 134.
    `tests/golden/run/fixedbugs-the-unacquired-sibling-leaks-loudly.hero` leaks
    one and must abort saying so; under the old compiler it exited 0 with an
    empty stderr. **That second case cannot be faked**: the harness requires the
    stderr substring AND a non-zero exit, so it passes only if both handles were
    enumerated. A regenerator can rewrite an expectation; it cannot make an
    unenumerated handle leak loudly.

    **And the module had zero tests before this.** It has four now, two that fire
    on the bug and two that hold the termination guard no golden can reach.

    **The compiler is not slower**, measured warm against warm on one input with
    the machine still: 54.98 s user unrepaired, 54.93 s repaired.
