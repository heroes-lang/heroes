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
collision stands there; the next number to issue is **037** (022 and 023 were issued on 2026-09-08, 022 was SPLIT on 2026-09-09 by panel 122 into 022 and 024, and all three closed the same day, 024 last, at M-held-bytes; **025** and **026** were issued and closed on 2026-09-11 at M-labelled-builtins and M-named-callbacks, **027** was issued 2026-09-11 beside panel 131 and **028** beside panel 132, and **029** was issued 2026-09-13 beside panel 135 and **030** the same day beside panel 137, **031** beside panel 139; **032** was issued and closed on 2026-09-14 at M-cleanup-verdict step 1, and it is the first since 025 that no sitting produced — the census that opens this milestone found it, and the compiler simply disagreed with `spec § 13`; **033** was issued on 2026-09-14 at the M-marked-acquisition close, by the full net before the push, and it is the first here found by attacking the shapes NEXT to a repair rather than the repair itself; **034** was issued on 2026-09-14 by panel 149's spec-warden while it was pricing 033's repair, and it is the first here that a SITTING CONVENED ON ANOTHER DEFECT found — the seat went looking for what the specification already said and found the compiler disagreeing with it somewhere nobody had asked about; **035** and **036** were issued the same day by the same sitting, 035 by its compiler-engineer and 036 by its completeness critic, so **panel 149 produced three defects while ruling on a fourth** and **039**, **040** and **041** were issued on 2026-09-15 by panel 150, which was convened on 037 and 038 and produced three more while ruling on them — 039 by its ffi-pragmatist, 040 and 041 by its completeness critic, and 039 is the first here that reaches a mechanism two earlier sittings built rather than a gap they left; the next number to issue is **052** — **050** and **051** were issued on 2026-09-16 by panel 158, 050 by its completeness critic while auditing the sitting's own framing and 051 by two seats independently, which makes 051 the second here that a COMMENT produced rather than a program; **049** was issued on 2026-09-16, and it is the first here that a REPAIR OF ANOTHER DEFECT created: defect 048's tag half worked and made a blessed artifact depend on the machine that produced it, which CI measured on two legs of one push; **048** was issued on 2026-09-16 by panel 156's ffi-pragmatist, at its own boundary and unasked: the sitting was about a crash message and nobody had put `--emit-c` to it, which makes it the second here a seat found while briefed on something else; **046** and **047** were issued on 2026-09-16 at M-check-completeness step 2, and they are unlike each other in a way worth keeping: 046 came from panel 155's ffi-pragmatist attacking the shapes NEXT to the one the sitting was convened about, and it falsified one of the three grounds that sitting's split rested on; **047 is the first here the AUTHOR opened by asking a question** — why is CI broken — and it is a platform fact measured on one machine and shipped for three, which is the rule `.claude/rules/platforms.md` exists to state; **045** was issued on 2026-09-15 by panel 153's completeness critic, as the measured COST of a route that seat found and that five seats had missed, so it is the first here that a critic filed out of an option nobody had listed; **044** was issued on 2026-09-15 by a six-agent adversarial sweep over the shapes NEXT to defect 037's repair, each finding put to an independent skeptic told to refute it, and it is the first here that a SWEEP found rather than a sitting, a suite or a census; **042** and **043** were issued on 2026-09-15 by panel 152, both by its llm-ergonomist, which was asked about a qualifier and found instead that the language cannot bind C's commonest struct shape and that the document's one example teaches the omission its own section forbids — **037** and **038** were both issued on 2026-09-15 at panel 149's ratification, out of what that sitting had NAMED and not filed: 037 is the `ptr` blind spot its ffi-pragmatist called the more serious of the two holes it looked at, and 038 is the per-element release its completeness critic found no seat had been briefed on) — 017 to 021 were
all issued on 2026-09-08 and all closed on 2026-09-08, and all five are in the
record.

Format: `- [ ] **NNN — <title>** | <what it does, in one line> | <where to look>`

*******************************************************************************
**OPEN: 3**

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

