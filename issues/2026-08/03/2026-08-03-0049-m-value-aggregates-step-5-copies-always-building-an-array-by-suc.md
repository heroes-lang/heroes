- [ ] **M-value-aggregates step 5** | `push` **copies, always** — building an array by successive push is O(n²), the same shape as `s + t`. Question: what would have to be true of `xs` for an in-place append to be legal, and why can the IR never know it? (The answer is in what a refcount of 1 means for a value held by a slot.)

    **Where to look:** runtime/heroes_runtime.h (hero_array_push) · design.md §4.10's "known performance consequence"
    **Why it matters:** the choice looks like a performance decision and is a correctness one
