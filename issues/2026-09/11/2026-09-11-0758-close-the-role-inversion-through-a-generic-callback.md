- [x] **M-named-callbacks** | close the role inversion through a generic callback: names on any function type, an expected type carrying none accepting a value whose type does, and the generic path comparing the names it is given | `docs/panel/129-the-names-inside-the-type.md` · `design.md:1432-1435` · `selfhost/check/generics.hero` · `selfhost/library_source.hero:84`

    **Origin:** the author, 2026-09-11, answering panel 129's second question with
    neither of the two answers the sitting recommended. The sitting called the
    `fold` case the price of §4.9's criterion rather than a defect against it; the
    author said to build the route that closes it.

    **The program it has to refuse**, run on both compilers at exit 0 the day this
    was filed:

        function append(item: str, acc: str) -> str
            return acc + item

        function main()
            xs: [str] = ["a", "b", "c"]
            print(xs.fold("", append))

    prints `cba` where the author meant `abc`. `fold`'s callback is
    `(function(B, A) -> B)`, two distinct letters, so M-labelled-types' rule does
    not fire and the roles are inverted at the DEFINITION rather than the call.

    **The four changes, as the sitting priced them.** A function type may name any
    subset of its parameters, and the refusal of a name where nothing can be
    confused goes — so `fold` can declare `(function(acc: B, item: A) -> B)`. A
    declared function used as a value carries its parameter names ALWAYS and not
    only at the confusable positions, because the expected type cannot ask for
    what the value did not bring. `table.fits` widens: an expected type carrying no
    names accepts a value whose type does, which is what keeps every function type
    in the tree working. And `check/generics.hero`'s `bind` compares the labels it
    is handed, because a generic parameter type is the only path that never
    compares whole ids and `fold`'s is generic.

    **The compiler seat's objection, kept whole because it is what was
    overridden.** It turns 26 id comparisons across 20 files into a relation and
    introduces variance into a language that has none — `[T1]` into `[T2]` with
    `push` is the covariant-array hole, and design.md has no ruling on variance, a
    silence the seat named rather than filled. And **it does not close the class**:
    it gives a careful author a way to close it one function at a time, so a
    user's own generic higher-order function with an unnamed callback type keeps
    the hole.

    **What it will cost that nothing has measured yet**: every function used as a
    value gains a named type, so the emitter gains a typedef per distinct name
    run, and the emission traces move. Measure that before writing anything, and
    measure `heroes mutate --operator swap-args` on a corpus of generic
    higher-order calls before and after — the sitting's own headline moved zero
    over `examples/` and the next one should say so first if it does the same.

    **CLOSED 2026-09-11 at M-named-callbacks**, and NOT as priced. Two of the
    four parts the sitting costed were unnecessary, and a measurement said so
    before a line was written: a role inversion is invisible only when the two
    parameters end up one type, and there they share a type, so the names already
    existed. What was missing was that `fold`'s own type could not say what it
    wanted. Two changes and a library signature.

    **And the rule is not the one the sitting recommended.** Firing only where the
    callee reused the type's own word for another position catches **2 of 7**
    transposed callbacks in this repository; agreement catches **7 of 7** at the
    price of a permanent naming mandate and six programs renamed once each. The
    author was given both numbers and chose agreement.

    **What the item warned about and what actually happened.** It predicted the
    emission traces would move: they did not, once the explanation was moved out
    of the embedded library text, where five comment lines had shifted every
    `#line` in every program's C. It predicted the headline over `examples/` would
    not move: it did not, and the record says so first.
