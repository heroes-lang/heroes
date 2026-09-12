# M-generated-programs — programs nobody wrote


**Scheduled by author instruction 2026-09-06** — § Who scheduled what carries the
instruction, the four choices the author made and the measurements that placed the
row. The warrant is **§1.12**: a Heroes program must not segfault, and the
compiler is a Heroes program. Defect 007 is that sentence failing on a **valid**
file, `heroes check` at exit **139** with nothing on either stream.

**What it delivers.** A generator, written in Heroes, that composes programs
**valid by construction** and **knows what each one must print before the compiler
is asked**; five oracles over every program it writes; a reducer that takes a
failure down to something a person can read; the defects that come out, repaired
at the class rather than at the witness; and a committed corpus of the reduced
witnesses in the net, which is what keeps them shut.

**Its first step is a catalogue, and it is enumerated from the world** (CLAUDE.md
§1, which says a list names where it came from). Csmith and YARPGen state in their
own papers which classes of bug they found — YARPGen's count is **more than 220**
in GCC, LLVM and the Intel compiler — a survey of compiler fuzzing exists, Zig
carries an issue titled *Compiler crashes found with fuzzing*, Go keeps
`test/fixedbugs` and Nim its `tests/`. The nearest corpus of all is this
repository: **fourteen** defect entries in `docs/work/DONE.md` and **76**
regression cases named after one — 49 with the `fixedbugs-` prefix under `check/`,
`run/` and `unsupported/`, 27 in `tests/golden/fixedbugs/`. The catalogue is a
`docs/measurements/` file naming each shape with its source and marking what was
read and what was not.

**The two moves that make it more than a test of the lexer**, both borrowed, both
named here so the milestone does not re-derive them. Generation goes **from the
type, never from the text**: it starts at *an expression of type `i64` is needed*
and descends, choosing at each node among the forms that type admits, so the
program type-checks by construction and a refusal from `heroes check` is itself a
defect. And **the answer is computed while the program is built**: every node
carries its value, so the generator writes the program and its `main.expected`
together, with no second compiler standing in as judge. That is Csmith's checksum
trick, and it is the only thing that makes the silent class — exit 0, wrong number
— visible at all.

**Heroes has no undefined behaviour, and that changes the generator's job.** An
overflow, a division by zero and an index out of range all abort by design, so
there is nothing to steer around the way YARPGen must for C; there is a choice to
declare instead. The clean arm picks values that make the operation safe by
construction. A smaller, declared arm expects the **abort and its message**, which
is how the guard rails get checked rather than assumed.

**The five oracles**, all of them, by author decision 2026-09-06 against the two
narrower options offered: (1) the compiler does not fall over — no 139, no 134, no
`internal error`, no silent exit, and every `hero_unreachable` reached is a defect,
of which `selfhost/` holds **63**; (2) the answer is the computed one, in the three
configurations the net already runs — `-O0`, `-O2`, `--sanitize`
(`tests/harness/suite_corpus.hero::configurations()`) — whose disagreement is the
only differential arm there is while there is one backend; (3) `heroes fmt`
re-prints it byte for byte, the class that produced defects 003 and 004, one of
which rewrote a compiler source into a different program at exit 0; (4) what
`heroes check` accepts, `heroes build` compiles, which is M-check-completeness's
promise put under a volume nobody writes by hand; (5) no leak, with the Linux leg
under `--sanitize` for whatever declares an `extern` (CLAUDE.md § Commands, where
LeakSanitizer is the reason that leg exists).

**The two hard shapes are the author's own, and the record agrees with them.**
The C boundary — generated `extern` groups against a header generated with them:
structs by value, `@` out-parameters, callbacks, `cstr`, `owned` — is where four
of the fourteen entries are (010, 013 and both 014s). Depth — records inside
variants inside maps, generics
instantiated across `use` lines, expressions hundreds of levels deep — is
defect 007.

