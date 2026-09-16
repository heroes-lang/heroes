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
collision stands there; the next number to issue is **037** (022 and 023 were issued on 2026-09-08, 022 was SPLIT on 2026-09-09 by panel 122 into 022 and 024, and all three closed the same day, 024 last, at M-held-bytes; **025** and **026** were issued and closed on 2026-09-11 at M-labelled-builtins and M-named-callbacks, **027** was issued 2026-09-11 beside panel 131 and **028** beside panel 132, and **029** was issued 2026-09-13 beside panel 135 and **030** the same day beside panel 137, **031** beside panel 139; **032** was issued and closed on 2026-09-14 at M-cleanup-verdict step 1, and it is the first since 025 that no sitting produced — the census that opens this milestone found it, and the compiler simply disagreed with `spec § 13`; **033** was issued on 2026-09-14 at the M-marked-acquisition close, by the full net before the push, and it is the first here found by attacking the shapes NEXT to a repair rather than the repair itself; **034** was issued on 2026-09-14 by panel 149's spec-warden while it was pricing 033's repair, and it is the first here that a SITTING CONVENED ON ANOTHER DEFECT found — the seat went looking for what the specification already said and found the compiler disagreeing with it somewhere nobody had asked about; **035** and **036** were issued the same day by the same sitting, 035 by its compiler-engineer and 036 by its completeness critic, so **panel 149 produced three defects while ruling on a fourth** and **039**, **040** and **041** were issued on 2026-09-15 by panel 150, which was convened on 037 and 038 and produced three more while ruling on them — 039 by its ffi-pragmatist, 040 and 041 by its completeness critic, and 039 is the first here that reaches a mechanism two earlier sittings built rather than a gap they left; the next number to issue is **050** — **049** was issued on 2026-09-16, and it is the first here that a REPAIR OF ANOTHER DEFECT created: defect 048's tag half worked and made a blessed artifact depend on the machine that produced it, which CI measured on two legs of one push; **048** was issued on 2026-09-16 by panel 156's ffi-pragmatist, at its own boundary and unasked: the sitting was about a crash message and nobody had put `--emit-c` to it, which makes it the second here a seat found while briefed on something else; **046** and **047** were issued on 2026-09-16 at M-check-completeness step 2, and they are unlike each other in a way worth keeping: 046 came from panel 155's ffi-pragmatist attacking the shapes NEXT to the one the sitting was convened about, and it falsified one of the three grounds that sitting's split rested on; **047 is the first here the AUTHOR opened by asking a question** — why is CI broken — and it is a platform fact measured on one machine and shipped for three, which is the rule `.claude/rules/platforms.md` exists to state; **045** was issued on 2026-09-15 by panel 153's completeness critic, as the measured COST of a route that seat found and that five seats had missed, so it is the first here that a critic filed out of an option nobody had listed; **044** was issued on 2026-09-15 by a six-agent adversarial sweep over the shapes NEXT to defect 037's repair, each finding put to an independent skeptic told to refute it, and it is the first here that a SWEEP found rather than a sitting, a suite or a census; **042** and **043** were issued on 2026-09-15 by panel 152, both by its llm-ergonomist, which was asked about a qualifier and found instead that the language cannot bind C's commonest struct shape and that the document's one example teaches the omission its own section forbids — **037** and **038** were both issued on 2026-09-15 at panel 149's ratification, out of what that sitting had NAMED and not filed: 037 is the `ptr` blind spot its ffi-pragmatist called the more serious of the two holes it looked at, and 038 is the per-element release its completeness critic found no seat had been briefed on) — 017 to 021 were
all issued on 2026-09-08 and all closed on 2026-09-08, and all five are in the
record.

Format: `- [ ] **NNN — <title>** | <what it does, in one line> | <where to look>`

*******************************************************************************
**OPEN: 1**

- [ ] **049 — `--emit-c` writes the pre-probe tag spelling, and the obvious repair makes the artifact platform-dependent** | a program binding `record <Name> tag <name>` gets C that clang refuses — 20 errors on `structtag` — and the probe that fixes it made the SAME program emit two different files on two machines | `selfhost/cli/artifact.hero` · `docs/panel/157-the-artifact-of-a-build-that-never-happened.md` R2 and R4

    **Origin:** the tag half of defect 048, withdrawn 2026-09-16 within the hour
    of landing, by CI. 048's §1.12 half — the pointee check on the `--emit-c`
    path — **stands and is closed**; this is only the spelling.

    **The repair worked and its consequence is the defect.** The advisory probe
    asks clang `-fsyntax-only`, harvests the `struct` request, discards the
    verdict and re-emits: `structtag`'s artifact went from **20 clang errors to
    0**, with `struct probe *` written 21 times. But it made the artifact **a
    function of the local headers**, and `tests/emission/` blesses those bytes.

    **Measured by CI, both legs of one push:**

    | | the blessed line |
    |---|---|
    | macOS | `(struct addrinfo * *)0` — the probe learned |
    | Windows | `(addrinfo * *)0` — no `netdb.h`, so it learned nothing |

    The probe's degradation is correct by design — *on a machine without the
    header it learns nothing and the output is yesterday's* — and that is
    precisely what a byte-for-byte blessing cannot tolerate. **It is defect
    047's own class**, a golden encoding one platform's answer, committed inside
    a repair on the day the rule naming it was written
    (`.claude/rules/diagnostics-and-goldens.md`, CL-076).

    **So the sitting's question is not how to spell a tag.** It is: a blessed
    artifact cannot depend on the machine that produced it, so either
    `--emit-c` stays deterministic and a tagged binding keeps getting C that
    does not compile, or those programs stop being blessed by bytes and are
    judged by a predicate instead. The second is **panel 157's R4**, adopted and
    unbuilt, filed with its price in
    `docs/work/milestones/M-package-manager.md`: build both ways, compare exit
    code and stdout, and LINK.

    **What is owed** is that choice, made at a sitting rather than in the hour
    after a red leg — and the withdrawn probe is thirty lines that can be put
    back in one commit, kept in `selfhost/cli/artifact.hero`'s own comment with
    its date and its measurement.

*******************************************************************************
