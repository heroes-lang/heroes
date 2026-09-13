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
collision stands there; the next number to issue is **032** (022 and 023 were issued on 2026-09-08, 022 was SPLIT on 2026-09-09 by panel 122 into 022 and 024, and all three closed the same day, 024 last, at M-held-bytes; **025** and **026** were issued and closed on 2026-09-11 at M-labelled-builtins and M-named-callbacks, **027** was issued 2026-09-11 beside panel 131 and **028** beside panel 132, and **029** was issued 2026-09-13 beside panel 135 and **030** the same day beside panel 137, **031** beside panel 139) — 017 to 021 were
all issued on 2026-09-08 and all closed on 2026-09-08, and all five are in the
record.

Format: `- [ ] **NNN — <title>** | <what it does, in one line> | <where to look>`

*******************************************************************************
**OPEN: 1**

- [ ] **031 — one copy of a value can free what every other copy holds, at exit 0 with no diagnostic** | a `record Holder { cell: ptr }`, a copy, `free` through the copy, then a write through the original's own field: builds clean, runs to exit 0, and AddressSanitizer says `heap-use-after-free, WRITE of size 8` | `spec/heroes-spec.md` § 13's *"Unmarked pointers are never freed"* · `examples/ledger/db/sqlite.hero:225`, `:270` · `docs/panel/139-the-sentence-was-false-and-so-were-four-of-its-neighbours.md` § Found beside the sitting

    **Origin:** panel 139, 2026-09-13. The spec-warden named it as a suspect while
    enumerating the neighbours of defect 030's false sentence and proposed a test;
    the coordinator wrote and ran it, so the entry is measured rather than argued.

    **Reproducer**, and it is eight lines. Bind `a: Holder @ Holder(cell: malloc(size: 8))`,
    copy it with `b: Holder @ a`, call a function `closed(h: Holder)` that does
    `free(h.cell)` — passing `b` — then write through `a.cell`. **Build exit 0,
    zero diagnostics. Run exit 0**, both prints reached. Under `--sanitize`:
    `ERROR: AddressSanitizer: heap-use-after-free on address 0x6020000000f0`,
    `WRITE of size 8`, naming the `.hero` line.

    **Measured on two platforms the same night, and — unlike defect 029 — it does
    not change class.** Darwin arm64 and x86-64 Linux (Debian 13, clang 22, glibc
    2.41) both build at exit 0 with zero diagnostics, both reach both prints, both
    exit 0, and both report `heap-use-after-free, WRITE of size 8` under
    `--sanitize`, each naming `uaf.hero:22`. That matters because 029's reproducer
    **does** change class — a segfault on Darwin, a wrong answer at exit 0 on
    Linux — so a reader must not assume the family behaves uniformly. Windows is
    **unrun** and is not inferred.

    **Cause, and why it is the sharper twin of 030.** 030 is *two copies reach one
    foreign thing*; this is *one copy can destroy what the other holds*. Both come
    from a `ptr` carrying neither identity nor ownership, and neither the checker
    nor the ownership pass models a foreign lifetime. The signature is what makes
    it dangerous: `closed(h: Holder)` takes its argument **without `@`**, which
    spec § 9 makes a promise that nothing the caller passed is changed — and this
    one invalidates it for every copy in the program.

    **Ruled 2026-09-13 at panel 145 (M-handle-verdict step 2): WART adopted, defect
    NOT closed.** design.md Part 8 wart 20 states the cost in the present tense and
    the remedy priced: a consume mark on a C parameter (not `owned`, which means
    the opposite direction) with the rule *consume through a borrowed parameter is
    refused* — which refuses THIS reproducer as written at line 14, the `free`
    inside `closed(h: Holder)`, and refuses 2 of 17 correct calls in the shipped
    binding, both repairable by `@`. **What it cannot refuse is the class**: the
    copy `b: Holder @ a`, whose refusal is an affine handle, core and unpriced —
    owed a count at step 5. The critic asked whether a wart is admissible for a
    §1.12 violation at exit 0 and the sitting answered: only where §4.10 already
    places `ptr` outside the guarantee, and only while the cheapest guard is being
    built. **This defect closes when the step-5 prototype refuses this program in
    `heroes check` at ≤ 150 code lines, or when the author ratifies the wart with
    that measured cost in front of them.** Until then it stays here.

    **It is shipped, not synthetic.** `examples/ledger/db/sqlite.hero:225`
    `function closed(db: Db) -> i64` and `:270` `function finalized(statement: Statement) -> i64`
    are exactly this shape, by value, handing C a pointer it frees.

    **What is owed, and what will NOT do.** Not a repair at the witness: the class
    is the language's, and panel 139 measured that a rule keyed on *reaches C
    through a `ptr`* cannot be written, because an opaque-handle header does not
    const-qualify its readers — `sqlite3_column_count` is not const, compiled — so
    such a rule would refuse 13 of 17 functions in the shipped binding, four of
    which mutate nothing, and CLAUDE.md § 12's burden on a refusal could not be
    met. The sitting that owns this is **M-handle-verdict**, whose subject is what
    `ptr` carries; panel 139's sentence tells a reader it can happen, which is
    worth having and is not the repair. **§1.12 is a goal of the language and
    CLAUDE.md § Precedence ranks it above cost**, so this defect does not close by
    being documented.

*******************************************************************************
