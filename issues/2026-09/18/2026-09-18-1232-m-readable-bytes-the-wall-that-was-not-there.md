- [ ] **M-readable-bytes mutation drill** | The milestone was opened on two walls and one was not there. **Before reading: a C function fills a struct the caller owns. You cannot write a 256-element literal. List every way you can think of to obtain that struct.** | `docs/panel/163-the-wall-was-not-there-and-the-brief-said-it-was.md`

    **Where to look after answering:** the answer three seats found by running
    it — **ask the library for one**. A function returning the record, declared
    in the `extern` group or written in Heroes, plus `partial` to name only the
    fields you read. 874 tokens with no C at all, against 3979 for the literal.

    **Why it matters.** The repository's own golden tests already returned a
    struct by value from C in **seven** places while three sittings argued about
    how to make it possible. The capability was not missing; the question was
    never asked.

    **The question to carry away.** The brief that convened panel 163 said the
    compiler did not have three things it had. Ask what those three sentences
    have in common, and why a brief is the one document in a sitting where that
    failure is invisible.