- [ ] **050 — a test of a doubly-fallible value asks the outer layer and reads as asking the inner one** | `m["b"].is_err()` on a `{str: i64?}` and `find(xs, …).is_err()` on a `[i64?]` are `check` 0 and RUN, and both answer *was the key there* where the line reads *did the stored value fail* | `docs/panel/158-the-sitting-produced-a-resolution-and-left-it-off-its-own-ballot.md` R4 · `selfhost/check/ops.hero`

    **Origin:** panel 158's completeness critic, which ran it while auditing the
    sitting's framing; re-run by the coordinator at the synthesis. Neither route
    needs a generic and neither needs a map: `find` over `[i64?]` reaches it with
    built-ins § 11 hands out thirteen lines apart.

    **The reproducer, measured 2026-09-16**, `check` 0, built, and it prints both
    lines:

    ```
    m: {str: i64?} @ {}
    m["a"] @ fail(code: "parse", msg: "not a number")

    if m["b"].is_err()
        print("this reads as: the stored value failed")
    ```

    `m["b"]` is an `i64??`. `.is_err()` peels the OUTER level and answers *the key
    was not there*, which is true — and the program was written to ask the other
    question.

    **Why it is a defect and not the doubly-fallible question.** Panel 158 framed
    its sitting as *consistency, not safety*, and the ffi seat measured that
    honestly: nothing leaks, nothing corrupts, nothing answers wrongly at the C
    boundary. **This is the third box the dichotomy has no name for** — a
    plausible mistake that compiles, runs, and gives a confident wrong answer to
    the question the author asked. That is design.md §1.1's own subject rather
    than §1.12's, and CLAUDE.md's opening sentence — every plausible LLM mistake
    is a compile error — is what it falsifies.

    **What is owed.** A refusal, or a diagnostic, at the point a `.is_err()`,
    `.must()` or `.default(v)` is applied to a value whose payload is itself
    fallible. The critic measured that of the four repairs panel 158 weighed,
    **only option 1 would have closed this**, and option 1 protects zero programs
    — so the repair is its own, not a by-product. Note that `.default(0)` on the
    same shape IS loud today, exit 1 with two diagnostics, so the three operations
    do not agree with each other and that disagreement is the place to start.

- [ ] **051 — a comment asserts an invariant the compiler falsifies, and a design document cites it as live** | `selfhost/check/table.hero:66-67` says a fallible's argument is never itself a fallible; `--dump-ir` names `i64???` at 27 sites | `selfhost/check/table.hero:66-67` · `docs/design/design.md:2836-2837`

    **Origin:** panel 158, found independently by its compiler-engineer and its
    spec-warden, and narrowed by its completeness critic.

    **The false sentence**, verbatim: *"`T?` — never nested: `T??` is rejected by
    the parser, so this node's argument is never itself a fallible."* The premise
    is true — the parser does reject the written form — and **the conclusion does
    not follow**, because the checker builds the node from a container's element
    type without any written form passing through. Measured false three ways at
    exit 0 in one session: the map route, the `find` route, and `wrap<T>` applied
    twice, the last naming `i64???` at 27 sites under `--dump-ir`.

    **It is `.claude/rules/module-shape.md` § *a premise about the world expires
    silently* exactly**: the argument stayed valid and only the premise died, so
    the comment goes on reading as correct.

    **And it has a reader.** `docs/design/design.md:2836-2837` cites those lines
    as a live invariant inside the `alias` costing, so a design decision rests on
    a sentence the compiler falsifies. That document is append-only
    (CLAUDE.md §14), so the correction is added underneath with its date rather
    than substituted.

    **The critic corrected one over-claim and it is kept here**:
    `selfhost/check/builtins.hero:17` says *"`T??` is the one nesting Heroes'
    parser refuses"*, which is **true** and claims nothing about what the checker
    can build. One comment is false, not two.

    **The enumeration nobody ran, and it is what the repair actually owes.** Is
    any `.fallible` CONSUMER non-recursive? The node is touched in twenty-odd
    files; three are proved recursive — the release path by the ffi seat, both
    renderers by the compiler seat. The rest are unchecked, and a non-recursive
    consumer resting on that comment is where a real defect would be.

*******************************************************************************
