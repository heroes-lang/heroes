# M-panic-location — a panic names its line


**Scheduled by author instruction 2026-09-03**, the cheapest half of what the
author called *"a strong runtime"*, and its warrant is **§1.12**: a program that
stops must say where.

**Measured 2026-09-03.** `hero_panic` flushes stdout, prints `panic: <msg>` and
calls `abort()` (`runtime/parts/panic.c:21-25`); an out-of-range index, an
overflow, a `.must()` on an error and a slice that splits a character all funnel
through it, and none names a `.hero` file, a line or a function. The one abort
that does is the stack guard: `runtime/parts/stack.c:202-213` walks back with
`dladdr` to the first Heroes frame on POSIX, and `stack.c:292` says Windows names
the failure and not the function until dbghelp and a PDB are measured on the box.
The emitted C carries `#line` (CLAUDE.md §7), so `__FILE__` and `__LINE__` at
every runtime call that can abort already resolve to the `.hero` position — the
information is in the binary and the runtime is not told.

**What it delivers.** Every panic names the `.hero` file, the line and the Heroes
function, on the three platforms, measured on each before its commit (CLAUDE.md
§ Commands); `HERO_RUNTIME_ABI` +1 with the two-phase edit (`seed/README.md`); a
`fixedbugs` case per abort class (CLAUDE.md §9). **Soundness lane**: no surface,
no diagnostic class, no spec token. What it must not do is slow a program's happy
path or the compiler — the location is passed, never computed, and the corpus
leg's time before and after is the measurement.

**Why here, and why it may move.** It touches the runtime, which
M-isolated-threads holds until it closes; nothing else depends on it, so it may
be taken the day the threads land. Its witnesses are the corpus under
`--sanitize` and Part 11's metric 4, whose turns-to-green a panic that names its
line shortens.

**Widened and corrected 2026-10-14.** The row keeps 71 and widens to what a
failure shows: an `assert` on a record shows both sides rather than the
expression alone, as spec § 12 says today, and a panic names the file, the
line and the Heroes function, from the runtime's type descriptors and the
formatter M-typed-inspection wrote. It touches spec § 12, so it is a sitting.
Facts above that have moved: `hero_panic` is at `runtime/parts/panic.c:39-43`;
M-isolated-threads closed on 2026-09-06; and the premise that `__FILE__` and
`__LINE__` already resolve to the `.hero` line at every abort is false for
`.must()` and `assert`, which `selfhost/emit/abort.hero` points at the
generated file on purpose.

**Opened 2026-10-26, at row 72, in the lane `panic-location`**, by `/step`
with no step named, the ROADMAP's next row. What the opening measured, with the
lane's stage 2 (`sources 413da33e7969760a`), so that the steps start from it:

- **Four issues open** on the GitHub milestone (`open --milestone
  M-panic-location`): task 812, the "before"; feature 1001, `main` cannot
  fail; feature 1155, the assert path; feature 2083, the widened row.
- **The "before", one program per class, built and its binary run**: an index
  past the end, `panic: array index out of range`; `i64` max plus one, `panic:
  integer overflow`; `.must()` on an error, `panic: .must() on an error: odd: not
  divisible by two`; a slice through `è`, `panic: string slice splits a
  character`; `assert a == b` on two `i64`, `assert failed: a == b` with `left:
  3` and `right: 4`; on two records, `assert failed: p == q` alone; a recursion
  whose base case is never reached, `panic: stack exhausted in deep.down`; each
  at exit 134, and no line names a `.hero` file or a line. A `main` that matches
  a `fail` and prints it exits 0. `heroes test` names a failing `assert` by its
  test's title alone (`FAIL "shifting moves x"`).
- **Where each abort is written** (`heroes build --emit-c`): the index check,
  the overflow and the division by zero are written in the program's C under
  the author's `#line`; `.must()` and `assert` under the generated file's, on
  purpose; the rest inside the runtime, which is told no position: 145 calls of
  `hero_panic*` or `hero_abort()` in 19 files of `runtime/` (`grep -cE`), and
  29 lines in 9 modules of `selfhost/emit/` that write a panic's call.
- **The author's three answers of 2026-10-26**, each the recommended one: the
  full panel, sitting 230; issue 1001 in the same sitting with a resolution of
  its own, so either can be ratified without the other; and the blind seat run
  as at most four `claude -p` sessions of at most 3 USD each.
- **Corrected beneath the paragraph of 2026-10-14**: *from the runtime's type
  descriptors* names the route two ratified sittings refused, a sixth `HeroDesc`
  member (panel 117's part 4, ratified 2026-09-07) and run-time reflection
  (panel 131's part 1, a design.md Part 6 row, ratified 2026-09-11); and
  panel 132's part 6 found § 12's *until this language renders one* naming a
  condition nothing worked toward, which is why the clause is gone. The widening
  was written by the coordinator in `44f98952b`, the chain's re-ordering, under
  the author's instruction of that day to promote *what the language lacks for
  its end goal*, and not as a ruling on how a record is shown. How a side is
  shown is the sitting's question, against those refusals and their grounds.
