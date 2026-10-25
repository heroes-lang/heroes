# M-latency-ceiling — a latency the compiler may not lose without a decision

**Scheduled 2026-10-14, at row 74 (75 since 2026-10-18), by the author's instruction**, before the
core packages. Its warrant is CLAUDE.md § Verification's *never slow the
compiler down*, a rule with no instrument that holds it over months.

**What was measured.** `heroes check selfhost/main.hero` takes 5.8 s and the
write-time hook pays it on every write of a compiler module; one module checks
in 0.18 s (`.claude/rules/verification.md`, 2026-09-29). Clang compiled the
seed in 3.4 s in August (`seed/README.md`), 17.98 s at `5876e9153` and 7.86 s
at `612e686d8` (2026-10-14, `real` beside `user`). Part 11's metric 4 counts
the turns a model takes to reach green, and every turn pays a `heroes check`:
latency is part of what the language claims, not only a comfort.
`CONTRIBUTING.md` ranks speed below the other reasons for a change.

**What it delivers.** A ceiling held like the specification's: `heroes check`
on the compiler, `heroes check` on a program that uses packages once there
are some, and clang on the seed, each timed with `/usr/bin/time -p` on a still
machine at every milestone close and moving only by a recorded decision. The
incremental frontend M-lsp-server carried since 2026-09-03 becomes this
ceiling's return condition: it is built when the ceiling would otherwise give
way, and not before. And a sentence in `CONTRIBUTING.md` that names latency
as a valid reason for a change.

**Named for the deliverable**, the ceiling. Not `M-fast-check`, a direction;
not `M-compile-speed`, the area. A row of its own for the incremental
frontend, `M-incremental-frontend`, was considered and not entered: a
frontend nobody yet needs is built when this ceiling says so.

**A comparison read on 2026-10-25, and four things it suggests for the
ceiling's instrument.** Goose's benchmark notes (github.com/aardappel/goose,
[goose/bench/notes.md](https://github.com/aardappel/goose/blob/master/bench/notes.md) § Caveats) report that "a freshly written executable costs up
to 10x its steady-state time on the first execution", that "differences under
~10% are within measurement noise" on their one machine, and that compiler
versions are compared "by building both and running them side by side", never
across dates. So, before this row's first number: (1) a timing is the best of
N stated runs after one discarded; (2) the noise floor is measured first, the
same compiler timed twice on the still machine; (3) a breach is declared only
by an interleaved comparison in one sitting against the previous tag's
compiler rebuilt, the dated history staying history; (4) two questions,
unrun: whether tcc compiles the emitted C11 (`_Generic`, `_Static_assert`,
`__builtin_*_overflow`), for a development build that skips clang's optimiser
and as a third witness of the seed's C beside clang and gcc. **And one reading
of that day**: in a fresh worktree on this MacBook Air (`Mac15,13`, eight
cores) the seed's build read `real 21.97` against `user 13.80` and `sys 0.65`,
at a load of seven with another session's work running; the ratio discards it
as a duration, so it neither confirms nor refutes the start page's *under four
seconds*, which this row re-times on a still machine.
