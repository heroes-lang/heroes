# M-corpus-depth — the rung between a program and the compiler *(closed 2026-09-04)*


**OPEN 2026-09-03**, scheduled and then moved ahead of M-isolated-threads the same
day by author instruction (§ Who scheduled what), from a plan measured that day.
**Its first two steps are not programs**: `heroes mutate` learns to check a module
below a nested program's root from that root (it refused the whole corpus for a
day), and the net gains the gate every leg runs plus the full score on the tag
run — because a corpus milestone measured by an instrument that cannot read the
corpus would be measuring nothing. Form coverage of `examples/` reached zero unexercised at
M-corpus-coverage (journal 029); what the corpus is thin in is **shape**, and it
was measured over the 78 files: the six `extern` programs are 65–192 lines and
single-module, none with a `variant`, a nested container, a generic or an
`@`-parameter structure — FFI and "real program" are disjoint sets; generics are
declared in three programs and never used across a `use`; direct self-recursion
lives in three programs at depths bounded by tiny inputs; one program checks an
answer somebody else wrote down (`sieve/`); one writes a file (`todo/`); and
between `json/` at 671 lines and the compiler at 50,452 there is nothing, where
Nim keeps a `manyloc` corpus under its own `tests/`, Zig a `standalone` one under `test/`, and Rust `rustc-perf`'s pinned
crates (nine languages surveyed that day, twelve recurring patterns, ten of them
already here).

**Nine programs, three families.** Five with a public oracle — `nbody/`,
`spectral/`, `fannkuch/`, `binarytrees/` against the benchmarks game's published
outputs at a fixed n, `checksum/` against CRC-32's and Adler-32's catalogue check
values and RFC 4648's Base64 vectors — every number fetched from its source the day
it is written and quoted with its URL. Two large — `interpreter/`, ≥ 1,500 lines in
three directories with generics instantiated across modules and a 500-deep
expression as the stack guard's positive witness, and `query/`, a CSV query engine
over 5,000 rows it generates itself, copy-on-write measured at volume for the
first time. One FFI at program scale — `ledger/`, sqlite with ≥ 15 functions
bound, where sqlite's aggregate and Heroes' over the same rows must agree.

**Step 0 landed with the scheduling**: `heroes mutate` scored over the 35-program
corpus for the first time since 2026-08-13, the corpus leg timed alone, both in
`docs/measurements/014-mutate-over-thirty-five.md`; `examples/README.md`
rewritten to list every program; and two record faults repaired (§ Who scheduled
what names the third). The brief is the `SCHEDULED.md` item naming this
milestone; the record is journal 032 at close. Cost is projected at **+1:15** on
the corpus leg and checked at close against step 0's number; every FFI program is
measured on the Mac, the Linux image and the Windows box before its commit.

*******************************************************************************
**OPEN: 0**

*******************************************************************************
