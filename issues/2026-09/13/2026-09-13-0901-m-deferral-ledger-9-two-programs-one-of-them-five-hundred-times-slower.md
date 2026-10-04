- [ ] **M-deferral-ledger 9** | Write the same accumulation three ways — `parts @ parts.push("x")` on a plain name, `w.parts @ w.parts.push("x")` through a record field, and `put(@w.parts, "x")` through an `@` parameter — and time all three. Then build each with `--emit-c` and count `hero_array_push_owned`. Say, before reading the answer, which of the three you expected to be the slow one.

    **Where to look:** the three programs are in the sitting,
    `docs/panel/144-the-cost-is-in-the-spelling-and-nobody-had-written-the-cheap-one.md`
    § What was measured; `selfhost/ir/place_store.hero:27-31`, which states its own
    limit in one sentence; and `docs/records/log/2026-08-26-0001-…`, where this
    repository did exactly this repair to itself.

    **Why it matters:** two of the three are 1 000 000 pushes in 0.02 seconds and
    one is 50 000 in 11.51 — twenty times the work in five hundred times less
    time — and nothing in the source distinguishes them to the eye. The rule is
    that the optimisation fires only when the path is **empty**, and `@w.parts`
    resolves the path at the call, so inside the function the place is bare. Four
    panel seats measured this and each generalised from the one shape it wrote:
    one concluded the array was fine, two concluded record fields were doomed, and
    the sixth agent found the spelling that is neither. Seeing that *the cost lives
    in how you write it, not in what you write it on* is the whole lesson, and the
    second half is that the answer was already in the project's own history,
    twenty days old, and nobody went and looked.
