- [ ] **M-closures-verdict close — walkthrough** | The route a capturing closure would have taken through the compiler, opened one file at a time: where the parser would put it and why a module of its own is `error[module_cycle]`, what `.func_ref` becomes in the IR, and the three tables that would each need a new case. Read `selfhost/ir/emissions.hero`'s `.func_ref` arm and `selfhost/emit/ctype.hero`'s comment, then say which of the two would have to change first

    **Where to look:** selfhost/ir/emissions.hero · selfhost/emit/ctype.hero · selfhost/grammar_expr.hero's knot map
    **Why it matters:** the refusal rests on a count of places, and the walk is what makes the count mean something rather than being a number
