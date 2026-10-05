- [ ] **M-struct-passing close — the offers** | M-selfhost-port, per-type functions complete (emit_perfn.hero) | **Why do a variant's case payloads come BEFORE the variant itself, twice?** Name the two orderings (typedefs, function bodies) and what breaks in C if either flips

    **Where to look:** emit_perfn.hero's doc, the payload-before-outer test
    **Why it matters:** the outer eq calls the inner ones — C wants everything declared before use, and the checker's type_order only solved half of it
