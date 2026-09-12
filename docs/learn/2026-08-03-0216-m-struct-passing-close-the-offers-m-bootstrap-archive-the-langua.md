- [ ] **M-struct-passing close — the offers** | M-bootstrap-archive (`mutate`, the language's own help) | The thirteen operators each match on every `ExprKind` case by name, because `_` is forbidden on a variant. Say what that costs in lines, and what it buys the day a fourteenth expression form is added

    **Where to look:** selfhost/mutate/edits.hero · spec:120
    **Why it matters:** the Rust has a `_` in the same place, and that is where the lexer's `nullptr` defect came from
