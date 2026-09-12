- [x] Covered | M-selfhost-port / DEFECT | **FIXED 2026-08-17, found by the port's own resolver — the first program to write `xs[i].f @ v`.** Raw symptom, before any reading: two selfhost tests FAIL with `panic: entered unreachable code — this is a compiler bug, please report it`, exit 134; minimised to 10 lines (`locals[0].reads @ locals[0].reads + 1`). Cause: `emit/inst.rs` dispatches every store whose place path holds an index to `write_element`, whose trailing-Field arm answered `return None` under a comment claiming a trailing field store never reaches it — true only of an index-free path, a premise that expired silently when the dispatch was written against `any(Index)` (CLAUDE.md §11's class, in the emitter's own prose). And the first repair leaked: a plain assignment left the OLD field value alive (`1 heap blocks still live at exit` on a heap str), because `hero_array_set`'s descriptor release is bypassed when the store replaces a field rather than the element. Fix: the arm releases the old member when counted (typed decref, satellite `_release` for aggregates), then assigns; the COW spine above was already unshared on the way down. Attacked at the adjacent shapes before commit: two array levels, plain element store, map insert, sharing (`kept` sees the old value), heap-str overwrite, all ASan-clean | crates/heroes/src/emit/container.rs · tests/golden/run/fixedbugs-a-field-stored-behind-an-index.hero | the witness was one line of the resolver port, and the class was every record-in-array mutation in the self-hosted compiler

- **waM-selfhost-port closed 2026-08-17 — the fixpoint | - **walkthrough** — the fixpoint recipe run live, four commands, and the hash
  appearing twice on screen (`docs/journal/021-selfhost-port.md` § Goal)

- **thM-selfhost-port closed 2026-08-17 — the fixpoint | - **the nine defects** — five in the port, four in the bootstrap, each with its
  symptom, its cause and the six-line program that reproduces it

- **thM-selfhost-port closed 2026-08-17 — the fixpoint | - **the differential** — how 2,570 comparisons found what 27,230 unit tests and
  573 bootstrap tests did not, and the two times the instrument lied to me

- **exM-selfhost-port closed 2026-08-17 — the fixpoint | - **exit quiz** — the three LEARN items queued today (caret width, the counter
  class, the two streams)

- **paM-selfhost-fixpoint closed 2026-08-18 — the seed, and four defects the fixpoint could not see | - **panel 085** — the seed's shape (A1: raw C, committed, refreshed in the commit
  that breaks it) and the split (B4: the archive leaves this milestone). Two
  vetoes ride with it, A2 and B2, both the compiler-engineer's.

- **thM-selfhost-fixpoint closed 2026-08-18 — the seed, and four defects the fixpoint could not see | - **the archive blocker** — `selfhost/main.hero:45` read the standard library from
  `crates/` at run time through `.default("")`, so the self-hosted compiler was
  unusable outside the repository root and blamed the author's line for it. Panel
  028 R3 had ruled the library embedded; the port had reversed a ratified
  decision, invisibly, because nobody ran it anywhere else.

- **`gM-selfhost-fixpoint closed 2026-08-18 — the seed, and four defects the fixpoint could not see | - **`getenv` was one extern away** — the note said the language has no getenv, and
  what the file spelling cost was a diagnostic that lied.

- **thM-selfhost-fixpoint closed 2026-08-18 — the seed, and four defects the fixpoint could not see | - **three emitted-C divergences** — the `#line` file name (81 programs), float
  literal spelling (19), and a fixed-array read that emitted an **undeclared
  identifier** (3, at exit 2). Byte-identical emitted C went **26 → 127 of 127**.

- **a M-selfhost-fixpoint closed 2026-08-18 — the seed, and four defects the fixpoint could not see | - **a silent 0.0** — `x: f64 @ 1_0.5` printed the wrong number at exit 0 in the
  bootstrap for as long as the separator has existed.

- **a M-selfhost-fixpoint closed 2026-08-18 — the seed, and four defects the fixpoint could not see | - **a fifth class for §7** — a field declared scalar where the header has an array
  was the compiler's exit 2 and is now the author's exit 1.

- **thM-selfhost-fixpoint closed 2026-08-18 — the seed, and four defects the fixpoint could not see | - **the instrument** — `tests/differential.rs`, whose expectation is the other
  compiler, over 140 programs, and it fires.

- **thM-selfhost-fixpoint closed 2026-08-18 — the seed, and four defects the fixpoint could not see | - **the seed** — `seed/heroes.c`, one clang line, tested from `git archive HEAD`.

- **paM-selfhost-fixpoint closed 2026-08-18 — the seed, and four defects the fixpoint could not see | - **panel 065's two overdue predictions** — one confirmed, one falsified.

`/learn` offers, all optional and all the author's call:

- **a M-selfhost-fixpoint closed 2026-08-18 — the seed, and four defects the fixpoint could not see | - **a walkthrough of the seed chain**: how a 21 MB C file, one clang invocation
  and no Heroes compiler produce a compiler that writes that same file again.
  Three commands, and the middle one is the whole idea of bootstrapping.

- **thM-selfhost-fixpoint closed 2026-08-18 — the seed, and four defects the fixpoint could not see | - **the four defects, as a diagnosis drill**: each one's raw symptom is in this
  milestone's journal with its fixing commit, and none of the four is guessable
  from the symptom — the `t20` one especially, where the emitted C names a
  variable that appears nowhere else in the file.

- **goM-selfhost-fixpoint closed 2026-08-18 — the seed, and four defects the fixpoint could not see | - **golden ratification** for the milestone's adversarial cases:
  `fixedbugs/ffi-a-field-the-header-has-as-an-array.hero` and
  `run/fixedbugs-a-float-with-a-digit-separator.hero`, both still marked pending.

- **thM-selfhost-fixpoint closed 2026-08-18 — the seed, and four defects the fixpoint could not see | - **the hex float renderer, read line by line**: why `m - 1.0` is exact, why the
  subnormal branch scales by a power of two instead of dividing, and why the nan
  test has to come first (`<` on a nan aborts).
