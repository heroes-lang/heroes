- [x] **025 — the label rule stops at the compiler's own built-ins** | `fail("a", "b")` and `xs.slice(1, 3)` compile, though both take two parameters of one type, while `range(1, 4)` and every function written in Heroes are refused | `spec § 9 Functions and calls` · `selfhost/check/builtins.hero` · `selfhost/library_source.hero`

    **Origin:** panel 126's ergonomist seat, 2026-09-11, which called the shape
    of a `fail` call *a coin flip the prose alone cannot settle* and wrote the
    two sides of it in two programs. Measured after: **neither side is refused.**

    **The reproducer**, run on the compiler built from the seed at `834d804f`:

        function a() -> i64?
            return fail("code_here", "message here")      # accepted
        function b() -> i64?
            return fail(code: "code_here", msg: "message here")   # accepted
        function two_strings(one: str, other: str) -> str
            return one + other
        function main()
            print(two_strings("x", "y"))                  # error[needs_label], twice

    and `xs.slice(1, 3)` is accepted while `range(1, 4)` is refused
    `error[needs_label]` on both arguments.

    **The seam is the implementation showing through the surface.** `slice` and
    `fail` are the compiler's own built-ins and never pass the check that reads a
    signature's parameter types; `range`, `map`, `filter`, `fold`, `find`, `any`
    and `all` are written in Heroes (`spec § 11 Built-ins` says so in the
    document's own words) and are checked like any other function. So the rule
    holds for the seven a reader is told are written in Heroes and lapses for the
    rest, and nothing in the language explains the difference.

    **Why it is filed as a defect of the compiler and not of the document**
    (CLAUDE.md §12): `spec § 9` states *when two parameters in a signature share
    a type, named arguments are mandatory at the call site*, with no exception,
    and the spec beats the compiler. Two repairs are available and both are the
    panel's, because a diagnostic class is a panel path (CLAUDE.md §4): the check
    reaches the built-in table, which makes `fail("a", "b")` an error in
    programs that exist today, or the document names the exception, which spends
    tokens on a seam rather than on a rule. **The document's own example was
    written before this was measured and is legal either way**: `spec § 6`'s
    `fail("empty", "no first element")` is what the compiler accepts today, and
    if the first repair is taken that line moves with it.

    **What it is not.** Not a crash, not a wrong answer: every program named here
    exits as the compiler says it will. It is the third shape this list admits, a
    silence where a message is owed, and it was invisible until a blind reader
    wrote both forms of the same call on purpose.

    **THE REPAIR, 2026-09-11, panel 127 and M-labelled-builtins.** Both halves
    of the rule reach the two built-ins: `fail` and `slice` carry their parameter
    names as literals in `selfhost/inventory.hero`, `selfhost/check/labels.hero`
    enforces a missing label as `needs_label` and a label in the wrong position
    as `wrong_label`, and the two call paths in `selfhost/check/walk.hero` ask
    it. **The missing-label half alone would have killed nothing**, measured: the
    mutation operator moves label and value together, so the order half is what
    kills the swap. **1022 call sites** were relabelled across the tree, the
    seven inside the embedded library text and the ten inside the compiler's own
    test programs by hand; one malformed code, `fail("compiler bug", …)`, became
    `compiler_bug`, found by the sitting's historian predicting that one of the
    1022 would be malformed. **`wrong_label`'s fix is now a `guess`** when the
    written label names another position, because renaming it leaves the values
    swapped: a fix that keeps the defect it names.

    **What the repair is worth, measured on the instrument that existed**:
    `heroes mutate --operator swap-args --survivors` over the corpus read 1441
    of 1910 killed and 367 survivors before, **1733 of 1910 killed and 75
    survivors** after, with the **159** swapped `fail(` calls falling to
    **zero**. The document cost **one** real token: § 6's shape line and example
    gained the labels at +8 and § 11's duplicated `slice(from:, to:)` paid −7.
    Five decided file ceilings rose by their measured lines, eight emission
    traces were re-blessed and proved to differ only in their `#line` numbers,
    and the seed was regenerated with the fixpoint verified.

    **What it did not close** is defect 026: a function type names no
    parameters, so the same swap is still silent through a function value and
    through a C callback.
