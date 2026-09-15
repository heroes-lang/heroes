- [x] **043 — the specification's only FFI example teaches the omission it exists to prevent** | `sqlite3_open` and `sqlite3_close` are an acquire-and-release pair and the example carried neither `acquires` nor `consumes`, so a reader copied a binding that leaks in silence | **repaired 2026-09-15**, panel 153 R1, ratification pending | `spec/heroes-spec.md` § 13's fenced example · `docs/design/design.md` §4.19's own copy of it · `docs/measurements/010-spec-budget-ledger.md` row 5989

    **Origin:** found 2026-09-15 by **panel 152's llm-ergonomist**, which named
    the example as the CAUSE of its own predicted failure rate rather than as a
    coincidence, while it was answering a question about something else.

    **What landed.** The fence gains the two words the section's own rules ask
    for — `@out: Db acquires sqlite3_close` and `db: Db consumes` — and
    `design.md` §4.19's own copy of the same fence, which carried the identical
    bare pair, is corrected with it on the spec-warden's note.

    **What paid, and it is the sentence the defect is about.** *Unmarked pointers
    are never freed.* is removed from § 13. Panel 153's llm-ergonomist read the
    section with each fence in place, ten independent readings each, and counted
    how many omitted a mark the section requires: **8 of 10 under the fence as it
    stood, six of them compiling in silence; 2 of 10 under the corrected fence.**
    **Two of those twenty readings took that very sentence as permission to omit
    the marks**, so the removal both pays the bill and lowers the rate it is
    paying for. Its content survives: `owned` says what the compiler frees,
    `acquires` and `consumes` say what the program owes, and the abort at `main`
    return catches the handle nobody gave back.

    **The measurement, on the program rather than on the prose.** The same
    forgetful reader — opens `:memory:`, prints the code, never closes — built
    against each fence with the same compiler:

    | the fence the reader copied | what the program does |
    |---|---|
    | as § 13 carried it | prints `0`, **exit 0, stderr empty**, the database leaks |
    | with the two marks | prints `0`, then `panic: 1 C handle(s) never given back … The first is at 0x1030202c0`, **exit 134** |

    **The precondition is met by the instrument and not by a reading.**
    `.claude/rules/spec-shape.md` asks that a change to a fence be COMPILED
    before it is written, and `tests/harness/suite_special.hero` builds the
    section's first fence into a program and runs it against real SQLite:
    `special` **10 passed, 0 failed** on the corrected text.

    **The price, and the prediction it scores.** **+0 real and +0 vendored** —
    7974 stays 7974 on `claude-opus-5`, the vendored maximum stays 5989, digest
    `21a9dc541cfa2fa8`. Panel 153's spec-warden predicted 7974 real, 5990
    vendored and digest `2ec6dad90753bed2` for this package, so the prediction
    **SPLIT**: the real figure, which is what the ceiling judges, held exactly;
    the vendored figure came in one token cheaper and the digest differs, which
    by the seat's own clause means the landed text is not byte-identical to the
    text priced. It is said here rather than rounded away.

    **The sitting's own question, answered.** *May a section's one worked example
    be incomplete on the section's own rule?* The spec-warden's answer is no, and
    it names the instrument that makes it enforceable: `special/the spec's
    example` compiles that fence as a program, so an example the section's rules
    would refuse is an example the harness refuses.
