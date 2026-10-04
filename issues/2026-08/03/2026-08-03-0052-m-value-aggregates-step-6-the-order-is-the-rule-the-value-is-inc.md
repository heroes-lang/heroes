- [ ] **M-value-aggregates step 6** | The order is the rule: the value is increfed **before** the outermost unshare. Task: trace `n.children[0] @ n` with the incref moved to *after* the unshare, and say what the stored value would then refer to. (The answer is why `Op::CowCheck` is not an IR instruction — a judge built a three-block cycle out of the hoistable form.)

    **Where to look:** archive/bootstrap-rs/heroes/src/own.rs (the indexed-store arm) · docs/panel/022 R3
    **Why it matters:** it is the one place in the pass where instruction order carries the correctness argument
