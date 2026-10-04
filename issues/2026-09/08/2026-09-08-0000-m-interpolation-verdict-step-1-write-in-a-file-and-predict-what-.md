- [ ] **M-interpolation-verdict step 1** | Write `x = "a" + b + c` in a file and predict what `heroes parse --dump-ast` prints for that line, out of four candidate shapes, before running it | `heroes parse <file> --dump-ast` · `spec/heroes-spec.md` § Operators | the answer is why the dump can be used as a measuring instrument, and the wrong answers are why raw text cannot

    **Origin:** M-interpolation-verdict step 1, 2026-09-08, the step that used
    the dump to count 2536 holes in 1461 concatenations.
    **The four candidates:** (a) `x = "a" + b + c`, unchanged; (b)
    `bind x = ("a" + b + c)`; (c) `bind x = (("a" + b) + c)`; (d)
    `bind x = ("a" + (b + c))`. Exactly one is right, and which one it is
    settles two things a text search has to guess: where the operator's
    precedence put the grouping, and whether a chain broken across three source
    lines comes back as one.
    **The question after:** `+` on `str` is left-associative here. Name one
    program whose PRINTED result would differ if it were right-associative, and
    one where it could not.
