- [ ] **M-struct-passing close — the offers** | M-selfhost-port, the ownership pass | **The whole chain now runs: lower -> mono -> own -> verify at owned, on strings, loops, records and a generic.** What did the first real run of the owned checks find, and in WHICH module was the defect?

    **Where to look:** ir_own tests, ir_phases.hero's counter comment
    **Why it matters:** a check wired but never exercised is a check that may itself be wrong — the pass and its verifier debugged each other
