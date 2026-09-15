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
collision stands there; the next number to issue is **037** (022 and 023 were issued on 2026-09-08, 022 was SPLIT on 2026-09-09 by panel 122 into 022 and 024, and all three closed the same day, 024 last, at M-held-bytes; **025** and **026** were issued and closed on 2026-09-11 at M-labelled-builtins and M-named-callbacks, **027** was issued 2026-09-11 beside panel 131 and **028** beside panel 132, and **029** was issued 2026-09-13 beside panel 135 and **030** the same day beside panel 137, **031** beside panel 139; **032** was issued and closed on 2026-09-14 at M-cleanup-verdict step 1, and it is the first since 025 that no sitting produced — the census that opens this milestone found it, and the compiler simply disagreed with `spec § 13`; **033** was issued on 2026-09-14 at the M-marked-acquisition close, by the full net before the push, and it is the first here found by attacking the shapes NEXT to a repair rather than the repair itself; **034** was issued on 2026-09-14 by panel 149's spec-warden while it was pricing 033's repair, and it is the first here that a SITTING CONVENED ON ANOTHER DEFECT found — the seat went looking for what the specification already said and found the compiler disagreeing with it somewhere nobody had asked about; **035** and **036** were issued the same day by the same sitting, 035 by its compiler-engineer and 036 by its completeness critic, so **panel 149 produced three defects while ruling on a fourth** and **039**, **040** and **041** were issued on 2026-09-15 by panel 150, which was convened on 037 and 038 and produced three more while ruling on them — 039 by its ffi-pragmatist, 040 and 041 by its completeness critic, and 039 is the first here that reaches a mechanism two earlier sittings built rather than a gap they left; the next number to issue is **044** — **042** and **043** were issued on 2026-09-15 by panel 152, both by its llm-ergonomist, which was asked about a qualifier and found instead that the language cannot bind C's commonest struct shape and that the document's one example teaches the omission its own section forbids — **037** and **038** were both issued on 2026-09-15 at panel 149's ratification, out of what that sitting had NAMED and not filed: 037 is the `ptr` blind spot its ffi-pragmatist called the more serious of the two holes it looked at, and 038 is the per-element release its completeness critic found no seat had been briefed on) — 017 to 021 were
all issued on 2026-09-08 and all closed on 2026-09-08, and all five are in the
record.

Format: `- [ ] **NNN — <title>** | <what it does, in one line> | <where to look>`

*******************************************************************************
**OPEN: 2**

- [ ] **042 — spec § 13 cannot bind a struct that is both READ and POINTED AT, which is the shape C uses most** | the fielded record and the handle record would both need `tag addrinfo`, and two records may not name one tag, so `getaddrinfo` has no writable binding at all | `spec/heroes-spec.md` § 13 · `selfhost/check/decls.hero`'s `one_tag_one_type`

    **Origin:** found 2026-09-15 by **panel 152's llm-ergonomist**, reading only
    the specification, while answering a question about something else. It calls
    it *"a language finding rather than an ergonomics one, and it is worth more
    than the qualifier question"*.

    **The shape, and it is everywhere.** `getaddrinfo` hands back
    `struct addrinfo **`, which needs `record AI tag addrinfo` with no fields —
    the handle. Reading `ai_family` off what comes back needs
    `record AddrInfo tag addrinfo` WITH fields. Both carry `tag addrinfo`, and
    `spec § 13` says *"two records may not name one tag"*. **So the binding as a
    whole is unwritable.**

    **The three ways out are all closed by text the seat quoted.** A self-pointer
    field `ai_next: AddrInfo` is refused by `spec § 3` — a record holding itself
    by value has no size. `ai_next: ptr` is admitted and then **nothing in the
    language turns a `ptr` back into an `AddrInfo`**: no dereference, no cast, no
    field access. The list head arrives and the list cannot be walked. And
    passing the fielded record where C wants `const struct addrinfo *` needs an
    address-of the language does not have, since `@` is licensed for
    out-parameters only.

    **It is not one header's quirk**: `struct stat` under `lstat`, and every
    linked list in every C header, are the same shape.

    **What is owed**: a sitting. `one_tag_one_type` was narrowed deliberately at
    panel 145 — *"two Heroes types over one C type is a hole nothing can close"* —
    so widening it is a design question and not a repair, and the answer may be a
    different construct rather than a wider rule.

- [ ] **043 — the specification's only FFI example teaches the omission it exists to prevent** | `sqlite3_open` and `sqlite3_close` are an acquire-and-release pair and the example carries neither `acquires` nor `consumes`, so a reader copies a binding that leaks in silence | `spec/heroes-spec.md` § 13's fenced example

    **Origin:** found 2026-09-15 by **panel 152's llm-ergonomist**, which named
    the example as the CAUSE of its own predicted failure rate rather than as a
    coincidence.

    **The example, verbatim from `spec § 13`:**

    ```
    extern "sqlite3.h" link "sqlite3"
        constant SQLITE_OK: i64
        record Db tag sqlite3
        function sqlite3_open(path: cstr, @out: Db) -> i64
        function sqlite3_close(db: Db) -> i64
    ```

    **`sqlite3_close` ends a database's life and does not say `consumes`.
    `sqlite3_open` begins one and does not say `acquires`.** The seat's
    prediction: **at least 8 in 10** models omit both on this shape, following
    the example, and *"that omission compiles, runs, leaks, and produces no
    diagnostic and no abort"*.

    **Why nothing fires, and it is the machinery working as designed.**
    `check/acquiring.hero` arms only where some `extern` declares `consumes`. The
    example declares none, so `consumed_types` is empty, `bindings_say_which`
    returns at its first line, and the runtime counter is never incremented
    either. Three sittings built a mechanism the document's own example steps
    around.

    **PRICED 2026-09-15, so the sitting starts from a number rather than from a
    guess.** The repair is two words — `@out: Db acquires sqlite3_close` and
    `db: Db consumes` — and it costs **+7 on the vendored ranks**, base 5988 to
    5995. The binding `real` figure was not taken, because taking it means
    writing the draft to the spec path and refreshing the pinned record, which is
    the change itself.

    **It is not landed here, and the reason is the contract rather than caution.**
    It is spec text, so design.md §1.6's payment rule applies: an addition owes a
    named removal or a registered prediction. A removal IS available and panel
    152's spec-warden already measured it — dropping *"what the header leaves
    opaque"* is **−3 real** — but that seat has already allocated it to panel
    150's unpaid correction, so spending it twice is the arithmetic a sitting
    exists to do.

    **THE PRECONDITION IS MET, 2026-09-15**: `.claude/rules/spec-shape.md` asks
    that a change to a fence be COMPILED before it is written, because an example
    in the one document a reader is told to trust is a claim and an unexecuted
    claim expires in silence. The corrected example was built and run against
    real SQLite:

    ```
    function sqlite3_open(path: cstr, @out: Db acquires sqlite3_close) -> i64
    function sqlite3_close(db: Db consumes) -> i64
    ```

    opened `:memory:`, closed it, printed, **exit 0**. So the sitting inherits a
    claim that has been executed, which is the one thing this defect is about:
    the example as it stands was never run against its own section's rule.

    **What is owed**: the example gains the two marks. It is spec text, so it is
    priced and it is the panel's, and the same sitting should ask whether the one
    worked example of a section may be incomplete on the section's own rule.

*******************************************************************************
