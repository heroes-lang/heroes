- [ ] **panel? M-syntax-tree.2** | The foreign-word registry reserves words that are plausible *identifiers*: the appendix had a variant case `.var` and could not lex. Found the same class as panel 013, Heroes-side

    **Where to look:** archive/bootstrap-rs/heroes/src/lexer/keywords.rs (foreign_word), design.md appendix (`.var` → `.variable`)
    **Why it matters:** `var case union use include class try const` are all unusable as names today; the registry's price is now measurable
