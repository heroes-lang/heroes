- [x] **panel 125** | the lease clause says a forgotten lease is *counted* where every other runtime stop in the document *aborts*, and the word never says the program dies: does §4.19's fourth case take the document's own verb, at +0 or +4 vendored tokens? | `spec/heroes-spec.md:246`, the verb at `spec:71` and its five other uses | **ANSWERED 2026-09-10, the +4 wording** (`/decide`, `a`): the clause reads *a lease nobody ends aborts when `main` returns, saying how many*. SPEC_TOKENS 4206 to 4210, REAL_TOKENS 5373 to 5378 on `claude-opus-5`, digest `527e1b762302f9ca`, ledger row 62, the seed regenerated at the fixpoint. The +0 variant was priced and refused for dropping the count the message prints. The prediction is now an instrument: `tests/golden/run/lease-open-through-exit.hero` pins the `exit(code:)` path where the gate never runs, which no golden covered — **corrected 2026-09-10: that golden could not exist**, because a `run/` case is also run under `--sanitize` and this program leaks by construction, so LeakSanitizer reports it and the case is red; what pays in its place is that report itself, the Linux leg naming the allocation site while the runtime's count stays silent (`docs/measurements/025`)

    **Origin:** the site panel's languages seat, 2026-09-10, on the sentence the
    author ratified hours earlier. It is the same sitting's fourth point read
    once more, so it is filed under it rather than as a new number.

    **What is measured.** *Abort* is defined at `spec:71`, *an abort ends the
    program, saying why*, and the document uses it for overflow, an index, a
    slice that splits a character, division by zero, recursion too deep, a
    `nan` in an ordering or a `sort`, a shift count outside 0..63, and an
    argument that is not UTF-8. The lease is the one runtime stop that does not
    say it, and the program does die: exit 134, run twice. Two wordings priced
    on a scratch copy with the vendored instrument: *aborts when `main`
    returns, saying how many* is **4210**, +4; *aborts when `main` returns* is
    **4206**, +0, and drops the count the message actually prints.

    **The recommendation is the +4 wording**, because the count is what the
    message gives and a reader who is told only *aborts* will look for a name.
    **It is a language document, so CLAUDE.md §4 sends it to the panel** rather
    than to a session's judgement, and the author may take it directly instead.
    A prediction is available and pays for it either way: **no golden covers a
    forgotten lease leaving through `exit()`**, measured over every
    `tests/golden/run/*.hero` (four name `lease(`, none of those names `exit(`),
    so the bypass the site now asserts rests on one scratch run. The golden is
    owed with the amendment.