**Where it lives, and what it must never become.** A Heroes program under
`tests/harness/`, run by `heroes run` as the net is, so §10's stopping rule is
untouched and no verb is proposed. The long hunt stays **outside** the net: a
suite that generates at random goes red at random, and this project has already
paid for an instrument nobody trusts. What enters the net is the committed,
reduced corpus, deterministic under a seed. The reducer starts from
`selfhost/mutate/sites.hero`, split out as *the primitives every mutation operator
is built out of: a text edit, a span, and the four questions about a tree node*.
A seed is an integer, and `examples/montecarlo/main.hero` already carries a
deterministic stream with a test saying why — so a defect is reported by its
number and anybody can reproduce it.

**Why here.** After the four verdict milestones, because a generator has to know
the final surface; after M-check-completeness, whose promise is oracle 4; after
M-panic-location, because a crash that names its file, its line and its function
is the difference between a triage of minutes and one of hours. Before the books,
the channels and the gate, so that what gets written about and shipped is a
compiler that has been shot at. **The sixth oracle is dated rather than
promised**: when M-qbe-backend exists, the same generated program through two
backends is differential testing in the full sense, and that is the one thing this
milestone deliberately leaves to a later one.

*******************************************************************************
**OPEN: 4**

- [ ] **M-generated-programs** | the catalogue of shapes, enumerated from the world | `tests/golden/fixedbugs/` · `docs/work/DONE.md`, the defect entries

    **Origin:** scheduled with the chain row, 2026-09-06, out of the author's
    instruction that the hunt look at the bugs found in other similar
    compilers. At its first step, and this item is a starting point rather than
    the list.

    **What one search found the day the row entered.** Csmith (Utah) generates
    random C programs and found hundreds of latent defects in GCC and LLVM by
    differential testing; YARPGen (Intel) generates programs free of undefined
    behaviour and reported **more than 220** bugs to GCC, LLVM and the Intel
    compiler, its own stated contribution being *generation policies* for
    diversity; a survey of compiler fuzzing exists (arXiv 2306.06884) and an
    OOPSLA'19 study asks how much the bugs found this way matter in practice;
    Zig carries an issue titled *Compiler crashes found with fuzzing*.

    **The nearest corpus is this repository, and it was measured rather than
    recalled**: **fourteen** defect entries in `docs/work/DONE.md`, **76**
    regression cases named after one (49 with the `fixedbugs-` prefix under
    `check/`, `run/` and `unsupported/`, 27 in `tests/golden/fixedbugs/`), and
    **63** points in `selfhost/` where the compiler declares a case impossible.

    **What is owed**: a `docs/measurements/` file naming each shape with its
    source and marking what was read and what was not, because CLAUDE.md §1 says
    an enumeration carries where it came from — and the two shapes the author
    named, the C boundary and depth, enter it with a number beside them rather
    than as an impression.

    **Where to look also:** `https://dl.acm.org/doi/10.1145/3428264` ·
    `https://arxiv.org/pdf/2306.06884` ·
    `https://github.com/ziglang/zig/issues/10121`.
    **Why it matters:** a generator aimed at the shapes one session can think of
    measures that session, and the whole point of the milestone is to be aimed
    at the world.

    **Re-verified 2026-09-10: STILL OPEN, and every count it states is now
    LOW.** `docs/work/DONE.md` carries **19** numbered defect entries, not fourteen —
    18 distinct ids, since 014 was issued twice and the next is 025
    (`docs/work/DEFECTS.md:20-26`). Regression cases are **58** `fixedbugs-*.hero`
    under `tests/golden/` (17 check, 35 run, 6 unsupported) against the item's 49, and
    **28** in `tests/golden/fixedbugs/` against 27: **86** in total, not 76. **And the
    63 is UNSETTLED**: no command reproduces it. Over `selfhost/**/*.hero`,
    `impossible` is 9, `cannot happen` 4, `internal error` 27 and `unreachable`
    **119**, so the catalogue's own first act is to define what it counts before it
    counts it.

