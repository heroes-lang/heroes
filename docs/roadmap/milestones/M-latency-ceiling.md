# M-latency-ceiling — a latency the compiler may not lose without a decision

**Scheduled 2026-10-14, at row 74, by the author's instruction**, before the
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
