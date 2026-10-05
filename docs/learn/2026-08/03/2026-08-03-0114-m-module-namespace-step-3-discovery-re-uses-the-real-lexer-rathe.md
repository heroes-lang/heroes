- [ ] **M-module-namespace step 3** | **Discovery re-uses the real lexer rather than a prefix scanner** (panel 031 R8), and one test is the whole argument: `# use geom` and `print("use geom")` name no module. Question: what would the spec rule the compiler-engineer asked for have bought, what would it have cost, and which CLAUDE.md section forbids the reason it would have been bought for?

    **Where to look:** archive/bootstrap-rs/heroes/src/modules/mod.rs (`uses_of`) · docs/panel/031 R8
    **Why it matters:** a second grammar for the same text is two grammars that can disagree
