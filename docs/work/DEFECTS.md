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
collision stands there; the next number to issue is **031** (022 and 023 were issued on 2026-09-08, 022 was SPLIT on 2026-09-09 by panel 122 into 022 and 024, and all three closed the same day, 024 last, at M-held-bytes; **025** and **026** were issued and closed on 2026-09-11 at M-labelled-builtins and M-named-callbacks, **027** was issued 2026-09-11 beside panel 131 and **028** beside panel 132, and **029** was issued 2026-09-13 beside panel 135 and **030** the same day beside panel 137) — 017 to 021 were
all issued on 2026-09-08 and all closed on 2026-09-08, and all five are in the
record.

Format: `- [ ] **NNN — <title>** | <what it does, in one line> | <where to look>`

*******************************************************************************
**OPEN: 2**

- [ ] **029 — a swapped opaque handle compiles at zero diagnostics and segfaults** | `examples/sqlite/main.hero` with line 74 changed from `sqlite3_step(statement)` to `sqlite3_step(db)` builds with no diagnostic and exits 139, because `sqlite3 *` and `sqlite3_stmt *` are both `ptr` and the probe passes a `void *` C converts in silence | `docs/panel/135-the-form-was-cheap-and-the-reasons-under-it-were-borrowed.md` § Found beside the sitting · `examples/sqlite/main.hero:46-48,74` · `selfhost/emit/callback_guard.hero` · design.md §1.12, §4.19

    **Origin:** 2026-09-13, panel 135's ffi-pragmatist, measuring what a
    transparent alias buys the boundary (nothing against this class); rebuilt
    from source and re-run by the coordinator in the seat's copy before filing.

    **Reproducer.** Copy `examples/sqlite/main.hero`, change line 74 to
    `if sqlite3_step(db) == SQLITE_ROW`, `heroes build` it: **exit 0, zero
    diagnostics**. Run it: **exit 139**. The unchanged example built with the same
    binary prints `rows: 3` / `longest: 6` at exit 0. Measured on Darwin arm64,
    clang 21, the SDK's `sqlite3.h`.

    **2026-09-13, the second platform run, and it makes this defect WORSE rather
    than confirming it.** The line above said the other two platforms were unrun;
    the author put the Linux container up and it was run. On x86-64 Linux (Debian
    13, clang 22, glibc 2.41) the same swapped program **builds clean AND runs to
    completion at exit 0**, printing `rows: -1` and `longest: -1` where the
    unchanged example prints `rows: 3` / `longest: 6`. So the class is not a crash:
    **on Darwin it is a loud segfault and on Linux it is a wrong answer at exit 0**,
    which is the shape this list names first and the hardest to notice. A reader
    who met it on a Mac would file a crash; a program shipping on Linux returns a
    number nobody checks. **Windows stays unrun and is not inferred**: that box has
    no sqlite3 (`pkg-config` absent, measured the same night).

    **Cause.** Every C pointer that is not a `cstr` is one Heroes type, `ptr`
    (spec § 3). The extern's probe is `(void)(sqlite3_step)(a0)` with `a0` a
    `void *`, so clang has nothing to compare: the header's `sqlite3_stmt *` and
    the program's `sqlite3 *` meet at a parameter C converts without a word.
    `examples/sqlite` has two pointee types collapsing to `ptr` and six call sites
    that hand one on (`main.hero:56,74-76,82,89`); `heroes mutate`'s `swap-args`
    swaps a label with its value and cannot produce this mutant.

    **What is owed.** A sitting on the class and not a patch at the witness: the
    repair is a fact about `ptr`, not about `alias` — the *distinct types* door
    design.md Part 7 item 5 keeps open, and the one the FFI seat asked for by
    name. The boundary's needs, stated by that seat and not designed: the same C
    spelling as `ptr` (`void *`, 8 bytes, no box), `nullptr` as its literal, an
    `@` out-parameter still emitted `(void *)&x`, the `_Static_assert` probe
    unchanged, and `Db` refused where `Stmt` is written so the build above fails
    at `main.hero:74` instead of at run time. Panel 109 refused `ptr owned` as a
    new `Ty` case (179 arms in 49 files); the sitting owes the cheaper route or
    the reason there is none. design.md §1.12 says a Heroes program must not
    segfault, and `.claude/rules/c-boundary.md` says that goal wins here first.

- [ ] **030 — the specification's promise about copies is false through a `ptr` field** | two copies of one record holding a `sqlite3_stmt *` advance the SAME C cursor: `a` sees row 1, `b` sees row 2, and `a.handle == b.handle` — while spec § 3 says *"Every value behaves as an independent copy … No aliasing exists anywhere"* | `spec/heroes-spec.md` § 3 · `examples/ledger/db/sqlite.hero:287` · `docs/panel/137-the-hole-was-two-operations-wide-and-the-answer-was-a-library-function.md` § Found beside the sitting

    **Origin:** 2026-09-13, panel 137's ffi-pragmatist, measuring what a
    structural iteration rule would cost at the C boundary. Found while testing
    something else, and it is independent of everything that sitting ruled on.

    **Reproducer.** A `record Statement` with one field, `handle: ptr`, holding a
    `sqlite3_stmt *`. Bind `a: Statement @ st`, then `b: Statement @ a`, then
    advance each once through `sqlite3_step`. Measured against a real in-memory
    database on Darwin arm64: **`a sees id 1`, `b sees id 2`, and
    `a.handle == b.handle after the copy: true`** — `b` was copied BEFORE `a`
    advanced and still sees row 2. Hand-written C of the same shape under
    `-Wall -Wextra` is accepted at zero warnings and gives the identical answer.

    **Cause.** The copy copies the address, and the state is in C. Every Heroes
    value is an independent copy of what Heroes owns; a `ptr` field owns nothing,
    so two copies reach one C object. **This is not hypothetical and not new**:
    `examples/ledger/db/sqlite.hero:287` ships `stepped(statement: Statement)`,
    which takes the wrapper **by value**, not `@`, and mutates the C cursor
    through it — a function whose signature promises it changes nothing.

    **What is owed.** Not a compiler change: refusing this would refuse the FFI.
    What is wrong is the sentence, and it is the one a reader is most confident
    about, so the repair is **a panel**: § 3 must say that a `ptr` and a `cstr`
    are copied ADDRESSES and that two copies of a value holding one reach the
    same foreign state. CLAUDE.md § 12 says spec beats compiler, which is exactly
    why a false sentence in the spec is the defect rather than the compiler's
    behaviour. Until it says so, `@` on a parameter is not what tells a reader
    whether a call can change what they passed.

*******************************************************************************
