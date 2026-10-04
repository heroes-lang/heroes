- [ ] **M-deferral-ledger 5** | design.md § 4.10 says the FFI's `ptr` and `cstr` sit OUTSIDE the value-semantics guarantee, named as an exception rather than left to be discovered. design.md Part 9 says everything that breaks the guarantees goes in a `raw` module, pointers first. Find both sentences, say which one the language actually implements, and then say what the other one was still doing in the document eleven months later.

    **Where to look:** design.md § 4.10's *"Two exceptions, named rather than
    discovered later"*; Part 9's *"Everything that breaks the guarantees goes in
    `raw`"* and the correction landed beneath it; § 1.11 on where everything comes
    from; and the sitting,
    `docs/panel/140-the-document-had-already-ruled-against-part-9-and-nobody-told-part-9.md`.

    **Why it matters:** no seat's brief named the contradiction — the seat that
    compiles C bindings found it by going to read what the document says about the
    types it works with every day. Two sentences can sit two thousand lines apart
    in one file, each correct-looking, and only somebody who needs both on the same
    afternoon will notice they cannot both be true.
