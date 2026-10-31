# 061: the latency ledger, one row per reading of the clocks on a still machine

**A dated record, append-only** (`.claude/rules/records.md` § A record is
never rewritten): one row per reading of the latencies CLAUDE.md
§ Verification says the compiler may not lose, taken at the opening of
M-latency-ceiling (2026-10-30) and at every milestone close from its own on,
by the instrument `tests/harness/latency.hero`, whose word `record` appends
the row and prints the pins. A row is never rewritten; a reading since
falsified is answered by a later row. And a comparison across rows is a
comparison across dates, which Goose's benchmark notes, read 2026-10-25 for
the milestone's page, say is not a measurement: a breach of the ceiling is
declared by the instrument's word `compare`, two compilers alternated in one
sitting, and never by two rows of this table.

**What enforces it is not this file.** `tests/harness/suite_latency.hero`
pins the newest row's day, sources key and three bests, and the `latency`
suite of the net reads this table back on every run: a row appended and not
pinned, a pin moved and no row, or a row that reader cannot parse is red.
The suite runs no clock. The numbers are taken by `heroes run
tests/harness/latency.hero -- record <compiler>`, from the repository's root,
with the machine still and nothing else running (CLAUDE.md § Verification):
N runs per clock, the first discarded, a run whose `real` stands more than
0.1 s above `user` plus `sys` discarded as waiting, the best of what is left
the number and its spread the noise; `tests/harness/latency_clock.hero` is
the rule, with the tests that hold it.

**The columns.** `taken` the day; `tree` the commit `git rev-parse
--short=9 HEAD` names; `sources` the second line of the compiler's
`--version`, the key of what it was compiled from (panel 222); `check`,
`seed` and `build` the best `real` in seconds of `heroes check
selfhost/main.hero`, of `clang -I runtime seed/heroes.c runtime/runtime.c`
and of `heroes build selfhost/main.hero` with the cache warm, each with its
`spread` beside it, the worst kept run less the best, and `kept` the runs
kept of the runs made; `load` the one-minute load average before and after
the reading; `toolchain` clang's first line, the operating system and the
machine; `note` what the reading was taken for. The reader finds a column by
its name, so a column entering after `note` moves nothing: the one for
`heroes check` on a program that uses packages enters when such a program
exists (row 76 of the chain).

| taken | tree | sources | check | check spread | check kept | seed | seed spread | seed kept | build | build spread | build kept | load | toolchain | note |
|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|
| 2026-10-30 | ccdf13f53 | 23825438285ee301 | 6.81 | 0.14 | 4/5 | 8.18 | 0.05 | 4/5 | 37.25 | 0.22 | 4/5 | 3.13 then 3.55 | Apple clang version 21.0.0 (clang-2100.3.34.2), macOS 27.0.1, Mac15,13 | the opening, by the instrument |
