- [ ] **M-cstr-lifetime, walkthrough** | Read `selfhost/check/lending.hero` top to bottom and say, before looking at the tests, which of its three clauses would catch each of five programs | `selfhost/check/lending.hero` · `tests/golden/check/fixedbugs-a-lend-*.hero` | three clauses that look like one rule are three different kinds of rule, and telling them apart is the whole lesson

    **Origin:** M-cstr-lifetime close, 2026-09-09.
    **The five programs**, and each is one of the golden cases or a near miss:
    `return ("a" + n.to_str()).cstr()` · `return pass(c: s.cstr())` where
    `pass` answers `cstr` · `record H` with a `c: cstr` field · `c = s.cstr()`
    then two extern calls · `strlen(s: "legal".cstr())`. For each, name which
    clause fires, or say none.
    **The question after:** one of those five is refused by TWO clauses at
    once, and one is sound today and refused anyway. Say which, and say what
    the second one buys.
