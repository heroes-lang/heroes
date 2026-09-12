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
collision stands there; the next number to issue is **030** (022 and 023 were issued on 2026-09-08, 022 was SPLIT on 2026-09-09 by panel 122 into 022 and 024, and all three closed the same day, 024 last, at M-held-bytes; **025** and **026** were issued and closed on 2026-09-11 at M-labelled-builtins and M-named-callbacks, **027** was issued 2026-09-11 beside panel 131 and **028** beside panel 132, and **029** was issued 2026-09-13 beside panel 135) — 017 to 021 were
all issued on 2026-09-08 and all closed on 2026-09-08, and all five are in the
record.

Format: `- [ ] **NNN — <title>** | <what it does, in one line> | <where to look>`

*******************************************************************************
**OPEN: 1**

- [ ] **029 — a swapped opaque handle compiles at zero diagnostics and segfaults** | `examples/sqlite/main.hero` with line 74 changed from `sqlite3_step(statement)` to `sqlite3_step(db)` builds with no diagnostic and exits 139, because `sqlite3 *` and `sqlite3_stmt *` are both `ptr` and the probe passes a `void *` C converts in silence | `docs/panel/135-the-form-was-cheap-and-the-reasons-under-it-were-borrowed.md` § Found beside the sitting · `examples/sqlite/main.hero:46-48,74` · `selfhost/emit/callback_guard.hero` · design.md §1.12, §4.19

    **Origin:** 2026-09-13, panel 135's ffi-pragmatist, measuring what a
    transparent alias buys the boundary (nothing against this class); rebuilt
    from source and re-run by the coordinator in the seat's copy before filing.

    **Reproducer.** Copy `examples/sqlite/main.hero`, change line 74 to
    `if sqlite3_step(db) == SQLITE_ROW`, `heroes build` it: **exit 0, zero
    diagnostics**. Run it: **exit 139**. The unchanged example built with the same
    binary prints `rows: 3` / `longest: 6` at exit 0. Measured on Darwin arm64,
    clang 21, the SDK's `sqlite3.h`; the other two platforms are *unrun*.

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

*******************************************************************************
