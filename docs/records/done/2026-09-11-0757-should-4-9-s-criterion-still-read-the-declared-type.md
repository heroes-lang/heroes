- [x] **panel 129** | should §4.9's criterion still read the DECLARED type, now that a generic callback's roles can be inverted in silence? | `design.md:1432-1435` · `docs/panel/129-the-names-inside-the-type.md` · `selfhost/library_source.hero:84`

    **Origin:** panel 128's compiler seat found the program, 2026-09-11, and
    panel 129 classified it. `fold` handed a function that reads its two
    parameters the other way round prints `cba` where the author meant `abc`, at
    exit 0, on both compilers — re-run in the landing session.

    **Why it is a question and not a defect.** design.md §4.9 says the criterion
    is two parameters sharing a type *in the signature*, and *"it applies to
    declared types, not to types after monomorphisation, otherwise a generic
    function's call convention would change depending on whether its type
    parameters collapse"*. `fold`'s callback is `(function(B, A) -> B)`, two
    distinct letters, so it is outside the rule BY that ruling. The compiler seat
    gave a second, independent ground: it is a parameter-role inversion at a
    DEFINITION rather than an argument inversion at a call, so no rule of §4.9's
    shape can reach it whatever the criterion says.

    **The route that would close it, priced rather than recommended.** Allow
    names on any function type, keep them mandatory on a same-typed pair, and let
    an expected type carrying no names accept a value whose type does — then
    `fold` could declare `(function(acc: B, item: A) -> B)`. The compiler seat
    called it sound at run time and objected on shape: it turns 26 id comparisons
    across 20 files into a relation and introduces variance into a language that
    has none, and design.md has no ruling on variance. **It does not close the
    class either** — it gives a careful author a way to close it one function at
    a time.

    **ANSWERED BY THE AUTHOR, 2026-09-11, and not with either of the two answers
    the sitting recommended.** Asked whether the `fold` role inversion is a
    question about §4.9's criterion, a defect, or work to do now, the author said
    **build the route that closes it**. So it is not a decision any more, it is
    work: `docs/work/SCHEDULED.md` carries it with the compiler seat's objection
    attached, because that objection is what the author overrode and the record
    keeps it whole.