- [ ] **M-generated-programs** | the generator computes the answer while it builds the program | `tests/harness/suite_corpus.hero` § configurations · `examples/montecarlo/main.hero`

    **Origin:** scheduled with the chain row, 2026-09-06, the author taking all
    five oracles over the two narrower options offered. The net's three
    configurations are the differential arm.

    **The two moves, so the milestone does not re-derive them.** Generation goes
    from the TYPE and never from the text — start at *an expression of type
    `i64` is needed* and descend among the forms that type admits — so the
    program type-checks by construction and a refusal from `heroes check` is
    itself a defect. And every node carries its value as it is built, so the
    generator writes the program and its `main.expected` together and needs no
    second compiler as judge; that is Csmith's checksum trick, and it is what
    makes the silent class visible at all.

    **Heroes has no undefined behaviour, which changes the job**: overflow,
    division by zero and an index out of range all abort by design, so there is
    nothing to steer around the way YARPGen must for C — there is a clean arm
    whose values are safe by construction, and a smaller declared arm whose
    expected result IS the abort and its message.

    **What already exists and must be reused rather than rebuilt**: the three
    configurations are `tests/harness/suite_corpus.hero::configurations()`,
    measured 2026-09-06 as `["-O0", "-O2", "--sanitize"]`, and their
    disagreement is the only differential oracle there is until M-qbe-backend
    gives the tree a second backend; a deterministic pseudo-random stream driven
    by an integer seed is in `examples/montecarlo/main.hero`, with a test saying
    why a seed must reproduce.

    **Where to look also:** `docs/ROADMAP.md` § M-generated-programs.
    **Why it matters:** exit 0 with a wrong number is the class nobody can write
    a golden case for in advance, and an oracle the generator carries is the
    only kind that scales with the programs.

    **Re-verified 2026-09-10: STILL OPEN, and both premises hold exactly.**
    `tests/harness/suite_corpus.hero:304-305` is still
    `function configurations() -> [str]` returning `["-O0", "-O2", "--sanitize"]`, and
    `examples/montecarlo/main.hero:41` still carries the seeded stream with its two
    tests at `:85-89`. No generator exists: the net registers 20 suites and none is
    generative.

- [ ] **M-generated-programs** | the reducer starts from `mutate/sites.hero`, and only reduced witnesses enter the net | `selfhost/mutate/sites.hero` · `tests/harness/main.hero` · `CLAUDE.md` §10

    **Origin:** scheduled with the chain row, 2026-09-06, the author placing the
    generator in the harness rather than behind a new verb.

    **A generated crasher is unreadable, and an unreadable crasher is not
    repaired.** The reduction is the ordinary one — remove a declaration, a
    function, a branch, re-run, keep the removal if the symptom survives — and
    its primitives are in the tree already: `selfhost/mutate/sites.hero` was
    split out on 2026-08-19 as *the primitives every mutation operator is built
    out of: a text edit, a span, and the four questions about a tree node that
    decide whether a site is one*.

    **What must NOT happen to the net.** The long hunt stays outside it: a suite
    that generates at random goes red at random, and CLAUDE.md § Commands
    already carries what this project pays for an instrument nobody trusts. What
    enters is the committed corpus of reduced witnesses, deterministic, run like
    any other case, with the free-running hunt on tags and by hand. **And the
    tool question is settled before it is asked**: a Heroes program under
    `tests/harness/` run by `heroes run` proposes no verb, so §10's stopping
    rule is untouched — if the harness route is ever measured impossible, THAT
    is the sitting, not the convenience.

    **Why it matters:** the difference between a fuzzer that finds bugs and one
    that files noise is the reducer, and the difference between a net people
    trust and one they ignore is determinism.

    **Re-verified 2026-09-10: STILL OPEN.** `selfhost/mutate/sites.hero` is
    **149** lines and there is no reducer and no generated-corpus suite. CLAUDE.md
    §10's *"never a second binary, never a script, never a Makefile"* is unchanged,
    so the premise the item rests its shape on is intact.

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

*******************************************************************************
