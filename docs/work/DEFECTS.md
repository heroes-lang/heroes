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
collision stands there; the next number to issue is **034** (022 and 023 were issued on 2026-09-08, 022 was SPLIT on 2026-09-09 by panel 122 into 022 and 024, and all three closed the same day, 024 last, at M-held-bytes; **025** and **026** were issued and closed on 2026-09-11 at M-labelled-builtins and M-named-callbacks, **027** was issued 2026-09-11 beside panel 131 and **028** beside panel 132, and **029** was issued 2026-09-13 beside panel 135 and **030** the same day beside panel 137, **031** beside panel 139; **032** was issued and closed on 2026-09-14 at M-cleanup-verdict step 1, and it is the first since 025 that no sitting produced — the census that opens this milestone found it, and the compiler simply disagreed with `spec § 13`; **033** was issued on 2026-09-14 at the M-marked-acquisition close, by the full net before the push, and it is the first here found by attacking the shapes NEXT to a repair rather than the repair itself) — 017 to 021 were
all issued on 2026-09-08 and all closed on 2026-09-08, and all five are in the
record.

Format: `- [ ] **NNN — <title>** | <what it does, in one line> | <where to look>`

*******************************************************************************
**OPEN: 1**

- [ ] **033 — `unmarked_handle_producer` does not look inside a returned record** | an `extern` handing a handle back in a struct FIELD carries no mark, the checker says nothing, and the imbalance is caught at exit instead of at compile time | `selfhost/check/acquiring.hero` · `tests/golden/run/abort-handle-given-back-unmarked.hero` · `runtime/parts/alloc.c`'s fourth exit check

    **Origin:** found 2026-09-14 at the M-marked-acquisition close, by the full
    net before the push and then by attacking the shapes next to the one that
    provoked it (CLAUDE.md § RUN IT). Filed after the tag, so the tag's own
    commit stands over a clean list.

    **The reproducer runs and it is in the tree.** `pair_make(n: i64) -> Pair`
    where `record Pair` holds `s: Slot` and `Slot` is a handle: the program
    compiles with no diagnostic, prints 7, and aborts at exit 134 saying the
    count went to -1. R5 reads RESULTS and `@` out-parameters and asks
    `handle_behind` of each; a record type whose FIELD is a handle answers no,
    so `pair_make` is not a producer as far as the rule is concerned.

    **It is a real C shape and not a contrivance.** `struct addrinfo` carries
    pointers the caller must free, and a struct-returning constructor handing
    back an owned member is ordinary in C. The three shipped bindings here do
    not use it, which is why nothing fired.

    **What is owed, and why it is not done here.** Widening what a diagnostic
    refuses is a diagnostic CLASS, so CLAUDE.md § 4 sends it to the panel; the
    milestone was closed and tagged when this was found. The question for that
    sitting is not *should the rule see fields* but **how deep**: a handle
    behind two records, behind an optional, behind a list. The cheap answer
    stops at one level and would be a premise about the world in the sense
    `.claude/rules/module-shape.md` refuses.

    **What holds meanwhile is written down rather than assumed**: the program
    does not corrupt anything silently. The counter catches it at exit, and
    since 2026-09-14 its message names this shape explicitly as one of the
    three that reach it. Robustness is preserved (CLAUDE.md § Precedence 3);
    what is missing is the earlier catch.

*******************************************************************************
