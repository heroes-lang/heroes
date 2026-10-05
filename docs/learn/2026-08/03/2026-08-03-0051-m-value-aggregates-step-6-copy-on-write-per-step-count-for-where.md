- [ ] **M-value-aggregates step 6** | **Copy-on-write, per step.** Count: for `g.rows[0].cells[0] @ 7` where `h = g` shares every level, how many heap blocks exist before the write and how many after — and which of them does `h` still point at? Then say what a single unshare at the primitive would have left `h` pointing at

    **Where to look:** tests/golden/run/adversarial-cow-per-step.hero · runtime/runtime.c (hero_array_unshare, hero_array_set) · docs/panel/022 R2
    **Why it matters:** panel 022 measured the wrong version passing every instrument this project has
