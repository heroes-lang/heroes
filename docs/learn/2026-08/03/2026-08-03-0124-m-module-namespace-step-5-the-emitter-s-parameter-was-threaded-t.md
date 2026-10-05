- [ ] **M-module-namespace step 5** | The emitter's `module: &str` parameter was threaded through eight functions and is now derived at each site from the declaration's own span. Task: name the two things the ROOT module still names, and say why neither of them is a declaration

    **Where to look:** archive/bootstrap-rs/heroes/src/emit/mod.rs · archive/bootstrap-rs/heroes/src/emit/writer.rs
    **Why it matters:** a parameter that is always the same value is a parameter that is about to be wrong
