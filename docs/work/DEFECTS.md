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
collision stands there; the next number to issue is **037** (022 and 023 were issued on 2026-09-08, 022 was SPLIT on 2026-09-09 by panel 122 into 022 and 024, and all three closed the same day, 024 last, at M-held-bytes; **025** and **026** were issued and closed on 2026-09-11 at M-labelled-builtins and M-named-callbacks, **027** was issued 2026-09-11 beside panel 131 and **028** beside panel 132, and **029** was issued 2026-09-13 beside panel 135 and **030** the same day beside panel 137, **031** beside panel 139; **032** was issued and closed on 2026-09-14 at M-cleanup-verdict step 1, and it is the first since 025 that no sitting produced — the census that opens this milestone found it, and the compiler simply disagreed with `spec § 13`; **033** was issued on 2026-09-14 at the M-marked-acquisition close, by the full net before the push, and it is the first here found by attacking the shapes NEXT to a repair rather than the repair itself; **034** was issued on 2026-09-14 by panel 149's spec-warden while it was pricing 033's repair, and it is the first here that a SITTING CONVENED ON ANOTHER DEFECT found — the seat went looking for what the specification already said and found the compiler disagreeing with it somewhere nobody had asked about; **035** and **036** were issued the same day by the same sitting, 035 by its compiler-engineer and 036 by its completeness critic, so **panel 149 produced three defects while ruling on a fourth** and the next number to issue is **037**) — 017 to 021 were
all issued on 2026-09-08 and all closed on 2026-09-08, and all five are in the
record.

Format: `- [ ] **NNN — <title>** | <what it does, in one line> | <where to look>`

*******************************************************************************
**OPEN: 1**

- [ ] **036 — `owned <freer>` after a non-`cstr` result checks clean and makes clang blame the compiler** | a freer named after an integer result silently retypes it as `str?`, emits `h_library_validated` on the integer, and exits 2 saying the compiler failed | `selfhost/check/freer.hero` · `.claude/rules/c-boundary.md`'s four exit-1 classes

    **Origin:** found 2026-09-14 by **panel 149's completeness critic**, outside
    the sitting's question. **Reproduced by the coordinator, and the first
    attempt FAILED**, which is what the entry below records rather than hides.

    **The reproducer, and the condition the first run was missing.** Written
    without declaring the freer, `function labs(x: i64) -> i64 owned free` is
    refused correctly: `error[unknown_freer]` at exit 1, at check and at build.
    **The defect needs the freer DECLARED**:

    ```
    extern "stdlib.h"
        function free(p: ptr)
        function labs(x: i64) -> i64 owned free
    ```

    `heroes check` exits **0**. `heroes build` exits **2**:
    `internal error: compiling the generated C failed`, with clang reporting
    *"incompatible integer to pointer conversion passing 'int64_t' … to parameter
    of type 'const char *'"* at `h_library_validated(t5)`.

    **So the guard exists and watches the wrong half.** `unknown_freer` asks
    whether the NAME resolves and never whether the RESULT is a type `owned` can
    apply to, and `spec § 13` gives `owned` only after *"a `cstr` result or a
    `char **` out-parameter"*. The compiler disagrees with the specification
    (CLAUDE.md § 12), so this costs no spec token.

    **It is a fifth exit-1 class.** `.claude/rules/c-boundary.md` names four
    classes where a clang failure is the author's `extern` and exits 1. This one
    exits 2, the compiler blaming itself for a `.hero` file's mistake, which is
    the failure §4.19's guarantee exists to prevent. Panel 149's ffi-pragmatist
    filed a sibling of it independently: a `tag` naming a C type that needs the
    `struct` keyword also exits 2.

*******************************************************************************
