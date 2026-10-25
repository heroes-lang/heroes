# M-compared-languages — the same tasks, written by a model in five languages

**Scheduled 2026-10-14, at row 79 (80 since 2026-10-18), by the author's instruction**, meant as:
*to prove the claim I will have to compare: the specification of programs of
different kinds generated in Heroes, and in Go, Rust, C and Zig, counting the
tokens spent and the tests that pass, where no language wins because its
libraries are higher-level*. After the packages and the framework, before the
gate and the posts that will cite its numbers. No warrant is claimed.

**Two questions, two experiments.** Whether the design works is Part 11's
control arm, `heroes check` against `heroes check --permissive`, *the only
comparison that isolates design from training-data familiarity*
(design.md Part 11); it is M-thesis-harness's. Whether a reader should use
Heroes is this comparison, and training data confounds it: models write the
languages they have seen far better than those they have not (MultiPL-T,
https://arxiv.org/abs/2308.09895), and they have seen no Heroes. A tie is a
strong result, and a loss on first-try rates does not refute the design.
What this experiment can refute is registered before its first run: that a
Heroes program fails silently, compiling and running and failing the hidden
tests, less often than the same program in the other four.

**Measured**: the tokens spent to green, the Heroes specification counted,
with and without the prompt cache; the turns to green, capped at five; hidden
black-box tests (standard input and output, files, HTTP on loopback); the
silent failures; crashes and undefined behaviour under each toolchain's
sanitizers. A program's length is reported and never ranked, since named
arguments lengthen Heroes on purpose.

**Three families of tasks.** F1, the core alone, each language's allowed
imports listed. F2, the same C library for all (sqlite3, libcurl, zlib: Go
through cgo, Rust through `extern "C"` blocks, Zig through `@cImport`, C
directly, Heroes through `extern`), which neutralises the libraries and
measures the FFI. F3, idiomatic, each language with its standard library and
Heroes with its core packages. Mobile and the browser are out, and the
results say so: Heroes runs on neither.

**Protocol**: Part 11's, with the author's held-out tasks never committed
(issue 999); frozen prompts; Wilson intervals; one non-Anthropic model; every
toolchain pinned, Zig's version recorded beside its API changes between minor
versions; the Linux container. The cost is estimated before, and each paid run
is asked for. F1 and F2 may run early, inside M-thesis-harness. Precedents:
MultiPL-E and MultiPL-T; Aider Polyglot, 225 Exercism exercises in six
languages with hidden tests.

**Named for the deliverable**, the languages compared. Not
`M-language-shootout`, the name of a benchmark that measures speed; not
`M-polyglot-benchmark`, Aider's; not `M-benchmark`, one word and an area.

**A comparison read on 2026-10-25, and three things it suggests for the
report.** Goose publishes its losses beside its headline, every benchmark row
with its cause or "cause not re-established", and says above its aggregate how
it was composed: "It selects the fastest Goose implementation separately for
each backend and comparison tier" (github.com/aardappel/goose,
[goose/bench/summary.md](https://github.com/aardappel/goose/blob/master/bench/summary.md), [goose/bench/results.md](https://github.com/aardappel/goose/blob/master/bench/results.md)). So: (1) this row's report owes a
section of where Heroes loses, per task family, written before its headline;
(2) it states above its first table which sample, arm and turn each cell
takes; (3) the pure problems of the suites Goose adopted (PLB, plb2, kostya's
benchmarks) are shapes for F1 tasks and LangArena's four ecosystem tasks,
Base64, JSON, CSV and regular expressions, the split between F2 and F3, each
rewritten to Part 11's task form with hidden cases and never used as a venue:
every one of those suites ranks speed, a Part 2 non-goal, and sits in every
model's training data. Goose's own documents show the cost of the other way:
its compiler was tuned on its sixteen rows by construction ([goose/bench/plan.md](https://github.com/aardappel/goose/blob/master/bench/plan.md)
derives each change from a row's deficit), which is the objection the
held-out seal here answers before it is made.

**Decided by the author on 2026-10-25** (the answer *3a*): Goose joins this
comparison as a second language no measured model has read, an exploratory arm
of F1 and never the primary. Its repository was born on 2026-08-30, after the
June 2026 cutoffs design.md Part 11 records for the four models, so beside
Heroes it isolates a language designed for a model's mistakes from the
familiarity Go, Rust, C and Zig bring; its prompt is its tutorial and its
reference, 19,406 and 47,707 vendored tokens against this specification's
7,854 on the same instrument, which is design.md § 1.2's first factor made
visible. LangArena (github.com/kostya/LangArena) is not a venue before
M-core-packages: eight of its fifty tasks need Base64, JSON, CSV and regular
expressions, and it ranks speed, a Part 2 non-goal.
