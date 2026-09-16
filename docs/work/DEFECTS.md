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
collision stands there; the next number to issue is **037** (022 and 023 were issued on 2026-09-08, 022 was SPLIT on 2026-09-09 by panel 122 into 022 and 024, and all three closed the same day, 024 last, at M-held-bytes; **025** and **026** were issued and closed on 2026-09-11 at M-labelled-builtins and M-named-callbacks, **027** was issued 2026-09-11 beside panel 131 and **028** beside panel 132, and **029** was issued 2026-09-13 beside panel 135 and **030** the same day beside panel 137, **031** beside panel 139; **032** was issued and closed on 2026-09-14 at M-cleanup-verdict step 1, and it is the first since 025 that no sitting produced — the census that opens this milestone found it, and the compiler simply disagreed with `spec § 13`; **033** was issued on 2026-09-14 at the M-marked-acquisition close, by the full net before the push, and it is the first here found by attacking the shapes NEXT to a repair rather than the repair itself; **034** was issued on 2026-09-14 by panel 149's spec-warden while it was pricing 033's repair, and it is the first here that a SITTING CONVENED ON ANOTHER DEFECT found — the seat went looking for what the specification already said and found the compiler disagreeing with it somewhere nobody had asked about; **035** and **036** were issued the same day by the same sitting, 035 by its compiler-engineer and 036 by its completeness critic, so **panel 149 produced three defects while ruling on a fourth** and **039**, **040** and **041** were issued on 2026-09-15 by panel 150, which was convened on 037 and 038 and produced three more while ruling on them — 039 by its ffi-pragmatist, 040 and 041 by its completeness critic, and 039 is the first here that reaches a mechanism two earlier sittings built rather than a gap they left; the next number to issue is **048** — **046** and **047** were issued on 2026-09-16 at M-check-completeness step 2, and they are unlike each other in a way worth keeping: 046 came from panel 155's ffi-pragmatist attacking the shapes NEXT to the one the sitting was convened about, and it falsified one of the three grounds that sitting's split rested on; **047 is the first here the AUTHOR opened by asking a question** — why is CI broken — and it is a platform fact measured on one machine and shipped for three, which is the rule `.claude/rules/platforms.md` exists to state; **045** was issued on 2026-09-15 by panel 153's completeness critic, as the measured COST of a route that seat found and that five seats had missed, so it is the first here that a critic filed out of an option nobody had listed; **044** was issued on 2026-09-15 by a six-agent adversarial sweep over the shapes NEXT to defect 037's repair, each finding put to an independent skeptic told to refute it, and it is the first here that a SWEEP found rather than a sitting, a suite or a census; **042** and **043** were issued on 2026-09-15 by panel 152, both by its llm-ergonomist, which was asked about a qualifier and found instead that the language cannot bind C's commonest struct shape and that the document's one example teaches the omission its own section forbids — **037** and **038** were both issued on 2026-09-15 at panel 149's ratification, out of what that sitting had NAMED and not filed: 037 is the `ptr` blind spot its ffi-pragmatist called the more serious of the two holes it looked at, and 038 is the per-element release its completeness critic found no seat had been briefed on) — 017 to 021 were
all issued on 2026-09-08 and all closed on 2026-09-08, and all five are in the
record.

Format: `- [ ] **NNN — <title>** | <what it does, in one line> | <where to look>`

*******************************************************************************
**OPEN: 2**

- [ ] **046 — both static walks abandon past a nesting bound, so `check` accepts what its own rule refuses** | a record chain 16 deep ending in an `f64` map key, or in a `partial` group record compared with `==`, is `check` 0 where 15 deep is `check` 1 — and no generic is involved | `selfhost/check/map_keys.hero:115-119` · `selfhost/check/partial.hero:44-51` · `docs/panel/155-the-hole-was-never-made-by-the-generic.md` R4

    **Origin:** panel 155, 2026-09-16, found by its ffi-pragmatist while testing
    the shapes beside the one the sitting was convened about, and re-measured at
    the synthesis by the coordinator with one construction for both rules.

    **The reproducers, measured 2026-09-16 on a compiler built from the seed.**
    `L0` holds the offending thing and `Lk` holds `inner: L(k-1)`:

    | depth | `{Ln: i64}` keyed through to an `f64` | `a == b` on `Ln` holding a `partial` |
    |---|---|---|
    | 1, 8, 14, **15** | `check` **1**, `error[float_map_key]` | `check` **1**, `error[ffi_partial_operation]` |
    | **16**, 17, 18, 24 | `check` **0**, no diagnostic | `check` **0**, no diagnostic |

    Both then build at 0 and abort at **134** with a named message — `panic: a map
    key that is not equal to itself (nan)` and `panic: …_FileStat_eq: a partial
    record has no structural equality`. **So the runtime guard holds in both
    cases and this is a silence where a message is owed, not a corruption.** That
    is what keeps it here rather than at §1.12's rank.

    **Both walks abandon at the SAME depth**, measured with an identical
    construction. Panel 155's completeness critic reported the float twin giving
    up one level shallower; that reading compared two nests built differently, and
    the corrected measurement makes the two rules identical rather than merely
    similar. It also settles the sitting's R4: the spec-warden offered *only the
    float rule has a guard leaning on it* as one of three grounds for splitting
    the question, and that ground is false — `check/partial.hero:44-51` and
    `check/map_keys.hero:115-119` state the same `depth > 16` give-up, licensed by
    the same kind of guard, in the same words.

    **What is owed.** A total walk, or a bound with a diagnostic behind it rather
    than a silence. The `depth > 16` line in `check/map_keys.hero` calls its
    absence *"the safe direction"* and says so **because the runtime guard sits
    behind this rule** — which is true and is exactly the argument defect 035
    falsified in the twin handle walk, where no guard sat behind it. A guard that
    licenses a compile-time silence is a guard the compiler is leaning on to be
    wrong quietly.

    **Where to look also:** `tests/golden/run/abort-map-key-nan.hero` ·
    `docs/records/done/` for defect 035, the same line in the handle walk.
    **Why it matters:** this milestone is named for what `heroes check` accepts,
    and the sitting spent five seats on generics while straight-line code with no
    generic in it walks through both rules.

- [ ] **047 — defect 045's blame line is measured on one platform and fails on the other two** | `tests/golden/surface-fixtures/nullread/` is green on Darwin, names the wrong caller on Linux, and produces no message at all on Windows — `main` has been red since 2026-09-15 | `.github/workflows/` · `docs/panel/154-the-class-was-never-argument-shaped-and-one-flag-closed-it.md` · `.claude/rules/platforms.md`

    **Origin:** the author, 2026-09-16, who asked why CI was broken. The two most
    recent pushes to `main` both failed, runs `35022453104` and `35025566586`.

    **What CI measured**, one test, `surface/a null handle read through by C says
    so instead of dying at 139, and the level is part of the claim (defect 045,
    half of it)`, under `heroes run tests/golden/surface-fixtures/nullread/main.hero -O0`:

    | leg | stdout, expected `7` | the panic line | the caller named |
    |---|---|---|---|
    | **Darwin arm64** | `7` | present | `node_value` — green |
    | **Linux x86-64** | empty | **present and correct** | says `main.main`, not `node_value` |
    | **Windows x86-64** | empty | **absent — stderr empty** | none |

    Darwin reads `surface` 109 passed; Linux 108 passed 1 failed; Windows 101
    passed 1 failed. Every other suite is 0 failed on all three legs.

    **So they are two defects wearing one test.** On Linux the guard fires and
    attributes the call to the wrong function; on Windows it does not fire at all
    and the program dies silently, which is the exact behaviour defect 045 was
    filed to end.

    **The rule this is a case of**, and it is the reason the entry names it rather
    than just the symptom: `.claude/rules/platforms.md` — *a platform fact is run
    on a platform, or it is an inference*, and the three are measured from this Mac
    **before** the commit. The repair was measured on Darwin and shipped for three.
    Panel 154's ratification record separately says route A's guard was *"adopted
    and unlanded"*, with a trigger naming `selfhost/emit/ops.hero`; a test
    demanding the blame line shipped anyway and passes here, so nothing local could
    have caught it.

    **What is owed.** The measurement on both machines rather than from here — Docker
    was up and the Windows box was not, measured below — then
    the repair, then the three-platform measurement the rule asks for before the
    commit. **Whether the test's claim or the compiler is what is wrong is an open
    question and not a premise**: it may be that the blame line is owed and absent,
    or that the test asserts a caller name only one platform's unwinder can give.
    Nothing here decides that, and the CI log cannot.

    **MEASURED ON THE LINUX MACHINE 2026-09-16, and the diagnosis inverts.**
    The entry above was written from CI logs; this is the container, built from
    `docs/ref/environment/linux/Dockerfile`, image `heroes-linux`, `uname -m`
    `x86_64`, the tree copied in read-only and the seed built inside. Same
    program, `heroes run tests/golden/surface-fixtures/nullread/main.hero -O0`:

    | | macOS arm64 (this Mac) | Linux x86-64 (the container) |
    |---|---|---|
    | exit | 134 | 134 |
    | stdout | `7` | **empty** |
    | stderr | `panic: … at offset 0x0, called from node_value` | `panic: … at offset 0x0, called from main.main` |

    **The green leg is the suspect one.** `hero_stack_blame`
    (`runtime/parts/stack.c:297`) asks `dladdr` which symbol holds the PC; if
    that symbol is a Heroes one it is returned at once, otherwise it is kept as
    a fallback and the frame chain is walked for a Heroes symbol.
    `hero_stack_is_heroes` tests for the `h_` prefix, so the C function
    `node_value` is **not** one — it is the fallback. `main.main` is
    `h_main_main` demangled, which is the frame walk **succeeding**.

    So on Linux the walk finds the author's own function, which is what the
    handler's own comment says it is for: *"the fault is inside the C function
    the program called, and its caller is the author's own line"*. On this Mac
    the walk finds nothing and falls back to the C function that faulted. **The
    fixture pins the fallback**, and `.expected` has been asserting the macOS
    failure as the answer since defect 045's repair landed. Which behaviour is
    correct is now a question with a measurement under it rather than a premise:
    the candidate repair is the fixture and the arm64 frame walk, not the Linux
    runtime.

    **The lost `7` is a second and separate defect in the same test.** It is
    printed before the fault. `hero_stack_say` writes with a raw `write`, while
    `print` goes through buffered stdio, and the handler ends at `abort()`.
    macOS's libc flushes stdio on abort and glibc does not, so the line the
    program had already produced is destroyed by the crash report. A program
    that aborts must not lose what it printed first, and nothing in the tree
    says which of the two behaviours is intended.

    **Windows stays UNRUN, and the reading below is from the source, not from
    the machine.** `runtime/parts/stack.c:567-590` has exactly two arms in its
    vectored handler: `EXCEPTION_STACK_OVERFLOW`, and `EXCEPTION_ACCESS_VIOLATION`
    **with `ExceptionAddress == 0`**, which is the PC — a null FUNCTION POINTER
    CALLED, defect 013. A null pointer READ THROUGH has a valid PC inside the C
    function and the touched address in `ExceptionInformation[1]`, which no arm
    reads, so the exception falls through to `EXCEPTION_CONTINUE_SEARCH` and the
    process dies with nothing on either stream — which is what CI measured. The
    file's own comment names `ExceptionInformation[1]` two paragraphs above the
    gap, while explaining why the POSIX arm reads the PC instead. **The
    measurement is owed on the box**: `tailscale status` reads `apponfly-vps …
    offline, last seen 15m ago` at 2026-09-16, and only the author can power it
    from the appOnFly console.

    **And `runtime/parts/stack.c:440-442` had already said this was owed**, in
    its own words: the class closes with *"each platform's own floor asked of
    the platform, which needs a measurement on the Linux and Windows machines
    rather than on this one"*. It was written and nobody ran it.

*******************************************************************************
