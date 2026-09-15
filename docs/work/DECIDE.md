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

- [ ] **panel 154** | The null dereference at the C boundary is closed by a compile flag; is route A's guard worth landing for the BLAME LINE alone, and does route B's fourth word ever earn itself? | `docs/panel/154-the-class-was-never-argument-shaped-and-one-flag-closed-it.md` R3 and R4

    **Origin:** panel 154, 2026-09-15, adopted provisionally and in force as the
    default. Four of five seats converged on route B, a word permitting null on
    the parameters where C accepts it, and the sitting refused that convergence
    because its completeness critic measured a route nobody had listed: one
    string in `selfhost/cli/flags.hero`, `-fno-delete-null-pointer-checks`,
    which closes every shape the sitting could measure at zero surface, zero
    spec tokens and zero refusals of legal C, because it guards the dereference
    rather than the argument.

    **What the default costs while this is open.** The class is closed and the
    message is poorer than it could be: under the flag at `-O2` the panic says
    `called from main` where at `-O0` it says the C function's own caller,
    because inlining flattened the frame. That is the one thing route A's guard
    buys, measured, and it is a diagnostic cost rather than a correctness one.

    **Recommendation**: land route A's guard when a milestone touches
    `emit/ops.hero` anyway — its patch exists, formatted and passing `canonical`,
    `layout` and `order`, 31 insertions across 4 files, timed at 10^8 iterations
    with medians identical — and leave route B's word unwritten until a SECOND
    shipped program is refused by the guard. Exactly one exists today, and the
    rule the sitting was held to is the compiler-engineer's own: two is a class
    and one is a witness. Against it: four seats adopted B on its merits, and a
    reader of the specification cannot see a flag.


- [ ] **panel 153** | Does the LANGUAGE need a way to read the struct behind a pointer C hands back, when a header of the author's own already does it at zero language cost? | `docs/panel/153-the-option-set-was-short-by-one-and-the-route-nobody-listed-already-ships.md` R2 · `docs/work/DEFECTS.md` 045

    **Origin:** panel 153, 2026-09-15, adopted provisionally and in force as the
    default. The sitting's Q1 was put as A-or-B and its completeness critic
    measured a third answer nobody had listed: three lines of header, a
    `static inline` returning the struct by value, and the SHIPPED compiler reads
    `ai_family` and walks `ai_next` with output byte-identical to the equivalent
    C program. No compiler change, no new word, no spec token.

    **What the default costs while this is open.** The read works and the null
    read segfaults at exit 139 (defect 045, filed and repaired on its own clock),
    and nothing in `spec/heroes-spec.md` tells a reader that the route exists, so
    a model reading the prompt meets a wall the language does not actually have.

    **The decision, in one line**: the built-in buys a null guard and a spec
    sentence at +77 real; the header buys the same read at zero. Is the guard
    worth a new primitive, or is the guard the thing to build with no new
    construct at all? Three sub-questions ride with it: is a `.h` beside a
    `.hero` a supported input class against `.claude/rules/cli-surface.md`'s
    refusal of a fourth, what suite judges one, and does it need a spec sentence.

    **Recommendation**: build the guard, not the primitive. It reaches every
    binding rather than only the ones written through a new construct, it is what
    §1.12 asks for by name, and it leaves the read where §1.11 says everything in
    this language comes from. Against it: the historian objected to copy-out as
    the ONLY read path, and a route the specification does not mention is a route
    a reader of the prompt cannot find.


*******************************************************************************
