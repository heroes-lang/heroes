- [ ] **M-struct-passing close — the offers** | M-selfhost-port, the resolver complete | **The driver's order is the design.** resolve() runs: hole scan, collect, per-declaration walk, unused_uses, report_unused, cycles, sort. Two questions with one answer each: why must cycles run LAST (what is incomplete before every body is walked), and why does the hole scan run FIRST as a flat pass over the arena instead of inside the walk?

    **Where to look:** selfhost/resolve.hero, the driver's comments · resolve/mod.rs
    **Why it matters:** each position in that order fixes a defect the record can name — the cycle check reads a table only the walk completes, and a hole inside a construct the walk gives up on must still suspend the sweep
