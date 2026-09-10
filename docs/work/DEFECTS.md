# DEFECTS — the compiler defects that are still open

Read by whoever attacks a defect. Every item is a **measured** failure of the
compiler on a program — a crash, a wrong answer at exit 0, a silence where a
message is owed — carrying its reproducer, its cause where known, and what is
owed. The file exists by author instruction 2026-09-03: one file inside
`docs/work/`, so that everything is tidy.

**Only open defects live here.** The moment one is repaired its entry is ticked,
gains a *The repair* section with the measurements that prove it, and moves to
`docs/work/DONE.md`, the record (CLAUDE.md §3). A repair is owed at the class
and not at the witness, with a `tests/golden/fixedbugs/` case per shape.

**The shape.** One line per item, then the body indented four spaces — a
reproducer, a cause and a measurement are the entry, not decoration. Nothing
lives outside the two banners, and this file is why `records/lists` exists: it
had grown 2753 bytes of prose about five already-repaired defects, every one of
them already in the record.

**Numbers are never reused, and 014 was issued twice** — `docs/work/DONE.md`
carries a Windows-diagnostic defect and an FFI-boundary defect both numbered
014, filed a day apart. A record is not rewritten (CLAUDE.md §14), so the
collision stands there; the next number to issue is **025** (022 and 023 were issued on 2026-09-08, 022 was SPLIT on 2026-09-09 by panel 122 into 022 and 024, and all three closed the same day, 024 last, at M-held-bytes) — 017 to 021 were
all issued on 2026-09-08 and all closed on 2026-09-08, and all five are in the
record.

Format: `- [ ] **NNN — <title>** | <what it does, in one line> | <where to look>`

*******************************************************************************
**OPEN: 1**

- [ ] **025 — the label rule stops at the compiler's own built-ins** | `fail("a", "b")` and `xs.slice(1, 3)` compile, though both take two parameters of one type, while `range(1, 4)` and every function written in Heroes are refused | `spec § 9 Functions and calls` · `selfhost/check/builtins.hero` · `selfhost/library_source.hero`

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

*******************************************************************************
