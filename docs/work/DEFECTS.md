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
collision stands there; the next number to issue is **037** (022 and 023 were issued on 2026-09-08, 022 was SPLIT on 2026-09-09 by panel 122 into 022 and 024, and all three closed the same day, 024 last, at M-held-bytes; **025** and **026** were issued and closed on 2026-09-11 at M-labelled-builtins and M-named-callbacks, **027** was issued 2026-09-11 beside panel 131 and **028** beside panel 132, and **029** was issued 2026-09-13 beside panel 135 and **030** the same day beside panel 137, **031** beside panel 139; **032** was issued and closed on 2026-09-14 at M-cleanup-verdict step 1, and it is the first since 025 that no sitting produced — the census that opens this milestone found it, and the compiler simply disagreed with `spec § 13`; **033** was issued on 2026-09-14 at the M-marked-acquisition close, by the full net before the push, and it is the first here found by attacking the shapes NEXT to a repair rather than the repair itself; **034** was issued on 2026-09-14 by panel 149's spec-warden while it was pricing 033's repair, and it is the first here that a SITTING CONVENED ON ANOTHER DEFECT found — the seat went looking for what the specification already said and found the compiler disagreeing with it somewhere nobody had asked about; **035** and **036** were issued the same day by the same sitting, 035 by its compiler-engineer and 036 by its completeness critic, so **panel 149 produced three defects while ruling on a fourth** and **039**, **040** and **041** were issued on 2026-09-15 by panel 150, which was convened on 037 and 038 and produced three more while ruling on them — 039 by its ffi-pragmatist, 040 and 041 by its completeness critic, and 039 is the first here that reaches a mechanism two earlier sittings built rather than a gap they left; the next number to issue is **049** — **048** was issued on 2026-09-16 by panel 156's ffi-pragmatist, at its own boundary and unasked: the sitting was about a crash message and nobody had put `--emit-c` to it, which makes it the second here a seat found while briefed on something else; **046** and **047** were issued on 2026-09-16 at M-check-completeness step 2, and they are unlike each other in a way worth keeping: 046 came from panel 155's ffi-pragmatist attacking the shapes NEXT to the one the sitting was convened about, and it falsified one of the three grounds that sitting's split rested on; **047 is the first here the AUTHOR opened by asking a question** — why is CI broken — and it is a platform fact measured on one machine and shipped for three, which is the rule `.claude/rules/platforms.md` exists to state; **045** was issued on 2026-09-15 by panel 153's completeness critic, as the measured COST of a route that seat found and that five seats had missed, so it is the first here that a critic filed out of an option nobody had listed; **044** was issued on 2026-09-15 by a six-agent adversarial sweep over the shapes NEXT to defect 037's repair, each finding put to an independent skeptic told to refute it, and it is the first here that a SWEEP found rather than a sitting, a suite or a census; **042** and **043** were issued on 2026-09-15 by panel 152, both by its llm-ergonomist, which was asked about a qualifier and found instead that the language cannot bind C's commonest struct shape and that the document's one example teaches the omission its own section forbids — **037** and **038** were both issued on 2026-09-15 at panel 149's ratification, out of what that sitting had NAMED and not filed: 037 is the `ptr` blind spot its ffi-pragmatist called the more serious of the two holes it looked at, and 038 is the per-element release its completeness critic found no seat had been briefed on) — 017 to 021 were
all issued on 2026-09-08 and all closed on 2026-09-08, and all five are in the
record.

Format: `- [ ] **NNN — <title>** | <what it does, in one line> | <where to look>`

*******************************************************************************
**OPEN: 1**

- [ ] **048 — `heroes build --emit-c` emits C that does not compile, for any `tag` binding** | a program with `record <Name> tag <name>` builds fine, and the same program's `--emit-c` output gives 15 clang errors: it writes `node *` where the header says `struct node` | `selfhost/cli/pointee.hero` · `selfhost/cli/assemble.hero` · `docs/panel/156-the-blame-line-was-never-a-platform-fact.md`

    **Origin:** panel 156's ffi-pragmatist, 2026-09-16, found at its own boundary
    while measuring something else — the sitting was about a crash message and
    nobody had asked about `--emit-c`.

    **The compiler is CORRECT and only `--emit-c` hands out the wrong round.**
    Defect 037's repair (panels 150-152) compiles the author's word first, reads
    clang's own refusal — *must use 'struct' tag to refer to type 'node'* — and
    recompiles with `struct node *`. That is a ROUND, and `cli/assemble.hero` is
    where it lives. The seat counted `struct node` **12 times** in the translation
    units that produced objects and **0 times** in the ones that did not: so the
    build is right and the artifact `--emit-c` writes is the pre-probe spelling.

    **The reproducer** is `tests/golden/surface-fixtures/structtag/main.hero`,
    which `surface` already runs at `run` and which passes: `heroes build … -o`
    works, `heroes build … --emit-c` writes C that clang refuses.

    **Why it matters more than an artifact usually would.** `--emit-c` is not a
    debugging convenience here: it is how the SEED is made
    (`heroes build selfhost/main.hero --emit-c -o seed/heroes.c`, `seed/README.md`),
    and CI asserts on every push that `seed/heroes.c` is exactly what today's
    source emits. `selfhost/` declares no `record … tag` of its own today —
    measured 2026-09-16, the 28 occurrences are comments, message strings and
    parser fixtures — so the seed is not affected and the fixpoint held at this
    milestone. **The day the compiler binds a C library through a tagged handle,
    it would be.**

    **What is owed.** The probe round applied to the `--emit-c` path as it is to
    the object path, or a refusal saying `--emit-c` cannot serve a tagged
    binding — a silence that writes a file nobody can compile is the shape this
    list exists for. **And a surface row that runs clang over what `--emit-c`
    wrote**, which nothing does today: `emission` reads `--emit-c` output but does
    not compile it, which is why 15 clang errors were invisible to every suite.

*******************************************************************************
