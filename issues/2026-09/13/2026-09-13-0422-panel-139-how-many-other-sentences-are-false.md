- [ ] **panel 139** | One absolute sentence in the specification was found false by accident. Take the grep the sitting used — every `never`, `always`, `anywhere`, `cannot`, `no ... at all` in the document — and pick three the sitting did NOT run. For each, write the shortest program that would falsify it if it is false, say whether you expect it to, and then run them.

    **Where to look:** the suspect list in
    `docs/panel/139-the-sentence-was-false-and-so-were-four-of-its-neighbours.md`
    § What the sitting found, point 6; `spec/heroes-spec.md` § 4's *"There are no
    mutable globals"*, § 5's *"All bindings are initialised"*, § 10's *"grows in
    place while nothing else holds"*; and CLAUDE.md § RUN IT on what a negative
    claim rests on.

    **Why it matters:** the sitting's own finding is that the document says
    `anywhere` and means *among the values this language owns*, and that the two
    read identically until a program proves otherwise. Four more sentences have
    the same shape and nobody has run them. This item is the only one in this list
    whose answer is not already somewhere in the repository.
