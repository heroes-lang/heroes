- [ ] **M-struct-passing close — the offers** | M-bootstrap-archive (the emission oracle) | Four guards keep this suite from quietly stopping: a floor, a collision check, an orphan sweep, a skip floor. Pick the ORPHAN sweep and say which mistake it catches that the other three cannot

    **Where to look:** tests/harness/suite_emission.hero::orphans
    **Why it matters:** a renamed case leaves bytes nobody compares, and nothing else in the repository would notice
