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
collision stands there; the next number to issue is **037** (022 and 023 were issued on 2026-09-08, 022 was SPLIT on 2026-09-09 by panel 122 into 022 and 024, and all three closed the same day, 024 last, at M-held-bytes; **025** and **026** were issued and closed on 2026-09-11 at M-labelled-builtins and M-named-callbacks, **027** was issued 2026-09-11 beside panel 131 and **028** beside panel 132, and **029** was issued 2026-09-13 beside panel 135 and **030** the same day beside panel 137, **031** beside panel 139; **032** was issued and closed on 2026-09-14 at M-cleanup-verdict step 1, and it is the first since 025 that no sitting produced — the census that opens this milestone found it, and the compiler simply disagreed with `spec § 13`; **033** was issued on 2026-09-14 at the M-marked-acquisition close, by the full net before the push, and it is the first here found by attacking the shapes NEXT to a repair rather than the repair itself; **034** was issued on 2026-09-14 by panel 149's spec-warden while it was pricing 033's repair, and it is the first here that a SITTING CONVENED ON ANOTHER DEFECT found — the seat went looking for what the specification already said and found the compiler disagreeing with it somewhere nobody had asked about; **035** and **036** were issued the same day by the same sitting, 035 by its compiler-engineer and 036 by its completeness critic, so **panel 149 produced three defects while ruling on a fourth** and **039**, **040** and **041** were issued on 2026-09-15 by panel 150, which was convened on 037 and 038 and produced three more while ruling on them — 039 by its ffi-pragmatist, 040 and 041 by its completeness critic, and 039 is the first here that reaches a mechanism two earlier sittings built rather than a gap they left; the next number to issue is **046** — **045** was issued on 2026-09-15 by panel 153's completeness critic, as the measured COST of a route that seat found and that five seats had missed, so it is the first here that a critic filed out of an option nobody had listed; **044** was issued on 2026-09-15 by a six-agent adversarial sweep over the shapes NEXT to defect 037's repair, each finding put to an independent skeptic told to refute it, and it is the first here that a SWEEP found rather than a sitting, a suite or a census; **042** and **043** were issued on 2026-09-15 by panel 152, both by its llm-ergonomist, which was asked about a qualifier and found instead that the language cannot bind C's commonest struct shape and that the document's one example teaches the omission its own section forbids — **037** and **038** were both issued on 2026-09-15 at panel 149's ratification, out of what that sitting had NAMED and not filed: 037 is the `ptr` blind spot its ffi-pragmatist called the more serious of the two holes it looked at, and 038 is the per-element release its completeness critic found no seat had been briefed on) — 017 to 021 were
all issued on 2026-09-08 and all closed on 2026-09-08, and all five are in the
record.

Format: `- [ ] **NNN — <title>** | <what it does, in one line> | <where to look>`

*******************************************************************************
**OPEN: 1**

- [ ] **045 — a null handle handed to a C function that reads through it SEGFAULTS, which §1.12 forbids outright** | `cstr` arguments are guarded on their way out and handles are not, so `ai_read(p: nullptr)` against a header that dereferences its parameter builds clean and dies at **exit 139** with nothing on either stream | `selfhost/emit/ops.hero`'s call emission · `guard_cstr_arguments` beside it · `runtime/parts/` for the abort shapes

    **Origin:** measured 2026-09-15 by **panel 153's completeness critic**, as the
    one cost of the read route it found that five seats had missed, and re-run by
    the coordinator before this entry was written.

    **The reproducer is three lines of header and six of Heroes.**

    ```c
    /* airead.h */
    #include <netdb.h>
    typedef struct addrinfo AddrInfoValue;
    static inline AddrInfoValue ai_read(const struct addrinfo *p) { return *p; }
    ```
    ```
    extern "airead.h"
        record AI tag addrinfo
        record AddrInfoValue partial
            ai_family: i32
            ai_next: AI
        function ai_read(p: AI) -> AddrInfoValue borrows

    function main()
        empty: AI @ nullptr
        node = ai_read(p: empty)
        print(node.ai_family)
    ```

    `heroes build` exits **0**, the program dies at **exit 139**, SIGSEGV, and
    **both streams are empty**. §1.12 says a Heroes program must not segfault,
    and CLAUDE.md § Precedence puts that at rank 3, above elegance, tokens,
    ergonomics, compiler size and speed.

    **The asymmetry is the cause, and half of it already shipped.** A null `cstr`
    reaching a C function is `panic: a null cstr was passed to a C function`,
    exit 134, because `guard_cstr_arguments` wraps every `cstr` argument on its
    way out (`.claude/rules/c-boundary.md`). A handle is the same thing — an
    address the program may hold as `nullptr` and C will dereference — and
    nothing wraps it.

    **What is owed, and the question inside it.** A blanket guard is wrong:
    `freeaddrinfo(NULL)` and `sqlite3_close(NULL)` are legal C and real programs
    call them. So the rule has to say WHICH handle arguments are guarded, and the
    binding author is the only one who knows — which is a diagnostic class and a
    possible surface word, so it goes to the panel. `sqlite3_free` and the
    `owned` machinery are the precedent for a mark that says what a call does
    with what it is given.

    **THE DEFECT IS WORSE THAN IT WAS FILED, measured 2026-09-15 while its first
    repair was being attacked at the shapes beside it.** The entry above measures
    a null the optimiser can PROVE. Written so it cannot — the null arrives from
    a C function whose argument depends on `args()` — the three levels disagree,
    and the disagreement is the finding:

    | level | what the program does |
    |---|---|
    | `-O0` | `panic: a null pointer was read through …, called from node_value`, **exit 134** — the runtime guard below |
    | `-O2` | prints the WRONG VALUE for a node that does not exist, **exit 0**, both streams otherwise empty |
    | `--sanitize` | `runtime error: member access within null pointer`, UBSan, exit 0 |

    **A wrong answer at exit 0 is what this list exists for, and no signal
    handler can ever reach it**, because at `-O2` clang assumes the pointer is
    non-null — that is what the standard licenses — and folds the fault away
    entirely. So the repair cannot live in the runtime: **it has to be a check
    before the call**, which is what `guard_cstr_arguments` already does for
    every `cstr`, and which for a handle needs the binding author to say whether
    NULL is legal for that parameter.

    **A PARTIAL REPAIR LANDED 2026-09-15 and it is named partial rather than
    counted.** `runtime/parts/stack.c` turns a read through the null page into
    `panic: a null pointer was read through`, exit 134, naming the Heroes
    function that called C — defect 013's own resolution one argument over, on
    the measurement that a field's offset is smaller than a page. It closes the
    `-O0` shape and the silent 139, it costs nothing, and it leaves the `-O2`
    wrong answer standing. The case is
    `tests/golden/surface-fixtures/nullread/main.hero`.

*******************************************************************************
