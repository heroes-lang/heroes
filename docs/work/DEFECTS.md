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

- [ ] **060 — a header record with a `const` member makes the compiler blame itself** | `internal error: compiling the generated C failed` at exit 2 for an `extern` the author wrote, where `.claude/rules/c-boundary.md` asks for exit 1 on the `.hero` line | `selfhost/emit/ffi.hero` · `selfhost/cli/produce.hero`

    **Origin:** panel 161's ffi-pragmatist found it beside the `char` question, 2026-09-17; reproduced by the coordinator on Darwin arm64 before filing.

    **The reproducer**, two files, and it needs no arm64:

    ```c
    typedef struct { const int32_t id; int32_t n; } Locked;
    static inline int32_t locked_id(Locked l) { return l.id; }
    ```
    ```
    extern "cm.h"
        record Locked
            id: i32
            n: i32
        function locked_id(l: Locked) -> i32

    function main()
        x = Locked(id: 7, n: 1)
        print(x.locked_id().to_i64().must())
    ```

    `heroes build` answers `internal error: compiling the generated C failed`,
    three clang warnings about `default initialization of an object of type
    'Locked' with const member`, and `error: cannot assign to variable 't3' with
    const-qualified data member 'id'`.

    **What is wrong is the VERDICT, not the refusal.** A C struct with a `const`
    member cannot be assigned after declaration, so refusing the program is
    right. Saying *internal error* is not: `.claude/rules/c-boundary.md` names
    one class of clang failure that belongs to the author's own `extern`, and
    the mechanism for it already exists — `cli/produce.hero`'s `blamed()` asks
    `emit/ffi.hero`'s `explain()`, which walks clang's stderr and hands back
    diagnostics on the `.hero` line. **No reader there recognises this message**,
    so `explain` returns nothing and the failure falls through to exit 2. Today
    the author is told the compiler is broken.

    **What it needs is a decision and not only a reader.** `spec § 13` has the
    shape already: a `partial` record is bindable, and `==` and map-key use on it
    are compile errors. A record with a `const` member is the same kind of thing
    — bindable and readable, not constructible from Heroes — and saying which
    operations it refuses is a language question rather than a message.

    **The shapes beside it, one of them already measured** (CL-061): a `const`
    member in a `partial` record, a `const` member never constructed from Heroes,
    a nested record holding one, and a `const` ARRAY member — panel 161's
    ffi-pragmatist measured that `const char[N]` is refused at every spelling and
    that **it is the `const` and not the `char`**, since `const int32_t n[4]` is
    refused too. Whether those refusals carry a good message or this same
    internal error is **unrun**.

*******************************************************************************
