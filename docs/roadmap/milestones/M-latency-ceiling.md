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

## Opened 2026-10-30

**Opened by the author's instruction of 2026-10-30**, meant as: *re-read the
latest commits, then get ready to do the step that is next*; on the trunk and
in no lane yet, the opening writing records alone, every other session of this
Mac offline. Step 0, as rows 73 and 74 opened.

**What it starts from**, each number its command's, measured at this opening
on this Mac with the machine still (load 1.81 before and 2.27 after,
Amphetamine holding it awake): `Mac15,13`, Apple silicon, four performance and four efficiency cores, macOS 27.0.1,
Apple clang 21.0.0; the compiler `0.5.0`, `sources 23825438285ee301`, the one
`m-compatibility-promise` placed today, `selfhost/`, `runtime/` and `seed/`
unchanged since by `git diff --stat`, the tree at 739781f5f, 698 modules and
181,690 lines of Heroes, the seed 276,991 lines and 28,876,131 bytes. Each
clock is `/usr/bin/time -p`, N stated, the first run discarded, `real` read
beside `user` plus `sys` on every run and a run more than 0.1 s above them
discarded as waiting (CLAUDE.md § Verification; decision 2218):

- **`heroes check selfhost/main.hero`**, seven runs: the six kept read 6.90,
  7.04, 6.99, 7.03, 7.05 and 7.02 s, every one within 0.1 s of `user` plus
  `sys`; **best 6.90 s**, spread 0.15 s. The discarded first read 6.86, the
  fastest of the seven. The history above says 5.8 s on 2026-09-29, when
  `.claude/rules/verification.md` counted 308 modules.
- **clang on the seed**, `clang -I runtime seed/heroes.c runtime/runtime.c -o`,
  seven runs: of the six kept, 8.52 s against `user` plus `sys` 8.32 was
  waiting and is discarded; the five read 8.30, 8.35, 8.29, 8.31 and 8.35 s,
  **best 8.29 s**, spread 0.06 s.
- **`heroes build selfhost/main.hero -o`** with the cache warm, four runs
  stated, the number issue 715 is about and the clocks above do not see: the
  three kept read 37.73, 37.71 and 37.79 s, **best 37.71 s**, spread 0.08 s;
  the first, 88.60 s with `user` plus `sys` 83.16, worked on a cache partly
  cold and is discarded as the first is. So the loop on the compiler's own
  source costs 37.7 s today, of which the frontend is 6.9; the other 30.8 s
  are the emission that computes its own cache key (panel 093's R4) and what
  follows it, the half issue 715 calls architectural.
- **`heroes check` on a program with packages**: no such program exists, the
  packages being row 76's; the clock stands here and takes its first number
  when one does.

**Two things the numbers say.** First, the site's start page and front page,
in both editions, promise the seed built *under four seconds* on a MacBook Air,
the machine every measurement there comes from, and `seed/README.md` restates
3.4 s undated in three places beside its dated reading of 2026-08-19 on a
22,025,792-byte seed: today's best of five is 8.29 s, more than twice the
promise, on that machine. The sentence expired in silence as the seed grew,
and is defect 2281, `blocking` for a false sentence in public, filed at this
opening and to be repaired on this row; the route its issue recommends is the
one the site's build already takes for the start page's count of verbs
(`.claude/rules/verification.md` § The site's build), the number read from the
record the instrument writes, so that it cannot expire again. Second, task
2279's 2.0 %, 6.89 s against 7.03 s in seven alternated pairs this morning
with a spread of 0.05 s inside each binary, is smaller than the spread of one
binary's six runs this afternoon, 0.15 s; so the regression is neither
confirmed nor refuted here, and the bisect the task owes runs its pairs
interleaved in one sitting, as the comparison rule above asks of a breach.

**The order of the steps**, the coordinator's, each a commit: **step 1 the
instrument**, a program of the harness beside `tests/harness/issues.hero` and
`tests/harness/thesis.hero` (`main.hero` is a program and not a verb for
CLAUDE.md § 10's reason, panel 085's R4), which runs the clocks above as this
opening ran them by hand, discards by the ratio, alternates two compilers in
one sitting, and writes the row a ledger of `docs/measurements/` keeps, a suite
of the net holding the pinned newest row to the file as `spec` holds the token
count and running no clock itself; with it the site's number, defect 2281,
read from that record; **step 2 task 2279's bisect** with the instrument;
**step 3** the ceiling's number as a decision issue with its recommendation,
the `CONTRIBUTING.md` sentence, and the close checklist's bullet in `/step`;
then the close, the ledger's row taken at it. The incremental frontend is not
built on this row: issue 715 stays its return condition, and the step that
would build it is the one at which the interleaved comparison breaches the
ceiling.
