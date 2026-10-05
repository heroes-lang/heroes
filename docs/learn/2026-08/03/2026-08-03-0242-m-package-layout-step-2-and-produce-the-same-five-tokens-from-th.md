- [ ] **M-package-layout step 2** | `use a/b` and `use a / b` produce the SAME five tokens from the lexer, and the compiler reads them as two different programs. Say what distinguishes them, and then say which of the compiler's two readers of a use line would get it wrong if that rule lived in only one of them

    **Where to look:** selfhost/parse/use_line.hero (the cursor reader) · selfhost/module/paths.hero (`path_at`, the token reader, and the test that makes them agree)
    **Why it matters:** panel 031 R8 refused a second grammar for one text, and this is that refusal one level down — the same rule in two places, with a test that fires when they diverge
