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
**OPEN: 2**

- [ ] **063 — an overstated extent on `f.ptr()` corrupts memory at exit 0** | `f.ptr()` lends a field's address and the CALL states the extent, and nothing checks that number against the field: a larger one reads and WRITES past the field, silently | `selfhost/check/lend_types.hero`, `spec § 13`'s `f.ptr()` sentence

    **Origin:** panel 165, 2026-09-19, found independently by the spec-warden
    and the ffi-pragmatist while pricing route 6, and the write half by the
    completeness critic. Shipped at `ef7b013b` (defect 061's repair), so it is
    eight commits old.

    **Reproducer**, an `i8[8]` field and a C function told it is longer:

        s: Sl @ sl_make()                  # struct sl { char name[8]; int id; }
        print(to_str(s.id.to_i64().must()))       # 7
        sl_fill(p: s.name.ptr(), n: 64)
        print(to_str(s.id.to_i64().must()))       # 1094795585 == 0x41414141

    Read direction: `sum_n(p: t.name.ptr(), n: 4096)` returns a sum over 4096
    bytes, exit 0. Under `./heroes run --sanitize`, `AddressSanitizer:
    stack-buffer-overflow`, `This frame has 1 object(s): [32, 44) 'h0_t' <==
    Memory access at offset 44 overflows this variable`.

    **Why it is rank 3 and not a price.** design.md §1.12 says a Heroes program
    must not corrupt memory, and this corrupts a sibling field of a record the
    program owns, with every suite green. CLAUDE.md § Precedence puts that above
    elegance, token cost, ergonomics, compiler size and speed.

    **What is owed.** A refusal at the class, not at the witness: the extent
    argument is checked against the field's own extent wherever it is a
    compile-time constant, and the shapes beside it are attacked — a literal, a
    header `constant`, an expression, a value read at run time, `[]` and a
    nested field. The ffi-pragmatist's cheaper half is worth pricing first:
    `t.name.len()` as a compile-time constant, so the honest call needs no
    literal at all.

    **CORRECTION, 2026-09-19, by the session that wrote the line above.** The
    first sentence is **not implementable as written**, measured by attempting
    it: **nothing declares which argument is the extent.** `sum_n(p: ptr, n:
    i64)` is two independent parameters and the compiler has no relation between
    them to check, so there is no *"the extent argument"* to find. What is
    actually owed is one of two things, and both are sittings: **route 13**,
    declaring on the parameter which sibling carries the extent, which is the
    only form where the number can be checked at all; or the `len()` widening,
    which removes the literal without checking anything and is a change to
    `spec § 11`'s built-in.

- [ ] **065 — C writes into an IMMUTABLE binding, with no `@` anywhere** | a field lent with `f.ptr()` from a binding declared `=` is written by C and the program reads the new bytes back, falsifying `spec § 3` and `spec § 5` at exit 0 | `spec § 5`'s `=` sentence, `selfhost/check/lend_types.hero`

    **Origin:** panel 165's completeness critic, 2026-09-19. No seat found it,
    and the shipped golden `tests/golden/run/ffi-a-byte-field-crosses-to-c.hero`
    demonstrates the write as correct behaviour — on a `@` binding, which is why
    it never showed.

    **Reproducer**, and `t` is bound with `=`:

        t = sl_make()
        print(to_str(t.name[0].to_i64().must()))   # 72
        sl_fill(p: t.name.ptr(), n: 8)             # no @ on the binding,
        print(to_str(t.name[0].to_i64().must()))   # none on the parameter,
                                                   # none at the call site
        before: 72
        after : 65

    **What it falsifies, in the document a reader is told to trust.** `spec § 5`:
    *"`=` binds once, forever"* and *"only a declared `@` name can be mutated"*.
    `spec § 3`: *"Every value behaves as an independent copy."* Both are false
    for this program and both are what the language sells.

    **What is owed, and it is a question this entry does not answer.** Either
    the lend of a field to a `ptr` a C function may write requires `@` at the
    binding, the parameter and the call site — which is `spec § 13`'s own
    sentence *"A C out-parameter is an `@` parameter"* reaching one more type,
    and is what panel 165's llm-ergonomist held its veto for — or the language
    says in writing that a `ptr` lend is a hole in `=`. The first is a language
    change and owes a sitting.

*******************************************************************************
