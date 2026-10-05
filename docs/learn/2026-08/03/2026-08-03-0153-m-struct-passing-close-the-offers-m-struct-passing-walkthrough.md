- [ ] **M-struct-passing close — the offers** | M-struct-passing, walkthrough | **Why does emitting *nothing* make the mechanism sound?** Read `archive/bootstrap-rs/heroes/src/emit/types.rs`'s first arm — eleven lines that return early for a group's `record` — then `emit/extern_record.rs`. The question to answer is not what the code does, it is what property the early return buys that no assertion could have bought

    **Where to look:** archive/bootstrap-rs/heroes/src/emit/types.rs · emit/extern_record.rs
    **Why it matters:** it is the one design decision in this milestone, and it is a deletion
