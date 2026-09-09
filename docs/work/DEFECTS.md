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
collision stands there; the next number to issue is **024** (022 and 023 were issued on 2026-09-08, and 022 was SPLIT on 2026-09-09 by panel 122, which is why 024 exists before 022 is closed) — 017 to 021 were
all issued on 2026-09-08 and all closed on 2026-09-08, and all five are in the
record.

Format: `- [ ] **NNN — <title>** | <what it does, in one line> | <where to look>`

*******************************************************************************
**OPEN: 1**

- [ ] **024 — a lend the C side RETAINS past the call is a use-after-free that every position rule blesses** | every `.cstr()` is an argument of an extern call, the C function keeps the pointer, and the answer is silently wrong at exit 0 | `examples/ledger/db/sqlite.hero:305-309` · `design.md` §4.19's reserved keyword · `docs/panel/122-the-lend-was-two-defects.md`

    **Origin:** panel 122, 2026-09-09, built independently by the
    compiler-engineer and the ffi-pragmatist from opposite ends. Split out of
    defect 022 by that sitting's R5, because the rule 022 closes cannot reach
    this half and closing the milestone as though it could would put a false
    sentence in the record.

    **Two reproducers, both run.** A C side that keeps what it is handed:
    `static const char *held; void stash_put(const char *s) { held = s; }`,
    called in a loop with `stash_put(s: ("row-" + at.to_str()).cstr())` —
    accepted at exit 0 with zero diagnostics, prints `0` where 14 is the answer,
    and `--sanitize` reports heap-use-after-free freed by the owner slot's
    rebind. And the same class through SQLite, `sqlite3_bind_text(..., destructor:
    nullptr)`, which is SQLITE_STATIC and means *the string is static, keep it*:
    in a loop with `sqlite3_step` after it, `matched rows: 0` where 1 is right,
    freed at a `main.c` line with **no `.hero` position at all**.

    **Why no position rule reaches it.** The discriminator is the C function's
    own contract — SQLITE_STATIC against SQLITE_TRANSIENT, the fifth argument —
    and both are `const char *` in the header. Nothing about where the Heroes
    expression stands distinguishes them.

    **The project already knew and wrote it as a rule for a human.**
    `examples/ledger/db/sqlite.hero:305-309` ships that shape with this above
    it: *"a caller must step before it drops the string, and every caller here
    does, in the next line. Written down because it is the one place a
    correct-looking rearrangement would be a use-after-free."*

    **What is owed.** design.md §4.19 reserves the vocabulary: *"a borrowed
    pointer you must not touch — Reserve a keyword"*, case 2 of three, of which
    `owned` is case 1 and landed at panel 109. So the repair is a declaration-site
    annotation on the extern's parameter, not a rule about Heroes positions, and
    it is a panel of its own. Until then the sound route is expressible and was
    run clean: declare `SQLITE_TRANSIENT` in the group and pass it.

    **Why it matters:** the measured owner slot is released at function exit **or
    when its site re-executes, whichever comes first**, so a loop frees the
    previous iteration's bytes while C still holds the pointer. This is the
    shape a working FFI program reaches for, on §4.19's own acceptance ladder.

*******************************************************************************
