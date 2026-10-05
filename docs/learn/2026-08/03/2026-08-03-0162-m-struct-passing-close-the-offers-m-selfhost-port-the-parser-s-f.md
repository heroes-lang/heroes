- [ ] **M-struct-passing close — the offers** | M-selfhost-port, the parser's front door (parse.hero) | `parse` concatenates diagnostics lexer-first, then parser — never sorted across stages. The test feeds a tab on line 2 and `junk` on line 3: which diagnostic comes out FIRST, and what would interleaving by position have put first instead?

    **Where to look:** selfhost/parse.hero, the second test
    **Why it matters:** a shape error and the word error that caused it read better together — the order is a design decision, not an accident of the pipeline
