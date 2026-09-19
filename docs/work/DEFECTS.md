# DEFECTS — the compiler defects that are still open

Every item is a **measured** failure of the compiler on a program — a crash, a
wrong answer at exit 0, a silence where a message is owed — carrying its
reproducer, its cause where known, and what is owed. **Only open defects live
here**: a repaired one is ticked, gains a *The repair* section with the
measurements that prove it, and moves to `docs/records/done/`. A repair is owed
at the class and not at the witness, with a `tests/golden/fixedbugs/` case per
shape.

**The shape** is `.claude/rules/records.md` § The lists, and § A live list is a
preamble, a count and its items is why this preamble is fifteen lines. **The
next number is READ, never remembered** — `records/numbering` takes one above
the highest issued across this file and `docs/records/done/`. Who issued which
number since 2026-09-08, and why 014 exists twice, is
`docs/records/log/2026-09-16-2200-the-defect-register-leaves-the-list.md`.

Format: `- [ ] **NNN — <title>** | <what it does, in one line> | <where to look>`

*******************************************************************************
**OPEN: 1**

- [ ] **066 — a `ptr` lend has no lifetime rule, so C may keep the address past the frame** | a field's address handed to C outlives the binding it came from, and a later C call reads a dead frame at exit 0 | `selfhost/check/lending.hero`'s `field_lend_escapes`, `spec § 13`'s lease sentences

    **Origin:** panel 166's completeness critic, 2026-09-19. No seat found it and
    no route A–G touches it. Shipped at `ef7b013b` with the lend itself.

    **Reproducer**, nine lines, and C does the keeping:

        function lend_and_return()
            s: Sl2 @ mk()
            keep(p: s.name.ptr())      # C parks the address in a static

        function main()
            lend_and_return()          # the frame dies here
            print(to_str(later()))     # C reads it anyway

        C reads the dead frame: 1      exit 0, no diagnostic
        --sanitize: AddressSanitizer: stack-use-after-scope

    **What the compiler does check, and why it is not enough.** It refuses the
    two escapes **Heroes** can see — `field_lend_escapes` and
    `field_lend_needs_a_place`, both re-run — and nothing looks at the C side.
    `sqlite3_bind_text` with a null destructor is declared in this repository at
    `examples/ledger/db/sqlite.hero:107`, which is the same shape with a real
    library behind it.

    **What is owed, and the language already owns the answer shape.** `spec § 13`:
    *"`x: cstr @ s.lease()` is a COPY of the bytes that C may read for as long as
    the program says, and `end_lease(@x)` frees it"*, and *"a lease nobody ends …
    aborts when `main` returns, saying how many"*. Panel 164 opened the address
    route and did not carry the lifetime rule across with it. Either the lend
    gains one, or the document says the address dies with the frame and the
    program is on its own — and the second is the shape panel 166's seats vetoed
    for the write direction.

*******************************************************************************
