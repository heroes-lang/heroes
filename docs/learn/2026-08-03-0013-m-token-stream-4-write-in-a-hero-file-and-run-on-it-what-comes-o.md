- [ ] **M-token-stream.4** | Write `let x = 5` in a .hero file and run `heroes lex` on it: what comes out, and what travels attached to the diagnostic?

    **Where to look:** spec/reserved-words.md · lexer tests (certain_fix_travels_with_the_diagnostic)
    **Why it matters:** the thesis made executable: the likeliest LLM mistake fails loudly with the repair pre-written
