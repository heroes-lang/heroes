- [ ] **M-robustness-guards step 4** | `down(100000)` at `-O2` prints `100000`; `down(1000000)` at `-O2` panics. Which of these two sentences about clang follows from the pair — *it turned the recursion into a loop* or *it made each frame smaller* — and what single extra run would tell them apart?

    **Where to look:** tests/golden/surface-fixtures/deep/main.hero (its comment) · docs/records/journal/031 § What surprised
    **Why it matters:** one measurement at one depth was written down as a property of the optimiser and was wrong
