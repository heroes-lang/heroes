- [ ] **M-interpolation-verdict step 2** | Four candidate rules for what may stand inside `{...}`. Rank them by what they cost the SPEC, before reading the number | `docs/measurements/022-the-narrow-rule-costs-more-than-the-wide-one.md` · `design.md` §1.2 | the ranking most readers write down is upside down, and seeing why is seeing what §1.2 actually prices

    **Origin:** M-interpolation-verdict step 2, 2026-09-08.
    **The four:** only a bare name · a name and its `.field` steps · any
    postfix run, a call or an index included · any expression at all. Rank them
    cheapest-first in spec tokens, with the opening sentence, the example and
    the escape rule held identical in all four so only the rule sentence moves.
    **The question after:** the ROADMAP's entry calls the bare name *"the
    cheapest rule to write and to lex"*, and it is right about the lexer. Say
    in one sentence why that does not make it the cheapest rule, and which of
    the two costs §1.2's formula counts.
