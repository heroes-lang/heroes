# DEFECTS — the compiler defects that are still open

Every item is a **measured** failure of the compiler on a program — a crash, a
wrong answer at exit 0, a silence where a message is owed — carrying its
reproducer, its cause where known, and what is owed. **Only open defects live
here**: a repaired one is ticked, gains a *The repair* section with the
measurements that prove it, and moves to `docs/records/done/`. A repair is owed
at the class and not at the witness, with a `tests/golden/fixedbugs/` case per
shape.

**The shape** is `.claude/rules/records.md` § The lists, and § A live list is a
preamble, a count and its items is why this preamble is fifteen lines. **The
next number is READ, never remembered** — `records/numbering` takes one above
the highest issued across this file and `docs/records/done/`. Who issued which
number since 2026-09-08, and why 014 exists twice, is
`docs/records/log/2026-09-16-2200-the-defect-register-leaves-the-list.md`.

Format: `- [ ] **NNN — <title>** | <what it does, in one line> | <where to look>`

*******************************************************************************
**OPEN: 3**

- [ ] **112 — emission is quadratic in the size of a function** | `emit/unread.assigned` scans every instruction of a function for each temporary it declares, so `build --emit-c` less `check` reads about 3.5, 7.7 and 19.7 s at 250, 500 and 1000 units of a generated program whose `main` makes one call per unit; the lowering's `ir/flatten.call` and `ir/owned_release.library_validated` lead the rest | `selfhost/emit/unread.hero:84` (`assigned`)

    **Origin:** lane 105's agent, 2026-09-27 (2,196 of 6,633 samples of
    `build --emit-c` at 1000 units in `assigned`); the ladder re-run by the
    coordinator on the trunk's compiler at `2b1a1f24`, `build --emit-c` 4.01,
    9.12 and 24.02 s user at 250, 500 and 1000 units against `check` 0.56,
    1.45 and 4.31, at a load near 4.

    **Why it is a defect.** The same shape as 111 in the back end: one pass
    per temporary where one pass per function answers every temporary.

- [ ] **114 — the emitted C zeroes every temporary of a function at its entry, so a lookup that returns early pays for every arm** | the emitter declares each temporary at the top of the C function with `= {0}`, and a `match` over a string of twenty arms declares them all: the emitted `h_keywords_keyword` zeroes 139 temporaries on every call to make 21 string comparisons, and on the default build line (plain `clang`, no optimisation, CLAUDE.md § Commands) every one is executed; `memset` was about a fifth of `fmt`'s samples after defect 105's repair | `selfhost/emit/` (the temporaries' declarations) · `seed/heroes.c` (`h_keywords_keyword`)

    **Origin:** lane 105's agent, 2026-09-27 (`memset` 240 of 1,113 samples
    of `fmt` on eight copies of `walk.hero`, under the lexer's keyword and
    punctuation lookups); the emitted function read by the coordinator in
    the trunk's seed at `2b1a1f24`: 941 lines, 139 `= {0}`, 21 string
    comparisons.

    **Why it is a defect.** Every Heroes program pays it, the compiler first:
    work the program never reads, done at each call. The repair keeps the
    guarantee the zeroing buys, every temporary initialised before any read,
    and moves the initialisation to where the temporary's life begins.

- [ ] **116 — a statement continued at its own margin after a trailing operator compiles, where design.md says a long expression at depth zero is broken inside parentheses or not at all** | `y = a +` over `1` at the statement's own indentation is `check` 0 and prints 6: the lexer plants no terminator after `+` (Go's rule) and the same margin plants no indent, so the statement goes on; one level deeper it is refused; design.md §4.15 reads *at bracket depth zero every line's indentation is structural: a long expression is broken inside parentheses or not at all*, and *trailing-operator continuation at depth zero (Nim's rule) was considered and deferred* | `selfhost/layout.hero` (`is_line_ender`, `maybe_terminator`) · `docs/design/design.md` §4.15

    **Origin:** lane B's agent, 2026-09-27, beside defect 107 (a comment on
    its own line in such a continuation is refused by `fmt` at exit 2, since
    the owner rule reads the continued line as a new logical line); searched
    `docs/work/DEFECTS.md`, `docs/records/` and `docs/panel/` for *same
    indentation*, *depth zero* and *Nim's rule* and found only panel 007's
    deferral and panel 180's reports quoting it. Re-run by the coordinator on
    the trunk at `f08b192d`: `y = a +` / `1` and `y = xs.` / `len()` at the
    same margin, `check` 0 and `run` printing 6 and 2; the continuation one
    level deeper, `check` 1.

    **Why it is a defect, and why it needs a sitting.** The compiler admits a
    form the source of truth says was deferred; the spec's own last-token
    rule reads as admitting it. Whether the repair refuses it, as design.md
    says, or design.md admits it, is what the lexer does: a panel path
    (CLAUDE.md § 4).

*******************************************************************************
