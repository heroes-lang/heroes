- [ ] **M-generated-programs** | `--sanitize` runs at `-O2`, where clang deletes the leaks LeakSanitizer exists to find | `tests/harness/suite_corpus.hero` § configurations · `CLAUDE.md` §8

    **Origin:** measured 2026-09-07 at M-declared-freer step 5, in the Linux
    container, while building a positive control for the `owned` release. Its
    home because that milestone's whole job is aiming an instrument at the
    compiler, and this is a measured hole in one of the instruments it will lean
    on.

    `tests/harness/suite_corpus.hero`'s `configurations()` is `["-O0", "-O2",
    "--sanitize"]`, and `--sanitize` alone reaches `heroes run`, whose default
    is `-O2` (`heroes --help`). Measured on one program, 200 unfreed allocations
    through a `static inline` header function: at **`-O0`** the binary is **exit
    1** with `LeakSanitizer: detected memory leaks, 4200 byte(s) in 200
    object(s)` naming the `.hero` line; at **`-O2`** it is **exit 0 with nothing
    said**, because the allocation is visible to the optimiser and its only use
    is a null test, so LLVM removes it. **The instrument was proved live in the
    same session** — a plain C `malloc` with the pointer dropped is reported at
    both levels — so this is the optimiser and not a broken sanitizer.

    **How narrow it is, stated rather than left to be found**: a call into a
    real library cannot be elided, which is why `examples/ledger/`'s 40-byte
    leak WAS caught by CI in this exact configuration on 2026-09-04. What is
    invisible is a leak through C the optimiser can see through — a header of
    `static inline` wrappers, which panel 114 R6 made the answer for a package's
    thin C half, so the class is about to get larger rather than smaller.

    **Two further false negatives found the same day and worth carrying**,
    because both cost a wrong conclusion before they were understood: a pointer
    still held in a live local is **reachable** and not leaked, and
    LeakSanitizer scans the stack **conservatively**, so a stale pointer in a
    dead frame is reachable too — a single-allocation leak in a Heroes program
    is therefore usually invisible, because CLAUDE.md §7 hoists every local to
    the prologue.

    **Recommended: a fourth configuration, `-O0 --sanitize`, for the programs
    that declare an `extern` only** — which is CLAUDE.md §8's own narrowing, 8
    programs of 44, so the cost is small and it lands where the class actually
    lives.

    **Where to look also:** `tests/harness/suite_run.hero` ·
    `docs/ref/environment/linux/LINUX-MACHINE.md` · `docs/panel/114` R6.
    **Why it matters:** the leg this project names as its judge for a C leak is
    the one leg whose optimisation level can delete the evidence.

    **Re-verified 2026-09-10: STILL OPEN in substance, one citation FALSE.**
    The substance holds: `configurations()` is still the same three and there is no
    fourth `-O0 --sanitize` arm, and `--sanitize` alone still means `-O2` for `run`.
    **The false citation is CLAUDE.md §8's**: `grep -n sanitize CLAUDE.md` returns
    **nothing at all**, and §8 is Error discipline. That narrowing is **CL-055**, and
    it lives in `.claude/rules/platforms.md` and `.claude/rules/c-boundary.md`, where
    it carries **no program count**; re-measured, programs that declare an `extern`
    are **20 of 55** directories, not 8 of 44. **This file was not touched on
    2026-09-10** (last commit `f0d95197`, 2026-09-06): that day's sanitizer work
    landed on a golden which was then removed, and
    `docs/measurements/025` measures the same instrument from the other side — on
    Linux, `run … --sanitize` reported a 29-byte leak where `build --sanitize` plus
    running the binary was silent.
