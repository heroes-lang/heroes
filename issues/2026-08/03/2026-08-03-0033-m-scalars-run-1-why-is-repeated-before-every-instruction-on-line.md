- [ ] **M-scalars-run.1** | Why is `#line 8 "f.hero"` repeated before every instruction on line 8 rather than emitted once? One sentence, and it is about C rather than about Heroes

    **Where to look:** archive/bootstrap-rs/heroes/src/emit/writer.rs (the module doc) · tools/spike/01-first.c:19
    **Why it matters:** the frozen target had this wrong and nobody noticed for two milestones
