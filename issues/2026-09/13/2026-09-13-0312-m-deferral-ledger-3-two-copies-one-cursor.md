- [ ] **M-deferral-ledger 3** | The specification says *"Every value behaves as an independent copy: after `b = a`, mutating `b` never changes `a`. No aliasing exists anywhere."* Write the shortest program you can that makes that sentence false, and say which single field type is what makes it possible. Then say why the repair is a sentence in the specification rather than a change to the compiler, and which rule of the operating contract decides that.

    **Where to look:** `spec/heroes-spec.md` § 3; `docs/work/DEFECTS.md` item 030
    and its reproducer; `examples/ledger/db/sqlite.hero:287`, which ships the
    shape; CLAUDE.md § 12's first sentence about the spec and the compiler.

    **Why it matters:** this is the one sentence a reader of Heroes is most
    confident about, and it is the language's whole argument against a borrow
    checker. It is true of everything Heroes owns and false of an address Heroes
    borrowed, and the document does not say which it means. The function that
    ships this shape takes its argument **by value**, not with `@`, so its
    signature promises it changes nothing — which is precisely what `@` exists to
    make visible.
