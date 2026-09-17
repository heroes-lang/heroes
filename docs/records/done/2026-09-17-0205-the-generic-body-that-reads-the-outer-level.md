- [x] **056 — a generic body reads the outer level of whatever it is instantiated with** | `is_bad<A>(x: A?) -> bool` returning `x.is_err()`, called with `A := i64?`, passes under every option panel 160 weighed | costed 2026-09-17, M-check-completeness step 15; filed rather than repaired

    **Origin:** 2026-09-17, panel 160's compiler-engineer, confirmed by its
    llm-ergonomist from the specification alone (its hesitation 10) without
    either knowing of the other.

    **The hole.** A generic body is checked ONCE, with a `.generic` payload, and
    instantiation happens after the checker in `selfhost/ir/mono.hero`. So a
    refusal that reads the written type cannot see that `A` arrived fallible.
    Measured: the probe checks at exit 0 under the prototypes of option A and of
    option E alike.

    **Why it is filed and not repaired.** Live instances: **0** in `selfhost/`,
    **0** in the corpus. Of eight corpus generics carrying an `A?`, the two that
    touch it use `.len()` and `==`. And the repair is a post-`mono` check — a
    second layer under design.md §1.7 — which panel 160's compiler-engineer said
    it would **veto** in a resolution that required it without a costing.

    **What is owed** is the costing, before anyone argues about the rule: how
    many lines a post-`mono` check is, what it does to `check` time, and whether
    the library's own `find<A>(xs: [A], f) -> A?` reaches it when `A` is
    instantiated fallible. That last one is **UNRUN** and is the question that
    decides whether this is theoretical or one `find` away.

    **COSTED 2026-09-17**, `docs/measurements/034-the-generic-body-that-reads-the-outer-level.md`.
    The half that was open has an answer: **the library cannot reach the hole.**
    `find` constructs — `ok(x)`, `fail(…)` — and `any` and `all` hand the element
    to the caller's own predicate and return its `bool`; `map`, `filter` and
    `fold` do the same. Not one of the six applies a reader to `A`. So the hole
    is reachable only from a generic the AUTHOR writes, and what the library can
    do — hand a caller a nested value — option E closes at the call site.

    **The rest of the costing is still UNRUN and the record says so**: neither
    shape has been built, so no line count and no `check`-time figure exists for
    either. What is measured is that the call-site instantiation table already
    exists and is already walked (`check/walk.hero:1802`), so the shape that fits
    the existing seam adds an obligation set and a walk rather than a pass.

    **Filed rather than repaired**, on zero live instances, so the next sitting
    starts from a measurement instead of from the argument — which is the
    condition panel 160's compiler-engineer attached to its non-veto.
