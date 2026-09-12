- [ ] **M-cstr-lifetime, golden ratification** | Four cases were written and marked nothing; read them and decide whether each pins what it claims to pin | `tests/golden/check/fixedbugs-a-lend-*.hero` and their `.expected` | a golden that pins the wrong thing is worse than none, and the only way to know is to read the case against the rule

    **Origin:** M-cstr-lifetime close, 2026-09-09. The four are the returned
    lend, the two-hop laundering, the record field and the bound lend.
    **The question after:** one of the four provokes two diagnostics and its
    annotations say so. Work out why the OTHER three provoke exactly one each,
    and then say which clause would have to change for that count to move.
