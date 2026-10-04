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
