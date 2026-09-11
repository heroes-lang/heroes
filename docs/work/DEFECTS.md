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
collision stands there; the next number to issue is **028** (022 and 023 were issued on 2026-09-08, 022 was SPLIT on 2026-09-09 by panel 122 into 022 and 024, and all three closed the same day, 024 last, at M-held-bytes; **025** and **026** were issued and closed on 2026-09-11 at M-labelled-builtins and M-named-callbacks, and **027 was issued 2026-09-11** beside panel 131) — 017 to 021 were
all issued on 2026-09-08 and all closed on 2026-09-08, and all five are in the
record.

Format: `- [ ] **NNN — <title>** | <what it does, in one line> | <where to look>`

*******************************************************************************
**OPEN: 1**

- [ ] **027 — a header field of type `char *` makes a binding unbindable, and the refusal arrives as an internal error** | `cstr` emits as `const char *`, so a legal `extern` record naming a non-const `char *` field is refused by the compiler's own `-Werror`, with a message that accuses the compiler's internals and names no repair | `selfhost/emit/ctype.hero` · `selfhost/emit/extern_record.hero` · `selfhost/cli/flags.hero:42-51` · `spec § 13`

    **Origin:** the ffi-pragmatist seat's boundary experiment at panel 131,
    2026-09-11, where it appears as a byproduct outside every verdict.
    Reproduced by the coordinator on the frozen tree the same day before filing.

    **Reproducer**, run 2026-09-11 from the seed-built compiler:

        extern "pwd.h"
            record Passwd tag passwd partial
                pw_name: cstr

        function main()
            p = Passwd(pw_name: nullptr)
            print(p.pw_name == nullptr)

        $ ./heroes build main.hero -o prog
        internal error: compiling the generated C failed:
        main.hero:6:37: error: initializing 'char *' with an expression of type
        'const char *' discards qualifiers
        [-Werror,-Wincompatible-pointer-types-discards-qualifiers]
            6 |     t2 = (struct passwd){.pw_name = t1};
        the generated C is at build/tu-62384ff23cf80c8d/main.c
        error: clang refused the generated C
        $ echo $?
        2

    **The control**, the same record with `pw_uid: u32` instead: exit 0, binary
    written. So it is `cstr` against a non-const `char *` and nothing wider.

    **Two things are wrong and they are owed separately.** The **diagnostic** is
    the louder one: `spec § 13` promises that clang checks every field against
    the header and that one which disagrees *is refused*, so a refusal is the
    contract being kept — but it arrives as `internal error`, which tells the
    author the compiler has a bug in itself, points at a generated file, and
    names no repair. CLAUDE.md § 8 asks a diagnostic to carry everything needed
    to fix the program without opening another file. And the **rule** is the
    quieter one, and it is a question rather than a premise until somebody rules
    on it: `spec § 13` says a parameter and a field are declared at the header's
    own width and sign, and says nothing about const. Whether `cstr` should emit
    as `char *` inside an `extern` record, whether a second spelling is owed, or
    whether the honest answer is a named refusal, is not settled here.

    **What is NOT known**: how many real headers this reaches. `struct passwd`
    and `struct group` have non-const `char *` fields; the corpus's 20 `extern`
    programs have not been swept for it. That sweep is owed with the repair, at
    the class rather than at this witness.

*******************************************************************************
