- [ ] **M-struct-passing close — the offers** | M-selfhost-port, the checker complete (checker.hero) | **The driver's order is seven positions, each fixing a named defect.** Why do sizes run FIRST (what would a later answer be worth after a containment cycle), why do map_keys and the fixed sweep run AFTER decls (which table do they read, and what happens when it is empty), and why is counted LAST (what does a TyId past the table's end read as)?

    **Where to look:** selfhost/checker.hero's comments · types/mod.rs::check
    **Why it matters:** the order is not style — every position is a fixed defect, and shuffling two of them reintroduces one
