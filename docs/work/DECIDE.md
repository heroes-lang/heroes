# DECIDE — the decisions the compiler is waiting on

Read by **`/decide`**. Every item asks **what should be true**, and until it is
answered the compiler goes on behaving some way by default — so the item names
that default, because it is the cost of leaving the item open.

**Only open items live here.** The moment one is answered it is ticked with the
verdict written into it and moved to `docs/work/DONE.md`, the record. Rank by
what an item blocks, never by age, and verify it against the repository before
putting it to the author: asking a settled question is the one cost this list
cannot pay.

**The shape.** One line per item, and an optional body indented four spaces
under it. The first field is the item's **origin**, and where that origin is a
sitting it is spelled `panel NNN`, padded — because
`tests/harness/suite_records.hero`'s `queued` check reads the `- [ ] ` lines
alone and scans them for exactly that, so a citation that slides into the body
makes every pending sitting report as unqueued, and it fails silently. Nothing
lives outside the two banners: `records/lists` is the executor of that.

Format: `- [ ] **<origin>** | <the question, in one line> | <where to look>`

*******************************************************************************
**OPEN: 2**

- [ ] **panel 128** | ratify what the sitting landed on defect 026: a label at a call through a function value is refused, the compiler's note says the call is positional, and the document says it too; or ask for the conservative route to be built and measured instead | `docs/panel/128-the-type-that-names-nothing.md` · `docs/measurements/010-spec-budget-ledger.md` row 65

    **Origin:** the sitting of 2026-09-11, five seats, on the author's
    instruction to convene it. **The default the compiler is running on is what
    landed**, so a ratification settles whether the small half was the right
    half to take.

    **Three routes were vetoed, each by a different seat on soundness**: the
    document's positional sentence as balloted, because invented labels were
    accepted and the sentence was therefore false; refusing a function type
    whose parameters share a type, because it forecloses three steps of the FFI
    ladder and deletes a comparator that compiles today; and names inside the
    type, because it is core by Part 5's test, touches eleven modules and
    inverts a shipped certain fix. A veto is a refusal rather than a price.

    **What is open after it, in one number**: `heroes mutate --operator
    swap-args` reads 50% on a call through a function value, before and after.
    The inversion is still silent, and at the C boundary the ffi seat measured
    it reaching memory three times, once at exit 0 with a heap-use-after-free.

    **The recommendation is to ratify and let the scheduled item take the rest.**
    The conservative route is priced and unbuilt, and its own vetoing seat named
    what would lift the veto: build it, show the two ceilings raised and the
    instrument at 100% under about 150 code lines. That is a measurement, not an
    argument, and it belongs to a milestone rather than to a night.

- [ ] **panel 127** | ratify the repair of defect 025 as adopted: the label rule reaches `fail` and `slice`, both halves of it, and 1022 call sites in the repository grew eleven characters each to pay for it | `docs/panel/127-the-rule-that-did-not-reach-its-own-library.md` · `docs/measurements/010-spec-budget-ledger.md` row 64

    **Origin:** the sitting of 2026-09-11, five seats, on the author's word
    `sistemami il difetto` the evening panel 126 filed it. **The default the
    compiler is running on is the repair**, which landed with M-labelled-builtins.

    **Two seats vetoed the alternative** and the record carries both: the
    spec-warden because the clause naming the built-ins as an exception is a
    false sentence, `range(1, 4)` and `write_file("out.txt", "hello")` being
    built-ins the rule already refuses, and the ergonomist because it would
    withdraw the locality guarantee from a class of calls to save two words in
    one of them.

    **What a ratification settles** is whether the rule stays where it now
    reaches, at the price of eleven characters at every failure site anyone
    writes from here on. The measurement that argues for it: the classic
    argument inversion went from 367 survivors to 75 over the corpus, with the
    159 swapped `fail(` calls falling to zero, `heroes mutate --operator
    swap-args --survivors` before and after. **The recommendation is to keep
    it**, and the conservative route is recorded beside it in the sitting rather
    than argued away.

*******************************************************************************
