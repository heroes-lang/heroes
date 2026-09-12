- [x] **reasoning — the chain judged against a production-readiness criterion nobody had written** | 2026-09-10, the session that asked whether the chain was missing steps for the language to be usable for **production code that is not mission critical** at 1.0 | **Settled**: the criterion exists now and lives in `docs/ROADMAP.md` § What production-ready means, by author decision, because the criterion is not the language; **four rows entered** and all four were put with a recommendation and accepted — `M-cleanup-verdict`, `M-arm-platform`, `M-deployable-binary`, `M-compatibility-promise`, 63 rows to 67; **M-core-packages was not a new row and its step order was**, the web stack moving from steps 10-12 to 2-6 at no dependency cost; **CL-036's walk gained the two things that colour a program**; and **all 42 items of `docs/work/SCHEDULED.md` were verified, three pairs merged, twelve added and the file ordered by the chain for the first time**. **The finding that framed all of it**: design.md defines v1 as *compiles itself* and production readiness **nowhere** — no section on distribution, versioning, stability or a 1.0 promise over 3,612 lines, and the only `1.0` in the file is Go's. **Six of the criterion's ten rows were owned already**, which is the answer to the question as asked | `docs/ROADMAP.md` § What production-ready means, § The chain rows 47, 49, 63, 66 · `DESIGN-LOG.md`, the 2026-09-10 row · `docs/work/SCHEDULED.md` § the header's two new paragraphs · `.claude/rules/diagnostics-and-goldens.md` § A new surface form · `tests/harness/suite_records.hero` `ROWS` 67 -> 71 | the session that found the chain in better shape than the question assumed, and the four things nothing owned

    **THIRTEEN CANDIDATES WERE MET WITH THE RULE THAT ALREADY RULED ON EACH, and
    the table is here so none of them comes back as new** — which is
    `docs/work/DONE.md:2413`'s own stated reason for its table, one exercise
    earlier. Cross-compilation and a `--target` flag: refused at
    `DESIGN-LOG.md:539`, and `M-arm-platform` is a real machine rather than a
    reversal. Prebuilt binaries in a channel: refused there too and reaffirmed
    twice. A REPL: Part 6's interpreter shapes, and `heroes run` is the loop.
    Out-of-memory handling: nothing owed, `runtime/parts/alloc.c` already panics
    cleanly. A standard library: §1.11, the founding constraint, and what it is
    wanted for arrives as M-package-manager's distributable bindings. Typed errors
    and an error cause chain: panel 034 ruled it **stays** Part 8 wart 5, *the
    answer is a `constant`, not a feature*. A per-project manifest or a lockfile:
    panel 056 and §10's three input classes, with the pinning answer owed by
    M-package-manager's sitting. String search, split, trim and case: already
    M-core-packages step 1, by name and by count. `sort` by a field: the `sort`
    package, generic, in Heroes, because function values exist. A decimal type for
    money: Principle 0, and `i64` cents is the shape. Relocatable modules:
    M-core-packages question (ii), since `use` cannot climb (panel 099 R1).
    Catching an abort or a supervisor: Part 6 refuses exceptions permanently, and
    the deployment model is **crash and be restarted**, which is why
    M-compatibility-promise's paragraph has to say so rather than let a stranger
    find out. A foreign thread calling Heroes: deliberate,
    `runtime/parts/thread.c` panics by name, so async C libraries are out of
    scope and the same paragraph says that too.

    **What the four rows deliver, in one sentence each, because the ROADMAP
    carries the rest.** `M-cleanup-verdict`: the ruling on a release bound to a
    scope, warranted by a silence with its vocabulary named — `defer`, `RAII`,
    `scope guard`, `scope-bound`, `destructor`, `finally` and `cleanup on` return
    nothing about a construct over design.md, `spec/` and all 125 sittings, while
    `owned <C function>` and `lease`/`end_lease` landed four and two days earlier
    and put a release obligation on every path. `M-arm-platform`: the fourth real
    machine, with the `char`-signedness prediction registered so it can be scored.
    `M-deployable-binary`: what the machine that runs a program needs, measured
    here as `libSystem` alone on this Mac and **unrun** on the other two, plus the
    `-O0` default `heroes build` ships. `M-compatibility-promise`: the instrument
    under M-publication-gate's paragraph, and the sentence §10 forces — a program
    cannot declare its language version, so the promise is one-directional.

    **What the SCHEDULED sweep found, and it is the number worth keeping.** Of
    **42** items, **none had been done**. Five rested on a sentence that is now
    false: the spec headroom *40 tokens free* voided by
    `docs/measurements/010`'s own closing section (**706** free today); **247 and
    151** hoisted locals that do not reproduce (~119 and ~122 in cached emitted C,
    and which of three explanations holds is UNSETTLED without a build); a
    CLAUDE.md §8 citation for the `--sanitize` narrowing that **does not exist**
    (it is CL-055, in `.claude/rules/platforms.md`, and carries no program count);
    a claim that `design.md` asserts an lldb golden runs in batch mode, which
    design.md **never said**; and a thread-guard sentence that M-isolated-threads
    had already made false, which makes that item cheaper rather than staler. Most
    of the rest carried pointers that had drifted, because design.md grew 150 to
    290 lines and CLAUDE.md's §§7-11 became one-line pointers into
    `.claude/rules/`. **A line-number citation is exactly what
    `records/citations` cannot judge**, since it reads a path claim, so every one
    of those had been passing on every run — including one inside
    `tests/harness/suite_records.hero` itself, repaired in this commit.

    **And three faults in the ROADMAP were found while the rows were written**,
    each one a sentence no instrument reads: § The chain said *rows 1-37 are
    done, 38-59 are next* where 44 were done, seven closes behind; § The
    milestones said *eight closed* and *nineteen* scheduled where the file holds
    41 sections, 18 and 23; and **seven sections of closed milestones did not say
    so in their headings**, against that section's own stated rule, which is the
    shape it exists to prevent. § Where we are's spec numbers were four tokens
    and one digest stale (5373 / 4203 / 711 / `5a9b2886` against 5378 / 4210 /
    766 / `527e1b76`), and § Where we are is the one place those counts live.
